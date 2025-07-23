
/*******************************************************************************
	Menu_graf.c
	(Generated from interface file Menu_graf.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ArrowBG.h>
#include <Xm/TextF.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo Menu_graf.i
   tipo 
   release 1.10
   data 3/28/95
   reserved @(#)Menu_graf.i	1.10
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include <Xm/Xm.h>
#include <grsf22.h>
#include "menu_graf.h"

/* Widget della finestra per la funzione Extract,Cut,Shift del tempo */
extern swidget Edit_Time_Win;
extern swidget Edt_start_time;
extern swidget Edt_stop_time;
extern swidget textField_Stop;
extern swidget textField_Start;
extern swidget Titolo_edit;
extern swidget Etichetta_start;
extern swidget Etichetta_stop;
/* Widget della finestra di uscita dalla Menu_graf */
extern swidget Menu_graf_exit;
extern swidget label_output;
/* Widget della finestra per il cambio della descrizione della variabile  */
extern swidget Description;
extern swidget labelDescription;
extern swidget labelNomevar;
extern swidget textDescription;
extern swidget textNomevar;
/* Widget della finestra per il salvataggio del file */
extern swidget create_File_Save();
extern swidget name_save;
/* Widget vari  */
extern swidget create_SimulationTitle();
extern swidget create_FilterWindow();
extern swidget fileSelectionBox1;
extern swidget create_fileSelectionBox1();
extern swidget CalcWindow;
extern swidget create_CalcWindow();
extern swidget Message;
extern swidget Sampling;
extern swidget Resampling;

/* Variabile per identificazione tipo di find  */
extern int Find_sel;
/* Variabile per identificazione salvataggio effettuato */
extern int Salvato;
/* Variabile per identificazione modifiche apportate */
extern int Modifica;
/* Variabile per identificazione nome file destinatario assegnato */
extern int nome_valido;
/* Funzione per la lettura della directory di lavoro */
extern char *getcwd();
/* Identificazione del processo di grafics */
extern int pid;
/* Variabili di uso vario */
extern int Scelta;
extern char *Nomefile;
extern char Path_appo[];
extern int campioni;
extern FILE *fpAPPO,*fpDAT;

extern FILE *fpELENCO;

/* Stringhe delle etichette della MainWindow  */
XmString label_start_time,label_stop_time,label_path_f22;
XmString label_delta_campioni,label_numero_campioni,label_num_var;

char nomi_var [NUM_VAR+1][LUN_NOME+1];

int TypeCalc;                       /*  Tipo di operazione da eseguire
					 sulle var  */
int Input_Message;

int trovata,i,j;
float t_iniziale,t_finale;  	    /* Tempi della simulazione registrata
						 su f22  */
S_SEL_DATI bufdati[NUM_VAR];
S_HEAD1 header1;		    /*  Primo header letto da f22  */
S_HEAD2 header2;   		    /*  Secondo header letto da f22  */
char *s_app,nomevariabile[9];
char **simboli,*descriz;
int indice,n_last,Clip_id;
char *path_22dat,f22source[50],f22target[50],f22undo[50]; /* Path_name f22 */
XmString *x_simboli;
/* Dichiarazione variabili per uso Clipboard  */
XmString nome_clip;
char *Stringa_clip,**lista_varClip,*path_22datClip;
float delta_nuovoClip,t_inizialeClip,t_finaleClip;
int numeroClip;

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
	Widget	Uxf22_modify;
	Widget	Uxmenu1;
	Widget	UxFilePane;
	Widget	UxSeparatore1;
	Widget	UxSeparatore2;
	Widget	UxSeparatore3;
	Widget	UxEditVarPane;
	Widget	UxDescriptionItem;
	Widget	UxCutUnselItem_V;
	Widget	UxEditTimePane;
	Widget	UxResamplingItem;
	Widget	UxAxBItem;
	Widget	UxxyItem;
	Widget	UxFindPane;
	Widget	Uxform1;
	Widget	UxscrolledWindow1;
	Widget	Uxl_path_name;
	Widget	Uxl_num_var;
	Widget	Uxl_stop_time;
	Widget	Uxl1_sec;
	Widget	Uxl_start_time;
	Widget	Uxl_start_time2;
	Widget	Uxl_start_time1;
	Widget	Uxl1_sec1;
	Widget	Uxl2_sec;
	Widget	Uxlabel13;
	Widget	Uxl1_sec2;
} _UxCMenu_graf;

#define f22_modify              UxMenu_grafContext->Uxf22_modify
#define menu1                   UxMenu_grafContext->Uxmenu1
#define FilePane                UxMenu_grafContext->UxFilePane
#define Separatore1             UxMenu_grafContext->UxSeparatore1
#define Separatore2             UxMenu_grafContext->UxSeparatore2
#define Separatore3             UxMenu_grafContext->UxSeparatore3
#define EditVarPane             UxMenu_grafContext->UxEditVarPane
#define DescriptionItem         UxMenu_grafContext->UxDescriptionItem
#define CutUnselItem_V          UxMenu_grafContext->UxCutUnselItem_V
#define EditTimePane            UxMenu_grafContext->UxEditTimePane
#define ResamplingItem          UxMenu_grafContext->UxResamplingItem
#define AxBItem                 UxMenu_grafContext->UxAxBItem
#define xyItem                  UxMenu_grafContext->UxxyItem
#define FindPane                UxMenu_grafContext->UxFindPane
#define form1                   UxMenu_grafContext->Uxform1
#define scrolledWindow1         UxMenu_grafContext->UxscrolledWindow1
#define l_path_name             UxMenu_grafContext->Uxl_path_name
#define l_num_var               UxMenu_grafContext->Uxl_num_var
#define l_stop_time             UxMenu_grafContext->Uxl_stop_time
#define l1_sec                  UxMenu_grafContext->Uxl1_sec
#define l_start_time            UxMenu_grafContext->Uxl_start_time
#define l_start_time2           UxMenu_grafContext->Uxl_start_time2
#define l_start_time1           UxMenu_grafContext->Uxl_start_time1
#define l1_sec1                 UxMenu_grafContext->Uxl1_sec1
#define l2_sec                  UxMenu_grafContext->Uxl2_sec
#define label13                 UxMenu_grafContext->Uxlabel13
#define l1_sec2                 UxMenu_grafContext->Uxl1_sec2

static _UxCMenu_graf	*UxMenu_grafContext;

Widget	Menu_graf;
Widget	LoadItem;
Widget	SaveItem;
Widget	Save_asItem;
Widget	AsciiItem;
Widget	UndoItem;
Widget	ExitItem;
Widget	File_Pane;
Widget	CopyItem_V;
Widget	CutItem_V;
Widget	PasteItem_V;
Widget	EditTitleItem;
Widget	Var_Pane;
Widget	ExtractItem;
Widget	ShiftItem;
Widget	SamplingItem;
Widget	Time_Pane;
Widget	CalculationPane;
Widget	FilterItem;
Widget	Calc_Pane;
Widget	ViewPane;
Widget	GraphItem;
Widget	View_Pane;
Widget	Multiple_Find_Item;
Widget	Single_Find_Item;
Widget	Find_Pane;
Widget	num_var;
Widget	lista_var;
Widget	textField_find;
Widget	Find_Butt_Up;
Widget	Find_Butt_Down;
Widget	path_f22;
Widget	start_time;
Widget	Push_Select;
Widget	Push_Deselect;
Widget	delta_campioni;
Widget	num_campioni;
Widget	stop_time;
Widget	MAX_Int;
Widget	MIN_Int;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_Menu_graf();

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_LoadItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	XmString Messaggio;
	char stringa[100];
	Position PosX,PosY;
	 
	if (Salvato == 0)
		{
		Input_Message = 1;
		strcpy (stringa,"Changes not saved!Continue load?\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message,XmNdialogType, (XtArgVal) XmDIALOG_WARNING);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		}
	if (Salvato == 1)
		{
		fileSelectionBox1 = create_fileSelectionBox1();
		get_something (Menu_graf, XmNx, (void*) &PosX);
		get_something (Menu_graf, XmNy, (void*) &PosY);
		set_something_val (fileSelectionBox1, XmNx, (XtArgVal) PosX+10);
		set_something_val (fileSelectionBox1, XmNy, (XtArgVal) PosY+10);
		UxPopupInterface(fileSelectionBox1,no_grab);
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_SaveItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	char cdo[100];
	Position PosX,PosY;
	
	if ((nome_valido == 1) && (Modifica == 1))
		{
		strcpy (cdo,"cp ");
		strcat (cdo,f22source);
		strcat (cdo," ");
		strcat (cdo,Nomefile);
		system (cdo);
		Modifica = 0;
		}
	else
		{
		if (nome_valido != 1) 
			{
			get_something (Menu_graf, XmNx, (void*) &PosX);
			get_something (Menu_graf, XmNy, (void*) &PosY);
			UxPopupInterface (create_File_Save(NORMAL_SAVE,PosX,PosY,f22source), no_grab);
			}
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_Save_asItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	UxPopupInterface (create_File_Save(NORMAL_SAVE,PosX,PosY,f22source), no_grab);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_AsciiItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int *ListaSele,NumSele,selezione;
	XmString Messaggio;
	char stringa[100],*pathAscii;
	Position PosX,PosY;
	
	selezione = XmListGetSelectedPos (lista_var, &ListaSele, &NumSele);
	if (selezione)
		{
		if (NumSele > MAX_VAR_RECORDED)
			{
		        strcpy (stringa,"Max selectable variables superated\0");
	        	Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		        set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
		        set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		        UxPopupInterface (Message,no_grab);
		        XmStringFree (Messaggio);
			}
		else
			{
			pathAscii = SalvaInAscii (ListaSele,NumSele);
			get_something (Menu_graf, XmNx, (void*) &PosX);
			get_something (Menu_graf, XmNy, (void*) &PosY);
			UxPopupInterface (create_File_Save(ASCII_SAVE,PosX,PosY,pathAscii),
						no_grab);
			}
		}
	else
		{
	        strcpy (stringa,"Must be selected at least one item\0");
	        Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
	        set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
	        set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
	        UxPopupInterface (Message,no_grab);
	        XmStringFree (Messaggio);
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_UndoItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	char stringa[100];
	
	Uccidi_grafics (pid);
	fclose (fpAPPO);
	fclose (fpDAT);
	strcpy (stringa,"cp ");
	strcat (stringa,f22undo);
	strcat (stringa," ");
	strcat (stringa,f22source);
	strcat (stringa,"\0");
	system (stringa);
	Carica_f22 ();
	set_something_val (UndoItem, XmNsensitive, (XtArgVal) False);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_ExitItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	char stringa[100];
	XmString Messaggio;
	
	if ((Modifica == 1) && (Salvato == 0))
		{
		strcpy (stringa,"CHANGES NOT SAVED! EXIT?\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		}
	else
		{
		strcpy (stringa,"Do you really want to exit Menu Graf?\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		}
	set_something_val (label_output, XmNlabelString, (XtArgVal) Messaggio);
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	set_something_val (Menu_graf_exit, XmNx, (XtArgVal) PosX+10);
	set_something_val (Menu_graf_exit, XmNy, (XtArgVal) PosY+10);
	UxPopupInterface (Menu_graf_exit, no_grab);
	XmStringFree (Messaggio);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_CopyItem_V( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int *lista_sele,selezione,num_sele;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if (ControllaValiditaPerClip (selezione,campioni)==0)
		{
		set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) True);
		nome_clip = XmStringCreateSimple ("Mia Clipboard");
		Stringa_clip = (char *)Prepara_stringa(lista_sele,num_sele,f22source);
		ScriviClipboard (Stringa_clip,label13);
		}
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_CutItem_V( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int selezione,num_sele,*lista_sele;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if (ControllaValiditaPerClip (selezione,campioni) == 0)
		{
		set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) True);
		nome_clip = XmStringCreateSimple ("Mia Clipboard");
		Stringa_clip = (char *)Prepara_stringa(lista_sele, num_sele, f22undo);
		ScriviClipboard (Stringa_clip,label13);
		F22_CutVar (lista_sele,num_sele);
		}
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_PasteItem_V( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	LeggiClipboard (&Stringa_clip,label13);
	Scomponi_stringa (Stringa_clip);
	F22_PasteVar ();
	set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_DescriptionItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	XmString Messaggio;
	int selezione,*lista_sele,num_sele;
	char stringa[100];
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if ((selezione == 0) || (num_sele != 1))
	{
		strcpy (stringa,"Must be selected one item !\0");
		Messaggio = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
	}else
	{
		set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
		descriz = (char *)XtMalloc(LUN_SIMB+1);
	 
		indice = lista_sele[0] - 1;
		j=0;
		for (i=8;i<LUN_SIMB+1;i++)
		{
			descriz [j] = simboli[indice][i];
			j++;
		}
		strcpy (nomevariabile, nomi_var[indice]);
		Azzera_text (textDescription);
		XmTextFieldSetString (textDescription,descriz);
		XmTextFieldSetString (textNomevar, nomevariabile);
		Messaggio = XmStringCreate (nomevariabile, XmSTRING_DEFAULT_CHARSET);
		set_something_val (labelNomevar, XmNlabelString, (XtArgVal) Messaggio);
		Messaggio = XmStringCreate (descriz, XmSTRING_DEFAULT_CHARSET);
		set_something_val (labelDescription, XmNlabelString, (XtArgVal) Messaggio);
		UxPopupInterface (Description, no_grab);
	}
	XmStringFree (Messaggio);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_CutUnselItem_V( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int *lista_sele,num_sele,selezione;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	F22_CutVarUnsel (lista_sele,num_sele);
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_EditTitleItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	UxPopupInterface (create_SimulationTitle(), no_grab);
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_ExtractItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	char stringa[100];
	XmString AppoStringa;
	
	strcpy (stringa,"EXTRACT TIME\0");
	AppoStringa = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
	set_something_val (Titolo_edit, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	strcpy (stringa,"New start time\0");
	AppoStringa = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
	set_something_val (Etichetta_start, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	strcpy (stringa,"New stop time\0");
	AppoStringa = XmStringCreate (stringa,XmSTRING_DEFAULT_CHARSET);
	set_something_val (Etichetta_stop, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	set_something_val (textField_Start, XmNsensitive, (XtArgVal) True);
	set_something_val (textField_Stop, XmNsensitive, (XtArgVal) True);
	set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
	
	set_something_val (Edt_start_time, XmNlabelString, (XtArgVal) label_start_time);
	set_something_val (Edt_stop_time, XmNlabelString, (XtArgVal) label_stop_time);
	Azzera_text (textField_Start);
	Azzera_text (textField_Stop);
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	set_something_val (Edit_Time_Win, XmNx, (XtArgVal) PosX+10);
	set_something_val (Edit_Time_Win, XmNy, (XtArgVal) PosY+10);
	UxPopupInterface (Edit_Time_Win,no_grab);
	Scelta = EXTRACT_TIME;
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_ShiftItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	char stringa[100];
	XmString AppoStringa;
	
	strcpy (stringa, "SHIFT TIME\0");
	AppoStringa = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
	set_something_val (Titolo_edit, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	strcpy (stringa, "Shift time value\0");
	AppoStringa = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
	set_something_val (Etichetta_start, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	strcpy (stringa, "\0");
	AppoStringa = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
	set_something_val (Etichetta_stop, XmNlabelString, (XtArgVal) AppoStringa);
	XmStringFree (AppoStringa);
	
	set_something_val (textField_Start, XmNsensitive, (XtArgVal) True);
	set_something_val (textField_Stop, XmNsensitive, (XtArgVal) False);
	set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
	
	set_something_val (Edt_start_time, XmNlabelString, (XtArgVal) label_start_time);
	set_something_val (Edt_stop_time, XmNlabelString, (XtArgVal) label_stop_time);
	Azzera_text (textField_Start);
	Azzera_text (textField_Stop);
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	set_something_val (Edit_Time_Win, XmNx, (XtArgVal) PosX+10);
	set_something_val (Edit_Time_Win, XmNy, (XtArgVal) PosY+10);
	UxPopupInterface (Edit_Time_Win, no_grab);
	Scelta = SHIFT_TIME;
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_SamplingItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	set_something_val (Sampling, XmNx, (XtArgVal) PosX+10);
	set_something_val (Sampling, XmNy, (XtArgVal) PosY+10);
	set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
	UxPopupInterface (Sampling, no_grab);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_ResamplingItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position PosX,PosY;
	
	get_something (Menu_graf, XmNx, (void*) &PosX);
	get_something (Menu_graf, XmNy, (void*) &PosY);
	set_something_val (Resampling, XmNx, (XtArgVal) PosX+10);
	set_something_val (Resampling, XmNy, (XtArgVal) PosY+10);
	set_something_val (PasteItem_V, XmNsensitive, (XtArgVal) False);
	UxPopupInterface (Resampling, no_grab);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_FilterItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Position px,py;
	int selezione,*lista_sele,num_sele;
	char stringa[100];
	XmString Messaggio;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if (num_sele != 1)
		{
		strcpy (stringa,"Must be selected one item!\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
		{
		get_something (Menu_graf, XmNx, (void*) &px);
		get_something (Menu_graf, XmNy, (void*) &py);
		UxPopupInterface (create_FilterWindow(px,py,(lista_sele[0]-1)),
					 no_grab);
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_AxBItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int selezione,*lista_sele,num_sele;
	char stringa[100];
	XmString Messaggio;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if (num_sele != 1)
		{
		strcpy (stringa,"Must be selected one item!\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
		{
		TypeCalc = ELAB_LINEARE;
		CalcWindow = create_CalcWindow(lista_sele);
		UxPopupInterface (CalcWindow, no_grab);
		}
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_xyItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int selezione,*lista_sele,num_sele;
	char stringa[100];
	XmString Messaggio;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	if (num_sele != 2)
		{
		strcpy (stringa,"Must be selected two item!\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_ERROR);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
		{
		TypeCalc = SOMMA_ALGEBRICA;
		CalcWindow = create_CalcWindow (lista_sele);
		UxPopupInterface (CalcWindow, no_grab);
		}
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_GraphItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	extern char nomi_var[NUM_VAR+1][LUN_NOME+1];
	
	int indice,indice2,indice3,indice4;
	int selezione,*lista_sele,num_sele;
	int i;
	
	rout_appo();
	 
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);	
	
	if (selezione == 0) num_sele = 0;
	switch (num_sele)
	{
		case 0:
			pid = attiva_prog_par ("graphics",path_22dat,nomi_var[0],0);
			break;
		case 1: 
			indice = lista_sele[0] - 1;
			pid = attiva_prog_par ("graphics",path_22dat,
				nomi_var[indice],0);
			break;
		case 2:
			indice = lista_sele[0] - 1;
			indice2 = lista_sele [1] - 1;
			pid = attiva_prog_par ("graphics",path_22dat,
				nomi_var[indice],
				nomi_var[indice2],0);
			break;
		case 3:
			indice = lista_sele[0] - 1;
			indice2 = lista_sele[1] - 1;
			indice3 = lista_sele[2] - 1;
			pid = attiva_prog_par ("graphics",path_22dat,
				nomi_var[indice],
				nomi_var[indice2],
				nomi_var[indice3],0);
			break;
		default:
			indice = lista_sele[0] - 1;
			indice2 = lista_sele[1] - 1;
			indice3 = lista_sele[2] - 1;
			indice4 = lista_sele[3] - 1;
			pid = attiva_prog_par ("graphics",path_22dat,
				nomi_var[indice],
				nomi_var[indice2],
				nomi_var[indice3],
				nomi_var[indice4],0);
			break;
	}
	XtFree (lista_sele);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	valueChangedCB_Multiple_Find_Item( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Find_sel = MULTIPLO;
	set_something_val (Multiple_Find_Item, XmNset, (XtArgVal) True);
	set_something_val (Single_Find_Item, XmNset, (XtArgVal) False);
	set_something_val (lista_var, XmNselectionPolicy, (XtArgVal) XmMULTIPLE_SELECT);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	valueChangedCB_Single_Find_Item( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	Find_sel = SINGOLO;
	set_something_val (Single_Find_Item, XmNset, (XtArgVal) True);
	set_something_val (Multiple_Find_Item, XmNset, (XtArgVal) False);
	set_something_val (lista_var, XmNselectionPolicy, (XtArgVal) XmEXTENDED_SELECT);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	multipleSelectionCB_lista_var( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	valueChangedCB_textField_find( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	char *controllo;
	
	controllo = XmTextFieldGetString (textField_find);
	if ((controllo[0] != '\0') && (controllo[0] != '\n'))
		{
		set_something_val (Find_Butt_Up, XmNsensitive, (XtArgVal) True);
		set_something_val (Find_Butt_Down, XmNsensitive, (XtArgVal) True);
		}
	else
		{	
		set_something_val (Find_Butt_Up, XmNsensitive, (XtArgVal) False);
		set_something_val (Find_Butt_Down, XmNsensitive, (XtArgVal) False);
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_Find_Butt_Up( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int Valore;
	char *Trovare,stringa[100];
	XmString Messaggio;
	
	Trovare = XmTextFieldGetString (textField_find);
	Valore = Cerca_su (Trovare);
	if (trovata == 0)
		{
		strcpy (stringa,"String not found\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message,no_grab);
		XmStringFree (Messaggio);
		}
	else	
		{
		XmListSetPos (lista_var, Valore);
		XmListSelectPos (lista_var, Valore, True);
		}
	
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_Find_Butt_Down( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int Valore;
	char stringa[100],*Trovare;
	XmString Messaggio;
	
	Trovare = XmTextFieldGetString (textField_find);
	Valore = Cerca_giu (Trovare);
	if (trovata == 0)
		{
		strcpy (stringa,"String not found\0");
		Messaggio = XmStringCreate (stringa, XmSTRING_DEFAULT_CHARSET);
		set_something_val (Message, XmNdialogType, (XtArgVal) XmDIALOG_INFORMATION);
		set_something_val (Message, XmNmessageString, (XtArgVal) Messaggio);
		UxPopupInterface (Message, no_grab);
		XmStringFree (Messaggio);
		}
	else
	 	{
		XmListSetPos (lista_var,Valore);
		XmListSelectPos (lista_var, Valore, True);
		}
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_Push_Select( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	{
	int j,i,selezione,*lista_sele,num_sele;
	
	selezione = XmListGetSelectedPos (lista_var, &lista_sele, &num_sele);
	set_something_val (lista_var, XmNselectionPolicy, (XtArgVal) XmMULTIPLE_SELECT);
	if (num_sele != header1.nvar)
		{
		if (selezione)
			{
			for (i=1;i<=header1.nvar+1;i++)
				{
				for (j=0;j<num_sele;j++)
				if (lista_sele[j]!=i) XmListSelectPos (lista_var,i,1);
				}
			}
		else
			{
			for (i=1;i<=header1.nvar+1;i++)
			 	XmListSelectPos (lista_var,i,1);
			}
		}
	set_something_val (lista_var, XmNselectionPolicy, (XtArgVal) XmEXTENDED_SELECT);
	}
	UxMenu_grafContext = UxSaveCtx;
}

static void	activateCB_Push_Deselect( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCMenu_graf           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMenu_grafContext;
	UxMenu_grafContext = UxContext =
			(_UxCMenu_graf *) UxGetContext( UxWidget );
	XmListDeselectAllItems (lista_var);
	UxMenu_grafContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Menu_graf()
{
	Widget	FilePane_shell;
	Widget	EditVarPane_shell;
	Widget	EditTimePane_shell;
	Widget	CalculationPane_shell;
	Widget	ViewPane_shell;
	Widget	FindPane_shell;

	Menu_graf = XtVaCreatePopupShell( "Menu_graf",
			topLevelShellWidgetClass, UxTopLevel,
			XmNminWidth, 500,
			XmNminHeight, 400,
			XmNheight, 645,
			XmNwidth, 542,
			XmNy, 0,
			XmNx, 32,
			NULL );

	UxPutContext( Menu_graf, (char *) UxMenu_grafContext );

	f22_modify = XtVaCreateManagedWidget( "f22_modify",
			xmMainWindowWidgetClass, Menu_graf,
			XmNy, 0,
			XmNx, 200,
			XmNheight, 645,
			XmNwidth, 542,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( f22_modify, (char *) UxMenu_grafContext );

	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass, f22_modify,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu1, (char *) UxMenu_grafContext );

	FilePane_shell = XtVaCreatePopupShell ("FilePane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FilePane = XtVaCreateWidget( "FilePane",
			xmRowColumnWidgetClass, FilePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( FilePane, (char *) UxMenu_grafContext );

	LoadItem = XtVaCreateManagedWidget( "LoadItem",
			xmPushButtonWidgetClass, FilePane,
			RES_CONVERT( XmNmnemonic, "L" ),
			RES_CONVERT( XmNlabelString, "Load.." ),
			NULL );

	UxPutContext( LoadItem, (char *) UxMenu_grafContext );

	SaveItem = XtVaCreateManagedWidget( "SaveItem",
			xmPushButtonWidgetClass, FilePane,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Save" ),
			NULL );

	UxPutContext( SaveItem, (char *) UxMenu_grafContext );

	Save_asItem = XtVaCreateManagedWidget( "Save_asItem",
			xmPushButtonWidgetClass, FilePane,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNmnemonic, "A" ),
			RES_CONVERT( XmNlabelString, "Save As.." ),
			NULL );

	UxPutContext( Save_asItem, (char *) UxMenu_grafContext );

	Separatore1 = XtVaCreateManagedWidget( "Separatore1",
			xmSeparatorWidgetClass, FilePane,
			NULL );

	UxPutContext( Separatore1, (char *) UxMenu_grafContext );

	AsciiItem = XtVaCreateManagedWidget( "AsciiItem",
			xmPushButtonWidgetClass, FilePane,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNmnemonic, "s" ),
			RES_CONVERT( XmNlabelString, "Ascii save.." ),
			NULL );

	UxPutContext( AsciiItem, (char *) UxMenu_grafContext );

	Separatore2 = XtVaCreateManagedWidget( "Separatore2",
			xmSeparatorWidgetClass, FilePane,
			NULL );

	UxPutContext( Separatore2, (char *) UxMenu_grafContext );

	UndoItem = XtVaCreateManagedWidget( "UndoItem",
			xmPushButtonWidgetClass, FilePane,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNmnemonic, "U" ),
			RES_CONVERT( XmNlabelString, "Undo" ),
			NULL );

	UxPutContext( UndoItem, (char *) UxMenu_grafContext );

	Separatore3 = XtVaCreateManagedWidget( "Separatore3",
			xmSeparatorWidgetClass, FilePane,
			NULL );

	UxPutContext( Separatore3, (char *) UxMenu_grafContext );

	ExitItem = XtVaCreateManagedWidget( "ExitItem",
			xmPushButtonWidgetClass, FilePane,
			RES_CONVERT( XmNmnemonic, "x" ),
			RES_CONVERT( XmNlabelString, "Exit" ),
			NULL );

	UxPutContext( ExitItem, (char *) UxMenu_grafContext );

	File_Pane = XtVaCreateManagedWidget( "File_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, FilePane,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( File_Pane, (char *) UxMenu_grafContext );

	EditVarPane_shell = XtVaCreatePopupShell ("EditVarPane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	EditVarPane = XtVaCreateWidget( "EditVarPane",
			xmRowColumnWidgetClass, EditVarPane_shell,
			XmNsensitive, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( EditVarPane, (char *) UxMenu_grafContext );

	CopyItem_V = XtVaCreateManagedWidget( "CopyItem_V",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );

	UxPutContext( CopyItem_V, (char *) UxMenu_grafContext );

	CutItem_V = XtVaCreateManagedWidget( "CutItem_V",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "u" ),
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );

	UxPutContext( CutItem_V, (char *) UxMenu_grafContext );

	PasteItem_V = XtVaCreateManagedWidget( "PasteItem_V",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "P" ),
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );

	UxPutContext( PasteItem_V, (char *) UxMenu_grafContext );

	DescriptionItem = XtVaCreateManagedWidget( "DescriptionItem",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Description.." ),
			NULL );

	UxPutContext( DescriptionItem, (char *) UxMenu_grafContext );

	CutUnselItem_V = XtVaCreateManagedWidget( "CutUnselItem_V",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "U" ),
			RES_CONVERT( XmNlabelString, "Cut Unselected" ),
			NULL );

	UxPutContext( CutUnselItem_V, (char *) UxMenu_grafContext );

	EditTitleItem = XtVaCreateManagedWidget( "EditTitleItem",
			xmPushButtonWidgetClass, EditVarPane,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Simulation Title" ),
			NULL );

	UxPutContext( EditTitleItem, (char *) UxMenu_grafContext );

	Var_Pane = XtVaCreateManagedWidget( "Var_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsensitive, FALSE,
			XmNsubMenuId, EditVarPane,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Edit Variables" ),
			NULL );

	UxPutContext( Var_Pane, (char *) UxMenu_grafContext );

	EditTimePane_shell = XtVaCreatePopupShell ("EditTimePane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	EditTimePane = XtVaCreateWidget( "EditTimePane",
			xmRowColumnWidgetClass, EditTimePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( EditTimePane, (char *) UxMenu_grafContext );

	ExtractItem = XtVaCreateManagedWidget( "ExtractItem",
			xmPushButtonWidgetClass, EditTimePane,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Extract.." ),
			NULL );

	UxPutContext( ExtractItem, (char *) UxMenu_grafContext );

	ShiftItem = XtVaCreateManagedWidget( "ShiftItem",
			xmPushButtonWidgetClass, EditTimePane,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Shift.." ),
			NULL );

	UxPutContext( ShiftItem, (char *) UxMenu_grafContext );

	SamplingItem = XtVaCreateManagedWidget( "SamplingItem",
			xmPushButtonWidgetClass, EditTimePane,
			RES_CONVERT( XmNmnemonic, "a" ),
			RES_CONVERT( XmNlabelString, "Sampling.." ),
			NULL );

	UxPutContext( SamplingItem, (char *) UxMenu_grafContext );

	ResamplingItem = XtVaCreateManagedWidget( "ResamplingItem",
			xmPushButtonWidgetClass, EditTimePane,
			RES_CONVERT( XmNmnemonic, "R" ),
			RES_CONVERT( XmNlabelString, "Resampling.." ),
			NULL );

	UxPutContext( ResamplingItem, (char *) UxMenu_grafContext );

	Time_Pane = XtVaCreateManagedWidget( "Time_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsensitive, FALSE,
			XmNsubMenuId, EditTimePane,
			RES_CONVERT( XmNmnemonic, "d" ),
			RES_CONVERT( XmNlabelString, "Edit Time" ),
			NULL );

	UxPutContext( Time_Pane, (char *) UxMenu_grafContext );

	CalculationPane_shell = XtVaCreatePopupShell ("CalculationPane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	CalculationPane = XtVaCreateWidget( "CalculationPane",
			xmRowColumnWidgetClass, CalculationPane_shell,
			XmNsensitive, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( CalculationPane, (char *) UxMenu_grafContext );

	FilterItem = XtVaCreateManagedWidget( "FilterItem",
			xmPushButtonWidgetClass, CalculationPane,
			XmNancestorSensitive, TRUE,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "Filter.." ),
			NULL );

	UxPutContext( FilterItem, (char *) UxMenu_grafContext );

	AxBItem = XtVaCreateManagedWidget( "AxBItem",
			xmPushButtonWidgetClass, CalculationPane,
			RES_CONVERT( XmNmnemonic, "A" ),
			RES_CONVERT( XmNlabelString, "Ax+B.." ),
			NULL );

	UxPutContext( AxBItem, (char *) UxMenu_grafContext );

	xyItem = XtVaCreateManagedWidget( "xyItem",
			xmPushButtonWidgetClass, CalculationPane,
			RES_CONVERT( XmNmnemonic, "x" ),
			RES_CONVERT( XmNlabelString, "x+y.." ),
			NULL );

	UxPutContext( xyItem, (char *) UxMenu_grafContext );

	Calc_Pane = XtVaCreateManagedWidget( "Calc_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsensitive, FALSE,
			XmNsubMenuId, CalculationPane,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Calc" ),
			NULL );

	UxPutContext( Calc_Pane, (char *) UxMenu_grafContext );

	ViewPane_shell = XtVaCreatePopupShell ("ViewPane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	ViewPane = XtVaCreateWidget( "ViewPane",
			xmRowColumnWidgetClass, ViewPane_shell,
			XmNsensitive, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( ViewPane, (char *) UxMenu_grafContext );

	GraphItem = XtVaCreateManagedWidget( "GraphItem",
			xmPushButtonWidgetClass, ViewPane,
			RES_CONVERT( XmNmnemonic, "G" ),
			RES_CONVERT( XmNlabelString, "Graphics.." ),
			NULL );

	UxPutContext( GraphItem, (char *) UxMenu_grafContext );

	View_Pane = XtVaCreateManagedWidget( "View_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsensitive, TRUE,
			XmNsubMenuId, ViewPane,
			RES_CONVERT( XmNmnemonic, "V" ),
			RES_CONVERT( XmNlabelString, "View" ),
			NULL );

	UxPutContext( View_Pane, (char *) UxMenu_grafContext );

	FindPane_shell = XtVaCreatePopupShell ("FindPane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FindPane = XtVaCreateWidget( "FindPane",
			xmRowColumnWidgetClass, FindPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( FindPane, (char *) UxMenu_grafContext );

	Multiple_Find_Item = XtVaCreateManagedWidget( "Multiple_Find_Item",
			xmToggleButtonWidgetClass, FindPane,
			RES_CONVERT( XmNmnemonic, "M" ),
			RES_CONVERT( XmNlabelString, "Multiple" ),
			NULL );

	UxPutContext( Multiple_Find_Item, (char *) UxMenu_grafContext );

	Single_Find_Item = XtVaCreateManagedWidget( "Single_Find_Item",
			xmToggleButtonWidgetClass, FindPane,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Single" ),
			NULL );

	UxPutContext( Single_Find_Item, (char *) UxMenu_grafContext );

	Find_Pane = XtVaCreateManagedWidget( "Find_Pane",
			xmCascadeButtonWidgetClass, menu1,
			XmNsensitive, FALSE,
			XmNsubMenuId, FindPane,
			RES_CONVERT( XmNmnemonic, "i" ),
			RES_CONVERT( XmNlabelString, "Find" ),
			NULL );

	UxPutContext( Find_Pane, (char *) UxMenu_grafContext );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, f22_modify,
			XmNx, 0,
			XmNy, 15,
			XmNnavigationType, XmTAB_GROUP,
			XmNhorizontalSpacing, 0,
			XmNnoResize, FALSE,
			XmNresizePolicy, XmRESIZE_ANY,
			NULL );

	UxPutContext( form1, (char *) UxMenu_grafContext );

	num_var = XtVaCreateManagedWidget( "num_var",
			xmLabelWidgetClass, form1,
			XmNtopOffset, 300,
			XmNrightOffset, 200,
			XmNrightAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, TRUE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 170,
			XmNx, 280,
			NULL );

	UxPutContext( num_var, (char *) UxMenu_grafContext );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, form1,
			XmNtopWidget, num_var,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 70,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftOffset, 25,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightPosition, 50,
			XmNrightOffset, 25,
			XmNrightAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 200,
			XmNx, 30,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxMenu_grafContext );

	lista_var = XtVaCreateManagedWidget( "lista_var",
			xmListWidgetClass, scrolledWindow1,
			XmNselectionPolicy, XmEXTENDED_SELECT,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			XmNlistSizePolicy, XmCONSTANT,
			XmNheight, 146,
			XmNwidth, 473,
			NULL );

	UxPutContext( lista_var, (char *) UxMenu_grafContext );

	textField_find = XtVaCreateManagedWidget( "textField_find",
			xmTextFieldWidgetClass, form1,
			XmNsensitive, FALSE,
			XmNmaxLength, 20,
			XmNcolumns, 8,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 35,
			XmNwidth, 190,
			XmNy, 530,
			XmNx, 25,
			NULL );

	UxPutContext( textField_find, (char *) UxMenu_grafContext );

	Find_Butt_Up = XtVaCreateManagedWidget( "Find_Butt_Up",
			xmArrowButtonGadgetClass, form1,
			XmNsensitive, FALSE,
			XmNresizable, FALSE,
			XmNleftPosition, 10,
			XmNleftWidget, textField_find,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNwidth, 35,
			XmNy, 625,
			XmNx, 270,
			NULL );

	UxPutContext( Find_Butt_Up, (char *) UxMenu_grafContext );

	Find_Butt_Down = XtVaCreateManagedWidget( "Find_Butt_Down",
			xmArrowButtonGadgetClass, form1,
			XmNsensitive, FALSE,
			XmNancestorSensitive, TRUE,
			XmNarrowDirection, XmARROW_DOWN,
			XmNleftWidget, Find_Butt_Up,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNwidth, 35,
			XmNy, 625,
			XmNx, 325,
			NULL );

	UxPutContext( Find_Butt_Down, (char *) UxMenu_grafContext );

	l_path_name = XtVaCreateManagedWidget( "l_path_name",
			xmLabelWidgetClass, form1,
			XmNleftOffset, 30,
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightWidget, NULL,
			XmNrightOffset, 0,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Input file:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 15,
			XmNx, 40,
			NULL );

	UxPutContext( l_path_name, (char *) UxMenu_grafContext );

	path_f22 = XtVaCreateManagedWidget( "path_f22",
			xmLabelWidgetClass, form1,
			XmNleftWidget, l_path_name,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 72,
			XmNrightAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 375,
			XmNy, 10,
			XmNx, 130,
			NULL );

	UxPutContext( path_f22, (char *) UxMenu_grafContext );

	l_num_var = XtVaCreateManagedWidget( "l_num_var",
			xmLabelWidgetClass, form1,
			XmNtopOffset, 200,
			XmNrightWidget, num_var,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Variables number:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 135,
			XmNy, 170,
			XmNx, 130,
			NULL );

	UxPutContext( l_num_var, (char *) UxMenu_grafContext );

	l_stop_time = XtVaCreateManagedWidget( "l_stop_time",
			xmLabelWidgetClass, form1,
			XmNleftOffset, 0,
			XmNresizable, FALSE,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrightWidget, NULL,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Stop time:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 85,
			XmNy, 55,
			XmNx, 310,
			NULL );

	UxPutContext( l_stop_time, (char *) UxMenu_grafContext );

	l1_sec = XtVaCreateManagedWidget( "l1_sec",
			xmLabelWidgetClass, form1,
			XmNrightWidget, NULL,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "sec." ),
			XmNfontList, UxConvertFontList( "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1" ),
			XmNheight, 25,
			XmNwidth, 40,
			XmNy, 55,
			XmNx, 210,
			NULL );

	UxPutContext( l1_sec, (char *) UxMenu_grafContext );

	start_time = XtVaCreateManagedWidget( "start_time",
			xmLabelWidgetClass, form1,
			XmNtopOffset, 45,
			XmNrightWidget, l1_sec,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 50,
			XmNx, 150,
			NULL );

	UxPutContext( start_time, (char *) UxMenu_grafContext );

	l_start_time = XtVaCreateManagedWidget( "l_start_time",
			xmLabelWidgetClass, form1,
			XmNtopOffset, 50,
			XmNleftOffset, 40,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightWidget, start_time,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Start time:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 70,
			XmNy, 55,
			XmNx, 45,
			NULL );

	UxPutContext( l_start_time, (char *) UxMenu_grafContext );

	Push_Select = XtVaCreateManagedWidget( "Push_Select",
			xmPushButtonWidgetClass, form1,
			XmNleftWidget, Find_Butt_Down,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNlabelString, "Select all" ),
			XmNheight, 35,
			XmNwidth, 90,
			XmNy, 560,
			XmNx, 315,
			NULL );

	UxPutContext( Push_Select, (char *) UxMenu_grafContext );

	Push_Deselect = XtVaCreateManagedWidget( "Push_Deselect",
			xmPushButtonWidgetClass, form1,
			XmNbottomOffset, 15,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftWidget, Push_Select,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNlabelString, "Deselect all" ),
			XmNheight, 35,
			XmNwidth, 90,
			XmNy, 560,
			XmNx, 415,
			NULL );

	UxPutContext( Push_Deselect, (char *) UxMenu_grafContext );

	delta_campioni = XtVaCreateManagedWidget( "delta_campioni",
			xmLabelWidgetClass, form1,
			XmNleftOffset, 120,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 90,
			XmNx, 135,
			NULL );

	UxPutContext( delta_campioni, (char *) UxMenu_grafContext );

	l_start_time2 = XtVaCreateManagedWidget( "l_start_time2",
			xmLabelWidgetClass, form1,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Sampl. Num.:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 90,
			XmNy, 95,
			XmNx, 305,
			NULL );

	UxPutContext( l_start_time2, (char *) UxMenu_grafContext );

	num_campioni = XtVaCreateManagedWidget( "num_campioni",
			xmLabelWidgetClass, form1,
			XmNrightOffset, 72,
			XmNrightAttachment, XmATTACH_FORM,
			XmNresizable, FALSE,
			XmNleftWidget, l_start_time2,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 90,
			XmNx, 405,
			NULL );

	UxPutContext( num_campioni, (char *) UxMenu_grafContext );

	l_start_time1 = XtVaCreateManagedWidget( "l_start_time1",
			xmLabelWidgetClass, form1,
			XmNrightWidget, delta_campioni,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 30,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "Sampl. Int.:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 95,
			XmNy, 95,
			XmNx, 30,
			NULL );

	UxPutContext( l_start_time1, (char *) UxMenu_grafContext );

	l1_sec1 = XtVaCreateManagedWidget( "l1_sec1",
			xmLabelWidgetClass, form1,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "sec." ),
			XmNfontList, UxConvertFontList( "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1" ),
			XmNheight, 25,
			XmNwidth, 40,
			XmNy, 95,
			XmNx, 210,
			NULL );

	UxPutContext( l1_sec1, (char *) UxMenu_grafContext );

	stop_time = XtVaCreateManagedWidget( "stop_time",
			xmLabelWidgetClass, form1,
			XmNleftWidget, l_stop_time,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightWidget, NULL,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 50,
			XmNx, 405,
			NULL );

	UxPutContext( stop_time, (char *) UxMenu_grafContext );

	l2_sec = XtVaCreateManagedWidget( "l2_sec",
			xmLabelWidgetClass, form1,
			XmNresizable, FALSE,
			XmNleftWidget, stop_time,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "sec." ),
			XmNfontList, UxConvertFontList( "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1" ),
			XmNheight, 25,
			XmNwidth, 37,
			XmNy, 55,
			XmNx, 485,
			NULL );

	UxPutContext( l2_sec, (char *) UxMenu_grafContext );

	MAX_Int = XtVaCreateManagedWidget( "MAX_Int",
			xmLabelWidgetClass, form1,
			XmNrightOffset, 120,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, TRUE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 130,
			XmNx, 350,
			NULL );

	UxPutContext( MAX_Int, (char *) UxMenu_grafContext );

	MIN_Int = XtVaCreateManagedWidget( "MIN_Int",
			xmLabelWidgetClass, form1,
			XmNrightWidget, MAX_Int,
			XmNrightOffset, 15,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_END,
			RES_CONVERT( XmNbottomShadowColor, "LightSlateGrey" ),
			XmNrecomputeSize, TRUE,
			RES_CONVERT( XmNlabelString, "" ),
			XmNfontList, UxConvertFontList( "fixed" ),
			RES_CONVERT( XmNbackground, "#a8afc6" ),
			XmNheight, 25,
			XmNwidth, 65,
			XmNy, 130,
			XmNx, 290,
			NULL );

	UxPutContext( MIN_Int, (char *) UxMenu_grafContext );

	label13 = XtVaCreateManagedWidget( "label13",
			xmLabelWidgetClass, form1,
			XmNrightWidget, MIN_Int,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNrecomputeSize, FALSE,
			RES_CONVERT( XmNlabelString, "MIN/MAX Samples Interval:" ),
			XmNalignment, XmALIGNMENT_END,
			XmNheight, 25,
			XmNwidth, 175,
			XmNy, 130,
			XmNx, 95,
			NULL );

	UxPutContext( label13, (char *) UxMenu_grafContext );

	l1_sec2 = XtVaCreateManagedWidget( "l1_sec2",
			xmLabelWidgetClass, form1,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "sec." ),
			XmNfontList, UxConvertFontList( "-adobe-times-medium-r-normal--14-140-75-75-p-74-iso8859-1" ),
			XmNheight, 25,
			XmNwidth, 40,
			XmNy, 130,
			XmNx, 430,
			NULL );

	UxPutContext( l1_sec2, (char *) UxMenu_grafContext );

	XtAddCallback( Menu_graf, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( LoadItem, XmNactivateCallback,
			activateCB_LoadItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( SaveItem, XmNactivateCallback,
			activateCB_SaveItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Save_asItem, XmNactivateCallback,
			activateCB_Save_asItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( AsciiItem, XmNactivateCallback,
			activateCB_AsciiItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( UndoItem, XmNactivateCallback,
			activateCB_UndoItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( ExitItem, XmNactivateCallback,
			activateCB_ExitItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( CopyItem_V, XmNactivateCallback,
			activateCB_CopyItem_V,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( CutItem_V, XmNactivateCallback,
			activateCB_CutItem_V,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( PasteItem_V, XmNactivateCallback,
			activateCB_PasteItem_V,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( DescriptionItem, XmNactivateCallback,
			activateCB_DescriptionItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( CutUnselItem_V, XmNactivateCallback,
			activateCB_CutUnselItem_V,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( EditTitleItem, XmNactivateCallback,
			activateCB_EditTitleItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( ExtractItem, XmNactivateCallback,
			activateCB_ExtractItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( ShiftItem, XmNactivateCallback,
			activateCB_ShiftItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( SamplingItem, XmNactivateCallback,
			activateCB_SamplingItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( ResamplingItem, XmNactivateCallback,
			activateCB_ResamplingItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( FilterItem, XmNactivateCallback,
			activateCB_FilterItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( AxBItem, XmNactivateCallback,
			activateCB_AxBItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( xyItem, XmNactivateCallback,
			activateCB_xyItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( GraphItem, XmNactivateCallback,
			activateCB_GraphItem,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Multiple_Find_Item, XmNvalueChangedCallback,
			valueChangedCB_Multiple_Find_Item,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Single_Find_Item, XmNvalueChangedCallback,
			valueChangedCB_Single_Find_Item,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( lista_var, XmNmultipleSelectionCallback,
			multipleSelectionCB_lista_var,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( textField_find, XmNvalueChangedCallback,
			valueChangedCB_textField_find,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Find_Butt_Up, XmNactivateCallback,
			activateCB_Find_Butt_Up,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Find_Butt_Down, XmNactivateCallback,
			activateCB_Find_Butt_Down,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Push_Select, XmNactivateCallback,
			activateCB_Push_Select,
			(XtPointer) UxMenu_grafContext );

	XtAddCallback( Push_Deselect, XmNactivateCallback,
			activateCB_Push_Deselect,
			(XtPointer) UxMenu_grafContext );


	XmMainWindowSetAreas( f22_modify, menu1, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, form1 );

	return ( Menu_graf );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_Menu_graf()
{
	Widget                  rtrn;
	_UxCMenu_graf           *UxContext;

	UxMenu_grafContext = UxContext =
		(_UxCMenu_graf *) XtMalloc( sizeof(_UxCMenu_graf) );

	rtrn = _Uxbuild_Menu_graf();

	return(rtrn);
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_Menu_graf()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_Menu_graf();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

