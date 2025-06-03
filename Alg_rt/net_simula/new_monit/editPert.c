
/*******************************************************************************
       editPert.c
       (Generated from interface file editPert.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/PanedW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
XtIntervalId timer_editPert;
int editPertRate = 3;      /* in secondi */
#include "messaggi.h"
extern int edit_pert_interface_active;
extern int _MAX_PERTUR;
TIPO_PERT *perturbazioni;
TIPO_PERT *perturbazioni_old;

#include "bistrutt.h"
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	UxeditPertMenu;
	Widget	UxeditPertMenuFile;
	Widget	UxeditPertMenuFileQuit;
	Widget	Uxmenu10_top_b1;
	Widget	UxeditPertMenuEdit;
	Widget	UxeditPertMenuEditTimer;
	Widget	Uxmenu10_top_b2;
	Widget	UxframeComm;
	Widget	Uxform53;
	Widget	UxeditPertQuitpb;
	Widget	UxpanedWindow3;
	Widget	Uxform54;
	Widget	UxeditPertAttScrollWin;
	Widget	UxeditPertAttScrollList;
	Widget	Uxlabel12;
	Widget	UxeditPertFreqUpdateLabel;
	Widget	Uxform55;
	Widget	UxeditPertUnattScrollWin;
	Widget	UxeditPertUnattScrollList;
	Widget	Uxlabel14;
	swidget	UxUxParent;
} _UxCeditPert;

static _UxCeditPert            *UxEditPertContext;
#define editPertMenu            UxEditPertContext->UxeditPertMenu
#define editPertMenuFile        UxEditPertContext->UxeditPertMenuFile
#define editPertMenuFileQuit    UxEditPertContext->UxeditPertMenuFileQuit
#define menu10_top_b1           UxEditPertContext->Uxmenu10_top_b1
#define editPertMenuEdit        UxEditPertContext->UxeditPertMenuEdit
#define editPertMenuEditTimer   UxEditPertContext->UxeditPertMenuEditTimer
#define menu10_top_b2           UxEditPertContext->Uxmenu10_top_b2
#define frameComm               UxEditPertContext->UxframeComm
#define form53                  UxEditPertContext->Uxform53
#define editPertQuitpb          UxEditPertContext->UxeditPertQuitpb
#define panedWindow3            UxEditPertContext->UxpanedWindow3
#define form54                  UxEditPertContext->Uxform54
#define editPertAttScrollWin    UxEditPertContext->UxeditPertAttScrollWin
#define editPertAttScrollList   UxEditPertContext->UxeditPertAttScrollList
#define label12                 UxEditPertContext->Uxlabel12
#define editPertFreqUpdateLabel UxEditPertContext->UxeditPertFreqUpdateLabel
#define form55                  UxEditPertContext->Uxform55
#define editPertUnattScrollWin  UxEditPertContext->UxeditPertUnattScrollWin
#define editPertUnattScrollList UxEditPertContext->UxeditPertUnattScrollList
#define label14                 UxEditPertContext->Uxlabel14
#define UxParent                UxEditPertContext->UxUxParent


Widget	editPert;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_editPert();
int SD_editpert (int , TIPO_PERT *, int , int );
int richiesta_editPertRate (Widget);
int updateEditPertLabel (Widget);
int displayPert (Widget );
int attiva_timer_editPert (Widget );
int read_background_color (Widget);
int read_foreground_color (Widget);
int creaCampiVarSec (Widget , Widget ,M_COMP_TYPE *, MALF_SET *, M_COMPONENT *);


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

int uscita_editPert ()
{
edit_pert_interface_active = 0;
free (perturbazioni);
free (perturbazioni_old);
stop_timer (timer_editPert);
DistruggiInterfaccia (XtParent(editPert));
}
/******************************************************/
int leggiPert ()
{
   if (SD_editpert (BANCO, perturbazioni,0,_MAX_PERTUR) > 0)
      fprintf (stderr,"PERT ricevute\n");
   else
      fprintf (stderr,"PERT *** ERRORE ricezione PERT ***\n");
   return (0);
}
/******************************************************/

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_editPertMenuFileQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	uscita_editPert();
	}
	UxEditPertContext = UxSaveCtx;
}

static	void	activateCB_editPertMenuEditTimer( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	richiesta_editPertRate (editPert);
	updateEditPertLabel (editPertFreqUpdateLabel);
	}
	UxEditPertContext = UxSaveCtx;
}

static	void	unmapCB_form53( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	
	}
	UxEditPertContext = UxSaveCtx;
}

static	void	activateCB_editPertQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	uscita_editPert ();
	
	}
	UxEditPertContext = UxSaveCtx;
}

static	void	singleSelectionCB_editPertAttScrollList( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	
	}
	UxEditPertContext = UxSaveCtx;
}

static	void	singleSelectionCB_editPertUnattScrollList( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCeditPert            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxEditPertContext;
	UxEditPertContext = UxContext =
			(_UxCeditPert *) UxGetContext( UxWidget );
	{
	
	}
	UxEditPertContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_editPert()
{
	Widget		_UxParent;
	Widget		editPertMenuFile_shell;
	Widget		editPertMenuEdit_shell;


	/* Creation of editPert */
	_UxParent = XtVaCreatePopupShell( "editPert_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 410,
			XmNy, 320,
			XmNwidth, 600,
			XmNheight, 560,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "editPert",
			NULL );

	editPert = XtVaCreateWidget( "editPert",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 600,
			XmNheight, 560,
			NULL );
	UxPutContext( editPert, (char *) UxEditPertContext );


	/* Creation of editPertMenu */
	editPertMenu = XtVaCreateManagedWidget( "editPertMenu",
			xmRowColumnWidgetClass,
			editPert,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( editPertMenu, (char *) UxEditPertContext );


	/* Creation of editPertMenuFile */
	editPertMenuFile_shell = XtVaCreatePopupShell ("editPertMenuFile_shell",
			xmMenuShellWidgetClass, editPertMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	editPertMenuFile = XtVaCreateWidget( "editPertMenuFile",
			xmRowColumnWidgetClass,
			editPertMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( editPertMenuFile, (char *) UxEditPertContext );


	/* Creation of editPertMenuFileQuit */
	editPertMenuFileQuit = XtVaCreateManagedWidget( "editPertMenuFileQuit",
			xmPushButtonWidgetClass,
			editPertMenuFile,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( editPertMenuFileQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_editPertMenuFileQuit,
		(XtPointer) UxEditPertContext );

	UxPutContext( editPertMenuFileQuit, (char *) UxEditPertContext );


	/* Creation of menu10_top_b1 */
	menu10_top_b1 = XtVaCreateManagedWidget( "menu10_top_b1",
			xmCascadeButtonWidgetClass,
			editPertMenu,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, editPertMenuFile,
			NULL );
	UxPutContext( menu10_top_b1, (char *) UxEditPertContext );


	/* Creation of editPertMenuEdit */
	editPertMenuEdit_shell = XtVaCreatePopupShell ("editPertMenuEdit_shell",
			xmMenuShellWidgetClass, editPertMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	editPertMenuEdit = XtVaCreateWidget( "editPertMenuEdit",
			xmRowColumnWidgetClass,
			editPertMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( editPertMenuEdit, (char *) UxEditPertContext );


	/* Creation of editPertMenuEditTimer */
	editPertMenuEditTimer = XtVaCreateManagedWidget( "editPertMenuEditTimer",
			xmPushButtonWidgetClass,
			editPertMenuEdit,
			RES_CONVERT( XmNlabelString, "Set Timer ..." ),
			NULL );
	XtAddCallback( editPertMenuEditTimer, XmNactivateCallback,
		(XtCallbackProc) activateCB_editPertMenuEditTimer,
		(XtPointer) UxEditPertContext );

	UxPutContext( editPertMenuEditTimer, (char *) UxEditPertContext );


	/* Creation of menu10_top_b2 */
	menu10_top_b2 = XtVaCreateManagedWidget( "menu10_top_b2",
			xmCascadeButtonWidgetClass,
			editPertMenu,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, editPertMenuEdit,
			NULL );
	UxPutContext( menu10_top_b2, (char *) UxEditPertContext );


	/* Creation of frameComm */
	frameComm = XtVaCreateManagedWidget( "frameComm",
			xmFrameWidgetClass,
			editPert,
			XmNx, 48,
			XmNy, 288,
			XmNwidth, 690,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frameComm, (char *) UxEditPertContext );


	/* Creation of form53 */
	form53 = XtVaCreateManagedWidget( "form53",
			xmFormWidgetClass,
			frameComm,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 20,
			XmNwidth, 851,
			XmNheight, 82,
			NULL );
	XtAddCallback( form53, XmNunmapCallback,
		(XtCallbackProc) unmapCB_form53,
		(XtPointer) UxEditPertContext );

	UxPutContext( form53, (char *) UxEditPertContext );


	/* Creation of editPertQuitpb */
	editPertQuitpb = XtVaCreateManagedWidget( "editPertQuitpb",
			xmPushButtonWidgetClass,
			form53,
			XmNx, 240,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 52,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNbottomOffset, 15,
			XmNleftPosition, 40,
			XmNleftOffset, 0,
			XmNrightPosition, 60,
			NULL );
	XtAddCallback( editPertQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_editPertQuitpb,
		UxGetWidget(editPert) );

	UxPutContext( editPertQuitpb, (char *) UxEditPertContext );


	/* Creation of panedWindow3 */
	panedWindow3 = XtVaCreateManagedWidget( "panedWindow3",
			xmPanedWindowWidgetClass,
			editPert,
			XmNx, 110,
			XmNy, 130,
			XmNwidth, 360,
			XmNheight, 260,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frameComm,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, editPertMenu,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( panedWindow3, (char *) UxEditPertContext );


	/* Creation of form54 */
	form54 = XtVaCreateManagedWidget( "form54",
			xmFormWidgetClass,
			panedWindow3,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 70,
			XmNy, 50,
			XmNwidth, 380,
			XmNheight, 130,
			XmNpaneMinimum, 200,
			NULL );
	UxPutContext( form54, (char *) UxEditPertContext );


	/* Creation of editPertAttScrollWin */
	editPertAttScrollWin = XtVaCreateManagedWidget( "editPertAttScrollWin",
			xmScrolledWindowWidgetClass,
			form54,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 0,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNwidth, 180,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNleftOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 30,
			NULL );
	UxPutContext( editPertAttScrollWin, (char *) UxEditPertContext );


	/* Creation of editPertAttScrollList */
	editPertAttScrollList = XtVaCreateManagedWidget( "editPertAttScrollList",
			xmListWidgetClass,
			editPertAttScrollWin,
			XmNwidth, 170,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( editPertAttScrollList, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_editPertAttScrollList,
		(XtPointer) UxEditPertContext );

	UxPutContext( editPertAttScrollList, (char *) UxEditPertContext );


	/* Creation of label12 */
	label12 = XtVaCreateManagedWidget( "label12",
			xmLabelWidgetClass,
			form54,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 190,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 2,
			XmNbottomWidget, editPertAttScrollWin,
			XmNleftOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 2,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, ACT_PERT_LABEL ),
			NULL );
	UxPutContext( label12, (char *) UxEditPertContext );


	/* Creation of editPertFreqUpdateLabel */
	editPertFreqUpdateLabel = XtVaCreateManagedWidget( "editPertFreqUpdateLabel",
			xmLabelWidgetClass,
			form54,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 190,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 2,
			XmNbottomWidget, editPertAttScrollWin,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 2,
			RES_CONVERT( XmNlabelString, PERT_FREQ_UPD ),
			NULL );
	UxPutContext( editPertFreqUpdateLabel, (char *) UxEditPertContext );


	/* Creation of form55 */
	form55 = XtVaCreateManagedWidget( "form55",
			xmFormWidgetClass,
			panedWindow3,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 70,
			XmNy, 50,
			XmNwidth, 380,
			XmNheight, 130,
			NULL );
	UxPutContext( form55, (char *) UxEditPertContext );


	/* Creation of editPertUnattScrollWin */
	editPertUnattScrollWin = XtVaCreateManagedWidget( "editPertUnattScrollWin",
			xmScrolledWindowWidgetClass,
			form55,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 0,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNwidth, 180,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 30,
			NULL );
	UxPutContext( editPertUnattScrollWin, (char *) UxEditPertContext );


	/* Creation of editPertUnattScrollList */
	editPertUnattScrollList = XtVaCreateManagedWidget( "editPertUnattScrollList",
			xmListWidgetClass,
			editPertUnattScrollWin,
			XmNwidth, 170,
			XmNheight, 115,
			XmNautomaticSelection, TRUE,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );
	XtAddCallback( editPertUnattScrollList, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_editPertUnattScrollList,
		(XtPointer) UxEditPertContext );

	UxPutContext( editPertUnattScrollList, (char *) UxEditPertContext );


	/* Creation of label14 */
	label14 = XtVaCreateManagedWidget( "label14",
			xmLabelWidgetClass,
			form55,
			XmNx, 10,
			XmNy, 0,
			XmNwidth, 190,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 2,
			XmNbottomWidget, editPertUnattScrollWin,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 2,
			XmNleftOffset, 5,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, UNACT_PERT_LABEL ),
			NULL );
	UxPutContext( label14, (char *) UxEditPertContext );

	XtVaSetValues(form53,
			XmNdefaultButton, NULL,
			NULL );


	XtAddCallback( editPert, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxEditPertContext);


	return ( editPert );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_editPert( _UxUxParent )
	swidget	_UxUxParent;
{
	Widget                  rtrn;
	_UxCeditPert            *UxContext;

	UxEditPertContext = UxContext =
		(_UxCeditPert *) UxNewContext( sizeof(_UxCeditPert), False );

	UxParent = _UxUxParent;

	{
		edit_pert_interface_active = 1;
		perturbazioni = (TIPO_PERT *)malloc (sizeof(TIPO_PERT)*_MAX_PERTUR*2);
		perturbazioni_old = (TIPO_PERT *)malloc (sizeof(TIPO_PERT)*_MAX_PERTUR*2);
		rtrn = _Uxbuild_editPert();

		leggiPert();
		memcpy (perturbazioni_old,perturbazioni,sizeof(TIPO_PERT)*_MAX_PERTUR*2);
		displayPert (rtrn);
		updateEditPertLabel (editPertFreqUpdateLabel);
		attiva_timer_editPert (UxGetWidget(rtrn));
		UxPopupInterface (rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

