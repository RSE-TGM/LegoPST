/**********************************************************************
*
*       C Header:               config.h
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Thu Jul 16 15:35:03 1998 %
*
**********************************************************************/
/* 
 * CONFIG.h
 *
 * per la descrizione delle pagine di tipo DRAWING e ICON LIBRARY sono state
 * usate delle strutture ricorsive 
 *
*/

#include "pagresdef.h"
#include <Xl/Xl.h>
#include <Rt/RtMemory.h>


#ifndef _CONFIG_H
#define _CONFIG_H

#define FILEPAGINADEFAULT  "FilePaginaDefault"

#define TIPO_PAGINA          101
#define TIPO_LIBRARY         102
#define TIPO_REGOLATION      103

#define TYPE_SINOTTICO       104
#define TYPE_STAZIONE        105
#define TYPE_LIBRERIA        106
#define TYPE_REGOLAZIONE     107
#define TYPE_TELEPERM        108

#define SCHEME_NOT_IN_USE       0
#define SCHEME_IN_USE           1
#define NUMLIVELLI              6

/* vecchie definizioni da non usare piu */
#define TIPO_STAZIONI   "Stazioni"
#define TIPO_SINOTTICO  "Sinottico"
#define TIPO_LIBRERIA   "Libreria"
#define TIPO_REGOLAZIONE   "Regolazione"
#define TIPO_TELEPERM   "Teleperm"
#define TIPO_DEF        TIPO_SINOTTICO 
/****************************************/

#define DELTAX        5
#define DELTAY        5

#define COMPOSTO      1001
#define SEMPLICE      1000

#define SUCOMPOSITE 2000
#define SUDRAWING   2001

#define L_CLASSE_OGGETTI 20     /* lunghezza classe oggetti */
#define TIPO_DRAWING    1       /* identificazione tipi oggetto */
#define TIPO_OBJLIB     2
#define MAXCHAR_PAGNAME 50      /* lunghezza massima nomi pagina */
#define MAXLENFILENAME  150      /* lunghezza massima nomi file pagina */
#define MAXCHARDISPLAY  15      /* lunghezza massima nomi display */

#define APERTA True             /* stato pagina/iconlib */    
#define CHIUSA False 

#define EXIT_WITH_SAVE  1234      /* exit from application with save */
#define CLOSE_PAGE      1235      /* close the page */
#define KILL_SIMULATOR  1236      /* close the page */
#define EXIT_NO_CHANGE  1237      /* exit from application with no change */

#define MODIFY_LIBRARY      9992  /* operazioni su pagine */
#define DELETE_LIBRARY      9993      
#define CREATE_LIBRARY      9994
#define CREATE_REGOLATION   9995
#define CREATE_PAGE         9996
#define MODIFY_PAGE         9997
#define COPY_PAGE           9998
#define DELETE_PAGE         9999      /* operazioni sulla pagina */
#define CREATE_LSTSCHM      9123      /* creazione lista_schemi.dat */

#define WCUT                 1   /* id cut operations */
#define WCOPY                2   /*    copy */
#define WPASTE               3   /*    paste */

#define IMPORT_PAGE   7770      /* operazioni import page-iconlibrary */
#define IMPORT_LIB    7771


#define LOAD_CONTEXT  8000  /* definisco le operazioni sul context */
#define SAVE_CONTEXT  8001    
#define DEF_CONTEXT   8002  /* define new contet */
#define MOD_CONTEXT   8003  /* modify context */
#define SAVE_AND_LOAD_NEWCTX  8004  /* save,define e load di un nuovo context */
#define SAVE_AND_LOAD_OLDCTX  8005  /* save e load di un vecchio context */
#define ALIGN_CONTEXT 8006  /* allinea il context alle pagine CAPPE */

#define PRINT_ALL     8007  /* stampa di tutte le pagine caricate nel context */
#define SEVERE    91   /* levello severita' errori */
#define WARNING   92
#define MESSAGGE  93

#define MAXPAGOPEN 5  /* definisco il numero di pagine che possono essere
 			 contemporaneamente aperte */

#define XDEF  100
#define YDEF  100
#define MIN_WIDTH  300
#define MIN_HEIGHT 300
#define BACKGROUND_DEF "lime green"
#define DEF_SNAP	1
#define DEF_REG_SNAP	8
#define MAX_SNAP	100

#define STD_LIB_NAME     "std"   /* nome libreria standard */

/* estensioni file */
#define CTX_EXT          ".ctx"
#define PAG_EXT          ".pag"
#define BKG_EXT          ".bkg"
#define LIB_EXT          ".lib"
#define RTF_EXT          ".rtf"
#define REG_EXT          ".reg"
#define ERR_EXT          "_err"


#define MAXWIDFORLIST 300       /* numero massimo di widget per lista */

/* definizioni stringhe translation table */
#define ADD_ICON_TRANS      "#override <Btn2Down>: istanzia_widget(%lx)"
#define ADD_ICON_PARENT     "#augment  <Btn2Down>: istanzia_widget_parent(%lx)"
#define ADD_ICON_PARENT_PARENT     "#augment  <Btn2Down>: istanzia_widget_parent_parent(%lx)"
#define SELECT_ZONE_TRANS   "#override <Btn1Down>: select_zone(%lx %lx)\n<KeyPress>Right:move_tasti()\n<KeyPress>Left:move_tasti()\n<KeyPress>Up:move_tasti()\n<KeyPress>Down:move_tasti()"
#define SELECT_ZONE_AUGM    "#augment  <Btn1Down>: select_zone(%lx)\n<KeyPress>Right:move_tasti()\n<KeyPress>Left:move_tasti()\n<KeyPress>Up:move_tasti()\n<KeyPress>Down:move_tasti()"
#define MOVE_WIDGET_TRANS   "#override <Btn2Down>: move_or_resize(%lx)"
#define MOVE_WIDGET_AUGM    "#augment  <Btn2Down>: move_or_resize(%lx)"
#define MOVE_PARENT_TRANS   "#augment  <Btn2Down>: move_or_resize(%lx)"

/* nomi delle variabili di enviroment definite e utilizzate runtime */
#define ENVCTX "LEGOMMI_CTX"
#define ENVPAG "LEGOMMI_PAG"
#define ENVLIB "LEGOMMI_LIB"
#define ENVICO "LEGOMMI_ICO"
#define ENVRTF "LEGOMMI_RTF"
#define WORK_PAG "LEGOMMI_WORK" /* 21/11*/


#define MAXDESCR 300
#define MAXLENHOSTNAME 100

#define DIMTAG 2               /* lunghezza tag 2 char */

extern XmString errorString;
extern int error_level;      /* livello severita' degli errori */
extern char *error_msg[];

/* Struttura contenente le risorse relative alle pagine aggiunte
   nel file Context.ctx. (CAPPE)				 */
typedef struct {
   char descr_page[MAXDESCR];
   char type_page[15];
   char tagPag[DIMTAG+1];
   int  refresh_freq;
   char hier[50];
   int  in_use;
} INFO_PAGE;

/* struttura descrittiva enviroment corrente */
typedef struct {
   char fileContext[MAXLENFILENAME];   /* nome del file Context */
   char descrizione[MAXDESCR];         /* descrizione del Project */
   char project[MAXLENFILENAME];       /* path pagine context */
   char pages[MAXLENFILENAME];         /* path pagine .pag e .bkg */
   char objectLibrary[MAXLENFILENAME]; /* path libreria oggetti .lib */
   char animatedIconLibrary[MAXLENFILENAME];   /* path libreria icone animate .ico */ 
   char simulator[MAXLENFILENAME];   /* path variabili simulatore .rtf */
   int  pag_num;                       /* numero pagine definite */
   char **page_list;                   /* lista nomi file risorse pagina */
   INFO_PAGE **res_page;	       /*lista risorse pagina da Context.ctx*/
   int  iconlib_num;                   /* numero icon library definite */
   char **iconlib_list;                /* lista nomi file risorse object library */
   char **iconlib_label;               /* lista label object library (utilizzate nel menu) */
   int  num_display;                   /* numero display configurati per run_time*/
   char **elenco_display;              /* elenco display configurati per run_time*/
   Widget topwidget;                   /* id widget top level shell del configuratore */
   XrmDatabase db;                     /* database risorse */ 
   char hostname[MAXLENHOSTNAME];      /* nome host (+ codice) per simulatore*/
   char hostnameS[MAXLENHOSTNAME];      /* nome host (+ codice) per scada */
   char nextTagPag[DIMTAG+1];                    /* prossima TAG di pagina */
} PAGEDIT_CONTEXT;


/* descrizione geometry della pagina */
typedef struct pag_geometry {

   Position x;               /* posione e dimensione della Shell */
   Position y;
   Dimension width;
   Dimension height;
   Dimension draw_width;     /* dimensione della drawing area */
   Dimension draw_height;
   char  background[100];  /* sfondo della drawing area */
   char descriz[200];           /* descrizione */
   char descriz_2[200];           /* descrizione */
   char descriz_3[200];           /* descrizione */
   char descriz_4[200];           /* descrizione */
   char tipo[15];                    /* tipo pagina ancora usato ma da abolire */
#ifdef NUOVA_VERS
/* per future implementazioni da utilizzare al posto del campo tipo */
   int tipo_pag;                    /* tipo pagina nuovo  */
#endif
   Dimension height_ext2;

} PAG_GEOMETRY;

/* descrizione della pagina di tipo DRAWING */
typedef struct pagina {
   char nomepag[MAXCHAR_PAGNAME];          /* nome della pagina */
   char filepag[MAXLENFILENAME];           /* nome file della pagina estensione .pag */
   char filebkg[MAXLENFILENAME];           /* nome file background  estensione  .bkg */
   PAG_GEOMETRY geom;
   Widget topwidget;            /* id topLevelShell della PAGINA*/
   Widget drawing;              /* id drawing area */
   XrmDatabase db;              /* database risorse pagina */
   Cardinal num_widget;         /* numero widget nella pagina */
   char *elenco_wid;            /* elenco dei widget nella pagina */
   int stati;                   /* stato della pagina 0=chuisa 1=aperta ... */
   int next_num;                /* numero progressivo widget pagina */
#ifndef DESIGN_TIME
   struct pagina *pagsucc;      /* pagina successiva */
#else
   int *pagsucc;
#endif
   /* 
     aggiunta di Fabio 15/7/96
   */
   char gerarchia[50];		/* gerarchia  della pagina*/
   int refreshFreq;             /* frequenza refresh pagina in centesimi di secondo */
   char tagPag[DIMTAG+1];              /* tag della pagina */
   char  nextTagRegol[DIMTAG+1];         /* prossima tag di regolazione */
   int numTagReg;                 /*numero icone di regolazione */
   int in_use;                  /* definisce se una pagina di regolazione e'
				   in uso per la generazione della task  */
   int dispreg_visual_mode;    /* modalita' di visualizzazione dei DispReg */
   int port_visual_mode;    /* modalita' di visualizzazione delle porte */
   int interfaceport_visual_mode;  /* modalita' di visualizzazione delle porte
					di interfaccia */
   char compilationDate[20];
   char regolationDate[20];
   char reg_task_sigla[5];  /* prime 2 lettere di task reg */
   int snap_pag;            /* snap settato */
   Pixmap sfondo;           /* eventuale pixmap di sfondo */
} PAGINA; 

/* descrizione delle pagine tipo ICON LIBRARY */
typedef struct {

   Widget topwidget;            /* widget della top level shell  della ICONLIB */
   Widget drawing;              /* widget della area di drawing */
   XrmDatabase db;              /* database icon library */
   int num_widget;              /* numero degli widget */
   AN_OBJ *listaobj;
   Dimension draw_width;     /* dimensione della drawing area */
   Dimension draw_height;
   
} ICONLIB;


/* per individuazione vertice in move resize widget */

typedef struct _Rettang {
                 int px;           /* coordinata x espressa in % */
                 int py;           /* coordinata y espressa in % */
                 int pwidth;       /* percentuale della width del widget */
                 int pheight;      /* percentuale della height del widget */
                 int rect;         /* indice del rettangolo individuato */
               } Rettang;
   
#define RECT1    1
#define RECT2    2
#define RECT3    3
#define RECT4    4
#define RECT5    5
#define RECT6    6
#define RECT7    7
#define RECT8    8
#define RECTMOVE 9
  

#define X0  0
#define X1  25
#define X2  75
#define Y0  0
#define Y1  25
#define Y2  75
#define W0  25
#define H0  25
#define W1  50
#define H1  50

#define  RectangleWidgetNum   9
  
/* per individuazione vertice in move resize widget */

static Rettang RectanglesWidget[] = {
               {
                  X1,
                  Y1,
                  W1,
                  H1,
                  RECTMOVE,
               },
               {
                  X0,
                  Y0,
                  W0,
                  H0,
                  RECT1,
               },
               {
                  X1,
                  Y0,
                  W1,
                  H0,
                  RECT2,
               },
               {
                  X2,
                  Y0,
                  W0,
                  H0,
                  RECT3,
               },
               {
                  X2,
                  Y1,
                  W0,
                  H1,
                  RECT4,
               },
               {
                  X2,
                  Y2,
                  W0,
                  H0,
                  RECT5,
               },
               {
                  X1,
                  Y2,
                  W1,
                  H0,
                  RECT6,
               },
               {
                  X0,
                  Y2,
                  W0,
                  H0,
                  RECT7,
               },
               {
                  X0,
                  Y1,
                  W0,
                  H1,
                  RECT8,
               }
            };
     

/* definizione valori ASCII per TAG pagine e Regolazioni */

#define ASCII0    48    /* ascii value 0 */
#define ASCII9    57    /* ascii value 9 */
#define ASCIIA    65    /* ascii value A */
#define ASCIIZ    90    /* ascii value Z */


#define CONTEXT_LOAD    550
#define CONTEXT_NOLOAD  551
#define CONTEXT_SAVE    552
#define CONTEXT_CHANGE  553

#define HOLE 45







/*
 definizione delle confinfo per il configuratore della pagina
*/
#ifndef DESIGN_TIME
static XlConfInfo confinfo_pag[] = {
        {
        XlNnomePag,
        XlCNomePag,
        XlDnomePag,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,nomepag[0]),
        sizeof(char)*MAXCHAR_PAGNAME,
        },
        {
        XlNdescrPag,
        XlCDescrPag,
        XlDdescrPag,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.descriz[0]),
        sizeof(char)*200,
        },
        {
        XlNdescrPag_2,
        XlCDescrPag_2,
        XlDdescrPag_2,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.descriz_2[0]),
        sizeof(char)*200,
        },
        {
        XlNdescrPag_3,
        XlCDescrPag_3,
        XlDdescrPag_3,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.descriz_3[0]),
        sizeof(char)*200,
        },
        {
        XlNdescrPag_4,
        XlCDescrPag_4,
        XlDdescrPag_4,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.descriz_4[0]),
        sizeof(char)*200,
        },
        {
        XtNelenco_wid0,
        XtCElenco_wid0,
        XtDelenco_wid0,
        XlRText,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,elenco_wid),
        sizeof(char *),
        },
        {
        XlNtipoPag,
        XlCTipoPag,
        XlDtipoPag,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.tipo[0]),
        sizeof(char)*15,
        },
        {
        XlNxPag,
        XlCXPag,
        XlDxPag,
        XlRPosition,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.x),
        sizeof(Position),
        },
        {
        XlNyPag,
        XlCYPag,
        XlDyPag,
        XlRPosition,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.y),
        sizeof(Position),
        },
        {
        XlNwidthPag,
        XlCWidthPag,
        XlDwidthPag,
        XlRDimension,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.width),
        sizeof(Dimension),
        },
        {
        XlNheightPag,
        XlCHeightPag,
        XlDheightPag,
        XlRDimension,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.height),
        sizeof(Dimension),
        },
        {
        XlNwidthDraw,
        XlCWidthDraw,
        XlDwidthDraw,
        XlRDimension,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.draw_width),
        sizeof(Dimension),
        },
        {
        XlNheightDraw,
        XlCHeightDraw,
        XlDheightDraw,
        XlRDimension,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.draw_height),
        sizeof(Dimension),
        },
        {
        XlNtagPag,
        XlCTagPag,
        XlDtagPag,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,tagPag[0]),
        sizeof(char)*(DIMTAG+1),
        },
        {
        XlNsnapPag,
        XlCSnapPag,
        XlDsnapPag,
        XlRInt,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,snap_pag),
        sizeof(int),
        },
        {
        XtNnum_widget,
        XtCNum_widget,
        XtDnum_widget,
        XlRInt,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,num_widget),
        sizeof(int),
        },
        {
        XtNnextnum,
        XtCNextnum,
        XtNnextnum,
        XlRInt,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,next_num),
        sizeof(int),
        },
        {
        XlNnumTagReg,
        XlCNumTagReg,
        XlDnumTagReg,
        XlRInt,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,numTagReg),
        sizeof(int),
        },
        {
        XlNnextTagReg,
        XlCNextagReg,
        XlDnextTagReg,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,nextTagRegol[0]),
        sizeof(char)*(DIMTAG+1),
        },
        {
        XlNschemeInUse,
        XlCSchemeInUse,
        XlDschemeInUse,
        XlRInt,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,in_use),
        sizeof(int),
        },
        {
        XlNbackgroundDraw,
        XlCBackgroundDraw,
        XlDbackgroundDraw,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.background[0]),
        sizeof(char)*100,
        },
        {
        XlNgerarchia,
        XlCGerarchia,
        XlDgerarchia,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,gerarchia[0]),
        sizeof(char)*50,
        },
	{
        XlNcompilationDate,
        XlCCompilationDate,
        XlDcompilationDate,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,compilationDate[0]),
        sizeof(char)*20,
        },
        {
        XlNregolationDate,
        XlCRegolationDate,
        XlDregolationDate,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,regolationDate[0]),
        sizeof(char)*20,
        },
        {
        XlNheightExt2,
        XlCHeightExt2,
        XlDheightExt2,
        XlRDimension,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,geom.height_ext2),
        sizeof(Dimension),
        },
	{
        XlNregTaskSigla,
        XlCRegTaskSigla,
        XlDregTaskSigla,
        XlRTextAlloc,
        0,
        NULL,
        XlRGrIo,
        XtOffsetOf(PAGINA,reg_task_sigla[0]),
        sizeof(char)*5,
        },
};
#endif


/*
 * Prototiping di funzioni di uso generale
 */
/*
 funzioni contenute in pagina.c
*/
Boolean WidSelectedAreInEdit(PAGINA* );
Boolean PagGetInUse(PAGINA *);
Boolean pagina_init(char *,PAGINA **,int);
Boolean pagina_load_file(PAGINA * , int);
char *format_hierarchy(char *);
char *hierarchy_format(char *);
Boolean pagina_getres(PAGINA *,AN_OBJ**);
Boolean pagina_save(PAGINA* ,int, int);
PAGINA *GetPaginaFromWidget(Widget);
PAGINA *GetPaginaFromName(char *);
char *GetTypeFromName(char *);
Boolean RaisePagina(char *);
Boolean PagIsOpen(char *);
Boolean cvtPixelFromString(Widget,char *,Pixel *);
/*
	chiamate relative alle risorse della pagina
*/
void DisplayResPagina(PAGINA *);
Boolean PutResPaginaDb(PAGINA *,XrmDatabase *);
Boolean GetResPaginaDb(PAGINA *,XrmDatabase , Boolean);
#endif   


void testata(char *, char *);
void XlErrComp(char* ,char *,char *,char *);
void pagina_free(PAGINA *);



// GUAG2025 Estratto da RtMemory 
void *RtMalloc(size_t,char *,int);
void *RtCalloc(size_t,size_t,char *,int);
void *RtRealloc(void *,size_t,char *,int);
void  RtFree(void *,char *, int);
char *  RtNewString(char *,char *,int);
size_t RtMemoryAllocated(void *);
Boolean RtCheckPointer(void *);
void * RtIntRecord(void *, char *, int);
/*
	ridefinizione delle chiamate 
*/
#define RtRecord(punt)  RtIntRecord(punt,__FILE__,__LINE__)
#define malloc(size)	RtMalloc(size,__FILE__,__LINE__)
#define free(punt)	RtFree(punt,__FILE__,__LINE__)
#define realloc(punt,size)	RtRealloc(punt,size,__FILE__,__LINE__)
#define calloc(num_elet,size_ele)	RtCalloc(num_elet,size_ele,__FILE__,__LINE__)

#define XtMalloc(size)	RtMalloc(size,__FILE__,__LINE__)
#define XtFree(punt)	RtFree(punt,__FILE__,__LINE__)
#define XtRealloc(punt,size)	RtRealloc(punt,size,__FILE__,__LINE__)
#define XtCalloc(num_elet,size_ele)	RtCalloc(num_elet,size_ele,__FILE__,__LINE__)
#define XtNewString(stringa)	RtNewString(stringa,__FILE__,__LINE__)
// fine Estratto da RtMemory 

char *alloca_memoria(int , int );
int libera_memoria(char *);
int new_save_context(PAGEDIT_CONTEXT *);
int compile_all_reg();
Boolean ContextLoad();
void SetMsg();
void iconlib_buttCB(Widget ,int , XmAnyCallbackStruct *);
void set_connect(Widget,int);
Widget XlGetSelectedWidget (Widget );
int get_selected(PAGINA *,Widget **,Cardinal *);
int PagGetType(PAGINA *);
int delete_widget(PAGINA *);
void DrawGroup(Widget);
void DrawUngroup(Widget); 
Widget	create_drawShell( );
int show_message(char *);
void DrawSetZoom(Widget,float);
void PaginaSetZoom(Widget,float);
void pagina_setta_resource(PAGINA *);
int pagina_create_widget(PAGINA *, AN_OBJ *);
void undo_draget_copy(Widget,Widget);

extern  void *		UxNewContext();

int iconlib_init( char *, ICONLIB ** );
int iconlib_getres(ICONLIB *,AN_OBJ **);
int iconlib_create_widget(ICONLIB *,AN_OBJ *);
void iconlib_def_translation(ICONLIB *);
void pag_wgeometry(FILE *,PAGINA *);
int add_library_item(XmString ,int );
void modify_item(XmString , int );
Boolean SettaTagInPag(char * , char * );
void modify_library_item(XmString ,int );
void get_geometry(Widget ,Position *,Position *,Dimension  *,Dimension *); 
void CreaClipboardList();
int display_resources(PAGEDIT_CONTEXT *);
void do_cancella(char *,int );
void DrawSetSnap(Widget,int);
int setPageditEnv(PAGEDIT_CONTEXT *);
int verifica_direttori_context(PAGEDIT_CONTEXT *);
int TagPagSetNew(PAGEDIT_CONTEXT * ,int);
int clear_topLevel();
void SettaToggle(Widget );
Boolean SetRegoDisplayMode(Widget ,int , int , int );
Boolean GetRegoDisplayMode(Widget ,int *, int *, int *);
Boolean OlDestroyDatabaseTopologia(OlDatabaseTopologiaObject );
int estrai_nome(char *, char *);
Boolean XlIsXlManager(Widget );
void removefromlibrarylist(int );
Boolean XlCreatePixmapFromGIF(Widget, char*, Pixmap *);
void desel_all(PAGINA *);
void assign_name(PAGINA *,int ,Widget ,char *);
Boolean PagGetConnectMode(Widget );
int TagPagCalcNext(char *,char *);
//int OlFindConnectionByPort(OlConnObject conn, char *ind_port, char* ind_icon,char *nome_pag);
Widget	create_ClipBoard();
void DrawDeleteAll(Widget,XEvent*);
//void undoListDeleteAll(XdListaUndo);
void EndPick(Widget,XEvent *);
int resize_widget(PAGINA *,Widget ,XEvent *,int );
int move_widget(PAGINA *,Widget ,XEvent *);
void StartPageditMove(Widget);
int select_connections(int,Widget *,XRectangle *,int);
void get_punto_porta(Widget,int*,int*);
int delete_estr_conn(Widget,int,int,int*,int*,int*,int*);
void AddTransWid(PAGINA *,Widget );
void DeselectAllDraget(Widget, Boolean);




/* fine config.h */
