/*
 * preview.c
 *
 * 
 */
#include <stdio.h>
#include <malloc.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/ToggleB.h>
#include <Xm/Xm.h>

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "bistrutt.h"
#include "parametri.h"
#include "preview.h"
#include "messaggi.h"
#include "filtri.h"

#include "cursore.h"
#include "previewSnap.h"
#include "file_selection.h"
#include "cont_rec.h"
#include "selVar.h"
#include "tabelle_malf.h"

#include "option.h"
#include "banco_globals.h"

void viewToggleCb();
extern char *creaTestoMalfList ();
SNAP_VIEW snap_view;

extern Widget areaMessaggi;
extern Widget previewSnap;

extern VARIABILI *variabili;
extern int tot_variabili;
extern char *ind_sh_top;

extern char *FILES_PATH;

char   *nome_blocco(char *,int,int);


int set_variabili_default (Widget);
int scrivi_lista_var (FILE *,Widget);
int leggi_lista_var (FILE *,Widget);
int check_lista_var(Widget);
int selezione_file (Widget,int,int,char*);
int load_preview (Widget,char*);
void set_cursor (Widget,int);
int carica_lista_var(Widget);
int clear_preview_entry(PREVIEW_DATA *,int);
int SD_snapview (int, SNAP_VIEW *);
int add_preview_entry (Widget, PREVIEW_DATA *, int,int);
int lista_modblock (int,int,int**,int*,int*,int);
int lista_match(Widget,char*,Boolean *,int);


char *Utstrstr(char *,char *);









int crea_preview_entry (parent)
Widget parent;
{
int i;
Widget BaseWidget;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(parent); 
	BaseWidget = Ctx->UxviewRc;

for (i=0; i<MAX_VIEW_ENTRY; i++)
	{
	sprintf (c_num[i],"%d",i);

	Ctx->UxViewScrollForm[i] = XtVaCreateManagedWidget( "",
         xmFormWidgetClass, Ctx->UxviewRc,
         XmNrubberPositioning, FALSE,
         XmNresizePolicy, XmRESIZE_NONE,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightWidget, Ctx->UxviewRc,
         NULL );

   Ctx->UxViewScrollTb[i] = XtVaCreateManagedWidget( "",
         xmToggleButtonWidgetClass, Ctx->UxViewScrollForm[i],
         XmNtopAttachment, XmATTACH_FORM,
         XmNleftOffset, 0,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNheight, 10,
         XmNwidth, 25,
         XmNy, 0,
         XmNx, 5,
         NULL );
	UxPutContext(Ctx->UxViewScrollTb[i], (char *)Ctx);

   Ctx->UxViewScrollText[i] = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, Ctx->UxViewScrollForm[i],
			XmNalignment    , XmALIGNMENT_END,
         XmNtopAttachment, XmATTACH_FORM,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNrightOffset, 5,
         XmNrightAttachment, XmATTACH_FORM,
			XmNwidth, 150,
         NULL );

   Ctx->UxViewScrollLabel[i] = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, Ctx->UxViewScrollForm[i],
			XmNalignment    , XmALIGNMENT_BEGINNING,
         XmNbottomAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNtopOffset, 0,
         XmNrightWidget, Ctx->UxViewScrollText[i],
         XmNrightOffset, 5,
         XmNrightAttachment, XmATTACH_WIDGET,
         XmNleftWidget, Ctx->UxViewScrollTb[i],
         XmNleftOffset, 5,
         XmNleftAttachment, XmATTACH_WIDGET,
         XmNwidth, 300,
         NULL );

	XtAddCallback(Ctx->UxViewScrollTb[i],
						XmNvalueChangedCallback,viewToggleCb,c_num[i]);

	}

}
/*************************************************************/
void viewToggleCb (w, data, reason)
Widget w;
char *data;
XmToggleButtonCallbackStruct *reason;
{
Boolean stato;
int nriga;
int i;
PREVIEW_DATA *p;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);

	p = (PREVIEW_DATA*)&(Ctx->Uxpreview_data);

		stato = reason->set;   /* stato del toggle */
		nriga = atoi(data);

		if (stato)					/* se set deve essere il solo  */
			{
			for (i=0; i<MAX_VIEW_ENTRY; i++)
				{
				if (i == nriga) 
					continue;
				if (p->tb_stat[i])
					{
					p->tb_stat[i] = False;
					XtVaSetValues (Ctx->UxViewScrollTb[i], XmNset, False, NULL);
					}
				}
			}
		p->tb_stat[nriga] = stato;
}
/*************************************************************/
int get_file_view(w)
Widget w;
{
/*
 * apre il file con la lista delle variabili e carica la
 *  struttura
 */
FILE *fp;
char path[400];
int i;
int size=0;

   strcpy (path, FILES_PATH);
   strcat (path,"/");
   strcat (path,VIEW_FILE);

   printf ("apre file %s\n",path);

   if ((fp = fopen (path,"r")) == NULL)
      {
      printf ("il file %s non esiste\n",path);
      if ((fp = fopen (path,"w")) == NULL)
         {
         printf ("non e' possibile creare il file %s\n",path);
         return (-1);
         }
      set_variabili_default (w);
      scrivi_lista_var (fp,w);
      }
   leggi_lista_var(fp,w);
   fclose(fp);

   check_lista_var (w);
   return(0);

}
/*************************************************************/
int loadas_preview (w, modo, tipo)
Widget w;         /* chiamante        */
int modo; /* lettura scrittura              */
int tipo; /* VAR_FOR_CR, PREVIEW_FILE, etc) */
{
char nome[MAX_PATH_LEN];
int ret;

   if ((ret = selezione_file (w,modo,tipo,nome)) < 0)
      return (ret);

	load_preview (w,nome);

   return (0);
}
/*************************************************************/
int load_preview (w,nome)
Widget w;
char *nome;
{
char *messaggio;
FILE *fp;
char path[MAX_PATH_LEN];

	 messaggio = (char *)malloc(strlen(LOAD_FILE_PREV)+
                              strlen(nome)+100+
                              strlen(NOT_ACCESSIBLE));
   set_cursor (w,CLOCK);

	strcpy (path,nome);

	if (!strcmp(nome,VIEW_FILE)) /* legge il file di default    */
      {
      strcpy (path, FILES_PATH);
      strcat (path,"/");
      strcat (path,VIEW_FILE);
		}

   if ((fp = fopen (path,"r")) == NULL)
      {
      sprintf (messaggio,"%s \n %s",nome,NOT_ACCESSIBLE);
      attenzione (w, messaggio);
      free (messaggio);
      return (-1);
      }

   leggi_lista_var (fp,w);
   fclose (fp);

   check_lista_var (w);
   get_val_view (w);
   display_view (w);

   sprintf (messaggio,"%s \n %s", LOAD_FILE_PREV,nome);
   set_cursor (w,NORMALE);
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (0);
}
/*************************************************************/
int saveas_preview (w, modo, tipo)
Widget w;         /* chiamante        */
int modo; /* lettura scrittura              */
int tipo; /* VAR_FOR_CR, PREVIEW_FILE, etc) */
{
char nome[MAX_PATH_LEN];
char *messaggio;
FILE *fp;
int ret;

   if ((ret = selezione_file (w,modo,tipo,nome)) < 0)
      return (ret);
   messaggio = (char *)malloc(strlen(SAVE_FILE_PREV)+
										strlen(nome)+10+
										strlen(NOT_ACCESSIBLE));
   set_cursor (w,CLOCK);

	if ((fp = fopen (nome,"w")) == NULL)
     	{
		sprintf (messaggio,"%s \n %s",nome,NOT_ACCESSIBLE);
		attenzione (w, messaggio);
		free (messaggio);
		return (-1);
		}

	carica_lista_var (w);
	scrivi_lista_var (fp,w);
	fclose (fp);

   sprintf (messaggio,"%s \n %s", SAVE_FILE_PREV,nome);
   set_cursor (w,NORMALE);
   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return (0);
}
/*************************************************************/
int salva_preview_list (w,s)
Widget w;
char *s;
{
int esito = 0;
char *messaggio;
FILE *fp;
char path[MAX_PATH_LEN];

	if (!strcmp(s,VIEW_FILE)) /* salva sul path di default  */
		{
		strcpy (path, FILES_PATH);
   	strcat (path,"/");
   	strcat (path,VIEW_FILE);
		if ((fp = fopen (path,"w")) == NULL)
      	{
			messaggio = (char*)malloc(strlen(path)+strlen(NOT_ACCESSIBLE)+20);
			sprintf (messaggio,"%s \n %s",path,NOT_ACCESSIBLE);
			attenzione (w, messaggio);
			free (messaggio);
			}
		carica_lista_var (w);
		scrivi_lista_var (fp,w);
		fclose (fp);
		}
	return (esito);
}
/*************************************************************/
int set_variabili_default (w)
Widget w;
{
int i;
LISTA_VAR *p;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
	p = (LISTA_VAR *)&(Ctx->Uxlista_var);

 /*  for (i=0; i<MAX_VIEW_ENTRY; i++)  */
   for (i=0; i<0; i++)
      strcpy (p->var_view[i],variabili[i+1].nome);
  
   return (0);
}
/*************************************************************/
int carica_lista_var (w)
Widget w;
{
int i;
LISTA_VAR *p;
PREVIEW_DATA *pv;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
   p = (LISTA_VAR *)&(Ctx->Uxlista_var);
	pv = (PREVIEW_DATA *)&(Ctx->Uxpreview_data);

   for (i=0; i<MAX_VIEW_ENTRY; i++)
      strcpy (p->var_view[i],pv->label[i]);
 
   return (0);
}
/*************************************************************/
int scrivi_lista_var (fp,w)
FILE *fp;
Widget w;
{
LISTA_VAR *p;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
   p = (LISTA_VAR *)&(Ctx->Uxlista_var);

   fwrite ((char *)p, sizeof(LISTA_VAR), 1 ,fp);
   return (0);
}
/*************************************************************/
int leggi_lista_var (fp,w)
FILE *fp;
Widget w;
{
LISTA_VAR *p;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
   p = (LISTA_VAR *)&(Ctx->Uxlista_var);

   fread ((char *)p, sizeof(LISTA_VAR), 1 ,fp);
   return (0);
}
/*************************************************************/
int check_lista_var (w)
Widget w;
{
int i,k,kk;
int indx;
PREVIEW_DATA *p;
LISTA_VAR *l;
char *messaggio;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
   p = (PREVIEW_DATA*)&(Ctx->Uxpreview_data);
	l = (LISTA_VAR*)&(Ctx->Uxlista_var);

	messaggio = (char *)malloc (strlen(VAR_NOT_EXIST)+MAX_LUN_NOME_VAR+20);
   p->nentry = 0;
   for (i=0; i<MAX_VIEW_ENTRY; i++)
      {
		if (strlen(l->var_view[i]) <= 0)
			{
			clear_preview_entry (p, i);
			continue;
			}
		if ((indx = get_label_indx (l->var_view[i])) < 0)
			{
			sprintf (messaggio,"%s\n %s ",l->var_view[i],VAR_NOT_EXIST);
			attenzione (w,messaggio);
			add_message (areaMessaggi,messaggio,LIVELLO_3);
			clear_preview_entry (p, i);
			continue;
			}
		else
			add_preview_entry (w, p, i, indx);
      }

	free (messaggio);
   return (0);
}
/*************************************************************/
int add_preview_entry (w, p, i, indx)
Widget w;              /* interfaccia chiamante              */
PREVIEW_DATA *p;       /* struttura da riempire              */
int i;                 /* indice nella lista                 */
int indx;              /* indice nel database variabili      */
{
char *app;
int k,n;
char *messaggio;
int esito;

	esito = 0;

/*
 * se entra dall' esterno:
 *		prima cerca l' eventuale toggle destinazione settato
 *		eventualmente cerca il primo posto libero
 */
	if (i == NON_NOTA) /* inserimento dall' esterno   */
		{
		for (k=0; k<MAX_VIEW_ENTRY; k++)
			{
			if (p->tb_stat[k])
				{
				printf ("add_prev. indice  %d\n",k);
				i = k;
				break;
				}
			}
		if (k == MAX_VIEW_ENTRY)	/* nessun toggle settato	*/
			{
			for (n=0; n<MAX_VIEW_ENTRY; n++)
				{
				if (p->entry_valida[n] == False)		/* primo slot libero */
					{
					i = n;
					break;
					}
				}
			}
		if (n == MAX_VIEW_ENTRY)   /* nessun toggle e niente spazio */
			{
			messaggio = (char *)malloc (strlen(NO_MORE_ENTRIES)+1);
			sprintf (messaggio,"%s",NO_MORE_ENTRIES);
			attenzione (w, messaggio);
			free (messaggio);
			return (-1);
			}				
		}
	p->entry_valida[i] = True;
   p->tb_stat[i] = False;
	strcpy (p->label[i],variabili[indx].nome);
   strncpy (p->descrizione[i],variabili[indx].descr,MAX_LUN_DESCR_VIEW-1);
   p->descrizione[i][MAX_LUN_DESCR_VIEW-1] = '\0';
   p->puntatore[i] = variabili[indx].addr;
   p->nentry++;

	return (0);
}
/*************************************************************/
int delete_preview_entry (p, w)
PREVIEW_DATA *p;
Widget w;
{
int i;
	for (i=0; i<MAX_VIEW_ENTRY; i++)
   	{
   	if (p->tb_stat[i])
      	{
      	p->tb_stat[i] = False;
      	clear_preview_entry (p, i);
      	}
   	}
	display_view (w);
	return (0);
}
/*************************************************************/
int clear_preview_entry (p, i)
PREVIEW_DATA *p;
int i;
{
char *app;

   app = (char *)malloc(strlen(ENTRY_AVAILABLE)+10);
	p->tb_stat[i] = False;
   p->entry_valida[i] = False;
   p->puntatore[i] = -1;
	strcpy (p->label[i], "");
   sprintf (app,"%s",ENTRY_AVAILABLE);
   strncpy (p->descrizione[i],app,MAX_LUN_DESCR_VIEW-1);
   p->descrizione[i][MAX_LUN_DESCR_VIEW-1] = '\0';
   free(app);
	return (0);
}
/*************************************************************/
int get_val_view (w)
Widget w;
{
int ic_num;
int i;
char *messaggio;
int esito;
PREVIEW_DATA *p;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);
   p = (PREVIEW_DATA*)&(Ctx->Uxpreview_data);

   esito = 0;

/* dati per aggiornamento */
   snap_view.nvar = p->nentry;
   for (i=0; i<MAX_VIEW_ENTRY; i++)
      snap_view.var_val[i].puntatore = p->puntatore[i];
   snap_view.record = Ctx->Uxn;
   snap_view.which  = Ctx->Uxwhich;

   switch (Ctx->Uxwhich)
      {
      case CI_VIEW:
         messaggio = (char *)malloc (strlen(READ_SNAP_VIEW)+strlen(OPER_FALLITA)+20);
         sprintf (messaggio,"%s %d",READ_SNAP_VIEW,Ctx->Uxn);
         break;
      case BT_VIEW:
         messaggio = (char *)malloc (strlen(READ_BT_VIEW)+strlen(OPER_FALLITA)+20);
         sprintf (messaggio,"%s %d",READ_BT_VIEW,Ctx->Uxn);
         break;
		default:
			printf ("get_val_view: which non riconosciuto: %d\n",Ctx->Uxwhich);
			break;
      }
	if (!esito)
   	if (SD_snapview(BANCO, &snap_view) <= 0)
      	{
      	sprintf (messaggio,"%s : %s",messaggio,OPER_FALLITA);
      	esito = -1;
      	attenzione(w,messaggio);
      	}

   add_message (areaMessaggi,messaggio,LIVELLO_3);
   free (messaggio);
   return(esito);
}
/**************************************************************/
int display_view (w)
Widget w;
{
int i;
char app[20];
char *app_1;

   _UxCpreviewSnap *Ctx = (_UxCpreviewSnap*)UxGetContext(w);

	app_1=(char *)malloc(sizeof(char)*(MAX_LUN_NOME_VAR+MAX_LUN_DESCR_VIEW+10));
   for (i=0; i<MAX_VIEW_ENTRY; i++)
      {
		XtVaSetValues (Ctx->UxViewScrollTb[i], 
							XmNset, Ctx->Uxpreview_data.tb_stat[i], NULL);
		sprintf (app_1,"%s %s",Ctx->Uxpreview_data.label[i],
									  Ctx->Uxpreview_data.descrizione[i]);
      XtVaSetValues (Ctx->UxViewScrollLabel[i],
         RES_CONVERT(XmNlabelString,app_1), NULL);
		if (!Ctx->Uxpreview_data.entry_valida[i])
			strcpy (app," ");
		else
			sprintf (app,"%12.4f",snap_view.var_val[i].valore);
      XtVaSetValues (Ctx->UxViewScrollText[i],
         RES_CONVERT(XmNlabelString,app),NULL);

      }
	free (app_1);
   return (0);
}
/**************************************************************/
int display_var (lista, m, b, kks, tipo)
Widget lista;
int m;       /* modello   */
int b;       /* blocco    */
int kks; /* filtro kks attivo/disattivo */
int tipo; /* filtro tipo attivo/disattivo */
{
int totale,i,k;
Widget p;
#define ALLOC_MIN    5000     /* minimo 5000 variabili */
int num_alloc;
int size_alloc;
int num_alloc_sel;
Widget ListaToSel;


_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(lista);

	ListaToSel   = Ctx->UxselVarToSelect1;

	p = Ctx->UxselVar;
	set_cursor (p, CLOCK);
/*
 * alloca i vettori per i puntatori al database variabili
 */
   if (!Ctx->Uxfirst_time)
		{
                if(Ctx->Uxpunt_var)
                   free (Ctx->Uxpunt_var);
                if(Ctx->Uxpunt_var_kks)
		   free (Ctx->Uxpunt_var_kks);
                if(Ctx->Uxpunt_var_tipo)
		   free (Ctx->Uxpunt_var_tipo);
                if(Ctx->Uxpunt_var_lista)
		   free (Ctx->Uxpunt_var_lista);
		}
	Ctx->Uxfirst_time = 0;

	Ctx->Uxsel_mod = 1;  /* per funzioni find   */
/*	Ctx->Uxsel_mod_b = 1;*/  /* per funzioni find   */

	num_alloc = 1;

/*
 * pulisce le lista
 */
   XtUnmanageChild(ListaToSel);
	XmListDeleteAllItems (ListaToSel);
/************************************************************************
 * FILTRI:
 *    sono previsti i seguenti filtri:
 *          -  filtro lego (modulo/blocco)
 *          -  filtro KKS  (tag nella descrizione della variabile)=misure
 *          -  filtro tipo di grandezza (primo carattere della label )
 *          -  filtro ingresso/uscita
 *          -  filtro var. aingabili
 *    Al momento attuale la relazione tra i filtri e' di tipo AND
 *       nell' ordine specificato sopra.
 *                                           30/1/96 fm
 ************************************************************************/
/* FILTRO PRINCIPALE
 * carica le liste
 */
/*
 * carica la lista
 */
	Ctx->Uxn_varlist = 0;
	size_alloc = ALLOC_MIN;
	Ctx->Uxpunt_var = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc);

	lista_modblock (m,b,
                  &Ctx->Uxpunt_var,&Ctx->Uxn_varlist,&num_alloc,
                  size_alloc);

	Ctx->Uxpunt_var_kks  = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc);
	Ctx->Uxpunt_var_tipo  = (int *)malloc (sizeof(int)*ALLOC_MIN*num_alloc);

/*****************************
 * FILTRO KKS
 */

   if (kks)          /* filtro kks attivo  */
      {
printf ("*** FILTRO KKS ATTIVO [%s] ***\n",Ctx->Uxstringa_kks);
      lista_kks (Ctx->Uxpunt_var,Ctx->Uxn_varlist,&Ctx->Uxn_varlist_kks,
								Ctx->Uxpunt_var_kks,Ctx->Uxstringa_kks);
printf ("*** KKS sel %d \n",Ctx->Uxn_varlist_kks);
		}

/*****************************
 * FILTRO TIPO
 */
   if (tipo && !kks)
      {
printf ("*** FILTRO TIPO ATTIVO  ***\n");
      lista_tipo (Ctx->Uxpunt_var,Ctx->Uxn_varlist,&Ctx->Uxn_varlist_tipo,
                              Ctx->Uxpunt_var_tipo,Ctx->Uxfiltro_tipo);
      printf ("TIPI: %d sel \n",Ctx->Uxn_varlist_tipo);
      }

   if (tipo && kks)
      {
printf ("*** FILTRO TIPO ATTIVO  ***\n");
      lista_tipo (Ctx->Uxpunt_var_kks,Ctx->Uxn_varlist_kks,&Ctx->Uxn_varlist_tipo,
                              Ctx->Uxpunt_var_tipo,Ctx->Uxfiltro_tipo);
      printf ("TIPI: %d sel \n",Ctx->Uxn_varlist_tipo);
      }
/*****************************
 * DISPLAY DELLA LISTA
 * (N.B.: la relazione tra i filtri e AND)
 */

   size_alloc = ALLOC_MIN;

   num_alloc_sel = 0;
   if (kks && !tipo)
      {
      Ctx->Uxpunt_var_lista  = (int *)malloc (sizeof(int)*Ctx->Uxn_varlist_kks);
      display_item_cr (ListaToSel,
                  Ctx->Uxpunt_var_kks,Ctx->Uxn_varlist_kks,
                  &Ctx->Uxpunt_var_lista,&num_alloc_sel,size_alloc);
      Ctx->Uxn_varlist_lista = num_alloc_sel;
      }
   else if (tipo)
      {
      Ctx->Uxpunt_var_lista  = (int *)malloc (sizeof(int)*Ctx->Uxn_varlist_tipo);
      display_item_cr (ListaToSel,
                  Ctx->Uxpunt_var_tipo,Ctx->Uxn_varlist_tipo,
                  &Ctx->Uxpunt_var_lista,&num_alloc_sel,size_alloc);
      Ctx->Uxn_varlist_lista = num_alloc_sel;
      }
   else
      {
      Ctx->Uxpunt_var_lista  = (int *)malloc (sizeof(int)*Ctx->Uxn_varlist);
      display_item_cr (ListaToSel,
                  Ctx->Uxpunt_var,Ctx->Uxn_varlist,
                  &Ctx->Uxpunt_var_lista,&num_alloc_sel,size_alloc);
      Ctx->Uxn_varlist_lista = num_alloc_sel;
      }
	XtManageChild (ListaToSel);

        XtVaSetValues (Ctx->UxselOkpb,XmNsensitive,False,NULL);
        XtVaSetValues (Ctx->UxselVarMenuPopupSelect,XmNsensitive,False,NULL);

	set_cursor (p, NORMALE);
   return(0);
}
/**********************************************************/
int display_blocchi (lista)
Widget lista;
{
   int       k;
   char      testo[200];
   char     *str;
   XmString  c_str;
   int       totale;

	_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(lista);

   XtUnmanageChild(lista);
	Ctx->Uxn_blocchi = 0;

	XmListDeleteAllItems (lista);

   for (k = 0; k < numero_blocchi(ind_sh_top, Ctx->Uxmod_sel); k++)
   {
      sprintf(testo, "%s", nome_blocco(ind_sh_top, Ctx->Uxmod_sel, k+1));
      str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
      strcpy(str, testo);
      c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
      XmListAddItemUnselected(lista, c_str, 0);
		Ctx->Uxn_blocchi++;
      XSync(UxDisplay, False);
      if (k == Ctx->Uxblocco_sel)
         XmListSelectPos(lista, k, False);
      XtFree(str);
      XmStringFree(c_str);
   }
   if (k == Ctx->Uxblocco_sel)
      XmListSelectPos(lista, k, False);
	Ctx->Uxsel_mod_b = 1;
   XSync(UxDisplay, False);
   XtManageChild(lista);
}
/**************************************************************/
int next_pos_bloc (lista, dir, text)
Widget lista;
int dir;
Widget text;
{
   _UxCselVar *Ctx = (_UxCselVar*)UxGetContext(lista);

   Ctx->Uxstringa_match_b = XmTextGetString (text);
   if (strlen(Ctx->Uxstringa_match_b) == 0) /* no input */
      {
      XtFree (Ctx->Uxstringa_match_b);
      return (0);
      }

   if (Ctx->Uxfirst_time_find_b)
      {
      Ctx->Uxold_stringa_match_b = 
						(char *)malloc(strlen(Ctx->Uxstringa_match_b)+1);
      strcpy (Ctx->Uxold_stringa_match_b,Ctx->Uxstringa_match_b);
      Ctx->Uxmatch_b = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_blocchi);
      Ctx->Uxnmatch_b = lista_match_bloc (lista,
				Ctx->Uxstringa_match_b,Ctx->Uxmatch_b,
				Ctx->Uxn_blocchi,Ctx->Uxmod_sel);
      Ctx->Uxlast_match_b = 0;
      Ctx->Uxfirst_time_find_b = 0;
      }

   if (strcmp(Ctx->Uxold_stringa_match_b,Ctx->Uxstringa_match_b) || 
							Ctx->Uxsel_mod_b )
      {
printf ("nuovo match con [%s] \n",Ctx->Uxstringa_match_b);
      free (Ctx->Uxold_stringa_match_b);
      Ctx->Uxold_stringa_match_b = 
				(char *)malloc(strlen(Ctx->Uxstringa_match_b)+1);
      strcpy (Ctx->Uxold_stringa_match_b,Ctx->Uxstringa_match_b);
      free (Ctx->Uxmatch_b);
      Ctx->Uxmatch_b = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_blocchi);
      Ctx->Uxnmatch_b = lista_match_bloc (lista,
				Ctx->Uxstringa_match_b,Ctx->Uxmatch_b,
							Ctx->Uxn_blocchi,Ctx->Uxmod_sel);
      Ctx->Uxlast_match_b = 0;
      if (Ctx->Uxsel_mod_b) Ctx->Uxsel_mod_b = 0;
      }

   if (Ctx->Uxnmatch_b > 0)
      posiziona_lista (lista, Ctx->Uxmatch_b,dir,&Ctx->Uxlast_match_b);

   XtFree (Ctx->Uxstringa_match_b);
   return (0);
}
/**************************************************************/
int next_pos (lista, dir, text)
Widget lista;
int dir;
Widget text;
{
int interfaccia;

_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(lista);

	interfaccia = Ctx->Uxchi;

   Ctx->Uxstringa_match = XmTextGetString (text);
   if (strlen(Ctx->Uxstringa_match) == 0) /* no input */
      {
      XtFree (Ctx->Uxstringa_match);
      return (0);
      }

   if (Ctx->Uxfirst_time_find)
      {
      Ctx->Uxold_stringa_match = (char *)malloc(strlen(Ctx->Uxstringa_match)+1);
      strcpy (Ctx->Uxold_stringa_match,Ctx->Uxstringa_match);
		if ((interfaccia != MALFUNZIONI) && (interfaccia != SOMMARIO_MALF) &&
       (interfaccia != FUNZIONI_REMOTE) && (interfaccia != SOMMARIO_FREM))
      	Ctx->Uxmatch = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_varlist);
		else
			{
      	Ctx->Uxmatch = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_conf_malf);
			}
      Ctx->Uxnmatch = lista_match (lista,Ctx->Uxstringa_match,
															Ctx->Uxmatch,interfaccia);
      Ctx->Uxlast_match = 0;
      Ctx->Uxfirst_time_find = 0;
      }
   if (strcmp(Ctx->Uxold_stringa_match,Ctx->Uxstringa_match) || Ctx->Uxsel_mod )
      {
      free (Ctx->Uxold_stringa_match);
      Ctx->Uxold_stringa_match = (char *)malloc(strlen(Ctx->Uxstringa_match)+1);
      strcpy (Ctx->Uxold_stringa_match,Ctx->Uxstringa_match);
      free (Ctx->Uxmatch);
		if ((interfaccia != MALFUNZIONI) && (interfaccia != SOMMARIO_MALF) &&
       (interfaccia != FUNZIONI_REMOTE) && (interfaccia != SOMMARIO_FREM))
      	Ctx->Uxmatch = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_varlist);
		else
			{
         Ctx->Uxmatch = (Boolean *)malloc(sizeof(Boolean)*Ctx->Uxn_conf_malf);
         }
      Ctx->Uxnmatch = lista_match (lista,Ctx->Uxstringa_match,
															Ctx->Uxmatch,interfaccia);
      Ctx->Uxlast_match = 0;
      if (Ctx->Uxsel_mod) Ctx->Uxsel_mod = 0;
      }

   if (Ctx->Uxnmatch > 0)
      posiziona_lista (lista, Ctx->Uxmatch,dir,&Ctx->Uxlast_match);

   XtFree (Ctx->Uxstringa_match);
   return (0);
}
/***********************************************************/
int lista_match(lista,s,flag,interf)
Widget lista;
char *s;
Boolean *flag;
int interf;
{
int i;
int nmatch = 0;
extern M_COMPONENT *m_component;
extern M_COMP_TYPE *m_comp_type;
char testo[1000];
int comp_i;				/* indice in M_COMPONENT 	*/
int comp_type_i;		/* indice in M_COMP_TYPE	*/

_UxCselVar *Ctx = (_UxCselVar*)UxGetContext(lista);

	if ((interf != MALFUNZIONI) && (interf != SOMMARIO_MALF) &&
		 (interf != FUNZIONI_REMOTE) && (interf != SOMMARIO_FREM))
		{
   	for (i=0; i<Ctx->Uxn_varlist_lista; i++)
      	{
      	if ((Utstrstr (variabili[Ctx->Uxpunt_var_lista[i]].nome, s) != NULL) ||
          	(Utstrstr (variabili[Ctx->Uxpunt_var_lista[i]].descr, s) != NULL))
         	{
         	flag[i] = True;
         	nmatch ++;
         	}
      	else
         	flag[i] = False;
      	}
		}
	else  /* si tratta di malf. o frem		*/
		{
		for (i=0; i<Ctx->Uxn_conf_malf_lista; i++)
         {
			comp_i = Ctx->Uxpunt_malf_lista[i];
			strcpy (testo,creaTestoMalfList(m_component,comp_i,m_comp_type));
			if (Utstrstr (testo, s) != NULL)
				{
				flag[i] = True;
            nmatch ++;
				}
			else
            flag[i] = False;
			}
		}
   return (nmatch);
}
/***********************************************************/
int lista_match_bloc (lista,s,flag,n,mod)
Widget lista;
char *s;
Boolean *flag;
int n;
int mod;
{
int i;
int nmatch = 0;

   for (i=0; i<n; i++)
      {
      if (Utstrstr (nome_blocco(ind_sh_top, mod, i+1), s) != NULL) 
         {
         flag[i] = True;
         nmatch ++;
         }
      else
         flag[i] = False;
      }
   return (nmatch);
}
