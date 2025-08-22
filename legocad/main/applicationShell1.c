
/*******************************************************************************
	applicationShell1.c
	(Generated from interface file applicationShell1.i)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/ArrowBG.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/CascadeBG.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/PushBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo applicationShell1.i
   tipo 
   release 2.28
   data 5/17/95
   reserved @(#)applicationShell1.i	2.28
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <libutilx.h>
#ifdef HYPERHELP_USED
#include "winhelp.h"
#endif
#include "legomain.h"
#include "lc_errore.h"
ERR_LEVEL err_level;
char *getcwd();
char *getenv();
extern swidget create_dati_selectionD();
extern swidget create_vis_msg();
extern swidget create_questionUscitaLegocad();
extern swidget popup_EnvironmentDialog();
extern swidget create_question_environment();

/* Missing function declarations */
extern int test_transient();
extern int test_steady();
extern int test_data_editor();
extern int test_topologia();
extern void set_pixmap_cursor(Widget, char *);
extern int proc_legocad_attivi();
extern void attiva_topology();
extern void esegui_crealg1();
extern void attiva_data();
extern void esegui_crealg3();
extern void attiva_steady();
extern void esegui_crealg5sk();
extern void attiva_transient();
extern void attiva_librarian();
extern void attiva_graphics();
extern void attiva_tables();
extern void attiva_autodoc();
extern void testa_ambiente();
extern void chdir_path_legocad();
swidget w_selectionD;
int stato; /* stato di selezione del modello */
int tipo_modello; /* specifica se modello di solo processo
		     regolazione o misto regolaz. + processo */
Bool grafica_on=1; /* indica che si desidera lavorare anche con i diagrammi
                     simbolici dei modelli */ 
char path_user[FILENAME_MAX]; /* path name che individua l'utente legocad */
char path_legocad[FILENAME_MAX]; /* path_name che individua la sottodirectory
                           legocad per l'utente */
char path_libut[FILENAME_MAX];  /* path_name che individua la libreria dei moduli
			  di processo */
char path_libreg[FILENAME_MAX]; /* path_name che individua la libreria dei moduli
                             di regolazione */
void chiudi_prog_legocad();
Bool ok_path_legocad;
Bool ok_path_libut;
Bool ok_path_libreg;
Bool ok_libut;
Bool ok_libreg;
char path_modello[FILENAME_MAX]; /* path name del modello */
char appoggio[FILENAME_MAX];

char *path_icone;  /* PATH directory in cui risiedono le
                      icone utilizzate da LEGOCAD; il direttorio
                      e' specificato dalla variabile di ambiente
                      LEGOCAD_ICO
                   */
#ifdef DESIGN_TIME
char buf_path_icone[FILENAME_MAX];
#endif
/*
 nomi (completi di path) delle icone da caricare per rappresentare
 i bottoni con le varie fasi del progetto
*/
char icon[FILENAME_MAX];
/*
  PID relativi alle attivita' per la
  costruzione del modello: vengono utilizzati
  per sapere se tali attivita' sono ancora attive
*/
pid_t pid_topology,pid_data,pid_steady,pid_transient,pid_librarian;

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxapplicationShell1MenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	Boolean		*ctd;
{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, (XButtonPressedEvent *)event );
		XtManageChild( menu );
	}
}

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
	Widget	UxmainWindow1;
	Widget	UxEdit;
	Widget	Uxmenu1_p1;
	Widget	UxProcessoButton;
	Widget	UxControlloButton;
	Widget	UxMixedButton;
	Widget	Uxsep1;
	Widget	UxOrderingButton;
	Widget	Uxsep2;
	Widget	UxExitButton;
	Widget	Uxmenu1_top_b1;
	Widget	UxEditModels;
	Widget	UxCopyModelButton;
	Widget	UxTraining;
	Widget	UxTraining_b1;
	Widget	UxExamplesButton;
	Widget	Uxmenu1_top_b2;
	Widget	Uxmenu_p4;
	Widget	UxOption_pane;
	Widget	UxGrafica;
	Widget	UxListeButton;
	Widget	Uxmenu_p4_b2;
	Widget	UxEdit_top_b1;
	Widget	UxUtilities;
	Widget	UxEdi14_utils;
	Widget	UxEdi14_run;
	Widget	UxEdi14_save;
	Widget	UxEdi14_viewout;
	Widget	UxUtilities_b1;
	Widget	UxSnaf14_utils;
	Widget	UxSnaf14_run;
	Widget	UxSnaf14_view;
	Widget	UxSnaf14_print;
	Widget	UxUtilities_b2;
	Widget	UxForaus_utils;
	Widget	UxEdit_foraus;
	Widget	Uxforaus_copy;
	Widget	Uxforaus_manage;
	Widget	UxForaus_edit;
	Widget	UxForaus_print;
	Widget	UxForaus_delete;
	Widget	UxUtilities_b3;
	Widget	UxEdit_top_b2;
	Widget	Uxhelp_pane;
	Widget	Uxhelp_contents;
	Widget	Uxhelp_search;
	Widget	Uxhelp_on;
	Widget	UxEdit_p11_b4;
	Widget	Uxhelp_on_help;
	Widget	UxEdit_p11_b6;
	Widget	Uxhelp_about;
	Widget	UxEdit_top_b3;
	Widget	UxpanedWindow1;
	Widget	Uxform2;
	Widget	Uxlabel1;
	Widget	Uxlabel2;
	Widget	UxLegocadPathLabel;
	Widget	Uxlabel4;
	Widget	Uxlabel5;
	Widget	UxModuliLibLabel;
	Widget	UxRegLibLabel;
	Widget	Uxform1;
	Widget	UxlabelCostruzione;
	Widget	UxrowColumn5;
	Widget	UxTopologia_frame3;
	Widget	UxTopologia_pb5;
	Widget	UxarrowButtonGadget10;
	Widget	UxDati_frame3;
	Widget	UxDati_pb5;
	Widget	UxarrowButtonGadget11;
	Widget	UxStazionario_frame3;
	Widget	UxStazionario_pb5;
	Widget	UxarrowButtonGadget12;
	Widget	UxTransitorio_frame3;
	Widget	UxTransitorio_pb5;
	Widget	Uxform3;
	Widget	UxlabelUtilita;
	Widget	UxrowColumn2;
	Widget	UxLibrarian_frame3;
	Widget	UxLibrarian_rc;
	Widget	UxLibrarian_pb1;
	Widget	UxLibrarian_pb2;
	Widget	UxGrafica_frame;
	Widget	UxGrafica_rc;
	Widget	UxGrafica_pb1;
	Widget	UxGrafica_pb2;
	Widget	UxTavole_frame;
	Widget	UxTavole_rc;
	Widget	UxTavole_pb1;
	Widget	UxTavole_pb2;
	Widget	UxDocumentazione_frame;
	Widget	UxDocumentazione_rc;
	Widget	UxDocumentazione_pb1;
	Widget	UxDocumentazione_pb2;
	Widget	Uxform5;
	Widget	UxlabelMessaggi;
	Widget	UxscrolledWindow3;
	Widget	Uxmessage_menu;
	Widget	Uxmenu1_p3_title;
	Widget	Uxmessage_menu_p1_b2;
	Widget	Uxmenu1_p3_b1;
	Widget	Uxwsel;
} _UxCapplicationShell1;

#define mainWindow1             UxApplicationShell1Context->UxmainWindow1
#define Edit                    UxApplicationShell1Context->UxEdit
#define menu1_p1                UxApplicationShell1Context->Uxmenu1_p1
#define ProcessoButton          UxApplicationShell1Context->UxProcessoButton
#define ControlloButton         UxApplicationShell1Context->UxControlloButton
#define MixedButton             UxApplicationShell1Context->UxMixedButton
#define sep1                    UxApplicationShell1Context->Uxsep1
#define OrderingButton          UxApplicationShell1Context->UxOrderingButton
#define sep2                    UxApplicationShell1Context->Uxsep2
#define ExitButton              UxApplicationShell1Context->UxExitButton
#define menu1_top_b1            UxApplicationShell1Context->Uxmenu1_top_b1
#define EditModels              UxApplicationShell1Context->UxEditModels
#define CopyModelButton         UxApplicationShell1Context->UxCopyModelButton
#define Training                UxApplicationShell1Context->UxTraining
#define Training_b1             UxApplicationShell1Context->UxTraining_b1
#define ExamplesButton          UxApplicationShell1Context->UxExamplesButton
#define menu1_top_b2            UxApplicationShell1Context->Uxmenu1_top_b2
#define menu_p4                 UxApplicationShell1Context->Uxmenu_p4
#define Option_pane             UxApplicationShell1Context->UxOption_pane
#define Grafica                 UxApplicationShell1Context->UxGrafica
#define ListeButton             UxApplicationShell1Context->UxListeButton
#define menu_p4_b2              UxApplicationShell1Context->Uxmenu_p4_b2
#define Edit_top_b1             UxApplicationShell1Context->UxEdit_top_b1
#define Utilities               UxApplicationShell1Context->UxUtilities
#define Edi14_utils             UxApplicationShell1Context->UxEdi14_utils
#define Edi14_run               UxApplicationShell1Context->UxEdi14_run
#define Edi14_save              UxApplicationShell1Context->UxEdi14_save
#define Edi14_viewout           UxApplicationShell1Context->UxEdi14_viewout
#define Utilities_b1            UxApplicationShell1Context->UxUtilities_b1
#define Snaf14_utils            UxApplicationShell1Context->UxSnaf14_utils
#define Snaf14_run              UxApplicationShell1Context->UxSnaf14_run
#define Snaf14_view             UxApplicationShell1Context->UxSnaf14_view
#define Snaf14_print            UxApplicationShell1Context->UxSnaf14_print
#define Utilities_b2            UxApplicationShell1Context->UxUtilities_b2
#define Foraus_utils            UxApplicationShell1Context->UxForaus_utils
#define Edit_foraus             UxApplicationShell1Context->UxEdit_foraus
#define foraus_copy             UxApplicationShell1Context->Uxforaus_copy
#define foraus_manage           UxApplicationShell1Context->Uxforaus_manage
#define Foraus_edit             UxApplicationShell1Context->UxForaus_edit
#define Foraus_print            UxApplicationShell1Context->UxForaus_print
#define Foraus_delete           UxApplicationShell1Context->UxForaus_delete
#define Utilities_b3            UxApplicationShell1Context->UxUtilities_b3
#define Edit_top_b2             UxApplicationShell1Context->UxEdit_top_b2
#define help_pane               UxApplicationShell1Context->Uxhelp_pane
#define help_contents           UxApplicationShell1Context->Uxhelp_contents
#define help_search             UxApplicationShell1Context->Uxhelp_search
#define help_on                 UxApplicationShell1Context->Uxhelp_on
#define Edit_p11_b4             UxApplicationShell1Context->UxEdit_p11_b4
#define help_on_help            UxApplicationShell1Context->Uxhelp_on_help
#define Edit_p11_b6             UxApplicationShell1Context->UxEdit_p11_b6
#define help_about              UxApplicationShell1Context->Uxhelp_about
#define Edit_top_b3             UxApplicationShell1Context->UxEdit_top_b3
#define panedWindow1            UxApplicationShell1Context->UxpanedWindow1
#define form2                   UxApplicationShell1Context->Uxform2
#define label1                  UxApplicationShell1Context->Uxlabel1
#define label2                  UxApplicationShell1Context->Uxlabel2
#define LegocadPathLabel        UxApplicationShell1Context->UxLegocadPathLabel
#define label4                  UxApplicationShell1Context->Uxlabel4
#define label5                  UxApplicationShell1Context->Uxlabel5
#define ModuliLibLabel          UxApplicationShell1Context->UxModuliLibLabel
#define RegLibLabel             UxApplicationShell1Context->UxRegLibLabel
#define form1                   UxApplicationShell1Context->Uxform1
#define labelCostruzione        UxApplicationShell1Context->UxlabelCostruzione
#define rowColumn5              UxApplicationShell1Context->UxrowColumn5
#define Topologia_frame3        UxApplicationShell1Context->UxTopologia_frame3
#define Topologia_pb5           UxApplicationShell1Context->UxTopologia_pb5
#define arrowButtonGadget10     UxApplicationShell1Context->UxarrowButtonGadget10
#define Dati_frame3             UxApplicationShell1Context->UxDati_frame3
#define Dati_pb5                UxApplicationShell1Context->UxDati_pb5
#define arrowButtonGadget11     UxApplicationShell1Context->UxarrowButtonGadget11
#define Stazionario_frame3      UxApplicationShell1Context->UxStazionario_frame3
#define Stazionario_pb5         UxApplicationShell1Context->UxStazionario_pb5
#define arrowButtonGadget12     UxApplicationShell1Context->UxarrowButtonGadget12
#define Transitorio_frame3      UxApplicationShell1Context->UxTransitorio_frame3
#define Transitorio_pb5         UxApplicationShell1Context->UxTransitorio_pb5
#define form3                   UxApplicationShell1Context->Uxform3
#define labelUtilita            UxApplicationShell1Context->UxlabelUtilita
#define rowColumn2              UxApplicationShell1Context->UxrowColumn2
#define Librarian_frame3        UxApplicationShell1Context->UxLibrarian_frame3
#define Librarian_rc            UxApplicationShell1Context->UxLibrarian_rc
#define Librarian_pb1           UxApplicationShell1Context->UxLibrarian_pb1
#define Librarian_pb2           UxApplicationShell1Context->UxLibrarian_pb2
#define Grafica_frame           UxApplicationShell1Context->UxGrafica_frame
#define Grafica_rc              UxApplicationShell1Context->UxGrafica_rc
#define Grafica_pb1             UxApplicationShell1Context->UxGrafica_pb1
#define Grafica_pb2             UxApplicationShell1Context->UxGrafica_pb2
#define Tavole_frame            UxApplicationShell1Context->UxTavole_frame
#define Tavole_rc               UxApplicationShell1Context->UxTavole_rc
#define Tavole_pb1              UxApplicationShell1Context->UxTavole_pb1
#define Tavole_pb2              UxApplicationShell1Context->UxTavole_pb2
#define Documentazione_frame    UxApplicationShell1Context->UxDocumentazione_frame
#define Documentazione_rc       UxApplicationShell1Context->UxDocumentazione_rc
#define Documentazione_pb1      UxApplicationShell1Context->UxDocumentazione_pb1
#define Documentazione_pb2      UxApplicationShell1Context->UxDocumentazione_pb2
#define form5                   UxApplicationShell1Context->Uxform5
#define labelMessaggi           UxApplicationShell1Context->UxlabelMessaggi
#define scrolledWindow3         UxApplicationShell1Context->UxscrolledWindow3
#define message_menu            UxApplicationShell1Context->Uxmessage_menu
#define menu1_p3_title          UxApplicationShell1Context->Uxmenu1_p3_title
#define message_menu_p1_b2      UxApplicationShell1Context->Uxmessage_menu_p1_b2
#define menu1_p3_b1             UxApplicationShell1Context->Uxmenu1_p3_b1
#define wsel                    UxApplicationShell1Context->Uxwsel

static _UxCapplicationShell1	*UxApplicationShell1Context;

Widget	applicationShell1;
Widget	Topologia_rc;
Widget	Topologia_pb6;
Widget	Dati_rc;
Widget	Dati_pb6;
Widget	Stazionario_rc;
Widget	Stazionario_pb6;
Widget	Transitorio_rc;
Widget	Transitorio_pb6;
Widget	labelNomeModello;
Widget	messageST;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_applicationShell1();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

int help_not_available_msg()
{
   extern swidget create_vis_msg();
   char msg[100];

   sprintf(msg,"help not available for %s\n",getenv("OS") );
   create_vis_msg(msg);
}

int reset_wsel()
{
   wsel = NULL;
}  

/* update_pulldown()
   a seconda della situazione dell'ambiente LEGOCAD
   aggiorna l'attivazione delle voci del pulldown menu.
*/
int update_pulldown()
{
/*
 la libreria moduli processo esiste: posso costruire modelli
 di processo
*/
if(ok_libut)
	UxPutSensitive(ProcessoButton,"true");
else
	UxPutSensitive(ProcessoButton,"false");
/*
 la libreria moduli regolazione esiste: posso costruire modelli
 di regolazione
*/
if(ok_libreg)
{
	UxPutSensitive(ControlloButton,"true"); 
	UxPutSensitive(MixedButton,"true"); 
}
else
{
	UxPutSensitive(ControlloButton,"false");
	UxPutSensitive(MixedButton,"false");
}

UxPutSensitive(Edi14_run,"false");
UxPutSensitive(Edi14_save,"false");
UxPutSensitive(Edi14_viewout,"false");
}
	

/* aggiorna_attivi()
   in base allo stato di avanzamento di creazione del modello lego
   rende attivi o disattivi i bottoni relativi alle diverse fasi di
   sviluppo.
*/
int aggiorna_attivi()
{
#ifndef DESIGN_TIME
char app[100];
/*
 testa se e' attivabile il calcolo del transitorio
*/
if(test_transient()==0)
	{
                UxPutSensitive(Edi14_run,"true");
                UxPutSensitive(Edi14_save,"true");
                UxPutSensitive(Edi14_viewout,"true");

		UxPutSensitive(Transitorio_pb5,"true");
		UxPutSensitive(Transitorio_pb6,"true");
		UxPutBackground(Transitorio_rc,"LightBlue");
		UxPutBackground(Transitorio_pb6,"LightBlue");

		UxPutSensitive(Stazionario_pb5,"true");
		UxPutSensitive(Stazionario_pb6,"true");
		UxPutBackground(Stazionario_rc, "LightBlue");
		UxPutBackground(Stazionario_pb6, "LightBlue");

		UxPutSensitive(Dati_pb5,"true");
		UxPutSensitive(Dati_pb6,"true");
		UxPutBackground(Dati_rc,"LightBlue");
		UxPutBackground(Dati_pb6,"LightBlue");

		UxPutSensitive(Topologia_pb5,"true");
		UxPutSensitive(Topologia_pb6,"true");
		UxPutBackground(Topologia_rc,"LightBlue");
		UxPutBackground(Topologia_pb6,"LightBlue");
		
	}
else if (test_steady()==0)
	{
                UxPutSensitive(Edi14_run,"true");
                UxPutSensitive(Edi14_save,"true");
                UxPutSensitive(Edi14_viewout,"true");

		UxPutSensitive(Transitorio_pb5,"false");
		UxPutSensitive(Transitorio_pb6,"false");
		UxPutBackground(Transitorio_rc,"SteelBlue");
		UxPutBackground(Transitorio_pb6,"SteelBlue");
		

		UxPutSensitive(Stazionario_pb5,"true");
		UxPutSensitive(Stazionario_pb6,"true");
		UxPutBackground(Stazionario_rc,"LightBlue");
		UxPutBackground(Stazionario_pb6,"LightBlue");

		UxPutSensitive(Dati_pb5,"true");
		UxPutSensitive(Dati_pb6,"true");
		UxPutBackground(Dati_rc,"LightBlue");
		UxPutBackground(Dati_pb6,"LightBlue");

		UxPutSensitive(Topologia_pb5,"true");
		UxPutSensitive(Topologia_pb6,"true");
		UxPutBackground(Topologia_rc,"LightBlue");
		UxPutBackground(Topologia_pb6,"LightBlue");
	}
/*
 Testa la possibilta' di eseguire l'attivita' dati
*/
else if (test_data_editor()==0) 
	{
                UxPutSensitive(Edi14_run,"true");
                UxPutSensitive(Edi14_save,"true");
                UxPutSensitive(Edi14_viewout,"true");

		UxPutSensitive(Transitorio_pb5,"false");
		UxPutSensitive(Transitorio_pb6,"false");
		UxPutBackground(Transitorio_rc,"SteelBlue");
		UxPutBackground(Transitorio_pb6,"SteelBlue");
		

		UxPutSensitive(Stazionario_pb5,"false");
		UxPutSensitive(Stazionario_pb6,"false");
		UxPutBackground(Stazionario_rc,"SteelBlue");
		UxPutBackground(Stazionario_pb6,"SteelBlue");

		UxPutSensitive(Dati_pb5,"true");
		UxPutSensitive(Dati_pb6,"true");
		UxPutBackground(Dati_rc,"LightBlue");
		UxPutBackground(Dati_pb6,"LightBlue");

		UxPutSensitive(Topologia_pb5,"true");
		UxPutSensitive(Topologia_pb6,"true");
		UxPutBackground(Topologia_rc,"LightBlue");
		UxPutBackground(Topologia_pb6,"LightBlue");
	}
else if (test_topologia()==0)
        {
                UxPutSensitive(Edi14_run,"false");
                UxPutSensitive(Edi14_save,"false");
                UxPutSensitive(Edi14_viewout,"false");

                UxPutSensitive(Transitorio_pb5,"false");
                UxPutSensitive(Transitorio_pb6,"false");
                UxPutBackground(Transitorio_rc,"SteelBlue");
                UxPutBackground(Transitorio_pb6,"SteelBlue");


                UxPutSensitive(Stazionario_pb5,"false");
                UxPutSensitive(Stazionario_pb6,"false");
                UxPutBackground(Stazionario_rc,"SteelBlue");
                UxPutBackground(Stazionario_pb6,"SteelBlue");


                UxPutSensitive(Dati_pb5,"false");
                UxPutSensitive(Dati_pb6,"false");
                UxPutBackground(Dati_rc,"SteelBlue");
                UxPutBackground(Dati_pb6,"SteelBlue");


                UxPutSensitive(Topologia_pb5,"true");
                UxPutSensitive(Topologia_pb6,"true");
                UxPutBackground(Topologia_rc,"LightBlue");
                UxPutBackground(Topologia_pb6,"LightBlue");

        }
else 
	{
                UxPutSensitive(Edi14_run,"false");
                UxPutSensitive(Edi14_save,"false");
                UxPutSensitive(Edi14_viewout,"false");

		UxPutSensitive(Transitorio_pb5,"false");
		UxPutSensitive(Transitorio_pb6,"false");
		UxPutBackground(Transitorio_rc,"SteelBlue");
		UxPutBackground(Transitorio_pb6,"SteelBlue");
		

		UxPutSensitive(Stazionario_pb5,"false");
		UxPutSensitive(Stazionario_pb6,"false");
		UxPutBackground(Stazionario_rc,"SteelBlue");
		UxPutBackground(Stazionario_pb6,"SteelBlue");
		

		UxPutSensitive(Dati_pb5,"false");
		UxPutSensitive(Dati_pb6,"false");
		UxPutBackground(Dati_rc,"SteelBlue");
		UxPutBackground(Dati_pb6,"SteelBlue");
		

		UxPutSensitive(Topologia_pb5,"false");
		UxPutSensitive(Topologia_pb6,"false");
		UxPutBackground(Topologia_rc,"SteelBlue");
		UxPutBackground(Topologia_pb6,"SteelBlue");

        
        strcpy(app,path_libut);
        strcat(app,"   Not Compiled");
        UxPutLabelString(ModuliLibLabel,app);
        UxPutForeground(ModuliLibLabel,"Red");
		
	}
/*
 Rende attiva l'attivita' di documentazione automatica del modello
*/

UxPutSensitive(Documentazione_pb1,"true");
UxPutSensitive(Documentazione_pb2,"true");
UxPutBackground(Documentazione_rc,"LightBlue");
UxPutBackground(Documentazione_pb2,"LightBlue");

#endif
}

/*
 aggiorna la possibilita' di accesso alle routines di utilita'
*/
int aggiorna_utilities()
{
UxPutSensitive(Librarian_pb1,"true");
UxPutSensitive(Librarian_pb2,"true");
UxPutBackground(Librarian_pb2,"CadetBlue");
UxPutBackground(Librarian_rc,"CadetBlue");

if(ok_path_libut)
	{
		
	}
}


int agg_label_ambiente()
{
char app[FILENAME_MAX];
if(ok_path_legocad)
	{
	UxPutLabelString(LegocadPathLabel,path_legocad);
	UxPutForeground(LegocadPathLabel,"Black");
	}
else
	{
	UxPutLabelString(LegocadPathLabel,"Not Defined");
	UxPutForeground(LegocadPathLabel,"Red");
	}
if(ok_libut)
	{
	app[0]=0;
	strcpy(app,path_libut);
	strcat(app,"   Exist");
	UxPutLabelString(ModuliLibLabel,app);
	UxPutForeground(ModuliLibLabel,"Black");
	}
else if (ok_path_libut)
	{
	app[0]=0;
	strcpy(app,path_libut);
	strcat(app," - Not Present");
	UxPutLabelString(ModuliLibLabel,app);
	UxPutForeground(ModuliLibLabel,"Red");
	}
else
	{
	UxPutLabelString(ModuliLibLabel,"Not Defined");
	UxPutForeground(ModuliLibLabel,"Red");
	}
if(ok_libreg)
	{
	app[0]=0;
	strcpy(app,path_libreg);
	strcat(app,"   Exist");
	UxPutLabelString(RegLibLabel,app);
	UxPutForeground(RegLibLabel,"Black");
	}
else if (ok_path_libreg)
	{
	app[0]=0;
	strcpy(app,path_libreg);
	strcat(app,"   Not Present");
	UxPutLabelString(RegLibLabel,app);
	UxPutForeground(RegLibLabel,"Red");
	}
else
	{ 
	UxPutLabelString(RegLibLabel,"Not Defined");
	UxPutForeground(RegLibLabel,"Red");
	}
return(0);
}





int show_warning(mess)
char *mess;
{
  char   *str, *new_str;
  extern swidget messageST;

  str = UxGetText( messageST );
  new_str = (char*) malloc ( sizeof(char*) * ( strlen(mess)+strlen(str)+1 ) );
  strcpy( new_str, str );

  strcat( new_str, mess );
  UxPutText( messageST, new_str );
  XmTextShowPosition(UxGetWidget(messageST),strlen(new_str)-1);
  free( str );
  free( new_str );
}



int show_error(mess)
char *mess;
{
create_vis_msg(mess);
}

int set_icone()
{
#ifndef DESIGN_TIME
path_icone=getenv("LEGOCAD_ICO");
if(path_icone==NULL)
	{
	printf("\n Environment variable LEGOCAD_ICO\n is not defined");
	exit(1);
	}
#else
strcpy((char *)buf_path_icone,"./icons");
path_icone=buf_path_icone;
#endif

strcpy(icon,path_icone);
strcat(icon,"/topology.bm");
UxPutLabelInsensitivePixmap(Topologia_pb5,icon);
UxPutLabelPixmap(Topologia_pb5,icon);

strcpy(icon,path_icone);
strcat(icon,"/dati.bm");
UxPutLabelInsensitivePixmap(Dati_pb5,icon);
UxPutLabelPixmap(Dati_pb5,icon);

strcpy(icon,path_icone);
strcat(icon,"/stazionario.bm");
UxPutLabelInsensitivePixmap(Stazionario_pb5,icon);
UxPutLabelPixmap(Stazionario_pb5,icon);

strcpy(icon,path_icone);
strcat(icon,"/transitorio.bm");
UxPutLabelInsensitivePixmap(Transitorio_pb5,icon);
UxPutLabelPixmap(Transitorio_pb5,icon);

strcpy(icon,path_icone);
strcat(icon,"/librarian.bm");
UxPutLabelInsensitivePixmap(Librarian_pb1,icon);
UxPutLabelPixmap(Librarian_pb1,icon);

strcpy(icon,path_icone);
strcat(icon,"/grafica.bm");
UxPutLabelInsensitivePixmap(Grafica_pb1,icon);
UxPutLabelPixmap(Grafica_pb1,icon);

strcpy(icon,path_icone);
strcat(icon,"/tavole.bm");
UxPutLabelInsensitivePixmap(Tavole_pb1,icon);
UxPutLabelPixmap(Tavole_pb1,icon);

strcpy(icon,path_icone);
strcat(icon,"/documentazione.bm");
UxPutLabelInsensitivePixmap(Documentazione_pb1,icon);
UxPutLabelPixmap(Documentazione_pb1,icon);
}

int set_win_cursor(cursore)
char *cursore;
{

set_pixmap_cursor(UxGetWidget(applicationShell1), cursore );

}

int reset_win_cursor()
{

undef_cursore(UxGetWidget(applicationShell1));
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	popupCB_applicationShell1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_ProcessoButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	
	extern swidget SelModelsActivate();
	
	tipo_modello= TIPO_PROCESSO;
	
	if( wsel == NULL )
	   wsel = SelModelsActivate(SELECT_MODEL);
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_ControlloButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern swidget SelModelsActivate();
	tipo_modello= TIPO_REGOLAZIONE;
	/*
	SelModelsActivate();
	*/
	
	   SelModelsActivate(SELECT_MODEL);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_MixedButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern swidget SelModelsActivate();
	tipo_modello= TIPO_MIX;
	/*
	SelModelsActivate();
	*/
	   SelModelsActivate(SELECT_MODEL);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_OrderingButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern swidget SelModelsActivate();
	
	tipo_modello = TIPO_PROCESSO;
	if( wsel == NULL)
	   wsel = SelModelsActivate( REORDER_MODEL );	
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_ExitButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	char messaggio[FILENAME_MAX];
	swidget w;
	/* testa se vi sono processi legocad attivi */
	if(proc_legocad_attivi())
		w=create_questionUscitaLegocad("Legocad subprocesses already active\n Do You really want to exit LEGOCAD ?");
	else
		w=create_questionUscitaLegocad("Exit LEGOCAD?");
	UxPopupInterface(w,no_grab);
		
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_CopyModelButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	/*
	   extern swidget create_CopyModel();
	
	   create_CopyModel();
	
	   extern swidget create_form6();
	   swidget sw;
	
	   sw =create_form6();
	**/
	 
	   extern swidget create_CopyModel();
	   swidget sw;
	
	   sw = create_CopyModel();
	   UxPopupInterface(sw,no_grab);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Training_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	/*
	   extern swidget create_vis_msg();
	   create_vis_msg("Function not yet implemented");
	*/
	extern swidget create_TrainingModels();
	
	create_TrainingModels();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	valueChangedCB_Grafica( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	grafica_on=1;
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	valueChangedCB_ListeButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	grafica_on=0;
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	mapCB_Edi14_utils( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	FILE *fp;
	
	if( (fp = fopen("f14.dat","r")) == NULL)
	{
	   UxPutSensitive(Edi14_run,"false");
	   UxPutSensitive(Edi14_save,"false");
	   UxPutSensitive(Edi14_viewout,"false");
	}
	else
	{
	   UxPutSensitive(Edi14_run,"true");
	   UxPutSensitive(Edi14_save,"true");
	   UxPutSensitive(Edi14_viewout,"true");
	fclose(fp);
    }   
	
	
	
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Edi14_run( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	/*
	   questa callback deve attivare
	   l'edi 14 dopo avere selezionato
	   il file14 da utilizzare 
	
		create_vis_msg("Function development in progress");
	*/
	   extern swidget create_edi14FileSelection();
	   swidget filesel;
	   filesel = create_edi14FileSelection(EDI14_INPUT_SELECTION);
	   UxPopupInterface(filesel,no_grab);
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Edi14_save( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern swidget create_selectionBoxDialog1();
	swidget wid;
	
	wid = create_selectionBoxDialog1();
	UxPopupInterface(wid,no_grab);
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Edi14_viewout( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	   system("edit_f01 edi14.out 0");
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	mapCB_Snaf14_utils( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	FILE *fp;
	
	if( (fp = fopen("snapshot.dat","r")) == NULL)
	   UxPutSensitive(Snaf14_run,"false");
	else
	   UxPutSensitive(Snaf14_run,"true");
	fclose(fp);
	
	if( (fp = fopen("f14.s","r")) == NULL)
	{
	   UxPutSensitive(Snaf14_view,"false");
	   UxPutSensitive(Snaf14_print,"false");
	}
	else
	{
	   UxPutSensitive(Snaf14_view,"true");
	   UxPutSensitive(Snaf14_print,"true");
	
	fclose(fp);
	}
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Snaf14_run( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	system("xterm -T SNAF14 -sb -e snaf14 &");
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Snaf14_view( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	  char comando[200];
	
	  sprintf(comando,"%s","edit_f01 f14.s 0");
	  system(comando);
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Snaf14_print( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	   extern int print_file();
	
	   print_file("f14.s");
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	mapCB_Foraus_utils( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern char path_modello[];
	FILE *fp;
	char pathfile[FILENAME_MAX];
	
	sprintf(pathfile,"%s/%s",path_modello,FILE_FORAUS);
	
	if( (fp = fopen(pathfile,"r")) == NULL)
	{
	   UxPutSensitive(Foraus_print,"false");
	   UxPutSensitive(Foraus_delete,"false");
	}
	else
	{
	   fclose(fp);
	   UxPutSensitive(Foraus_print,"true");
	   UxPutSensitive(Foraus_delete,"true");
	}
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	mapCB_Edit_foraus( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern char path_modello[];
	FILE *fp;
	char pathfile[FILENAME_MAX];
	
	sprintf(pathfile,"%s/%s",path_modello,FILE_FORAUS);
	
	if( (fp = fopen(pathfile,"r")) == NULL)
	{
	   UxPutSensitive(foraus_copy,"false");
	   UxPutSensitive(foraus_manage,"false");
	}
	else
	{
	   fclose(fp);
	   UxPutSensitive(foraus_copy,"true");
	   UxPutSensitive(foraus_manage,"true");
	}
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_foraus_copy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	swidget swid;
	extern swidget create_CopyForaus();
	
	swid = create_CopyForaus();
	UxPopupInterface(swid,no_grab);
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_foraus_manage( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	   extern swidget create_vis_msg();
	   extern swidget create_Foraus();
	   swidget wfor;
	/*
	   create_vis_msg("Function not yet implemented");
	   return;
	*/
	   wfor = create_Foraus();
	   UxPopupInterface(wfor,no_grab);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Foraus_print( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	   extern int print_file();
	
	   print_file(FILE_FORAUS);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Foraus_delete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	/***
	extern swidget create_vis_msg();
	create_vis_msg("Function not yet implemented");
	***/
	extern swidget create_question_operation();
	create_question_operation(NULL,DELETE_FORAUS);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_help_contents( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifdef HYPERHELP_USED
	char *path_hyper;
	printf("PRIMA DI GETENV!!!\n");
	path_hyper = getenv("HLPPATH");
	if(path_hyper == NULL) {
		printf("SONO QUI!!!!!!\n");
		create_vis_msg("Environment variable HLPPATH not defined!");
	}	
	else {
		WinHelp(UxDisplay,"legohelp.hlp",HELP_CONTENTS,0);
	}
#else
	help_not_available_msg();
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_help_search( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifdef HYPERHELP_USED
	/*char help_file[200];
	sprintf(help_file,"%s/legohelp/legohelp.hlp",getenv("LEGOROOT")); */
	WinHelp(UxDisplay,"legohelp.hlp",HELP_COMMAND,(unsigned long) "TextSearch()");
#else
	help_not_available_msg();
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_help_on( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef HYPERHELP_USED
	help_not_available_msg();
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_help_on_help( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifdef HYPERHELP_USED
	WinHelp(UxDisplay,"",HELP_HELPONHELP, 0);
#else
	help_not_available_msg();
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_help_about( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	{
	extern swidget create_vis_msg();
	static char *msg="Version 1.0\nLEGOCAD model builder\n\nProperty of ENEL";
	
	create_vis_msg(msg);
	}
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	mapCB_form1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Topologia_pb5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_topology();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Topologia_pb6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_topology();
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_arrowButtonGadget10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	esegui_crealg1();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Dati_pb5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{ attiva_data(); }
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Dati_pb6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{ attiva_data(); }
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_arrowButtonGadget11( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	esegui_crealg3();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Stazionario_pb5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{ attiva_steady(); }
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Stazionario_pb6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{ attiva_steady();}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_arrowButtonGadget12( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	esegui_crealg5sk();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Transitorio_pb5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{ attiva_transient();}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Transitorio_pb6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_transient();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Librarian_pb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_librarian();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Librarian_pb2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_librarian();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Grafica_pb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_graphics();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Grafica_pb2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_graphics();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Tavole_pb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_tables();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Tavole_pb2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_tables();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Documentazione_pb1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_autodoc();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_Documentazione_pb2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	attiva_autodoc();
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static void	activateCB_menu1_p3_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	extern swidget messageST;
	UxPutText( messageST,"");
	}
	UxApplicationShell1Context = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_applicationShell1()
{
	Widget	menu1_p1_shell;
	Widget	EditModels_shell;
	Widget	Training_shell;
	Widget	menu_p4_shell;
	Widget	Option_pane_shell;
	Widget	Utilities_shell;
	Widget	Edi14_utils_shell;
	Widget	Snaf14_utils_shell;
	Widget	Foraus_utils_shell;
	Widget	Edit_foraus_shell;
	Widget	help_pane_shell;
	Widget	message_menu_shell;

	applicationShell1 = XtVaCreatePopupShell( "applicationShell1",
			applicationShellWidgetClass, UxTopLevel,
			XmNtitle, "Legocad",
			XmNiconName, "Legocad",
			XmNheight, 600,
			XmNwidth, 640,
			XmNy, 218,
			XmNx, 248,
			NULL );

	UxPutContext( applicationShell1, (char *) UxApplicationShell1Context );

	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass, applicationShell1,
			XmNheight, 600,
			XmNwidth, 640,
			XmNy, 15,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( mainWindow1, (char *) UxApplicationShell1Context );

	Edit = XtVaCreateManagedWidget( "Edit",
			xmRowColumnWidgetClass, mainWindow1,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( Edit, (char *) UxApplicationShell1Context );

	menu1_p1_shell = XtVaCreatePopupShell ("menu1_p1_shell",
			xmMenuShellWidgetClass, Edit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1_p1 = XtVaCreateWidget( "menu1_p1",
			xmRowColumnWidgetClass, menu1_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu1_p1, (char *) UxApplicationShell1Context );

	ProcessoButton = XtVaCreateManagedWidget( "ProcessoButton",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "P" ),
			RES_CONVERT( XmNlabelString, "Process modelling..." ),
			NULL );

	UxPutContext( ProcessoButton, (char *) UxApplicationShell1Context );

	ControlloButton = XtVaCreateManagedWidget( "ControlloButton",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Control system modelling" ),
			NULL );

	UxPutContext( ControlloButton, (char *) UxApplicationShell1Context );

	MixedButton = XtVaCreateManagedWidget( "MixedButton",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "X" ),
			RES_CONVERT( XmNlabelString, "Mixed system modelling" ),
			NULL );

	UxPutContext( MixedButton, (char *) UxApplicationShell1Context );

	sep1 = XtVaCreateManagedWidget( "sep1",
			xmSeparatorGadgetClass, menu1_p1,
			NULL );

	UxPutContext( sep1, (char *) UxApplicationShell1Context );

	OrderingButton = XtVaCreateManagedWidget( "OrderingButton",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNlabelString, "Models Management" ),
			NULL );

	UxPutContext( OrderingButton, (char *) UxApplicationShell1Context );

	sep2 = XtVaCreateManagedWidget( "sep2",
			xmSeparatorGadgetClass, menu1_p1,
			NULL );

	UxPutContext( sep2, (char *) UxApplicationShell1Context );

	ExitButton = XtVaCreateManagedWidget( "ExitButton",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "Q" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );

	UxPutContext( ExitButton, (char *) UxApplicationShell1Context );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass, Edit,
			XmNsensitive, TRUE,
			XmNsubMenuId, menu1_p1,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxApplicationShell1Context );

	EditModels_shell = XtVaCreatePopupShell ("EditModels_shell",
			xmMenuShellWidgetClass, Edit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	EditModels = XtVaCreateWidget( "EditModels",
			xmRowColumnWidgetClass, EditModels_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( EditModels, (char *) UxApplicationShell1Context );

	CopyModelButton = XtVaCreateManagedWidget( "CopyModelButton",
			xmPushButtonGadgetClass, EditModels,
			RES_CONVERT( XmNlabelString, "Copy Model..." ),
			NULL );

	UxPutContext( CopyModelButton, (char *) UxApplicationShell1Context );

	Training_shell = XtVaCreatePopupShell ("Training_shell",
			xmMenuShellWidgetClass, EditModels,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Training = XtVaCreateWidget( "Training",
			xmRowColumnWidgetClass, Training_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Training, (char *) UxApplicationShell1Context );

	Training_b1 = XtVaCreateManagedWidget( "Training_b1",
			xmPushButtonWidgetClass, Training,
			RES_CONVERT( XmNlabelString, "Select..." ),
			NULL );

	UxPutContext( Training_b1, (char *) UxApplicationShell1Context );

	ExamplesButton = XtVaCreateManagedWidget( "ExamplesButton",
			xmCascadeButtonWidgetClass, EditModels,
			XmNsubMenuId, Training,
			RES_CONVERT( XmNlabelString, "Training Examples" ),
			NULL );

	UxPutContext( ExamplesButton, (char *) UxApplicationShell1Context );

	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonWidgetClass, Edit,
			XmNsubMenuId, EditModels,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu1_top_b2, (char *) UxApplicationShell1Context );

	menu_p4_shell = XtVaCreatePopupShell ("menu_p4_shell",
			xmMenuShellWidgetClass, Edit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_p4 = XtVaCreateWidget( "menu_p4",
			xmRowColumnWidgetClass, menu_p4_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu_p4, (char *) UxApplicationShell1Context );

	Option_pane_shell = XtVaCreatePopupShell ("Option_pane_shell",
			xmMenuShellWidgetClass, menu_p4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Option_pane = XtVaCreateWidget( "Option_pane",
			xmRowColumnWidgetClass, Option_pane_shell,
			XmNradioBehavior, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Option_pane, (char *) UxApplicationShell1Context );

	Grafica = XtVaCreateManagedWidget( "Grafica",
			xmToggleButtonWidgetClass, Option_pane,
			XmNset, TRUE,
			RES_CONVERT( XmNlabelString, "Symbolic Diagram" ),
			NULL );

	UxPutContext( Grafica, (char *) UxApplicationShell1Context );

	ListeButton = XtVaCreateManagedWidget( "ListeButton",
			xmToggleButtonWidgetClass, Option_pane,
			RES_CONVERT( XmNlabelString, "Only by lists" ),
			NULL );

	UxPutContext( ListeButton, (char *) UxApplicationShell1Context );

	menu_p4_b2 = XtVaCreateManagedWidget( "menu_p4_b2",
			xmCascadeButtonWidgetClass, menu_p4,
			XmNsubMenuId, Option_pane,
			RES_CONVERT( XmNlabelString, "Display" ),
			NULL );

	UxPutContext( menu_p4_b2, (char *) UxApplicationShell1Context );

	Edit_top_b1 = XtVaCreateManagedWidget( "Edit_top_b1",
			xmCascadeButtonGadgetClass, Edit,
			XmNsubMenuId, menu_p4,
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( Edit_top_b1, (char *) UxApplicationShell1Context );

	Utilities_shell = XtVaCreatePopupShell ("Utilities_shell",
			xmMenuShellWidgetClass, Edit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Utilities = XtVaCreateWidget( "Utilities",
			xmRowColumnWidgetClass, Utilities_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Utilities, (char *) UxApplicationShell1Context );

	Edi14_utils_shell = XtVaCreatePopupShell ("Edi14_utils_shell",
			xmMenuShellWidgetClass, Utilities,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Edi14_utils = XtVaCreateWidget( "Edi14_utils",
			xmRowColumnWidgetClass, Edi14_utils_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Edi14_utils, (char *) UxApplicationShell1Context );

	Edi14_run = XtVaCreateManagedWidget( "Edi14_run",
			xmPushButtonWidgetClass, Edi14_utils,
			RES_CONVERT( XmNlabelString, "run Edi14" ),
			NULL );

	UxPutContext( Edi14_run, (char *) UxApplicationShell1Context );

	Edi14_save = XtVaCreateManagedWidget( "Edi14_save",
			xmPushButtonWidgetClass, Edi14_utils,
			RES_CONVERT( XmNlabelString, "Save Result Edi14" ),
			NULL );

	UxPutContext( Edi14_save, (char *) UxApplicationShell1Context );

	Edi14_viewout = XtVaCreateManagedWidget( "Edi14_viewout",
			xmPushButtonWidgetClass, Edi14_utils,
			RES_CONVERT( XmNlabelString, "View Edi14 results" ),
			NULL );

	UxPutContext( Edi14_viewout, (char *) UxApplicationShell1Context );

	Utilities_b1 = XtVaCreateManagedWidget( "Utilities_b1",
			xmCascadeButtonWidgetClass, Utilities,
			XmNsubMenuId, Edi14_utils,
			RES_CONVERT( XmNlabelString, "Edi14" ),
			NULL );

	UxPutContext( Utilities_b1, (char *) UxApplicationShell1Context );

	Snaf14_utils_shell = XtVaCreatePopupShell ("Snaf14_utils_shell",
			xmMenuShellWidgetClass, Utilities,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Snaf14_utils = XtVaCreateWidget( "Snaf14_utils",
			xmRowColumnWidgetClass, Snaf14_utils_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Snaf14_utils, (char *) UxApplicationShell1Context );

	Snaf14_run = XtVaCreateManagedWidget( "Snaf14_run",
			xmPushButtonWidgetClass, Snaf14_utils,
			RES_CONVERT( XmNlabelString, "run Snaf14" ),
			NULL );

	UxPutContext( Snaf14_run, (char *) UxApplicationShell1Context );

	Snaf14_view = XtVaCreateManagedWidget( "Snaf14_view",
			xmPushButtonWidgetClass, Snaf14_utils,
			RES_CONVERT( XmNlabelString, "View Results" ),
			NULL );

	UxPutContext( Snaf14_view, (char *) UxApplicationShell1Context );

	Snaf14_print = XtVaCreateManagedWidget( "Snaf14_print",
			xmPushButtonWidgetClass, Snaf14_utils,
			RES_CONVERT( XmNlabelString, "Print Results " ),
			NULL );

	UxPutContext( Snaf14_print, (char *) UxApplicationShell1Context );

	Utilities_b2 = XtVaCreateManagedWidget( "Utilities_b2",
			xmCascadeButtonWidgetClass, Utilities,
			XmNsubMenuId, Snaf14_utils,
			RES_CONVERT( XmNlabelString, "Snaf14" ),
			NULL );

	UxPutContext( Utilities_b2, (char *) UxApplicationShell1Context );

	Foraus_utils_shell = XtVaCreatePopupShell ("Foraus_utils_shell",
			xmMenuShellWidgetClass, Utilities,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Foraus_utils = XtVaCreateWidget( "Foraus_utils",
			xmRowColumnWidgetClass, Foraus_utils_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Foraus_utils, (char *) UxApplicationShell1Context );

	Edit_foraus_shell = XtVaCreatePopupShell ("Edit_foraus_shell",
			xmMenuShellWidgetClass, Foraus_utils,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Edit_foraus = XtVaCreateWidget( "Edit_foraus",
			xmRowColumnWidgetClass, Edit_foraus_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Edit_foraus, (char *) UxApplicationShell1Context );

	foraus_copy = XtVaCreateManagedWidget( "foraus_copy",
			xmPushButtonWidgetClass, Edit_foraus,
			RES_CONVERT( XmNlabelString, "Copy from ..." ),
			NULL );

	UxPutContext( foraus_copy, (char *) UxApplicationShell1Context );

	foraus_manage = XtVaCreateManagedWidget( "foraus_manage",
			xmPushButtonWidgetClass, Edit_foraus,
			RES_CONVERT( XmNlabelString, "Manage ..." ),
			NULL );

	UxPutContext( foraus_manage, (char *) UxApplicationShell1Context );

	Foraus_edit = XtVaCreateManagedWidget( "Foraus_edit",
			xmCascadeButtonWidgetClass, Foraus_utils,
			XmNsubMenuId, Edit_foraus,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( Foraus_edit, (char *) UxApplicationShell1Context );

	Foraus_print = XtVaCreateManagedWidget( "Foraus_print",
			xmPushButtonWidgetClass, Foraus_utils,
			RES_CONVERT( XmNlabelString, "Print foraus" ),
			NULL );

	UxPutContext( Foraus_print, (char *) UxApplicationShell1Context );

	Foraus_delete = XtVaCreateManagedWidget( "Foraus_delete",
			xmPushButtonWidgetClass, Foraus_utils,
			RES_CONVERT( XmNlabelString, "Delete foraus" ),
			NULL );

	UxPutContext( Foraus_delete, (char *) UxApplicationShell1Context );

	Utilities_b3 = XtVaCreateManagedWidget( "Utilities_b3",
			xmCascadeButtonWidgetClass, Utilities,
			XmNsubMenuId, Foraus_utils,
			RES_CONVERT( XmNlabelString, "Foraus" ),
			NULL );

	UxPutContext( Utilities_b3, (char *) UxApplicationShell1Context );

	Edit_top_b2 = XtVaCreateManagedWidget( "Edit_top_b2",
			xmCascadeButtonWidgetClass, Edit,
			XmNsubMenuId, Utilities,
			RES_CONVERT( XmNlabelString, "Utilities" ),
			NULL );

	UxPutContext( Edit_top_b2, (char *) UxApplicationShell1Context );

	help_pane_shell = XtVaCreatePopupShell ("help_pane_shell",
			xmMenuShellWidgetClass, Edit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	help_pane = XtVaCreateWidget( "help_pane",
			xmRowColumnWidgetClass, help_pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( help_pane, (char *) UxApplicationShell1Context );

	help_contents = XtVaCreateManagedWidget( "help_contents",
			xmPushButtonWidgetClass, help_pane,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Contents" ),
			NULL );

	UxPutContext( help_contents, (char *) UxApplicationShell1Context );

	help_search = XtVaCreateManagedWidget( "help_search",
			xmPushButtonWidgetClass, help_pane,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Search..." ),
			NULL );

	UxPutContext( help_search, (char *) UxApplicationShell1Context );

	help_on = XtVaCreateManagedWidget( "help_on",
			xmPushButtonWidgetClass, help_pane,
			RES_CONVERT( XmNlabelString, "Help On..." ),
			NULL );

	UxPutContext( help_on, (char *) UxApplicationShell1Context );

	Edit_p11_b4 = XtVaCreateManagedWidget( "Edit_p11_b4",
			xmSeparatorWidgetClass, help_pane,
			NULL );

	UxPutContext( Edit_p11_b4, (char *) UxApplicationShell1Context );

	help_on_help = XtVaCreateManagedWidget( "help_on_help",
			xmPushButtonWidgetClass, help_pane,
			RES_CONVERT( XmNmnemonic, "H" ),
			RES_CONVERT( XmNlabelString, "How to Use Help" ),
			NULL );

	UxPutContext( help_on_help, (char *) UxApplicationShell1Context );

	Edit_p11_b6 = XtVaCreateManagedWidget( "Edit_p11_b6",
			xmSeparatorWidgetClass, help_pane,
			NULL );

	UxPutContext( Edit_p11_b6, (char *) UxApplicationShell1Context );

	help_about = XtVaCreateManagedWidget( "help_about",
			xmPushButtonWidgetClass, help_pane,
			RES_CONVERT( XmNlabelString, "About Legocad..." ),
			NULL );

	UxPutContext( help_about, (char *) UxApplicationShell1Context );

	Edit_top_b3 = XtVaCreateManagedWidget( "Edit_top_b3",
			xmCascadeButtonGadgetClass, Edit,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNsubMenuId, help_pane,
			RES_CONVERT( XmNmnemonic, "H" ),
			RES_CONVERT( XmNlabelString, "Help" ),
			NULL );

	UxPutContext( Edit_top_b3, (char *) UxApplicationShell1Context );

	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass, mainWindow1,
			XmNheight, 300,
			XmNwidth, 755,
			NULL );

	UxPutContext( panedWindow1, (char *) UxApplicationShell1Context );

	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass, panedWindow1,
			XmNallowResize, TRUE,
			XmNheight, 112,
			XmNwidth, 627,
			XmNy, 3,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form2, (char *) UxApplicationShell1Context );

	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass, form2,
			RES_CONVERT( XmNlabelString, "Legocad Environment:" ),
			XmNheight, 20,
			XmNwidth, 170,
			XmNy, 5,
			XmNx, 10,
			NULL );

	UxPutContext( label1, (char *) UxApplicationShell1Context );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, form2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Process Modules Library:" ),
			XmNheight, 20,
			XmNwidth, 160,
			XmNy, 55,
			XmNx, 25,
			NULL );

	UxPutContext( label2, (char *) UxApplicationShell1Context );

	LegocadPathLabel = XtVaCreateManagedWidget( "LegocadPathLabel",
			xmLabelWidgetClass, form2,
			RES_CONVERT( XmNlabelString, "Not defined" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-o-normal--14-140-75-75-p-78-iso8859-1" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 435,
			XmNy, 35,
			XmNx, 190,
			NULL );

	UxPutContext( LegocadPathLabel, (char *) UxApplicationShell1Context );

	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass, form2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Control Modules Library:" ),
			XmNheight, 20,
			XmNwidth, 155,
			XmNy, 75,
			XmNx, 25,
			NULL );

	UxPutContext( label4, (char *) UxApplicationShell1Context );

	label5 = XtVaCreateManagedWidget( "label5",
			xmLabelWidgetClass, form2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Legocad Development Root:" ),
			XmNheight, 20,
			XmNwidth, 165,
			XmNy, 35,
			XmNx, 25,
			NULL );

	UxPutContext( label5, (char *) UxApplicationShell1Context );

	ModuliLibLabel = XtVaCreateManagedWidget( "ModuliLibLabel",
			xmLabelWidgetClass, form2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-o-normal--14-140-75-75-p-78-iso8859-1" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 435,
			XmNy, 55,
			XmNx, 190,
			NULL );

	UxPutContext( ModuliLibLabel, (char *) UxApplicationShell1Context );

	RegLibLabel = XtVaCreateManagedWidget( "RegLibLabel",
			xmLabelWidgetClass, form2,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-o-normal--14-140-75-75-p-78-iso8859-1" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 440,
			XmNy, 75,
			XmNx, 190,
			NULL );

	UxPutContext( RegLibLabel, (char *) UxApplicationShell1Context );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, panedWindow1,
			XmNpaneMinimum, 170,
			XmNheight, 175,
			XmNwidth, 642,
			XmNy, 120,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form1, (char *) UxApplicationShell1Context );

	labelCostruzione = XtVaCreateManagedWidget( "labelCostruzione",
			xmLabelGadgetClass, form1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNrightAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Model construction:" ),
			XmNheight, 25,
			XmNwidth, 160,
			XmNy, 30,
			XmNx, 5,
			NULL );

	UxPutContext( labelCostruzione, (char *) UxApplicationShell1Context );

	rowColumn5 = XtVaCreateManagedWidget( "rowColumn5",
			xmRowColumnWidgetClass, form1,
			XmNtopWidget, labelCostruzione,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNorientation, XmHORIZONTAL,
			XmNheight, 107,
			XmNwidth, 544,
			XmNy, 53,
			XmNx, 44,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( rowColumn5, (char *) UxApplicationShell1Context );

	Topologia_frame3 = XtVaCreateManagedWidget( "Topologia_frame3",
			xmFrameWidgetClass, rowColumn5,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 101,
			XmNwidth, 77,
			XmNy, 3,
			XmNx, 2,
			NULL );

	UxPutContext( Topologia_frame3, (char *) UxApplicationShell1Context );

	Topologia_rc = XtVaCreateManagedWidget( "Topologia_rc",
			xmRowColumnWidgetClass, Topologia_frame3,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 97,
			XmNwidth, 74,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( Topologia_rc, (char *) UxApplicationShell1Context );

	Topologia_pb5 = XtVaCreateManagedWidget( "Topologia_pb5",
			xmPushButtonWidgetClass, Topologia_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNsensitive, FALSE,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 68,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Topologia_pb5, (char *) UxApplicationShell1Context );

	Topologia_pb6 = XtVaCreateManagedWidget( "Topologia_pb6",
			xmPushButtonWidgetClass, Topologia_rc,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			RES_CONVERT( XmNlabelString, "Topology" ),
			XmNheight, 29,
			XmNwidth, 68,
			XmNy, 16,
			XmNx, 3,
			NULL );

	UxPutContext( Topologia_pb6, (char *) UxApplicationShell1Context );

	arrowButtonGadget10 = XtVaCreateManagedWidget( "arrowButtonGadget10",
			xmArrowButtonGadgetClass, rowColumn5,
			XmNsensitive, TRUE,
			XmNshadowThickness, 0,
			XmNarrowDirection, XmARROW_RIGHT,
			XmNheight, 101,
			XmNwidth, 33,
			XmNy, 3,
			XmNx, 83,
			NULL );

	UxPutContext( arrowButtonGadget10, (char *) UxApplicationShell1Context );

	Dati_frame3 = XtVaCreateManagedWidget( "Dati_frame3",
			xmFrameWidgetClass, rowColumn5,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 101,
			XmNwidth, 88,
			XmNy, 3,
			XmNx, 120,
			NULL );

	UxPutContext( Dati_frame3, (char *) UxApplicationShell1Context );

	Dati_rc = XtVaCreateManagedWidget( "Dati_rc",
			xmRowColumnWidgetClass, Dati_frame3,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 97,
			XmNwidth, 84,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( Dati_rc, (char *) UxApplicationShell1Context );

	Dati_pb5 = XtVaCreateManagedWidget( "Dati_pb5",
			xmPushButtonWidgetClass, Dati_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 78,
			XmNy, 3,
			XmNx, 2,
			NULL );

	UxPutContext( Dati_pb5, (char *) UxApplicationShell1Context );

	Dati_pb6 = XtVaCreateManagedWidget( "Dati_pb6",
			xmPushButtonWidgetClass, Dati_rc,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNsensitive, FALSE,
			RES_CONVERT( XmNlabelString, "Data Editor" ),
			XmNheight, 29,
			XmNwidth, 78,
			XmNy, 16,
			XmNx, 2,
			NULL );

	UxPutContext( Dati_pb6, (char *) UxApplicationShell1Context );

	arrowButtonGadget11 = XtVaCreateManagedWidget( "arrowButtonGadget11",
			xmArrowButtonGadgetClass, rowColumn5,
			XmNsensitive, TRUE,
			XmNshadowThickness, 0,
			XmNarrowDirection, XmARROW_RIGHT,
			XmNheight, 101,
			XmNwidth, 33,
			XmNy, 3,
			XmNx, 211,
			NULL );

	UxPutContext( arrowButtonGadget11, (char *) UxApplicationShell1Context );

	Stazionario_frame3 = XtVaCreateManagedWidget( "Stazionario_frame3",
			xmFrameWidgetClass, rowColumn5,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 101,
			XmNwidth, 99,
			XmNy, 3,
			XmNx, 248,
			NULL );

	UxPutContext( Stazionario_frame3, (char *) UxApplicationShell1Context );

	Stazionario_rc = XtVaCreateManagedWidget( "Stazionario_rc",
			xmRowColumnWidgetClass, Stazionario_frame3,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 97,
			XmNwidth, 95,
			XmNy, 2,
			XmNx, 1,
			NULL );

	UxPutContext( Stazionario_rc, (char *) UxApplicationShell1Context );

	Stazionario_pb5 = XtVaCreateManagedWidget( "Stazionario_pb5",
			xmPushButtonWidgetClass, Stazionario_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 90,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Stazionario_pb5, (char *) UxApplicationShell1Context );

	Stazionario_pb6 = XtVaCreateManagedWidget( "Stazionario_pb6",
			xmPushButtonWidgetClass, Stazionario_rc,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNlabelString, "Steady State" ),
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNheight, 29,
			XmNwidth, 90,
			XmNy, 16,
			XmNx, 3,
			NULL );

	UxPutContext( Stazionario_pb6, (char *) UxApplicationShell1Context );

	arrowButtonGadget12 = XtVaCreateManagedWidget( "arrowButtonGadget12",
			xmArrowButtonGadgetClass, rowColumn5,
			XmNsensitive, TRUE,
			XmNshadowThickness, 0,
			XmNarrowDirection, XmARROW_RIGHT,
			XmNheight, 101,
			XmNwidth, 33,
			XmNy, 3,
			XmNx, 350,
			NULL );

	UxPutContext( arrowButtonGadget12, (char *) UxApplicationShell1Context );

	Transitorio_frame3 = XtVaCreateManagedWidget( "Transitorio_frame3",
			xmFrameWidgetClass, rowColumn5,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 101,
			XmNwidth, 111,
			XmNy, 3,
			XmNx, 387,
			NULL );

	UxPutContext( Transitorio_frame3, (char *) UxApplicationShell1Context );

	Transitorio_rc = XtVaCreateManagedWidget( "Transitorio_rc",
			xmRowColumnWidgetClass, Transitorio_frame3,
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 97,
			XmNwidth, 108,
			XmNy, 2,
			XmNx, 1,
			NULL );

	UxPutContext( Transitorio_rc, (char *) UxApplicationShell1Context );

	Transitorio_pb5 = XtVaCreateManagedWidget( "Transitorio_pb5",
			xmPushButtonWidgetClass, Transitorio_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 102,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Transitorio_pb5, (char *) UxApplicationShell1Context );

	Transitorio_pb6 = XtVaCreateManagedWidget( "Transitorio_pb6",
			xmPushButtonWidgetClass, Transitorio_rc,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNlabelString, "Transient Calc." ),
			RES_CONVERT( XmNbackground, "SteelBlue" ),
			XmNheight, 29,
			XmNwidth, 102,
			XmNy, 16,
			XmNx, 3,
			NULL );

	UxPutContext( Transitorio_pb6, (char *) UxApplicationShell1Context );

	labelNomeModello = XtVaCreateManagedWidget( "labelNomeModello",
			xmLabelGadgetClass, form1,
			XmNtopPosition, 0,
			XmNtopOffset, 20,
			XmNleftOffset, 150,
			XmNleftAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--18-180-75-75-p-98-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Model not selected" ),
			XmNalignment, XmALIGNMENT_CENTER,
			XmNheight, 20,
			XmNwidth, 320,
			XmNy, 10,
			XmNx, 150,
			NULL );

	UxPutContext( labelNomeModello, (char *) UxApplicationShell1Context );

	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass, panedWindow1,
			XmNpaneMinimum, 135,
			XmNheight, 127,
			XmNwidth, 626,
			XmNy, 303,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form3, (char *) UxApplicationShell1Context );

	labelUtilita = XtVaCreateManagedWidget( "labelUtilita",
			xmLabelGadgetClass, form3,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopWidget, NULL,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Utilities:" ),
			XmNheight, 25,
			XmNwidth, 724,
			XmNy, 0,
			XmNx, 0,
			NULL );

	UxPutContext( labelUtilita, (char *) UxApplicationShell1Context );

	rowColumn2 = XtVaCreateManagedWidget( "rowColumn2",
			xmRowColumnWidgetClass, form3,
			XmNresizeHeight, FALSE,
			XmNleftOffset, 40,
			XmNspacing, 20,
			XmNtopWidget, labelUtilita,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNorientation, XmHORIZONTAL,
			XmNheight, 110,
			XmNwidth, 442,
			XmNy, 25,
			XmNx, 40,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( rowColumn2, (char *) UxApplicationShell1Context );

	Librarian_frame3 = XtVaCreateManagedWidget( "Librarian_frame3",
			xmFrameWidgetClass, rowColumn2,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 97,
			XmNwidth, 85,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Librarian_frame3, (char *) UxApplicationShell1Context );

	Librarian_rc = XtVaCreateManagedWidget( "Librarian_rc",
			xmRowColumnWidgetClass, Librarian_frame3,
			RES_CONVERT( XmNbackground, "Steel Blue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 98,
			XmNwidth, 83,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( Librarian_rc, (char *) UxApplicationShell1Context );

	Librarian_pb1 = XtVaCreateManagedWidget( "Librarian_pb1",
			xmPushButtonWidgetClass, Librarian_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 75,
			XmNy, 0,
			XmNx, 5,
			NULL );

	UxPutContext( Librarian_pb1, (char *) UxApplicationShell1Context );

	Librarian_pb2 = XtVaCreateManagedWidget( "Librarian_pb2",
			xmPushButtonWidgetClass, Librarian_rc,
			RES_CONVERT( XmNbackground, "Steel Blue" ),
			RES_CONVERT( XmNlabelString, "Librarian" ),
			XmNheight, 25,
			XmNwidth, 75,
			XmNy, 15,
			XmNx, 5,
			NULL );

	UxPutContext( Librarian_pb2, (char *) UxApplicationShell1Context );

	Grafica_frame = XtVaCreateManagedWidget( "Grafica_frame",
			xmFrameWidgetClass, rowColumn2,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 94,
			XmNwidth, 77,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Grafica_frame, (char *) UxApplicationShell1Context );

	Grafica_rc = XtVaCreateManagedWidget( "Grafica_rc",
			xmRowColumnWidgetClass, Grafica_frame,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 103,
			XmNwidth, 94,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( Grafica_rc, (char *) UxApplicationShell1Context );

	Grafica_pb1 = XtVaCreateManagedWidget( "Grafica_pb1",
			xmPushButtonWidgetClass, Grafica_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 75,
			XmNy, 13,
			XmNx, 13,
			NULL );

	UxPutContext( Grafica_pb1, (char *) UxApplicationShell1Context );

	Grafica_pb2 = XtVaCreateManagedWidget( "Grafica_pb2",
			xmPushButtonWidgetClass, Grafica_rc,
			RES_CONVERT( XmNbackground, "Cadet Blue" ),
			RES_CONVERT( XmNlabelString, "Graph" ),
			XmNheight, 25,
			XmNwidth, 75,
			XmNy, 15,
			XmNx, 10,
			NULL );

	UxPutContext( Grafica_pb2, (char *) UxApplicationShell1Context );

	Tavole_frame = XtVaCreateManagedWidget( "Tavole_frame",
			xmFrameWidgetClass, rowColumn2,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 94,
			XmNwidth, 77,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Tavole_frame, (char *) UxApplicationShell1Context );

	Tavole_rc = XtVaCreateManagedWidget( "Tavole_rc",
			xmRowColumnWidgetClass, Tavole_frame,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 91,
			XmNwidth, 73,
			XmNy, 2,
			XmNx, 2,
			NULL );

	UxPutContext( Tavole_rc, (char *) UxApplicationShell1Context );

	Tavole_pb1 = XtVaCreateManagedWidget( "Tavole_pb1",
			xmPushButtonWidgetClass, Tavole_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 75,
			XmNy, 13,
			XmNx, 13,
			NULL );

	UxPutContext( Tavole_pb1, (char *) UxApplicationShell1Context );

	Tavole_pb2 = XtVaCreateManagedWidget( "Tavole_pb2",
			xmPushButtonWidgetClass, Tavole_rc,
			RES_CONVERT( XmNbackground, "Cadet Blue" ),
			RES_CONVERT( XmNlabelString, "Tables" ),
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 90,
			XmNx, 260,
			NULL );

	UxPutContext( Tavole_pb2, (char *) UxApplicationShell1Context );

	Documentazione_frame = XtVaCreateManagedWidget( "Documentazione_frame",
			xmFrameWidgetClass, rowColumn2,
			XmNshadowType, XmSHADOW_OUT,
			XmNheight, 94,
			XmNwidth, 77,
			XmNy, 3,
			XmNx, 3,
			NULL );

	UxPutContext( Documentazione_frame, (char *) UxApplicationShell1Context );

	Documentazione_rc = XtVaCreateManagedWidget( "Documentazione_rc",
			xmRowColumnWidgetClass, Documentazione_frame,
			RES_CONVERT( XmNbackground, "Steel Blue" ),
			XmNentryAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNforeground, "darkslategrey" ),
			XmNborderWidth, 0,
			XmNheight, 150,
			XmNwidth, 150,
			XmNy, 60,
			XmNx, 120,
			NULL );

	UxPutContext( Documentazione_rc, (char *) UxApplicationShell1Context );

	Documentazione_pb1 = XtVaCreateManagedWidget( "Documentazione_pb1",
			xmPushButtonWidgetClass, Documentazione_rc,
			RES_CONVERT( XmNbackground, "#b0c4de" ),
			XmNshadowThickness, 1,
			XmNlabelType, XmPIXMAP,
			XmNheight, 10,
			XmNwidth, 75,
			XmNy, 13,
			XmNx, 13,
			NULL );

	UxPutContext( Documentazione_pb1, (char *) UxApplicationShell1Context );

	Documentazione_pb2 = XtVaCreateManagedWidget( "Documentazione_pb2",
			xmPushButtonWidgetClass, Documentazione_rc,
			RES_CONVERT( XmNbackground, "Steel Blue" ),
			RES_CONVERT( XmNlabelString, "Doc" ),
			XmNheight, 25,
			XmNwidth, 115,
			XmNy, 90,
			XmNx, 365,
			NULL );

	UxPutContext( Documentazione_pb2, (char *) UxApplicationShell1Context );

	form5 = XtVaCreateManagedWidget( "form5",
			xmFormWidgetClass, panedWindow1,
			XmNallowResize, TRUE,
			XmNheight, 200,
			XmNwidth, 629,
			XmNy, 435,
			XmNx, 3,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( form5, (char *) UxApplicationShell1Context );

	labelMessaggi = XtVaCreateManagedWidget( "labelMessaggi",
			xmLabelGadgetClass, form5,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Messages:" ),
			XmNheight, 20,
			XmNwidth, 619,
			XmNy, 1,
			XmNx, 5,
			NULL );

	UxPutContext( labelMessaggi, (char *) UxApplicationShell1Context );

	scrolledWindow3 = XtVaCreateManagedWidget( "scrolledWindow3",
			xmScrolledWindowWidgetClass, form5,
			XmNresizable, TRUE,
			XmNspacing, 2,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 150,
			XmNwidth, 250,
			XmNtopWidget, labelMessaggi,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 21,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow3, (char *) UxApplicationShell1Context );

	messageST = XtVaCreateManagedWidget( "messageST",
			xmTextWidgetClass, scrolledWindow3,
			XmNnavigationType, XmNONE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNcolumns, 500,
			XmNscrollTopSide, FALSE,
			XmNresizeWidth, TRUE,
			XmNresizeHeight, TRUE,
			XmNscrollVertical, TRUE,
			XmNscrollHorizontal, TRUE,
			XmNeditable, FALSE,
			XmNcursorPositionVisible, FALSE,
			XmNy, 0,
			XmNx, 0,
			XmNheight, 60,
			XmNwidth, 612,
			NULL );

	UxPutContext( messageST, (char *) UxApplicationShell1Context );

	message_menu_shell = XtVaCreatePopupShell ("message_menu_shell",
			xmMenuShellWidgetClass, messageST,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	message_menu = XtVaCreateWidget( "message_menu",
			xmRowColumnWidgetClass, message_menu_shell,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( message_menu, (char *) UxApplicationShell1Context );

	menu1_p3_title = XtVaCreateManagedWidget( "menu1_p3_title",
			xmLabelWidgetClass, message_menu,
			RES_CONVERT( XmNlabelString, "MESSAGE" ),
			NULL );

	UxPutContext( menu1_p3_title, (char *) UxApplicationShell1Context );

	message_menu_p1_b2 = XtVaCreateManagedWidget( "message_menu_p1_b2",
			xmSeparatorWidgetClass, message_menu,
			NULL );

	UxPutContext( message_menu_p1_b2, (char *) UxApplicationShell1Context );

	menu1_p3_b1 = XtVaCreateManagedWidget( "menu1_p3_b1",
			xmPushButtonWidgetClass, message_menu,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Clear Window" ),
			NULL );

	UxPutContext( menu1_p3_b1, (char *) UxApplicationShell1Context );

	XtAddCallback( applicationShell1, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( applicationShell1, XmNpopupCallback,
			popupCB_applicationShell1,
			(XtPointer) UxApplicationShell1Context );

	XtVaSetValues(Edit,
			XmNmenuHelpWidget, Edit_top_b3,
			NULL );

	XtAddCallback( ProcessoButton, XmNactivateCallback,
			activateCB_ProcessoButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( ControlloButton, XmNactivateCallback,
			activateCB_ControlloButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( MixedButton, XmNactivateCallback,
			activateCB_MixedButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( OrderingButton, XmNactivateCallback,
			activateCB_OrderingButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( ExitButton, XmNactivateCallback,
			activateCB_ExitButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( CopyModelButton, XmNactivateCallback,
			activateCB_CopyModelButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Training_b1, XmNactivateCallback,
			activateCB_Training_b1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Grafica, XmNvalueChangedCallback,
			valueChangedCB_Grafica,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( ListeButton, XmNvalueChangedCallback,
			valueChangedCB_ListeButton,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Edi14_utils, XmNmapCallback,
			mapCB_Edi14_utils,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Edi14_run, XmNactivateCallback,
			activateCB_Edi14_run,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Edi14_save, XmNactivateCallback,
			activateCB_Edi14_save,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Edi14_viewout, XmNactivateCallback,
			activateCB_Edi14_viewout,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Snaf14_utils, XmNmapCallback,
			mapCB_Snaf14_utils,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Snaf14_run, XmNactivateCallback,
			activateCB_Snaf14_run,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Snaf14_view, XmNactivateCallback,
			activateCB_Snaf14_view,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Snaf14_print, XmNactivateCallback,
			activateCB_Snaf14_print,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Foraus_utils, XmNmapCallback,
			mapCB_Foraus_utils,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Edit_foraus, XmNmapCallback,
			mapCB_Edit_foraus,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( foraus_copy, XmNactivateCallback,
			activateCB_foraus_copy,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( foraus_manage, XmNactivateCallback,
			activateCB_foraus_manage,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Foraus_print, XmNactivateCallback,
			activateCB_Foraus_print,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Foraus_delete, XmNactivateCallback,
			activateCB_Foraus_delete,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( help_contents, XmNactivateCallback,
			activateCB_help_contents,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( help_search, XmNactivateCallback,
			activateCB_help_search,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( help_on, XmNactivateCallback,
			activateCB_help_on,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( help_on_help, XmNactivateCallback,
			activateCB_help_on_help,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( help_about, XmNactivateCallback,
			activateCB_help_about,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( form1, XmNmapCallback,
			mapCB_form1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Topologia_pb5, XmNactivateCallback,
			activateCB_Topologia_pb5,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Topologia_pb6, XmNactivateCallback,
			activateCB_Topologia_pb6,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( arrowButtonGadget10, XmNactivateCallback,
			activateCB_arrowButtonGadget10,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Dati_pb5, XmNactivateCallback,
			activateCB_Dati_pb5,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Dati_pb6, XmNactivateCallback,
			activateCB_Dati_pb6,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( arrowButtonGadget11, XmNactivateCallback,
			activateCB_arrowButtonGadget11,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Stazionario_pb5, XmNactivateCallback,
			activateCB_Stazionario_pb5,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Stazionario_pb6, XmNactivateCallback,
			activateCB_Stazionario_pb6,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( arrowButtonGadget12, XmNactivateCallback,
			activateCB_arrowButtonGadget12,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Transitorio_pb5, XmNactivateCallback,
			activateCB_Transitorio_pb5,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Transitorio_pb6, XmNactivateCallback,
			activateCB_Transitorio_pb6,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Librarian_pb1, XmNactivateCallback,
			activateCB_Librarian_pb1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Librarian_pb2, XmNactivateCallback,
			activateCB_Librarian_pb2,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Grafica_pb1, XmNactivateCallback,
			activateCB_Grafica_pb1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Grafica_pb2, XmNactivateCallback,
			activateCB_Grafica_pb2,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Tavole_pb1, XmNactivateCallback,
			activateCB_Tavole_pb1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Tavole_pb2, XmNactivateCallback,
			activateCB_Tavole_pb2,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Documentazione_pb1, XmNactivateCallback,
			activateCB_Documentazione_pb1,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( Documentazione_pb2, XmNactivateCallback,
			activateCB_Documentazione_pb2,
			(XtPointer) UxApplicationShell1Context );

	XtAddCallback( menu1_p3_b1, XmNactivateCallback,
			activateCB_menu1_p3_b1,
			(XtPointer) UxApplicationShell1Context );


	XtAddEventHandler( messageST, ButtonPressMask,
			False, _UxapplicationShell1MenuPost, message_menu );
	XmMainWindowSetAreas( mainWindow1, Edit, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, panedWindow1 );

	return ( applicationShell1 );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_applicationShell1()
{
	Widget                  rtrn;
	_UxCapplicationShell1   *UxContext;

	UxApplicationShell1Context = UxContext =
		(_UxCapplicationShell1 *) XtMalloc( sizeof(_UxCapplicationShell1) );

	{
		wsel = NULL;
		rtrn = _Uxbuild_applicationShell1();

		XSync(UxDisplay,False);
		
		/*
		 setta le icone associate ai botoni di attivazione
		*/
		int set_icone();
		
		
		
		/* 
		 Verifica la correttezza dell'ambiente per l'utente
		 il cui direttorio di lavoro e' specificato dalla
		 variabile di environment LEGOCAD_USER
		*/
		testa_ambiente();
		/*
		 Aggiorna le labels della main window sulle quali  
		 sono specificati i path names delle librerie dei
		 moduli utilizzate
		*/
		int agg_label_ambiente();
		/*
		 a seconda delle librerie presenti vengono abilitate
		 le voci per la creazione di modelli di processo e/o
		 di regolazione
		*/
		update_pulldown();
		/*
		 aggiorna la attivazione o meno dei bottoni delle
		 utilities utilizzabili
		*/
		int aggiorna_utilities();
		/*
		 si posizione nella directory di lavoro per l'utente
		 (specificata dalla variabile di environment LEGOCAD_USER)
		*/
		chdir_path_legocad();
		
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_applicationShell1()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_applicationShell1();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

