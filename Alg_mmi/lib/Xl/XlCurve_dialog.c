
/*******************************************************************************
       XlCurve_dialog.c
       (Generated from interface file XlCurve_dialog.i)
*******************************************************************************/

#include <stdio.h>

/*
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include <Xm/DrawingA.h>
*/

#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include "UxXt.h"
        
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrollBar.h>
#include <Xm/MessageB.h>
#include <Mrm/MrmPublic.h> 
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <X11/Shell.h>
#include <Xl/XlP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlGrafico.h>
#include <Xl/XlGraficoP.h>
#include "libutilx.h"
#include "sim_param.h"


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
	Widget	UxhandleVar;
	Widget	Uxform1;
	Widget	Uxtitle;
	Widget	UxrowColumn1;
	Widget	UxtoggleButton1;
	Widget	UxtoggleButton2;
	Widget	UxtoggleButton3;
	Widget	UxpushButOk;
	Widget	Uxpb_exit;
	swidget	UxUxParent;
	swidget	Uxcurve_obj;
	int	*Uxtoggle_state;
	int	*Uxnum_scale_vis;
} _UxChandleVar;

static _UxChandleVar           *UxHandleVarContext;
#define handleVar               UxHandleVarContext->UxhandleVar
#define form1                   UxHandleVarContext->Uxform1
#define title                   UxHandleVarContext->Uxtitle
#define rowColumn1              UxHandleVarContext->UxrowColumn1
#define toggleButton1           UxHandleVarContext->UxtoggleButton1
#define toggleButton2           UxHandleVarContext->UxtoggleButton2
#define toggleButton3           UxHandleVarContext->UxtoggleButton3
#define pushButOk               UxHandleVarContext->UxpushButOk
#define pb_exit                 UxHandleVarContext->Uxpb_exit
#define UxParent                UxHandleVarContext->UxUxParent
#define curve_obj               UxHandleVarContext->Uxcurve_obj
#define toggle_state            UxHandleVarContext->Uxtoggle_state
#define num_scale_vis           UxHandleVarContext->Uxnum_scale_vis

extern void*	UxNewContext (size_t , int );

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_handleVar();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButOk( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxChandleVar           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxHandleVarContext;
	UxHandleVarContext = UxContext =
			(_UxChandleVar *) UxGetContext( UxWidget );
	{
	/*
	   Si controlla quale toggle e' stato settato e si assegna
	   opportunamente il parametro ricevuto dalla callback.
	*/
	if(XmToggleButtonGetState(toggleButton1)) 
	   *toggle_state=1;
	else if(XmToggleButtonGetState(toggleButton2)) {
	   *toggle_state=2;
	   *num_scale_vis=(*num_scale_vis)+1;
	   printf("DIALOGSHELL1: num_scale_vis=%d\n",*num_scale_vis);
	}
	else if(XmToggleButtonGetState(toggleButton3))
	   *toggle_state=3;
	else
	   *toggle_state=0;
	 
	XtDestroyWidget(UxThisWidget);
	
	
	}
	UxHandleVarContext = UxSaveCtx;
}

static	void	activateCB_pb_exit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxChandleVar           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxHandleVarContext;
	UxHandleVarContext = UxContext =
			(_UxChandleVar *) UxGetContext( UxWidget );
	{
	XtDestroyWidget(UxThisWidget);
	
	
	}
	UxHandleVarContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_handleVar()
{
	Widget		_UxParent;


	/* Creation of handleVar */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	handleVar = XtVaCreatePopupShell( "handleVar",
			xmDialogShellWidgetClass,
			_UxParent,
			XmNwidth, 250,
			XmNheight, 380,
			XmNx, 700,
			XmNy, 650,
			NULL );
	UxPutContext( handleVar, (char *) UxHandleVarContext );


	/* Creation of form1 */
	form1 = XtVaCreateWidget( "form1",
			xmFormWidgetClass,
			handleVar,
			XmNwidth, 250,
			XmNheight, 260,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 10,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNnoResize, TRUE,
			NULL );
	UxPutContext( form1, (char *) UxHandleVarContext );


	/* Creation of title */
	title = XtVaCreateManagedWidget( "title",
			xmLabelWidgetClass,
			form1,
			XmNx, 5,
			XmNy, 0,
			XmNwidth, 240,
			XmNheight, 20,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( title, (char *) UxHandleVarContext );


	/* Creation of rowColumn1 */
	rowColumn1 = XtVaCreateManagedWidget( "rowColumn1",
			xmRowColumnWidgetClass,
			form1,
			XmNwidth, 200,
			XmNheight, 170,
			XmNx, 10,
			XmNy, 50,
			XmNentryAlignment, XmALIGNMENT_BEGINNING,
			XmNpacking, XmPACK_COLUMN,
			XmNspacing, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNradioBehavior, TRUE,
			XmNradioAlwaysOne, FALSE,
			NULL );
	UxPutContext( rowColumn1, (char *) UxHandleVarContext );


	/* Creation of toggleButton1 */
	toggleButton1 = XtVaCreateManagedWidget( "toggleButton1",
			xmToggleButtonWidgetClass,
			rowColumn1,
			XmNx, 20,
			XmNy, 20,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNwidth, 180,
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-100-100-100-m-90-iso8859-1" ),
			NULL );
	UxPutContext( toggleButton1, (char *) UxHandleVarContext );


	/* Creation of toggleButton2 */
	toggleButton2 = XtVaCreateManagedWidget( "toggleButton2",
			xmToggleButtonWidgetClass,
			rowColumn1,
			XmNx, 13,
			XmNy, 13,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-100-100-100-m-90-iso8859-1" ),
			NULL );
	UxPutContext( toggleButton2, (char *) UxHandleVarContext );


	/* Creation of toggleButton3 */
	toggleButton3 = XtVaCreateManagedWidget( "toggleButton3",
			xmToggleButtonWidgetClass,
			rowColumn1,
			XmNx, 13,
			XmNy, 41,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-100-100-100-m-90-iso8859-1" ),
			NULL );
	UxPutContext( toggleButton3, (char *) UxHandleVarContext );


	/* Creation of pushButOk */
	pushButOk = XtVaCreateManagedWidget( "pushButOk",
			xmPushButtonWidgetClass,
			form1,
			XmNx, 20,
			XmNy, 220,
			XmNheight, 35,
			XmNwidth, 60,
			XmNtopOffset, 100,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( pushButOk, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButOk,
		(XtPointer) UxHandleVarContext );

	UxPutContext( pushButOk, (char *) UxHandleVarContext );


	/* Creation of pb_exit */
	pb_exit = XtVaCreateManagedWidget( "pb_exit",
			xmPushButtonWidgetClass,
			form1,
			XmNx, 120,
			XmNy, 220,
			XmNheight, 35,
			XmNwidth, 120,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	XtAddCallback( pb_exit, XmNactivateCallback,
		(XtCallbackProc) activateCB_pb_exit,
		(XtPointer) UxHandleVarContext );

	UxPutContext( pb_exit, (char *) UxHandleVarContext );


	XtAddCallback( handleVar, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxHandleVarContext);


	return ( handleVar );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_handleVar( _UxUxParent, _Uxcurve_obj, _Uxtoggle_state, _Uxnum_scale_vis )
	swidget	_UxUxParent;
	swidget	_Uxcurve_obj;
	int	*_Uxtoggle_state;
	int	*_Uxnum_scale_vis;
{
	Widget                  rtrn;
	_UxChandleVar           *UxContext;

	UxHandleVarContext = UxContext =
		(_UxChandleVar *) UxNewContext( sizeof(_UxChandleVar), False );

	UxParent = _UxUxParent;
	curve_obj = _Uxcurve_obj;
	toggle_state = _Uxtoggle_state;
	num_scale_vis = _Uxnum_scale_vis;

	rtrn = _Uxbuild_handleVar();

	printf("create_handleVar: toggle_state=%d\tnum_scale_vis=%d\n",*toggle_state,*num_scale_vis);
	
	switch (*toggle_state) {
	   case 1:
	      XmToggleButtonSetState(toggleButton1,True,False);
	      break;
	   case 2:
	      XmToggleButtonSetState(toggleButton2,True,False);
	      break;
	   case 3:
	      XmToggleButtonSetState(toggleButton3,True,False);
	      break;
	}
	UxPopupInterface(rtrn,no_grab);
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

