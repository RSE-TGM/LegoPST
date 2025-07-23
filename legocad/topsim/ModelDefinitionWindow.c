
/*******************************************************************************
	ModelDefinitionWindow.c
	(Generated from interface file ModelDefinitionWindow.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo ModelDefinitionWindow.i
   tipo 
   release 1.24
   data 8/29/95
   reserved @(#)ModelDefinitionWindow.i	1.24
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#ifndef DESIGN_TIME
#include <stdlib.h>
#endif

#include "topsim.h"
#include "lg1_topsim.h"
#include "questionMsgDb.h"

extern SIMULATORE *simulatore;

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
	Widget	UxModelDefinitionWindow;
	Widget	Uxform4;
	Widget	UxModellabel;
	Widget	UxseparatorGadget1;
	Widget	UxLModelName;
	Widget	UxLModelDescription;
	Widget	UxPModelType;
	Widget	UxProcess;
	Widget	UxRegolation;
	Widget	UxNolego;
	Widget	UxModelType;
	Widget	UxPOS;
	Widget	UxDefOS;
	Widget	UxOSAIX;
	Widget	UxOSVMS;
	Widget	UxOSOSF1;
	Widget	UxOSLINUX;
	Widget	UxMOS;
	Widget	UxLActivationInterval;
	Widget	UxpushButton1;
	Widget	UxpushButton2;
	Widget	UxModelName;
	Widget	UxModelDescription;
	Widget	UxmodelPath;
	Widget	UxModelPath;
	Widget	UxRemoteModelPath;
	Widget	UxActivationInterval;
	Widget	UxLActivationInterval1;
	Widget	UxLModelType;
	Widget	UxLHostName;
	Widget	UxLOS;
	Widget	UxHostName;
	int	Uxtipo_modello;
	unsigned char	UxOSS[20];
	unsigned char	Uxhost_name[256];
	MODELLO	*Uxlocaltask;
	swidget	Uxselection_wid;
	MODELLO	*Uxtask;
	int	Uxtipo_operaz;
} _UxCModelDefinitionWindow;

#define ModelDefinitionWindow   UxModelDefinitionWindowContext->UxModelDefinitionWindow
#define form4                   UxModelDefinitionWindowContext->Uxform4
#define Modellabel              UxModelDefinitionWindowContext->UxModellabel
#define separatorGadget1        UxModelDefinitionWindowContext->UxseparatorGadget1
#define LModelName              UxModelDefinitionWindowContext->UxLModelName
#define LModelDescription       UxModelDefinitionWindowContext->UxLModelDescription
#define PModelType              UxModelDefinitionWindowContext->UxPModelType
#define Process                 UxModelDefinitionWindowContext->UxProcess
#define Regolation              UxModelDefinitionWindowContext->UxRegolation
#define Nolego                  UxModelDefinitionWindowContext->UxNolego
#define ModelType               UxModelDefinitionWindowContext->UxModelType
#define POS                     UxModelDefinitionWindowContext->UxPOS
#define DefOS                   UxModelDefinitionWindowContext->UxDefOS
#define OSAIX                   UxModelDefinitionWindowContext->UxOSAIX
#define OSVMS                   UxModelDefinitionWindowContext->UxOSVMS
#define OSOSF1                  UxModelDefinitionWindowContext->UxOSOSF1
#define OSLINUX                 UxModelDefinitionWindowContext->UxOSLINUX
#define MOS                     UxModelDefinitionWindowContext->UxMOS
#define LActivationInterval     UxModelDefinitionWindowContext->UxLActivationInterval
#define pushButton1             UxModelDefinitionWindowContext->UxpushButton1
#define pushButton2             UxModelDefinitionWindowContext->UxpushButton2
#define ModelName               UxModelDefinitionWindowContext->UxModelName
#define ModelDescription        UxModelDefinitionWindowContext->UxModelDescription
#define modelPath               UxModelDefinitionWindowContext->UxmodelPath
#define ModelPath               UxModelDefinitionWindowContext->UxModelPath
#define RemoteModelPath         UxModelDefinitionWindowContext->UxRemoteModelPath
#define ActivationInterval      UxModelDefinitionWindowContext->UxActivationInterval
#define LActivationInterval1    UxModelDefinitionWindowContext->UxLActivationInterval1
#define LModelType              UxModelDefinitionWindowContext->UxLModelType
#define LHostName               UxModelDefinitionWindowContext->UxLHostName
#define LOS                     UxModelDefinitionWindowContext->UxLOS
#define HostName                UxModelDefinitionWindowContext->UxHostName
#define tipo_modello            UxModelDefinitionWindowContext->Uxtipo_modello
#define OSS                     UxModelDefinitionWindowContext->UxOSS
#define host_name               UxModelDefinitionWindowContext->Uxhost_name
#define localtask               UxModelDefinitionWindowContext->Uxlocaltask
#define selection_wid           UxModelDefinitionWindowContext->Uxselection_wid
#define task                    UxModelDefinitionWindowContext->Uxtask
#define tipo_operaz             UxModelDefinitionWindowContext->Uxtipo_operaz

static _UxCModelDefinitionWindow	*UxModelDefinitionWindowContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_ModelDefinitionWindow();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

void set_sel_path(char *path)
{
    XmTextFieldSetString(ModelPath,path);
}

read_input_data(MODELLO *modap)
{
   char *appo;

   strcpy(modap->nome,XmTextFieldGetString(ModelName));

   tomaius(modap->nome);
   strcpy(modap->descr,XmTextFieldGetString(ModelDescription));
   strcpy(modap->local_path,XmTextFieldGetString(ModelPath));
   strcpy(modap->remote_path,XmTextFieldGetString(RemoteModelPath));
   modap->activation_interval = atof(XmTextFieldGetString(ActivationInterval));



   modap->tipo_task = tipo_modello;
   strcpy(modap->host,host_name);
   strcpy(modap->OS,OSS);
   strcpy(modap->guest,"guest");


  printf("nome=%s\n",modap->nome);
  printf("descr=%s\n",modap->descr);
  printf("local_path=%s\n",modap->local_path);
  printf("remote_path=%s\n",modap->remote_path);
  printf("activation_interval=%f\n",modap->activation_interval);
  printf("tipo_task=%d\n",modap->tipo_task);
  printf("host=%s\n",modap->host);
  printf("OS=%s\n",modap->OS);

}

riempi_campi(MODELLO *mod)
{
   char cfloat[50];

   XmTextFieldSetString(ModelName,mod->nome);
   XmTextFieldSetString(ModelDescription,mod->descr);
   XmTextFieldSetString(ModelPath,mod->local_path);
   XmTextFieldSetString(RemoteModelPath,mod->remote_path);
   XmTextFieldSetString(HostName,mod->host);
   sprintf(cfloat,"%f",mod->activation_interval);
   XmTextFieldSetString(ActivationInterval,cfloat);

    tipo_modello = mod->tipo_task;

   strcpy(OSS,mod->OS);

}     


set_menu_actual_button(swidget menuId,swidget buttonId)
{
   set_something_val( menuId,XmNmenuHistory, (XtArgVal) buttonId);
} 

set_menu(int operaz,MODELLO *model)
{
 
  if( operaz == MODTASK )
  {  
     /* setto il bottone del tipo modello */ 
     if ( model->tipo_task == PROCESSO )
        set_menu_actual_button(ModelType,Process);
     else if  ( model->tipo_task == REGOLAZIONE )
        set_menu_actual_button(ModelType,Regolation);
     else if  ( model->tipo_task == NOLEGO )
        set_menu_actual_button(ModelType,Nolego);

  }
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_Process( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	tipo_modello = PROCESSO;
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_Regolation( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	tipo_modello = REGOLAZIONE;
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_Nolego( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	tipo_modello = NOLEGO;
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_DefOS( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	strcpy(OSS,getenv("OS"));
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_OSAIX( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	strcpy(OSS,"AIX");
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_OSVMS( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	strcpy(OSS,"VMS");
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_OSOSF1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	strcpy(OSS,"OSF1");
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_OSLINUX( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	strcpy(OSS,"LINUX");
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_pushButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	{
	extern Boolean simulator_changed;
	extern int add_task();
	extern swidget ModelDefinitionInterface;
	extern int check_delta_time();
	extern void set_lista_bl();
	MODELLO qtask;
	swidget wid;
	extern swidget create_messageDialog();
	extern swidget create_msgToUser(); 
	Boolean tempi;
	
	if( tipo_operaz == ADDTASK )
	{
	   read_input_data(&qtask);
	
	
	   if( add_task(&qtask) != TRUE )
	   {
	      wid = create_messageDialog(ModelDefinitionWindow,ADDTASK);
	      UxPopupInterface(wid,no_grab); 
	   }
	   else
	   {
	      tempi = check_delta_time();
	      if( !tempi )
	         create_msgToUser(0,"WARNING Input Data ACTIVATION INTERVAL Not Correct"); 
	      simulator_changed=True;
	      ModelDefinitionInterface = NULL;
	
	      UxDestroyInterface(ModelDefinitionWindow);
	   }
	}
	else
	{
	   read_input_data(localtask);
	
	   tempi = check_delta_time();
	   if( !tempi )
	      create_msgToUser(0,"WARNING Input Data ACTIVATION INTERVAL Not Correct");
	
	   simulator_changed=True;
	   ModelDefinitionInterface = NULL;
	
	   set_listbl(simulatore);
	
	   UxDestroyInterface(ModelDefinitionWindow);
	}
	}
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_pushButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	{
	extern swidget ModelDefinitionInterface;
	
	ModelDefinitionInterface = NULL;
	
	UxDestroyInterface( ModelDefinitionWindow );
	}
	UxModelDefinitionWindowContext = UxSaveCtx;
}

static void	activateCB_modelPath( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCModelDefinitionWindow *UxSaveCtx, *UxContext;

	UxSaveCtx = UxModelDefinitionWindowContext;
	UxModelDefinitionWindowContext = UxContext =
			(_UxCModelDefinitionWindow *) UxGetContext( UxWidget );
	{
	{
	   extern swidget create_ModelSelection();
	
	   selection_wid = create_ModelSelection(XtParent(UxWidget));
	   UxPopupInterface(selection_wid,no_grab);
	}
	}
	UxModelDefinitionWindowContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ModelDefinitionWindow()
{
	Widget	PModelType_shell;
	Widget	POS_shell;

	ModelDefinitionWindow = XtVaCreatePopupShell( "ModelDefinitionWindow",
			topLevelShellWidgetClass, UxTopLevel,
			XmNwidth, 559,
			XmNheight, 491,
			XmNy, 169,
			XmNx, 182,
			NULL );

	UxPutContext( ModelDefinitionWindow, (char *) UxModelDefinitionWindowContext );

	form4 = XtVaCreateManagedWidget( "form4",
			xmFormWidgetClass, ModelDefinitionWindow,
			XmNwidth, 541,
			XmNheight, 376,
			XmNy, 100,
			XmNx, 100,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form4, (char *) UxModelDefinitionWindowContext );

	Modellabel = XtVaCreateManagedWidget( "Modellabel",
			xmLabelWidgetClass, form4,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1" ),
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Model Definition" ),
			XmNheight, 30,
			XmNwidth, 360,
			XmNy, 0,
			XmNx, 80,
			NULL );

	UxPutContext( Modellabel, (char *) UxModelDefinitionWindowContext );

	separatorGadget1 = XtVaCreateManagedWidget( "separatorGadget1",
			xmSeparatorGadgetClass, form4,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, Modellabel,
			XmNtopOffset, 2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNheight, 3,
			XmNwidth, 100,
			XmNy, 30,
			XmNx, 10,
			NULL );

	UxPutContext( separatorGadget1, (char *) UxModelDefinitionWindowContext );

	LModelName = XtVaCreateManagedWidget( "LModelName",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Model Name:" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 120,
			XmNx, 40,
			NULL );

	UxPutContext( LModelName, (char *) UxModelDefinitionWindowContext );

	LModelDescription = XtVaCreateManagedWidget( "LModelDescription",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Description:" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 180,
			XmNx, 40,
			NULL );

	UxPutContext( LModelDescription, (char *) UxModelDefinitionWindowContext );

	PModelType_shell = XtVaCreatePopupShell ("PModelType_shell",
			xmMenuShellWidgetClass, form4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	PModelType = XtVaCreateWidget( "PModelType",
			xmRowColumnWidgetClass, PModelType_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( PModelType, (char *) UxModelDefinitionWindowContext );

	Process = XtVaCreateManagedWidget( "Process",
			xmPushButtonWidgetClass, PModelType,
			RES_CONVERT( XmNlabelString, "Process" ),
			NULL );

	UxPutContext( Process, (char *) UxModelDefinitionWindowContext );

	Regolation = XtVaCreateManagedWidget( "Regolation",
			xmPushButtonWidgetClass, PModelType,
			RES_CONVERT( XmNlabelString, "Regolation" ),
			NULL );

	UxPutContext( Regolation, (char *) UxModelDefinitionWindowContext );

	Nolego = XtVaCreateManagedWidget( "Nolego",
			xmPushButtonWidgetClass, PModelType,
			RES_CONVERT( XmNlabelString, "Not LEGO" ),
			NULL );

	UxPutContext( Nolego, (char *) UxModelDefinitionWindowContext );

	ModelType = XtVaCreateManagedWidget( "ModelType",
			xmRowColumnWidgetClass, form4,
			XmNy, 230,
			XmNx, 180,
			XmNsubMenuId, PModelType,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( ModelType, (char *) UxModelDefinitionWindowContext );

	POS_shell = XtVaCreatePopupShell ("POS_shell",
			xmMenuShellWidgetClass, form4,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	POS = XtVaCreateWidget( "POS",
			xmRowColumnWidgetClass, POS_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( POS, (char *) UxModelDefinitionWindowContext );

	DefOS = XtVaCreateManagedWidget( "DefOS",
			xmPushButtonWidgetClass, POS,
			RES_CONVERT( XmNlabelString, "Default" ),
			NULL );

	UxPutContext( DefOS, (char *) UxModelDefinitionWindowContext );

	OSAIX = XtVaCreateManagedWidget( "OSAIX",
			xmPushButtonWidgetClass, POS,
			RES_CONVERT( XmNlabelString, "AIX" ),
			NULL );

	UxPutContext( OSAIX, (char *) UxModelDefinitionWindowContext );

	OSVMS = XtVaCreateManagedWidget( "OSVMS",
			xmPushButtonWidgetClass, POS,
			RES_CONVERT( XmNlabelString, "VMS" ),
			NULL );

	UxPutContext( OSVMS, (char *) UxModelDefinitionWindowContext );

	OSOSF1 = XtVaCreateManagedWidget( "OSOSF1",
			xmPushButtonWidgetClass, POS,
			RES_CONVERT( XmNlabelString, "OSF1" ),
			NULL );

	UxPutContext( OSOSF1, (char *) UxModelDefinitionWindowContext );

	OSLINUX = XtVaCreateManagedWidget( "OSLINUX",
			xmPushButtonWidgetClass, POS,
			RES_CONVERT( XmNlabelString, "LINUX" ),
			NULL );

	UxPutContext( OSLINUX, (char *) UxModelDefinitionWindowContext );

	MOS = XtVaCreateManagedWidget( "MOS",
			xmRowColumnWidgetClass, form4,
			XmNy, 280,
			XmNx, 390,
			XmNsubMenuId, POS,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( MOS, (char *) UxModelDefinitionWindowContext );

	LActivationInterval = XtVaCreateManagedWidget( "LActivationInterval",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Activation Interval:" ),
			XmNheight, 30,
			XmNwidth, 130,
			XmNy, 380,
			XmNx, 40,
			NULL );

	UxPutContext( LActivationInterval, (char *) UxModelDefinitionWindowContext );

	pushButton1 = XtVaCreateManagedWidget( "pushButton1",
			xmPushButtonWidgetClass, form4,
			XmNtopWidget, LActivationInterval,
			XmNtopOffset, 30,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 60,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 431,
			XmNx, 60,
			NULL );

	UxPutContext( pushButton1, (char *) UxModelDefinitionWindowContext );

	pushButton2 = XtVaCreateManagedWidget( "pushButton2",
			xmPushButtonWidgetClass, form4,
			XmNrightOffset, 60,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 30,
			XmNtopWidget, LActivationInterval,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 431,
			XmNx, 409,
			NULL );

	UxPutContext( pushButton2, (char *) UxModelDefinitionWindowContext );

	ModelName = XtVaCreateManagedWidget( "ModelName",
			xmTextFieldWidgetClass, form4,
			XmNeditable, FALSE,
			XmNheight, 30,
			XmNwidth, 167,
			XmNy, 120,
			XmNx, 190,
			NULL );

	UxPutContext( ModelName, (char *) UxModelDefinitionWindowContext );

	ModelDescription = XtVaCreateManagedWidget( "ModelDescription",
			xmTextFieldWidgetClass, form4,
			XmNheight, 30,
			XmNwidth, 342,
			XmNy, 180,
			XmNx, 190,
			NULL );

	UxPutContext( ModelDescription, (char *) UxModelDefinitionWindowContext );

	modelPath = XtVaCreateManagedWidget( "modelPath",
			xmPushButtonWidgetClass, form4,
			RES_CONVERT( XmNlabelString, "Local Path" ),
			XmNheight, 31,
			XmNwidth, 124,
			XmNy, 60,
			XmNx, 30,
			NULL );

	UxPutContext( modelPath, (char *) UxModelDefinitionWindowContext );

	ModelPath = XtVaCreateManagedWidget( "ModelPath",
			xmTextFieldWidgetClass, form4,
			XmNheight, 34,
			XmNwidth, 341,
			XmNy, 60,
			XmNx, 190,
			NULL );

	UxPutContext( ModelPath, (char *) UxModelDefinitionWindowContext );

	RemoteModelPath = XtVaCreateManagedWidget( "RemoteModelPath",
			xmTextFieldWidgetClass, form4,
			XmNheight, 30,
			XmNwidth, 340,
			XmNy, 330,
			XmNx, 190,
			NULL );

	UxPutContext( RemoteModelPath, (char *) UxModelDefinitionWindowContext );

	ActivationInterval = XtVaCreateManagedWidget( "ActivationInterval",
			xmTextFieldWidgetClass, form4,
			XmNheight, 30,
			XmNwidth, 147,
			XmNy, 380,
			XmNx, 190,
			NULL );

	UxPutContext( ActivationInterval, (char *) UxModelDefinitionWindowContext );

	LActivationInterval1 = XtVaCreateManagedWidget( "LActivationInterval1",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Remote Path:" ),
			XmNheight, 30,
			XmNwidth, 90,
			XmNy, 330,
			XmNx, 40,
			NULL );

	UxPutContext( LActivationInterval1, (char *) UxModelDefinitionWindowContext );

	LModelType = XtVaCreateManagedWidget( "LModelType",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Model Type:" ),
			XmNheight, 35,
			XmNwidth, 79,
			XmNy, 230,
			XmNx, 40,
			NULL );

	UxPutContext( LModelType, (char *) UxModelDefinitionWindowContext );

	LHostName = XtVaCreateManagedWidget( "LHostName",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Host Name:" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 280,
			XmNx, 40,
			NULL );

	UxPutContext( LHostName, (char *) UxModelDefinitionWindowContext );

	LOS = XtVaCreateManagedWidget( "LOS",
			xmLabelWidgetClass, form4,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "OS:" ),
			XmNheight, 30,
			XmNwidth, 30,
			XmNy, 280,
			XmNx, 350,
			NULL );

	UxPutContext( LOS, (char *) UxModelDefinitionWindowContext );

	HostName = XtVaCreateManagedWidget( "HostName",
			xmTextFieldWidgetClass, form4,
			XmNheight, 30,
			XmNwidth, 140,
			XmNy, 280,
			XmNx, 190,
			NULL );

	UxPutContext( HostName, (char *) UxModelDefinitionWindowContext );

	XtAddCallback( ModelDefinitionWindow, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( Process, XmNactivateCallback,
			activateCB_Process,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( Regolation, XmNactivateCallback,
			activateCB_Regolation,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( Nolego, XmNactivateCallback,
			activateCB_Nolego,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( DefOS, XmNactivateCallback,
			activateCB_DefOS,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( OSAIX, XmNactivateCallback,
			activateCB_OSAIX,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( OSVMS, XmNactivateCallback,
			activateCB_OSVMS,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( OSOSF1, XmNactivateCallback,
			activateCB_OSOSF1,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( OSLINUX, XmNactivateCallback,
			activateCB_OSLINUX,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( pushButton1, XmNactivateCallback,
			activateCB_pushButton1,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( pushButton2, XmNactivateCallback,
			activateCB_pushButton2,
			(XtPointer) UxModelDefinitionWindowContext );

	XtAddCallback( modelPath, XmNactivateCallback,
			activateCB_modelPath,
			(XtPointer) UxModelDefinitionWindowContext );



	return ( ModelDefinitionWindow );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_ModelDefinitionWindow( _Uxtask, _Uxtipo_operaz )
	MODELLO	*_Uxtask;
	int	_Uxtipo_operaz;
{
	Widget                  rtrn;
	_UxCModelDefinitionWindow *UxContext;

	UxModelDefinitionWindowContext = UxContext =
		(_UxCModelDefinitionWindow *) XtMalloc( sizeof(_UxCModelDefinitionWindow) );

	task = _Uxtask;
	tipo_operaz = _Uxtipo_operaz;
	{
		selection_wid = NULL;
		if( tipo_operaz == ADDTASK )
		{
		   tipo_modello = 1; /*(PROCESSO)*/
		   strcpy(OSS,getenv("OS"));
		   gethostname(host_name,MAXHOSTNAMELEN);
		}
		rtrn = _Uxbuild_ModelDefinitionWindow();

		if( tipo_operaz == MODTASK)
		{
		   localtask = task;
		   riempi_campi(localtask);
		   set_menu(tipo_operaz,localtask);
		}
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_ModelDefinitionWindow( _Uxtask, _Uxtipo_operaz )
	MODELLO	*_Uxtask;
	int	_Uxtipo_operaz;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_ModelDefinitionWindow( _Uxtask, _Uxtipo_operaz );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

