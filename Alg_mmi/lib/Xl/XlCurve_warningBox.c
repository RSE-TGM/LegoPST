
/*******************************************************************************
       XlCurve_warningBox.c
       (Generated from interface file XlCurve_warningBox.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */



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
	Widget	UxwarningDialog1;
	swidget	UxUxParent;
	int	Uxparametro;
} _UxCwarningDialog1;

static _UxCwarningDialog1      *UxWarningDialog1Context;
#define warningDialog1          UxWarningDialog1Context->UxwarningDialog1
#define UxParent                UxWarningDialog1Context->UxUxParent
#define parametro               UxWarningDialog1Context->Uxparametro



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_warningDialog1();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_warningDialog1()
{
	Widget		_UxParent;


	/* Creation of warningDialog1 */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "warningDialog1_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 340,
			XmNy, 520,
			XmNwidth, 400,
			XmNheight, 220,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "warningDialog1",
			NULL );

	warningDialog1 = XtVaCreateWidget( "warningDialog1",
			xmMessageBoxWidgetClass,
			_UxParent,
			XmNdialogType, XmDIALOG_WARNING,
			XmNwidth, 400,
			XmNheight, 220,
			XmNunitType, XmPIXELS,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNmessageAlignment, XmALIGNMENT_CENTER,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( warningDialog1, (char *) UxWarningDialog1Context );


	XtAddCallback( warningDialog1, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxWarningDialog1Context);


	return ( warningDialog1 );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_warningDialog1( _UxUxParent, _Uxparametro )
	swidget	_UxUxParent;
	int	_Uxparametro;
{
	Widget                  rtrn;
	_UxCwarningDialog1      *UxContext;

	UxWarningDialog1Context = UxContext =
		(_UxCwarningDialog1 *) UxNewContext( sizeof(_UxCwarningDialog1), False );

	UxParent = _UxUxParent;
	parametro = _Uxparametro;

	{
		XmString msg;
		rtrn = _Uxbuild_warningDialog1();

		switch (parametro) {
		      case 0:
		         msg=XmStringCreateSimple("Error in 'Var Lego' field!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 1:
		         msg=XmStringCreateSimple("Error in Low Lim field!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 2:
		         msg=XmStringCreateSimple("Low Lim greather than High Lim!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 3:
		         msg=XmStringCreateSimple("Unit Mis string is empty!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 4:
		         msg=XmStringCreateSimple("Error in change color field!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 5:
		         msg=XmStringCreateSimple("Error opening file f22circ!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 6:
		         msg=XmStringCreateSimple("Error reading name-descr variables!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 7:
		         msg=XmStringCreateSimple("Error in High Lim field!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		      case 8:
		         msg=XmStringCreateSimple("Error in selection field!!!");
		         XtVaSetValues(warningDialog1,XmNmessageString,msg,NULL);
		         XtFree(msg);
		         break;
		   }
		   UxPopupInterface(rtrn,no_grab);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

