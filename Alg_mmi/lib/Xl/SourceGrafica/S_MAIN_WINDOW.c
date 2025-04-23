/**********************************************************************
*
*       C Source:               S_MAIN_WINDOW.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:10:35 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: S_MAIN_WINDOW.c,2 %  (%full_filespec: 1,csrc,S_MAIN_WINDOW.c,2 %)";
#endif
/*
   modulo S_MAIN_WINDOW.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)S_MAIN_WINDOW.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)S_MAIN_WINDOW.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*******************************************************************************
	S_MAIN_WINDOW.c

*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xl/Xl.h>
#include <Xl/XlGrafico.h>
#include "S_MAIN_WINDOW.h"

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

#include "libutilx.h"


static _UxCS_MAIN_WINDOW	*UxS_MAIN_WINDOWContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_S_MAIN_WINDOW();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_S_MAIN_WINDOW(parent,PuntGraf)
Widget parent;
S_XLGRAFICO *PuntGraf;
{
S_XLGRAFICO *pXlGraf;
Dimension width_grafico,height_grafico;
Widget	S_MAIN_WINDOW_shell;
Widget	menu1_shell;
Widget	pop_file_shell;
Widget	pop_grafico_shell;
Widget	pop_misure_shell;
Widget	pop_def_shell;
WidgetClass ClasseOggetto;

/*  Setto il puntatore della struttura variabili del grafics nella variabile
	specifica  */
S_MAIN_WINDOW_Graf = PuntGraf;
pXlGraf = PuntGraf;

ClasseOggetto = XtClass (XtParent(parent));
if ( strcmp(XlGetClassName(ClasseOggetto),"Composite") )
  {
  S_MAIN_WINDOW_shell =XtParent(XtParent(parent));
  S_MAIN_WINDOW = XtParent(XtParent(parent));
  }
else
  {
  S_MAIN_WINDOW_shell =XtParent(parent);
  S_MAIN_WINDOW = XtParent(parent);
  }

get_something(parent, XmNwidth, (void*) &width_grafico);
get_something(parent, XmNheight, (void*) &height_grafico);
width_grafico-=10;
height_grafico-=10;

	contenitore = XtVaCreateManagedWidget( "contenitore",
			xmFormWidgetClass, parent,
			XmNx, 5,
			XmNy, 5,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNhorizontalSpacing, 15,
			XmNverticalSpacing, 0,
			XmNwidth, width_grafico,
			XmNheight, height_grafico,
			XmNresizePolicy,XmRESIZE_ANY,
			RES_CONVERT( XmNbackground, "light gray" ),
			NULL );

	UxPutContext( contenitore, (char *) UxS_MAIN_WINDOWContext );

	create_proc( contenitore,
			1, (XtPointer) NULL );

	tempo = XtVaCreateManagedWidget( "tempo",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNx, 930,
			XmNy, 370,
			XmNwidth, 83,
			XmNheight, 20,
			RES_CONVERT( XmNbackground, "black" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 40,
			NULL );

	UxPutContext( tempo, (char *) UxS_MAIN_WINDOWContext );

	create_proc( tempo,
			16, (XtPointer) NULL );

	ord1 = XtVaCreateManagedWidget( "ord1",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNx, 930,
			XmNy, 30,
			XmNwidth, 83,
			XmNheight, 300,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 1,
			XmNbottomWidget, tempo,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 1,
			RES_CONVERT( XmNbackground, "black" ),
			NULL );

	UxPutContext( ord1, (char *) UxS_MAIN_WINDOWContext );

	create_proc( ord1,
			9, (XtPointer) NULL );

	val1 = XtVaCreateManagedWidget( "val1",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNresizable, FALSE,
			XmNmarginWidth, 2,
			XmNmarginHeight, 2,
			XmNx, 880,
			XmNy, 442,
			XmNwidth, 115,
			XmNheight, 60,
			RES_CONVERT( XmNbackground, "black" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 2,
			XmNrightWidget, ord1,
			XmNleftAttachment, XmATTACH_NONE,
			XmNleftOffset, 0,
			NULL );

	UxPutContext( val1, (char *) UxS_MAIN_WINDOWContext );

	create_proc( val1,
			14, (XtPointer) NULL );

	mis1 = XtVaCreateManagedWidget( "mis1",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNrightOffset, 1,
			XmNwidth, 825,
			XmNmarginWidth, 1,
			XmNmarginHeight, 1,
			XmNrightWidget, val1,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNx, 15,
			XmNy, 442,
			XmNheight, 60,
			RES_CONVERT( XmNbackground, "black" ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( mis1, (char *) UxS_MAIN_WINDOWContext );

	create_proc( mis1,
			7, (XtPointer) NULL );

	tim1 = XtVaCreateManagedWidget( "tim1",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNborderWidth, 1,
			XmNx, 10,
			XmNy, 330,
			XmNwidth, 900,
			XmNheight, 20,
			RES_CONVERT( XmNbackground, "navy blue" ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 1,
			XmNbottomWidget, mis1,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 2,
			XmNrightWidget, ord1,
			NULL );

	UxPutContext( tim1, (char *) UxS_MAIN_WINDOWContext );

	create_proc( tim1,
			8, (XtPointer) NULL );

	draw0 = XtVaCreateManagedWidget( "draw0",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNtopOffset, 1,
			XmNrightWidget, ord1,
			XmNx, 1,
			XmNy, 1,
			XmNwidth, 1023,
			XmNheight, 20,
			RES_CONVERT( XmNbackground, "navy blue" ),
			XmNborderWidth, 0,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 2,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( draw0, (char *) UxS_MAIN_WINDOWContext );

	create_proc( draw0,
			5, (XtPointer) NULL );

	draw1 = XtVaCreateManagedWidget( "draw1",
			xmDrawingAreaWidgetClass, contenitore,
			XmNtraversalOn, FALSE,
                        XmNnavigationType, XmNONE,
			XmNborderWidth, 0,
			XmNx, 10,
			XmNy, 30,
			XmNwidth, 900,
			XmNheight, 300,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 1,
			XmNtopWidget, draw0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 1,
			XmNbottomWidget, tim1,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 2,
			XmNrightWidget, ord1,
			RES_CONVERT( XmNbackground, "black" ),
			NULL );

	UxPutContext( draw1, (char *) UxS_MAIN_WINDOWContext );

	create_proc( draw1,
			6, (XtPointer) NULL );

	menu1_shell = XtVaCreatePopupShell ("menu1_shell",
			xmMenuShellWidgetClass, draw1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1 = XtVaCreateWidget( "menu1",
			xmRowColumnWidgetClass, menu1_shell,
			XmNwhichButton, 2,  
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu1, (char *) UxS_MAIN_WINDOWContext );
	create_proc( menu1, 10, (XtPointer) NULL );

	pop_file_shell = XtVaCreatePopupShell ("pop_file_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	pop_file = XtVaCreateWidget( "pop_file",
			xmRowColumnWidgetClass, pop_file_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( pop_file, (char *) UxS_MAIN_WINDOWContext );

#ifndef SCADA_MMI
	p_dir_button = XtVaCreateManagedWidget( "p_dir_button",
			xmPushButtonWidgetClass, pop_file,
			RES_CONVERT( XmNlabelString, "Open archive file" ),
			NULL );

	UxPutContext( p_dir_button, (char *) UxS_MAIN_WINDOWContext );
#endif

	p_HC_button = XtVaCreateManagedWidget( "p_HC_button",
			xmPushButtonWidgetClass, pop_file,
			RES_CONVERT( XmNlabelString, "Hard Copy" ),
			NULL );

	UxPutContext( p_HC_button, (char *) UxS_MAIN_WINDOWContext );

	file_pd = XtVaCreateManagedWidget( "file_pd",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, pop_file,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( file_pd, (char *) UxS_MAIN_WINDOWContext );

	pop_grafico_shell = XtVaCreatePopupShell ("pop_grafico_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	pop_grafico = XtVaCreateWidget( "pop_grafico",
			xmRowColumnWidgetClass, pop_grafico_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( pop_grafico, (char *) UxS_MAIN_WINDOWContext );

	p_zoom_control_button = XtVaCreateManagedWidget( "p_zoom_control_button",
			xmPushButtonWidgetClass, pop_grafico,
			RES_CONVERT( XmNlabelString, "Zoom" ),
			NULL );

	UxPutContext( p_zoom_control_button, (char *) UxS_MAIN_WINDOWContext );

	p_nozoom_control_button = XtVaCreateManagedWidget( "p_nozoom_control_button",
			xmPushButtonWidgetClass, pop_grafico,
			RES_CONVERT( XmNlabelString, "Redraw" ),
			NULL );

	UxPutContext( p_nozoom_control_button, (char *) UxS_MAIN_WINDOWContext );

#ifndef SCADA_MMI
	menu1_p2_b3 = XtVaCreateManagedWidget( "menu1_p2_b3",
			xmSeparatorWidgetClass, pop_grafico,
			NULL );

	UxPutContext( menu1_p2_b3, (char *) UxS_MAIN_WINDOWContext );

	p_selgra_control_button = XtVaCreateManagedWidget( "p_selgra_control_button",
			xmPushButtonWidgetClass, pop_grafico,
			RES_CONVERT( XmNlabelString, "Select group's variables" ),
			NULL );

	UxPutContext( p_selgra_control_button, (char *) UxS_MAIN_WINDOWContext );

	p_memgra_control_button = XtVaCreateManagedWidget( "p_memgra_control_button",
			xmPushButtonWidgetClass, pop_grafico,
			RES_CONVERT( XmNlabelString, "Store group's variables" ),
			NULL );

	UxPutContext( p_memgra_control_button, (char *) UxS_MAIN_WINDOWContext );
#endif

	grafici_pd = XtVaCreateManagedWidget( "grafici_pd",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, pop_grafico,
			RES_CONVERT( XmNlabelString, "Graph" ),
			NULL );

	UxPutContext( grafici_pd, (char *) UxS_MAIN_WINDOWContext );

	create_proc( grafici_pd,
			(char *)84, (XtPointer) NULL );

	pop_misure_shell = XtVaCreatePopupShell ("pop_misure_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	pop_misure = XtVaCreateWidget( "pop_misure",
			xmRowColumnWidgetClass, pop_misure_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( pop_misure, (char *) UxS_MAIN_WINDOWContext );

	p_umis_control_button = XtVaCreateManagedWidget( "p_umis_control_button",
			xmPushButtonWidgetClass, pop_misure,
			RES_CONVERT( XmNlabelString, "Change units" ),
			NULL );

	UxPutContext( p_umis_control_button, (char *) UxS_MAIN_WINDOWContext );

	p_fsca_control_button = XtVaCreateManagedWidget( "p_fsca_control_button",
			xmPushButtonWidgetClass, pop_misure,
			RES_CONVERT( XmNlabelString, "Change Y scaling" ),
			NULL );

	UxPutContext( p_fsca_control_button, (char *) UxS_MAIN_WINDOWContext );

#ifndef SCADA_MMI
	menu1_p3_b3 = XtVaCreateManagedWidget( "menu1_p3_b3",
			xmSeparatorWidgetClass, pop_misure,
			NULL );

	UxPutContext( menu1_p3_b3, (char *) UxS_MAIN_WINDOWContext );

	p_selmis_control_button = XtVaCreateManagedWidget( "p_selmis_control_button",
			xmPushButtonWidgetClass, pop_misure,
			RES_CONVERT( XmNlabelString, "Select variable" ),
			NULL );

	UxPutContext( p_selmis_control_button, (char *) UxS_MAIN_WINDOWContext );
#endif

	misure_pd = XtVaCreateManagedWidget( "misure_pd",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, pop_misure,
			RES_CONVERT( XmNlabelString, "Variables" ),
			NULL );

	UxPutContext( misure_pd, (char *) UxS_MAIN_WINDOWContext );

	create_proc( misure_pd,
			(char *) 85, (XtPointer) NULL );

	pop_def_shell = XtVaCreatePopupShell ("pop_def_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	pop_def = XtVaCreateWidget( "pop_def",
			xmRowColumnWidgetClass, pop_def_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( pop_def, (char *) UxS_MAIN_WINDOWContext );

	p_defumis_button = XtVaCreateManagedWidget( "p_defumis_button",
			xmPushButtonWidgetClass, pop_def,
			RES_CONVERT( XmNlabelString, "Units" ),
			NULL );

	UxPutContext( p_defumis_button, (char *) UxS_MAIN_WINDOWContext );

	p_collima_button = XtVaCreateManagedWidget( "p_collima_button",
			xmPushButtonWidgetClass, pop_def,
			RES_CONVERT( XmNlabelString, "Collimate On" ),
			NULL );

	UxPutContext( p_collima_button, (char *) UxS_MAIN_WINDOWContext );

	memcollima_proc( p_collima_button,
			(XtPointer) UxS_MAIN_WINDOWContext, (XtPointer) NULL );

	p_zoomord_button = XtVaCreateManagedWidget( "p_zoomord_button",
			xmPushButtonWidgetClass, pop_def,
			RES_CONVERT( XmNlabelString, "Y Zoom On" ),
			NULL );

	UxPutContext( p_zoomord_button, (char *) UxS_MAIN_WINDOWContext );

	memzoomord_proc( p_zoomord_button,
			(XtPointer) UxS_MAIN_WINDOWContext, (XtPointer) NULL );

	p_secondi_button = XtVaCreateManagedWidget( "p_secondi_button",
			xmPushButtonWidgetClass, pop_def,
			RES_CONVERT( XmNlabelString, "Time in seconds" ),
			NULL );

	UxPutContext( p_secondi_button, (char *) UxS_MAIN_WINDOWContext );

	memsecondi_proc( p_secondi_button,
			(XtPointer) UxS_MAIN_WINDOWContext, (XtPointer) NULL );

	def_pd = XtVaCreateManagedWidget( "def_pd",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, pop_def,
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( def_pd, (char *) UxS_MAIN_WINDOWContext );

	XtAddCallback( S_MAIN_WINDOW, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxS_MAIN_WINDOWContext );

	XtAddCallback( tempo, XmNexposeCallback,
			draw_proc,
			(XtPointer)16 );

	XtAddCallback( ord1, XmNexposeCallback,
			draw_proc,
			(XtPointer)9 );
	XtAddCallback( ord1, XmNresizeCallback,
			resize_proc,
			(XtPointer)9 );

	XtAddCallback( val1, XmNexposeCallback,
			draw_proc,
			(XtPointer)14 );

	XtAddCallback( mis1, XmNexposeCallback,
			draw_proc,
			(XtPointer)7 );

	XtAddCallback( tim1, XmNexposeCallback,
			draw_proc,
			(XtPointer)8 );
	XtAddCallback( tim1, XmNresizeCallback,
			resize_proc,
			(XtPointer)8 );

	XtAddCallback( draw0, XmNexposeCallback,
			draw_proc,
			(XtPointer)5 );

	XtAddCallback( mis1, XmNinputCallback,
			PostIt,
			(XtPointer)7 );

	XtAddCallback( draw1, XmNinputCallback,
			PostIt,
			(XtPointer)6 );

	XtAddCallback( draw1, XmNexposeCallback,
			draw_proc,
			(XtPointer)6 );
	XtAddCallback( draw1, XmNresizeCallback,
			resize_proc,
			(XtPointer)6 );

#ifndef SCADA_MMI
	XtAddCallback( p_dir_button, XmNactivateCallback,
			activate_proc,
			(char *)75 );
#endif

	XtAddCallback( p_HC_button, XmNactivateCallback,
			HC_proc,
			(char *)4 );

	XtAddCallback( p_zoom_control_button, XmNactivateCallback,
			zoom_proc,
			(char *)1 );

	XtAddCallback( p_nozoom_control_button, XmNactivateCallback,
			zoom_proc,
			(char *) 0 );

#ifndef SCADA_MMI
	XtAddCallback( p_selgra_control_button, XmNactivateCallback,
			activate_proc,
			(char *)50 );

	XtAddCallback( p_memgra_control_button, XmNactivateCallback,
			activate_proc,
			(char *) 57 );
#endif

	XtAddCallback( p_umis_control_button, XmNactivateCallback,
			activate_proc,
			(char *) 32 );

	XtAddCallback( p_fsca_control_button, XmNactivateCallback,
			activate_proc,
			(char *)41 );

#ifndef SCADA_MMI
	XtAddCallback( p_selmis_control_button, XmNactivateCallback,
			activate_proc,
			(char *)20 );
#endif

	XtAddCallback( p_defumis_button, XmNactivateCallback,
			activate_proc,
			(char *)69 );

	XtAddCallback( p_collima_button, XmNactivateCallback,
			collima_proc,
			(XtPointer) UxS_MAIN_WINDOWContext );

	XtAddCallback( p_zoomord_button, XmNactivateCallback,
			zoomord_proc,
			(XtPointer) UxS_MAIN_WINDOWContext );

	XtAddCallback( p_secondi_button, XmNactivateCallback,
			secondi_proc,
			(XtPointer) UxS_MAIN_WINDOWContext );

	return ( S_MAIN_WINDOW ); /* S_MAIN_WINDOW   */
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_S_MAIN_WINDOW(parent,PuntGraf)
Widget parent;
S_XLGRAFICO *PuntGraf;
{
	Widget                  rtrn;
	_UxCS_MAIN_WINDOW       *UxContext;

	UxS_MAIN_WINDOWContext = UxContext =
		(_UxCS_MAIN_WINDOW *) XtMalloc( sizeof(_UxCS_MAIN_WINDOW) );

	rtrn = _Uxbuild_S_MAIN_WINDOW(parent,PuntGraf);

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_S_MAIN_WINDOW(parent,PuntGraf)
Widget parent;
S_XLGRAFICO *PuntGraf;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_S_MAIN_WINDOW(parent,PuntGraf);

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

