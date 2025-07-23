
/*******************************************************************************
	nnEdit.c
	(Generated from interface file nnEdit.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/DrawnB.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/PanedW.h>
#include <Xm/Scale.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
#include <Xm/Separator.h>
#include <Xm/ToggleB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo nnEdit.i
   tipo 
   release 1.13
   data 11/13/95
   reserved @(#)nnEdit.i	1.13
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif
/* nnEdit.c
* Scopo:       editare note / non-note
* Dati:        dai files "f03.dat" ed "f14.dat": presunte in memoria;
*              dal file "fpp.dat": legge;
* Risultati:   salva "f14.dat" ed "fpp.dat"
* Routines:    write_file_f14 (da libreria liblegocad.a),
*              {read,write}_file_fpp (da file fpp.o)
* Moduli:      dialogo nnFilter (in =.o), nnValue (in =.o)
* Note:
*  (1)     interfaccia, callback etc composta con AIC in modo Xt;
*  (2)     variabili private e routines ausiliarie statiche;
*  (3)     mono-sessione, chiusura controllata, monito privato;
* Autore:      C.Lusso (CISE) 1994
*/


 /* include: */
#include <Xm/MessageB.h>
#include "f03.h"
#include "f14.h"
#include "libutilx.h"
#include "helpkit.h"
#include "nn.h"
#include "fpp.h"
 /* define: */
/* bit per opzioni di 'loStato':
*	NN_LIBERO       valore iniziale;
*	NN_OCCUPATO     On: dialogo occupato, Off: libero;
*	NN_MODE_NN      On: Modo Normale, Off: Modo Paia;
*	NN_CHG_NN       On: cambiate vars, Off: nessuna;
*	NN_CHG_PP       On: cambiate paia, Off: nessuna;
*/
#define NN_LIBERO       0L
#define NN_OCCUPATO     0x01L
#define NN_MODE_NN      0x02L
#define NN_VARS_ALL     0x10L
#define NN_VARS_FILTER  0x20L
#define NN_CHG_NN       0x100L
#define NN_CHG_PP       0x200L
 /* enum: */
/* messaggi */
enum {
 ennEmsg_null,
 ennEmsg_save_14_pp,
 ennEmsg_save_14,
 ennEmsg_save_pp,
 ennEmsg_save_none,
 ennEmsg_wait,
 ennEmsg_values,
 ennEmsg_itemK,
 ennEmsg_itemUnk,
 ennEmsg_badPair,
 ennEmsg_pairDel,
 ennEmsg_notavail,
 ennEmsgs
};
 /* variabili: */
extern VAL_VAR *valinp;
extern VAL_VAR *valout;
/* variabili statiche:
	long
 loStato;               stato della pagina
	String
 nnEzHpage,		pagina helpkit
 nnEazMsgs[ennEmsgs],   messaggi all'utente
 nnEszClose,            conferma uscita se modifiche pendenti
 nnEszSetup;            conferma tutte inp-vars Note e tutte out-vars Non-note
*/
static long
 loStato = NN_LIBERO;
static String
 nnEzHpage = "nnEdit",
 nnEazMsgs[ennEmsgs] = {
  "",
  "saved file_14 and file_pp",
  "saved file_14",
  "saved file_pp",
  "no file saved",
  "Wait please...",
  "no vars selected",
  "item already marked Known",
  "item already marked Unknown",
  "conflicting selection",
  "also the Pair will be deleted!",
  "not available",
 },
 nnEszClose = "Do you want to save changes ?",
 nnEszSetup = "Do you want ALL inp-var Known and ALL out-var Unknown ?";
static /*XtCallbackProc*/ void
 nnEcloseDlgCB(),
 nnEsetupDlgCB();
static Elenco_callback conferma_close = {
 { "Yes",       nnEcloseDlgCB,  0 },
 { "No" ,       nnEcloseDlgCB,  1 },
 { "Cancel",    nnEcloseDlgCB,  2 } };
static Elenco_callback conferma_setup = {
 { "Ok",        nnEsetupDlgCB,  0 },
 { "Cancel",    nnEsetupDlgCB,  1 },
 { NULL,        NULL,           NULL } };
static Find_struct
 fsOut,
 fsInp;
/* funzioni: */


extern swidget create_nnFilter( Widget, String);

extern swidget create_nnValues( Widget, String, VAL_VAR *, int *);
/*extern swidget create_nnGraph( void );*/

/* variabili di istanza:
Widget
 nnE_labVars[8],
 wgtFindOut,
 wgtFindInp,
 nnE_help;
Find_struct
 fsOut,
 fsInp;
VAL_VAR
 *avvEdit;		[neqsis+nu] copia privata delle valout + valinp
int
 cKnown,		conta vars 'note'
 cLinp, *aiLinp,	conta, [nu]    ipvrs di inp-vars Caricate;
 cLout, *aiLout,	conta, [neqsis] ipvrs di out-vars Caricate;
 cMinp, *aiMinp,	conta, [nu]     indici(0-) delle *aiLinp Marcate;
 cMout, *aiMout,	conta, [neqsis] indici(0-) delle *aiLout Marcate;
 cPair, mPair,		conta Paia totali, max Paia;
 cPinp, *aiPinp,	conta, [mPair] posizioni(1-) delle *aiMinp Paiate;
 cPout, *aiPout,	conta, [mPair] posizioni(1-) delle *aiMout Paiate;
 iPinpSel, iPoutSel;	posizioni selezionate: =0 no, >0 si, <0 paio;
char
 **azPinp,      [mPair] nomi delle inp-vars Paiate in *aiMinp;
 **azPout;      [mPair] nomi delle out-vars Paiate in *aiMout;
XtIntervalId
 idMsg;
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
	Widget	UxnnEdit;
	Widget	UxnnE_mainWin;
	Widget	UxnnE_mu;
	Widget	UxnnE_mu_pF;
	Widget	UxnnE_mu_pF_bL;
	Widget	UxnnE_mu_pF_bS;
	Widget	UxnnE_mu_pF_bZ;
	Widget	UxnnE_mu_top_bF;
	Widget	UxnnE_mu_pM;
	Widget	UxnnE_mu_pM_tN;
	Widget	UxnnE_mu_pM_tP;
	Widget	UxnnE_mu_top_bM;
	Widget	UxnnE_mu_pO;
	Widget	UxnnE_mu_pO_bG;
	Widget	UxnnE_mu_pO_bV;
	Widget	UxnnE_mu_pO_sep;
	Widget	UxnnE_mu_pO_bK;
	Widget	UxnnE_mu_top_bO;
	Widget	UxnnE_mu_pH;
	Widget	UxnnE_mu_pH_bP;
	Widget	UxnnE_mu_top_bH;
	Widget	UxnnE_form;
	Widget	UxnnE_labModel;
	Widget	UxnnE_formVars;
	Widget	UxnnE_scale;
	Widget	UxnnE_panedWin;
	Widget	UxnnE_formOut;
	Widget	UxnnE_labOut;
	Widget	UxnnE_labLout;
	Widget	UxnnE_labMout;
	Widget	UxnnE_pbOadd;
	Widget	UxnnE_pbOdel;
	Widget	UxnnE_sWinLout;
	Widget	UxnnE_sListLout;
	Widget	UxnnE_sWinMout;
	Widget	UxnnE_sListMout;
	Widget	UxnnE_pbPadd;
	Widget	UxnnE_formInp;
	Widget	UxnnE_labInp;
	Widget	UxnnE_labLinp;
	Widget	UxnnE_labMinp;
	Widget	UxnnE_pbIadd;
	Widget	UxnnE_pbIdel;
	Widget	UxnnE_sWinLinp;
	Widget	UxnnE_sListLinp;
	Widget	UxnnE_sWinMinp;
	Widget	UxnnE_sListMinp;
	Widget	UxnnE_pbPdel;
	Widget	UxnnE_omuV_p1;
	Widget	UxnnE_omuVAll;
	Widget	UxnnE_omuVFilter;
	Widget	UxnnE_omuVars;
	Widget	UxnnE_dbMsg;
	Widget	UxwCloseDlg;
	Widget	UxwSetupDlg;
	Widget	UxnnE_labVars[8];
	Widget	UxwgtFindOut;
	Widget	UxwgtFindInp;
	Widget	UxnnE_help;
	String	UxnnEzH;
	VAL_VAR	*UxavvEdit;
	int	UxcKnown;
	int	UxcLinp;
	int	*UxaiLinp;
	int	UxcLout;
	int	*UxaiLout;
	int	UxcMinp;
	int	*UxaiMinp;
	int	UxcMout;
	int	*UxaiMout;
	int	UxcPair;
	int	UxmPair;
	int	UxcPinp;
	int	*UxaiPinp;
	int	UxcPout;
	int	*UxaiPout;
	int	UxiPinpSel;
	int	UxiPoutSel;
	int	UxlastMsg;
	unsigned char	**UxazPinp;
	unsigned char	**UxazPout;
	XtIntervalId	UxidMsg;
	String	UxnnEzHfile;
} _UxCnnEdit;

#define nnEdit                  UxNnEditContext->UxnnEdit
#define nnE_mainWin             UxNnEditContext->UxnnE_mainWin
#define nnE_mu                  UxNnEditContext->UxnnE_mu
#define nnE_mu_pF               UxNnEditContext->UxnnE_mu_pF
#define nnE_mu_pF_bL            UxNnEditContext->UxnnE_mu_pF_bL
#define nnE_mu_pF_bS            UxNnEditContext->UxnnE_mu_pF_bS
#define nnE_mu_pF_bZ            UxNnEditContext->UxnnE_mu_pF_bZ
#define nnE_mu_top_bF           UxNnEditContext->UxnnE_mu_top_bF
#define nnE_mu_pM               UxNnEditContext->UxnnE_mu_pM
#define nnE_mu_pM_tN            UxNnEditContext->UxnnE_mu_pM_tN
#define nnE_mu_pM_tP            UxNnEditContext->UxnnE_mu_pM_tP
#define nnE_mu_top_bM           UxNnEditContext->UxnnE_mu_top_bM
#define nnE_mu_pO               UxNnEditContext->UxnnE_mu_pO
#define nnE_mu_pO_bG            UxNnEditContext->UxnnE_mu_pO_bG
#define nnE_mu_pO_bV            UxNnEditContext->UxnnE_mu_pO_bV
#define nnE_mu_pO_sep           UxNnEditContext->UxnnE_mu_pO_sep
#define nnE_mu_pO_bK            UxNnEditContext->UxnnE_mu_pO_bK
#define nnE_mu_top_bO           UxNnEditContext->UxnnE_mu_top_bO
#define nnE_mu_pH               UxNnEditContext->UxnnE_mu_pH
#define nnE_mu_pH_bP            UxNnEditContext->UxnnE_mu_pH_bP
#define nnE_mu_top_bH           UxNnEditContext->UxnnE_mu_top_bH
#define nnE_form                UxNnEditContext->UxnnE_form
#define nnE_labModel            UxNnEditContext->UxnnE_labModel
#define nnE_formVars            UxNnEditContext->UxnnE_formVars
#define nnE_scale               UxNnEditContext->UxnnE_scale
#define nnE_panedWin            UxNnEditContext->UxnnE_panedWin
#define nnE_formOut             UxNnEditContext->UxnnE_formOut
#define nnE_labOut              UxNnEditContext->UxnnE_labOut
#define nnE_labLout             UxNnEditContext->UxnnE_labLout
#define nnE_labMout             UxNnEditContext->UxnnE_labMout
#define nnE_pbOadd              UxNnEditContext->UxnnE_pbOadd
#define nnE_pbOdel              UxNnEditContext->UxnnE_pbOdel
#define nnE_sWinLout            UxNnEditContext->UxnnE_sWinLout
#define nnE_sListLout           UxNnEditContext->UxnnE_sListLout
#define nnE_sWinMout            UxNnEditContext->UxnnE_sWinMout
#define nnE_sListMout           UxNnEditContext->UxnnE_sListMout
#define nnE_pbPadd              UxNnEditContext->UxnnE_pbPadd
#define nnE_formInp             UxNnEditContext->UxnnE_formInp
#define nnE_labInp              UxNnEditContext->UxnnE_labInp
#define nnE_labLinp             UxNnEditContext->UxnnE_labLinp
#define nnE_labMinp             UxNnEditContext->UxnnE_labMinp
#define nnE_pbIadd              UxNnEditContext->UxnnE_pbIadd
#define nnE_pbIdel              UxNnEditContext->UxnnE_pbIdel
#define nnE_sWinLinp            UxNnEditContext->UxnnE_sWinLinp
#define nnE_sListLinp           UxNnEditContext->UxnnE_sListLinp
#define nnE_sWinMinp            UxNnEditContext->UxnnE_sWinMinp
#define nnE_sListMinp           UxNnEditContext->UxnnE_sListMinp
#define nnE_pbPdel              UxNnEditContext->UxnnE_pbPdel
#define nnE_omuV_p1             UxNnEditContext->UxnnE_omuV_p1
#define nnE_omuVAll             UxNnEditContext->UxnnE_omuVAll
#define nnE_omuVFilter          UxNnEditContext->UxnnE_omuVFilter
#define nnE_omuVars             UxNnEditContext->UxnnE_omuVars
#define nnE_dbMsg               UxNnEditContext->UxnnE_dbMsg
#define wCloseDlg               UxNnEditContext->UxwCloseDlg
#define wSetupDlg               UxNnEditContext->UxwSetupDlg
#define nnE_labVars             UxNnEditContext->UxnnE_labVars
#define wgtFindOut              UxNnEditContext->UxwgtFindOut
#define wgtFindInp              UxNnEditContext->UxwgtFindInp
#define nnE_help                UxNnEditContext->UxnnE_help
#define nnEzH                   UxNnEditContext->UxnnEzH
#define avvEdit                 UxNnEditContext->UxavvEdit
#define cKnown                  UxNnEditContext->UxcKnown
#define cLinp                   UxNnEditContext->UxcLinp
#define aiLinp                  UxNnEditContext->UxaiLinp
#define cLout                   UxNnEditContext->UxcLout
#define aiLout                  UxNnEditContext->UxaiLout
#define cMinp                   UxNnEditContext->UxcMinp
#define aiMinp                  UxNnEditContext->UxaiMinp
#define cMout                   UxNnEditContext->UxcMout
#define aiMout                  UxNnEditContext->UxaiMout
#define cPair                   UxNnEditContext->UxcPair
#define mPair                   UxNnEditContext->UxmPair
#define cPinp                   UxNnEditContext->UxcPinp
#define aiPinp                  UxNnEditContext->UxaiPinp
#define cPout                   UxNnEditContext->UxcPout
#define aiPout                  UxNnEditContext->UxaiPout
#define iPinpSel                UxNnEditContext->UxiPinpSel
#define iPoutSel                UxNnEditContext->UxiPoutSel
#define lastMsg                 UxNnEditContext->UxlastMsg
#define azPinp                  UxNnEditContext->UxazPinp
#define azPout                  UxNnEditContext->UxazPout
#define idMsg                   UxNnEditContext->UxidMsg
#define nnEzHfile               UxNnEditContext->UxnnEzHfile

static _UxCnnEdit	*UxNnEditContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_nnEdit();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* funzioni ausiliarie - prototipi: */
static void libera_dati_privati( void);
static void inizia_dati_privati( void);
static void mostra_cLMP( void);
static XtTimerCallbackProc msgOff( XtPointer data, XtIntervalId *id );
static void msgOn( int e );
static int ceInElencoInt( int iPos, int cElenco, int *aiElenco );
static int ceInElencoStr( String zPos, int cElenco, String *azElenco );
static void nnE_load_NN( void );
static void nnE_load_PP( void );
static void deletePair( int k );
void fnnF_ritorna_E( int *);
void fnnV_ritorna_E( Boolean);
static void nnE_close_OK( void);
static XtCallbackProc
 nnEdestroyShellCB(),
 nnEloadCB(),
 nnEsaveCB(),
 nnEcloseCB(),
 nnEsetupCB(),
 nnEsimpleCB_omuVars(),
 nnEhelpCB();

/* funzioni ausiliarie - definizioni: */
/* ---------------------------------- */
static void libera_dati_privati( void)
{
 int i;
   XtFree( (char *) aiLinp);
   XtFree( (char *) aiLout);
   XtFree( (char *) aiMinp);
   XtFree( (char *) aiMout);
   XtFree( (char *) aiPinp);
   XtFree( (char *) aiPout);
   for (i=0; i<cPair; i++) {
      XtFree( azPout[i]);
      XtFree( azPinp[i]);
   }
   XtFree( (char *) azPinp);
   XtFree( (char *) azPout);
   XtFree( (char *) avvEdit);
   if (idMsg)
      XtRemoveTimeOut( idMsg);
}

/* ---------------------------------- */
static void inizia_dati_privati( void)
{
 int i;
   aiLinp = (int *) XtMalloc( sizeof(int) * nu);
   aiLout = (int *) XtMalloc( sizeof(int) * neqsis);
   aiMinp = (int *) XtMalloc( sizeof(int) * nu);
   aiMout = (int *) XtMalloc( sizeof(int) * neqsis);
   mPair = (neqsis > nu) ? nu : neqsis;
   aiPinp = (int *) XtMalloc( sizeof(int) * mPair);
   aiPout = (int *) XtMalloc( sizeof(int) * mPair);
   azPinp = (char **) XtMalloc( sizeof(char *) * mPair);
   azPout = (char **) XtMalloc( sizeof(char *) * mPair);
   avvEdit = (VAL_VAR *) XtMalloc( sizeof(VAL_VAR) * (neqsis+nu) );
   memcpy( &avvEdit[0], &valout[0], sizeof(VAL_VAR) * neqsis);
   memcpy( &avvEdit[neqsis], &valinp[0], sizeof(VAL_VAR) * nu);
   cKnown = 0;
   for (i=0; i<neqsis+nu; i++)
      if (avvEdit[i].noto)
         cKnown++;
   cPair = read_file_fpp( azPout, azPinp);
   nnEsimpleCB_omuVars( NULL, NN_VARS_ALL, 0);
   XmToggleButtonSetState( nnE_mu_pM_tN, True, True);
}

/* ---------------------------------- */
static void mostra_cLMP( void)
{
 static char z[60], zF[] ="%d";
   sprintf( z, zF, neqsis);
   UxPutStrRes( nnE_labVars[4], XmNlabelString, z );
   sprintf( z, zF, nu);
   UxPutStrRes( nnE_labVars[6], XmNlabelString, z );
   sprintf( z, zF, cKnown);
   UxPutStrRes( nnE_labVars[7], XmNlabelString, z );
   sprintf( z, zF, neqsis+nu-cKnown);
   UxPutStrRes( nnE_labVars[5], XmNlabelString, z );
   sprintf( z, "%d listed, %d unknown", cLinp, cMinp);
   UxPutStrRes( nnE_labLinp, XmNlabelString, z );
   sprintf( z, "%d listed, %d known", cLout, cMout);
   UxPutStrRes( nnE_labLout, XmNlabelString, z );
   sprintf( z, "%d paired (of %d)", cPinp, cPair);
   UxPutStrRes( nnE_labMinp, XmNlabelString, z );
   sprintf( z, "%d paired (of %d)", cPout, cPair);
   UxPutStrRes( nnE_labMout, XmNlabelString, z );
}

/* ---------------------------------- */
static XtTimerCallbackProc msgOff( XtPointer data, XtIntervalId *id )
{
   UxPutStrRes( nnE_dbMsg, XmNlabelString, nnEazMsgs[ennEmsg_null]);
   idMsg = 0;
}

/* ---------------------------------- */
static void msgOn( /*String z*/ int e )
{
 static unsigned long durata = 3000;
   if (idMsg)
      XtRemoveTimeOut( idMsg);
   UxPutStrRes( nnE_dbMsg, XmNlabelString, nnEazMsgs[ lastMsg=e ]);
   idMsg = XtAppAddTimeOut( UxAppContext, durata, msgOff, NULL);
}

/* ---------------------------------- */
static int ceInElencoInt( int iPos, int cElenco, int *aiElenco )
{
 int i;
   for (i=0; i<cElenco; i++)
      if (iPos == aiElenco[i])
         break;
   return (i<cElenco) ? (i+1) : 0;
}

/* ---------------------------------- */
static int ceInElencoStr( String zPos, int cElenco, String *azElenco )
{
 int i;
   for (i=0; i<cElenco; i++)
      if (strcmp( zPos, azElenco[i]) == 0)
         break;
   return (i<cElenco) ? (i+1) : 0;
}

/* ---------------------------------- */
static void nnE_load_NN( void )
{
/* carica le liste nel modo Normal */
 XmStringTable axsLout, axsLinp, axsMout, axsMinp;
 char zL[2+100+1], zM[2+20];
 int i, ibloc, j, ivj, ivk, k;
   axsLout = (XmStringTable) XtMalloc( sizeof(XmString) * neqsis );
   axsLinp = (XmStringTable) XtMalloc( sizeof(XmString) * nu );
   axsMout = (XmStringTable) XtMalloc( sizeof(XmString) * neqsis );
   axsMinp = (XmStringTable) XtMalloc( sizeof(XmString) * nu );
   zL[1] =zM[1] =' ';
   zL[2+100] ='\0';
   for (i=0; i<cPair; i++)
      aiPinp[i] =aiPout[i] =0;
   for (i=0, cMout=cPout=0; i<cLout; i++) {
      ivj =aiLout[i];
      strncpy( zL +2, nom_sivar[ivj-1], 100);
      if (avvEdit[ivj-1].noto) {
         zL[0] = '*';
         aiMout[cMout] =i;
         strncpy( zM+2, sivar[ivj-1], 8);
         zM[2+8] ='\0';
         if (k = ceInElencoStr( zM+2, cPair, azPout)) {
            zM[0] ='-';
            strcat( zM, "-");
            strcat( zM, azPinp[k-1]);
            aiPout[k-1] = cMout +1;
            cPout++;
         } else
            zM[0] =' ';
         axsMout[cMout++] = XmStringCreateSimple( zM );
      } else
         zL[0] = ' ';
      axsLout[i] = XmStringCreateSimple( zL );
   }
   for (i=0, cMinp=cPinp=0; i<cLinp; i++) {
      ivj =aiLinp[i];
      strncpy( zL +2, nom_vari[-ivj-1], 100);
      if (! avvEdit[neqsis-ivj-1].noto) {
         zL[0] = ' ';
         aiMinp[cMinp] =i;
         strncpy( zM+2, vari[-ivj-1], 8);
         zM[2+8] ='\0';
         if (k = ceInElencoStr( zM+2, cPair, azPinp)) {
            zM[0] ='-';
            strcat( zM, "-");
            strcat( zM, azPout[k-1]);
            aiPinp[k-1] = cMinp +1;
            cPinp++;
         } else
            zM[0] =' ';
         axsMinp[cMinp++] = XmStringCreateSimple( zM );
      } else
         zL[0] = '*';
      axsLinp[i] = XmStringCreateSimple( zL );
   }
   for (i=0; i<cPair; i++) {
      if (!aiPout[i]) {
         for (k=0; k<neqsis; k++)
            if (strncmp( sivar[k], azPout[i], 8) == 0)
               break;
         aiPout[i] = -k-1;
      }
      if (!aiPinp[i]) {
         for (k=0; k<nu; k++)
            if (strncmp( vari[k], azPinp[i], 8) == 0)
               break;
         aiPinp[i] = -k-1;
      }
   }
   XmListDeleteAllItems( nnE_sListLinp);
   XmListDeleteAllItems( nnE_sListMinp);
   XmListDeleteAllItems( nnE_sListLout);
   XmListDeleteAllItems( nnE_sListMout);
   XtVaSetValues( nnE_sListLout,
    XmNitemCount, cLout,
    XmNitems, axsLout,
    NULL );
   XtVaSetValues( nnE_sListMout,
    XmNitemCount, cMout,
    XmNitems, axsMout,
    NULL );
   XtVaSetValues( nnE_sListLinp,
    XmNitemCount, cLinp,
    XmNitems, axsLinp,
    NULL );
   XtVaSetValues( nnE_sListMinp,
    XmNitemCount, cMinp,
    XmNitems, axsMinp,
    NULL );
   for (i=0; i<cLout; i++)
      XmStringFree( axsLout[i]);
   for (i=0; i<cLinp; i++)
      XmStringFree( axsLinp[i]);
   for (i=0; i<cMout; i++)
      XmStringFree( axsMout[i]);
   for (i=0; i<cMinp; i++)
      XmStringFree( axsMinp[i]);
   XtFree( (char *) axsLout);
   XtFree( (char *) axsLinp);
   XtFree( (char *) axsMout);
   XtFree( (char *) axsMinp);
   mostra_cLMP();
   XtSetSensitive( nnE_pbIadd, False);
   XtSetSensitive( nnE_pbIdel, False);
   XtSetSensitive( nnE_pbOadd, False);
   XtSetSensitive( nnE_pbOdel, False);
}

/* ---------------------------------- */
static void nnE_load_PP( void )
{
/* carica le liste nel Modo PP */
 XmStringTable axsMout, axsMinp;
 char zM[20];
 int ivj, ivk, k, i;
   axsMout = (XmStringTable) XtMalloc( sizeof(XmString) * neqsis );
   axsMinp = (XmStringTable) XtMalloc( sizeof(XmString) * nu );
   zM[1] =' ';
   cMinp =cMout =cPinp =cPout =0;
   for (ivj=0; ivj<neqsis; ivj++) {
      if (! avvEdit[ivj].noto)
         continue;
      aiMout[cMout] = ivj;
      strncpy( zM+2, sivar[ivj], 8);
      zM[2+8] ='\0';
      for (k=0; k<cPair; k++)
         if (strcmp( zM+2, azPout[k]) == 0)
            break;
      if (k<cPair) {
         zM[0] ='-';
         strcat( zM, "-");
         strcat( zM, azPinp[k]);
         aiPout[k] = cMout +1;
         cPout++;
      } else
         zM[0] =' ';
      axsMout[cMout++] = XmStringCreateSimple( zM );
   }
   for (ivj=0; ivj<nu; ivj++) {
      if (avvEdit[neqsis+ivj].noto)
         continue;
      aiMinp[cMinp] = ivj;
      strncpy( zM+2, vari[ivj], 8);
      zM[2+8] ='\0';
      for (k=0; k<cPair; k++)
         if (strcmp( zM+2, azPinp[k]) == 0)
            break;
      if (k<cPair) {
         zM[0] ='-';
         strcat( zM, "-");
         strcat( zM, azPout[k]);
         aiPinp[k] = cMinp +1;
         cPinp++;
      } else
         zM[0] =' ';
      axsMinp[cMinp++] = XmStringCreateSimple( zM );
   }
   XmListDeselectAllItems( nnE_sListMinp);
   XmListDeselectAllItems( nnE_sListMout);
   XtVaSetValues( nnE_sListMout,
    XmNitemCount, cMout,
    XmNitems, axsMout,
    NULL );
   XtVaSetValues( nnE_sListMinp,
    XmNitemCount, cMinp,
    XmNitems, axsMinp,
    NULL );
   for (i=0; i<cMout; i++)
      XmStringFree( axsMout[i]);
   for (i=0; i<cMinp; i++)
      XmStringFree( axsMinp[i]);
   XtFree( (char *) axsMout);
   XtFree( (char *) axsMinp);
   mostra_cLMP();
   XtSetSensitive( nnE_pbPadd, False);
   XtSetSensitive( nnE_pbPdel, False);
   iPinpSel =iPoutSel =0;
}

/* ---------------------------------- */
static void deletePair( int k )
{
   XtFree( azPout[k-1]);
   XtFree( azPinp[k-1]);
   for (; k<cPair; k++) {
      azPout[k-1] = azPout[k];
      azPinp[k-1] = azPinp[k];
      aiPout[k-1] = aiPout[k];
      aiPinp[k-1] = aiPinp[k];
   }
   cPair--;
   aiPinp[cPair] =aiPout[cPair] =0;
}

/* ---------------------------------- */
void fnnF_ritorna_E( int *aiEsito)
{
 int i;
   if (aiEsito) {
      cLout =aiEsito[0];
      cLinp =aiEsito[1];
      for (i=0; i<cLout; i++)
         aiLout[i] = aiEsito[2+i];
      for (i=0; i<cLinp; i++)
         aiLinp[i] = neqsis -aiEsito[2+cLout+i];
      nnE_load_NN();
   }
}

/* ---------------------------------- */
void fnnV_ritorna_E( Boolean ok)
{
   if (ok)
      loStato |= NN_CHG_NN;
}

/* ---------------------------------- */
static void nnE_close_OK( void)
{
   UxDestroyInterface( nnEdit);
   exit(0);
}

/* ---------------------------------- */
static XtCallbackProc nnEdestroyShellCB(
 Widget UxWidget, XtPointer pC, XmAnyCallbackStruct *pA )
{ /* risponde a UxDestroyInterface() e a Close da Shell */
   libera_dati_privati();
   XtFree( nnEzH);
   loStato = NN_LIBERO;
   exit(0);
}

/* ---------------------------------- */
static XtCallbackProc nnEloadCB(
 Widget UxWidget, XtPointer pC, XmAnyCallbackStruct *pA )
{ /* carica/ripristina marcature e paia */
 int i;
   if (loStato & NN_OCCUPATO)
      libera_dati_privati();
   inizia_dati_privati();
   if (loStato & NN_OCCUPATO) {
      if (loStato & NN_MODE_NN)
         nnE_load_NN();
      else
         nnE_load_PP();
   }
   loStato &= (~NN_CHG_NN & ~NN_CHG_PP);
}

/* ---------------------------------- */
static XtCallbackProc nnEsaveCB(
 Widget UxWidget, XtPointer pC, XmAnyCallbackStruct *pA )
{
/* salva marcature e paia */
 FILE *fp;
 int bNN, bPP, e;
   bNN = loStato & NN_CHG_NN;
   bPP = loStato & NN_CHG_PP;
   if (bNN) {
      memcpy( &valout[0], &avvEdit[0], sizeof(VAL_VAR) * neqsis);
      memcpy( &valinp[0], &avvEdit[neqsis], sizeof(VAL_VAR) * nu);
      fp = fopen( FILE_F14, "w" );
      write_file_f14( fp);
      fclose( fp);
   }
   if (bPP) {
      write_file_fpp( sigla, cPair, azPout, azPinp);
   }
   if (bNN) {
      e = (bPP) ? ennEmsg_save_14_pp : ennEmsg_save_14;
   } else {
      e = (bPP) ? ennEmsg_save_pp : ennEmsg_save_none;
   }
   msgOn( e );
   loStato &= (~NN_CHG_NN & ~NN_CHG_PP);
}

/* ---------------------------------- */
static XtCallbackProc nnEcloseCB(
 Widget UxWidget, XtPointer pC, XmAnyCallbackStruct *pA )
{ /* chiude la pagina 'nnEdit' */
   if (loStato & NN_CHG_NN || loStato & NN_CHG_PP) {
      if (!wCloseDlg)
         wCloseDlg = conferma( UxTopLevel, nnEszClose, conferma_close );
      XtManageChild( wCloseDlg);
   } else
      nnE_close_OK();
}

/* ---------------------------------- */
static /*XtCallbackProc*/ void nnEcloseDlgCB(
 Widget UxWidget, int iC, XmAnyCallbackStruct *pA )
{ /* risponde al WarningDialog aperto da Menu File->Close */
   if (iC == 2) {
      XtUnmanageChild( wCloseDlg );
      return;
   }
   if (iC == 0)
      nnEsaveCB( NULL, 0, 0);
   nnE_close_OK();
}

/* ---------------------------------- */
static XtCallbackProc nnEsetupCB(
 Widget UxWidget, XtPointer pC, XmAnyCallbackStruct *pA )
{ /* risponde a Menu Options->Setup */
   if (!wSetupDlg)
      wSetupDlg = conferma( UxTopLevel, nnEszSetup, conferma_setup );
   XtManageChild( wSetupDlg);
}

/* ---------------------------------- */
static /*XtCallbackProc*/ void nnEsetupDlgCB(
 Widget UxWidget, int iC, XmAnyCallbackStruct *pA )
{ /* risponde al Dialogo aperto da Menu Options->Setup */
 int ivj;
   if (iC == 0) {
      for (ivj=0; ivj<neqsis; ivj++)
         avvEdit[ivj].noto = False;
      for (ivj=0; ivj<nu; ivj++)
         avvEdit[neqsis+ivj].noto = True;
      cKnown = nu;
      loStato |= NN_CHG_NN;
      while (cPair)
         deletePair( cPair +1 );
      cPair = 0;
      loStato |= NN_CHG_PP;
      nnEsaveCB( NULL, 0, 0);
      nnE_load_NN();
   }
   XtUnmanageChild( wSetupDlg );
}

/* ---------------------------------- */
static XtCallbackProc nnEsimpleCB_omuVars(
 Widget UxWidget, int q, XmPushButtonCallbackStruct *pA )
{
/* carica variabili (tutte, o filtrate) */
 int i;
   switch (q) {
   case NN_VARS_ALL:
      cLout =neqsis;
      cLinp =nu;
      for (i=0; i<cLout; i++)
         aiLout[i] = i+1;
      for (i=0; i<cLinp; i++)
         aiLinp[i] = -(i+1);
      if (UxWidget)
         nnE_load_NN();
      break;
   case NN_VARS_FILTER:

      create_nnFilter( UxWidget, nnEzH);

      break;
   default:
     break;
   }
}

/* ---------------------------------- */
static XtCallbackProc nnEhelpCB(
 Widget wgt, XtPointer pC, XmPushButtonCallbackStruct *pA )
{
/* espone Aiuti in linea */
   XtManageChild( nnE_help);
}
/* ---------------------------------- */

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	valueChangedCB_nnE_mu_pM_tN( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* imposta modo Normale */
	 XmToggleButtonCallbackStruct *pA =
	(XmToggleButtonCallbackStruct *)UxCallbackArg;
	   if (pA->set && !(loStato & NN_MODE_NN)) {
	      loStato |= NN_MODE_NN;
	      XtSetSensitive( nnE_mu_pO, True);
	      XtSetSensitive( nnE_pbPadd, False);
	      XtSetSensitive( nnE_pbPdel, False);
	      XtSetSensitive( nnE_omuVars, True);
	      nnE_load_NN();
	   }
	}
	UxNnEditContext = UxSaveCtx;
}

static void	valueChangedCB_nnE_mu_pM_tP( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* imposta modo Paia */
	 XmToggleButtonCallbackStruct *pA =
	(XmToggleButtonCallbackStruct *)UxCallbackArg;
	   if (pA->set && (loStato & NN_MODE_NN)) {
	      loStato &= ~NN_MODE_NN;
	      XtSetSensitive( nnE_mu_pO, False);
	      XtSetSensitive( nnE_omuVars, False);
	      XmListDeleteAllItems( nnE_sListLinp);
	      XmListDeleteAllItems( nnE_sListLout);
	      XtSetSensitive( nnE_pbIadd, False);
	      XtSetSensitive( nnE_pbIdel, False);
	      XtSetSensitive( nnE_pbOadd, False);
	      XtSetSensitive( nnE_pbOdel, False);
	      nnE_load_PP();
	   }
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_mu_pO_bG( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* apre pagina grafica */
	   /*create_nnGraph();*/
	   msgOn( ennEmsg_notavail );
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_mu_pO_bV( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* apre pagina ancillare 'nnValues' per editare valori */
	 int i, *aiI, cI, *aiO, cO, cValues, *aiValues;
	   if (!XmListGetSelectedPos( nnE_sListLinp, &aiI, &cI))
	      cI = 0;
	   if (!XmListGetSelectedPos( nnE_sListLout, &aiO, &cO))
	      cO = 0;
	   if (cValues = cI+cO) {
	      aiValues = (int *)XtMalloc( sizeof(int) * (1 + cValues) );
	      aiValues[0] = cValues;
	      if (cO) {
	         for (i=0; i<cO; i++)
	            aiValues[1+i] = aiLout[aiO[i]-1];
	         XtFree( (char *) aiO);
	      }
	      if (cI) {
	         for (i=0; i<cI; i++)
	            aiValues[1+cO+i] = aiLinp[aiI[i]-1];
	         XtFree( (char *) aiI);
	      }
	      create_nnValues( UxWidget, nnEzH, avvEdit, aiValues);
	      XtFree( (char *) aiValues);
	   } else
	      msgOn( ennEmsg_values );
	}
	UxNnEditContext = UxSaveCtx;
}

static void	createCB_nnE_formVars( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	 static String saz[8] = {
	  "Equations:", "Unknown:", "Input vars:", "Known:",
	  "EquationsD", "UnknownD", "Input varsD", "KnownD",
	 };
	 static Arg a[12] = {
	  { XmNtopAttachment, XmATTACH_POSITION },
	  { XmNbottomAttachment, XmATTACH_POSITION },
	  { XmNleftAttachment, XmATTACH_POSITION },
	  { XmNrightAttachment, XmATTACH_POSITION },
	  { XmNtopPosition, -1 },
	  { XmNbottomPosition, -1 },
	  { XmNleftPosition, -1 },
	  { XmNrightPosition, -1 },
	  { XmNborderWidth, -1 },
	  { XmNleftOffset, 10 },
	  { XmNrightOffset, 10 },
	  { XmNalignment, XmALIGNMENT_CENTER },
	 };
	 static int
	  saiTopBottom[3] = { 0, 50, 100 },
	  saiLeftRight[5] = { 0, 25, 50, 75, 100 },
	  saiBorderWidth[2] = { 0, 1 };
	 int i, j, k;
	   for (j=0, k=0; j<2; j++) {
	      a[4].value = saiTopBottom[j];
	      a[5].value = saiTopBottom[j+1];
	      a[8].value = saiBorderWidth[j];
	      for (i=0; i<4; i++, k++) {
	         a[6].value = saiLeftRight[i];
	         a[7].value = saiLeftRight[i+1];
	         nnE_labVars[k] = XtCreateWidget( saz[k],
	          xmLabelWidgetClass, UxWidget, a, 12 );
	      }
	   }
	   XtManageChildren( nnE_labVars, 8);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	valueChangedCB_nnE_scale( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	 XmScaleCallbackStruct *pA =
	(XmScaleCallbackStruct *)UxCallbackArg;
	/*
	 Arg a[1];
	   XtSetArg( a[0], XmNrightPosition, pA->value);
	   XtSetValues( nnE_sWinLout, a, 1);
	   XtSetValues( nnE_sWinLinp, a, 1);
	*/
	   set_something_val( nnE_sWinLout, XmNrightPosition, (XtArgVal) pA->value);
	   set_something_val( nnE_sWinLinp, XmNrightPosition, (XtArgVal) pA->value);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbOadd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* marca 'note' le out-var selezionate a sx */
	 int *aiPos, cPos, iS, i;
	 char zL[2+100+1], zM[2+8+1];
	 XmString xsL, xsM;
	   XmListGetSelectedPos( nnE_sListLout, &aiPos, &cPos );
	   zL[0] ='*';
	   zL[1] =zM[0] =zM[1] =' ';
	   zL[2+100] =zM[2+8] ='\0';
	   while (cPos) {
	      iS = aiPos[--cPos] -1;
	      if (ceInElencoInt( iS, cMout, aiMout))
	         continue;
	      for (i=cMout; i>0; i--)
	         if (aiMout[i-1] > iS)
	            aiMout[i] =aiMout[i-1];
	         else
	            break;
	      aiMout[i] = iS;
	      strncpy( zL+2, nom_sivar[aiLout[iS]-1], 100);
	      xsL =XmStringCreateSimple( zL);
	      XmListReplaceItemsPos( nnE_sListLout, &xsL, 1, iS+1 );
	      strncpy( zM+2, sivar[aiLout[iS]-1], 8);
	      xsM =XmStringCreateSimple( zM);
	      XmListAddItem( nnE_sListMout, xsM, i +1);
	      for (i=0; i<cPout; i++)
	         if (aiPout[i] > iS)
	            aiPout[i]++;
	      XmStringFree( xsL);
	      XmStringFree( xsM);
	      cMout++;
	      avvEdit[ aiLout[iS]-1].noto = True;
	      cKnown++;
	      loStato |=NN_CHG_NN;
	   }
	   XtFree( (char *) aiPos);
	   set_something_val( nnE_sListMout, XmNvisibleItemCount, (XtArgVal) cMout);
	   mostra_cLMP();
	   XmListDeselectAllItems( nnE_sListLout);
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbOdel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* marca 'non-note' le out-var selezionate a dx */
	 int *aiPos, cPos, iS, iM, iP, jP, jM, i;
	 char zL[2+100+1], zM[2+8+1];
	 XmString xsL, xsM;
	if (XmListGetSelectedPos( nnE_sListMout, &aiPos, &cPos )) {
	   zL[1] = zM[0] = zM[1] = ' ';
	   zL[2+100] ='\0';
	   while (cPos) {
	      iS = aiPos[--cPos];
	      iM =aiMout[iS-1];
	      zL[0] = ' ';
	      strncpy( zL+2, nom_sivar[aiLout[iM]-1], 100);
	      xsL =XmStringCreateSimple( zL);
	      XmListReplaceItemsPos( nnE_sListLout, &xsL, 1, iM+1 );
	      XmStringFree( xsL);
	      XmListDeletePos( nnE_sListMout, iS );
	      for (i=iS; i<cMout; i++)
	         aiMout[i-1] =aiMout[i];
	      cMout--;
	      avvEdit[aiLout[iM]-1].noto = False;
	      cKnown--;
	      loStato |= NN_CHG_NN;
	      if (iP = ceInElencoInt( iS, cPair, aiPout)) {
	         if ((jP=aiPinp[iP-1]) > 0) {
	          int *ajPos, cjPos;
	            jM = aiLinp[aiMinp[jP-1]];
	            zL[0] ='*';
	            strncpy( zL+2, nom_vari[-jM-1], 100);
	            xsL =XmStringCreateSimple( zL);
	            XmListReplaceItemsPos( nnE_sListLinp, &xsL, 1, aiMinp[jP-1]+1 );
	            XmStringFree( xsL);
	            if (XmListGetSelectedPos( nnE_sListMinp, &ajPos, &cjPos)) {
	               if (i = ceInElencoInt( jP, cjPos, ajPos))
	                  XmListSelectPos( nnE_sListMinp, jP, True);
	               XtFree( (char *) ajPos);
	            }
	            XmListDeletePos( nnE_sListMinp, jP);
	            for (i=jP; i<cMinp; i++)
	               aiMinp[i-1] =aiMinp[i];
	            cMinp--;
	            for (i=0; i<cPair; i++)
	               if (aiPinp[i]>jP)
	                  aiPinp[i]--;
	            cPinp--;
	         } else
	            jM = jP;
	         deletePair( iP);
	         for (i=0; i<cPair; i++)
	            if (aiPout[i]>iS)
	               aiPout[i]--;
	         cPout--;
	         avvEdit[neqsis-jM-1].noto = True;
	         cKnown++;
	         loStato |= NN_CHG_PP;
	      }
	   }
	   XmListDeselectAllItems( nnE_sListMout);
	   set_something_val( nnE_sListMout, XmNvisibleItemCount, (XtArgVal) cMout ? cMout : 1);
	   set_something_val( nnE_sListMinp, XmNvisibleItemCount, (XtArgVal) cMinp ? cMinp : 1);
	   mostra_cLMP();
	   XtFree( (char *) aiPos);
	   }
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	singleSelectionCB_nnE_sListLout( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* sente selezioni di out-var da marcare 'note' */
	 XmListCallbackStruct *pA =
	(XmListCallbackStruct *)UxCallbackArg;
	   int cSel;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cSel );
	   XmListDeselectAllItems( nnE_sListMout);
	   XtSetSensitive( nnE_pbOdel, False);
	   if (cSel && ceInElencoInt( pA->item_position -1, cMout, aiMout))
	      msgOn( ennEmsg_itemK );
	   XtSetSensitive( nnE_pbOadd, cSel);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	singleSelectionCB_nnE_sListMout( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* sente selezione di 1 out-var */
	 XmListCallbackStruct *pA =
	(XmListCallbackStruct *) UxCallbackArg;
	if (loStato & NN_MODE_NN) {
	/* modo Normal: da marcare 'non-nota' */
	   int cSel;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cSel );
	   XmListDeselectAllItems( nnE_sListLout);
	   if (cSel && ceInElencoInt( pA->item_position, cPair, aiPout))
	      msgOn( ennEmsg_pairDel );
	   XtSetSensitive( nnE_pbOadd, False);
	   XtSetSensitive( nnE_pbOdel, cSel);
	} else {
	/* modo Pairs: */
	 int cS, iP;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cS);
	   iP =ceInElencoInt( pA->item_position, cPout, aiPout);
	   iPoutSel = (cS) ? ((iP) ? -iP : pA->item_position) : 0;
	   if (iPinpSel<0
	    || iPinpSel*iPoutSel<0) {
	      msgOn( ennEmsg_badPair );
	      XmListDeselectItem( UxWidget, pA->item);
	      iPoutSel =0;
	   } else {
	      XtSetSensitive( nnE_pbPadd, False);
	      XtSetSensitive( nnE_pbPdel, False);
	      if (iPinpSel>0 && iPoutSel>0)
	         XtSetSensitive( nnE_pbPadd, True);
	      else if (iPoutSel<0)
	         XtSetSensitive( nnE_pbPdel, True);
	   }
	}
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbPadd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* crea 1 Paio */
	 char zM[20];
	 XmString xsM;
	   azPinp[cPair] =(char *) XtMalloc( 8+1);
	   azPout[cPair] =(char *) XtMalloc( 8+1);
	   strncpy( azPinp[cPair], vari[aiMinp[iPinpSel-1]], 8);
	   strncpy( azPout[cPair], sivar[aiMout[iPoutSel-1]], 8);
	   azPinp[cPair][8] =azPout[cPair][8] =zM[20] ='\0';
	   zM[0] =zM[10] ='-';
	   zM[1] =' ';
	   strncpy( zM+2, azPinp[cPair], 8);
	   strncpy( zM+11, azPout[cPair], 8);
	   xsM =XmStringCreateSimple( zM);
	   XmListReplaceItemsPos( nnE_sListMinp, &xsM, 1, iPinpSel);
	   XmStringFree( xsM);
	   strncpy( zM+2, azPout[cPair], 8);
	   strncpy( zM+11, azPinp[cPair], 8);
	   xsM =XmStringCreateSimple( zM);
	   XmListReplaceItemsPos( nnE_sListMout, &xsM, 1, iPoutSel);
	   XmStringFree( xsM);
	   cPair++;
	   aiPinp[cPinp++] =iPinpSel;
	   aiPout[cPout++] =iPoutSel;
	   mostra_cLMP();
	   loStato |=NN_CHG_PP;
	   iPinpSel =iPoutSel =0;
	   XmListDeselectAllItems( nnE_sListMinp);
	   XmListDeselectAllItems( nnE_sListMout);
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbIadd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* marca 'non-note' le inp-vars selezionate a sx */
	 int *aiPos, cPos, iS, i;
	 char zL[2+100+1], zM[2+8+1];
	 XmString xsL, xsM;
	   XmListGetSelectedPos( nnE_sListLinp, &aiPos, &cPos );
	   zL[0] =zL[1] =zM[0] =zM[1] =' ';
	   zL[2+100] =zM[2+8] ='\0';
	   while (cPos) {
	      iS = aiPos[--cPos] -1;
	      if (ceInElencoInt( iS, cMinp, aiMinp))
	         continue;
	      for (i=cMinp; i>0; i--)
	         if (aiMinp[i-1] > iS)
	            aiMinp[i] =aiMinp[i-1];
	         else
	            break;
	      aiMinp[i] = iS;
	      strncpy( zL+2, nom_vari[-aiLinp[iS]-1], 100);
	      xsL =XmStringCreateSimple( zL);
	      XmListReplaceItemsPos( nnE_sListLinp, &xsL, 1, iS+1 );
	      strncpy( zM+2, vari[-aiLinp[iS]-1], 8);
	      xsM =XmStringCreateSimple( zM);
	      XmListAddItem( nnE_sListMinp, xsM, i +1);
	      for (i=0; i<cPinp; i++)
	         if (aiPinp[i] > iS)
	            aiPinp[i]++;
	      XmStringFree( xsL);
	      XmStringFree( xsM);
	      cMinp++;
	      avvEdit[ neqsis-aiLinp[iS]-1 ].noto = False;
	      cKnown--;
	      loStato |= NN_CHG_NN;
	   }
	   XtFree( (char *) aiPos);
	   set_something_val( nnE_sListMinp, XmNvisibleItemCount, (XtArgVal) cMinp);
	   mostra_cLMP();
	   XmListDeselectAllItems( nnE_sListLinp);
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbIdel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* marca 'note' le inp-var selezionate a dx */
	 int *aiPos, cPos, iS, iM, iP, jP, jM, i;
	 char zL[2+100+1], zM[2+8+1];
	 XmString xsL, xsM;
	if (XmListGetSelectedPos( nnE_sListMinp, &aiPos, &cPos )) {
	   zL[1] =zM[0] =zM[1] =' ';
	   zL[2+100] ='\0';
	   while (cPos) {
	      iS = aiPos[--cPos];
	      iM =aiMinp[iS-1];
	      zL[0] ='*';
	      strncpy( zL+2, nom_vari[-aiLinp[iM]-1], 100);
	      xsL =XmStringCreateSimple( zL);
	      XmListReplaceItemsPos( nnE_sListLinp, &xsL, 1, iM+1 );
	      XmStringFree( xsL);
	      XmListDeletePos( nnE_sListMinp, iS );
	      for (i=iS; i<cMinp; i++)
	         aiMinp[i-1] =aiMinp[i];
	      cMinp--;
	      avvEdit[neqsis-aiLinp[iM]-1].noto = True;
	      cKnown++;
	      loStato |= NN_CHG_NN;
	      if (iP =ceInElencoInt( iS, cPair, aiPinp)) {
	         if ((jP=aiPout[iP-1]) > 0) {
	          int *ajPos, cjPos;
	            jM = aiLout[aiMout[jP-1]];
	            zL[0] =' ';
	            strncpy( zL+2, nom_sivar[jM-1], 100);
	            xsL =XmStringCreateSimple( zL);
	            XmListReplaceItemsPos( nnE_sListLout, &xsL, 1, aiMout[jP-1]+1 );
	            XmStringFree( xsL);
	            if (XmListGetSelectedPos( nnE_sListMout, &ajPos, &cjPos)) {
	               if (i = ceInElencoInt( jP, cjPos, ajPos))
	                  XmListSelectPos( nnE_sListMout, jP, True);
	               XtFree( (char *) ajPos);
	            }
	            XmListDeletePos( nnE_sListMout, jP);
	            for (i=jP; i<cMout; i++)
	               aiMout[i-1] =aiMout[i];
	            cMout--;
	            for (i=0; i<cPair; i++)
	               if (aiPout[i]>jP)
	                  aiPout[i]--;
	            cPout--;
	         } else
	            jM = -jP;
	         deletePair( iP);
	         for (i=0; i<cPair; i++)
	            if (aiPinp[i]>iS)
	               aiPinp[i]--;
	         cPinp--;
	         avvEdit[jM-1].noto = True;
	         cKnown--;
	         loStato |= NN_CHG_PP;
	      }
	   }
	   XmListDeselectAllItems( nnE_sListMinp);
	   set_something_val( nnE_sListMinp, XmNvisibleItemCount, (XtArgVal) cMinp ? cMinp : 1);
	   set_something_val( nnE_sListMout, XmNvisibleItemCount, (XtArgVal) cMout ? cMout : 1);
	   mostra_cLMP();
	   XtFree( (char *) aiPos);
	   }
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	singleSelectionCB_nnE_sListLinp( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* sente selezioni di inp-var da marcare 'non-note' */
	 XmListCallbackStruct *pA =
	(XmListCallbackStruct *)UxCallbackArg;
	   int cSel;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cSel );
	   XmListDeselectAllItems( nnE_sListMinp );
	   XtSetSensitive( nnE_pbIdel, False);
	   if (cSel && ceInElencoInt( pA->item_position -1, cMinp, aiMinp))
	      msgOn( ennEmsg_itemUnk );
	   XtSetSensitive( nnE_pbIadd, cSel);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	singleSelectionCB_nnE_sListMinp( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* sente selezione di 1 inp-var */
	 XmListCallbackStruct *pA =
	(XmListCallbackStruct *) UxCallbackArg;
	if (loStato & NN_MODE_NN) {
	/* modo Normal: da marcare 'nota' */
	   int cSel;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cSel );
	   XmListDeselectAllItems( nnE_sListLinp);
	   if (cSel && ceInElencoInt( pA->item_position, cPair, aiPinp))
	      msgOn( ennEmsg_pairDel );
	   XtSetSensitive( nnE_pbIadd, False);
	   XtSetSensitive( nnE_pbIdel, cSel);
	} else {
	/* modo Pairs: */
	 int cS, iP;
	   get_something( UxWidget, XmNselectedItemCount, (void*) &cS);
	   iP =ceInElencoInt( pA->item_position, cPinp, aiPinp);
	   iPinpSel = (cS) ? ((iP) ? -iP : pA->item_position) : 0;
	   if (iPoutSel<0
	    || iPinpSel*iPoutSel<0) {
	      msgOn( ennEmsg_badPair );
	      XmListDeselectItem( UxWidget, pA->item);
	      iPinpSel =0;
	   } else {
	      XtSetSensitive( nnE_pbPadd, False);
	      XtSetSensitive( nnE_pbPdel, False);
	      if (iPinpSel>0 && iPoutSel>0)
	         XtSetSensitive( nnE_pbPadd, True);
	      else if (iPinpSel<0)
	         XtSetSensitive( nnE_pbPdel, True);
	   }
	}
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_pbPdel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	/* cancella 1 Paio */
	 int iP;
	 char zM[2+8+1];
	 XmString xsM;
	   iP = (iPinpSel) ? -iPinpSel : -iPoutSel;
	   zM[0] =zM[1] =' ';
	   strcpy( zM+2, azPinp[iP-1]);
	   xsM =XmStringCreateSimple( zM);
	   XmListReplaceItemsPos( nnE_sListMinp, &xsM, 1, aiPinp[iP-1]);
	   XmStringFree( xsM);
	   strcpy( zM+2, azPout[iP-1]);
	   xsM =XmStringCreateSimple( zM);
	   XmListReplaceItemsPos( nnE_sListMout, &xsM, 1, aiPout[iP-1]);
	   XmStringFree( xsM);
	   deletePair( iP);
	   cPinp--;
	   cPout--;
	   mostra_cLMP();
	   loStato |=NN_CHG_PP;
	   iPinpSel =iPoutSel =0;
	   XmListDeselectAllItems( nnE_sListMinp);
	   XmListDeselectAllItems( nnE_sListMout);
	   XtSetSensitive( UxWidget, False);
	}
	UxNnEditContext = UxSaveCtx;
}

static void	activateCB_nnE_dbMsg( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCnnEdit              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxNnEditContext;
	UxNnEditContext = UxContext =
			(_UxCnnEdit *) UxGetContext( UxWidget );
	{
	   msgOn( lastMsg );
	}
	UxNnEditContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_nnEdit()
{
	Widget	nnE_mu_pF_shell;
	Widget	nnE_mu_pM_shell;
	Widget	nnE_mu_pO_shell;
	Widget	nnE_mu_pH_shell;
	Widget	nnE_omuV_p1_shell;

	nnEdit = XtVaCreatePopupShell( "nnEdit",
			topLevelShellWidgetClass, UxTopLevel,
			XmNy, 151,
			XmNx, 314,
			XmNdeleteResponse, XmDESTROY,
			XmNheight, 648,
			XmNwidth, 806,
			NULL );

	UxPutContext( nnEdit, (char *) UxNnEditContext );

	nnE_mainWin = XtVaCreateManagedWidget( "nnE_mainWin",
			xmMainWindowWidgetClass, nnEdit,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( nnE_mainWin, (char *) UxNnEditContext );

	nnE_mu = XtVaCreateManagedWidget( "nnE_mu",
			xmRowColumnWidgetClass, nnE_mainWin,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( nnE_mu, (char *) UxNnEditContext );

	nnE_mu_pF_shell = XtVaCreatePopupShell ("nnE_mu_pF_shell",
			xmMenuShellWidgetClass, nnE_mu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nnE_mu_pF = XtVaCreateWidget( "nnE_mu_pF",
			xmRowColumnWidgetClass, nnE_mu_pF_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nnE_mu_pF, (char *) UxNnEditContext );

	nnE_mu_pF_bL = XtVaCreateManagedWidget( "nnE_mu_pF_bL",
			xmPushButtonWidgetClass, nnE_mu_pF,
			RES_CONVERT( XmNlabelString, "Load" ),
			NULL );

	UxPutContext( nnE_mu_pF_bL, (char *) UxNnEditContext );

	nnE_mu_pF_bS = XtVaCreateManagedWidget( "nnE_mu_pF_bS",
			xmPushButtonWidgetClass, nnE_mu_pF,
			RES_CONVERT( XmNlabelString, "Save" ),
			NULL );

	UxPutContext( nnE_mu_pF_bS, (char *) UxNnEditContext );

	nnE_mu_pF_bZ = XtVaCreateManagedWidget( "nnE_mu_pF_bZ",
			xmPushButtonWidgetClass, nnE_mu_pF,
			RES_CONVERT( XmNlabelString, "Close" ),
			NULL );

	UxPutContext( nnE_mu_pF_bZ, (char *) UxNnEditContext );

	nnE_mu_top_bF = XtVaCreateManagedWidget( "nnE_mu_top_bF",
			xmCascadeButtonWidgetClass, nnE_mu,
			XmNsubMenuId, nnE_mu_pF,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( nnE_mu_top_bF, (char *) UxNnEditContext );

	nnE_mu_pM_shell = XtVaCreatePopupShell ("nnE_mu_pM_shell",
			xmMenuShellWidgetClass, nnE_mu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nnE_mu_pM = XtVaCreateWidget( "nnE_mu_pM",
			xmRowColumnWidgetClass, nnE_mu_pM_shell,
			XmNradioBehavior, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nnE_mu_pM, (char *) UxNnEditContext );

	nnE_mu_pM_tN = XtVaCreateManagedWidget( "nnE_mu_pM_tN",
			xmToggleButtonWidgetClass, nnE_mu_pM,
			RES_CONVERT( XmNlabelString, "Normal" ),
			NULL );

	UxPutContext( nnE_mu_pM_tN, (char *) UxNnEditContext );

	nnE_mu_pM_tP = XtVaCreateManagedWidget( "nnE_mu_pM_tP",
			xmToggleButtonWidgetClass, nnE_mu_pM,
			RES_CONVERT( XmNlabelString, "Pairs" ),
			NULL );

	UxPutContext( nnE_mu_pM_tP, (char *) UxNnEditContext );

	nnE_mu_top_bM = XtVaCreateManagedWidget( "nnE_mu_top_bM",
			xmCascadeButtonGadgetClass, nnE_mu,
			XmNsubMenuId, nnE_mu_pM,
			RES_CONVERT( XmNlabelString, "Mode" ),
			NULL );

	UxPutContext( nnE_mu_top_bM, (char *) UxNnEditContext );

	nnE_mu_pO_shell = XtVaCreatePopupShell ("nnE_mu_pO_shell",
			xmMenuShellWidgetClass, nnE_mu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nnE_mu_pO = XtVaCreateWidget( "nnE_mu_pO",
			xmRowColumnWidgetClass, nnE_mu_pO_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nnE_mu_pO, (char *) UxNnEditContext );

	nnE_mu_pO_bG = XtVaCreateManagedWidget( "nnE_mu_pO_bG",
			xmPushButtonWidgetClass, nnE_mu_pO,
			RES_CONVERT( XmNlabelString, "Graph" ),
			NULL );

	UxPutContext( nnE_mu_pO_bG, (char *) UxNnEditContext );

	nnE_mu_pO_bV = XtVaCreateManagedWidget( "nnE_mu_pO_bV",
			xmPushButtonWidgetClass, nnE_mu_pO,
			RES_CONVERT( XmNlabelString, "Values" ),
			NULL );

	UxPutContext( nnE_mu_pO_bV, (char *) UxNnEditContext );

	nnE_mu_pO_sep = XtVaCreateManagedWidget( "nnE_mu_pO_sep",
			xmSeparatorWidgetClass, nnE_mu_pO,
			NULL );

	UxPutContext( nnE_mu_pO_sep, (char *) UxNnEditContext );

	nnE_mu_pO_bK = XtVaCreateManagedWidget( "nnE_mu_pO_bK",
			xmPushButtonWidgetClass, nnE_mu_pO,
			RES_CONVERT( XmNlabelString, "all Var Startup" ),
			NULL );

	UxPutContext( nnE_mu_pO_bK, (char *) UxNnEditContext );

	nnE_mu_top_bO = XtVaCreateManagedWidget( "nnE_mu_top_bO",
			xmCascadeButtonGadgetClass, nnE_mu,
			XmNsubMenuId, nnE_mu_pO,
			RES_CONVERT( XmNlabelString, "Options" ),
			NULL );

	UxPutContext( nnE_mu_top_bO, (char *) UxNnEditContext );

	nnE_mu_pH_shell = XtVaCreatePopupShell ("nnE_mu_pH_shell",
			xmMenuShellWidgetClass, nnE_mu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nnE_mu_pH = XtVaCreateWidget( "nnE_mu_pH",
			xmRowColumnWidgetClass, nnE_mu_pH_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nnE_mu_pH, (char *) UxNnEditContext );

	nnE_mu_pH_bP = XtVaCreateManagedWidget( "nnE_mu_pH_bP",
			xmPushButtonWidgetClass, nnE_mu_pH,
			RES_CONVERT( XmNlabelString, "on Page" ),
			NULL );

	UxPutContext( nnE_mu_pH_bP, (char *) UxNnEditContext );

	nnE_mu_top_bH = XtVaCreateManagedWidget( "nnE_mu_top_bH",
			xmCascadeButtonWidgetClass, nnE_mu,
			XmNsubMenuId, nnE_mu_pH,
			RES_CONVERT( XmNlabelString, "Help" ),
			NULL );

	UxPutContext( nnE_mu_top_bH, (char *) UxNnEditContext );

	nnE_form = XtVaCreateManagedWidget( "nnE_form",
			xmFormWidgetClass, nnE_mainWin,
			XmNmarginWidth, 10,
			XmNmarginHeight, 10,
			NULL );

	UxPutContext( nnE_form, (char *) UxNnEditContext );

	nnE_labModel = XtVaCreateManagedWidget( "nnE_labModel",
			xmLabelWidgetClass, nnE_form,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labModel, (char *) UxNnEditContext );

	nnE_formVars = XtVaCreateManagedWidget( "nnE_formVars",
			xmFormWidgetClass, nnE_form,
			XmNtopOffset, 10,
			XmNtopWidget, nnE_labModel,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, 40,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( nnE_formVars, (char *) UxNnEditContext );

	createCB_nnE_formVars( nnE_formVars,
			(XtPointer) UxNnEditContext, (XtPointer) NULL );

	nnE_scale = XtVaCreateManagedWidget( "nnE_scale",
			xmScaleWidgetClass, nnE_form,
			XmNshowValue, TRUE,
			XmNvalue, 70,
			XmNminimum, 60,
			XmNmaximum, 80,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, nnE_formVars,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNorientation, XmHORIZONTAL,
			NULL );

	UxPutContext( nnE_scale, (char *) UxNnEditContext );

	nnE_panedWin = XtVaCreateManagedWidget( "nnE_panedWin",
			xmPanedWindowWidgetClass, nnE_form,
			XmNwidth, 790,
			XmNtopWidget, nnE_scale,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_panedWin, (char *) UxNnEditContext );

	nnE_formOut = XtVaCreateManagedWidget( "nnE_formOut",
			xmFormWidgetClass, nnE_panedWin,
			XmNmarginWidth, 5,
			XmNmarginHeight, 5,
			XmNshadowThickness, 2,
			XmNheight, 237,
			XmNy, 3,
			XmNx, 3,
			XmNpaneMinimum, 160,
			NULL );

	UxPutContext( nnE_formOut, (char *) UxNnEditContext );

	nnE_labOut = XtVaCreateManagedWidget( "nnE_labOut",
			xmLabelWidgetClass, nnE_formOut,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Output variables" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labOut, (char *) UxNnEditContext );

	nnE_labLout = XtVaCreateManagedWidget( "nnE_labLout",
			xmLabelWidgetClass, nnE_formOut,
			XmNtopWidget, nnE_labOut,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labLout, (char *) UxNnEditContext );

	nnE_labMout = XtVaCreateManagedWidget( "nnE_labMout",
			xmLabelWidgetClass, nnE_formOut,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, nnE_labLout,
			XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
			NULL );

	UxPutContext( nnE_labMout, (char *) UxNnEditContext );

	nnE_pbOadd = XtVaCreateManagedWidget( "nnE_pbOadd",
			xmPushButtonWidgetClass, nnE_formOut,
			RES_CONVERT( XmNlabelString, " Mark " ),
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_pbOadd, (char *) UxNnEditContext );

	nnE_pbOdel = XtVaCreateManagedWidget( "nnE_pbOdel",
			xmPushButtonWidgetClass, nnE_formOut,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Unmark" ),
			XmNheight, 35,
			XmNleftOffset, 10,
			XmNleftWidget, nnE_pbOadd,
			XmNleftAttachment, XmATTACH_WIDGET,
			NULL );

	UxPutContext( nnE_pbOdel, (char *) UxNnEditContext );

	nnE_sWinLout = XtVaCreateManagedWidget( "nnE_sWinLout",
			xmScrolledWindowWidgetClass, nnE_formOut,
			XmNshadowThickness, 0,
			XmNbottomOffset, 5,
			XmNrightPosition, 70,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, nnE_pbOadd,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNtopWidget, nnE_labLout,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( nnE_sWinLout, (char *) UxNnEditContext );

	nnE_sListLout = XtVaCreateManagedWidget( "nnE_sListLout",
			xmListWidgetClass, nnE_sWinLout,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNheight, 120,
			XmNwidth, 390,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );

	UxPutContext( nnE_sListLout, (char *) UxNnEditContext );

	nnE_sWinMout = XtVaCreateManagedWidget( "nnE_sWinMout",
			xmScrolledWindowWidgetClass, nnE_formOut,
			XmNshadowThickness, 0,
			XmNy, 38,
			XmNx, 426,
			XmNleftWidget, nnE_sWinLout,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopWidget, nnE_sWinLout,
			XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNbottomWidget, nnE_sWinLout,
			XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( nnE_sWinMout, (char *) UxNnEditContext );

	nnE_sListMout = XtVaCreateManagedWidget( "nnE_sListMout",
			xmListWidgetClass, nnE_sWinMout,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNheight, 120,
			XmNwidth, 140,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );

	UxPutContext( nnE_sListMout, (char *) UxNnEditContext );

	nnE_pbPadd = XtVaCreateManagedWidget( "nnE_pbPadd",
			xmPushButtonWidgetClass, nnE_formOut,
			RES_CONVERT( XmNlabelString, "Pcreate" ),
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_pbPadd, (char *) UxNnEditContext );

	nnE_formInp = XtVaCreateManagedWidget( "nnE_formInp",
			xmFormWidgetClass, nnE_panedWin,
			XmNshadowThickness, 2,
			XmNmarginWidth, 5,
			XmNmarginHeight, 5,
			XmNheight, 220,
			XmNpaneMinimum, 160,
			NULL );

	UxPutContext( nnE_formInp, (char *) UxNnEditContext );

	nnE_labInp = XtVaCreateManagedWidget( "nnE_labInp",
			xmLabelWidgetClass, nnE_formInp,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Input variables" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labInp, (char *) UxNnEditContext );

	nnE_labLinp = XtVaCreateManagedWidget( "nnE_labLinp",
			xmLabelWidgetClass, nnE_formInp,
			XmNtopWidget, nnE_labInp,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labLinp, (char *) UxNnEditContext );

	nnE_labMinp = XtVaCreateManagedWidget( "nnE_labMinp",
			xmLabelWidgetClass, nnE_formInp,
			XmNtopWidget, nnE_labLinp,
			XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_labMinp, (char *) UxNnEditContext );

	nnE_pbIadd = XtVaCreateManagedWidget( "nnE_pbIadd",
			xmPushButtonWidgetClass, nnE_formInp,
			RES_CONVERT( XmNlabelString, "Unmark" ),
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_pbIadd, (char *) UxNnEditContext );

	nnE_pbIdel = XtVaCreateManagedWidget( "nnE_pbIdel",
			xmPushButtonWidgetClass, nnE_formInp,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, " Mark " ),
			XmNheight, 35,
			XmNleftOffset, 10,
			XmNleftWidget, nnE_pbIadd,
			XmNleftAttachment, XmATTACH_WIDGET,
			NULL );

	UxPutContext( nnE_pbIdel, (char *) UxNnEditContext );

	nnE_sWinLinp = XtVaCreateManagedWidget( "nnE_sWinLinp",
			xmScrolledWindowWidgetClass, nnE_formInp,
			XmNshadowThickness, 0,
			XmNy, 38,
			XmNx, 14,
			XmNbottomOffset, 5,
			XmNtopWidget, nnE_labLinp,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, nnE_pbIadd,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNrightPosition, 70,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftAttachment, XmATTACH_FORM,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( nnE_sWinLinp, (char *) UxNnEditContext );

	nnE_sListLinp = XtVaCreateManagedWidget( "nnE_sListLinp",
			xmListWidgetClass, nnE_sWinLinp,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNheight, 120,
			XmNwidth, 390,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );

	UxPutContext( nnE_sListLinp, (char *) UxNnEditContext );

	nnE_sWinMinp = XtVaCreateManagedWidget( "nnE_sWinMinp",
			xmScrolledWindowWidgetClass, nnE_formInp,
			XmNshadowThickness, 0,
			XmNy, 38,
			XmNx, 426,
			XmNleftWidget, nnE_sWinLinp,
			XmNleftOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, nnE_sWinLinp,
			XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNbottomWidget, nnE_sWinLinp,
			XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( nnE_sWinMinp, (char *) UxNnEditContext );

	nnE_sListMinp = XtVaCreateManagedWidget( "nnE_sListMinp",
			xmListWidgetClass, nnE_sWinMinp,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			XmNheight, 120,
			XmNwidth, 140,
			XmNselectionPolicy, XmSINGLE_SELECT,
			NULL );

	UxPutContext( nnE_sListMinp, (char *) UxNnEditContext );

	nnE_pbPdel = XtVaCreateManagedWidget( "nnE_pbPdel",
			xmPushButtonWidgetClass, nnE_formInp,
			RES_CONVERT( XmNlabelString, "Pdelete" ),
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 35,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );

	UxPutContext( nnE_pbPdel, (char *) UxNnEditContext );

	nnE_omuV_p1_shell = XtVaCreatePopupShell ("nnE_omuV_p1_shell",
			xmMenuShellWidgetClass, nnE_form,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	nnE_omuV_p1 = XtVaCreateWidget( "nnE_omuV_p1",
			xmRowColumnWidgetClass, nnE_omuV_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( nnE_omuV_p1, (char *) UxNnEditContext );

	nnE_omuVAll = XtVaCreateManagedWidget( "nnE_omuVAll",
			xmPushButtonWidgetClass, nnE_omuV_p1,
			RES_CONVERT( XmNlabelString, "All" ),
			NULL );

	UxPutContext( nnE_omuVAll, (char *) UxNnEditContext );

	nnE_omuVFilter = XtVaCreateManagedWidget( "nnE_omuVFilter",
			xmPushButtonWidgetClass, nnE_omuV_p1,
			RES_CONVERT( XmNlabelString, "Filter" ),
			NULL );

	UxPutContext( nnE_omuVFilter, (char *) UxNnEditContext );

	nnE_omuVars = XtVaCreateManagedWidget( "nnE_omuVars",
			xmRowColumnWidgetClass, nnE_form,
			XmNbottomWidget, nnE_scale,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
			RES_CONVERT( XmNlabelString, "Show:" ),
			XmNsubMenuId, nnE_omuV_p1,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( nnE_omuVars, (char *) UxNnEditContext );

	nnE_dbMsg = XtVaCreateManagedWidget( "nnE_dbMsg",
			xmDrawnButtonWidgetClass, nnE_form,
			XmNrightOffset, 10,
			XmNleftOffset, 10,
			XmNtopOffset, 5,
			RES_CONVERT( XmNforeground, "Red" ),
			XmNtopWidget, nnE_formVars,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightWidget, nnE_scale,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftWidget, nnE_omuVars,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, nnE_scale,
			XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
			XmNheight, 40,
			XmNwidth, 410,
			XmNy, 40,
			XmNx, 120,
			NULL );

	UxPutContext( nnE_dbMsg, (char *) UxNnEditContext );

	XtAddCallback( nnEdit, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnEdit, XmNdestroyCallback,
			nnEdestroyShellCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mainWin, XmNhelpCallback,
			nnEhelpCB,
			(XtPointer) UxNnEditContext );

	XtVaSetValues(nnE_mu,
			XmNmenuHelpWidget, nnE_mu_top_bH,
			NULL );

	XtAddCallback( nnE_mu_pF_bL, XmNactivateCallback,
			nnEloadCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pF_bS, XmNactivateCallback,
			nnEsaveCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pF_bZ, XmNactivateCallback,
			nnEcloseCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pM_tN, XmNvalueChangedCallback,
			valueChangedCB_nnE_mu_pM_tN,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pM_tP, XmNvalueChangedCallback,
			valueChangedCB_nnE_mu_pM_tP,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pO_bG, XmNactivateCallback,
			activateCB_nnE_mu_pO_bG,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pO_bV, XmNactivateCallback,
			activateCB_nnE_mu_pO_bV,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pO_bK, XmNactivateCallback,
			nnEsetupCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_mu_pH_bP, XmNactivateCallback,
			nnEhelpCB,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_scale, XmNvalueChangedCallback,
			valueChangedCB_nnE_scale,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbOadd, XmNactivateCallback,
			activateCB_nnE_pbOadd,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbOdel, XmNactivateCallback,
			activateCB_nnE_pbOdel,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_sListLout, XmNsingleSelectionCallback,
			singleSelectionCB_nnE_sListLout,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_sListMout, XmNsingleSelectionCallback,
			singleSelectionCB_nnE_sListMout,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbPadd, XmNactivateCallback,
			activateCB_nnE_pbPadd,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbIadd, XmNactivateCallback,
			activateCB_nnE_pbIadd,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbIdel, XmNactivateCallback,
			activateCB_nnE_pbIdel,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_sListLinp, XmNsingleSelectionCallback,
			singleSelectionCB_nnE_sListLinp,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_sListMinp, XmNsingleSelectionCallback,
			singleSelectionCB_nnE_sListMinp,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_pbPdel, XmNactivateCallback,
			activateCB_nnE_pbPdel,
			(XtPointer) UxNnEditContext );

	XtAddCallback( nnE_omuVAll, XmNactivateCallback,
			nnEsimpleCB_omuVars,
			(XtPointer) NN_VARS_ALL );

	XtAddCallback( nnE_omuVFilter, XmNactivateCallback,
			nnEsimpleCB_omuVars,
			(XtPointer) NN_VARS_FILTER );

	XtAddCallback( nnE_dbMsg, XmNactivateCallback,
			activateCB_nnE_dbMsg,
			(XtPointer) UxNnEditContext );


	XmMainWindowSetAreas( nnE_mainWin, nnE_mu, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, nnE_form );

	return ( nnEdit );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_nnEdit( _UxnnEzHfile )
	String	_UxnnEzHfile;
{
	Widget                  rtrn;
	_UxCnnEdit              *UxContext;

	UxNnEditContext = UxContext =
		(_UxCnnEdit *) XtMalloc( sizeof(_UxCnnEdit) );

	nnEzHfile = _UxnnEzHfile;
	{
		/* codice iniziale: */
		if (loStato & NN_OCCUPATO) {
		   fprintf( stderr, "\a");
		   return NULL;
		}
		/* legge file "f03.dat" per dati su blocchi e variabili */
		   if (read_file_f03()) {
		      printf( "\nCannot open file f03\n");
		      exit(1);
		   }
		/* legge file "f14.h" per valori e attributi delle variabili */
		   if (read_file_f14( neqsis, nu) ) {
		      printf( "Cannot open file f14\n");
		      exit(1);
		   }
		   printf( "\n");
		rtrn = _Uxbuild_nnEdit();

		/* codice finale: */
		UxPopupInterface( rtrn, no_grab);
		
		{ char ahModel[22];
		   sprintf( ahModel, "Model name: %.8s", sigla);
		   UxPutStrRes( nnE_labModel, XmNlabelString, ahModel);
		}
		UxPutStrRes( nnE_dbMsg, XmNlabelString, nnEazMsgs[ennEmsg_null]);
		{ /* a[10] per find_kit() della libreria libutilx... */
		 Arg a[10];
		 Cardinal n;
		   n=0;
		   XtSetArg( a[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		   XtSetArg( a[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
		   XtSetArg( a[n], XmNleftOffset, 10); n++;
		   XtSetArg( a[n], XmNleftWidget, nnE_pbOdel ); n++;
		   wgtFindOut = find_kit( nnE_formOut, a, n, &fsOut );
		   fsOut.ID_lista = (Widget) nnE_sListLout;
		   n=0;
		   XtSetArg( a[n], XmNbottomAttachment, XmATTACH_FORM); n++;
		   XtSetArg( a[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
		   XtSetArg( a[n], XmNleftOffset, 10); n++;
		   XtSetArg( a[n], XmNleftWidget, nnE_pbIdel ); n++;
		   wgtFindInp = find_kit( nnE_formInp, a, n, &fsInp );
		   fsInp.ID_lista = (Widget) nnE_sListLinp;
		}
		wCloseDlg = wSetupDlg = 0;
		nnEzH = strdup( nnEzHfile);
		nnE_help = helpkit_propInfo( nnE_mainWin, nnEzH, nnEzHpage);
		nnEloadCB( NULL, 0, 0);
		lastMsg = idMsg = 0;
		loStato |= NN_OCCUPATO;
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_nnEdit( _UxnnEzHfile )
	String	_UxnnEzHfile;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_nnEdit( _UxnnEzHfile );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

