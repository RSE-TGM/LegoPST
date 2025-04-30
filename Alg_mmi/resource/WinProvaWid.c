/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif

/*******************************************************************************
	WinProvaWid.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/DrawingA.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo WinProvaWid.c
   tipo 
   release 5.2
   data 2/13/96
   reserved @(#)WinProvaWid.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)WinProvaWid.c	5.2\t2/13/96";
/*
        Fine sezione per SCCS
*/

#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <Xl/Xl.h>
#include <Ol/OlDatabaseTopologia.h>
#include "config.h"
#include "res_edit.h"

extern  Display	*UxDisplay;
extern Widget UxTopLevel;

extern swidget create_Resource();
Arg args[50];

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
	Widget	UxWinProvaWid;
	Widget	UxmainWindow4;
	Widget	Uxmenu4;
	Widget	UxFileMenu;
	Widget	UxExitItem;
	Widget	UxFileSave;
	Widget	Uxmenu4_top_b1;
	Widget	UxConfPane;
	Widget	UxResourceItem;
	Widget	Uxmenu4_top_b2;
	Widget	UxdrawingArea1;
	XrmDatabase	UxRisDb;
	OlDatabaseTopologiaObject	UxDataBase;
} _UxCWinProvaWid;

#define WinProvaWid             UxWinProvaWidContext->UxWinProvaWid
#define mainWindow4             UxWinProvaWidContext->UxmainWindow4
#define menu4                   UxWinProvaWidContext->Uxmenu4
#define FileMenu                UxWinProvaWidContext->UxFileMenu
#define ExitItem                UxWinProvaWidContext->UxExitItem
#define FileSave                UxWinProvaWidContext->UxFileSave
#define menu4_top_b1            UxWinProvaWidContext->Uxmenu4_top_b1
#define ConfPane                UxWinProvaWidContext->UxConfPane
#define ResourceItem            UxWinProvaWidContext->UxResourceItem
#define menu4_top_b2            UxWinProvaWidContext->Uxmenu4_top_b2
#define drawingArea1            UxWinProvaWidContext->UxdrawingArea1
#define RisDb                   UxWinProvaWidContext->UxRisDb
#define DataBase                UxWinProvaWidContext->UxDataBase

static _UxCWinProvaWid	*UxWinProvaWidContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_WinProvaWid();
extern OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *, char *, int , char *,char *,char *);

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_ExitItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCWinProvaWid         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxWinProvaWidContext;
	UxWinProvaWidContext = UxContext =
			(_UxCWinProvaWid *) UxGetContext( UxWidget );
	XrmPutFileDatabase (RisDb,"FileRisorse");
	exit (0);
	UxWinProvaWidContext = UxSaveCtx;
}

static void	activateCB_FileSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCWinProvaWid         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxWinProvaWidContext;
	UxWinProvaWidContext = UxContext =
			(_UxCWinProvaWid *) UxGetContext( UxWidget );
	{
	XrmPutFileDatabase (RisDb,"FileRisorse");
	}
	UxWinProvaWidContext = UxSaveCtx;
}

static void	activateCB_ResourceItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCWinProvaWid         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxWinProvaWidContext;
	UxWinProvaWidContext = UxContext =
			(_UxCWinProvaWid *) UxGetContext( UxWidget );
	{
	PAGINA *Pagina;
	char pippo[50],messaggio[100];
	Widget pluto;
	
	Pagina = (PAGINA *)XtCalloc(1,sizeof(PAGINA));
	
	strcpy (pippo,"Nome Pagina");
	
	strcpy (Pagina->nomepag,pippo);
	Pagina->topwidget = (Widget )WinProvaWid;
	Pagina->drawing = (Widget )drawingArea1;
	Pagina->db = RisDb;
	strcpy (Pagina->geom.tipo,TIPO_LIBRERIA); 
	
	pluto = (Widget )create_Resource(Pagina,DataBase);
	
	if (pluto == NULL)
	  {
	  strcpy (messaggio,"Widget already edited!");
	  VisualizzaMessageBox (GENERICA,WinProvaWid,messaggio,drawingArea1);
	  }
	else UxPopupInterface (pluto,no_grab);
	}
	UxWinProvaWidContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_WinProvaWid()
{
	Widget	FileMenu_shell;
	Widget	ConfPane_shell;

	WinProvaWid = XtVaCreatePopupShell( "WinProvaWid",
			topLevelShellWidgetClass, UxTopLevel,
			XmNtitle, "Window Prova Oggetti",
			XmNiconName, "Prova Wid",
			XmNheight, 485,
			XmNwidth, 500,
			XmNy, 220,
			XmNx, 380,
			NULL );

	UxPutContext( WinProvaWid, (char *) UxWinProvaWidContext );

	mainWindow4 = XtVaCreateManagedWidget( "mainWindow4",
			xmMainWindowWidgetClass, WinProvaWid,
			XmNheight, 105,
			XmNwidth, 60,
			XmNy, 25,
			XmNx, 50,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( mainWindow4, (char *) UxWinProvaWidContext );

	menu4 = XtVaCreateManagedWidget( "menu4",
			xmRowColumnWidgetClass, mainWindow4,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu4, (char *) UxWinProvaWidContext );

	FileMenu_shell = XtVaCreatePopupShell ("FileMenu_shell",
			xmMenuShellWidgetClass, menu4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FileMenu = XtVaCreateWidget( "FileMenu",
			xmRowColumnWidgetClass, FileMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( FileMenu, (char *) UxWinProvaWidContext );

	ExitItem = XtVaCreateManagedWidget( "ExitItem",
			xmPushButtonWidgetClass, FileMenu,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Exit" ),
			NULL );

	UxPutContext( ExitItem, (char *) UxWinProvaWidContext );

	FileSave = XtVaCreateManagedWidget( "FileSave",
			xmPushButtonWidgetClass, FileMenu,
			RES_CONVERT( XmNlabelString, "Save" ),
			NULL );

	UxPutContext( FileSave, (char *) UxWinProvaWidContext );

	menu4_top_b1 = XtVaCreateManagedWidget( "menu4_top_b1",
			xmCascadeButtonWidgetClass, menu4,
			XmNsubMenuId, FileMenu,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu4_top_b1, (char *) UxWinProvaWidContext );

	ConfPane_shell = XtVaCreatePopupShell ("ConfPane_shell",
			xmMenuShellWidgetClass, menu4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	ConfPane = XtVaCreateWidget( "ConfPane",
			xmRowColumnWidgetClass, ConfPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( ConfPane, (char *) UxWinProvaWidContext );

	ResourceItem = XtVaCreateManagedWidget( "ResourceItem",
			xmPushButtonWidgetClass, ConfPane,
			RES_CONVERT( XmNmnemonic, "R" ),
			RES_CONVERT( XmNlabelString, "Resource" ),
			NULL );

	UxPutContext( ResourceItem, (char *) UxWinProvaWidContext );

	menu4_top_b2 = XtVaCreateManagedWidget( "menu4_top_b2",
			xmCascadeButtonWidgetClass, menu4,
			XmNsubMenuId, ConfPane,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Configure" ),
			NULL );

	UxPutContext( menu4_top_b2, (char *) UxWinProvaWidContext );

	drawingArea1 = XtVaCreateManagedWidget( "drawingArea1",
			xmDrawingAreaWidgetClass, mainWindow4,
			NULL );

	UxPutContext( drawingArea1, (char *) UxWinProvaWidContext );

	XtAddCallback( WinProvaWid, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxWinProvaWidContext );

	XtAddCallback( ExitItem, XmNactivateCallback,
			activateCB_ExitItem,
			(XtPointer) UxWinProvaWidContext );

	XtAddCallback( FileSave, XmNactivateCallback,
			activateCB_FileSave,
			(XtPointer) UxWinProvaWidContext );

	XtAddCallback( ResourceItem, XmNactivateCallback,
			activateCB_ResourceItem,
			(XtPointer) UxWinProvaWidContext );


	XmMainWindowSetAreas( mainWindow4, menu4, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, drawingArea1 );

	return ( WinProvaWid );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_WinProvaWid()
{
	Widget                  rtrn;
	_UxCWinProvaWid         *UxContext;

	UxWinProvaWidContext = UxContext =
		(_UxCWinProvaWid *) XtMalloc( sizeof(_UxCWinProvaWid) );

	{
		AN_OBJ *plista_an_obj,Appo;
		Widget Pippo;
		int i;
		XrmDatabase DatabaseAppoggio;
		rtrn = _Uxbuild_WinProvaWid();

		RisDb = XrmGetFileDatabase("FileRisorse");
		DatabaseAppoggio = (XrmDatabase )XrmGetDatabase (UxDisplay);
		XrmSetDatabase (UxDisplay,RisDb);
		
		plista_an_obj=(AN_OBJ *)XlGetAnObjList(UxTopLevel);
		/*
		 Crea gli oggetti definiti dalla lista nel file delle risorse
		*/
		i = 0;
		Appo = plista_an_obj[i];
		
		while (Appo.nome[0] != '\0')
			{
			Pippo = XtVaCreateManagedWidget (Appo.nome,
					Appo.classe, drawingArea1,
					NULL);
			i++;
			Appo = plista_an_obj[i];
			}
		
		XrmSetDatabase (UxDisplay,RisDb);
		
		/*  Creazione database topologico  */
		DataBase = OlCreateDatabaseTopologia ("olrtf",
				"/home/legoroot/legommi/resource/MIEITEST/variabili.rtf",
				2000000,
				"/home/legoroot/legommi/resource/MIEITEST/FileSubSystem.edf",
				"/home/legoroot/legommi/resource/MIEITEST/FileComponent.edf",
				"/home/legoroot/legommi/resource/MIEITEST/FileDefVarCC.edf");
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_WinProvaWid()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_WinProvaWid();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

