
/*******************************************************************************
       Resource.c
       (Generated from interface file Resource.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo Resource.i
   tipo 
   release 5.2
   data 2/13/96
   reserved @(#)Resource.i	5.2
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <ctype.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xl/Xl.h>
/*
#include <Xl/XlCoreP.h>
*/
#include <Xl/XlIconReg.h>
#include <Xl/XlPictograph.h>
#include <Xl/XlOperableKeys.h>
#include <Xl/XlPixvar.h>
#include <Ol/OlDatabaseTopologia.h>
#include <Ol/OlForm.h>
#include "res_edit.h"
#include "config.h"

extern Display *UxDisplay;
extern swidget create_FontsViewer();
extern swidget create_ColorViewer();
extern swidget create_AnimateIconSelection();
extern swidget create_VariableEditor();
extern swidget create_DatabaseSelection();
extern swidget create_FormSelection();
extern swidget create_ValueSelection();
extern swidget create_VariablesSelection(); 
extern char *Gruppi_resource[];
extern Widget GetPadre();
extern int Usato();
extern char *WidGetName();
extern Display *GetDisplay();
extern WidgetClass GetClass();

void CreaRowColumnRisorse ( );
int EseguiLoadResources();
void set_connect(Widget,int);
Widget XlGetSelectedWidget (Widget );


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
	Widget	UxResource;
	Widget	UxmainWindow1;
	Widget	UxpanedWindow1;
	Widget	Uxform1;
	Widget	UxscrolledWindow1;
	Widget	UxscrolledList1;
	Widget	Uxlabel1;
	Widget	Uxform2;
	Widget	UxscrolledWindow2;
	Widget	UxWidres_rc;
	Widget	UxGeometric_RC;
	Widget	UxSpecific_RC;
	Widget	UxColor_RC;
	Widget	UxVarCC1_rc;
	Widget	UxVarCC2_rc;
	Widget	UxVarCC3_rc;
	Widget	UxVarCC4_rc;
	Widget	UxGruppiPane;
	Widget	Uxmenu2;
	Widget	UxpushButton1;
	Widget	UxpushButton15;
	Widget	Uxmenu1;
	Widget	UxFilePane;
	Widget	UxLoadItem;
	Widget	UxResetItem;
	Widget	UxCloseItem;
	Widget	Uxmenu1_top_b1;
	Widget	UxOggettoS;
	swidget	UxPaginaDrawing;
	XrmDatabase	UxPuntDB;
	Widget	*UxRigaPerVar;
	Widget	UxVariabEdit;
	int	*UxTipoVariabile;
	PAGINA	*UxPagConf;
	OlDatabaseTopologiaObject	UxDataBaseTo;
	PAGINA	*UxStrutture;
	OlDatabaseTopologiaObject	UxDbase;
} _UxCResource;

static _UxCResource            *UxResourceContext;
#define Resource                UxResourceContext->UxResource
#define mainWindow1             UxResourceContext->UxmainWindow1
#define panedWindow1            UxResourceContext->UxpanedWindow1
#define form1                   UxResourceContext->Uxform1
#define scrolledWindow1         UxResourceContext->UxscrolledWindow1
#define scrolledList1           UxResourceContext->UxscrolledList1
#define label1                  UxResourceContext->Uxlabel1
#define form2                   UxResourceContext->Uxform2
#define scrolledWindow2         UxResourceContext->UxscrolledWindow2
#define Widres_rc               UxResourceContext->UxWidres_rc
#define Geometric_RC            UxResourceContext->UxGeometric_RC
#define Specific_RC             UxResourceContext->UxSpecific_RC
#define Color_RC                UxResourceContext->UxColor_RC
#define VarCC1_rc               UxResourceContext->UxVarCC1_rc
#define VarCC2_rc               UxResourceContext->UxVarCC2_rc
#define VarCC3_rc               UxResourceContext->UxVarCC3_rc
#define VarCC4_rc               UxResourceContext->UxVarCC4_rc
#define GruppiPane              UxResourceContext->UxGruppiPane
#define menu2                   UxResourceContext->Uxmenu2
#define pushButton1             UxResourceContext->UxpushButton1
#define pushButton15            UxResourceContext->UxpushButton15
#define menu1                   UxResourceContext->Uxmenu1
#define FilePane                UxResourceContext->UxFilePane
#define LoadItem                UxResourceContext->UxLoadItem
#define ResetItem               UxResourceContext->UxResetItem
#define CloseItem               UxResourceContext->UxCloseItem
#define menu1_top_b1            UxResourceContext->Uxmenu1_top_b1
#define OggettoS                UxResourceContext->UxOggettoS
#define PaginaDrawing           UxResourceContext->UxPaginaDrawing
#define PuntDB                  UxResourceContext->UxPuntDB
#define RigaPerVar              UxResourceContext->UxRigaPerVar
#define VariabEdit              UxResourceContext->UxVariabEdit
#define TipoVariabile           UxResourceContext->UxTipoVariabile
#define PagConf                 UxResourceContext->UxPagConf
#define DataBaseTo              UxResourceContext->UxDataBaseTo
#define Strutture               UxResourceContext->UxStrutture
#define Dbase                   UxResourceContext->UxDbase


Widget	CoreItem;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_Resource();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/************************************************************
	Controlla se sono state fatte delle modifiche 
	alle risorse di un oggetto che non sono state
	salvate 
************************************************************/

Boolean ControllaModifiche (swidget Ogg)
{
int Numero;
Widget *Riga,*Figli;
int i;
char str[5];
XmString Stringa,Confronto;

strcpy (str,"|");
Confronto = XmStringCreateSimple (str);

get_something (Ogg, XmNnumChildren, (void*) &Numero);
get_something (Ogg, XmNchildren, (void*) &Figli);

for (i=0;i<Numero;i++)
	{
	get_something (Figli[i], XmNchildren, (void*) &Riga);
	get_something (Riga[1], XmNlabelString, (void*) &Stringa);
	if (XmStringCompare(Stringa,Confronto)) return (True);
	XmStringFree (Stringa);
	}
return (False);
}

/**************************************************************
	Routine che crea tutti i rowcolumn per la visualizzazione
	e la gestione delle risorse
**************************************************************/

void CreaRowColumnRisorse (Widget Wdg)
{
XtDestroyWidget (Widres_rc);

/*  Creazione di un rowcol che conterra' i rowcol
	 relativi ad un gruppo di risorse da visualizzare  */
Widres_rc = (swidget )XtVaCreateWidget ("Widres_rc",
		xmRowColumnWidgetClass, scrolledWindow2,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		XmNnavigationType, XmTAB_GROUP,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (Widres_rc ,   (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative a core  */
Geometric_RC = (swidget )XtVaCreateWidget ("Geometric_RC",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (Geometric_RC ,(char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative al tipo di variabile  */
Specific_RC = (swidget )XtVaCreateWidget ("Specific_RC",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (Specific_RC , (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative ai colori  */
Color_RC = (swidget )XtVaCreateWidget ("Color_RC",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (Color_RC ,    (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative alle
	variabile 1 di cambio colori  */
VarCC1_rc = (swidget )XtVaCreateWidget ("VarCC1_rc",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (VarCC1_rc ,   (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative alla
	variabile 2 di cambio colori  */
VarCC2_rc = (swidget )XtVaCreateWidget ("VarCC2_rc",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (VarCC2_rc ,   (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative alla
	variabile 3 di cambio colori  */
VarCC3_rc = (swidget )XtVaCreateWidget ("VarCC3_rc",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (VarCC3_rc ,   (char *) UxResourceContext); 
#endif

/*  Creazione del rowcol per le risorse relative alla
	variabile 4 di cambio colori  */
VarCC4_rc = (swidget )XtVaCreateWidget ("VarCC4_rc",
		xmRowColumnWidgetClass, Widres_rc,
		XmNnumColumns, 100,
		XmNisAligned, False,
		XmNspacing, 0,
		XmNmarginHeight, 0,
		XmNmarginTop, 0,
		XmNmarginBottom, 0,
		NULL);
#ifndef DESIGN_TIME
	UxPutContext (VarCC4_rc ,   (char *) UxResourceContext); 
#endif
}

/**********************************************************
	Rilegge tutte le risorse del nuovo oggetto selezionato
	e ne visualizza le risorse, se del vecchio oggetto 
	sono presenti delle modifiche che non sono state salvate
	viene comunicato con una finestra di dialogo
**********************************************************/

int EseguiLoadResources()
{
Widget WdgAppoggio,OthRes;
/*
XlCoreClassRec *classe_oggetto;
*/
WidgetClass classe_oggetto;

Boolean Occupato,selezionato;
char *Appoggio2,appoggio[100];
int indice;
XmString StrLab;
Arg arg[3];

/* Cerca nella drawing area l'oggetto selezionato */
WdgAppoggio = (Widget )XlGetSelectedWidget (PaginaDrawing);

/*  Mette a False l'attributo InEdit dell'oggetto che si sta per abbandonare */
if ( (OggettoS != NULL) && (OggettoS != WdgAppoggio) )
  {
  XtSetArg (arg[0], XlNinEdit, False);
  XtSetArg (arg[1], XlNwEdit, NULL);
  XtSetValues (OggettoS, arg, 2);
  }

/*  Controlla se l'oggetto selezionato trovato e' gia in edit in un altro
      Resource Editor  */
if (WdgAppoggio != NULL)
  {
  XtSetArg (arg[0], XlNinEdit, &Occupato);
  XtGetValues (WdgAppoggio, arg, 1);
  if (WdgAppoggio == OggettoS) Occupato=False;
  if (!Occupato) OggettoS = WdgAppoggio;
  else
    {
    XtSetArg (arg[0], XlNwEdit, &OthRes);
    XtGetValues (WdgAppoggio, arg, 1);
#ifndef DESIGN_TIME
    XRaiseWindow (XtDisplay(OthRes),XtWindow(OthRes));
#endif
    }
  }

if ( (OggettoS != NULL) && (!Occupato) )
	{
	/*  Setta il valore dell'oggetto selezionato in editazione  */
	XtSetArg (arg[0], XlNinEdit, True);
	XtSetArg (arg[1], XlNwEdit, Resource);
	XtSetValues (OggettoS, arg, 2);
	set_something (ResetItem, XmNsensitive, (void*) True);
	set_something (pushButton1, XmNsensitive, (void*) True);
	/*  Carico il nome e la classe dell'oggetto selezionato  */
	strcpy (appoggio,WidGetName(OggettoS));
	strcat (appoggio,"  ----  ");


        classe_oggetto = GetClass (OggettoS);
	Appoggio2 = XtNewString ( XlGetClassName(classe_oggetto) );
	strcat (appoggio,Appoggio2);
	XtFree (Appoggio2);
	StrLab = XmStringCreateSimple (appoggio);
	/*  Salvo l'indice del rowcolumn attualmente visualizzato */
	if (Usato(Geometric_RC)) indice = 0;
	if (Usato(Specific_RC)) indice = 1;
	if (Usato(Color_RC)) indice = 2;
	if (Usato(VarCC1_rc)) indice = 3;
	if (Usato(VarCC2_rc)) indice = 4;
	if (Usato(VarCC3_rc)) indice = 5;
	if (Usato(VarCC4_rc)) indice = 6;
	/*  Ricreo i rowcolumn delle risorse  */
	CreaRowColumnRisorse (Widres_rc);
	PrepListaRes (Geometric_RC, OggettoS, XlRGrGeom, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (Specific_RC, OggettoS, XlRGrIo, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (Color_RC, OggettoS, XlRGrColor, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (VarCC1_rc, OggettoS, XlRGrVCC1, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (VarCC2_rc, OggettoS, XlRGrVCC2, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (VarCC3_rc, OggettoS, XlRGrVCC3, DataBaseTo,
				PagGetType(PagConf));
	PrepListaRes (VarCC4_rc, OggettoS, XlRGrVCC4, DataBaseTo,
				PagGetType(PagConf));

	if (indice == 0) XtManageChild (Geometric_RC);
	if (indice == 1) XtManageChild (Specific_RC);
	if (indice == 2) XtManageChild (Color_RC);
	if (indice == 3) XtManageChild (VarCC1_rc);
	if (indice == 4) XtManageChild (VarCC2_rc);
	if (indice == 5) XtManageChild (VarCC3_rc);
	if (indice == 6) XtManageChild (VarCC4_rc);
	XtManageChild (Widres_rc);
	/*  Elimino dalla lista il nome del vecchio oggetto
		ed aggiungo il nome di quello nuovo  */
	XmListDeleteAllItems (scrolledList1);
	XmListAddItemUnselected (scrolledList1,StrLab,0);
	}
else
	{
	if (!Occupato)
	  {
	  XmListDeleteAllItems (scrolledList1);
	  set_something (ResetItem, XmNsensitive, (void*) False);
	  set_something (pushButton1, XmNsensitive, (void*) False);
	  strcpy (appoggio,"No Widget selected");
	  StrLab = XmStringCreateSimple (appoggio);
	  XmListAddItemUnselected (scrolledList1, StrLab, 0);
	  }
	}
XmStringFree (StrLab);
}

/************************************************
	Setta nella label per la segnalazione degli
	errori o delle modifiche il segno di errore
************************************************/

void SettaErrore (Widget Wid)
{
XmString Errore;
char stringa[3];

strcpy (stringa,"X");
Errore = XmStringCreateSimple (stringa);
set_something (Wid , XmNlabelString, (void*) Errore);
XmStringFree (Errore);
}

/*********************************************************
	Salva tutte le risorse del rowcolumn Riga nel database
	facendo i controlli che i valori letti dal rowcolumn siano
	coerenti con i tipi
*********************************************************/

int EseguiApply (Widget *Riga,int NumFigli,XlConfInfo *confinfo)
{
Widget *Ogget,Padre;
XmString StrLab;
int i;

int NumeroErrori;
int index;                   /*  Indice relativo alla confinfo   */
char *StrIndex;              /*  Stringa dell'indice relativo alla confinfo   */

NumeroErrori = 0;
Padre = GetPadre (OggettoS);
StrLab = XmStringCreateSimple (" ");
for ( i = 0 ; i < NumFigli ; i++ )
{
	/*  Legge dal rowcolumn di riga la lista dei suoi figli  */
	get_something (Riga[i], XmNchildren, (void*) &Ogget);

	/*  Azzera la stringa delle modifiche effettuate  */
	set_something (Ogget[1], XmNlabelString, (void*) StrLab);

	/*  Legge l'indice della confinfo relativa alla risorsa 
            memorizzata come nome dell'oggetto nascosto */
/**************************************************/
#ifndef DESIGN_TIME
	StrIndex = XtName(Ogget[3]);
#endif
/***************************************************/
	/*  Converte la stringa dell'indice della confinfo letta
		in un intero per poter essere utilizzato come indice  */
	index = atoi (StrIndex);
	

	NumeroErrori += EseguiControlliInput (confinfo[index],PuntDB,
						OggettoS,Ogget);
}
XmStringFree (StrLab);
return (NumeroErrori);
}

/*************************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*************************************************************/

int PushColorCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	XmString StrLab;
	char *s;
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	/*  Legge la label del bottone  */
	get_something (RCOgget[0], XmNlabelString, (void*) &StrLab);
	s = XmTextFieldGetString (RCOgget[2]);

	Creato = (Widget )create_ColorViewer(EDITOR_VER,s,StrLab,PuntDB,RCOgget,
				Resource);
	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

	XtFree (s);
return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushVarInpCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	char *StrIndex;
	int index;

        WidgetClass WidClassSel;
	XlConfInfo *conf;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	/*  Legge l'indice della confinfo dal textField nascosto */
/************************************************/
#ifndef DESIGN_TIME
	StrIndex = XtName(RCOgget[3]);
#endif
/***************************************************/
	index = atoi (StrIndex);

	/*  Legge la confinfo dell'oggetto selezionato  */

        WidClassSel = GetClass (OggettoS);  
	conf = XlGetConfinfoWid (OggettoS);
	TipoVariabile[0] = conf[index].db_type;

	RigaPerVar [0] = RCOgget [0];
	RigaPerVar [1] = RCOgget [1];
	RigaPerVar [2] = RCOgget [2];
	RigaPerVar [3] = RCOgget [3];
	UxPopupInterface (VariabEdit, no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushVarOutCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	char *StrIndex;
	int index;

        WidgetClass WidClassSel;
	XlConfInfo *conf;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	/*  Legge l'indice della confinfo dal textField nascosto */
/***********************************/
#ifndef DESIGN_TIME
	StrIndex = XtName(RCOgget[3]);
#endif
/*******************************/
	index = atoi (StrIndex);

	/*  Legge la confinfo dell'oggetto selezionato  */

	WidClassSel = GetClass (OggettoS);
	conf = XlGetConfinfoWid (OggettoS);
	TipoVariabile[0] = conf[index].db_type;

	RigaPerVar [0] = RCOgget [0];
	RigaPerVar [1] = RCOgget [1];
	RigaPerVar [2] = RCOgget [2];
	RigaPerVar [3] = RCOgget [3];
	UxPopupInterface (VariabEdit, no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushFontCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	char *s;
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	/*  Legge la label del bottone  */
	s = (char *)XmTextFieldGetString (RCOgget[2]);
	Creato = (Widget )create_FontsViewer(s,RCOgget,Resource);

	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

	XtFree (s);
return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushIconCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	char *BitmapWid="Bitmap";
        char *ChangeWid="ChangePage";
	char *IconWid="Icon";
	char *IconRegWid="IconReg";
	WidgetClass ClassWidSel;
	char *NomeClasseSel;
	Widget *RCOgget,padre;
	Widget Creato;
	char *Stringa;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	ClassWidSel = (WidgetClass)GetClass (OggettoS);
	NomeClasseSel = XlGetClassName (ClassWidSel);

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	Stringa = (char *)CvtPixToStr (OggettoS,XmNbackground);
/*
Aggiunta per bitmap su ChangePage
*/
	if ( (strcmp(NomeClasseSel,ChangeWid)) == 0 ) 
		Creato = (Widget )create_AnimateIconSelection(SEL_BITMAP,
					Resource,RCOgget,OggettoS,Stringa);

/* carlo 7/7/97
*/
        if( XlIsOperableKeys(OggettoS) ||
            XlIsPixvar(OggettoS) ||
            ( strcmp(NomeClasseSel,BitmapWid) == 0) )
		Creato = (Widget )create_AnimateIconSelection(SEL_BITMAP,
					Resource,RCOgget,OggettoS,Stringa);
/*       
	if ( (strcmp(NomeClasseSel,BitmapWid)) == 0 ) 
		Creato = (Widget )create_AnimateIconSelection(SEL_BITMAP,
					Resource,RCOgget,OggettoS,Stringa);
*/
	else if ( (strcmp(NomeClasseSel,IconWid) == 0) ||
		(strcmp(NomeClasseSel,IconRegWid)== 0) ||
                (strcmp(NomeClasseSel,XlCPictograph)==0) )
		Creato = (Widget )create_AnimateIconSelection (SEL_ICONA,
					Resource,RCOgget,OggettoS,Stringa);

	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif
	XtFree (Stringa);
return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushFormCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	Creato = (Widget )create_FormSelection(Resource,RCOgget);

	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif
return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushVarRegCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	/*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	Creato = (Widget )create_VariablesSelection(Resource,RCOgget,
		OggettoS);

	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif
return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushInputCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	char Messaggio[100];
	Widget *RCOgget,padre;
	Widget Creato;
	OlFormObject PuntatoreForm;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif
	/*  Controlla se e' stata precedentemente caricata la form  */
	get_something (OggettoS, XlNobjectForm, (void*) &PuntatoreForm);
   	if (PuntatoreForm == NULL)
	  {
	  strcpy (Messaggio,"Form file not loaded!");
	  VisualizzaMessageBox(GENERICA,Resource,Messaggio,OggettoS);
	  } 
	else
	  {
	  /*  Legge i figli del rowcolumn piu' grande  */
	  padre = GetPadre (wid);
	  get_something (padre, XmNchildren, (void*) &RCOgget);

	  /*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	  set_something (RCOgget[2], XmNsensitive, (void*) False);
	  set_something (RCOgget[0], XmNsensitive, (void*) False);

	  Creato = (Widget )create_ValueSelection (Resource,RCOgget,OggettoS,PuntatoreForm);
	  UxPopupInterface (Creato, no_grab);
	  }

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif
return (OK);
}

/**************************************************************
   Funzione di callback associata al pushButton dell'OptionMenu
	presente nella lista delle risorse creata runtime   
**************************************************************/

int OptResCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *Riga;
	Widget Parent;

	Parent = GetPadre (wid);
	Parent = GetPadre (Parent);
	Parent = GetPadre (Parent);
	get_something (Parent, XmNchildren, (void*) &Riga);
	SettaModifica (Riga[1]);	

return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushCompCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

        /*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	Creato = (Widget )create_DatabaseSelection(Resource,SEL_COMP,RCOgget,
					DataBaseTo);
	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

return (OK);
}

/*******************************************************
   Funzione di callback associata al pushButton presente
	nella lista delle risorse creata runtime   
*******************************************************/

int PushSubSCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *RCOgget,padre;
	Widget Creato;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

        /*  Legge i figli del rowcolumn piu' grande  */
	padre = GetPadre (wid);
	get_something (padre, XmNchildren, (void*) &RCOgget);

	/*  Setta a False l'attributo sensitive del textfield
		che sta per essere aggiornato     */
	set_something (RCOgget[2], XmNsensitive, (void*) False);
	set_something (RCOgget[0], XmNsensitive, (void*) False);

	Creato = (Widget )create_DatabaseSelection(Resource,SEL_SUBS,RCOgget,
					DataBaseTo);
	UxPopupInterface (Creato , no_grab);

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

return (OK);
}

/**************************************************************
   Funzione di callback associata alla modifica della stringa
	presente nel textfield della lista delle risorse   
**************************************************************/

int TextResCB (swidget wid,int ind_res, XmAnyCallbackStruct *call_data)
{
	Widget *Riga;
	Widget Parent;

#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	Parent = GetPadre (wid);
	get_something (Parent, XmNchildren, (void*) &Riga);
	SettaModifica (Riga[1]);	

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif

return (OK);
}

/*************************************************************
    Funzione di callback associata al pushButton dell'OptionMenu 
	per la selezione dei gruppi di risorse da visualizzare  
*************************************************************/

int OptMenuCB (swidget wid,int ind_voce, XmAnyCallbackStruct *call_data)
{
#ifndef DESIGN_TIME
        _UxCResource *UxSaveCtx,*UxContext;

        UxSaveCtx = UxResourceContext;
        UxResourceContext = UxContext =
                        (_UxCResource *)UxGetContext(wid);
#endif

	if (Usato (Geometric_RC)) XtUnmanageChild (Geometric_RC);
	if (Usato (Specific_RC))  XtUnmanageChild (Specific_RC);
	if (Usato (Color_RC))     XtUnmanageChild (Color_RC);
	if (Usato (VarCC1_rc))    XtUnmanageChild (VarCC1_rc);
	if (Usato (VarCC2_rc))    XtUnmanageChild (VarCC2_rc);
	if (Usato (VarCC3_rc))    XtUnmanageChild (VarCC3_rc);
	if (Usato (VarCC4_rc))    XtUnmanageChild (VarCC4_rc);

	switch (ind_voce)
	{
	case 0:
		XtManageChild (Geometric_RC);
	break;
	case 1:
		XtManageChild (Specific_RC);
	break;
	case 2:
		XtManageChild (Color_RC);
	break;
	case 3:
		XtManageChild (VarCC1_rc);
	break;
	case 4:
		XtManageChild (VarCC2_rc);
	break;
	case 5:
		XtManageChild (VarCC3_rc);
	break;
	case 6:
		XtManageChild (VarCC4_rc);
	break;
	}		

#ifndef DESIGN_TIME
        UxResourceContext = UxSaveCtx;
#endif
return (OK);
}

/**************************************************************
	Creazione di un pushButton
**************************************************************/

Widget CreaPushButton (parent, nome, label, width)
Widget parent;
char *nome;
XmString label;
Dimension width;
{
        Widget appo;
        Arg args[6];
        int narg;

        narg = 0;
        XtSetArg (args[narg], XmNmarginHeight, 0); narg++;
        XtSetArg (args[narg], XmNmarginTop, 0); narg++;
        XtSetArg (args[narg], XmNmarginBottom, 0); narg++;
        if (width != NULL)
        {
                XtSetArg (args[narg], XmNrecomputeSize, FALSE ); narg++;
                XtSetArg (args[narg], XmNwidth, width ); narg++;
        }
        XtSetArg (args[narg], XmNlabelString, label); narg++;
        appo = XmCreatePushButton (parent, nome, args, narg);
        XtManageChild (appo);

#ifndef DESIGN_TIME
		UxPutContext(appo,(char *) UxResourceContext);
#endif

        return (appo);
}

/*****************************************************************
	Creazione dell'option menu dei gruppi 
*****************************************************************/

int CreaOptionMenu (Widget Padre,Widget ogg)
{
        Widget  *ButtGruppi;
        char  *label_butt;
	extern int NumGruppi;
        int i;
        XmString StrLab;

        XtDestroyWidget (CoreItem);
        ButtGruppi = (Widget *)XtCalloc (NumGruppi, sizeof(Widget));
        if (ButtGruppi == NULL) return (NON_ALLOCATA);
        for ( i = 0 ; i < NumGruppi ; i++ )
        {

printf("CreaOptionMenu: Gruppi_resource[%d]=%s \n", i, Gruppi_resource[i]);

                label_butt = XtNewString ( Gruppi_resource[i] );
                StrLab = XmStringCreateSimple (label_butt);
                ButtGruppi[i] = (Widget )CreaPushButton (ogg, "optbt",
					StrLab, NULL);
                XtAddCallback (ButtGruppi[i],XmNactivateCallback, OptMenuCB,i);
		XtFree (label_butt);
		XmStringFree (StrLab);
        }
        set_something (Padre, XmNmenuHistory, (void*) ButtGruppi[1]);
        return (OK);
}

/***********************************************************
	Creazione di un TextField
***********************************************************/

Widget CreaTextField (parent, nome, label, width)
Widget parent;
char *nome,*label;
Dimension width;

{
        Widget appo;
        Arg args[10];
        int narg;

        narg = 0;
        XtSetArg (args[narg], XmNmarginHeight, 0); narg++;
        XtSetArg (args[narg], XmNmarginTop, 0); narg++;
        XtSetArg (args[narg], XmNmarginBottom, 0); narg++;
        if (width != NULL)
        {
                XtSetArg (args[narg], XmNrecomputeSize, FALSE ); narg++;
                XtSetArg (args[narg], XmNwidth, width ); narg++;
        }
        XtSetArg (args[narg], XmNvalue, label); narg++;
        appo = (Widget )XmCreateTextField (parent, nome, args, narg);
        XtManageChild (appo);

#ifndef DESIGN_TIME
		UxPutContext(appo,(char *) UxResourceContext);
#endif

       return (appo);
}

/*********************************************
  disabilita i bottoni di load,close,reset
  del resource editor quando e' stato lanciato 
  il draw o il bitmap editor
**********************************************/   
void disableButton()
{
   set_something(pushButton15,XmNsensitive,(void*) False); /* load button*/
   set_something(LoadItem,XmNsensitive,(void*) False);     /* load item menu */ 
   set_something(CloseItem,XmNsensitive,(void*) False);    /* close item menu */
   set_something(ResetItem,XmNsensitive,(void*) False);    /* reset item menu */ 
}

/*********************************************
  riabilita i bottoni di load,close,reset
  del resource editor quando viene chiusi  
  il draw o il bitmap editor
**********************************************/   
void riableButton()
{
   set_something(pushButton15,XmNsensitive,(void*) True); /* load button*/
   set_something(LoadItem,XmNsensitive,(void*) True);     /* load item menu */ 
   set_something(CloseItem,XmNsensitive,(void*) True);    /* close item menu */
   set_something(ResetItem,XmNsensitive,(void*) True);    /* reset item menu */ 
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCResource            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResourceContext;
	UxResourceContext = UxContext =
			(_UxCResource *) UxGetContext( UxWidget );
	{
#ifdef ON_PAGEDIT  
		extern void AddTransWid ();
	        extern Boolean PagGetConnectMode();
		extern float get_def_zoom();
#endif
	int perc_zoom;
	/*
	XlCoreClassRec *WidClassSel; 
	*/
	WidgetClass WidClassSel;
	XlConfInfo *confinfo;
	XmString Errore;
	char *NomeOggetto,appo[100];
	Widget PaginaGrafica,*Riga;
	int NumFigli;
	int NumErr;
	XrmDatabase DefDB;
	
	/***
		Salva il database del display
	***/
	DefDB = (XrmDatabase )XrmGetDatabase (UxDisplay);
	
	NumErr = 0;
	/*  Ricava la classe dell'oggetto selezionato  */
	/*
	WidClassSel = (XlCoreClassRec *) GetClass(OggettoS);
	*/
	WidClassSel = GetClass(OggettoS);
	
	/*  Esegue apply delle risorse del gruppo Geometric  */
	get_something (Geometric_RC, XmNchildren, (void*) &Riga);
	get_something (Geometric_RC, XmNnumChildren, (void*) &NumFigli);
	confinfo = XlGetConfinfoCore (OggettoS);
	NumErr += EseguiApply (Riga , NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo IO Variable  */
	get_something (Specific_RC, XmNchildren, (void*) &Riga);
	get_something (Specific_RC, XmNnumChildren, (void*) &NumFigli);
	confinfo = XlGetConfinfoWid (OggettoS);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo Color   */
	get_something (Color_RC, XmNchildren, (void*) &Riga);
	get_something (Color_RC, XmNnumChildren, (void*) &NumFigli);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo VCC1   */
	get_something (VarCC1_rc, XmNchildren, (void*) &Riga);
	get_something (VarCC1_rc, XmNnumChildren, (void*) &NumFigli);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo VCC2   */
	get_something (VarCC2_rc, XmNchildren, (void*) &Riga);
	get_something (VarCC2_rc, XmNnumChildren, (void*) &NumFigli);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo VCC3   */
	get_something (VarCC3_rc, XmNchildren, (void*) &Riga);
	get_something (VarCC3_rc, XmNnumChildren, (void*) &NumFigli);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Esegue apply delle risorse del gruppo VCC4   */
	get_something (VarCC4_rc, XmNchildren, (void*) &Riga);
	get_something (VarCC4_rc, XmNnumChildren, (void*) &NumFigli);
	NumErr += EseguiApply (Riga, NumFigli, confinfo);
	
	/*  Distrugge e ricrea l'oggetto selezionato per leggere 
		le nuove risorse dal database   */
	if (NumErr != 0)
		{
		strcpy (appo,"An error detected in input!");
		VisualizzaMessageBox (GENERICA, Resource, appo, OggettoS); 
		}
	else
		{
		PaginaGrafica = GetPadre (OggettoS);
		NomeOggetto = XtNewString (WidGetName(OggettoS));
	/*
	 Nel caso icona di regolazione gestisce gli eventuali spostamenti
	 di connessioni legate all'icona
	*/
#ifdef ON_PAGEDIT
		if(XlIsIconReg(OggettoS))
	        	apply_gest_conn1(OggettoS);
#endif        
	
		XtDestroyWidget (OggettoS);
	/***
		Setta per la ricreazione dell'oggetto modificato il display
		relativo alla pagina
	***/
		XrmSetDatabase (UxDisplay,PuntDB);
	
		OggettoS = (Widget )XtVaCreateManagedWidget ( NomeOggetto,
			WidClassSel, PaginaGrafica,
			XlNselected, True,
			XlNinEdit, True,
			XlNwEdit, Resource,
			NULL);
#ifdef ON_PAGEDIT
	/*
	 se e' settato lo zoom di pagina bisogna tenerne conto
	 nel ricreare l'oggetto (se l'oggetto non e' figlio
	 di XlComposite
	*/
		
		if(XlIsXlComposite(XtParent(OggettoS))==False)
			{
			perc_zoom=100.0 * get_def_zoom(PagConf->drawing);
			set_something(OggettoS,XlNfattZoom,(void*) perc_zoom);
			}
	
		if(XlIsIconReg(OggettoS))
	/*
	 gestisce eventuali spostamenti delle connessioni
	*/
			apply_gest_conn2(OggettoS);
#endif	
		XtFree (NomeOggetto);
	
	/***
		Ricarica nella struttura di UxDisplay il database di default
	***/
		XrmSetDatabase (UxDisplay,DefDB);
		}
	
#ifdef ON_PAGEDIT 
		AddTransWid (PagConf,OggettoS);
	/*
	 Se la pagina e' di regolazione puo' darsi che
	 sia in connect mode: in tal caso deve essere
	 nuovamente settato il flag di connect mode sulle
	 porte dell'oggetto ricreato
	*/
		if(PagGetConnectMode(PagConf->drawing))
			{
			set_connect(PaginaGrafica ,True);
			}
	
	/*
		se l'oggetto e' una icona di regolazione, una porta, un dispReg
		e' neccessario settare il display mode attivo sulla pagina
	*/
		SetSetMode(OggettoS);
	
#endif
	
	}
	UxResourceContext = UxSaveCtx;
}

static	void	activateCB_pushButton15( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCResource            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResourceContext;
	UxResourceContext = UxContext =
			(_UxCResource *) UxGetContext( UxWidget );
	{
	Boolean ModificheGeometric; /*  Variabile identificazione modifiche apportate
					alle risorse di geometric  */
	Boolean ModificheSpecific;  /*  Variabile identificazione modifiche apportate
					alle risorse di specific   */
	Boolean ModificheColor;     /*  Variabile identificazione modifiche apportate
					alle risorse di color      */
	Boolean ModificheVCC1;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 1 */
	Boolean ModificheVCC2;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 2 */
	Boolean ModificheVCC3;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 3 */
	Boolean ModificheVCC4;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 4 */
	
	char Str[100];
	
	/*
		Controllo se sono state fatte modifiche che non 
		sono state applicate     
	*/
	ModificheGeometric = ControllaModifiche (Geometric_RC);
	ModificheSpecific = ControllaModifiche (Specific_RC);
	ModificheColor = ControllaModifiche (Color_RC);
	ModificheVCC1 = ControllaModifiche (VarCC1_rc);
	ModificheVCC2 = ControllaModifiche (VarCC2_rc);
	ModificheVCC3 = ControllaModifiche (VarCC3_rc);
	ModificheVCC4 = ControllaModifiche (VarCC4_rc);
	
	if (ModificheGeometric || ModificheSpecific || ModificheColor 
		|| ModificheVCC1 || ModificheVCC2 || ModificheVCC3 || ModificheVCC4)
		{
		strcpy (Str,"You have not applied new modific,continue?");
		VisualizzaMessageBox (LOAD,Resource,Str,OggettoS);
		}
	else EseguiLoadResources();
	
	}
	UxResourceContext = UxSaveCtx;
}

static	void	activateCB_LoadItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCResource            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResourceContext;
	UxResourceContext = UxContext =
			(_UxCResource *) UxGetContext( UxWidget );
	{
	Boolean ModificheGeometric; /*  Variabile identificazione modifiche apportate
					alle risorse di geometric  */
	Boolean ModificheSpecific;  /*  Variabile identificazione modifiche apportate
					alle risorse di specific   */
	Boolean ModificheColor;     /*  Variabile identificazione modifiche apportate
					alle risorse di color      */
	Boolean ModificheVCC1;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 1 */
	Boolean ModificheVCC2;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 2 */
	Boolean ModificheVCC3;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 3 */
	Boolean ModificheVCC4;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 4 */
	
	char Str[100];
	
	/*
		Controllo se sono state fatte modifiche che non 
		sono state applicate     
	*/
	ModificheGeometric = ControllaModifiche (Geometric_RC);
	ModificheSpecific = ControllaModifiche (Specific_RC);
	ModificheColor = ControllaModifiche (Color_RC);
	ModificheVCC1 = ControllaModifiche (VarCC1_rc);
	ModificheVCC2 = ControllaModifiche (VarCC2_rc);
	ModificheVCC3 = ControllaModifiche (VarCC3_rc);
	ModificheVCC4 = ControllaModifiche (VarCC4_rc);
	
	if (ModificheGeometric || ModificheSpecific || ModificheColor 
		|| ModificheVCC1 || ModificheVCC2 || ModificheVCC3 || ModificheVCC4)
		{
		strcpy (Str,"You have not applied new modific,continue?");
		VisualizzaMessageBox (LOAD,Resource,Str,OggettoS);
		}
	else EseguiLoadResources();
	}
	UxResourceContext = UxSaveCtx;
}

static	void	activateCB_ResetItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCResource            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResourceContext;
	UxResourceContext = UxContext =
			(_UxCResource *) UxGetContext( UxWidget );
	{
	int indice;
	
	if (Usato(Geometric_RC)) indice = 0;
	if (Usato(Specific_RC)) indice = 1;
	if (Usato(Color_RC)) indice = 2;
	if (Usato(VarCC1_rc)) indice = 3;
	if (Usato(VarCC2_rc)) indice = 4;
	if (Usato(VarCC3_rc)) indice = 5;
	if (Usato(VarCC4_rc)) indice = 6;
	
	CreaRowColumnRisorse (Widres_rc);
	
	PrepListaRes (Geometric_RC, OggettoS, XlRGrGeom, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (Specific_RC, OggettoS, XlRGrIo, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (Color_RC, OggettoS, XlRGrColor, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (VarCC1_rc, OggettoS, XlRGrVCC1, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (VarCC2_rc, OggettoS, XlRGrVCC2, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (VarCC3_rc, OggettoS, XlRGrVCC3, DataBaseTo,
					PagGetType(PagConf));
	PrepListaRes (VarCC4_rc, OggettoS, XlRGrVCC4, DataBaseTo,
					PagGetType(PagConf));
	
	if (indice == 0) XtManageChild (Geometric_RC);
	if (indice == 1) XtManageChild (Specific_RC);
	if (indice == 2) XtManageChild (Color_RC);
	if (indice == 3) XtManageChild (VarCC1_rc);
	if (indice == 4) XtManageChild (VarCC2_rc);
	if (indice == 5) XtManageChild (VarCC3_rc);
	if (indice == 6) XtManageChild (VarCC4_rc);
	XtManageChild (Widres_rc);
	}
	UxResourceContext = UxSaveCtx;
}

static	void	activateCB_CloseItem( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCResource            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxResourceContext;
	UxResourceContext = UxContext =
			(_UxCResource *) UxGetContext( UxWidget );
	{
	Boolean ModificheGeometric; /*  Variabile identificazione modifiche apportate
					alle risorse di geometric  */
	Boolean ModificheSpecific;  /*  Variabile identificazione modifiche apportate
					alle risorse di specific   */
	Boolean ModificheColor;     /*  Variabile identificazione modifiche apportate
					alle risorse di color      */
	Boolean ModificheVCC1;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 1 */
	Boolean ModificheVCC2;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 2 */
	Boolean ModificheVCC3;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 3 */
	Boolean ModificheVCC4;	    /*  Variabile identificazione modifiche apportate
					alle risorse della variabile cambio colore 4 */
	
	char Str[100];
	Arg arg[3];
	
	/*
		Controllo se sono state fatte modifiche che non 
		sono state applicate     
	*/
	ModificheGeometric = ControllaModifiche (Geometric_RC);
	ModificheSpecific = ControllaModifiche (Specific_RC);
	ModificheColor = ControllaModifiche (Color_RC);
	ModificheVCC1 = ControllaModifiche (VarCC1_rc);
	ModificheVCC2 = ControllaModifiche (VarCC2_rc);
	ModificheVCC3 = ControllaModifiche (VarCC3_rc);
	ModificheVCC4 = ControllaModifiche (VarCC4_rc);
	
	if (ModificheGeometric || ModificheSpecific || ModificheColor 
		|| ModificheVCC1 || ModificheVCC2 || ModificheVCC3 || ModificheVCC4)
		{
		strcpy (Str,"You have not applied new modific,continue?");
	 	VisualizzaMessageBox (CLOSE,Resource,Str,OggettoS);
		}
	else
		{
		if (OggettoS)
			{
			XtSetArg (arg[0], XlNinEdit, False);
			XtSetArg (arg[1], XlNwEdit, NULL);
			XtSetValues (OggettoS, arg, 2);
			}
		XtDestroyWidget (Resource);
		}
	}
	UxResourceContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_Resource()
{
	Widget		_UxParent;
	Widget		GruppiPane_shell;
	Widget		FilePane_shell;


	/* Creation of Resource */
	_UxParent = PagConf->topwidget;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	Resource = XtVaCreatePopupShell( "Resource",
			topLevelShellWidgetClass,
			_UxParent,
			XmNx, 424,
			XmNy, 5,
			XmNwidth, 550,
			XmNheight, 600,
			XmNiconName, "Resource Editor",
			XmNtitle, "Widget Resource Editor",
			XmNallowShellResize, TRUE,
			XmNdeleteResponse, XmDO_NOTHING,
			NULL );
	UxPutContext( Resource, (char *) UxResourceContext );


	/* Creation of mainWindow1 */
	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass,
			Resource,
			XmNunitType, XmPIXELS,
			XmNx, 15,
			XmNy, 25,
			XmNwidth, 545,
			XmNheight, 480,
			NULL );
	UxPutContext( mainWindow1, (char *) UxResourceContext );


	/* Creation of panedWindow1 */
	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass,
			mainWindow1,
			NULL );
	UxPutContext( panedWindow1, (char *) UxResourceContext );


	/* Creation of form1 */
	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 3,
			XmNy, 3,
			XmNwidth, 539,
			XmNheight, 87,
			XmNpaneMaximum, 1000,
			XmNpaneMinimum, 80,
			NULL );
	UxPutContext( form1, (char *) UxResourceContext );


	/* Creation of scrolledWindow1 */
	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass,
			form1,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 20,
			XmNy, 30,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNbottomPosition, 90,
			XmNleftPosition, 3,
			XmNrightPosition, 97,
			XmNtopPosition, 45,
			NULL );
	UxPutContext( scrolledWindow1, (char *) UxResourceContext );


	/* Creation of scrolledList1 */
	scrolledList1 = XtVaCreateManagedWidget( "scrolledList1",
			xmListWidgetClass,
			scrolledWindow1,
			XmNwidth, 435,
			XmNheight, 50,
			NULL );
	UxPutContext( scrolledList1, (char *) UxResourceContext );


	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			form1,
			XmNx, 15,
			XmNy, 10,
			XmNwidth, 135,
			XmNheight, 25,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			RES_CONVERT( XmNlabelString, "Selected widget:" ),
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNbottomWidget, NULL,
			XmNbottomPosition, 40,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 3,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 25,
			XmNtopPosition, 2,
			NULL );
	UxPutContext( label1, (char *) UxResourceContext );


	/* Creation of form2 */
	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 3,
			XmNy, 91,
			XmNwidth, 557,
			XmNheight, 317,
			XmNpaneMinimum, 150,
			XmNpaneMaximum, 1000,
			NULL );
	UxPutContext( form2, (char *) UxResourceContext );


	/* Creation of scrolledWindow2 */
	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass,
			form2,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 10,
			XmNy, 50,
			XmNwidth, 510,
			XmNheight, 120,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 90,
			XmNleftPosition, 3,
			XmNrightPosition, 97,
			XmNtopPosition, 10,
			NULL );
	UxPutContext( scrolledWindow2, (char *) UxResourceContext );


	/* Creation of Widres_rc */
	Widres_rc = XtVaCreateManagedWidget( "Widres_rc",
			xmRowColumnWidgetClass,
			scrolledWindow2,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 128,
			XmNheight, 180,
			XmNnavigationType, XmTAB_GROUP,
			XmNisAligned, FALSE,
			XmNnumColumns, 100,
			XmNresizeHeight, FALSE,
			XmNresizeWidth, FALSE,
			NULL );
	UxPutContext( Widres_rc, (char *) UxResourceContext );


	/* Creation of Geometric_RC */
	Geometric_RC = XtVaCreateManagedWidget( "Geometric_RC",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 3,
			XmNy, 3,
			XmNwidth, 122,
			XmNheight, 16,
			XmNisAligned, FALSE,
			XmNnumColumns, 100,
			XmNresizeWidth, FALSE,
			XmNresizeHeight, FALSE,
			XmNspacing, 10,
			NULL );
	UxPutContext( Geometric_RC, (char *) UxResourceContext );


	/* Creation of Specific_RC */
	Specific_RC = XtVaCreateManagedWidget( "Specific_RC",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 5,
			XmNy, 25,
			XmNwidth, 120,
			XmNheight, 30,
			NULL );
	UxPutContext( Specific_RC, (char *) UxResourceContext );


	/* Creation of Color_RC */
	Color_RC = XtVaCreateManagedWidget( "Color_RC",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 5,
			XmNy, 40,
			XmNwidth, 120,
			XmNheight, 30,
			NULL );
	UxPutContext( Color_RC, (char *) UxResourceContext );


	/* Creation of VarCC1_rc */
	VarCC1_rc = XtVaCreateManagedWidget( "VarCC1_rc",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 10,
			XmNy, 70,
			XmNwidth, 110,
			XmNheight, 20,
			NULL );
	UxPutContext( VarCC1_rc, (char *) UxResourceContext );


	/* Creation of VarCC2_rc */
	VarCC2_rc = XtVaCreateManagedWidget( "VarCC2_rc",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 10,
			XmNy, 100,
			XmNwidth, 110,
			XmNheight, 10,
			NULL );
	UxPutContext( VarCC2_rc, (char *) UxResourceContext );


	/* Creation of VarCC3_rc */
	VarCC3_rc = XtVaCreateManagedWidget( "VarCC3_rc",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 10,
			XmNy, 100,
			XmNwidth, 80,
			XmNheight, 10,
			NULL );
	UxPutContext( VarCC3_rc, (char *) UxResourceContext );


	/* Creation of VarCC4_rc */
	VarCC4_rc = XtVaCreateManagedWidget( "VarCC4_rc",
			xmRowColumnWidgetClass,
			Widres_rc,
			XmNx, 10,
			XmNy, 160,
			XmNwidth, 110,
			XmNheight, 20,
			NULL );
	UxPutContext( VarCC4_rc, (char *) UxResourceContext );


	/* Creation of GruppiPane */
	GruppiPane_shell = XtVaCreatePopupShell ("GruppiPane_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	GruppiPane = XtVaCreateWidget( "GruppiPane",
			xmRowColumnWidgetClass,
			GruppiPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( GruppiPane, (char *) UxResourceContext );


	/* Creation of CoreItem */
	CoreItem = XtVaCreateManagedWidget( "CoreItem",
			xmPushButtonWidgetClass,
			GruppiPane,
			RES_CONVERT( XmNlabelString, "Core" ),
			XmNmappedWhenManaged, FALSE,
			NULL );
	UxPutContext( CoreItem, (char *) UxResourceContext );


	/* Creation of menu2 */
	menu2 = XtVaCreateManagedWidget( "menu2",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, GruppiPane,
			XmNtopOffset, 0,
			XmNleftOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNbottomWidget, NULL,
			XmNbottomPosition, 8,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 3,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 40,
			XmNtopPosition, 2,
			NULL );
	UxPutContext( menu2, (char *) UxResourceContext );


	/* Creation of pushButton1 */
	pushButton1 = XtVaCreateManagedWidget( "pushButton1",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 220,
			XmNy, 430,
			XmNwidth, 125,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			RES_CONVERT( XmNlabelString, "Apply" ),
			XmNleftPosition, 30,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 70,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomPosition, 98,
			XmNtopPosition, 92,
			NULL );
	XtAddCallback( pushButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton1,
		(XtPointer) UxResourceContext );

	UxPutContext( pushButton1, (char *) UxResourceContext );


	/* Creation of pushButton15 */
	pushButton15 = XtVaCreateManagedWidget( "pushButton15",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 130,
			XmNy, 430,
			XmNwidth, 125,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Load" ),
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 98,
			XmNleftPosition, 80,
			XmNrightPosition, 97,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopPosition, 92,
			NULL );
	XtAddCallback( pushButton15, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton15,
		(XtPointer) UxResourceContext );

	UxPutContext( pushButton15, (char *) UxResourceContext );


	/* Creation of menu1 */
	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass,
			mainWindow1,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( menu1, (char *) UxResourceContext );


	/* Creation of FilePane */
	FilePane_shell = XtVaCreatePopupShell ("FilePane_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FilePane = XtVaCreateWidget( "FilePane",
			xmRowColumnWidgetClass,
			FilePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( FilePane, (char *) UxResourceContext );


	/* Creation of LoadItem */
	LoadItem = XtVaCreateManagedWidget( "LoadItem",
			xmPushButtonWidgetClass,
			FilePane,
			RES_CONVERT( XmNlabelString, "Load Resources" ),
			RES_CONVERT( XmNmnemonic, "L" ),
			NULL );
	XtAddCallback( LoadItem, XmNactivateCallback,
		(XtCallbackProc) activateCB_LoadItem,
		(XtPointer) UxResourceContext );

	UxPutContext( LoadItem, (char *) UxResourceContext );


	/* Creation of ResetItem */
	ResetItem = XtVaCreateManagedWidget( "ResetItem",
			xmPushButtonWidgetClass,
			FilePane,
			RES_CONVERT( XmNlabelString, "Reset Resources" ),
			RES_CONVERT( XmNmnemonic, "R" ),
			NULL );
	XtAddCallback( ResetItem, XmNactivateCallback,
		(XtCallbackProc) activateCB_ResetItem,
		(XtPointer) UxResourceContext );

	UxPutContext( ResetItem, (char *) UxResourceContext );


	/* Creation of CloseItem */
	CloseItem = XtVaCreateManagedWidget( "CloseItem",
			xmPushButtonWidgetClass,
			FilePane,
			RES_CONVERT( XmNlabelString, "Close" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( CloseItem, XmNactivateCallback,
		(XtCallbackProc) activateCB_CloseItem,
		(XtPointer) UxResourceContext );

	UxPutContext( CloseItem, (char *) UxResourceContext );


	/* Creation of menu1_top_b1 */
	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "File" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, FilePane,
			NULL );
	UxPutContext( menu1_top_b1, (char *) UxResourceContext );


	XtAddCallback( Resource, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxResourceContext);

	XmMainWindowSetAreas( mainWindow1, menu1, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, panedWindow1 );

	return ( Resource );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_Resource( _UxStrutture, _UxDbase )
	PAGINA	*_UxStrutture;
	OlDatabaseTopologiaObject	_UxDbase;
{
	Widget                  rtrn;
	_UxCResource            *UxContext;

	UxResourceContext = UxContext =
		(_UxCResource *) UxNewContext( sizeof(_UxCResource), False );

	Strutture = _UxStrutture;
	Dbase = _UxDbase;

	{
		int iret;
		Boolean selezionato,Occupato;
		XmString StrLab; 
		char *Appoggio2,appoggio[100];
		WidgetClass classe_oggetto;
		
		char TitoloPag[120];
		Arg arg[3];
		Widget WdgAppoggio;
		
		OggettoS = NULL;
		
		DataBaseTo = Dbase;
		PagConf = Strutture;
		PaginaDrawing = (swidget )PagConf->drawing;
		PuntDB = PagConf->db;
		
		Occupato = False;
		OggettoS = NULL;
		rtrn = _Uxbuild_Resource();
printf("create_Resource:inizio \n");
		strcpy (TitoloPag,"Widget Resource Editor on page: ");
		strcat (TitoloPag,PagConf->nomepag);
		set_something (Resource, XmNtitle, (void*) TitoloPag);
		
		/*  Creazione dei rowcolumn di pagina  */
		CreaRowColumnRisorse (Widres_rc);
		
		/* Cerca nella drawing area l'oggetto selezionato */
		OggettoS = (Widget )XlGetSelectedWidget (PaginaDrawing);
		
		/*  Inserisce nella lista degli oggetti selezionati il nome dell
			oggetto selezionato e prepara i rowcolumn con le liste delle
			risorse  */
		
		if ( OggettoS != NULL )
			{
			/* Setta l'attributo dell'oggetto selezionato in editazione  */
			XtSetArg (arg[0], XlNinEdit, True);
			XtSetArg (arg[1], XlNwEdit, Resource);
			XtSetValues (OggettoS, arg, 2);
			/*  Setta l'attributo sensitive */
			set_something (pushButton1, XmNsensitive, (void*) True);
			set_something (ResetItem, XmNsensitive, (void*) True);
			PrepListaRes (Geometric_RC, OggettoS, XlRGrGeom, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (Specific_RC, OggettoS, XlRGrIo, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (Color_RC, OggettoS, XlRGrColor, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (VarCC1_rc, OggettoS, XlRGrVCC1, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (VarCC2_rc, OggettoS, XlRGrVCC2, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (VarCC3_rc, OggettoS, XlRGrVCC3, DataBaseTo,
							PagGetType(PagConf));
			PrepListaRes (VarCC4_rc, OggettoS, XlRGrVCC4, DataBaseTo,
							PagGetType(PagConf));
			strcpy (appoggio,WidGetName(OggettoS));
			strcat (appoggio,"  ----  ");
		
		        classe_oggetto = GetClass (OggettoS);
		        Appoggio2 = XtNewString( XlGetClassName(classe_oggetto) );
			strcat (appoggio,Appoggio2);
			XtFree (Appoggio2);
			}
		else
			{
			strcpy (appoggio,"No Widget selected");
			set_something (ResetItem, XmNsensitive, (void*) False);
			set_something (pushButton1, XmNsensitive, (void*) False);
			}
		
		/*  Crea l'OptionMenu che contiene le voci dei gruppi
			da visualizzare nella lista delle risorse   */
		
		iret = CreaOptionMenu (menu2,GruppiPane);
		if (iret == NON_ALLOCATA) printf ("Manca spazio per allocare OptMenu!\n");
		if (iret == OK) printf ("Option Menu creato correttamente\n");
		
		/*   Visualizza il rwocolumn che contiene le risorse di specific  */
		XtManageChild (Specific_RC);
		XtManageChild (Widres_rc);
		
		StrLab = XmStringCreateSimple (appoggio);
		XmListAddItemUnselected (scrolledList1, StrLab, 0);
		XmStringFree (StrLab);
		
		/*  Crea l'interfaccia del variable editor */
		RigaPerVar = (Widget *)XtCalloc ( 4 , sizeof(Widget) );
		TipoVariabile = (int *)XtCalloc ( 1 , sizeof(int) );
		
		VariabEdit = (Widget )create_VariableEditor (PaginaDrawing,RigaPerVar,Resource,
							TipoVariabile,Dbase);
		
		/*  Setta la posizione della interfaccia rispetto al parent  */
		InterfaceSetPos (Resource,PagConf->topwidget);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

