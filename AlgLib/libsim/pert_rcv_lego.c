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
static char SccsID[] = "@(#)pert_rcv_lego.c	5.1\t11/7/95";
/*
   modulo pert_rcv_lego.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)pert_rcv_lego.c	5.1
*/
# include <stdio.h>
#include <string.h>
# include <errno.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#endif
#if defined VMS || defined HELIOS
# include "vmsipc.h"
#endif
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "libnet.h"
#include <Rt/RtMemory.h>


float *converti_float();
int *converti_int();

void pert_rcv_lego(id_msg_pert,fp)
int id_msg_pert;
int fp;
{
int tipo_ritorno;
MSG_PERT messaggio_pert;
PACCHETTO_NET pacchetto_net;
int i=0;

while((msg_rcv(id_msg_pert,&messaggio_pert,sizeof(TIPO_PERT),0,
        !IPC_NOWAIT,TIMEOUT_INF))>0)
        {
             /* spedisce il pacchetto in rete  */
             printf("LEGOGRAF - Ricevuta perturbazione\n");

            /* conversione pacchetto */
             memcpy(&messaggio_pert.perturbazione.tipo,converti_int(
                &messaggio_pert.perturbazione.tipo,TRASMISSIONE),sizeof(int));
             memcpy(&messaggio_pert.perturbazione.indirizzo,converti_int(
                &messaggio_pert.perturbazione.indirizzo,TRASMISSIONE),sizeof(int));
             memcpy(&messaggio_pert.perturbazione.valore_delta,converti_float(
                &messaggio_pert.perturbazione.valore_delta,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.rateo_durata,converti_float(
                &messaggio_pert.perturbazione.rateo_durata,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.period,converti_float(
                &messaggio_pert.perturbazione.period,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.phase,converti_float(
                &messaggio_pert.perturbazione.phase,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.meanvalue,converti_float(
                &messaggio_pert.perturbazione.meanvalue,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.wide,converti_float(
                &messaggio_pert.perturbazione.wide,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.t_null_var,converti_float(
                &messaggio_pert.perturbazione.t_null_var,TRASMISSIONE),sizeof(float));
             memcpy(&messaggio_pert.perturbazione.t,converti_float(
                &messaggio_pert.perturbazione.t,TRASMISSIONE),sizeof(float));

             /* spedizione pacchetto */
             writen(fp,(char*)&(messaggio_pert.perturbazione),sizeof(TIPO_PERT));
        }
}


