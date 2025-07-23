
/*******************************************************************************
	info_ashell.c
	(Generated from interface file info_ashell.i)
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/ArrowB.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/BulletinB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <X11/Shell.h>
#include <Xm/ScrollBar.h>

#include "libutilx.h"
#include "autodoc.h"
void lcDestroySwidget(Widget);
int leggi_all_var_f14(char*,InfoBlock **,int*,int*);
int Empty(char*);




/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo info_ashell.i
   tipo 
   release 1.12
   data 3/31/95
   reserved @(#)info_ashell.i	1.12
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/


#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "autodoc.h"

int copia_file(char*, char*);
void cancella_testi(HeaderNote **);


/************************************************************************/
/* VARIABILI GLOBALI ESTERNE                                            */
/************************************************************************/

extern Display *display;
extern Pixmap pixm_noto;
extern Pixel color_bg_label, color_select_label;

XFontStruct *finfo;
extern XmFontList fontList;

extern Arg args[];
extern int nargs;

extern XmString cstring;

extern char file_tempdati[], file_tempvar[];

extern StructVarianti varianti_modello[];
extern byte num_varianti, num_file_f14;

extern WindowInfoStruct *winfo_list;

/************************************************************************/
/* VARIABILI GLOBALI                                                    */
/************************************************************************/

/* intestazione dati geometrici e fisici: */

static TextWidgetPos text_wdg_dati[] =  { {"Name",        0,   100,  8, False },
                                          {"Value",       110, 110, 10, False },
                                          {"Data source", 230, 270, 27, True  },
                                          {"Rev.",        510,  60,  4, True  },
                                          {"Page",        580,  50,  3, True  },
                                          {"Date",        640, 100,  8, True  },
                                          {"Note",        750,  70,  5, True  } };


static int num_wdg_dati = sizeof(text_wdg_dati)/sizeof(text_wdg_dati[0]);


/* intestazione variabili del blocco nel caso in cui si considera solo il */
/* file f14.dat del modello */

static TextWidgetPos text_wdg_var[] =  { {"Symbol",      20,  100,  8, False },
                                         {"Value",       130, 110, 10, False },
                                         {"Unit",        250,  80,  7, False },
                                         {"Descr.",      340,  40,  0, False },
                                         {"Data source", 390, 270, 27, True  },
                                         {"Rev.",        670,  60,  4, True  },
                                         {"Page",        740,  50,  3, True  },
                                         {"Date",        800, 100,  8, True  } };

static int num_wdg_var = sizeof(text_wdg_var)/sizeof(text_wdg_var[0]);


/* intestazione variabili del blocco nel caso in cui si considerano piu' */
/* file f14.dat (varianti del modello) */

static TextWidgetPos text_wdg_var2[] = { {"Symbol",     20, 100,  8, False },
                                         {"Unit",      130,  80,  7, False },
                                         {"",          220, 110, 10, False },
                                         {"",          340, 110, 10, False },
                                         {"",          460, 110, 10, False },
                                         {"",          580, 110, 10, False },
                                         {"",          700, 110, 10, False },
                                         {"Descr.",    820,  20,  0, False } };

static int num_wdg_var2 = sizeof(text_wdg_var2)/sizeof(text_wdg_var2[0]);

/*******************************************************************************
	The definition of the context structure:
	If you create multiple instances of your interface, the context
	structure ensures that your callbacks use the variables for the
	correct instance.

	For each swidget in the interface, each argument to the Interface
	function, and each variable in the Instance Specific section of the
	Declarations Editor, there is an entry in the context structure.
	and a #define.  The #define makes the variable name refer to the
	corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	Uxinfo_ashell;
	Widget	Uxform1;
	Widget	Uxpb_note_blocco;
	Widget	Uxlabel2;
	Widget	Uxlabel_nome_blocco;
	Widget	Uxlabel3;
	Widget	Uxlabel_descr_blocco;
	Widget	Uxpb_info_ok;
	Widget	Uxpb_info_apply;
	Widget	Uxpb_info_cancel;
	Widget	UxseparatorGadget1;
	Widget	UxseparatorGadget3;
	Widget	UxbulletinBoard1;
	Widget	Uxlabel_int1;
	Widget	Uxlabel_int2;
	Widget	Uxlabel_int3;
	Widget	Uxlabel_int4;
	Widget	Uxlabel_int5;
	Widget	Uxlabel_int6;
	Widget	Uxlabel_int7;
	Widget	Uxlabel_int8;
	Widget	UxlabelGadget12;
	Widget	Uxtext_find_nome;
	Widget	UxarrowButton3;
	Widget	UxarrowButton4;
	Widget	UxscrolledWindow2;
	Widget	Uxlabel_int_nome;
	Widget	Uxlabel_nome_var;
	Widget	Uxlabel_int_descr;
	Widget	Uxlabel_tipo_e_descr;
	unsigned char	*Uxnota_blocco;
	WidgetBlock	*Uxwidget_blocco;
	int	Uxnum_info;
	Widget	Uxlast_toggle;
	swidget	Uxwin_note_block;
	Boolean	Uxopen_win_note;
	HeaderNote	*Uxptr_note;
	int	Uxind_ricerca;
	Widget	Uxwrowcol;
	unsigned char	*Uxnome_blocco;
	unsigned char	*Uxdescr_blocco;
	unsigned char	Uxflag_blocco;
	WindowInfoStruct	*Uxwinfo;
} _UxCinfo_ashell;

#define info_ashell             UxInfo_ashellContext->Uxinfo_ashell
#define form1                   UxInfo_ashellContext->Uxform1
#define pb_note_blocco          UxInfo_ashellContext->Uxpb_note_blocco
#define label2                  UxInfo_ashellContext->Uxlabel2
#define label_nome_blocco       UxInfo_ashellContext->Uxlabel_nome_blocco
#define label3                  UxInfo_ashellContext->Uxlabel3
#define label_descr_blocco      UxInfo_ashellContext->Uxlabel_descr_blocco
#define pb_info_ok              UxInfo_ashellContext->Uxpb_info_ok
#define pb_info_apply           UxInfo_ashellContext->Uxpb_info_apply
#define pb_info_cancel          UxInfo_ashellContext->Uxpb_info_cancel
#define separatorGadget1        UxInfo_ashellContext->UxseparatorGadget1
#define separatorGadget3        UxInfo_ashellContext->UxseparatorGadget3
#define bulletinBoard1          UxInfo_ashellContext->UxbulletinBoard1
#define label_int1              UxInfo_ashellContext->Uxlabel_int1
#define label_int2              UxInfo_ashellContext->Uxlabel_int2
#define label_int3              UxInfo_ashellContext->Uxlabel_int3
#define label_int4              UxInfo_ashellContext->Uxlabel_int4
#define label_int5              UxInfo_ashellContext->Uxlabel_int5
#define label_int6              UxInfo_ashellContext->Uxlabel_int6
#define label_int7              UxInfo_ashellContext->Uxlabel_int7
#define label_int8              UxInfo_ashellContext->Uxlabel_int8
#define labelGadget12           UxInfo_ashellContext->UxlabelGadget12
#define text_find_nome          UxInfo_ashellContext->Uxtext_find_nome
#define arrowButton3            UxInfo_ashellContext->UxarrowButton3
#define arrowButton4            UxInfo_ashellContext->UxarrowButton4
#define scrolledWindow2         UxInfo_ashellContext->UxscrolledWindow2
#define label_int_nome          UxInfo_ashellContext->Uxlabel_int_nome
#define label_nome_var          UxInfo_ashellContext->Uxlabel_nome_var
#define label_int_descr         UxInfo_ashellContext->Uxlabel_int_descr
#define label_tipo_e_descr      UxInfo_ashellContext->Uxlabel_tipo_e_descr
#define nota_blocco             UxInfo_ashellContext->Uxnota_blocco
#define widget_blocco           UxInfo_ashellContext->Uxwidget_blocco
#define num_info                UxInfo_ashellContext->Uxnum_info
#define last_toggle             UxInfo_ashellContext->Uxlast_toggle
#define win_note_block          UxInfo_ashellContext->Uxwin_note_block
#define open_win_note           UxInfo_ashellContext->Uxopen_win_note
#define ptr_note                UxInfo_ashellContext->Uxptr_note
#define ind_ricerca             UxInfo_ashellContext->Uxind_ricerca
#define wrowcol                 UxInfo_ashellContext->Uxwrowcol
#define nome_blocco             UxInfo_ashellContext->Uxnome_blocco
#define descr_blocco            UxInfo_ashellContext->Uxdescr_blocco
#define flag_blocco             UxInfo_ashellContext->Uxflag_blocco
#define winfo                   UxInfo_ashellContext->Uxwinfo

static _UxCinfo_ashell	*UxInfo_ashellContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_info_ashell();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** Widget crea_text(padre, testo, y, text_info)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare
 ***     int y : posizione y
 ***     char *testo : testo da visualizzare/modificare
 ***     TextWidgetPos *text_info : struttura che contiene le informazioni
 ***                                sul text-widget (posizione, editabile ...)
 ***
 ***   Descrizione:
 ***     Crea i text widget utilizzati per visualizzare o modificare le
 ***     informazioni delle variabili o dei dati geometrici di un blocco
 ***     (sorgente del dato, release, pagina, ... ).
 ***/
Widget crea_text(padre, y, testo, text_info)
Widget padre;
int y;
char *testo;
TextWidgetPos *text_info;
{
   Widget wdg;
   char *classe;

   if ( text_info->editable )
      classe = "TextInfoBlock";
   else
      classe = "TextInfoReadonly";
   nargs = 0;
   XtSetArg (args[nargs],XmNy,y); nargs++;
   XtSetArg (args[nargs],XmNheight,ALTEZZA_TEXT); nargs++;
   XtSetArg (args[nargs],XmNleftAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg (args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg (args[nargs],XmNx,text_info->leftpos); nargs++;
   XtSetArg (args[nargs],XmNwidth,text_info->rightpos); nargs++;
   XtSetArg (args[nargs],XmNmaxLength,text_info->num_col);nargs++;
   XtSetArg (args[nargs],XmNfontList,fontList);nargs++;
   XtSetArg (args[nargs],XmNvalue,testo);nargs++;
   XtSetArg (args[nargs],XmNeditable,text_info->editable);nargs++;
   wdg = (Widget) XmCreateTextField(padre,classe,args,nargs);
   XtManageChild (wdg);

   return(wdg);
}
/*** Widget crea_labelG(padre, testo, y, text_info)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare
 ***     int y : posizione y
 ***     char *testo : testo da visualizzare/modificare
 ***     TextWidgetPos *text_info : struttura che contiene le informazioni
 ***                                sul text-widget (posizione, editabile ...)
 ***
 ***   Descrizione:
 ***     Crea i text widget utilizzati per visualizzare o modificare le
 ***     informazioni delle variabili o dei dati geometrici di un blocco
 ***     (sorgente del dato, release, pagina, ... ).
 ***/
Widget crea_labelG(padre, y, testo, text_info)
Widget padre;
int y;
char *testo;
TextWidgetPos *text_info;
{
   Widget wdg;

   cstring = CREATE_CSTRING(testo);
   nargs = 0;
   XtSetArg (args[nargs],XmNy,y); nargs++;
   XtSetArg (args[nargs],XmNheight,ALTEZZA_TEXT); nargs++;
   XtSetArg (args[nargs],XmNx,text_info->leftpos); nargs++;
   XtSetArg (args[nargs],XmNwidth,text_info->rightpos); nargs++;
   XtSetArg (args[nargs],XmNfontList,fontList);nargs++;
   XtSetArg (args[nargs],XmNlabelString,cstring);nargs++;
   XtSetArg (args[nargs],XmNalignment,XmALIGNMENT_BEGINNING);nargs++;
   XtSetArg (args[nargs],XmNrecomputeSize,False);nargs++;
   wdg = (Widget) XmCreateLabel(padre,"TextInfoReadonly",args,nargs);
   XtManageChild (wdg);
   XmStringFree(cstring);
   return(wdg);
}

/*** Widget crea_toggle(padre, y, text_info, indice)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare
 ***     int y : posizione y
 ***     TextWidgetPos *text_info : struttura che contiene le informazioni
 ***                                sul text-widget (posizione, editabile ...)
 ***     int indice : indice array widget_blocco[];
 ***
 ***   Descrizione:
 ***     Crea i toggle-button utilizzati per visualizzare la descrizione
 ***     di una variabile.
 ***/
Widget crea_toggle(padre, y, text_info, h, indice, contesto)
Widget padre;
int y;
TextWidgetPos *text_info;
int h;
int indice;
char *contesto;
{
   Widget wdg;
   swidget swdg;

   nargs = 0;
   XtSetArg (args[nargs],XmNy,y); nargs++;
   XtSetArg (args[nargs],XmNheight,h); nargs++;
   XtSetArg (args[nargs],XmNleftAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg (args[nargs],XmNrightAttachment,XmATTACH_POSITION); nargs++;
   XtSetArg (args[nargs],XmNx,text_info->leftpos); nargs++;
   XtSetArg (args[nargs],XmNwidth,text_info->rightpos); nargs++;
   XtSetArg (args[nargs], XmNlabelString, CREATE_CSTRING("")); nargs++;
   XtSetArg (args[nargs], XmNuserData, indice); nargs++;
   wdg = XmCreateToggleButton(padre,"TextInfoBlock",args,nargs);
   XtAddCallback(wdg,XmNarmCallback,callback_toggle,contesto);
   XtAddCallback(wdg,XmNdisarmCallback,setta_toggle,NULL);

   return(wdg);
}

/*** void setta_label_int(swdg, info_pos)
 ***   Parametri:
 ***     swidget swdg : shadow widget;
 ***     TextWidgetPos *info_pos : informazioni sulle position.
 ***
 ***   Descrizione:
 ***     Setta le position delle label di intestazione.
 ***/
void setta_label_int(swdg, info_pos)
swidget swdg;
TextWidgetPos *info_pos;
{
   cstring = CREATE_CSTRING(info_pos->intestazione);
   nargs = 0;
   XtSetArg(args[nargs], XmNx, info_pos->leftpos); nargs++;
   XtSetArg(args[nargs], XmNlabelString, cstring); nargs++;
   XtSetValues(UxGetWidget(swdg), args, nargs);
   XmStringFree(cstring);
}

/*** int crea_label(padre, testo, y)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare
 ***     int y : posizione y
 ***     char *testo : testo da visualizzare
 ***
 ***   Descrizione:
 ***     Crea la label di commento. Ritorna l'altezza della label
 ***/
int crea_label(padre, y, testo)
Widget padre;
int y;
char *testo;
{
   Widget wdg;
   Dimension h;
   XmString cstring;

   cstring = CREATE_CSTRING(testo);

   nargs = 0;
   XtSetArg(args[nargs], XmNx, 0); nargs++;
   XtSetArg(args[nargs], XmNy, y); nargs++;
   XtSetArg(args[nargs], XmNlabelString, cstring); nargs++;
   XtSetArg(args[nargs], XmNfontList, fontList); nargs++;
   XtSetArg(args[nargs], XmNrecomputeSize, False); nargs++;
   XtSetArg(args[nargs], XmNheight, ALTEZZA_TEXT); nargs++;
   XtSetArg(args[nargs], XmNalignment, XmALIGNMENT_BEGINNING); nargs++;
   wdg = XmCreateLabel(padre,"Commento",args,nargs);
   XtManageChild(wdg);

   XmStringFree(cstring);

   get_something(wdg, XmNheight, (void*) &h);

   return((int) h);
}

/*** int crea_label_pixmap(padre, y)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare
 ***     int y : posizione y
 ***
 ***   Descrizione:
 ***     Crea la label-pixmap.
 ***/
int crea_label_pixmap(padre, y)
Widget padre;
int y;
{
   Widget wdg;

   nargs = 0;
   XtSetArg(args[nargs], XmNx, 0); nargs++;
   XtSetArg(args[nargs], XmNy, y); nargs++;
   XtSetArg(args[nargs], XmNlabelPixmap, pixm_noto); nargs++;
   XtSetArg(args[nargs], XmNlabelType, XmPIXMAP); nargs++;
   XtSetArg(args[nargs], XmNrecomputeSize, True); nargs++;
   XtSetArg(args[nargs], XmNalignment, XmALIGNMENT_BEGINNING); nargs++;
   wdg = XmCreateLabel(padre,"LabelPixmap",args,nargs);
   XtManageChild(wdg);

   return(0);
}

/*** Widget crea_pb_note(padre, y, indice, contesto, sensitivo)
 ***   Parametri:
 ***     Widget padre: padre del widget da creare.
 ***     int y : posizione y.
 ***     int indice : indice array widget_blocco[].
 ***     char *contesto : contesto della window.
 ***     Boolean sensitivo : indica se il push-button deve essere attivo.
 ***
 ***   Descrizione:
 ***     Crea il push-button NOTE di un dato geometrico
 ***/
Widget crea_pb_note(padre, y, indice, contesto, sensitivo)
Widget padre;
int y;
int indice;
char *contesto;
Boolean sensitivo;
{
   Widget wdg;
   XmString cstring;

   cstring = CREATE_CSTRING("N");

   nargs = 0;
   XtSetArg(args[nargs], XmNx, 830); nargs++;
   XtSetArg(args[nargs], XmNwidth, 20); nargs++;
   XtSetArg(args[nargs], XmNy, y+5); nargs++;
   XtSetArg(args[nargs], XmNlabelString, cstring); nargs++;
   XtSetArg(args[nargs], XmNrecomputeSize, True); nargs++;
   XtSetArg(args[nargs], XmNsensitive, sensitivo); nargs++;
   XtSetArg(args[nargs], XmNuserData, indice); nargs++;

   wdg = XmCreatePushButton(padre,"NoteDatoGeometrico",args,nargs);
   XtAddCallback(wdg,XmNactivateCallback,callback_note,contesto);
   XtManageChild(wdg);

   XmStringFree(cstring);

   return(wdg);
}

/*** void set_label(wdg, stringa)
 ***    Parametri:
 ***       Widget wdg: label 
 ***       char *stringa: stringa
 ***
 ***    Descrizione:
 ***       Assegna una stringa alla label wdg
 ***/
void set_label(wdg, stringa)
Widget wdg;
char *stringa;
{
   XmString cstring;

   cstring = CREATE_CSTRING(stringa);
   set_something_val(wdg, XmNlabelString, (XtArgVal) cstring);
   XmStringFree(cstring);
}

/*** void callback_note(w, contesto, call_data)
 ***   Parametri:
 ***      Widget w: widget
 ***      char *contesto: contesto
 ***      XmAnyCallbackStruct *call_data: non utilizzato.
 ***
 ***   Descrizione:
 ***      Callback dei push-button delle note dei dati geometrici.
 ***/
void callback_note(w, contesto, call_data)
Widget w;
char *contesto;
XmAnyCallbackStruct *call_data;
{
   int ind;
   char *str;
   HeaderNote *p_nota;
#ifndef DESIGN_TIME
   _UxCinfo_ashell *UxSaveCtx, *UxContext;
   WidgetBlock *wb;

   UxSaveCtx = UxInfo_ashellContext;
   UxInfo_ashellContext = UxContext = (_UxCinfo_ashell *) contesto;

   get_something(w, XmNuserData, (void*) &ind);

   wb = &widget_blocco[ind];

   if (!wb->data.open_win)
   {
      str = XmTextGetString(wb->data.nota);
      p_nota = cerca_testo(ptr_note, str);
      if (p_nota == NULL)
         p_nota = appendi_lista(&ptr_note, str);

      wb->data.win_note = create_window_notes(&p_nota->testo, nome_blocco, str,
					 &wb->data.open_win);
      free(str);
   }

   UxInfo_ashellContext = UxSaveCtx;
#endif
}

/*** void callback_ins_nota(w, contesto, call_data)
 ***   Parametri:
 ***      Widget w: widget
 ***      char *contesto: contesto
 ***      XmAnyCallbackStruct *call_data: non utilizzato.
 ***
 ***   Descrizione:
 ***      Questa callback attiva i push-button delle note se l'utente ha
 ***      inserito qualcosa nel text delle note.
 ***/
void callback_ins_nota(w, contesto, call_data)
Widget w;
char *contesto;
XmAnyCallbackStruct *call_data;
{
   int ind;
   char *str;
#ifndef DESIGN_TIME
   _UxCinfo_ashell *UxSaveCtx, *UxContext;
   WidgetBlock *wb;

   UxSaveCtx = UxInfo_ashellContext;
   UxInfo_ashellContext = UxContext = (_UxCinfo_ashell *) contesto;

   get_something(w, XmNuserData, (void*) &ind);

   wb = &widget_blocco[ind];

   str = XmTextGetString(w);
   if (strlen(str))
      set_something_val(wb->data.pb_nota, XmNsensitive, (XtArgVal) True);
   else
      set_something_val(wb->data.pb_nota, XmNsensitive, (XtArgVal) False);
   XtFree(str);

   UxInfo_ashellContext = UxSaveCtx;
#endif
}

/*** void callback_toggle(w, contesto, tg_struct)
 ***   Parametri:
 ***      Widget w: widget
 ***      char *contesto: contesto
 ***      XmToggleButtonCallbackStruct *tg_struct: non utilizzato.
 ***
 ***   Descrizione:
 ***      Visualizzazione della descrizione della variabile selezionata con
 ***      il toggle-button.
 ***/
void callback_toggle(w, contesto, tg_struct)
Widget w;
char *contesto;
XmToggleButtonCallbackStruct *tg_struct;
{
   int ind;
#ifndef DESIGN_TIME
   _UxCinfo_ashell *UxSaveCtx, *UxContext;
   UxSaveCtx = UxInfo_ashellContext;
   UxInfo_ashellContext = UxContext = (_UxCinfo_ashell *) contesto;

   get_something(w, XmNuserData, (void*) &ind);

   if (last_toggle != w)
   {
      if (last_toggle != NULL)
         XmToggleButtonSetState(last_toggle, False, False);
      last_toggle = w;
      UxPutLabelString(label_nome_var, widget_blocco[ind].vars.nome);
      UxPutLabelString(label_tipo_e_descr, widget_blocco[ind].vars.tipo_e_descr);
   }

   tg_struct->set = True;

   UxInfo_ashellContext = UxSaveCtx;
#endif
}

/*** void setta_toggle(w, client_data, tg_struct)
 ***   Parametri:
 ***      Widget w: widget
 ***      XtPointer client_data: non utilizzato.
 ***      XmToggleButtonCallbackStruct *tg_struct: struttura toggle-button.
 ***
 ***   Descrizione:
 ***     Il toggle button rimane settato.
 ***/
void setta_toggle(w, client_data, tg_struct)
Widget w;
XtPointer client_data;
XmToggleButtonCallbackStruct *tg_struct;
{
   XmToggleButtonSetState(w, True, False);
}

/*** int salva_info_blocco(chiudi)
 ***   Parametri:
 ***      Boolean chiudi : indica se si deve chiudere la window oppure no
 ***
 ***   Descrizione:
 ***      Salva le informazioni del blocco nel file temporaneo.
 ***      N.B. : SETTARE IL CONTEXT CORRETTO PRIMA DI CHIAMARE QUESTA FUNZIONE!
 ***/
int salva_info_blocco( chiudi )
Boolean chiudi;
{
   FILE *fp_temp, *fp_app;
   char file_app[256], *file_temp;
   char int_blocco[30], buffer[91];
   char *nome, *sorg, *rel, *pag, *data, *nota;
   int i;
   HeaderNote *p_nota;

#ifndef DESIGN_TIME
   if (flag_blocco != VARIABILI_N_F14)
   {
      if (flag_blocco == DATI_GEOMETRICI)
         file_temp = file_tempdati;
      else
         file_temp = file_tempvar;

      tmpnam(file_app);

      if ((fp_app = fopen(file_app,"w+")) == NULL)
         return(1);

      if ((fp_temp = fopen(file_temp,"r")) == NULL)
         return(1);

      sprintf(int_blocco,"*LG*DATI DEL BLOCCO %s\n",nome_blocco);

      while ( fgets(buffer,90,fp_temp) != NULL && 
              strncmp(buffer,int_blocco,strlen(int_blocco)-1) )
          fputs(buffer, fp_app);

      fputs(int_blocco, fp_app);

   /* copia le nuove informazioni */
      for ( i=0 ; i<num_info ; i++)
      {

         if (flag_blocco == DATI_GEOMETRICI)
         {
            nome = widget_blocco[i].data.nome;
            sorg = XmTextFieldGetString(widget_blocco[i].data.source);
            rel = XmTextFieldGetString(widget_blocco[i].data.rel);
            pag = XmTextFieldGetString(widget_blocco[i].data.pag);
            data = XmTextFieldGetString(widget_blocco[i].data.data);
            nota = XmTextFieldGetString(widget_blocco[i].data.nota);
         }
         else
         {
            nome = widget_blocco[i].vars.nome;
            sorg = XmTextFieldGetString(widget_blocco[i].vars.source);
            rel = XmTextFieldGetString(widget_blocco[i].vars.rel);
            pag = XmTextFieldGetString(widget_blocco[i].vars.pag);
            data = XmTextFieldGetString(widget_blocco[i].vars.data);
            nota = "";
         }

         fprintf(fp_app,"%-8.8s|%-27.27s|%-4.4s|%-3.3s|%-8.8s|%-5.5s\n",
                         nome, sorg, rel, pag, data, nota);

         XtFree(sorg);
         XtFree(rel);
         XtFree(pag);
         XtFree(data);
         if (flag_blocco == DATI_GEOMETRICI)
            XtFree(nota);
      }

   /* note generali del blocco */
      if (nota_blocco != NULL)
      {
         fprintf(fp_app, "*LG*NOTE GENERALI DEL BLOCCO %s\n", nome_blocco);
         fprintf(fp_app, "%s\n",nota_blocco);
      }

   /* note dei singoli dati geometrici */
      if (flag_blocco == DATI_GEOMETRICI)
         for ( p_nota = ptr_note ; p_nota != NULL ; p_nota = p_nota->next_header )
         {
            for ( i=0 ; i<num_info ; i++ )
            {
                nota = XmTextFieldGetString(widget_blocco[i].data.nota);
                if ( !strcmp(nota, p_nota->nota) )
                   break;
                XtFree(nota);
            }   
         
            if ( i<num_info )
            {
               fprintf(fp_app, "*LG*NOTE DEL BLOCCO %s NOTA %s\n",
	 	               nome_blocco, p_nota->nota);
               fprintf(fp_app, "%s\n", p_nota->testo);
               XtFree(nota);
            }
         }

   /* copia le informazioni relative ai blocchi rimanenti */
      if ( !feof(fp_temp) )
      {
         while ( fgets(buffer, 90, fp_temp) != NULL && 
                 strncmp(buffer, "*LG*DATI DEL BLOCCO ", 19));
         if ( !feof(fp_temp) )
         {
            fputs(buffer, fp_app);
            while ( fgets(buffer,90,fp_temp) != NULL )
               fputs(buffer, fp_app);
         }
      }

      fclose(fp_app);
      fclose(fp_temp);

      copia_file(file_app, file_temp);

#ifdef VMS
      remove(file_app);
#else
      unlink(file_app);
#endif
   }
#endif

   if (chiudi)
      chiudi_info_ashell();
}

/*** void aggiorna_unita_misura()
 ***
 ***   Descrizione: 
 ***      aggiorna i valori delle variabili delle window aperte 
 ***      secondo le unita di misura stabilite.
 ***/
void aggiorna_unita_misura()
{
   WindowInfoStruct *win_info;
   int i, j;
   char strval[20];

#ifndef DESIGN_TIME
   _UxCinfo_ashell *UxSaveCtx, *UxContext;
   UxSaveCtx = UxInfo_ashellContext;

   for ( win_info = winfo_list ; win_info != NULL ; win_info = win_info->next )
   {
       UxInfo_ashellContext = UxContext = (_UxCinfo_ashell *) UxGetContext(win_info->swindow);

       for ( i=0 ; i<num_info ; i++ )
       {
          WidgetBlock *wb;
          double fval;

          wb = widget_blocco+i;

          if (win_info->flag == VARIABILI)
             XmTextSetString(wb->vars.unita, dim_unita_misura(wb->vars.nome[0]));
          else
             set_label(wb->vars.unita, dim_unita_misura(wb->vars.nome[0]));

          for ( j=0 ; j<wb->vars.num_valori ; j++ )
          {
              fval = converti_val( *wb->vars.nome, wb->vars.val_mks[j] );
              sprintf( strval, "%10.4e", fval);
              if (win_info->flag == VARIABILI)
                 XmTextSetString( wb->vars.valore[j], strval );
              else
                 set_label( wb->vars.valore[j], strval );
          }
       }
   }

   UxInfo_ashellContext = UxSaveCtx;
#endif
}

/*** void find_var_name(verso)
 ***    Parametri:
 ***       byte verso : indica il verso di ricerca ( 0: GIU', 1:SU' )
 ***
 ***    Descrizione:
 ***       ricerca la variabile nella lista. Se la variabile viene trovata
 ***       il nome della variabile viene evidenziato.
 ***/
void find_var_name(verso)
byte verso;
{
   char *nome_da_trovare, *nome;
   int i, step, valore, slider_size, len_nome, minimum, maximum;
   Dimension bb_height;
   Widget vscrollbar;

#ifndef DESIGN_TIME
   nome_da_trovare = XmTextGetString(UxGetWidget(text_find_nome));
   len_nome = strlen(nome_da_trovare);

   step = (verso) ? 1 : -1;

   for ( i = ind_ricerca+step ; i < num_info && i >= 0 ; i += step )
   {
       if (flag_blocco == DATI_GEOMETRICI)
          nome = widget_blocco[i].data.nome;
       else
          nome = widget_blocco[i].vars.nome;

       if ( Utstrstr(nome, nome_da_trovare) != NULL )
          break;
   }

   if ( i >= 0 && i < num_info )
   {
      get_something( UxGetWidget(scrolledWindow2), XmNverticalScrollBar,
		     (char*)&vscrollbar );
      get_something( wrowcol, XmNheight, (void*) &bb_height );

      nargs = 0;
      XtSetArg( args[nargs], XmNsliderSize, &slider_size ); nargs++;
      XtSetArg( args[nargs], XmNminimum, &minimum ); nargs++;
      XtSetArg( args[nargs], XmNmaximum, &maximum ); nargs++;
      XtGetValues( vscrollbar, args, nargs );

      valore = (i*(ALTEZZA_TEXT+5)*(maximum-minimum) )/bb_height+minimum;

      if ( valore > (maximum - slider_size) )
         valore = maximum - slider_size;

      XmScrollBarSetValues( vscrollbar, valore, slider_size, 0, 0, True );

      switch (flag_blocco)
      {
           case DATI_GEOMETRICI:
                XmTextFieldSetSelection(widget_blocco[i].data.wnome,0,8,CurrentTime);
                break;

           case VARIABILI:
                XmTextFieldSetSelection(widget_blocco[i].vars.wnome,0,8,CurrentTime);
                break;

           case VARIABILI_N_F14:
		set_something_val(widget_blocco[ind_ricerca].vars.wnome, XmNbackground, (XtArgVal) color_bg_label);
		set_something_val(widget_blocco[i].vars.wnome, XmNbackground, (XtArgVal) color_select_label);
                break;
      }

      ind_ricerca = i;

   }

   XtFree(nome_da_trovare);
#endif
}

/*** void chiudi_info_ashell()
 ***
 ***   Descrizione: 
 ***      chiude la window delle informazioni del blocco e dealloca la memoria
 ***      utilizzata. N.B. : SETTARE IL CONTEXT CORRETTO PRIMA DI CHIAMARE 
 ***      QUESTA FUNZIONE!
 ***/
void chiudi_info_ashell()
{
   int i;
   WindowInfoStruct *winfoprec = NULL, *winfotmp;

#ifndef DESIGN_TIME
   free(nome_blocco);
   free(descr_blocco);

   if (open_win_note)
      UxDestroySwidget(win_note_block);

   if (flag_blocco == DATI_GEOMETRICI)
   {
      for ( i=0 ; i<num_info ; i++)
      {
         if (widget_blocco[i].data.open_win)
            UxDestroySwidget(widget_blocco[i].data.win_note);
      }
   }

   cancella_testi(&ptr_note);

   if (winfo != NULL)
   {
      for ( winfotmp = winfo_list ; winfotmp != winfo && winfotmp != NULL ; 
            winfoprec = winfotmp, winfotmp = winfotmp->next );

      if (winfotmp != NULL)
      {
         if (winfoprec != NULL)
            winfoprec->next = winfotmp->next;
         else
            winfo_list = NULL;
         free(winfo);
      }
   }

   XtFree((char*)widget_blocco);
   UxDestroySwidget(info_ashell);
#endif
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pb_note_blocco( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	if (!open_win_note)
	   win_note_block = create_window_notes(&nota_blocco, nome_blocco, NULL,
						&open_win_note );
#endif
	}
	UxInfo_ashellContext = UxSaveCtx;
}

static void	activateCB_pb_info_ok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
	salva_info_blocco(True);
	}
	UxInfo_ashellContext = UxSaveCtx;
}

static void	activateCB_pb_info_apply( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
	salva_info_blocco(False);
	}
	UxInfo_ashellContext = UxSaveCtx;
}

static void	activateCB_pb_info_cancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
	chiudi_info_ashell();
	
	}
	UxInfo_ashellContext = UxSaveCtx;
}

static void	activateCB_arrowButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
	find_var_name(0);
	}
	UxInfo_ashellContext = UxSaveCtx;
}

static void	activateCB_arrowButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCinfo_ashell         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxInfo_ashellContext;
	UxInfo_ashellContext = UxContext =
			(_UxCinfo_ashell *) UxGetContext( UxWidget );
	{
	find_var_name(1);
	}
	UxInfo_ashellContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_info_ashell()
{

	info_ashell = XtVaCreatePopupShell( "info_ashell",
			applicationShellWidgetClass, UxTopLevel,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNdefaultFontList, UxConvertFontList( "-adobe-helvetica-Bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			XmNheight, 410,
			XmNwidth, 940,
			XmNy, 150,
			XmNx, 200,
			NULL );

	UxPutContext( info_ashell, (char *) UxInfo_ashellContext );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, info_ashell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 190,
			XmNwidth, 460,
			XmNy, 110,
			XmNx, 170,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form1, (char *) UxInfo_ashellContext );

	pb_note_blocco = XtVaCreateManagedWidget( "pb_note_blocco",
			xmPushButtonWidgetClass, form1,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "Note..." ),
			RES_CONVERT( XmNforeground, "Green" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( pb_note_blocco, (char *) UxInfo_ashellContext );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, form1,
			XmNrightAttachment, XmATTACH_NONE,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNleftWidget, pb_note_blocco,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Block name:" ),
			XmNheight, 20,
			XmNwidth, 90,
			XmNy, 10,
			XmNx, 100,
			NULL );

	UxPutContext( label2, (char *) UxInfo_ashellContext );

	label_nome_blocco = XtVaCreateManagedWidget( "label_nome_blocco",
			xmLabelWidgetClass, form1,
			XmNrightAttachment, XmATTACH_NONE,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNleftWidget, label2,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 110,
			XmNy, 10,
			XmNx, 200,
			NULL );

	UxPutContext( label_nome_blocco, (char *) UxInfo_ashellContext );

	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass, form1,
			XmNrightAttachment, XmATTACH_NONE,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNleftWidget, label_nome_blocco,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Description:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 100,
			XmNy, 10,
			XmNx, 325,
			NULL );

	UxPutContext( label3, (char *) UxInfo_ashellContext );

	label_descr_blocco = XtVaCreateManagedWidget( "label_descr_blocco",
			xmLabelWidgetClass, form1,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, label3,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 400,
			XmNy, 10,
			XmNx, 435,
			NULL );

	UxPutContext( label_descr_blocco, (char *) UxInfo_ashellContext );

	pb_info_ok = XtVaCreateManagedWidget( "pb_info_ok",
			xmPushButtonWidgetClass, form1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightPosition, 30,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 242,
			XmNy, 370,
			XmNx, 10,
			NULL );

	UxPutContext( pb_info_ok, (char *) UxInfo_ashellContext );

	pb_info_apply = XtVaCreateManagedWidget( "pb_info_apply",
			xmPushButtonWidgetClass, form1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightPosition, 64,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftPosition, 36,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNheight, 30,
			XmNwidth, 236,
			XmNy, 370,
			XmNx, 302,
			NULL );

	UxPutContext( pb_info_apply, (char *) UxInfo_ashellContext );

	pb_info_cancel = XtVaCreateManagedWidget( "pb_info_cancel",
			xmPushButtonWidgetClass, form1,
			XmNbottomPosition, 0,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftPosition, 70,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNheight, 30,
			XmNwidth, 242,
			XmNy, 370,
			XmNx, 588,
			NULL );

	UxPutContext( pb_info_cancel, (char *) UxInfo_ashellContext );

	separatorGadget1 = XtVaCreateManagedWidget( "separatorGadget1",
			xmSeparatorGadgetClass, form1,
			XmNtopWidget, label_nome_blocco,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 10,
			XmNwidth, 820,
			XmNy, 40,
			XmNx, 10,
			NULL );

	UxPutContext( separatorGadget1, (char *) UxInfo_ashellContext );

	separatorGadget3 = XtVaCreateManagedWidget( "separatorGadget3",
			xmSeparatorGadgetClass, form1,
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, pb_info_cancel,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 10,
			XmNwidth, 820,
			XmNy, 350,
			XmNx, 10,
			NULL );

	UxPutContext( separatorGadget3, (char *) UxInfo_ashellContext );

	bulletinBoard1 = XtVaCreateManagedWidget( "bulletinBoard1",
			xmBulletinBoardWidgetClass, form1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtopWidget, separatorGadget1,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 940,
			XmNy, 130,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_ANY,
			NULL );

	UxPutContext( bulletinBoard1, (char *) UxInfo_ashellContext );

	label_int1 = XtVaCreateManagedWidget( "label_int1",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 0,
			XmNx, 10,
			NULL );

	UxPutContext( label_int1, (char *) UxInfo_ashellContext );

	label_int2 = XtVaCreateManagedWidget( "label_int2",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 0,
			XmNx, 90,
			NULL );

	UxPutContext( label_int2, (char *) UxInfo_ashellContext );

	label_int3 = XtVaCreateManagedWidget( "label_int3",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 0,
			XmNx, 160,
			NULL );

	UxPutContext( label_int3, (char *) UxInfo_ashellContext );

	label_int4 = XtVaCreateManagedWidget( "label_int4",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 0,
			XmNx, 230,
			NULL );

	UxPutContext( label_int4, (char *) UxInfo_ashellContext );

	label_int5 = XtVaCreateManagedWidget( "label_int5",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 60,
			XmNy, 0,
			XmNx, 300,
			NULL );

	UxPutContext( label_int5, (char *) UxInfo_ashellContext );

	label_int6 = XtVaCreateManagedWidget( "label_int6",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 0,
			XmNx, 390,
			NULL );

	UxPutContext( label_int6, (char *) UxInfo_ashellContext );

	label_int7 = XtVaCreateManagedWidget( "label_int7",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 0,
			XmNx, 480,
			NULL );

	UxPutContext( label_int7, (char *) UxInfo_ashellContext );

	label_int8 = XtVaCreateManagedWidget( "label_int8",
			xmLabelWidgetClass, bulletinBoard1,
			RES_CONVERT( XmNforeground, "#e8b500" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 0,
			XmNx, 580,
			NULL );

	UxPutContext( label_int8, (char *) UxInfo_ashellContext );

	labelGadget12 = XtVaCreateManagedWidget( "labelGadget12",
			xmLabelGadgetClass, form1,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "FIND:" ),
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 34,
			XmNwidth, 50,
			XmNy, 320,
			XmNx, 10,
			NULL );

	UxPutContext( labelGadget12, (char *) UxInfo_ashellContext );

	text_find_nome = XtVaCreateManagedWidget( "text_find_nome",
			xmTextFieldWidgetClass, form1,
			XmNtopAttachment, XmATTACH_NONE,
			XmNresizable, FALSE,
			XmNleftWidget, labelGadget12,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 8,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNwidth, 100,
			XmNy, 311,
			XmNx, 60,
			NULL );

	UxPutContext( text_find_nome, (char *) UxInfo_ashellContext );

	arrowButton3 = XtVaCreateManagedWidget( "arrowButton3",
			xmArrowButtonWidgetClass, form1,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, text_find_nome,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 34,
			XmNwidth, 34,
			XmNy, 310,
			XmNx, 210,
			NULL );

	UxPutContext( arrowButton3, (char *) UxInfo_ashellContext );

	arrowButton4 = XtVaCreateManagedWidget( "arrowButton4",
			xmArrowButtonWidgetClass, form1,
			XmNarrowDirection, XmARROW_DOWN,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, arrowButton3,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 34,
			XmNwidth, 34,
			XmNy, 310,
			XmNx, 250,
			NULL );

	UxPutContext( arrowButton4, (char *) UxInfo_ashellContext );

	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass, form1,
			XmNbottomWidget, text_find_nome,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtopWidget, bulletinBoard1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 210,
			XmNwidth, 800,
			XmNy, 130,
			XmNx, 20,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindow2, (char *) UxInfo_ashellContext );

	label_int_nome = XtVaCreateManagedWidget( "label_int_nome",
			xmLabelGadgetClass, form1,
			XmNresizable, FALSE,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, arrowButton4,
			XmNleftOffset, 30,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Var. name:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 77,
			XmNy, 315,
			XmNx, 293,
			NULL );

	UxPutContext( label_int_nome, (char *) UxInfo_ashellContext );

	label_nome_var = XtVaCreateManagedWidget( "label_nome_var",
			xmLabelGadgetClass, form1,
			XmNresizable, FALSE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, label_int_nome,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 97,
			XmNy, 315,
			XmNx, 393,
			NULL );

	UxPutContext( label_nome_var, (char *) UxInfo_ashellContext );

	label_int_descr = XtVaCreateManagedWidget( "label_int_descr",
			xmLabelGadgetClass, form1,
			XmNresizable, FALSE,
			RES_CONVERT( XmNlabelString, "Type and descr.:" ),
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftWidget, label_nome_var,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 120,
			XmNy, 315,
			XmNx, 500,
			NULL );

	UxPutContext( label_int_descr, (char *) UxInfo_ashellContext );

	label_tipo_e_descr = XtVaCreateManagedWidget( "label_tipo_e_descr",
			xmLabelGadgetClass, form1,
			XmNresizable, FALSE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, label_int_descr,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, separatorGadget3,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 150,
			XmNy, 320,
			XmNx, 770,
			NULL );

	UxPutContext( label_tipo_e_descr, (char *) UxInfo_ashellContext );

	XtAddCallback( info_ashell, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( pb_note_blocco, XmNactivateCallback,
			activateCB_pb_note_blocco,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( pb_info_ok, XmNactivateCallback,
			activateCB_pb_info_ok,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( pb_info_apply, XmNactivateCallback,
			activateCB_pb_info_apply,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( pb_info_cancel, XmNactivateCallback,
			activateCB_pb_info_cancel,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( arrowButton3, XmNactivateCallback,
			activateCB_arrowButton3,
			(XtPointer) UxInfo_ashellContext );

	XtAddCallback( arrowButton4, XmNactivateCallback,
			activateCB_arrowButton4,
			(XtPointer) UxInfo_ashellContext );



	return ( info_ashell );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_info_ashell( _Uxnome_blocco, _Uxdescr_blocco, _Uxflag_blocco, _Uxwinfo )
	unsigned char	*_Uxnome_blocco;
	unsigned char	*_Uxdescr_blocco;
	unsigned char	_Uxflag_blocco;
	WindowInfoStruct	*_Uxwinfo;
{
	Widget                  rtrn;
	_UxCinfo_ashell         *UxContext;

	UxInfo_ashellContext = UxContext =
		(_UxCinfo_ashell *) XtMalloc( sizeof(_UxCinfo_ashell) );

	nome_blocco = _Uxnome_blocco;
	descr_blocco = _Uxdescr_blocco;
	flag_blocco = _Uxflag_blocco;
	winfo = _Uxwinfo;
	{
		/* variabili locali */
		   InfoBlock *dati_blocco;
		   int i, j, k, y, num_int, num_dati, num_var_out, num_colonne;
		   TextWidgetPos *text_position;
		   BlockData *bdata;
		   BlockVar *bvar;
		   BlockVarNmod *bvarN;
		   Widget *wtoggle;
		   char stringa[200], *contesto;
		   WidgetBlock *wb;
		   swidget form_descr_e_tipo;
		
		/* inizializzazione puntatore note */
		   ptr_note = NULL;
		   open_win_note = False;
		
		   switch (flag_blocco)
		   {
		      case DATI_GEOMETRICI:
		           text_position = text_wdg_dati;
		           num_int = num_wdg_dati;
		           break;
		
		      case VARIABILI:
		           text_position = text_wdg_var;
		           num_int = num_wdg_var;
		           break;
		
		      case VARIABILI_N_F14:
		           text_position = text_wdg_var2;
		           num_int = num_wdg_var2;
		           break;
		   }
		rtrn = _Uxbuild_info_ashell();

		/* inizializzazione */
		   nota_blocco = NULL;
		
		   last_toggle = NULL;
		
		   ind_ricerca = 0;
		
		   contesto = UxGetContext(info_ashell);
		
		/* intestazione */
		   switch (flag_blocco)
		   {
		      case DATI_GEOMETRICI:
		         num_colonne = 8;
		         UxPutLabelString(label_nome_blocco, nome_blocco);
		         UxPutLabelString(label_descr_blocco, descr_blocco);
		
		         leggi_record_f14(nome_blocco,&dati_blocco, &num_dati, &ptr_note,
					  &nota_blocco);
		
		         UxPutTitle(info_ashell, TITOLO_DATI_GEOMETRICI);
		
		         setta_label_int(label_int1, &text_position[0] );
		         setta_label_int(label_int2, &text_position[1] );
		         setta_label_int(label_int3, &text_position[2] );
		         setta_label_int(label_int4, &text_position[3] );
		         setta_label_int(label_int5, &text_position[4] );
		         setta_label_int(label_int6, &text_position[5] );
		         setta_label_int(label_int7, &text_position[6] );
		         UxPutLabelString(label_int_nome,"");
		         UxPutLabelString(label_int_descr,"");
		         break;
		
		      case VARIABILI:
		         num_colonne = 8;
		         UxPutLabelString(label_nome_blocco, nome_blocco);
		         UxPutLabelString(label_descr_blocco, descr_blocco);
		
		         UxPutTitle(info_ashell, TITOLO_VARIABILI);
		
		         leggi_var_f01(nome_blocco,&dati_blocco, &num_dati, &nota_blocco,
		                       flag_blocco);
		
		         setta_label_int(label_int1, &text_position[0] );
		         setta_label_int(label_int2, &text_position[1] );
		         setta_label_int(label_int3, &text_position[2] );
		         setta_label_int(label_int4, &text_position[3] );
		         setta_label_int(label_int5, &text_position[4] );
		         setta_label_int(label_int6, &text_position[5] );
		         setta_label_int(label_int7, &text_position[6] );
		         setta_label_int(label_int8, &text_position[7] );
		         wtoggle = (Widget *) calloc(num_dati, sizeof(Widget));
		         break;
		
		      case VARIABILI_N_F14:
		         num_colonne = 8;
		         UxDestroySwidget(label_nome_blocco);
		         UxDestroySwidget(label_descr_blocco);
		         UxDestroySwidget(pb_note_blocco);
		         UxDestroySwidget(label2);
		         UxDestroySwidget(label3);
		         UxPutSensitive(pb_info_ok, "false");
		         UxPutSensitive(pb_info_apply, "false");
		
		         UxPutTitle(info_ashell, TITOLO_STEADY_STATE_VER);
		
		         leggi_all_var_f14(nome_blocco,&dati_blocco, &num_dati, &num_var_out);
		
		         for ( i = 0, j = 2 ; i < num_varianti ; i++ )
		            if (varianti_modello[i].abilitato)
		               strcpy(text_position[j++].intestazione, 
		                      varianti_modello[i].titolo);
		         for ( ; j < num_varianti+2 ; j++ )
		            strcpy(text_position[j].intestazione, "< NONE >");
		
		         setta_label_int(label_int1, &text_position[0] );
		         setta_label_int(label_int2, &text_position[1] );
		         setta_label_int(label_int3, &text_position[2] );
		         setta_label_int(label_int4, &text_position[3] );
		         setta_label_int(label_int5, &text_position[4] );
		         setta_label_int(label_int6, &text_position[5] );
		         setta_label_int(label_int7, &text_position[6] );
		         setta_label_int(label_int8, &text_position[7] );
		         wtoggle = (Widget *) calloc(num_dati, sizeof(Widget));
		         break;
		   }
		
		/* Crea il contenitore dei text e delle label */
		   nargs = 0;
		   XtSetArg(args[nargs], XmNresizePolicy, XmRESIZE_GROW); nargs++;
		   XtSetArg(args[nargs], XmNmarginHeight, 0); nargs++;
		   XtSetArg(args[nargs], XmNmarginWidth, 0); nargs++;
		   wrowcol = XmCreateBulletinBoard(UxGetWidget(scrolledWindow2), "rowcol", 
					       args, nargs);
		
		/* alloca la memoria per i text-widget */
		   widget_blocco = (WidgetBlock *) XtCalloc(num_dati, sizeof(WidgetBlock));
		
		   num_info = 0;
		   for ( i = 0, y = 0 ; i < num_dati ; i++ )
		   {
		       wb = &widget_blocco[num_info];
		
		       switch (dati_blocco[i].flag)
		       {
		          case VARIABILI:
		             bvar = &dati_blocco[i].var;
		
		             if (bvar->noto)
			        crea_label_pixmap(wrowcol, y+10);
		
		             strcpy(wb->vars.nome,bvar->nome);
		             sprintf(wb->vars.tipo_e_descr,"--%s-- %s",bvar->tipovar,
								  bvar->descrizione);
		             wb->vars.val_mks[0] = bvar->fval_mks;
		             wb->vars.num_valori = 1;
		             wb->vars.wnome = crea_text(wrowcol,y,bvar->nome, &text_position[0]);
		             wb->vars.valore[0] = crea_text(wrowcol,y,bvar->valore,&text_position[1]);
		             wb->vars.unita = crea_text(wrowcol,y,dim_unita_misura(bvar->nome[0]),
					           &text_position[2]);
		             wtoggle[num_info] = crea_toggle(wrowcol, y, &text_position[3],
							     ALTEZZA_TEXT, num_info,contesto);
		             wb->vars.source = crea_text(wrowcol,y,bvar->sorgente,&text_position[4]);
		             wb->vars.rel = crea_text(wrowcol,y,bvar->release,&text_position[5]);
		             wb->vars.pag = crea_text(wrowcol,y,bvar->pagina,&text_position[6]);
		             wb->vars.data = crea_text(wrowcol,y,bvar->data,&text_position[7]);
		             num_info++;
		             y += ALTEZZA_TEXT+5;
		             break;
		
		          case VARIABILI_N_F14:
		             bvarN = &dati_blocco[i].varN;
		
		             if (bvarN->noto)
		                crea_label_pixmap(wrowcol, y+10);
		
		             strcpy(wb->vars.nome,bvarN->nome);
		             sprintf(wb->vars.tipo_e_descr,"%s %s",bvarN->nome_blk,
							      bvarN->descrizione);
		             wb->vars.num_valori = bvarN->num_valori;
		             for ( k=0 ; k<bvarN->num_valori ; k++)
		                 wb->vars.val_mks[k] = bvarN->fval_mks[k];
		             wb->vars.wnome = crea_labelG(wrowcol,y,bvarN->nome, &text_position[0]);
		             wb->vars.unita = crea_labelG(wrowcol,y,dim_unita_misura(bvarN->nome[0]),
						          &text_position[1]);
		             wb->vars.valore[0] = crea_labelG(wrowcol,y,bvarN->valori[0],&text_position[2]);
		             wb->vars.valore[1] = crea_labelG(wrowcol,y,bvarN->valori[1],&text_position[3]);
		             wb->vars.valore[2] = crea_labelG(wrowcol,y,bvarN->valori[2],&text_position[4]);
		             wb->vars.valore[3] = crea_labelG(wrowcol,y,bvarN->valori[3],&text_position[5]);
		             wb->vars.valore[4] = crea_labelG(wrowcol,y,bvarN->valori[4],&text_position[6]);
		             wb->vars.num_valori = bvarN->num_valori;
		             wtoggle[num_info] = crea_toggle(wrowcol, y, &text_position[7],
							     ALTEZZA_TEXT, num_info,contesto);
		             num_info++;
		             y += ALTEZZA_TEXT+5;
		             break;
		
		          case DATI_GEOMETRICI:
		             bdata = &dati_blocco[i].dati;
		
		             wb->data.open_win = False;
		             strcpy(wb->data.nome,bdata->nome);
		             wb->vars.wnome = crea_text(wrowcol,y,bdata->nome,&text_position[0]);
		             crea_text(wrowcol,y,bdata->valore,&text_position[1]);
		             wb->data.source = crea_text(wrowcol,y,bdata->sorgente,&text_position[2]);
		             wb->data.rel = crea_text(wrowcol,y,bdata->release,&text_position[3]);
		             wb->data.pag = crea_text(wrowcol,y,bdata->pagina,&text_position[4]);
		             wb->data.data = crea_text(wrowcol,y,bdata->data,&text_position[5]);
		             wb->data.nota = crea_text(wrowcol,y,bdata->note,&text_position[6]);
		             XtAddCallback(widget_blocco[num_info].data.nota,
					   XmNvalueChangedCallback,
					   callback_ins_nota,
					   UxGetContext(rtrn));
		
		             set_something_val(wb->data.nota,XmNuserData, (XtArgVal) num_info);
		             wb->data.pb_nota = crea_pb_note(wrowcol,y,num_info,
							     contesto, !Empty(bdata->note));
		             num_info++;
		
		             y += ALTEZZA_TEXT+5;
		             break;
		
		          case COMMENTO:
		
		             crea_label(wrowcol,y,dati_blocco[i].comment.commento);
		             y += ALTEZZA_TEXT+5;
		
		             break;
		      }
		   }
		
		   if ( flag_blocco == VARIABILI_N_F14 || flag_blocco == VARIABILI)
		   {
		      XtManageChildren(wtoggle, num_info);
		      free(wtoggle);
		   }
		
		   XtFree((char*)dati_blocco);
		
		   XtManageChild(wrowcol);
		
		   UxPopupInterface(rtrn, no_grab);
		   return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_info_ashell( _Uxnome_blocco, _Uxdescr_blocco, _Uxflag_blocco, _Uxwinfo )
	unsigned char	*_Uxnome_blocco;
	unsigned char	*_Uxdescr_blocco;
	unsigned char	_Uxflag_blocco;
	WindowInfoStruct	*_Uxwinfo;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_info_ashell( _Uxnome_blocco, _Uxdescr_blocco, _Uxflag_blocco, _Uxwinfo );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

