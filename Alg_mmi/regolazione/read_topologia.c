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
   modulo read_topologia.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)read_topologia.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)read_topologia.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/******************** File: GENF14.C *******************/
/* Generazione del file F14.DAT, partendo dal file.top */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <top_icon.h>


char pag_ds[LUNDESC];
short icn_nm;       
int fl_macro[NICON];         /* Flag (1 se il blocco e` un macromodulo) */
char var_assoc[NICON][NVAR_IN_ICON][9]; /* Nome dei dati fisici e geom. associati */
float val_assoc[NICON][NVAR_IN_ICON];   /* Valore dei dati fisici e geom. */

/* ----------  VARIABILI DI APPOGGIO NELLA LETTURA DEL FILE .reg -----------*/
short pr_ndat[NICON];        /* Numero di dati fisici e geom. del blocco */
float pr_val[NICON][NVAR_IN_ICON];
char pr_nome[NICON][9];             /* Nome del blocco dello schema */
char icn_sigl[NICON][5];            /* Nome della icone del catalogo */
int pr_nuvin[NICON], pr_nuvot[NICON], pr_nuvdt[NICON];
char pr_var[NICON][NVAR_IN_ICON][9];

/* ----------  VARIABILI DI COMODO NELLA LETTURA DEL FILE .reg -----------*/
/*             Servono per avere un unico codice di lettura per
               gen_f14 e gen_topologia                                    */
int short pr_v_fl[NICON][NVAR_IN_ICON];
int short pr_nuvto[NICON];
char pr_desc[NICON][LUNDESC];
char pr_type[NICON][NVAR_IN_ICON][3];
char pr_idvin[NICON][NVAR_IN_ICON][7];
char pr_vds[NICON][NVAR_IN_ICON][LUNDESC];
char pr_viorg[NICON][NVAR_IN_ICON][9];
/* ------------------- PROTOTIPI DELLE FUNZIONI ------------------------- */
/* GUAG2025: Array statico globale per evitare sia stack overflow che allocazione dinamica */
static TOP_ICON global_top_icon_buffer;

/*
   Legge il file nome_file.reg caricando le variabili necessarie 

*/
int read_topologia(char *nome_file)
{
FILE *ff=NULL;
int ret=1;
int i,j,k;
int size;
TOP_ICON *rec;
char stringa[100];
int num_var;

/*  Apertura del file della topologia  */
    if( !(ff=fopen(nome_file,"r")) )
       {
       fprintf(stderr,"read_topologia : Non si puo' aprire il file %s\n",
               nome_file);
       return(-1);
       }

/*  Lettura della descrizione della pagina di regolazione */
    fread(stringa,LUNDESC,sizeof(char),ff);
    if( strlen(stringa)<LUNDESC )
       strcpy(pag_ds,stringa);
    else
       {
       strncpy(pag_ds,stringa,LUNDESC-1);
       pag_ds[LUNDESC] = '\0';
       }

/*  Lettura TOP_ICON, caricamento delle variabili globali corrispondenti */
    /* GUAG2025: Usa buffer statico globale per evitare stack overflow e allocazione dinamica */
    rec = &global_top_icon_buffer;
    
    i = 0;
    while(ret==1)
       {
       ret = fread(rec,sizeof(TOP_ICON),1,ff);
       if(ret!=1)
         break;
       
       /* GUAG2025: Controllo limiti array per evitare buffer overflow */
       if(i >= NICON)
         {
         fprintf(stderr,"ERRORE: Troppi icone nel file %s (max %d)\n", nome_file, NICON);
         fclose(ff);
         return(-1);
         }
         
       icn_nm++;
       pr_nuvto[i] = rec->pr_nuvto;
       pr_nuvin[i] = rec->pr_nuvin;
       pr_nuvot[i] = rec->pr_nuvot;
       pr_nuvdt[i] = rec->pr_nuvdt;
       strcpy(icn_sigl[i],rec->icn_sigl);
       strcpy(pr_nome[i],rec->pr_nome);
                    strcpy(pr_desc[i],rec->pr_desc);
                    
       /* GUAG2025: Controllo limiti variabili per icona */
       if(pr_nuvto[i] > NVAR_IN_ICON)
         {
         fprintf(stderr,"ERRORE: Troppi variabili nell'icona %d del file %s (max %d)\n", i, nome_file, NVAR_IN_ICON);
         fclose(ff);
         return(-1);
         }
         
       for(k=0;k<pr_nuvto[i];k++)
          {
                    pr_v_fl[i][k] = rec->pr_v_fl[k];
                    strcpy(pr_type[i][k],rec->pr_type[k]);
                    strcpy(pr_idvin[i][k],rec->pr_idvin[k]);
                    strcpy(pr_vds[i][k],rec->pr_vds[k]);
                    strcpy(pr_viorg[i][k],rec->pr_viorg[k]);
          strcpy(pr_var[i][k],rec->pr_var[k]);
          /* Ora e' float nel file
          strcpy(stringa,rec->pr_val[k]);
          pr_val[i][k] = (float)atof(stringa);
          */
          pr_val[i][k] = rec->pr_val[k];
/*
          printf("pr_var=%s pr_val=%f (i=%d, k=%d) \n",
                  pr_var[i][k],pr_val[i][k],i,k);
*/
/************* PROVA MARCELLO  */
          if( !strcmp(rec->pr_idvin[k],"INTE") )
             {
             /* debug
             printf("   read_topologia: Resetto il flag della\n   variabile %s collegata con icona di inetrfaccia\n   (icona=%d,var. numero=%d)\n",
                     pr_viorg[i][k],i,k);
             */
             strcpy(pr_var[i][k],pr_viorg[i][k]);
             pr_v_fl[i][k] = 0;
             }
/************* FINE PROVA MARCELLO  */
          }
       fl_macro[i] = rec->fl_macro;
       pr_ndat[i]  = rec->pr_ndat;
/*printf("%d)  pr_nome=%s  pr_ndat=%d\n",i,pr_nome[i],pr_ndat[i]);*/

       /* GUAG2025: Controllo limiti dati per icona */
       if(pr_ndat[i] > NVAR_IN_ICON)
         {
         fprintf(stderr,"ERRORE: Troppi dati nell'icona %d del file %s (max %d)\n", i, nome_file, NVAR_IN_ICON);
         fclose(ff);
         return(-1);
         }
         
       for(k=0;k<pr_ndat[i];k++)
          {
          strcpy(var_assoc[i][k],rec->var_assoc[k]);
          /* Ora e' float nel file
          strcpy(stringa,rec->val_assoc[k]);
          val_assoc[i][k] = (float)atof(stringa);
          */
          val_assoc[i][k] = rec->val_assoc[k];
          }
       i++;
       }

    fclose(ff);
    return(1);
}
