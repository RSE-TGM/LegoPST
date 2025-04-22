/**********************************************************************
*
*       C Source:               seleziona_var.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Jul  8 10:51:25 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: seleziona_var.c-2 %  (%full_filespec: seleziona_var.c-2:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)seleziona_var.c	5.1\t11/7/95";
/*
   modulo seleziona_var.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)seleziona_var.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sim_param.h"
#include "sim_types.h"
#include <Rt/RtMemory.h>

SELEZIONE selezione;
extern VARIABILI *variabili;
extern char *ind_sh_top;	/* puntatore inizio shared memory sh_var */
extern int tot_variabili;	/* numero totale delle variabili */

extern FLAG_SEL flag_sel;
extern int nu_var_sel;

char   *nome_modello (char *, int);
char   *nome_blocco(char *,int,int);
int     numero_modelli (char *);
int     numero_blocchi (char *, int);


void scrive_default(nome_file)
   char     *nome_file;
{
   FILE     *fp;
   int       num_modello;
   int       num_bl;
   int       k;

   if (!(fp = fopen(nome_file, "w")))
      exit(fprintf(stderr,"\nimpossibile creare il file %s ", nome_file));

   fprintf(fp, "****\n");
   fprintf(fp, "NOME MODELLO\n");
   fprintf(fp, "****\n");
   fprintf(fp, "NOME SELEZIONE\n");
   fprintf(fp, "AUTORE SELEZIONE\n");
   fprintf(fp, "VERSIONE SELEZIONE\n");
   fprintf(fp, "****\n");

   for (num_modello = 0; num_modello < numero_modelli(ind_sh_top);
	num_modello++)
   {

      fprintf(fp, "MODELLO %s\n", nome_modello(ind_sh_top, num_modello + 1));
      fprintf(fp, "****\n");

      if (flag_sel == SEL_LGPH)
      {
	 for (num_bl = 0; num_bl < numero_blocchi(ind_sh_top, num_modello + 1);
	      num_bl++)
	 {
	    fprintf(fp, "%s\n",
		    nome_blocco(ind_sh_top, num_modello + 1, num_bl + 1));
	    fprintf(fp, "****\n");
	 /* for (k = 0; k < tot_variabili; k++) if (variabili[k].mod ==
	  * num_modello + 1 && variabili[k].blocco == num_bl + 1) fprintf(fp,
	        "%s\n", variabili[k].nome); */ fprintf(fp, "****\n");
	 }

	 fprintf(fp, "****\n");
      }

   }

   fclose(fp);

#if defined UNIX
   if (flag_sel == SEL_REG)
      system("rm -f recorder.rtf");
#endif

#if defined VMS
   if (flag_sel == SEL_REG)
      system("delete/nolog/noconf recorder.rtf;*");
#endif

}






void legge_sel(nome_file)
   char     *nome_file;
{
   FILE     *fp;
   FILE     *fp_bin;
   int       num_var;
   char      c;
   char     *px;
   char      riga[82];
   int       i, k;
   int       num_riga;
   int       k_var;
   int       trovata;
   char      nome_bl[9];
   char      nome_mod[9];
   int       errore_formato=0;
   int       incr_modello=0;
   int       no_rtf=0;

printf("legge_sel: lettura selezione variabili per la grafica dal file %s\n",nome_file);

   k=0;
   while(nome_file[k]!=0)
        if(nome_file[k]==';')
                {
                nome_file[k]=0;
                break;
                }
        else
                k++;

   fp_bin=NULL;

   nu_var_sel = 0;
   if (!(fp = fopen(nome_file, "r")))
   {
      if (!strcmp(nome_file, "recorder.edf") ||
	  !strcmp(nome_file, "varforlgf.edf"))
      {
	 scrive_default(nome_file);
	 if (!(fp = fopen(nome_file, "r")))
	    exit(fprintf(stderr,"\nil file %s non esiste - fine irregolare",
			nome_file));
      }
      else
	 exit(fprintf(stderr,"\nil file %s non esiste - fine irregolare", nome_file));
   }


   if (flag_sel == SEL_REG)
      for (i = 0; i < tot_variabili; i++)
	 variabili[i].typ = NO_GRAF_SEL;
   else
      for (i = 0; i < tot_variabili; i++)
	 variabili[i].typlg = NO_GRAF_SEL;


   if (flag_sel == SEL_REG &&
       !strcmp(nome_file, "recorder.edf") &&
       (fp_bin = fopen("recorder.rtf", "r"))&&
	tempo_file("recorder.rtf","recorder.edf"))
   {
      fread(&selezione, sizeof(SELEZIONE), 1, fp_bin);
      fread(&num_var, sizeof(int), 1, fp_bin);
      for (k = 0; k < num_var; k++)
      {
	 fread(&k_var, sizeof(int), 1, fp_bin);
	 variabili[k_var].typ = GRAF_SEL;
	 nu_var_sel++;
      }
      fclose(fp_bin);
   }
   else
   {
      
      if(fp_bin) 
	fclose(fp_bin);
      no_rtf=1;
      for (num_riga = 1; num_riga < 8; num_riga++)
      {

	 fgets(riga, 81, fp);
	 riga[strlen(riga) - 1] = 0;
	 if (num_riga == 2)
	    sprintf(selezione.nome_mod, "%s", riga);
	 if (num_riga == 4)
	    sprintf(selezione.nome_selezione, "%s", riga);
	 if (num_riga == 5)
	    sprintf(selezione.autore_selezione, "%s", riga);
	 if (num_riga == 6)
	    sprintf(selezione.versione_selezione, "%s", riga);
      }

      for ( ;; )
      {
	 if ( ! fgets(riga, 81, fp) )
           break;

         *( strchr(riga, '\n') ) = '\0';
         strcpy( nome_mod, riga+8 );
	 /* controlla che il formato del file 
	    di seleziona della grafica sia corretto sia corretto  */
         ++incr_modello;
	 if ( strcmp(nome_mod,
                  nome_modello(ind_sh_top,incr_modello)) &&
		 (flag_sel == SEL_REG))
		   {
		   errore_formato=1;
		   fprintf(stderr,
	             "Attenzione errore nel formato del file %s\n",nome_file);
		   }


	 for (;;)
	 {

	    fscanf(fp, "%s\n", riga);

	    if (riga[1] == '*')
	       if (flag_sel == SEL_REG)
		  break;
	       else
	       {
		  fgets(riga, 81, fp);
                  strncpy( nome_bl, riga, 8 );
		  if (riga[1] == '*')
		     break;
		  else
		  {
		     fgets(riga, 81, fp );
		     if (riga[1] == '*')
			continue;
		  }
	       }



	    trovata = 0;
	    for (i = 0; i < tot_variabili; i++)
	    {
	       px = strchr(riga, ' ');
	       if (px != NULL)
		  *px = 0;
	       if ( !strcmp(nome_mod,
                            nome_modello(ind_sh_top,variabili[i].mod) ) &&
	            ( flag_sel == SEL_REG || 
		      !strncmp(nome_blocco(ind_sh_top,variabili[i].mod,
                                           variabili[i].blocco), 
                               nome_bl, 8) ) &&
		    !(strncmp(variabili[i].nome, riga, strlen(riga))))
	       {
		  trovata = 1;
		  if (flag_sel == SEL_REG)
		     variabili[i].typ = GRAF_SEL;
		  else
		     variabili[i].typlg = GRAF_SEL;
		  nu_var_sel++;
		  break;
	       }
	    }
	    if (!trovata)
	       fprintf(stderr,"legge_sel: la variabile %s del modello %s non esiste\n",
		      riga, nome_mod);
	 }
      }
   }
   fclose(fp);
if(errore_formato || no_rtf)
	{
	scrive_sel(nome_file);
	}
/* fine delle procedura  */
}



void scrive_sel(nome_file)
   char     *nome_file;
{
   FILE     *fp;
   FILE     *fp_bin;
   char      c;
   char      riga[80];
   char      dummy[9];
   int       k, j;
   int       num_modello;
   int       num_bl;
   int       out_bin = 0;
   int       num_var = 0;

   if (!(fp = fopen(nome_file, "w")))
      exit(fprintf(stderr,"scrive_sel: impossibile creare il file %s - fine irregolare\n", 
	nome_file));
   if (!strcmp(nome_file, "recorder.edf"))
      out_bin = 1;
   if (out_bin)
      if (flag_sel == SEL_REG && !(fp_bin = fopen("recorder.rtf", "w")))
	 exit(fprintf(stderr,"Impossibile creare *.rtf\n"));

   fprintf(fp, "****\n");
   fprintf(fp, "%s\n", selezione.nome_mod);
   fprintf(fp, "****\n");
   fprintf(fp, "%s\n", selezione.nome_selezione);
   fprintf(fp, "%s\n", selezione.autore_selezione);
   fprintf(fp, "%s\n", selezione.versione_selezione);
   fprintf(fp, "****\n");


   if (flag_sel == SEL_REG)
   {
      if (out_bin)
	{
	 fwrite(&selezione, sizeof(SELEZIONE), 1, fp_bin);
	}

      for (num_modello = 0; num_modello < numero_modelli(ind_sh_top);
	   num_modello++)
      {
	 fprintf(fp, "MODELLO %s\n", nome_modello(ind_sh_top, num_modello + 1));

	 for (k = 0; k < tot_variabili; k++)
	    if ((variabili[k].typ == GRAF_SEL) &&
		(variabili[k].mod == num_modello + 1))
	    {
	       ++num_var;
	       fprintf(fp, "%s\n", variabili[k].nome);
/***************
                        esclusione variabili doppie

                                                         
	       for (j = k + 1; j < tot_variabili; j++)
		  if (variabili[k].addr == variabili[j].addr)
		     variabili[j].typ = NO_GRAF_SEL;
****************************/
	    }
	 fprintf(fp, "****\n");
      }

      if (out_bin)
      {
	 fwrite(&num_var, sizeof(int), 1, fp_bin);
	 for (k = 0; k < tot_variabili; k++)
	    if (variabili[k].typ == GRAF_SEL)
	       fwrite(&k, sizeof(int), 1, fp_bin);
      }
   }
   else
   {

      for (num_modello = 0; num_modello < numero_modelli(ind_sh_top);
	   num_modello++)
      {
	 fprintf(fp, "MODELLO %s\n", nome_modello(ind_sh_top, num_modello + 1));
	 fprintf(fp, "****\n");
	 for (num_bl = 0; num_bl < numero_blocchi(ind_sh_top, num_modello + 1);
	      num_bl++)
	 {
	    fprintf(fp, "%s\n",
		    nome_blocco(ind_sh_top, num_modello + 1, num_bl + 1));
	    fprintf(fp, "****\n");
	    for (k = 0; k < tot_variabili; k++)
	       if (variabili[k].mod == num_modello + 1 &&
		   variabili[k].blocco == num_bl + 1 &&
		   variabili[k].typlg == GRAF_SEL)
		  fprintf(fp, "%s\n", variabili[k].nome);
	    fprintf(fp, "****\n");
	 }
	 fprintf(fp, "****\n");
      }
   }

#if defined UNIX
   if (flag_sel == SEL_LGPH)
      system("touch S04");
#endif

#if defined VMS
   if (flag_sel == SEL_LGPH)
   {
      system("copy  S04. S04.TEMP");
      system("copy  S04.TEMP S04.");
      system("del   S04.TEMP.*");
      system("purge S04.");
   }
#endif

   fclose(fp);
if (out_bin)
	{
	 sleep(1);
	 fclose(fp_bin);
	}
}

void elimina_f22()
{
#if defined VMS
system("delete/nolog/noconf  f22circ.dat;*");
system("delete/nolog/noconf  f22circ.hd;*");
#endif
#if defined UNIX
system("rm -f f22circ.dat");
system("rm -f f22circ.hd");
#endif
}
