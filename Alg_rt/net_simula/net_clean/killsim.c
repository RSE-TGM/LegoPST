/**********************************************************************
*
*       C Source:               killsim.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Fri Aug 29 15:09:38 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: killsim.c-15 %  (%full_filespec: killsim.c-15:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)killsim.c	5.4\t3/14/96";
/*
   modulo killsim.c
   tipo 
   release 5.4
   data 3/14/96
   reserved @(#)killsim.c	5.4
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sim_param.h>
#include <sim_ipc.h>
#include <Cs/Cs.h>
#include <sys/types.h>
#include <sys/stat.h>


#define KGRAPHICS 26
#define MAXRIGA FILENAME_MAX *10



typedef struct strin_st {
  char *stringa;
  int lun_stringa;
  } STRIN_ST;

extern void testata(char *, char *);
static int legge_riga_bin_k(char*,FILE*);
static void separa_str(char *, int , STRIN_ST []);
int elimina_processo(char *, int *, char *);


static int legge_riga_bin_k(char*riga,FILE*fp)
{
 int c, k;

 for (k=0; k<MAXRIGA; k++)
   riga[k] = (char)NULL;

 for (k=0; (c = fgetc( fp)) != '\n'; k++)
  {
  riga[k]=c;
  if(c==EOF) return(0);
  }

 riga[k]=(char)NULL;
 return(1);
}



static void separa_str(char *riga, int nstr, STRIN_ST strin[])
{
char *s;
int i;

for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, " \t");
  if(strin[i].stringa==(char*)NULL)
     {
     strin[i].stringa=(char*)malloc(3);
     sprintf(strin[i].stringa," ");
     strin[i].lun_stringa = strlen(strin[i].stringa);
     }
  else
     {
     strin[i].lun_stringa = ( s ? strlen( s) : 0 );
     }
  s = (char*)NULL; }
}



char riga[MAXRIGA];
char app_riga[MAXRIGA];
char app_proc[MAXRIGA];
char nome[MAXRIGA];
char appoggio[MAXRIGA];




int main()
{
FILE *fp;
int k = 0;
int key_int;
int kk;
int pid;
int memopid;
STRIN_ST strin[15];
char comando[FILENAME_MAX + 20];
char path_tmp[FILENAME_MAX];
char pid_proc[10];
static char *processo[]={
			"net_monit",
			"banco",
			"cad_monit",
			"net_prepf22",
			"lg5sk",
 			"net_sked",
			"cad_sked",
			"demone",
			"net_updbase",
			"net_read",
			"xaing",
			"xstaz",
			"xplot",
			"pf22shr",
			"Xgraf",
			"Mgraf",
			"mandb",
			"acqmandb",
			"dispatcher",
			"net_tast",
			"client_scada",
			"banco_bridge",
			"agg_scada",
			"agg_manovra",
			"agg_manovra_pert",
			"bm",
			"graphics"};
int tot_proc=27;
int key_usr_key;
int tot_code=BASE_TASK+MAX_MODEL;
int tot_sem=NUM_MAX_SEM;
int tot_mem=NUM_MAX_SHM;
int shr_usr_key;
int shr_usr_keyS;
int ret;

struct stat buf;


/* redirezione output  e stampa versione */
testata("killsim",SccsID);

/*
   Il file di appoggio contenente l'output del comando 'ps' e del comando
   'ipcs'viene caricato nella directory 'tmp' dell'utente.
*/ 
strcpy(path_tmp,getenv("HOME"));
strcat(path_tmp,"/tmp");
if(stat(path_tmp,&buf)==-1) {
   fprintf(stderr,"Non esiste la directory d'appoggio:\t%s. \n",path_tmp);
   exit(1);
      }
strcat(path_tmp,"/lista_");
sprintf(pid_proc,"%d",getpid());
strcat(path_tmp,pid_proc);
strcat(path_tmp,".proc");
#ifdef DEBUG
printf("File d'appoggio:\t\n%s\n",path_tmp);
#endif

#if defined AIX 
strcpy(comando,"ps -ef>");
#endif
#if defined OSF1 || defined SCO_UNIX || defined LINUX
strcpy(comando,"ps -e | grep -v net_tast> ");
#endif
#if defined ULTRIX
strcpy(comando,"ps -ax>");
#endif
strcat(comando,path_tmp);
system(comando);
/*
printf("comando:DEBUG:\t\n%s\n",comando);
*/

/* decodifica SHR_USR_KEY */
if((getenv("SHR_USR_KEY")==NULL)||(getenv("SHR_USR_KEYS")==NULL))
	{
	printf("killsim :shared user keys non settate correttamente\n");
	exit(0);
	}
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
shr_usr_keyS = atoi((char *)getenv("SHR_USR_KEYS"));

/*
   alg_respawn: script per il lancio ripetuto del banco; viene eliminato
   per primo 
*/
   elimina_processo("alg_respawn",&memopid,path_tmp);

/*
   net_prep_f22 lo termino per secondo
*/
elimina_processo("net_prepf22",&memopid,path_tmp);

if((fp=fopen(path_tmp,"r"))==NULL) {
   fprintf(stderr,"Elim. processi --> Error opening file:\t\n%s\n",path_tmp);
   exit(1);
}
/*
   Eliminazione degli altri processi
*/
while(1)
	{
	ret = legge_riga_bin_k(riga,fp);
        if( ret==0 )
           break;
        strcpy(appoggio,riga);
	strcpy(app_riga,riga);
#if defined AIX
	separa_str(riga,8,strin);
	strcpy(nome,strin[7].stringa);
        if( !strcmp(processo[KGRAPHICS-1],nome))
            {
            separa_str(appoggio,9,strin);
            if( (strin[8].stringa == NULL)    ||
                (strlen(strin[8].stringa)==0) ||
                (strstr(strin[8].stringa,"f22circ")==NULL) )
                continue;
            }
#endif
#if defined SCO_UNIX
	separa_str(riga,4,strin);
	strcpy(nome,strin[3].stringa);
        if( !strcmp(processo[KGRAPHICS-1],nome))
            {
            separa_str(appoggio,5,strin);
            /*printf("strin[4].stringa =%s|\n",strin[4].stringa);*/
            if( (strin[4].stringa == NULL)    ||
                (strlen(strin[4].stringa)==0) ||
                (strstr(strin[4].stringa,"f22circ")==NULL) )
                continue;
            }
#endif
#if defined ULTRIX  || defined OSF1 || defined LINUX
	separa_str(riga,5,strin);
	strcpy(nome,strin[4].stringa);
/*      graphics : si ri-estrae strin per cercare il pathname di f22circ */
        if( !strcmp(processo[KGRAPHICS-1],nome))
            {
            separa_str(appoggio,6,strin);
            if( (strin[5].stringa == NULL)    || 
                (strlen(strin[5].stringa)==0) ||
                (strstr(strin[5].stringa,"f22circ")==NULL) )
                continue;
            }
#endif
#if defined AIX
        pid=atoi(strin[1].stringa);
#else
	pid=atoi(strin[0].stringa);
#endif
        if( pid!=memopid )
	   for(k=0;k<tot_proc;k++)
		{
		strcpy(app_proc,processo[k]);
#if defined SCO_UNIX
		app_proc[8]=0;
#endif
		if(strstr(app_riga,app_proc)!=NULL)
			{
			sprintf(comando,"kill -9 %d",pid);
			/*
                        printf("app_riga=%s|\n",app_riga);
                        printf("\n comando inviato = %s", comando);
                        */
			system(comando);
			}
		}
	}
fclose(fp);
strcpy(comando,"rm ");
strcat(comando,path_tmp);
system(comando);
/* 
printf("comando DEBUG:\t\n%s\n",comando);
*/

/*
    Eliminazione IPCS
*/
printf("Eliminazione code\n");
/*
   Si scarica l'output del comando 'ipcs' relativo alle code nel file 
   d'appogio.
*/
#if defined LINUX
/*
   Il proprietario si cerca con $(id -un), non con $USER: in un container, in
   un servizio o sotto "env -i" USER non c'e', "grep $USER" diventava "grep"
   senza argomenti, e killsim non cancellava ne' code ne' SHM ne' semafori
   (visto il 2026-10-16 con il bundle FMU in python:3.11-slim).
*/
strcpy(comando,"ipcs -q  | grep \"$(id -un)\" | cut -f 2 -d ' '  > ");
#else
strcpy(comando,"ipcs -q > ");
#endif
strcat(comando,path_tmp);
system(comando);
/*
printf("killsim:DEBUG --> comando=%s\n",comando);
*/
if((fp=fopen(path_tmp,"r"))==NULL) {
   fprintf(stderr,"Elim. code --> Error opening file:\t\n%s\n",path_tmp);
   exit(1);
}

k=0;
while(1)
	{
	ret = legge_riga_bin_k(riga,fp);
        if( ret==0 )
           break;
	strcpy(app_riga,riga);
#if defined LINUX
/* non fa niente */
#else
        separa_str(riga,3,strin);
#if defined AIX || defined OSF1_40 || defined LINUX
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 16);
#else
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 10);
#endif
#endif
#if defined LINUX
/* test fittizio per LINUX      */
        if ( 1 == 1)
#else
        if((key_int>=shr_usr_key) &&
           (key_int<(shr_usr_key+tot_code)))
#endif
           {
#if defined LINUX
// printf("ipcrm msg %s \n", riga);
	   sprintf(comando,"ipcrm msg %s 2>>/dev/null", riga);
#else
	   sprintf(comando,"ipcrm -Q %d 2>>/dev/null",key_int);
#endif
	   system(comando);
        }
        k++;
        } 

printf("Eliminazione coda mmi-client\n");
#if defined LINUX
/* non fa niente */
#else
	sprintf(comando,"ipcrm -Q %d 2>>/dev/null",shr_usr_key+FROM_MMI_MSG);
	system(comando);
#endif
#if defined LINUX
/* non fa niente */
#else
	sprintf(comando,"ipcrm -Q %d 2>>/dev/null",shr_usr_keyS+FROM_MMI_MSGS);
	system(comando);
#endif
fclose(fp);


printf("Eliminazione delle shared memory\n");
#if defined LINUX
strcpy(comando,"ipcs -m  | grep \"$(id -un)\" | cut -f 2 -d ' '  > ");
#else
strcpy(comando,"ipcs -m > ");
#endif
strcat(comando,path_tmp);
system(comando);
/*
printf("killsim:DEBUG --> comando=%s\n",comando);
*/
if((fp=fopen(path_tmp,"r"))==NULL) {
   fprintf(stderr,"Elim. shared mem --> Error opening file :\t\n%s\n",path_tmp);
   exit(1);
}

k=0;
while(1)
	{
	ret = legge_riga_bin_k(riga,fp);
        if( ret==0 )
           break;
        strcpy(app_riga,riga);
#if defined LINUX
/* non fa niente */
#else
        separa_str(app_riga,3,strin);
#if defined AIX || defined OSF1_40 || defined LINUX
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 16);
#else
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 10);
#endif
#endif
#if defined LINUX
/* test fittizio per LINUX      */
        if ( 1 == 1)
#else
        if(((key_int>=shr_usr_key) && (key_int<(shr_usr_key+tot_mem))) ||
           (key_int>=(shr_usr_key+OFFSET_SHM_ALLARMI)))
#endif
           {
#if defined LINUX
printf("ipcrm shm %s \n", riga);
	   sprintf(comando,"ipcrm shm %s 2>>/dev/null", riga);
#else
	   sprintf(comando,"ipcrm -M %d 2>>/dev/null",key_int);
#endif
	   system(comando);
           }
        k++;
        } 
fclose(fp);


// printf("Eliminazione semafori\n");
#if defined LINUX
strcpy(comando,"ipcs -s  | grep \"$(id -un)\" | cut -f 2 -d ' '  > ");
#else
strcpy(comando,"ipcs -s > ");
#endif
strcat(comando,path_tmp);
system(comando);
/*
printf("killsim:DEBUG --> comando=%s\n",comando);
*/
if((fp=fopen(path_tmp,"r"))==NULL) {
   fprintf(stderr,"Elim. semafori --> Error opening file :\t\n%s\n",path_tmp);
   exit(1);
}

k=0;
while(1)
	{
	ret = legge_riga_bin_k(riga,fp);
        if( ret==0 )
           break;
        strcpy(app_riga,riga);
#if defined LINUX
/* non fa niente */
#else
        separa_str(app_riga,3,strin);
#if defined AIX || defined OSF1_40 
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 16);
#else
        key_int=(int)strtol(strin[2].stringa, (char **)NULL, 10);
#endif
#endif
#if defined LINUX
        if( 1==1 ) /* test fittizio per LINUX */
#else
        if((key_int>=(shr_usr_key+OFF_SEM)) && (key_int<(shr_usr_key+OFF_SEM+tot_sem)) ||
           ((key_int>=(shr_usr_keyS+OFF_SEM))))
#endif
           {
#if defined LINUX
// printf("ipcrm sem %s \n",riga);
	   sprintf(comando,"ipcrm sem %s 2>>/dev/null",riga);
#else
	   sprintf(comando,"ipcrm -S %d 2>>/dev/null",key_int);
#endif
	   system(comando);
           }
        k++;
        } 
fclose(fp);

/*
strcpy(comando,"rm ");
strcat(comando,path_tmp);
system(comando);
*/

}



int elimina_processo(char *processo, int *pidproc, char *file_temp)
{
FILE *fp;
int k = 0;
int pid;
STRIN_ST strin[5];
char comando[50];
int ret;


if((fp=fopen(file_temp,"r"))==NULL) {
   fprintf(stderr,"Error opening file:\t\n%s\n",file_temp);
   exit(1);
}
while(1)
	{
        ret = legge_riga_bin_k(riga,fp);
        if( ret==0 )
            break;
/*
printf("elimina_processo: riga = %s\n",riga);
*/
        strcpy(app_riga,riga);
#if defined AIX || defined SCO_UNIX
        separa_str(riga,8,strin);
        strcpy(nome,strin[7].stringa);
#endif
#if defined ULTRIX  || defined OSF1 || defined LINUX
        separa_str(riga,5,strin);
/*
printf("elimina_processo: strin[4].stringa = %s\n",strin[4].stringa);
*/
        strcpy(nome,strin[4].stringa);
#endif
        if( strstr(app_riga,processo)!=NULL )
            {
#if defined AIX
            pid=atoi(strin[1].stringa);
#else
            pid=atoi(strin[0].stringa);
#endif
            *pidproc=pid;
            sprintf(comando,"kill -9 %d",pid);
/*
            printf("nome=%s|  [%d]\n",nome,pid);
            printf("\n comando:",comando);
*/
            system(comando);
            break;
            }
        }
   fclose(fp);
   return(0);
}
