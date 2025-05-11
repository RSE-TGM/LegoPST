/*
   modulo compstaz.inc
   tipo
   release 1.1
   data 6/15/93
   reserved @(#)compstaz.inc    1.1
*/
#include <stdio.h>
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



