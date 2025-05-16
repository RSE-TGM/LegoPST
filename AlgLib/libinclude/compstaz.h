/*
   modulo compstaz.inc
   tipo
   release 1.1
   data 6/15/93
   reserved @(#)compstaz.inc    1.1
*/

#ifndef _compstaz_h_
#define _compstaz_h_


#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

typedef char* caddr_t;

typedef struct collega {
  char coll [MAX_COLL] [8];
  } COLLEG_ST;

typedef struct strin_st {
  char *stringa;
  int lun_stringa;
  } STRIN_ST;


#define ERR_DES         0
#define ERR_PAG         1
#define ERR_POS         2
#define ERR_LAB         3
#define ERR_UMIS                4
#define ERR_SCAL                5
#define ERR_SCALERR             6
#define ERR_MINMAX              7
#define ERR_MINMAXERR   8
#define ERR_ASSOC               9
#define ERR_LOGICA              10
#define ERR_STATO               11
#define ERR_ESAMR               12
#define ERR_VALORE              13
#define ERR_VALERR              14
#define ERR_RSTATO              15
#define ERR_RPIU                        16
#define ERR_RMENO               17
#define ERR_RTARGET             18
#define ERR_TARGET              19
#define ERR_DESVAL              20
#define ERR_GRUP1               21
#define ERR_GRUP2               22
#define ERR_OGGETTO             23
#define ERR_COLORE              24
#define ERR_ETIC                25
#define ERR_INPUT               26
#define ERR_INPUT_BLINK         27
#define ERR_OUTPUT              28
#define ERR_PERTURB             29
#define ERR_INPUT_ERR           30
#define ERR_OFFSET              31
#define ERR_INIBIZ              32


void legge_riga( char *riga, int *lun, int *nriga );
// void separa_str( char *, int , int , STRIN_ST []);
int separa_str(char [],int ,int ,STRIN_ST []);

void errore(int,char*);
int co_float(int,int,STRIN_ST *,float *);
int check_model(char *,int *);
int check_output(char *,int ,int *);
int check_input(char *,int ,int *);

int staz_am1_c(int,int,int);
int staz_sa1_c(int,int,int);
int staz_sp1_c(int,int,int);
int staz_spd_c(int,int,int);
int staz_id1_c(int,int,int);
int staz_br1_c(int,int,int);
int staz_tr1_c(int,int,int);
int staz_mr1_c(int,int,int);
int staz_lu1_c(int,int,int);
int staz_am2_c(int,int,int);
int staz_am3_c(int,int,int);
int staz_amd_c(int,int,int);
int staz_sd1_c(int,int,int);
int staz_gen_c(int,int,int);

// int c_led(TIPO_LED *);
//int c_luce(TIPO_LUCE *,int,int,int,int,int,int*,char*);
// int c_puls(TIPO_PULS *);
// int c_pulsluce(TIPO_PULS_LUCE *);
// int c_lamp(TIPO_LAMPADA *);
// int c_selet(TIPO_SELETTORE *);
// int c_indic(TIPO_INDICATORE *,int);
// int c_stringa(TIPO_STRINGA_DESCR *);
// int c_display(TIPO_DISPLAY *);
// int c_tasto(TIPO_TASTO *);
// int c_setval(TIPO_SET_VALORE *);
// int c_displayscal(TIPO_DISPLAY_SCALATO *);
// int c_sincro(TIPO_INDICATORE_SINCRO *,int);

int c_led();
int c_luce();
int c_puls();
int c_pulsluce();
int c_lamp();
int c_selet();
int c_indic();
int c_stringa();
int c_display();
int c_tasto();
int c_setval();
int c_displayscal();
int c_sincro();




int is_neg(char*);

int compila_new_staz(int,int,int);
int cerca_staz(char*,int);
void lungh(char[],int *,int );

void CercoPosMax(FILE *);
int read_pagina();
char *CostruisciRigaInput(char *, char *,int );
char *CostruisciRigaOutput(char *, char *,char *,char *);
char *RetColore(char *);
char *RetColoreBlink(char *);
float get_valore(char*);
void crea_sfondo(Widget,Dimension,Dimension);
int crea_sfondoP(Pixmap *,char *,int,int);
int add_refresh (XtCallbackProc, caddr_t);
//int g_perturba(int,int,int,float,float,int,int);
int g_perturba(int ,int ,float ,float ,int ,int ,int );
int del_refresh (caddr_t);

void am1_ago_mis(Window,float,int);
void am2_ago_mis(Window,float,int);
void am3_ago_mis(Window,float,int);
void sp1_ago_mis(Window,float,int);
void sa1_ago_mis(Window,float,int);

void am2_ago_err(Window,float,int);
void am3_ago_err(Window,float,int);


void am1_set_button(int,int,int);
void am2_set_button(int,int,int);
void am3_set_button(int,int,int);
void amd_set_button(int,int,int);
void id1_set_button(int,int,int);
void sp1_set_button(int,int);
void spd_set_button(int,int);

void staz_am1_r(int*,int*,int*);
void staz_am2_r(int*,int*,int*);
void staz_am3_r(int*,int*,int*);
void staz_amd_r(int*,int*,int*);
void staz_id1_r(int*,int*,int*);
void staz_spd_r(int*,int*,int*);
void staz_br1_r(int*,int*,int*);
void staz_sd1_r(int*,int*,int*);
void staz_sa1_r(int*,int*,int*);
void staz_lu1_r(int*,int*,int*);
void staz_mr1_r(int*,int*,int*);
void staz_sp1_r(int*,int*,int*);
void staz_tr1_r(int*,int*,int*);
void staz_gen_r(int*,int*,int*,int*);

int ef_cluster();
int wait_ef(int,int);
int tra_nom_log_s(char*,int,char*,char*);

void init_staz();
void LoadColor(Widget);
void get_something(Widget,char *,char * );
void set_something(Widget,char *,char * );
void init_gc();
int ef_is_set(int,int);
int set_ef(int,int);
int cnew_staz(int);
int clr_ef(int,int);
void crea_new_staz(int,int,int,int);

int add_redraw(XtCallbackProc,caddr_t);
//void load_font(XFontStruct **,char[],XmFontList *);

void load_font(XFontStruct **,XmFontList *,char*);
void pr_pagina();
void pr_stazione();

void crled(int ,Widget,OGGETTO	*,TIPO_LED *);
void crpulsluce(int ,Widget,OGGETTO	*,TIPO_PULS_LUCE *);
void crstringa(int ,Widget,OGGETTO	*,TIPO_STRINGA_DESCR *);
void crlampada(int ,Widget,OGGETTO	*,TIPO_LAMPADA *);
void crpulsante(int ,Widget,OGGETTO	*,TIPO_PULS *);
//void crselet(int ,Widget,OGGETTO	*,TIPO_LED *);

void crselet(int ,Widget,OGGETTO	*,TIPO_SELETTORE *,int);

void crindic(int ,Widget,OGGETTO	*,TIPO_INDICATORE *,int);
void crdisplay(int ,Widget,OGGETTO	*,TIPO_DISPLAY *);
void crluce(int ,Widget,OGGETTO	*,TIPO_LUCE *);
void crtasto(int ,Widget,OGGETTO	*,TIPO_TASTO *);
void crsetval(int ,Widget,OGGETTO	*,TIPO_SET_VALORE *);
void crdisplaysc(int ,Widget,OGGETTO	*,TIPO_DISPLAY_SCALATO *);
void crsincro(int ,Widget,OGGETTO	*,TIPO_INDICATORE_SINCRO *,int);

int libera_trefr(DATI_REFRESH *);
int libera_tredraw(DATI_REFRESH *);

void blink_led(Widget);
void blink_lampada(Widget,TIPO_LAMPADA *);
void blink_puls(Widget,TIPO_PULS_LUCE *);

void LoadPixmapSelet(Widget,int);
int del_redraw(caddr_t);
void draw_barra(Widget,float,TIPO_INDICATORE *);
void draw_barra_or(Widget,float,TIPO_INDICATORE *,int);
void draw_ago_err(Widget,float,float, TIPO_INDICATORE *);
void draw_ago(Widget,float,TIPO_INDICATORE *);
void draw_ago_sincro(Widget,float,TIPO_INDICATORE_SINCRO *);


void crea_gc_indic();
void crea_gc_sincro();
void blink_luce(Widget);

int pert_up(int ,int ,float );
int pert_down(int ,int ,float );
int pert_scalino(int ,int ,float ,float );




#endif










