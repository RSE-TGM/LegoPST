
/*******************************************************************************
	Overwrite.c
	(Generated from interface file Overwrite.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo Overwrite.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)Overwrite.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include "menu_graf.h"

extern swidget File_Save;
extern int nome_valido;
extern int Modifica,Salvato;
extern int pid;
extern FILE *fpAPPO,*fpDAT;

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
	Widget	Uxseparator3;
	Widget	UxpushButton5;
	Widget	UxpushButton6;
	unsigned char	*UxSorgente;
	unsigned char	*UxDestinatario;
	int	UxFl;
	int	Uxfl;
	unsigned char	*UxSorg;
	unsigned char	*UxDestin;
} _UxCOverwrite;

#define separator3              UxOverwriteContext->Uxseparator3
#define pushButton5             UxOverwriteContext->UxpushButton5
#define pushButton6             UxOverwriteContext->UxpushButton6
#define Sorgente                UxOverwriteContext->UxSorgente
#define Destinatario            UxOverwriteContext->UxDestinatario
#define Fl                      UxOverwriteContext->UxFl
#define fl                      UxOverwriteContext->Uxfl
#define Sorg                    UxOverwriteContext->UxSorg
#define Destin                  UxOverwriteContext->UxDestin

static _UxCOverwrite	*UxOverwriteContext;

Widget	Overwrite;
Widget	label_overwrite;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Overwrite();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_pushButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCOverwrite           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOverwriteContext;
	UxOverwriteContext = UxContext =
			(_UxCOverwrite *) UxGetContext( UxWidget );
	{
	extern char *appo;
	
	char cdo[100];
	
	Uccidi_grafics ();
	fclose (fpAPPO);
	fclose (fpDAT);
	strcpy (cdo,"cp ");
	strcat (cdo,Sorgente);
	strcat (cdo," ");
	strcat (cdo,Destinatario);
	system (cdo);
	nome_valido = 1;
	Modifica = 0;
	Salvato = 1;
	if (Fl == ASCII_SAVE) 
		{
		strcpy (cdo,"rm ");
		strcat (cdo, Sorgente);
		system (cdo);
		}
	UxDestroyInterface (File_Save);
	UxDestroyInterface (Overwrite);
	}
	UxOverwriteContext = UxSaveCtx;
}

static void	activateCB_pushButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCOverwrite           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxOverwriteContext;
	UxOverwriteContext = UxContext =
			(_UxCOverwrite *) UxGetContext( UxWidget );
	{
	UxDestroyInterface (Overwrite);
	}
	UxOverwriteContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Overwrite()
{
	Widget	Overwrite_shell;

	Overwrite_shell = XtVaCreatePopupShell( "Overwrite_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 711,
			XmNy, 743,
			XmNwidth, 288,
			XmNheight, 121,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "Overwrite",
			NULL );

	Overwrite = XtVaCreateWidget( "Overwrite",
			xmFormWidgetClass, Overwrite_shell,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			XmNheight, 121,
			XmNwidth, 288,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( Overwrite, (char *) UxOverwriteContext );

	separator3 = XtVaCreateManagedWidget( "separator3",
			xmSeparatorWidgetClass, Overwrite,
			XmNheight, 15,
			XmNwidth, 285,
			XmNy, 55,
			XmNx, 5,
			NULL );

	UxPutContext( separator3, (char *) UxOverwriteContext );

	label_overwrite = XtVaCreateManagedWidget( "label_overwrite",
			xmLabelWidgetClass, Overwrite,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNheight, 45,
			XmNwidth, 280,
			XmNy, 5,
			XmNx, 5,
			NULL );

	UxPutContext( label_overwrite, (char *) UxOverwriteContext );

	pushButton5 = XtVaCreateManagedWidget( "pushButton5",
			xmPushButtonWidgetClass, Overwrite,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNheight, 45,
			XmNwidth, 90,
			XmNy, 70,
			XmNx, 5,
			NULL );

	UxPutContext( pushButton5, (char *) UxOverwriteContext );

	pushButton6 = XtVaCreateManagedWidget( "pushButton6",
			xmPushButtonWidgetClass, Overwrite,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 45,
			XmNwidth, 90,
			XmNy, 70,
			XmNx, 190,
			NULL );

	UxPutContext( pushButton6, (char *) UxOverwriteContext );

	XtAddCallback( Overwrite, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxOverwriteContext );

	XtAddCallback( pushButton5, XmNactivateCallback,
			activateCB_pushButton5,
			(XtPointer) UxOverwriteContext );

	XtAddCallback( pushButton6, XmNactivateCallback,
			activateCB_pushButton6,
			(XtPointer) UxOverwriteContext );



	return ( Overwrite );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Overwrite( _Uxfl, _UxSorg, _UxDestin )
	int	_Uxfl;
	unsigned char	*_UxSorg;
	unsigned char	*_UxDestin;
{
	Widget                  rtrn;
	_UxCOverwrite           *UxContext;

	UxOverwriteContext = UxContext =
		(_UxCOverwrite *) XtMalloc( sizeof(_UxCOverwrite) );

	fl = _Uxfl;
	Sorg = _UxSorg;
	Destin = _UxDestin;
	{
		char stringa[100];
		XmString Messaggio;
		
		Sorgente = XtNewString (Sorg);
		Destinatario = XtNewString (Destin);
		Fl = fl;
		rtrn = _Uxbuild_Overwrite();

		strcpy (stringa,"File exist; overwrite it?\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (label_overwrite, XmNlabelString, (XtArgVal) Messaggio);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Overwrite( _Uxfl, _UxSorg, _UxDestin )
	int	_Uxfl;
	unsigned char	*_UxSorg;
	unsigned char	*_UxDestin;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Overwrite( _Uxfl, _UxSorg, _UxDestin );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

