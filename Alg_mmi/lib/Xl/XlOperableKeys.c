/**********************************************************************
*
*       C Source:               XlOperableKeys.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:41:33 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlOperableKeys.c-6.1.3 %  (%full_filespec: XlOperableKeys.c-6.1.3:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlOperableKeys.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlOperableKeys.c - widget operable keys
 *  questo widget simula i bottoni operable keys di teleperm
 */
#include <stdio.h>
#include <string.h>
#include <Xm/XmP.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <X11/StringDefs.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlOperableKeysP.h>
#include <Ol/OlPert.h>
#include "nullo.bmp"
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNnormFg,
        XlCNormFg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlOperableKeysRec,operableKeys.norm_fg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNhstateBg,
        XlCHstateBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlOperableKeysRec,operableKeys.hstate_bg),
        XmRString,
        XtDefaultForeground
        },
	{
        XlNlstateBg,
        XlCLstateBg,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlOperableKeysRec,operableKeys.lstate_bg),
        XmRString,
        XtDefaultBackground
        },
	{
        XlNnormalFont,
        XlCNormalFont,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.normalfont),
        XmRImmediate,
        "fixed"
        },
        {
        XlNoperableKeysType,
        XlCOperableKeysType,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlOperableKeysRec,operableKeys.tipo),
        XmRString,
        (XtPointer)DEFAULT_TIPO
        },
        {
        XlNoperableKeysDraw,
        XlCOperableKeysDraw,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlOperableKeysRec,operableKeys.draw),
        XmRString,
        (XtPointer)DEFAULT_DRAW
        },
        {
        XlNoperableKeysDrawPush,
        XlCOperableKeysDrawPush,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlOperableKeysRec,operableKeys.drawPush),
        XmRString,
        (XtPointer)DEFAULT_DRAWPUSH
        },
        {
        XlNfillButton,
        XlCFillButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlOperableKeysRec,operableKeys.fill_button),
        XmRString,
        (XtPointer)DEFAULT_FILL
        },
        {
        XlNlabel,
        XlCLabel,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.label),
        XmRImmediate,
        ""
        },
        {
        XlNbuttonLabel,
        XlCButtonLabel,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.button_label),
        XmRImmediate,
        ""
        },
        {
        XlNnameBmp,
        XlCNameBmp,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.name_bmp),
        XmRImmediate,
        ""
        },
        {
        XlNvarInput,
        XlCVarInput,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.str_var_input),
        XmRImmediate,
        ""
        },
        {
        XlNvarOutput,
        XlCVarOutput,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.str_var_output),
        XmRImmediate,
        ""
        },
        {
        XlNvarOutput2,
        XlCVarOutput2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlOperableKeysRec,operableKeys.str_var_output2),
        XmRImmediate,
        ""
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
	{
        XlNnormFg,
        XlCNormFg,
        XlDnormFg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlOperableKeysRec,operableKeys.norm_fg),
        sizeof(Pixel),
        },
	{
        XlNhstateBg,
        XlCHstateBg,
        XlDhstateBg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlOperableKeysRec,operableKeys.hstate_bg),
        sizeof(Pixel),
        },
 	{
        XlNlstateBg,
        XlCLstateBg,
        XlDlstateBg,
        XlRColor,
        NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlOperableKeysRec,operableKeys.lstate_bg),
        sizeof(Pixel),
        },
        {
        XlNnormalFont,
        XlCNormalFont,
        XlDnormalFont,
        XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.normalfont),
        sizeof(XFontStruct*)
        },
        {
        XlNoperableKeysType,
        XlCOperableKeysType,
        XlDoperableKeysType,
        XlROption,
        NULL,
        XlOoperableKeysType,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.tipo),
        sizeof(int)
        },
        {
        XlNoperableKeysDraw,
        XlCOperableKeysDraw,
        XlDoperableKeysDraw,
        XlROption,
        NULL,
        XlOoperableKeysDraw,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.draw),
        sizeof(int)
        },
        {
        XlNoperableKeysDrawPush,
        XlCOperableKeysDrawPush,
        XlDoperableKeysDrawPush,
        XlROption,
        NULL,
        XlOoperableKeysDrawPush,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.drawPush),
        sizeof(int)
        },
        {
        XlNlabel,
        XlCLabel,
        XlDlabel,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.label),
        sizeof(char*),
        },
        {
        XlNbuttonLabel,
        XlCButtonLabel,
        XlDbuttonLabel,
        XlRText,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.button_label),
        sizeof(char*),
        },
        {
        XlNnameBmp,
        XlCNameBmp,
        XlDnameBmp,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.name_bmp),
        sizeof(char*),
        },
        {
        XlNfillButton,
        XlCFillButton,
        XlDfillButton,
        XlROption,
        NULL,
        XlOfillButton,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.fill_button),
        sizeof(int)
        },
        {
        XlNvarInput,
        XlCVarInput,
        XlDvarInput,
        XlRTipoVarInp,
        XlRVarDX, /* XlRVarDA, */
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.var_input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNvarOutput,
        XlCVarOutput,
        XlDvarOutput,
        XlRTipoVarOut,
        XlRVarXO, /* XlRVarDO, */
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.var_output),
        sizeof(PUNT_VAROUT)
        },
        {
        XlNvarOutput2,
        XlCVarOutput2,
        XlDvarOutput2,
        XlRTipoVarOut,
        XlRVarXO, /* XlRVarDO, */
        NULL,
        XlRGrIo,
        XtOffsetOf(XlOperableKeysRec,operableKeys.var_output2),
        sizeof(PUNT_VAROUT)
        },
        };

/* dichiarazioni funzioni varie */
static void GetNormFgGC();
static void GethstateGC();
static void GetlstateGC();
static void GetClearGC();
static void GetAllGCs();
static Boolean LoadFont();
static void ActivateToggle();
static Boolean LoadBitmap();
static void LoadNullBitmap();
static void add_translation();
static void CreaFigli();
static void DistruggiFigli();
static void SettaActionsFigli();

static int LoadStateBitmap();
static void switch_bitmap();

         
/* dichiarazione dei metodi (methods) */
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();
#if defined XPRINTER_USED
static Boolean Print();
#endif


/* actions del widget XlOperableKeys */
static void Seleziona();
static void AddSelez();
static void DoLayout();

/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

static char textTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char buttonTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";


/* Inizializzazione del class record */
XlOperableKeysClassRec xlOperableKeysClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "OperableKeys",
    /* widget_size              */      sizeof(XlOperableKeysRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      actions,
    /* num_actions              */      XtNumber(actions),
    /* resources                */      resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      TRUE,
    /* compress_exposure        */      XtExposeCompressMultiple, /* TRUE*/
    /* compress_enterleave      */      TRUE,
    /* visible_interest         */      FALSE,
    /* destroy                  */      Destroy,
    /* resize                   */      Resize,    /* XtInheritResize, */
    /* expose                   */      Redisplay, /*XtInheritExpose */ 
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
  { /* Constraint class fields   */
    /* geometry_manager          */ 	GeometryManager, /*XtInheritGeometryManager,  */
    /* change_managed            */     XtInheritChangeManaged,
    /* insert_child              */     XtInheritInsertChild,
    /* delete_child              */     XtInheritDeleteChild,
    /* extension                 */     NULL
  },
  { /* legomanager fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia */    NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* drawBorderIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* operableKeys fields */
    /* empty                    */      0
  }
};

WidgetClass xlOperableKeysWidgetClass = (WidgetClass) &xlOperableKeysClassRec;


/*************************************
 *         geometry manager          *
 *************************************/

static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
   return( XtGeometryYes );
}

/*************************************
 *         metodo di resize          *
 *************************************/
static void Resize(w)
Widget w;
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;
   
   return;
}



/*************************************
 *         GC di clear               *
 *************************************/

static void GetClearGC(w)
Widget w;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                               | GCLineStyle;
   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   values.foreground = cw->core.background_pixel;	
   values.background = cw->operableKeys.norm_fg;
   values.line_width = 0;
   values.line_style = LineSolid;

   cw->operableKeys.clear_gc = XtGetGC(cw, valuemask, &values);
}

static void GetlstateGC(w)
Widget w;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                               | GCLineStyle;
   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   values.foreground = cw->operableKeys.norm_fg;
   values.background = cw->operableKeys.lstate_bg;
   values.line_width = 0;
   values.line_style = LineSolid;

   cw->operableKeys.lstate_gc = XtGetGC(cw, valuemask, &values);
}

static void GethstateGC(w)
Widget w;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                               | GCLineStyle;
   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   values.foreground = cw->operableKeys.norm_fg;
   values.background = cw->operableKeys.hstate_bg;
   values.line_width = 0;
   values.line_style = LineSolid;

   cw->operableKeys.hstate_gc = XtGetGC(cw, valuemask, &values);
}

/*************************************
 *         get all GC                *
 *************************************/

static void GetAllGCs(w)
Widget w;
{
   GethstateGC(w);
   GetlstateGC(w);
   GetClearGC(w);
}


/*************************************
 *         metodo di Initialize      *
 *************************************/

static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
   XlOperableKeysWidget new = (XlOperableKeysWidget)tnew;

   /* carico i font
   */
   if( !LoadFont(new,new->operableKeys.normalfont) )
        if( !LoadFont(new,"fixed") )
                XlError("XlOperableKeys","Initialize",
		        "Impossibile caricare default font");

   /* setto il tipo di fill
   if( new->operableKeys.fill_button==DEFAULT_FILL )
	 new->operableKeys.lstate_bg=new->core.background_pixel;
   */
	
   /* carico i GC da usare (quello di clear)
   */
   GetAllGCs( new );

   new->operableKeys.text=NULL;
   new->operableKeys.bottone=NULL;
   new->operableKeys.bitmap_norm=NULL;
   new->operableKeys.bitmap_lstate=NULL;
   new->operableKeys.bitmap_hstate=NULL;

   /* disabilito i tasti di tipo impulse      
      cosi che non possono inviare perturbazioni
   */
   new->operableKeys.impAble=False;

   CreaFigli( new );
   SettaActionsFigli( new );


}


#define superclass (&xlManagerClassRec)

/*************************************
 *  routine di Selezione oggetto     *
 *************************************/

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* Richiama il metodo di select
   */
   if((strcmp(XtName(w),"__text")==0)||(strcmp(XtName(w),"__bottone")==0))
        (*superclass->xlmanager_class.select) (XtParent(w),0);
   else
        (*superclass->xlmanager_class.select) (w,0);
}

/*************************************
 *  routine di Selezione multipla    *
 *************************************/

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* richiama il metodo di select 
   */
   if((strcmp(XtName(w),"__text")==0)||(strcmp(XtName(w),"__bottone")==0))
        (*superclass->xlmanager_class.select) (XtParent(w),1);
   else
        (*superclass->xlmanager_class.select) (w,1);
}

/*************************************
 *       metodo di SeltValues        *
 *************************************/

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
   XlOperableKeysWidget curcw= (XlOperableKeysWidget) current;
   XlOperableKeysWidget newcw= (XlOperableKeysWidget) new;
   Arg comp_args[2],argum[3];
   Boolean do_redisplay = False;
   XmString str;                   /* stringa per label bottone */


   /* set colore foreground
   */
   if(curcw->operableKeys.norm_fg!=newcw->operableKeys.norm_fg)
   {
      XtReleaseGC(curcw,curcw->operableKeys.hstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.lstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.clear_gc);
      GetAllGCs(newcw);
      XtSetArg(comp_args[0],XmNforeground,newcw->operableKeys.norm_fg);
      XtSetValues(newcw,comp_args,1);
      do_redisplay = True;
   }

   /* set colore di background con stato1 variabile di controllo
   */
   if(curcw->operableKeys.hstate_bg!=newcw->operableKeys.hstate_bg)
   {
      XtReleaseGC(curcw,curcw->operableKeys.hstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.lstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.clear_gc);
      GetAllGCs(newcw);
      XtSetArg(comp_args[0],XmNbackground,newcw->operableKeys.hstate_bg);
      XtSetValues(newcw,comp_args,1);
      do_redisplay = True;
   }

   /* set colore di background con stato0 variabile di controllo
   */
   if(curcw->operableKeys.lstate_bg!=newcw->operableKeys.lstate_bg)
   {
      XtReleaseGC(curcw,curcw->operableKeys.hstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.lstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.clear_gc);
      GetAllGCs(newcw);
      XtSetArg(comp_args[0],XmNbackground,newcw->operableKeys.lstate_bg);
      XtSetValues(newcw,comp_args,1);
      do_redisplay = True;
   }

   /* set fill del bottone
   if(curcw->operableKeys.fill_button!=newcw->operableKeys.fill_button)
   {
      XtReleaseGC(curcw,curcw->operableKeys.hstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.lstate_gc);
      XtReleaseGC(curcw,curcw->operableKeys.clear_gc);
      GetAllGCs(newcw);
      if(newcw->operableKeys.fill_button==DEFAULT_FILL)
             newcw->operableKeys.lstate_bg=newcw->core.background_pixel;
      XtReleaseGC(curcw,curcw->operableKeys.clear_gc);
      do_redisplay = True;
   }
   */

   /* distinzione del metodo se in config o run_time
   */
   if(curcw->xlmanager.config!=newcw->xlmanager.config)
   {
      if(newcw->xlmanager.config==True)
          SettaActionsFigli(newcw);
      else
      {
          DistruggiFigli(newcw);
          CreaFigli(newcw);
      }
      do_redisplay = True;
   }

   /* se draw=DRAW_BITMAP set del bitmap
   */
   if(newcw->operableKeys.draw==DRAW_BITMAP)  
      if(strcmp(curcw->operableKeys.name_bmp,newcw->operableKeys.name_bmp))
      {
         /* Libero la pixmap del vecchio widget e rialloco la nuova
         */
         if( curcw->operableKeys.bitmap_norm != NULL)
           XFreePixmap(XtDisplay(current),newcw->operableKeys.bitmap_norm);

         if( LoadBitmap(newcw) == False )
	    LoadNullBitmap(new);

         XtSetArg(argum[0],XtNwidth,newcw->operableKeys.button_width);     
	 XtSetArg(argum[1],XtNheight,newcw->operableKeys.button_height);
	 XtSetArg(argum[2],XmNlabelPixmap,(newcw->operableKeys.bitmap_norm));
	 XtSetValues(newcw,argum,3);
         do_redisplay = True;
      }

   /* se draw=DRAW_LABEL set della stringa 
   */
   if(newcw->operableKeys.draw==DRAW_LABEL)  
      if(strcmp(curcw->operableKeys.button_label,newcw->operableKeys.button_label))
      {
  	 str=XmStringCreateSimple(newcw->operableKeys.button_label);
	 XtSetArg(argum[0],XmNlabelString,str);		
	 XtSetValues(newcw,argum,1);
	 XmStringFree(str);
         do_redisplay = True;
      }

   /* se del font
   */
   if(strcmp(curcw->operableKeys.normalfont,newcw->operableKeys.normalfont))
   {
      if(!LoadFont(newcw,newcw->operableKeys.normalfont))
         if(!LoadFont(newcw,"fixed"))
              XlError("XlOperableKeys","SetValues","Impossibile caricare default font");
        do_redisplay = True;
   }

   /* set della width
   */
   if(curcw->core.width != newcw->core.width)
	newcw->core.width=curcw->core.width;

   /* risorsa height
   */
   if(curcw->core.height != newcw->core.height)
	newcw->core.height=curcw->core.height;

   /* risorsa width0
   */
   if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
	newcw->xlmanager.width0 = curcw->xlmanager.width0;

   /* risorsa height0
   */
   if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
	newcw->xlmanager.height0 = curcw->xlmanager.height0;

   return do_redisplay;

}

/*************************************
 *       metodo di Destroy           *
 *************************************/
static void Destroy(w)
Widget w;
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   /* Rilascio i GC
   */
   if(cw->operableKeys.clear_gc)
        XtReleaseGC(cw,cw->operableKeys.clear_gc);

   if(cw->operableKeys.hstate_gc)
      XtReleaseGC(cw,cw->operableKeys.hstate_gc);

   if(cw->operableKeys.lstate_gc)
      XtReleaseGC(cw,cw->operableKeys.lstate_gc);

   /* libero la font_struct 
   */
   if(cw->operableKeys.font_info)
        XFreeFont(XtDisplay(cw),cw->operableKeys.font_info);

   /* libero la font_list  
   */
   if(cw->operableKeys.font_list)
        XmFontListFree(cw->operableKeys.font_list);

   /* libero la Pixmap 
   */
   if(cw->operableKeys.bitmap_norm)
        XFreePixmap(XtDisplay(cw),cw->operableKeys.bitmap_norm);

   if(cw->operableKeys.bitmap_hstate)
        XFreePixmap(XtDisplay(cw),cw->operableKeys.bitmap_hstate);

   if(cw->operableKeys.bitmap_lstate)
        XFreePixmap(XtDisplay(cw),cw->operableKeys.bitmap_lstate);

}

/*********************************************
 * switch bitmap
 *
 * effettua lo switch tra i bitmap relativi
 * allo stato 0 e 1 della variabile
 * in base al valore della variabile di input
 * Se il bottone e' di tipo OpKeyImp e 
 * se il bottone IMPULSE non e' stato premuto
 * lo stato dell'OpKey deve risultare disabled
 * indipendentemente dallo stato della variabile
 * di controllo.
 **********************************************/

static void switch_bitmap(Widget w)
{

   extern Boolean OWImpKeyGetState();

   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   /* variabile di stato a 1 abilito i bottoni
      Se e' un OPKEY_IMPULSE e IMPULSE non e' premuto
      disabilito comunque il bottone
   */
   if( cw->operableKeys.var_input.valore == 1 ) 
   {
      if((cw->operableKeys.tipo == OPKEY_IMPULSE) && !cw->operableKeys.impAble )
      {
         XtVaSetValues(cw->operableKeys.bottone,
                    XmNlabelPixmap,
                    cw->operableKeys.bitmap_lstate,
                    XmNbackground,
                    cw->operableKeys.lstate_bg,
                    NULL);
         return;
      }

      XtVaSetValues(cw->operableKeys.bottone,
                    XmNlabelPixmap,
                    cw->operableKeys.bitmap_hstate,
                    XmNbackground,
                    cw->operableKeys.hstate_bg,
                    NULL);
   }
   else
      XtVaSetValues(cw->operableKeys.bottone,
                    XmNlabelPixmap,
                    cw->operableKeys.bitmap_lstate,
                    XmNbackground,
                    cw->operableKeys.lstate_bg,
                    NULL);
}

/*************************************
 *       metodo di Refresh           *
 *************************************/
static Boolean Refresh(Widget w)
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget) w;

   if(cw->xlmanager.config==True)
        return(True);

   if(!XtIsWidget(w))
   {
        XlWarning("XlOperableKeys","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
   }

   if(xlOperableKeysClassRec.xlmanager_class.database==NULL)
   {
        XlWarning("XlOperableKeys","Refresh","database non definito");
        return(False);
   }

   /* disegno i bitmap con colore di sfondo
      relativo allo stato della variabile di input
   */
   switch_bitmap( w );


   if(!(*superclass->xlmanager_class.refresh) (w))
   {
        XlWarning("XlSetVAlore","Refresh","errore refresh classe superiore");
        return(False);
   }

   return(True);
}

static Boolean Compile(Widget w)
{
   int i;

   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;

   if(!XtIsWidget(w))
   {
        XlWarning("XlManager","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
   }

   if(xlOperableKeysClassRec.xlmanager_class.compilatore==NULL)
   {
       XlErrComp(w,"Compile","Compiler not defined",NULL);
       /* Evidenzia graficamente sull'oggetto la presenza/assenza di errore
       */
       XlEvError(cw,True);
       return(False);
   }


   /* le variabili di input e almeno una di output devono essere configurate
   */
   if( strlen(cw->operableKeys.str_var_input) == 0)
   {
       XlErrComp(w,"Compile","Input variable not defined",NULL);
       XlEvError(cw,True);
       return(False);
   }

   if( strlen(cw->operableKeys.str_var_output) == 0)
   {
      if( strlen(cw->operableKeys.str_var_output2) == 0)
      {
          XlErrComp(w,"Compile","Output variable not defined",NULL);
          XlEvError(cw,True);
          return(False);
      }
   }


   if(!(*superclass->xlmanager_class.compile) (w,
        xlOperableKeysClassRec.xlmanager_class.compilatore))
   {
       XlErrComp(w,"Compile","Error in upper-class",NULL);
       XlEvError(cw,True);
       return(False);
   }

   return(True);
}


static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;
   register int x,y;
   unsigned int width,height;
   XmDrawingAreaCallbackStruct str;
   Arg args[3];

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

   (*superclass->core_class.expose) (w,event,NULL);
   XClearArea(XtDisplay(cw),XtWindow(cw),0,0,0,0,False);

   XClearArea(XtDisplay(cw),XtWindow(cw->operableKeys.bottone),0,0,0,0,True);

   if(cw->operableKeys.text!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->operableKeys.text),0,0,0,0,True);


   /* Richiamo del metodo di disegno del bordo se l'oggetto e' selezionato
   */
   (*superclass->xlmanager_class.drawBorderIfSelected) (w);

}

static Boolean LoadFont( cw,fontname)
Widget cw;
char fontname[];
{
   XlOperableKeysWidget Xl_cw = (XlOperableKeysWidget) cw;
   static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

   /* Carica il font ottenendo la descrizione del font stesso */

   char **pf;
   int i,nf;

   if((Xl_cw->operableKeys.font_info = XLoadQueryFont(XtDisplay(cw),fontname)) == NULL)
   {
        XlWarning("XlOperableKeys","LoadFont","Cannot open font");
        return(False);
   }
   else
   {
        Xl_cw->operableKeys.font_list =
                XmFontListCreate ( Xl_cw->operableKeys.font_info, charset);
        if (Xl_cw->operableKeys.font_list == NULL)
        {
                XlWarning("XlOperableKeys","LoadFont","FONTLIST NULL");
                return(False);
        }
   }

   return(True);

}

static void ActivateToggle(w,info,str)
Widget w;
XtPointer info,str;
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget)XtParent(w);
   XlManagerClassRec * wclass; /* puntatore alla widget class del widget */

   /* Chiama la procedura selectRunTime, se e' stata settata
   N.B.:
   ----
   La select run time che interessa il operableKeys e' settata dall' mmi 
   nel file csrc other_mmi.c .In SettaSelect viene settata con XlSetSelectRunTime
   la funzione SelectRunTime come funzione-risorsa di XloperableKeys.
   In tale SelectRunTime viene fatto lo switch sul operableKeys.tipo per 
   lanciare la funzione corretta.
   */

   (*superclass->xlmanager_class.select) (cw,0);


}


static Boolean LoadBitmap(w)
Widget w;
{
   /*
   Procedura per la lettura da file del bitmap che si vuole caricare.
   Se il file contenente il bitmap cercato non esiste o non si riesce a
   caricare il bitmap per qualunque altro motivo viene "ritornato" un valore
   di fallimento che permettera' l' invocazione di LoadNullBitmap per 
   caricamento bitmap di default
   */

   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;
   int xh,yh;
   int ret;		/*valore di ritorno*/
   Arg args[2];
   char path_bitmap[1024];
   char *disegni_dir;
   FILE *fp;
   char _nome_file_bitmap[MAXLUNGBMP];
   Pixmap pix;	/*pixmap di appoggio*/


   /* concateno al nome del file l'estensione
   */

   sprintf(_nome_file_bitmap,"%s.bmp",cw->operableKeys.name_bmp);


   /* verifico che cw->operableKeys.name_bmp contenga qualche cosa
   */
   if(cw->operableKeys.name_bmp==NULL)
   {
        XlWarning("XlBitmap","LoadBitmap",
                "nome del file non definito");
        LoadNullBitmap(cw);
        return(False);
   }

   /* verifico se il file si trova nella directory corrente
   */
   if((fp=fopen(_nome_file_bitmap,"r"))!=NULL)
   {
        fclose(fp);
        strcpy(path_bitmap,_nome_file_bitmap);
   }
   else
   {
	disegni_dir=getenv("LEGOMMI_ICO");	/*trovo la dir delle bitmap*/
	if(disegni_dir==NULL)
        {
                XlWarning("XlOperableKeys","LoadBitmap",
                        "impossibile leggere la variabili LEGOMMI_ICO");
                LoadNullBitmap(cw);
                return(False);
        }
                
	strcpy(path_bitmap,disegni_dir);
        strcat(path_bitmap,"/");
        strcat(path_bitmap,_nome_file_bitmap);
        if((fp=fopen(path_bitmap,"r"))!=NULL)
            fclose(fp);
        else
        {
                XlWarning("XlOperableKeys","LoadBitmap",
                        "impossibile aprire il file in LEGOMMI_ICO");
        	LoadNullBitmap(cw);
        	return(False);
        }
		
   }

   /* Leggo dapprima il file bitmap e ricavo le dimensioni del bitmap
      messe in button_width/heighti. Il bitmap e' posto nel pixmap di appoggio pix
   */

  ret=XReadBitmapFile(XtDisplay(w),
                RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
                path_bitmap,&(cw->operableKeys.button_width),
                &(cw->operableKeys.button_height),
                &pix,&xh,&yh);

  if(ret==BitmapSuccess)
	printf("Bitmap caricato correttamente\n");
   else if(ret==BitmapOpenFailed)
        printf("Apertura fallita\n");
   else if(ret==BitmapFileInvalid)
        printf("File bitmap non valido \n");
   else if(ret==BitmapNoMemory)
        printf("No memory\n");
   if(ret!=BitmapSuccess)
   {
	XlWarning("XloperableKeys","LoadBitmap",
                "Impossibile leggere il file con XReadBitmap");
	return(False);
   }

   /* Ora creo il pixmap che verra' poi importato dal bottone.
   Viene messo nella risorsa bitmap_norm
   */
   cw->operableKeys.bitmap_norm=XCreatePixmap(XtDisplay(w), 
			RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
			cw->operableKeys.button_width,
			cw->operableKeys.button_height,
			DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w))));

   /* Copio il pix ID (letta da file) nella risorsa bitmap_norm
   */
   XCopyPlane(XtDisplay(w),pix,cw->operableKeys.bitmap_norm,
        cw->operableKeys.hstate_gc,0,0,
	cw->operableKeys.button_width,cw->operableKeys.button_height,
	0, 0, 1);

   cw->operableKeys.bitmap_hstate=XCreatePixmap(XtDisplay(w), 
			RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
			cw->operableKeys.button_width,
			cw->operableKeys.button_height,
			DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w))));

   XCopyPlane(XtDisplay(w),pix,cw->operableKeys.bitmap_hstate,
        cw->operableKeys.hstate_gc,0,0,
	cw->operableKeys.button_width,cw->operableKeys.button_height,
	0, 0, 1);

   cw->operableKeys.bitmap_lstate=XCreatePixmap(XtDisplay(w), 
			RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
			cw->operableKeys.button_width,
			cw->operableKeys.button_height,
			DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w))));

   XCopyPlane(XtDisplay(w),pix,cw->operableKeys.bitmap_lstate,
        cw->operableKeys.lstate_gc,0,0,
	cw->operableKeys.button_width,cw->operableKeys.button_height,
	0, 0, 1);

   /* Libero il pixmap di appoggio
   */
   XFreePixmap(XtDisplay(w),pix);

return(True);
}

static void LoadNullBitmap(w)
Widget w;
{
   /* Funzione che carica comunque un bitmap di default (nullo.bmp) 
      importato in una include
   */
   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;
   Arg args[2];
   char *bmpNameBits;
   unsigned int bmpNameWidth,bmpNameHeight;

   XlWarning("XloperableKeys","LoadNullBitmap",
                "utilizzo i bitmaps di default");

   /* Carico il bitmap di default coerente con il tipo di bottone
   */
   bmpNameBits=nullo_bits;
   bmpNameWidth=nullo_width;
   bmpNameHeight=nullo_height;

   /* uso XCreateBitmapFromData perche' il bitmap e' gia' stato caricato come
      var globale includendo con include il .bmp
   */
   if((cw->operableKeys.bitmap_norm=XCreatePixmapFromBitmapData(XtDisplay(w),
		 RootWindow(XtDisplay(w),XScreenNumberOfScreen(XtScreen(w))),
		bmpNameBits,
                bmpNameWidth,bmpNameHeight,
                cw->operableKeys.norm_fg,cw->operableKeys.hstate_bg,
                DefaultDepth(XtDisplay(w),XScreenNumberOfScreen(XtScreen(w)))))== False)
   {
	XlWarning("XloperableKeys","LoadNullBitmap",
                "Errore nella creazione del pixmap");
        return;
   }

   cw->operableKeys.button_width=nullo_width;
   cw->operableKeys.button_height=nullo_height;

}

/****************************************************
 * loadHstateBitmap
 *
 * carica il bitmap associatao 
 * allo statpo 1 della variabile
 * di input
 * la XmGetPixmap oltre a permettere
 * di settare facilmente il fg,bg di un pixmap
 * ne effettua ancge la cache
 ****************************************************/

static int LoadStateBitmap(Widget w)
{
   XlOperableKeysWidget cw= (XlOperableKeysWidget)w;
   char *path_bmp;
   char nome_bmp[255];
   char nome_bmp1[255];
int numero_screen;


   /* costruisco e verifico la path per i bitmap
   */
   path_bmp = getenv("LEGOMMI_ICO");
   if( path_bmp == NULL )
   {
      XlWarning("XloperableKeys","LoadStateBitmap",
               "Invalid LEGOMMI_ICO bitmap path");
      return(False);
   }

   /* controllo il nome del bitmap 
   */
   if( strlen(cw->operableKeys.name_bmp) == 0 )
   {
      XlWarning("XloperableKeys","LoadStateBitmap",
                "Invalid bitmap name");
      return(False);
   }

   /* costruisco il nome del bitmap completo di path
   */

   sprintf(nome_bmp,"%s/%s.bmp",path_bmp,cw->operableKeys.name_bmp);


   /* carico il pixmap con colore hstate_bg
    *  (prelevandolo eventualmente dalla cache)
   */
   	cw->operableKeys.bitmap_hstate = XmGetPixmap(XtScreen(w),
                                                nome_bmp,
                                                cw->operableKeys.norm_fg,
                                                cw->operableKeys.hstate_bg);


      
   /* carico il pixmap con colore lstate_bg
    *  (prelevandolo eventualmente dalla cache)
   */
   	cw->operableKeys.bitmap_lstate = XmGetPixmap(XtScreen(w),
                                                nome_bmp,
                                                cw->operableKeys.norm_fg,
                                                cw->operableKeys.lstate_bg);


   if( cw->operableKeys.bitmap_hstate == XmUNSPECIFIED_PIXMAP )
   {
      XlWarning("XloperableKeys","Load hstate Bitmap",
                "Bitmap not found");
      return(False);
   }

   if( cw->operableKeys.bitmap_lstate == XmUNSPECIFIED_PIXMAP )
   {
        XlWarning("XloperableKeys","Load lstate Bitmap",
                "Bitmap not found");
           return(False);
   }

   return(True);
}

static void add_translation(Widget wid,char *stringa)
{
   XtTranslations tr;
   char *policy;

   /* verifico la consistenza dei parametri 
   */
   if (wid && stringa)
   {
       /* converto la stringa in XtTranslation 
       */
       tr = XtParseTranslationTable(stringa);
       policy = stringa;
       if (*policy == '#')
       {
           policy++;
           switch( *policy )
           {
               case 'r':
                 set_something_val(wid, XmNtranslations, (XtArgVal)tr);
               break;
               case 'a':
                  XtAugmentTranslations(wid, tr);
               break;
               case 'o':
               default:
                  XtOverrideTranslations(wid, tr);
               break;
           }
       }
       else
           XtOverrideTranslations(wid, tr);
   }
}

static void CreaFigli(tnew)
Widget tnew;
{
   XlOperableKeysWidget new = (XlOperableKeysWidget)tnew;
   int i,app_int;
   Arg arg[25];
   Dimension w_font,h_font,text_width,text_height;
   Dimension bottone_width,bottone_height;
   XmString str;

   h_font=new->operableKeys.font_info->ascent+new->operableKeys.font_info->descent;

   if(new->operableKeys.draw==DRAW_BITMAP)
   {
	new->operableKeys.button_width=DEFAULT_WIDTH;
	new->operableKeys.button_height=DEFAULT_HEIGHT;
	if(!LoadBitmap(new))
       	{
        	XlWarning("XlOperableKeys","Initialize",
			"Carico il bitmap di default");
                LoadNullBitmap(new);
	}

	/* Riempio ampiezza e altezza con quelle ricavate in LoadBitmap
	*/
	bottone_width=new->operableKeys.button_width;
	bottone_height=new->operableKeys.button_height;
   }
   else if(new->operableKeys.draw==DRAW_LABEL)  
   {
	if(!strcmp(new->operableKeys.button_label,""))
		str=XmStringCreateSimple("Button");
	else		
		str=XmStringCreateSimple(new->operableKeys.button_label);	
   }

   /* creazione widget bottone button
   */
   i=0;
   XtSetArg(arg[i],XmNx,3); i++;
   XtSetArg(arg[i],XmNy,3); i++;

   if(new->operableKeys.draw==DRAW_BITMAP) /*se carico bitmap*/
   {
	/* Setto le Dim del bottone in base a quellle del pixmap
	*/
	/* Setto ampiezza e altezza di core (come in ogni Initialize)
	in base alle dimensioni del pixmap e quindi del bottone
	*/
	new->core.width=bottone_width+6;
	new->core.height=bottone_height+6;
	XtSetArg(arg[i],XmNwidth,bottone_width); i++;
	XtSetArg(arg[i],XmNheight,bottone_height); i++;
	XtSetArg(arg[i],XmNspacing,0); i++;
	XtSetArg(arg[i],XmNlabelType,XmPIXMAP); i++;

	/* Carica pixmap sul bottone
	*/	
	if((new->operableKeys.bitmap_norm)!=NULL)
		XtSetArg(arg[i],XmNlabelPixmap,
			(new->operableKeys.bitmap_norm)); i++;
   }
   else if(new->operableKeys.draw==DRAW_LABEL) 
   {
	XtSetArg(arg[i],XmNlabelType,XmSTRING); i++;
	XtSetArg(arg[i],XmNlabelString, str); i++;
	XtSetArg(arg[i],XmNfontList,new->operableKeys.font_list); i++;
	XtSetArg(arg[i],XmNforeground,new->operableKeys.norm_fg); i++;
   }

   /* Calcola automaticamente le dimensioni del bottone in base a label
   */
   XtSetArg(arg[i],XmNrecomputeSize,True); i++;

   /* Parte modificata introducendo nuova risorsa
   */
   XtSetArg(arg[i],XmNbackground,new->operableKeys.hstate_bg);i++;
   XtSetArg(arg[i],XmNmarginWidth,0);i++;
   XtSetArg(arg[i],XmNmarginHeight,0);i++;
   XtSetArg(arg[i],XmNborderWidth,0);i++;

   /* prova Fabio
   */
   XtSetArg(arg[i],XmNnavigationType,XmNONE);i++;
   XtSetArg(arg[i],XmNtraversalOn,FALSE); i++;
   new->operableKeys.bottone=XmCreatePushButton(new,"__bottone",arg,i);
   XtAddCallback(new->operableKeys.bottone,
                XmNactivateCallback,ActivateToggle,NULL);
   XtManageChild(new->operableKeys.bottone);

   /* Libero la stringa Motif e calcolo le dimensioni di core
   */
   if(new->operableKeys.draw==DRAW_LABEL)
   {
	XmStringFree(str);

        /* Leggo le dimensioni del bottone e setto le dimensioni di core
        */
	XtSetArg(arg[0],XmNwidth,&bottone_width);
	XtSetArg(arg[1],XmNheight,&bottone_height);
	XtGetValues(new->operableKeys.bottone,arg,2);

        /* Setto l' altezza di core
	*/
	new->core.width=bottone_width+6;
	new->core.height=bottone_height+6;
   }

   /* creazione text widget
   */
   if(strcmp(new->operableKeys.label,"")) /* setto la heigth di core per text */
	new->core.height=bottone_height+6+h_font;

   h_font=new->operableKeys.font_info->ascent+new->operableKeys.font_info->descent;
   w_font=new->operableKeys.font_info->max_bounds.width;
   if(strcmp(new->operableKeys.label,""))
   {
      i=0;
      XtSetArg(arg[i],XmNx,3); i++;
      XtSetArg(arg[i],XmNy,bottone_height+3); i++;
      XtSetArg(arg[i],XmNwidth,new->core.width); i++;
      XtSetArg(arg[i],XmNheight,new->core.height-bottone_height-6); i++;
      XtSetArg(arg[i],XmNmaxLength,10); i++;
      XtSetArg(arg[i],XmNborderWidth,0); i++;
      XtSetArg(arg[i],XmNresizeHeight,False); i++;
      XtSetArg(arg[i],XmNresizeWidth,False); i++;
      XtSetArg(arg[i],XmNfontList,new->operableKeys.font_list); i++;
      XtSetArg(arg[i],XmNshadowThickness,0); i++;
      XtSetArg(arg[i],XmNmarginWidth,0); i++;
      XtSetArg(arg[i],XmNmarginHeight,0); i++;
      XtSetArg(arg[i],XmNforeground,new->operableKeys.norm_fg); i++;
      XtSetArg(arg[i],XmNbackground,new->core.background_pixel);i++;
      XtSetArg(arg[i],XmNlabelString,
		XmStringCreateLtoR
		(new->operableKeys.label,XmSTRING_DEFAULT_CHARSET));i++;
      new->operableKeys.text= XmCreateLabel(new,"__text",arg,i);
      XtManageChild(new->operableKeys.text);
   }
   else
	new->operableKeys.text=NULL;

}

static void DistruggiFigli(tnew)
Widget tnew;
{
   XlOperableKeysWidget new = (XlOperableKeysWidget)tnew;

   if(new->operableKeys.text!=NULL)
        XtDestroyWidget(new->operableKeys.text);
   if(new->operableKeys.bottone!=NULL)
        XtDestroyWidget(new->operableKeys.bottone);

   new->operableKeys.text=NULL;
   new->operableKeys.bottone=NULL;

}

static void SettaActionsFigli(tnew)
Widget tnew;
{
   XlOperableKeysWidget new = (XlOperableKeysWidget)tnew;

   if(new->xlmanager.config==True)
   {
	if(new->operableKeys.text!=NULL)
        	add_translation(new->operableKeys.text,textTranslations);
	if(new->operableKeys.bottone!=NULL)
        	add_translation(new->operableKeys.bottone,buttonTranslations);
   }
}


/*************************************************
  XlIsOperableKeys restituisce True se lo widget 
   e' della classe OperableKeys
**************************************************/
Boolean XlIsOperableKeys( Widget w )
{
   CoreWidgetClass wclass;

   if( w != NULL && XtIsWidget(w) )
   {
      wclass=XtClass(w);

      if( strcmp(wclass->core_class.class_name,"OperableKeys")==0)
         return(True);
      else
         return(False);
   }
   else
     return(False);
}

/**************************************************************
   XlIsOperableKeysImp restituisce True se lo widget
   e' della classe OperableKeys ed e' di tipo OPKEY_IMPULSE
***************************************************************/
Boolean XlIsOperableKeysImp( Widget w )
{
   XlOperableKeysWidget cw = (XlOperableKeysWidget)w;

   if( XlIsOperableKeys(w) )
   {
      if( cw->operableKeys.tipo == OPKEY_IMPULSE ) 
        return(True);
      else
        return(False);
   }
   else
        return(False);
}

/**************************************************************
   XlIsOperableKeysExec restituisce True se lo widget
   e' della classe OperableKeys ed e' di tipo OPKEY_EXEC
***************************************************************/
Boolean XlIsOperableKeysExec( Widget w )
{
   XlOperableKeysWidget cw = (XlOperableKeysWidget)w;

   if( XlIsOperableKeys(w) )
   {
      if( cw->operableKeys.tipo == OPKEY_EXEC )
        return(True);
      else
        return(False);
   }
   else
        return(False);

}

/**************************************************************
   XlIsOperableKeysDirect restituisce True se lo widget
   e' della classe OperableKeys ed e' di tipo OPKEY_DIRECT
***************************************************************/
Boolean XlIsOperableKeysDirect( Widget w )
{
   XlOperableKeysWidget cw = (XlOperableKeysWidget)w;

   if( XlIsOperableKeys(w) )
   {
      if( cw->operableKeys.tipo == OPKEY_DIRECT )
        return(True);
      else
        return(False);
   }
   else
        return(False);

}

/******************************************************
 * set risorsa impAble True o False 
 * per abilitazione disabilitazione invio comandi OperableKeys
 * di tipo OPEKEY_IMPULSE
 ******************************************************/
Boolean XlOperableKeysAbleImp( Widget w, Boolean stato )
{
   XlOperableKeysWidget cw = (XlOperableKeysWidget)w;

   if( XlIsOperableKeysImp(w) )
   {
      cw->operableKeys.impAble = stato;
      return(True);
   }
   else
     return(False);
}


/*************************************** 
 * XlOperableKeysSendPert
 * 
 * invia la perturbazione associata allo widget
 * chiamata dalla callback del bottone Ejecutar 
 * della operating window
 * il tipo Imp deve essere abilitato per inviare la perturbazione
 * per il tipo exec questa viene invocata
 * dal tasto execute e nella selectRunTime (other_mmi.c)
 * viene memorizzato l'ultimo indice di widget che deve inviare
 * la perturbazione
 ***************************************/

void XlOperableKeysSendPert( Widget w )
{
   XlOperableKeysWidget cw = (XlOperableKeysWidget)w;

   if( XlIsOperableKeysDirect( w ) )
   {
      (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output);
      (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output2);
   }
   else if ( XlIsOperableKeysImp( w ) )
   {
      if( cw->operableKeys.impAble == True)
      {
        (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output);
        (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output2);
      }
   }
   else if( XlIsOperableKeysExec( w ) )
   {
      (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output);
      (olPertClassRec.olpert_class.pertsend)(xlOperableKeysClassRec.xlmanager_class.pert,
                                                cw->operableKeys.var_output2);
   }
}

/*        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
   return(XlHardCopyWidget(w));
}
#endif
