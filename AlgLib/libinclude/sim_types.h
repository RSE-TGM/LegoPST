/**********************************************************************
*
*       C Header:               sim_types.h
*       Subsystem:              3
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue May  4 10:33:22 1999 %
*
**********************************************************************/
/*
   modulo sim_types.h
   tipo 
   release 5.2
   data 2/14/96
   reserved @(#)sim_types.h	5.2
*/

#ifndef _sim_types_h_
#define _sim_types_h_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#if defined MFFR
#include "buffer_mffr.h"
#include "snap_sked.h"
#endif

//#include "sked.h"


// struct stato_cr_st
//         {
//         int last_snap_save;
//         int last_snap_load;

//         int last_bktk_save;
//         int last_bktk_load;

//         };


// typedef struct stato_cr_st STATO_CR;
// #define NUM_VAR_GRAFICS 6000
// typedef struct s_dati {
//     char spare[2];                /* per VMS */
//     float t;
//     float mis[NUM_VAR_GRAFICS];
//     } S_DATI;


//#include "sim_param_h"


   /* 
    *  ******
    *  DEFINE
    *  ******
    */
# define  STATO          0    /* per variabili di stato                 */
# define  INGRESSO_NC    1    /* per variabili di ingresso non connesse */
# define  INGRESSO_C     2    /* per variabili di ingresso connesse     */
# define  NO_GRAF_SEL    3    /* per variabili da registare             */
# define  GRAF_SEL       4    /* per variabili da registare             */
# define  NO_GRAF_SEL_MOM 5   /* per variabili da registare             */
# define  GRAF_SEL_MOM   6    /* per variabili da registare             */
# define  VERO           1 
# define  FALSO          0 
# define  ANALOGICO	 0    /* da utilizzare nel campo tipo_var */
# define  DIGITALE	 1    /* dell struttura VARIABILI	  */
#if defined REPLAY
# define  PERT_RUN_TIME  1    /* perturbazione run-time inviata ... */
# define  PERT_REPLAY    2    /* perturbazione inviata durante il replay */
#endif
#define MAX_UPDOWN      50    /* massimo numero di pert UPDOWN contemporanee */
#define NO_ALLINEA     -55    /* indica banco manovra non e' configurato */
#define NUOVA_CI        11    /* indica il caricamento di una nuovo database */
#define AREA_SPARE    4000    /* Dimensione dell'area spare in snap e bktk  */


/*
        Definizioni per il lancio con parametri
        dei processi.
*/
#define ARGV_N_SNAP         "-num_snap"
#define ARGV_N_BKTK         "-num_bktk"
#define ARGV_N_CAMP_CR      "-num_camp_cr"
#define ARGV_N_VAR_CR       "-num_var_cr"
#define ARGV_N_PERT_ACTIVE  "-num_pert_active"
#define ARGV_N_SPARE        "-num_spare_forsnap"
#define ARGV_N_PERTCL       "-clear_pert_bktk"
#define TIME_N_BKTK         "-time_bktk"

/*
        Definizioni per testing parametri Simulator con parametri
        dei file delle registrazioni attive.
*/
#define NOPARAM    26765
#define ERRSTARTUP 3
#define PAREDF      "parametri.out"
#define STARTUP_ERROR "STARTUP ERROR\nActivation and file session parameters are incongruous. Available options :\n1) CONTINUE: continues with file Simulator parameter and\n   resets active recording file which differ.\n2) SHOW DIFFERENCE: displays differences.\n3) EXIT: closes the session."


/*
        Questo file contiene tutte le definizioni e
        i tipi riguardanti le perturbazioni
*/
#define PERT_SCALINO           1
#define PERT_RAMPA             2
#define PERT_RAMPA_STOP        3
#define PERT_IMPULSO           4
#define PERT_IMPULSO_STOP      5
#define PERT_UP                6
#define PERT_DOWN              7
#define PERT_NOT               8
#define PERT_TO                9
#define PERT_TO_STOP          10
#define PERT_PERIODIC         11
#define PERT_PERIODIC_STOP    12
#define PERT_WHITENOISE       13
#define PERT_WHITENOISE_STOP  14
#define PERT_UP_REPLAY        15
#define PERT_MALFUNCTION      16
#define PERT_MALFUNCTION_STOP 17
#define MIN_PERT              PERT_SCALINO
#define MAX_PERT              PERT_MALFUNCTION_STOP


   /* 
    *  *********
    *  STRUTTURE
    *  *********
    */

   /*
    *   1)
    *   La struttura MODEL contiene le informazioni utilizzate run-time dallo
    *   schedulatore per ciascun modello							   
    *   char name[MAX_LUN_NOME_MODELLO];
    *        (nome del modello)
    *   int  lun_name;
    *        (lunghezza del nome del modello)
    *   char descr[65];
    *        (breve descrizione del modello)
    *   char sistema_operativo[MAX_LUN_SISTEMA];
    *        (sistema operativo della macchina dove viene implementato
    *        il modello)
    *   char host[MAX_LUN_HOST];
    *        (nome della macchina dove viene implementato il modello)
    *   char utente[MAX_LUN_UTENTE];
    *        (utente della dove viene implementato il modello)
    *   char path[80];
    *        (path dell'eseguibile del modello (privato di /proc))
    *   int  lun_path;
    *         (lunghezza del path)
    *   float dt;
    *        (passo di integrazione del modello)
    *   int punt_ivus;
    *        (puntatore relativo in shared memory all'inizio delle
    *         variabili di uscita del modello)
    *   int punt_ivin;
    *        (puntatore relativo in shared memory all'inizio delle
    *        variabili di ingresso del modello)
    *   int punt_ider;
    *        (puntatore relativo in shared memory all'inizio delle
    *        derivate del modello)
    *   int punt_idati;
    *        (puntatore relativo in shared memory all'inizio dei
    *        dati del modello)
    *   int punt_ifine;
    *        (puntatore relativo in shared memory alla fine dell'area
    *        utilizzata dalla task)
    *   int num_ic;
    *        (numero degli ingressi connessi del modello)
    *   int punt_ing[MAX_COLL];
    *        (puntatore assoluto ad una variabile del modello che
    *        risulta connessa alla variabile di uscita di un altro modello
    *        ex: (*var)punt_ing[i]=(*var)punt_usc[i])
    *   int punt_usc[MAX_COLL];
    *        (puntatore assoluto ad una variabile di un altro modello
    *        componete il simulatore che deve essere connesso al
    *        modello in oggetto)
    */
struct model_st                  
       {
        char name[MAX_LUN_NOME_MODELLO+3];
        int  lun_name;
        char descr[68];
	char sistema_operativo[MAX_LUN_SISTEMA];
	char host[MAX_LUN_HOST];
	char utente[MAX_LUN_UTENTE];
        char path[80];
        int  lun_path;
        int  tipo_task;
        float dt;
        int punt_ivus;
        int punt_ivin;
        int punt_ider;
	int punt_idati;
	int punt_ifine;
        int num_ic;
        int punt_ing[MAX_COLL];
        int punt_usc[MAX_COLL];
       } ;
typedef struct model_st MODEL ;


   /* 
    *   2)
    *   La struttura COLLEGAMENTO contiene il numero delle variabili collegate 
    *   ed i puntatori all'elenco dei puntatori a queste ultime 
    *   utilizzo: COLLEGAMENTO collegamento vettoriale a 2 dim di MAX_MODEL.
    *   (E' da preferire l'allocazione dimamica)
    */
struct collegamento_st
	{
	int numero_collegamenti;
	int *ingresso;
	int *uscita;
	};
typedef struct collegamento_st COLLEGAMENTO;


   /* 
    *   3)
    *   Il tipo definito SCAMBIO, utilizzato: scambio[MAX_MODEL][MAX_DT_DEC] contiene 
    *    la tabella di svincolo per lo scambio di dati fra i singoli modelli ad ogni 
    *    micro passo di tempo. 
    *    ex: scambio[0][4].read[3]=1 -> il modello 0 deve leggere dei dati
    *   				dal modello 3 a DT=4
    *    (Sarebbe preferibile allocarla dinamicamente)
    */
struct scambio_st
	{
	int read[MAX_MODEL];
	int write[MAX_MODEL];
	};
typedef struct scambio_st SCAMBIO;


   /* 
    *   4)
    *   La struttura TABEL tabel[MAX_MODEL] contiene la tabella per
    *   l'attivazione dei singoli modelli all'interno del passo di simulazione
    *   ex: tabel[0].inst[5] -> il modello 0 deve essere risvegliato a DT=5
    */
struct tabel_st               
       {
        int inst[MAX_DT_DEC];
	int forzatura;
       } ;
typedef struct tabel_st TABEL ;


   /* 
    *   5)
    *   La struttura NOMI viene utilizzata come struttura di appoggio 
    *   per la creazione del file binario variabili.rtf:
    *
    *   char name[MAX_MODEL][MAX_LUN_NOME_MODELLO];
    *        (nomi dei modelli componenti la simulazione)
    *   int nmod;
    *        (numero dei modelli)
    *   int num_blocchi[MAX_MODEL];
    *        (numero dei blocchi di ciascun modello)
    *   char nom_bloc[MAX_MODEL][N002][MAX_LUN_NOME_BLOCCO];
    *        (nomi dei blocchi divisi per modello)
    *   int num_dati_bloc[MAX_MODEL][N002+1];
    *        (numero dei dati del blocco)
    */
struct nomi_st
	{
	 char name[MAX_MODEL][MAX_LUN_NOME_MODELLO];
	 int nmod;
	 int num_blocchi[MAX_MODEL];
#if defined UNIX
	 char **nom_bloc[MAX_MODEL];
         int **num_dati_bloc;
#else
	 char nom_bloc[MAX_MODEL][N002][MAX_LUN_NOME_BLOCCO];
         int num_dati_bloc[MAX_MODEL][N002+1];
#endif
	};
typedef struct nomi_st NOMI;
	 	

   /*
    *   5)
    *   La struttura S02 contiene la prima parte del file binario
    *   topologia.rtf:
    *   int nmod;
    *       (numero dei modelli componenti la simulazione)
    *   int nrip;
    *       (numero di sottopassi nei quali viene suddiviso un
    *        singolo passo di simulazione)
    *   MODEL model[MAX_MODEL];
    *       (vedi struttura MODEL)
    *   TABEL tabel[MAX_MODEL];
    *       (vedi struttura TABEL)
    *   char macro_modello[12];
    *       (nome del modello)
    *   int  lun_macro_modello;
    *       (lunghezza del nome del modello)
    *   char descr_macro_modello[68];
    *       (breve descrizione del modello (la compilazione di questo
    *        campo e' facoltativa)
    */
struct s02_st     {         
                   int nmod;
                   int nrip;
                   MODEL model[MAX_MODEL];
                   TABEL tabel[MAX_MODEL];
                   char macro_modello[12];
                   int  lun_macro_modello;
                   char descr_macro_modello[68];
		   char host_bm[MAX_LUN_HOST];
		   char host_scada[MAX_LUN_HOST];
		   char host_bi[MAX_LUN_HOST];
                  }  ;
typedef struct s02_st  S02;


   /*
    *   6)
    *   Le struttura F04 e VARLEGO sono  strutture di appoggio interne 
    *   al processo NET_COMPI 
    */
struct f04_st     {
                   int nbl;
                   int neqal;
                   int nvart;
                   int neqsis;
                   int nu;
                   int nbl1;
#if defined UNIX
                   char **nom_bloc;
                   int *ip;
                   int *ipvrs;
                   char **nom_sivar;
                   int *iout_sivar;
                   char **nom_vari;
                   int *iout_vari;
#else
                   char nom_bloc[N002][MAX_LUN_NOME_BLOCCO];
                   int ip[M003];
                   int ipvrs[N005];
                   char nom_sivar[N003][100];
                   int iout_sivar[N003];
                   char nom_vari[N004][100];
                   int iout_vari[N004];
#endif
                   int ndati;
                   int lun_path;
                   char path[90];
#if defined UNIX
                   int *ipdati;
#else
                   int ipdati[N002+1];
#endif
                  } ;
typedef struct f04_st F04;


   /*
    *   7)
    *   Le struttura F04 S04 e VARLEGO sono  strutture di appoggio interne 
    *   al processo NET_COMPI 
    */
struct varlego_st {
                   int neqsis;
                   int nu;
                   int nderi;
		   int ndati;
#if defined UNIX
		   char **nom_sivar;
                   char **nom_vari;
#else
		   char nom_sivar[N003][100];
                   char nom_vari[N004][100];
#endif
                  } ;
typedef struct varlego_st VARLEGO;


   /*
    *   8)
    *   La struttra VARIABILI viene utilizzata per accedere al file
    *   variabili.rft, del quale, durante la simulazione, esiste un 
    *   immagine in memoria.
    *   char nome[MAX_LUN_NOME_VAR];
    *           (nome della variabile)
    *   char descr[MAX_LUN_DESCR_VAR];
    *           (descrizione della variabile;)
    *   unsigned short  mod;
    *           (indice del modello 1..)
    *   unsigned short  blocco;
    *           (indice del blocco 1..)
    *   char  tipo;
    *           (tipo di variabile: ingresso non conn/conn uscita o stato)
    *   int  addr;
    *           (indirizzo in database della variabile)
    *   char  typ;  
    *           (variabile selezionata o meno per la grafica)
    *   char  typlg;  
    *           (variabile selezionata o meno per la grafica)
    *   char tipo_var;
    *           (variabile digitale o analogica)
    */
struct variabili_st {
                char nome[MAX_LUN_NOME_VAR];
                char descr[MAX_LUN_DESCR_VAR];
                unsigned short  mod; 
		unsigned short  blocco;
                char  tipo;
                int  addr;
                char  typ;
                char  typlg;
		int tipo_var;
               } ;
typedef struct variabili_st VARIABILI ;


struct variabili_slim_st {
                char nome[MAX_LUN_NOME_VAR];
                char  tipo;
                int  addr;
               } ;
typedef struct variabili_slim_st VARIABILISLIM ;
   /*
    *   9)
    *   La struttra NOMI_MODELLI viene utilizzata per accedere al file
    *   variabili.rft, del quale, durante la simulazione, esiste un immagine 
    *   in memoria.
    *   char nome[MAX_LUN_NOME_MODELLO]; 
    *            nome del modello
    *   unsigned short num_blocchi;
    *            numero dei blocchi del modello
    *   int offset;
    *            indice di appoggio 
    */
struct nomi_modelli_st{
		char nome[MAX_LUN_NOME_MODELLO];
		unsigned short num_blocchi;
		int offset;
		};
typedef struct nomi_modelli_st NOMI_MODELLI ;


   /*
    *  10)
    *   La struttra NOMI_BLOCCHI viene utilizzata per accedere al file
    *   variabili.rft, del quale, durante la simulazione, esiste un immagine 
    *   in memoria.
    *   char nome[MAX_LUN_NOME_BLOCCO];
    *        (nome del blocco)
    *   int  num_dati;
    *        (numero dei dati del blocco)
    */
struct nomi_blocchi_st{
		char nome[MAX_LUN_NOME_BLOCCO];
                int  num_dati;
		};
typedef struct nomi_blocchi_st NOMI_BLOCCHI ;


   /*
    *  11)
    *  La struttura SELEZIONE viene utilizzata dal processoNET_MONIT per
    *  scrivere e leggere l'header del file recorder.var
    *  char nome_mod[80];
    *       (nome del simulatore)
    *  char nome_selezione[80];
    *       (nome della selezione grafica)
    *  char autore_selezione[80];
    *       (autore della selezione grafica)
    *  char versione_selezione[80];
    *       (versione della selezione grafica)
    */
struct selezione_st {
                 char nome_mod[80];
                 char nome_selezione[80];
                 char autore_selezione[80];
                 char versione_selezione[80];
                };
typedef struct selezione_st SELEZIONE;


   /* 
    *   12)
    *   La struttura NUMERI_MODELLI contiene per un generico
    *   modello il valore di stati, ingressi,derivate e dati.
    *   Sostituisce nel file RtDbPuntiP.h la struttura 
    *   HEAD_DBPUNTI_MODEL in modo quindi da averla dichiarata
    *   anche per l'esterno e poterla estrarre.
    */
typedef struct numeri_modelli_st
        {
        int stati;
        int ingressi;
        int derivate;
        int dati;
        } NUMERI_MODELLI;


   /*
    *   13)
    *   runtime_replay: indica se e' una perturbazione proveniente
    *   dall'esterno (PERT_RUN_TIME) oppure gestita
    *   durante il replay (PERT_REPLAY) [solo per REPLAY]
    *   posizione: posizione nell'area pert
    *   tipo: tipo di perturbazione (vedi sopra)
    *   indirizzo: indirizzo in shared memory della variabile da perturbare
    *   esaminata: variabile di appoggio;
    *   valore_delta: valore finale o delta, utilizzato da SCALINO, RAMPA,
    *               IMPULSO
    *   rateo_durata: rateo della rampa (delta su secondi) o durata
    *               dell'impulso
    *   t: se >=0 : tempo a cui attivare la perturbazione
    *   se <0  : delta t a cui attivare la perturbazione
    */
struct tipo_pert_st {
#if defined REPLAY
	int runtime_replay;
#endif
        int posizione;
        int tipo;
        int indirizzo;
        int esaminata;
        float valore_delta; /* parametro T1 per pert. periodiche */
        float rateo_durata; /* parametro T2 per pert. periodiche */
        float t;
        float period;
        float phase;
        float meanvalue;
        float wide;
        float t_null_var;   /* tempo nullo per spike o varianza
                                per white noise                 */
        float post;
        };
typedef struct tipo_pert_st TIPO_PERT;


   /* 
    *   14)
    *   La struttura SNTAB viene utilizzato come header di ciascun 
    *   snapshot registrato nel file snapshot.dat
    *   int stat;
    *       (stato dello snapshot (libero[0] o occupato[1]))
    *   int pos;
    *       (posizione all'interno del file)
    *   int prog;
    *       (indice dello snapshot)
    *   char descr[SNAP_DESC];
    *       (descrizione dello snapshot)
    */
struct sntab_st {
		 int stat;
		 int pos;
		 int prog;
		 int mod;
		 int forzato;
                 float tempo;               /* Aggiunta per renderla uguale */
		 float val[MAX_VAL_SNAP];
                 char descr[SNAP_DESC];
                 char datasn[DATA_SNAP];
		 char temposn[TEMPO_SNAP];
                } ;
typedef struct sntab_st SNTAB;


   /* 
    *   15)
    *   Analoga alla SNTAB ma per i backtrack.
    */
#if defined BACKTRACK
struct bktab_st {
                 int stat;
                 int pos;
                 int prog;
                 int mod;
                 int forzato;
                 float tempo;
                 float val[MAX_VAL_SNAP];
                 char descr[SNAP_DESC];
                 char datasn[DATA_SNAP];
                 char temposn[TEMPO_SNAP];
                } ;
typedef struct bktab_st BKTAB;
#endif


   /*
    *   15)
    *   Questa struttura contiene le define che fissa il file Simulator
    *   in modo che resti fissato, per il file che la contenga, le 
    *   caratteristiche di costruzione.
    */
typedef struct simulator_st
        {
        int max_snap_shot;
        int max_back_track;
        int max_campioni;
        int num_var;
        int max_pertur;
        int spare_snap;
        int pert_clear;
        int spare1;
        int spare2;
        int spare3;
        }SIMULATOR;


   /*
    *   16)
    *   Questa struttura costituira' l'header per i file delle 
    *   registrazioni attive. snapshot.dat backtrack.dat
    */
typedef struct header_registrazioni_st
        {
        int size_area_dati;
        SIMULATOR       simulator;
        NUMERI_MODELLI  num_modelli[MAX_MODEL];
        char            area_spare[512];
        }HEADER_REGISTRAZIONI;



   /*
    *   17)
    *   Enum
    */
typedef enum _flag_sel { SEL_REG, SEL_LGPH } FLAG_SEL;


   /* 
    *  ***********
    *  PROTOTYPING
    *  ***********
    */
int pert_snd(int, int, int, 
		float, float, float, float, float, float, float, float);
int pert_snd_replay(int,int,int,float,float,
		float,float,float,float,float,float);
int pert_down(int ,int ,float);
int pert_impulso(int ,int , float ,float ,float );
int pert_impulso_stop(int ,int ,float );
int pert_nega(int ,int ,float );
int pert_period(int ,int ,float ,float , float ,float ,float ,float ,
                float ,float );
int pert_period_stop(int ,int ,float );
int pert_rampa(int ,int ,float , float ,float );
int pert_rampa_stop(int ,int ,float );
int pert_scalino(int ,int ,float ,float );
int pert_to(int ,int ,float , float ,float );
int pert_to_stop(int ,int ,float );
int pert_up(int ,int ,float );
int pert_whitenoise(int ,int ,float , float ,float );
int pert_whitenoise_stop(int ,int ,float );
int pert_rcv(int );
int pert_rcv_null(int);
int msg_rcv(int, void*, size_t, long, int,int);

int msg_snd (int, void*, size_t, int);
int msg_close(int);

void reg_prolog();
//void reg_rdshm(float *, float *, float *, float *,
//               int *, int *, int *, int *,
//               float *, float *);

//void reg_rdshm(float (*xy)[],float (*uu)[],float (*px)[],float (*dati)[], int *neqsis, int *nu,int *neqdif,int *ndati,float (*cnxy)[],float (*cnuu)[]);
void reg_rdshm();
              
//void reg_wrshm(float *, float *, float *, float *,
//               int *, int *, int *, int *,
//               float *, float *);               
//void reg_wrshm(float (*xy)[],float (*uu)[],float (*px)[],float (*dati)[], int *neqsis, int *nu,int *neqdif,int *ndati,float (*cnxy)[],float (*cnuu)[]);
void reg_wrshm();

//void reg_ing(float,int*,int*,float);
//void reg_ing(float (*xy)[], int*,int*, float (*uu)[]);
void reg_ing();

int msg_close_fam();
void initialize_syncronization( INTERFACE_VAR *vartask);
void syncronize_sicre_run_time(void);
void read_S02_net(void);
void write_S02_net(int);

void scrive_default(char*);
char   *nome_modello (char*, int);
void scrive_default(char*);

//void reg_wrshm(float (*)[], float (*)[], float (*)[], float (*)[],
//               int *, int *, int *, int *,
//               float (*)[], float (*)[]);




void legge_sel(char*);
int tempo_file(char*,char*);
void scrive_sel(char*);
void elimina_f22(void);
int sem_create(int,int);
void sem_close(int);
void sem_signal(int);
int sem_set(int,int);
void sem_wait(int);
void blocca_shr(void);
void distruggi_var (int);
void cfree2(char**);
void ifree2(int**);
void distruggi_var (int);
void sim_shvar_free(void);
void output_ascii_big (char*);
void output_ascii_big (char*);
int GetParLego(void);
int     shresist (int);
//void reg_ing(float*[],int*,int*,float*[]);
void reg_prolog(void);
int msg_create_fam(int,int);
int msg_create_fam_sim(int,int,int);
int sem_create(int,int);
int msg_create(int,int);
void distruggi_shrmem(int);
void init_umis();
int cerca_num_umis();
void MostraPertFile(char *);

void legge_riga_bin(char*,int*,FILE *);

void var_bin(void);
void fill_space (char*, int);
int  writen(int,char*,int);
int readn(int,char*,int);

void open_22dat(void);
void close_22dat(void);
void read_22dat(char,int,int,int);
void read_nomi(FILE*,unsigned long *);
void list_vargraf(void);
int load_stato_cr_arch();

void set_min_max();

int     numero_modelli (char*);
int     numero_blocchi (char*, int);
int cerca_blocco(char *,char *,FILE **);
int scrivi_dati_f14(char *, char *, int , int ,char *,int );
int cerca_blocco(char *,char *,FILE **);
int legge_dati_var(char *, int , int );
int numero_dati();
void path_liste(char *, char **, char **);
int     numero_variabili (char*);
int     numero_var_bl(VARIABILI *,int,int,int);
int scelta_lista(char *, int , int , char *,char **, char **, char *, char *);
void r_lstmod_();
void leggi_for_();
void lancia_i2_();

char **cdim2(int,int);
int    NomeProcesso(int , char *);
int FileExist(char *, int );
int ConfrontoPar(HEADER_REGISTRAZIONI  , int  , char * );

int test_host(char*);
int getindbyname(char*);
int setta_dim_buffer(int , int );

void do_demone(int , char *,  char *, int , char *, char *, HEADER_REGISTRAZIONI *);

void sospendi(unsigned int );

int from_dispatcher (int, int*, int*, char*, int*, int);
int SD_editic (int , SNTAB *, int , int );

int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);




#endif /* Fine ifndef _sim_types_h */
