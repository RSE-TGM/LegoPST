/**********************************************************************
*
*       C Source:               cleanlocmmi.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Jul 15 11:24:44 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: cleanlocmmi.c-10 %  (%full_filespec: cleanlocmmi.c-10:csrc:1 %)";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sim_param.h>
#include <sim_ipc.h>
#include <Cs/Cs.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAXRIGA FILENAME_MAX *10

static int legge_riga_bin_2(char *,FILE *);


typedef struct strin_st {
  char *stringa;
  int lun_stringa;
  } STRIN_ST;



int legge_riga_bin_2(riga,fp)
char *riga;
FILE *fp;
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



int separa_str(char *riga, int nstr, STRIN_ST strin[])
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



void elimina_processi_mmi(fp)
FILE *fp;
{
int      k, ret;
char     riga[MAXRIGA];
char     comando[30];
int      pid;
STRIN_ST strin[5];

k=0;
while(1)
   {
   ret = legge_riga_bin_2(riga,fp);
      if( ret==0 )
         break;
   separa_str(riga,2,strin);
#if defined AIX
   pid=atoi(strin[1].stringa);
#else
   pid=atoi(strin[0].stringa);
#endif
   sprintf(comando,"kill -9 %d",pid);
   system(comando);
   k++;
   }

return;
}



int main()
{
int           shr_usr_key, shr_usr_keyS;
char          pid_proc[10];
char          path_tmp[FILENAME_MAX];
char          comando[FILENAME_MAX + 20];
struct stat   buf;
FILE          *fp;
int           k, ret, key_int;
char          riga [MAXRIGA];
STRIN_ST      strin[10];


/* decodifica SHR_USR_KEY e SHR_USR_KEYS */
   if((getenv("SHR_USR_KEY")==NULL)||(getenv("SHR_USR_KEYS")==NULL))
      {
      printf("cleanloc :shared user keys non settate correttamente\n");
      exit(1);
      }
   shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));
   shr_usr_keyS = atoi((char *)getenv("SHR_USR_KEYS"));

/*
   Costruzione del nome del file d'appoggio. In questo file viene scaricato
   l'output dei comandi 'ps' e 'ipcs'.
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
/*
printf("cleanloc DEBUG: path_tmp = %s\n",path_tmp);
*/

/* Eliminazione dei processi client_mmi e client_scada */
printf("Eliminazione processi client_mmi e client_scada\n");
   strcpy(comando,"ps -e | grep client_mmi > ");
   strcat(comando,path_tmp);
   system(comando);
   if((fp=fopen(path_tmp,"r"))==NULL)
      {
      fprintf(stderr,"Elim. processi --> Error opening file:\t\n%s\n",path_tmp);
      exit(1);
      }
   elimina_processi_mmi(fp);
   fclose(fp);
   strcpy(comando,"ps -e | grep client_scada > ");
   strcat(comando,path_tmp);
   system(comando);
   if((fp=fopen(path_tmp,"r"))==NULL)
      {
      fprintf(stderr,"Elim. processi --> Error opening file:\t\n%s\n",path_tmp);
      exit(1);
      }
   elimina_processi_mmi(fp);
   fclose(fp);

/* Eliminazione delle code fra MMI e client */
printf("Eliminazione code mmi-client\n");
#if defined AIX || OSF1_40 || LINUX9
   sprintf(comando,"ipcrm -Q %d 2>>/dev/null",shr_usr_key+FROM_MMI_MSG);
#elif defined  LINUX && !defined LINUX9
   sprintf(comando,"ipcrm msg %d 2>>/dev/null",shr_usr_key+FROM_MMI_MSG);
#endif
   system(comando);
#if defined AIX || OSF1_40 || LINUX9
   sprintf(comando,"ipcrm -Q %d 2>>/dev/null",shr_usr_keyS+FROM_MMI_MSGS);
#elif defined  LINUX && !defined LINUX9
   sprintf(comando,"ipcrm msg %d 2>>/dev/null",shr_usr_keyS+FROM_MMI_MSGS);
#endif
   system(comando);

/* Eliminazione semafori SCADA ed MMI */
printf("Eliminazione semafori scada-mmi\n");
#if defined AIX || OSF1_40 || LINUX9
   sprintf(comando,"ipcrm -S %d 2>>/dev/null",shr_usr_keyS+OFFSET_SEM_SCADA);
#elif defined  LINUX  && !defined LINUX9
   sprintf(comando,"ipcrm sem %d 2>>/dev/null",shr_usr_keyS+OFFSET_SEM_SCADA);
#endif
   system(comando);
#if defined AIX || OSF1_40 || LINUX9
   sprintf(comando,"ipcrm -S %d 2>>/dev/null",shr_usr_keyS+OFFSET_SEM_MMI);
#elif defined  LINUX  && !defined LINUX9
   sprintf(comando,"ipcrm sem %d 2>>/dev/null",shr_usr_keyS+OFFSET_SEM_MMI);
#endif
   system(comando);

/* Eliminazione delle shared memory */
printf("Eliminazione shared memory\n");
   strcpy(comando,"ipcs -m > ");
   strcat(comando,path_tmp);
   system(comando);

   if((fp=fopen(path_tmp,"r"))==NULL)
      {
      fprintf(stderr,"Elim. shr mem --> Error opening file :\t\n%s\n",path_tmp);
      exit(1);
      }
   k=0;
   while(1)
      {
      ret = legge_riga_bin_2(riga,fp);
      if( ret==0 )
         break;
      separa_str(riga,3,strin);
#if defined AIX || OSF1_40 || LINUX
      key_int=(int)strtol(strin[2].stringa, (char **)NULL, 16);
#else
      key_int=(int)strtol(strin[2].stringa, (char **)NULL, 10);
#endif
      if(key_int>=(shr_usr_key+OFFSET_SHM_ALLARMI))
         {
         sprintf(comando,"ipcrm -M %d 2>>/dev/null",key_int);
         system(comando);
         }
      k++;
      }
   fclose(fp);
   strcpy(comando,"rm ");
   strcat(comando,path_tmp);
   system(comando);


}





