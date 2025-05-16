

#ifndef _sked_fun_h_
#define _sked_fun_h_


int skey_stampa_errore(int , char * );
int ControlParam(int );
void to_someone(int, int, int, int, char*, int);
int sked_banco(int , char *);
int sked_replay(int);
void sked_prolog_code();
void sked_prolog();
void sked_s02();
void sked_start();
void sked_statistiche();
void            scoda_buffer();
void sked_legograf(float off_time);
void sked_pf22(int);
void sked_stato(int);
void sked_slave();
Boolean RtCheckPointer(void *);
void sked_next();
void sked_task();

void init_area_pert();
void costruisci_var (char**, VARIABILI **, int*);
void snap_tab_read_piac();
int bktk_tab_read_piac();
int load_stato_cr(STATO_CR *);
int save_stato_cr(STATO_CR *);
int ins_tacca_bktk_piac(int);
int sked_tempo_iniziale();
int aggancia_sem_shr();
void sked_timer(int );
int sked_fine();
void do_demone(int , char *,  char *, int , char *, char *, HEADER_REGISTRAZIONI *);
int sgancia_sem_shr();
int msg_close_fam_sim(int);
void sked_startup();
void sked_errore();
void            sked_ingressi();
int libera_shr();
void sked_stato_intermedio();
int getTimeLicense(double *);
int sked_stato_task();
void            sked_trasmette();
int gest_area_pert(int, float);
void costruisci_pacchetto(int, int, int);

void remove_updown(float*);
void rm_updown_from_pert();
void remove_updown_slave(float*, int);
void cut_updown();
int conf_host(char*,char*);
int socketscrittura(char*,int);
void sked_statistiche();
void            scoda_buffer();
void snap_slot(int, int, int);
void sked_snapshot(int, short);
int snap_save(int);
int snap_del(int);
void snap_tab_read();
void sked_snap_mod_descr(int , char *);
char *io_sono(void);
int sked_prepara_header_reg(HEADER_REGISTRAZIONI *h_reg);
void sked_aggiornamento();
int sked_raccogli_pert();
int remove_pert(int);
int rm_pert_remote();
void export_db();
void bktk_clear_piac();
int snap_slot_piac(int , int , int );
int sked_task_info(int *);
void data(int*, int*, int*);
int bktk_slot_piac(int, int, int);
void sked_snapshot_piac(int, short);
void sked_backtrack_piac(int, short);
int leggi_dati_f14(char *, char *, int ,int , char **,int *);
int bktk_slot(int, int, int);
int bktk_load(int);
int bktk_del(int);
int ins_tacca_bktk(int);
void del_tacca_bktk();
void bktk_tab_read();
int sked_backtrack(int, short);
int bktk_save(int);
void sked_raccogli_updown(int *, float *);
void suono_errore();
int sked_raccogli_datisnap(int , int , PACCHETTO_NET *, int );
int remove_updown_file_snap(float *, TIPO_PERT *, TIPO_PERT *);
int  StampePertFile(TIPO_PERT *, TIPO_PERT *, int , int );
int bktk_tab_read_piac();


#endif