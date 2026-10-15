/**********************************************************************
*
*       C Header:               dispatcher.h
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Sep 26 18:14:09 2005 %
*
**********************************************************************/
/*
   modulo dispatcher.h
   tipo 
   release 5.1
   data 11/16/95
   reserved @(#)dispatcher.h	5.1
*/
#ifndef DISPATCHER_H
#define DISPATCHER_H
/*******************************************************************/
#define COMANDO_SGANCIO		1
#define COMANDO_RUN		2
#define COMANDO_STOP		3
#define COMANDO_FREEZE		4
#define COMANDO_BACKTRACK	5
#define COMANDO_REPLAY		6
#define COMANDO_INITIALIZE	7
#define COMANDO_EDITIC		8
#define COMANDO_LOADIC		9
#define COMANDO_SAVEIC		10
#define COMANDO_REMOVE		11
#define COMANDO_TIMESCALING	12
#define COMANDO_STEPCR		13
#define COMANDO_STEPBT		14
#define COMANDO_STEP            15
#define COMANDO_GO_UP           16
#define COMANDO_AGGANCIO	17
#define COMANDO_LSNAPREG	18
#define COMANDO_LSNAPLOAD	19
#define COMANDO_LBTREG		20
#define COMANDO_LBTLOAD		21
#define COMANDO_LISTACI		22
#define COMANDO_STATO		23
#define COMANDO_EDITSINGLEIC	24
#define COMANDO_EDITSINGLEBT	25
#define COMANDO_EDITBT		26
#define COMANDO_LOADBT		27
#define COMANDO_SAVEBT		28
#define COMANDO_CLEAR 		29
#define COMANDO_EDITPERT 	30
#define COMANDO_REMOVEPERT 	31
#define COMANDO_ALLINEA		32
#define COMANDO_STEPSCALING     33
#define COMANDO_EDITDATA     	34
#define COMANDO_MODIDATA     	35
#define COMANDO_MODIC		36
#define COMANDO_VALIDATEBT      37
#define COMANDO_NEWVARCR        38
#define COMANDO_VARUPD          39
#define COMANDO_CHIUSURA	100
#define DATI_DISPATCHER		500
#define DATI_ASINCRONI		700
#define COMANDO_PERT_STEP_BY_NAME	1000
#define COMANDO_PERT_STEP_BY_ADDR	1001
#define COMANDO_MAXSIMTIME	42


/*
        queste define andranno inserite in sim_ipc.h
*/

#define ID_MSG_TO_MONIT         40
#define ID_MSG_FROM_MONIT       41
#define ID_MSG_TO_BI            42
#define ID_MSG_FROM_BI          43
#define ID_MSG_TO_SUPER         44
#define ID_MSG_FROM_SUPER       45
#define ID_MSG_TO_BM            46
#define ID_MSG_FROM_BM          47
#define ID_MSG_TO_TAST        	48
#define ID_MSG_FROM_TAST        49
#define ID_MSG_TO_SKED          50
#define ID_MSG_FROM_SKED        51
#define ID_MSG_TO_MANDB         52
#define ID_MSG_FROM_MANDB       53


#define TIME_ACK		200
#define NELEM_PACCHETTO		10  /* Numoro di elemento conemporaneamente
                                       spediti, usato per le perturbazioni
                                       tra disp e SD_editpert */

/*  #define BANCO 			1 **/
#define BANCO 			2
#define BI 			1
#define MONIT 			2
#define SUPERVISIONE		3
#define BM 			4
#define TASTIERA 		5
#define SKED 			6
#define TRANSPUTER		7
#define DISPATCHER		8
#define NET_PREPF22_CIRC	9
#define NEW_MONIT		10
#define SNAF14			11	/* sna2f14 (comando snaf14): solo per ControlParam */


/* include per i messaggi fra:

	MONIT <-> SKED
	BANCO <-> SKED

*/

# define MAX_LUN_COMANDI_SKDIS        200    /*lunghezza massima dei comandi*/

struct comando_banco_st {
	char sorgente;
	char dati[MAX_LUN_COMANDI_SKDIS];
		};

typedef struct comando_banco_st COMANDO_SKDIS;

struct msg_banco_st {
        long mtype;
	COMANDO_SKDIS comando_banco;
                };

typedef struct msg_banco_st MSG_SKDIS;

# define MAX_LUN_COMANDI_DISPATCHER    1000    /*lunghezza massima dei comandi*/

struct comando_dispatcher_st {
        int sorgente;
        int tipo;
        int num;
        int num_tot;
        int size;
        char dati[MAX_LUN_COMANDI_DISPATCHER];
                };

typedef struct comando_dispatcher_st COMANDO_DISPATCHER;

struct msg_dispatcher_st {
        long mtype;
        COMANDO_DISPATCHER comando_dispatcher;
                };

typedef struct msg_dispatcher_st MSG_DISPATCHER;

struct msg_ack_st {
        long mtype;
	int ret;
/*
        COMANDO_DISPATCHER comando_dispatcher;
*/
                };

typedef struct msg_ack_st MSG_ACK;

/* elenco dei comandi */

#define SKDIS_STATO		  1
#define SKDIS_STATO_ACK		  2
#define SKDIS_AGGANCIO_MONIT      3
#define SKDIS_AGGANCIO_MONIT_ACK  4
#define SKDIS_SGANCIO_MONIT	  5
#define SKDIS_SGANCIO_MONIT_ACK	  6
#define SKDIS_AGGANCIO_BANCO      7
#define SKDIS_AGGANCIO_BANCO_ACK  8
#define SKDIS_SGANCIO_BANCO	  9
#define SKDIS_SGANCIO_BANCO_ACK	 10
#define SKDIS_INITIALIZE	 11
#define SKDIS_INITIALIZE_ACK	 12
#define SKDIS_RUN		 13
#define SKDIS_RUN_ACK		 14
#define SKDIS_FREEZE	 	 15
#define SKDIS_FREEZE_ACK	 16
#define SKDIS_STEP		 17
#define SKDIS_STEP_ACK		 18
#define SKDIS_BACKTRACK		 19
#define SKDIS_BACKTRACK_ACK	 20
#define SKDIS_REPLAY		 21
#define SKDIS_REPLAY_ACK	 22
#define SKDIS_STOP		 23
#define SKDIS_STOP_ACK		 24
#define SKDIS_TIMESCALING	 25
#define SKDIS_TIMESCALING_ACK	 26
#define SKDIS_STEPCR		 27
#define SKDIS_STEPCR_ACK	 28
#define SKDIS_SET		 29
#define SKDIS_SET_ACK		 30
#define SKDIS_LOADIC		 31
#define SKDIS_LOADIC_ACK	 32
#define SKDIS_SAVEIC		 33
#define SKDIS_SAVEIC_ACK	 34
#define SKDIS_EDITIC		 35
#define SKDIS_EDITIC_ACK	 36
#define SKDIS_REMOVE		 38
#define SKDIS_REMOVE_ACK	 39
#define SKDIS_GO_UP		 40
#define SKDIS_GO_UP_ACK	         41
#define SKDIS_GO_DOWN		 42
#define SKDIS_GO_DOWN_ACK        43

#define SKDIS_AGGANCIO_BI      7
#define SKDIS_AGGANCIO_BI_ACK  8

#define SKDIS_AGGANCIO_BM      44
#define SKDIS_AGGANCIO_BM_ACK  45
#define SKDIS_AGGANCIO_SUPERVISIONE      46
#define SKDIS_AGGANCIO_SUPERVISIONE_ACK  47
#define SKDIS_SGANCIO_BM      48
#define SKDIS_SGANCIO_BM_ACK  49
#define SKDIS_SGANCIO_SUPERVISIONE      50
#define SKDIS_SGANCIO_SUPERVISIONE_ACK  51

#define SKDIS_AGGANCIO_MANDB		52
#define SKDIS_AGGANCIO_MANDB_ACK	53
#define SKDIS_SGANCIO_MANDB		54
#define SKDIS_SGANCIO_MANDB_ACK		55
#define SKDIS_MANDB_SIGNAL		56
#define SKDIS_MANDB_SIGNAL_ACK		57



#define SKDIS_LISTACI			58
#define SKDIS_LISTACI_ACK		59


#define SKDIS_LOADBT		 60
#define SKDIS_LOADBT_ACK	 61
#define SKDIS_SAVEBT		 62
#define SKDIS_SAVEBT_ACK	 63
#define SKDIS_EDITBT	 	 64
#define SKDIS_EDITBT_ACK	 65

#define SKDIS_CLEAR 	 	 64
#define SKDIS_CLEAR_ACK	 	 65

#define SKDIS_EDITPERT 	 	 66
#define SKDIS_EDITPERT_ACK	 67

#define SKDIS_REMOVEPERT 	 68
#define SKDIS_REMOVEPERT_ACK	 69

#define SKDIS_ALLINEA 	 	 70
#define SKDIS_ALLINEA_ACK	 71

#define SKDIS_STEPSCALING  	 72
#define SKDIS_STEPSCALING_ACK	 73

#define SKDIS_STATO_INT           74
#define SKDIS_STATO_INT_ACK               75

#define SKDIS_NEWVARCR            76
#define SKDIS_NEWVARCR_ACK                77

#define SKDIS_VARUPD          78
#define SKDIS_VARUPD_ACK      79

#define SKDIS_VALIDATEBT                 80
#define SKDIS_VALIDATEBT_ACK             81

#define SKDIS_EDITDATA  	 82
#define SKDIS_EDITDATA_ACK	 83

#define SKDIS_MODIDATA  	 84
#define SKDIS_MODIDATA_ACK	 85

#define SKDIS_MODIC		 86
#define SKDIS_MODIC_ACK	     	 87

#define SKDIS_FREEZE_REST	 90

#define SKDIS_OFFSET		 100

#define SKDIS_MAXSIMTIME	 42

/* strutture inserite nel campo dati del COMANDO_SKDIS */

struct statistica_sked_st{
		int stato_sked;
		float tempo_sim;
		float tempo_ritardo;
		float timescaling_sim;
		float stepscaling_sim;
		int   grandezze_mem;
		float passo_reg;    
		float tempi_sked[MAX_SLAVE+1];
		float tempi_task[MAX_MODEL];
		float tempo_cpu;
		int stato_task[MAX_MODEL];
#if defined BANCO_MANOVRA
		int stato_agg_manovra[MAX_SLAVE+2];
		int stato_agg_manovra_pert[MAX_SLAVE+1];
		int stato_agg_manovra_stato;
		int stato_agg_scada;
#endif
		int stato_snapshot;
		int stato_prepf22;
		int operazione_eseguita;
		int num_operazione;
		};

typedef struct statistica_sked_st STATISTICA_SKED;

/*

struct snapshot_sked_st{
		char stat;
		char spare;
		short prog;
		char descr[SNAP_DESC];
		};

typedef struct snapshot_sked_st SNAPSHOT_SKED;
*/

/*******************************************************/
#endif
