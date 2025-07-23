/**********************************************************************
*
*	C Header:		client_s.h
*	Subsystem:		4
*	Description:	
*	%created_by:	lomgr %
*	%date_created:	Thu Aug 28 17:27:13 2003 %
*
**********************************************************************/

#define errore(stringa)        exit(fprintf(stderr,"Attention !!!\nError in process client_scada[%d]: %s\n",getpid(),stringa))
#define warning(stringa)       fprintf(stderr,"Attention !!!\nWarning in processclient_scada[%d]: %s\n",getpid(),stringa)
#define check(status,string) \
        if(status == -1) perror(string)

// GUAG2025 porting da dcethread a pthread
//#define sleep(time) p_sospendi(time*1000)

#define UNLOCK	        0
#define LOCKALARMREQ	1
#define LOCKMANUALREQ   2
#define LOCKRIC         3
#define LOCKALARMDES	4
#define LOCKRICALL	5

#define ALARMREQ        0
#define MANUALREQ       1
/*
TEMPO TIMEOUT per miniASD
*/
#define TIMEOUT_MINIASD    20
#define TIMEOUT_MINIASD_RICALL   240 


/*
Definisco una struttura per lock miniASD.
type_lock indica se il lock e' attivo ed in tal caso quale operaz. 
lo ha attivato.
time_start e' il tempo in cui e' stato settato il lock. (per TIMEOUT)

N.B.
Quando effettuo una operazione per miniASD devo attendere che l' operazione 
sia terminata da parte SCADA (SCADA e miniASD non hanno alcun link) prima di
poter fare una altra operazione con miniASD
*/

typedef struct lock_asd_st
        {
        int  type_lock;
        time_t time_start;
	} LOCK_MINIASD;

typedef struct zone_st
        {
        int stato;
        int id_shm;
        char *ind;
        int shmvid;
        int size;
        int num_punti;
	int t_agg;
        int tipo[MAX_PUNTI_SCADA];
        int puntdb[MAX_PUNTI_SCADA];
        int pos[MAX_PUNTI_SCADA];
        } ZONE;


typedef struct buffer_trend_st
        {
        float tempo;
        float dati[4];
	} BUFFER_TREND;


typedef struct s_send_allarmi_ {
                        S_HEAHOST  hea;
                        char zona;
                        char video;
                        char pag[l_pagina];
                        short spare1;
#if defined OSF1_40 || defined LINUX
                        short spare2;
#endif
                        short tipo_arg_1;
                        char stringa[8];
                        short tipo_arg_2;
                        short ind_db;
                        short tipo_arg_3;
                        short criterio;
                        short tipo_arg_4;
                        short tipo_comando;
                        short tappo;
                       } S_SEND_ALLARMI ;


typedef struct s_send_allarmi_zone_ {
                        S_HEAHOST  hea;
                        char zona;
                        char video;
                        char pag[l_pagina];
                        short spare1;
#if defined OSF1_40 || defined LINUX
                        short spare2;
#endif
                        short tipo_arg_1;
                        char stringa[8];
                        short tipo_arg_2;
                        short ind_db;
                        short tipo_arg_3;
                        short criterio;
                        short tipo_arg_4;
                        short tipo_comando;
                       } S_SEND_ALLARMI_ZONE ;


//void main_client_scada_command();
//void main_client_scada_data();
void *main_client_scada_command(void *arg);
void *main_client_scada_data(void *arg);
int cerca_zona_libera();
int libera_zona();
int libera_tutte_le_zone();
int ins_token_zona();
int change_token_zona();
void p_sospendi(int);
int ins_dati_shm();
int ins_dati_miniASD();
int ins_dati_shm_all();
float conv_t_sim(short, short, short, short);
void ins_dati_allarmi();
void ins_parziale_dati_allarmi();

extern void f22_open(char *,char *,char *,int );
extern void f22_write(float,float*,int);
extern void f22_close();;
