
/*******************************************************************************
	File_Save.c
	(Generated from interface file File_Save.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo File_Save.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)File_Save.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include "menu_graf.h"

extern FILE *fpAPPO,*fpDAT;
extern swidget Message;
extern swidget create_Overwrite();
extern int numero_temp;
extern char Path_appo[];
extern int nome_valido;
extern int Modifica,Salvato;

char *Nomefile; /* Nome file valido per i salvataggi */

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
	Widget	Uxlabel5;
	int	UxSalvaCome;
	unsigned char	*UxfileSave;
	int	UxOpzione;
	Position	UxPX;
	Position	UxPY;
	unsigned char	*UxNomeFile;
} _UxCFile_Save;

#define label5                  UxFile_SaveContext->Uxlabel5
#define SalvaCome               UxFile_SaveContext->UxSalvaCome
#define fileSave                UxFile_SaveContext->UxfileSave
#define Opzione                 UxFile_SaveContext->UxOpzione
#define PX                      UxFile_SaveContext->UxPX
#define PY                      UxFile_SaveContext->UxPY
#define NomeFile                UxFile_SaveContext->UxNomeFile

static _UxCFile_Save	*UxFile_SaveContext;

Widget	File_Save;
Widget	name_save;
Widget	push_save;
Widget	push_cancel;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_File_Save();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_push_save( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFile_Save           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFile_SaveContext;
	UxFile_SaveContext = UxContext =
			(_UxCFile_Save *) UxGetContext( UxWidget );
	{
	int risul;
	char cdo[100],stringa[100],*appoggio;
	XmString Messaggio;
	
	appoggio = XmTextFieldGetString (name_save);
	if (appoggio [0] == '\0')
		{
		strcpy (stringa,"File name is not valid !\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
		{
		risul = Controllo_gia_presente (appoggio);
		if (risul == 0)
			{
			if (SalvaCome == NORMAL_SAVE)
				{
				Uccidi_grafics ();
				fclose (fpAPPO);
				fclose (fpDAT);
				strcpy (cdo,"cp ");
				strcat (cdo,fileSave);
				strcat (cdo," ");
				strcat (cdo,appoggio);
				system (cdo);
				nome_valido = 1;
				Modifica = 0;
				Salvato = 1;
				Nomefile = XtNewString (appoggio);
				}
			if (SalvaCome == ASCII_SAVE)
				{
				Uccidi_grafics ();
				strcpy (cdo, "cp ");
				strcat (cdo, fileSave);
				strcat (cdo, " ");
				strcat (cdo, appoggio);
				system (cdo);
				strcpy (cdo, "rm ");
				strcat (cdo, fileSave);
				system (cdo);
				}
			UxDestroyInterface (File_Save);
			}
		else
			{
			if (SalvaCome == ASCII_SAVE)
				UxPopupInterface (create_Overwrite(ASCII_SAVE,
					fileSave,appoggio), no_grab);
			else
				UxPopupInterface (create_Overwrite(NORMAL_SAVE,
					fileSave,appoggio), no_grab);
	
			XmStringFree (Messaggio); 
			}
		}
	}
	UxFile_SaveContext = UxSaveCtx;
}

static void	activateCB_push_cancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCFile_Save           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFile_SaveContext;
	UxFile_SaveContext = UxContext =
			(_UxCFile_Save *) UxGetContext( UxWidget );
	{
	UxDestroyInterface (File_Save);
	}
	UxFile_SaveContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_File_Save()
{
	Widget	File_Save_shell;

	File_Save_shell = XtVaCreatePopupShell( "File_Save_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 624,
			XmNy, 343,
			XmNwidth, 334,
			XmNheight, 137,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "File_Save",
			XmNiconName, "File_Save",
			NULL );

	File_Save = XtVaCreateManagedWidget( "File_Save",
			xmFormWidgetClass, File_Save_shell,
			XmNheight, 137,
			XmNwidth, 334,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( File_Save, (char *) UxFile_SaveContext );

	name_save = XtVaCreateManagedWidget( "name_save",
			xmTextFieldWidgetClass, File_Save,
			XmNheight, 40,
			XmNwidth, 315,
			XmNy, 35,
			XmNx, 10,
			NULL );

	UxPutContext( name_save, (char *) UxFile_SaveContext );

	label5 = XtVaCreateManagedWidget( "label5",
			xmLabelWidgetClass, File_Save,
			RES_CONVERT( XmNlabelString, "Output file name" ),
			XmNheight, 25,
			XmNwidth, 140,
			XmNy, 10,
			XmNx, 5,
			NULL );

	UxPutContext( label5, (char *) UxFile_SaveContext );

	push_save = XtVaCreateManagedWidget( "push_save",
			xmPushButtonWidgetClass, File_Save,
			RES_CONVERT( XmNlabelString, "SAVE" ),
			XmNheight, 45,
			XmNwidth, 100,
			XmNy, 80,
			XmNx, 10,
			NULL );

	UxPutContext( push_save, (char *) UxFile_SaveContext );

	push_cancel = XtVaCreateManagedWidget( "push_cancel",
			xmPushButtonWidgetClass, File_Save,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNheight, 45,
			XmNwidth, 100,
			XmNy, 80,
			XmNx, 225,
			NULL );

	UxPutContext( push_cancel, (char *) UxFile_SaveContext );

	XtAddCallback( File_Save, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxFile_SaveContext );

	XtAddCallback( push_save, XmNactivateCallback,
			activateCB_push_save,
			(XtPointer) UxFile_SaveContext );

	XtAddCallback( push_cancel, XmNactivateCallback,
			activateCB_push_cancel,
			(XtPointer) UxFile_SaveContext );



	return ( File_Save );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_File_Save( _UxOpzione, _UxPX, _UxPY, _UxNomeFile )
	int	_UxOpzione;
	Position	_UxPX;
	Position	_UxPY;
	unsigned char	*_UxNomeFile;
{
	Widget                  rtrn;
	_UxCFile_Save           *UxContext;

	UxFile_SaveContext = UxContext =
		(_UxCFile_Save *) XtMalloc( sizeof(_UxCFile_Save) );

	Opzione = _UxOpzione;
	PX = _UxPX;
	PY = _UxPY;
	NomeFile = _UxNomeFile;
	{
		SalvaCome = Opzione;
		fileSave = XtNewString (NomeFile);
		rtrn = _Uxbuild_File_Save();

		set_something_val (File_Save, XmNx, (XtArgVal) PX+10);
		set_something_val (File_Save, XmNy, (XtArgVal) PY+10);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_File_Save( _UxOpzione, _UxPX, _UxPY, _UxNomeFile )
	int	_UxOpzione;
	Position	_UxPX;
	Position	_UxPY;
	unsigned char	*_UxNomeFile;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_File_Save( _UxOpzione, _UxPX, _UxPY, _UxNomeFile );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

