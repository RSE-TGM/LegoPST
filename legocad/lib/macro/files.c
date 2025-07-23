/**********************************************************************
*
*       C Source:               files.c
*       Subsystem:              2
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 12:13:14 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: files.c,2 %  (%full_filespec: 2,csrc,files.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)files.c	2.7\t4/11/95";
/*
        Fine sezione per SCCS
*/

/* files.c - subroutine di lettura/scrittura files macroblocks.dat */

/* Calleri Nicola, Buffo Massimo 26/04/93 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef VMS
#include <stdlib.h>
#else
#include <malloc.h>
#endif 
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

#include "macro.h"
#include "ico_mesg.h"
#include "files.h"
#include "file_icone.h"

extern Widget warning_widget;
extern char *error_mesg[];
extern char NomeModello[];


/*** leggi_macroblocchi( mblocks, num_mblocks )
 *** Parametri:
 ***    MacroBlockType *mblocks : struttura dei macroblocchi da aggiornare
 ***    int *num_mblocks : numero di macroblocchi letti
legge i macroblocchi nel file macroblocks.dat */
leggi_macroblocchi( mblocks, num_mblocks, macro_allocate )
MacroBlockType **mblocks;
int *num_mblocks, *macro_allocate;
{
   char buffer[100], temp[10];
   int  stato = 0;
   long offset;
   FILE *fp;
 
/* Alloca i primi 100 elementi */
   *mblocks = (MacroBlockType *) calloc_mem( 100, sizeof(MacroBlockType));
   *macro_allocate = 100;
   *num_mblocks = 0;

   if (( fp = fopen( FILE_MACROBLOCKS, "r")) == NULL )
      if (( fp = fopen( FILE_MACROBLOCKS, "w")) == NULL )
         s_error(APPLICATION_NAME, error_mesg, EOPENMACROBLK, 1);
      else
      { /* Se il file macroblocks.dat non esiste allora viene creato */
         fprintf(fp, "****\n");
         fprintf(fp, "%s\n", NomeModello);
         fprintf(fp, "****\n");
         fclose(fp);
         return(1);
      }

   stato = True;
   while( leggi_file_macro(fp, &((*mblocks)[ *num_mblocks ]), stato) != -1 )
   {
printf("nome macroblocco letto: %s\n", (*mblocks)[ *num_mblocks ].nome_macro);
      stato = False;
      (*mblocks)[ *num_mblocks ].wwinblock = NULL;
      (*mblocks)[ *num_mblocks ].wtavblock = NULL;

   /* Alloca e carica i blocchi */
      offset = ftell(fp);
      leggi_blocchi( fp, *mblocks, *num_mblocks );
      fseek(fp, offset, SEEK_SET);

      (*num_mblocks)++;

   /* Alloca altri 100 elementi se necessario */
      if ( *num_mblocks > *macro_allocate )
           *mblocks = (MacroBlockType *) 
                      realloc_mem( *mblocks, *macro_allocate += 100,
			 	   sizeof(MacroBlockType));
   }
   fclose(fp);
   return(0);
}
/*** modifica_macroblocco( mblock, nome_macro, descr_macro )
 *** Parametri :
 ***     MacroBlockType *mblock : struttura macroblocco.
 ***     char *nome_macro : nome del macroblocco.
 ***     char *descr_macro : descrizione del macroblocco.
modifica il macroblocco nel file macroblocks.dat */
modifica_macroblocco( mblock, nome_macro, descr_macro )
MacroBlockType *mblock;
char *nome_macro, *descr_macro;
{
   FILE *fp;

   if (( fp = fopen( FILE_MACROBLOCKS, "r+")) == NULL )
      return(1);
 
   if (cerca_macro_nel_file(fp, mblock->nome_macro) == 0)
   {
      strcpy( mblock->nome_macro, nome_macro );
      strcpy( mblock->descr_macro, descr_macro );
      fprintf( fp, "%-14s %04d %04d %-50s\n",
		mblock->nome_macro,
		mblock->pos_icona.posx,
		mblock->pos_icona.posy,
		mblock->descr_macro);
   }
   fclose(fp);
   return(0);
}

/*** aggiungi_macroblocco( mblock )
 *** Parametri :
 ***     MacroBlockType *mblock : struttura macroblocco.
aggiunge il macroblocco nel file macroblocks.dat */
aggiungi_macroblocco( mblock, nome_macro, descr_macro )
MacroBlockType *mblock;
{
   FILE *fp;

   if (( fp = fopen( FILE_MACROBLOCKS, "r+")) == NULL )
      return(1);
 
   fseek(fp, 0L, SEEK_END);
   fprintf( fp, "%-14s %04d %04d %-50s\n",
		mblock->nome_macro,
		mblock->pos_icona.posx,
		mblock->pos_icona.posy,
		mblock->descr_macro);
   fprintf( fp, "****\n");
   fprintf( fp, "****\n");
   fclose(fp);
   return(0);
}

/*** salva_macroblocchi( mblocks, num_mblocks )
 *** Parametri:
 ***    MacroBlockType *mblocks : struttura dei macroblocchi da salvare
 ***    int num_mblocks : numero di macroblocchi letti
salva i macroblocchi nel file macroblocks.dat */
salva_macroblocchi( mblocks, num_mblocks )
MacroBlockType *mblocks;
int *num_mblocks;
{
   int i, j, flag = 0;
   char buffer[100], file_temp[50];
   FILE *fp, *fp_tmp;
   Boolean cancella = False;


printf("salvataggio...\n");


/* apertura file macroblocks.dat */
   if (( fp = fopen( FILE_MACROBLOCKS, "w")) == NULL )
      return(1);

/* nome modello e descrizione */
   fprintf(fp, "****\n");
   fprintf(fp, "%s\n", NomeModello);
   fprintf(fp, "****\n");

   for ( i=0 ; i < *num_mblocks ; i++ )
   {
       if ( !mblocks[i].cancellato )
       {
/***
             memcpy( &mblocks[j], &mblocks[i], sizeof(MacroBlockType));
             XtRemoveEventHandler( mblocks[i].wmacro, ButtonPressMask, False,
                                   macro_bpress, i );
             XtRemoveEventHandler( mblocks[i].wmacro, ButtonReleaseMask, False,
                                   macro_brelease, i );
             XtAddEventHandler( mblocks[i].wmacro, ButtonPressMask,
			         False, macro_bpress, j );
             XtAddEventHandler( mblocks[i].wmacro, ButtonReleaseMask,
				 False, macro_brelease, j );

             def_handler_macro(j);
             j++;
***/

       /* scrivi l'header del macroblocco nel file temporaneo */  
          fprintf( fp, "%-14s %04d %04d %-50s\n", mblocks[i].nome_macro,
	              				  mblocks[i].pos_icona.posx,
	             				  mblocks[i].pos_icona.posy,
	              				  mblocks[i].descr_macro);

       /* stringa di inizio macroblocco */
          fprintf(fp, "****\n");

       /* salva i blocchi */
	  for ( j=0 ; j<mblocks[i].num_blocchi ; j++ )
	     salva_blocco( fp, &mblocks[i].blocks[j] );

       /* salva le linee grafiche */
	  for ( j=0 ; j<mblocks[i].num_line ; j++ )
	     salva_linea( fp, &mblocks[i].line[j] );

       /* stringa di terminazione sezione macroblocco */
          fprintf(fp, "****\n");
       }
       else
       {
          printf("cancello.\n");
          cancella = True;
          mblocks[i].cancellato = -1;  /* macro cancellato anche nel file */
          if (mblocks[i].blocks != NULL)
	  {
             XtFree(mblocks[i].blocks);
	     mblocks[i].blocks = NULL;
	  }
       }
   }

   fclose(fp);
   return(0);
}

/*** leggi_file_macro( fp, mblock, flag )
 *** Parametri:
 ***   FILE *fp : file pointer macroblocks.dat 
 ***  MacroBlcokType *mblock : struttura macroblocco da riempire
 ***  Boolean flag : indica se bisogna leggere il file dall'inizio.
legge la prima macro e ,ad ogni chiamata , le successive che incontra nel
file  macroblocks.dat. restituisce 0 se trovato , altrimenti restituisce -1 */
leggi_file_macro( fp, mblock, flag )
FILE *fp;
MacroBlockType *mblock;
Boolean flag;
{
   char buffer[100], *str;
   Boolean macro_found = True, primo = False;

   if (flag)
   {
      fseek( fp, 0L, SEEK_SET );

      while( (str = fgets(buffer, 100, fp)) != NULL )
      {
         if ( !strncmp(buffer, "****", 4) )
            break;
      }
      if ( str == NULL )
         return(-1);
      if ( fgets(buffer, 100, fp) == NULL)   /* salta il nome del modulo */
         return(-1);
      macro_found = False;
   }
   else
      macro_found = True;
/* ricerca dei macroblocchi */
   while( (str = fgets(buffer, 100, fp)) != NULL )
   {
      long offs_macro;

      if ( !strncmp(buffer, "****", 4) )
      {
         primo = True;
         macro_found = !macro_found;
         if (macro_found)
            offs_macro = ftell(fp);
         continue;
      } 

      if (macro_found && primo)
      {
      /* Recupera i dati del macroblocco */
	 sscanf(buffer, "%s %d %d\n", mblock->nome_macro,
	                              &mblock->pos_icona.posx,
		                      &mblock->pos_icona.posy);
         copy_n_car(mblock->descr_macro, buffer + OFFS_DESCR_MACRO, 50); 
	 trim_blank(mblock->descr_macro);
         mblock->cancellato = False;
/**** Lorenzo Castiglioni ***/
        mblock->selezionato = False;
/**** fine L.C.           ***/
         fseek(fp, offs_macro, SEEK_SET);
         return(0);
      }
   }
   return(-1);
}

/*** cerca_macro_nel_file(fp, nome_macro)
 ***    Parametri:
 ***   FILE *fp : file pointer macroblocks.dat
 *** char *nome_macro : nome del macroblocco da cercare
cerca un macroblocco nel file. Restituisce 0 se trovato, -1 altrimenti.
Il File pointer viene posizionato sul record del macroblocco. */
cerca_macro_nel_file(fp, nome_macro)
FILE *fp;
char *nome_macro;
{
   int stato;
   MacroBlockType mblock;

   stato = True;
   while( leggi_file_macro(fp, &mblock, stato) != -1 )
   {
      stato = False;
      if ( !strcmp( nome_macro, mblock.nome_macro ))
         return(0);
   }
   return(-1);
}

/*** cerca_macroblocco( macroblocks, num_macro, nome_macro )
 *** Parametri:
 ***    MacroBlockType *macroblocks : struttura macroblocchi.
 ***    int num_macro : numero elementi in macroblocks.
 *** char *nome_macro : nome del macroblocco da cercare.
cerca una macro in macroblocks. restituisce 0 se trovato , altrimenti
restituisce -1 */
cerca_macroblocco( macroblocks, num_macro, nome_macro )
MacroBlockType *macroblocks;
int num_macro;
char *nome_macro;
{
   while ( num_macro-- )
   {
      if ( macroblocks[num_macro].cancellato )
         continue;
      if ( strcmp(macroblocks[num_macro].nome_macro, nome_macro) == 0)
         return(0);
   }
   return(-1);
}

/*** carica_blocchi_macro( mblock )
 ***   Parametri:
 ***     MacroBlockType *mblock : struttura macroblocco da aggiornare
legge il file macroblocks.dat ed aggiorna la struttura MacroBlockType del
relativo macroblocco */
carica_blocchi_macro( mblock, k )
MacroBlockType *mblock;
int k;
{
   FILE *fp;

/* inizializza il numero dei blocchi selezionati */
   mblock[k].num_blocchi_selez = 0;

   if (( fp = fopen( FILE_MACROBLOCKS, "r")) == NULL )
      return(1);

   if (cerca_macro_nel_file(fp, mblock[k].nome_macro) == -1)
      return(-1);

   leggi_blocchi( mblock, k );
   fclose( fp );
}

/*** leggi_blocchi( fp, mblock )
 ***   Parametri:
 ***     FILE *fp : file pointer di macroblocks.dat.
 ***     MacroBlockType *mblock : puntatore alla struttura macroblocco da
 ***				  aggiornare.
recupera i blocchi dal file macroblocks.dat ed aggiorna la struttura
macroblocco. Il file pointer deve essere posizionato sul nome del
macroblocco */
leggi_blocchi( fp, mblock, k )
FILE *fp;
MacroBlockType *mblock;
int k;
{
   int iblk = 0, iline = 0, i, *num_blk_alloc, *num_line_alloc, tipo;
   short int with_arrow, dummy;
   char buffer[100], tempo[20];

   fgets(buffer, 100, fp);
   fgets(buffer, 100, fp);

/* Alloca la memoria per i primi 100 elementi */
   num_blk_alloc = &mblock[k].num_blocchi_alloc;
   *num_blk_alloc = 100;
   mblock[k].blocks = (BlockType *) calloc_mem(*num_blk_alloc,
					       sizeof(BlockType));

   num_line_alloc = &mblock[k].num_line_alloc;
   *num_line_alloc = 50;
   mblock[k].line = (GLine *) calloc_mem(*num_line_alloc, sizeof(GLine));
   
   while ( fgets(buffer, 100, fp) != NULL )
   {
      if ( !strncmp(buffer, "****",4) )
         break;

      sscanf( buffer, "%d", &tipo);
      switch ( tipo )
      {
         case TP_BLOCK:
              sscanf( buffer, "%1hd %8s %4s %2hd %4d %4d %1hd\n",
	                       &mblock[k].blocks[iblk].tipo,
	                       mblock[k].blocks[iblk].nome_blocco,
	   	               mblock[k].blocks[iblk].nome_modulo,
		               &mblock[k].blocks[iblk].num_icona,
		               &mblock[k].blocks[iblk].pos_icona.posx,
		               &mblock[k].blocks[iblk].pos_icona.posy,
		               &mblock[k].blocks[iblk].flag );
              mblock[k].blocks[iblk].descr_blocco[0]='\0';
              if( strlen(mblock[k].blocks[iblk].nome_blocco) < 8 )
                 strncat(mblock[k].blocks[iblk].nome_blocco,"        ",8 - strlen(mblock[k].blocks[iblk].nome_blocco));
              printf("nome blocco '%s' len %d\n",mblock[k].blocks[iblk].nome_blocco,strlen(mblock[k].blocks[iblk].nome_blocco));
              break;

         case TP_SYMBOL:
              sscanf( buffer, "%1hd %*8s %2hd %4d %4d",
		               &mblock[k].blocks[iblk].tipo,
		               &mblock[k].blocks[iblk].num_icona,
                               &mblock[k].blocks[iblk].pos_icona.posx,
                               &mblock[k].blocks[iblk].pos_icona.posy);
              mblock[k].blocks[iblk].nome_blocco[0]='\0';
              mblock[k].blocks[iblk].descr_blocco[0]='\0';
              mblock[k].blocks[iblk].nome_modulo[0]='\0';
              break;

         case TP_REMARK:
              sscanf( buffer, "%1hd %*8s %4d %4d",
		              &mblock[k].blocks[iblk].tipo,
                              &mblock[k].blocks[iblk].pos_icona.posx,
                              &mblock[k].blocks[iblk].pos_icona.posy);
              copy_n_car( mblock[k].blocks[iblk].descr_blocco, buffer+21, 50 );
	      trim_blank( mblock[k].blocks[iblk].descr_blocco );
              mblock[k].blocks[iblk].nome_blocco[0]='\0';
              mblock[k].blocks[iblk].nome_modulo[0]='\0';
              break;

         case TP_LINE:
              sscanf( buffer, "%*1d %*8s %2hd %2hd %2hd %2hd %1hd %3d",
                              &mblock[k].line[iline].thickness,
                              &mblock[k].line[iline].color,
                              &mblock[k].line[iline].length_arrow,
                              &mblock[k].line[iline].width_arrow,
			      &with_arrow,
			      &mblock[k].line[iline].num_points);
              mblock[k].line[iline].with_arrow = (with_arrow) ? True : False; 
              mblock[k].line[iline].points = 
                        (XPoint *) calloc_mem(mblock[k].line[iline].num_points, 
                                              sizeof(XPoint));

	/* recupera le coordinate dei punti */
              for (i=0 ; i<mblock[k].line[iline].num_points ; i++)
              {
                  if (fgets(buffer, 100, fp) == NULL)
		     break;

                  sscanf(buffer, "(%hd,%hd)", 
			 &mblock[k].line[iline].points[i].x,
			 &mblock[k].line[iline].points[i].y );
              }
              break;
      }
   
      if (tipo == TP_LINE)
      {
          iline++;
          if ( iline >= *num_line_alloc )
             mblock[k].line = (GLine *) realloc_mem( mblock[k].line,
					             *num_line_alloc += 50,
						     sizeof(GLine));
      }
      else
      {
          mblock[k].blocks[iblk].ind_macro = k;
          mblock[k].blocks[iblk].selezionato = False;
          mblock[k].blocks[iblk].cancellato = False;

/***      printf( "blocco %hd : %hd %s %s %hd %d %d %.20s\n", iblk,
		  mblock[k].blocks[iblk].tipo,
		  mblock[k].blocks[iblk].nome_blocco,
	    	  mblock[k].blocks[iblk].nome_modulo,
		  mblock[k].blocks[iblk].num_icona,
		  mblock[k].blocks[iblk].pos_icona.posx,
		  mblock[k].blocks[iblk].pos_icona.posy,
		  mblock[k].blocks[iblk].descr_blocco ); ***/
          iblk++;

          if ( iblk >= *num_blk_alloc )
             mblock[k].blocks = (BlockType *) 
					realloc_mem( mblock[k].blocks,
					             *num_blk_alloc += 100,
						     sizeof(BlockType));
       }
   }
   printf("blocchi letti: %d, linee grafiche lette: %d\n", iblk, iline);
   mblock[k].num_blocchi = iblk;
   mblock[k].num_line = iline;
/* L. Castiglioni 1-3-94 **/
   mblock[k].num_blocchi_selez=0;
   mblock[k].num_line_selez=0;
/* fine L.C. */
}

/*** salva_blocchi_macro( mblock )
 ***   Parametri:
 ***     MacroBlockType *mblock : struttura macroblocco da aggiornare
salva nel file macroblocks.dat i blocchi del macroblocco relativo */
salva_blocchi_macro( macroblocks, ind_macro )
MacroBlockType *macroblocks;
int ind_macro;
{
   short int trovato = 0;
   int i, j, flag = 0;
   char buffer[100], nome_macro[15], stringa[120];
   MacroBlockType *mblock;
   FILE *fp, *fp_tmp;

   mblock = &macroblocks[ind_macro];

/* apertura file macroblocks.dat */
   if (( fp = fopen( FILE_MACROBLOCKS, "r")) == NULL )
      return(1);

/* apertura file temporaneo macroblocks.tmp */
   if (( fp_tmp = fopen( FILE_MACROBLOCKS_TMP, "w")) == NULL )
      return(1);

/* nome modello e descrizione */
   while( fgets(buffer, 100, fp) != NULL )
   {
      fputs(buffer, fp_tmp);
      if ( !strncmp(buffer, "****", 4) )
         break;
   }
   fgets(buffer, 100, fp);   /* salta il nome del modello */
   fputs(buffer, fp_tmp);

   i = j = 0;
/* ricerca dei macroblocchi */
   while( fgets(buffer, 100, fp) != NULL )
   {
      if ( !strncmp(buffer, "****", 4) )
      {
         fputs(buffer, fp_tmp);
         if (trovato == 1) /* macroblocco da considerare. Salvare i blocchi */
         {
            for ( j = 0, i = 0 ; i < mblock->num_blocchi ; i++ )
               if ( !mblock->blocks[i].cancellato )
               {
                  salva_blocco( fp_tmp, &mblock->blocks[i] );

              /* aggiorna l'array dei blocchi, cancellando i blocchi */
	      /* marcati DELETED */
                  memcpy( &mblock->blocks[j], &mblock->blocks[i],
                          sizeof(BlockType));
                  set_something_val( mblock->blocks[j].wpixmap, XmNuserData, (XtArgVal) &mblock->blocks[j] );
	      /* gestione della selezione */
		  def_translations_block(ind_macro, j);
                  j++;
               }
            trovato = -1;   /* il flag indica che i blocchi sono stati */
			    /* salvati */
         }
         flag = (flag+1)%2;    /* 1 - inizio macro, 0 - blocchi macro */
         continue;
      } 

      if (flag == 0)
         if ( !trovato )  /* i blocchi non si riferiscono alla macro */
			  /* considerata */
            fputs(buffer, fp_tmp);

      if (flag == 1) /* dati del macroblocco */
      {
         sscanf(buffer, "%s", nome_macro);
         if ( !strcmp(nome_macro, mblock->nome_macro) )
	    trovato = 1; /* macroblocco trovato -> salvare i blocchi. */
         else
	    trovato = 0; /* macroblocco non e' quello da considerare */
         fputs(buffer, fp_tmp);
      }
   }

   fclose(fp);
   fclose(fp_tmp);

#ifdef VMS

   remove( FILE_MACROBLOCKS );

#else

   unlink( FILE_MACROBLOCKS );

#endif

   rename( FILE_MACROBLOCKS_TMP, FILE_MACROBLOCKS );
 
   mblock->num_blocchi = j;
/*   printf("\nnumero blocchi della macro %s : %d\n", mblock->nome_macro, j);
   for (i=0 ; i<j ; i++ )
      printf("Blocco %d : %s %s<\n", i, mblock->blocks[i].nome_blocco,
					mblock->blocks[i].descr_blocco );
   printf("\n"); ***/
   return(0);
}

/*** salva_blocco( fp, block )
 *** Parametri :
 ***    FILE *fp;
 ***    BlockType *block : blocco da salvare nel file 
salva un blocco nel file puntato da fp nella posizione corrente */
salva_blocco( fp, block )
FILE *fp;
BlockType *block;
{
   if (block->cancellato)
      return;

   switch (block->tipo)
   {
      case TP_BLOCK :
           fprintf( fp, "%1d ",block->tipo );
           fprintf( fp, "%8.8s ",block->nome_blocco );
           fprintf( fp, "%4.4s ",block->nome_modulo );
           fprintf( fp, "%2d ",block->num_icona );
           fprintf( fp, "%4d %4d ",block->pos_icona.posx,
	                          block->pos_icona.posy );
           fprintf( fp, "%1d\n",block->flag );
                    break;

      case TP_SYMBOL:
           fprintf( fp, "%1d ", block->tipo);
           fprintf( fp, "%8.8s ", "*SYMBOL*");
           fprintf( fp, "%2d ", block->num_icona);
           fprintf( fp, "%4d %4d\n", block->pos_icona.posx,
                                     block->pos_icona.posy);
                    break;

      case TP_REMARK:
           fprintf( fp, "%1d ", block->tipo);
           fprintf( fp, "%8.8s ", "*REMARK*");
           fprintf( fp, "%4d %4d ", block->pos_icona.posx,
                                   block->pos_icona.posy);
           fprintf( fp, "%-50.50s\n",block->descr_blocco );
                    break;
   }
}

/*** salva_linea( fp, line )
 *** Parametri :
 ***    FILE *fp: file pointer.
 ***    GLine *line : gruppo di linee da salvare. */
salva_linea(fp, line)
FILE *fp;
GLine *line;
{
   int i;

   fprintf( fp, "%1d %8s %2d %2d %2d %2d %1hd %3d\n",
	        TP_LINE,
                "*GLINES*",
                line->thickness,
                line->color,
                line->length_arrow,
                line->width_arrow,
                (line->with_arrow) ? 1 : 0,
		line->num_points);

/* recupera le coordinate dei punti */
   for (i=0 ; i<line->num_points ; i++)
       fprintf(fp, "(%d,%d)\n", 
		   line->points[i].x,
		   line->points[i].y );
}

/*** char *calloc_mem(num, nbytes)
 *** Parametri:
 ***     num : numero di celle da allocare
 ***   nbytes: dimensione della singola cella
utilizza la funzione di sistema malloc per allocare la memoria e ne controlla
l'uscita ***/
char *calloc_mem(num, nbytes)
int num;
int nbytes;
{
   char *p;

   if ( num*nbytes == 0 )
      return(NULL);
/***** LORENZO
   p = XtMalloc((unsigned int) num * (unsigned int) nbytes);
*******/
   p = malloc((unsigned int) num * (unsigned int) nbytes);
   if (p) return(p);

   fprintf(stderr,"no more memory.\n");
   menu_macro( NULL, K_QUIT, NULL); /* uscita dall'applicativo */
}

/*** char *realloc_mem(ptr, num, nbytes)
 *** Parametri:
 ***     ptr : puntatore all'area di memoria precedentemente allocata
 ***     num : numero di celle da allocare
 ***   nbytes: dimensione della singola cella
aumenta la dimensione della memoria precedentemente allocata ***/
char *realloc_mem(ptr, num, nbytes)
char *ptr;
int num;
int nbytes;
{
   char *p;

   if ( num*nbytes == 0 )
      return(NULL);
/**** LORENZO
   p = XtRealloc(ptr, (unsigned int)nbytes * (unsigned int) num);
*******/
   p = realloc(ptr, (unsigned int)nbytes * (unsigned int) num);
   if (p) return(p);

   fprintf(stderr,"no more memory.\n"); /* uscita dall'applicativo */
   menu_macro( NULL, K_QUIT, NULL);
}

/* Piatto del giorno 10 Dic. 1992: Arelle in agrodolce.			*/

/* fine files.c */
