/**********************************************************************
*
*       C Source:               XlBitmap.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:30:03 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlBitmap.c-5.1.3 %  (%full_filespec: XlBitmap.c-5.1.3:csrc:1 %)";
#endif
/*
   modulo XlBitmap.c
   tipo 
   release 5.2
   data 2/16/96
   reserved @(#)XlBitmap.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlBitmap.c	5.2\t2/16/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlBitmap.c - widget bitmap per stazioni
 */
#include <Xm/XmP.h>
#ifndef VMS
#include <X11/Xmu/Xmu.h>
#endif
#include <X11/StringDefs.h>

#include <stdio.h>
#include <string.h>
#include <Xl/XlP.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlBitmapP.h>
#include <Xl/nullo.bmp>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 10
#define DEFAULTHEIGHT 10





/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNcomponent,
        XlCComponent,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlBitmapRec,bitmap.component),
        XmRImmediate,
        ""
        },
        {
        XlNsubsystem,
        XlCSubsystem,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlBitmapRec,bitmap.subsystem),
        XmRImmediate,
        ""
        },
        {
        XlNcolorNorm,
        XlCColorNorm,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.color_norm),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNnomeFile,
        XlCNomeFile,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlBitmapRec,bitmap.nome_file_bitmap),
        XmRImmediate,
        "nullo.bmp"
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.automatico),
        XmRString,
        XlDefaultColoreAutomatico1
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.stimato),
        XmRString,
        XlDefaultColoreStimato1
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione1
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo1
        },
#endif
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo),
        XmRString,
        XlDefaultColoreBassissimo1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso1
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto1
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo1
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo1
        },
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.altissimo),
        XmRString,
        XlDefaultColoreAltissimo1
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
    	{
        XlNtrasparent,
        XlCTrasparent,
        XlDtrasparent,
        XlROption,
        NULL,
        XlOtrasparent,
        XlRGrIo,
        XtOffsetOf(XlBitmapRec,xlcore.trasparent),
        sizeof(int),
        },
	{
        XlNcomponent,
        XlCComponent,
        XlDcomponent,
        XlRComponent,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlBitmapRec,bitmap.component),
        sizeof(char*)
        },
        {
        XlNsubsystem,
        XlCSubsystem,
        XlDsubsystem,
        XlRSubSystem,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlBitmapRec,bitmap.subsystem),
        sizeof(char*)
        },
        {
        XlNcolorNorm,
        XlCColorNorm,
        XlDcolorNorm,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlBitmapRec,bitmap.color_norm),
        sizeof(Pixel)
        },
 	{
        XlNnomeFile,
        XlCNomeFile,
        XlDnomeFile,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlBitmapRec,bitmap.nome_file_bitmap),
        sizeof(char*)
        },
	{
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvarInputCambioColore1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XlDcoloreFuoriScansione1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XlDcoloreAutomatico1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XlDcoloreStimato1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.stimato),
        sizeof(Pixel)
        },

        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XlDcoloreFuoriAttendib1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XlDcoloreBassissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XlDvaloreBassissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XlDcoloreBassissimoBasso1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XlDvaloreBassissimoBasso1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XlDcoloreBassoAlto1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XlDvaloreBassoAlto1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XlDcoloreAltoAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XlDvaloreAltoAltissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XlDcoloreAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlBitmapRec,bitmap.cambio_colore.altissimo),
        sizeof(Pixel)
        },
	};
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlBitmapClassRec xlBitmapClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Bitmap",
    /* widget_size              */      sizeof(XlBitmapRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      NULL,
    /* num_actions              */      0,
    /* resources                */      resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      TRUE,
    /* compress_exposure        */      XtExposeCompressMultiple, /* TRUE*/
    /* compress_enterleave      */      TRUE,
    /* visible_interest         */      FALSE,
    /* destroy                  */      Destroy,
    /* resize                   */      XtInheritResize,
    /* expose                   */      Redisplay,
    /* set_values               */      SetValues,
    /* set_values_hook          */      NULL,
    /* set_values_almost        */      XtInheritSetValuesAlmost,
    /* get_values_hook          */      NULL,
    /* accept_focus             */      NULL,
    /* version                  */      XtVersion,
    /* callback_private         */      NULL,
    /* tm_table                 */      defaultTranslations,
    /* query_geometry           */      XtInheritQueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* legocore fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia  */   NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* DrawBorserIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* bitmap fields */
    /* empty                    */      0
  }
};

WidgetClass xlBitmapWidgetClass = (WidgetClass) &xlBitmapClassRec;



/*
static GC RefreshGC(cambio_colore)
CAMBIO_COLORE cambio_colore;
{
float valore = cambio_colore.input.valore;


if(valore<=cambio_colore.bassissimo_val)
	return(cambio_colore.bassissimo_gc);

if((valore>cambio_colore.bassissimo_val)&&
	(valore<=cambio_colore.bassissimo_basso_val))
		return(cambio_colore.bassissimo_basso_gc);

if((valore>cambio_colore.bassissimo_basso_val)&&
	(valore<=cambio_colore.basso_alto_val))
		return(cambio_colore.basso_alto_gc);

if((valore>cambio_colore.basso_alto_val)&&
	(valore<=cambio_colore.alto_altissimo_val))
		return(cambio_colore.alto_altissimo_gc);

return(cambio_colore.altissimo_gc);
}
*/

static void GetNormFgGC(w)
Widget w;
{
XlBitmapWidget cw= (XlBitmapWidget)w;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction | GCClipMask;

values.foreground = cw->bitmap.color_norm;
values.background = cw->core.background_pixel;

values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;
values.clip_mask=cw->bitmap.bitmap_norm;
cw->bitmap.norm_gc = XtGetGC(cw, valuemask, &values);
}

static void GetClearGC(w)
Widget w;
{
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;
XlBitmapWidget cw= (XlBitmapWidget) w;
values.foreground = cw->core.background_pixel;
values.background = cw->bitmap.color_norm;
values.line_width = 0;
values.line_style = LineSolid;
cw->bitmap.clear_gc = XtGetGC(cw, valuemask, &values);
}


static void GetAllGCs(w)
Widget w;
{
XlBitmapWidget cw= (XlBitmapWidget)w;

GetNormFgGC(w);
XlGetCambioColoreGC(w,&(cw->bitmap.cambio_colore),
	NULL,cw->bitmap.bitmap_norm,False);
GetClearGC(w);
}


static void LoadNullBitmap(w)
Widget w;
{
XlBitmapWidget cw= (XlBitmapWidget)w;
Arg args[2];
int screen_num=DefaultScreen(XtDisplay(w));

XlWarning("XlBitmap","LoadNullBitmap",
		"utilizzo il bitmap di default");

cw->bitmap.bitmap_norm=XCreateBitmapFromData(XtDisplay(cw),XtWindow(cw),
		nullo_bits,nullo_width,nullo_height);


XtSetArg(args[0],XtNwidth,nullo_width);
XtSetArg(args[1],XtNheight,nullo_height);
XtSetValues(w,args,2);
}

static void LoadBitmap(w)
Widget w;
{
XlBitmapWidget cw= (XlBitmapWidget)w;
Pixmap pix;
int xh,yh;
int screen_num=DefaultScreen(XtDisplay(w));
int ret;
unsigned int width,height;
Arg args[2];
char path_bitmap[1024];
char *disegni_dir;
FILE *fp;
char _nome_file_bitmap[FILENAME_MAX];

/*
	concateno al nome del file l'estensione
*/
sprintf(_nome_file_bitmap,"%s.bmp",cw->bitmap.nome_file_bitmap);

/*
	verifico che cw->bitmap.nome_file_bitmap contenga qualche cosa
*/
if((cw->bitmap.nome_file_bitmap==NULL)||
	(!strcmp(_nome_file_bitmap,"nullo.bmp")))
	{
	XlWarning("XlBitmap","LoadBitmap",
		"nome del file non definito");
	LoadNullBitmap(cw);
	return;
	}


/*
	verifico se il file si trova nella directory corrente
*/
if((fp=fopen(_nome_file_bitmap,"r"))!=NULL)
	{
	fclose(fp);
	strcpy(path_bitmap,_nome_file_bitmap);
	}
else
	{
	disegni_dir=getenv("LEGOMMI_ICO");
	if(disegni_dir==NULL)
		{
		XlWarning("XlBitmap","LoadBitmap",
			"impossibile leggere la variabili LEGOMMI_ICO");
		LoadNullBitmap(cw);
		return;
		}
	strcpy(path_bitmap,disegni_dir);
	strcat(path_bitmap,"/");
	strcat(path_bitmap,_nome_file_bitmap);
	if((fp=fopen(path_bitmap,"r"))!=NULL)
		{
		fclose(fp);
		}
	else
		{
		XlWarning("XlBitmap","LoadBitmap",
			"impossibile aprire il file in LEGOMMI_ICO-->");
		XlWarning("XlBitmap","LoadBitmap",
			path_bitmap);
		LoadNullBitmap(cw);
		return;
		}
	}


ret=XReadBitmapFile(XtDisplay(w), XtWindow(w),
		path_bitmap,&width,&height,
		&(cw->bitmap.bitmap_norm),&xh,&yh);

XtSetArg(args[0],XtNwidth,width);
XtSetArg(args[1],XtNheight,height);
XtSetValues(w,args,2);
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
Arg arg[5];
XlBitmapWidget new = (XlBitmapWidget)tnew;
Pixel back_parent;

/*
	inizializzo a zero il campo bitmap_norm
*/
new->bitmap.bitmap_norm=NULL;

/*
	inizializzo a NULL il campo oldGC
*/
new->bitmap.oldGC=NULL;

/*
        leggo il background del padre e lo setto come background dell'icona
*/
XtSetArg(arg[0],XmNbackground,&back_parent);
XtGetValues(XtParent(tnew),arg,1);
XtSetArg(arg[0],XmNbackground,back_parent);
XtSetValues(tnew,arg,1);


}

#define superclass (&xlCoreClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlCore per la gestione della
 selezione
*/
(*superclass->xlcore_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*superclass->xlcore_class.select)(w,1);
}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlBitmapWidget curcw= (XlBitmapWidget) current;
XlBitmapWidget newcw= (XlBitmapWidget) new;
Boolean do_redisplay = False;


if(curcw->bitmap.color_norm != newcw->bitmap.color_norm)
        {
        XtReleaseGC(curcw,curcw->bitmap.norm_gc);
        GetNormFgGC(newcw);
        do_redisplay = True;
        }
if(strcmp(curcw->bitmap.nome_file_bitmap,newcw->bitmap.nome_file_bitmap))
	{
	XFreePixmap(XtDisplay(curcw),curcw->bitmap.bitmap_norm);
	LoadBitmap(newcw);
        do_redisplay = True;
	}


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlBitmapWidget cw= (XlBitmapWidget) w;

/*
	Rilascio i GC
*/
if(cw->bitmap.norm_gc)
	XtReleaseGC(cw,cw->bitmap.norm_gc);
if(cw->bitmap.clear_gc)
	XtReleaseGC(cw,cw->bitmap.clear_gc);
XlFreeCambioColoreGC(cw,cw->bitmap.cambio_colore,False);
/*
	Rilascio l'area allocata per il Pixmap
*/
if(cw->bitmap.bitmap_norm)
	XFreePixmap(XtDisplay(cw),cw->bitmap.bitmap_norm);
}


static Boolean Compile(Widget w)
{
Boolean errors=False;
XlBitmapWidget cw= (XlBitmapWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlBitmap","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
if(strlen(cw->bitmap.cambio_colore.str_input)==0)
	{
        XlErrComp(w,"Compile","Input variable not defined",NULL);
        errors=True;
        }

if(xlBitmapClassRec.xlcore_class.compilatore==NULL)
        {
        XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlBitmapClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }

XlEvError(cw,errors);
return(True);
}

static Boolean Refresh(Widget w)
{
XlBitmapWidget cw= (XlBitmapWidget)w;
int width=cw->core.width;
int height=cw->core.height;
XExposeEvent event;
GC RetGC;



if(cw->xlcore.config == True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlBitmap","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlBitmapClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlBitmap","Refresh","database non definito");
        return(False);
        }

if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlBitmap","Refresh","errore refresh classe superiore");
        return(False);
        }



/*
	Richiamo il metodo di Redisplay di XlCore per la gestione della
	trasparenza e del contorno di selezione dell'oggetto
*/
event.x=0;
event.y=0;
event.width=width;
event.height=height;


RetGC=XlFlagToGC(cw->bitmap.cambio_colore);

if(cw->bitmap.oldGC!=RetGC)
	{
	(*superclass->core_class.expose) (w,&event,NULL);
	if(cw->bitmap.bitmap_norm)
   		XCopyPlane(XtDisplay(cw),cw->bitmap.bitmap_norm,
          		XtWindow(cw),RetGC,0,0,width,height,0,0,1);
	cw->bitmap.oldGC=RetGC;
	}
return(True);
}




static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlBitmapWidget cw= (XlBitmapWidget)w;
register int x,y;
unsigned int width,height;
GC RetGC;



if(cw->bitmap.bitmap_norm==NULL)
	{
	LoadBitmap(cw);
	GetAllGCs(cw);
	}


if(event)
        {
        x=event->x;
        y=event->y;
        width=event->width;
        height=event->height;
        }
else
        {
        x=0;
        y=0;
        width=cw->core.width;
        height=cw->core.height;
        }


/*
	Richiamo il metodo di Redisplay di XlCore per la gestione della
	trasparenza e del contorno di selezione dell'oggetto
*/
(*superclass->core_class.expose) (w,event,NULL);

if(cw->xlcore.config == True)
	RetGC = cw->bitmap.cambio_colore.bassissimo_gc;
else
	RetGC=XlFlagToGC(cw->bitmap.cambio_colore);

if(cw->bitmap.bitmap_norm)
   XCopyPlane(XtDisplay(cw),cw->bitmap.bitmap_norm,
          XtWindow(cw),RetGC,x,y,width,height,x,y,1);
/*
	Richiamo del metodo di disegno del bordo se l'oggetto
	e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);
}


/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
return(XlHardCopyWidget(w));
}
#endif
