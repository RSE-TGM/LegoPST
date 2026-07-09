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
static char SccsID[] = "@(#)uni_mis.c	5.1\t11/10/95";
/*
   modulo uni_mis.c
   tipo
   release 5.1
   data 11/10/95
   reserved @(#)uni_mis.c	5.1
*/
/*
 *  uni_mis.c
 *      contiene le routines per default unita' di
 *      misura
 *
 *  Il file delle unita' di misura viene cercato da init_umis() in
 *  quest'ordine (la directory corrente e' quella di lancio del tool,
 *  cioe' la directory della simulazione):
 *    1) ./uni_misc.cfg              file TESTO per-simulazione
 *                                   (righe <tipo>=<unita'>, applicate sopra
 *                                   i default compilati; righe non valide
 *                                   segnalate e ignorate)
 *    2) ./uni_misc.dat              binario legacy per-directory
 *                                   (comportamento storico di Alg_mmi)
 *    3) $HOME/defaults/uni_misc.dat binario globale per-utente
 *                                   (creato coi default se assente)
 *  agg_umis() salva SEMPRE sullo stesso file da cui init_umis() ha letto.
 */
#include <stdio.h>      /* For printf and so on. */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <uni_mis_val.h>
#include <ctype.h>
#include "sim_types.h"

//#include <sim_types.h>
 void init_umis();
 void agg_umis();
 int cerca_umis(char*,int);
 int cerca_num_umis();
 int crea_umis_cfg_locale();
 const char *umis_file_attivo();
 int umis_file_is_cfg();

extern void chdefaults();       /* libutil: si posiziona in $HOME/defaults */

#define UMIS_CFG_FILE "uni_misc.cfg"
#define UMIS_DAT_FILE "uni_misc.dat"

/* file attivo delle unita' di misura: determinato da init_umis(),
   usato da agg_umis() per salvare dove si e' letto */
static char umis_path[FILENAME_MAX] = "";
static int  umis_cfg = 0;       /* 1 = formato testo .cfg, 0 = binario .dat */

/* elimina spazi/tab in coda */
static void umis_rtrim(char *s)
{
int l;
for(l=strlen(s); l>0 && (s[l-1]==' '||s[l-1]=='\t'); l--)
        s[l-1]='\0';
}

/*
   legge un file testo <tipo>=<unita'> e applica le selezioni sopra i
   valori correnti della tabella uni_mis (default compilati).
   Ritorna 1 se il file esiste ed e' stato letto, 0 altrimenti.
*/
static int leggi_umis_cfg(const char *path)
{
FILE *fp;
char riga[256],um[L_NOMI_UMIS+1];
char *p,*tipo,*unita;
int i,j,itipo,isel,num_umis;

fp=fopen(path,"r");
if(fp==NULL)
        return(0);
num_umis=cerca_num_umis();
while(fgets(riga,sizeof(riga),fp)!=NULL)
        {
        for(p=riga; *p==' '||*p=='\t'; p++);
        if(*p=='#'||*p=='\n'||*p=='\r'||*p=='\0')
                continue;
        tipo=p;
        p=strchr(p,'=');
        if(p==NULL)
                {
                fprintf(stderr,"init_umis: riga senza '=' ignorata in %s\n",path);
                continue;
                }
        *p='\0';
        unita=p+1;
        unita[strcspn(unita,"\n\r")]='\0';
        umis_rtrim(tipo);
        while(*unita==' '||*unita=='\t') unita++;
        umis_rtrim(unita);

        itipo=(-1);
        for(i=0;i<num_umis;i++)
                if(strcasecmp(tipo,uni_mis[i].codice)==0)
                        {
                        itipo=i;
                        break;
                        }
        if(itipo<0)
                {
                fprintf(stderr,"init_umis: tipo di misura '%s' sconosciuto in %s (riga ignorata)\n",tipo,path);
                continue;
                }
        isel=(-1);
        for(j=0;j<N_TIPI_UMIS;j++)
                {
                strncpy(um,uni_mis[itipo].codm[j],L_NOMI_UMIS);
                um[L_NOMI_UMIS]='\0';
                umis_rtrim(um);
                if(um[0]=='\0')
                        continue;
                if(strcasecmp(unita,um)==0)
                        {
                        isel=j;
                        break;
                        }
                }
        if(isel<0)
                {
                fprintf(stderr,"init_umis: unita' '%s' non valida per %s in %s (riga ignorata)\n",unita,uni_mis[itipo].codice,path);
                continue;
                }
        uni_mis[itipo].sel=isel;
        }
fclose(fp);
return(1);
}

/*
   scrive la tabella corrente in formato testo <tipo>=<unita'>.
   Ritorna 1 se il file e' stato scritto, 0 altrimenti.
*/
static int scrivi_umis_cfg(const char *path)
{
FILE *fp;
char um[L_NOMI_UMIS+1];
int i,num_umis;

fp=fopen(path,"w");
if(fp==NULL)
        {
        perror("scrivi_umis_cfg: scrittura file unita' di misura");
        return(0);
        }
fprintf(fp,"# LegoPST - unita' di misura della simulazione (per-directory)\n");
fprintf(fp,"# formato: <tipo>=<unita'>  (unita' tra quelle previste per il tipo)\n");
fprintf(fp,"# i tipi non elencati usano il default compilato (MKS)\n");
num_umis=cerca_num_umis();
for(i=0;i<num_umis;i++)
        {
        strncpy(um,uni_mis[i].codm[uni_mis[i].sel],L_NOMI_UMIS);
        um[L_NOMI_UMIS]='\0';
        umis_rtrim(um);
        fprintf(fp,"%s=%s\n",uni_mis[i].codice,um);
        }
fclose(fp);
return(1);
}

/*
   apertura file unita' di misura e lettura dati nella tabella uni_mis;
   ordine di ricerca: vedi commento in testa al modulo. Se nessun file
   esiste crea $HOME/defaults/uni_misc.dat coi default compilati
   (tabella uni_mis, vedi uni_mis_val.h).
   La directory corrente viene ripristinata prima di ritornare.
*/

void init_umis()
{
int i,num_umis;
FILE *fpUMIS;
char dir_lancio[FILENAME_MAX];

num_umis=cerca_num_umis();
if(getcwd(dir_lancio,sizeof(dir_lancio))==NULL)
        dir_lancio[0]='\0';

/* 1) file testo per-simulazione nella directory corrente */
snprintf(umis_path,sizeof(umis_path),"%s/%s",dir_lancio,UMIS_CFG_FILE);
if(leggi_umis_cfg(umis_path))
        {
        umis_cfg=1;
        return;
        }

/* 2) binario legacy nella directory corrente (storico Alg_mmi) */
snprintf(umis_path,sizeof(umis_path),"%s/%s",dir_lancio,UMIS_DAT_FILE);
fpUMIS=fopen(umis_path,"r");
if(fpUMIS!=NULL)
        {
        for(i=0;i<num_umis;i++)
                fread(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
        fclose(fpUMIS);
        umis_cfg=0;
        return;
        }

/* 3) binario globale in $HOME/defaults */
chdefaults();
fpUMIS=fopen(UMIS_DAT_FILE,"r");
if(fpUMIS==NULL)
        {
/*
  se il file non era apribile in lettura lo inizializza con i default
*/
        fpUMIS=fopen(UMIS_DAT_FILE,"w");
        if(fpUMIS == NULL)
                {
                printf("\n init_umis:  errore apertura file unita' di misura\n");
                exit(0);
                }
        for(i=0;i<num_umis;i++)
                fwrite(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
        }
else
        {
        for(i=0;i<num_umis;i++)
                fread(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
        }
fclose(fpUMIS);
umis_cfg=0;
if(getcwd(umis_path,sizeof(umis_path)-strlen(UMIS_DAT_FILE)-2)!=NULL)
        {
        strcat(umis_path,"/");
        strcat(umis_path,UMIS_DAT_FILE);
        }
else
        strcpy(umis_path,UMIS_DAT_FILE);
if(dir_lancio[0]!='\0')
        chdir(dir_lancio);
}

/* agg_umis()
    aggiorna il file delle unita' di misura scrivendo i valori correnti
    sullo stesso file (e nello stesso formato) da cui init_umis() ha letto
*/
void agg_umis()
{
int i,num_umis;
FILE *fpUMIS;

if(umis_path[0]=='\0')
        {
        /* init_umis mai chiamata: comportamento storico (binario in cwd) */
        strcpy(umis_path,UMIS_DAT_FILE);
        umis_cfg=0;
        }
if(umis_cfg)
        {
        scrivi_umis_cfg(umis_path);
        return;
        }
num_umis=cerca_num_umis();
fpUMIS=fopen(umis_path,"w");
if(fpUMIS==NULL)
        {
        perror("agg_umis: scrittura file unita' di misura");
        return;
        }
for(i=0;i<num_umis;i++)
        fwrite(&uni_mis[i],sizeof(S_UNI_MIS),1,fpUMIS);
fclose(fpUMIS);
}

/* crea_umis_cfg_locale()
    scrive la tabella corrente nel file testo per-simulazione
    uni_misc.cfg della directory corrente e lo rende il file attivo
    (i successivi agg_umis salveranno li'). Ritorna 1 se ok.
*/
int crea_umis_cfg_locale()
{
char dir[FILENAME_MAX];

if(getcwd(dir,sizeof(dir))==NULL)
        return(0);
snprintf(umis_path,sizeof(umis_path),"%s/%s",dir,UMIS_CFG_FILE);
umis_cfg=1;
return(scrivi_umis_cfg(umis_path));
}

/* path del file unita' di misura attivo (quello letto da init_umis) */
const char *umis_file_attivo()
{
return(umis_path);
}

/* 1 se il file attivo e' il formato testo per-simulazione */
int umis_file_is_cfg()
{
return(umis_cfg);
}

/*
 * cerca_umis
 *      trova l'indice nella tabella delle unita' di misura in base
 *      al primo carattere della sigla.
 */

int cerca_umis(char *descr, int silent)
// cerca_umis
// Cerca l'indice della unità di misura in base alla descrizione
//
// Se silent è 1 non stampa l'errore se non trova l'unità di misura
// ritorna l'indice della unità di misura trovata, oppure l'ultima
// unità di misura se non trova quella richiesta.
// Se descr è NULL o vuota ritorna 0 come default:
{
int i,num_umis;

    // Verifica parametro valido
    if(descr == NULL || descr[0] == '\0') {
        printf("ERRORE: cerca_umis:  chiamato con descrizione vuota\n");
        return 0; // Ritorna prima unità di misura come default
    }

num_umis=cerca_num_umis();
for(i=0;i<num_umis;i++)
	{
	if(toupper(descr[0]) == (int) uni_mis[i].type)
		return(i);
	}
if (!silent) {fprintf(stderr,"ERRORE: cerca_umis: unità di misura non trovata per '%s', uso default\n", descr);}
return(num_umis-1); /* se non e' stata trovata l'unita' di misura
                       assume come umis l'ultima in tabella che
                       corrisponde al caso di undefined */
}

/*
   ricerca quante unita' di misura sono previste
*/

int cerca_num_umis()
{
int i;

i= sizeof (uni_mis) / sizeof (S_UNI_MIS);
return(i);
}
