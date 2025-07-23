
/*******************************************************************************
	nmod_app_shell.c
	(Generated from interface file nmod_app_shell.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/BulletinB.h>
#include <Xm/ScrolledW.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/CascadeBG.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo nmod_app_shell.i
   tipo 
   release 2.23
   data 1/18/96
   reserved @(#)nmod_app_shell.i	2.23
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif
/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "definizioni.h"

#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

extern int num_yes_toggle;
extern swidget nmod_dialog_equations;
extern char file_jac_temp[];

extern Boolean dbox_userdata_managed;
extern Boolean dbox_userjac_managed;
extern Boolean dbox_userresid_managed;
extern Boolean dbox_usereq_managed;

extern StructVars *variabili;
extern StructDati *dati_geom;

extern byte jacobian_type;

extern byte tipo_modulo;

extern char nome_modulo[], descr_modulo[];
extern char *nome_nuovo_modulo, *descr_nuovo_modulo;

extern Arg args[20];
extern Cardinal argcount;
extern char message[], *font;

extern Widget UxTopLevel;
extern Dialog_geometry geom_attention;
extern Widget attention_wdg;

extern WdgDatiGeometrici *wdati_geometrici;
extern WdgVariabili *wvar_stato, *wvar_algebriche, *wvar_ingresso;

extern XmString cstring;

/****************************************************************/
/* VARIABILI GLOBALI 						*/
/****************************************************************/

/* Variabili globali per dimensionare la matrice jacobiana */
/* (VALORI DI DEFAULT) */
int num_var_stato = 0;
int num_var_algebriche = 0;
int num_var_ingresso = 0;
int num_dati_geometrici = 0;
int jac_rows = 20, 
    jac_cols = 30;

/* variabili utilizzate per memorizzare i text delle window relative */
/* alle subroutine I2,JC,D1 e residui (NEWMOD) */
char *str_dichI2 = NULL, *str_codI2 = NULL;
char *str_dichJC = NULL, *str_codJC = NULL;
char *str_dichMOD = NULL, *str_codMOD = NULL,*str_resMOD[MAX_EQUAZIONI];
char *str_signeq[MAX_EQUAZIONI], *str_uniteq[MAX_EQUAZIONI],
     *str_cosnor[MAX_EQUAZIONI];

Boolean bboard_flag = True;
Boolean bool_dialog_config = True;
Boolean bool_dialog_equation = True;
Boolean nmod_def_managed = False;
Boolean nmod_def_initialized = False;
Boolean non_salvato = True;

Widget wconfirm_save;
Widget _nmod_varsetup;
Widget scrollw_label1, scrollw_label2, scrollw_label3;
Widget dialog_setup;

int option_scelto = K_OPT_VOID;
int posy_text;

JacToggStruct jactoggstruct[MAX_EQUAZIONI][MAX_VARIABILI_IN_OUT];

/* variabili utilizzate per l'attivazione dell'editor (view delle subroutine) */
Widget wtxteditor;  /* variabile TAMPAX */
Widget weditor[7];  /* widget delle window di editor (1 per ciascuna sub.) */
char nomitmp[7][256]; /* nomi di file temporanei (come sopra) */

/*************************************************************/
/* Struttura di callback per l'editor (view delle subroutine */
/*************************************************************/
Elenco_callback funz_editor = {
        { "Done",  editor_done, 0},
        { NULL, NULL, NULL },
        { NULL, NULL, NULL }};

Dialog_geometry geom_editor = { TRUE, NULL, NULL, 700, 500};

/****************************************************/
/* Struttura di callback per gli widget di conferma */
/****************************************************/
Elenco_callback rilancia_setup_nmod = {
        {"Yes", setup_again, YES },
        {"No" , setup_again, NO  },
        { NULL, NULL,   NULL    }};

Elenco_callback funz_confirm_save = {
        {"Yes", confirm_save, YES },
        {"No" , confirm_save, NO  },
        { NULL, NULL,   NULL    }};

Elenco_callback funz_confirm_quit = {
        {"Yes",    confirm_quit, YES    },
        {"No" ,    confirm_quit, NO     },
        {"Cancel", confirm_quit, CANCEL }};

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
	Widget	Uxnmod_app_shell;
	Widget	Uxnmod_form;
	Widget	Uxnmod_pb_ok;
	Widget	UxrowColumn1;
	Widget	Uxnmod_menubar;
	Widget	Uxmb_nmod_file;
	Widget	Uxpb_nmod_quit;
	Widget	Uxmenu1_top_b3;
	Widget	Uxmb_nmod_config;
	Widget	Uxmenu1_top_b4;
	Widget	Uxmb_nmod_user;
	Widget	Uxpb_nmod_I2;
	Widget	Uxnmod_menubar_top_b1;
	Widget	Uxmb_nmod_view;
	Widget	Uxpb_nmod_vI2;
	Widget	Uxpb_nmod_vI3;
	Widget	Uxpb_nmod_vI4;
	Widget	Uxpb_nmod_vC1;
	Widget	Uxpb_nmod_vJC;
	Widget	Uxpb_nmod_vMOD;
	Widget	Uxpb_nmod_vD1;
	Widget	Uxnmod_menubar_top_b2;
	Widget	Uxnmod_label_nome;
	Widget	Uxnmod_text_nome;
	Widget	Uxnmod_label_descr;
	Widget	Uxnmod_text_descr;
	Widget	Uxnmod_sep1;
	Widget	Uxnmod_menu_opt;
	Widget	Uxnmod_menu_p1;
	Widget	Uxopt_stato;
	Widget	Uxopt_ingresso;
	Widget	Uxopt_out;
	Widget	Uxopt_all;
	Widget	Uxnmod_menu_p1_b6;
	Widget	Uxopt_dati;
	Widget	Uxnmod_menu1;
	Widget	Uxnmod_sep2;
	Widget	Uxnmod_scroll_window;
	Boolean	Uxmodifica;
} _UxCnmod_app_shell;

#define nmod_app_shell          UxNmod_app_shellContext->Uxnmod_app_shell
#define nmod_form               UxNmod_app_shellContext->Uxnmod_form
#define nmod_pb_ok              UxNmod_app_shellContext->Uxnmod_pb_ok
#define rowColumn1              UxNmod_app_shellContext->UxrowColumn1
#define nmod_menubar            UxNmod_app_shellContext->Uxnmod_menubar
#define mb_nmod_file            UxNmod_app_shellContext->Uxmb_nmod_file
#define pb_nmod_quit            UxNmod_app_shellContext->Uxpb_nmod_quit
#define menu1_top_b3            UxNmod_app_shellContext->Uxmenu1_top_b3
#define mb_nmod_config          UxNmod_app_shellContext->Uxmb_nmod_config
#define menu1_top_b4            UxNmod_app_shellContext->Uxmenu1_top_b4
#define mb_nmod_user            UxNmod_app_shellContext->Uxmb_nmod_user
#define pb_nmod_I2              UxNmod_app_shellContext->Uxpb_nmod_I2
#define nmod_menubar_top_b1     UxNmod_app_shellContext->Uxnmod_menubar_top_b1
#define mb_nmod_view            UxNmod_app_shellContext->Uxmb_nmod_view
#define pb_nmod_vI2             UxNmod_app_shellContext->Uxpb_nmod_vI2
#define pb_nmod_vI3             UxNmod_app_shellContext->Uxpb_nmod_vI3
#define pb_nmod_vI4             UxNmod_app_shellContext->Uxpb_nmod_vI4
#define pb_nmod_vC1             UxNmod_app_shellContext->Uxpb_nmod_vC1
#define pb_nmod_vJC             UxNmod_app_shellContext->Uxpb_nmod_vJC
#define pb_nmod_vMOD            UxNmod_app_shellContext->Uxpb_nmod_vMOD
#define pb_nmod_vD1             UxNmod_app_shellContext->Uxpb_nmod_vD1
#define nmod_menubar_top_b2     UxNmod_app_shellContext->Uxnmod_menubar_top_b2
#define nmod_label_nome         UxNmod_app_shellContext->Uxnmod_label_nome
#define nmod_text_nome          UxNmod_app_shellContext->Uxnmod_text_nome
#define nmod_label_descr        UxNmod_app_shellContext->Uxnmod_label_descr
#define nmod_text_descr         UxNmod_app_shellContext->Uxnmod_text_descr
#define nmod_sep1               UxNmod_app_shellContext->Uxnmod_sep1
#define nmod_menu_opt           UxNmod_app_shellContext->Uxnmod_menu_opt
#define nmod_menu_p1            UxNmod_app_shellContext->Uxnmod_menu_p1
#define opt_stato               UxNmod_app_shellContext->Uxopt_stato
#define opt_ingresso            UxNmod_app_shellContext->Uxopt_ingresso
#define opt_out                 UxNmod_app_shellContext->Uxopt_out
#define opt_all                 UxNmod_app_shellContext->Uxopt_all
#define nmod_menu_p1_b6         UxNmod_app_shellContext->Uxnmod_menu_p1_b6
#define opt_dati                UxNmod_app_shellContext->Uxopt_dati
#define nmod_menu1              UxNmod_app_shellContext->Uxnmod_menu1
#define nmod_sep2               UxNmod_app_shellContext->Uxnmod_sep2
#define nmod_scroll_window      UxNmod_app_shellContext->Uxnmod_scroll_window
#define modifica                UxNmod_app_shellContext->Uxmodifica

static _UxCnmod_app_shell	*UxNmod_app_shellContext;

Widget	pb_nmod_save;
Widget	pb_nmod_varsetup;
Widget	pb_nmod_jactopology;
Widget	pb_nmod_JC;
Widget	pb_nmod_residual;
Widget	pb_nmod_D1;
Widget	nmod_label_tit1;
Widget	nmod_label_tit2;
Widget	nmod_label_tit3;
Widget	nmod_bboard;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_nmod_shell();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*** setup_again ()
 ***
 ***   Descrizione:
 ***     Callback chiamata premendo i pushButton della finestra
 ***     di rilancio setup nuovo modulo (NEWMOD).
 ***/
void setup_again (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
   switch (which_button)
   {
      case YES:

      /* Cancellazione eventuale file jac temporaneo */
         if (!access(file_jac_temp,F_OK))
            if ( unlink(file_jac_temp) )
               printf ("Can't delete jacobian temporary file. Failure.\n");

      /* Visualizzazione finestra setup */
         create_nmod_dialog_config();

      break;
  
      case NO:
         XtDestroyWidget(dialog_setup);
      break;
   }
}

/*** editor_done(...)
 ***
 ***   Descrizione:
 ***     Callback chiamata dall'editor di libreria (libutilx) 
 ***/
void editor_done( w, indice, call_data)
Widget w;
int indice;
XmAnyCallbackStruct *call_data;
{
   Text_find_struct *ptr;

/* Dealloca la memoria */
   get_something(weditor[indice], XmNuserData, (void*) &ptr);
   XtFree(ptr);
 
/* Cancella file temporaneo */
#ifdef VMS
   remove(nomitmp[indice]);
#else
   unlink(nomitmp[indice]);
#endif

/* chiudi la window */
   XtDestroyWidget(weditor[indice]);
}

/*** void confirm_save()
 ***
 ***   Descrizione:
 ***     Chiamata dalla window di conferma durante il save quando il modulo
 ***     esiste gia' nel file lista_moduli.dat
 ***/
void confirm_save (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
   if (which_button == YES)
      salva_e_genera();
   XtDestroyWidget(wconfirm_save);
}

/*** void confirm_quit()
 ***
 ***   Descrizione:
 ***     Chiamata quando si esce dalla window della NEWMOD e le informazioni
 ***     non sono state salvate.
 ***/
void confirm_quit (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
   if (which_button == YES)
      salva_e_genera();

/* Chiudi la window di conferma */
   XtDestroyWidget(wconfirm_save);

/* Se e' stato premuto 'CANCEL' non uscire dalla window NEWMOD */
   if (which_button == CANCEL)
      return; 

/* Dealloca la memoria e chiudi la window della NEWMOD */
   quit_newmod();
}

/*** salva_e_genera()
 ***
 ***   Descrizione:
 ***     La funzione salva le informazione della NEWMOD, genera il FORTRAN ed 
 ***     aggiorna il file lista_moduli.dat 
 ***/
salva_e_genera()
{
   if ( !salva_interfaccia())
   {
      non_salvato = False;

      scrivi_messaggio("NEWMOD information succesfully saved.");

   /* Generazione codice FORTRAN */
      if (genera_fortran(nome_nuovo_modulo, descr_nuovo_modulo) == 0)
      {
         scrivi_messaggio("OK! FORTRAN module generate correctly.");

      /* Aggiorna il file lista_moduli.dat (se necessario) */
         aggiungi_modulo_lista(LIBUT,nome_nuovo_modulo,descr_nuovo_modulo,True);

      /* Aggiorna il file icon_list.dat (se necesasrio) */
         copia_nella_libgraf(nome_nuovo_modulo,descr_nuovo_modulo,
                             ONLY_FORTRAN,FALSE,getenv("LEGOCAD_USER"));

      /* Aggiorna la scroll-list dei moduli di processo (se e' quella */
      /* visualizzata in questo momento). */
         if (tipo_modulo == LIBUT)
         {
            char nome_e_descr[100];

            cambia_tipo_modulo (NULL,LIBUT,NULL);
            sprintf(nome_e_descr,"%s  %s",nome_nuovo_modulo,descr_nuovo_modulo);
            setta_label_modsel(nome_e_descr);
         }
      }
      else
         scrivi_messaggio("FORTRAN generation error!.");
   }
   else
     scrivi_messaggio("Cannot save NEWMOD information (file busy).");
}

/***
 *** menu_newmod_init()
 ***
 ***   Descrizione:
 ***     Disabilita alcune voci di menu della dialog-box della NEWMOD.
 ***     Tali voci di menu vengono attivate qunado l'utente ha premuto
 ***     OK e le informazioni sono corrette.
 ***/
menu_newmod_init()
{
/* Disabilita l'accesso alle sezioni USER (I2,JC,D1,resid.,D1) */
   set_something_val (UxGetWidget(pb_nmod_I2),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_JC),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_residual),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_D1),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vI2),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vJC),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vMOD),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vD1),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vI3),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vI4),XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_vC1),XmNsensitive, (XtArgVal) False);

/* La matrice jacobiana e il salvataggio su file vengono imbibiti */
   set_something_val (UxGetWidget(pb_nmod_jactopology), XmNsensitive, (XtArgVal) False);
   set_something_val (UxGetWidget(pb_nmod_save), XmNsensitive, (XtArgVal) False);

   nmod_def_initialized = False;
}

/*** quit_newmod()
 ***
 ***    Descrizione:
 ***       Dealloca la memoria utilizzata durante la sessione NEWMOD e 
 ***       ritorna al menu principale.
 ***/
quit_newmod()
{
   int i,j;

   UxDestroySwidget(nmod_app_shell);
   nmod_def_managed = False;

   scrivi_messaggio("Deallocating memory ... wait please!");

/* Dealloca la memoria utilizzata per i text-widget delle variabili e dei */
/* dati geometrici */
   XtFree(wvar_stato);
   wvar_stato = NULL;

   XtFree(wvar_algebriche);
   wvar_algebriche = NULL;

   XtFree(wvar_ingresso);
   wvar_ingresso = NULL;

   XtFree(wdati_geometrici);
   wdati_geometrici = NULL;
   
/* Deallocazione delle stringhe utilizzate per l'inserimento del codice */
/* utente ed altro */
   if (nmod_def_initialized)
   {
      XtFree(variabili);
      variabili = NULL;
      XtFree(dati_geom);
      dati_geom = NULL;

      XtFree(str_dichI2);
      str_dichI2 = NULL;

      XtFree(str_codI2);
      str_codI2 = NULL;

      XtFree(str_dichJC);
      str_dichJC = NULL;

      XtFree(str_codJC);
      str_codJC = NULL;

      for ( i=0 ; i<jac_rows ; i++ )
         for ( j=0 ; j<jac_cols ; j++ )
         {
            XtFree(jactoggstruct[i][j].stringa);
            jactoggstruct[i][j].stringa = NULL;
         }

      XtFree(str_dichMOD);
      str_dichMOD = NULL;
    
      XtFree(str_codMOD);
      str_codMOD = NULL;

      for ( i=0 ; i<MAX_EQUAZIONI ; i++ )
      {
         XtFree(str_resMOD[i]);
         str_resMOD[i] = NULL;

         XtFree(str_signeq[i]);
         str_signeq[i] = NULL;

         XtFree(str_uniteq[i]);
         str_uniteq[i] = NULL;
      
         XtFree(str_cosnor[i]);  
         str_cosnor[i] = NULL;
      }
   }
   scrivi_messaggio("Done.");
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_nmod_pb_ok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if ( controlla_input_def(UxGetWidget(nmod_text_nome), UxGetWidget(nmod_text_descr)) )
	   return;
	else
	{
	   non_salvato = True;
	   sprintf(message," Ok. New module definitions correct.");
	   attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,geom_attention);   
	   scrivi_messaggio (message);
	
	/* Abilitazione voci di menu jacobiano */
	   set_something_val (UxGetWidget(pb_nmod_jactopology),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_save),XmNsensitive, (XtArgVal) True);
	
	/* Abilitazione voci di menu user sections */
	   set_something_val (UxGetWidget(pb_nmod_I2),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_residual),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_D1),XmNsensitive, (XtArgVal) True);
	
	/* Se l'utente ha scelto lo jacobiano numerico */
	/* la sezione JC e' inibita */
	   set_something_val (UxGetWidget(pb_nmod_JC), XmNsensitive,
	                  (jacobian_type == NUMERICAL) ? False : True);
	
	   set_something_val (UxGetWidget(pb_nmod_vI2),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vJC),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vMOD),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vD1),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vI3),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vI4),XmNsensitive, (XtArgVal) True);
	   set_something_val (UxGetWidget(pb_nmod_vC1),XmNsensitive, (XtArgVal) True);
	
	}
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_save( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	long offset;
	
	if (controlla_lista_moduli(LIBUT, nome_nuovo_modulo, &offset))
	{
	   sprintf(message,"The module %s already exist. Overwrite it?",
	                   nome_nuovo_modulo);
	   wconfirm_save = (Widget) conferma (UxTopLevel,message,funz_confirm_save); 
	}
	else
	   salva_e_genera();
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_quit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (non_salvato && nmod_def_initialized)
	{
	   sprintf(message,"You have made changes.\nDo you wan't save information",
	                   nome_nuovo_modulo);
	   wconfirm_save = (Widget) conferma (UxTopLevel,message,funz_confirm_quit); 
	}
	else
	   quit_newmod();
	
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_varsetup( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (!bool_dialog_equation)
	   sprintf(message,"You must close the window of the jacobian configuration!");
	
	if (dbox_userjac_managed)
	   sprintf(message,"You must close the window of the JC subroutine!");
	
	if (dbox_userresid_managed)
	   sprintf(message,"You must close the window of the residual subroutine!");
	
	if (dbox_usereq_managed)
	   sprintf(message,"You must close the window of the D1 subroutine!");
	
	if (!bool_dialog_equation || dbox_userjac_managed || dbox_userresid_managed ||
	    dbox_usereq_managed)
	{
	    attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,geom_attention);
	    return;
	}
	
	if (!bool_dialog_config)
	{
	   scrivi_messaggio ("Setup window already in use.");
	   return;
	}
	
	if (nmod_def_initialized)
	{
	   sprintf(message,"WARNING: Modifications to module setup will result\n");
	   strcat(message,"in resetting jacobian topology. Continue anyway ?");
	   dialog_setup = (Widget) conferma(UxTopLevel,message,rilancia_setup_nmod);
	   XtManageChild (dialog_setup);
	}
	else
	{
	   bool_dialog_config = False;
	   create_nmod_dialog_config();
	}
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_jactopology( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (dbox_userjac_managed)
	{
	   sprintf(message,"You must close the window of the JC subroutine!");
	   attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,geom_attention);
	   return;
	}
	
	if (bool_dialog_equation)
	{
	   create_dialog_equation();
	   bool_dialog_equation = False;
	}
	
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_I2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (!dbox_userdata_managed)
	   create_dbox_userdata();
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_JC( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (!dbox_userjac_managed)
	   create_dbox_userjac();
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_residual( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (!dbox_userresid_managed)
	   create_dbox_userresid();
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_D1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	if (!dbox_usereq_managed)
	   create_dbox_usereq();
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vI2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[0]);
	   if ((fp = fopen(nomitmp[0],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 0;  /* indice array */
	
	   genmodI2(fp, nome_nuovo_modulo, descr_nuovo_modulo, dati_geom,
		        num_dati_geometrici, variabili, numero_variabili,
	                str_dichI2, str_codI2);
	
	   fclose(fp);
	
	   weditor[0] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[0], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE I2 (DATA SECTION)");
	   set_something_val(weditor[0], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vI3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[1]);
	   if ((fp = fopen(nomitmp[1],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 1;  /* indice array */
	
	   genmodI3(fp, nome_nuovo_modulo, variabili, numero_variabili,
	                num_var_stato, num_var_algebriche);
	
	   fclose(fp);
	
	   weditor[1] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[1], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE I3 (VARIABLE LIST SECTION)");
	   set_something_val(weditor[1], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vI4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[2]);
	   if ((fp = fopen(nomitmp[2],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 2;  /* indice array */
	
	   genmodI4(fp, nome_nuovo_modulo);
	
	   fclose(fp);
	
	   weditor[2] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[2], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE I4");
	   set_something_val(weditor[2], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vC1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[3]);
	   if ((fp = fopen(nomitmp[3],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 3;  /* indice array */
	
	   genmodC1(fp, nome_nuovo_modulo, descr_nuovo_modulo, jacobian_type,
	                dati_geom, num_dati_geometrici,
	                variabili, numero_variabili);
	
	   fclose(fp);
	
	   weditor[3] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[3], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE C1 (RESIDUAL EVALUATION)");
	   set_something_val(weditor[3], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vJC( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[4]);
	   if ((fp = fopen(nomitmp[4],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 4;  /* indice array */
	
	   genmodJC(fp, nome_nuovo_modulo, dati_geom, num_dati_geometrici,
	                variabili, numero_variabili, num_var_stato, num_var_algebriche,
	                jactoggstruct, str_dichJC, str_codJC);
	
	   fclose(fp);
	
	   weditor[4] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[4], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE JC (JACOBIAN SECTION)");
	   set_something_val(weditor[4], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vMOD( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[5]);
	   if ((fp = fopen(nomitmp[5],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 5;  /* indice array */
	
	   genmodMOD(fp, nome_nuovo_modulo, descr_nuovo_modulo, dati_geom,
		        num_dati_geometrici, variabili, numero_variabili,num_var_stato,
		        num_var_algebriche, str_dichMOD, str_codMOD, str_resMOD);
	
	   fclose(fp);
	
	   weditor[5] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[5], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE RESIDUAL");
	   set_something_val(weditor[5], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_pb_nmod_vD1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	   FILE *fp;
	   int numero_variabili;
	
	   numero_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;
	
	/* nome file temporaneo */
	   tmpnam(nomitmp[6]);
	   if ((fp = fopen(nomitmp[6],"w")) == NULL)
	   {
	      scrivi_messaggio("Can't open temporary file!");
	      return;
	   }
	
	   funz_editor[0].parametro = 6;  /* indice array */
	
	   genmodD1(fp, nome_nuovo_modulo, descr_nuovo_modulo, variabili, 
			numero_variabili, num_var_stato, num_var_algebriche,
			jactoggstruct, str_signeq, str_uniteq, str_cosnor);
	
	   fclose(fp);
	
	   weditor[6] = (Widget) editor(UxTopLevel, &wtxteditor, nomitmp[6], False,
	                                font, funz_editor, geom_editor);
	   cstring = CREATE_CSTRING("VIEW SUBROUTINE D1 (EQUATION SECTION)");
	   set_something_val(weditor[6], XmNdialogTitle, (XtArgVal) cstring);
	   XmStringFree(cstring);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_opt_stato( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	cambia_tipo_text(UxGetWidget(nmod_bboard), K_OPT_STATO, True);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_opt_ingresso( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	cambia_tipo_text(UxGetWidget(nmod_bboard), K_OPT_ALGEBRAIC, True);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_opt_out( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	cambia_tipo_text(UxGetWidget(nmod_bboard), K_OPT_INGRESSO, True);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_opt_all( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	cambia_tipo_text(UxGetWidget(nmod_bboard), K_OPT_ALL, True);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	activateCB_opt_dati( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	cambia_tipo_text(UxGetWidget(nmod_bboard), K_OPT_DATI,True);
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

static void	createCB_nmod_bboard( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnmod_app_shell      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNmod_app_shellContext;
	UxNmod_app_shellContext = UxContext =
			(_UxCnmod_app_shell *) UxGetContext( UxWidget );
	{
	
	}
	UxNmod_app_shellContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_nmod_app_shell()
{
	Widget	mb_nmod_file_shell;
	Widget	mb_nmod_config_shell;
	Widget	mb_nmod_user_shell;
	Widget	mb_nmod_view_shell;
	Widget	nmod_menu_p1_shell;

	nmod_app_shell = XtVaCreatePopupShell( "nmod_app_shell",
			applicationShellWidgetClass, UxTopLevel,
			XmNkeyboardFocusPolicy, XmEXPLICIT,
			XmNgeometry, "+115+215",
			XmNtitle, "NEWMOD - NEW MODULE DEFINITION",
			XmNheight, 600,
			XmNwidth, 650,
			XmNy, 58,
			XmNx, 172,
			NULL );

	UxPutContext( nmod_app_shell, (char *) UxNmod_app_shellContext );

	nmod_form = XtVaCreateManagedWidget( "nmod_form",
			xmFormWidgetClass, nmod_app_shell,
			XmNheight, 100,
			XmNwidth, 100,
			XmNy, 10,
			XmNx, 20,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nmod_form, (char *) UxNmod_app_shellContext );

	nmod_pb_ok = XtVaCreateManagedWidget( "nmod_pb_ok",
			xmPushButtonWidgetClass, nmod_form,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_NONE,
			XmNleftOffset, 10,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 530,
			XmNx, 20,
			NULL );

	UxPutContext( nmod_pb_ok, (char *) UxNmod_app_shellContext );

	rowColumn1 = XtVaCreateManagedWidget( "rowColumn1",
			xmRowColumnWidgetClass, nmod_form,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_TIGHT,
			XmNspacing, 20,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 30,
			XmNwidth, 580,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( rowColumn1, (char *) UxNmod_app_shellContext );

	nmod_menubar = XtVaCreateManagedWidget( "nmod_menubar",
			xmRowColumnWidgetClass, rowColumn1,
			XmNspacing, 20,
			XmNheight, 30,
			XmNy, 0,
			XmNx, 0,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( nmod_menubar, (char *) UxNmod_app_shellContext );

	mb_nmod_file_shell = XtVaCreatePopupShell ("mb_nmod_file_shell",
			xmMenuShellWidgetClass, nmod_menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_nmod_file = XtVaCreateWidget( "mb_nmod_file",
			xmRowColumnWidgetClass, mb_nmod_file_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_nmod_file, (char *) UxNmod_app_shellContext );

	pb_nmod_save = XtVaCreateManagedWidget( "pb_nmod_save",
			xmPushButtonWidgetClass, mb_nmod_file,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Save and Generate FTN" ),
			NULL );

	UxPutContext( pb_nmod_save, (char *) UxNmod_app_shellContext );

	pb_nmod_quit = XtVaCreateManagedWidget( "pb_nmod_quit",
			xmPushButtonWidgetClass, mb_nmod_file,
			RES_CONVERT( XmNmnemonic, "Q" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );

	UxPutContext( pb_nmod_quit, (char *) UxNmod_app_shellContext );

	menu1_top_b3 = XtVaCreateManagedWidget( "menu1_top_b3",
			xmCascadeButtonGadgetClass, nmod_menubar,
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, mb_nmod_file,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b3, (char *) UxNmod_app_shellContext );

	mb_nmod_config_shell = XtVaCreatePopupShell ("mb_nmod_config_shell",
			xmMenuShellWidgetClass, nmod_menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_nmod_config = XtVaCreateWidget( "mb_nmod_config",
			xmRowColumnWidgetClass, mb_nmod_config_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_nmod_config, (char *) UxNmod_app_shellContext );

	pb_nmod_varsetup = XtVaCreateManagedWidget( "pb_nmod_varsetup",
			xmPushButtonWidgetClass, mb_nmod_config,
			RES_CONVERT( XmNmnemonic, "v" ),
			RES_CONVERT( XmNlabelString, "I/O variables setup" ),
			NULL );

	UxPutContext( pb_nmod_varsetup, (char *) UxNmod_app_shellContext );

	pb_nmod_jactopology = XtVaCreateManagedWidget( "pb_nmod_jactopology",
			xmPushButtonWidgetClass, mb_nmod_config,
			RES_CONVERT( XmNmnemonic, "t" ),
			RES_CONVERT( XmNlabelString, "Jacobian matrix topology" ),
			NULL );

	UxPutContext( pb_nmod_jactopology, (char *) UxNmod_app_shellContext );

	menu1_top_b4 = XtVaCreateManagedWidget( "menu1_top_b4",
			xmCascadeButtonGadgetClass, nmod_menubar,
			XmNsubMenuId, mb_nmod_config,
			RES_CONVERT( XmNmnemonic, "M" ),
			RES_CONVERT( XmNlabelString, "Module configuration" ),
			NULL );

	UxPutContext( menu1_top_b4, (char *) UxNmod_app_shellContext );

	mb_nmod_user_shell = XtVaCreatePopupShell ("mb_nmod_user_shell",
			xmMenuShellWidgetClass, nmod_menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_nmod_user = XtVaCreateWidget( "mb_nmod_user",
			xmRowColumnWidgetClass, mb_nmod_user_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_nmod_user, (char *) UxNmod_app_shellContext );

	pb_nmod_I2 = XtVaCreateManagedWidget( "pb_nmod_I2",
			xmPushButtonWidgetClass, mb_nmod_user,
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Data  Section (I2)" ),
			NULL );

	UxPutContext( pb_nmod_I2, (char *) UxNmod_app_shellContext );

	pb_nmod_JC = XtVaCreateManagedWidget( "pb_nmod_JC",
			xmPushButtonWidgetClass, mb_nmod_user,
			RES_CONVERT( XmNmnemonic, "J" ),
			RES_CONVERT( XmNlabelString, "Jacobian Section (JC)" ),
			NULL );

	UxPutContext( pb_nmod_JC, (char *) UxNmod_app_shellContext );

	pb_nmod_residual = XtVaCreateManagedWidget( "pb_nmod_residual",
			xmPushButtonWidgetClass, mb_nmod_user,
			RES_CONVERT( XmNmnemonic, "R" ),
			RES_CONVERT( XmNlabelString, "Residuals Section" ),
			NULL );

	UxPutContext( pb_nmod_residual, (char *) UxNmod_app_shellContext );

	pb_nmod_D1 = XtVaCreateManagedWidget( "pb_nmod_D1",
			xmPushButtonWidgetClass, mb_nmod_user,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Equations Section (D1)" ),
			NULL );

	UxPutContext( pb_nmod_D1, (char *) UxNmod_app_shellContext );

	nmod_menubar_top_b1 = XtVaCreateManagedWidget( "nmod_menubar_top_b1",
			xmCascadeButtonGadgetClass, nmod_menubar,
			XmNsubMenuId, mb_nmod_user,
			RES_CONVERT( XmNmnemonic, "U" ),
			RES_CONVERT( XmNlabelString, "User" ),
			NULL );

	UxPutContext( nmod_menubar_top_b1, (char *) UxNmod_app_shellContext );

	mb_nmod_view_shell = XtVaCreatePopupShell ("mb_nmod_view_shell",
			xmMenuShellWidgetClass, nmod_menubar,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	mb_nmod_view = XtVaCreateWidget( "mb_nmod_view",
			xmRowColumnWidgetClass, mb_nmod_view_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( mb_nmod_view, (char *) UxNmod_app_shellContext );

	pb_nmod_vI2 = XtVaCreateManagedWidget( "pb_nmod_vI2",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Data Section (I2)" ),
			NULL );

	UxPutContext( pb_nmod_vI2, (char *) UxNmod_app_shellContext );

	pb_nmod_vI3 = XtVaCreateManagedWidget( "pb_nmod_vI3",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Variable List Section (I3)" ),
			NULL );

	UxPutContext( pb_nmod_vI3, (char *) UxNmod_app_shellContext );

	pb_nmod_vI4 = XtVaCreateManagedWidget( "pb_nmod_vI4",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Subroutine I4" ),
			NULL );

	UxPutContext( pb_nmod_vI4, (char *) UxNmod_app_shellContext );

	pb_nmod_vC1 = XtVaCreateManagedWidget( "pb_nmod_vC1",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Residual Evaluation (C1)" ),
			NULL );

	UxPutContext( pb_nmod_vC1, (char *) UxNmod_app_shellContext );

	pb_nmod_vJC = XtVaCreateManagedWidget( "pb_nmod_vJC",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Jacobian section (JC)" ),
			NULL );

	UxPutContext( pb_nmod_vJC, (char *) UxNmod_app_shellContext );

	pb_nmod_vMOD = XtVaCreateManagedWidget( "pb_nmod_vMOD",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Residual Section" ),
			NULL );

	UxPutContext( pb_nmod_vMOD, (char *) UxNmod_app_shellContext );

	pb_nmod_vD1 = XtVaCreateManagedWidget( "pb_nmod_vD1",
			xmPushButtonWidgetClass, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "Equations Section (D1)" ),
			NULL );

	UxPutContext( pb_nmod_vD1, (char *) UxNmod_app_shellContext );

	nmod_menubar_top_b2 = XtVaCreateManagedWidget( "nmod_menubar_top_b2",
			xmCascadeButtonGadgetClass, nmod_menubar,
			RES_CONVERT( XmNmnemonic, "V" ),
			XmNsubMenuId, mb_nmod_view,
			RES_CONVERT( XmNlabelString, "View code" ),
			NULL );

	UxPutContext( nmod_menubar_top_b2, (char *) UxNmod_app_shellContext );

	nmod_label_nome = XtVaCreateManagedWidget( "nmod_label_nome",
			xmLabelWidgetClass, nmod_form,
			XmNtopWidget, rowColumn1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 15,
			RES_CONVERT( XmNlabelString, "Module name:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 100,
			XmNy, 30,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_label_nome, (char *) UxNmod_app_shellContext );

	nmod_text_nome = XtVaCreateManagedWidget( "nmod_text_nome",
			xmTextWidgetClass, nmod_form,
			XmNtopWidget, rowColumn1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 4,
			XmNtopOffset, 10,
			XmNleftWidget, nmod_label_nome,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNcolumns, 4,
			XmNheight, 35,
			XmNwidth, 80,
			XmNy, 30,
			XmNx, 130,
			NULL );

	UxPutContext( nmod_text_nome, (char *) UxNmod_app_shellContext );

	nmod_label_descr = XtVaCreateManagedWidget( "nmod_label_descr",
			xmLabelWidgetClass, nmod_form,
			XmNtopWidget, rowColumn1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 15,
			XmNleftWidget, nmod_text_nome,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Description:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 30,
			XmNx, 230,
			NULL );

	UxPutContext( nmod_label_descr, (char *) UxNmod_app_shellContext );

	nmod_text_descr = XtVaCreateManagedWidget( "nmod_text_descr",
			xmTextWidgetClass, nmod_form,
			XmNtopWidget, rowColumn1,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmaxLength, 50,
			XmNtopOffset, 10,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, nmod_label_descr,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNcolumns, 50,
			XmNheight, 35,
			XmNwidth, 250,
			XmNy, 30,
			XmNx, 330,
			NULL );

	UxPutContext( nmod_text_descr, (char *) UxNmod_app_shellContext );

	nmod_sep1 = XtVaCreateManagedWidget( "nmod_sep1",
			xmSeparatorGadgetClass, nmod_form,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, nmod_label_nome,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 2,
			XmNwidth, 564,
			XmNy, 70,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_sep1, (char *) UxNmod_app_shellContext );

	nmod_menu_opt = XtVaCreateManagedWidget( "nmod_menu_opt",
			xmRowColumnWidgetClass, nmod_form,
			XmNwhichButton, 1,
			XmNspacing, 0,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, nmod_sep1,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNheight, 35,
			XmNwidth, 200,
			XmNy, 110,
			XmNx, 20,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nmod_menu_opt, (char *) UxNmod_app_shellContext );

	nmod_menu_p1_shell = XtVaCreatePopupShell ("nmod_menu_p1_shell",
			xmMenuShellWidgetClass, nmod_menu_opt,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nmod_menu_p1 = XtVaCreateWidget( "nmod_menu_p1",
			xmRowColumnWidgetClass, nmod_menu_p1_shell,
			XmNwidth, 200,
			XmNheight, 35,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nmod_menu_p1, (char *) UxNmod_app_shellContext );

	opt_stato = XtVaCreateManagedWidget( "opt_stato",
			xmPushButtonWidgetClass, nmod_menu_p1,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "State output variables" ),
			NULL );

	UxPutContext( opt_stato, (char *) UxNmod_app_shellContext );

	opt_ingresso = XtVaCreateManagedWidget( "opt_ingresso",
			xmPushButtonWidgetClass, nmod_menu_p1,
			RES_CONVERT( XmNmnemonic, "g" ),
			RES_CONVERT( XmNlabelString, "Algebraic output variables" ),
			NULL );

	UxPutContext( opt_ingresso, (char *) UxNmod_app_shellContext );

	opt_out = XtVaCreateManagedWidget( "opt_out",
			xmPushButtonWidgetClass, nmod_menu_p1,
			RES_CONVERT( XmNmnemonic, "I" ),
			RES_CONVERT( XmNlabelString, "Input variables" ),
			NULL );

	UxPutContext( opt_out, (char *) UxNmod_app_shellContext );

	opt_all = XtVaCreateManagedWidget( "opt_all",
			xmPushButtonWidgetClass, nmod_menu_p1,
			RES_CONVERT( XmNmnemonic, "A" ),
			RES_CONVERT( XmNlabelString, "All variables" ),
			NULL );

	UxPutContext( opt_all, (char *) UxNmod_app_shellContext );

	nmod_menu_p1_b6 = XtVaCreateManagedWidget( "nmod_menu_p1_b6",
			xmSeparatorWidgetClass, nmod_menu_p1,
			NULL );

	UxPutContext( nmod_menu_p1_b6, (char *) UxNmod_app_shellContext );

	opt_dati = XtVaCreateManagedWidget( "opt_dati",
			xmPushButtonWidgetClass, nmod_menu_p1,
			RES_CONVERT( XmNmnemonic, "d" ),
			RES_CONVERT( XmNlabelString, "Geometrical data" ),
			NULL );

	UxPutContext( opt_dati, (char *) UxNmod_app_shellContext );

	nmod_menu1 = XtVaCreateManagedWidget( "nmod_menu1",
			xmRowColumnWidgetClass, nmod_menu_opt,
			XmNwhichButton, 1,
			XmNspacing, 0,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNwidth, 200,
			XmNheight, 35,
			XmNy, -10,
			XmNx, 0,
			XmNsubMenuId, nmod_menu_p1,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( nmod_menu1, (char *) UxNmod_app_shellContext );

	nmod_sep2 = XtVaCreateManagedWidget( "nmod_sep2",
			xmSeparatorGadgetClass, nmod_form,
			XmNtopWidget, nmod_menu_opt,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 7,
			XmNwidth, 564,
			XmNy, 150,
			XmNx, 10,
			NULL );

	UxPutContext( nmod_sep2, (char *) UxNmod_app_shellContext );

	nmod_label_tit1 = XtVaCreateManagedWidget( "nmod_label_tit1",
			xmLabelWidgetClass, nmod_form,
			XmNtopWidget, nmod_sep2,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Description" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 140,
			XmNy, 210,
			XmNx, 20,
			NULL );

	UxPutContext( nmod_label_tit1, (char *) UxNmod_app_shellContext );

	nmod_label_tit2 = XtVaCreateManagedWidget( "nmod_label_tit2",
			xmLabelWidgetClass, nmod_form,
			XmNtopWidget, nmod_sep2,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "FORTRAN symbol" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNy, 210,
			XmNx, 270,
			NULL );

	UxPutContext( nmod_label_tit2, (char *) UxNmod_app_shellContext );

	nmod_label_tit3 = XtVaCreateManagedWidget( "nmod_label_tit3",
			xmLabelWidgetClass, nmod_form,
			XmNtopWidget, nmod_sep2,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "normalization value" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 130,
			XmNy, 210,
			XmNx, 420,
			NULL );

	UxPutContext( nmod_label_tit3, (char *) UxNmod_app_shellContext );

	nmod_scroll_window = XtVaCreateManagedWidget( "nmod_scroll_window",
			xmScrolledWindowWidgetClass, nmod_form,
			XmNtopWidget, nmod_label_tit1,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, nmod_pb_ok,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNheight, 327,
			XmNwidth, 564,
			XmNy, 200,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( nmod_scroll_window, (char *) UxNmod_app_shellContext );

	nmod_bboard = XtVaCreateManagedWidget( "nmod_bboard",
			xmBulletinBoardWidgetClass, nmod_scroll_window,
			XmNheight, 1,
			XmNwidth, 1,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_ANY,
			NULL );

	UxPutContext( nmod_bboard, (char *) UxNmod_app_shellContext );

	createCB_nmod_bboard( nmod_bboard,
			(XtPointer) UxNmod_app_shellContext, (XtPointer) NULL );

	XtAddCallback( nmod_app_shell, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( nmod_pb_ok, XmNactivateCallback,
			activateCB_nmod_pb_ok,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_save, XmNactivateCallback,
			activateCB_pb_nmod_save,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_quit, XmNactivateCallback,
			activateCB_pb_nmod_quit,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_varsetup, XmNactivateCallback,
			activateCB_pb_nmod_varsetup,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_jactopology, XmNactivateCallback,
			activateCB_pb_nmod_jactopology,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_I2, XmNactivateCallback,
			activateCB_pb_nmod_I2,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_JC, XmNactivateCallback,
			activateCB_pb_nmod_JC,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_residual, XmNactivateCallback,
			activateCB_pb_nmod_residual,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_D1, XmNactivateCallback,
			activateCB_pb_nmod_D1,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vI2, XmNactivateCallback,
			activateCB_pb_nmod_vI2,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vI3, XmNactivateCallback,
			activateCB_pb_nmod_vI3,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vI4, XmNactivateCallback,
			activateCB_pb_nmod_vI4,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vC1, XmNactivateCallback,
			activateCB_pb_nmod_vC1,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vJC, XmNactivateCallback,
			activateCB_pb_nmod_vJC,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vMOD, XmNactivateCallback,
			activateCB_pb_nmod_vMOD,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( pb_nmod_vD1, XmNactivateCallback,
			activateCB_pb_nmod_vD1,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( nmod_text_nome, XmNmodifyVerifyCallback,
			text_maius_callback,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( opt_stato, XmNactivateCallback,
			activateCB_opt_stato,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( opt_ingresso, XmNactivateCallback,
			activateCB_opt_ingresso,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( opt_out, XmNactivateCallback,
			activateCB_opt_out,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( opt_all, XmNactivateCallback,
			activateCB_opt_all,
			(XtPointer) UxNmod_app_shellContext );

	XtAddCallback( opt_dati, XmNactivateCallback,
			activateCB_opt_dati,
			(XtPointer) UxNmod_app_shellContext );

	XtVaSetValues(nmod_menu1,
			XmNmenuHistory, opt_all,
			NULL );



	return ( nmod_app_shell );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_nmod_shell( _Uxmodifica )
	Boolean	_Uxmodifica;
{
	Widget                  rtrn;
	_UxCnmod_app_shell      *UxContext;

	UxNmod_app_shellContext = UxContext =
		(_UxCnmod_app_shell *) XtMalloc( sizeof(_UxCnmod_app_shell) );

	modifica = _Uxmodifica;
	{
		/* carica i widget di testo relativi alle equazioni di stato */
		/* (il default iniziale dell'option menu). */
		option_scelto = K_OPT_ALL;
		
		_nmod_varsetup = UxGetWidget(pb_nmod_varsetup);
		
		/* inizializzazione delle variabili */
		num_var_stato = 0;
		num_var_algebriche = 0;
		num_var_ingresso = 0;
		num_dati_geometrici = 0;
		
		scrollw_label1 = NULL;
		rtrn = _Uxbuild_nmod_app_shell();

		UxPopupInterface(rtrn, no_grab);
		
		if (modifica)
		{
		   XmTextSetString(UxGetWidget(nmod_text_nome), nome_modulo);
		   XmTextSetString(UxGetWidget(nmod_text_descr), descr_modulo);
		   if (leggi_file_interfaccia() != 0)
		   {
		      sprintf(message,"Cannot read NEWMOD information (check permission).");
		      attention_wdg = (Widget) attention(UxTopLevel,message,MAPPA,
		                                         geom_attention);
		   }
		
		   nome_nuovo_modulo = XmTextGetString(UxGetWidget(nmod_text_nome));
		   descr_nuovo_modulo = XmTextGetString(UxGetWidget(nmod_text_descr));
		   nmod_def_initialized = True;
		   set_something_val (UxGetWidget(pb_nmod_JC),XmNsensitive,
		                  (jacobian_type == NUMERICAL) ? False : True);
		}
		else
		{
		   alloca_text_widget(UxGetWidget(nmod_bboard),5, 5, 5, 5);
		/* jacobiano di tipo ANALITICO per default */
		   jacobian_type = ANALYTICAL;
		
		   menu_newmod_init();
		}
		
		posy_text = 10;
		bboard_flag = False;
		
		nmod_def_managed = True;
		
		non_salvato = False;
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_nmod_shell( _Uxmodifica )
	Boolean	_Uxmodifica;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_nmod_shell( _Uxmodifica );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

