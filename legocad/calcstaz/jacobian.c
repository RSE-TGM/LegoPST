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
static char SccsID[] = "@(#)jacobian.c	1.7\t3/30/95";
/*
        Fine sezione per SCCS
*/

/*** FILE jacobian.c ***/
/* Buffo Massimo , Calleri Nicola 2/7/1992 */

/* il file contiene i moduli che gestiscono la diagnostica relativa alla */
/* matrice jacobiana */

#include <stdio.h>
#include <fcntl.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Mrm/MrmAppl.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/Label.h>
#include <Xm/CascadeB.h>
#include <Xm/BulletinB.h>
#include <Xm/ScrolledW.h>
#include <Xm/SeparatoG.h>

#include "calcstaz.h"
#include "libutilx.h"
#include "f03.h"
#include "f11.h"
#include "messaggi.h"

extern char *error_mesg[];

extern VOCE_MENU jacobiano_non_sing[];
extern VOCE_MENU jacobiano_solo_sing[];
extern VOCE_MENU jacobiano_sing_righe[];
extern VOCE_MENU jacobiano_sing_col[];
extern VOCE_MENU analisi_jacobiano_si[];
extern short numero_voci_jacobiano;

extern Display *display;
extern XFontStruct *finfo;
extern XmFontList fontList;
extern char *font_name; 

extern XmString *elenco_item;
extern int numero_item;

extern int fd_f11;
extern long int offset_ris_f11;

extern MrmHierarchy s_RMHierarchy;
extern MrmType *dummy_class;

extern Widget toplevel_widget;
extern Widget info_widget;
extern Widget widget_array[];

extern int 	iterazione_selezionata;

extern char	jac_singolare;
extern int	n_null_row, n_null_column, ar_null_row[] , ar_null_column[];

extern char     *int_colonna_jac, *riga_jac;
extern XmString *int_riga_jac;

extern Dialog_geometry geom_information;  /* Per Dialog di informazione
					     sullo jacobiano		*/

extern void free_array_XmString( XmString, int );

/*-----------------------------------------------------------------------*/
/*** show_jacobian( w, client_data, call_data)
 ***
funzione di analisi dello jacobiano  */
void show_jacobian (w, client_data, call_data)
Widget w;
int *client_data;
XmAnyCallbackStruct *call_data;
{
   Widget sbar_values_hor, sbar_values_vert;
   Widget sbar_rows_hor, sbar_rows_vert;
   Widget sbar_cols_hor, sbar_cols_vert;
   char   temp[150];
   XmString intestazione[1];
   int row, col;
   Arg args[15];
   int argcount, incr, i;

   def_cursore (widget_array[K_JACOBIAN_WINDOW], OROLOGIO); 
   switch (*client_data) {
/*************************************** show_jacobian  ****************/
       case K_JACOBIAN_BLOCKS_WINDOW:
/***********************************************************************/

         if (widget_array[ K_JACOBIAN_BLOCKS_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "jacobian_blocks_dialog",
                                toplevel_widget,
                                &widget_array[K_JACOBIAN_BLOCKS_WINDOW],
                                &dummy_class) != MrmSUCCESS )
                s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         get_something( widget_array[K_JACOBIAN_VALUES_LIST],
                         XmNverticalScrollBar, (void*) &sbar_values_vert );
         get_something( widget_array[K_JACOBIAN_VALUES_LIST],
                         XmNhorizontalScrollBar, (void*) &sbar_values_hor );
         get_something( widget_array[K_JACOBIAN_ROWS_LIST],
                         XmNverticalScrollBar, (void*) &sbar_rows_vert );
         get_something( widget_array[K_JACOBIAN_ROWS_LIST],
                         XmNhorizontalScrollBar, (void*) &sbar_rows_hor );
         get_something( widget_array[K_JACOBIAN_COLS_LIST],
                         XmNverticalScrollBar, (void*) &sbar_cols_vert );
         get_something( widget_array[K_JACOBIAN_COLS_LIST],
                         XmNhorizontalScrollBar, (void*) &sbar_cols_hor );

         XtAddCallback(sbar_values_vert, XmNincrementCallback, increment,
                       sbar_rows_vert);
         XtAddCallback(sbar_values_vert, XmNdecrementCallback, increment,
                       sbar_rows_vert);
         XtAddCallback(sbar_values_vert, XmNdragCallback, increment,
                       sbar_rows_vert);
         XtAddCallback(sbar_values_vert, XmNpageIncrementCallback, increment,
                       sbar_rows_vert);
         XtAddCallback(sbar_values_vert, XmNpageDecrementCallback, increment,
                       sbar_rows_vert);

         XtAddCallback(sbar_values_hor, XmNincrementCallback, increment,
                       sbar_cols_hor);
         XtAddCallback(sbar_values_hor, XmNdecrementCallback, increment,
                       sbar_cols_hor);
         XtAddCallback(sbar_values_hor, XmNdragCallback, increment,
                       sbar_cols_hor);
         XtAddCallback(sbar_values_hor, XmNpageIncrementCallback, increment,
                       sbar_cols_hor);
         XtAddCallback(sbar_values_hor, XmNpageDecrementCallback, increment,
                       sbar_cols_hor);

         XtManageChild( widget_array[K_JACOBIAN_BLOCKS_WINDOW] );

         XtSetKeyboardFocus (widget_array[K_JACOBIAN_BLOCKS_WINDOW],
                             widget_array[K_JACOBIAN_BLOCKS_LIST]);

	/* Passaggio del TITOLO alla finestra */
         sprintf(temp,"BLOCK BY BLOCK JACOBIAN MATRIX AT ITERATION # %d",
		  iterazione_selezionata);
         set_something_val( widget_array[*client_data],
                        XmNdialogTitle, (XtArgVal) CREATE_CSTRING(temp));

         numero_item = nbl+1;

      /* Alloca la memoria */
         alloca_memoria_x_items(numero_item);

      /* Primo elemento della lista: selezione di tutti i blocchi */
         strcpy(temp,"********  ALL MODEL BLOCKS");
         elenco_item[ 0 ] = CREATE_CSTRING(temp);

      /* Lettura blocchi dal vettore nom_bloc letto dal file F03.DAT */
         for ( i=1 ; i<=nbl ; i++ )
         {
            sprintf(temp, SF_JACOBIAN_BLOCKS_LIST, nom_bloc[i-1]);
            elenco_item[ i ] = CREATE_CSTRING(temp);
         }

      /* Aggiorna la scrolled list con la lista dei blocchi */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++;
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetValues (widget_array[K_JACOBIAN_BLOCKS_LIST], args, argcount);

      /* Disabilito la selezione nelle tre-finestre-tre  */
         XSelectInput(display, XtWindow(widget_array[K_JACOBIAN_VALUES_LIST]),
                ExposureMask);
         XSelectInput(display, XtWindow(widget_array[K_JACOBIAN_ROWS_LIST]),
                ExposureMask);
         XSelectInput(display, XtWindow(widget_array[K_JACOBIAN_COLS_LIST]),
                ExposureMask);

         XtUnmapWidget( sbar_cols_hor );
         XtUnmapWidget( sbar_cols_vert );
         XtUnmapWidget( sbar_rows_hor );
         XtUnmapWidget( sbar_rows_vert );

         free_array_XmString(elenco_item, numero_item ); 
         XtFree(elenco_item);

       break;

/*************************************** show_jacobian  ****************/
       case K_JACOBIAN_VARIABLES_WINDOW:
/***********************************************************************/
         if (widget_array[ K_JACOBIAN_VARIABLES_WINDOW ] == NULL )
            if (MrmFetchWidget(s_RMHierarchy, "jacobian_variables_dialog",
                               toplevel_widget,
                               &widget_array[K_JACOBIAN_VARIABLES_WINDOW],
                               &dummy_class) != MrmSUCCESS )
               s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[K_JACOBIAN_VARIABLES_WINDOW] );

         XtSetKeyboardFocus (widget_array[K_JACOBIAN_VARIABLES_WINDOW],
                             widget_array[K_JACOBIAN_VARIABLES_VAR_LIST]);

      /* Passaggio del TITOLO alla finestra */
         sprintf(temp,"JACOBIAN VALUES OF SYSTEM VARIABLES AT ITERATION # %d",
		  iterazione_selezionata);
         set_something_val( widget_array[*client_data],
                        XmNdialogTitle, (XtArgVal) CREATE_CSTRING(temp));
        
         numero_item = neqsis;

      /* Alloca la memoria */
         alloca_memoria_x_items(numero_item);

      /* inserimento delle variabili di sistema nella scrolled-list */
         for( i=0; i<neqsis ; i++ )
         {
            int i_var;

            i_var = ipvrs[ ipvrt_f11[ ips_f11[i]-1 ]-1 ];
            if ( i_var >= 0 )
               sprintf(temp, SF_JACOBIAN_VARIABLES_VAR_LIST,
                       nom_sivar[i_var-1]);
            else
               sprintf(temp, SF_JACOBIAN_VARIABLES_VAR_LIST,
                       nom_vari[-i_var-1]);

            elenco_item[i] = CREATE_CSTRING(temp);
         }

      /* Inserisci i dati nella scrolled-list delle variabili */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, numero_item); argcount++;
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
         XtSetValues (widget_array[K_JACOBIAN_VARIABLES_VAR_LIST],
                      args, argcount);
         free_array_XmString( elenco_item, numero_item );
         XtFree(elenco_item);
       break;

/*************************************** show_jacobian *****************/
       case K_JAC_SING_INFO_WINDOW:
/***********************************************************************/
/*     INFORMATION DIALOG CREATA DA 'C'     */
        if (abs(iflag) == 1)
          strcpy(temp,"The jacobian matrix is singular in its structure.");
       else
          strcpy(temp,"The jacobian matrix is singular in its coefficients.");

/*  RIGHE E/O COLONNE NULLE...  */
       if (n_null_row  && n_null_column)
          strcat(temp,"\nIt contains also null rows and columns.");
       else if (n_null_row && !n_null_column)
          strcat(temp,"\nIt contains also null rows.");
       else if (!n_null_row && n_null_column)
          strcat(temp,"\nIt contains also null columns.");
       else   /*  PER DIMENSIONARE LA DIALOG BOX COME NEGLI ALTRI CASI... */
          strcat(temp,"\n                              ");

       info_widget=information(toplevel_widget,temp,MAPPA,geom_information);
       break;

/*************************************** show_jacobian *****************/
       case K_JAC_SING_ROW_WINDOW:
/***********************************************************************/
          if (widget_array[ K_JAC_SING_ROW_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "jacobian_sing_row_dialog",
                                toplevel_widget,
                                &widget_array[K_JAC_SING_ROW_WINDOW],
                                &dummy_class) != MrmSUCCESS )
               s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

          XtManageChild( widget_array[K_JAC_SING_ROW_WINDOW] );

         XtSetKeyboardFocus (widget_array[K_JAC_SING_ROW_WINDOW],
                             widget_array[K_JAC_SING_ROW_LIST]);

	/* Passaggio del TITOLO alla finestra */
         sprintf(temp,"NULL ROWS LIST AT ITERATION # %d",
		  iterazione_selezionata);
         set_something_val( widget_array[*client_data],
                        XmNdialogTitle, (XtArgVal) CREATE_CSTRING(temp));

      /* Alloca la memoria */
         alloca_memoria_x_items(n_null_row);

      /* lettura dell'array che contiene le righe nulle */
         for( i=0 ; i<n_null_row ; i++ )
         {
            int k;

            k = ar_null_row[i];
            sprintf(temp, SF_JAC_SING_ROW_LIST, k,nom_bloc[ iresbl[k-1]-1 ]);
            elenco_item[i] = CREATE_CSTRING(temp);
         }

      /* Inserisci i dati nella scrolled-list delle variabili */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, n_null_row); argcount++;
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
         XtSetValues (widget_array[K_JAC_SING_ROW_LIST],
                      args, argcount);
         free_array_XmString( elenco_item, n_null_row );
         XtFree( elenco_item );
       break;

/*************************************** show_jacobian *****************/
       case K_JAC_SING_COL_WINDOW:
/***********************************************************************/
         if (widget_array[ K_JAC_SING_COL_WINDOW ] == NULL )
             if (MrmFetchWidget(s_RMHierarchy, "jacobian_sing_col_dialog",
                                toplevel_widget,
                                &widget_array[K_JAC_SING_COL_WINDOW],
                                &dummy_class) != MrmSUCCESS )
               s_error( APPLICATION_NAME, error_mesg, EFETCHDBOX, 1 );

         XtManageChild( widget_array[K_JAC_SING_COL_WINDOW] );

         XtSetKeyboardFocus (widget_array[K_JAC_SING_COL_WINDOW],
                             widget_array[K_JAC_SING_COL_LIST]);

      /* Passaggio del TITOLO alla finestra */
         sprintf(temp,"NULL COLUMNS LIST AT ITERATION # %d",
		  iterazione_selezionata);
         set_something_val( widget_array[*client_data],
                        XmNdialogTitle, (XtArgVal) CREATE_CSTRING(temp));

      /* Alloca la memoria */
         alloca_memoria_x_items(n_null_column);

      /* lettura dell'array che contiene le colonne nulle */
         for( i=0 ; i<n_null_column ; i++ )
         {
            int k, i_var;

            k = ar_null_column[i];

            i_var = ipvrs[ ipvrt_f11[ ips_f11[k-1]-1 ]-1 ];
            if ( i_var >= 0 )
               sprintf(temp, SF_JAC_SING_COL_LIST, nom_sivar[i_var-1]);
            else
               sprintf(temp, SF_JAC_SING_COL_LIST, nom_vari[-i_var-1]);

            elenco_item[i] = CREATE_CSTRING(temp);
         }

      /* Inserisci i dati nella scrolled-list delle variabili */
         argcount = 0;
         XtSetArg(args[argcount], XmNitemCount, n_null_column); argcount++;
         XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
         XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
         XtSetValues (widget_array[K_JAC_SING_COL_LIST],
                      args, argcount);
         free_array_XmString( elenco_item, n_null_column );
         XtFree( elenco_item );

      /* Scrittura dei messaggi per l'utente sfigato  */
          strcpy (temp,
"The values of the above shown variables couldn't be computed by any equation");
	  strcat (temp,"\nin the whole system.");
	  set_something_val (widget_array[K_JAC_SING_COL_REASON_1],XmNlabelString, (XtArgVal) CREATE_CSTRING(temp));
	  strcpy(temp,"Such variables are likely to be set KNOWN.");
	  set_something_val (widget_array[K_JAC_SING_COL_REASON_2],XmNlabelString, (XtArgVal) CREATE_CSTRING(temp));
       break;
   }
   undef_cursore (widget_array[K_JACOBIAN_WINDOW]);
}

/*-----------------------------------------------------------------------*/
/*
 *** selez_jac_proc()
 *** parametri
 ***   Widget w : identificativo del widget che ha effettuato la callback
 ***   int *param :
 ***   XmListCallbackStruct *list_info : struttura che contiene le informazioni
                        sulla scrolled-list (item selezionato , ecc.. )
 Procedura di callback delle scrolled list relative alle window dello
jacobiano ogni volta che viene effettuata una selezione di un elemento
della lista */
void selez_jac_proc(w, widget_num, list_info)
Widget               w;
int                  *widget_num;
XmListCallbackStruct *list_info;
{
   Cardinal argcount;
   Arg args[10];
   char *item, stringa[100],temp[150];
   int  i, j, k, eq_iniz, eq_fine, eqz, num_blocco;
   XmString intestazione[1];
   int row, col, num_var, *no_var_blocco;
   int unkn;  /* verifica se ALMENO una variabile � incognita --->
		   cambia opportunamente il messaggio all'utente	*/

/* Converti la compound string selezionata in array di caratteri */
   item = extract_string( list_info->item );
   def_cursore (widget_array[*widget_num], OROLOGIO);
   switch ( *widget_num ) {
/***************************************   selez_jac_proc   ****************/
       case K_JACOBIAN_LIST: 
/***************************************************************************/
          sscanf(item,"%4d ",&iterazione_selezionata);
          jac_singolare = item[30];
	  if (jac_singolare == '*')
          {

      /* Se lo jacobiano e' singolare controlla se ci sono righe e/o colonne */
      /* nulle. */

      /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
             lseek(fd_f11, offset_ris_f11, SEEK_SET);
  
      /* ciclo di lettura dei dati */
             while( !read_data_f11( fd_f11 ) && 
    	            (irc11 != C_OK || irc11 != C_NON_OK))
                if ( irc11 == C_RESIDUI || irc11 == C_SOLUZIONI ||
		     irc11 == C_JACOBIANO || irc11 == C_PROBLEMI )
                   if ( itert > iterazione_selezionata )
                      break;

      /* Controllo sulle righe (il vettore irj e' ordinato) */
             j = k = 0;
             for( i=1 ; i <= neqsis ; i++ )
             {
                int mem;

                mem = j;
                while( j<nzter && irj[j] == i) j++;
                if( mem == j )
                   ar_null_row[k++] = i;
             }
             n_null_row = k;

       /* Controllo sulle colonne */
             k = 0;
             for(i=1; i <= neqsis; i++)
             {
                for(j=0; j<nzter; j++)
                   if ( icj[j] == i )
                      break;

                if( j == nzter )
                  ar_null_column[k++] = i;
             }
             n_null_column = k;
             if ( n_null_column && n_null_row )
	        cambia_stato_menu (numero_voci_jacobiano,analisi_jacobiano_si);
             else
                if ( n_null_column )
	           cambia_stato_menu (numero_voci_jacobiano,
                                      jacobiano_sing_col);
                else
                   if ( n_null_row )
	              cambia_stato_menu (numero_voci_jacobiano,
                                      jacobiano_sing_righe);
                   else
	              cambia_stato_menu (numero_voci_jacobiano,
                                      jacobiano_solo_sing);

          }
	  else 
	     cambia_stato_menu (numero_voci_jacobiano,jacobiano_non_sing);
          break;

/***************************************   selez_jac_proc   ************/
       case K_JACOBIAN_BLOCKS_LIST: /* Dialog box matr. jacob. block x block */
/***********************************************************************/
   def_cursore (widget_array[K_JACOBIAN_BLOCKS_WINDOW], OROLOGIO);
            i = list_info->item_position-1;
         /* Se e' stato selezionato ******** ALL MODEL BLOCKS */
            if ( i == 0 ) 
            {
         /* ... allora considera tutte le equazioni */
               eq_iniz = 0;
               eq_fine = neqsis;

         /* ... e tutte le variabili! */
               num_var = neqsis;
               no_var_blocco = (int *) XtCalloc(num_var,sizeof(int));
               for( j=0; j<neqsis; no_var_blocco[ j ] = j++ );
            }
            else
            {
         /* ... altrimenti considera solo le equazioni di quel blocco */
               eq_iniz = cerca_elemento( iresbl, neqsis, i );
               j = eq_iniz;
               if ( eq_iniz != -1 )
               {
                  while( j < neqsis && iresbl[j++] == i );
                  if ( j == neqsis && iresbl[j-1] == i )
                     eq_fine = j;
                  else
                     eq_fine = j-1;

         /* ... e le variabili di sistema di quel blocco! */
               num_var = ip[i]-ip[i-1];

           /* Dimensiona per il numero di variabili del blocco */
           /* (Quindi , comprese quelle note...successivamente */
           /* questo parametro assumera' il valore corretto ). */
               no_var_blocco = (int *) XtCalloc(num_var,sizeof(int));
               for( k=0, j=ip[i-1]-1; j<ip[i]-1; j++ )
                  if ( ipvrs_f11[j] > 0 )
                     no_var_blocco[k++] = ipvrs_f11[j]-1;

          /* k e' il numero delle variabili di sistema contenute nel blocco */
               num_var = k;
               }
            }

       /* Lettura dei dati del calcolo dello stazionario dal file F11.DAT */
            lseek(fd_f11, offset_ris_f11, SEEK_SET);
  
       /* ciclo di lettura dei dati */
            while( !read_data_f11( fd_f11 ) &&
		   (irc11 != C_OK || irc11 != C_NON_OK))
               if ( irc11 == C_RESIDUI || irc11 == C_SOLUZIONI ||
		    irc11 == C_JACOBIANO || irc11 == C_PROBLEMI )
                  if ( itert > iterazione_selezionata )
                     break;

      /* Alloca la memoria */
            alloca_memoria_x_items( eq_fine-eq_iniz );

      /* componi la matrice jacobiana */
            numero_item = 0;
            for( row = eq_iniz, i=0 ; row < eq_fine; row++,i++ )
            {
               strcpy(riga_jac,"");
          
          /* valori della matrice jacobiana */ 
               for( j = 0 ; j < num_var; j++ )
               {
                  char str1[20];

                  col = no_var_blocco[j];
                  sprintf(temp,"%s ",
                          nzero(str1,valore_jacobiano( row+1, col+1),
                                "% 8.4e"," 0."));
                  strcat(riga_jac, temp);
               }

          /* intestazione delle righe della matrice (numero equazione) */
               elenco_item[ i ] = CREATE_CSTRING(riga_jac);
               sprintf(temp,"%3d",i+1);
               int_riga_jac[ i ] = CREATE_CSTRING( temp );
            } 
  
     /* Composizione della stringa di intestazione delle colonne */
     /* (incognite del sistema) */
            strcpy(int_colonna_jac,"");
            for( j = 0 ; j < num_var; j++ )
            {
               col = no_var_blocco[j];
               strcat(int_colonna_jac, "  ");
               app_n_car(int_colonna_jac, varsi[ col ], 8);
               strcat(int_colonna_jac, "  ");
            }
            intestazione[0] = CREATE_CSTRING( int_colonna_jac );
  
      /* Inserisci i dati nella scrolled-list della matrice jacobiana */
            argcount = 0;
            XtSetArg(args[argcount], XmNitemCount, eq_fine-eq_iniz); argcount++;
            XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
            XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
            XtSetValues (widget_array[K_JACOBIAN_VALUES_LIST], args, argcount);
  
      /* Inserisci i dati nella scrolled-list intestazione della colonna */
            argcount = 0;
            XtSetArg(args[argcount], XmNitemCount, eq_fine-eq_iniz); argcount++;
            XtSetArg(args[argcount], XmNitems, int_riga_jac ); argcount++;
            XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
            XtSetValues (widget_array[K_JACOBIAN_ROWS_LIST], args, argcount);
  
            argcount = 0;
            XtSetArg(args[argcount], XmNitemCount, 1); argcount++;
            XtSetArg(args[argcount], XmNitems, intestazione );
            argcount++;
            XtSetArg(args[argcount], XmNvisibleItemCount ,1); argcount++;
            XtSetValues (widget_array[K_JACOBIAN_COLS_LIST], args, argcount); 
  
            XmStringFree(intestazione[0]);
            free_array_XmString( elenco_item, eq_fine-eq_iniz );
            XtFree(no_var_blocco);
            XtFree( elenco_item );
            numero_item = 0;
            undef_cursore (widget_array[K_JACOBIAN_BLOCKS_WINDOW]);
            break; 

/***************************************   selez_jac_proc   ************/
       case K_JACOBIAN_VARIABLES_VAR_LIST: /* elenco var. */
/***********************************************************************/
            def_cursore (widget_array[K_JACOBIAN_VARIABLES_WINDOW], OROLOGIO);
        /* Il numero della variabile nella lista e' anche l'ordine */
        /* in cui compare nel vettore varsi! */
            i = list_info->item_position;

        /* ciclo di lettura dei dati */
            while( !read_data_f11( fd_f11 ) &&
                   (irc11 != C_OK || irc11 != C_NON_OK))
               if ( irc11 == C_RESIDUI || irc11 == C_SOLUZIONI ||
	            irc11 == C_JACOBIANO || irc11 == C_PROBLEMI )
                  if ( itert > iterazione_selezionata )
                     break;

        /* Alloca la memoria */
            alloca_memoria_x_items(neqsis);

        /* lettura dei dati da inserire nella scrolled-list */
            k = 0;
            for( j=0 ; j<neqsis ; j++ )
            {
               double val;

               if ( (val=valore_jacobiano(j+1, i)) != 0. )
               {
                  char str1[20];

                  sprintf(temp, SF_JACOBIAN_VARIABLES_EQ_LIST,
			  j+1, noblc[ iresbl[j]-1 ],
                          nzero(str1,valore_jacobiano( j+1, i),"% 8.4e"," 0."));
                  elenco_item[ k++ ] = CREATE_CSTRING(temp);
               }
            } 

            argcount = 0;
            XtSetArg(args[argcount], XmNitemCount, k); argcount++;
            XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
            XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
            XtSetValues (widget_array[K_JACOBIAN_VARIABLES_EQ_LIST], args, 
                         argcount); 
  
            free_array_XmString( elenco_item, k );
            XtFree( elenco_item );
            undef_cursore (widget_array[K_JACOBIAN_VARIABLES_WINDOW]);
            break;

/***************************************   selez_jac_proc   **************/
       case K_JAC_SING_ROW_LIST: /* elenco var. coinvolte nell'equazione */
/*************************************************************************/
            def_cursore (widget_array[K_JAC_SING_ROW_WINDOW], OROLOGIO);
         /* Leggi il numero dell'equazione di sitema selezionato */
            sscanf(item,"%5d",&eqz);
            num_blocco = iresbl[ eqz-1 ];

         /* Alloca la memoria */
            alloca_memoria_x_items(ip[num_blocco]-ip[num_blocco-1]);

            for(i=0 ; i < ip[num_blocco]-ip[num_blocco-1] ; i++ )
            {
               if ( itopva[eqz-1][i] )
               {
		  int num_var;

                  num_var = ip[num_blocco-1]+i;
                  if( ipvrs_f11[ num_var-1 ] >= 0 )
		  {
                    strcpy(temp," ");
		    unkn = TRUE;
		  }
                  else
                    strcpy(temp,"*");

                  if ( ipvrs[ num_var-1 ] >= 0 )
                     sprintf(temp, SF_JAC_SING_ROW_VAR_LIST,
                             nom_sivar[ ipvrs[ num_var-1 ]-1]);
                  else
                     sprintf(temp, SF_JAC_SING_ROW_VAR_LIST,
                               nom_vari[ -ipvrs[ num_var-1 ]-1]);

                  elenco_item[ i ] = CREATE_CSTRING(temp);
               }
            }
	    if (unkn) 
		strcpy (temp,
		     "One of the above shown variables might be set UNKNOWN.");
	    else
		strcpy (temp,
	     "At least one of the above shown variables must be set UNKNOWN.");
	    set_something_val(widget_array[K_JAC_SING_ROW_REASON],XmNlabelString, (XtArgVal) CREATE_CSTRING(temp));

            argcount = 0;
            XtSetArg(args[argcount], XmNitemCount, i); argcount++;
            XtSetArg(args[argcount], XmNitems, elenco_item ); argcount++;
            XtSetArg(args[argcount], XmNvisibleItemCount ,10); argcount++;
            XtSetValues (widget_array[K_JAC_SING_ROW_VAR_LIST], args, 
                         argcount); 
  
            free_array_XmString( elenco_item, i );
            XtFree( elenco_item );
            undef_cursore (widget_array[K_JAC_SING_ROW_WINDOW]);
            break;
   } 
   undef_cursore (widget_array[*widget_num]);
} 

/*-----------------------------------------------------------------------*/
void increment(w, dato, list_info)
Widget w;
Widget dato;
XmScrollBarCallbackStruct *list_info;
{
    static int i=0;

    if ( !i )
    {
       i++;
       XtCallCallbacks(dato, XmNincrementCallback, list_info);
       i = 0;
    }
    else
    {
       i = 0;
       return;
    }
}

/*-----------------------------------------------------------------------*/
/*** funzione valore_jacobiano( row, col )
 ***
Data la coppia di valori che identifica l'elemento della matrice jacobiana
la funzione restituisce 0 se il valore non e' stato trovato , altrimenti
ritorna il valore */
double valore_jacobiano( row, col )
int row, col;
{
   int i=0;

   if ( (i = cerca_elemento( irj, nzter, row )) == -1)
      return((double) 0);

   for ( ; i < nzter && irj[i] == row ; i++)
      if ( icj[i] == col )
         return( (double) fj[i] );

   return((double) 0);
}

/***  FINE FILE jacobian.c  ***/
