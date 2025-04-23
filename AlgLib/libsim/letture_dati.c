/**********************************************************************
*
*       C Source:               letture_dati.c
*       Subsystem:              3
*       Description:
*       %created_by:    mauric %
*       %date_created:  Fri Nov 21 13:08:54 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: letture_dati.c-7 %  (%full_filespec: letture_dati.c-7:csrc:3 %)";
#endif
/*
   modulo letture_dati.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)letture_dati.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)letture_dati.c	5.1\t11/7/95";
/*
        Fine sezione per SCCS
*/
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <string.h>
#include <errno.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include <stdlib.h>
# include "vmsipc.h"
#endif
#include "sim_param.h"
#include "sim_types.h"
#include "libnet.h"
#include "mod_data.h"
#include <Rt/RtMemory.h>


extern S02 s02_;     /* file topologia */

float    (*var)[];/* area shared */


char   *nome_blocco(char *,int,int);

/*
   Determina il nome del modulo LEGO in 8 caratteri.
*/
char *nome_bl8(char *ind_shm_top, int nm, int nb, char *nomemodulo)
{
char app[10];
char tmp[10];
char nomebl[200];
char *ret;
char *punt;
int i;

     strcpy(nomebl,nome_blocco(ind_shm_top,nm,nb));
     i=0;
     while ( nomebl[i]!=' ' && i<8  )
        {
        app[i] = nomebl[i];
        i++;
        }
     app[i] = '\0';
     ret = (char*)calloc(strlen(app)+1,sizeof(char));
     memcpy(ret,app,strlen(app)+1);
/*   Controllo se il modulo e' di processo o regolazione */
     punt = strstr(nomebl,"MODULO");
     strcpy(tmp,"MODULO");
     if (punt==NULL)
        {
        punt = strstr(nomebl,"REGOL.");
        strcpy(tmp,"REGOL.");
        if (punt==NULL)
           {
/*         Non e' un blocco ne' di processo ne' di regolazione */
           nomemodulo[0] = '\0';
           return(NULL); 
           }
        }
     punt+=strlen(tmp) + 1;
     memcpy(nomemodulo,punt,4);
     nomemodulo[4] = '\0';
     return(ret);
}


/*
    In funzione del numero del  modello, dal  numero del blocco, dal
    path del file  f14.dat viene letta per righe la regione dati del
    blocco cercato e memorizzata a partire dal puntatore p_iniz  che
    viene restituito assieme al numero di righe memorizzate.
*/
int leggi_dati_f14(char *ind_shm_top, char *path_f14, int numero_mod,
                   int numero_bl, char **p_iniz,int *nrighe)
{
FILE *fp_f14;
char nome_blf14[9];
char *app;
char nome_4per_lista[5];
char *p_iniz_tmp;
char riga_f14[LUN_RIGA_F14];
char *ret;
int i=0,dim=0,pos_riga_prec;
int size;
short blocco_trovato=FALSE;
int n_bl_md;
int n_md;

     printf("***********************************************\n");
     n_md = numero_modelli(ind_shm_top);
     n_bl_md = numero_blocchi(ind_shm_top,numero_mod);
     if ( (numero_bl<=0)  || (numero_bl>n_bl_md)  ||
          (numero_mod<=0) || (numero_mod>n_md)        )
        {
        fprintf(stderr,"Uscita da leggi_dati: numero_mod=%d  numero_bl=%d\n",
                numero_mod,numero_bl);
        return(-1);
        }
     app = (char*)nome_bl8(ind_shm_top,numero_mod,numero_bl,nome_4per_lista);
     if (app!=NULL)
        {
        strcpy(nome_blf14,app);
        free(app);
        }
     if ( !strlen(nome_blf14) )
        {
        fprintf(stderr,"leggi_dati_f14:nome blocco in SH-m non corretto\n");
        return(-1);
        }
     printf("legggi_dati:\n nomef14=%s|   nomelst=%s|\n",
            nome_blf14,nome_4per_lista);
/*   Posizionamento sul blocco cercato */
     if ( -1==cerca_blocco(path_f14,nome_blf14,&fp_f14) )
        {
        fprintf(stderr,"leggi_dati_f14: blocco non trovato!\n");
        fclose(fp_f14);
        return(-1);
        }
     pos_riga_prec=ftell(fp_f14);
/*   Calcolo il numero di righe della zona dati da memorizzare */
     fgets(riga_f14,LUN_RIGA_F14,fp_f14);
     dim=1;
     while ( (fgets(riga_f14,LUN_RIGA_F14,fp_f14) !=NULL) &&
              strncmp(riga_f14,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14)) &&
             (strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE)))   )
        dim++;
     size = sizeof(riga_f14)*dim;
     *p_iniz=(char*)calloc(1,size);
     *nrighe = dim;
     p_iniz_tmp=(*p_iniz);
     fseek(fp_f14,pos_riga_prec,0);
     fgets(riga_f14,LUN_RIGA_F14,fp_f14);
     memcpy(p_iniz_tmp,riga_f14,sizeof(riga_f14));
     p_iniz_tmp+=sizeof(riga_f14);
     while (    (fgets(riga_f14,LUN_RIGA_F14,fp_f14) !=NULL)
             && strncmp(riga_f14,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14))
             && strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE))                  )
        {
        memcpy(p_iniz_tmp,riga_f14,sizeof(riga_f14));
        p_iniz_tmp+=sizeof(riga_f14);
        }
      p_iniz_tmp=(*p_iniz);
      fclose(fp_f14);
      printf("***********************************************\n");
      return(1);
/*
      printf("***********************************************\n");
      for (i=0;i<dim;i++)
        {
        strncpy(riga_f14,p_iniz_tmp,sizeof(riga_f14));
        printf("%s",riga_f14);
        p_iniz_tmp+=sizeof(riga_f14);
        }
*/
}




/*
    In funzione del numero del  modello, dal  numero del  blocco, dal
    path del file  f14.dat viene letto in memoria a partire da p_iniz
    le righe del blocco cercato che vengono scritte nella corrispondente 
    posizione su un file f14.mom
*/
int scrivi_dati_f14(char *ind_shm_top, char *path_f14, int numero_mod,
                int numero_bl,char *p_iniz,int nrighe)
{
FILE *fp_f14;
char *nome_bl;
char comando[120];
char listmod[100];
char path_f14mom[FILENAME_MAX];
char *punt_idati;
char *punt;
char riga_f14[LUN_RIGA_F14];
char *ret;
char *app;
char nome_blf14[9];
char nome_4per_lista[5];
int i=0,off,ki,lung,pos_riga_prec;
int lnomlist,lnomfile,lnombloc;
int n_md;
int n_bl_md;

     printf("-----------------------------------------\n");
     n_md = numero_modelli(ind_shm_top);
     n_bl_md = numero_blocchi(ind_shm_top,numero_mod);
     if ( (numero_bl<=0)  || (numero_bl>n_bl_md)  ||
          (numero_mod<=0) || (numero_mod>n_md)        )
        {
        fprintf(stderr,"Uscita da scrivi_dati: numero_mod=%d  numero_bl=%d\n",
                numero_mod,numero_bl);
        return(-1);
        }
     app = (char*)nome_bl8(ind_shm_top,numero_mod,numero_bl,nome_4per_lista);
     if (app!=NULL)
        {
        strcpy(nome_blf14,app);
        free(app);
        }
     if ( !strlen(nome_blf14) )
        {
        fprintf(stderr,"scrivi_dati_f14:nome blocco in SH-m non corretto\n");
        return(-1);
        }
     printf("scrivi_dati:\n nome_blf14=%s|   nome_4perlista=%s|\n",
            nome_blf14,nome_4per_lista);
/*   Creo il file path ... f14.mom copiandolo dal file passato  */
     lung = strlen(path_f14);
     i = 1;
     while ( path_f14[lung-i]!='.' )
        i++;
     lung = lung - (i-1);
     strcpy(path_f14mom,path_f14);
     path_f14mom[lung]='\0';
     punt = &path_f14mom[lung];
     strncat(path_f14mom,MOM,3);
     printf("mom=%s|   %d\n",path_f14mom,strlen(path_f14mom));
     comando[0]='\0';
     strcat(comando,"cp ");
     strcat(comando,path_f14);
     strcat(comando," ");
     strcat(comando,path_f14mom);
     system(comando);
/*   Fase di scrittura  */
     punt=p_iniz;
     if ( -1==cerca_blocco(path_f14mom,nome_blf14,&fp_f14) )
        {
        fprintf(stderr,"scrivi_dati_f14: blocco non trovato!\n");
        fclose(fp_f14);
        return(-1);
        }
     off = ftell(fp_f14);
     for (i=0;i<nrighe;i++)
        {
        /* printf("%s",punt); */
        fgets(riga_f14,LUN_RIGA_F14,fp_f14);
        fseek(fp_f14,off,0);
        fputs(punt,fp_f14);
        off+=strlen(riga_f14);
        fseek(fp_f14,off,0);
        punt+=sizeof(riga_f14);
        }
     fclose(fp_f14);
     printf("-----------------------------------------\n");
     return(1);
}


/*
   Cerca nel file specificato la zona con i dati del blocco nome_bl
   posizionandosi alla riga precedente l'inizio della zona dati.   
*/
int cerca_blocco(char *path_f14,char *nome_bl,FILE **ffp_f14)
{
char riga_f14[LUN_RIGA_F14];
char *ret;
int i=0,dim=0,pos_riga_prec;
int size;
short blocco_trovato=FALSE;

     *ffp_f14=fopen(path_f14,"r+");
        if ( *ffp_f14==NULL )
           {
           fprintf(stderr,"cerca_blocco:errore apertura f14 ");
           return(-1);
           }
     while (    (fgets(riga_f14,LUN_RIGA_F14,*ffp_f14) !=NULL)
             && (strncmp(riga_f14,HEADER_SEZ_DATI,strlen(HEADER_SEZ_DATI)))
             && (strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE)))          );
     do
        {
        do
           {
           pos_riga_prec=ftell(*ffp_f14);
           }
        while (    ( (ret=fgets(riga_f14,LUN_RIGA_F14,*ffp_f14)) !=NULL)
                && strncmp(riga_f14,HEADER_BLOCCO_F14,strlen(HEADER_BLOCCO_F14))
                && strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE))  );
        if ( !strncmp(&(riga_f14[OFF_NOME_BLOC_DATI]),nome_bl,strlen(nome_bl)) )
              blocco_trovato = TRUE;
        }
     while(     strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE))
             && !blocco_trovato
             && ( (ret=fgets(riga_f14,LUN_RIGA_F14,*ffp_f14)) !=NULL )  
             && ( strncmp(&(riga_f14[OFF_NOME_BLOC_DATI]),nome_bl,
                          strlen(nome_bl)) )                              );
     if ( !strncmp(riga_f14,FINE_FILE,strlen(FINE_FILE)) || ret==NULL )
        {
        printf("cerca_blocco : ATTENZIONE FINE FILE\n");
        return(-1);
        }
     fseek(*ffp_f14,pos_riga_prec,0);
     return(1);
}


/*
   Legge il file dati_var.edf e scrive in SHM i valori dei dati
   gli spiazzamenti opportuni.
*/
int legge_dati_var(char *ind_shm_top, int nmodello, int nblocco)
{
FILE *fp_datiedf;
int off_idati;
int dati_preced=0;
int i;
int j;
float val;
float off;
char comando[50];
int kk=100;

     for (i=1;i<nblocco;i++)
        dati_preced+=numero_dati(ind_shm_top,nmodello,i);
     fp_datiedf=fopen("dati_var.edf","r");
     if ( fp_datiedf==NULL )
        {
        fprintf(stderr,"leggi_dati_var:errore apertura dati_var.edf\n ");
        return(-1);
        }
     printf("off_idati=%d    dati_prec=%d\n",
             s02_.model[nmodello-1].punt_idati,dati_preced);
     printf("I dati sono scritti in Shared memory con questi valori\n");
     printf("Offset dato dalla somma:\n");
     printf("(s02_.model[nmodello-1].punt_idati-1)+(off-1) + dati_preced\n");
     while ( 0 < fscanf(fp_datiedf,"%*s %2d %*s %f %*s %f",
                          &j,&val,&off)   )
       {
       printf("leggi_dati_var.edf (float)=%-20.8f    Off_mod=%f\n",val,
               (off-1) + (float)dati_preced +
               (float)(s02_.model[nmodello-1].punt_idati-1));
/*     Assegnamento in shared memory */
       (*var)[(s02_.model[nmodello-1].punt_idati-1) + ((int)off-1) + 
              dati_preced]= val;
       }
     fclose(fp_datiedf);
/*   Cancello il file dati_var.edf */
     strcpy(comando,"rm dati_var.edf");
     system(comando);
     return(1);
}



/*
   Controlla la presenza del blocco cercato nella lista
   moduli oppure nella lista schemi.
*/
int scelta_lista(char *ind_shm_top, int nm, int nb, char *pf14,
                 char **path_lista_m, char **path_lista_s, 
                 char *n_4per_lista, char *n_8per_f14)
{
FILE *fp_lista;
int i,k;
char *punt;
char *ret;
char riga_f14[LUN_RIGA_F14];
char *app;

     path_liste(pf14, path_lista_m, path_lista_s);
     app = (char*)nome_bl8(ind_shm_top,nm,nb,n_4per_lista);
     if (app!=NULL)
        {
        strcpy(n_8per_f14,app);
        free(app);
        }
     printf("scelta lista:\n n_8per_f14=%s|   n_4per_lista=%s|\n",
            n_8per_f14,n_4per_lista);
     if ( !strlen(n_8per_f14) )
        {
        fprintf(stderr,"scelta_lista:nome blocco in SH-m non corretto\n");
        return(-1);
        }
/*   Cerco il blocco dapprima nella lista moduli poi nella lista schemi*/
     fp_lista=fopen(*path_lista_m,"r");
     if ( fp_lista==NULL )
        exit(fprintf(stderr,"presenza_file:errore apertura lista_moduli.dat"));
     while ( ( ret=fgets(riga_f14,LUN_RIGA_F14,fp_lista)) &&
             ( strncmp(n_4per_lista,riga_f14,4) )                   );
     fclose(fp_lista);
     if (ret != NULL)
        return(BLOCCO_PROCESSO);
     else
        {
        fp_lista=fopen(*path_lista_s,"r");
        if ( fp_lista==NULL )
           exit(fprintf(stderr,"presenza_file:errore apertura lista_schemi.dat"));
        while ( ( ret=fgets(riga_f14,LUN_RIGA_F14,fp_lista)) &&
                ( strncmp(n_4per_lista,riga_f14,4) )                   );
        fclose(fp_lista);
        if (ret != NULL)
           return(BLOCCO_REGOLAZIONE);
        return(-1);
        }
}


/*
   A partire dal path del file f14.dat determina  i
   path della lista moduli (per i moduli di processo)
   e della lista schemi (per i moduli di regolazione).
*/
void path_liste(char *p_f14, char **p_m, char **p_s)
{
char *punt;
char app[FILENAME_MAX];
char p_lista_m[FILENAME_MAX];
char p_lista_s[FILENAME_MAX];

     strcpy(app,p_f14);
     punt = (char*)strstr(app,LEGOCAD_PATH);
     punt+= LUNLEGO;
#if defined UNIX
     strcpy(punt,"/libut/");
#endif
#if defined VMS
     strcpy(punt,".libut]");
#endif
     strcpy(p_lista_m,app);
     strcat(p_lista_m,"lista_moduli.dat");
     p_lista_m[strlen(p_lista_m)] = '\0';
     strcpy(app,p_f14);
     punt = (char*)strstr(app,LEGOCAD_PATH);
     punt+= LUNLEGO;
#if defined UNIX
     strcpy(punt,"/libut_reg/");
#endif
#if defined VMS
     strcpy(punt,".libut_reg]");
#endif
     strcpy(p_lista_s,app);
     strcat(p_lista_s,"lista_schemi.dat");
     p_lista_s[strlen(p_lista_s)] = '\0';
     *p_m = (char*)calloc(strlen(p_lista_m)+1,sizeof(char));
     *p_s = (char*)calloc(strlen(p_lista_s)+1,sizeof(char));
     memcpy(*p_m,p_lista_m,strlen(p_lista_m)+1);
     memcpy(*p_s,p_lista_s,strlen(p_lista_s)+1);
}
/* Fine file letture_dati.c */
