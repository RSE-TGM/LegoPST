/**********************************************************************
*
*       C Source:               monit_aggancia.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 12:27:45 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: monit_aggancia.c-3p1 %  (%full_filespec: monit_aggancia.c-3p1:csrc:1 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)monit_aggancia.c	5.1\t11/7/95";
/*
   modulo monit_aggancia.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)monit_aggancia.c	5.1
*/
# include <stdio.h>
# include <errno.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS
# include <stdlib.h>
# include "vmsipc.h"
#endif
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "sked.h"
# include <Rt/RtMemory.h>

extern FLAG_SEL flag_sel;

int shr_usr_key;                /* chiave utente per shared     */

int stato_sked;

char *nome_file_selezione;  /* nome del file contenente le variabili da
                               registrare per la grafica */

VARIABILI *variabili;          /* database variabili di tutti i modelli */
char *ind_sh_top;          /* puntatore inizio shared memory sh_var */
int id_sh;                 /* identificatore sh_var */
int nmod;
int tot_variabili;

extern SNTAB *snapshot;     /* puntatore alla tabella snapshot  */
#if defined BACKTRACK
extern BKTAB *backtrack;     /* puntatore alla tabella backtrack  */
#endif
extern TIPO_PERT *perturbazioni;     /* puntatore alla tabella pert  */
extern TIPO_PERT *perturbazioni_old;     /* puntatore alla tabella pert  */

extern int _MAX_SNAP_SHOT;
extern int _MAX_BACK_TRACK;
extern int _MAX_CAMPIONI;
extern int _NUM_VAR;
extern int _MAX_PERTUR;
extern int _SPARE_SNAP;


void costruisci_var (char  **, VARIABILI **, int    *);

void monit_aggancia()
{
/* decodifica SHR_USR_KEY */
shr_usr_key = atoi((char *)getenv("SHR_USR_KEY"));

stato_sked=0;

/* creazione o aggancio code messaggi */
msg_create_fam(shr_usr_key,0);

/* aggancio shared memory sh_var */
costruisci_var(&ind_sh_top,&variabili,&id_sh);
/* legge numero dei modelli */
nmod=numero_modelli(ind_sh_top);
tot_variabili=numero_variabili(ind_sh_top);
/* legge la selezioni delle variabili di default */
nome_file_selezione=(char*)malloc(sizeof(char)*40);
/*
strcpy(nome_file_selezione,"recorder.edf");
flag_sel = SEL_REG;
legge_sel(nome_file_selezione);
strcpy(nome_file_selezione,"varforlgf.edf");
flag_sel = SEL_LGPH;
legge_sel(nome_file_selezione);
*/
/* alloca lo spazio per la tabella degli snapshot */
snapshot=(SNTAB*)malloc(sizeof(SNTAB)*_MAX_SNAP_SHOT);
#if defined BACKTRACK
/* alloca lo spazio per la tabella dei backtrack */
backtrack=(BKTAB*)malloc(sizeof(BKTAB)*_MAX_BACK_TRACK);
#endif

/* alloca lo spazio per la tabella delle perturbazioni */
perturbazioni = (TIPO_PERT*) malloc(sizeof(TIPO_PERT)*_MAX_PERTUR*2);
perturbazioni_old = (TIPO_PERT*) malloc(sizeof(TIPO_PERT)*_MAX_PERTUR*2);
}
