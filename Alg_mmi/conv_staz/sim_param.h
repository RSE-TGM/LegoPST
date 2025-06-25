/**********************************************************************
*
*	C Header:		sim_param.h
*	Instance:		5
*	Description:	
*	%created_by:	lomgr %
*	%date_created:	Mon Nov 23 17:30:19 1998 %
*
**********************************************************************/

/* ************************ Parametri LEGO  ************************ */
// GUAG 2025
#define N000              250                    /* MOLTIPLICATORE    */
#define N001              N000*10               /* N. MODULI         */
#define N002              N000*25               /* N. BLOCCHI        */
#define N003              N000*80               /* N. STATI + ALG. =
                                                   ORDINE MASSIMO
                                                   SISTEMA ALGEBRICO */
#define N004              N000*80               /* N. INGRESSI       */
#define N005              N000*180              /* N. VARIABILI      */
#define N007              N000*500              /* N. dati           */

/* *****   M001 = N. TOTALE INGRESSI = N005-N003             ******* */
/* *****   M002 = N. INGRESSI = USCITE = M001-N004           ******* */

#define M001             N005-N003
#define M002             M001
#define M003             N002+1
#define M004             N003+1
#define M005             N004+1


/* ************************ Vecchi Parametri LEGO  ************************ */

// #define N000              160                    /* MOLTIPLICATORE    */
// #define N001              N000*10               /* N. MODULI         */
// #define N002              N000*25               /* N. BLOCCHI        */
// #define N003              N000*60               /* N. STATI + ALG. =
//                                                    ORDINE MASSIMO
//                                                    SISTEMA ALGEBRICO */
// #define N004              N000*25               /* N. INGRESSI       */
// #define N005              N000*100              /* N. VARIABILI      */
// #define N007              N000*500              /* N. dati           */

// /* *****   M001 = N. TOTALE INGRESSI = N005-N003             ******* */
// /* *****   M002 = N. INGRESSI = USCITE = M001-N004           ******* */

// #define M001             N005-N003
// #define M002             M001-N004
// #define M003             N002+1
// #define M004             N003+1
// #define M005             N004+1

/* ************************ Parametri SKED  ************************ */

#define MAX_SLAVE           5     /* N. MAX DI SCHEDULATORI SLAVE    */
#if defined SCADA_MMI
#define MAX_MODEL          2     /* N. MAX MODELLI                  */
#define MAX_ALLOC_MODEL    15000   /* N. DI REAL*4 RISERVATI PER
                                     MODELLO NELLA SHARED MEMORY     */
#else
#define MAX_MODEL          20     /* N. MAX MODELLI                  */
#define MAX_ALLOC_MODEL    5500   /* N. DI REAL*4 RISERVATI PER
                                     MODELLO NELLA SHARED MEMORY     */
#endif
#define MAX_DT_DEC         20    /* PASSO DI TEMPO MASSIMO AMMESSO
                                     IN DECIMI DI SEC.               */
#if defined PIACENZA
#define MAX_COLL           600    /* N. MAX DI INGRESSI CONNESSI
                                     PER CIASCUN MODELLO             */
#endif
#if defined ENEL_CRA
#define MAX_COLL           300    /* N. MAX DI INGRESSI CONNESSI
                                     PER CIASCUN MODELLO             */
#endif
#define NUM_VAR_SUPP       1500   /* N. VARIABILI SUPPLEM. ALLOCATE
                                     NELLA SEZ. GLOB.                */
#define MAX_PERTUR         50     /* N. MAX DI PERTURBAZIONI PRESENTI
                                     CONTEMPORANEAMENTE              */
#define MAX_SNAP_SHOT      60     /* N. MAX DI SNAP_SHOT             */

#if defined BACKTRACK
#define MAX_BACK_TRACK     120    /* N. MAX DI BACK_TRACK            */
#endif

#define DIM_SNAP_AUS        4     /* Dimensione in byte dell'area
                                     ausiliaria allocata in coda ad
                                     ogni snapshot il valore deve essere
                                     un multiplo di 4 byte */

# define  MAX_LUN_NOME_MODELLO  9

# define  MAX_LUN_NOME_BLOCCO   80

#if defined SCADA_MMI
# define  MAX_LUN_NOME_VAR      11
#else
# define  MAX_LUN_NOME_VAR      9
#endif

# define  MAX_LUN_DESCR_VAR     101

# define  MAX_LUN_MACRO_MOD   10      /* lunghezza massima nome macro
                                                                 modello */
# define  MAX_LUN_PATH_MOD    80    /* lunghezza massima del path del
                                       modello   */

# define  MAX_LUN_PATH_SKED    80    /* lunghezza massima del path dello
                                        schedulatore   */

# define  MAX_LUN_HOST         256     /* lunghezza massima nome host */

# define  MAX_LUN_UTENTE       12     /* lunghezza massima nome utente */

# define  MAX_LUN_SISTEMA      12     /*lunghezza massima nome sistema
                                                        operativo*/

# define MAX_LUN_COMANDI        80    /*lunghezza massima dei comandi*/

#define SNAP_DESC        80     /* LUNGHEZZA DESCR. SNAPSHOT         */

#define MAX_VAL_SNAP     8      /* numero di variabili significative */

#define DATA_SNAP       12       /* lunghezza campo data header snapshot */

#define TEMPO_SNAP      12       /* lunghezza campo tempo header snapshot */

#define NUM_VAR_TOT_MOD    MAX_ALLOC_MODEL*MAX_MODEL

#define NUM_VAR_TOT_SHR    (NUM_VAR_TOT_MOD + NUM_VAR_SUPP)

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
#define TASK_PROCESSO           0
#define TASK_REGOLAZIONE        1
#define TASK_NOLEGO             2

