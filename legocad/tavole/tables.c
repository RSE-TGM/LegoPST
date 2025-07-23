/**********************************************************************
*
*       C Source:               tables.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 10:15:54 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: tables.c,2 %  (%full_filespec: 1,csrc,tables.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)tables.c	1.3\t3/29/95";
/*
        Fine sezione per SCCS
*/

/*  TABLES.C	versione per OSF/Motif delle tavole acqua-vapore ed
		eventuali future funzioni

    Versione 0.1          27.7.92           Buffo & Calleri	    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/SeparatoG.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/BulletinB.h>
#include <Xm/ToggleB.h>

//#include <libutilx.h>

#include  "../../AlgLib/libinclude/libutilx.h"
#include "tables.h"
#include "def_tav.h"
#include "sfondo.px"  /* Bitmap dell'icona dell'applicazione */

/*******************************************/
/* DEFINIZIONE  DELLE  VARIABILI  GLOBALI  */
/*******************************************/

/* VARIABILI UTILIZZATE NELLE FUNZIONI Xlib */
Display	    *display;
Pixmap      icona;

/* VARIABILI DI WIDGET */
Widget top_level, main_window, menubar, form_widget; 
Widget in_out_form, info_risultati;

/* Row-Column delle tre sezioni della finestra: toggle buttons,input,output */
Widget radio_box, input_box, output_box;

/* Separatori (Gadget) */
Widget sep1, sep2;

/* Push-button di visulizzazione risultati e clear dell'input */
Widget w_clear, w_equal;

/* descrizione della funzione selezionata */
Widget titolo_funzione;

Widget atoggles[MAX_TOGGLEB];  /* toggle-buttons relative alle funzioni da */
			       /* attivare */
Widget ainputlabel[MAX_INPUT_VALUE]; /* label descrizione degli input */
Widget ainputtext[MAX_INPUT_VALUE];  /* text di inserimento input */
Widget aoutputlabel[MAX_OUTPUT_VALUE]; /* label relativa alle descrizioni */
				       /* degli output */
Widget aresultlabel[MAX_OUTPUT_VALUE]; /* label dei risultati */
Widget aresultframe[MAX_OUTPUT_VALUE]; /* frame della label risultati */
float ainputval[MAX_INPUT_VALUE]; /* valori inseriti in input */
float aresultval[MAX_INPUT_VALUE]; /* valori inseriti in output */

/* widgets relative alla dialog-box di history */
Widget funct_history, function_list, done;

/* ALTRE VARIABILI GLOBALI */
int toggle_premuto = -1;
int row_count_history = 0; /* conteggia il numero di item nella scroll-list */
			   /* di history */

/* memorizza il tipo di funzioni da calcolare (vapore, fumi, ...)	*/
Struct_toggle *tavola_scelta;
int num_funzioni;    /* numero funzioni della tavola scelta */

/* compound string vuota! */
XmString stringa_nulla;
XmString cstring;

Arg args[20];
Cardinal nargs;

typedef void* Caddr_t;

/**** visualizza_default 
 * per default visualizza tel tavole del vapore
 * con settato il primo toggle button
 ****/

int visualizza_default()
{

/* parto con la window delle tavole attiva */
  selez_tavola(atoggles[0],K_VAPORE,NULL);

}

/********  INIZIO MAIN()  ********/
int main (int argc, char **argv)
{

/* Variabile stringa NULLA di comodo ... */
  stringa_nulla = CREATE_CSTRING("");

printf("argc= %d\n",argc);

/* Inizializzazione del Toolkit */
  top_level = XtInitialize(argv[0], "Tables", NULL,  0, &argc, argv);

  display = XtDisplay(top_level);
printf("display= %d\n",display);
/* Definizione titolo e nome icona */
  nargs=0;
  XtSetArg (args[nargs], XtNiconPixmap,
            XCreateBitmapFromData (display,
            XtScreen(top_level)->root,
            sfondo_bits, sfondo_width, sfondo_height)); nargs++;
  XtSetArg(args[nargs],XmNtitle,WINDOW_TITLE); nargs++;
  XtSetArg(args[nargs],XmNiconName,ICON_NAME); nargs++;
  XtSetValues (top_level,args,nargs);

/* Creazione della main window */
printf("nargs= %d",nargs);
  crea_main_window();

/* Creazione della dialog box FUNCTION HISTORY */
  crea_dialog();

  XtRealizeWidget (top_level);

  visualizza_default();


  XtMainLoop();
}
/********  FINE MAIN()  ********/

/****************************/
/* Funzioni di CALLBACK ... */
/****************************/

/*------------------------------------------------------------------*/
/*
 *** void quit_tables(w, client_data, call_data)
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** Caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
callback di terminazione dell'applicativo */
void quit_tables(w, client_data, call_data)
Widget w;

Caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
  XtCloseDisplay (XtDisplay(w));
  exit(0);
}

/*------------------------------------------------------------------*/
/*
 *** void apri_dialog(w, client_data, call_data)
 *** parametri:
 *** Widget w: identificativo del widget che ha effettuato la callback.
 *** Caddr_t client_data               : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
callback che visualizza la dialog-box di history. */
void apri_dialog(w, client_data, call_data)
Widget w;
Caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   Position x,y;  /* variabili per il geometry management */
   Dimension height;

/* Se la dialog-box e' gia' visualizzata allora riotrna */
   if (XtIsManaged(funct_history))
 	return;

   XtManageChild(funct_history);

/* Posizionamento della dialog-box a seconda della posizione della top-level */
   get_something(top_level,XmNx,(void*) &x);
   get_something(top_level,XmNy,(void*) &y);
   get_something(top_level,XmNheight,(void*) &height);
   nargs=0;
   XtSetArg(args[nargs],XmNx, x); nargs++;
   XtSetArg(args[nargs],XmNy, y+height+40); nargs++;
   XtSetValues(funct_history,args,nargs);

/* Disabilitazione della selezione nella scroll-list */
   XSelectInput(XtDisplay(top_level), XtWindow(function_list),
                ExposureMask);
}

/*------------------------------------------------------------------*/
/*
 *** void svuota_lista(w, client_data, call_data)
 *** Parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** Caddr_t client_data : non utilizzato.
 *** XmAnyCallbackStruct *call_data : non utilizzato. 
Cancella uno ad uno gli elementi dalla lista delle funzioni calcolate. */
void svuota_lista(w, client_data, call_data)
Widget w;
Caddr_t client_data; 
XmAnyCallbackStruct *call_data;
{
   while ( row_count_history-- )
       XmListDeletePos(function_list,0);
}

/*------------------------------------------------------------------*/
/*
 *** void selez_tavola()
 *** parametri:
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int tipo_tavola: tipo di tavola selezionato dalla menubar (acqua/vapore
 ***                  fumi, ...)
 *** XmAnyCallbackStruct *call_data : non utilizzato.
a  seconda della voce scelta dal menu select , visualizza i toggle-button
delle funzioni relative. */
void selez_tavola(w, tipo_tavola, call_data)
Widget w;
int tipo_tavola;
XmAnyCallbackStruct *call_data;
{
   XmToggleButtonCallbackStruct cdata;

   switch (tipo_tavola) {

     case K_VAPORE:    /* funzioni relative alle tavole acqua/vapore */
           tavola_scelta = funzioni_vapore;
           num_funzioni  = num_funz_vapore;
           break;

     case K_GAS:       /* funzioni relative alle tavole dei fumi */
           tavola_scelta = funzioni_gas;
           num_funzioni  = num_funz_gas;
           break;
   }

/* Visualizzazione dei toggle-button delle funzioni */
   display_toggle_function();

   set_something_val(atoggles[0],XmNset, (XtArgVal) True);
   cdata.set=1;
   toggle_on(atoggles[0],0,&cdata);
}

/*----------------------------------------------------------------------*/
/*
 *** void toggle_on()
 *** parametri :
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int indice : numero del toggle-button settato.
 *** XmToggleButtonCallbackStruct *call_data: informazioni sul toggle-button
Callback che viene chiamata quando viene premuto un toggle-button.
Viene utilizzata solo quando il toggle-button viene settato per aggiornare
l'elenco degli input e degli output relativi alla funzione scelta */
void toggle_on( w, indice, call_data)
Widget w;
int indice;
XmToggleButtonCallbackStruct *call_data;
{
   int i;

/* Se il toggle-button non e' settato allora ritorna */
   if ( !call_data->set )
      return;

/* aggiorna la variabile globale relativa al toggle-button premuto */
   toggle_premuto = indice;

/* Cancella le row-column di input ed output */
   if ( in_out_form != NULL )
      XtDestroyWidget( in_out_form );

/* Creo la nuova form relativa agli ingressi ed uscite della funzione scelta */
   crea_input_output();
}

/*----------------------------------------------------------------------*/

/*
 *** void next_text(w, indice, call_data)
 *** parametri :
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int indice : indice nell'array ainputtext[] relativo al text che ha
 ***              l'input-focus
 *** XmAnyCallbackStruct *call_data : non utilizzato.
Dovrebbe passare l'input-focus al successivo text widget di input ... */ 
void next_text(w, indice, call_data)
Widget w;
int indice;
XmAnyCallbackStruct *call_data;
{
   if ( indice+1 >= tavola_scelta[ toggle_premuto ].num_input )
      XtSetKeyboardFocus (input_box, ainputtext[0]);
   else
      XtSetKeyboardFocus (input_box, ainputtext[indice+1]);
}

/*----------------------------------------------------------------------*/
/*
 *** void text_verify(w, indice, call_data)
 *** parametri :
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** int indice : indice nell'array ainputtext[] relativo al text che ha 
 ***              l'input-focus
 *** XmTextVerifyCallbackStruct *call_data : informazione sul text-widget
controlla i caratteri che vengono inseriti nel widget text. Ovviamente,
trattandosi esclusivamente di dati numerici (reali o interi) , e'
previsto solo l'inserimento di caratteri numerici (per gli altri caratteri
un sonoro BEEP avverte che il carattere e' indesiderato!). */
void text_verify(w, indice, call_data)
Widget w;
int indice;
XmTextVerifyCallbackStruct *call_data;
{
   char *txt_da_inserire, *testo, *ptr;
   char annulla;
   int  lungh, lungh_testo, i;

/* recupera alcuni dati dalla struttura di informazioni del widget text: */

/* testo inserito fino ad adesso */
   testo = XmTextGetString(ainputtext[indice]);

/* testo da inserire (quindi da controllare ... ) */
   txt_da_inserire = call_data->text->ptr;

/* numero di caratteri da inserire */
   lungh = call_data->text->length;

/* flag di non accettazione dei caratteri da inserire. */
   annulla = 0;

/* esamina il testo da inserire carattere x carattere */
   while ( lungh-- && !annulla )
      switch (*(txt_da_inserire + lungh)) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': break;    /* OK. */

        case '.': if ( Utstrstr(testo,".") != NULL)
                     annulla = 1;  /* Se il . e' gia' stato inserito allora */
				   /* annulla l'inserimento */
                  break;

        case 'e':
        case 'E': if ( Utstrstr(testo,"e") != NULL )
                     annulla = 1;  /* Se la e (esponenziale) e' gia' stata */
                                   /* inserito allora annulla l'inserimento */
                  break;

        case '+':
        case '-': break;  /* OK. (o quasi... si dovrebbe effettuare un */
			  /* controllo un po' piu' complesso degli altri! ) */

        default : annulla = 1; /* qualsiasi altro carattere non e' da */
			       /* considerare! */
                  break;
      }

      if (annulla)
         call_data->doit = False;  /* testo non accettato! */
      else
         call_data->doit = True;  /* testo accettato! */

      XtFree(testo);
}

/*----------------------------------------------------------------------*/
/*
 *** void clear_input( w, client_data, call_data)
 *** parametri :
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** client_data : non utilizzato.
 *** call_data : non utilizzato.
funzione per la cancellazione dei dati inseriti nei widget text di input */
void clear_input( w, client_data, call_data)
Widget w;
Caddr_t client_data;
XmAnyCallbackStruct *call_data;
{
   int i;

/* Per ogni widget-text di input viene cancellato il suo contenuto. */
   for (i=0 ; i<tavola_scelta[ toggle_premuto ].num_input ; i++)
   {
       XmTextSetString( ainputtext[i], "0" );
       ainputval[i] = 0.0;
   }

/* Per ogni label dei risultati viene cancellato il suo contenuto. */
   for (i=0 ; i<tavola_scelta[ toggle_premuto ].num_output ; i++)
       set_something_val( aresultlabel[i],  XmNlabelString, (XtArgVal) stringa_nulla );
}

/*----------------------------------------------------------------------*/
/*
 *** void display_results( w, client_data, call_data)
 *** parametri :
 *** Widget w : identificativo del widget che ha effettuato la callback
 *** client_data : non utilizzato.
 *** call_data : non utilizzato.
funzione che attiva il calcolo della funzione scelta: legge i valori di
input dai widget text relativi, chiama la funzione dell'utente, visualizza
i risultati ed aggiorna la scroll-list di history. */
void display_results( w, client_data, call_data)
Widget w;
Caddr_t client_data;
XmAnyCallbackStruct *call_data;
{
   char *str_val, stringa[50];
   int i;

/* legge l'input dai text e lo converte in double */
   for (i=0 ; i<tavola_scelta[toggle_premuto].num_input ; i++)
   {
      str_val = XmTextGetString(ainputtext[i]);
      sscanf(str_val,"%f", &ainputval[i]);
   }

/* Chiama la funzione definita dal programmatore/utente nella struttura */
   tavola_scelta[toggle_premuto].attiva_calcolo 
                                 (tavola_scelta[toggle_premuto].param);

/* Visualizzazione dei risultati ottenuti */
   for (i=0 ; i<tavola_scelta[toggle_premuto].num_output ; i++)
   {
      sprintf(stringa,"% 8.4g", aresultval[i]);
      cstring = CREATE_CSTRING(stringa);
      nargs = 0;
      XtSetArg(args[nargs], XmNlabelString, cstring); nargs++;
      XtSetArg(args[nargs], XmNwidth, 100); nargs++;
      XtSetValues(aresultlabel[i], args, nargs);
      XmStringFree(cstring);
   } 

/* Inserimento dei risultati nella scroll-list di history */
   update_history (toggle_premuto,tavola_scelta);
}

/*********************************/
/* ALTRE FUNZIONI ...		 */
/*********************************/

/*------------------------------------------------------------------*/
/*
 *** void update_history(int num, Struct_toggle funzione[])
 *** parametri :
 *** int num: numero del toggle-button premuto nella main window
 *** Struct_toggle funzione: tipo di tavola di calcolo a cui accedere
 ***			     (vapore, fumi, ...)
Inserisce nella scroll-list dell'history dialog la funzione associata
al toggle-button premuto */
void update_history(num,funzione)
int num;
Struct_toggle funzione[];
{
   char stringa[100];  /* stringa da convertire in compound-string */
   short int i;

/* Composizione della stringa da inserire nella scroll-list di history */
/* inserimento del nome della funzione nella stringa */
   strcpy(stringa,funzione[num].nome_funzione);

   for (i=strlen(stringa) ; i<12 ; i++)
       strcat(stringa," ");

/* inserimento dei dati di input nella stringa */
   for (i=0 ; i<funzione[num].num_input ; i++)
   {
       strcat(stringa,funzione[num].sigle_input[i]);
       strcat(stringa," = ");
       sprintf(stringa+strlen(stringa),"%8.5e",ainputval[i]);
       strcat(stringa,"  ");
   }
   strcat(stringa," ");

/* inserimento dei risultati nella stringa */
   for (i=0 ; i<funzione[num].num_output ; i++)
   {
       strcat(stringa,funzione[num].sigle_output[i]);
       strcat(stringa," = ");
       sprintf(stringa+strlen(stringa),"%8.5e",aresultval[i]);
       strcat(stringa,"  ");
   }

/* Aggiornamento del numero di compound strings */
   if (row_count_history > MAX_ROW_HISTORY)
      XmListDeletePos(function_list,1);
   else
      row_count_history++;

/* Inserimento della stringa nella scroll-list */
   cstring = CREATE_CSTRING(stringa);
   XmListAddItemUnselected(function_list,cstring,0);
   XmListSetBottomPos(function_list,0);
   set_something_val(function_list,XmNvisibleItemCount, (XtArgVal) VISIBLE_ITEMS);
   XmStringFree(cstring);
}

/*------------------------------------------------------------------*/
/*
 *** void display_toggle_function()
visualizzazione dei toggle-button delle funzioni relativi alla tavola
scelta */
void display_toggle_function()
{
/* Elimina le row-column se sono state create in precedenza */
   if ( radio_box != NULL )
   {
      XtDestroyWidget( radio_box );
      radio_box = NULL;
   }

/* Cancella le row-column di input ed output */
   if ( in_out_form != NULL )
   {
      XtDestroyWidget( in_out_form );
      in_out_form = NULL;
   }

   if ( XtIsManaged(form_widget) )
      XtUnmanageChild(form_widget);

/* Visualizzazione dei toggle-buttons delle funzioni */
   crea_elenco_funzioni();

   XtManageChild(form_widget);
}

/*----------------------------------------------------------------------*/
/*** void chiama_tavH2O( num_funzione )
 *** Parametro:
 ***           num_funzione : numero della funzione da attivare.
funzione per il calcolo delle tavole acqua/vapore. Chiama la funzione calcola
del fortran. 
 N.B: GLI ARGOMENTI DEVONO ESSERE PUNTATORI OPPURE SCALARI
      CON L'OPERATORE DI INDIRIZZO */
void chiama_tavH2O( num_funzione )
int num_funzione;
{
   int iret=0;

   calcola_( &num_funzione, &ainputval[0], &ainputval[1], 
             &ainputval[2],
	     &iret, &aresultval[0],&aresultval[1] );

   if(iret)
   {
     XBell (display,50);  /* N.B. 100 = massimo volume */
     cstring = CREATE_CSTRING(mesg_errore(K_VAPORE, iret));
     set_something_val( info_risultati, XmNlabelString, (XtArgVal) cstring);
     XmStringFree(cstring);
   }
   else
     set_something_val( info_risultati, XmNlabelString, (XtArgVal) stringa_nulla );
}

/*
 *** mesg_errore( tavola, codice )
 *** parametri:
 ***    tavola : tavola di riferimento per i messaggi di errore
 ***    codice : codice dell'errore
ritorna una stringa inerente al codice di errore passato */
char *mesg_errore( tavola, codice )
int tavola, codice;
{
   char *messaggio;

   switch (tavola) {
        case K_VAPORE:
             if  ( codice == 1)
                messaggio = "RESULTS OUT OF H2O/STEAM TABLES";
             break;

        case K_GAS:
             break;
   }
   return(messaggio);
}

/* fine tables.c */
