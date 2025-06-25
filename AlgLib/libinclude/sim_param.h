/**********************************************************************
*
*       C Header:               sim_param.h
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Aug 23 15:11:39 2006 %
*
**********************************************************************/
#ifndef _sim_param_h_
#define _sim_param_h_

/*
Modifica del 5/12/97 per task-nolego-SICRE
*/
#define N000SICRE              250                    /* MOLTIPLICATORE    */
#define N001SICRE              N000SICRE*10               /* N. MODULI         */
#define N002SICRE              N000SICRE*25               /* N. BLOCCHI        */
#define N003SICRE              N000SICRE*1000               /* N. STATI + ALG. =
                                                   ORDINE MASSIMO
                                                   SISTEMA ALGEBRICO */
#define N004SICRE              N000SICRE*100               /* N. INGRESSI       */
#define N005SICRE              N000SICRE*1100              /* N. VARIABILI      */
/*#define N007              N000*500 */             /* N. dati           */
#define N007SICRE              1000000              /* N. dati           */


/*
Definisco costante per segnalare save snapshot
*/
#define SIGNAL_SAVE_SNAP      -2
#define SIGNAL_LOAD_SNAP      -3



typedef struct
{
   float xy[N003SICRE];
   float uu[N004SICRE];
   float px[N003SICRE];
   float dati[N007SICRE];
   float cnxy[N003SICRE];
   float cnuu[N004SICRE];
   float tstep;
   int ipr;
   int neqsis;
   int nu;
   int neqdif;
   int ndati;
   int ifine;
   int t_proc;
   int jac ;
   int itert ;
   float tsim;
   int tasksicre;
} INTERFACE_VAR;
/*
Viene definita la struttura parsked_st E quindi la struttura PARSKED
La struttura PARSKED e' utilizzata in reg_prolog.c per la variabile
parsked_
Tale struttura e' utilizzata solo in UNIX.
Versione precedente:
        in precedenza la struttura era def. direttam.  in reg_prolog.c

 */
/*
        Struttura per il controllo dei paramentri caratteristici del lego
*/
struct parsked_st
        {
        int kn000;
        int kn001;
        int kn002;
        int kn003;
        int kn004;
        int kn005;
        int kn007;
        int km001;
        int km002;
        int km003;
        int km004;
        int km005;
        };
typedef struct parsked_st PARSKED;



/* ************************ Parametri SKED  ************************ */

#define MAX_SLAVE	    5     /* N. MAX DI SCHEDULATORI SLAVE    */
#if defined SCADA_MMI
#define MAX_MODEL          2     /* N. MAX MODELLI                  */
#else
#define MAX_MODEL          100     /* N. MAX MODELLI                  */
#endif
#define MAX_DT_DEC         20    /* PASSO DI TEMPO MASSIMO AMMESSO
                                     IN DECIMI DI SEC.               */
#define MAX_COLL           50000   /* N. MAX DI INGRESSI CONNESSI
                                     PER CIASCUN MODELLO             */
#define NUM_VAR_SUPP       1500   /* N. VARIABILI SUPPLEM. ALLOCATE
                                     NELLA SEZ. GLOB.                */

#define DIM_SNAP_AUS	    4	  /* Dimensione in byte dell'area
				     ausiliaria allocata in coda ad
				     ogni snapshot il valore deve essere
				     un multiplo di 4 byte */

# define  MAX_LUN_NOME_MODELLO	9

# define  MAX_LUN_NOME_BLOCCO   80
/**************************************************************************
			Inizio modifiche integrazione SCADA-SIMULATORE
	Modifiche di fabio 11/1/97
****************************************************************************/
# define  MAX_LUN_NOME_VARS     33
# define  SIMULAID               0 /*   identificatore origine di una var 
					di tipo varinp SIMULATORE*/
# define  SCADAID                1 /*   identificatore origine di una var
					di tipo varinp SCADA */
/***************************************************************************
			Fine modifiche integrazione SCADA-SIMULATORE
****************************************************************************/

#if defined SCADA_MMI
# define  MAX_LUN_NOME_VAR	21
#else
# define  MAX_LUN_NOME_VAR	9
#endif
# define  MAX_LUN_DESCR_VAR	101

# define  MAX_LUN_MACRO_MOD   10      /* lunghezza massima nome macro modello */

# define  MAX_LUN_HOST         256     /* lunghezza massima nome host */

# define  MAX_LUN_UTENTE       12     /* lunghezza massima nome utente */

# define  MAX_LUN_SISTEMA      12     /*lunghezza massima nome sistema
							operativo*/

# define MAX_LUN_COMANDI	80    /*lunghezza massima dei comandi*/

#define SNAP_DESC        80     /* LUNGHEZZA DESCR. SNAPSHOT         */

#define MAX_VAL_SNAP     8      /* numero di variabili significative */

#define DATA_SNAP       12       /* lunghezza campo data header snapshot */

#define TEMPO_SNAP      12       /* lunghezza campo tempo header snapshot */

#define KP_FRE             NUM_VAR_TOT_MOD + 98 /* PUNTATORE ALLA
                                                   LOCAZIONE DELLA
                                                   SHARED MEMORY DOVE
                                                   VIENE MEMORIZZATO
                                                   IL PASSO DI
                                                   CAMPIONAMENTO DELLA
                                                   GRAFICA.         */

/*
	tipi di task possibili
*/
#define TASK_PROCESSO		0
#define TASK_REGOLAZIONE	1
#define TASK_NOLEGO		2


/*
	Struttura per parametri nuova reg000 -> sync_task
*/
typedef struct {
   float *xy;      /* punt a vettore variabili di uscita/stato */
   float *uu;      /* punt a vettore variabili di ingresso della task */
   float *px;      /* punt a vettore derivate */
   float *dati;    /* punt a elenco dei dati */
   float *cnxy;    /* punt a vettore normalizz. uscite task */
   float *cnuu;    /* punt a vettore normalizz. ingressi task */
   float *tstep;   /* punt a step di integr. */
   int *ipr;       /* punt a tipo di operazione per reg000 */
   int *neqsis;    /* punt a no equazioni sistema */
   int *nu;        /* punt a no ingressi */
   int *neqdif;    /* punt a no eq diff del sist */
   int *ndati;     /* punt a dim vettore dati */
   int *ifine;     /* punt a int per indicare la ifine */
   int *t_proc;
   int *jac;
   int *itert;
   float *tsim;    /* punt a tempo di simulazione */
   int *tasksicre; /* punt a tipo di task */

} SYNC_TASK_PARAM;
void syncronize_sicre_run_time(void);
void initialize_syncronization( INTERFACE_VAR * );
#endif
/***************************************************************************
			Gestione licenze	
****************************************************************************/
#define LICENSE_FULL            "full"   /* Tipologia della licenza: "full";
                                            esecuzione normale             */
#define LICENSE_DEMO            "demo"   /* Tipologia della licenza: "demo";
                                            esecuzione a tempo o con 
                                            funzionalita' limitate         */
#define LICENSE_TIME_FACT       86400.0  /* Numero di secondi in un giorno; 
                                            fattore utilizzato nella gestione
                                            della licenza a tempo          */
#define TIME_CHK_LIC            1.E20   /* Intervallo (in ms) di controllo
                                            dello stato della licenza      */ 
// GUAG205 controllo delle licenza disattivato con TIME_CHK_LIC =1.E20