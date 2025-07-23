
/*******************************************************************************
	ModelConnessionWindow.c
	(Generated from interface file ModelConnessionWindow.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/CascadeB.h>
#include <Xm/PushBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/CascadeBG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo ModelConnessionWindow.i
   tipo 
   release 1.24
   data 8/29/95
   reserved @(#)ModelConnessionWindow.i	1.24
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include "topsim.h"
#include "lg1_topsim.h"
#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

/***
Find_struct parametri_ulist;
Arg argb[15];
Cardinal nargb;
***/

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
	Widget	UxModelConnessionWindow;
	Widget	UxmenuBar1;
	Widget	UxfilePane1;
	Widget	UxClose;
	Widget	UxfileCascade1;
	Widget	UxOptionPane1;
	Widget	UxDefaultFilterPane1;
	Widget	UxNoFilterDef1;
	Widget	UxSetDefFilter1;
	Widget	Uxseparator13;
	Widget	UxDefineFilter1;
	Widget	UxmenuBar_top_b1;
	Widget	UxmainPW1;
	Widget	UxmainForm1;
	Widget	UxblListLb1;
	Widget	UxfindBlock;
	Widget	UxblListSW1;
	Widget	UxblListSL1;
	Widget	UxdescrMod;
	Widget	UxWindVariable;
	MODELLO	*Uxltask;
	Find_struct	Uxparametri_ulist;
	Arg	Uxargb[15];
	Cardinal	Uxnargb;
	MODELLO	*Uxmodel;
} _UxCModelConnessionWindow;

#define ModelConnessionWindow   UxModelConnessionWindowContext->UxModelConnessionWindow
#define menuBar1                UxModelConnessionWindowContext->UxmenuBar1
#define filePane1               UxModelConnessionWindowContext->UxfilePane1
#define Close                   UxModelConnessionWindowContext->UxClose
#define fileCascade1            UxModelConnessionWindowContext->UxfileCascade1
#define OptionPane1             UxModelConnessionWindowContext->UxOptionPane1
#define DefaultFilterPane1      UxModelConnessionWindowContext->UxDefaultFilterPane1
#define NoFilterDef1            UxModelConnessionWindowContext->UxNoFilterDef1
#define SetDefFilter1           UxModelConnessionWindowContext->UxSetDefFilter1
#define separator13             UxModelConnessionWindowContext->Uxseparator13
#define DefineFilter1           UxModelConnessionWindowContext->UxDefineFilter1
#define menuBar_top_b1          UxModelConnessionWindowContext->UxmenuBar_top_b1
#define mainPW1                 UxModelConnessionWindowContext->UxmainPW1
#define mainForm1               UxModelConnessionWindowContext->UxmainForm1
#define blListLb1               UxModelConnessionWindowContext->UxblListLb1
#define findBlock               UxModelConnessionWindowContext->UxfindBlock
#define blListSW1               UxModelConnessionWindowContext->UxblListSW1
#define blListSL1               UxModelConnessionWindowContext->UxblListSL1
#define descrMod                UxModelConnessionWindowContext->UxdescrMod
#define WindVariable            UxModelConnessionWindowContext->UxWindVariable
#define ltask                   UxModelConnessionWindowContext->Uxltask
#define parametri_ulist         UxModelConnessionWindowContext->Uxparametri_ulist
#define argb                    UxModelConnessionWindowContext->Uxargb
#define nargb                   UxModelConnessionWindowContext->Uxnargb
#define model                   UxModelConnessionWindowContext->Uxmodel

static _UxCModelConnessionWindow	*UxModelConnessionWindowContext;

Widget	modNameLb1;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_ModelConnessionWindow();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

void set_titolo(MODELLO *task)
{
   XmString xstring;

   xstring=XmStringCreateSimple(task->nome);
   set_something_val(modNameLb1,XmNlabelString, (XtArgVal) xstring);

   xstring=XmStringCreateSimple(task->descr);
   set_something_val(descrMod,XmNlabelString, (XtArgVal) xstring);
}

void set_lista_blocchi(MODELLO *task)
{
   int i;
   char cstring[STRLONG];
   XmString xstring;

   for(i=0;i<task->num_blocchi;i++)
   {
      sprintf(cstring,"%s%s  %s",task->blocchi[i].sigla_modulo,task->blocchi[i].sigla_blocco,task->blocchi[i].descr_blocco);
      xstring =  XmStringCreateSimple(cstring);
      XmListAddItem(blListSL1,xstring,0);
   }
}

void set_connwind_closure(Widget wind)
{
   WindVariable = wind;
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_Close( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelConnessionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelConnessionWindowContext;
	UxModelConnessionWindowContext = UxContext =
			(_UxCModelConnessionWindow *) UxGetContext( UxWidget );
	{
	extern int fin_conn_open;
	extern int distruggi_interfaccie();
	
	fin_conn_open--;
	distruggi_interfaccie(WindVariable );
	
	/*
	UxDestroyInterface( WindVariable );
	UxDestroyInterface( ModelConnessionWindow );
	*/
	
	}
	UxModelConnessionWindowContext = UxSaveCtx;
}

static void	singleSelectionCB_blListSL1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelConnessionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelConnessionWindowContext;
	UxModelConnessionWindowContext = UxContext =
			(_UxCModelConnessionWindow *) UxGetContext( UxWidget );
	{
	/*
	   devo recuperare il blocco selezionato dalla lista 
	   e ricostruire la varWin con le variabili del nuovo blocco 
	   posso fare in due modi 
	
		1) ditruggere la varwin e ricostruirla con il nuovo blocco
	
	 	2) distruggere solo il contenuto della varwin e 
		   riempirla con i dati del nuovo blocco
	*/
	
	/* provo la soluzione 2) */
	
	extern int change_active_block();
	
	int *pos_list,count;
	
	XmListGetSelectedPos(blListSL1,&pos_list,&count);
	
	if( count == 1)
	   change_active_block(ltask,pos_list[0]-1,WindVariable);
	}
	UxModelConnessionWindowContext = UxSaveCtx;
}

static void	extendedSelectionCB_blListSL1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelConnessionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelConnessionWindowContext;
	UxModelConnessionWindowContext = UxContext =
			(_UxCModelConnessionWindow *) UxGetContext( UxWidget );
	{
	
	}
	UxModelConnessionWindowContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ModelConnessionWindow()
{
	Widget	ModelConnessionWindow_shell;
	Widget	filePane1_shell;
	Widget	OptionPane1_shell;
	Widget	DefaultFilterPane1_shell;

	ModelConnessionWindow_shell = XtVaCreatePopupShell( "ModelConnessionWindow_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 800,
			XmNy, 5,
			XmNwidth, 560,
			XmNheight, 400,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "ModelConnessionWindow",
			XmNiconName, "ModelConnessionWindow",
			NULL );

	ModelConnessionWindow = XtVaCreateManagedWidget( "ModelConnessionWindow",
			xmMainWindowWidgetClass, ModelConnessionWindow_shell,
			RES_CONVERT( XmNbackground, "GhostWhite" ),
			XmNheight, 400,
			XmNwidth, 560,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( ModelConnessionWindow, (char *) UxModelConnessionWindowContext );

	menuBar1 = XtVaCreateManagedWidget( "menuBar1",
			xmRowColumnWidgetClass, ModelConnessionWindow,
			XmNspacing, 0,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menuBar1, (char *) UxModelConnessionWindowContext );

	filePane1_shell = XtVaCreatePopupShell ("filePane1_shell",
			xmMenuShellWidgetClass, menuBar1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	filePane1 = XtVaCreateWidget( "filePane1",
			xmRowColumnWidgetClass, filePane1_shell,
			XmNspacing, 0,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( filePane1, (char *) UxModelConnessionWindowContext );

	Close = XtVaCreateManagedWidget( "Close",
			xmPushButtonWidgetClass, filePane1,
			RES_CONVERT( XmNlabelString, "Close" ),
			NULL );

	UxPutContext( Close, (char *) UxModelConnessionWindowContext );

	fileCascade1 = XtVaCreateManagedWidget( "fileCascade1",
			xmCascadeButtonWidgetClass, menuBar1,
			XmNsubMenuId, filePane1,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "Model" ),
			NULL );

	UxPutContext( fileCascade1, (char *) UxModelConnessionWindowContext );

	OptionPane1_shell = XtVaCreatePopupShell ("OptionPane1_shell",
			xmMenuShellWidgetClass, menuBar1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	OptionPane1 = XtVaCreateWidget( "OptionPane1",
			xmRowColumnWidgetClass, OptionPane1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( OptionPane1, (char *) UxModelConnessionWindowContext );

	DefaultFilterPane1_shell = XtVaCreatePopupShell ("DefaultFilterPane1_shell",
			xmMenuShellWidgetClass, OptionPane1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	DefaultFilterPane1 = XtVaCreateWidget( "DefaultFilterPane1",
			xmRowColumnWidgetClass, DefaultFilterPane1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( DefaultFilterPane1, (char *) UxModelConnessionWindowContext );

	NoFilterDef1 = XtVaCreateManagedWidget( "NoFilterDef1",
			xmPushButtonGadgetClass, DefaultFilterPane1,
			RES_CONVERT( XmNlabelString, "No Filter" ),
			NULL );

	UxPutContext( NoFilterDef1, (char *) UxModelConnessionWindowContext );

	SetDefFilter1 = XtVaCreateManagedWidget( "SetDefFilter1",
			xmCascadeButtonGadgetClass, OptionPane1,
			XmNsubMenuId, DefaultFilterPane1,
			RES_CONVERT( XmNlabelString, "Set Default Filter" ),
			NULL );

	UxPutContext( SetDefFilter1, (char *) UxModelConnessionWindowContext );

	separator13 = XtVaCreateManagedWidget( "separator13",
			xmSeparatorGadgetClass, OptionPane1,
			NULL );

	UxPutContext( separator13, (char *) UxModelConnessionWindowContext );

	DefineFilter1 = XtVaCreateManagedWidget( "DefineFilter1",
			xmPushButtonGadgetClass, OptionPane1,
			RES_CONVERT( XmNlabelString, "Define Filter" ),
			NULL );

	UxPutContext( DefineFilter1, (char *) UxModelConnessionWindowContext );

	menuBar_top_b1 = XtVaCreateManagedWidget( "menuBar_top_b1",
			xmCascadeButtonWidgetClass, menuBar1,
			XmNsubMenuId, OptionPane1,
			RES_CONVERT( XmNmnemonic, "O" ),
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( menuBar_top_b1, (char *) UxModelConnessionWindowContext );

	mainPW1 = XtVaCreateManagedWidget( "mainPW1",
			xmPanedWindowWidgetClass, ModelConnessionWindow,
			RES_CONVERT( XmNbackground, "#7E88AB" ),
			XmNspacing, 20,
			XmNsashIndent, -10,
			NULL );

	UxPutContext( mainPW1, (char *) UxModelConnessionWindowContext );

	mainForm1 = XtVaCreateManagedWidget( "mainForm1",
			xmFormWidgetClass, mainPW1,
			XmNpaneMinimum, 250,
			XmNallowResize, FALSE,
			XmNpaneMaximum, 1000,
			XmNheight, 450,
			XmNwidth, 530,
			XmNy, 10,
			XmNx, 15,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( mainForm1, (char *) UxModelConnessionWindowContext );

	modNameLb1 = XtVaCreateManagedWidget( "modNameLb1",
			xmLabelWidgetClass, mainForm1,
			XmNtraversalOn, FALSE,
			XmNshadowThickness, 0,
			RES_CONVERT( XmNhighlightColor, "#cccc22224444" ),
			XmNhighlightOnEnter, FALSE,
			XmNhighlightThickness, 0,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "No model selected" ),
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNwidth, 330,
			XmNy, 55,
			XmNx, 105,
			NULL );

	UxPutContext( modNameLb1, (char *) UxModelConnessionWindowContext );

	blListLb1 = XtVaCreateManagedWidget( "blListLb1",
			xmLabelWidgetClass, mainForm1,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Blocks in model" ),
			XmNalignment, XmALIGNMENT_CENTER,
			XmNtopWidget, modNameLb1,
			XmNtopOffset, 25,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 15,
			XmNwidth, 110,
			XmNy, 155,
			XmNx, 25,
			NULL );

	UxPutContext( blListLb1, (char *) UxModelConnessionWindowContext );

	findBlock = XtVaCreateManagedWidget( "findBlock",
			xmRowColumnWidgetClass, mainForm1,
			XmNresizable, TRUE,
			XmNpacking, XmPACK_TIGHT,
			XmNnumColumns, 2,
			XmNleftOffset, 30,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNorientation, XmHORIZONTAL,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 20,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 32,
			XmNwidth, 422,
			XmNy, 390,
			XmNx, 30,
			NULL );

	UxPutContext( findBlock, (char *) UxModelConnessionWindowContext );

	blListSW1 = XtVaCreateManagedWidget( "blListSW1",
			xmScrolledWindowWidgetClass, mainForm1,
			XmNheight, 310,
			XmNtopWidget, blListLb1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 30,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, findBlock,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 100,
			XmNx, 30,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( blListSW1, (char *) UxModelConnessionWindowContext );

	blListSL1 = XtVaCreateManagedWidget( "blListSL1",
			xmListWidgetClass, blListSW1,
			XmNvisibleItemCount, 50,
			XmNselectedItemCount, 0,
			XmNlistMarginWidth, 5,
			XmNlistMarginHeight, 5,
			XmNshadowThickness, 2,
			XmNhighlightThickness, 0,
			XmNlistSizePolicy, XmCONSTANT,
			XmNscrollBarDisplayPolicy, XmAS_NEEDED,
			XmNlistSpacing, 0,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNheight, 279,
			XmNwidth, 494,
			NULL );

	UxPutContext( blListSL1, (char *) UxModelConnessionWindowContext );

	descrMod = XtVaCreateManagedWidget( "descrMod",
			xmLabelWidgetClass, mainForm1,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, blListLb1,
			XmNtopWidget, modNameLb1,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNheight, 36,
			XmNwidth, 515,
			XmNy, 33,
			XmNx, 7,
			NULL );

	UxPutContext( descrMod, (char *) UxModelConnessionWindowContext );

	XtAddCallback( ModelConnessionWindow, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxModelConnessionWindowContext );

	XtAddCallback( Close, XmNactivateCallback,
			activateCB_Close,
			(XtPointer) UxModelConnessionWindowContext );

	XtAddCallback( blListSL1, XmNsingleSelectionCallback,
			singleSelectionCB_blListSL1,
			(XtPointer) UxModelConnessionWindowContext );
	XtAddCallback( blListSL1, XmNextendedSelectionCallback,
			extendedSelectionCB_blListSL1,
			(XtPointer) UxModelConnessionWindowContext );


	XmMainWindowSetAreas( ModelConnessionWindow, menuBar1, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, mainPW1 );

	return ( ModelConnessionWindow );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_ModelConnessionWindow( _Uxmodel )
	MODELLO	*_Uxmodel;
{
	Widget                  rtrn;
	_UxCModelConnessionWindow *UxContext;

	UxModelConnessionWindowContext = UxContext =
		(_UxCModelConnessionWindow *) XtMalloc( sizeof(_UxCModelConnessionWindow) );

	model = _Uxmodel;
	{
		extern int fin_conn_open;
		char *salva_context;
		Widget block_find_form;
		ltask = model;
		rtrn = _Uxbuild_ModelConnessionWindow();

		{
		   set_titolo(ltask);
		
		   set_lista_blocchi(ltask);
		
		
		/* Inserimento find di libreria */
		   parametri_ulist.ID_lista = UxGetWidget(blListSL1);
		   parametri_ulist.num_elem = 100;
		   parametri_ulist.vis_elem = 1;
		   parametri_ulist.pos      = 0;
		
		   nargb=0;
		   XtSetArg(argb[nargb],XmNleftAttachment,XmATTACH_FORM);
		   nargb++;
		   XtSetArg(argb[nargb],XmNleftOffset,0);
		   nargb++;
		   XtSetArg(argb[nargb],XmNrightAttachment,XmATTACH_FORM);
		   nargb++;
		   XtSetArg(argb[nargb],XmNrightOffset,0);
		   nargb++;
		   XtSetArg(argb[nargb],XmNbottomAttachment,XmATTACH_FORM);
		   nargb++;
		   XtSetArg(argb[nargb],XmNbottomOffset,0);
		   nargb++;
		   XtSetArg(argb[nargb],XmNtopAttachment,XmATTACH_FORM);
		   nargb++;
		   XtSetArg(argb[nargb],XmNtopOffset,0);
		   nargb++;
		
		
		   block_find_form = find_kit(UxGetWidget(findBlock),argb,nargb,
		                                                 &parametri_ulist);
		
		   fin_conn_open++;
		
		
		   return(rtrn);
		
		}
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_ModelConnessionWindow( _Uxmodel )
	MODELLO	*_Uxmodel;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_ModelConnessionWindow( _Uxmodel );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

