
/*******************************************************************************
       IconShell.c
       (Generated from interface file IconShell.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
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
   modulo IconShell.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)IconShell.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include "config.h"
#include "libutilx.h"

WidgetClass istanzia_wid_class;
Widget *lista_widlib;


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
	Widget	UxIconShell;
	Widget	UxIconMainWindow1;
	Widget	UxmenuIcon;
	Widget	UxmenuIconFile;
	Widget	UxClose;
	Widget	UxFile;
	Widget	Uxform1;
	Widget	UxIconScrolledWindow;
	Widget	UxIconDrawingArea;
	XrmValue	UxValue;
	unsigned char	*UxTipo;
	int	UxNum_widget;
	ICONLIB	*UxPlib;
	unsigned char	*UxLibname;
} _UxCIconShell;

static _UxCIconShell           *UxIconShellContext;
#define IconShell               UxIconShellContext->UxIconShell
#define IconMainWindow1         UxIconShellContext->UxIconMainWindow1
#define menuIcon                UxIconShellContext->UxmenuIcon
#define menuIconFile            UxIconShellContext->UxmenuIconFile
#define Close                   UxIconShellContext->UxClose
#define File                    UxIconShellContext->UxFile
#define form1                   UxIconShellContext->Uxform1
#define IconScrolledWindow      UxIconShellContext->UxIconScrolledWindow
#define IconDrawingArea         UxIconShellContext->UxIconDrawingArea
#define Value                   UxIconShellContext->UxValue
#define Tipo                    UxIconShellContext->UxTipo
#define Num_widget              UxIconShellContext->UxNum_widget
#define Plib                    UxIconShellContext->UxPlib
#define Libname                 UxIconShellContext->UxLibname



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_IconShell();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_Close( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCIconShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxIconShellContext;
	UxIconShellContext = UxContext =
			(_UxCIconShell *) UxGetContext( UxWidget );
	UxPopdownInterface(IconShell);
	UxIconShellContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_IconShell()
{
	Widget		menuIconFile_shell;


	/* Creation of IconShell */
	IconShell = XtVaCreatePopupShell( "IconShell",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 1040,
			XmNy, 0,
			XmNwidth, 240,
			XmNheight, 1020,
			XmNtitle, "Icon Library",
			NULL );
	UxPutContext( IconShell, (char *) UxIconShellContext );


	/* Creation of IconMainWindow1 */
	IconMainWindow1 = XtVaCreateManagedWidget( "IconMainWindow1",
			xmMainWindowWidgetClass,
			IconShell,
			XmNunitType, XmPIXELS,
			XmNx, 40,
			XmNy, 250,
			XmNwidth, 110,
			XmNheight, 310,
			NULL );
	UxPutContext( IconMainWindow1, (char *) UxIconShellContext );


	/* Creation of menuIcon */
	menuIcon = XtVaCreateManagedWidget( "menuIcon",
			xmRowColumnWidgetClass,
			IconMainWindow1,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( menuIcon, (char *) UxIconShellContext );


	/* Creation of menuIconFile */
	menuIconFile_shell = XtVaCreatePopupShell ("menuIconFile_shell",
			xmMenuShellWidgetClass, menuIcon,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menuIconFile = XtVaCreateWidget( "menuIconFile",
			xmRowColumnWidgetClass,
			menuIconFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menuIconFile, (char *) UxIconShellContext );


	/* Creation of Close */
	Close = XtVaCreateManagedWidget( "Close",
			xmPushButtonWidgetClass,
			menuIconFile,
			RES_CONVERT( XmNlabelString, "Close" ),
			NULL );
	XtAddCallback( Close, XmNactivateCallback,
		(XtCallbackProc) activateCB_Close,
		(XtPointer) UxIconShellContext );

	UxPutContext( Close, (char *) UxIconShellContext );


	/* Creation of File */
	File = XtVaCreateManagedWidget( "File",
			xmCascadeButtonWidgetClass,
			menuIcon,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, menuIconFile,
			NULL );
	UxPutContext( File, (char *) UxIconShellContext );


	/* Creation of form1 */
	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass,
			IconMainWindow1,
			NULL );
	UxPutContext( form1, (char *) UxIconShellContext );


	/* Creation of IconScrolledWindow */
	IconScrolledWindow = XtVaCreateManagedWidget( "IconScrolledWindow",
			xmScrolledWindowWidgetClass,
			form1,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 240,
			XmNheight, 940,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( IconScrolledWindow, (char *) UxIconShellContext );


	/* Creation of IconDrawingArea */
	IconDrawingArea = XtVaCreateManagedWidget( "IconDrawingArea",
			xmDrawingAreaWidgetClass,
			IconScrolledWindow,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, -30,
			XmNy, 4,
			XmNwidth, 798,
			XmNheight, 990,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNnavigationType, XmNONE,
			XmNtraversalOn, FALSE,
			NULL );
	UxPutContext( IconDrawingArea, (char *) UxIconShellContext );


	XtAddCallback( IconShell, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxIconShellContext);

	XmMainWindowSetAreas( IconMainWindow1, menuIcon, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, form1 );

	return ( IconShell );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_IconShell( _UxLibname )
	unsigned char	*_UxLibname;
{
	Widget                  rtrn;
	_UxCIconShell           *UxContext;

	UxIconShellContext = UxContext =
		(_UxCIconShell *) UxNewContext( sizeof(_UxCIconShell), False );

	Libname = _UxLibname;

	{
		AN_OBJ *lista_icone = NULL;
		
		iconlib_init(Libname,&Plib);
		iconlib_getres(Plib,&lista_icone);
		rtrn = _Uxbuild_IconShell();

		Plib->topwidget = (Widget )rtrn;
		Plib->drawing = (Widget )IconDrawingArea;
		
		/*
		 *  setta le dimensioni della drawing area
		 */
		set_something(IconDrawingArea,XmNwidth,(void*) Plib->draw_width);
		set_something(IconDrawingArea,XmNheight,(void*) Plib->draw_height);
		
		iconlib_create_widget(Plib,lista_icone);
		iconlib_def_translation(Plib);
		if(lista_icone != NULL)
		   libera_memoria((char*)lista_icone);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

