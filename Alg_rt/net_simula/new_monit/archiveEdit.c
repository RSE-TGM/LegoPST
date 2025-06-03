
/*******************************************************************************
       archiveEdit.c
       (Generated from interface file archiveEdit.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>



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
	Widget	Uxform24;
	Widget	Uxframe9;
	Widget	Uxform25;
	Widget	UxpushButton16;
	Widget	UxpushButton17;
	Widget	UxpushButton18;
	Widget	Uxlabel19;
	Widget	Uxform26;
	Widget	UxscrolledWindow2;
	Widget	UxrowColumn3;
	Widget	Uxform20;
	Widget	Uxlabel20;
	Widget	Uxlabel21;
	Widget	Uxlabel22;
	Widget	Uxlabel23;
} _UxCarchiveEdit;

static _UxCarchiveEdit         *UxArchiveEditContext;
#define form24                  UxArchiveEditContext->Uxform24
#define frame9                  UxArchiveEditContext->Uxframe9
#define form25                  UxArchiveEditContext->Uxform25
#define pushButton16            UxArchiveEditContext->UxpushButton16
#define pushButton17            UxArchiveEditContext->UxpushButton17
#define pushButton18            UxArchiveEditContext->UxpushButton18
#define label19                 UxArchiveEditContext->Uxlabel19
#define form26                  UxArchiveEditContext->Uxform26
#define scrolledWindow2         UxArchiveEditContext->UxscrolledWindow2
#define rowColumn3              UxArchiveEditContext->UxrowColumn3
#define form20                  UxArchiveEditContext->Uxform20
#define label20                 UxArchiveEditContext->Uxlabel20
#define label21                 UxArchiveEditContext->Uxlabel21
#define label22                 UxArchiveEditContext->Uxlabel22
#define label23                 UxArchiveEditContext->Uxlabel23


Widget	archiveEdit;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_archiveEdit();
int richiesta_save_arch();
int DistruggiInterfaccia (Widget );

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButton16( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveEdit         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveEditContext;
	UxArchiveEditContext = UxContext =
			(_UxCarchiveEdit *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxArchiveEditContext = UxSaveCtx;
}

static	void	activateCB_pushButton17( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveEdit         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveEditContext;
	UxArchiveEditContext = UxContext =
			(_UxCarchiveEdit *) UxGetContext( UxWidget );
	{
	/*kjkkj*/
	}
	UxArchiveEditContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_archiveEdit()
{
	Widget		_UxParent;


	/* Creation of archiveEdit */
	_UxParent = XtVaCreatePopupShell( "archiveEdit_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 665,
			XmNy, 115,
			XmNwidth, 510,
			XmNheight, 385,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "archiveEdit",
			XmNiconName, "archiveEdit",
			NULL );

	archiveEdit = XtVaCreateManagedWidget( "archiveEdit",
			xmFormWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 510,
			XmNheight, 385,
			NULL );
	UxPutContext( archiveEdit, (char *) UxArchiveEditContext );


	/* Creation of form24 */
	form24 = XtVaCreateManagedWidget( "form24",
			xmFormWidgetClass,
			archiveEdit,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 16,
			XmNy, 108,
			XmNwidth, 750,
			XmNheight, 75,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form24, (char *) UxArchiveEditContext );


	/* Creation of frame9 */
	frame9 = XtVaCreateManagedWidget( "frame9",
			xmFrameWidgetClass,
			form24,
			XmNx, 10,
			XmNy, 405,
			XmNwidth, 795,
			XmNheight, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame9, (char *) UxArchiveEditContext );


	/* Creation of form25 */
	form25 = XtVaCreateManagedWidget( "form25",
			xmFormWidgetClass,
			frame9,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 75,
			NULL );
	UxPutContext( form25, (char *) UxArchiveEditContext );


	/* Creation of pushButton16 */
	pushButton16 = XtVaCreateManagedWidget( "pushButton16",
			xmPushButtonWidgetClass,
			form25,
			XmNy, 15,
			XmNwidth, 138,
			XmNheight, 41,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_SELF,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightOffset, 20,
			XmNleftPosition, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNx, 535,
			NULL );
	XtAddCallback( pushButton16, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton16,
		UxGetWidget(archiveEdit) );

	UxPutContext( pushButton16, (char *) UxArchiveEditContext );


	/* Creation of pushButton17 */
	pushButton17 = XtVaCreateManagedWidget( "pushButton17",
			xmPushButtonWidgetClass,
			form25,
			XmNx, 30,
			XmNy, 405,
			XmNwidth, 120,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "LOAD" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_SELF,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 10,
			NULL );
	XtAddCallback( pushButton17, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton17,
		(XtPointer) UxArchiveEditContext );

	UxPutContext( pushButton17, (char *) UxArchiveEditContext );


	/* Creation of pushButton18 */
	pushButton18 = XtVaCreateManagedWidget( "pushButton18",
			xmPushButtonWidgetClass,
			form25,
			XmNx, 165,
			XmNy, 20,
			XmNwidth, 120,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "DELETE" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, pushButton17,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightOffset, 50,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNleftOffset, 15,
			NULL );
	UxPutContext( pushButton18, (char *) UxArchiveEditContext );


	/* Creation of label19 */
	label19 = XtVaCreateManagedWidget( "label19",
			xmLabelWidgetClass,
			archiveEdit,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 675,
			XmNheight, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			RES_CONVERT( XmNbackground, "red" ),
			NULL );
	UxPutContext( label19, (char *) UxArchiveEditContext );


	/* Creation of form26 */
	form26 = XtVaCreateManagedWidget( "form26",
			xmFormWidgetClass,
			archiveEdit,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 30,
			XmNwidth, 675,
			XmNheight, 250,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNrightOffset, 20,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, form24,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label19,
			XmNtopOffset, 20,
			XmNbottomOffset, 20,
			NULL );
	UxPutContext( form26, (char *) UxArchiveEditContext );


	/* Creation of scrolledWindow2 */
	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass,
			form26,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 680,
			XmNheight, 250,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( scrolledWindow2, (char *) UxArchiveEditContext );


	/* Creation of rowColumn3 */
	rowColumn3 = XtVaCreateManagedWidget( "rowColumn3",
			xmRowColumnWidgetClass,
			scrolledWindow2,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 705,
			XmNheight, 415,
			NULL );
	UxPutContext( rowColumn3, (char *) UxArchiveEditContext );


	/* Creation of form20 */
	form20 = XtVaCreateManagedWidget( "form20",
			xmFormWidgetClass,
			rowColumn3,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 3,
			XmNwidth, 598,
			XmNheight, 32,
			NULL );
	UxPutContext( form20, (char *) UxArchiveEditContext );


	/* Creation of label20 */
	label20 = XtVaCreateManagedWidget( "label20",
			xmLabelWidgetClass,
			form20,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 37,
			XmNheight, 32,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			RES_CONVERT( XmNbackground, "green" ),
			NULL );
	UxPutContext( label20, (char *) UxArchiveEditContext );


	/* Creation of label21 */
	label21 = XtVaCreateManagedWidget( "label21",
			xmLabelWidgetClass,
			form20,
			XmNx, 37,
			XmNy, 0,
			XmNwidth, 108,
			XmNheight, 32,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, label20,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightPosition, 20,
			RES_CONVERT( XmNbackground, "green" ),
			NULL );
	UxPutContext( label21, (char *) UxArchiveEditContext );


	/* Creation of label22 */
	label22 = XtVaCreateManagedWidget( "label22",
			xmLabelWidgetClass,
			form20,
			XmNx, 218,
			XmNy, 0,
			XmNwidth, 321,
			XmNheight, 32,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftWidget, label21,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightOffset, 80,
			RES_CONVERT( XmNbackground, "green" ),
			NULL );
	UxPutContext( label22, (char *) UxArchiveEditContext );


	/* Creation of label23 */
	label23 = XtVaCreateManagedWidget( "label23",
			xmLabelWidgetClass,
			form20,
			XmNx, 540,
			XmNy, 0,
			XmNwidth, 61,
			XmNheight, 32,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, label22,
			RES_CONVERT( XmNbackground, "green" ),
			NULL );
	UxPutContext( label23, (char *) UxArchiveEditContext );


	XtAddCallback( archiveEdit, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxArchiveEditContext);


	return ( archiveEdit );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_archiveEdit()
{
	Widget                  rtrn;
	_UxCarchiveEdit         *UxContext;

	UxArchiveEditContext = UxContext =
		(_UxCarchiveEdit *) UxNewContext( sizeof(_UxCarchiveEdit), False );


	{
		printf ("colloquio edit archivi\n");
		rtrn = _Uxbuild_archiveEdit();

		UxPopupInterface (archiveEdit, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

