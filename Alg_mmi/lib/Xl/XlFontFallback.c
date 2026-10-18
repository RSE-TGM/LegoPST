/*
   XlFontFallback.c

   Caricamento font core X11 con fallback garantito a un font 8-bit.

   Problema: su server X che hanno i font Adobe solo in codifica
   iso10646-1 (es. WSLg con Fedora: xorg-x11-fonts-{75,100}dpi senza i
   pacchetti ISO8859-1), un nome wildcard come
   "-adobe-helvetica-bold-r-normal--14-*" aggancia la versione iso10646-1.
   Motif con un font 2-byte disegna a 16 bit -> testo a riquadri.
   Un nome esplicito -iso8859-1 invece non viene trovato e Motif ripiega
   su "fixed" generico.

   XlLoadFontFallback() rifiuta i font 2-byte e prova in cascata:
     1. il nome richiesto
     2. stessa famiglia/peso/size forzando iso8859-1
     3. misc-fixed iso8859-1 di peso e pixel size piu' vicini
     4. "fixed" (built-in in ogni server X)

   XlInstallFontFallback() sostituisce i convertitori Xt String->FontList
   di Motif, cosi' il fallback vale sia per le risorse (*fontList nei file
   risorse) sia per le conversioni da codice (UxConvertFontList).
   Va chiamata dopo XtAppInitialize e prima di creare i widget.

   Con LEGO_FONT_DEBUG definita stampa su stderr le sostituzioni.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <XlFontFallback.h>

#define XLFD_FIELDS 14
#define XLFD_FIELD_LEN 64

/* indici dei campi XLFD (dopo il '-' iniziale) */
#define F_WEIGHT   2
#define F_PIXEL    6
#define F_POINT    7
#define F_REGISTRY 12
#define F_ENCODING 13

#define DEFAULT_PIXEL 13

static int font_debug = -1;

static void debug_msg(const char *req, const char *got)
{
	if (font_debug < 0)
		font_debug = (getenv("LEGO_FONT_DEBUG") != NULL);
	if (font_debug)
		fprintf(stderr, "XlFontFallback: \"%s\" -> \"%s\"\n", req, got);
}

static int font_is_8bit(XFontStruct *fs)
{
	return (fs->min_byte1 == 0 && fs->max_byte1 == 0);
}

/* carica il font solo se e' a 8 bit, altrimenti NULL */
static XFontStruct *load_8bit(Display *disp, const char *name)
{
	XFontStruct *fs;

	if (name == NULL || *name == 0)
		return NULL;
	if ((fs = XLoadQueryFont(disp, name)) == NULL)
		return NULL;
	if (!font_is_8bit(fs)) {
		XFreeFont(disp, fs);
		return NULL;
	}
	return fs;
}

/*
   Divide un nome XLFD nei 14 campi. In un pattern con meno campi
   (es. "-adobe-helvetica-bold-r-normal--14-*-iso8859-1") l'ultimo '*'
   copre i campi mancanti, come fa il server X.
   Ritorna 0 se il nome non e' un XLFD.
*/
static int split_xlfd(const char *name, char f[XLFD_FIELDS][XLFD_FIELD_LEN])
{
	const char *p, *q;
	int n = 0, len, i, w, missing;

	if (name == NULL || name[0] != '-')
		return 0;
	p = name + 1;
	while (n < XLFD_FIELDS) {
		q = strchr(p, '-');
		len = q ? (int)(q - p) : (int)strlen(p);
		if (len >= XLFD_FIELD_LEN)
			return 0;
		memcpy(f[n], p, len);
		f[n][len] = 0;
		n++;
		if (q == NULL)
			break;
		p = q + 1;
	}
	if (n < XLFD_FIELDS) {
		/* l'ultimo campo '*' copre i campi mancanti */
		for (w = n - 1; w >= 0 && strcmp(f[w], "*") != 0; w--)
			;
		if (w < 0)
			return 0;
		missing = XLFD_FIELDS - n;
		for (i = n - 1; i > w; i--)
			strcpy(f[i + missing], f[i]);
		for (i = w + 1; i <= w + missing; i++)
			strcpy(f[i], "*");
	}
	return 1;
}

static void join_xlfd(char f[XLFD_FIELDS][XLFD_FIELD_LEN], char *out, int outlen)
{
	int i, len = 0;

	out[0] = 0;
	for (i = 0; i < XLFD_FIELDS && len < outlen; i++)
		len += snprintf(out + len, outlen - len, "-%s", f[i]);
}

static int is_number(const char *s)
{
	if (*s == 0)
		return 0;
	for (; *s; s++)
		if (!isdigit((unsigned char)*s))
			return 0;
	return 1;
}

/* pixel size richiesta; dal point size (decimi di punto, 75 dpi) se manca */
static int wanted_pixel(char f[XLFD_FIELDS][XLFD_FIELD_LEN])
{
	if (is_number(f[F_PIXEL]) && atoi(f[F_PIXEL]) > 0)
		return atoi(f[F_PIXEL]);
	if (is_number(f[F_POINT]) && atoi(f[F_POINT]) > 0)
		return (atoi(f[F_POINT]) * 75 + 360) / 720;
	return DEFAULT_PIXEL;
}

/*
   misc-fixed iso8859-1 del peso indicato con pixel size piu' vicina
   (a parita' di distanza la piu' piccola). Esclude le istanze
   scalabili (size 0), che darebbero bitmap riscalate.
*/
static XFontStruct *load_misc_fixed(Display *disp, const char *weight,
                                    int pixel, char *used, int usedlen)
{
	char pattern[128], f[XLFD_FIELDS][XLFD_FIELD_LEN];
	char **names;
	int count, i, best = -1, best_d = 0, best_px = 0, d, px;
	XFontStruct *fs = NULL;

	snprintf(pattern, sizeof(pattern),
	         "-misc-fixed-%s-r-normal--*-*-*-*-c-*-iso8859-1", weight);
	names = XListFonts(disp, pattern, 500, &count);
	if (names == NULL)
		return NULL;
	for (i = 0; i < count; i++) {
		if (!split_xlfd(names[i], f) || !is_number(f[F_PIXEL]))
			continue;
		px = atoi(f[F_PIXEL]);
		if (px <= 0)
			continue;
		d = abs(px - pixel);
		if (best < 0 || d < best_d || (d == best_d && px < best_px)) {
			best = i;
			best_d = d;
			best_px = px;
		}
	}
	if (best >= 0 && (fs = load_8bit(disp, names[best])) != NULL)
		snprintf(used, usedlen, "%s", names[best]);
	XFreeFontNames(names);
	return fs;
}

XFontStruct *XlLoadFontFallback(Display *disp, const char *fontname)
{
	char f[XLFD_FIELDS][XLFD_FIELD_LEN];
	char name[512], used[512];
	const char *weight;
	XFontStruct *fs;

	if ((fs = load_8bit(disp, fontname)) != NULL)
		return fs;

	used[0] = 0;
	if (split_xlfd(fontname, f)) {
		/* 2. stesso font in iso8859-1 */
		strcpy(f[F_REGISTRY], "iso8859");
		strcpy(f[F_ENCODING], "1");
		join_xlfd(f, name, sizeof(name));
		if ((fs = load_8bit(disp, name)) != NULL) {
			debug_msg(fontname, name);
			return fs;
		}

		/* 3. misc-fixed di peso e size simili */
		weight = (strstr(f[F_WEIGHT], "bold") || strstr(f[F_WEIGHT], "demi"))
		         ? "bold" : "medium";
		fs = load_misc_fixed(disp, weight, wanted_pixel(f), used, sizeof(used));
		if (fs == NULL && strcmp(weight, "bold") == 0)
			fs = load_misc_fixed(disp, "medium", wanted_pixel(f),
			                     used, sizeof(used));
		if (fs != NULL) {
			debug_msg(fontname, used);
			return fs;
		}
	}

	/* 4. fixed: esiste sempre */
	if ((fs = load_8bit(disp, "fixed")) == NULL)
		fs = XLoadQueryFont(disp, "fixed");
	debug_msg(fontname ? fontname : "(null)", "fixed");
	return fs;
}

/* ---------------------------------------------------------------------
   Convertitore Xt String -> XmFontList con fallback.
   Sintassi Motif: elementi separati da ',', ognuno "font[=tag]" oppure
   un font set "font1;font2:[tag]" (caricato senza fallback).
   --------------------------------------------------------------------- */

static char *trim(char *s)
{
	char *e;

	while (isspace((unsigned char)*s))
		s++;
	e = s + strlen(s);
	while (e > s && isspace((unsigned char)e[-1]))
		*--e = 0;
	return s;
}

static Boolean CvtStringToFontListFallback(Display *disp, XrmValuePtr args,
                                           Cardinal *num_args, XrmValuePtr from,
                                           XrmValuePtr to, XtPointer *data)
{
	static XmFontList static_list;
	XmFontList list = NULL;
	XmFontListEntry entry;
	XFontStruct *fs;
	char *buf, *item, *next, *tag, *sep;

	if (from->addr == NULL)
		return False;
	buf = XtNewString((char *)from->addr);

	for (item = buf; item != NULL; item = next) {
		if ((next = strchr(item, ',')) != NULL)
			*next++ = 0;
		item = trim(item);
		if (*item == 0)
			continue;

		entry = NULL;
		if ((sep = strchr(item, ':')) != NULL) {
			/* font set */
			*sep = 0;
			tag = trim(sep + 1);
			entry = XmFontListEntryLoad(disp, trim(item), XmFONT_IS_FONTSET,
			                            *tag ? tag : XmFONTLIST_DEFAULT_TAG);
		} else {
			tag = XmFONTLIST_DEFAULT_TAG;
			if ((sep = strchr(item, '=')) != NULL) {
				*sep = 0;
				tag = trim(sep + 1);
				item = trim(item);
			}
			if ((fs = XlLoadFontFallback(disp, item)) != NULL)
				entry = XmFontListEntryCreate(tag, XmFONT_IS_FONT, fs);
		}
		if (entry != NULL) {
			list = XmFontListAppendEntry(list, entry);
			XmFontListEntryFree(&entry);
		}
	}
	XtFree(buf);

	if (list == NULL) {
		XtDisplayStringConversionWarning(disp, (char *)from->addr, XmRFontList);
		return False;
	}
	if (to->addr != NULL) {
		if (to->size < sizeof(XmFontList)) {
			XmFontListFree(list);
			to->size = sizeof(XmFontList);
			return False;
		}
		*(XmFontList *)to->addr = list;
	} else {
		static_list = list;
		to->addr = (XPointer)&static_list;
	}
	to->size = sizeof(XmFontList);
	return True;
}

static void FontListDestructor(XtAppContext app, XrmValuePtr to,
                               XtPointer data, XrmValuePtr args,
                               Cardinal *num_args)
{
	XmFontListFree(*(XmFontList *)to->addr);
}

void XlInstallFontFallback(void)
{
	static const char *types[] = {
		XmRFontList, XmRLabelFontList, XmRButtonFontList, XmRTextFontList
	};
	int i;

	for (i = 0; i < (int)XtNumber(types); i++)
		XtSetTypeConverter(XmRString, types[i], CvtStringToFontListFallback,
		                   NULL, 0, XtCacheByDisplay, FontListDestructor);
}
