
/*******************************************************************************
       previewSnap.c
       (Generated from interface file previewSnap.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xm/Text.h>
#include <Xm/Label.h>
#include <malloc.h>

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include "parametri.h" 
#include "bistrutt.h"
#include "preview.h"
#include "file_selection.h"
#include "filtri.h"
#include "messaggi.h"
#ifndef MAX_VIEW_ENTRY
#define MAX_VIEW_ENTRY 10
#endif
extern Widget create_selVar ();
//#include "res_edit.h"
//int UxNewClassId();

static	int _UxIfClassId;
int	UxpreviewSnap_readFile_Id = -1;
char*	UxpreviewSnap_readFile_Name = "readFile";
int	UxpreviewSnap_displayVal_Id = -1;
char*	UxpreviewSnap_displayVal_Name = "displayVal";
int	UxpreviewSnap_crea_entry_Id = -1;
char*	UxpreviewSnap_crea_entry_Name = "crea_entry";
int	UxpreviewSnap_readVal_Id = -1;
char*	UxpreviewSnap_readVal_Name = "readVal";

#ifndef previewSnap_readFile
#define previewSnap_readFile( UxThis, pEnv ) \
	((int(*)())UxMethodLookup(UxThis, UxpreviewSnap_readFile_Id,\
			UxpreviewSnap_readFile_Name)) \
		( UxThis, pEnv )
#endif

#ifndef previewSnap_displayVal
#define previewSnap_displayVal( UxThis, pEnv ) \
	((int(*)())UxMethodLookup(UxThis, UxpreviewSnap_displayVal_Id,\
			UxpreviewSnap_displayVal_Name)) \
		( UxThis, pEnv )
#endif

#ifndef previewSnap_crea_entry
#define previewSnap_crea_entry( UxThis, pEnv ) \
	((int(*)())UxMethodLookup(UxThis, UxpreviewSnap_crea_entry_Id,\
			UxpreviewSnap_crea_entry_Name)) \
		( UxThis, pEnv )
#endif

#ifndef previewSnap_readVal
#define previewSnap_readVal( UxThis, pEnv ) \
	((int(*)())UxMethodLookup(UxThis, UxpreviewSnap_readVal_Id,\
			UxpreviewSnap_readVal_Name)) \
		( UxThis, pEnv )
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
	Widget	UxpreviewSnap;
	Widget	Uxframe13;
	Widget	Uxform23;
	Widget	UxpreviewQuitpb;
	Widget	UxpreviewMenu;
	Widget	UxpreviewMenuFile;
	Widget	UxpreviewMenuFileOpenDefpb;
	Widget	UxpreviewMenuFileOpenpb;
	Widget	UxpreviewMenuFile_b7;
	Widget	UxpreviewMenuFileSavepb;
	Widget	UxpreviewMenuFileSaveaspb;
	Widget	UxpreviewMenuFileSep1;
	Widget	UxpreviewMenuFileQuitpb;
	Widget	UxpreviewMenuFilecb;
	Widget	UxpreviewMenuEdit;
	Widget	UxpreviewMenuEditSelpb;
	Widget	UxpreviewMenuEditSep1;
	Widget	UxpreviewMenuEditDelete;
	Widget	UxpreviewMenuEditClearAllpb;
	Widget	UxpreviewMenuEditcb;
	Widget	UxpreviewMenuHelp;
	Widget	UxpreviewMenuHelpCommpb;
	Widget	UxpreviewMenuHelpcb;
	Widget	UxscrolledWindow4;
	Widget	UxviewRc;
	Widget	UxpreviewSnapPopup;
	Widget	UxpreviewSnapPopupOpenDef;
	Widget	UxpreviewSnapPopupMenuOpen;
	Widget	UxpreviewSnapPopupMenu_b3;
	Widget	UxpreviewSnapPopupMenuSaveDef;
	Widget	UxpreviewSnapPopupMenuSave;
	Widget	UxpreviewSnapPopupMenu_b6;
	Widget	UxpreviewSnapPopupMenuQuit;
	Widget	UxViewScrollForm[10];
	Widget	UxViewScrollTb[10];
	Widget	UxViewScrollText[10];
	Widget	UxViewScrollLabel[10];
	Widget	UxselVarWidg;
	PREVIEW_DATA	Uxpreview_data;
	LISTA_VAR	Uxlista_var;
	unsigned char	*Uxtestata;
	int	Uxwhich;
	int	Uxn;
	unsigned char	*Uxs;
} _UxCpreviewSnap;

static _UxCpreviewSnap         *UxPreviewSnapContext;
#define previewSnap             UxPreviewSnapContext->UxpreviewSnap
#define frame13                 UxPreviewSnapContext->Uxframe13
#define form23                  UxPreviewSnapContext->Uxform23
#define previewQuitpb           UxPreviewSnapContext->UxpreviewQuitpb
#define previewMenu             UxPreviewSnapContext->UxpreviewMenu
#define previewMenuFile         UxPreviewSnapContext->UxpreviewMenuFile
#define previewMenuFileOpenDefpb UxPreviewSnapContext->UxpreviewMenuFileOpenDefpb
#define previewMenuFileOpenpb   UxPreviewSnapContext->UxpreviewMenuFileOpenpb
#define previewMenuFile_b7      UxPreviewSnapContext->UxpreviewMenuFile_b7
#define previewMenuFileSavepb   UxPreviewSnapContext->UxpreviewMenuFileSavepb
#define previewMenuFileSaveaspb UxPreviewSnapContext->UxpreviewMenuFileSaveaspb
#define previewMenuFileSep1     UxPreviewSnapContext->UxpreviewMenuFileSep1
#define previewMenuFileQuitpb   UxPreviewSnapContext->UxpreviewMenuFileQuitpb
#define previewMenuFilecb       UxPreviewSnapContext->UxpreviewMenuFilecb
#define previewMenuEdit         UxPreviewSnapContext->UxpreviewMenuEdit
#define previewMenuEditSelpb    UxPreviewSnapContext->UxpreviewMenuEditSelpb
#define previewMenuEditSep1     UxPreviewSnapContext->UxpreviewMenuEditSep1
#define previewMenuEditDelete   UxPreviewSnapContext->UxpreviewMenuEditDelete
#define previewMenuEditClearAllpb UxPreviewSnapContext->UxpreviewMenuEditClearAllpb
#define previewMenuEditcb       UxPreviewSnapContext->UxpreviewMenuEditcb
#define previewMenuHelp         UxPreviewSnapContext->UxpreviewMenuHelp
#define previewMenuHelpCommpb   UxPreviewSnapContext->UxpreviewMenuHelpCommpb
#define previewMenuHelpcb       UxPreviewSnapContext->UxpreviewMenuHelpcb
#define scrolledWindow4         UxPreviewSnapContext->UxscrolledWindow4
#define viewRc                  UxPreviewSnapContext->UxviewRc
#define previewSnapPopup        UxPreviewSnapContext->UxpreviewSnapPopup
#define previewSnapPopupOpenDef UxPreviewSnapContext->UxpreviewSnapPopupOpenDef
#define previewSnapPopupMenuOpen UxPreviewSnapContext->UxpreviewSnapPopupMenuOpen
#define previewSnapPopupMenu_b3 UxPreviewSnapContext->UxpreviewSnapPopupMenu_b3
#define previewSnapPopupMenuSaveDef UxPreviewSnapContext->UxpreviewSnapPopupMenuSaveDef
#define previewSnapPopupMenuSave UxPreviewSnapContext->UxpreviewSnapPopupMenuSave
#define previewSnapPopupMenu_b6 UxPreviewSnapContext->UxpreviewSnapPopupMenu_b6
#define previewSnapPopupMenuQuit UxPreviewSnapContext->UxpreviewSnapPopupMenuQuit
#define ViewScrollForm          UxPreviewSnapContext->UxViewScrollForm
#define ViewScrollTb            UxPreviewSnapContext->UxViewScrollTb
#define ViewScrollText          UxPreviewSnapContext->UxViewScrollText
#define ViewScrollLabel         UxPreviewSnapContext->UxViewScrollLabel
#define selVarWidg              UxPreviewSnapContext->UxselVarWidg
#define preview_data            UxPreviewSnapContext->Uxpreview_data
#define lista_var               UxPreviewSnapContext->Uxlista_var
#define testata                 UxPreviewSnapContext->Uxtestata
#define which                   UxPreviewSnapContext->Uxwhich
#define n                       UxPreviewSnapContext->Uxn
#define s                       UxPreviewSnapContext->Uxs



#include "option.h"

#include "tabelle_malf.h"
#include "banco_globals.h"

int get_file_view(Widget);
int display_view (Widget);
int load_preview (Widget,char*);
int salva_preview_list(Widget,char*);
int loadas_preview (Widget, int, int);
int saveas_preview(Widget, int, int);
int delete_preview_entry(PREVIEW_DATA *,Widget);
int clear_preview_entry(PREVIEW_DATA *,int);

/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxpreviewSnapMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
    int ctd;
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

Widget	create_previewSnap();
int crea_preview_entry (Widget);
int get_val_view (Widget);



/*******************************************************************************
Declarations of methods
*******************************************************************************/

static int	_previewSnap_readFile();
static int	_previewSnap_displayVal();
static int	_previewSnap_crea_entry();
static int	_previewSnap_readVal();

/*******************************************************************************
       The following are method functions.
*******************************************************************************/

static int	Ux_readFile( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	get_file_view (UxGetWidget(UxThis));
}

static int	_previewSnap_readFile( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	int			_Uxrtrn;
	_UxCpreviewSnap         *UxSaveCtx = UxPreviewSnapContext;

	UxPreviewSnapContext = (_UxCpreviewSnap *) UxGetContext( UxThis );
	if (pEnv)
		pEnv->_major = NO_EXCEPTION;
	_Uxrtrn = Ux_readFile( UxThis, pEnv );
	UxPreviewSnapContext = UxSaveCtx;

	return ( _Uxrtrn );
}

static int	Ux_displayVal( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	display_view (UxGetWidget(UxThis));
}

static int	_previewSnap_displayVal( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	int			_Uxrtrn;
	_UxCpreviewSnap         *UxSaveCtx = UxPreviewSnapContext;

	UxPreviewSnapContext = (_UxCpreviewSnap *) UxGetContext( UxThis );
	if (pEnv)
		pEnv->_major = NO_EXCEPTION;
	_Uxrtrn = Ux_displayVal( UxThis, pEnv );
	UxPreviewSnapContext = UxSaveCtx;

	return ( _Uxrtrn );
}

static int	Ux_crea_entry( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	crea_preview_entry (UxGetWidget(UxThis));
}

static int	_previewSnap_crea_entry( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	int			_Uxrtrn;
	_UxCpreviewSnap         *UxSaveCtx = UxPreviewSnapContext;

	UxPreviewSnapContext = (_UxCpreviewSnap *) UxGetContext( UxThis );
	if (pEnv)
		pEnv->_major = NO_EXCEPTION;
	_Uxrtrn = Ux_crea_entry( UxThis, pEnv );
	UxPreviewSnapContext = UxSaveCtx;

	return ( _Uxrtrn );
}

static int	Ux_readVal( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	get_val_view (UxGetWidget(UxThis));
}

static int	_previewSnap_readVal( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	int			_Uxrtrn;
	_UxCpreviewSnap         *UxSaveCtx = UxPreviewSnapContext;

	UxPreviewSnapContext = (_UxCpreviewSnap *) UxGetContext( UxThis );
	if (pEnv)
		pEnv->_major = NO_EXCEPTION;
	_Uxrtrn = Ux_readVal( UxThis, pEnv );
	UxPreviewSnapContext = UxSaveCtx;

	return ( _Uxrtrn );
}


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_previewQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuFileOpenDefpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	load_preview (UxWidget,VIEW_FILE);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuFileOpenpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	loadas_preview (previewSnap,1,PREVIEW_FILE); /* 1 = lettura */
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuFileSavepb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	salva_preview_list (UxWidget, VIEW_FILE);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuFileSaveaspb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	saveas_preview (previewSnap,2,PREVIEW_FILE); /* 2 = in scrittura */
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuFileQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent(previewSnap));
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuEditSelpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
	if (!exist_Widget (selVarWidg))
	   selVarWidg = create_selVar (UxGetWidget(previewSnap), &preview_data,PREVIEW_CALLING,testata);
	printf (" chiamata da preview [%s]\n",testata);
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuEditDelete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	delete_preview_entry (&preview_data,UxWidget);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuEditClearAllpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   int i;
	      for (i=0; i<MAX_VIEW_ENTRY; i++)
	         clear_preview_entry (&preview_data, i);
	      display_view (UxWidget);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewMenuHelpCommpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{printf ("help\n");}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewSnapPopupOpenDef( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	load_preview (UxWidget,VIEW_FILE);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewSnapPopupMenuOpen( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	loadas_preview (previewSnap,1,PREVIEW_FILE); /* 1 = lettura */
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewSnapPopupMenuSaveDef( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	salva_preview_list (UxWidget, VIEW_FILE);
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewSnapPopupMenuSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	saveas_preview (previewSnap,2,PREVIEW_FILE); /* 2 = in scrittura */
#endif
	}
	UxPreviewSnapContext = UxSaveCtx;
}

static	void	activateCB_previewSnapPopupMenuQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCpreviewSnap         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPreviewSnapContext;
	UxPreviewSnapContext = UxContext =
			(_UxCpreviewSnap *) UxGetContext( UxWidget );
	{
	DistruggiInterfaccia (XtParent(previewSnap));
	}
	UxPreviewSnapContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_previewSnap()
{
	Widget		_UxParent;
	Widget		previewMenuFile_shell;
	Widget		previewMenuEdit_shell;
	Widget		previewMenuHelp_shell;
	Widget		previewSnapPopup_shell;


	/* Creation of previewSnap */
	_UxParent = XtVaCreatePopupShell( "previewSnap_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 725,
			XmNy, 165,
			XmNwidth, 650,
			XmNheight, 400,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "previewSnap",
			NULL );

	previewSnap = XtVaCreateWidget( "previewSnap",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 650,
			XmNheight, 400,
			NULL );
	UxPutContext( previewSnap, (char *) UxPreviewSnapContext );


	/* Creation of frame13 */
	frame13 = XtVaCreateManagedWidget( "frame13",
			xmFrameWidgetClass,
			previewSnap,
			XmNx, 10,
			XmNy, 405,
			XmNwidth, 600,
			XmNheight, 80,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame13, (char *) UxPreviewSnapContext );


	/* Creation of form23 */
	form23 = XtVaCreateManagedWidget( "form23",
			xmFormWidgetClass,
			frame13,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, -20,
			XmNwidth, 600,
			XmNheight, 68,
			NULL );
	UxPutContext( form23, (char *) UxPreviewSnapContext );


	/* Creation of previewQuitpb */
	previewQuitpb = XtVaCreateManagedWidget( "previewQuitpb",
			xmPushButtonWidgetClass,
			form23,
			XmNx, 493,
			XmNy, 35,
			XmNwidth, 100,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, QUITLABEL ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 15,
			XmNrightOffset, 0,
			XmNleftPosition, 40,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			XmNrightPosition, 60,
			NULL );
	XtAddCallback( previewQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewQuitpb,
		UxGetWidget(previewSnap) );

	UxPutContext( previewQuitpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenu */
	previewMenu = XtVaCreateManagedWidget( "previewMenu",
			xmRowColumnWidgetClass,
			previewSnap,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( previewMenu, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFile */
	previewMenuFile_shell = XtVaCreatePopupShell ("previewMenuFile_shell",
			xmMenuShellWidgetClass, previewMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	previewMenuFile = XtVaCreateWidget( "previewMenuFile",
			xmRowColumnWidgetClass,
			previewMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( previewMenuFile, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileOpenDefpb */
	previewMenuFileOpenDefpb = XtVaCreateManagedWidget( "previewMenuFileOpenDefpb",
			xmPushButtonWidgetClass,
			previewMenuFile,
			RES_CONVERT( XmNlabelString, LoadDefaultLabel ),
			NULL );
	XtAddCallback( previewMenuFileOpenDefpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuFileOpenDefpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuFileOpenDefpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileOpenpb */
	previewMenuFileOpenpb = XtVaCreateManagedWidget( "previewMenuFileOpenpb",
			xmPushButtonWidgetClass,
			previewMenuFile,
			RES_CONVERT( XmNlabelString, OpenMenuLabel2 ),
			NULL );
	XtAddCallback( previewMenuFileOpenpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuFileOpenpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuFileOpenpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFile_b7 */
	previewMenuFile_b7 = XtVaCreateManagedWidget( "previewMenuFile_b7",
			xmSeparatorWidgetClass,
			previewMenuFile,
			NULL );
	UxPutContext( previewMenuFile_b7, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileSavepb */
	previewMenuFileSavepb = XtVaCreateManagedWidget( "previewMenuFileSavepb",
			xmPushButtonWidgetClass,
			previewMenuFile,
			RES_CONVERT( XmNlabelString, SaveMenuLabel ),
			NULL );
	XtAddCallback( previewMenuFileSavepb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuFileSavepb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuFileSavepb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileSaveaspb */
	previewMenuFileSaveaspb = XtVaCreateManagedWidget( "previewMenuFileSaveaspb",
			xmPushButtonWidgetClass,
			previewMenuFile,
			RES_CONVERT( XmNlabelString, SaveMenuLabel2 ),
			NULL );
	XtAddCallback( previewMenuFileSaveaspb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuFileSaveaspb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuFileSaveaspb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileSep1 */
	previewMenuFileSep1 = XtVaCreateManagedWidget( "previewMenuFileSep1",
			xmSeparatorGadgetClass,
			previewMenuFile,
			NULL );
	UxPutContext( previewMenuFileSep1, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFileQuitpb */
	previewMenuFileQuitpb = XtVaCreateManagedWidget( "previewMenuFileQuitpb",
			xmPushButtonWidgetClass,
			previewMenuFile,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( previewMenuFileQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuFileQuitpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuFileQuitpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuFilecb */
	previewMenuFilecb = XtVaCreateManagedWidget( "previewMenuFilecb",
			xmCascadeButtonWidgetClass,
			previewMenu,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, previewMenuFile,
			NULL );
	UxPutContext( previewMenuFilecb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEdit */
	previewMenuEdit_shell = XtVaCreatePopupShell ("previewMenuEdit_shell",
			xmMenuShellWidgetClass, previewMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	previewMenuEdit = XtVaCreateWidget( "previewMenuEdit",
			xmRowColumnWidgetClass,
			previewMenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( previewMenuEdit, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEditSelpb */
	previewMenuEditSelpb = XtVaCreateManagedWidget( "previewMenuEditSelpb",
			xmPushButtonWidgetClass,
			previewMenuEdit,
			RES_CONVERT( XmNlabelString, SelectNewLabel ),
			NULL );
	XtAddCallback( previewMenuEditSelpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuEditSelpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuEditSelpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEditSep1 */
	previewMenuEditSep1 = XtVaCreateManagedWidget( "previewMenuEditSep1",
			xmSeparatorWidgetClass,
			previewMenuEdit,
			NULL );
	UxPutContext( previewMenuEditSep1, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEditDelete */
	previewMenuEditDelete = XtVaCreateManagedWidget( "previewMenuEditDelete",
			xmPushButtonWidgetClass,
			previewMenuEdit,
			RES_CONVERT( XmNlabelString, CutMenuLabel ),
			NULL );
	XtAddCallback( previewMenuEditDelete, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuEditDelete,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuEditDelete, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEditClearAllpb */
	previewMenuEditClearAllpb = XtVaCreateManagedWidget( "previewMenuEditClearAllpb",
			xmPushButtonWidgetClass,
			previewMenuEdit,
			RES_CONVERT( XmNlabelString, ClearAllLabel ),
			NULL );
	XtAddCallback( previewMenuEditClearAllpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuEditClearAllpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuEditClearAllpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuEditcb */
	previewMenuEditcb = XtVaCreateManagedWidget( "previewMenuEditcb",
			xmCascadeButtonWidgetClass,
			previewMenu,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, previewMenuEdit,
			NULL );
	UxPutContext( previewMenuEditcb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuHelp */
	previewMenuHelp_shell = XtVaCreatePopupShell ("previewMenuHelp_shell",
			xmMenuShellWidgetClass, previewMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	previewMenuHelp = XtVaCreateWidget( "previewMenuHelp",
			xmRowColumnWidgetClass,
			previewMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( previewMenuHelp, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuHelpCommpb */
	previewMenuHelpCommpb = XtVaCreateManagedWidget( "previewMenuHelpCommpb",
			xmPushButtonWidgetClass,
			previewMenuHelp,
			RES_CONVERT( XmNlabelString, "Command" ),
			NULL );
	XtAddCallback( previewMenuHelpCommpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewMenuHelpCommpb,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewMenuHelpCommpb, (char *) UxPreviewSnapContext );


	/* Creation of previewMenuHelpcb */
	previewMenuHelpcb = XtVaCreateManagedWidget( "previewMenuHelpcb",
			xmCascadeButtonWidgetClass,
			previewMenu,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, previewMenuHelp,
			NULL );
	UxPutContext( previewMenuHelpcb, (char *) UxPreviewSnapContext );


	/* Creation of scrolledWindow4 */
	scrolledWindow4 = XtVaCreateManagedWidget( "scrolledWindow4",
			xmScrolledWindowWidgetClass,
			previewSnap,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 35,
			XmNy, 55,
			XmNwidth, 270,
			XmNheight, 325,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 20,
			XmNtopWidget, previewMenu,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 20,
			XmNbottomWidget, frame13,
			XmNleftOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			NULL );
	UxPutContext( scrolledWindow4, (char *) UxPreviewSnapContext );


	/* Creation of viewRc */
	viewRc = XtVaCreateManagedWidget( "viewRc",
			xmRowColumnWidgetClass,
			scrolledWindow4,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 345,
			XmNheight, 10,
			NULL );
	UxPutContext( viewRc, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopup */
	previewSnapPopup_shell = XtVaCreatePopupShell ("previewSnapPopup_shell",
			xmMenuShellWidgetClass, previewSnap,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	previewSnapPopup = XtVaCreateWidget( "previewSnapPopup",
			xmRowColumnWidgetClass,
			previewSnapPopup_shell,
			XmNrowColumnType, XmMENU_POPUP,
			NULL );
	UxPutContext( previewSnapPopup, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupOpenDef */
	previewSnapPopupOpenDef = XtVaCreateManagedWidget( "previewSnapPopupOpenDef",
			xmPushButtonWidgetClass,
			previewSnapPopup,
			RES_CONVERT( XmNlabelString, LoadDefaultLabel ),
			NULL );
	XtAddCallback( previewSnapPopupOpenDef, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewSnapPopupOpenDef,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewSnapPopupOpenDef, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenuOpen */
	previewSnapPopupMenuOpen = XtVaCreateManagedWidget( "previewSnapPopupMenuOpen",
			xmPushButtonWidgetClass,
			previewSnapPopup,
			RES_CONVERT( XmNlabelString, OpenMenuLabel2 ),
			NULL );
	XtAddCallback( previewSnapPopupMenuOpen, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewSnapPopupMenuOpen,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewSnapPopupMenuOpen, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenu_b3 */
	previewSnapPopupMenu_b3 = XtVaCreateManagedWidget( "previewSnapPopupMenu_b3",
			xmSeparatorWidgetClass,
			previewSnapPopup,
			NULL );
	UxPutContext( previewSnapPopupMenu_b3, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenuSaveDef */
	previewSnapPopupMenuSaveDef = XtVaCreateManagedWidget( "previewSnapPopupMenuSaveDef",
			xmPushButtonWidgetClass,
			previewSnapPopup,
			RES_CONVERT( XmNlabelString, SaveMenuLabel ),
			NULL );
	XtAddCallback( previewSnapPopupMenuSaveDef, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewSnapPopupMenuSaveDef,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewSnapPopupMenuSaveDef, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenuSave */
	previewSnapPopupMenuSave = XtVaCreateManagedWidget( "previewSnapPopupMenuSave",
			xmPushButtonWidgetClass,
			previewSnapPopup,
			RES_CONVERT( XmNlabelString, SaveMenuLabel2 ),
			NULL );
	XtAddCallback( previewSnapPopupMenuSave, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewSnapPopupMenuSave,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewSnapPopupMenuSave, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenu_b6 */
	previewSnapPopupMenu_b6 = XtVaCreateManagedWidget( "previewSnapPopupMenu_b6",
			xmSeparatorWidgetClass,
			previewSnapPopup,
			NULL );
	UxPutContext( previewSnapPopupMenu_b6, (char *) UxPreviewSnapContext );


	/* Creation of previewSnapPopupMenuQuit */
	previewSnapPopupMenuQuit = XtVaCreateManagedWidget( "previewSnapPopupMenuQuit",
			xmPushButtonWidgetClass,
			previewSnapPopup,
			RES_CONVERT( XmNlabelString, QuitMenuLabel ),
			NULL );
	XtAddCallback( previewSnapPopupMenuQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_previewSnapPopupMenuQuit,
		(XtPointer) UxPreviewSnapContext );

	UxPutContext( previewSnapPopupMenuQuit, (char *) UxPreviewSnapContext );

	XtVaSetValues(form23,
			XmNdefaultButton, previewQuitpb,
			NULL );

	XtVaSetValues(previewMenu,
			XmNmenuHelpWidget, previewMenuHelpcb,
			NULL );


	XtAddCallback( previewSnap, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxPreviewSnapContext);

	XtAddEventHandler(previewSnap, ButtonPressMask,
			False, (XtEventHandler) _UxpreviewSnapMenuPost, (XtPointer) previewSnapPopup );

	return ( previewSnap );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_previewSnap( _Uxwhich, _Uxn, _Uxs )
	int	_Uxwhich;
	int	_Uxn;
	unsigned char	*_Uxs;
{
	Widget                  rtrn;
	_UxCpreviewSnap         *UxContext;
	static int		_Uxinit = 0;

	UxPreviewSnapContext = UxContext =
		(_UxCpreviewSnap *) UxNewContext( sizeof(_UxCpreviewSnap), False );

	which = _Uxwhich;
	n = _Uxn;
	s = _Uxs;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewClassId();
		UxpreviewSnap_readFile_Id = UxMethodRegister( _UxIfClassId,
					UxpreviewSnap_readFile_Name,
					(void*)_previewSnap_readFile );
		UxpreviewSnap_displayVal_Id = UxMethodRegister( _UxIfClassId,
					UxpreviewSnap_displayVal_Name,
					(void*)_previewSnap_displayVal );
		UxpreviewSnap_crea_entry_Id = UxMethodRegister( _UxIfClassId,
					UxpreviewSnap_crea_entry_Name,
					(void*)_previewSnap_crea_entry );
		UxpreviewSnap_readVal_Id = UxMethodRegister( _UxIfClassId,
					UxpreviewSnap_readVal_Name,
					(void*)_previewSnap_readVal );
		_Uxinit = 1;
	}

	{
		printf ("chiamata da %d rec = %d\n",which,n);
		selVarWidg =  (Widget)NULL;
		testata = (char *) malloc (strlen (s) + 1);
		strcpy (testata,s);
		rtrn = _Uxbuild_previewSnap();
		UxPutClassCode( previewSnap, _UxIfClassId );

		previewSnap_crea_entry (rtrn, &UxEnv);
		previewSnap_readFile (rtrn, &UxEnv);
		previewSnap_readVal (rtrn, &UxEnv);
		previewSnap_displayVal (rtrn, &UxEnv);
		XtVaSetValues (XtParent(UxGetWidget(rtrn)),
		   XmNtitle, s, XmNiconName, s, NULL);
		UxPopupInterface (rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

