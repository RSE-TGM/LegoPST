/**********************************************************************
*
*       C Source:               XlPixvar.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:43:21 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlPixvar.c-4.2.3 %  (%full_filespec: XlPixvar.c-4.2.3:csrc:1 %)";
#endif
/*
   modulo XlPixvar.c
   tipo 
   release 5.2
   data 1/5/96
   reserved @(#)XlPixvar.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlPixvar.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS

*/
/*
 *  XlPixvar.c - widget pixvar per stazioni
 */
#include <Xm/XmP.h>
#ifndef VMS
#include <X11/Xmu/Xmu.h>
#endif
#include <X11/StringDefs.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlPixvarP.h>
#include <Xl/nullo.bmp>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 10
#define DEFAULTHEIGHT 10

static int leggi_pict_program();

/* macro per la verifica sulla esistenza di un file
*/
struct stat Statbuf;
# define esiste(file)           (stat(file,&Statbuf)<0 ? 0:Statbuf.st_mode)

/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNcomponent,
        XlCComponent,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.component),
        XmRImmediate,
        ""
        },
	{
        XlNsubsystem,
        XlCSubsystem,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.subsystem),
        XmRImmediate,
        ""
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.normalfont),
        XmRImmediate,
        "fixed"
        },
	{
        XlNpixType1,
        XlCpixType1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.pixtype),
        XmRImmediate,
        0
        },
	{
        XlNnomeFilePix1,
        XlCNomeFilePix1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix1),
        XmRImmediate,
        "null"
        },
	{
        XlNnomeFilePix2,
        XlCNomeFilePix2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix2),
        XmRImmediate,
        "null"
        },
	{
        XlNnomeFilePix3,
        XlCNomeFilePix3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix3),
        XmRImmediate,
        "null"
        },
	{
        XlNnomeFilePix4,
        XlCNomeFilePix4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix4),
        XmRImmediate,
        "null"
        },
        {
        XlNnomeProgram,
        XlCNomeProgram,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.nome_file_program),
        XmRImmediate,
        "null"
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.top_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.bottom_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.left_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.right_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_1,
        XlCVarInputCambioColore1_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_1,
        XlCFlagOffset1_1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[0].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_2,
        XlCVarInputCambioColore1_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_2,
        XlCFlagOffset1_2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[1].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_3,
        XlCVarInputCambioColore1_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_3,
        XlCFlagOffset1_3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[2].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_4,
        XlCVarInputCambioColore1_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_4,
        XlCFlagOffset1_4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[3].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_5,
        XlCVarInputCambioColore1_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[4].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_5,
        XlCFlagOffset1_5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[4].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_6,
        XlCVarInputCambioColore1_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[5].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_6,
        XlCFlagOffset1_6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[5].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNcoloreDefault1,
        XlCColoreDefault1,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].defaultColor),
        XmRString,
        XlDefaultColore1
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
        XtOffsetOf(XlPixvarRec,xlcore.trasparent),
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
        XtOffsetOf(XlPixvarRec,pixvar.component),
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
        XtOffsetOf(XlPixvarRec,pixvar.subsystem),
        sizeof(char*)
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.normalfont),
        sizeof(XFontStruct* ) 
        },
	{
        XlNpixType1,
        XlCpixType1,
        XlDpixType1,
        XlROption,
        NULL,
        XlOpixType1,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.pixtype),
        sizeof(int)
        },
	{
        XlNnomeFilePix1,
        XlCNomeFilePix1,
        XlDnomeFilePix1,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix1),
        sizeof(char*)
        },
	{
        XlNnomeFilePix2,
        XlCNomeFilePix2,
        XlDnomeFilePix2,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix2),
        sizeof(char *)
        },
	{
        XlNnomeFilePix3,
        XlCNomeFilePix3,
        XlDnomeFilePix3,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix3),
        sizeof(char *)
        },
	{
        XlNnomeFilePix4,
        XlCNomeFilePix4,
        XlDnomeFilePix4,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.nome_pix4),
        sizeof(char *)
        },
        {
        XlNnomeProgram,
        XlCNomeProgram,
        XlDnomeProgram,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.nome_file_program),
        sizeof(char*)
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XlDtopBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.top_border),
        sizeof(int)
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XlDbottomBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.bottom_border),
        sizeof(int)
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XlDleftBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.left_border),
        sizeof(int)
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XlDrightBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPixvarRec,pixvar.right_border),
        sizeof(int)
        },
	{
        XlNvarInputCambioColore1_1,
        XlCVarInputCambioColore1_1,
        XlDvarInputCambioColore1_1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_1,
        XlCFlagOffset1_1,
        XlDflagOffset1_1,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[0].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_2,
        XlCVarInputCambioColore1_2,
        XlDvarInputCambioColore1_2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_2,
        XlCFlagOffset1_2,
        XlDflagOffset1_2,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[1].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_3,
        XlCVarInputCambioColore1_3,
        XlDvarInputCambioColore1_3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_3,
        XlCFlagOffset1_3,
        XlDflagOffset1_3,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[2].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_4,
        XlCVarInputCambioColore1_4,
        XlDvarInputCambioColore1_4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_4,
        XlCFlagOffset1_4,
        XlDflagOffset1_4,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[3].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_5,
        XlCVarInputCambioColore1_5,
        XlDvarInputCambioColore1_5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[4].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_5,
        XlCFlagOffset1_5,
        XlDflagOffset1_5,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[4].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_6,
        XlCVarInputCambioColore1_6,
        XlDvarInputCambioColore1_6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[5].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_6,
        XlCFlagOffset1_6,
        XlDflagOffset1_6,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].pict_var[5].FlagOffset),
        sizeof(int)
        },
        {
        XlNcoloreDefault1,
        XlCColoreDefault1,
        XlDcoloreDefault1,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPixvarRec,pixvar.cambio_colore[0].defaultColor),
        sizeof(XlAnimatedColor)
        },
	};
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void ClassInitialize();
static void Initialize();
static void Destroy();
static void Redisplay();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
static void LoadProgram();
static void LoadPixvar();
static Boolean LoadFont();
static void LoadNullBitmap();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* dichiarazione di funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static int EsistePix();
static BCK_OBJ *InsertBck();
static void redraw( );

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlPixvarClassRec xlPixvarClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Pixvar",
    /* widget_size              */      sizeof(XlPixvarRec),
    /* class_initialize         */      ClassInitialize,
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
  { /* pixvar fields             */
    /* num_pixvar                */      0,
    /* num program               */      0,
    /* program                   */      NULL
  }
};

WidgetClass xlPixvarWidgetClass = (WidgetClass) &xlPixvarClassRec;

static void ClassInitialize()
{
   xlPixvarClassRec.xlpixvar_class.programs=NULL;
   xlPixvarClassRec.xlpixvar_class.num_program=0;
}

static void GetClearGC(w)
Widget w;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground ; 
                            
   XlPixvarWidget cw= (XlPixvarWidget) w;

   values.foreground = cw->pixvar.cambio_colore[0].defaultColor.colore;
   values.background = cw->core.background_pixel; 

   cw->pixvar.clear_gc = XtGetGC(w, valuemask, &values);
}

static void GetDefaultGC(w)
Widget w;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground ; 

   XlPixvarWidget cw= (XlPixvarWidget) w;


   values.foreground = cw->pixvar.cambio_colore[0].defaultColor.colore;
   values.background = cw->core.background_pixel; 

   cw->pixvar.cambio_colore[0].defaultColor_gc = XtGetGC(w, valuemask, &values);
}

/* =================================================================
   verifica nella struttura della classe la presenza di uno sfondo
   17/7/97 viene verificato oltre al nome anche lo screen perche'
           avendo nella struttura dei Pixel value questi sono legati
           allo screen.
   ritorno:
                posizione: se e' stata trovata
                -1 : se non e' stata trovata
   =================================================================
*/
static int EsisteProgram(w,nome_program)
Widget w;
char *nome_program;
{
   int num_program;
   int i;

   num_program=xlPixvarClassRec.xlpixvar_class.num_program;

   for(i=0;i<num_program;i++)
        if(strcmp(xlPixvarClassRec.xlpixvar_class.programs[i].nome_program,
                    nome_program)==0) 
           if( XtScreen(w) == xlPixvarClassRec.xlpixvar_class.programs[i].scr )
                return(i);

   return(-1);
}


/* aggiorno la cache dei programmi nella classe dello widget
*/
static void aggiorna_classe_prg(Widget w,XlPixvarAnimaz *prg)
{
   int num_program;
   int nrow;
   int i,j,nval;
   int val;

   num_program = xlPixvarClassRec.xlpixvar_class.num_program;
   nrow = prg->nrow;
   
   /* se nella cache non c'e' niente la XtRealloc essendo il ptr pari a NULL 
      fa una semplice XtMalloc altrimenti alloca nuovo spazio e copia 
      il vecchio contenuto
   */    
   if( (xlPixvarClassRec.xlpixvar_class.programs =
           (XlPixvarAnimaz *)XtRealloc(
                      xlPixvarClassRec.xlpixvar_class.programs,
                      sizeof(XlPixvarAnimaz)*(num_program+1))) == NULL)
   {
         fprintf(stderr,"XlPixvar: Erroc in allocation of programs cache\n");
         return;
   }
   
   /* copio il nome del programma e il numero di righe nella cache 
      appena allocata
   */   
   memcpy(&xlPixvarClassRec.xlpixvar_class.programs[num_program],
          prg,sizeof(XlPixvarAnimaz));


   /* alloco spazio per le righe
      +1 perche chiudo il vettore con idvar = -1
      (se non sivuole piu' chiudere il vettore con -1 e' necessario rivedere
       le allocazioni, la leggi_pict_program e principalmete la XlValueToBlink)
   */
   if( 
     (xlPixvarClassRec.xlpixvar_class.programs[num_program].row =
      (XlPixvarAnimazRow *) XtCalloc(nrow+1,sizeof(XlPixvarAnimazRow))) == NULL) 
   {
         fprintf(stderr,"XlPixvar: Erroc in allocation of rows programs cache\n");
         return;
   }
  
   /* trasferisco i dati delle righe nella cache
   for(i=0;i<prg->nrow;i++)
   {
      xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].idvar =
      prg.row[i].idvar;
      xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].nval = 
      prg.row[i].nval;
      xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].animaz.colore = prg->row[i].animaz.colore;
      xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].animaz.blink_rate = prg->row[i].animaz.blink_rate;
      xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].animaz.trasparent = prg->row[i].animaz.trasparent;

   }
   */

   /* trasferisco i dati delle righe nella cache
      che contengono id della zona il colore animato , il gc
      e il numero dei valori di confronto
      nonche il vettore dei calori che pero' copio successivamente
   */
   for(i=0;i<prg->nrow;i++)
   {
     memcpy(&xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i],
          &prg->row[i],sizeof(XlPixvarAnimazRow));
   }

   

   /* alloco spazio per il vettore dei valori associato 
      a ciascuna riga
   */
   for(i=0;i<prg->nrow;i++)
   {
      nval = prg->row[i].nval;

      if( (xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].val=
         (int *)XtCalloc(1,sizeof(int)*nval))==NULL)
      {
         fprintf(stderr,"XlPixvar: Error in malloc caching values\n");
         return;
      }

      for(j=0;j<nval;j++)
      {
         val = prg->row[i].val[j];
         xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].val[j] = val;

         val = xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].val[j];

     }
  }
  xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].idvar = -1;

  xlPixvarClassRec.xlpixvar_class.num_program++;

}

/*****************************************************
 * LoadProgram
 * effettua la lettura del file program (.prg) associato
 * all'oggetto. Una volta caricato lo mette nella cache
 * della classe dell'oggetto. Di conseguenza l'oggetto  
 * successivo che fara' riferimento allo stesso program 
 * non carichera' il program ma semplicemente assnera'
 * l'indice di quello gia' presente nella cache.
 * - NB - il program caricato contenendo dei colori 
 *        in forma di Pixel e' legato allo screen
 *        percui oltre al nome del file program e' 
 *        identificato dallo SCREEN relativo all'oggetto  
 *        Per la implementazione attuale quindi un program
 *        puo' essere caricato piu' volte nella cache
 *        perche associato a piu' di uno screen
 *****************************************************/
static void LoadProgram(w)
Widget w;
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   XlPixvarClassRec *wPixvarClass;
   int num_program; 
   int pos_program;
   XlPixvarAnimaz *prg;

   /* inizializzo a null 
   */
   cw->pixvar.program=NULL;

   /* verifica la esistenza del program nella cache
      e se esiste ne individua la posizione nel vettore 
      NB oltre al nome nel programma di ricerca viene verificato
         anche lo screen dell'oggetto.
   */
   pos_program=EsisteProgram(XtParent(w),cw->pixvar.nome_file_program);
   

   /* se il programma non e' gia' stato caricato
      lo carica
   */
   if(pos_program == -1)
   {
      /* leggo il file program e lo metto direttamente nella cache
      */

      /* se nella cache non c'e' niente la XtRealloc essendo il ptr pari a NULL
         fa una semplice XtMalloc altrimenti alloca nuovo spazio e copia
         il vecchio contenuto
      */

      num_program = xlPixvarClassRec.xlpixvar_class.num_program;

      if( (xlPixvarClassRec.xlpixvar_class.programs =
           (XlPixvarAnimaz *)XtRealloc(
                      xlPixvarClassRec.xlpixvar_class.programs,
                      sizeof(XlPixvarAnimaz)*(num_program+1))) == NULL)
      {
         fprintf(stderr,"XlPixvar: Erroc in allocation of programs cache\n");
         return;
      }

      if( leggi_pict_program(w,cw->pixvar.nome_file_program,
                             &xlPixvarClassRec.xlpixvar_class.programs[num_program]) )
      {
         fprintf(stderr,"Error reading pixvar program %s",
                       cw->pixvar.nome_file_program);

         XtFree(xlPixvarClassRec.xlpixvar_class.programs);
         return;
      }

      xlPixvarClassRec.xlpixvar_class.num_program++;
      pos_program=EsisteProgram(XtParent(w),cw->pixvar.nome_file_program);
      wPixvarClass=(XlPixvarClassRec *)XtClass(w);
      cw->pixvar.index_program=pos_program;


   }
   else
   {

      /* gestisco qui il caso che lo sfondo sia gia' presente
         in memoria
      */
      pos_program=EsisteProgram(XtParent(w),cw->pixvar.nome_file_program);
      wPixvarClass=(XlPixvarClassRec *)XtClass(w);
      cw->pixvar.index_program=pos_program;
   }

}

/* load dei 4 pixmap o stringhe associati
   all'oggetto.
*/
static Boolean LoadPixmap(w,width,height)
Widget w;
int *width;
int *height;
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   int width_ret,height_ret,x_ret,y_ret;
   Pixmap bitmap,pixdest;
   char nomebmp[255];
   char bmpfname[255];   
   Display *disp;
   Arg args[2];
   int indice;

   
   disp = XtDisplay(w);


/* carico i quattro pixmap 
*/


   for( indice = 1; indice < 5; indice ++ )
   {

      /* identifico quale indice pixmap caricare
      */
      switch( indice )
      {
      case 1:
        sprintf(nomebmp,"%s",cw->pixvar.nome_pix1);
      break;
      case 2:
        sprintf(nomebmp,"%s",cw->pixvar.nome_pix2);
      break;
      case 3:
        sprintf(nomebmp,"%s",cw->pixvar.nome_pix3);
      break;
      case 4:
        sprintf(nomebmp,"%s",cw->pixvar.nome_pix4);
      break;
      }          
 

      if( !strlen(nomebmp) )
         continue;


      /* carico il Pixmap associati al pixvar
         cercando il file bitmap prima nel direttorio locale
         e successivamente in LEGOMMI_ICO
      */   
      if( !esiste(nomebmp) )
      {
          if( getenv("LEGOMMI_ICO") != NULL)         
          {
             sprintf(bmpfname,"%s/%s.bmp",getenv("LEGOMMI_ICO"),nomebmp);
             if( !esiste(bmpfname) )
             {
                fprintf(stderr,"Warning: Xlpixvar: %s file not found\n",bmpfname);
                return(False);
             }
          }
       }
       else
         strcpy(bmpfname,nomebmp);      

       /* leggo il file bitmap 
       */
       printf("XReadBitmapFile [%s]\n",bmpfname);

       if( (XReadBitmapFile( disp, 
                         RootWindow( disp, DefaultScreen(disp) ), 
                         bmpfname,
                         &width_ret, &height_ret, 
                         &bitmap,
                         &x_ret, &y_ret)) == -1)
       {
          fprintf(stderr,"Error: XlPixvar cannot read bitmap file %s\n",bmpfname);
          return(False);
       }   
      

      switch( indice )
      {
       case 1:
         cw->pixvar.pix1 = XCreatePixmap( disp, 
                           RootWindow( disp, DefaultScreen(disp)),
                           width_ret, height_ret,
                           DefaultDepth( disp, DefaultScreen(disp) ) );
         XCopyPlane( 
               disp,
               bitmap,
               cw->pixvar.pix1,
               (cw->pixvar).cambio_colore[0].defaultColor_gc,
               0,
               0,
               width_ret,
               height_ret,
               0, 
               0, 
               1);
                         
       break;
       case 2:
         cw->pixvar.pix2 = XCreatePixmap( disp, 
                           RootWindow( disp, DefaultScreen(disp)),
                           width_ret, height_ret,
                           DefaultDepth( disp, DefaultScreen(disp) ) );

         XCopyPlane( 
               disp,
               bitmap,
               cw->pixvar.pix2,
               (cw->pixvar).cambio_colore[0].defaultColor_gc ,
               0,
               0,
               width_ret,
               height_ret,
               0, 
               0, 
               1);  


       break;
       case 3:

         cw->pixvar.pix3 = XCreatePixmap( disp, 
                           RootWindow( disp, DefaultScreen(disp)),
                           width_ret, height_ret,
                           DefaultDepth( disp, DefaultScreen(disp) ) );


         XCopyPlane( 
               disp,
               bitmap,
               cw->pixvar.pix3,
               (cw->pixvar).cambio_colore[0].defaultColor_gc ,
               0,
               0,
               width_ret,
               height_ret,
               0, 
               0, 
               1);  


       break;
       case 4:
         cw->pixvar.pix4 = XCreatePixmap( disp, 
                           RootWindow( disp, DefaultScreen(disp)),
                           width_ret, height_ret,
                           DefaultDepth( disp, DefaultScreen(disp) ) );

         XCopyPlane( 
               disp,
               bitmap,
               cw->pixvar.pix4,
               (cw->pixvar).cambio_colore[0].defaultColor_gc ,
               0,
               0,
               width_ret,
               height_ret,
               0, 
               0, 
               1);  

       break;
      }   

      XFreePixmap(XtDisplay(w),bitmap);  
      
     *width = width_ret;
     *height = height_ret;

   }

   return(True);

}


/* metodo Initializa oggetto Pixvar
*/
static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
   Arg arg[5];
   XlPixvarWidget new = (XlPixvarWidget)tnew;
   int width,height,x,y,width_ret,height_ret;
   int zoom;
   int slen;
	
   /* inizializzazione nuova istanza di Pixvar
   */
   new->pixvar.region=NULL;
   new->pixvar.primo_refresh=1;

   new->pixvar.blinking = 0;
   new->pixvar.blink_freq = 0.0;

   new->pixvar.pix1 =  0;
   new->pixvar.pix2 =  0;
   new->pixvar.pix3 =  0;
   new->pixvar.pix4 =  0;

   new->pixvar.prevIndPix = -1;
   new->pixvar.currIndPix = -1;


   GetClearGC(new);
   GetDefaultGC(new);

   new->pixvar.cambio_colore[0].defaultColor.blink_rate = 0;

   /* carica il programma
   */
   LoadProgram(tnew);

   /* carico i quattro pixmap 
   */
   if( new->pixvar.pixtype == PIX )
   {
      if( !LoadPixmap(tnew,&width_ret,&height_ret) )
         LoadNullBitmap( tnew  );

      width = width_ret;
      height = height_ret;

   }
   else if(!LoadFont(new,new->pixvar.normalfont,False))
   {
          XlError("XlPixvar","Initialize","LoadFont selected failed");
          if(!LoadFont(new,"fixed",False))
              XlError("XlPixvar","Initialize",
                      "LoadFont of fixed font failed");
    
       height=new->pixvar.font_info->ascent+
                        new->pixvar.font_info->descent;
       width=XTextWidth(new->pixvar.font_info,
                                  new->pixvar.nome_pix1,
                                  strlen(new->pixvar.nome_pix1));
   }

   if( (width>0) && (height>0) )
   {

      if(XlIsXlComposite(XtParent(tnew)))
      {
         zoom=XlGetZoom(XtParent(tnew));
         width = width * zoom/100;
         height = height * zoom/100;
      }

      width++;
      height++;

      XtSetArg(arg[0],XmNwidth,width);
      XtSetArg(arg[1],XmNheight,height);
      XtSetValues(tnew,arg,2);
   }

   new->pixvar.cambio_colore[0].pict_var[0].input.valore = 0.0;
   new->pixvar.cambio_colore[0].pict_var[1].input.valore = 0.0;
   new->pixvar.cambio_colore[0].pict_var[2].input.valore = 0.0;
   new->pixvar.cambio_colore[0].pict_var[3].input.valore = 0.0;
   new->pixvar.cambio_colore[0].pict_var[4].input.valore = 0.0;
   new->pixvar.cambio_colore[0].pict_var[5].input.valore = 0.0;

}




/* ===========================
   Metodo di Selezione  
   in configurazione
   Gestito dalle Translation
   =========================== */

#define superclass (&xlCoreClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* Richiama la routine di select di XlCore per la gestione della
       selezione
   */
   (*superclass->xlcore_class.select) (w,0);
}


/* ===================================
   Metodo di aggiunta alla lista degli
   oggetti selezionati 
   in configurazione
   Gestito dalle Translation
   =================================== */

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* richiama il metodo di select 
   */
   (*superclass->xlcore_class.select)(w,1);
}


/* ===================================
   Metodo di SetValues
   =================================== */

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
   XlPixvarWidget curcw= (XlPixvarWidget) current;
   XlPixvarWidget newcw= (XlPixvarWidget) new;
   Boolean do_redisplay = False;

   if(curcw->pixvar.cambio_colore[0].defaultColor.colore != 
      newcw->pixvar.cambio_colore[0].defaultColor.colore)
   {
      XtReleaseGC( curcw, curcw->pixvar.cambio_colore[0].defaultColor_gc);
      XtReleaseGC( curcw, curcw->pixvar.clear_gc);
      GetClearGC( newcw );
      GetDefaultGC( newcw );
      do_redisplay = True; 
   }

   if(curcw->core.background_pixel != 
      newcw->core.background_pixel )
   {
      XtReleaseGC( curcw, curcw->pixvar.cambio_colore[0].defaultColor_gc);
      XtReleaseGC( curcw, curcw->pixvar.clear_gc);
      GetClearGC( newcw );
      GetDefaultGC( newcw );
      do_redisplay = True; 
   }

   if(strcmp(curcw->pixvar.normalfont,newcw->pixvar.normalfont))
   {
      if(!LoadFont(newcw,newcw->pixvar.normalfont),False)
      {
        XlError("XlPixvar","SetValues", "Load selected font failed");
        if(!LoadFont(newcw,"fixed"),False)
             XlError("XlPixvar","SetValues", "Load fixed font failed");
      }
      do_redisplay = True; 
   }

   if( strcmp(curcw->pixvar.nome_file_program,newcw->pixvar.nome_file_program) )
   {
      Compile(newcw);
      do_redisplay = True; 
   }

   return(do_redisplay);
}

/* ===================================
   Metodo di Destroy
   =================================== 
*/

static void Destroy(w)
Widget w;
{
   float bfreq[4];
   XlPixvarWidget cw= (XlPixvarWidget) w;


   if( cw->pixvar.clear_gc )
     XtReleaseGC(w,cw->pixvar.clear_gc);
   if( cw->pixvar.cambio_colore[0].defaultColor_gc)
     XtReleaseGC(w,cw->pixvar.cambio_colore[0].defaultColor_gc);

   if( cw->pixvar.pix1 )
     XFreePixmap( XtDisplay(w),cw->pixvar.pix1 );
   
   if( cw->pixvar.pix2 )
     XFreePixmap( XtDisplay(w),cw->pixvar.pix2 );
   
   if( cw->pixvar.pix3 )
     XFreePixmap( XtDisplay(w),cw->pixvar.pix3 );
   
   if( cw->pixvar.pix4 )
     XFreePixmap( XtDisplay(w),cw->pixvar.pix4 );

   /* elimina, se necessario, il widget dalla lista di blink 
   */
   if( cw->pixvar.blinking )
      XlRemoveBlink(cw,cw->pixvar.blinking );

}


/* ===================================
   Metodo di Compilazione
   =================================== 
*/

static Boolean Compile(Widget w)
{
   static char *icon_dir=NULL;
   Boolean errors=False;
   XlPixvarWidget cw= (XlPixvarWidget)w;
   char nome_file[255];

   if(!XtIsWidget(w))
   {
      XlWarning("XlPixvar","Compile",
                "il parametro in ingresso non e' un Widget");
      return(False);
   }

   /* get della icon directory
   */
   if(icon_dir==NULL)
   {
      icon_dir=getenv("LEGOMMI_ICO");
      if(icon_dir==NULL)
      {
         XlErrComp(w,"Compile","Environment variable LEGOMMI_ICO not defined",NULL);
         errors=True;
         XlEvError(cw,True);
      }
   }

   /* il nome del file e' dato dalla risorsa nomeProgram
      con estensione .prg
   */
   sprintf(nome_file,"%s/%s.prx",icon_dir,cw->pixvar.nome_file_program);

   if( !esiste(nome_file) )
   {
      XlErrComp(w,"Compile","Program file not exist",NULL);
      errors=True;
      XlEvError(cw,True);
   }

   if(xlPixvarClassRec.xlcore_class.compilatore==NULL)
   {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
   }

   if(!(*superclass->xlcore_class.compile) (w,
        xlPixvarClassRec.xlcore_class.compilatore))
   {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
   }

   XlEvError(cw,errors);

   return(True);
}

/* =======================================================================
   XlPixvar Refresh Method:
       utilizzato run_time
           Effettua il ridisegno del XlPixvar 
           Nel caso di blink  questa routine setta i colori e la frequenza
           di blink il ridisegno lo fa la BlinkProc 
   ========================================================================
*/
static Boolean Refresh(Widget w)
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   int width=cw->core.width;
   int height=cw->core.height;
   int wi,he;
   XExposeEvent event;
   XmDrawingAreaCallbackStruct str;
   float blink_freq;
   Pixel RetPixel;
   int indpix;
   Arg args[3];

   /* se siamo in configurazione non devo fare niente
   */
   if(cw->xlcore.config == True)
      return(True);

   if(!XtIsWidget(w))
   {
      XlWarning("XlPixvar","Refresh","il parametro in ingresso non e' un Widget");
      return(False);
   }

   if(xlPixvarClassRec.xlcore_class.database==NULL)
   {
      XlWarning("XlPixvar","Refresh","database non definito");
      return(False);
   }

   /* inizializzo 
   */
   if(cw->pixvar.primo_refresh== 1) /* primo refresh */
   {
      cw->pixvar.blinking = 0;
      cw->pixvar.blink_freq = 0.0;
      cw->pixvar.primo_refresh=0;
   }

   /* in base al valore della calcolata stabilisce il colore e la frequenza
      di blink dell'oggetto che sono specificate nel programma di animazione (.prx)
   */
   if(  XlValueToBlinkPixmap(w,&indpix,&RetPixel,&blink_freq) != True )
   {
      printf("Error: XlPixvar.XlValueToBlinkPixmap ritorna False\n");
      return(False);
   }

   /* se il disegno cambia e non deve blinkare
   */
   if( (cw->pixvar.currIndPix != indpix)  && !blink_freq ) 
   {
      XClearWindow(XtDisplay(w),XtWindow(w));
   }
     

   /* imposto il colore del GC
   */
   XSetForeground(XtDisplay(w),cw->pixvar.cambio_colore[0].defaultColor_gc,RetPixel);

   /* se il pixmap/stringa attuale deve blinkare lo stabilisco dalla frequenza
   */
   if( (blink_freq > (float)0.0) )
   {
     if( (cw->pixvar.blinking!=indpix) || (cw->pixvar.blink_freq!=blink_freq) )
     {
        if( cw->pixvar.blinking )
           XlRemoveBlink(cw,cw->pixvar.blinking);

        cw->pixvar.blinking=indpix;
        cw->pixvar.blink_freq = blink_freq;
        XlAddBlink(cw,indpix,blink_freq,(void*)BlinkProc);
     }
   }
   else
   {
      if( cw->pixvar.blinking > 0 )
      {
         XlRemoveBlink(cw,cw->pixvar.blinking);
         XClearWindow(XtDisplay(w),XtWindow(w));
         cw->pixvar.blinking=0;
         cw->pixvar.blink_freq=0;
      }
   }


   /* recupero le dimensioni nel caso della stringa
   */
   switch( indpix )
   {
     case 1:
        if( cw->pixvar.pixtype == STR )
        {
           height=cw->pixvar.font_info->ascent+
                        cw->pixvar.font_info->descent;
           width=XTextWidth(cw->pixvar.font_info,
                                  cw->pixvar.nome_pix1,
                                  strlen(cw->pixvar.nome_pix1));
        }
     break;
     case 2:
        if( cw->pixvar.pixtype == STR )
        {
           height=cw->pixvar.font_info->ascent+
                        cw->pixvar.font_info->descent;
           width=XTextWidth(cw->pixvar.font_info,
                                  cw->pixvar.nome_pix2,
                                  strlen(cw->pixvar.nome_pix2));
        }
     break;
     case 3:
        if( cw->pixvar.pixtype == STR )
        {
           height=cw->pixvar.font_info->ascent+
                        cw->pixvar.font_info->descent;
           width=XTextWidth(cw->pixvar.font_info,
                                  cw->pixvar.nome_pix3,
                                  strlen(cw->pixvar.nome_pix3));
        }
     break;
     case 4:
        if( cw->pixvar.pixtype == STR )
        {
           height=cw->pixvar.font_info->ascent+
                        cw->pixvar.font_info->descent;
           width=XTextWidth(cw->pixvar.font_info,
                                  cw->pixvar.nome_pix4,
                                  strlen(cw->pixvar.nome_pix4));
        }
     break;
   }

   /* Aggiusto le dimensioni dell'oggetto ogni volta che cambia
      La XtResizeWidget ritorna subito se le dimensioni non sono cambiate.
      QUESTO VALE SOLO PER LE STRINGHE (per i bitmap dovrei recuperare 
      le nuove dimensioni del pixmap; cosa che attualmente non faccio)
   */
   if( cw->pixvar.currIndPix != indpix )
      XtResizeWidget(w,width,height,cw->core.border_width);


   /* faccio il refresh del core
   */ 
   if(!(*superclass->xlcore_class.refresh) (w))
   {
      XlWarning("XlPixvar","Refresh","errore refresh classe superiore");
      return(False);
   }

   if( !cw->pixvar.blinking && (cw->pixvar.currIndPix != cw->pixvar.prevIndPix))
      cw->pixvar.prevIndPix = cw->pixvar.currIndPix;
   
   cw->pixvar.currIndPix = indpix;

   /* ridisegno la icona, solo se non deve blinkare
   */
   if( !cw->pixvar.blinking )
      redraw(w);

   return(True);
}


/*========================
  Metodo di Redisplay
  =======================
*/
static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   register int x,y;
   unsigned int width,height;
   XmDrawingAreaCallbackStruct str;


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


   /* Richiamo il metodo di Redisplay di XlCore per la gestione della
      trasparenza e del contorno di selezione dell'oggetto
   */
   (*superclass->core_class.expose) (w,event,NULL);

   /* nella redisplay della config mi appoggio solo al primo pixmap/stringa
      se sono in config
   */
   if( cw->xlcore.config )
   {
      cw->pixvar.currIndPix = 1;
      redraw(w);
   }
   else
      redraw(w);


   /* Richiamo del metodo di disegno del bordo se l'oggetto
      e' selezionato
   */
   (*superclass->xlcore_class.drawBorderIfSelected) (w);

}
 
static void redraw(w)
Widget w;
{
  XlPixvarWidget cw= (XlPixvarWidget)w;
  Display *disp = XlDisplay(w);
  Arg args[2];
  int height,width,zoom;
  
  /* run-time non ridisegno una cosa disegnata
     un ciclo prima
  */

  switch(cw->pixvar.currIndPix)
  {
     case 1:
     if( cw->pixvar.pixtype == STR ) 
        XlTestoSimple(cw,XlWindow(w),
                        cw->pixvar.cambio_colore[0].defaultColor_gc,
                        0,50,cw->pixvar.nome_pix1, cw->pixvar.font_info);
     else
        XCopyPlane(XtDisplay(w),
                  cw->pixvar.pix1,
                  XtWindow(w),
                  cw->pixvar.cambio_colore[0].defaultColor_gc,
                  0,0,cw->core.width,cw->core.height,0,0,1);
     break;
     case 2:
     if( cw->pixvar.pixtype == STR ) 
        XlTestoSimple(cw,XlWindow(w),
                        cw->pixvar.cambio_colore[0].defaultColor_gc,
                        0,50,cw->pixvar.nome_pix2, cw->pixvar.font_info);
     else
        XCopyPlane(XtDisplay(w),
                  cw->pixvar.pix2,
                  XtWindow(w),
                  cw->pixvar.cambio_colore[0].defaultColor_gc,
                  0,0,cw->core.width,cw->core.height,0,0,1);
     break;
     case 3:
     if( cw->pixvar.pixtype == STR ) 
        XlTestoSimple(cw,XlWindow(w),
                        cw->pixvar.cambio_colore[0].defaultColor_gc,
                        0,50,cw->pixvar.nome_pix3, cw->pixvar.font_info);
     else
        XCopyPlane(XtDisplay(w),
                  cw->pixvar.pix3,
                  XtWindow(w),
                  cw->pixvar.cambio_colore[0].defaultColor_gc,
                  0,0,cw->core.width,cw->core.height,0,0,1);
     break;
     case 4:
     if( cw->pixvar.pixtype == STR ) 
        XlTestoSimple(cw,XlWindow(w),
                        cw->pixvar.cambio_colore[0].defaultColor_gc,
                        0,50,cw->pixvar.nome_pix4, cw->pixvar.font_info);
     else
        XCopyPlane(XtDisplay(w),
                  cw->pixvar.pix4,
                  XtWindow(w),
                  cw->pixvar.cambio_colore[0].defaultColor_gc,
                  0,0,cw->core.width,cw->core.height,0,0,1);
     break;

  }

}

/* ================================================================
   procedura per gestione del Blink
   Parametri:
      Widget w;    pixvar da far blinkare
      int ind;     zona animata del pixvar che deve blinkare
      Boolean On;  Blink on/off
   ================================================================
*/
static void BlinkProc(Widget w,int ind,Boolean On)
{
   XlPixvarWidget cw= (XlPixvarWidget) w;

  if( On )
     XClearWindow(XtDisplay(w),XtWindow(w));
  else
     redraw(w);
  
}


/*=======================
        metodo di stampa
  =======================
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   extern Display *printer;

   (*superclass->xlcore_class.print) (w);

   /* aggiungere il ridisegno per la stampa
   *****************************************/

   return(True);
}
#endif




/* inserisce il prg nella cache
*/
int insert_prx_cache(Widget w)
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   int num_program = xlPixvarClassRec.xlpixvar_class.num_program;
   int i,nrow,nval;

   /* alloco memoria per contenere il vettore dei programs
   */
   if( !num_program )
   {
      if( (xlPixvarClassRec.xlpixvar_class.programs = 
                 (XlPixvarAnimaz *)XtCalloc(1,sizeof(XlPixvarAnimaz)))==NULL)
      {
         fprintf(stderr,"XlPixvar: Erroc in malloc of programs cache\n");
         return(1);
      }
   }
   else
   {
      num_program++;
      if( (xlPixvarClassRec.xlpixvar_class.programs =
               (XlPixvarAnimaz *)XtRealloc(xlPixvarClassRec.xlpixvar_class.programs,
                         sizeof(XlPixvarAnimaz)*num_program)) == NULL)
      {
         fprintf(stderr,"XlPixvar: Erroc in realloc of programs cache\n");
         return(1);
      }
   }
    

   /* copio i dati nella cache appena allocata
      i dati copiati sono il nome del program e il numero di righe
      (viene copiato anche il puntatore al vettore delle righe
       ma non dve essere usato perche appartiene all'oggetto, per cui 
       dopo alloco memoria per questo vettore di righe)
   
   memcpy( &xlPixvarClassRec.xlpixvar_class.programs[num_program], 
           cw->pixvar.program, sizeof(XlPixvarAnimaz));
   */

   strcpy(xlPixvarClassRec.xlpixvar_class.programs[num_program].nome_program,
          cw->pixvar.program->nome_program);
   xlPixvarClassRec.xlpixvar_class.programs[num_program].nrow = 
          cw->pixvar.program->nrow;
 
   nrow = xlPixvarClassRec.xlpixvar_class.programs[num_program].nrow;
   /* alloco memoria per contenere le righe del program nella cache
   */
   if( (xlPixvarClassRec.xlpixvar_class.programs[num_program].row =
         (XlPixvarAnimazRow *)XtCalloc( nrow,sizeof(XlPixvarAnimazRow)))== NULL)
   {
      fprintf(stderr,"XlPixvar: Error in malloc caching row\n");
      return(1);
   }

   /* copio i dati relativi alle righe 
   */
   memcpy( xlPixvarClassRec.xlpixvar_class.programs[num_program].row, 
           cw->pixvar.program->row, sizeof(XlPixvarAnimazRow)*nrow);

   /* a sua volta nelle righe c'e' un vettore di valori
      per cui devo allocare e copiare                
   */

   /* alloco memoria per contenere le righe del program nella cache
   */
   for(i=0;i<nrow;i++)
   {
      nval = cw->pixvar.program->row[i].nval;
      if( (xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].val=
         (int *)XtCalloc(nval,sizeof(int)))==NULL)
      {
         fprintf(stderr,"XlPixvar: Error in malloc caching values\n");
         return(1);
      }

      memcpy( &xlPixvarClassRec.xlpixvar_class.programs[num_program].row[i].val, 
           &cw->pixvar.program->row[i].val, sizeof(int)*nval);

   }

   /* incrementoil contatore della cache
   */
   xlPixvarClassRec.xlpixvar_class.num_program++;

   return(0);
}



/* routine di lettura file program .prg
   contenenti le istruzioni di animazione di un pixvar
   Gestisce anche la cache dei program

     formato del file da leggere .prx

@x individua il bitmap che deve essere visualizzato per quel valore

      num_prog nrighe
      prog @1 nval ival1 ival2 ival3 ... ivaln #color1 fblink_freq
      prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
      prog @3 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
      prog @4 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq

   NOTA: il vettore restituito e' terminato con un idvar = -1; 

*/



/***********************************************************************
 * leggi_pict_program
 * legge il file program (.prg) e ne memorizza i dati nella struttura 
 *
 *   formato del file da leggere .prg
 *
 *    num_prog nrighe
 *    prog @1 nval ival1 ival2 ival3 ... ivaln #color1 fblink_freq
 *    prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *    prog @3 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *    prog @4 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *
 * - NB - viene anche associato lo SCREEN per cui questo program e' 
 *        stato caricato
 *  NOTA: il vettore restituito e' terminato con un idvar = -1; 
 ******************************************************+*****************/

static int leggi_pict_program(Widget w,char *nome_pict,XlPixvarAnimaz *pict)
{
   unsigned short dummy;
   static char *icon_dir=NULL;
   char nome_file[255];
   char tok[20],idvar[5],colore[20];
   int nrighe,ret,i,j;
   FILE *fp;
   int loaded;

   if(icon_dir==NULL)
   {
      icon_dir=getenv("LEGOMMI_ICO");
      if(icon_dir==NULL)
         return(1);
   }

   /* il nome del file e' dato dalla risorsa nomeProgram
      con estensione .prg
   */
   sprintf(nome_file,"%s/%s.prx",icon_dir,nome_pict);
   if( !strlen(nome_file) )
   {
      fprintf(stderr,"Error in filename pixvar animation file: %s\n",nome_file);
      return(1);
   }

   /* apertura del file
   */
   if( (fp = fopen(nome_file,"r")) == NULL)
   {
      fprintf(stderr,"Error opening pixvar animation file: %s\n",nome_file);
      return(1);
   }

   /* lettura prima riga (num_prog 5)
   */
   if( !fscanf(fp,"%s %d",tok,&nrighe) )
   {
      fprintf(stderr,"Error reading pixvar animation file: %s\n",nome_file);
      fclose(fp);
      return(1);
   }
   
   /* verifica del token
   */
   if( strcmp(tok,"num_prog" ) )
   {
     fprintf(stderr,"Error in format pixvar animation file: %s\n",nome_file);
      fclose(fp);
     return(1);
   }

   if( !nrighe )
   {
     fprintf(stderr,"Error in format pixvar animation file: %s\n",nome_file);
      fclose(fp);
     return(1);
   }


   /********************************
       - NB -
      assegno il nome e lo SCREEN
   ********************************/

   strcpy(pict->nome_program,nome_pict);
   pict->scr = XtScreen(w);

   /* assegno il numero di righe
   */
   pict->nrow = nrighe;

   /* alloco memoria per le righe dei valori
      una in piu' perche chiudo il vettore con -1
   */
   if( ((pict->row)=(XlPixvarAnimazRow *)XtCalloc(nrighe+1,
                               sizeof(XlPixvarAnimazRow)))==NULL)
   {
      fprintf(stderr,"Error in malloc XlPixvarAnimaz\n");
      fclose(fp);
      return(1);
   }


   /* leggo ogni riga del file
   */
   for(i=0;i<nrighe;i++)
   {
      ret=fscanf(fp,"%s %s %d",tok,idvar,&pict->row[i].nval);
      if( !ret || (ret== EOF))
      {
         fprintf(stderr,"Error reading pixvar animation file: %s line: %d\n",
                             nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* il token deve essere 'prog'
      */
      if( strcmp(tok,"prog") )
      {
         fprintf(stderr,"Error reading pixvar animation file: %s line: %d\n",
                             nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* indice pixmap associato 1,2,3,4
      */
      pict->row[i].idvar = atoi(&idvar[1]);

      /* se ci sono valori specificati  li alloco e li leggo
         altrimenti leggo solo il colore
      */
      if( pict->row[i].nval )
      {
         if((pict->row[i].val=(int *)XtCalloc(pict->row[i].nval,sizeof(int)))==NULL)
         {
            fprintf(stderr,"Error in malloc value\n");
            fclose(fp);
            return(1);
         }

         /* leggo la lista di valori
         */
         for(j=0;j<pict->row[i].nval;j++)
         {
            ret=fscanf(fp,"%d",&pict->row[i].val[j]) ;
            if( !ret || (ret== EOF))
            {
               fprintf(stderr,"Error reading pixvar animation file: %s line: %d\n",
                             nome_file,i+1);
               fclose(fp);
               return(1);
            }

         }
      }

      /* leggo il colore e la blink freq e trasparenza
      */
      ret = fscanf(fp,"%s %f %d",colore,&pict->row[i].animaz.blink_rate,
                             &pict->row[i].animaz.trasparent) ;
      if( !ret || (ret== EOF))
      {
         fprintf(stderr,"Error reading pixvar animation file: %s line: %d\n",
                           nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* converto il colore in pixel
      */
      XlCvtStrToPixel(XtDisplay(w),colore,&pict->row[i].animaz.colore,&dummy,
                      &dummy,&dummy);

   }

   /***************************************
     termino il vettore con -1 in idvar
     ATT.NE e' importantissimo che il vettore
     sia terminato con -1 sempre perche' 
     le routine di conversione XlValueToBlinkPixel etc
     fanno uno while(idvar != -1)
   *****************************************/


   pict->row[i].idvar = -1;


   fclose(fp);
   return(0);
}

/*************************************************
  XlIsPixvar restituisce True se lo widget
   e' della classe XlCPixvar
**************************************************/
Boolean XlIsPixvar( Widget w )
{
   CoreWidgetClass wclass;

   if( w != NULL && XtIsWidget(w) )
   {
      wclass=XtClass(w);

      if( strcmp(wclass->core_class.class_name,XlCPixvar)==0)
         return(True);
      else
         return(False);
   }
   else
     return(False);
}


static Boolean LoadFont( w,fontname,modo)
Widget w;
char fontname[];
Boolean modo;
{
   XlPixvarWidget cw = (XlPixvarWidget) w;
   static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

   /* Carica il font ottenendo la descrizione del font stesso 
   */

   char **pf;
   int i,nf;
   Display *disp;

   disp=XlDisplay(w);

   /* Caso di stampa */
   if(modo)
   {
        if((cw->pixvar.font_info = XLoadQueryFont(disp,fontname)) == NULL)
           return(False);
   }
   else /* caso di display su video */
      if((cw->pixvar.font_info = XlGetFont(disp,fontname)) == NULL)
                return(False);
                
   return(True);
}


static void LoadNullBitmap(w)
Widget w;
{
   XlPixvarWidget cw= (XlPixvarWidget)w;
   Arg args[2];
   int screen_num=DefaultScreen(XtDisplay(w));
   Display *disp;
   int indice;

   disp = XtDisplay(w);

   XlWarning("XlPixvar","LoadNullBitmap", "XlPixvar: Default Null bitmap loaded");

   for( indice = 1; indice < 5 ; indice ++)
   {
      switch( indice )
      {
      case 1:
         cw->pixvar.pix1 = XCreateBitmapFromData(disp,
                                                 RootWindow( disp, DefaultScreen(disp)),
                                                 nullo_bits,nullo_width,nullo_height);
      break;
      case 2:
         cw->pixvar.pix2 = XCreateBitmapFromData(disp,
                                                 RootWindow( disp, DefaultScreen(disp)),
                                                 nullo_bits,nullo_width,nullo_height);
      break;
      case 3:
         cw->pixvar.pix3 = XCreateBitmapFromData(disp,
                                                 RootWindow( disp, DefaultScreen(disp)),
                                                 nullo_bits,nullo_width,nullo_height);
      break;
      case 4:
         cw->pixvar.pix4 = XCreateBitmapFromData(disp,
                                                 RootWindow( disp, DefaultScreen(disp)),
                                                 nullo_bits,nullo_width,nullo_height);
      break;
      }


      XtSetArg(args[0],XtNwidth,nullo_width );
      XtSetArg(args[1],XtNheight,nullo_height);
      XtSetValues(w,args,2);
   }
}
