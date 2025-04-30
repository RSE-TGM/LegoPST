
/*******************************************************************************
       VariableEditor.c
       (Generated from interface file VariableEditor.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo VariableEditor.i
   tipo 
   release 5.2
   data 2/6/96
   reserved @(#)VariableEditor.i	5.2
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <string.h>
#include <Xl/Xl.h>
#include <Ol/OlDatabaseTopologia.h>
#include "VarEdit.h"
#include "PertDef.h"
#include "UniMis.h"
#include "res_edit.h"
#include "sim_param.h"

extern swidget create_OneList();
extern swidget create_TwoList();
extern swidget create_PerturbEditor();
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
	Widget	UxVariableEditor;
	Widget	Uxform2;
	Widget	UxlabVarType;
	Widget	Uxseparator1;
	Widget	Uxlabel1;
	Widget	UxpushButton3;
	Widget	Uxseparator2;
	Widget	UxpushButton8;
	Widget	UxpushButton7;
	Widget	UxOptMenu;
	Widget	UxAllItem;
	Widget	UxComponentItem;
	Widget	UxSubSystemItem;
	Widget	Uxmenu1;
	Widget	Uxlabel4;
	Widget	UxtextNomeModello;
	Widget	UxtextNomeBlocco;
	Widget	UxtextNomeVariabile;
	Widget	Uxlabel2;
	Widget	UxpushButton4;
	Widget	Uxlabel3;
	Widget	UxpushButton5;
	Widget	UxpushButton6;
	Widget	UxtextNomePerturbazione;
	Widget	Uxseparator8;
	Widget	UxlabComp;
	Widget	UxlabSubS;
	Widget	Uxlabel8;
	Widget	Uxlabel9;
	Widget	UxlabComponent;
	Widget	UxlabSubSystem;
	Widget	UxlabVariableType;
	Widget	UxUnitPane;
	Widget	UxUnitItem;
	Widget	UxUnit;
	Widget	Uxseparator6;
	Widget	UxpushButton13;
	Widget	UxElabPane;
	Widget	UxNOPItem;
	Widget	UxNOTItem;
	Widget	UxElab;
	swidget	UxDrawA;
	Widget	UxListaBottoni[6];
	Widget	*UxRowColInp;
	OlDatabaseTopologiaObject	UxDbf;
	int	*UxFiltroPerVar;
	int	UxTipoVariabile;
	unsigned char	*UxUniMisura;
	Widget	UxOggettoSelezionato;
	Widget	UxParent4;
	Boolean	UxFirstTime;
	swidget	UxListaDA;
	Widget	*UxRCOgget;
	Widget	UxPadrino;
	int	*UxTipoVar;
	OlDatabaseTopologiaObject	UxDbTp;
} _UxCVariableEditor;

static _UxCVariableEditor      *UxVariableEditorContext;
#define VariableEditor          UxVariableEditorContext->UxVariableEditor
#define form2                   UxVariableEditorContext->Uxform2
#define labVarType              UxVariableEditorContext->UxlabVarType
#define separator1              UxVariableEditorContext->Uxseparator1
#define label1                  UxVariableEditorContext->Uxlabel1
#define pushButton3             UxVariableEditorContext->UxpushButton3
#define separator2              UxVariableEditorContext->Uxseparator2
#define pushButton8             UxVariableEditorContext->UxpushButton8
#define pushButton7             UxVariableEditorContext->UxpushButton7
#define OptMenu                 UxVariableEditorContext->UxOptMenu
#define AllItem                 UxVariableEditorContext->UxAllItem
#define ComponentItem           UxVariableEditorContext->UxComponentItem
#define SubSystemItem           UxVariableEditorContext->UxSubSystemItem
#define menu1                   UxVariableEditorContext->Uxmenu1
#define label4                  UxVariableEditorContext->Uxlabel4
#define textNomeModello         UxVariableEditorContext->UxtextNomeModello
#define textNomeBlocco          UxVariableEditorContext->UxtextNomeBlocco
#define textNomeVariabile       UxVariableEditorContext->UxtextNomeVariabile
#define label2                  UxVariableEditorContext->Uxlabel2
#define pushButton4             UxVariableEditorContext->UxpushButton4
#define label3                  UxVariableEditorContext->Uxlabel3
#define pushButton5             UxVariableEditorContext->UxpushButton5
#define pushButton6             UxVariableEditorContext->UxpushButton6
#define textNomePerturbazione   UxVariableEditorContext->UxtextNomePerturbazione
#define separator8              UxVariableEditorContext->Uxseparator8
#define labComp                 UxVariableEditorContext->UxlabComp
#define labSubS                 UxVariableEditorContext->UxlabSubS
#define label8                  UxVariableEditorContext->Uxlabel8
#define label9                  UxVariableEditorContext->Uxlabel9
#define labComponent            UxVariableEditorContext->UxlabComponent
#define labSubSystem            UxVariableEditorContext->UxlabSubSystem
#define labVariableType         UxVariableEditorContext->UxlabVariableType
#define UnitPane                UxVariableEditorContext->UxUnitPane
#define UnitItem                UxVariableEditorContext->UxUnitItem
#define Unit                    UxVariableEditorContext->UxUnit
#define separator6              UxVariableEditorContext->Uxseparator6
#define pushButton13            UxVariableEditorContext->UxpushButton13
#define ElabPane                UxVariableEditorContext->UxElabPane
#define NOPItem                 UxVariableEditorContext->UxNOPItem
#define NOTItem                 UxVariableEditorContext->UxNOTItem
#define Elab                    UxVariableEditorContext->UxElab
#define DrawA                   UxVariableEditorContext->UxDrawA
#define ListaBottoni            UxVariableEditorContext->UxListaBottoni
#define RowColInp               UxVariableEditorContext->UxRowColInp
#define Dbf                     UxVariableEditorContext->UxDbf
#define FiltroPerVar            UxVariableEditorContext->UxFiltroPerVar
#define TipoVariabile           UxVariableEditorContext->UxTipoVariabile
#define UniMisura               UxVariableEditorContext->UxUniMisura
#define OggettoSelezionato      UxVariableEditorContext->UxOggettoSelezionato
#define Parent4                 UxVariableEditorContext->UxParent4
#define FirstTime               UxVariableEditorContext->UxFirstTime
#define ListaDA                 UxVariableEditorContext->UxListaDA
#define RCOgget                 UxVariableEditorContext->UxRCOgget
#define Padrino                 UxVariableEditorContext->UxPadrino
#define TipoVar                 UxVariableEditorContext->UxTipoVar
#define DbTp                    UxVariableEditorContext->UxDbTp



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_VariableEditor();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/**** ****************************************************
        Routine che imposta l'option menu per le unita' di misura

void ModificaOptionUnit
        (
	Push			--> Lista dei bottoni dell'option menu;
        Iniziale                --> Prima lettera del nome della
                                        variabile;
	Unita			--> Stringa delle informazioni per le unita
					di misura;
        )

        Valore di ritorno       --> Nessun valore di ritorno;
*********************************************************/
void ModificaOptionUnit (Widget *Push,char Iniziale,char *Unita)
{
Arg arg[2];
XmString XmEtichetta;
int pret,i,j,Indice;
Boolean Trovata;
char Vuota1[30],Vuota2[30],Str[30],Eti[30],UnitaSel[30];

/* Dalla stringa della unita' di misura in input ricava il nome della 
	unita' di misura  */
if (Unita != NULL)
  pret = sscanf (Unita,"%s %s %s",Vuota1,Vuota2,UnitaSel);
if ( (pret < 3) || (Unita==NULL) ) strcpy (UnitaSel,"");

Indice =0;

/*  Distrugge e reinizializza i bottoni dell'option menu delle
	unita' di misura  */
i=0;
while (i < 6)
	{
	if (Push[i] != NULL)
	  XtDestroyWidget (Push[i]);
	Push[i] = NULL;
	i++;
	}

Trovata = False;
i = 0;
#ifndef SCADA_MMI  /* nel caso scada-mmi non ricerca le unita' di misura */
if (Iniziale != '\0')
  {

  while ( (SelUniMis[i].type != '!') && (!Trovata) )
    {
    if (SelUniMis[i].type == Iniziale)
      { 
      Trovata = True;
      j=0;
      while ( SelUniMis[i].codm[j][0] != ' ' && j<6)
        {
        /*  Controlla la selezione di default  */
	if(strlen(UnitaSel)==0)
		Indice=0;
        else if (strncmp(SelUniMis[i].codm[j],UnitaSel,strlen(UnitaSel))==0)
			Indice = j;

        strcpy (Str,SelUniMis[i].codm[j]);
        XmEtichetta = XmStringCreateSimple (Str);
        XtSetArg (arg[0], XmNlabelString, XmEtichetta);
        Push[j] = XmCreatePushButton (UnitPane, "Bottone", arg, 1);
        XtManageChild (Push[j]);
        XmStringFree (XmEtichetta);
        j++;
        }
      }
      i++;
    } 
  }
#endif /* not SCADA_MMI */
/* Controlla se non e' stata trovato nessun tipo di variabile conosciuto
	oppure se il campo del nome della variabile era vuoto  */
if ( (!Trovata) || (Iniziale == '\0') )
  {
  for (i=0;i<5;i++)
    {
    strcpy (Str,SelUniMis[14].codm[i]);
    XmEtichetta = XmStringCreateSimple (Str);
    XtSetArg (arg[0], XmNlabelString, XmEtichetta);
    Push[i]= XmCreatePushButton (UnitPane, "Bottone", arg, 1);
    XtManageChild (Push[i]);
    XmStringFree (XmEtichetta);
    }
  }

set_something (Unit, XmNmenuHistory, (void*) Push[Indice]);

if (Trovata)
  {
  set_something (Unit, XmNsensitive, (void*) True);
  }
else set_something (Unit, XmNsensitive, (void*) False);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	mapCB_VariableEditor( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	char *NomeModello,*NomeBlocco,*NomeVar,*Perturba;
	XmString XmStrTipo;
	char *Interpretare;
	Boolean Corretta;
	int uguali;
	Widget Pane,*ListaB;
	char Description[100],*Stringa;
	char *Subs,*Comp;
	XmString XmStr;
	Boolean Giusto;
	Arg arg[2];
	
	TipoVariabile = FiltroPerVar[0];
	UniMisura = NULL;
	
	XmStrTipo = (XmString )ControllaTipoVar (TipoVariabile,pushButton6,Elab);
	set_something (labVariableType,XmNlabelString,(void*) XmStrTipo);
	XmStringFree (XmStrTipo);
	
#ifdef ON_MMI
	Corretta = False;
	
	/*  Disabilito il bottone di Grab della variabile  */
	set_something (pushButton13, XmNsensitive, (void*) False);
	
	  /*  Leggo dalla lista degli oggetti in input le label per i
		component e per i subsystem  */
	  Comp = XmTextFieldGetString (RowColInp[0]);
	  Subs = XmTextFieldGetString (RowColInp[1]);
	  
	  /*  Setta le label dei component e subsystem  */
	  get_something (menu1, XmNsubMenuId, (void*) &Pane);
	  get_something (Pane, XmNchildren, (void*) &ListaB);
	
	  if (Comp[0] == '\0')
	    set_something (ListaB[1], XmNsensitive, (void*) False);
	  else
	    set_something (ListaB[1], XmNsensitive, (void*) True);
	
	  if (Subs[0] == '\0')
	    set_something (ListaB[2], XmNsensitive, (void*) False);
	  else
	    set_something (ListaB[2], XmNsensitive, (void*) True);
	
	  XmStr = XmStringCreateSimple (Comp);
	  set_something (labComponent, XmNlabelString, (void*) XmStr);
	  XmStringFree (XmStr);
	  XmStr = XmStringCreateSimple (Subs);
	  set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	  XmStringFree (XmStr);
	
	/*  Leggo dal textfield ls stringa della definizione della variabile
		per poterla interpretare  */
	Interpretare = XmTextFieldGetString (RowColInp[2]);
	
	Corretta = OlReadVariableString (Dbf,Interpretare, &NomeModello, &NomeBlocco,
		&NomeVar, &Perturba, &UniMisura, TipoVariabile);
	
#else
	
	/*  Legge se c'e' la descrizione della variabile  */
	Giusto = False;
	strcpy (Description,XmTextFieldGetString (RowColInp[2]));
	
	if (Description[0] != '\0')
	  {
	/*
	 Se esiste in selezione un oggetto del tipo icona animata
	 assume come label del component e del subsystem le
	 label dell'oggetto in selezione
	*/
	  OggettoSelezionato = (Widget )XlGetSelectedWidget (DrawA);
	  if ( XlIsAnimateIcon (OggettoSelezionato) )
	    {
	    /*  Setta le label dei component e subsystem  */
	    XtSetArg (arg[0], XlNcomponent, &Comp);
	    XtSetArg (arg[1], XlNsubsystem, &Subs);
	    XtGetValues (OggettoSelezionato, arg, 2);
	    get_something (menu1, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	
	    if (Comp[0] == '\0')
	       set_something (ListaB[1], XmNsensitive, (void*) False);
	    else
	       set_something (ListaB[1], XmNsensitive, (void*) True);
	
	    if (Subs[0] == '\0')
	      set_something (ListaB[2], XmNsensitive, (void*) False);
	    else
	      set_something (ListaB[2], XmNsensitive, (void*) True);
	
	    XmStr = XmStringCreateSimple (Comp);
	    set_something (labComponent, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    XmStr = XmStringCreateSimple (Subs);
	    set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    }
	  else
	    {
	    XmStr = XmStringCreateSimple (" ");
	    set_something (labComponent, XmNlabelString, (void*) XmStr);
	    set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    get_something (menu1, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	    set_something (ListaB[1], XmNsensitive, (void*) False);
	    set_something (ListaB[2], XmNsensitive, (void*) False);
	    }
	  Giusto = OlReadVariableString (Dbf,Description,&NomeModello,
	                                &NomeBlocco,&NomeVar,&Perturba,&UniMisura,
					TipoVariabile);
	  }
	else
	  {
	/*
	 Se non e' specificato il nome della variabile di animazione
	 ed esiste in selezione una icona animata o display ne ricava
	 il nome della variabile di animazione: component
	 e subsystem vengono invece ricavati solo nel caso di icona 
	 animata.
	*/ 
	  OggettoSelezionato = (Widget )XlGetSelectedWidget (DrawA);
	  if ( XlIsAnimateIcon (OggettoSelezionato))
	    {
	    /*  Setta le label dei component e subsystem  */
	    XtSetArg (arg[0], XlNcomponent, &Comp);
	    XtSetArg (arg[1], XlNsubsystem, &Subs);
	    XtGetValues (OggettoSelezionato, arg, 2);
	
	    get_something (menu1, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	    if (Comp[0] == '\0')
	      set_something (ListaB[1], XmNsensitive, (void*) False);
	    else
	      set_something (ListaB[1], XmNsensitive, (void*) True);
	
	    if (Subs[0] == '\0')
	      set_something (ListaB[2], XmNsensitive, (void*) False);
	    else
	      set_something (ListaB[2], XmNsensitive, (void*) True);
	
	    XmStr = XmStringCreateSimple (Comp);
	    set_something (labComponent, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    XmStr = XmStringCreateSimple (Subs);
	    set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    }
	if(XlIsAnimateIcon(OggettoSelezionato) || XlIsIndic(OggettoSelezionato) )
	    {
	    /*  Prende se c'e' la prima variabile di Cambio Colore  */
	    strcpy (Description,(char *)XlGetFirstVcc (OggettoSelezionato));
	    if (Description[0] != '\0')
	      Giusto = OlReadVariableString (Dbf,Description,&NomeModello,
	                                &NomeBlocco,&NomeVar,&Perturba,&UniMisura,
					TipoVariabile);
	    else
	      {
	      NomeModello = XtCalloc (1,sizeof(char));
	      NomeBlocco = XtCalloc (1,sizeof(char));
	      NomeVar = XtCalloc (1,sizeof(char));
	      Perturba = XtCalloc (1,sizeof(char));
	      UniMisura = XtCalloc (1,sizeof(char));
	      }
	    }
	  else
	    {
	    XmStr = XmStringCreateSimple (" ");
	    set_something (labComponent, XmNlabelString, (void*) XmStr);
	    set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    get_something (menu1, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	    set_something (ListaB[1], XmNsensitive, (void*) False);
	    set_something (ListaB[2], XmNsensitive, (void*) False);
	    NomeModello = XtCalloc (1,sizeof(char));
	    NomeBlocco = XtCalloc (1,sizeof(char));
	    NomeVar = XtCalloc (1,sizeof(char));
	    Perturba = XtCalloc (1,sizeof(char));
	    UniMisura = XtCalloc (1,sizeof(char));
	    }
	  }
#endif
	
	
	/*  Setta a True i bottoni dei blocchi e delle variabili  */
	if (NomeModello[0] != '\0')
	  {
	  set_something (pushButton4, XmNsensitive, (void*) True);
	  set_something (pushButton5, XmNsensitive, (void*) True);
	  }
#ifdef SCADA_MMI
	/*
	 Unmanage degli oggetti non utilizzati
	*/
	XtUnmanageChild(labVarType);
	XtUnmanageChild(separator1);
	XtUnmanageChild(label1);
	XtUnmanageChild(pushButton3);
	XtUnmanageChild(menu1);
	XtUnmanageChild(label4);
	XtUnmanageChild(label9);
	XtUnmanageChild(textNomeModello);
	XtUnmanageChild(labVariableType);
	XtUnmanageChild(labComponent);
	XtUnmanageChild(labComp);
	XtUnmanageChild(labSubS);
	XtUnmanageChild(labSubSystem);
	XtUnmanageChild(Unit);
	XtUnmanageChild(separator8);
	/*
	 Riposizionamento degli oggetti
	 rimanenti
	*/
	set_something(XtParent(VariableEditor),XmNheight,(void*) 260);
	
	set_something(label2, XmNtopPosition, (void*) 2);
	/* modifica la label BLOCK nella label TYPE  */
	XmStr = XmStringCreateSimple ("TYPE:");
	set_something(label2, XmNlabelString,(void*) XmStr);
	XmStringFree (XmStr);
	set_something(label2, XmNbottomPosition, (void*) 10);
	
	set_something(textNomeBlocco, XmNtopPosition, (void*) 2);
	set_something(textNomeBlocco, XmNbottomPosition, (void*) 15);
	set_something(pushButton4, XmNtopPosition, (void*) 2);
	set_something(pushButton4, XmNbottomPosition, (void*) 15);
	
	set_something(label3, XmNtopPosition, (void*) 19);
	set_something(label3, XmNbottomPosition, (void*) 32);
	set_something(textNomeVariabile, XmNtopPosition, (void*) 19);
	set_something(textNomeVariabile, XmNbottomPosition, (void*) 32);
	set_something(pushButton5, XmNtopPosition, (void*) 19);
	set_something(pushButton5, XmNbottomPosition, (void*) 32);
	
	set_something(pushButton6, XmNtopPosition, (void*) 35);
	set_something(pushButton6, XmNbottomPosition, (void*) 48);
	set_something(textNomePerturbazione, XmNtopPosition, (void*) 35);
	set_something(textNomePerturbazione, XmNbottomPosition, (void*) 48);
	
	set_something(label8,XmNtopPosition, (void*) 54);
	set_something(label8,XmNbottomPosition, (void*) 60);
	set_something(label8,XmNleftPosition, (void*) 2);
	set_something(label8,XmNrightPosition,(void*) 45);
	set_something(Elab,XmNtopPosition, (void*) 51);
	set_something(Elab,XmNbottomPosition,(void*) 63);
	set_something(Elab,XmNleftPosition,(void*) 45);
	set_something(Elab,XmNrightPosition,(void*) 70);
	
	set_something(separator6,XmNtopPosition ,(void*) 64);
	set_something(separator6,XmNbottomPosition, (void*) 67);
	
	set_something(pushButton13,XmNtopPosition,(void*) 68);
	set_something(pushButton13,XmNbottomPosition,(void*) 78);
	
	
	
	set_something(separator2,XmNtopPosition ,(void*) 79);
	set_something(separator2,XmNbottomPosition, (void*) 83);
	
	set_something(pushButton7,XmNtopPosition ,(void*) 85);
	set_something(pushButton7,XmNbottomPosition ,(void*) 96);
	
	set_something(pushButton8,XmNtopPosition ,(void*) 85);
	set_something(pushButton8,XmNbottomPosition ,(void*) 96);
	
	 
	/*
	 Se la stringa di descrizione della variabile non e'
	 completa inserisce come modello SUPERV e come blocchi
	 ALL
	*/
	if (Giusto == False)
	  {
	   XtFree(NomeModello);
	   XtFree(NomeBlocco);
	   NomeModello=XtNewString("SUPERVIS");
	   NomeBlocco=XtNewString("ALL");
	   set_something (pushButton4, XmNsensitive, (void*) True);
	   set_something (pushButton5, XmNsensitive, (void*) True);
	   }
#ifdef ON_MMI
	/*
	 Rende impossibile modificare il tipo di variabile
	*/
	set_something (pushButton3, XmNsensitive,(void*) False);
	set_something (pushButton4, XmNsensitive,(void*) False);
#endif
#endif
	/*  Setta i textField  */
	XmTextFieldSetString (textNomeModello,NomeModello);
	XmTextFieldSetString (textNomeBlocco,NomeBlocco);
	XmTextFieldSetString (textNomeVariabile,NomeVar);
	if ( (TipoVariabile==XlRVarDO) || (TipoVariabile==XlRVarAO) || 
		(TipoVariabile==XlRVarXO) )
	  XmTextFieldSetString (textNomePerturbazione,Perturba);
	if ( (TipoVariabile==XlRVarDA) )
	  {
	  XmTextFieldSetString (textNomePerturbazione," ");
	  uguali = strncmp (Perturba,"NOP",3);
	  if (uguali == 0)
	    {
	    get_something (Elab, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	    set_something (Elab, XmNmenuHistory, (void*) ListaB[0]);
	    }
	  uguali = strncmp (Perturba,"NOT",3);
	  if (uguali == 0)
	    {
	    get_something (Elab, XmNsubMenuId, (void*) &Pane);
	    get_something (Pane, XmNchildren, (void*) &ListaB);
	    set_something (Elab, XmNmenuHistory, (void*) ListaB[1]);
	    }
	  }
	
	/*  Setta la posizione dell'interfaccia secondo la posizione del Parent  */
	InterfaceSetPos (VariableEditor,Parent4);
	
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	Widget Selezionato,*ListaWid;
	char *ListaModelli;
	char *p,*Appoggio,*NomeModello,*ListaAppoggio;
	int TipoOption,risul,NumeroBlocchi,NumeroModuli;
	XmString XmSel;
	char *Sel;
	char *ConfrontoAll="All";
	char *ConfrontoComponent="Component";
	char *ConfrontoSubSystem="Sub System";
	
	get_something (menu1, XmNmenuHistory, (void*) &Selezionato);
	get_something (Selezionato, XmNlabelString, (void*) &XmSel);
	Sel = (char *)extract_string (XmSel);
	
	risul = strcmp (ConfrontoSubSystem,Sel);
	if (risul == 0)
		{
		TipoOption = SUBSYSTEM;
		get_something (labSubSystem, XmNlabelString, (void*) &XmSel);
		Sel = (char *)extract_string (XmSel);
		ListaModelli = (char *)OlGetModelFromSubSystem (Dbf,Sel);	
		}
	else ListaModelli = (char *)OlElencoModelli (Dbf);
	
	ListaWid = (Widget *)XtCalloc ( 7,sizeof(Widget) );
	
	ListaWid[0] = (Widget )pushButton3;
	ListaWid[1] = (Widget )pushButton4;
	ListaWid[2] = (Widget )pushButton5;
	ListaWid[3] = (Widget )textNomeModello;
	ListaWid[4] = (Widget )textNomeBlocco;
	ListaWid[5] = (Widget )textNomeVariabile;
	ListaWid[6] = (Widget )NULL;
	
	UxPopupInterface (create_OneList(VariableEditor,ListaWid,MODELLI,ListaModelli),
						no_grab);
	XtFree (ListaModelli);
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
#ifdef ON_MMI
		XtDestroyWidget ( XtParent(VariableEditor) );
#else
		set_something (RowColInp[0], XmNsensitive, (void*) True);
		set_something (RowColInp[2], XmNsensitive, (void*) True);
		UxPopdownInterface (VariableEditor);
#endif
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	int i;
	WidgetClass ClasseOggetto;
	char *modello,*blocco,*variabile,*perturba,*NomeClasse;
	char *Stringa,*Messaggio,Appoggio[20];
	char *Fittizia;
	
	modello = XmTextFieldGetString (textNomeModello);
	blocco = XmTextFieldGetString (textNomeBlocco);
	variabile = XmTextFieldGetString (textNomeVariabile);
	perturba = XmTextFieldGetString (textNomePerturbazione);
	
	if ( (TipoVariabile==XlRVarDA) || (TipoVariabile==XlRVarAA) || 
	     (TipoVariabile==XlRVarXA) || (TipoVariabile==XlRVarXX) ||
	     (TipoVariabile==XlRVarAX) || (TipoVariabile==XlRVarDX) ||
	     (TipoVariabile==XlRVarAing) ||(TipoVariabile==XlRVarPlot) )
	  {
	  if ( (modello[0] == '\0')||(blocco[0] == '\0')||(variabile[0] == '\0') )
		{
		Messaggio = XtNewString ("Input was incomplete!");
		VisualizzaMessageBox (GENERICA, VariableEditor, Messaggio,ListaDA);
		XtFree (Messaggio);
		}
	  else
		{
		Stringa = (char *)WriteInpVariableString (Dbf,modello,blocco,
				variabile,Elab);
		Stringa = (char *)WriteUniMisura (Stringa,Unit);
	
#ifdef ON_MMI
		ClasseOggetto = XtClass (ListaDA);
		NomeClasse = XlGetClassName (ClasseOggetto);
	
		if (strcmp(NomeClasse,"Grafico")==0)
		  RisettaVariabile (Stringa,ListaDA);
		else
		  {
		  XmTextFieldSetString (RowColInp[3], Stringa );
		  if(TipoVariabile==XlRVarAing)
	          	ActivateVarAing (RowColInp);
		  if(TipoVariabile==XlRVarPlot)
	          	ActivateVarPlot (RowColInp);
		  }
#else
		XmTextFieldSetString (RowColInp[2], Stringa);
		set_something (RowColInp[0], XmNsensitive, (void*) True);
		set_something (RowColInp[2], XmNsensitive, (void*) True);
#endif
		XtFree (Stringa);
		XtFree (modello);
		XtFree (blocco);
		XtFree (variabile);
	
#ifdef ON_MMI
		XtDestroyWidget ( XtParent (VariableEditor) );
#else
		UxPopdownInterface (VariableEditor);
#endif
		}
	  }
	
	if ( (TipoVariabile==XlRVarDO) || (TipoVariabile==XlRVarAO) || 
	     (TipoVariabile==XlRVarXO) ) 
	  {
	  if ( (modello[0] == '\0')||(blocco[0] == '\0')||(variabile[0] == '\0')||
		(perturba[0] == '\0') )
		{
		Messaggio = XtNewString ("Input was incomplete!");
		VisualizzaMessageBox (GENERICA, VariableEditor, Messaggio,VariableEditor);
		XtFree (Messaggio);
		}
	  else
		{
		Stringa = (char *)WriteOutVariableString (Dbf,modello,blocco,
						variabile,perturba);
		Stringa = (char *)WriteUniMisura (Stringa,Unit);
	
		XmTextFieldSetString (RowColInp[2], Stringa);
		set_something (RowColInp[0], XmNsensitive, (void*) True);
		set_something (RowColInp[2], XmNsensitive, (void*) True);
		XtFree (Stringa);
		XtFree (modello);
		XtFree (blocco);
		XtFree (variabile);
		XtFree (perturba);
	
		UxPopdownInterface (VariableEditor);
		}
	  }
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	valueChangedCB_textNomeVariabile( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	char *NomeVar;
	
	NomeVar = XmTextFieldGetString (textNomeVariabile);
	ModificaOptionUnit (ListaBottoni,NomeVar[0],(char *)UniMisura);
	XtFree (NomeVar);
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	Widget Selezionato,*ListaWid,Wdg;
	char *ListaBlocchi,*ListaModuli;
	char *p;
	char *Appoggio="ALL";
	char *ListaDaDistruggere,*NomeModello,*ListaAppoggio,*ListaModuliFilter;
	int TipoOption,NumeroBlocchi,NumeroModuli;
	XmString XmSel;
	char *Sel,*Sel2;
	char *ConfrontoAll="All";
	char *ConfrontoComponent="Component";
	char *ConfrontoSubSystem="Sub System";
	
	NomeModello = XmTextFieldGetString (textNomeModello);
	
	get_something (menu1, XmNmenuHistory, (void*) &Selezionato);
	get_something (Selezionato, XmNlabelString, (void*) &XmSel);
	Sel = (char *)extract_string (XmSel);
	
	if (strcmp (ConfrontoComponent,Sel) == 0)
		{
		TipoOption = COMPONENT;
		get_something (labComponent, XmNlabelString, (void*) &XmSel);
		Sel2 = (char *)extract_string (XmSel);
		ListaModuli = (char *)OlElencoModuli (Dbf, NomeModello, Sel2);
		ListaModuliFilter = (char *)OlGetModulFromComponent (Dbf,Sel2);
		ListaModuli = (char *)FilterList (ListaModuli,ListaModuliFilter);
		ListaBlocchi = (char *)OlElencoBlocchi (Dbf,NomeModello,NULL);
		ListaBlocchi = (char *)BlocksFilterFromComponent (ListaBlocchi,
							ListaModuli);
		free (Sel2);
		/*********************
			Aggiunta alla lista moduli della voce "all"
		*********************/
		ListaDaDistruggere = ListaModuli;
		memcpy (&NumeroModuli , ListaModuli, sizeof(int) );
		NumeroModuli++;
		ListaModuli += sizeof(int);
		ListaAppoggio = (char *)XtCalloc ( sizeof(int)+(NumeroModuli*5),
					sizeof(char) );
		memcpy (ListaAppoggio, &NumeroModuli, sizeof(int) ); 
		p = ListaAppoggio;
		p += sizeof(int);
		strncpy (p, Appoggio, strlen(Appoggio));
		p += 5;
		memcpy (p, ListaModuli, (NumeroModuli-1)*5);
		free (ListaDaDistruggere);
		ListaModuli = ListaAppoggio;	 
		}
	
	if (strcmp (ConfrontoSubSystem,Sel) == 0)
		{
		TipoOption = SUBSYSTEM;
		get_something (labSubSystem, XmNlabelString, (void*) &XmSel);
		Sel2 = (char *)extract_string (XmSel);
		ListaBlocchi = (char *)OlGetBlocksFromSubSystem (Dbf,NomeModello,Sel2);
		ListaModuli = NULL;
		free (Sel2);
		}
	
	if (strcmp (ConfrontoAll, Sel) == 0)
		{
		TipoOption = ALL;
		ListaModuli = (char *)OlElencoModuli (Dbf, NomeModello, NULL);
		ListaBlocchi = (char *)OlElencoBlocchi (Dbf, NomeModello, NULL);
		/*********************
			Aggiunta alla lista moduli della voce "all"
		*********************/
		ListaDaDistruggere = ListaModuli;
		memcpy (&NumeroModuli , ListaModuli, sizeof(int) );
		NumeroModuli++;
		ListaModuli += sizeof(int);
		ListaAppoggio = (char *)XtCalloc ( sizeof(int)+(NumeroModuli*5),
					sizeof(char) );
		memcpy (ListaAppoggio, &NumeroModuli, sizeof(int) ); 
		p = ListaAppoggio;
		p += sizeof(int);
		strncpy (p, Appoggio, strlen(Appoggio));
		p += 5;
		memcpy (p, ListaModuli, (NumeroModuli-1)*5);
		free (ListaDaDistruggere);
		ListaModuli = ListaAppoggio;	 
		}
	
	/********************
		Aggiunta alla lista blocchi della voce "all"  
	********************/
	ListaDaDistruggere = ListaBlocchi;
	memcpy (&NumeroBlocchi , ListaBlocchi, sizeof(int) );
	NumeroBlocchi++;
	ListaBlocchi += sizeof(int);
	ListaAppoggio = (char *)XtCalloc ( sizeof(int)+(NumeroBlocchi*
					MAX_LUN_NOME_BLOCCO), sizeof(char) );
	memcpy (ListaAppoggio, &NumeroBlocchi, sizeof(int) ); 
	p = ListaAppoggio;
	p += sizeof(int);
	memcpy (p, Appoggio, strlen(Appoggio));
	p += MAX_LUN_NOME_BLOCCO;
	memcpy (p, ListaBlocchi, (NumeroBlocchi-1)*MAX_LUN_NOME_BLOCCO);
	free (ListaDaDistruggere);
	ListaBlocchi = ListaAppoggio;	 
	
	ListaWid = (Widget *)XtCalloc ( 5,sizeof(Widget) );
	
	ListaWid[0] = (Widget )pushButton4;
	ListaWid[1] = (Widget )textNomeBlocco;
	ListaWid[2] = (Widget )textNomeVariabile;
	ListaWid[3] = (Widget )textNomeModello;
	ListaWid[4] = (Widget )NULL;
	
	UxPopupInterface (create_TwoList(VariableEditor,Dbf,ListaWid,ListaModuli,ListaBlocchi,
							TipoOption,OggettoSelezionato),
							no_grab);
	free (ListaModuli);
	free (ListaBlocchi);
	free (NomeModello);
	free (Sel);
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	int Indice,risul;
	Widget *ListaWid;
	char *ListaVariabili;
	char *BloccoAppoggio,*p,*PerVar,*Token;
	char *Confronto="ALL";
	char *NomeModello,*BloccoSel;
	char *Appoggio;
	
	ListaWid = (Widget *)XtCalloc ( 3,sizeof(Widget) );
	
	ListaWid[0] = (Widget )pushButton5;
	ListaWid[1] = (Widget )textNomeVariabile;
	ListaWid[2] = (Widget )NULL;
	
	NomeModello = (char *)XmTextFieldGetString (textNomeModello);
	
	BloccoSel = (char *)XmTextFieldGetString (textNomeBlocco);
	risul = strcmp (Confronto,BloccoSel);
	
	if ( risul == 0 )
		{
		ListaVariabili = (char *)OlElencoVariabili (Dbf,NomeModello,NULL,
								TipoVariabile);
		}
	else
		{
		BloccoAppoggio = XtNewString (BloccoSel);
		Appoggio = BloccoAppoggio;
		Token = strstr (BloccoAppoggio,"---->");
	
		if (Token == NULL)
			{
			PerVar = (char *)XtCalloc (sizeof(int)+MAX_LUN_NOME_BLOCCO,
								sizeof(char));
			p = PerVar;
			Indice = 1;
			memcpy (p, &Indice, sizeof(int) );
			p+= sizeof(int);
			strncpy (p, BloccoSel,strlen (BloccoSel));
			}
		else
			{
			Token = strtok (BloccoAppoggio,"---->");
			Token = strtok (NULL,"<----");
			Token ++;
			PerVar = (char *)OlElencoBlocchi (Dbf,NomeModello,Token);
			}
	
		ListaVariabili = (char *)OlElencoVariabili (Dbf,NomeModello,PerVar,
								TipoVariabile);
		XtFree (PerVar);
		XtFree (Appoggio);
		}
	
	UxPopupInterface (create_OneList(VariableEditor,ListaWid,VARIABILI,ListaVariabili),
						no_grab);
	XtFree (NomeModello);
	XtFree (BloccoSel);
	XtFree (ListaVariabili);
	
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	char *Stringa;
	
	Stringa = (char *)XmTextFieldGetString (textNomePerturbazione);
	
	UxPopupInterface (create_PerturbEditor(VariableEditor,Stringa,
						textNomePerturbazione), no_grab);
	}
	UxVariableEditorContext = UxSaveCtx;
}

static	void	activateCB_pushButton13( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCVariableEditor      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVariableEditorContext;
	UxVariableEditorContext = UxContext =
			(_UxCVariableEditor *) UxGetContext( UxWidget );
	{
	char *Unita,*Comp,*Subs,*Stringa,*NomeMod,*NomeBl,*NomeVa,*Pert;
	Boolean IconaAnimata,Giusto;
	char Messaggio[100];
	Widget Odg,*ListaB,Pane;
	XmString XmStr;
	Arg arg[3];
	int uguali;
	
	char *Nulla='\0';
	
	Odg = (Widget )XlGetSelectedWidget (DrawA);
	if (Odg != NULL)
	  {
	  IconaAnimata = XlIsAnimateIcon (Odg) || XlIsIndic(Odg);
	/*
	 Se l'oggetto in selezione e' del tipo icona animata ricava 
	 anche component e subsystem
	*/
	  if (XlIsAnimateIcon (Odg))
	    {
	    XtSetArg (arg[0], XlNcomponent, &Comp);
	    XtSetArg (arg[1], XlNsubsystem, &Subs);
	    XtGetValues (Odg, arg, 2);
	    XmStr = XmStringCreateSimple (Comp);
	    set_something (labComponent, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    XmStr = XmStringCreateSimple (Subs);
	    set_something (labSubSystem, XmNlabelString, (void*) XmStr);
	    XmStringFree (XmStr);
	    }
	/*
	 Se l'oggetto in selezione e' icona animata o indicatore
	 ricava il nome della variabile cambio colore
	*/
	  if(XlIsAnimateIcon (Odg) || XlIsIndic(Odg))
	    {
	    Stringa = (char *)XlGetFirstVcc (Odg);
	    if ( (Stringa == NULL) || (Stringa[0]=='\0') )
	      {
	      strcpy (Messaggio,"Variable not defined!");
	      VisualizzaMessageBox (GENERICA, VariableEditor,Messaggio,Odg);
	      }
	    else
	      {
	      Giusto = OlReadVariableString (Dbf,Stringa,&NomeMod,&NomeBl,&NomeVa,
	                                       &Pert,&UniMisura,TipoVariabile);
	      if (Giusto)
	        {
		XmTextFieldSetString (textNomeModello,NomeMod);
		XmTextFieldSetString (textNomeBlocco,NomeBl);
		XmTextFieldSetString (textNomeVariabile,NomeVa);
		if ( (TipoVariabile==XlRVarDO) || (TipoVariabile==XlRVarAO) || 
			(TipoVariabile==XlRVarXO) )
		  XmTextFieldSetString (textNomePerturbazione,Pert);
		if ( (TipoVariabile==XlRVarDA) )
		  {
		  XmTextFieldSetString (textNomePerturbazione,Nulla);
		  uguali = strncmp (Pert,"NOP",3);
		  if (uguali == 0)
		    {
		    get_something (Elab, XmNsubMenuId, (void*) &Pane);
		    get_something (Pane, XmNchildren, (void*) &ListaB);
		    set_something (Elab, XmNmenuHistory, (void*) ListaB[1]);
		    }
		  uguali = strncmp (Pert,"NOT",3);
		  if (uguali == 0)
		    {
		    get_something (Elab, XmNsubMenuId, (void*) &Pane);
		    get_something (Pane, XmNchildren, (void*) &ListaB);
		    set_something (Elab, XmNmenuHistory, (void*) ListaB[0]);
		    }
		  }
		set_something (pushButton4,XmNsensitive,(void*) True);
		set_something (pushButton5,XmNsensitive,(void*) True);
		}
	      else
	        {
		strcpy (Messaggio,"Error in read variable");
		VisualizzaMessageBox (GENERICA,VariableEditor,Messaggio,Odg); 
		}
	      }
	    }
	  else
	    {
	    strcpy (Messaggio,"Selected Widget must be AnimateIcon class!");
	    VisualizzaMessageBox (GENERICA,VariableEditor,Messaggio,Odg);
	    }
	  }
	else
	  {
	  strcpy (Messaggio,"Must be selected one object!");
	  VisualizzaMessageBox (GENERICA,VariableEditor,Messaggio,Odg);
	  }
	}
	UxVariableEditorContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_VariableEditor()
{
	Widget		_UxParent;
	Widget		OptMenu_shell;
	Widget		UnitPane_shell;
	Widget		ElabPane_shell;


	/* Creation of VariableEditor */
	_UxParent = XtVaCreatePopupShell( "VariableEditor_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 499,
			XmNy, 374,
			XmNwidth, 430,
			XmNheight, 512,
			XmNallowShellResize, FALSE,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "VariableEditor",
			NULL );

	VariableEditor = XtVaCreateWidget( "VariableEditor",
			xmFormWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 430,
			XmNheight, 512,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			NULL );
	XtAddCallback( VariableEditor, XmNmapCallback,
		(XtCallbackProc) mapCB_VariableEditor,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( VariableEditor, (char *) UxVariableEditorContext );


	/* Creation of form2 */
	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass,
			VariableEditor,
			XmNunitType, XmPIXELS,
			XmNx, 0,
			XmNy, 0,
			XmNnoResize, FALSE,
			XmNrightAttachment, XmATTACH_SELF,
			XmNleftAttachment, XmATTACH_SELF,
			XmNbottomAttachment, XmATTACH_SELF,
			XmNtopAttachment, XmATTACH_SELF,
			XmNheight, 550,
			XmNwidth, 450,
			NULL );
	UxPutContext( form2, (char *) UxVariableEditorContext );


	/* Creation of labVarType */
	labVarType = XtVaCreateManagedWidget( "labVarType",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 10,
			XmNwidth, 100,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Variable type:" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopPosition, 2,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 6,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 25,
			NULL );
	UxPutContext( labVarType, (char *) UxVariableEditorContext );


	/* Creation of separator1 */
	separator1 = XtVaCreateManagedWidget( "separator1",
			xmSeparatorWidgetClass,
			form2,
			XmNx, 2,
			XmNy, 60,
			XmNwidth, 416,
			XmNheight, 10,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNrightOffset, 0,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNtopPosition, 21,
			XmNbottomPosition, 23,
			XmNleftPosition, 1,
			XmNrightPosition, 99,
			NULL );
	UxPutContext( separator1, (char *) UxVariableEditorContext );


	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 80,
			XmNwidth, 90,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "MODEL:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 2,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 22,
			XmNtopPosition, 24,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 32,
			NULL );
	UxPutContext( label1, (char *) UxVariableEditorContext );


	/* Creation of pushButton3 */
	pushButton3 = XtVaCreateManagedWidget( "pushButton3",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 320,
			XmNy, 80,
			XmNwidth, 50,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "List.." ),
			XmNsensitive, TRUE,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftPosition, 80,
			XmNrightPosition, 95,
			XmNtopPosition, 24,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 32,
			NULL );
	XtAddCallback( pushButton3, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton3,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton3, (char *) UxVariableEditorContext );


	/* Creation of separator2 */
	separator2 = XtVaCreateManagedWidget( "separator2",
			xmSeparatorWidgetClass,
			form2,
			XmNx, 2,
			XmNy, 280,
			XmNwidth, 406,
			XmNheight, 10,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNbottomPosition, 92,
			XmNtopPosition, 90,
			XmNleftPosition, 1,
			XmNrightPosition, 99,
			NULL );
	UxPutContext( separator2, (char *) UxVariableEditorContext );


	/* Creation of pushButton8 */
	pushButton8 = XtVaCreateManagedWidget( "pushButton8",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 100,
			XmNy, 300,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "CANCEL" ),
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNbottomPosition, 99,
			XmNtopPosition, 93,
			XmNleftPosition, 27,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 47,
			NULL );
	XtAddCallback( pushButton8, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton8,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton8, (char *) UxVariableEditorContext );


	/* Creation of pushButton7 */
	pushButton7 = XtVaCreateManagedWidget( "pushButton7",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 300,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNbottomPosition, 99,
			XmNtopPosition, 93,
			XmNleftPosition, 5,
			XmNrightPosition, 25,
			NULL );
	XtAddCallback( pushButton7, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton7,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton7, (char *) UxVariableEditorContext );


	/* Creation of OptMenu */
	OptMenu_shell = XtVaCreatePopupShell ("OptMenu_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	OptMenu = XtVaCreateWidget( "OptMenu",
			xmRowColumnWidgetClass,
			OptMenu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( OptMenu, (char *) UxVariableEditorContext );


	/* Creation of AllItem */
	AllItem = XtVaCreateManagedWidget( "AllItem",
			xmPushButtonWidgetClass,
			OptMenu,
			RES_CONVERT( XmNlabelString, "All" ),
			NULL );
	UxPutContext( AllItem, (char *) UxVariableEditorContext );


	/* Creation of ComponentItem */
	ComponentItem = XtVaCreateManagedWidget( "ComponentItem",
			xmPushButtonWidgetClass,
			OptMenu,
			RES_CONVERT( XmNlabelString, "Component" ),
			NULL );
	UxPutContext( ComponentItem, (char *) UxVariableEditorContext );


	/* Creation of SubSystemItem */
	SubSystemItem = XtVaCreateManagedWidget( "SubSystemItem",
			xmPushButtonWidgetClass,
			OptMenu,
			RES_CONVERT( XmNlabelString, "Sub System" ),
			NULL );
	UxPutContext( SubSystemItem, (char *) UxVariableEditorContext );


	/* Creation of menu1 */
	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, OptMenu,
			XmNx, 290,
			XmNy, 30,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNleftPosition, 73,
			XmNrightPosition, 98,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopPosition, 8,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 14,
			NULL );
	UxPutContext( menu1, (char *) UxVariableEditorContext );


	/* Creation of label4 */
	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass,
			form2,
			XmNx, 300,
			XmNy, 10,
			XmNwidth, 90,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Select from:" ),
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNleftPosition, 73,
			XmNrightPosition, 96,
			XmNtopPosition, 2,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 6,
			NULL );
	UxPutContext( label4, (char *) UxVariableEditorContext );


	/* Creation of textNomeModello */
	textNomeModello = XtVaCreateManagedWidget( "textNomeModello",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 110,
			XmNy, 80,
			XmNwidth, 200,
			XmNheight, 40,
			XmNmaxLength, 200,
			XmNsensitive, TRUE,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNcolumns, 100,
			XmNeditable, FALSE,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNcursorPositionVisible, FALSE,
			XmNleftPosition, 25,
			XmNrightPosition, 77,
			XmNtopPosition, 24,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 32,
			NULL );
	UxPutContext( textNomeModello, (char *) UxVariableEditorContext );


	/* Creation of textNomeBlocco */
	textNomeBlocco = XtVaCreateManagedWidget( "textNomeBlocco",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 110,
			XmNy, 130,
			XmNwidth, 200,
			XmNheight, 40,
			XmNmaxLength, 200,
			XmNsensitive, TRUE,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNcolumns, 100,
			XmNeditable, FALSE,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNcursorPositionVisible, FALSE,
			XmNleftPosition, 25,
			XmNrightPosition, 77,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 42,
			XmNtopPosition, 34,
			NULL );
	UxPutContext( textNomeBlocco, (char *) UxVariableEditorContext );


	/* Creation of textNomeVariabile */
	textNomeVariabile = XtVaCreateManagedWidget( "textNomeVariabile",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 110,
			XmNy, 180,
			XmNwidth, 200,
			XmNheight, 40,
			XmNmaxLength, 200,
			XmNsensitive, TRUE,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNcolumns, 100,
			XmNeditable, FALSE,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNcursorPositionVisible, FALSE,
			XmNleftPosition, 25,
			XmNrightPosition, 77,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 52,
			XmNtopPosition, 44,
			NULL );
	XtAddCallback( textNomeVariabile, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_textNomeVariabile,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( textNomeVariabile, (char *) UxVariableEditorContext );


	/* Creation of label2 */
	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 130,
			XmNwidth, 90,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "BLOCK:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 2,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 22,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 42,
			XmNtopPosition, 34,
			NULL );
	UxPutContext( label2, (char *) UxVariableEditorContext );


	/* Creation of pushButton4 */
	pushButton4 = XtVaCreateManagedWidget( "pushButton4",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 320,
			XmNy, 130,
			XmNwidth, 50,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "List.." ),
			XmNsensitive, FALSE,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftPosition, 80,
			XmNrightPosition, 95,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 42,
			XmNtopPosition, 34,
			NULL );
	XtAddCallback( pushButton4, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton4,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton4, (char *) UxVariableEditorContext );


	/* Creation of label3 */
	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 180,
			XmNwidth, 90,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "VARIABLE:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 2,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 22,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 52,
			XmNtopPosition, 44,
			NULL );
	UxPutContext( label3, (char *) UxVariableEditorContext );


	/* Creation of pushButton5 */
	pushButton5 = XtVaCreateManagedWidget( "pushButton5",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 320,
			XmNy, 180,
			XmNwidth, 50,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "List.." ),
			XmNsensitive, FALSE,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNleftPosition, 80,
			XmNrightPosition, 95,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 52,
			XmNtopPosition, 44,
			NULL );
	XtAddCallback( pushButton5, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton5,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton5, (char *) UxVariableEditorContext );


	/* Creation of pushButton6 */
	pushButton6 = XtVaCreateManagedWidget( "pushButton6",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 230,
			XmNwidth, 90,
			XmNheight, 40,
			RES_CONVERT( XmNlabelString, "PERTURB." ),
			XmNsensitive, TRUE,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNrecomputeSize, FALSE,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 2,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 22,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 62,
			XmNtopPosition, 54,
			NULL );
	XtAddCallback( pushButton6, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton6,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton6, (char *) UxVariableEditorContext );


	/* Creation of textNomePerturbazione */
	textNomePerturbazione = XtVaCreateManagedWidget( "textNomePerturbazione",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 110,
			XmNy, 230,
			XmNwidth, 200,
			XmNheight, 40,
			XmNmaxLength, 200,
			XmNsensitive, TRUE,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNcolumns, 100,
			XmNeditable, FALSE,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNcursorPositionVisible, FALSE,
			XmNleftPosition, 25,
			XmNrightPosition, 77,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 62,
			XmNtopPosition, 54,
			NULL );
	UxPutContext( textNomePerturbazione, (char *) UxVariableEditorContext );


	/* Creation of separator8 */
	separator8 = XtVaCreateManagedWidget( "separator8",
			xmSeparatorWidgetClass,
			form2,
			XmNx, 280,
			XmNy, 10,
			XmNwidth, 10,
			XmNheight, 90,
			XmNorientation, XmVERTICAL,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNbottomWidget, NULL,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNleftPosition, 67,
			XmNrightPosition, 70,
			XmNtopPosition, 1,
			XmNbottomPosition, 20,
			NULL );
	UxPutContext( separator8, (char *) UxVariableEditorContext );


	/* Creation of labComp */
	labComp = XtVaCreateManagedWidget( "labComp",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 40,
			XmNwidth, 100,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Component:" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopPosition, 8,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 12,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 25,
			NULL );
	UxPutContext( labComp, (char *) UxVariableEditorContext );


	/* Creation of labSubS */
	labSubS = XtVaCreateManagedWidget( "labSubS",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 70,
			XmNwidth, 100,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Sub System:" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopPosition, 14,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 18,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 25,
			NULL );
	UxPutContext( labSubS, (char *) UxVariableEditorContext );


	/* Creation of label8 */
	label8 = XtVaCreateManagedWidget( "label8",
			xmLabelWidgetClass,
			form2,
			XmNx, 250,
			XmNy, 320,
			XmNwidth, 150,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Input Variable Operation" ),
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 68,
			XmNtopPosition, 64,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 55,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 97,
			NULL );
	UxPutContext( label8, (char *) UxVariableEditorContext );


	/* Creation of label9 */
	label9 = XtVaCreateManagedWidget( "label9",
			xmLabelWidgetClass,
			form2,
			XmNx, 40,
			XmNy, 320,
			XmNwidth, 150,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Unit Variable" ),
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 68,
			XmNtopPosition, 64,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 10,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 45,
			NULL );
	UxPutContext( label9, (char *) UxVariableEditorContext );


	/* Creation of labComponent */
	labComponent = XtVaCreateManagedWidget( "labComponent",
			xmLabelWidgetClass,
			form2,
			XmNx, 130,
			XmNy, 40,
			XmNwidth, 150,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 5,
			XmNleftWidget, NULL,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 5,
			XmNrightWidget, NULL,
			XmNleftPosition, 25,
			XmNrightPosition, 65,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopPosition, 8,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 12,
			NULL );
	UxPutContext( labComponent, (char *) UxVariableEditorContext );


	/* Creation of labSubSystem */
	labSubSystem = XtVaCreateManagedWidget( "labSubSystem",
			xmLabelWidgetClass,
			form2,
			XmNx, 130,
			XmNy, 70,
			XmNwidth, 150,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 5,
			XmNleftWidget, NULL,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 5,
			XmNrightWidget, NULL,
			XmNleftPosition, 25,
			XmNrightPosition, 65,
			XmNtopPosition, 14,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 18,
			NULL );
	UxPutContext( labSubSystem, (char *) UxVariableEditorContext );


	/* Creation of labVariableType */
	labVariableType = XtVaCreateManagedWidget( "labVariableType",
			xmLabelWidgetClass,
			form2,
			XmNx, 130,
			XmNy, 10,
			XmNwidth, 150,
			XmNheight, 20,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "" ),
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 5,
			XmNleftWidget, NULL,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 5,
			XmNrightWidget, NULL,
			XmNleftPosition, 25,
			XmNrightPosition, 65,
			XmNtopPosition, 2,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 6,
			NULL );
	UxPutContext( labVariableType, (char *) UxVariableEditorContext );


	/* Creation of UnitPane */
	UnitPane_shell = XtVaCreatePopupShell ("UnitPane_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	UnitPane = XtVaCreateWidget( "UnitPane",
			xmRowColumnWidgetClass,
			UnitPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( UnitPane, (char *) UxVariableEditorContext );


	/* Creation of UnitItem */
	UnitItem = XtVaCreateManagedWidget( "UnitItem",
			xmPushButtonWidgetClass,
			UnitPane,
			RES_CONVERT( XmNlabelString, "Da Fare" ),
			NULL );
	UxPutContext( UnitItem, (char *) UxVariableEditorContext );


	/* Creation of Unit */
	Unit = XtVaCreateManagedWidget( "Unit",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, UnitPane,
			XmNx, 70,
			XmNy, 340,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 76,
			XmNtopPosition, 70,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 13,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 40,
			NULL );
	UxPutContext( Unit, (char *) UxVariableEditorContext );


	/* Creation of separator6 */
	separator6 = XtVaCreateManagedWidget( "separator6",
			xmSeparatorWidgetClass,
			form2,
			XmNx, 2,
			XmNy, 370,
			XmNwidth, 428,
			XmNheight, 30,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomPosition, 80,
			XmNtopPosition, 78,
			XmNleftPosition, 1,
			XmNrightPosition, 99,
			NULL );
	UxPutContext( separator6, (char *) UxVariableEditorContext );


	/* Creation of pushButton13 */
	pushButton13 = XtVaCreateManagedWidget( "pushButton13",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 20,
			XmNy, 320,
			XmNwidth, 370,
			XmNheight, 40,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 0,
			XmNbottomWidget, NULL,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			RES_CONVERT( XmNlabelString, "GRAB SELECTED WIDGET" ),
			XmNbottomPosition, 88,
			XmNtopPosition, 82,
			XmNleftPosition, 5,
			XmNrightPosition, 95,
			NULL );
	XtAddCallback( pushButton13, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton13,
		(XtPointer) UxVariableEditorContext );

	UxPutContext( pushButton13, (char *) UxVariableEditorContext );


	/* Creation of ElabPane */
	ElabPane_shell = XtVaCreatePopupShell ("ElabPane_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	ElabPane = XtVaCreateWidget( "ElabPane",
			xmRowColumnWidgetClass,
			ElabPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( ElabPane, (char *) UxVariableEditorContext );


	/* Creation of NOPItem */
	NOPItem = XtVaCreateManagedWidget( "NOPItem",
			xmPushButtonWidgetClass,
			ElabPane,
			RES_CONVERT( XmNlabelString, "NOP" ),
			NULL );
	UxPutContext( NOPItem, (char *) UxVariableEditorContext );


	/* Creation of NOTItem */
	NOTItem = XtVaCreateManagedWidget( "NOTItem",
			xmPushButtonWidgetClass,
			ElabPane,
			RES_CONVERT( XmNlabelString, "Not" ),
			NULL );
	UxPutContext( NOTItem, (char *) UxVariableEditorContext );


	/* Creation of Elab */
	Elab = XtVaCreateManagedWidget( "Elab",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, ElabPane,
			XmNx, 290,
			XmNy, 340,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNtopOffset, 0,
			XmNtopWidget, NULL,
			XmNbottomAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 76,
			XmNtopPosition, 70,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftPosition, 60,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 90,
			NULL );
	UxPutContext( Elab, (char *) UxVariableEditorContext );


	XtAddCallback( VariableEditor, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxVariableEditorContext);


	return ( VariableEditor );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_VariableEditor( _UxListaDA, _UxRCOgget, _UxPadrino, _UxTipoVar, _UxDbTp )
	swidget	_UxListaDA;
	Widget	*_UxRCOgget;
	Widget	_UxPadrino;
	int	*_UxTipoVar;
	OlDatabaseTopologiaObject	_UxDbTp;
{
	Widget                  rtrn;
	_UxCVariableEditor      *UxContext;

	UxVariableEditorContext = UxContext =
		(_UxCVariableEditor *) UxNewContext( sizeof(_UxCVariableEditor), False );

	ListaDA = _UxListaDA;
	RCOgget = _UxRCOgget;
	Padrino = _UxPadrino;
	TipoVar = _UxTipoVar;
	DbTp = _UxDbTp;

	{
		char *Description,*NomeModello,*NomeBlocco,*NomeVar,*Perturba;
		Boolean  Giusto;
		
		DrawA = ListaDA;
		RowColInp = RCOgget;
		FiltroPerVar = TipoVar;
		Dbf = DbTp;
		Parent4 = Padrino;
		
		FirstTime = True;
		rtrn = _Uxbuild_VariableEditor();

		ListaBottoni[0] = (Widget )UnitItem;
		ListaBottoni[1] = (Widget )NULL;
		ListaBottoni[2] = (Widget )NULL;
		ListaBottoni[3] = (Widget )NULL;
		ListaBottoni[4] = (Widget )NULL;
		ListaBottoni[5] = (Widget )NULL;
		
		/*  Setta la posizione dell'interfaccia secondo il Parent 
		InterfaceSetPos (VariableEditor,Parent4);*/
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

