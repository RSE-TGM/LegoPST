/**********************************************************************
*
*       C Header:               XlP.h
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:09:13 2003 %
*
**********************************************************************/
/*
   modulo XlP.h
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlP.h	5.1
*/
/*
 *   Xl.h
 *   Contiene definizioni comuni utilizzate dai widget dell'estensione Xl al
 *   widget set di Motif
 */
#ifndef _XlP_h
#define _XlP_h
/*
 inclusione dell'header file pubblico
*/
#include <Xl/Xl.h>

/*
        Include tipici del simulatore
*/
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"


/*
	define per segnalazione errori e warning
*/
#define XlWarning(oggetto,procedura,stringa) \
fprintf(stderr,"\n*******\n* XlWarning [%s]: <proc: %s> %s\n*******\n",oggetto,procedura,stringa);
#define XlError(oggetto,procedura,stringa) \
exit(fprintf(stderr,"\n*******\n* XlError [%s]: <proc: %s> %s\n*******\n",oggetto,procedura,stringa));
/*
 Macro per evidenziare graficamente un oggetto
 su cui si e' verificato un errore.
 Usava "cw" al posto del suo argomento: funzionava solo perche' tutti la
 chiamano con cw.
*/
#define XlEvError(widget,flag)\
{\
Arg args[2];\
XtSetArg(args[0],XlNerrore,flag);\
XtSetValues((Widget)(widget),args,1);\
}

/* tipi di elaborazione sul valore di input appena estratto */

#define INP_NORM	0   /* input normale */
#define INP_NEG         1   /* input negato  */
#define MAX_LUN_UNIMIS  10  /* dimensione massima del campo char
			       contenente l'unita' di misura  */

/*
	strutture relative ai flags 
*/
typedef struct flag_digitale_st{
/*
	la piattaforma AIX questi campi li digerisce solo cosi' ...
*/
#if defined AIX
                unsigned sl : 1;   /* stato logico corrente */
                unsigned ri : 1;
                unsigned au : 1;
                unsigned in : 1;
                unsigned st : 1;
                unsigned fz : 1;   /* forzato */
                unsigned fa : 1;   /* fuori attendibilita' */
                unsigned fs : 1;   /* fuori scansione */
                unsigned trattamento : 4;
                unsigned spare : 2;
                unsigned ls : 1;
                unsigned as : 1;
#else
                unsigned short sl : 1;   /* stato logico corrente */
                unsigned short ri : 1;
                unsigned short au : 1;
                unsigned short in : 1;
                unsigned short st : 1;
                unsigned short fz : 1;   /* forzato */
                unsigned short fa : 1;   /* fuori attendibilita' */
                unsigned short fs : 1;   /* fuori scansione */
                unsigned short trattamento : 4;
                unsigned short spare : 2;
                unsigned short ls : 1;
                unsigned short as : 1;
#endif
                } FLAG_DIGITALE;

typedef struct flag_analogico_st{
#if defined AIX
                unsigned lc : 1;   /* fuori limite bass di conduzione */
                unsigned hc : 1;   /* fuori limite alto di conduzione */
                unsigned ls : 1;   /* fuori limite basso di sicurezza */
                unsigned hs : 1;   /* fuori limite alto di sicurezza */
                unsigned dl : 1;
                unsigned dh : 1;
                unsigned ri : 1;
                unsigned s3 : 1;
                unsigned au : 1;
                unsigned s2 : 1;
                unsigned al : 1;
                unsigned s1 : 1;
                unsigned st : 1;
                unsigned fz : 1;   /* forzato */
                unsigned fa : 1;   /* fuori attendibilita' */
                unsigned fs : 1;   /* fuori scansione */
#else
                unsigned short lc : 1;   /* fuori limite bass di conduzione */
                unsigned short hc : 1;   /* fuori limite alto di conduzione */
                unsigned short ls : 1;   /* fuori limite basso di sicurezza */
                unsigned short hs : 1;   /* fuori limite alto di sicurezza */
                unsigned short dl : 1;
                unsigned short dh : 1;
                unsigned short ri : 1;
                unsigned short s3 : 1;
                unsigned short au : 1;
                unsigned short s2 : 1;
                unsigned short al : 1;
                unsigned short s1 : 1;
                unsigned short st : 1;
                unsigned short fz : 1;   /* forzato */
                unsigned short fa : 1;   /* fuori attendibilita' */
                unsigned short fs : 1;   /* fuori scansione */
#endif
                } FLAG_ANALOGICO;

typedef union flag_mmi_st{
		FLAG_DIGITALE flag_digitale;
		FLAG_ANALOGICO flag_analogico;
                unsigned short flag_valore;    /* (carlo) comodo per confronto con valori */
		} FLAG_MMI;

typedef struct punt_unimis{
			float a;
		 	float b;
			char descr[MAX_LUN_UNIMIS];
			} PUNT_UNIMIS;

/* puntatore al data base dinamico di simulazione */
typedef struct punt_db {
			int   nmod;     /* indice modello */
                        int   indice;   /* indice variabile */
                        int   indice_rel;   /* indice variabile  per
					aggiornamento client-server  */
                        int   indice_relS;   /* indice variabile  per
					aggiornamento client-scada  */
                        int   tipo;     /* tipo della variabile 
						(se anal. o dig.)
                                             ( non utilizzato ) */
			int origin; /* origine della variabile
					(scada o simulatore) */
			} PUNT_DB;

typedef struct punt_varout{ 
			  PUNT_DB pdb;
                          int   tipo_pert;/* tipo perturbazione */
                          float value;    /* valore della perturbazione (se
                                             specificato per perturbaz.
                                             a scalino) */
			  float rateodurata;  /* valore del rateo se spec.
				perturbazione a rampa o durata se spec. 
				perturbazione a impulso */
		          PUNT_UNIMIS unimis;  /* unita' di misura
					e trasformazioni   */
                        } PUNT_VAROUT;

typedef struct punt_varinp {
			  PUNT_DB pdb;
			  float valore;    /* valore della variabile */
			  FLAG_MMI flag;   /* flag associato al valore 
					      acquisito           */
 			  int   tipo_elab; /* tipo di elaborazione
                                              da effettuarsi sul
                                             valore in input (es. negazione) */
		          PUNT_UNIMIS unimis;  /* unita' di misura
					e trasformazioni   */
                        } PUNT_VARINP;

#define NUMERO_SOGLIE	4

/* struct per risorse di tipo XlNanimatedColor
*/
typedef struct animated_pixel {
                        Pixel colore;
                        float blink_rate;    /* freq. Hz  */
                        Boolean trasparent;  /* trasparenza True/False */
                        } XlAnimatedColor;

typedef struct cambio_colore_st {
			char   *str_input;
			PUNT_VARINP input;
		/* flag automatico */
			Pixel automatico;
			GC automatico_gc;
		/* flag stimato */
			Pixel stimato;
			GC stimato_gc;
		/* fuori scansione */
			Pixel fuoriScansione;
			GC fuoriScansione_gc;
		/* fuori attendibilita' */
			Pixel fuoriAttendib;
			GC fuoriAttendib_gc;
		/* digitale on */
		        Pixel digitaleOn;
		        GC digitaleOn_gc;
		/* bassissimo */
			Pixel bassissimo;
			GC    bassissimo_gc;
			float bassissimo_val;
		/* bassissimo - basso */
			Pixel bassissimo_basso;
			GC    bassissimo_basso_gc;
			float bassissimo_basso_val;
		/* basso - alto */
			Pixel basso_alto;
			GC    basso_alto_gc;
			float basso_alto_val;
		/* alto - altissimo */
			Pixel alto_altissimo;
			GC    alto_altissimo_gc;
			float alto_altissimo_val;
		/* altissimo */
			Pixel altissimo;
			GC    altissimo_gc;
			} CAMBIO_COLORE;

/*
*/
typedef struct pict_var_st {
                        char   *str_input;
                        PUNT_VARINP input;
                        int FlagOffset;
                        } PICT_VAR;

/* struttura cambio colore per il pictograph
   ogni zona ha 6 variabili contribuenti
   qui viene inserito solo il colore di default
   gli altri colori, relativi agli stati, sono
   descritti dal file programma .prg
*/
typedef struct cambio_colore_pict_st {
                        PICT_VAR pict_var[6];
                        XlAnimatedColor defaultColor; /* colore default */
                        GC defaultColor_gc;
                        } CAMBIO_COLORE_PICT;

/* struttura dedicata alla gestione del frame
   per teleperm
*/
typedef struct cambio_colore_frame_st {
        char *str_input;
        PUNT_VARINP input;             /* variabile di controllo del frame */
        XlAnimatedColor owFrameColor;  /* colore del frame con O/W aperta */
        XlAnimatedColor varFrameColor; /* colore del frame in quando la variabile e'a a 1 */
        GC owFrame_gc;                 /* gc relativo a varFrameColor */
        GC frame_gc;                   /* gc relativo a owFrameColor */
        Boolean blink;                 /* controllo del blink */ 
	int frameWidth;                /* spessore del frame (tutto dentro) */
        } CAMBIO_COLORE_FRAME;        


/* struttura dedicata alla gestione del cambio colore su
   singola variabile per teleperm (usabile per variabili di Fault e/o di Controllo)
   pemette di controllare due colori con una variabile
*/
typedef struct cambio_colore_var_st {
        char *str_input;
        PUNT_VARINP input;
        XlAnimatedColor VarColorOn;    /* colore con variabile a 1 (o stato norm) */
        XlAnimatedColor VarColorOff;   /* colore con variabile a 0 (o stato opposto)*/
        GC varOn_gc;
        GC varOff_gc;
        } CAMBIO_COLORE_VAR;

/*
Struttura variabili di input per oggetto CaiVer 
*/
typedef struct input_all {
			char *str_input;
			PUNT_VARINP allarme;  /* Variabile di input */
			char type;             /* Tipo di CaiVer */
			}INPUT_ALL;    /* risorsa per confinfo */
/*
Strutture per descrizione risorsa gerarchia per descrizione allarmi
*/
typedef struct all_hierarchy {
		char   str_input[40];
		float input_valore;
			}ALL_HIERARCHY;    /* risorsa per confinfo */
typedef struct all_bit_situation {
                unsigned PEN_A: 1;
                unsigned PEN_W: 1;
                unsigned PEN_T: 1;
                unsigned PEN_S: 1;
		unsigned PEN_M: 1;
		unsigned PEN_F: 1;
                unsigned PEN_N: 1;
                unsigned PEN_Q: 1;   /* attualmente non implementato */
                unsigned ARC_A: 1;
                unsigned ARC_W: 1;
                unsigned ARC_T: 1;
                unsigned ARC_S: 1;
                unsigned ARC_M: 1;
                unsigned ARC_F: 1;
                unsigned ARC_N: 1;
                unsigned ARC_Q: 1;
                unsigned RIE_A: 1;
                unsigned RIE_W: 1;
                unsigned RIE_T: 1;
                unsigned RIE_S: 1;
                unsigned RIE_M: 1;
                unsigned RIE_F: 1;
                unsigned RIE_N: 1;
                unsigned RIE_Q: 1;
                } ALL_BIT_SITUATION;

typedef union all_situation {
		int valore;
		ALL_BIT_SITUATION valore_bit;
		} ALL_SITUATION;

typedef Boolean (*XlCompileFunc)(
#if NeedFunctionPrototypes
    Widget,      /* widget */
    OlCompilerObject /* compilatore */
#endif
);

typedef Boolean (*XlCheckIconRegFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef Boolean (*XlCheckPortFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef Boolean (*XlCheckDispRegFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef Boolean (*XlReadFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */,
    FILE *     /* args */
#endif
);

typedef Boolean (*XlWriteFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */,
    FILE *     /* args */
#endif
);

typedef Boolean (*XlRefreshFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef void (*XlSelectProc) (
#if NeedFunctionPrototypes
    Widget      /* widget */,
    int         /* type of selection */
#endif
);

typedef Boolean (*XlDrawBorderIfSelectedFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef void (*XlSelectRunTimeProc) (
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);

typedef void (*blinkcallbackproc)(
#if NeedFunctionPrototypes
    Widget,      /* widget */
    int,         /* indice variabile cambio colore */
    Boolean      /* On/Off  */
#endif
);

/*
	metodo di stampa attraverso il tool xprinter
*/
#if defined XPRINTER_USED
typedef Boolean (*XlPrintFunc)(
#if NeedFunctionPrototypes
    Widget      /* widget */
#endif
);
#endif

/*
	Definizioni dei tipi per le conversioni di tipo non
	definiti in Xm.h e Xmu.h
*/

#if !defined (LINUX)
#define XmRFloat "Float"
#define XmRBitmap "Bitmap"
#endif

#endif  /* chiusura condiz. per non ridefinire */
