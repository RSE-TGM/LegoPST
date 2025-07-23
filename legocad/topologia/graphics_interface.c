/**********************************************************************
*
*       C Source:               graphics_interface.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Mon Jan 13 15:08:30 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: graphics_interface.c-3 %  (%full_filespec: graphics_interface.c-3:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)graphics_interface.c	2.26\t4/11/95";
/*
        Fine sezione per SCCS
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

#include "libutilx.h"
#include "macro.h"
#include "utile.h"
#include "files.h"
#include "file_icone.h"
#include "ico_mesg.h"
#include "errore.h"
#include "lg1.h"

#ifdef GRAPHICS_VAR
extern int graphics_on;  /* pagina grafica visualizzata True/False */
#endif

#ifndef MAIN
#ifndef DATI
/*
#include "UxLib.h"
*/
#include "UxXt.h"
#include "lg1MainWindowType.h"
extern SELECTION bl_selezionati;
#endif
#endif

extern void seleziona_blocco();      /* seleziona/deseleziona un blocco in grafica */
extern delete_selected_macro();      /* cancella i macro selezionati */
extern void salva_macroblocchi();    
extern int modifiche; 
extern short int tipo_modulo;        /* identifica se di processo o regolazione */
extern void set_new_descr();    /* set descrizione e nome blocco in lista */

extern MacroBlockType *macroblocks;  /* vettore puntatore a descrittori macroblocchi */
extern int num_macro,macro_allocate; /* num macro istanziate, num macro allocate */

extern ERR_LEVEL err_level;
extern BLOCCO blocchi[];

extern dbx_nuovo_blocco;
extern void dialog_nuovo_blocco();
extern void carica_lista_moduli();
extern Widget text_nome_blocco;
extern Widget text_descr_blocco;

void Iadd_new_block_graf(int tipo)
{
   extern Widget text_nome_blocco,lista_moduli;
   

   if(tipo == REGOLAZIONE)
   {
      tipo_modulo = TP_BLOCK_REG;
      if (dbx_nuovo_blocco == NULL)
         dialog_nuovo_blocco();
      set_something_val(text_nome_blocco,XmNeditable, (XtArgVal) False);
   }
   else if (tipo == PROCESSO)
   {
      tipo_modulo = TP_BLOCK;
      if (dbx_nuovo_blocco == NULL)
         dialog_nuovo_blocco();
      set_something_val(text_nome_blocco,XmNeditable, (XtArgVal) True);
   }


   XmTextSetString( text_nome_blocco , "" );
   XmTextSetString( text_descr_blocco, "" );
   carica_lista_moduli(tipo_modulo);
   XtManageChild(dbx_nuovo_blocco);
   XmListSelectPos(lista_moduli,1,True);
}


void Isalva_grafica()
{
  int ret_stat;
  char path[1024];
  char direttorio[1024];
  char shell_command[300];

  strcpy ( path, modello.path_legocad );
  strcat ( path, modello.nome );
  strcpy(direttorio,path);
  strcat ( path, "/macroblocks.dat" );

  strcpy(shell_command,"cp ");
  strcat(shell_command,path);
  strcat(shell_command," ");
  strcat(shell_command,direttorio);
  strcat(shell_command,"/macroblocks.lastsave");
  ret_stat = system(shell_command);
  if( ret_stat == 127 )
     printf("I can't perform backup of macroblocks.dat\n");
  else if( ret_stat != 0)
     printf("I can't nake backup of macroblocks.dat\n");
  else
     printf("last version of macroblocks.dat is in macroblocks.lastsave\n");

   salva_macroblocchi(macroblocks, &num_macro);

}


int Icerca_blocco_in_macroblocchi()
{
   int i,j;
   MacroBlockType *ind;
   char modname[20],blname[20];

   for(i=0;i<num_macro;i++)
   {
      ind = &macroblocks[i];
      if(ind->cancellato)
         printf("macroblocco cancellato\n");
      else
         for(j=0;j< ind->num_blocchi;j++)
            if(!ind->blocks[j].cancellato)
            {
               strcpy(modname,ind->blocks[j].nome_modulo);
               strcpy(blname,ind->blocks[j].nome_blocco);
            }
   } 
}

int Idimmi_indice_blocco_e_macroblocco(char *bl_label,int *imacro,int *iblocco)
{
   int i,j;
   MacroBlockType *ind;
   char modname[20],blname[20];

   for(i=0;i<num_macro;i++)
   {
      ind = &macroblocks[i];
      if(ind->cancellato)
         printf("macroblocco cancellato\n");
      else
         for(j=0;j< ind->num_blocchi;j++)
            if(!ind->blocks[j].cancellato)
               if(!strcmp(bl_label,ind->blocks[j].nome_blocco))
               {
                  *imacro = i;
                  *iblocco = j;
                  return(1);
               }
   }
   return(-1);
}

int Iverifica_nome_blocco(char *nomebl) 
{
   int i,j;
   MacroBlockType *ind;
   char *nomeappo;

   for(i=0;i<num_macro;i++)
   {
      ind = &macroblocks[i];
      if(!ind->cancellato)
         for(j=0;j< ind->num_blocchi;j++)
            if(!ind->blocks[j].cancellato)
            {
               nomeappo = &ind->blocks[j].nome_blocco[4];
               if(!strcmp(nomebl,nomeappo))
                  return(0);
            }
   }
   return(1);
}
  
void Idel_graph_bloc()
{
   int i,j;
   MacroBlockType *ptr_macro;


   for(j=0;j<num_macro;j++)
   {
       ptr_macro = &macroblocks[j];
       for ( i=0 ; i<ptr_macro->num_blocchi ; i++ )
           if (ptr_macro->blocks[i].selezionato)
              cancella_blocco(ptr_macro, &ptr_macro->blocks[i]);
   }

}


int Iverifica_errore()
{
   extern ERR_LEVEL err_level;

   if(err_level)
   {
      err_level = NO_ERROR;
      return(1);
   }
   return(0);
}


int aggiorna_bl_selezionati(Widget lista)
{
   int i,*item_positions,item_count,retval;

   if ( bl_selezionati.pos )
      free(bl_selezionati.pos);
   retval=XmListGetSelectedPos(lista,&item_positions,&item_count);

   if(retval==False)
       item_count=0;
   bl_selezionati.num = item_count;
   if ( bl_selezionati.num )
   {
       bl_selezionati.pos = (int*) malloc ( sizeof(int) * item_count );
       memcpy ( bl_selezionati.pos, item_positions,sizeof(int) * item_count );
/*
 * I campi 'pos' vengono decrementati per poterli utilizzare come indici
 * nel vettore 'blocchi[]'.
 */
       for ( i=0; i<bl_selezionati.num; i++ )
           bl_selezionati.pos[i] -= 1;
    }
    else
       bl_selezionati.pos = NULL;
       
}

int Ideselect_this_block_from_list(Widget listaBlocchi,char *nome)
{
   int i,j,item_positions,item_count,retval;
   char labelbl[10];

   for(i=0;i<num_blocchi;i++)
   {
      strcpy(labelbl,blocchi[i].sigla_modulo);
      strcat(labelbl,blocchi[i].sigla_blocco);
      if(!strcmp(labelbl,nome))
      {
         XmListDeselectPos(listaBlocchi, i+1);
         aggiorna_bl_selezionati(listaBlocchi);
         break;
      } 
   } 
}

int Iselect_this_block_from_list(Widget listaBlocchi,char *nome)
{
   int i,position,narg,nentry;
   char entry[100],labelbl[10];
   XmString *lista;
   Arg      arg[2];


   for(i=0;i<num_blocchi;i++)
   {
      strcpy(labelbl,blocchi[i].sigla_modulo);
      strcat(labelbl,blocchi[i].sigla_blocco);
      if(!strcmp(labelbl,nome))
      {
         position = i;
         break;
      }
   }

/*** alloco e recupero la lista degli item */

   nentry = bl_selezionati.num;
   /*printf("\n\nIselect n.entry gia' selezionate %d\n",nentry);*/

   if(nentry)
   {
      lista = ( XmString *) malloc( (nentry+1) * sizeof(XmString) );
      /* recupero la vecchia lista */
      for(i=0;i < nentry;i++)
      {
         strcpy(entry,blocchi[bl_selezionati.pos[i]].sigla_modulo);
         strcat ( entry,  " (" );
         strcat ( entry, blocchi[bl_selezionati.pos[i]].sigla_blocco );
         strcat ( entry,  ")  " );
         strcat ( entry, blocchi[bl_selezionati.pos[i]].descr_blocco );
         lista[i]=XmStringCreateSimple(entry);  
      }
      /* e aggiungo la nuova entry */
      strcpy(entry,blocchi[position].sigla_modulo);
      strcat ( entry,  " (" );
      strcat ( entry, blocchi[position].sigla_blocco );
      strcat ( entry,  ")  " );
      strcat ( entry, blocchi[position].descr_blocco );
      lista[i]=XmStringCreateSimple(entry);
      nentry++;
   } 
   else
   {
      /* aggiungo la entry */
      lista = ( XmString *) malloc( (nentry+1) * sizeof(XmString) );
      strcpy(entry,blocchi[position].sigla_modulo);
      strcat ( entry,  " (" );
      strcat ( entry, blocchi[position].sigla_blocco );
      strcat ( entry,  ")  " );
      strcat ( entry, blocchi[position].descr_blocco );
      lista[0]=XmStringCreateSimple(entry);
      nentry = 1;
   }
 

   narg = 0;
   XtSetArg(arg[narg], XmNselectedItems, lista );narg++;
   XtSetArg(arg[narg], XmNselectedItemCount, nentry);narg++;   
   XtSetValues(listaBlocchi, arg, narg); 

   aggiorna_bl_selezionati(listaBlocchi);


   for ( i=0; i<bl_selezionati.num; i++ )
      XmStringFree(lista[i]); 
//      XtFree(lista[i]); 

   XtFree( lista );


}


/* set della descrizione e del nome blocco nella grafica */

Iset_graf_new_blname(char *oldname,char *nome_blocco)
{
   int i,j;
   char *strappo;
   extern void set_label();

   printf("effettuo la modifica nella grafica \n");
   for(i=0;i<num_macro;i++)
   {
       for(j=0;j<macroblocks[i].num_blocchi;j++)
       {
          if( macroblocks[i].blocks[j].selezionato && (!macroblocks[i].blocks[j].cancellato) )
          {
             strappo = &macroblocks[i].blocks[j].nome_blocco[4];
             strcpy(strappo,nome_blocco);
             printf("effettuo il cambiamaneto delle icone sulla macro %d\n",i);
/* 7-4-95 Micheletti 
  deve essere cambiata la label delle icone solo se la pagina
  delle icone e' aperta 
*/

             if( macroblocks[i].wwinblock != NULL)
                set_label( macroblocks[i].blocks[j].wlabel, macroblocks[i].blocks[j].nome_blocco );
          }
       }
   }

}

Iset_list_new_descr(char *new_nome_blocco,char * new_descr_blocco)
{
   set_new_descr(new_nome_blocco,new_descr_blocco);
}

Iget_descr_blocco(char *nome_blocco,char *descr)
{
   int i;
    
   for(i=0;i<num_blocchi;i++)
      if( !strcmp(nome_blocco,blocchi[i].sigla_blocco) )
         strcpy(descr,blocchi[i].descr_blocco);
}



/* struttura dati common per creazione icona */

struct {
      MacroBlockType *ptr_macro;
      int x;
      int y;
      int tipo;
      Boolean selez;
      PixmapInfo *pixmap_info;
      char label_blocco[9];
      char nome_modulo[5];
      Widget wdg;
      int ind_macro;
      
} par_creaico;

Iset_parico(MacroBlockType *ptr_macro,int x,int y,int tipo,Boolean selez,
            PixmapInfo *pix_info,char *label_blocco,char *nome_modulo,
            Widget wdg,int ind_macro)
{
   par_creaico.ptr_macro = ptr_macro;
   par_creaico.x = x;
   par_creaico.y = y;
   par_creaico.tipo = tipo;
   par_creaico.selez = selez;
   par_creaico.pixmap_info = pix_info;
   strcpy(par_creaico.label_blocco,label_blocco);
   strcpy(par_creaico.nome_modulo,nome_modulo);
   par_creaico.ind_macro = ind_macro;
   par_creaico.wdg = wdg;
}

Icrea_ico()
{
   extern int alloca_nuovo_blocco();
   extern int crea_icona();

     int ind;

  /* Aggiorna la struttura blocks del macroblocco relativo */
      ind = alloca_nuovo_blocco(par_creaico.ptr_macro, 
                                par_creaico.x, 
                                par_creaico.y, 
                                par_creaico.tipo, 
                                par_creaico.selez,
                                par_creaico.pixmap_info, 
                                par_creaico.label_blocco,
                                "", 
                                par_creaico.nome_modulo );

      crea_icona( par_creaico.wdg, 
                  par_creaico.pixmap_info, 
                  par_creaico.label_blocco, 
                  par_creaico.x, 
                  par_creaico.y,
                  FROM_WINDOW_BLOCK, 
                  par_creaico.ind_macro, 
                  ind );
}
