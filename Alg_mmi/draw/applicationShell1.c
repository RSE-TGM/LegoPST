
/*******************************************************************************
       applicationShell1.c
       (Generated from interface file applicationShell1.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo applicationShell1.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)applicationShell1.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#include <draw.h>
#include <Xd/XdLista.h>
#include <Xd/XdUndo.h>
/* dichiarazioni prima implicite (vedi Alg_mmi/lib/Xl/XlProto.h) */
void  set_something_val();/* AlgLib/libutilx */
extern swidget create_drawShell();
extern char *nome_file_draw;
extern int width_draw;
extern int height_draw;
extern int curr_step;
#ifndef ON_PAGEDIT
extern Widget FinestraDraw;
#endif


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
	Widget	UxapplicationShell1;
	Widget	UxmainWindow1;
	Widget	UxscrolledWindow1;
	Widget	UxdrawingArea3;
	Widget	UxDrawMainMenu;
	Widget	Uxmenu1_p1_title;
	Widget	UxUndoButton;
	Widget	UxDuplicateButton;
	Widget	UxCutButton;
	Widget	UxCopyButton;
	Widget	UxPasteButton;
	Widget	UxDeleteButton;
	Widget	UxSeparator_b9;
	Widget	UxTopButton;
	Widget	UxBottomButton;
	Widget	UxSeparator_b12;
	Widget	UxGroupButton;
	Widget	UxUngroupButton;
	XdLista	Uxlista_draget;
	XdLista	Uxlista_draget_undo;
	XdListaUndo	Uxlista_liste_undo;
	ST_DRAW_WID	Uxst_draw;
} _UxCapplicationShell1;

static _UxCapplicationShell1   *UxApplicationShell1Context;
#define applicationShell1       UxApplicationShell1Context->UxapplicationShell1
#define mainWindow1             UxApplicationShell1Context->UxmainWindow1
#define scrolledWindow1         UxApplicationShell1Context->UxscrolledWindow1
#define drawingArea3            UxApplicationShell1Context->UxdrawingArea3
#define DrawMainMenu            UxApplicationShell1Context->UxDrawMainMenu
#define menu1_p1_title          UxApplicationShell1Context->Uxmenu1_p1_title
#define UndoButton              UxApplicationShell1Context->UxUndoButton
#define DuplicateButton         UxApplicationShell1Context->UxDuplicateButton
#define CutButton               UxApplicationShell1Context->UxCutButton
#define CopyButton              UxApplicationShell1Context->UxCopyButton
#define PasteButton             UxApplicationShell1Context->UxPasteButton
#define DeleteButton            UxApplicationShell1Context->UxDeleteButton
#define Separator_b9            UxApplicationShell1Context->UxSeparator_b9
#define TopButton               UxApplicationShell1Context->UxTopButton
#define BottomButton            UxApplicationShell1Context->UxBottomButton
#define Separator_b12           UxApplicationShell1Context->UxSeparator_b12
#define GroupButton             UxApplicationShell1Context->UxGroupButton
#define UngroupButton           UxApplicationShell1Context->UxUngroupButton
#define lista_draget            UxApplicationShell1Context->Uxlista_draget
#define lista_draget_undo       UxApplicationShell1Context->Uxlista_draget_undo
#define lista_liste_undo        UxApplicationShell1Context->Uxlista_liste_undo
#define st_draw                 UxApplicationShell1Context->Uxst_draw


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
		XmMenuPosition( menu, (XButtonPressedEvent *) event );
		XtManageChild( menu );
	}
}


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_applicationShell1();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

XdLista get_lista(w)
Widget w;
{
XdLista ret_lista;
#ifndef DESIGN_TIME
        _UxCapplicationShell1   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxApplicationShell1Context;
        UxApplicationShell1Context = UxContext =
                        (_UxCapplicationShell1 *) UxGetContext( w );
#endif
ret_lista= lista_draget;
#ifndef DESIGN_TIME
 UxApplicationShell1Context = UxSaveCtx;
#endif
return(ret_lista);
}


XdLista get_lista_undo(w)
Widget w;
{
XdLista ret_lista;
#ifndef DESIGN_TIME
        _UxCapplicationShell1   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxApplicationShell1Context;
        UxApplicationShell1Context = UxContext =
                        (_UxCapplicationShell1 *) UxGetContext( w );
#endif
ret_lista= lista_draget_undo;
#ifndef DESIGN_TIME
 UxApplicationShell1Context = UxSaveCtx;
#endif
return(ret_lista);
}

void CreaUndoList(w)
Widget w;
{
#ifndef DESIGN_TIME
        _UxCapplicationShell1   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxApplicationShell1Context;
        UxApplicationShell1Context = UxContext =
                        (_UxCapplicationShell1 *) UxGetContext( w );
#endif
if(lista_draget_undo != NULL)
        XtFree((char *)lista_draget_undo);

lista_draget_undo=XdCreateLista();
#ifndef DESIGN_TIME
 UxApplicationShell1Context = UxSaveCtx;
#endif
}


XdListaUndo get_lista_liste_undo(w)
Widget w;
{
XdListaUndo ret_lista;
#ifndef DESIGN_TIME
        _UxCapplicationShell1   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxApplicationShell1Context;
        UxApplicationShell1Context = UxContext =
                        (_UxCapplicationShell1 *) UxGetContext( w );
#endif
ret_lista= lista_liste_undo;
#ifndef DESIGN_TIME
 UxApplicationShell1Context = UxSaveCtx;
#endif
return(ret_lista);
}



/*
 restituisce o setta una risorsa tra quelle definite
 nella struttura descrittiva delle risorse per il widget di disegno
 (ST_DRAW_WID) definita in draw.h (in legommi/include)
*/ 
int gs_draw_ris(Widget w,enum ris_draw_wid ris_num, void *ret,Boolean set)
{
int *retint;
GC *retGC;
Widget *retWid;
float *retfloat;
Region *retreg;
#ifndef DESIGN_TIME
        _UxCapplicationShell1   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxApplicationShell1Context;
        UxApplicationShell1Context = UxContext =
                        (_UxCapplicationShell1 *) UxGetContext( w );
#endif
switch(ris_num)
	{
        case DR_REGION:
        retreg=ret;
        if(set)
                st_draw.region=*retreg;
        else
                *retreg=st_draw.region;
        break;

	case DR_LINE_STYLE:
	retint=ret;
	if(set)
		st_draw.line_style=*retint;
	else
		*retint=st_draw.line_style;
	break;
	case DR_LINE_WIDTH:
	retint=ret;
	if(set)
		st_draw.line_width=*retint;
	else
		*retint=st_draw.line_width;
	break;
	case DR_DEF_LINE_STYLE:
	retint=ret;
	if(set)
		st_draw.def_line_style=*retint;
	else
		*retint=st_draw.def_line_style;
	break;
	case DR_DEF_LINE_WIDTH:
	retint=ret;
	if(set)
		st_draw.def_line_width=*retint;
	else
		*retint=st_draw.def_line_width;
	break;
	case DR_GC:
	retGC=ret;
	if(set)
		st_draw.gc=*retGC;
	else
		*retGC= st_draw.gc;
	break;
	case DR_GC_BG:
	retGC=ret;
	if(set)
		st_draw.gc_bg=*retGC;
	else
		*retGC= st_draw.gc_bg;
	break;
	case DR_DEF_GC:
	retGC=ret;
	if(set)
		st_draw.def_gc=*retGC;
	else
		*retGC= st_draw.def_gc;
	break;
	case DR_DEF_GC_BG:
	retGC=ret;
	if(set)
		st_draw.def_gc_bg=*retGC;
	else
		*retGC= st_draw.def_gc_bg;
	break;
	case DR_STEP:
	retint=ret;
	if(set)
		st_draw.step= *retint;
	else
		*retint=st_draw.step;
	break;
	case DR_GRID_ON:
	retint=ret;
	if(set)
		st_draw.grid_on= *retint;
	else
		*retint=st_draw.grid_on;
	break;
	case DR_DEF_FILLED:
	retint=ret;
	if(set)
		st_draw.def_filled= *retint;
	else
		*retint=st_draw.def_filled;
	break;
	case DR_DEF_ZOOM:
	retfloat=ret;
	if(set)
		st_draw.def_zoom=*retfloat;
	else
		*retfloat=st_draw.def_zoom;
	break;

	case DR_BOTT_FILL:
	retWid=ret;
	if(set)
		st_draw.bott_fill= *retWid;
	else
		*retWid= st_draw.bott_fill;
	break;

	case DR_BOTT_BORDER:
	retWid=ret;
	if(set)
		st_draw.bott_border= *retWid;
	else
		*retWid= st_draw.bott_border;
	break;
	
	case DR_DEF_BOTT_FILL:
	retWid=ret;
	if(set)
		st_draw.def_bott_fill= *retWid;
	else
		*retWid= st_draw.def_bott_fill;
	break;
	
	case DR_DEF_BOTT_BORDER:
	retWid=ret;
	if(set)
		st_draw.def_bott_border= *retWid;
	else
		*retWid= st_draw.def_bott_border;
	break;

	case DR_BOTT_ARROW:
	retWid=ret;
	if(set)
		st_draw.bott_arrow= *retWid;
	else
		*retWid= st_draw.bott_arrow;
	break;
	}

#ifndef DESIGN_TIME
 UxApplicationShell1Context = UxSaveCtx;
#endif
return(ret != NULL);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_UndoButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	
	extern int undo_draget_paste();
	
	/* undo sulla ultima operazione effettuata
	*/
	
	undo_draget_paste( FinestraDraw );
	
#endif
	
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_DuplicateButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	int dx,dy;
	extern int curr_step;
	
	if(curr_step > 1)
		dx=dy=curr_step;
	else
		dx=dy=5;
	
#ifndef ON_PAGEDIT
	draget_duplicate(FinestraDraw,dx,dy);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_CutButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	draget_cutS(FinestraDraw);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_CopyButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	draget_copyS(FinestraDraw);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_PasteButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	draget_paste(FinestraDraw);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_DeleteButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	DrawDelete(FinestraDraw, NULL);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_TopButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	DrawPutTop(FinestraDraw);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_BottomButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	DrawPutBottom(FinestraDraw);
#endif
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_GroupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	DrawGroup(FinestraDraw);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

static	void	activateCB_UngroupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCapplicationShell1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxApplicationShell1Context;
	UxApplicationShell1Context = UxContext =
			(_UxCapplicationShell1 *) UxGetContext( UxWidget );
	{
	DrawUngroup(FinestraDraw);
	}
	UxApplicationShell1Context = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_applicationShell1()
{
	Widget		DrawMainMenu_shell;


	/* Creation of applicationShell1 */
	applicationShell1 = XtVaCreatePopupShell( "applicationShell1",
			applicationShellWidgetClass,
			UxTopLevel,
			XmNx, 0,
			XmNy, 403,
			XmNwidth, 606,
			XmNheight, 520,
			XmNtitle, "Editing Animated Icon",
			XmNiconName, "Editing Animated Icon",
			NULL );
	UxPutContext( applicationShell1, (char *) UxApplicationShell1Context );


	/* Creation of mainWindow1 */
	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass,
			applicationShell1,
			XmNunitType, XmPIXELS,
			XmNwidth, 491,
			XmNheight, 450,
			NULL );
	UxPutContext( mainWindow1, (char *) UxApplicationShell1Context );


	/* Creation of scrolledWindow1 */
	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass,
			mainWindow1,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );
	UxPutContext( scrolledWindow1, (char *) UxApplicationShell1Context );


	/* Creation of drawingArea3 */
	drawingArea3 = XtVaCreateManagedWidget( "drawingArea3",
			xmDrawingAreaWidgetClass,
			scrolledWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 4,
			XmNy, 4,
			XmNwidth, XDisplayWidth(UxDisplay,UxScreen),
			XmNheight, XDisplayHeight(UxDisplay,UxScreen),
			NULL );
	UxPutContext( drawingArea3, (char *) UxApplicationShell1Context );


	/* Creation of DrawMainMenu */
	DrawMainMenu_shell = XtVaCreatePopupShell ("DrawMainMenu_shell",
			xmMenuShellWidgetClass, drawingArea3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	DrawMainMenu = XtVaCreateWidget( "DrawMainMenu",
			xmRowColumnWidgetClass,
			DrawMainMenu_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "<KeyUp>F4",
			NULL );
	UxPutContext( DrawMainMenu, (char *) UxApplicationShell1Context );


	/* Creation of menu1_p1_title */
	menu1_p1_title = XtVaCreateManagedWidget( "menu1_p1_title",
			xmLabelWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Edit menu" ),
			NULL );
	UxPutContext( menu1_p1_title, (char *) UxApplicationShell1Context );


	/* Creation of UndoButton */
	UndoButton = XtVaCreateManagedWidget( "UndoButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Undo" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			NULL );
	XtAddCallback( UndoButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_UndoButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( UndoButton, (char *) UxApplicationShell1Context );


	/* Creation of DuplicateButton */
	DuplicateButton = XtVaCreateManagedWidget( "DuplicateButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Duplicate" ),
			NULL );
	XtAddCallback( DuplicateButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_DuplicateButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( DuplicateButton, (char *) UxApplicationShell1Context );


	/* Creation of CutButton */
	CutButton = XtVaCreateManagedWidget( "CutButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( CutButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_CutButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( CutButton, (char *) UxApplicationShell1Context );


	/* Creation of CopyButton */
	CopyButton = XtVaCreateManagedWidget( "CopyButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( CopyButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_CopyButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( CopyButton, (char *) UxApplicationShell1Context );


	/* Creation of PasteButton */
	PasteButton = XtVaCreateManagedWidget( "PasteButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( PasteButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_PasteButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( PasteButton, (char *) UxApplicationShell1Context );


	/* Creation of DeleteButton */
	DeleteButton = XtVaCreateManagedWidget( "DeleteButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( DeleteButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_DeleteButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( DeleteButton, (char *) UxApplicationShell1Context );


	/* Creation of Separator_b9 */
	Separator_b9 = XtVaCreateManagedWidget( "Separator_b9",
			xmSeparatorWidgetClass,
			DrawMainMenu,
			NULL );
	UxPutContext( Separator_b9, (char *) UxApplicationShell1Context );


	/* Creation of TopButton */
	TopButton = XtVaCreateManagedWidget( "TopButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Put Top" ),
			NULL );
	XtAddCallback( TopButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_TopButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( TopButton, (char *) UxApplicationShell1Context );


	/* Creation of BottomButton */
	BottomButton = XtVaCreateManagedWidget( "BottomButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Put Bottom" ),
			NULL );
	XtAddCallback( BottomButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_BottomButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( BottomButton, (char *) UxApplicationShell1Context );


	/* Creation of Separator_b12 */
	Separator_b12 = XtVaCreateManagedWidget( "Separator_b12",
			xmSeparatorWidgetClass,
			DrawMainMenu,
			NULL );
	UxPutContext( Separator_b12, (char *) UxApplicationShell1Context );


	/* Creation of GroupButton */
	GroupButton = XtVaCreateManagedWidget( "GroupButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Group" ),
			NULL );
	XtAddCallback( GroupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_GroupButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( GroupButton, (char *) UxApplicationShell1Context );


	/* Creation of UngroupButton */
	UngroupButton = XtVaCreateManagedWidget( "UngroupButton",
			xmPushButtonWidgetClass,
			DrawMainMenu,
			RES_CONVERT( XmNlabelString, "Ungroup" ),
			NULL );
	XtAddCallback( UngroupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_UngroupButton,
		(XtPointer) UxApplicationShell1Context );

	UxPutContext( UngroupButton, (char *) UxApplicationShell1Context );


	XtAddCallback( applicationShell1, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxApplicationShell1Context);

	XtAddEventHandler(drawingArea3, ButtonPressMask,
			False, (XtEventHandler) _UxapplicationShell1MenuPost, (XtPointer) DrawMainMenu );
	XmMainWindowSetAreas( mainWindow1, (Widget) NULL, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, scrolledWindow1 );

	return ( applicationShell1 );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_applicationShell1()
{
	Widget                  rtrn;
	_UxCapplicationShell1   *UxContext;

	UxApplicationShell1Context = UxContext =
		(_UxCapplicationShell1 *) UxNewContext( sizeof(_UxCapplicationShell1), False );


	{
		lista_draget=XdCreateLista();
		lista_draget_undo=NULL;
		lista_liste_undo=undoCreateLists();
		/*
		 Inizializza a valori di default alcuni campi della
		 struttura descrivente gli attributi del disegno
		*/
		init_st_draw(&st_draw);
		rtrn = _Uxbuild_applicationShell1();

		/*
		 Se Draw e' richiamato da solo crea la draw shell
		*/
#ifndef ON_PAGEDIT
		UxPopupInterface(create_drawShell(drawingArea3,nome_file_draw),no_grab);
		/*
		 Carica il file se e' stato specificato da linea di comando
		*/
		if(nome_file_draw)
			{
			ReadBackground(drawingArea3,nome_file_draw);
			DrawSetSnap(drawingArea3,curr_step);
			if(curr_step >1)
				set_grid_on(drawingArea3,True);
			}
		set_something_val(rtrn,XmNx,(XtArgVal) 0);
		set_something_val(rtrn,XmNy,(XtArgVal) 403);
		set_something_val(rtrn,XmNtitle,(XtArgVal) nome_file_draw);
#endif
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

