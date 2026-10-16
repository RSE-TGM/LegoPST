/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit_variabili.c	5.3\t2/13/96";
/*
   modulo monit_variabili.c
   tipo 
   release 5.3
   data 2/13/96
   reserved @(#)monit_variabili.c	5.3
*/
/* movariabili legge la lista delle variabili di uscita e le mostra
 * all'utente per la scelta delle variabili da memorizzare  */

# include <stdio.h>
# include <string.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include "vmsipc.h"
#endif
# include <X11/Xlib.h>
# include <Mrm/MrmAppl.h>
# include <Xm/List.h>
# include <Xm/Text.h>
# include <Xm/Xm.h>
# include "libutilx.h"
# include "sim_param.h"		/* parametri generali LEGO          */
# include "sim_types.h"		/* tipi e costanti LEGO             */
# include "monit.h"
# include "monituil.h"
# include <Rt/RtMemory.h>

Widget    widget_array[MAX_WIDGETS];

/* ************** Definizione variabili globali ******************** */

extern FLAG_SEL flag_sel;
extern int nu_var_sel;

extern VARIABILI *variabili;	/* database delle variabili  */
extern int tot_variabili;	/* numero totale delle variabili      */
extern char *ind_sh_top;	/* puntatore inizio shared memory sh_var */
Display  *display;
char     *nome_file_selezione;	/* nome del file contenente le variabili da
				 * registrare per la grafica */
SELEZIONE selezione;		/* struttura contenente l'headre del file
				 * delle variabili                                  */
int       modello_selezionato = 1;
int       blocco_selezionato = 1;
int       flag_salva;		/* flag per l'operazione di salvataggio e
				 * caricamento delle variabili da registrare
				 * per la grafica  */
int       lung_non_sel;		/* numero di variabili non selezionate
				 * visualizzate */
int       start_ricerca = 1;


/* ***************** Elenco procedure inserite ******************** */
void 
mo_var_sel(Widget);
void 
mo_var(Widget);
void 
mo_var_sel_sigla(int, int);
void 
mo_var_sigla(int, int);
void 
mo_modelli(Widget);
void 
mo_blocchi(Widget);
void      seleziona_var();
void      paste_var_selez ();
void      seleziona_activate();
void      seleziona_file();
void      salva_file();
void      cerca_var_avanti();
void      cerca_var_indietro();
/* void set_something_val(); */


char   *nome_modello (char *, int);

/* **************** Procedure *************************************** */


void 
mo_var_sel(lista)
   Widget    lista;
/* *************** Definizione variabili locali  ***************** */
{
   int       k;
   char      testo[500];
   char     *str;
   int       totale;
   int       num_bl;
   int       posizione = 0;
   XmString  c_str;
   XmString  c_app_str;
   char      app_str[80];

/*
   XtUnmanageChild(lista);
   get_something(lista, XmNitemCount, (void*) &totale);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));
*/
   XmListDeleteAllItems(lista);

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);

   for (k = 1;
           (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == numero_blocchi(ind_sh_top,
                                                   modello_selezionato)+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == GRAF_SEL  ||
               variabili[k - 1].typ == GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == GRAF_SEL  ||
	       variabili[k - 1].typlg == GRAF_SEL_MOM )
           )
         )
      {
	 ++posizione;
	 sprintf( testo, "%s - %s", variabili[k - 1].nome,
                  variabili[k - 1].descr);
	 str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
	 strcpy(str, testo);
	 c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
	 XmListAddItemUnselected(lista, c_str, 0);
/*
	 XSync(display, False);
*/
         if (flag_sel == SEL_REG && variabili[k - 1].typ == GRAF_SEL_MOM ||
             flag_sel == SEL_LGPH && variabili[k - 1].typlg == GRAF_SEL_MOM)
	 {
	    XmListSelectPos(lista, posizione, False);
/*
	    XSync(display, False);
*/
	 }
	 XtFree(str);
	 XmStringFree(c_str);
      }
/* aggiorna la label k_elenco_variabili_sel_label  */
   sprintf(app_str, "Elenco variabili selezionate [%d]", posizione);
/* c_app_str=XmStringCreateLtoR(app_str,XmSTRING_DEFAULT_CHARSET);
 * set_something_val(widget_array[k_elenco_variabili_sel_label],
 * XmNlabelString,(void*) c_app_str); XmStringFree(c_app_str); */
/*
   XtManageChild(lista);
*/
}

void 
mo_var_sel_sigla(elemento, num_elementi)
   int       elemento;
   int       num_elementi;
{
   int       k, i, elemento_cor;
   int       posizione;
   int       num_bl;

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);

   for (i = 0; i < num_elementi; i++)
   {
      posizione = 0;
      elemento_cor = elemento - i;
/*
      XSync(display, False);
*/
      for (k = 1;
                (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == 1+numero_blocchi(ind_sh_top,
                                                 modello_selezionato) ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == GRAF_SEL  ||
	       variabili[k - 1].typ == GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == GRAF_SEL  ||
	       variabili[k - 1].typlg == GRAF_SEL_MOM )
           )
         )
	 {
	    ++posizione;
	    if (posizione == elemento_cor)
	    {
               if ( flag_sel == SEL_REG )
	         if (variabili[k - 1].typ == GRAF_SEL_MOM)
		    variabili[k - 1].typ = GRAF_SEL;
	         else
		    variabili[k - 1].typ = GRAF_SEL_MOM;
               else
	         if (variabili[k - 1].typlg == GRAF_SEL_MOM)
		    variabili[k - 1].typlg = GRAF_SEL;
	         else
		    variabili[k - 1].typlg = GRAF_SEL_MOM;

	       break;
	    }
	 }
      }
}

void 
mo_var_sigla(elemento, num_elementi)
   int       elemento;
   int       num_elementi;
{
   int       k, i, elemento_cor;
   int       posizione;
   int       num_bl;

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);
   for (i = 0; i < num_elementi; i++)
   {
      posizione = 0;
      elemento_cor = elemento - i;
/*
      XSync(display, False);
*/
      for (k = 1;
            (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
	 {
	    ++posizione;
	    if (posizione == elemento_cor)
	    {
               if ( flag_sel == SEL_REG )
	          if (variabili[k - 1].typ == NO_GRAF_SEL_MOM)
				     variabili[k - 1].typ = NO_GRAF_SEL;
	          else
				     variabili[k - 1].typ = NO_GRAF_SEL_MOM;
               else
	          if (variabili[k - 1].typlg == NO_GRAF_SEL_MOM)
		     variabili[k - 1].typlg = NO_GRAF_SEL;
	          else
		     variabili[k - 1].typlg = NO_GRAF_SEL_MOM;
	       break;
	    }
	 }
   }
}

void 
mo_var(lista)
   Widget    lista;
/* *************** Definizione variabili locali  ***************** */
{
   int       k;
   int       totale;
   char      testo[500];
   char     *str;
   XmString  c_str;
   int       posizione = 0;
   int       num_bl;
   XmString  c_app_str;
   char      app_str[80];


/*
   XtUnmanageChild(lista);
*/
/*
   get_something(lista, XmNitemCount, (void*) &totale);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));
*/
   XmListDeleteAllItems(lista);

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);

   for (k = 1;
                (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
      {
	 ++posizione;
	 sprintf( testo, "%s - %s", variabili[k - 1].nome, 
                  variabili[k - 1].descr);
	 str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
	 strcpy(str, testo);
	 c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
	 XmListAddItemUnselected(lista, c_str, 0);
	 /*XSync(display, False);*/
         if (flag_sel == SEL_REG && variabili[k - 1].typ == NO_GRAF_SEL_MOM ||
             flag_sel == SEL_LGPH && variabili[k - 1].typlg == NO_GRAF_SEL_MOM)
	 {
	    XmListSelectPos(lista, posizione, False);
	    /*XSync(display, False);*/
	 }
	 XtFree(str);
	 XmStringFree(c_str);
      }
   lung_non_sel = posizione;
/* aggiorna la label k_elenco_variabili_label  */
   sprintf(app_str, "Elenco variabili selezionate [%d]", posizione);
/* c_app_str=XmStringCreateLtoR(app_str,XmSTRING_DEFAULT_CHARSET);
 * set_something_val(widget_array[k_elenco_variabili_label],
 * XmNlabelString,(void*) c_app_str); XmStringFree(c_app_str); */
/*
   XtManageChild(lista);
*/
}

void 
mo_modelli(lista)
   Widget    lista;
{
   int       k;
   int       nmod;
   char      testo[200];
   char     *str;
   XmString  c_str;
   int       totale;

   nmod = numero_modelli(ind_sh_top);
/*
   XtUnmanageChild(lista);
   get_something(lista, XmNitemCount, (void*) &totale);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));
*/
   XmListDeleteAllItems(lista);

   for (k = 0; k < nmod; k++)
   {
      sprintf(testo, "%s", nome_modello(ind_sh_top, k + 1));
      str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
      strcpy(str, testo);
      c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
      XmListAddItemUnselected(lista, c_str, 0);
/*
      XSync(display, False);
*/
      if (k == modello_selezionato)
      	 XmListSelectPos(lista, k, False);
/*
      XSync(display, False);
*/
      XtFree(str);
      XmStringFree(c_str);
   }
/*
   XtManageChild(lista);
*/
}

void 
mo_blocchi(lista)
   Widget    lista;
{
   int       k;
   char      testo[200];
   char     *str;
   XmString  c_str;
   int       totale;
/*
   XtUnmanageChild(lista);

   get_something(lista, XmNitemCount, (void*) &totale);
   if (totale)
      for (k = 1; k <= totale; k++, XmListDeletePos(lista, 1));
*/
   XmListDeleteAllItems(lista);
   for (k = 0; k < numero_blocchi(ind_sh_top, modello_selezionato); k++)
   {
      sprintf(testo, "%s", nome_blocco(ind_sh_top, modello_selezionato, k + 1));
      str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
      strcpy(str, testo);
      c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
      XmListAddItemUnselected(lista, c_str, 0);
/*
      XSync(display, False);
*/
      if (k == blocco_selezionato)
      	 XmListSelectPos(lista, k, False);
/*
      XSync(display, False);
*/
      XtFree(str);
      XmStringFree(c_str);
   }
   if(lista != widget_array[k_elenco_blocchi_modat])
	{
   	sprintf(testo, "Tutti i blocchi");
   	str = (char *) XtMalloc(sizeof(char) * (strlen(testo) + 20));
   	strcpy(str, testo);
   	c_str = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
   	XmListAddItemUnselected(lista, c_str, 0);
   	XtFree(str);
   	XmStringFree(c_str);
	}
   if (k == blocco_selezionato)
      XmListSelectPos(lista, k, False);
/*
   XSync(display, False);
   XtManageChild(lista);
*/
}

void 
seleziona_var(w, tag, reason)
   Widget    w;
   int      *tag;
   XmListCallbackStruct *reason;
{
   int       i;
   XSync(display, False);
   if (*tag == k_elenco_modelli)
   {
      modello_selezionato = reason->item_position;
      blocco_selezionato = 1;
      mo_blocchi(widget_array[k_elenco_blocchi]);
      mo_var(widget_array[k_elenco_variabili]);
      mo_var_sel(widget_array[k_elenco_variabili_sel]);
      start_ricerca = 1;
   }
   if (*tag == k_elenco_blocchi)
   {
      blocco_selezionato = reason->item_position;
      mo_var(widget_array[k_elenco_variabili]);
      mo_var_sel(widget_array[k_elenco_variabili_sel]);
      start_ricerca = 1;
   }
   if (*tag == k_elenco_variabili)
   {
   ;
   /*
      mo_var_sigla(reason->item_position, reason->selected_item_count);
   */
   }

   if (*tag == k_elenco_variabili_sel)
   {
   ;
   /*
   mo_var_sel_sigla(reason->item_position, reason->selected_item_count);
   */
   }

}

void 
paste_var_selez (w, tag, reason)
   Widget    w;
   int      *tag;
   unsigned long *reason;
{
   int       k;
   int       j;
   XmString  c_app_str;
   char      app_str[80];
   char      mom[120];
   int       posizione = 0;

   XmString  *paste_str;
   XmString  add_str;
   int       itemcount;
   char      *text=NULL;
   int       jj,kk;
   int       num_bl;
   int       *vet_elem;
   int       num_elem;


   if (*tag == k_elenco_variabili_cerca_avanti)
      {
      cerca_var_avanti();
      return;
      }
   if (*tag == k_elenco_variabili_cerca_ind)
      {
      cerca_var_indietro();
      return;
      }

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);
   if ( (*tag != k_elenco_variabili_cerca_avanti) ||
        (*tag != k_elenco_variabili_cerca_ind)    )
       {
       if (*tag == k_elenco_variabili_aggiungi)
           {
           XtVaGetValues(widget_array[k_elenco_variabili],
                         XmNselectedItemCount,&itemcount,
                         XmNselectedItems,&paste_str,NULL);
           }
       else if (*tag == k_elenco_variabili_elimina)
              {
              XtVaGetValues(widget_array[k_elenco_variabili_sel],
                            XmNselectedItemCount,&itemcount,
                            XmNselectedItems,&paste_str,NULL);
              }

/*
       Ciclo su tutte le variabili per fare l'assegnamento
*/
/*
       printf("\n itemcount=%d mod=%d  blocco=%d tot_v=%d num_var_sel=%d\n",
                itemcount,modello_selezionato,blocco_selezionato,
                tot_variabili,nu_var_sel);
*/
       for(jj=0;jj<itemcount;jj++)
           {
           XmStringGetLtoR(paste_str[jj], XmSTRING_DEFAULT_CHARSET, &text);
           /*printf("TEXT=%10.10s\n",text);*/
/*        
           Ricreazione della lista cancellata e inserimento in quella
           che viene accresciuta.
*/
           for(k = 1;
              (k<=tot_variabili)&&(variabili[k-1].mod<=modello_selezionato);
               k++)
              {
/*
              Per le variabili del blocco del modello o se si tratta 
              del caso tutti i blocchi assegna il flag per la grafica
*/
              if ( 
                 (variabili[k - 1].mod == modello_selezionato)  &&
                 (blocco_selezionato == num_bl+1 ||
                 variabili[k - 1].blocco == blocco_selezionato) &&
                 (strstr(text,variabili[k - 1].nome)!=NULL)
                 ) 
                 {
                 if (flag_sel == SEL_REG)
                    {
                    if (*tag == k_elenco_variabili_aggiungi)
                       {
                       variabili[k - 1].typ = GRAF_SEL;
                       nu_var_sel++;
                       }
                    else if (*tag == k_elenco_variabili_elimina)
                       {
                       variabili[k - 1].typ = NO_GRAF_SEL;
                       nu_var_sel--;
                       }
                    }
                 else if (flag_sel == SEL_LGPH)
                    {
                    if (*tag == k_elenco_variabili_aggiungi)
                       {
                       variabili[k - 1].typlg = GRAF_SEL;
                       nu_var_sel++;
                       }
                    else if (*tag == k_elenco_variabili_elimina)
                       {
                       variabili[k - 1].typlg = NO_GRAF_SEL;
                       nu_var_sel--;
                       }
                    }
                 }  /* Fine if sulla variabile */

              } /*  Fine ciclo k  */
/*         Libero la stringa */
           if( text!=NULL)
             XtFree(text);
           }    /*  Fine ciclo jj (itemcount) */

/*
       Cancellazione  e inserimento oggetti coinvolti 
*/
       if (*tag == k_elenco_variabili_aggiungi)
          {
   		 XmListAddItems(widget_array[k_elenco_variabili_sel],paste_str,itemcount,0);
          XSync(display, False);
   		 XmListDeleteItems(widget_array[k_elenco_variabili],paste_str,itemcount);
          }
       else if (*tag == k_elenco_variabili_elimina)
          {
   		 XmListAddItems(widget_array[k_elenco_variabili],paste_str,itemcount,0);
          XSync(display, False);
   		 XmListDeleteItems(widget_array[k_elenco_variabili_sel],paste_str,itemcount);
          }



   /* visualizza la nuova selezione   */
/********
       mo_var(widget_array[k_elenco_variabili]);
       mo_var_sel(widget_array[k_elenco_variabili_sel]);
***********/

       sprintf(app_str, "Num. var. sel. >> %d", nu_var_sel);
       c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
       set_something_val(widget_array[k_elenco_variabili_nu_var_sel],
                        XmNlabelString, (XtArgVal) c_app_str);

   /* aggiorna la label k_elenco_variabili_dialog_label  */
       sprintf(app_str, "File %s ", nome_file_selezione);
       c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
       set_something_val(widget_array[k_elenco_variabili_dialog_label],
 		               XmNlabelString, (XtArgVal) c_app_str);
       set_something_val(widget_array[k_elenco_variabili_dialog_label],
 		               XmNwidth, (XtArgVal)630);
       set_something_val(widget_array[k_elenco_variabili_dialog_label],
 		               XmNheight, (XtArgVal)35);
       XmStringFree(c_app_str);
       }  /* Fine k_elenco_variabili_aggiungi-elimina */
}

void 
seleziona_activate(w, tag, reason)
   Widget    w;
   int      *tag;
   unsigned long *reason;
{
   int       k;
   int       j;
   XmString  c_app_str;
   char      app_str[80];
   char      mom[120];
   int       posizione = 0;

/*
   XSync(display, False);
*/
   if (*tag == k_elenco_variabili_cerca_avanti)
   {
      cerca_var_avanti();
   }
   if (*tag == k_elenco_variabili_cerca_ind)
   {
      cerca_var_indietro();
   }
   if ((*tag != k_elenco_variabili_cerca_avanti) &&
       (*tag != k_elenco_variabili_cerca_ind))
   {

      if (*tag == k_elenco_variabili_aggiungi)
         if ( flag_sel == SEL_REG )
         {
	    for (k = 1; k <= tot_variabili; k++)
	       if (variabili[k - 1].typ == NO_GRAF_SEL_MOM)
                 {
	          variabili[k - 1].typ = GRAF_SEL;
                  nu_var_sel++;
                 }
         }
         else
	    for (k = 1; k <= tot_variabili; k++)
	       if (variabili[k - 1].typlg == NO_GRAF_SEL_MOM)
                 { 
	          variabili[k - 1].typlg = GRAF_SEL;
                  nu_var_sel++;
                 }
      if (*tag == k_elenco_variabili_elimina)
         if ( flag_sel == SEL_REG )
         {
	    for (k = 1; k <= tot_variabili; k++)
	       if (variabili[k - 1].typ == GRAF_SEL_MOM)
                 {
	          variabili[k - 1].typ = NO_GRAF_SEL;
                  nu_var_sel--;
                 }
         }
         else
	    for (k = 1; k <= tot_variabili; k++)
	       if (variabili[k - 1].typlg == GRAF_SEL_MOM)
                 {
	          variabili[k - 1].typlg = NO_GRAF_SEL;
                  nu_var_sel--;
                 }
   /* verifica che non ci siano variabili doppie nella selezione  */
/***************
      if ( flag_sel == SEL_REG )
      for (k = 0; k < tot_variabili; k++)
	 if (variabili[k].typ == GRAF_SEL)
	 {
	    for (j = k + 1; j < tot_variabili; j++)
	       if (variabili[k].addr == variabili[j].addr  &&
                   variabili[j].typ == GRAF_SEL)
                 {
                  nu_var_sel--;
		  variabili[j].typ = NO_GRAF_SEL;
                 }
	 }
****************************/
   /* visualizza la nuova selezione   */
      mo_var(widget_array[k_elenco_variabili]);
      mo_var_sel(widget_array[k_elenco_variabili_sel]);

      sprintf(app_str, "Num. var. sel. >> %d", nu_var_sel);
      c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
      set_something_val(widget_array[k_elenco_variabili_nu_var_sel],
                    XmNlabelString, (XtArgVal)c_app_str);

   /* aggiorna la label k_elenco_variabili_dialog_label  */
      sprintf(app_str, "File %s ", nome_file_selezione);
      c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
      set_something_val(widget_array[k_elenco_variabili_dialog_label],
		    XmNlabelString, (XtArgVal)c_app_str);
      set_something_val(widget_array[k_elenco_variabili_dialog_label],
		    XmNwidth, (XtArgVal) 630);
      set_something_val(widget_array[k_elenco_variabili_dialog_label],
		    XmNheight, (XtArgVal)35);
      XmStringFree(c_app_str);
   }
}

void 
salva_file(w, tag, reason)
   Widget    w;
   int      *tag;
   unsigned long *reason;
{
   char     *nome;
   char     *testo_p;
   XmString  c_app_str;
   char      app_str[80];
   int       j, k;
/* legge il nome del file */
   testo_p = XmTextGetString
	(widget_array[k_salva_file_dialog_text]);
   strcpy(nome_file_selezione, testo_p);
   XtFree(testo_p);
/* carica l'header del file */
   testo_p = XmTextGetString
	(widget_array[k_elenco_variabili_modello]);
   strcpy(selezione.nome_mod, testo_p);
   XtFree(testo_p);
   testo_p = XmTextGetString
	(widget_array[k_elenco_variabili_descr]);
   strcpy(selezione.nome_selezione, testo_p);
   XtFree(testo_p);
   testo_p = XmTextGetString
	(widget_array[k_elenco_variabili_autore]);
   strcpy(selezione.autore_selezione, testo_p);
   XtFree(testo_p);
   testo_p = XmTextGetString
	(widget_array[k_elenco_variabili_versione]);
   strcpy(selezione.versione_selezione, testo_p);
   XtFree(testo_p);
 
/*******************************************
   if ( flag_sel == SEL_REG )
   for (k = 0; k < tot_variabili; k++)
      if (variabili[k].typ == GRAF_SEL)
      {
	 for (j = k + 1; j < tot_variabili; j++)
	    if (variabili[k].addr == variabili[j].addr)
	       variabili[j].typ = NO_GRAF_SEL;
      }
****************************************************/
   scrive_sel(nome_file_selezione);
   mo_var(widget_array[k_elenco_variabili]);
   mo_var_sel(widget_array[k_elenco_variabili_sel]);
   mo_blocchi(widget_array[k_elenco_blocchi]);
   mo_modelli(widget_array[k_elenco_modelli]);
/* aggiorna la label k_elenco_variabili_dialog_label  */
   sprintf(app_str, "File %s",
	   nome_file_selezione);
   c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNlabelString, (XtArgVal)c_app_str);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNwidth, (XtArgVal)630);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNheight, (XtArgVal)35);
   XmStringFree(c_app_str);
   XtUnmanageChild(widget_array[k_salva_file_dialog]);

}

void 
seleziona_file(w, tag, reason)
   Widget    w;
   int      *tag;
   XmFileSelectionBoxCallbackStruct *reason;
{
   char     *nome;
   char     *testo_p;
   XmString  c_app_str;
   char      app_str[80];
   int       j, k;

   if (reason->reason == 31)
   {
      XmStringGetLtoR(reason->value, XmSTRING_DEFAULT_CHARSET, &nome);
      if (nome != NULL)
      {
	 strcpy(nome_file_selezione, nome);
	 legge_sel(nome_file_selezione);
      /* carica l'header nelle label  */
	 XmTextSetString(widget_array[k_elenco_variabili_modello],
			 selezione.nome_mod);
	 XmTextSetString(widget_array[k_elenco_variabili_descr],
			 selezione.nome_selezione);
	 XmTextSetString(widget_array[k_elenco_variabili_autore],
			 selezione.autore_selezione);
	 XmTextSetString(widget_array[k_elenco_variabili_versione],
			 selezione.versione_selezione);
	 modello_selezionato = 1;
	 blocco_selezionato = 1;
	 mo_var(widget_array[k_elenco_variabili]);
	 mo_var_sel(widget_array[k_elenco_variabili_sel]);
	 mo_blocchi(widget_array[k_elenco_blocchi]);
	 mo_modelli(widget_array[k_elenco_modelli]);
      }
   }
   XtUnmanageChild(widget_array[k_selezione_file_dialog]);
/* aggiorna la label k_elenco_variabili_dialog_label  */
   sprintf(app_str, "File %s",
	   nome_file_selezione);
   c_app_str = XmStringCreateLtoR(app_str, XmSTRING_DEFAULT_CHARSET);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNlabelString, (XtArgVal)c_app_str);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNwidth, (XtArgVal)630);
   set_something_val(widget_array[k_elenco_variabili_dialog_label],
		 XmNheight, (XtArgVal)35);
   XmStringFree(c_app_str);
}


void 
cerca_var_avanti()
{
   char      app_str[80];
   char      mom[120];
   int       posizione = 0;
   int       k;
   int       trovata = 0;
   int num_bl;

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);

   strcpy(app_str, XmTextGetString
	  (widget_array[k_elenco_variabili_text_cerca]));
/* cerca da start_ricerca alla fine */
   for (k = 1;
        (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
      {
	 strcpy(mom, variabili[k - 1].nome);
	 ++posizione;
	 if ( Utstrstr(strcat(mom, variabili[k - 1].descr), app_str) != NULL  &&
	      k > start_ricerca  &&
              ( flag_sel == SEL_REG && variabili[k - 1].typ == NO_GRAF_SEL  ||
                flag_sel == SEL_LGPH && variabili[k - 1].typlg == NO_GRAF_SEL )
            )
	 {
	    start_ricerca = k;
            if ( flag_sel == SEL_REG )
	      variabili[k - 1].typ = NO_GRAF_SEL;
            else
	      variabili[k - 1].typlg = NO_GRAF_SEL;
	    trovata = 1;
	    UtListaScroll(widget_array[k_elenco_variabili],
			  lung_non_sel, 6, posizione - 1);
	    XSync(display, False);
	    break;
	 }
      }
   if (!trovata)
   {
   /* cerca dall'inizio a start_ricerca  */
      posizione = 0;
      for (k = 1;
            (k <= tot_variabili)&&(variabili[k - 1].mod <= modello_selezionato); k++)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
	 {
	    strcpy(mom, variabili[k - 1].nome);
	    ++posizione;
	    if (Utstrstr(strcat(mom, variabili[k - 1].descr), app_str) != NULL                       &&
                (flag_sel == SEL_REG && variabili[k - 1].typ == NO_GRAF_SEL  ||
                 flag_sel == SEL_LGPH && variabili[k - 1].typlg == NO_GRAF_SEL)
	               &&
                k <= start_ricerca)
	    {
	       start_ricerca = k;
            if ( flag_sel == SEL_REG )
	      variabili[k - 1].typ = NO_GRAF_SEL;
            else
	      variabili[k - 1].typlg = NO_GRAF_SEL;
	       UtListaScroll(widget_array[k_elenco_variabili],
			     lung_non_sel, 6, posizione - 1);
	       XSync(display, False);
	       break;
	    }
	 }
   }
}
void 
cerca_var_indietro()
{
   char      app_str[80];
   char      mom[120];
   int       posizione = 0;
   int       k;
   int       trovata = 0;
   int       num_bl;

   num_bl = numero_blocchi(ind_sh_top, modello_selezionato);

   posizione = lung_non_sel + 1;
   strcpy(app_str, XmTextGetString
	  (widget_array[k_elenco_variabili_text_cerca]));
/* cerca da start_ricerca alla fine */
   for (k = tot_variabili; k >= 1; k--)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
      {
	 strcpy(mom, variabili[k - 1].nome);
	 --posizione;
	 if ( Utstrstr(strcat(mom, variabili[k - 1].descr), app_str) != NULL
                       &&
              ( flag_sel == SEL_REG && variabili[k - 1].typ == NO_GRAF_SEL  ||
                flag_sel == SEL_LGPH && variabili[k - 1].typlg == NO_GRAF_SEL )
	               &&
              k < start_ricerca )
	 {
	    start_ricerca = k;
            if ( flag_sel == SEL_REG )
	      variabili[k - 1].typ = NO_GRAF_SEL;
            else
	      variabili[k - 1].typlg = NO_GRAF_SEL;
	    trovata = 1;
	    UtListaScroll(widget_array[k_elenco_variabili],
			  lung_non_sel, 6, posizione - 1);
	    XSync(display, False);
	    break;
	 }
      }
   if (!trovata)
   {
   /* cerca dall'inizio a start_ricerca  */
      posizione = lung_non_sel + 1;
      for (k = tot_variabili; k >= 1; k--)
      if ( variabili[k - 1].mod == modello_selezionato  &&
	   (  blocco_selezionato == num_bl+1 ||
	      variabili[k - 1].blocco == blocco_selezionato
           )  &&
	   ( flag_sel == SEL_REG && 
             ( variabili[k - 1].typ == NO_GRAF_SEL  ||
	       variabili[k - 1].typ == NO_GRAF_SEL_MOM )
                   ||
	     flag_sel == SEL_LGPH && 
             ( variabili[k - 1].typlg == NO_GRAF_SEL  ||
	       variabili[k - 1].typlg == NO_GRAF_SEL_MOM )
           )
         )
	 {
	    strcpy(mom, variabili[k - 1].nome);
	    --posizione;
	    if ( Utstrstr(strcat(mom, variabili[k - 1].descr), app_str) != NULL 
                       &&
                (flag_sel == SEL_REG && variabili[k - 1].typ == NO_GRAF_SEL  ||
                 flag_sel == SEL_LGPH && variabili[k - 1].typlg == NO_GRAF_SEL)
	               &&
                k >= start_ricerca )
	    {
	       start_ricerca = k;
            if ( flag_sel == SEL_REG )
	      variabili[k - 1].typ = NO_GRAF_SEL;
            else
	      variabili[k - 1].typlg = NO_GRAF_SEL;
	       UtListaScroll(widget_array[k_elenco_variabili],
			     lung_non_sel, 6, posizione - 1);
	       XSync(display, False);
	       break;
	    }
	 }
   }
}
