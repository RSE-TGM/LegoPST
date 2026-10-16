/**********************************************************************
*
*       C Source:               sked_start.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Nov  9 12:36:12 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked_start.c-30 %  (%full_filespec: sked_start.c-30:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked_start.c	5.3\t2/7/96";
/*
   modulo sked_start.c
   tipo
   release 5.3
   data 2/7/96
   reserved @(#)sked_start.c	5.3
*/
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
#ifndef LINUX
# include <siginfo.h>
#endif
# include <sys/types.h>
#include <sys/wait.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
#ifndef LINUX
# include <sys/mode.h>
#else
# include <sys/stat.h>
#endif
# include <errno.h>
# include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "sked.h"
# include "sked_fun.h"
# include "libmanovra.h"
# include "demone.h"
# include "f22_circ.h"

#if defined SCO_UNIX
#define vfork() fork()
#endif
extern char **environ;    /* Di sistema */
extern int      shr_usr_key;	/* chiave utente per shared  */

extern char     nodo_master[MAX_LUN_HOST];	/* nome del nodo MASTER   */

extern S02      s02_;		/* struttura modelli e var  */
COLLEGAMENTO    collegamento[MAX_MODEL][MAX_MODEL];
/*
 * tabella contente i collegamenti fra le task       
 */
SCAMBIO         scambio[MAX_MODEL][MAX_DT_DEC];
/*
 * tabella contente gli scambi fra le task       
 */
int             nrip;		/* numero di ripetizioni della tabella di
				 * ripetizione */
int             nmod;		/* numero delle task */

int             tot_snap;	/* numero degli snapshot  */

int             pidgr;		/* identificatore processo net_prepf22  */

extern int      demone_attivo;

int             pidsnap;	/* identificatore processo net_snap  */
int             piddemone;	/* identificatore pid processo demone */

int             pid_bm_master;	/* identificatore pid processo agg_manovra
				 * MASTER   */
int             pid_bm_stato;	/* identificatore pid processo
				 * agg_manovra_stato    */
int             pid_bm_slave;	/* identificatore pid processo agg_manovra
				 * SLAVE    */
int             pid_bm_pert;	/* identificatore pid processo
				 * agg_manovra_pert     */
int             pid_scada;	/* identificatore pid processo agg_scada            */

char            host[MAX_LUN_HOST];
char            host_bm_master[MAX_LUN_HOST];

extern int      fp_com[MAX_MODEL];	/* 0 -> task sulla stessa macchina 1
					 * -> task su macchine diverse fp ->
					 * socket scrittura */
extern int      id_msg_prep;
extern int      id_msg_snap;
int             id_msg_taskbm;
int             id_msg_taskscada;

extern int      id_msg_task[MAX_MODEL];
extern int      id_msg_rettask;

extern int      tipo_sked;	/* definisce il tipo di sked SLAVE=0 MASTER=1          */

extern float    velocita;
extern float    passo_grafica;
extern float    passo_legograf;

extern RtDbPuntiOggetto dbpunti;

int TimeoutTask;

int TimeAggStat;

int assegna_default;

pid_t pid_task[MAX_MODEL];

int ind_task_rest = -1;
int exit_sim = 0;

char ld_library_path_save[FILENAME_MAX];

char **environ_start;    /* Di appoggio: contiene il valore iniziale della
                            variabile di sitema environ                     */

extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;


extern int    stato_sked;		/* Inserito per restart task */
extern double tempo_sim;		/* Inserito per restart task */
extern int    fp_ordini[MAX_MODEL];

int ParAttesa();

static void update_envir(char**,char[][FILENAME_MAX], char**, int, char*);

static void update_envir(char **envir, char envp[][FILENAME_MAX], char** task_envp, int env_idx, char* reinit)
{
// printf("-----------------------> DEBUG update_envir: inizio - N001=%s \n",getenv("N001"));
  // Copia le variabili d'ambiente esistenti in task_envp, saltando LD_LIBRARY_PATH
   for (char **env = envir; *env != NULL && env_idx < MAXENVVAR - 10; ++env) { // -10 per lasciare spazio
// printf("-----------------------> DEBUG update_envir: N001=%s *env=%s\n",getenv("N001"), *env);
       if (strncmp(*env, "LD_LIBRARY_PATH=", strlen("LD_LIBRARY_PATH=")) == 0) {
           printf("update_envir: Rimuovo LD_LIBRARY_PATH: %s \n", *env);
           // Salva il valore se necessario
           char* ld_path_value = index(*env, '=');
           if (ld_path_value != NULL) {
               strcpy(ld_library_path_save, ld_path_value + 1);
               printf("update_envir: ld_library_path_save = %s \n", ld_library_path_save);
           }
       } else {
           // Copia la stringa nel buffer envp e il puntatore in task_envp
           if (strlen(*env) < FILENAME_MAX) {
// printf("-----------------------> DEBUG update_envir: N001=%s *env=%s env_idx=%d\n",getenv("N001"), *env, env_idx);
            strcpy(envp[env_idx], *env);
// printf("-----------------------> DEBUG update_envir: DOPO strcoy N001=%s *env=%s env_idx=%d\n",getenv("N001"), *env, env_idx);

            task_envp[env_idx] = envp[env_idx];
               env_idx++;
           } else {
               printf("WARNING: Variabile d'ambiente troppo lunga: %s\n", *env);
           }
       }
   }

   // Aggiungi le tue variabili specifiche
   // Assicurati che FILENAME_MAX sia sufficiente per queste stringhe!
   //
   // commento NUMMOD, l'ho già messo prima di chiamare questa function update_envir
   // sprintf(envp[env_idx], "NUMMOD=%d", nmod); // Rimuovi \00, sprintf aggiunge il terminatore
   // task_envp[env_idx] = envp[env_idx];
   // env_idx++;

   // Nota: le tue variabili SIM_NUM_MOD, SIM_VAR_INI, WORK_DIR, e il NUOVO LD_LIBRARY_PATH
   // vengono impostate DOPO, quando prepari per ogni singola task.
   // Questo è corretto perché sono specifiche per ogni task.

   sprintf(envp[env_idx], "_MAX_SNAP_SHOT=%d", _MAX_SNAP_SHOT);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_MAX_BACK_TRACK=%d", _MAX_BACK_TRACK);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_MAX_CAMPIONI=%d", _MAX_CAMPIONI);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_NUM_VAR=%d", _NUM_VAR);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_MAX_PERTUR=%d", _MAX_PERTUR);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_SPARE_SNAP=%d", _SPARE_SNAP);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "_PERT_CLEAR=%d", _PERT_CLEAR);
   task_envp[env_idx] = envp[env_idx];
   env_idx++;
   sprintf(envp[env_idx], "REINITTASK=%s", reinit); // appoggio (reinit) era "NO"
   task_envp[env_idx] = envp[env_idx];
   env_idx++;

   // Termina l'array task_envp con NULL
   task_envp[env_idx] = NULL;
   
   return;
   
   // Ora 'task_envp' contiene l'ambiente desiderato per execve,
   // e 'environ' del processo corrente è rimasto intatto.

   // Quando prepari l'ambiente per ogni task specifica (più avanti nel codice):
   // char specific_task_envp_storage[MAXENVVAR][FILENAME_MAX];
   // char *specific_task_envp[MAXENVVAR];
   // int specific_idx = 0;
   // for (int k=0; task_envp[k] != NULL && specific_idx < MAXENVVAR - 5; ++k) {
   //     strcpy(specific_task_envp_storage[specific_idx], task_envp[k]);
   //     specific_task_envp[specific_idx] = specific_task_envp_storage[specific_idx];
   //     specific_idx++;
   // }
   // sprintf(specific_task_envp_storage[specific_idx], "SIM_NUM_MOD=%d", i + 1); specific_task_envp[specific_idx++] = ...;
   // sprintf(specific_task_envp_storage[specific_idx], "SIM_VAR_INI=%d", ip); specific_task_envp[specific_idx++] = ...;
   // sprintf(specific_task_envp_storage[specific_idx], "WORK_DIR=%s", path); specific_task_envp[specific_idx++] = ...;
   // sprintf(specific_task_envp_storage[specific_idx], "LD_LIBRARY_PATH=%s:%s", path_proc, ld_library_path_save); specific_task_envp[specific_idx++] = ...;
   // specific_task_envp[specific_idx] = NULL;
   // execve(task_name, task_argv, specific_task_envp);

// ... (resto del codice sked_start)

}


void restart_task()
{
int             i, j, ip, fpid;
int		jjjj;
int             offset_task;
char            *path;
char            *p_model_dir;
char            *p_path_out;
char            *path_iniziale;
char            path_out[FILENAME_MAX];
char            path_link[FILENAME_MAX];
char            path_proc[FILENAME_MAX];
char            task_name[FILENAME_MAX];
char            path_file_from[FILENAME_MAX];
char            path_file_to[FILENAME_MAX];
char            argv[6][FILENAME_MAX];	/* argomenti per execve */
char            *task_argv[6];
char            envp[MAXENVVAR][FILENAME_MAX];	/* var di env. per execve */
char            *task_envp[MAXENVVAR];
STAT_PROC       *stat_proc;
MESSAGGI_TASK   messaggi_task;	/* messaggi per le task */
char            appoggio[10];
char sicre_appo[200],*sicrep,*sicrepp;


   task_argv[0] = argv[0];
   task_argv[1] = argv[1];
   task_argv[2] = NULL;

   for (i = 0; i < (MAXENVVAR - 1); i++)
      task_envp[i] = envp[i];

   task_envp[MAXENVVAR - 1] = NULL;

   sprintf(task_envp[2], "NUMMOD=%d\00", nmod);

   environ = environ_start;

   strcpy(appoggio,"YES");

i=7; /* Salto le prime 7 celle gi�riempite */


//--- nuova funzione update_envir 
update_envir(environ, envp, task_envp, i, appoggio);

//  codice sostituito ...
// while(*environ)
//    {
//    // elimino la variabile d'ambiente LD_LIBRARY_PATH e salvo temporaneamente il contenuto in ld_library_path_save
//    if ( strncmp(*environ,"LD_LIBRARY_PATH",strlen("LD_LIBRARY_PATH")))
//    {
//       strcpy(task_envp[i],*environ);
//       i++;
//    }
//    else
//    {
//    	printf("restart_task: tolgo LD_LIBRARY_PATH: %s \n", *environ);
// 	strcpy(ld_library_path_save, (index(*environ,'=')+1));
//    	printf("restart_task: ld_library_path_save = %s \n", ld_library_path_save);
//    }

//    *environ++;   /*
//    Controllo che No di var di ambiente non sia > di MAXENVVAR
//    */
//    if(i>MAXENVVAR-10)
//       {
//       printf("ERROR: Too many environment variables in restart_task\n");
//       printf("ERROR: MAXENVVAR =%d  \n",MAXENVVAR); 
//       break;
//       }
//    }
//    sprintf(task_envp[i++], "_MAX_SNAP_SHOT=%d\00", _MAX_SNAP_SHOT);
//    sprintf(task_envp[i++], "_MAX_BACK_TRACK=%d\00",_MAX_BACK_TRACK);
//    sprintf(task_envp[i++], "_MAX_CAMPIONI=%d\00",  _MAX_CAMPIONI);
//    sprintf(task_envp[i++], "_NUM_VAR=%d\00",       _NUM_VAR);
//    sprintf(task_envp[i++], "_MAX_PERTUR=%d\00",    _MAX_PERTUR);
//    sprintf(task_envp[i++], "_SPARE_SNAP=%d\00",    _SPARE_SNAP);
//    sprintf(task_envp[i++], "_PERT_CLEAR=%d\00",    _PERT_CLEAR);
//    sprintf(task_envp[i++], "REINITTASK=%s\00", appoggio);
//    task_envp[i] = NULL;

/*
for (jjjj=0; jjjj<i; jjjj++)
   printf("(task_envp[%d]=%s\n",jjjj,task_envp[jjjj]);
*/

   offset_task = 0;
   path = (char *)malloc(FILENAME_MAX+1);
   path_iniziale = (char *) getcwd((char *) NULL, FILENAME_MAX+1);
   strcpy(path_out,path_iniziale);
   strcat(path_out,"/out");

   for (i = 0; i < nmod; i++)
   {
      if(ind_task_rest == i) {
/*
printf("restart_task: DEBUG ind_task = %d\n",i);
*/
/*
   Settaggio della variabile d'ambiente che dice alla reg000 che la task
   viene riattivata dopo un crash.
*/
         if (fp_com[i] == 0)
         {
	    ip = offset_task;
	    j = s02_.model[i].lun_path;
	    path = (char *) s02_.model[i].path;
/*
 punta al nome della dir. ove risiede il modello:
 serve per poter dare lo stesso nome alla dir. che
 verra' creata a valle di ./out
*/
            p_model_dir= path + (strlen(path)-1); 
/*
 Se il path e' chiuso da slash salta tale terminatore
*/
	    if(*p_model_dir == '/' && (p_model_dir-path)>1) p_model_dir--;
	    while((*p_model_dir)!='/' && (p_model_dir-path)>1)
		p_model_dir--;
            p_model_dir++;
/*
 gestione di eventuale path relativo di posizione task in S01
 In path_proc viene memorizzato il path assoluto della dir ./proc
*/
	    if(path[0]!='/')   /* e' un path relativo */
	    {
	       strcpy(path_proc,path_iniziale);
	       strcat(path_proc,"/");
	       strcat(path_proc,path);
	    }
            else
               strcpy(path_proc,path);
	    strcat(path_proc,"proc");

	    sprintf(task_name, "%s/lg5sk\00", path_proc);
printf("restart_task ------> %s\n",task_name);
	    sprintf(task_argv[0], "%s\00","lg5sk");
	    sprintf(task_argv[1], "%s\00", (char *) s02_.model[i].name);

            sprintf(task_envp[3], "SIM_NUM_MOD=%d\00", i + 1);
            sprintf(task_envp[4], "SIM_VAR_INI=%d\00", ip);
            sprintf(task_envp[5], "WORK_DIR=%s\00", path);
            sprintf(task_envp[6], "LD_LIBRARY_PATH=%s:%s\00", path_proc, ld_library_path_save);
printf("\n LD_LIBRARY_PATH: %s\n",task_envp[6]);

	    RtDbPPutDt(dbpunti,i,s02_.model[i].dt);

/*
   Aggiunge al path ./out il nome della directory ove il  modello
   risiede  e crea la directory medesima
*/
	    strcat(path_out,"/");
	    strcat(path_out,p_model_dir);
printf("PATH restart path_out=%s p_model_dir=%s \n", path_out, p_model_dir);
            if(mkdir(path_out,S_IRWXU|S_IRGRP|S_IROTH))
            {
               perror("\n restart_task: creazione path per files .out");
            }

            strcpy(path_link,path_out);
            strcat(path_link,"/proc");

/*
 se esiste gia' il link simbolico (caduta precedente di una task)
 lo elimina in modo che il prossimo link sia consistente
*/
            unlink(path_link);

/*
 effettua il link simbolico tra la directory proc della task
 ed una dir. proc sotto la directory /out a valle del simulatore
*/

printf("\n LINK SYM tra %s e %s\n",path_proc,path_link);



            if(symlink(path_proc,path_link))
            {
               perror("\n restart_task: creazione symlink con dir. proc");
               exit(1);
            }

            fpid = vfork();
	    if (fpid == 0)
	    {
#ifndef SCO_UNIX
	       sigsetmask(0);	/* sblocca tutti i segnali */
#endif
	       if (chdir(path_out) == (-1))
	       {
	          fprintf(stderr, "restart_task: errore chdir task %d\n", i);
	          perror("-->");
	          _exit(0);
	       }
               strcpy(path_file_from,path_out);
	       strcat(path_file_from,"/lg5.out");
               strcpy(path_file_to,path_out);
	       strcat(path_file_to,"/lg5.out.core");
               rename(path_file_from ,path_file_to); 
	       execve(task_name, task_argv, task_envp);
	       fprintf(stderr, "restart_task: Errori nella creazione Modelli\n");
	       fprintf(stderr, "restart_task: Task n. %d %s", i, s02_.model[i].name);
	       fprintf(stderr, "restart_task: pid  %d  errore n. %d\n\n", getpid(), errno);
	       _exit(0);
	    }
            else
            {
               pid_task[i]=fpid;
            } 
	    chdir(path_iniziale);
	    stat_proc = RtDbPGetPuntTaskStat(dbpunti,i);
	    stat_proc-> pid = fpid;
	/* attesa primo ciclo delle task  */
/*
	    if (msg_rcv(id_msg_rettask, &messaggi_task,
	        sizeof(messaggi_task.mtext), (long)(i + 1),
	        !IPC_NOWAIT, TimeoutTask) < 0)
	    {
	       sked_stato(STATO_ERRORE);
	       sked_errore();
	    }
*/
         }
      }
      offset_task = s02_.model[i].punt_ifine -1 ;
   }

}





void sigchild_handler(int sig, siginfo_t *sip, void *extra)
{
pid_t kid;
int   k;


   if(exit_sim == 0) {
      kid = sip->si_pid;
      for (k=0;k<nmod;k++) {
         if(kid==pid_task[k]) {
            ind_task_rest = k;
            break;
         }
      }
      if(ind_task_rest!=-1) { 
      printf("SIGCHILD_HANDLER: process child pid = %d\n",kid);
/*
   Il processo figlio che ha inviato il segnale e' una delle task
*/
/*
printf("SIGCHILD_HANDLER: sip->si_code = %d\n", sip->si_code);
*/
         if ((sip->si_code == CLD_DUMPED) || (sip->si_code == CLD_KILLED) ||
             (sip->si_code == CLD_EXITED)) {
            printf("SIGCHILD_HANDLER: CLD_DUMPED o CLD_KILLED o CLD_EXITED!!!\n");
            kid = wait(0); 
            printf("SIGCHILD_HANDLER: ret wait = %d ind_task = %d\n",kid,ind_task_rest);
            restart_task();

         }
         else {
            printf("SIGCHILD_HANDLER: OTHER!!!\n");
         }
      }
      else {
         printf("SIGCHILD_HANDLER:segnale da processo figlio != da task!!!\n");
      }
   }

}


void sked_start()
{
   char *p_path_out;
   char *p_model_dir;
   char comando[FILENAME_MAX];
   char path_out[FILENAME_MAX];
   char path_proc[FILENAME_MAX];
   char path_link[FILENAME_MAX];
   int offset_task;
   MSG_AUS         messaggio_aus;
   char           *path;
   char           *path_legort_bin;
   char           *path_legommi_bin;
   char           *path_iniziale;
   char           *app;
   char            task_name[FILENAME_MAX];
   char            argv[6][FILENAME_MAX];	/* argomenti per execve */
   char           *task_argv[6];
   char            envp[MAXENVVAR][FILENAME_MAX];	/* var di env. per execve */
   char           *task_envp[MAXENVVAR];
   int             i, j, ip, fpid, msgsz;
   char           *getenv();	/* funzioni utilizzate           */
   MESSAGGI_TASK   messaggi_task;	/* messaggi per le task */
   STAT_PROC	   *stat_proc;
   int             fp_read[MAX_MODEL];	/* 0 -> task sulla stessa macchina 1
					 * -> task su macchine diverse fp ->
					 * socket lettura */
   int             par_attesa=1;
   char sicre_appo[200],*sicrep,*sicrepp;

   struct sigaction sa_task;
   int             ret;
   char            appoggio[10];
   int		jjjj;

printf("-----------------------> DEBUG sked_start: Inizio - N001=%s \n",getenv("N001"));

   path = (char *) malloc(FILENAME_MAX+1);
   app = (char *) malloc(FILENAME_MAX+1);
   path_legort_bin = (char *) malloc(FILENAME_MAX+1);
   path_legommi_bin = (char *) malloc(FILENAME_MAX+1);

/* Gestione delle variabili d'ambiente di interesse
   Prima di eventuali mascheramenti */

   assegna_default = 1;
   if ( (getenv("SKED_TIMEOUT") != NULL) &&
        (strlen(getenv("SKED_TIMEOUT"))>0) )
     {
     TimeoutTask = atoi( getenv("SKED_TIMEOUT") );
     if( TimeoutTask > 0 )
         {
         fprintf(stderr,"\t SKED_TIMEOUT : %d\n",TimeoutTask);
         assegna_default = 0;
         }
     }
   if ( assegna_default == 1 )
     {
     TimeoutTask = TIMEOUT_TASK;
     fprintf(stderr,
 "\t NON definito o non corretto in env. SKED_TIMEOUT, uso default (%dsec.)\n",
             TIMEOUT_TASK);
     }

   assegna_default = 1;
   if ( (getenv("SKED_TIME_AGG_STAT") != NULL) &&
        (strlen(getenv("SKED_TIME_AGG_STAT"))>0) )
     {
     TimeAggStat = atoi( getenv("SKED_TIME_AGG_STAT") );
     if( TimeAggStat > 0 )
         {
         fprintf(stderr,"\t SKED_TIME_AGG_STAT : %d\n",TimeAggStat);
         assegna_default = 0;
         }
     }
   if ( assegna_default == 1 )
     {
     TimeAggStat = TIME_AGG_STAT;
     fprintf(stderr, "\t NON definito o non corretto in env. SKED_TIME_AGG_STAT, uso default (%dmsec.)\n", TIME_AGG_STAT);
     }


/*
DEBUG
*/
// printf("\t DEBUG **** \n");
   path_legort_bin=getenv("LEGORT_BIN");
   path_legommi_bin=getenv("LEGOMMI_BIN");
// printf("LEGORT=%s\n",path_legort_bin);
// printf("LEGOMMI=%s\n",path_legommi_bin);

   /* Esecuzione lettura S02 */
/***
   sked_s02();
**/
   nrip = s02_.nrip;
   nmod = s02_.nmod;
// printf("Ho letto il file S02 mod = %d rip =%d\n",s02_.nmod,s02_.nrip);

   /* esecuzione startup  */
   sked_startup();

printf("-----------------------> DEBUG sked_start: Dopo Sked_startup - N001=%s \n",getenv("N001"));

   /* codifica variabili */
   task_argv[0] = argv[0];
   task_argv[1] = argv[1];
   task_argv[2] = NULL;

   for (i = 0; i < (MAXENVVAR - 1); i++)
      task_envp[i] = envp[i];

   task_envp[MAXENVVAR - 1] = NULL;

   sprintf(task_envp[2], "NUMMOD=%d\00", nmod);

   environ_start = environ;

   strcpy(appoggio,"NO");

i=7; /* Salto le prime 6 celle già riempite */

printf("-----------------------> DEBUG sked_start: prima di while - N001=%s \n",getenv("N001"));

//--- nuova funzione update_envir 
update_envir(environ, envp, task_envp, i, appoggio);

//  codice sostituito ...
// while(*environ)
//    {
//   /* printf("env[%d]=%s\n",i,*environ);*/
//    // elimino la variabile d'ambiente LD_LIBRARY_PATH e salvo temporaneamente il contenuto in ld_library_path_save
// printf("-----------------------> DEBUG sked_start: DENTRO while - N001=%s *environ=%s\n",getenv("N001"), *environ);

//    if ( strncmp(*environ,"LD_LIBRARY_PATH",strlen("LD_LIBRARY_PATH")))
//    {
//       strcpy(task_envp[i],*environ);
//       i++;
//    }
//    else
//    {
//    	printf("sked_start: tolgo LD_LIBRARY_PATH: %s \n", *environ);
// 	strcpy(ld_library_path_save, (index(*environ,'=')+1));
//    	printf("sked_start: ld_library_path_save = %s \n", ld_library_path_save);		   
//    }   

//    *environ++;
//    /*
//    Controllo che No di var di ambiente non sia > di MAXENVVAR
//    */
//    if(i>MAXENVVAR-10)
//       {
//       printf("ERROR: Too many environment (%d) variables in sked_start\n",i );
//       printf("ERROR: MAXENVVAR =%d  \n",MAXENVVAR);
//       break;
//       }
//    }
//    sprintf(task_envp[i++], "_MAX_SNAP_SHOT=%d\00", _MAX_SNAP_SHOT);
//    sprintf(task_envp[i++], "_MAX_BACK_TRACK=%d\00",_MAX_BACK_TRACK);
//    sprintf(task_envp[i++], "_MAX_CAMPIONI=%d\00",  _MAX_CAMPIONI);
//    sprintf(task_envp[i++], "_NUM_VAR=%d\00",       _NUM_VAR);
//    sprintf(task_envp[i++], "_MAX_PERTUR=%d\00",    _MAX_PERTUR);
//    sprintf(task_envp[i++], "_SPARE_SNAP=%d\00",    _SPARE_SNAP);
//    sprintf(task_envp[i++], "_PERT_CLEAR=%d\00",    _PERT_CLEAR);
//    sprintf(task_envp[i++], "REINITTASK=%s\00",    appoggio);
//    task_envp[i] = NULL;
//  FINE codice sostituito ...


/*
for (jjjj=0; jjjj<i; jjjj++)
   printf("(task_envp[%d]=%s\n",jjjj,task_envp[jjjj]);
*/

printf("-----------------------> DEBUG sked_start: lancio processi N001=%s N007=%s \n",getenv("N001"),getenv("N007"));
/* 
       lancio processi 
   1)  agg_manovra       MASTER   hostbm
   2)  agg_manovra_stato hostbm
*/
#if defined BANCO_MANOVRA
   /*
    * creazione coda messaggi con agg_manovra SLAVE 
    */
   id_msg_taskbm = msg_create(shr_usr_key + ID_MSG_TASKBM, 1);
   if ((tipo_sked == MASTER) && (s02_.host_bm[0] != NULL))
   {
      printf("Inizio attivazione agg_manovra\n");
      sprintf(host_bm_master, "%s_master", s02_.host_bm);
      /*********************************************************************
      printf("Invio messaggio datagram a %s\n",host_bm_master);
      do_demone(SLAVE,host_bm_master,host,DEMONE_START_BM,"spare1","spare2");
      *********************************************************************/
      sprintf(task_name, "%s/agg_manovra\00", path_legort_bin);

      for (i=0; i<3; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0],"%s\00", "agg_manovra");
      sprintf(task_argv[1],"%s\00", "MASTER");
      sprintf(task_argv[2], "%s\00", s02_.host_bm);
      task_argv[3] = NULL;
      printf("sked_start: attivazione processo agg_manovra MASTER %s\n", s02_.host_bm);
      pid_bm_master = vfork();
      if (pid_bm_master == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
      stat_proc = RtDbPGetPuntAusStat(dbpunti,AGG_MANOVRA);
      stat_proc-> pid =  pid_bm_master;

      printf("Inizio attivazione agg_manovra_stato\n");
      sprintf(task_name, "%s/agg_manovra_stato\00", path_legort_bin);
      for (i=0; i<3; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0],"%s\00", "agg_manovra_stato");
      sprintf(task_argv[1], "%s\00", s02_.host_bm);
      task_argv[2] = NULL;
      printf("sked_start: attivazione processo agg_manovra_stato %s\n", s02_.host_bm);
      pid_bm_stato = vfork();
      if (pid_bm_stato == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
      stat_proc = RtDbPGetPuntAusStat(dbpunti,AGG_MANOVRA + 2 * MAX_SLAVE + 3);
      stat_proc-> pid =  pid_bm_stato;

   }
#endif

   /* lancio processo agg_manovra SLAVE  */
#if defined BANCO_MANOVRA
   if (s02_.host_bm[0] != NULL)
   {
      printf("Inizio attivazione agg_manovra\n");
      sprintf(task_name, "%s/agg_manovra\00", path_legort_bin);
      for (i=0; i<4; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0], "%s\00","agg_manovra");
      sprintf(task_argv[1],"%s\00", "SLAVE");
      sprintf(task_argv[2], "%s\00", s02_.host_bm);
      sprintf(task_argv[3], "%s\00", nodo_master);
      task_argv[4] = NULL;
      printf("sked_start: attivazione processo agg_manovra SLAVE %s %s\n",
	     s02_.host_bm, nodo_master);
      pid_bm_slave = vfork();
      if (pid_bm_slave == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
   }
   stat_proc = RtDbPGetPuntAusStat(dbpunti,AGG_MANOVRA + 1);
   stat_proc-> pid =  pid_bm_slave;
#endif

   /* lancio processo agg_manovra_pert  */
#if defined BANCO_MANOVRA

   if (s02_.host_bm[0] != NULL)
   {
      printf("Inizio attivazione agg_manovra_pert\n");
      sprintf(task_name, "%s/agg_manovra_pert\00", path_legort_bin);
      for (i=0; i<2; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0], "%s\00","agg_manovra_pert");
      sprintf(task_argv[1], "%s\00", s02_.host_bm);
      task_argv[2] = NULL;
      printf("sked_start: attivazione processo agg_manovra_pert %s\n", s02_.host_bm);
      pid_bm_pert = vfork();
      if (pid_bm_pert == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
      stat_proc = RtDbPGetPuntAusStat(dbpunti,AGG_MANOVRA + MAX_SLAVE + 2);
      stat_proc-> pid =  pid_bm_pert;
   }
#endif

   /* lancio processo agg_scada */
#if defined SCADA

   if ((tipo_sked == MASTER) && (s02_.host_scada[0] != NULL))
   {
      id_msg_taskscada = msg_create(shr_usr_key + ID_MSG_AGG_SCADA, 1);
      printf("Inizio attivazione agg_scada\n");
      printf("per agg_scada LD_LIBRARY_PATH=%s\n",getenv("LD_LIBRARY_PATH"));
      sprintf(task_name, "%s/agg_scada\00", path_legort_bin);
      for (i=0; i<2; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0], "%s\00","agg_scada");
      sprintf(task_argv[1], "%s\00", s02_.host_scada);
      sprintf(task_envp[2], "LD_LIBRARY_PATH=%s:%s\00", path_proc, ld_library_path_save);
      task_argv[3] = NULL;
      printf("sked_start: attivazione processo agg_scada %s\n", s02_.host_scada);
      pid_scada = vfork();
      if (pid_scada == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
   }
#endif

   /* lancio task */
printf("Attivazione task\n");


   offset_task = 0;


/*
 Crea la dir ./out dove verranno inserite una subdirectory per
 ogni modello costituente il simulatore, con nome uguale al
 modello e che verranno utilizzate per i files di output:
 lg5.out e f21.dat
*/
 path_iniziale = (char *) getcwd((char *) NULL, FILENAME_MAX+1);
 strcpy(path_out,path_iniziale);
 strcat(path_out,"/out");
/*
 elimina una eventuale directory /out gia' esistente
*/
/* commentato per utilizzo task non lego 
 sprintf(comando,"rm -rf %s",path_out);
 system(comando);
*/
 if(mkdir(path_out,S_IRWXU|S_IRGRP|S_IROTH))
		{
		perror("\n sked_start: creazione path per files .out - 1");
/*
		exit(1);
*/
		}
 strcat(path_out,"/");
/*
 salvo un puntatore alla posizione dopo ./out nella stringa 
 del path per poi poter aggiungere i nomi dei modelli
*/
 p_path_out= &path_out[strlen(path_out)];

/*
   Settaggio della funzione che deve gestire il segnale SIGCHLD generato
   dalle task in particolare in corrispondenza del crash di una di queste.
*/   
   sa_task.sa_sigaction = sigchild_handler;
   sigemptyset(&sa_task.sa_mask);

/* come primo tentativo includo LINUX nell'else (Villa) */
#ifdef OSF1_40
   sa_task.sa_flags = SA_CLDNOTIFY|SA_SIGINFO;
#else
   sa_task.sa_flags = SA_SIGINFO;
#endif

   ret = sigaction(SIGCHLD, &sa_task, 0);
   if (ret) {
      perror("sigaction");
      exit(1);
   }


   for (i = 0; i < nmod; i++)
    {
      if (fp_com[i] == 0)
      {
	 ip = offset_task;
	 j = s02_.model[i].lun_path;
	 path = (char *) s02_.model[i].path;
/*
 punta al nome della dir. ove risiede il modello:
 serve per poter dare lo stesso nome alla dir. che
 verra' creata a valle di ./out
*/
        p_model_dir= path + (strlen(path)-1); 
/*
 Se il path e' chiuso da slash salta tale terminatore
*/
	if(*p_model_dir == '/' && (p_model_dir-path)>1) p_model_dir--;
	while((*p_model_dir)!='/' && (p_model_dir-path)>1)
		p_model_dir--;
        p_model_dir++;
/*
 gestione di eventuale path relativo di posizione task in S01
 In path_proc viene memorizzato il path assoluto della dir ./proc
*/
	 if(path[0]!='/')   /* e' un path relativo */
		{
		strcpy(path_proc,path_iniziale);
		strcat(path_proc,"/");
		strcat(path_proc,path);
		}
	 else
	 	strcpy(path_proc,path);
	 strcat(path_proc,"proc");

	 sprintf(task_name, "%s/lg5sk\00", path_proc);
printf("------>%s\n",task_name);
printf("-----------------------> DEBUG Sked_start - check di una variable di env.: N001=%s \n",getenv("N001"));
	 sprintf(task_argv[0], "%s\00","lg5sk");

	 sprintf(task_argv[1], "%s\00", (char *) s02_.model[i].name);

	 sprintf(task_envp[3], "SIM_NUM_MOD=%d\00", i + 1);
	 sprintf(task_envp[4], "SIM_VAR_INI=%d\00", ip);
	 sprintf(task_envp[5], "WORK_DIR=%s\00", path);
    sprintf(task_envp[6], "LD_LIBRARY_PATH=%s:%s\00", path_proc, ld_library_path_save);
printf("\n LD_LIBRARY_PATH: %s\n",task_envp[6]);

	 RtDbPPutDt(dbpunti,i,s02_.model[i].dt);
/*
 aggiunge al path ./out il nome della directory ove il  modello
   risiede  e crea la directory medesima
*/
	 
	 strcpy(p_path_out, p_model_dir);
/*
	 if(p_path_out[0]=='$')
		p_path_out[0]='_';
*/
         printf("\n DEBUG: path_out=%s\n",path_out);
         if(mkdir(path_out,S_IRWXU|S_IRGRP|S_IROTH))
		{
		perror("\n sked_start: creazione path per files .out - 2 path_out\n");
/*
		exit(1);
*/
		}

	 strcpy(path_link,path_out);
	 strcat(path_link,"/proc");
/*
 se esiste gia' il link simbolico (caduta precedente di una task)
 lo elimina in modo che il prossimo link sia consistente
*/
	 unlink(path_link);
/*
 effettua il link simbolico tra la directory proc della task
 ed una dir. proc sotto la directory /out a valle del simulatore
*/
printf("\n LINK SYM tra %s e %s\n",path_proc,path_link);
	 
	 
	 if(symlink(path_proc,path_link))
		{
		perror("\n sked_start: creazione symlink con dir. proc");
		exit(1);
		}

printf("-----------------------> DEBUG - sked_sstart -  Prima di vfork (Parent)-  N001=%s \n",getenv("N001"));
	 fpid = vfork();
	if (fpid == 0) // child process, la task
	 {
printf("----------------------> sked_start DEBUG: CHILD: child process pid = %d genitore = %d\n", getpid(), getppid()); 

#ifndef SCO_UNIX
	    sigsetmask(0);	/* sblocca tutti i segnali */
#endif
	    if (chdir(path_out) == (-1))
	    {
	       fprintf(stderr, "sked_start: errore chdir task %d\n", i);
	       perror("-->");
	       _exit(0);
	    }
printf("\n ATTIVA TASK con path %s\n", task_name);
printf(" ATTIVA TASK N001=%s \n",getenv("N001"));
	    execve(task_name, task_argv, task_envp);
	    fprintf(stderr, "sked_start: Errori nella creazione Modelli\n");
	    fprintf(stderr, "sked_start: Task n. %d %s", i, s02_.model[i].name);
	    fprintf(stderr, "sked_start: pid  %d  errore n. %d\n\n", getpid(), errno);
	    _exit(0);
	 } 
   else    // parent process
    {

printf("sked_start: PARENT - pid[%d] = %d\n", i, fpid);
            pid_task[i]=fpid;
    } 
	 chdir(path_iniziale);
	 stat_proc = RtDbPGetPuntTaskStat(dbpunti,i);
	 stat_proc-> pid = fpid;
	 /* attesa primo ciclo delle task  */
	 if (msg_rcv(id_msg_rettask, &messaggi_task,
		     sizeof(messaggi_task.mtext), (long)(i + 1),
		     !IPC_NOWAIT, TimeoutTask) < 0)
	 {
	    sked_stato(STATO_ERRORE);
	    sked_errore();
	 }
      }
   offset_task = s02_.model[i].punt_ifine -1 ;
   }
   free(path_iniziale);
   /* lancio processo net_prepf22  */
   if (tipo_sked == MASTER)
   {
      for (i=0; i<5; i++)
     	 task_argv[i] = argv[i];

      sprintf(task_name, "%s/net_prepf22\00", path_legort_bin);
      sprintf(task_argv[0], "%s\00","net_prepf22");
      sprintf(task_argv[1], "%s\00",ARGV_N_CAMP_CR);
      sprintf(task_argv[2], "%d\00",_MAX_CAMPIONI);
      sprintf(task_argv[3], "%s\00",ARGV_N_VAR_CR);
      sprintf(task_argv[4], "%d\00",_NUM_VAR);
      task_argv[5] = NULL;

      printf("sked_start: attivazione processo PREP_F22\n");
      pidgr = vfork();
      if (pidgr == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
      stat_proc = RtDbPGetPuntAusStat(dbpunti,PREP_F22 - 1);
      stat_proc-> pid = pidgr;
      /* attende lo start-up del prep_f22  */
      par_attesa = ParAttesa();
      par_attesa = 10;
      if (TIMEOUT_SCADUTO == msg_rcv(id_msg_prep, &messaggio_aus, 
                                     sizeof(TIPO_AUS), (long)(PREP_F22 + OFFSET_AUS), 
                                     !IPC_NOWAIT, TIMEOUT_AUS * par_attesa ))
      {
         stat_proc = RtDbPGetPuntAusStat(dbpunti,PREP_F22 - 1);
         stat_proc-> pid =  -1;
	 /* era signal(SIGKILL, pid): non uccideva niente */
	 if (pidgr > 0)
	    kill(pidgr, SIGKILL);
      }
   }
   /* lancio processo demone  per la connessione con l'mmi */

   if ((tipo_sked == MASTER) && (demone_attivo == 1))
   {
      sprintf(task_name, "%s/demone_mmi\00", path_legommi_bin);
      for (i=0; i<2; i++)
     	 task_argv[i] = argv[i];
      sprintf(task_argv[0], "%s\00","demone_mmi");
      sprintf(task_argv[1], "%s\00","MASTER");
      task_argv[2] = NULL;
      printf("sked_start: attivazione processo demone_mmi\n");
      piddemone = vfork();
      if (piddemone == 0)
      {
#ifndef SCO_UNIX
	 sigsetmask(0);		/* sblocca tutti i segnali */
#endif
	 execve(task_name, task_argv, task_envp);
	 _exit(0);
      }
   }


#if defined BANCO_MANOVRA
   /* attesa termine start-up banco manovra */
   if (pid_bm_master > 0)
      if (TIMEOUT_SCADUTO == msg_rcv(id_msg_taskbm, &messaggio_aus, 
                                     sizeof(TIPO_AUS), (long)(AGG_MANOVRA+OFFSET_AUS),
                                     !IPC_NOWAIT, 1000))
      {
         stat_proc = RtDbPGetPuntAusStat(dbpunti,AGG_MANOVRA);
         stat_proc-> pid =  -1;
	 /* era signal(SIGKILL, pid): non uccideva niente */
	 if (pid_bm_master > 0)
	    kill(pid_bm_master, SIGKILL);
      }
#endif
#if defined SCADA
   /* attesa termine start-up scada */
   printf("attesa termine start-up scada\n");
   if (pid_scada > 0)
      if (TIMEOUT_SCADUTO == msg_rcv(id_msg_taskscada,
				     &messaggio_aus, sizeof(TIPO_AUS),
                                     (long)(AGG_SCADA + OFFSET_AUS), !IPC_NOWAIT, 1000))
      {
	 signal(SIGKILL, (__sighandler_t)pid_scada);
      }
   printf("Start-up scada terminato\n");
#endif
}


int ParAttesa()
{
int ret;
double fret;
double coef=10.0/7000000.0;
int num=_MAX_CAMPIONI * _NUM_VAR;

   fret = coef * num; 
   /*printf("\t\t ParAttesa coef=%f num=%d prod=%f\n",
              coef,num,fret);*/
   fret += 1;
   if( fret> 10 )
       fret=11;
   return((int)fret);
}



