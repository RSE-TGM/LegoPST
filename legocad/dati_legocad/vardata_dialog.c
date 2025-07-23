
/*******************************************************************************
	vardata_dialog.c
	(Generated from interface file vardata_dialog.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/ArrowB.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/Form.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo vardata_dialog.i
   tipo 
   release 2.21
   data 11/3/95
   reserved @(#)vardata_dialog.i	2.21
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include <malloc.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <Xm/MenuShell.h>
#include <Xm/BulletinB.h>
#include "f14.h"
#include "f03.h"
#include "uni_mis.h"
#include "dati.h"
extern Boolean glob_modified;
extern int n_editors;
extern S_UNI_MIS uni_mis[];
extern VAL_VAR *valout;
extern VAL_VAR *valinp;
extern DATO_RIGA *array_dati;
extern HEADER_DATI *header_dati;
extern DATO_DESCR *array_descr;
extern int num_note;
#define TIPO_INP 1 /* variabile di ingresso per il modello */
#define TIPO_OUT 0 /* variabile di uscita per il modello */
#define TIPO_INPBLO 2 /* variabile in ingresso per il blocco */
#define TIPO_OUTBLO 3 /* variabile in uscita per il blocco */
#define TIPO_DOPPIA -1 /* variabile annullata dalla lista perche'
                          gia' presente */

/*
 definizione tipo di riga nella lista parametri
*/
#define TIPO_RIGA_DESCR 0
#define TIPO_RIGA_PARAM 1
#define TIPO_RIGA_INTESTA 2
/*
 struttura per editing dei valori delle variabili
*/
typedef struct riga_edi{
			int tipo; /* indica se di input o di output al modello */
			int punt; /* indica la posizione in valinp o valout */
			int io_blo; /* indica se ingresso o uscita dal blocco */
			char etichetta[120];
			char valore_str[120];
			int num_options;
			int num_opt_sel;
			int nota;
			char opzione[N_TIPI_UMIS][L_NOMI_UMIS];
			swidget sw_opt; /* option menu */
			swidget sw_val; /* testo per inserim.valore */
			swidget sw_nn;  /* toggle button per noto/non noto */
			} RIGA_EDI;
/*
 strutture per editing dei dati (parametri) associati ai blocchi
*/
typedef struct param {
			char descr_str[9];
			char valore_str[11];
			swidget sw_val;
			int punt; /* posizione nell'array dei dati */
			} PARAM;

typedef union u_param{
			char descr_str[L_RIGA_F14];
			PARAM par[3];
			} U_PARAM;

typedef struct riga_param {
			int tipo_riga;
			U_PARAM u;
			} RIGA_PARAM;
/*
 lista per editing dei valori da assegnare alle variabili
*/
#ifdef ORIGINALE
typedef struct edit_list{
		int num_righe;
		int last_selected;
		RIGA_EDI *riga;
		} EDIT_LIST;
/*
 lista per editing dei dati (parametri)
*/
typedef struct param_list{
		int num_righe;
		int last_selected;
		RIGA_PARAM *riga;
		} PARAM_LIST;
#else
typedef struct edit_list{
		int num_righe;
		int last_selected;
		RIGA_EDI riga[1000];
		} EDIT_LIST;
/*
 lista per editing dei dati (parametri)
*/
typedef struct param_list{
		int num_righe;
		int last_selected;
		RIGA_PARAM riga[1000];
		} PARAM_LIST;
#endif

/*
 union utilizzata per l'individuazione nella opt_callback
 del bottone di opzione selezionato
*/
typedef union riga_opt{
		char c[2];
		int i;
		}RIGA_OPT;
void opt_callback();
void mod_callback();

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
	Widget	Uxvardata_dialog;
	Widget	Uxapply_button;
	Widget	Uxfind_rc;
	Widget	UxlabelGadget4;
	Widget	Uxfind_text;
	Widget	UxarrowButton2;
	Widget	UxarrowButton1;
	Widget	Uxedit_pulldown;
	Widget	Uxedit_File;
	Widget	Uxedit_Load;
	Widget	Uxedit_Close;
	Widget	Uxmenu1_top_b1;
	Widget	Uxedit_Edit;
	Widget	Uxedit_Restore;
	Widget	Uxmenu1_top_b2;
	Widget	UxDoc;
	Widget	UxBDoc;
	Widget	Uxedit_pulldown_top_b1;
	Widget	UxscrolledWindow2;
	Widget	UxlistaBlocchiSel;
	Widget	Uxvalori_p1;
	Widget	Uxvalori_Variables;
	Widget	Uxvalori_Inp_Var;
	Widget	Uxvalori_Out_Var;
	Widget	Uxvalori_Data;
	Widget	Uxmenu_valori;
	Widget	Uxintesta_rc;
	Widget	UxlabelGadget1;
	Widget	UxlabelGadget2;
	Widget	UxlabelGadget3;
	Widget	Uxvardata_sw;
	Widget	Uxvardata_rc;
	Widget	Uxsw_top;
	int	Uxlista_edit_on;
	int	Uxtipo_lista;
	EDIT_LIST	Uxlista;
	PARAM_LIST	Uxlparam;
	Boolean	Uxmodified;
	int	Uxtipolista;
} _UxCvardata_dialog;

#define vardata_dialog          UxVardata_dialogContext->Uxvardata_dialog
#define apply_button            UxVardata_dialogContext->Uxapply_button
#define find_rc                 UxVardata_dialogContext->Uxfind_rc
#define labelGadget4            UxVardata_dialogContext->UxlabelGadget4
#define find_text               UxVardata_dialogContext->Uxfind_text
#define arrowButton2            UxVardata_dialogContext->UxarrowButton2
#define arrowButton1            UxVardata_dialogContext->UxarrowButton1
#define edit_pulldown           UxVardata_dialogContext->Uxedit_pulldown
#define edit_File               UxVardata_dialogContext->Uxedit_File
#define edit_Load               UxVardata_dialogContext->Uxedit_Load
#define edit_Close              UxVardata_dialogContext->Uxedit_Close
#define menu1_top_b1            UxVardata_dialogContext->Uxmenu1_top_b1
#define edit_Edit               UxVardata_dialogContext->Uxedit_Edit
#define edit_Restore            UxVardata_dialogContext->Uxedit_Restore
#define menu1_top_b2            UxVardata_dialogContext->Uxmenu1_top_b2
#define Doc                     UxVardata_dialogContext->UxDoc
#define BDoc                    UxVardata_dialogContext->UxBDoc
#define edit_pulldown_top_b1    UxVardata_dialogContext->Uxedit_pulldown_top_b1
#define scrolledWindow2         UxVardata_dialogContext->UxscrolledWindow2
#define listaBlocchiSel         UxVardata_dialogContext->UxlistaBlocchiSel
#define valori_p1               UxVardata_dialogContext->Uxvalori_p1
#define valori_Variables        UxVardata_dialogContext->Uxvalori_Variables
#define valori_Inp_Var          UxVardata_dialogContext->Uxvalori_Inp_Var
#define valori_Out_Var          UxVardata_dialogContext->Uxvalori_Out_Var
#define valori_Data             UxVardata_dialogContext->Uxvalori_Data
#define menu_valori             UxVardata_dialogContext->Uxmenu_valori
#define intesta_rc              UxVardata_dialogContext->Uxintesta_rc
#define labelGadget1            UxVardata_dialogContext->UxlabelGadget1
#define labelGadget2            UxVardata_dialogContext->UxlabelGadget2
#define labelGadget3            UxVardata_dialogContext->UxlabelGadget3
#define vardata_sw              UxVardata_dialogContext->Uxvardata_sw
#define vardata_rc              UxVardata_dialogContext->Uxvardata_rc
#define sw_top                  UxVardata_dialogContext->Uxsw_top
#define lista_edit_on           UxVardata_dialogContext->Uxlista_edit_on
#define tipo_lista              UxVardata_dialogContext->Uxtipo_lista
#define lista                   UxVardata_dialogContext->Uxlista
#define lparam                  UxVardata_dialogContext->Uxlparam
#define modified                UxVardata_dialogContext->Uxmodified
#define tipolista               UxVardata_dialogContext->Uxtipolista

static _UxCvardata_dialog	*UxVardata_dialogContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	popup_vardata();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* is_not_blank 
*/

int is_not_blank(char *str)
{
   int i,len,appo,ret;

   len = strlen(str);

   ret = False;
   
   for(i=0;i<len;i++)
   {
      if(str[i] == ' ')
         appo = False;
      else
         appo = True;
   
      ret |= appo;
   }

   return(ret);
}

/*
 aggiorna_all_lists
	aggiorna la lista dei blocchi selezionati e la lista delle
        variabili in editing in base ai blocchi in selezione 
        nella main window .
*/
extern int tot_descr; /* numero totale di descrizioni inframmezzate all'area
                         dati: viene settato della routine di libreria
                         read_file_f14 */
aggiorna_all_lists()
{
int i;
extern swidget listaBlocchi;
char *sel_items;
int nitems; /* numero di blocchi selezionati nella main window */
int nitems_sel; /* numero di blocchi nella lista di blocchi selezionati */
if(lista_edit_on)
	{
	UxDestroySwidget(sw_top);
	lista_edit_on=False;
	}
nitems=UxGetSelectedItemCount(listaBlocchi);
/* 
 se vi sono blocchi selezionati procede all'aggiornamento delle
 liste
*/
if(nitems)
	{
	sel_items=UxGetSelectedItems(listaBlocchi);
/*
  Aggiorna la lista dei blocchi selezionati
*/
/*
 Se si e' in modalita' replace list cancella la lista attuale dei
 blocchi selezionati 
*/
	nitems_sel=nitems;
	XmListDeleteAllItems(UxGetWidget(listaBlocchiSel));
	for(i=0;i<nitems;i++)
		{
		aggiungi_item_n(listaBlocchiSel,&sel_items[80*i+i],80);
		}
/*
 Aggiorna la lista delle variabili appartenenti ai blocchi
*/

	aggiorna_lista_var(sel_items,nitems,80);
	lista_edit_on=True;
	create_edit(vardata_rc);
	}
}


/*
 create_edit
 creazione grafica della lista per l'editing dei valori delle
 variabili o dei valori dei parametri
*/ 
create_edit(parent)
swidget parent;
{
/*
 creazione di un row_column che conterra' l'intera lista
*/

sw_top =  XtVaCreateManagedWidget ("top_rowcol",
                xmBulletinBoardWidgetClass, parent,
                XmNmarginHeight,0,
		XmNmarginWidth,0,
                XmNmarginTop,0,
                XmNmarginBottom,0,
                NULL);
/*
 Esamina se deve essere visualizzata la lista dei parametri
 o una delle liste relative alle variabili
*/
if(tipo_lista == ALL_DATA)
	create_edit_param();
else
	create_edit_var();
}

/*
 create_edit_var
 creazione lista di editing per le variabili
*/
create_edit_var()
{
RIGA_OPT r_opt;
int	i,j;
int 	tipovar_ok; /* tipo di variabile da accettarsi in
		       visualizzazione in base al tipo
                       di lista richiesta */
int num_selezio;
swidget menuvar_p1_shell;
swidget sw,sw1;
swidget sw_riga;
swidget menu1; /* option menu */
swidget menu1_p1; /* pulldown associato all'option menu */
swidget sw_push[N_TIPI_UMIS]; /* push button dell'option menu */
int altezza; /* altezza di una riga */
int posiz;
/*
 in base al tipo di lista richiesta stabilisce quali sono
 le variabili accettabili
*/	
if(tipo_lista== INP_VAR)
	tipovar_ok=TIPO_INPBLO;
else if (tipo_lista== OUT_VAR)
	tipovar_ok=TIPO_OUTBLO;
/*
 crea le righe componenti l'intera lista
*/
altezza=30;
posiz=0;
for (i=0; i<lista.num_righe; i++)
	{
	if(lista.riga[i].io_blo!=TIPO_DOPPIA)
		{
/*
 se e' stata richiesta una lista specifica effettua la creazione
 della riga solo per le variabili di tipo accettabile
*/
		if(tipo_lista!=ALL_VAR)
			{
		  	if(lista.riga[i].io_blo!=tipovar_ok)
				continue;
			}
/*
 crea un row_column per ogni riga
*/
		
		sw_riga=UxCreateRowColumn("r_rowcol",sw_top);
		UxPutPacking(sw_riga,"pack_tight");
		UxPutOrientation(sw_riga,"horizontal");
		UxPutNumColumns(sw_riga,1);
		UxPutY(sw_riga, posiz*altezza);
		posiz++;
		UxCreateWidget(sw_riga);
/*
 crea il toggle button per la selezione noto/ non noto
*/
		
		sw = UxCreateToggleButton("togg", sw_riga);
		UxPutLabelString(sw,"");
		UxPutSelectColor(sw,"red");
		UxPutIndicatorSize(sw,10);
		UxPutMarginHeight(sw,0);
		UxPutHighlightThickness(sw,1);
		UxPutShadowThickness(sw,0);
		UxPutSpacing(sw,0);
		lista.riga[i].sw_nn=sw;
		if(lista.riga[i].nota)
			UxPutSet(sw,"true");
		else
			UxPutSet(sw,"false");
#ifndef DESIGN_TIME
		UxPutContext(sw, (char *)UxVardata_dialogContext );
#endif

		UxAddCallback(sw,XmNvalueChangedCallback,mod_callback,NULL);
		UxCreateWidget(sw);	
/*
 crea il widget Text per l'input del valore da assegnare alla
 variabile
*/	
		
		sw = UxCreateText("text", sw_riga);
		UxPutHeight(sw,20);
		UxPutMarginHeight(sw,1);
		UxPutColumns(sw,10);
		UxPutMaxLength(sw,10);
		if(lista.riga[i].valore_str)
                   if( is_not_blank( lista.riga[i].valore_str ) )
			UxPutText(sw,lista.riga[i].valore_str);
#ifndef DESIGN_TIME
		UxPutContext(sw, (char *)UxVardata_dialogContext );
#endif

		UxAddCallback(sw,XmNvalueChangedCallback,mod_callback,NULL);
		UxCreateWidget(sw);
		lista.riga[i].sw_val=sw;
/*
 crea l'option menu per la selezione della unita' di misura
*/
/*
 1. Creazione della shell per il pulldown menu
*/
		menuvar_p1_shell = XtVaCreatePopupShell ("menuvar_p1_shell",
                        xmMenuShellWidgetClass, sw_riga,
                        XmNwidth, 1,
                        XmNheight, 1,
                        XmNallowShellResize, TRUE,
                        XmNoverrideRedirect, TRUE,
                        NULL );
/*
 2. Creazione del pulldown menu (menu pane)
*/

		menu1_p1 =  XtVaCreateWidget ("rcb",
        		xmRowColumnWidgetClass, menuvar_p1_shell,
        		XmNmarginHeight,0,
        		XmNrowColumnType, XmMENU_PULLDOWN,
        		NULL);

		
/*
 Creazione dei bottoni degli option menu
*/
		for(j=0;j<lista.riga[i].num_options;j++)
			{
			sw_push[j]=UxCreatePushButton("opt",menu1_p1);

			UxPutLabelString(sw_push[j],lista.riga[i].opzione[j]);
			UxPutRecomputeSize(sw_push[j],"false");
			UxPutWidth(sw_push[j],60);
			UxPutMarginHeight(sw_push[j],0);

			r_opt.c[0]=(char)i;
			r_opt.c[1]=(char)j;
#ifndef DESIGN_TIME
			UxPutContext(sw_push[j], (char *)UxVardata_dialogContext );
#endif

			UxAddCallback(sw_push[j],XmNactivateCallback,opt_callback,r_opt.i);
			UxCreateWidget(sw_push[j]);
			}

		
		
		num_selezio=lista.riga[i].num_opt_sel;
		menu1= XtVaCreateManagedWidget( "menuvar_valori",
                        xmRowColumnWidgetClass, sw_riga,
			XmNmarginHeight,0,
                        XmNsubMenuId, menu1_p1,
                        XmNrowColumnType, XmMENU_OPTION,
			XmNmenuHistory,sw_push[num_selezio],
                        NULL );


		lista.riga[i].sw_opt=menu1;


/*
 crea la label contenente nome e descrizione della variabile
*/			
	
		sw = UxCreateLabelGadget("label", sw_riga);
		UxPutLabelString(sw, lista.riga[i].etichetta);
		UxPutMarginHeight(sw,0);
		UxPutMarginTop(sw,0);
		UxPutMarginBottom(sw,0);
		UxCreateWidget(sw);
		}
				
	}
}

/*
 create_edit_param
 creazione lista di editing parametri
*/
create_edit_param()
{
int i,k;
swidget sw_riga;
swidget sw;
char s[128];
int altezza=30;
/*
 crea le righe componenti l'intera lista
*/
for (i=0; i<lparam.num_righe; i++)
	{
/*
 crea un row_column per ogni riga
*/
	sprintf(s,"r_rowcol%d",i);
	sw_riga=UxCreateRowColumn(s,sw_top);
	UxPutPacking(sw_riga,"pack_tight");
	UxPutOrientation(sw_riga,"horizontal");
	UxPutNumColumns(sw_riga,1);
	UxPutMarginHeight(sw_riga,0);
	UxPutY(sw_riga , altezza*i);
	UxCreateWidget(sw_riga);

	if(lparam.riga[i].tipo_riga==TIPO_RIGA_DESCR)
		{
		sprintf(s, "label%d", i);
		sw = UxCreateLabelGadget(s, sw_riga);
		UxPutLabelString(sw, lparam.riga[i].u.descr_str);
		UxPutMarginHeight(sw,2);
		UxPutMarginTop(sw,2);
		UxPutMarginBottom(sw,2);
		UxPutForeground(sw_riga,"#0000ff");
		UxCreateWidget(sw);
		}
	else if (lparam.riga[i].tipo_riga==TIPO_RIGA_INTESTA)
		{
		sprintf(s, "titolo%d", i);
		sw = UxCreateLabelGadget(s, sw_riga);
		UxPutLabelString(sw, lparam.riga[i].u.descr_str);
		UxPutMarginHeight(sw,2);
		UxPutMarginTop(sw,2);
		UxPutMarginBottom(sw,2);
		UxPutForeground(sw_riga,"#ff0000");
		UxCreateWidget(sw);
		}
	else
		{
		UxPutPacking(sw_riga,"pack_column");
		UxPutEntryAlignment(sw_riga,"alignment_end");
		for(k=0;k<3;k++)
			{
			if(lparam.riga[i].u.par[k].descr_str[0]==0)
				continue;
			sprintf(s, "label%d%d", i,k);
			sw = UxCreateLabelGadget(s, sw_riga);
			UxPutLabelString(sw, lparam.riga[i].u.par[k].descr_str);
			UxPutMarginHeight(sw,0);
			UxPutMarginTop(sw,0);
			UxPutMarginBottom(sw,0);
			UxCreateWidget(sw);
/*
 crea il widget Text per l'input del valore da assegnare al
 parametro
*/	
			sprintf(s, "text%d%d", i,k);
			sw = UxCreateText(s, sw_riga);
			UxPutHeight(sw,15);
			UxPutMaxLength(sw,10);
			UxPutColumns(sw,10);
			UxPutMarginHeight(sw,0);
			UxPutMarginTop(sw,0);
			UxPutMarginBottom(sw,0);

			if(lparam.riga[i].u.par[k].valore_str)
                           if( is_not_blank( lparam.riga[i].u.par[k].valore_str) )
				UxPutText(sw,lparam.riga[i].u.par[k].valore_str);
#ifndef DESIGN_TIME
			UxPutContext(sw, (char *)UxVardata_dialogContext );
#endif

			UxAddCallback(sw,XmNvalueChangedCallback,mod_callback,NULL);

			UxCreateWidget(sw);
			lparam.riga[i].u.par[k].sw_val=sw;
			}
		}
	}		
}

/*
 recreate_edit()
	distrugge e ricrea la lista di editing
*/
recreate_edit()
{
/*
 cancella la lista attuale
*/
XtDestroyWidget(sw_top);
/*
 crea la nuova lista 
*/
create_edit(vardata_rc);
}


/*
aggiorna_lista_var
	in base ai blocchi selezionati visualizza le informazioni relative
	alle variabili
*/
aggiorna_lista_var(blocchi,nblocchi,lun_nome)
char *blocchi;
int nblocchi;
int lun_nome;
{
int i;
int j,k;
int ind_blo;
int ind_descr; /* indice nell'array delle descrizioni
                  dei parametri del blocco */
int ind_riga_dato;  /* indice nell'array dei parametri */
int num_descr;
int cont;
int inizio_ingressi;
int num_riga,tot_righe;
int num_riga_param,tot_righe_param;
int *array_blo; /* contiene gli indici di tutti i blocchi */

num_riga=0;
num_riga_param=0;
tot_righe=0;
tot_righe_param=0;
/*
 se necessario libera la memoria allocata per la struttura
 lista di editing
*/
free_righe_edit();
/*
 esamina ogni blocco selezionato
*/
array_blo= (int *)XtMalloc(nblocchi*sizeof(int));
if(array_blo==NULL)
	{
	printf("\n malloc non riuscita ");
	return(0);
	}
/*
 calcola il numero totale di righe necessario per l'editing
 delle variabili (tot_righe) ed il numero totale di righe 
 necessario per l'editing dei parametri
*/
for(i=0;i<nblocchi;i++)
	{
/*
 ricerca in base al nome la posizione del blocco nella lista dei
 blocchi del modello
*/
	array_blo[i]=cerca_blocco(&blocchi[i*lun_nome+i],lun_nome);
	if(array_blo[i]==-1) 
		{
		printf("\n blocco non trovato");
		free(array_blo);
		return(0);
		}
	tot_righe+=(ip[array_blo[i]+1]-ip[array_blo[i]]);
/*
 calcola il numero di righe per la lista dei parametri
*/
	tot_righe_param+=(header_dati[array_blo[i]].num_righe_dati +
                          header_dati[array_blo[i]].num_descr+1);
	}
/*
  alloca lo spazio necessario per la struttura delle righe da
  editare
*/
alloca_righe_edit(tot_righe,tot_righe_param);

for(i=0;i<nblocchi;i++)
	{
	ind_blo=array_blo[i];
/*
 individua la posizione nelle liste delle variabili delle descrizioni
 delle variabili stesse
*/
	inizio_ingressi=nusta[ind_blo]+nusci[ind_blo];
	cont=0;
	for(j=ip[ind_blo]-1;j<(ip[ind_blo+1]-1);j++)
		{
/*
 riempie la riga di editing in base alle informazioni relative
 alla variabile
*/
		if(cont >= inizio_ingressi)
			fill_riga_edi(j,num_riga,TIPO_INPBLO);
		else
			fill_riga_edi(j,num_riga,TIPO_OUTBLO);
		cont++;  
		num_riga++;
		}
/*
 riempie le righe per l'editing dei parametri
*/
	ind_descr=header_dati[ind_blo].inizio_descr;
	ind_riga_dato=header_dati[ind_blo].inizio;
	num_descr=header_dati[ind_blo].num_descr;
/*
 copia il titolo relativo al blocco
*/
	strcpy(lparam.riga[num_riga_param].u.descr_str,
                &header_dati[ind_blo].titolo[strlen(HEADER_BLOCCO_F14)]);	  
	lparam.riga[num_riga_param].tipo_riga=TIPO_RIGA_INTESTA;
	num_riga_param++;

	for(j=0;j<header_dati[ind_blo].num_righe_dati;j++)
		{
		if(num_descr)
		   while(array_descr[ind_descr].dato_seguente==ind_riga_dato &&
                        ind_descr < tot_descr )
                	{
			lparam.riga[num_riga_param].tipo_riga=TIPO_RIGA_DESCR;
			strcpy(lparam.riga[num_riga_param].u.descr_str,
				&array_descr[ind_descr].descr[strlen(DESCRIZIONE_IN_DATI)]);
                        ind_descr++;
			num_riga_param++;
                        }
                
                for(k=0;k<array_dati[ind_riga_dato].num_dati;k++)
			{
			sprintf(lparam.riga[num_riga_param].u.par[k].descr_str,
				"%s",
				array_dati[ind_riga_dato].dato[k].descr);
			str_cut(lparam.riga[num_riga_param].u.par[k].descr_str);
			strncpy(lparam.riga[num_riga_param].u.par[k].valore_str,
				  array_dati[ind_riga_dato].dato[k].val,10);
			lparam.riga[num_riga_param].u.par[k].valore_str[10]=0;
			app_n_blank(lparam.riga[num_riga_param].u.par[k].valore_str,10);
			lparam.riga[num_riga_param].u.par[k].punt=ind_riga_dato;
                        }
                if(array_dati[ind_riga_dato].num_dati)
			{
			lparam.riga[num_riga_param].tipo_riga=TIPO_RIGA_PARAM;
			num_riga_param++;
                        ind_riga_dato++;
			}
		}
/*
 potrebbero esserci ancora delle descrizioni da riportare
*/
        while(ind_descr < num_descr)
                {
                lparam.riga[num_riga_param].tipo_riga=TIPO_RIGA_DESCR;
                strcpy(lparam.riga[num_riga_param].u.descr_str,
                &array_descr[ind_descr].descr[strlen(DESCRIZIONE_IN_DATI)]);
                ind_descr++;
                num_riga_param++;
                }
	}
/*
 esamina le righe di editing per vedere se vi sono righe
 doppie; in tal caso modifica il tipo delle righe duplicate
 in TIPO_DOPPIO
*/
filtra_righe_edit();
free(array_blo);
}

/*
 alloca_righe_edit
*/
alloca_righe_edit(num_righe,num_righe_param)
int num_righe;
int num_righe_param;
{
lista.last_selected= -1;
lista.num_righe=num_righe;
#ifdef ORIGINALE
lista.riga=(RIGA_EDI *)calloc(lista.num_righe,sizeof(RIGA_EDI));
#else
memset(lista.riga,0,lista.num_righe*sizeof(RIGA_EDI));
#endif
lparam.last_selected= -1;
lparam.num_righe=num_righe_param;
#ifdef ORIGINALE
lparam.riga=(RIGA_PARAM *)calloc(lparam.num_righe,sizeof(RIGA_PARAM));
#else
memset(lparam.riga,0,lparam.num_righe*sizeof(RIGA_PARAM));
#endif
}

/*
free_righe_edit
*/
free_righe_edit()
{

#ifdef ORIGINALE
if(lista.riga)
	free(lista.riga);
lista.riga=NULL;
#endif
lista.num_righe=0;

#ifdef ORIGINALE
if(lparam.riga)
	free(lparam.riga);
lparam.riga=NULL;
#endif
lparam.num_righe=0;

}

/*
 filtra_righe_edit()
	esamina le righe di editing per vedere se vi sono righe
 	doppie; in tal caso modifica il tipo delle righe duplicate
 	in TIPO_DOPPIO
*/
filtra_righe_edit()
{
int i,j;
for(i=0;i<lista.num_righe;i++)
	{
	for(j=0;j<lista.num_righe;j++)
		{
		if(j==i) continue;
		if(lista.riga[i].io_blo==TIPO_DOPPIA) continue;
		if(lista.riga[i].tipo==lista.riga[j].tipo &&
		   lista.riga[i].punt==lista.riga[j].punt)
			{
/* 
 le due righe sono uguali viene eliminata con precedenza la riga 
 descrivente un ingresso.
*/
			if(lista.riga[j].io_blo==TIPO_INPBLO &&
                           lista.riga[i].io_blo==TIPO_OUTBLO) 
				{
				lista.riga[j].io_blo=TIPO_DOPPIA;
				}
			else if(lista.riga[j].io_blo==TIPO_OUTBLO &&
                                lista.riga[i].io_blo==TIPO_INPBLO) 
				{
				lista.riga[i].io_blo=TIPO_DOPPIA;
				}
			}
		}
	}
}
                           
		
/*
 cerca_blocco
*/
int cerca_blocco(nome,lun_nome)
char *nome;
int lun_nome;
{
int i;
for(i=0;i<nbl;i++)
	{
	if(memcmp(nome,nom_bloc[i],lun_nome)==0)
		return(i);
	}
return(-1);
}

/*
 fill_riga_edi
*/
fill_riga_edi(pos_var,num_riga,io_blo)
int pos_var;
int num_riga;
int io_blo;  /* indica se in ingresso o uscita da un blocco */
{
int j,i;
int ind_var; /* indice della variabile nei vettori nomsivar,nomvari, 
                valinp e valout */
VAL_VAR *p_var;
if(ipvrs[pos_var]>0)
	{
	ind_var=ipvrs[pos_var]-1;
	sprintf(lista.riga[num_riga].etichetta,"%.100s",nom_sivar[ind_var]);
	p_var= &valout[ind_var];
	lista.riga[num_riga].tipo=TIPO_OUT;
	}
else
	{
	ind_var= -(ipvrs[pos_var])-1;
	sprintf(lista.riga[num_riga].etichetta,"%.100s",nom_vari[ind_var]);
	p_var= &valinp[ind_var];
	lista.riga[num_riga].tipo=TIPO_INP;
	}
lista.riga[num_riga].io_blo=io_blo;
lista.riga[num_riga].punt=ind_var;
spr_float(lista.riga[num_riga].valore_str,p_var->val);
lista.riga[num_riga].num_options=0;
for(i=0;i<N_TIPI_UMIS;i++)
	{
	if(strcmp(uni_mis[p_var->ind_umis].codm[i],"      "))
		{
		strcpy(lista.riga[num_riga].opzione[i],
			uni_mis[p_var->ind_umis].codm[i]);
		lista.riga[num_riga].num_options++;
		}
	}
if(strcmp(uni_mis[p_var->ind_umis].codm[0],"---")==0)
	lista.riga[num_riga].num_options=1;

lista.riga[num_riga].num_opt_sel=p_var->sel_umis;
lista.riga[num_riga].nota=p_var->noto;
}

/* verifica dato
   controlla che l'input il dato introdotto sia compreso nei caratteri printabili
*/

int verifica_dato(char *str)
{
  int i,ok;

   ok = 1;
   for(i=0;i<strlen(str);i++)
      if(!isprint((int)str[i]))
      {
        ok = 0;
        break;
      }

   return(ok);
}


/*
 salva_variazioni
	salva le variazioni effettuate nella lista di editing
*/
salva_variazioni()
{
int i,k;
int ind_riga_dato; /* posizione nell'array dei dati */
int tipovar_ok;
VAL_VAR *p_var;
Widget w_button;
char *str;
float save_val;
int iret;
char appoggio[200];
int app;
extern swidget create_vis_msg();
/*
 E' in editing la lista relativa ai parametri
*/
if(tipo_lista==ALL_DATA)
	{
	for(i=0;i<lparam.num_righe;i++)
		{
		if(lparam.riga[i].tipo_riga==
			TIPO_RIGA_PARAM)
			{
			ind_riga_dato=lparam.riga[i].u.par[0].punt;
			for(k=0;k< array_dati[ind_riga_dato].num_dati;k++)
				{
				str=UxGetText(lparam.riga[i].u.par[k].sw_val);
                                strcpy(appoggio,str);
				app_n_blank(appoggio,10);
				appoggio[10]=0;
                                if( verifica_dato(appoggio) )
                                   {
				   strcpy(array_dati[ind_riga_dato].dato[k].val,
                                        appoggio);	
				   strcpy(lparam.riga[i].u.par[k].valore_str,appoggio);
                                   }
                                else
          		           {
		                   sprintf(appoggio,
			                  "Invalid input field in line %d data %8.8s.\n It was substituted with previous value",
        		                   i+1,lparam.riga[i].u.par[k].descr_str);
		                   create_vis_msg(appoggio);
		           
		                   UxPutText(lista.riga[i].sw_val,lparam.riga[i].u.par[k].valore_str);
		                   }
				}
			}
		}
	}
/*
 E' in editing una lista relativa alle variabili
*/
else 
{
for(i=0;i<lista.num_righe;i++)
	{
	if(tipo_lista== INP_VAR)
		tipovar_ok=TIPO_INPBLO;
	else if (tipo_lista== OUT_VAR)
		tipovar_ok=TIPO_OUTBLO;

	if(lista.riga[i].io_blo==TIPO_DOPPIA) continue;
	if(tipo_lista!=ALL_VAR)
			{
		  	if(lista.riga[i].io_blo!=tipovar_ok)
				continue;
			}
	if(lista.riga[i].tipo==TIPO_OUT)
		p_var= &valout[lista.riga[i].punt];
	else
		p_var= &valinp[lista.riga[i].punt];
/*
 Ricava il valore della variabile nell'unita' di misura
 correntemente selezionata
*/
	str=UxGetText(lista.riga[i].sw_val);
	save_val=p_var->val;
	iret=sscanf(str,"%f%s",&(p_var->val),appoggio);
	if(iret!=1)
		{
		sprintf(appoggio,
			"Invalid input field in line %d variable %8.8s.\n It was substituted with previous value",
        		i+1,lista.riga[i].etichetta);
		create_vis_msg(appoggio);
		p_var->val=save_val;
		UxPutText(lista.riga[i].sw_val,lista.riga[i].valore_str);
		}
	else
		strcpy(lista.riga[i].valore_str,str);
	str=UxGetSet(lista.riga[i].sw_nn);
/*
 ritorna 'true' o 'false' in forma di stringhe; quindi e' sufficiente
 testare la prima lettera.
*/
	if(str[0]=='t' && p_var->noto==0)
		{
		lista.riga[i].nota=1;
		p_var->noto=1;
		num_note++;
		}
	else if (str[0]=='f' && p_var->noto==1)
		{
		lista.riga[i].nota=0;
		p_var->noto=0;
		num_note--;
		}
	p_var->sel_umis=lista.riga[i].num_opt_sel; 
	}
}
}

void mod_callback(w,u_data,data)
Widget w;
caddr_t u_data;
XmAnyCallbackStruct *data;
{
#ifndef DESIGN_TIME
/*
 Salvataggio e recupero del Context relativo alla window per la quale e'
 stata richiamata la funzione di callback
*/
_UxCvardata_dialog      *UxSaveCtx, *UxContext;
swidget                 UxThisWidget;

UxThisWidget = UxWidgetToSwidget( w );
UxSaveCtx = UxVardata_dialogContext;
UxVardata_dialogContext = UxContext =
                        (_UxCvardata_dialog *) UxGetContext( UxThisWidget );
#endif
modified=True;
#ifndef DESIGN_TIME
/*
 Ripristino del Context precedente alla chiamata della presente
 callback function
*/
UxVardata_dialogContext = UxSaveCtx;
#endif
}


void opt_callback(w,opt,data)
Widget w;
RIGA_OPT opt;
XmAnyCallbackStruct *data;
{
float value,value_def;
char *str;
char app[20];
int umis_sel,umis_sel_old,num_riga,ind_umis;
VAL_VAR *p_var; /* puntatore alla variabile corrente */
#ifndef DESIGN_TIME
/*
 Salvataggio e recupero del Context relativo alla window per la quale e'
 stata richiamata la funzione di callback
*/
_UxCvardata_dialog      *UxSaveCtx, *UxContext;
swidget                 UxThisWidget;

UxThisWidget = UxWidgetToSwidget( w );
UxSaveCtx = UxVardata_dialogContext;
UxVardata_dialogContext = UxContext =
                        (_UxCvardata_dialog *) UxGetContext( UxThisWidget );
#endif
/*
 se e' cambiata l'unita' di misura selezionata
*/
num_riga=opt.c[0];
umis_sel=opt.c[1];

if(umis_sel!=lista.riga[num_riga].num_opt_sel)
	{
	if(lista.riga[num_riga].tipo==TIPO_OUT)
		p_var= &valout[lista.riga[num_riga].punt];
	else
		p_var= &valinp[lista.riga[num_riga].punt];
	umis_sel_old=lista.riga[num_riga].num_opt_sel;
	lista.riga[num_riga].num_opt_sel=umis_sel;
	sscanf(UxGetText(lista.riga[num_riga].sw_val),"%f",&value);
/*
 trasforma il valore nell'unita' di misura di default
*/
	ind_umis=p_var->ind_umis;
	value_def=(value-uni_mis[ind_umis].B[umis_sel_old])
                  /uni_mis[ind_umis].A[umis_sel_old];
/*
 trasforma il valore nella nuova unita' di misura
*/
	value=value_def*uni_mis[ind_umis].A[umis_sel]+
		uni_mis[ind_umis].B[umis_sel];
	spr_float(app,value);	
	UxPutText(lista.riga[num_riga].sw_val,app);
	}
#ifndef DESIGN_TIME
/*
 Ripristino del Context precedente alla chiamata della presente
 callback function
*/
UxVardata_dialogContext = UxSaveCtx;
#endif
}
/*
 scrolla_lista 
	effettua lo scrolling della lista di editing a seguito di una
 	operazione di find
*/
scrolla_lista(pos,poslista)
int pos;
int poslista;
{
Widget w_bar;
int maximum;
int pos_slider;
int dim_slider;
int free_slider; /* corsa libera per lo slider */
float delta; /* spostamento da dare allo slider corrispondente
              alle righe da scrollare */
XEvent event;
XmScrollBarCallbackStruct call_data;
call_data.reason=XmCR_VALUE_CHANGED;
call_data.event=&event;


if(lista_edit_on)
	{
/*
	elimina l'evidenziazione sulla riga precedentemente 
	selezionata
*/
	elim_selez_lista();

/*
 Ricava l'indice di widget della scroll bar verticale
*/
	get_something(UxGetWidget(vardata_sw),XmNverticalScrollBar,(void*) &w_bar);
/*
 Ricava dimensione massima e dimensione attuale dello slider
 della scroll bar per determinare la corsa libera dello slider
*/
	get_something(w_bar,XmNmaximum,(void*) &maximum);
	get_something(w_bar,XmNsliderSize,(void*) &dim_slider);
	free_slider=maximum-dim_slider;
/*
 determina la frazione rispetto al totale di cui si deve spostare
 lo slider
*/
	if (tipo_lista == ALL_DATA)
		delta=((float)pos/(float)lparam.num_righe); /* percentuale di incremento */
	else
		delta=((float)pos/(float)lista.num_righe);
/*
 determina lo spostamento da richiedere allo slider
*/
	pos_slider=(delta*maximum);
	if(pos_slider>free_slider)
		pos_slider=free_slider;
/*
 sposta lo slider seleziona la riga a cui ci si deve posizionare ed
 esegue la callback relativa allo spostamento effettuato dallo 
 slider
*/ 
	set_something_val(w_bar,XmNvalue, (XtArgVal) pos_slider); 
	call_data.value=pos_slider;
	set_selez_lista(poslista);

	XtCallCallbacks(w_bar,XmNvalueChangedCallback,&call_data);

	}
}

elim_selez_lista()
{
int poslista;
if(tipo_lista==ALL_DATA)
	{
	if(lparam.last_selected!=-1)
		{
		poslista=lparam.last_selected;
		if(lparam.riga[poslista].tipo_riga==TIPO_RIGA_PARAM)
			{
			XmTextSetHighlight(UxGetWidget(lparam.riga[poslista].u.par[0].sw_val),
				0,strlen(lparam.riga[poslista].u.par[0].valore_str),
				XmHIGHLIGHT_NORMAL);
			}
		}  	
	}
else
	{
	if(lista.last_selected!=-1)
		{
		poslista=lista.last_selected;
		XmTextSetHighlight(UxGetWidget(lista.riga[poslista].sw_val),
			0,strlen(lista.riga[poslista].valore_str),XmHIGHLIGHT_NORMAL);
		}
	}
}

set_selez_lista(poslista)
int poslista;
{
if(tipo_lista==ALL_DATA)
	{
	if(lparam.riga[poslista].tipo_riga==TIPO_RIGA_PARAM)
		{
		XmTextSetHighlight(UxGetWidget(lparam.riga[poslista].u.par[0].sw_val),
				0,strlen(lparam.riga[poslista].u.par[0].valore_str),
				XmHIGHLIGHT_SELECTED);
		}
	lparam.last_selected=poslista;
	}
else
	{
	XmTextSetHighlight(UxGetWidget(lista.riga[poslista].sw_val),
			0,strlen(lista.riga[poslista].valore_str),
			XmHIGHLIGHT_SELECTED);
	lista.last_selected=poslista;
	}
}



cerca_in_lista(str,direz,posvis,poslist)
char *str;
int direz;
int *posvis;
int *poslist;
{
int posriga;
int iret;
if(tipo_lista==ALL_DATA)
	{
	iret=cerca_in_lparam(str,direz,&posriga,poslist);
	*posvis=*poslist;
	}
else
	iret=cerca_in_lista_edi(str,direz,posvis,poslist);
return(iret);
}



cerca_in_lista_edi(str,direz,posvis,poslist)
char *str;
int direz;
int *posvis;
int *poslist;
{
int i;
int start,inc;
int trovato;
int tipovar_ok;

if(tipo_lista== INP_VAR)
	tipovar_ok=TIPO_INPBLO;
else if (tipo_lista== OUT_VAR)
	tipovar_ok=TIPO_OUTBLO;

if(lista.last_selected!=-1)
	start=lista.last_selected;
else 
	start=0;
trovato=0;
inc=direz;
i=next_pos_lista(start,inc);

while(!trovato && i!= start)
	{
	if((strstr(lista.riga[i].etichetta,str) ||
	   strstr(lista.riga[i].valore_str,str)) &&
		lista.riga[i].io_blo!=TIPO_DOPPIA &&
	   (lista.riga[i].io_blo==tipovar_ok || tipo_lista == ALL_VAR))
		trovato=1;
	else
		{
		i=next_pos_lista(i,inc);
		}
	}
if(i==start && !trovato) 
	{
	*poslist= -1;
	*posvis= -1;
	return(-1);
	}
else
	{
	*poslist=i;
	*posvis=plista_to_pvis(i);
	return(*posvis);
	}
}


/*
 cerca_in_lparam
 	ricerca nella lista dei parametri
*/
cerca_in_lparam(str,direz,posriga,poslist)
char *str;
int direz;
int *poslist;
int *posriga; /* posizione all'interno della riga */
{
int start,i,j;
int trovato;
if(lparam.last_selected!=-1)
	start=lparam.last_selected;
else 
	start=0;
trovato=0;
i=next_pos_lista(start,direz);

while(!trovato && i!= start)
	{
	if(lparam.riga[i].tipo_riga!=TIPO_RIGA_PARAM)
		{
		if(strstr(lparam.riga[i].u.descr_str,str))
			{
			trovato=1;
			*poslist=i;
			*posriga= -1; /* indica che si e' trovata una riga di commento */
			return(*poslist);
			}
		else
			{
			
			i=next_pos_lista(i,direz);
			}
		}
	else
		{
		for(j=0;j<3;j++)
			{
			if(lparam.riga[i].u.par[j].descr_str[0]!=0 &&
			   (strstr(lparam.riga[i].u.par[j].valore_str,
				  str) ||
			    strstr(lparam.riga[i].u.par[j].descr_str,
				  str)))
				{
				trovato=1;
				*poslist=i;
				*posriga=j;
				return(i);
				}
			}
		i=next_pos_lista(i,direz);
		}
	}			
if(i==start && !trovato) 
	{
	*poslist= -1;
	*posriga= -1;
	return(-1);
	}
}



next_pos_lista(inizio,direz)
int inizio;  /* posizione di partenza */
int direz;  /* direzione dello spostamento
		==1 -> in avanti */
{
int i;
int num_righe;
if(tipo_lista==ALL_DATA)
	num_righe=lparam.num_righe;
else
	num_righe=lista.num_righe;
if(direz==1)
	{
	if(inizio < (num_righe-1))	
		i=inizio+1;	
	else
		i=0;
	}
else
	{
	if(inizio> 0)
		i=inizio-1;
	else
		i=num_righe-1;
	}
return(i);
}


/*
 plista_to_pvis
   trasforma da posizione nella lista a posizione nella
   edit window scrollabile tenendo conto del tipo di 
   variabili visualizzate
*/
plista_to_pvis(poslista)
int poslista;
{
int i;
int posvis; /* posizione in visualizzazione */
int tipovar_ok;
posvis=0;

if(tipo_lista== INP_VAR)
	tipovar_ok=TIPO_INPBLO;
else if (tipo_lista== OUT_VAR)
	tipovar_ok=TIPO_OUTBLO;

for(i=0;i<poslista;i++)
	{
	if(lista.riga[i].io_blo==TIPO_DOPPIA) continue;
	if(tipo_lista==ALL_VAR)
		posvis++;	
	else if(lista.riga[i].io_blo==tipovar_ok) posvis++;
	}
return(posvis);
}

void confirm_apply (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
/*
 L'utente ha richiesto di continuare
 senza salvare (Continue)
*/
if (which_button == 0)
	{	
        aggiorna_all_lists();
	modified=False;
	}

/* Chiudi la window di conferma */
XtDestroyWidget(w);

}

void confirm_close (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
/*
 L'utente ha richiesto di continuare perdendo
 i dati (Continue)
*/
if (which_button == 0)
	{
	modified=False;
	free_righe_edit();
	UxDestroyInterface(vardata_dialog);
	n_editors--;
	}

/* Chiudi la window di conferma */
XtDestroyWidget(w);

}

void confirm_recreate (w,which_button,call_data)
Widget w;
int which_button;
XmAnyCallbackStruct *call_data;
{
Widget sw_sel; 
/* Chiudi la window di conferma */
XtDestroyWidget(w);
XSync((XtDisplay(w)),False);
/*
 Se Continue l'utente prosegue 
 altrimenti (Cancel) ritorna alla 
 condizione di partenza
*/
if (which_button != 100)
	{
	tipo_lista=which_button;
	recreate_edit();
	modified=False;
	}
else
/*
 Riposiziona l'option menu sulla 
 opzione di partenza
*/
	{
	switch(tipo_lista)
		{
		case ALL_VAR:
		sw_sel=valori_Variables;
		break;
		case INP_VAR:
		sw_sel=valori_Inp_Var;
		break;
		case OUT_VAR:
		sw_sel=valori_Out_Var;
                break;
		case ALL_DATA:
		sw_sel=valori_Data;
		break;
		}
	set_something_val(menu_valori,
                     XmNmenuHistory, (XtArgVal) sw_sel);
	}

}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_apply_button( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	/*
	 estrae dalla window di editing le
	 variazioni effettuate e le salva
	*/
	salva_variazioni();
	/*
	aggiorna i contatori di var note/non note
	*/
	update_contatori();
	modified=False;
	glob_modified=True;
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_arrowButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	char *str;
	char str2[50];
	int posiz,poslista;
	extern swidget create_vis_msg();
	str=UxGetText(find_text);
	
	posiz=cerca_in_lista(str,-1,&posiz,&poslista);
	if(posiz!=-1)
		scrolla_lista(posiz,poslista);
	else
		{
		sprintf(str2,"Cannot find: %s",str);
		create_vis_msg(str2); 
		}
	
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_arrowButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	char *str;
	char str2[50];
	int posiz,poslista;
	extern swidget create_vis_msg();
	str=UxGetText(find_text);
	
	posiz=cerca_in_lista(str,1,&posiz,&poslista);
	if(posiz!=-1)
		scrolla_lista(posiz,poslista);
	else
		{
		sprintf(str2,"Cannot find: %s",str);
		create_vis_msg(str2); 
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_edit_Load( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	extern Widget conferma();
	extern Widget UxTopLevel;
	static Elenco_callback data_agg_callback = {
	        {"Continue",  confirm_apply, 0 },
	        {"Cancel", confirm_apply, 1 },
	        {NULL,NULL, 0 }};
	
	if(modified)
		{
		conferma(vardata_dialog,"You have modified but not applied Variables values.\n All changes WILL BE LOST !!\n Continue?",
			data_agg_callback);
		}
	else
		aggiorna_all_lists(); 
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_edit_Close( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	
	extern Widget conferma();
	
	static Elenco_callback data_close_callback = {
	        {"Continue",  confirm_close, 0 },
	        {"Cancel", confirm_close, 1 },
	        {NULL,NULL, 0 }};
	
	if(modified)
	        {
	
	
	        conferma(vardata_dialog,"You have modified but not applied Variables values.\nAll changes WILL BE LOST!!\nContinue?",
	                data_close_callback);
	
	        }
	else
		{
		free_righe_edit();
		UxDestroyInterface(vardata_dialog);
		n_editors--;
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_edit_Restore( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	recreate_edit();
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_BDoc( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	   extern swidget listaBlocchi;
	   extern swidget create_vis_msg();
	   int num_selez;
	   int *item_positions;
	   extern char **nom_bloc;
	   int i,retval;
	   char **nome_mod;
	   char comando[100];
	   char doc_name[100];
	   char msg[100];
	    
	   retval=XmListGetSelectedPos(listaBlocchi,&item_positions,&num_selez);
	   if( retval == False )
	      num_selez = 0;
	
	
	   if( num_selez )
	   {
	      nome_mod = malloc( sizeof(char *) * num_selez );
	      if( nome_mod == NULL)
	      {
	         sprintf(msg,"%s\n","Doc Read: Error in malloc");
	         create_vis_msg(msg);
	      }
	
	      for(i=0;i<num_selez;i++)      
	      {
	         nome_mod[i]=malloc( sizeof(char) *5 );
	         printf("nome_blocco selezionato = %s\n",nom_bloc[i]);
	         sprintf(nome_mod[i],"%.4s",nom_bloc[ (item_positions[i]-1) ]);
	         tominus(nome_mod[i]);
	         printf("nome_modulo selezionato = %s\n",nome_mod[i]);
	      }
	      
	
	
	/* visualizzo solo il documento relativo al
	   primo modulo selezionato
	*/
	      sprintf(doc_name,"%s/legocad/libut/%s.doc",getenv("LEGOCAD_USER"),nome_mod[0]);
	 
	      if( esiste_file( doc_name ) )
	      {
	         sprintf(comando,"asedit -v %s &",doc_name);      
	         system(comando);
	      } 
	      else
	      {
	         sprintf(msg,"%s file not found\n",doc_name);
	         create_vis_msg(msg);
	      }
	   }
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_valori_Variables( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	extern Widget conferma();
	static Elenco_callback data_agg_callback = {
	        {"Continue",  confirm_recreate, ALL_VAR },
	        {"Cancel", confirm_recreate, 100 },
	        {NULL,NULL, 0 }};
	
	tipo_lista=ALL_VAR;
	if(lista_edit_on)
		{
		if(modified)
	        	{
	        	conferma(vardata_dialog,
	                 "You have modified but not applied Variables values.\nAll changes WILL BE LOST!!\nContinue?",
	                 data_agg_callback);
	        	}
		else
	/*
	 ricrea la nuova lista in base alla selezione effettuata
	*/
			{
			tipo_lista=ALL_VAR;
			recreate_edit();
			}
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_valori_Inp_Var( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	extern Widget conferma();
	static Elenco_callback data_agg_callback = {
	        {"Continue",  confirm_recreate, INP_VAR },
	        {"Cancel", confirm_recreate, 100 },
	        {NULL,NULL, 0 }};
	
	
	if(lista_edit_on)
		{
		if(modified)
	        	{
	        	conferma(vardata_dialog,
	                 "You have modified but not applied Variables values.\n All changes WILL BE LOST!!\nContinue?",
	                 data_agg_callback);
	        	}
		else
	/*
	 ricrea la nuova lista in base alla selezione effettuata
	*/
			{
			tipo_lista=INP_VAR;
			recreate_edit();
			}
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_valori_Out_Var( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	extern Widget conferma();
	static Elenco_callback data_agg_callback = {
	        {"Continue",  confirm_recreate, OUT_VAR },
	        {"Cancel", confirm_recreate, 100 },
	        {NULL,NULL, 0 }};
	
	if(lista_edit_on)
		{
		if(modified)
	        	{
	        	conferma(vardata_dialog,
	                 "You have modified but not applied Variables values.\nAll changes WILL BE LOST !!\nContinue?",
	                 data_agg_callback);
	        	}
		else
	/*
	 ricrea la nuova lista in base alla selezione effettuata
	*/
			{
			tipo_lista=OUT_VAR;
			recreate_edit();
			}
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

static void	activateCB_valori_Data( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCvardata_dialog      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxVardata_dialogContext;
	UxVardata_dialogContext = UxContext =
			(_UxCvardata_dialog *) UxGetContext( UxWidget );
	{
	extern Widget conferma();
	static Elenco_callback data_agg_callback = {
	        {"Continue",  confirm_recreate, ALL_DATA },
	        {"Cancel", confirm_recreate, 100 },
	        {NULL,NULL, 0 }};
	
	if(lista_edit_on)
		{
		if(modified)
	        	{
	        	conferma(vardata_dialog,
	                 "You have modified but not applied Variables values.\nAll changes WILL BE LOST !!\nContinue?",
	                 data_agg_callback);
	        	}
		else
	/*
	 ricrea la nuova lista in base alla selezione effettuata
	*/
			{
			tipo_lista=ALL_DATA;
			recreate_edit();
			}
		}
	}
	UxVardata_dialogContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_vardata_dialog()
{
	Widget	vardata_dialog_shell;
	Widget	edit_File_shell;
	Widget	edit_Edit_shell;
	Widget	Doc_shell;
	Widget	valori_p1_shell;

	vardata_dialog_shell = XtVaCreatePopupShell( "vardata_dialog_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 184,
			XmNy, 116,
			XmNwidth, 635,
			XmNheight, 566,
			XmNallowShellResize, FALSE,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "vardata_dialog",
			NULL );

	vardata_dialog = XtVaCreateWidget( "vardata_dialog",
			xmFormWidgetClass, vardata_dialog_shell,
			XmNrubberPositioning, FALSE,
			XmNautoUnmanage, FALSE,
			RES_CONVERT( XmNdialogTitle, "Variables values & data" ),
			XmNresizePolicy, XmRESIZE_GROW,
			XmNheight, 566,
			XmNwidth, 635,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( vardata_dialog, (char *) UxVardata_dialogContext );

	apply_button = XtVaCreateManagedWidget( "apply_button",
			xmPushButtonGadgetClass, vardata_dialog,
			XmNbottomOffset, 10,
			XmNleftPosition, 40,
			XmNrightPosition, 60,
			XmNresizable, FALSE,
			XmNancestorSensitive, TRUE,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopWidget, NULL,
			XmNtopAttachment, XmATTACH_NONE,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNrecomputeSize, TRUE,
			RES_CONVERT( XmNlabelString, "       Apply      " ),
			XmNheight, 30,
			XmNwidth, 140,
			XmNy, 0,
			XmNx, 296,
			NULL );

	UxPutContext( apply_button, (char *) UxVardata_dialogContext );

	find_rc = XtVaCreateManagedWidget( "find_rc",
			xmRowColumnWidgetClass, vardata_dialog,
			XmNbottomOffset, 10,
			XmNbottomWidget, apply_button,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNborderWidth, 1,
			XmNorientation, XmHORIZONTAL,
			XmNheight, 40,
			XmNwidth, 310,
			XmNy, 368,
			XmNx, 8,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( find_rc, (char *) UxVardata_dialogContext );

	labelGadget4 = XtVaCreateManagedWidget( "labelGadget4",
			xmLabelGadgetClass, find_rc,
			RES_CONVERT( XmNlabelString, "Find:" ),
			XmNheight, 20,
			XmNwidth, 70,
			XmNy, 385,
			XmNx, 15,
			NULL );

	UxPutContext( labelGadget4, (char *) UxVardata_dialogContext );

	find_text = XtVaCreateManagedWidget( "find_text",
			xmTextFieldWidgetClass, find_rc,
			XmNresizeWidth, FALSE,
			XmNmarginWidth, 3,
			XmNmarginHeight, 1,
			XmNheight, 26,
			XmNwidth, 176,
			XmNy, 4,
			XmNx, 40,
			NULL );

	UxPutContext( find_text, (char *) UxVardata_dialogContext );

	arrowButton2 = XtVaCreateManagedWidget( "arrowButton2",
			xmArrowButtonWidgetClass, find_rc,
			XmNarrowDirection, XmARROW_UP,
			XmNheight, 27,
			XmNwidth, 27,
			XmNy, 4,
			XmNx, 219,
			NULL );

	UxPutContext( arrowButton2, (char *) UxVardata_dialogContext );

	arrowButton1 = XtVaCreateManagedWidget( "arrowButton1",
			xmArrowButtonWidgetClass, find_rc,
			XmNarrowDirection, XmARROW_DOWN,
			XmNheight, 27,
			XmNwidth, 27,
			XmNy, 4,
			XmNx, 253,
			NULL );

	UxPutContext( arrowButton1, (char *) UxVardata_dialogContext );

	edit_pulldown = XtVaCreateManagedWidget( "edit_pulldown",
			xmRowColumnWidgetClass, vardata_dialog,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( edit_pulldown, (char *) UxVardata_dialogContext );

	edit_File_shell = XtVaCreatePopupShell ("edit_File_shell",
			xmMenuShellWidgetClass, edit_pulldown,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	edit_File = XtVaCreateWidget( "edit_File",
			xmRowColumnWidgetClass, edit_File_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( edit_File, (char *) UxVardata_dialogContext );

	edit_Load = XtVaCreateManagedWidget( "edit_Load",
			xmPushButtonWidgetClass, edit_File,
			RES_CONVERT( XmNlabelString, "Load" ),
			NULL );

	UxPutContext( edit_Load, (char *) UxVardata_dialogContext );

	edit_Close = XtVaCreateManagedWidget( "edit_Close",
			xmPushButtonWidgetClass, edit_File,
			RES_CONVERT( XmNlabelString, "Close" ),
			NULL );

	UxPutContext( edit_Close, (char *) UxVardata_dialogContext );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass, edit_pulldown,
			XmNsubMenuId, edit_File,
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxVardata_dialogContext );

	edit_Edit_shell = XtVaCreatePopupShell ("edit_Edit_shell",
			xmMenuShellWidgetClass, edit_pulldown,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	edit_Edit = XtVaCreateWidget( "edit_Edit",
			xmRowColumnWidgetClass, edit_Edit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( edit_Edit, (char *) UxVardata_dialogContext );

	edit_Restore = XtVaCreateManagedWidget( "edit_Restore",
			xmPushButtonWidgetClass, edit_Edit,
			RES_CONVERT( XmNlabelString, "Restore values" ),
			NULL );

	UxPutContext( edit_Restore, (char *) UxVardata_dialogContext );

	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonWidgetClass, edit_pulldown,
			XmNsubMenuId, edit_Edit,
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu1_top_b2, (char *) UxVardata_dialogContext );

	Doc_shell = XtVaCreatePopupShell ("Doc_shell",
			xmMenuShellWidgetClass, edit_pulldown,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Doc = XtVaCreateWidget( "Doc",
			xmRowColumnWidgetClass, Doc_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( Doc, (char *) UxVardata_dialogContext );

	BDoc = XtVaCreateManagedWidget( "BDoc",
			xmPushButtonWidgetClass, Doc,
			RES_CONVERT( XmNlabelString, "Module Doc" ),
			NULL );

	UxPutContext( BDoc, (char *) UxVardata_dialogContext );

	edit_pulldown_top_b1 = XtVaCreateManagedWidget( "edit_pulldown_top_b1",
			xmCascadeButtonWidgetClass, edit_pulldown,
			RES_CONVERT( XmNmnemonic, "D" ),
			XmNsubMenuId, Doc,
			RES_CONVERT( XmNlabelString, "Doc" ),
			NULL );

	UxPutContext( edit_pulldown_top_b1, (char *) UxVardata_dialogContext );

	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass, vardata_dialog,
			XmNheight, 60,
			XmNbottomOffset, -95,
			XmNtopOffset, 5,
			XmNtopWidget, edit_pulldown,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, edit_pulldown,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 36,
			XmNx, 10,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow2, (char *) UxVardata_dialogContext );

	listaBlocchiSel = XtVaCreateManagedWidget( "listaBlocchiSel",
			xmListWidgetClass, scrolledWindow2,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNlistSizePolicy, XmCONSTANT,
			XmNvisibleItemCount, 3,
			XmNheight, 80,
			XmNwidth, 615,
			NULL );

	UxPutContext( listaBlocchiSel, (char *) UxVardata_dialogContext );

	valori_p1_shell = XtVaCreatePopupShell ("valori_p1_shell",
			xmMenuShellWidgetClass, vardata_dialog,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	valori_p1 = XtVaCreateWidget( "valori_p1",
			xmRowColumnWidgetClass, valori_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( valori_p1, (char *) UxVardata_dialogContext );

	valori_Variables = XtVaCreateManagedWidget( "valori_Variables",
			xmPushButtonWidgetClass, valori_p1,
			RES_CONVERT( XmNlabelString, "All Variables" ),
			NULL );

	UxPutContext( valori_Variables, (char *) UxVardata_dialogContext );

	valori_Inp_Var = XtVaCreateManagedWidget( "valori_Inp_Var",
			xmPushButtonWidgetClass, valori_p1,
			RES_CONVERT( XmNlabelString, "Input Variables" ),
			NULL );

	UxPutContext( valori_Inp_Var, (char *) UxVardata_dialogContext );

	valori_Out_Var = XtVaCreateManagedWidget( "valori_Out_Var",
			xmPushButtonWidgetClass, valori_p1,
			RES_CONVERT( XmNlabelString, "Output Variables" ),
			NULL );

	UxPutContext( valori_Out_Var, (char *) UxVardata_dialogContext );

	valori_Data = XtVaCreateManagedWidget( "valori_Data",
			xmPushButtonWidgetClass, valori_p1,
			RES_CONVERT( XmNlabelString, "Data" ),
			NULL );

	UxPutContext( valori_Data, (char *) UxVardata_dialogContext );

	menu_valori = XtVaCreateManagedWidget( "menu_valori",
			xmRowColumnWidgetClass, vardata_dialog,
			XmNy, 100,
			XmNx, 10,
			XmNtopWidget, scrolledWindow2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftPosition, 0,
			XmNleftOffset, 20,
			XmNbottomWidget, NULL,
			XmNbottomAttachment, XmATTACH_NONE,
			RES_CONVERT( XmNlabelString, "Show: " ),
			XmNsubMenuId, valori_p1,
			XmNrowColumnType, XmMENU_OPTION,
			NULL );

	UxPutContext( menu_valori, (char *) UxVardata_dialogContext );

	intesta_rc = XtVaCreateManagedWidget( "intesta_rc",
			xmRowColumnWidgetClass, vardata_dialog,
			XmNtopWidget, menu_valori,
			XmNentryAlignment, XmALIGNMENT_CENTER,
			XmNadjustLast, TRUE,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomWidget, NULL,
			XmNbottomOffset, 0,
			XmNbottomAttachment, XmATTACH_NONE,
			XmNpacking, XmPACK_COLUMN,
			XmNnumColumns, 3,
			XmNrightOffset, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 20,
			XmNwidth, 700,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( intesta_rc, (char *) UxVardata_dialogContext );

	labelGadget1 = XtVaCreateManagedWidget( "labelGadget1",
			xmLabelGadgetClass, intesta_rc,
			RES_CONVERT( XmNlabelString, "Known/Unknown      Value  " ),
			XmNheight, 25,
			XmNwidth, 170,
			XmNy, 7,
			XmNx, 12,
			NULL );

	UxPutContext( labelGadget1, (char *) UxVardata_dialogContext );

	labelGadget2 = XtVaCreateManagedWidget( "labelGadget2",
			xmLabelGadgetClass, intesta_rc,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Units" ),
			XmNheight, 25,
			XmNwidth, 554,
			XmNy, 0,
			XmNx, 200,
			NULL );

	UxPutContext( labelGadget2, (char *) UxVardata_dialogContext );

	labelGadget3 = XtVaCreateManagedWidget( "labelGadget3",
			xmLabelGadgetClass, intesta_rc,
			RES_CONVERT( XmNlabelString, "Variable name - description" ),
			XmNheight, 20,
			XmNwidth, 90,
			XmNy, 10,
			XmNx, 120,
			NULL );

	UxPutContext( labelGadget3, (char *) UxVardata_dialogContext );

	vardata_sw = XtVaCreateManagedWidget( "vardata_sw",
			xmScrolledWindowWidgetClass, vardata_dialog,
			XmNtopWidget, intesta_rc,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, find_rc,
			XmNbottomOffset, 2,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNheight, 320,
			XmNwidth, 720,
			XmNy, 120,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( vardata_sw, (char *) UxVardata_dialogContext );

	vardata_rc = XtVaCreateManagedWidget( "vardata_rc",
			xmRowColumnWidgetClass, vardata_sw,
			XmNadjustMargin, FALSE,
			XmNadjustLast, TRUE,
			XmNisAligned, TRUE,
			XmNpacking, XmPACK_COLUMN,
			XmNorientation, XmHORIZONTAL,
			XmNnumColumns, 1,
			XmNheight, 410,
			XmNwidth, 700,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( vardata_rc, (char *) UxVardata_dialogContext );

	XtAddCallback( vardata_dialog, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxVardata_dialogContext );

	XtVaSetValues(vardata_dialog,
			XmNdefaultButton, apply_button,
			NULL );

	XtAddCallback( apply_button, XmNactivateCallback,
			activateCB_apply_button,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( arrowButton2, XmNactivateCallback,
			activateCB_arrowButton2,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( arrowButton1, XmNactivateCallback,
			activateCB_arrowButton1,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( edit_Load, XmNactivateCallback,
			activateCB_edit_Load,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( edit_Close, XmNactivateCallback,
			activateCB_edit_Close,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( edit_Restore, XmNactivateCallback,
			activateCB_edit_Restore,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( BDoc, XmNactivateCallback,
			activateCB_BDoc,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( valori_Variables, XmNactivateCallback,
			activateCB_valori_Variables,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( valori_Inp_Var, XmNactivateCallback,
			activateCB_valori_Inp_Var,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( valori_Out_Var, XmNactivateCallback,
			activateCB_valori_Out_Var,
			(XtPointer) UxVardata_dialogContext );

	XtAddCallback( valori_Data, XmNactivateCallback,
			activateCB_valori_Data,
			(XtPointer) UxVardata_dialogContext );



	return ( vardata_dialog );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_popup_vardata( _Uxtipolista )
	int	_Uxtipolista;
{
	Widget                  rtrn;
	_UxCvardata_dialog      *UxContext;

	UxVardata_dialogContext = UxContext =
		(_UxCvardata_dialog *) XtMalloc( sizeof(_UxCvardata_dialog) );

	tipolista = _Uxtipolista;
	{
		swidget sw_sel; /* bottone dell'option menu che
				   definisce il tipo di lista desiderato */
		rtrn = _Uxbuild_vardata_dialog();

		modified=False; /* setta il Boolean che indica se sono state
		                   effettuate modifiche */
		tipo_lista=tipolista;
		switch(tipo_lista)
			{
			case ALL_VAR:
				sw_sel=valori_Variables;
				break;
			case INP_VAR:
				sw_sel=valori_Inp_Var;
				break;
			case OUT_VAR:
				sw_sel=valori_Out_Var;
				break;
			case ALL_DATA:
				sw_sel=valori_Data;
				break;
			}
		set_something_val(UxGetWidget(menu_valori),
		                       XmNmenuHistory,
		                       UxGetWidget(sw_sel));
		
		lista_edit_on=False;
#ifdef ORIGINALE
		lista.riga=NULL;
#endif
		lista.num_righe=0;
		aggiorna_all_lists();
		UxPopupInterface(rtrn, no_grab);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	popup_vardata( _Uxtipolista )
	int	_Uxtipolista;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_popup_vardata( _Uxtipolista );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

