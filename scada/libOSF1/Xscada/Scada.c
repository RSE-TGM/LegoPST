/**********************************************************************
*
*       C Source:               Scada.c
*       Subsystem:              1
*       Description:
*       %created_by:    mauric %
*       %date_created:  Thu Aug  6 16:21:38 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: Scada.c-3 %  (%full_filespec: Scada.c-3:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#ifdef XSCADA_INTERFACE
/*******************************************************************************
	Scada.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>
#include "LogBitmap.bmp"
#include "DebugBitmap.bmp"
#include "MandbBitmap.bmp"
#include "ScadaIconBitmap.bmp"

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

Widget LogWidget = NULL;
Widget DebugWidget = NULL;
Widget MandbWidget = NULL;
char *XScadaLog = NULL;
char *XScadaDebug[100];
extern double tempo_d;

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
	Widget	UxScada;
	Widget	UxmainWindow1;
	Widget	Uxmenu2;
	Widget	Uxmenu2_p1;
	Widget	Uxmenu2_p1_b1;
	Widget	Uxmenu2_p1_b2;
	Widget	Uxmenu2_p1_b3;
	Widget	Uxmenu2_p1_b4;
	Widget	Uxmenu2_p1_b5;
	Widget	Uxmenu2_top_b1;
	Widget	UxpanedWindow1;
	Widget	Uxform1;
	Widget	Uxlabel1;
	Widget	Uxform2;
	Widget	UxButtonLog;
	Widget	UxButtonMandb;
	Widget	UxButtonDebug;
	Widget	Uxlabel2;
	Widget	Uxlabel3;
	Widget	Uxlabel4;
        XtIntervalId    Uxtimer_license;
} _UxCScada;

#define Scada                   UxScadaContext->UxScada
#define mainWindow1             UxScadaContext->UxmainWindow1
#define menu2                   UxScadaContext->Uxmenu2
#define menu2_p1                UxScadaContext->Uxmenu2_p1
#define menu2_p1_b1             UxScadaContext->Uxmenu2_p1_b1
#define menu2_p1_b2             UxScadaContext->Uxmenu2_p1_b2
#define menu2_p1_b3             UxScadaContext->Uxmenu2_p1_b3
#define menu2_p1_b4             UxScadaContext->Uxmenu2_p1_b4
#define menu2_p1_b5             UxScadaContext->Uxmenu2_p1_b5
#define menu2_top_b1            UxScadaContext->Uxmenu2_top_b1
#define panedWindow1            UxScadaContext->UxpanedWindow1
#define form1                   UxScadaContext->Uxform1
#define label1                  UxScadaContext->Uxlabel1
#define form2                   UxScadaContext->Uxform2
#define ButtonLog               UxScadaContext->UxButtonLog
#define ButtonMandb             UxScadaContext->UxButtonMandb
#define ButtonDebug             UxScadaContext->UxButtonDebug
#define label2                  UxScadaContext->Uxlabel2
#define label3                  UxScadaContext->Uxlabel3
#define label4                  UxScadaContext->Uxlabel4
#define timer_license           UxScadaContext->Uxtimer_license

static _UxCScada	*UxScadaContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Scada();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

activate_logs()
{
XAddScadaLog("Popup Log\n");
if(LogWidget==NULL)
        {
        LogWidget = popup_ScadaLogMain();
        UxPopupInterface(LogWidget, no_grab);
        }
else
        XRaiseWindow( XtDisplay(LogWidget),XtWindow(LogWidget));
}

activate_mandb()
{
XAddScadaLog("Popup Mandb\n");
if(MandbWidget==NULL)
        {
        MandbWidget = create_ScadaMandbMain();
        UxPopupInterface(MandbWidget, no_grab);
        }
else
        XRaiseWindow( XtDisplay(MandbWidget),XtWindow(MandbWidget));
}

activate_debug()
{
XAddScadaLog("Popup Debug\n");
if(DebugWidget==NULL)
        {
        DebugWidget = popup_ScadaDebugMain();        
	UxPopupInterface(DebugWidget, no_grab);
        }
else
        XRaiseWindow( XtDisplay(DebugWidget),XtWindow(DebugWidget));
}

void startup_XScada()
{
int i;

for(i=0;i<100;i++)
        {
        XScadaDebug[i] = XtMalloc(2);
        strcpy(XScadaDebug[i],"");
        }
}

expireLicense(Widget cw)
{
#ifndef DESIGN_TIME
_UxCScada	*Context;
int             ret;
double          tempo_lic_corr;



Context =(_UxCScada *) UxGetContext(cw);
Context->Uxtimer_license = XtAppAddTimeOut (
            XtWidgetToApplicationContext (cw),
            (unsigned long) (2000) ,expireLicense, cw);

/*
 *   Si controlla se la licenza e' ancora disponibile
 */
   ret = getTimeLicense(&tempo_lic_corr);
   if(ret == 0) {
      if(tempo_lic_corr > tempo_d) {
/*
 *   Scaduta disponibilita' licenza.
 */
         printf("###################################################\n");
         printf("###################################################\n");
         printf("#                                                 #\n");
         printf("#           SCADA LICENSE IS EXPIRED!!!           #\n");
         printf("#                                                 #\n");
         printf("###################################################\n");
         printf("###################################################\n");
         QuitScada(0);
      }
   }

#endif
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_menu2_p1_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	activate_logs();
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_menu2_p1_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	activate_mandb();
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_menu2_p1_b3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	activate_debug();
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_menu2_p1_b5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	QuitScada(0);
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_ButtonLog( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	{
	activate_logs();
	}
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_ButtonMandb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	activate_mandb();
	UxScadaContext = UxSaveCtx;
}

static void	activateCB_ButtonDebug( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCScada               *UxSaveCtx, *UxContext;

	UxSaveCtx = UxScadaContext;
	UxScadaContext = UxContext =
			(_UxCScada *) UxGetContext( UxWidget );
	activate_debug();
	UxScadaContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Scada()
{
	Widget	menu2_p1_shell;
	Pixmap pix;

	Scada = XtVaCreatePopupShell( "Scada",
			topLevelShellWidgetClass, UxTopLevel,
			XmNheight, 350,
			XmNwidth, 350,
			XmNy, 220,
			XmNx, 230,
			NULL );

	UxPutContext( Scada, (char *) UxScadaContext );

	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass, Scada,
			XmNheight, 330,
			XmNwidth, 320,
			XmNy, 20,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( mainWindow1, (char *) UxScadaContext );

	menu2 = XtVaCreateManagedWidget( "menu2",
			xmRowColumnWidgetClass, mainWindow1,
			XmNmenuAccelerator, "<KeyUp>F10",
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu2, (char *) UxScadaContext );

	menu2_p1_shell = XtVaCreatePopupShell ("menu2_p1_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2_p1 = XtVaCreateWidget( "menu2_p1",
			xmRowColumnWidgetClass, menu2_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu2_p1, (char *) UxScadaContext );

	menu2_p1_b1 = XtVaCreateManagedWidget( "menu2_p1_b1",
			xmPushButtonWidgetClass, menu2_p1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Log" ),
			NULL );

	UxPutContext( menu2_p1_b1, (char *) UxScadaContext );

	menu2_p1_b2 = XtVaCreateManagedWidget( "menu2_p1_b2",
			xmPushButtonWidgetClass, menu2_p1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Mandb" ),
			NULL );

	UxPutContext( menu2_p1_b2, (char *) UxScadaContext );

	menu2_p1_b3 = XtVaCreateManagedWidget( "menu2_p1_b3",
			xmPushButtonWidgetClass, menu2_p1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Debug" ),
			NULL );

	UxPutContext( menu2_p1_b3, (char *) UxScadaContext );

	menu2_p1_b4 = XtVaCreateManagedWidget( "menu2_p1_b4",
			xmSeparatorGadgetClass, menu2_p1,
			NULL );

	UxPutContext( menu2_p1_b4, (char *) UxScadaContext );

	menu2_p1_b5 = XtVaCreateManagedWidget( "menu2_p1_b5",
			xmPushButtonWidgetClass, menu2_p1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			RES_CONVERT( XmNlabelString, "Exit" ),
			NULL );

	UxPutContext( menu2_p1_b5, (char *) UxScadaContext );

	menu2_top_b1 = XtVaCreateManagedWidget( "menu2_top_b1",
			xmCascadeButtonWidgetClass, menu2,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			XmNsubMenuId, menu2_p1,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu2_top_b1, (char *) UxScadaContext );

	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass, mainWindow1,
			RES_CONVERT( XmNforeground, "#5050a0" ),
			RES_CONVERT( XmNtopShadowColor, "#a05050" ),
			RES_CONVERT( XmNbottomShadowColor, "#50a0a0" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			NULL );

	UxPutContext( panedWindow1, (char *) UxScadaContext );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, panedWindow1,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 100,
			XmNwidth, 290,
			XmNy, 50,
			XmNx, 20,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form1, (char *) UxScadaContext );

	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass, form1,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--25-180-100-100-p-138-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Scada" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 120,
			XmNwidth, 320,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( label1, (char *) UxScadaContext );

	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass, panedWindow1,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 160,
			XmNwidth, 340,
			XmNy, 140,
			XmNx, 0,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form2, (char *) UxScadaContext );

	ButtonLog = XtVaCreateManagedWidget( "ButtonLog",
			xmPushButtonWidgetClass, form2,
			XmNleftPosition, 4,
			XmNtopPosition, 20,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 90,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNlabelType, XmPIXMAP,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 100,
			XmNwidth, 80,
			XmNy, 30,
			XmNx, 20,
			NULL );

	UxPutContext( ButtonLog, (char *) UxScadaContext );

	ButtonMandb = XtVaCreateManagedWidget( "ButtonMandb",
			xmPushButtonWidgetClass, form2,
			XmNtopOffset, 0,
			XmNtopPosition, 20,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNleftPosition, 40,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNlabelType, XmPIXMAP,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 90,
			XmNwidth, 100,
			XmNy, 50,
			XmNx, 130,
			NULL );

	UxPutContext( ButtonMandb, (char *) UxScadaContext );

	ButtonDebug = XtVaCreateManagedWidget( "ButtonDebug",
			xmPushButtonWidgetClass, form2,
			XmNrightPosition, 95,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopPosition, 20,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNlabelType, XmPIXMAP,
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 100,
			XmNwidth, 80,
			XmNy, 40,
			XmNx, 250,
			NULL );

	UxPutContext( ButtonDebug, (char *) UxScadaContext );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, form2,
			RES_CONVERT( XmNlabelString, "Log" ),
			XmNtopWidget, ButtonLog,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftPosition, 5,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomWidget, ButtonLog,
			XmNbottomPosition, 80,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 30,
			XmNwidth, 50,
			XmNy, 120,
			XmNx, 20,
			NULL );

	UxPutContext( label2, (char *) UxScadaContext );

	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass, form2,
			RES_CONVERT( XmNlabelString, "Mandb" ),
			XmNleftPosition, 40,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopWidget, ButtonMandb,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomPosition, 80,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 120,
			XmNx, 140,
			NULL );

	UxPutContext( label3, (char *) UxScadaContext );

	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass, form2,
			XmNrightPosition, 95,
			XmNrightAttachment, XmATTACH_POSITION,
			RES_CONVERT( XmNlabelString, "Debug" ),
			XmNtopWidget, ButtonDebug,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftPosition, 83,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomPosition, 80,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#50a0a0" ),
			XmNheight, 30,
			XmNwidth, 60,
			XmNy, 120,
			XmNx, 270,
			NULL );

	UxPutContext( label4, (char *) UxScadaContext );

	XtAddCallback( Scada, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxScadaContext );

	XtAddCallback( menu2_p1_b1, XmNactivateCallback,
			activateCB_menu2_p1_b1,
			(XtPointer) UxScadaContext );

	XtAddCallback( menu2_p1_b2, XmNactivateCallback,
			activateCB_menu2_p1_b2,
			(XtPointer) UxScadaContext );

	XtAddCallback( menu2_p1_b3, XmNactivateCallback,
			activateCB_menu2_p1_b3,
			(XtPointer) UxScadaContext );

	XtAddCallback( menu2_p1_b5, XmNactivateCallback,
			activateCB_menu2_p1_b5,
			(XtPointer) UxScadaContext );

/*
	XtVaSetValues(ButtonLog,
			RES_CONVERT( XmNlabelPixmap, "/usr/users/scada/xscada/LogBitmap.bmp" ),
			NULL );
*/
	LoadBitmap(ButtonLog,&pix,LogBitmap_bits,LogBitmap_width,
		LogBitmap_height);
        set_something_val(ButtonLog,XmNlabelPixmap, (XtArgVal) pix);

	XtAddCallback( ButtonLog, XmNactivateCallback,
			activateCB_ButtonLog,
			(XtPointer) UxScadaContext );

/*
	XtVaSetValues(ButtonMandb,
			RES_CONVERT( XmNlabelPixmap, "/usr/users/scada/xscada/MandbBitmap.bmp" ),
			NULL );
*/
	LoadBitmap(ButtonMandb,&pix,MandbBitmap_bits,MandbBitmap_width,
		MandbBitmap_height);
        set_something_val(ButtonMandb,XmNlabelPixmap, (XtArgVal) pix);

	XtAddCallback( ButtonMandb, XmNactivateCallback,
			activateCB_ButtonMandb,
			(XtPointer) UxScadaContext );

/*
	XtVaSetValues(ButtonDebug,
			RES_CONVERT( XmNlabelPixmap, "/usr/users/scada/xscada/DebugBitmap.bmp" ),
			NULL );
*/
	LoadBitmap(ButtonDebug,&pix,DebugBitmap_bits,DebugBitmap_width,
		DebugBitmap_height);
        set_something_val(ButtonDebug,XmNlabelPixmap, (XtArgVal) pix);

	XtAddCallback( ButtonDebug, XmNactivateCallback,
			activateCB_ButtonDebug,
			(XtPointer) UxScadaContext );


	XmMainWindowSetAreas( mainWindow1, menu2, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, panedWindow1 );

	LoadBitmap(Scada,&pix,ScadaIconBitmap_bits,ScadaIconBitmap_width,
		ScadaIconBitmap_height);
        set_something_val(Scada,XmNiconPixmap, (XtArgVal) pix);

	return ( Scada );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Scada()
{
	Widget                  rtrn;
	_UxCScada               *UxContext;

	UxScadaContext = UxContext =
		(_UxCScada *) XtMalloc( sizeof(_UxCScada) );

	rtrn = _Uxbuild_Scada();
/*
   Si attiva un timer per la gestione a tempo delle licenze.
   Ogni 2 sec viene richiamata la funzione expireLicense che controlla se
   e' scaduto o meno il tempo di utilizzo della licenza.
*/
        timer_license = XtAppAddTimeOut (
                        XtWidgetToApplicationContext (rtrn),
                        (unsigned long) (2000), expireLicense, rtrn);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Scada()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Scada();

	return ( _Uxrtrn );
}
#endif
/*******************************************************************************
	END OF FILE
*******************************************************************************/

