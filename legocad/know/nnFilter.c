
/*******************************************************************************
	nnFilter.c
	(Generated from interface file nnFilter.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/Separator.h>
#include <Xm/ArrowB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/BulletinB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo nnFilter.i
   tipo 
   release 1.13
   data 11/13/95
   reserved @(#)nnFilter.i	1.13
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif


/* nnFilter.c       (ancillare di nnEdit.c)
* Scopo:       filtra variabili per nnEdit.
* Dati:        da files 'f03.dat', assunti precaricati.
* Risultati:   se Ok, elenco variabili selezionate.
* Note:        si auto-distrugge all'uscita.
* Autore:      C.Lusso (CISE) 1994
*/


#include <Xm/RowColumn.h>
#include "f03.h"
#include "libutilx.h"
#include "helpkit.h"

/* dati globali: - */
#define NNF_TEXT_MAXLEN 20
static String
 nnFzHpage = "nnFilter";
static Find_struct
 fsFindBloc;
/* funzione in "nnEdit.c" */
extern void fnnF_ritorna_E( int *);

/* dati di istanza:
Widget
 nnF_help;      pagina privata di Aiuto (bozza)
int
 cVars,         conta variabili totali (=neqsis+nu)
 cSelVars,      conta variabili selezionate
 cSelOut,       conta out-var selezionate
 cSelInp;       conta inp-var selezionate
Boolean
 oTextChg,      spia se testo del Filtro cambia
 *abSelVars;    elenco [cVars] spie variabili selezionate
char
 *phFilter;     punta testo del Filtro
*/

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
	Widget	UxnnFilter;
	Widget	UxnnF_labCaso;
	Widget	UxnnF_labMod;
	Widget	UxnnF_swinMod;
	Widget	UxnnF_slistMod;
	Widget	UxnnF_aroBloc;
	Widget	UxnnF_labBloc;
	Widget	UxnnF_swinBloc;
	Widget	UxnnF_slistBloc;
	Widget	UxnnF_aroVars;
	Widget	UxnnF_sep;
	Widget	UxnnF_labFilter;
	Widget	UxnnF_textFilter;
	Widget	UxnnF_labVars;
	Widget	UxnnF_swinVars;
	Widget	UxnnF_slistVars;
	Widget	UxnnF_pbOk;
	Widget	UxnnF_pbCancel;
	Widget	UxnnF_pbHelp;
	Widget	UxwgtFindBloc;
	Widget	UxnnF_help;
	int	UxcVars;
	int	UxcSelVars;
	int	UxcSelOut;
	int	UxcSelInp;
	Boolean	UxoTextChg;
	Boolean	*UxabSelVars;
	unsigned char	*UxphFilter;
	Widget	UxnnF_parent;
	String	UxnnFzHfile;
} _UxCnnFilter;

#define nnFilter                UxNnFilterContext->UxnnFilter
#define nnF_labCaso             UxNnFilterContext->UxnnF_labCaso
#define nnF_labMod              UxNnFilterContext->UxnnF_labMod
#define nnF_swinMod             UxNnFilterContext->UxnnF_swinMod
#define nnF_slistMod            UxNnFilterContext->UxnnF_slistMod
#define nnF_aroBloc             UxNnFilterContext->UxnnF_aroBloc
#define nnF_labBloc             UxNnFilterContext->UxnnF_labBloc
#define nnF_swinBloc            UxNnFilterContext->UxnnF_swinBloc
#define nnF_slistBloc           UxNnFilterContext->UxnnF_slistBloc
#define nnF_aroVars             UxNnFilterContext->UxnnF_aroVars
#define nnF_sep                 UxNnFilterContext->UxnnF_sep
#define nnF_labFilter           UxNnFilterContext->UxnnF_labFilter
#define nnF_textFilter          UxNnFilterContext->UxnnF_textFilter
#define nnF_labVars             UxNnFilterContext->UxnnF_labVars
#define nnF_swinVars            UxNnFilterContext->UxnnF_swinVars
#define nnF_slistVars           UxNnFilterContext->UxnnF_slistVars
#define nnF_pbOk                UxNnFilterContext->UxnnF_pbOk
#define nnF_pbCancel            UxNnFilterContext->UxnnF_pbCancel
#define nnF_pbHelp              UxNnFilterContext->UxnnF_pbHelp
#define wgtFindBloc             UxNnFilterContext->UxwgtFindBloc
#define nnF_help                UxNnFilterContext->UxnnF_help
#define cVars                   UxNnFilterContext->UxcVars
#define cSelVars                UxNnFilterContext->UxcSelVars
#define cSelOut                 UxNnFilterContext->UxcSelOut
#define cSelInp                 UxNnFilterContext->UxcSelInp
#define oTextChg                UxNnFilterContext->UxoTextChg
#define abSelVars               UxNnFilterContext->UxabSelVars
#define phFilter                UxNnFilterContext->UxphFilter
#define nnF_parent              UxNnFilterContext->UxnnF_parent
#define nnFzHfile               UxNnFilterContext->UxnnFzHfile

static _UxCnnFilter	*UxNnFilterContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_nnFilter();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* codice ausiliario: */
static void mostra_selMod( void);
static void mostra_selBloc( void);
static void mostra_selVars( void);
static XtCallbackProc
 nnFslistBlocSelelectionCB(),
 nnFcancelCB(),
 nnFhelpCB();

/* -------------------------------- */
static void mostra_selMod( void)
{
 char z[40];
 int c;
   get_something( nnF_slistMod, XmNselectedItemCount, (void*) &c);
   sprintf( z, "modules %d, selected %d", nst, c);
   UxPutStrRes( nnF_labMod, XmNlabelString, z);
}

/* -------------------------------- */
static void mostra_selBloc( void)
{
 char z[40];
 int c;
   get_something( nnF_slistBloc, XmNselectedItemCount, (void*) &c);
   sprintf( z, "blocks %d, selected %d", nbl, c);
   UxPutStrRes( nnF_labBloc, XmNlabelString, z);
}

/* -------------------------------- */
static void mostra_selVars( void)
{
 char z[50];
   sprintf( z, "vars %d, selected %d (= %d + %d)",
    cVars, cSelVars, cSelOut, cSelInp);
   UxPutStrRes( nnF_labVars, XmNlabelString, z);
}

/* -------------------------------- */
static XtCallbackProc nnFslistBlocSelelectionCB(
 Widget w,
 XtPointer pC,
 XmListCallbackStruct *pA)
{
   mostra_selBloc();
}

/* ------------------ */
static XtCallbackProc nnFcancelCB(
 Widget w,
 XtPointer pC,
 XmPushButtonCallbackStruct *pA)
{
   XtFree( abSelVars);
   XtFree( phFilter);
   UxDestroyInterface( nnFilter );
}

/* ------------------ */
static XtCallbackProc nnFhelpCB(
 Widget w,
 XtPointer pC,
 XmAnyCallbackStruct *pA)
{
   XtManageChild( nnF_help );
}

/* codice ausiliario: fine. */

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	multipleSelectionCB_nnF_slistMod( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* gestisce selezioni della ScrolledList dei Moduli */
	 XmListCallbackStruct *pA =
	(XmListCallbackStruct *) UxCallbackArg;
	 int ip=pA->item_position;
	   if (ip > nst) {
	      XmListDeselectAllItems( nnF_slistMod);
	      if (ip == nst+1) {
	         int i=0;
	         while (i++ < nst)
	            XmListSelectPos( nnF_slistMod, i, False);
	      }
	   }
	   mostra_selMod();
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	createCB_nnF_slistMod( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* carica la ScrolledList dei Moduli */
	 Boolean *ab;
	 char zM[4+1];
	 XmString xs;
	 int i, iM;
	   ab = (Boolean *) XtMalloc( sizeof(Boolean) * nst);
	   for (iM=0; iM<nst; iM++)
	      ab[iM] = False;
	   zM[4] = '\0';
	   for (i=0; i<nbl; i++) {
	      iM = islb[i];
	      if (ab[ iM -1 ])
	         continue;
	      strncpy( zM, nosub[i], 4);
	      xs = XmStringCreateSimple( zM);
	      XmListAddItemUnselected( nnF_slistMod, xs, iM);
	      XmStringFree( xs);
	      ab[ iM -1 ] = True;
	   }
	   XtFree( ab);
	   xs = XmStringCreateSimple( "(all)");
	   XmListAddItemUnselected( nnF_slistMod, xs, 0);
	   XmStringFree( xs);
	   xs = XmStringCreateSimple( "(none)");
	   XmListAddItemUnselected( nnF_slistMod, xs, 0);
	   XmStringFree( xs);
	   set_something_val( nnF_slistMod, XmNvisibleItemCount, (XtArgVal) nst+2);
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	activateCB_nnF_aroBloc( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* dai Moduli selezionati, seleziona Blocchi */
	 int *ai, c, ia, i;
	   XmListDeselectAllItems( nnF_slistBloc);
	   if (XmListGetSelectedPos( nnF_slistMod, &ai, &c)) {
	      for (ia=0; ia<c; ia++)
	         for (i=0; i<nbl; i++)
	            if (islb[i] == ai[ia])
	               XmListSelectPos( nnF_slistBloc, i+1, False);
	      XtFree( (char *) ai);
	   }
	   mostra_selBloc();
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	createCB_nnF_slistBloc( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* carica la ScrolledList dei Blocchi */
	 XmString xs;
	 char zB[80+1];
	 int i;
	   zB[80] = '\0';
	   for (i=0; i<nbl; i++) {
	      strncpy( zB, nom_bloc[i], 80);
	      xs = XmStringCreateSimple( zB);
	      XmListAddItemUnselected( nnF_slistBloc, xs, i+1);
	      XmStringFree( xs);
	   }
	   set_something_val( nnF_slistBloc, XmNvisibleItemCount, (XtArgVal) nbl);
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	activateCB_nnF_aroVars( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* dai Blocchi selezionati, seleziona Variabili:
	 * (1) delimita le parole-chiave nel Filtro;
	 * (2) costruisce nuovo elenco di variabili
	 */
	 int *ai, c, i, ibloc, j, ivj, iz, k;
	 Boolean bOk;
	 char *ph, *pV, ahV[100+1];
	 XmString xs;
	 static int chFilter, czFilter, al[NNF_TEXT_MAXLEN/2];
	 static char *azFilter[NNF_TEXT_MAXLEN/2];
	   if (oTextChg) {
	      XtFree( phFilter);
	      ph = phFilter = XmTextGetString( nnF_textFilter);
	      czFilter = 0;
	      do {
	         while (*ph && isspace(*ph))
	            ph++;
	         if (*ph)
	            azFilter[czFilter++] = ph;
	         else
	            break;
	         while (*ph && !isspace(*ph))
	            ph++;
	         if (*ph)
	            *ph++ = '\0';
	         else
	            break;
	      } while (*ph);
	      for (iz=0; iz<czFilter; iz++)
	         if ((al[iz]=strlen(azFilter[iz])) > 4)
	            azFilter[iz][al[iz]=4] = '\0';
	   }
	   XmListDeleteAllItems( nnF_slistVars);
	   for (i=0; i<cVars; i++)
	      abSelVars[i] = False;
	   cSelVars = cSelOut = cSelInp = 0;
	   ahV[100] = '\0';
	   if (XmListGetSelectedPos( nnF_slistBloc, &ai, &c)) {
	      UxPutStrRes( nnF_labVars, XmNlabelString, "Processing...");
	      XmUpdateDisplay( nnF_labVars);
	      for (i=0; i<c; i++) {
	         ibloc = ai[i] -1;
	         for (j=ip[ibloc]-1; j<ip[ibloc+1]-1; j++) {
	            ivj = ((ivj = ipvrs[j]) > 0) ? ivj : (neqsis -ivj);
	            if (abSelVars[ivj-1])
	               continue;
	            pV = (ivj<=neqsis) ? sivar[ivj-1] : vari[ivj-neqsis-1];
	            for (iz=0, bOk=False; iz<czFilter && !bOk; iz++)
	               for (k=0; k<al[iz]; k++) {
	                  if (bOk = azFilter[iz][k] == '*')
	                     break;
	                  if (bOk = azFilter[iz][k] == '?')
	                     continue;
	                  if (bOk = pV[k] == azFilter[iz][k])
	                     continue;
	                  else
	                     break;
	               }
	            if (!bOk)
	               continue;
	            abSelVars[ivj-1] = True;
	            cSelVars++;
	            if (ivj<=neqsis)
	              cSelOut++;
	            else
	              cSelInp++;
	         }
	      }
	      XtFree( (char *) ai);
	      for (i=0; i<cVars; i++) {
	         if (!abSelVars[i])
	            continue;
	         pV = (i<neqsis) ? nom_sivar[i] : nom_vari[i-neqsis];
	         strncpy( ahV, pV, 100);
	         xs = XmStringCreateSimple( ahV);
	         XmListAddItemUnselected( nnF_slistVars, xs, 0);
	         XmStringFree( xs);
	      }
	      set_something_val( nnF_slistVars,
	       XmNvisibleItemCount, (XtArgVal) ? cSelVars : 1);
	   }
	   mostra_selVars();
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	modifyVerifyCB_nnF_textFilter( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* traduce in maiuscolo i caratteri immessi */
	 XmTextVerifyCallbackStruct *pA =
	(XmTextVerifyCallbackStruct *) UxCallbackArg;
	 char *ph;
	 int i;
	   for (i=0, ph=pA->text->ptr; i<pA->text->length; i++, ph++)
	      if (islower(*ph))
	         *ph = toupper( *ph);
	   oTextChg = True;
	}
	UxNnFilterContext = UxSaveCtx;
}

static void	activateCB_nnF_pbOk( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnFilter            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnFilterContext;
	UxNnFilterContext = UxContext =
			(_UxCnnFilter *) UxGetContext( UxWidget );
	{
	/* ritorna le variabili selezionate */
	 int *aiReturn, i, k;
	   if (cSelVars) {
	      aiReturn = (int *) XtMalloc( sizeof(int) * (2+cSelVars));
	      for (i=0, k=2; i<cVars; i++)
	         if (abSelVars[i])
	            aiReturn[k++] = i+1;
	      aiReturn[0] = cSelOut;
	      aiReturn[1] = cSelInp;
	   } else
	      aiReturn = NULL;
	   fnnF_ritorna_E( aiReturn);
	   XtFree( (char *) aiReturn );
	   nnFcancelCB( 0, 0, 0);
	}
	UxNnFilterContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_nnFilter()
{
	Widget	nnFilter_shell;

	nnFilter_shell = XtVaCreatePopupShell( "nnFilter_shell",
			xmDialogShellWidgetClass, nnF_parent,
			XmNx, 255,
			XmNy, 23,
			XmNwidth, 500,
			XmNheight, 600,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "nnFilter",
			NULL );

	nnFilter = XtVaCreateWidget( "nnFilter",
			xmBulletinBoardWidgetClass, nnFilter_shell,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNnoResize, TRUE,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNautoUnmanage, FALSE,
			XmNheight, 600,
			XmNwidth, 500,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nnFilter, (char *) UxNnFilterContext );

	nnF_labCaso = XtVaCreateManagedWidget( "nnF_labCaso",
			xmLabelWidgetClass, nnFilter,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNheight, 20,
			XmNwidth, 380,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( nnF_labCaso, (char *) UxNnFilterContext );

	nnF_labMod = XtVaCreateManagedWidget( "nnF_labMod",
			xmLabelWidgetClass, nnFilter,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNheight, 20,
			XmNwidth, 150,
			XmNy, 40,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_labMod, (char *) UxNnFilterContext );

	nnF_swinMod = XtVaCreateManagedWidget( "nnF_swinMod",
			xmScrolledWindowWidgetClass, nnFilter,
			XmNvisualPolicy, XmVARIABLE,
			XmNshadowThickness, 0,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNwidth, 70,
			XmNheight, 180,
			XmNy, 60,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_swinMod, (char *) UxNnFilterContext );

	nnF_slistMod = XtVaCreateManagedWidget( "nnF_slistMod",
			xmListWidgetClass, nnF_swinMod,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNwidth, 90,
			XmNheight, 120,
			XmNselectionPolicy, XmMULTIPLE_SELECT,
			NULL );

	UxPutContext( nnF_slistMod, (char *) UxNnFilterContext );

	createCB_nnF_slistMod( nnF_slistMod,
			(XtPointer) UxNnFilterContext, (XtPointer) NULL );

	nnF_aroBloc = XtVaCreateManagedWidget( "nnF_aroBloc",
			xmArrowButtonWidgetClass, nnFilter,
			XmNarrowDirection, XmARROW_RIGHT,
			XmNheight, 70,
			XmNwidth, 30,
			XmNy, 110,
			XmNx, 100,
			NULL );

	UxPutContext( nnF_aroBloc, (char *) UxNnFilterContext );

	nnF_labBloc = XtVaCreateManagedWidget( "nnF_labBloc",
			xmLabelWidgetClass, nnFilter,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNheight, 20,
			XmNwidth, 200,
			XmNy, 40,
			XmNx, 180,
			NULL );

	UxPutContext( nnF_labBloc, (char *) UxNnFilterContext );

	nnF_swinBloc = XtVaCreateManagedWidget( "nnF_swinBloc",
			xmScrolledWindowWidgetClass, nnFilter,
			XmNvisualPolicy, XmVARIABLE,
			XmNshadowThickness, 0,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNheight, 180,
			XmNwidth, 340,
			XmNy, 60,
			XmNx, 140,
			NULL );

	UxPutContext( nnF_swinBloc, (char *) UxNnFilterContext );

	nnF_slistBloc = XtVaCreateManagedWidget( "nnF_slistBloc",
			xmListWidgetClass, nnF_swinBloc,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNwidth, 340,
			XmNheight, 140,
			XmNselectionPolicy, XmMULTIPLE_SELECT,
			NULL );

	UxPutContext( nnF_slistBloc, (char *) UxNnFilterContext );

	createCB_nnF_slistBloc( nnF_slistBloc,
			(XtPointer) UxNnFilterContext, (XtPointer) NULL );

	nnF_aroVars = XtVaCreateManagedWidget( "nnF_aroVars",
			xmArrowButtonWidgetClass, nnFilter,
			XmNarrowDirection, XmARROW_DOWN,
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 300,
			XmNx, 220,
			NULL );

	UxPutContext( nnF_aroVars, (char *) UxNnFilterContext );

	nnF_sep = XtVaCreateManagedWidget( "nnF_sep",
			xmSeparatorWidgetClass, nnFilter,
			XmNheight, 10,
			XmNwidth, 460,
			XmNy, 300,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_sep, (char *) UxNnFilterContext );

	nnF_labFilter = XtVaCreateManagedWidget( "nnF_labFilter",
			xmLabelWidgetClass, nnFilter,
			RES_CONVERT( XmNlabelString, "Vars Filter:" ),
			XmNheight, 30,
			XmNwidth, 120,
			XmNy, 340,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_labFilter, (char *) UxNnFilterContext );

	nnF_textFilter = XtVaCreateManagedWidget( "nnF_textFilter",
			xmTextWidgetClass, nnFilter,
			XmNeditMode, XmSINGLE_LINE_EDIT,
			XmNheight, 30,
			XmNwidth, 260,
			XmNy, 340,
			XmNx, 140,
			NULL );

	UxPutContext( nnF_textFilter, (char *) UxNnFilterContext );

	nnF_labVars = XtVaCreateManagedWidget( "nnF_labVars",
			xmLabelWidgetClass, nnFilter,
			XmNheight, 20,
			XmNwidth, 460,
			XmNy, 370,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_labVars, (char *) UxNnFilterContext );

	nnF_swinVars = XtVaCreateManagedWidget( "nnF_swinVars",
			xmScrolledWindowWidgetClass, nnFilter,
			XmNvisualPolicy, XmVARIABLE,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNheight, 120,
			XmNwidth, 460,
			XmNy, 390,
			XmNx, 20,
			NULL );

	UxPutContext( nnF_swinVars, (char *) UxNnFilterContext );

	nnF_slistVars = XtVaCreateManagedWidget( "nnF_slistVars",
			xmListWidgetClass, nnF_swinVars,
			XmNsensitive, FALSE,
			XmNwidth, 410,
			XmNheight, 120,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			NULL );

	UxPutContext( nnF_slistVars, (char *) UxNnFilterContext );

	nnF_pbOk = XtVaCreateManagedWidget( "nnF_pbOk",
			xmPushButtonWidgetClass, nnFilter,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 540,
			XmNx, 40,
			NULL );

	UxPutContext( nnF_pbOk, (char *) UxNnFilterContext );

	nnF_pbCancel = XtVaCreateManagedWidget( "nnF_pbCancel",
			xmPushButtonWidgetClass, nnFilter,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 540,
			XmNx, 210,
			NULL );

	UxPutContext( nnF_pbCancel, (char *) UxNnFilterContext );

	nnF_pbHelp = XtVaCreateManagedWidget( "nnF_pbHelp",
			xmPushButtonWidgetClass, nnFilter,
			RES_CONVERT( XmNlabelString, "Help" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 540,
			XmNx, 380,
			NULL );

	UxPutContext( nnF_pbHelp, (char *) UxNnFilterContext );

	XtAddCallback( nnFilter, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnFilter, XmNunmapCallback,
			nnFcancelCB,
			(XtPointer) UxNnFilterContext );
	XtAddCallback( nnFilter, XmNhelpCallback,
			nnFhelpCB,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_slistMod, XmNmultipleSelectionCallback,
			multipleSelectionCB_nnF_slistMod,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_aroBloc, XmNactivateCallback,
			activateCB_nnF_aroBloc,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_slistBloc, XmNmultipleSelectionCallback,
			nnFslistBlocSelelectionCB,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_aroVars, XmNactivateCallback,
			activateCB_nnF_aroVars,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_textFilter, XmNmodifyVerifyCallback,
			modifyVerifyCB_nnF_textFilter,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_pbOk, XmNactivateCallback,
			activateCB_nnF_pbOk,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_pbCancel, XmNactivateCallback,
			nnFcancelCB,
			(XtPointer) UxNnFilterContext );

	XtAddCallback( nnF_pbHelp, XmNactivateCallback,
			nnFhelpCB,
			(XtPointer) UxNnFilterContext );



	return ( nnFilter );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_nnFilter( _UxnnF_parent, _UxnnFzHfile )
	Widget	_UxnnF_parent;
	String	_UxnnFzHfile;
{
	Widget                  rtrn;
	_UxCnnFilter            *UxContext;

	UxNnFilterContext = UxContext =
		(_UxCnnFilter *) XtMalloc( sizeof(_UxCnnFilter) );

	nnF_parent = _UxnnF_parent;
	nnFzHfile = _UxnnFzHfile;
	{
		/* codice iniziale: */
		cVars = neqsis+nu;
		cSelVars = cSelOut = cSelInp = 0;
		abSelVars = (Boolean *) XtMalloc( sizeof(Boolean) * cVars);
		phFilter = NULL;
		rtrn = _Uxbuild_nnFilter();

		/* codice finale: */
		XtManageChild( rtrn);
		nnF_help = helpkit_propInfo( rtrn, nnFzHfile, nnFzHpage);
		{
		 char z[24];
		   sprintf( z, "Model name: %.8s", sigla);
		   UxPutStrRes( nnF_labCaso, XmNlabelString, z);
		}
		{ /* a[10] per find_kit() della libreria libutilx... */
		 Arg al[10];
		 Cardinal n=0;
		   XtSetArg( al[n], XmNx, 20); n++;
		   XtSetArg( al[n], XmNy, 250); n++;
		   wgtFindBloc = find_kit( nnFilter, al, n, &fsFindBloc );
		   fsFindBloc.ID_lista = (Widget) nnF_slistBloc;
		}
		
		XmTextSetMaxLength( nnF_textFilter, NNF_TEXT_MAXLEN);
		XmTextSetString( nnF_textFilter, "*");
		mostra_selMod();
		mostra_selBloc();
		XtSetSensitive( nnF_slistVars, False);
		mostra_selVars();
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_nnFilter( _UxnnF_parent, _UxnnFzHfile )
	Widget	_UxnnF_parent;
	String	_UxnnFzHfile;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_nnFilter( _UxnnF_parent, _UxnnFzHfile );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

