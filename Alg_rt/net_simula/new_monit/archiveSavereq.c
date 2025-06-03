
/*******************************************************************************
       archiveSavereq.c
       (Generated from interface file archiveSavereq.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xm/Text.h>

#ifndef DESIGN_TIME
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include <stdlib.h>
#include <math.h>
#include "archivi.h"
/* #include "interf_attive.h" */
extern int archsave_interface_active;
#endif
#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

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
	Widget	Uxform14;
	Widget	Uxframe5;
	Widget	Uxform15;
	Widget	UxarchSaveReqQuitpb;
	Widget	UxarchSaveReqpb;
	Widget	Uxframe6;
	Widget	Uxform13;
	Widget	Uxlabel13;
	Widget	UxarchSaveReqCommText;
	Widget	UxarchSaveReqMenu;
	Widget	UxarchSaveMenuFile;
	Widget	UxarchSaveMenuFileQuit;
	Widget	UxarchSaveReqMenuFilecb;
	Widget	UxarchSaveReqMenuHelp;
	Widget	UxarchSaveReqMenuHelpComm;
	Widget	UxarchSaveReqMenuHelpcb;
	Widget	UxrowColumn2;
	Widget	Uxform16;
	Widget	Uxlabel15;
	Widget	UxarchSaveReqTinitText;
	Widget	Uxform17;
	Widget	Uxlabel16;
	Widget	UxarchSaveReqTfinText;
	Widget	Uxform18;
	Widget	Uxlabel17;
	Widget	UxarchSaveReqDiskText;
	Widget	Uxform19;
	Widget	Uxlabel18;
	Widget	UxarchSaveReqDiskavailText;
} _UxCarchiveSavereq;

static _UxCarchiveSavereq      *UxArchiveSavereqContext;
#define form14                  UxArchiveSavereqContext->Uxform14
#define frame5                  UxArchiveSavereqContext->Uxframe5
#define form15                  UxArchiveSavereqContext->Uxform15
#define archSaveReqQuitpb       UxArchiveSavereqContext->UxarchSaveReqQuitpb
#define archSaveReqpb           UxArchiveSavereqContext->UxarchSaveReqpb
#define frame6                  UxArchiveSavereqContext->Uxframe6
#define form13                  UxArchiveSavereqContext->Uxform13
#define label13                 UxArchiveSavereqContext->Uxlabel13
#define archSaveReqCommText     UxArchiveSavereqContext->UxarchSaveReqCommText
#define archSaveReqMenu         UxArchiveSavereqContext->UxarchSaveReqMenu
#define archSaveMenuFile        UxArchiveSavereqContext->UxarchSaveMenuFile
#define archSaveMenuFileQuit    UxArchiveSavereqContext->UxarchSaveMenuFileQuit
#define archSaveReqMenuFilecb   UxArchiveSavereqContext->UxarchSaveReqMenuFilecb
#define archSaveReqMenuHelp     UxArchiveSavereqContext->UxarchSaveReqMenuHelp
#define archSaveReqMenuHelpComm UxArchiveSavereqContext->UxarchSaveReqMenuHelpComm
#define archSaveReqMenuHelpcb   UxArchiveSavereqContext->UxarchSaveReqMenuHelpcb
#define rowColumn2              UxArchiveSavereqContext->UxrowColumn2
#define form16                  UxArchiveSavereqContext->Uxform16
#define label15                 UxArchiveSavereqContext->Uxlabel15
#define archSaveReqTinitText    UxArchiveSavereqContext->UxarchSaveReqTinitText
#define form17                  UxArchiveSavereqContext->Uxform17
#define label16                 UxArchiveSavereqContext->Uxlabel16
#define archSaveReqTfinText     UxArchiveSavereqContext->UxarchSaveReqTfinText
#define form18                  UxArchiveSavereqContext->Uxform18
#define label17                 UxArchiveSavereqContext->Uxlabel17
#define archSaveReqDiskText     UxArchiveSavereqContext->UxarchSaveReqDiskText
#define form19                  UxArchiveSavereqContext->Uxform19
#define label18                 UxArchiveSavereqContext->Uxlabel18
#define archSaveReqDiskavailText UxArchiveSavereqContext->UxarchSaveReqDiskavailText


Widget	archiveSavereq;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_archiveSavereq();
int free_disk();
int check_data_save_req();
int richiesta_save_arch();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*********************************************/
#ifndef DESIGN_TIME
int init_dati()
{
char stringa[10];

   disco_libero = free_disk();
   sprintf (stringa,"%d",disco_libero);
   XmTextSetString (UxGetWidget(archSaveReqDiskavailText),stringa); 
   return(0);
}
#endif

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_archSaveReqQuitpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	archsave_interface_active = 0;
	DistruggiInterfaccia (XtParent((Widget)UxClientData));
#endif
	
	
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	activateCB_archSaveReqpb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char testo[20];
	
	archsavetinit = 0.0;
	archsavetfin = 0.0;
	
	if (read_txt (UxGetWidget(archSaveReqTinitText),testo))
	   {
	   printf ("testo 1 non valido\n");
	   strcpy (testo,"0.0");
	   write_txt(UxGetWidget(archSaveReqTinitText),testo);
	   }
	archsavetinit = (float)atof(testo);
	printf ("testo = %s %f\n",testo,archsavetinit);
	
	if (read_txt (UxGetWidget(archSaveReqTfinText),testo))
	   {
	   printf ("testo 2 non valido\n");
	   strcpy (testo,"0.0");
	   write_txt(UxGetWidget(archSaveReqTfinText),testo);
	   }
	archsavetfin = (float)atof(testo);
	printf ("testo = %s %f\n",testo,archsavetfin);
	
	disco_libero = free_disk();
	sprintf (testo,"%d",disco_libero);
	write_txt(UxGetWidget(archSaveReqDiskavailText),testo);
	
	read_txt (UxGetWidget(archSaveReqCommText),testo);
	strcpy (comm_arch_save, testo);
	
	if (!check_data_save_req())
	   {
	   richiesta_save_arch();
	   printf ("save ok\n");
	   }
	else
	   {
	   printf ("save no\n");
	   }
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	activateCB_archSaveMenuFileQuit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	archsave_interface_active = 0;
	XtDestroyWidget(XtParent(UxGetWidget(archiveSavereq)));
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	activateCB_archSaveReqTinitText( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char testo[MAX_LUN_COMMENTO];
	   if (read_txt(UxWidget,testo))
	      {
	      printf ("entry non corretta \n");
	      strcpy (testo,"0.0");
	      write_txt(UxWidget,testo);
	      }
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	losingFocusCB_archSaveReqTinitText( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char testo[MAX_LUN_COMMENTO];
	   if (read_txt(UxWidget,testo))
	      {
	      printf ("entry non corretta \n");
	      strcpy (testo,"0.0");
	      write_txt(UxWidget,testo);
	      }
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	activateCB_archSaveReqTfinText( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char testo[MAX_LUN_COMMENTO];
	   if (read_txt(UxWidget,testo))
	      {
	      printf ("entry non corretta \n"); 
	      strcpy (testo,"0.0");
	      write_txt(UxWidget,testo);
	
	      }
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

static	void	losingFocusCB_archSaveReqTfinText( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSavereq      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSavereqContext;
	UxArchiveSavereqContext = UxContext =
			(_UxCarchiveSavereq *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char testo[MAX_LUN_COMMENTO];
	   if (read_txt(UxWidget,testo))
	      {
	      printf ("entry non corretta \n");
	      strcpy (testo,"0.0");
	      write_txt(UxWidget,testo);
	      }
#endif
	}
	UxArchiveSavereqContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_archiveSavereq()
{
	Widget		_UxParent;
	Widget		archSaveMenuFile_shell;
	Widget		archSaveReqMenuHelp_shell;


	/* Creation of archiveSavereq */
	_UxParent = XtVaCreatePopupShell( "archiveSavereq_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 689,
			XmNy, 150,
			XmNwidth, 418,
			XmNheight, 429,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "archiveSavereq",
			XmNiconName, "archiveSavereq",
			NULL );

	archiveSavereq = XtVaCreateManagedWidget( "archiveSavereq",
			xmFormWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 418,
			XmNheight, 429,
			XmNnoResize, TRUE,
			NULL );
	UxPutContext( archiveSavereq, (char *) UxArchiveSavereqContext );


	/* Creation of form14 */
	form14 = XtVaCreateManagedWidget( "form14",
			xmFormWidgetClass,
			archiveSavereq,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 16,
			XmNy, 108,
			XmNwidth, 750,
			XmNheight, 75,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_NONE,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form14, (char *) UxArchiveSavereqContext );


	/* Creation of frame5 */
	frame5 = XtVaCreateManagedWidget( "frame5",
			xmFrameWidgetClass,
			form14,
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
	UxPutContext( frame5, (char *) UxArchiveSavereqContext );


	/* Creation of form15 */
	form15 = XtVaCreateManagedWidget( "form15",
			xmFormWidgetClass,
			frame5,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 745,
			XmNheight, 75,
			NULL );
	UxPutContext( form15, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqQuitpb */
	archSaveReqQuitpb = XtVaCreateManagedWidget( "archSaveReqQuitpb",
			xmPushButtonWidgetClass,
			form15,
			XmNx, 493,
			XmNy, 35,
			XmNwidth, 202,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "QUIT" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_SELF,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNrightOffset, 20,
			XmNleftPosition, 0,
			XmNleftOffset, 100,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			NULL );
	XtAddCallback( archSaveReqQuitpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_archSaveReqQuitpb,
		UxGetWidget(archiveSavereq) );

	UxPutContext( archSaveReqQuitpb, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqpb */
	archSaveReqpb = XtVaCreateManagedWidget( "archSaveReqpb",
			xmPushButtonWidgetClass,
			form15,
			XmNx, 30,
			XmNy, 405,
			XmNwidth, 205,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "SAVE" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 15,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_SELF,
			XmNrightOffset, 100,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 15,
			NULL );
	XtAddCallback( archSaveReqpb, XmNactivateCallback,
		(XtCallbackProc) activateCB_archSaveReqpb,
		(XtPointer) UxArchiveSavereqContext );

	UxPutContext( archSaveReqpb, (char *) UxArchiveSavereqContext );


	/* Creation of frame6 */
	frame6 = XtVaCreateManagedWidget( "frame6",
			xmFrameWidgetClass,
			archiveSavereq,
			XmNx, 0,
			XmNy, 265,
			XmNwidth, 415,
			XmNheight, 85,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, form14,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( frame6, (char *) UxArchiveSavereqContext );


	/* Creation of form13 */
	form13 = XtVaCreateManagedWidget( "form13",
			xmFormWidgetClass,
			frame6,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 305,
			XmNwidth, 420,
			XmNheight, 74,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNrubberPositioning, FALSE,
			RES_CONVERT( XmNdialogTitle, "" ),
			NULL );
	UxPutContext( form13, (char *) UxArchiveSavereqContext );


	/* Creation of label13 */
	label13 = XtVaCreateManagedWidget( "label13",
			xmLabelWidgetClass,
			form13,
			XmNx, 20,
			XmNy, 5,
			XmNwidth, 285,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "Comment:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label13, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqCommText */
	archSaveReqCommText = XtVaCreateManagedWidget( "archSaveReqCommText",
			xmTextFieldWidgetClass,
			form13,
			XmNx, 20,
			XmNy, 35,
			XmNwidth, 378,
			XmNheight, 30,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNvalue, "",
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, label13,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			NULL );
	UxPutContext( archSaveReqCommText, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqMenu */
	archSaveReqMenu = XtVaCreateManagedWidget( "archSaveReqMenu",
			xmRowColumnWidgetClass,
			archiveSavereq,
			XmNrowColumnType, XmMENU_BAR,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( archSaveReqMenu, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveMenuFile */
	archSaveMenuFile_shell = XtVaCreatePopupShell ("archSaveMenuFile_shell",
			xmMenuShellWidgetClass, archSaveReqMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	archSaveMenuFile = XtVaCreateWidget( "archSaveMenuFile",
			xmRowColumnWidgetClass,
			archSaveMenuFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( archSaveMenuFile, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveMenuFileQuit */
	archSaveMenuFileQuit = XtVaCreateManagedWidget( "archSaveMenuFileQuit",
			xmPushButtonWidgetClass,
			archSaveMenuFile,
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );
	XtAddCallback( archSaveMenuFileQuit, XmNactivateCallback,
		(XtCallbackProc) activateCB_archSaveMenuFileQuit,
		(XtPointer) UxArchiveSavereqContext );

	UxPutContext( archSaveMenuFileQuit, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqMenuFilecb */
	archSaveReqMenuFilecb = XtVaCreateManagedWidget( "archSaveReqMenuFilecb",
			xmCascadeButtonWidgetClass,
			archSaveReqMenu,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, archSaveMenuFile,
			NULL );
	UxPutContext( archSaveReqMenuFilecb, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqMenuHelp */
	archSaveReqMenuHelp_shell = XtVaCreatePopupShell ("archSaveReqMenuHelp_shell",
			xmMenuShellWidgetClass, archSaveReqMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	archSaveReqMenuHelp = XtVaCreateWidget( "archSaveReqMenuHelp",
			xmRowColumnWidgetClass,
			archSaveReqMenuHelp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( archSaveReqMenuHelp, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqMenuHelpComm */
	archSaveReqMenuHelpComm = XtVaCreateManagedWidget( "archSaveReqMenuHelpComm",
			xmPushButtonWidgetClass,
			archSaveReqMenuHelp,
			RES_CONVERT( XmNlabelString, "Command" ),
			NULL );
	UxPutContext( archSaveReqMenuHelpComm, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqMenuHelpcb */
	archSaveReqMenuHelpcb = XtVaCreateManagedWidget( "archSaveReqMenuHelpcb",
			xmCascadeButtonWidgetClass,
			archSaveReqMenu,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNsubMenuId, archSaveReqMenuHelp,
			NULL );
	UxPutContext( archSaveReqMenuHelpcb, (char *) UxArchiveSavereqContext );


	/* Creation of rowColumn2 */
	rowColumn2 = XtVaCreateManagedWidget( "rowColumn2",
			xmRowColumnWidgetClass,
			archiveSavereq,
			XmNx, 0,
			XmNy, 25,
			XmNwidth, 418,
			XmNheight, 440,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, frame6,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, archSaveReqMenu,
			XmNtopOffset, 20,
			NULL );
	UxPutContext( rowColumn2, (char *) UxArchiveSavereqContext );


	/* Creation of form16 */
	form16 = XtVaCreateManagedWidget( "form16",
			xmFormWidgetClass,
			rowColumn2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 5,
			XmNwidth, 485,
			XmNheight, 45,
			XmNfractionBase, 10,
			NULL );
	UxPutContext( form16, (char *) UxArchiveSavereqContext );


	/* Creation of label15 */
	label15 = XtVaCreateManagedWidget( "label15",
			xmLabelWidgetClass,
			form16,
			XmNx, 0,
			XmNy, 15,
			XmNwidth, 150,
			XmNheight, 25,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 3,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Initial time:" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( label15, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqTinitText */
	archSaveReqTinitText = XtVaCreateManagedWidget( "archSaveReqTinitText",
			xmTextFieldWidgetClass,
			form16,
			XmNx, 165,
			XmNy, 10,
			XmNwidth, 227,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 4,
			XmNrightOffset, 20,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNtopAttachment, XmATTACH_NONE,
			XmNvalue, "",
			NULL );
	XtAddCallback( archSaveReqTinitText, XmNactivateCallback,
		(XtCallbackProc) activateCB_archSaveReqTinitText,
		(XtPointer) UxArchiveSavereqContext );
	XtAddCallback( archSaveReqTinitText, XmNlosingFocusCallback,
		(XtCallbackProc) losingFocusCB_archSaveReqTinitText,
		(XtPointer) UxArchiveSavereqContext );

	UxPutContext( archSaveReqTinitText, (char *) UxArchiveSavereqContext );


	/* Creation of form17 */
	form17 = XtVaCreateManagedWidget( "form17",
			xmFormWidgetClass,
			rowColumn2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 13,
			XmNy, 13,
			XmNwidth, 485,
			XmNheight, 45,
			XmNfractionBase, 10,
			NULL );
	UxPutContext( form17, (char *) UxArchiveSavereqContext );


	/* Creation of label16 */
	label16 = XtVaCreateManagedWidget( "label16",
			xmLabelWidgetClass,
			form17,
			XmNx, 0,
			XmNy, 10,
			XmNwidth, 150,
			XmNheight, 25,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 3,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Final time:" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( label16, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqTfinText */
	archSaveReqTfinText = XtVaCreateManagedWidget( "archSaveReqTfinText",
			xmTextFieldWidgetClass,
			form17,
			XmNx, 165,
			XmNy, 10,
			XmNwidth, 227,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 4,
			XmNrightOffset, 20,
			XmNvalue, "",
			NULL );
	XtAddCallback( archSaveReqTfinText, XmNactivateCallback,
		(XtCallbackProc) activateCB_archSaveReqTfinText,
		(XtPointer) UxArchiveSavereqContext );
	XtAddCallback( archSaveReqTfinText, XmNlosingFocusCallback,
		(XtCallbackProc) losingFocusCB_archSaveReqTfinText,
		(XtPointer) UxArchiveSavereqContext );

	UxPutContext( archSaveReqTfinText, (char *) UxArchiveSavereqContext );


	/* Creation of form18 */
	form18 = XtVaCreateManagedWidget( "form18",
			xmFormWidgetClass,
			rowColumn2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 13,
			XmNy, 61,
			XmNwidth, 485,
			XmNheight, 45,
			XmNfractionBase, 10,
			NULL );
	UxPutContext( form18, (char *) UxArchiveSavereqContext );


	/* Creation of label17 */
	label17 = XtVaCreateManagedWidget( "label17",
			xmLabelWidgetClass,
			form18,
			XmNx, 5,
			XmNy, 10,
			XmNwidth, 150,
			XmNheight, 25,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 3,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Required disk:" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( label17, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqDiskText */
	archSaveReqDiskText = XtVaCreateManagedWidget( "archSaveReqDiskText",
			xmTextFieldWidgetClass,
			form18,
			XmNx, 165,
			XmNy, 10,
			XmNwidth, 227,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 4,
			XmNrightOffset, 20,
			XmNeditable, FALSE,
			XmNcursorPositionVisible, FALSE,
			XmNvalue, "",
			NULL );
	UxPutContext( archSaveReqDiskText, (char *) UxArchiveSavereqContext );


	/* Creation of form19 */
	form19 = XtVaCreateManagedWidget( "form19",
			xmFormWidgetClass,
			rowColumn2,
			XmNresizePolicy, XmRESIZE_ANY,
			XmNx, 5,
			XmNy, 220,
			XmNwidth, 412,
			XmNheight, 48,
			XmNfractionBase, 10,
			NULL );
	UxPutContext( form19, (char *) UxArchiveSavereqContext );


	/* Creation of label18 */
	label18 = XtVaCreateManagedWidget( "label18",
			xmLabelWidgetClass,
			form19,
			XmNx, 5,
			XmNy, 10,
			XmNwidth, 150,
			XmNheight, 25,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 3,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Available disk:" ),
			XmNalignment, XmALIGNMENT_END,
			NULL );
	UxPutContext( label18, (char *) UxArchiveSavereqContext );


	/* Creation of archSaveReqDiskavailText */
	archSaveReqDiskavailText = XtVaCreateManagedWidget( "archSaveReqDiskavailText",
			xmTextFieldWidgetClass,
			form19,
			XmNx, 234,
			XmNy, 10,
			XmNwidth, 331,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 4,
			XmNrightOffset, 20,
			XmNresizeWidth, TRUE,
			XmNvalue, "",
			NULL );
	UxPutContext( archSaveReqDiskavailText, (char *) UxArchiveSavereqContext );

	XtVaSetValues(archSaveReqMenu,
			XmNmenuHelpWidget, archSaveReqMenuHelpcb,
			NULL );


	XtAddCallback( archiveSavereq, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxArchiveSavereqContext);


	return ( archiveSavereq );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_archiveSavereq()
{
	Widget                  rtrn;
	_UxCarchiveSavereq      *UxContext;

	UxArchiveSavereqContext = UxContext =
		(_UxCarchiveSavereq *) UxNewContext( sizeof(_UxCarchiveSavereq), False );


	{
		printf ("colloquio archive editing\n");
		rtrn = _Uxbuild_archiveSavereq();

#ifndef DESIGN_TIME
		init_dati ();
		archsave_interface_active = 1;
#endif                
		UxPopupInterface (archiveSavereq, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

