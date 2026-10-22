/*
	balloon.c - il balloon con il nome completo delle variabili
	(ottobre 2026)

	Attivo da Option -> Set visual mode, casella "Variable balloon" (vale
	per la pagina finche' resta aperta, non si salva nel .pag). Passando
	col mouse vicino a una porta di un'icona compare, dopo un attimo, il
	nome completo della variabile e il collegamento:

		I14441SL
		<- U13I41SL

	Il nome completo e' nome della porta + tag del blocco + sigla della
	task (le due lettere dopo "r_" nel nome della directory, come in
	compilazione): I1 + 4441 + SL. Si calcola sempre dalla pagina com'e'
	adesso.

	Il collegamento viene da due fonti:
	- il filo nella pagina, quando arriva direttamente alla porta di un
	  altro blocco (nome calcolato come sopra): e' lo stato attuale;
	- proc/<pagina>_01.dat, scritto dalla compilazione dello schema: risolve
	  anche i fili che passano per diramazioni o icone di interfaccia, ma
	  e' fermo all'ultima compilazione. Per questo, quando il .pag e' piu'
	  recente del _01.dat, il balloon lo dice, e quando le due fonti non
	  coincidono le mostra entrambe.
	Righe del _01.dat usate:
		UA4441SL  --UA-- descrizione          uscita del blocco
		U13I41SL  --IN-- #I14441SL#...         ingresso alimentato da U13I41SL
		I14441SL  --CO-- descrizione          ingresso collegato
		IP4N41SL  --IN-- descrizione          ingresso non collegato (un
		                                       dato del modello)
	Per le icone di interfaccia l'altro capo viene da Connessioni.reg.

	La zona sensibile e' allargata: le porte sono di pochi pixel, e il
	balloon risponde alla porta piu' vicina entro RAGGIO pixel. Per questo
	il movimento del mouse si segue sulla drawing area (gli eventi di moto
	sopra le icone risalgono a lei) invece che sulle singole porte.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include "config.h"
#include <Xl/Xl.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Ol/OlConn.h>
#include "libutilx.h"

#define RAGGIO        8      /* pixel attorno al centro della porta */
#define RITARDO_MS  350      /* attesa prima di mostrare il balloon */
#define MAX_TESTO  2048

extern PAGINA *GetPaginaFromWidget();
extern char *OlEstrRegTaskName(char *, char *);
extern int OlFindConnectionByPort(OlConnObject, char *, char *, char *);

typedef struct balloon_pag {
	Widget drawing;
	Widget shell, label;
	Widget porta;              /* porta sotto il mouse, o NULL */
	XtIntervalId timer;
	int x_root, y_root;
	struct balloon_pag *succ;
} BALLOON;

static BALLOON *balloon_lista = NULL;

static BALLOON *balloon_cerca(Widget drawing)
{
	BALLOON *b;

	for(b = balloon_lista; b != NULL; b = b->succ)
		if(b->drawing == drawing)
			return(b);
	return(NULL);
}

static void balloon_nascondi(BALLOON *b)
{
	if(b->timer)
		{
		XtRemoveTimeOut(b->timer);
		b->timer = 0;
		}
	if(b->shell != NULL)
		XtPopdown(b->shell);
}

/* ------------------------------------------------------------------ */
/* il nome completo                                                    */

static char *sigla_task(void)
{
	static char sigla[8];
	static int fatta = 0;
	char buf[FILENAME_MAX], *s;
	int i;

	if(!fatta)
		{
		fatta = 1;
		sigla[0] = 0;
		s = OlEstrRegTaskName(XlGetenv(ENVPAG), buf);
		if(s != NULL)
			{
			strncpy(sigla, s, sizeof(sigla) - 1);
			sigla[sizeof(sigla) - 1] = 0;
			free(s);
			for(i = 0; sigla[i]; i++)
				sigla[i] = toupper((unsigned char)sigla[i]);
			}
		}
	return(sigla);
}

/* nome completo della variabile di una porta di un blocco; False se la
   porta non e' di un blocco con tag */
static Boolean nome_completo(Widget porta, char *nome)
{
	Widget icona = XtParent(porta);
	char *var = NULL, *tag = NULL;

	nome[0] = 0;
	if(!XlIsIconReg(icona) || XlIsInterfaceIconReg(icona))
		return(False);
	XtVaGetValues(porta, XlNportName, &var, NULL);
	XtVaGetValues(icona, XlNtagName, &tag, NULL);
	if(var == NULL || tag == NULL || var[0] == 0 || tag[0] == 0)
		return(False);
	sprintf(nome, "%s%s%s", var, tag, sigla_task());
	return(True);
}

static Boolean porta_ingresso(Widget porta)
{
	return(XlPortIsInput(porta));
}

/* ------------------------------------------------------------------ */
/* il _01.dat                                                          */

static void path_01(char *nomepag, char *path)
{
	char minus[MAX_LUN_NOME_PAG + 1];
	int i;

	for(i = 0; nomepag[i] && i < MAX_LUN_NOME_PAG; i++)
		minus[i] = tolower((unsigned char)nomepag[i]);
	minus[i] = 0;
	sprintf(path, "%s/proc/%s_01.dat", XlGetenv(ENVPAG), minus);
}

/* parole 1 e 2 di una riga del _01.dat, e il resto dopo la seconda */
static int parole(char *riga, char *p1, char *p2, char **resto)
{
	char *s = riga;
	int n;

	p1[0] = p2[0] = 0;
	*resto = "";
	if(sscanf(s, "%15s %15s%n", p1, p2, &n) < 2)
		return(0);
	s += n;
	while(*s == ' ' || *s == '\t')
		s++;
	*resto = s;
	s[strcspn(s, "\r\n")] = 0;
	/* via gli spazi in fondo */
	n = strlen(s);
	while(n > 0 && s[n - 1] == ' ')
		s[--n] = 0;
	return(2);
}

/* nome dell'ingresso in "#I14441SL#....<===3I41" */
static Boolean ingresso_in(char *resto, char *ingresso)
{
	char *f;

	if(resto[0] != '#')
		return(False);
	f = strchr(resto + 1, '#');
	if(f == NULL || f - resto - 1 > 15)
		return(False);
	strncpy(ingresso, resto + 1, f - resto - 1);
	ingresso[f - resto - 1] = 0;
	return(True);
}

/*
	Cerca nel _01.dat della pagina la variabile nome:
	- ingresso: da chi e' alimentato (da) e descrizione; libero = 1 se la
	  compilazione lo ha lasciato non collegato;
	- uscita: descrizione.
	Torna 0 se il file non c'e', 1 se c'e' ma la variabile no, 2 trovata.
*/
static int cerca_01(char *nomepag, char *nome, char *da, int *libero,
                    char *descr)
{
	char path[FILENAME_MAX], riga[512], p1[16], p2[16], ing[16];
	char *resto;
	FILE *fp;
	int trovata = 1;

	da[0] = descr[0] = 0;
	*libero = 0;
	path_01(nomepag, path);
	if((fp = fopen(path, "r")) == NULL)
		return(0);
	while(fgets(riga, sizeof(riga), fp) != NULL)
		{
		if(parole(riga, p1, p2, &resto) < 2)
			continue;
		if(strcmp(p2, "--IN--") == 0 && ingresso_in(resto, ing) &&
		   strcmp(ing, nome) == 0)
			{
			strcpy(da, p1);
			trovata = 2;
			}
		else if(strcmp(p1, nome) == 0 &&
		        (strcmp(p2, "--CO--") == 0 || strcmp(p2, "--UA--") == 0 ||
		         (strcmp(p2, "--IN--") == 0 && resto[0] != '#')))
			{
			strncpy(descr, resto, 80);
			descr[80] = 0;
			if(strcmp(p2, "--IN--") == 0)
				*libero = 1;
			trovata = 2;
			}
		}
	fclose(fp);
	return(trovata);
}

/*
	Gli ingressi alimentati dall'uscita nome, in tutti i _01.dat della
	task: "I14M41SL" se nella stessa pagina, "RSL3:I1xxxxSL" se in
	un'altra. Aggiunge a lista (separati da ", ").
*/
static int consumatori_01(char *nomepag, char *nome, char *lista, int max)
{
	char dir[FILENAME_MAX], path[FILENAME_MAX], riga[512];
	char p1[16], p2[16], ing[16], pag[64], *resto, *s;
	char mia[MAX_LUN_NOME_PAG + 1];
	struct dirent *d;
	DIR *dp;
	FILE *fp;
	int n = 0, i;

	for(i = 0; nomepag[i] && i < MAX_LUN_NOME_PAG; i++)
		mia[i] = tolower((unsigned char)nomepag[i]);
	mia[i] = 0;
	sprintf(dir, "%s/proc", XlGetenv(ENVPAG));
	if((dp = opendir(dir)) == NULL)
		return(0);
	while((d = readdir(dp)) != NULL)
		{
		s = strstr(d->d_name, "_01.dat");
		if(s == NULL || s[7] != 0 || s - d->d_name >= (int)sizeof(pag))
			continue;
		strncpy(pag, d->d_name, s - d->d_name);
		pag[s - d->d_name] = 0;
		sprintf(path, "%s/%s", dir, d->d_name);
		if((fp = fopen(path, "r")) == NULL)
			continue;
		while(fgets(riga, sizeof(riga), fp) != NULL)
			{
			if(parole(riga, p1, p2, &resto) < 2)
				continue;
			if(strcmp(p2, "--IN--") != 0 || strcmp(p1, nome) != 0 ||
			   !ingresso_in(resto, ing))
				continue;
			if((int)(strlen(lista) + strlen(pag) + strlen(ing) + 4) >= max)
				continue;
			if(n > 0)
				strcat(lista, (n % 3) ? ", " : ",\n   ");
			if(strcmp(pag, mia) != 0)
				{
				for(i = 0; pag[i]; i++)
					pag[i] = toupper((unsigned char)pag[i]);
				strcat(lista, pag);
				strcat(lista, ":");
				}
			strcat(lista, ing);
			n++;
			}
		fclose(fp);
		}
	closedir(dp);
	return(n);
}

/* il .pag e' stato salvato dopo l'ultima compilazione? */
static Boolean compilazione_vecchia(char *nomepag)
{
	char path[FILENAME_MAX];
	struct stat spag, s01;

	path_01(nomepag, path);
	if(stat(path, &s01) != 0)
		return(False);
	sprintf(path, "%s/%s.pag", XlGetenv(ENVPAG), nomepag);
	if(stat(path, &spag) != 0)
		return(False);
	return(spag.st_mtime > s01.st_mtime);
}

/* ------------------------------------------------------------------ */
/* il testo                                                            */

/*
	L'altro capo, in un'altra pagina, di un'icona di interfaccia, da
	Connessioni.reg: qui = la variabile da questa parte, la = "PAGINA:VAR"
	dall'altra; uscita = True se da questa parte c'e' l'uscita.
	Torna 0 se il file non si legge, 1 se l'icona non e' collegata, 2 se
	trovata.
*/
static int altro_capo(PAGINA *pag, Widget icona, char *qui, char *la,
                      Boolean *uscita)
{
	char file_conn[FILENAME_MAX];
	WidgetList figli;
	Cardinal nfigli;
	OlConnObject conn;
	CONNESSIONE *c;
	int j, ret = -1;

	qui[0] = la[0] = 0;
	*uscita = False;
	sprintf(file_conn, "%s/Connessioni.reg", XlGetenv(ENVPAG));
	if((conn = OlCreateConn(file_conn)) == NULL)
		return(0);
	XtVaGetValues(icona, XmNchildren, &figli, XmNnumChildren, &nfigli, NULL);
	for(j = 0; j < (int)nfigli && ret < 0; j++)
		if(XlIsInterfacePort(figli[j]))
			ret = OlFindConnectionByPort(conn, XtName(figli[j]),
			        XtName(icona), pag->nomepag);
	if(ret >= 0)
		{
		c = OlGetConn(conn, ret);
		*uscita = (strcmp(c->u_nome_pagina, pag->nomepag) == 0);
		if(*uscita)
			{
			sprintf(qui, "%s%s%s", c->u_nome_var, c->u_nome_tag,
			        sigla_task());
			sprintf(la, "%.60s:%s%s%s", c->i_nome_pagina,
			        c->i_nome_var, c->i_nome_tag, sigla_task());
			}
		else
			{
			sprintf(qui, "%s%s%s", c->i_nome_var, c->i_nome_tag,
			        sigla_task());
			sprintf(la, "%.60s:%s%s%s", c->u_nome_pagina,
			        c->u_nome_var, c->u_nome_tag, sigla_task());
			}
		}
	OlDestroyConn(conn);
	return(ret >= 0 ? 2 : 1);
}

/*
	L'altro capo del filo della porta, com'e' adesso nella pagina: il nome
	completo se e' la porta di un blocco, "PAGINA:VAR" se e' un'icona di
	interfaccia collegata a un'altra pagina. False se il filo non c'e' o
	arriva altrove (per esempio a un nodo di diramazione).
*/
static Boolean collegata_dal_vivo(BALLOON *b, Widget porta, char *nome)
{
	char *conn = NULL, cerca[64], qui[32];
	Boolean uscita;
	Widget altra;
	PAGINA *pag;

	nome[0] = 0;
	XtVaGetValues(porta, XlNportNameConnect, &conn, NULL);
	if(conn == NULL || conn[0] == 0 || strchr(conn, ' ') != NULL)
		return(False);
	sprintf(cerca, "*%.60s", conn);
	altra = XtNameToWidget(b->drawing, cerca);
	if(altra == NULL || !XlIsPort(altra))
		return(False);
	if(XlIsInterfaceIconReg(XtParent(altra)))
		{
		pag = GetPaginaFromWidget(b->drawing);
		return(pag != NULL &&
		       altro_capo(pag, XtParent(altra), qui, nome, &uscita) == 2);
		}
	return(nome_completo(altra, nome));
}

static void testo_interfaccia(PAGINA *pag, Widget porta, char *t)
{
	char qui[32], la[96];
	Boolean uscita;

	sprintf(t, "Interface icon");
	switch(altro_capo(pag, XtParent(porta), qui, la, &uscita))
		{
		case 0:
			strcat(t, "\nConnessioni.reg not readable");
			break;
		case 1:
			strcat(t, "\nnot connected to another page");
			break;
		default:
			sprintf(t + strlen(t), "\n%s  %s  %s", qui,
			        uscita ? "->" : "<-", la);
		}
}

static void testo_porta(BALLOON *b, Widget porta, char *t)
{
	PAGINA *pag = GetPaginaFromWidget(b->drawing);
	char nome[32], vivo[96], comp[32], descr[96], lista[1024];
	char *var = NULL;
	int libero, esito;

	t[0] = 0;
	if(pag == NULL)
		return;
	if(XlIsInterfaceIconReg(XtParent(porta)))
		{
		testo_interfaccia(pag, porta, t);
		return;
		}
	if(!nome_completo(porta, nome))
		{
		XtVaGetValues(porta, XlNportName, &var, NULL);
		sprintf(t, "port %s (no block tag)", var ? var : "?");
		return;
		}
	collegata_dal_vivo(b, porta, vivo);
	esito = cerca_01(pag->nomepag, nome, comp, &libero, descr);
	strcpy(t, nome);
	if(descr[0])
		sprintf(t + strlen(t), "   %s", descr);

	if(porta_ingresso(porta))
		{
		if(vivo[0] && comp[0] && strcmp(vivo, comp) != 0)
			sprintf(t + strlen(t), "\n<- %s   (last compile: %s)",
			        vivo, comp);
		else if(vivo[0])
			sprintf(t + strlen(t), "\n<- %s", vivo);
		else if(comp[0])
			sprintf(t + strlen(t), "\n<- %s", comp);
		else
			strcat(t, "\nnot connected (model input)");
		}
	else
		{
		lista[0] = 0;
		consumatori_01(pag->nomepag, nome, lista, sizeof(lista));
		if(lista[0])
			sprintf(t + strlen(t), "\n-> %s", lista);
		if(vivo[0] && strstr(lista, vivo) == NULL)
			sprintf(t + strlen(t), "\n-> %s%s", vivo,
			        lista[0] ? "   (not in last compile)" : "");
		if(!lista[0] && !vivo[0])
			strcat(t, "\nnot connected");
		}

	if(esito == 0)
		strcat(t, "\n(scheme never compiled: connections from the page only)");
	else if(esito == 1)
		strcat(t, "\n(not in the last compile)");
	else if(compilazione_vecchia(pag->nomepag))
		strcat(t, "\n(page saved after the last compile)");
}

/* ------------------------------------------------------------------ */
/* il balloon                                                          */

static Boolean porta_ancora_qui(BALLOON *b, Widget porta)
{
	WidgetList icone, figli;
	Cardinal nicone, nfigli;
	int i, j;

	XtVaGetValues(b->drawing, XmNchildren, &icone,
	              XmNnumChildren, &nicone, NULL);
	for(i = 0; i < (int)nicone; i++)
		{
		if(!XtIsComposite(icone[i]))
			continue;
		XtVaGetValues(icone[i], XmNchildren, &figli,
		              XmNnumChildren, &nfigli, NULL);
		for(j = 0; j < (int)nfigli; j++)
			if(figli[j] == porta)
				return(True);
		}
	return(False);
}

static void balloon_mostra(XtPointer client_data, XtIntervalId *id)
{
	BALLOON *b = (BALLOON *) client_data;
	char testo[MAX_TESTO];
	XmString xs;
	Dimension w, h;
	int x, y, sw, sh;
	Screen *scr;

	b->timer = 0;
	if(b->porta == NULL || !porta_ancora_qui(b, b->porta))
		{
		b->porta = NULL;
		return;
		}
	testo_porta(b, b->porta, testo);
	if(testo[0] == 0)
		return;
	if(b->shell == NULL)
		{
		b->shell = XtVaCreatePopupShell("variableBalloon",
		        overrideShellWidgetClass, b->drawing,
		        XmNborderWidth, 1, NULL);
		b->label = XtVaCreateManagedWidget("variableBalloonLabel",
		        xmLabelWidgetClass, b->shell,
		        XmNalignment, XmALIGNMENT_BEGINNING,
		        XmNmarginWidth, 6, XmNmarginHeight, 4,
		        XtVaTypedArg, XmNbackground, XmRString,
		            "LightYellow", strlen("LightYellow") + 1,
		        XtVaTypedArg, XmNforeground, XmRString,
		            "Black", strlen("Black") + 1,
		        NULL);
		}
	xs = XmStringCreateLtoR(testo, XmFONTLIST_DEFAULT_TAG);
	XtVaSetValues(b->label, XmNlabelString, xs, NULL);
	XmStringFree(xs);
	XtRealizeWidget(b->shell);
	XtVaGetValues(b->label, XmNwidth, &w, XmNheight, &h, NULL);
	scr = XtScreen(b->drawing);
	sw = WidthOfScreen(scr);
	sh = HeightOfScreen(scr);
	x = b->x_root + 14;
	y = b->y_root + 18;
	if(x + (int)w + 2 > sw) x = b->x_root - (int)w - 14;
	if(y + (int)h + 2 > sh) y = b->y_root - (int)h - 10;
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	XtVaSetValues(b->shell, XmNx, x, XmNy, y,
	              XmNwidth, w, XmNheight, h, NULL);
	XtPopup(b->shell, XtGrabNone);
}

/* la porta piu' vicina al punto (x,y) della drawing area, entro RAGGIO */
static Widget porta_vicina(Widget drawing, int x, int y)
{
	WidgetList icone, figli;
	Cardinal nicone, nfigli;
	Position ix, iy, px, py;
	Dimension pw, ph;
	Widget migliore = NULL;
	int i, j, dx, dy, d, dmin = RAGGIO * RAGGIO + 1;

	XtVaGetValues(drawing, XmNchildren, &icone,
	              XmNnumChildren, &nicone, NULL);
	for(i = 0; i < (int)nicone; i++)
		{
		if(!XtIsManaged(icone[i]) || !XtIsComposite(icone[i]))
			continue;
		XtVaGetValues(icone[i], XmNx, &ix, XmNy, &iy, NULL);
		XtVaGetValues(icone[i], XmNchildren, &figli,
		              XmNnumChildren, &nfigli, NULL);
		for(j = 0; j < (int)nfigli; j++)
			{
			if(!XlIsPort(figli[j]) || !XtIsManaged(figli[j]))
				continue;
			XtVaGetValues(figli[j], XmNx, &px, XmNy, &py,
			              XmNwidth, &pw, XmNheight, &ph, NULL);
			dx = x - (ix + px + pw / 2);
			dy = y - (iy + py + ph / 2);
			d = dx * dx + dy * dy;
			if(d < dmin)
				{
				dmin = d;
				migliore = figli[j];
				}
			}
		}
	return(migliore);
}

static void balloon_evento(Widget w, XtPointer client_data, XEvent *ev,
                           Boolean *cont)
{
	BALLOON *b = (BALLOON *) client_data;
	Widget porta;

	if(ev->type == LeaveNotify)
		{
		/* uscire verso un'icona (una finestra figlia) non e' uscire */
		if(ev->xcrossing.detail == NotifyInferior)
			return;
		b->porta = NULL;
		balloon_nascondi(b);
		return;
		}
	if(ev->type != MotionNotify || ev->xmotion.window != XtWindow(w))
		return;
	b->x_root = ev->xmotion.x_root;
	b->y_root = ev->xmotion.y_root;
	porta = porta_vicina(w, ev->xmotion.x, ev->xmotion.y);
	if(porta == b->porta)
		return;
	balloon_nascondi(b);
	b->porta = porta;
	if(porta != NULL)
		b->timer = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
		        RITARDO_MS, balloon_mostra, (XtPointer) b);
}

static void balloon_libera(BALLOON *b)
{
	BALLOON **pp;

	if(b->timer)
		XtRemoveTimeOut(b->timer);
	for(pp = &balloon_lista; *pp != NULL; pp = &(*pp)->succ)
		if(*pp == b)
			{
			*pp = b->succ;
			break;
			}
	XtFree((char *) b);
}

/* la pagina si chiude: lo shell del balloon (figlio della drawing area)
   se ne va con lei, qui si toglie il resto */
static void balloon_distrutto(Widget w, XtPointer client_data, XtPointer cb)
{
	balloon_libera((BALLOON *) client_data);
}

Boolean PagBalloonGet(Widget drawing)
{
	return(balloon_cerca(drawing) != NULL);
}

void PagBalloonSet(Widget drawing, Boolean attivo)
{
	BALLOON *b = balloon_cerca(drawing);

	if(attivo && b == NULL)
		{
		b = (BALLOON *) XtCalloc(1, sizeof(BALLOON));
		b->drawing = drawing;
		b->succ = balloon_lista;
		balloon_lista = b;
		XtAddEventHandler(drawing, PointerMotionMask | LeaveWindowMask,
		        False, balloon_evento, (XtPointer) b);
		XtAddCallback(drawing, XmNdestroyCallback,
		        balloon_distrutto, (XtPointer) b);
		}
	else if(!attivo && b != NULL)
		{
		XtRemoveEventHandler(drawing, PointerMotionMask | LeaveWindowMask,
		        False, balloon_evento, (XtPointer) b);
		XtRemoveCallback(drawing, XmNdestroyCallback,
		        balloon_distrutto, (XtPointer) b);
		if(b->shell != NULL)
			XtDestroyWidget(b->shell);
		balloon_libera(b);
		}
}
