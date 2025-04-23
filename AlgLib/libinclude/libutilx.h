/**********************************************************************
*
*       C Header:               libutilx.h
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Jan 21 12:46:45 1997 %
*
**********************************************************************/
/*
   modulo libutilx.h
   tipo 
   release 5.1
   data 11/16/95
   reserved @(#)libutilx.h	5.1
*/
/*   Include della libreria di funzioni libx  */
#ifdef _LIBUTILX_H
#else
#define _LIBUTILX_H

/* define per la gestione dei widget  */
#define NON_ESISTE      0
#define CREA 		1
#define MAPPA		2
#define UNMAPPA		4
#define DISTRUGGI	8

/* define per i tipe pre-definiti (dalla libx) di cursori       */
/* usando questi non e' necessario includere <X11/cursorfont.h> */
#define BARCA		1001
#define PENDOLO		1002
#define SCAMBIA		1003
#define PENNA		1004
#define TESCHIO		1005
#define OROLOGIO 	1006

/* tale variabile viene usata in commento.c */
/* se = 1 vengono visualizzati i commenti */
#define DEBUG 		0
typedef struct Elenco_callback {
                                  char *button_label;
                                  void (*callback)();
                                  int  parametro;
                               } Elenco_callback[3];

/*   Definizione del tipo Dialog_geometry per passare ai Dialog di libreria
     (UTILX) le coordinate dell'origine, l'ampiezza e l'altezza
*/
typedef struct {
	int default_pos;  /* True = posizione al centro della top_level 
			     False= posizione specificata come sotto   */
	int x,y;
	int width,height;
	} Dialog_geometry;

/* struttura utilizzata dal find_kit() per la ricerca di stringhe */
/* nelle scrolled-list */
typedef struct {
                  Widget ID_lista, ID_testo, ID_label, ID_fnext, ID_fprev;
                  int    num_elem;
                  int    vis_elem;
                  int    pos;
                } Find_struct;

typedef struct {
                  Widget ID_text,
                         ID_pattern,
                         ID_label,
                         ID_fnext,
                         ID_fprev;
                } Text_find_struct;

/*
 Definizioni delle funzioni presenti in libreria
*/
char *Utstrstr(char *,char *);
Widget attesa(Widget,char *,int);
void def_cursore(Widget,int);
void undef_cursore(Widget);

void get_something(Widget,char *,char * );
void set_something(Widget,char *,char * );
/*
 void get_something();
void set_something();
*/
void UtListaScroll(Widget,int,int,int);
Widget attenzione(Widget,char *,int);
Widget information(Widget,char *,int,Dialog_geometry);
Widget attention(Widget,char *,int,Dialog_geometry);
Widget conferma(Widget,char *,Elenco_callback);
Widget editor(Widget,Widget *,char *,int,char *,Elenco_callback,
              Dialog_geometry);
Widget find_kit(Widget , Arg [], int, Find_struct *);
char *extract_string(XmString);
Widget errore(Widget,char *,int);
int UtStatoWidget(Widget);
void commento(char *);
int spazio_disco(char *);
Widget find_in_a_text (Widget,ArgList,int,Text_find_struct*);

void chdefaults();

Widget WidAttenzione(Widget,char*,int);
Widget WidErrore(Widget,char*,int);
void abilita_menu(int);
//void load_file(char*,S_XLGRAFICO);
void load_file();
void load_variables(int,char**,Widget);

void d2free(char**);
void prep_str_tim(float,float);








#endif
