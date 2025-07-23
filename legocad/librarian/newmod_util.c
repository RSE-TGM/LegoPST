/**********************************************************************
*
*       C Source:               newmod_util.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Mar  8 19:42:38 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: newmod_util.c-2 %  (%full_filespec: newmod_util.c-2:csrc:1 %)";
#endif
/*
   modulo newmod_util.c
   tipo 
   release 2.25
   data 2/6/96
   reserved @(#)newmod_util.c	2.25
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)newmod_util.c	2.25\t2/6/96";
/*
        Fine sezione per SCCS
*/

/*** File newmod_util.c ( o funzioni ausiliarie per librarian )

 ***     Creato da Buffolo Bill in data 19/10/1993

 Il file contiene le funzioni utilizzate nella NEWMOD di librarian
 ( voce Call NEWMOD ).

 Tali funzioni sono di supporto all'interfaccia UIMX o AIC "nmod_app_shell".

 In fede ... Buffo & Calleri

 ***/

/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/Label.h>

#include "UxXt.h"

#include <libutilx.h>
#include <file_icone.h>

#include "definizioni.h"

/****************************************************************/
/* VARIABILI GLOBALI ESTERNE					*/
/****************************************************************/

/* font per i Text-Widget (Courier-Bold-14) */
extern XmFontList fontList;

extern int num_yes_toggle;

/* Variabili globali per dimensionare la matrice jacobiana */
/* (VALORI DI DEFAULT) */
extern int num_var_stato;
extern int num_var_algebriche;
extern int num_var_ingresso;
extern int num_dati_geometrici;
extern int jac_rows, jac_cols;

/* variabili utilizzate per memorizzare i text delle window relative */
/* alle subroutine I2,JC,D1 e residui (NEWMOD) */
extern char *str_dichI2, *str_codI2;
extern char *str_dichJC, *str_codJC;
extern char *str_dichMOD, *str_codMOD,*str_resMOD[MAX_EQUAZIONI];
extern char *str_signeq[MAX_EQUAZIONI], *str_uniteq[MAX_EQUAZIONI],
            *str_cosnor[MAX_EQUAZIONI];

extern JacToggStruct jactoggstruct[MAX_EQUAZIONI][MAX_VARIABILI_IN_OUT];
extern byte jacobian_type;

extern Boolean bool_dialog_config;
extern Boolean bool_dialog_equation;
extern Boolean nmod_def_managed;
extern Boolean nmod_def_initialized;

/* flag che indica la modifica di un modulo creato con la NEWMOD */
/* ( Call NEWMOD -> Modify ) */
extern Boolean modifica_newmod;

extern swidget nmod_bboard;
extern swidget nmod_label_tit1, nmod_label_tit2, nmod_label_tit3;

extern Widget scrollw_label1, scrollw_label2, scrollw_label3;

extern int option_scelto;
extern int posy_text;

extern Arg args[20];
extern Cardinal argcount;

extern Widget UxTopLevel;
extern Dialog_geometry geom_attention;
extern Widget attention_wdg;
extern Widget dialog_setup;

/****************************************************************/
/* VARIABILI GLOBALI 						*/
/****************************************************************/

/* Text-widget relativi alle variabili ed ai dati geometrici */
WdgVariabili *wvar_stato, *wvar_algebriche, *wvar_ingresso;
WdgDatiGeometrici *wdati_geometrici;

/* variabili utilizzate nella creazione di un modulo (NEWMOD) */
StructVars *variabili;
StructDati *dati_geom;

char *nome_nuovo_modulo, *descr_nuovo_modulo;

/***************************************************************/
/*** cambia_tipo_text(bboard, tipo, flag)
 ***   Parametri:
 ***     Widget bboard : bullettin-board 
 ***     int tipo : tipo text-widget (ingresso, uscita, stato, dati, ... )
 ***     Boolean flag : se True indica che bisogna fare l'unmanage dei widget
 ***                    presenti nella scrolled-window
 ***
 ***   Descrizione:
 ***     Cambia il tipo di text-widget da visualizzare nella scrolled-window.
 ***     Viene fatto riferimento alla variabile globale option_scelto. 
 ***/
cambia_tipo_text(bboard, tipo, flag)
Widget bboard;
int tipo;
Boolean flag;
{
   if (flag)
   {
      if (option_scelto == tipo)
         return;

      if (option_scelto == K_OPT_ALL)
      {
         XtDestroyWidget(scrollw_label1);
         XtDestroyWidget(scrollw_label2);
         XtDestroyWidget(scrollw_label3);
         load_text_widget(K_OPT_STATO,    UNMANAGE);
         load_text_widget(K_OPT_ALGEBRAIC,UNMANAGE);
         load_text_widget(K_OPT_INGRESSO, UNMANAGE);
      }
      else
         load_text_widget (option_scelto, UNMANAGE);
   }

   if (option_scelto == K_OPT_ALL && scrollw_label1 != NULL)
   {
      XtDestroyWidget(scrollw_label1);
      XtDestroyWidget(scrollw_label2);
      XtDestroyWidget(scrollw_label3);
   }

   option_scelto = tipo;

   if (tipo != K_OPT_DATI)
   {
      set_label(UxGetWidget(nmod_label_tit1), "Description");
      set_label(UxGetWidget(nmod_label_tit2), "FORTRAN Symbol");
      set_label(UxGetWidget(nmod_label_tit3), "Normalized value");
   }
   else
   {
      set_label(UxGetWidget(nmod_label_tit1), "Data symbol");
      set_label(UxGetWidget(nmod_label_tit2), "Fortran variable");
      set_label(UxGetWidget(nmod_label_tit3), "");
   }

   posy_text = 10;

   if (option_scelto == K_OPT_ALL)
   {
      scrollw_label1 = crea_label(bboard, "STATE OUTPUT VARIABLES", 10, 
                                  posy_text);
      posy_text += 25;
      load_text_widget(K_OPT_STATO, MANAGE);
      posy_text += 10;

      scrollw_label2 = crea_label(bboard, "ALGEBRAIC OUTPUT VARIABLES", 10, 
                                  posy_text);
      posy_text += 25;
      load_text_widget(K_OPT_ALGEBRAIC, MANAGE);
      posy_text += 10;

      scrollw_label3 = crea_label(bboard, "INPUT VARIABLES", 10, posy_text);
      posy_text += 25;
      load_text_widget(K_OPT_INGRESSO, MANAGE);
   }
   else
      load_text_widget(option_scelto, MANAGE);
}

/***************************************************************/
/*** load_text_widget(tipo, op)
 ***   Parametri:
 ***      int tipo : tipo di text da caricare (stato,ingresso,uscite,dati,...)
 ***      int op : operazione da effettuare (MANAGE-UNMANAGE-DESTROY)
 ***
 ***   Descrizione:
 ***      Carica (crea se non sono ancora stati creati) i text widget relativi 
 ***      alla tipologia selezionata dal menu option
 ***/
load_text_widget(tipo, op)
int tipo, op;
{
   WdgVariabili *wdg_eq;
   WdgDatiGeometrici *wdg_dati;
   Boolean *creati;
   int i, num_variabili;

   if (tipo == K_OPT_VOID)
      return;
   
   switch (tipo)
   {
       case K_OPT_STATO:
          wdg_eq = wvar_stato;
          num_variabili = num_var_stato;
          break;

       case K_OPT_ALGEBRAIC:
          wdg_eq = wvar_algebriche;
          num_variabili = num_var_algebriche;
          break;

       case K_OPT_INGRESSO:
          wdg_eq = wvar_ingresso;
          num_variabili = num_var_ingresso;
          break;

       case K_OPT_DATI:
          wdg_dati = wdati_geometrici;
          num_variabili = num_dati_geometrici;
          break;
    }

    switch (op)
    {
       case MANAGE:
          for(i=0 ; i<num_variabili ; i++)
          {
             if (tipo != K_OPT_DATI)
             {
                set_something_val(wdg_eq[i].text_descr, XmNy, (XtArgVal) posy_text);
                set_something_val(wdg_eq[i].text_simbolo, XmNy, (XtArgVal) posy_text);
                set_something_val(wdg_eq[i].text_valore, XmNy, (XtArgVal) posy_text);
             }
             else
             {
                set_something_val(wdg_dati[i].text_dato, XmNy, (XtArgVal) posy_text);
                set_something_val(wdg_dati[i].text_sigla, XmNy, (XtArgVal) posy_text);
             }
             posy_text += 30;
          }

          for(i=0 ; i<num_variabili ; i++)
             if (tipo != K_OPT_DATI)
             {
                XtManageChild(wdg_eq[i].text_descr);
                XtManageChild(wdg_eq[i].text_simbolo);
                XtManageChild(wdg_eq[i].text_valore);
             }
             else
             {
                XtManageChild(wdg_dati[i].text_dato);
                XtManageChild(wdg_dati[i].text_sigla);
             }
          break;

       case UNMANAGE:
          for(i=0 ; i<num_variabili ; i++)
             if (tipo != K_OPT_DATI)
             {
                XtUnmanageChild(wdg_eq[i].text_descr);
                XtUnmanageChild(wdg_eq[i].text_simbolo);
                XtUnmanageChild(wdg_eq[i].text_valore);
             }
             else
             {
                XtUnmanageChild(wdg_dati[i].text_dato);
                XtUnmanageChild(wdg_dati[i].text_sigla);
             }
          break;

       case DESTROY:
          for (i=0 ; i<num_variabili ; i++)
             if (tipo != K_OPT_DATI)
             {
                XtDestroyWidget(wdg_eq[i].text_descr);
                XtDestroyWidget(wdg_eq[i].text_simbolo);
                XtDestroyWidget(wdg_eq[i].text_valore);
             }
             else
             {
                XtDestroyWidget(wdg_dati[i].text_dato);
                XtDestroyWidget(wdg_dati[i].text_sigla);
             }
    }
}

/***************************************************************/
/*** alloca_text_widget(bboard, num_st, num_alg, num_in, num_dati)
 ***   Parametri:
 ***     Widget bboard: widget del bulletin-board (la madre di tutti i 
 ***                    text-widget relativi ai dati geometrici ed alle
 ***                    variabili).
 ***     int num_st   : numero variabili di stato
 ***     int num_alg  : numero variabili di uscita algebriche
 ***     int num_in   : numero variabili di ingresso
 ***     int num_dati : numero dati geometrici
 ***
 ***   Descrizione:
 ***     alloca la memoria per gli array che contengono i text-widget 
 ***/
alloca_text_widget(bboard, num_st, num_alg, num_in, num_dati)
Widget bboard;
int num_st, num_alg, num_in, num_dati;
{
   int i, j;

/* Creazione/distruzione text per variabili di stato */
/* Distruggi i widget in eccedenza */
   for (i=num_st ; i<num_var_stato ; i++)
   {
       XtDestroyWidget(wvar_stato[i].text_descr);
       XtDestroyWidget(wvar_stato[i].text_simbolo);
       XtDestroyWidget(wvar_stato[i].text_valore);
   }
   wvar_stato = (WdgVariabili *) XtRealloc(wvar_stato,
					  num_st*sizeof(WdgVariabili));
   for (i=num_var_stato ; i<num_st ; i++)
   {
       wvar_stato[i].text_descr = crea_text(bboard, 10,  posy_text, 240, 50);
       wvar_stato[i].text_simbolo = crea_text(bboard, 260, posy_text, 70, 4);
       wvar_stato[i].text_valore = crea_text(bboard, 410, posy_text, 90, 6); 
   }

/* Creazione/distruzione text per variabili algebriche */
   for (i=num_alg ; i<num_var_algebriche ; i++)
   {
       XtDestroyWidget(wvar_algebriche[i].text_descr);
       XtDestroyWidget(wvar_algebriche[i].text_simbolo);
       XtDestroyWidget(wvar_algebriche[i].text_valore);
   }  
   wvar_algebriche = (WdgVariabili *) XtRealloc(wvar_algebriche,
					       num_alg*sizeof(WdgVariabili));
   for (i=num_var_algebriche ; i<num_alg ; i++)
   {
      wvar_algebriche[i].text_descr = crea_text(bboard,10,posy_text,240,50);
      wvar_algebriche[i].text_simbolo = crea_text(bboard,260,posy_text,70,4);
      wvar_algebriche[i].text_valore = crea_text(bboard,410,posy_text,90,6);
   }

/* Creazione/distruzione text per variabili di ingresso */
   for (i=num_in ; i<num_var_ingresso ; i++)
   {
       XtDestroyWidget(wvar_ingresso[i].text_descr);
       XtDestroyWidget(wvar_ingresso[i].text_simbolo);
       XtDestroyWidget(wvar_ingresso[i].text_valore);
   }
   wvar_ingresso = (WdgVariabili *) XtRealloc(wvar_ingresso, 
					     num_in*sizeof(WdgVariabili));
   for (i=num_var_ingresso ; i<num_in ; i++)
   {
      wvar_ingresso[i].text_descr = crea_text(bboard,10,posy_text,240,50);
      wvar_ingresso[i].text_simbolo = crea_text(bboard,260,posy_text,70,4);
      wvar_ingresso[i].text_valore = crea_text(bboard,410,posy_text,90,6);
   }

/* Creazione/distruzione text per dati geometrici */
   for (i=num_dati ; i<num_dati_geometrici ; i++)
   {
       XtDestroyWidget(wdati_geometrici[i].text_dato);
       XtDestroyWidget(wdati_geometrici[i].text_sigla);
   }
   wdati_geometrici = (WdgDatiGeometrici *) XtRealloc(wdati_geometrici, 
                                            num_dati*sizeof(WdgDatiGeometrici));
   for (i=num_dati_geometrici ; i<num_dati ; i++)
   {
       wdati_geometrici[i].text_dato = crea_text(bboard,10,posy_text,240,8);
       wdati_geometrici[i].text_sigla = crea_text(bboard,260,posy_text,70,6);
   }

/*** 
   if ( num_st+num_alg > (num_var_stato+num_var_algebriche) || flag)
   {
      jactoggstruct = (JacToggStruct **) XtRealloc(jactoggstruct,
                                 (num_st+num_alg)*sizeof(JacToggStruct *));
      for (i=0 ; i < num_st+num_alg ; i++)
          jactoggstruct[i] = (JacToggStruct *) XtRealloc(jactoggstruct[i],
                              (num_st+num_alg+num_in)*sizeof(JacToggStruct));
      printf("alloco jactoggstruct [%2d][%2d]\n", 
              num_st+num_alg, num_st+num_alg+num_in);
   }
***/

/* Aggiornamento di tutte le variabili utilizzate per i dimensionamenti */
   num_var_stato = num_st;
   num_var_algebriche = num_alg;
   num_var_ingresso = num_in;
   num_dati_geometrici = num_dati;
   jac_rows = num_var_stato + num_var_algebriche;
   jac_cols = num_var_stato + num_var_algebriche + num_var_ingresso;

/* Allocazione memoria per i text relativi ai residui ed al significato */
/* delle equazioni */
/***** SURGELATO ...
   str_resMOD = (char **) XtRealloc(str_resMOD, jac_rows*sizeof(char *));
   for ( i=0 ; i<jac_rows ; i++ ) str_resMOD[i] = NULL;
   str_signeq = (char **) XtRealloc(str_signeq, jac_rows*sizeof(char *));
   for ( i=0 ; i<jac_rows ; i++ ) str_signeq[i] = NULL;
   str_uniteq = (char **) XtRealloc(str_uniteq, jac_rows*sizeof(char *));
   for ( i=0 ; i<jac_rows ; i++ ) str_uniteq[i] = NULL;
   str_cosnor = (char **) XtRealloc(str_cosnor, jac_rows*sizeof(char *));
   for ( i=0 ; i<jac_rows ; i++ ) str_cosnor[i] = NULL;
*****/

   cambia_tipo_text(UxGetWidget(nmod_bboard), option_scelto, False);
}

/***************************************************************/
/*** Widget crea_text(wdg, posx, posy, width, colonne)
 ***   Parametri:
 ***     Widget padre: pater del text.
 ***     int posx, posy, width: posizione e larghezza.
 ***     int colonne: max colonne;
 ***
 ***   Descrizione:
 ***     Utilita' per la creazione dei text-widget nella scrolled-window.
 ***/
Widget crea_text(padre, posx, posy, width, colonne)
Widget padre;
int posx, posy, width, colonne;
{
    Widget wdg;

    argcount = 0;
    XtSetArg(args[argcount], XmNx, posx); argcount++;
    XtSetArg(args[argcount], XmNy, posy); argcount++;
    XtSetArg(args[argcount], XmNwidth, width); argcount++;
    XtSetArg(args[argcount], XmNmaxLength, colonne); argcount++;
    XtSetArg(args[argcount], XmNfontList, fontList); argcount++;
    wdg = XmCreateText(padre, "NewmodText", args, argcount);
    XtAddCallback(wdg,XmNmodifyVerifyCallback,text_maius_callback,NULL);
    return(wdg);
}


/***************************************************************/
/*** Widget crea_label(padre, stringa, posx, posy)
 ***   Parametri:
 ***     Widget padre : pater-widget del text;
 ***     char *stringa : stringa da inserire nella label;
 ***     int posx, posy : posizione del widget;
 ***
 ***   Descrizione:
 ***     Utilita' per la creazione dei label-widget nella scrolled-window
 ***/
Widget crea_label(padre, stringa, posx, posy)
Widget padre;
char *stringa;
int posx, posy;
{
   Widget wdg;
   XmString cstring;

   cstring = CREATE_CSTRING(stringa);
   argcount = 0;
   XtSetArg(args[argcount], XmNx, posx); argcount++;
   XtSetArg(args[argcount], XmNy, posy); argcount++;
   XtSetArg(args[argcount], XmNlabelString, cstring); argcount++;
   wdg = XmCreateLabel(padre, "NewmodScrollwLabel", args, argcount);
   XmStringFree(cstring);
   XtManageChild(wdg);
   return(wdg);
}


/***************************************************************/
/*** set_label(wdg, stringa)
 ***   Parametri:
 ***      Widget wdg : label-widget
 ***      char *stringa : stringa da assegnare alla label-widget
 ***
 ***   Descrizione:
 ***      Utilita' che assegna una stringa ad una label
 ***/
set_label(wdg, stringa)
Widget wdg;
char *stringa;
{
    XmString cstring;

    cstring = CREATE_CSTRING(stringa);
    set_something_val(wdg, XmNlabelString, (XtArgVal) cstring);
    XmStringFree(cstring);
}

/***************************************************************/
/*** controlla_input_def(wdg_nome, wdg_descr)
 ***   Parametri:
 ***     Widget wdg_nome, wdg_descr: widget relativi al nome modulo e 
 ***                                 descrizione.
 ***
 ***   Descrizione:
 ***     Controlla che siano stati inseriti correttamente TUTTI 
 ***     gli anput nella mainWindow di NEWMOD.
 ***     I controlli riguardano nell'ordine:
 ***     - nome del modulo;
 ***     - simboli delle variabili;
 ***     - non-ripetitivita' dei simboli delle variabili.
 ***/
controlla_input_def (wdg_nome, wdg_descr)
Widget wdg_nome, wdg_descr;
{
   int i,j, num_variabili;
   char message[200], *buffer, suffix[10];
   char *tmp_nome, *tmp_descr;

   tmp_nome = XmTextGetString(wdg_nome);
   tmp_descr = XmTextGetString(wdg_descr);

/* Controllo dell'ortografia del nome di modulo */
   if ((strlen(tmp_nome) != 4 ) || contiene_blanks(tmp_nome))
   {
      sprintf (message,"Invalid module name. Modify to continue.");
      attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					  geom_attention);
      scrivi_messaggio (message);
      XtFree(tmp_nome);
      XtFree(tmp_descr);
      return (-1);
   }

   num_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;

   variabili = (StructVars *) XtRealloc(variabili, 
				        num_variabili*sizeof(StructVars));
   dati_geom = (StructDati *) XtRealloc(dati_geom,
					num_dati_geometrici*sizeof(StructDati));

/*************************************************/
/* V A R I A B I L I   D I   S T A T O           */
/*************************************************/
   for (i=0; i < num_var_stato; i++)
   {
   /* S I M B O L I */
      buffer = XmTextGetString(wvar_stato[i].text_simbolo);

      if (strlen(buffer) != 4 || contiene_blanks(buffer) || !isalpha(*buffer))
      {
         if (strlen(buffer) < 4 && strlen(buffer) > 0)
            sprintf (message,
                  "The lenght of the symbol %s is less than 4 (%s state var.)", 
                   buffer, ordinale (i+1,suffix));
         else
            sprintf (message, "Invalid %s state output variable symbol: %s.",
                     ordinale (i+1,suffix),
		     strcmp(buffer,"") ? buffer : "(empty)");
         strcat (message, " Modify to continue.");
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					     geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[i].nome, buffer);
         XtFree(buffer);
      }

   /* V A L O R I */
      buffer = XmTextGetString (wvar_stato[i].text_valore);

      if (strlen(buffer) == 0 || contiene_blanks(buffer))
      {
         sprintf (message,
         "Invalid value (%s) of state output variable %s. Modify to continue",
                  buffer,variabili[i].nome);
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					     geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[i].norm, buffer);
         XtFree(buffer);
      }

      buffer = XmTextGetString(wvar_stato[i].text_descr);
      strcpy(variabili[i].descr, buffer);
      XtFree(buffer);
   }

/*************************************************/
/* V A R I A B I L I   A L G E B R I C H E       */
/*************************************************/
   for (i=0; i < num_var_algebriche; i++)
   {
      j = i + num_var_stato;

   /* S I M B O L I */
      buffer = XmTextGetString(wvar_algebriche[i].text_simbolo);

      if (strlen(buffer) != 4 || contiene_blanks(buffer) || !isalpha(*buffer))
      {
         if (strlen(buffer) < 4 && strlen(buffer) > 0)
            sprintf (message,
                  "The lenght of the symbol %s is less than 4 (%s state alg.)",
                   buffer, ordinale (i+1,suffix));
         else
             sprintf (message,
	           "Invalid %s algebraic output variable symbol: %s.",
                    ordinale (i+1,suffix), 
                    strcmp(buffer,"") ? buffer : "(empty)");
         strcat(message, " Modify to continue.");
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
                                             geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[j].nome,buffer);
         XtFree(buffer);
      }

   /* V A L O R I */
      buffer = XmTextGetString(wvar_algebriche[i].text_valore);

      if (strlen(buffer) == 0 || contiene_blanks(buffer))
      {
         sprintf (message,
         "Invalid value (%s) of algebraic variable %s. Modify to continue",
                  buffer,variabili[j].nome);
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
                                             geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[j].norm,buffer);
         XtFree(buffer);
      }

      buffer = XmTextGetString(wvar_algebriche[i].text_descr);
      strcpy(variabili[j].descr,buffer);
      XtFree(buffer);
   }

/*************************************************/
/* V A R I A B I L I   D I   I N G R E S S O     */
/*************************************************/
   for (i=0; i < num_var_ingresso; i++)
   {
      j = i + num_var_stato + num_var_algebriche;

   /* S I M B O L I */
      buffer = XmTextGetString(wvar_ingresso[i].text_simbolo);

      if ( strlen(buffer) != 4 || contiene_blanks(buffer) || !isalpha(*buffer))
      {
         if (strlen(buffer) < 4 && strlen(buffer) > 0)
            sprintf (message,
                  "The lenght of the symbol %s is less than 4 (%s state inp.)" ,
                   buffer, ordinale (i+1,suffix));
         else
            sprintf (message, "Invalid %s input variable symbol: %s. ",
                  ordinale (i+1,suffix), 
                  strcmp(buffer,"") ? buffer : "(empty)");
         strcat(message, " Modify to continue.");
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					     geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[j].nome, buffer);
         XtFree(buffer);
      }

   /* V A L O R I */
      buffer = XmTextGetString (wvar_ingresso[i].text_valore);

      if (strlen(buffer) == 0 || contiene_blanks(buffer))
      {
         sprintf (message,
         "Invalid value ( %s ) of input variable %s. Modify to continue",
                  buffer,variabili[j].nome);
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					     geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(variabili[j].norm, buffer);
         XtFree(buffer);
      }

      buffer = XmTextGetString(wvar_ingresso[i].text_descr);
      strcpy(variabili[j].descr, buffer);
      XtFree(buffer);
   }

/*************************************************/
/* D A T I   G E O M E T R I C I                 */
/*************************************************/
   for (i=0; i < num_dati_geometrici; i++)
   {
   /* SIMBOLO  (max. 8 char) */
      buffer = XmTextGetString (wdati_geometrici[i].text_dato);

      if ( strlen(buffer) == 0 || strlen(buffer) > 8 )
      {
         sprintf (message,"Invalid data ( %s ). Modify to continue", buffer);
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
					     geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(dati_geom[i].nome, buffer);
         XtFree(buffer);
      }

   /* VARIABILE FORTRAN (max. 6 char) */
      buffer = XmTextGetString (wdati_geometrici[i].text_sigla);

      if ( contiene_blanks(buffer) || strlen(buffer) > 6 )
      {
         sprintf (message,
                  "Invalid  %s Fortran variable: %s. Modify to continue",
                  ordinale (i+1,suffix),
                  strcmp(buffer,"") ? buffer : "(empty)");
         attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
                                             geom_attention);
         scrivi_messaggio (message);
         XtFree(buffer);
         return (-1);
      }
      else
      {
         strcpy(dati_geom[i].sigla, buffer);
         XtFree(buffer);
      }
   }

/* Controllo non-ripetitivita' simboli delle variabili */
   if ( nomi_var_unici() )
      return (1);

/* Inizializzazione stati Toggles dello jacobiano */
   for (i=0; i < jac_rows; i++ )
      for (j=0; j < jac_cols; j++ )
         jactoggstruct[i][j].jac_yes = True;
   num_yes_toggle = jac_rows * jac_cols;

/* Se la matrice jacobiana e' visualizzata */
/* aggiorna comunque i nomi delle variabili */
   if (!bool_dialog_equation)
      aggiorna_jac_var_labels();

   nmod_def_initialized = True;

   nome_nuovo_modulo = tmp_nome;
   descr_nuovo_modulo = tmp_descr;

   return (0);
}

/*************************************************************/
/*** salva_interfaccia ()
 ***
 ***   Descrizione:
 ***     Scrive nel file interface (*.i) le informazioni
 ***     relative al modulo in creazione con la NEWMOD.
 ***     Il file viene aperto e cbiuso dalla funzione chiamante.
 *** 
 ***     last revised 31.1.96
 ***/
salva_interfaccia ()
{
   FILE *fp;
   char filename[256], *message;
   int i,j,k=0,num_variabili;

   componi_file_modulo(filename,nome_nuovo_modulo,FILE_IFACE,LIBUT,LIB_UTENTE);

   if ( (fp = fopen (filename,"w+")) == NULL)
   {
      sprintf (message,"Can't open interface file %s.", filename);
      scrivi_messaggio(message);
      return(1);
   }

/* Nome e descrizione modulo */
   fprintf (fp,"%.4s  %.50s\n", nome_nuovo_modulo,descr_nuovo_modulo);

   fprintf (fp,"****   NUM VAR (st,alg,in,data)\n");

/* Numero di variabili e dati */
   fprintf (fp,"%d\n", num_var_stato);
   fprintf (fp,"%d\n", num_var_algebriche);
   fprintf (fp,"%d\n", num_var_ingresso);
   fprintf (fp,"%d\n", num_dati_geometrici);

/* Descrizione, simbolo e valore variabili di STATO */
   num_variabili = num_var_stato + num_var_algebriche + num_var_ingresso;
   for (i=0; i<num_variabili; i++)
   {
/* intestazione gruppo di variabili (stato, algebriche o ingresso) */
      if ( i == 0 ) fprintf (fp,"****   STATE VARIABLES\n");
      if ( i == num_var_stato ) fprintf (fp,"****   ALGEBRAIC VARIABLES\n");
      if ( i == num_var_stato + num_var_algebriche )
         fprintf (fp,"****   INPUT VARIABLES\n");

      fprintf (fp,"%-4s %-6s %-50s\n",
               variabili[i].nome, variabili[i].norm, variabili[i].descr);
   }

   fprintf (fp,"****   GEOMETRICAL DATA\n");

/* Descrizione, simbolo e valore DATI GEOMETRICI */
   for (i=0; i<num_dati_geometrici; i++)
      fprintf (fp,"%-8s  %-6s\n", dati_geom[i].nome, dati_geom[i].sigla);

/* Tipo di jacobiano */
   fprintf (fp,"****   JACOBIAN TYPE (0 analytical  -  1 numerical)\n");
   fprintf (fp,"%d\n",jacobian_type);

/* Topologia della matrice jacobiana */
   fprintf (fp,"****   JACOBIAN DIPENDENCE COORDINATES (row,col)\n");

   for (i=0; i<jac_rows; i++)
      for (j=0; j<jac_cols; j++)
         if (jactoggstruct[i][j].jac_yes)
            fprintf (fp,"%3d %3d\n", i,j);

/********************************************************************/
/* Inizio sezioni utente (le PARTI GUIDATE sono separate da '++++') */
/********************************************************************/
/* SEZIONE DATI (I2) */
   fprintf (fp,"****   DATA SECTION I2: Declarations\n");
   if( str_dichI2 != NULL )
      fprintf (fp,"%s\n", str_dichI2);
   else
      fprintf (fp,"%s\n", "");

   fprintf (fp,"****   DATA SECTION I2: Code\n");
   if( str_codI2 != NULL )
      fprintf (fp,"%s\n", str_codI2);
   else
      fprintf (fp,"%s\n", "");

/* SEZIONE JACOBIANO (JC) */
   fprintf (fp,"****   JACOBIAN SECTION JC: Declarations\n");

   if( str_dichJC != NULL)     
      fprintf (fp,"%s\n", str_dichJC);
   else
      fprintf (fp,"%s\n", "");

   fprintf (fp,"****   SECTION JC: Text\n");
   if( str_codJC != NULL)  
      fprintf (fp,"%s\n", str_codJC);
   else
      fprintf (fp,"%s\n", "");

   fprintf (fp,"****   SECTION JC: Coefficients\n");
   for (i=0; i<jac_rows; i++)
      for (j=0; j<jac_cols; j++)
         if (jactoggstruct[i][j].jac_yes)
         {
            fprintf (fp,"++++   Residual %3d - Variable %s (%3d)\n",
                     i+1,variabili[j].nome,j+1);
	    if( jactoggstruct[i][j].stringa != NULL )        
               fprintf (fp,"%s\n", jactoggstruct[i][j].stringa);
	    else
               fprintf (fp,"%s\n", "");
         }

/* SEZIONE RESIDUI */
   fprintf (fp,"****   RESIDUALS SECTION: Declarations\n");
   if( str_dichMOD != NULL) 
      fprintf (fp,"%s\n", str_dichMOD);
   else
      fprintf (fp,"%s\n", "");

   fprintf (fp,"****   RESIDUALS SECTION: Code\n");
   if( str_codMOD != NULL )
      fprintf (fp,"%s\n", str_codMOD);
   else
      fprintf (fp,"%s\n", "");

   fprintf (fp,"****   RESIDUALS SECTION: Residuals\n");
   for (i=0; i<num_var_stato+num_var_algebriche; i++)
   {
      fprintf (fp,"++++   RESIDUALS EQUATIONS %d\n",i);
      if( str_resMOD[i] != NULL )
         fprintf (fp,"%s\n", str_resMOD[i]);
      else
         fprintf (fp,"%s\n", "");

   }

/* SEZIONE EQUAZIONI */
   fprintf (fp,"****   EQUATIONS SECTION (Description,Unit,Norm. constant)\n");
   for (i=0,k=0; i<num_var_stato+num_var_algebriche; i++)
      fprintf (fp,"%-50s %-6s %-6s\n", 
                  str_signeq[i], str_uniteq[i], str_cosnor[i]);
     
   fclose(fp);
   return (0);
}

/*** nomi_var_unici ()
 ***
 ***   Descrizione:
 ***     Funzione che controlla l'unicita' dei nome delle variabili
 ***     del nuovo modulo.
 ***/
nomi_var_unici ()
{

   int i,j,num_variabili;
   char suffix1 [10], suffix2 [10];
   char message [200];

   num_variabili = num_var_stato+num_var_algebriche+num_var_ingresso;

/* Controllo tra tutte le variabili */
   for (i=0; i<num_variabili-1; i++)
      for (j=i+1; j<num_variabili; j++)
         if ( !strcmp(variabili[i].nome,variabili[j].nome) )
         {
            sprintf (message, 
            "Illegal identity between the  %s  and  %s variable symbol: %s.\n",
                     ordinale (i+1,suffix1),ordinale (j+1,suffix2),
		     variabili[i].nome);
            strcat (message,"Modify to continue.");
            attention_wdg = (Widget) attention (UxTopLevel,message,MAPPA,
                                                geom_attention);
            scrivi_messaggio (message);
            return (-1);
         }
   return (0);
}

/*** text_maius_callback(w, client_data, text_struct)
 ***    Parametri:
 ***       Widget w : widget text
 ***       XtPointer client_data : non utilizzato.
 ***       XmTextCallbackStruct *text_struct : struttura info text
 ***
 ***    Descrizione:
 ***       Forza l'inserimento di caratteri maiuscoli nei text
 ***       (utilizzata nei text della NEWMOD).
 ***/ 
void text_maius_callback(w, client_data, text_struct)
Widget w;
XtPointer client_data;
XmTextVerifyCallbackStruct *text_struct;
{
   char *txt;
   int i,lung;

   txt = text_struct->text->ptr;
   lung = text_struct->text->length;

   for ( i=0 ; i<lung ; i++ )
      txt[i] = toupper(txt[i]);

   text_struct->doit = True;
}

/*** init_vars() 
 ***
 ***    Descrizione:
 ***      Inizializzazione di alcune variabili globali
 ***/
init_vars()
{
   int i;

   for ( i=0 ; i<MAX_EQUAZIONI ; i++ )
   {
      str_resMOD[i] = NULL;
      str_signeq[i] = NULL;
      str_uniteq[i] = NULL;
      str_cosnor[i] = NULL;
   }
}

/* fine newmod_util.c */
