! UIMX ascii 2.5 key: 9321                                                      

*topLevelShellMain.class: topLevelShell
*topLevelShellMain.gbldecl: /*\
        Inizio sezione per SCCS\
*/\
/*\
   modulo topLevelShellMain.i\
   tipo \
   release 5.2\
   data 1/22/96\
   reserved @(#)topLevelShellMain.i	5.2\
*/\
/*\
        Variabile per identificazione della versione\
*/\
/*\
        Fine sezione per SCCS\
*/\
#include <stdio.h>\
#include <string.h>\
#include <stdlib.h>\
\
#include <sys/param.h>\
\
#ifdef SCO_UNIX\
#include <Xm/MwmUtil.h>\
#else\
#include <Xm/MwmUtil.h>\
#endif\
\
#include <Xm/List.h>\
#include <time.h> \
#ifndef DESIGN_TIME\
#include <Xl/Xl.h>\
#endif\
\
\
#include <Xl/XlUtilBkg.h>\
#include <Ol/OlDatabasePunti.h>\
#include <Ol/OlDatabaseTopologia.h>\
\
#include <Ol/OlTree.h> \
\
#ifndef DESIGN_TIME\
#include <Ol/OlPertP.h>\
#else\
#include <Ol/OlPert.h>\
#endif\
#include <Xm/Protocols.h>\
#include "other.h"\
#include "pagresdef.h"\
#include "legommi_icon.bmp"\
\
\
Widget listDrawing[MAX_PAGE];\
OlDatabasePuntiObject database_simulatore;\
OlDatabaseTopologiaObject database_topologia;\
int database_type;\
OlTreeObject root_oltree;\
\
OlPertObject pert;\
\
extern char *nome_file_context;\
extern Widget		UxTopLevel;\
Widget MessageText;\
FILE *fpLog;\
AN_PAG *pagine;\
int totale_pagine;\
\
int  display_selezionato;\
AN_DISPLAY *elenco_display;\
int numero_display;\
int shr_usr_key;\
int shr_usr_keyS;\
\
char app_pathdb[LUN_PATH];\
char app_pathproject[LUN_PATH];\
char app_hostname[LUN_PATH];\
char app_hostnameS[LUN_PATH];\
char startup_pages[MAX_STARTUP_PAGES][L_NOME_PAGINE];\
int num_startup_pages;\
Cursor cursor_waitMain;\
extern Boolean topologia_on; /* definite in other_mmi.c */\
extern int port_daemon;\
\
#define FILE_DB  "variabili.rtf"\
#define FILE_COMPONENT "FileComponent.edf"\
#define FILE_SUBSYSTEM "FileSubSystem.edf"\
#define FILE_DEFVARCC  "FileDefVarCC.edf" \
#define WARNINGSTRING   "WARNING: Sked Simulator path different from \n Context Simulator path \n Retry anyway ?                         "\
\
extern swidget popup_topLevelShellStaz();\
Boolean setta_elenco_display( );\
Boolean crea_elenco_display( );\
extern swidget create_topLevelShell1( );\
void OpenStartupPage( );\
void OpenNewPage( );\
static void  activateCB_setta_Display();\
Boolean InserisciItem();\
void openPage_proc();\
void quit_proc();\
int creaOlDbPunti(char *,Widget);\
\
extern void CheckMmiStatus( );\
void    activateCB_pushButtonQuit();\
\
extern swidget create_teleperm();\
\
extern OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *, char *, int , char *,char *,char *);\
/** risorse che vengono lette dal Context\
**/\
static XtResource res_descrizione[]= {\
                                {\
                                XlNdescription,\
                                XlCDescription,\
                                XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
static XtResource res_pathico[]= {\
                                {\
                                XlNanimatedIconLibraries,\
                                XlCAnimatedIconLibraries,\
                                XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
static XtResource res_pathpagine[]= {\
                                {\
                                XlNpages,\
                                XlCPages,\
                                XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
static XtResource res_pathsimulator[]= {\
				{\
				XlNsimulator,\
				XlCSimulator,\
				XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
			\
static XtResource res_hostname[]= {\
				{\
				XlNhostName,\
				XlCHostName,\
				XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
static XtResource res_elenco_display[]= {\
                                {\
                                XlNdisplayList,\
                                XlCDisplayList,\
                                XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
static XtResource res_numero_display[]= {\
                                {\
                                XlNnumDisplay,\
                                XlCNumDisplay,\
                                XmRInt,\
                                sizeof(int),\
                                0,\
                                XmRImmediate,\
                                0\
                                }\
                                };\
\
static XtResource res_hostnameS[]= {\
				{\
				XlNhostNameS,\
				XlCHostNameS,\
				XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
\
char *descrizione_impianto;\
char *path_ico;\
char *path_pagine;\
char *path_simulator;\
char *hostname;\
char *hostnameS;\
static char app_pathpagine[LUN_PATH];\
static char app_pathsimulator[LUN_PATH];\
static char app_pathSubSystem[LUN_PATH];\
static char app_pathComponent[LUN_PATH];\
static char app_pathDefVarCC[LUN_PATH];\
char *cdisp;\
\
int LoadCtxResDb();\
\
XrmDatabase ApplDb;  /* contiene il Db delle risorse generale della applicazione */\
/*\
Variabile globale per pagina allarmi\
*/\
Boolean allPageFlag;\
Boolean scada_on;\
Boolean alldisp=False;   /* la prima startup page va su tutti i\
                            display configurati (default No)*/\
\
extern char path_sim_correct[FILENAME_MAX];\
extern popup_Warning_sim();
*topLevelShellMain.ispecdecl: XtIntervalId timer_id;\
XmFontList FontLista, FontLabel;\
int modo_elenco;\
 /* tipo di database dei Punti */\
\
time_t time_start,time_end;
*topLevelShellMain.ispeclist: timer_id, FontLista, FontLabel, modo_elenco, time_start, time_end
*topLevelShellMain.ispeclist.timer_id: "XtIntervalId", "%timer_id%"
*topLevelShellMain.ispeclist.FontLista: "XmFontList", "%FontLista%"
*topLevelShellMain.ispeclist.FontLabel: "XmFontList", "%FontLabel%"
*topLevelShellMain.ispeclist.modo_elenco: "int", "%modo_elenco%"
*topLevelShellMain.ispeclist.time_start: "time_t", "%time_start%"
*topLevelShellMain.ispeclist.time_end: "time_t", "%time_end%"
*topLevelShellMain.funcdecl: swidget popup_topLevelShellMain()\

*topLevelShellMain.funcname: popup_topLevelShellMain
*topLevelShellMain.funcdef: "swidget", "<popup_topLevelShellMain>(%)"
*topLevelShellMain.icode: Pixmap pix;\
char app_testata[300];\
char lochostname[MAXHOSTNAMELEN + 1];\
XtIntervalId t_id,t_id_status;\
Atom WM_DELETE_WINDOW; /* per close con MWM*/\
int setApplResDb();\
char WarningString[300];\
int ris=1;\
\
time(&time_start);\
printf("INIZIO top time_start=%ld\n",time_start);\
\
if(!LoadFont(FONT_LISTA,&FontLista,UxDisplay))\
	exit(0);\
if(!LoadFont(FONT_LABEL,&FontLabel,UxDisplay))\
	exit(0);\
\
modo_elenco=MODO_ELENCO_ALL;\
\
\
/*\
	Carica il database contenente il context\
*/\
time(&time_end);\
printf("\n\n prima di LoadCtxResDb=%ld\n",(time_end-time_start));\
time(&time_start);\
if(!LoadCtxResDb())\
	exit(0);\
time(&time_end);\
printf("\n\n dopo di LoadCtxResDb=%ld\n",(time_end-time_start));\
time(&time_start);\
strcpy(app_hostname,hostname);\
strcpy(app_hostnameS,hostnameS);\
\
\
XlPutenv("LEGOMMI_ICO",path_ico);\
XlPutenv("LEGOMMI_PAGINE",path_pagine);\
\
sprintf(app_pathdb,"%s",path_simulator);\
sprintf(app_pathsimulator,"%s/%s",path_simulator,FILE_DB);\
sprintf(app_pathSubSystem,"./%s",FILE_SUBSYSTEM);\
sprintf(app_pathComponent,"./%s",FILE_COMPONENT);\
sprintf(app_pathDefVarCC,"./%s",FILE_DEFVARCC);\
\
/* setta e salva su file ApplDb.res il db delle risorse della applicazione\
*/\
time(&time_end);\
printf("Prima di setApplResDb time_end=%ld tempo trasc=%ld\n", (time_end-time_start));\
time(&time_start); \
setApplResDb();\
time(&time_end);\
printf("\n\nDopo setApplResDb tempo trasc=%ld\n", (time_end-time_start));\
/* Setto var globale se pag allarmi aperta a False*/\
allPageFlag=False;\
time(&time_start);
*topLevelShellMain.fcode: time(&time_end);\
printf("\n\nDopo Ux_build tempo trasc=%ld\n", (time_end-time_start));\
time(&time_start);\
#ifndef DESIGN_TIME\
LoadBitmap(form4,&pix,legommi_icon_bits,legommi_icon_width,legommi_icon_height);\
#endif\
set_something(rtrn,XmNiconPixmap,pix);\
set_something(labelDescrizione,XmNlabelString,\
	 XmStringCreateLtoR(descrizione_impianto,XmSTRING_DEFAULT_CHARSET));\
set_something(listPage,XmNfontList,FontLista);\
set_something(menu2_AllPages,XmNfontList,FontLabel);\
set_something(menu2_Sinottici,XmNfontList,FontLabel);\
set_something(menu2_Stazioni,XmNfontList,FontLabel);\
set_something(pushButtonQuit,XmNfontList,FontLabel);\
set_something(XmOptionLabelGadget(menuDisplay),XmNfontList,FontLabel);\
set_something(XmOptionLabelGadget(menuTipoPagina),XmNfontList,FontLabel);\
set_something(buttonOpenPage,XmNfontList,FontLabel);\
\
\
/* set hostbame della machhina dove � partito legommi\
*/\
if( gethostname(lochostname,MAXHOSTNAMELEN) != 0) \
{\
   printf("Error on gethostname. Exit.\n");\
   exit(0);\
}   \
lochostname[MAXHOSTNAMELEN] = '\0';\
\
sprintf(app_testata,"Lego-MMI - %s",lochostname);\
\
\
set_something(labelMain,XmNlabelString,\
	 XmStringCreateLtoR(app_testata,XmSTRING_DEFAULT_CHARSET));\
time(&time_end);\
printf("\n\n Prima di setta elenco display=%ld\n",(time_end-time_start));\
time(&time_start);\
\
setta_elenco_display(rtrn);\
time(&time_end);\
printf("\n\n Dopo di setta elenco display=%ld\n",(time_end-time_start));\
time(&time_start);\
UxPopupInterface(rtrn, no_grab);\
cursor_waitMain=XlCreateWaitCursor(rtrn);\
time(&time_end);\
printf("\n\n Dopo UxPopupInterface time=%ld\n",(time_end-time_start));\
time(&time_start);\
XlSetCursor(rtrn,cursor_waitMain);\
\
\
#ifndef DESIGN_TIME\
t_id = XtAppAddTimeOut (XtWidgetToApplicationContext (rtrn),\
                        (unsigned long) BLINK_RATE, XlBlink, rtrn);\
t_id_status = XtAppAddTimeOut (XtWidgetToApplicationContext (rtrn),\
                        (unsigned long) 1000, CheckMmiStatus, rtrn);\
#endif\
/*\
	registra l'indice della message list\
*/\
#ifndef DESIGN_TIME\
MessageText=scrolledTextMessage;\
#endif\
/*\
	Apre il file di log\
*/\
if((int)(fpLog=fopen("LegoMMI.log","w"))<0)\
	{\
	fpLog=NULL;\
	}\
\
SettaSelect();\
crea_elenco_display(UxAppContext);\
/*\
Creazione oggetto topologia:\
Se topologia_on -> topologia completa (con caricam. variabili.rtf in shm\
else -> topologia "leggera" cioe' solo per leggere file fnomi.rtf\
*/\
time(&time_end);\
printf("\n\n Dopo  crea_elenco_display time=%ld\n",(time_end-time_start));\
time(&time_start);\
                if(topologia_on)  /* Se mmi lanciato con opzione -Topologia */\
                        {\
			if(app_hostnameS && strlen(app_hostnameS))\
                        	XlPutenv("LEGOMMI_RTF",path_simulator);\
			database_topologia=OlCreateDatabaseTopologia("olrtf",\
                                app_pathsimulator,\
                                shr_usr_key,app_pathSubSystem,app_pathComponent,\
                                app_pathComponent);\
                if(( database_topologia==NULL ) )			\
			printf("\n\nErrore database topologia non allocato\n\n");			\
			}\
                else\
                        {\
/*\
Se deve essere attivato lo scada utilizza topologia "leggera"\
*/\
			if(app_hostnameS && strlen(app_hostnameS))\
				{\
					/* path dove si trova fnomi.rtf */\
                        	XlPutenv("LEGOMMI_RTF",path_simulator);\
                        	database_topologia=OlCreateDatabaseTopologia("olscada",\
                                			app_pathsimulator,\
                                			shr_usr_key,app_pathSubSystem,app_pathComponent,\
                                			app_pathComponent);\
                		if(( database_topologia==NULL ) )			\
					printf("\n\nErrore database topologia non allocato\n\n");               \
				}\
		        } /* end else topologia_on */\
\
time(&time_end);\
printf("\n\nCreaz.  topologia time=%ld\n",(time_end-time_start));\
/*	\
\
Creazione dell'oggetto OlTree \
        \
*/\
time(&time_start);\
show_warning("Loading hierarchy!!\n\n");\
\
if (Build_OlTree(database_topologia) == True) { \
   /* Oggetto OlTre allocato correttamente */\
   show_warning("\nHierarchy loaded!!");\
   printf("Oggetto OlTree COSTRUITO!!!\n");\
   \
/***\
   printf("\n\n****************VERIFICA!!**********************\n");\
   TestOlTree();\
****/\
\
\
}\
else {\
   show_warning("Hierarchy not loaded!!!");\
   printf("ERRORE nella costruzione dell'oggetto OlTree!!!\n");\
     \
     } \
\
time(&time_end);\
printf("\n\nCreaz. OlTree time=%ld\n",(time_end-time_start));\
/*\
	Creazione oggetto database e momentaneamente anche dell'oggetto\
	compilatore\
*/\
time(&time_start);\
if( XlGetenv("SHR_USR_KEY") != NULL )\
   shr_usr_key=atoi(XlGetenv("SHR_USR_KEY"));\
else\
{\
   printf("SHR_USR_KEY environment variable can be not set. Exit\n");\
   exit(1);\
}\
\
if( XlGetenv("SHR_USR_KEYS") != NULL )\
   shr_usr_keyS=atoi(XlGetenv("SHR_USR_KEYS"));\
else\
{\
   printf("SHR_USR_KEY environment variable can be not set. Exit\n");\
   exit(1);\
}\
\
\
#ifdef FABIO\
#ifndef DESIGN_TIME\
\
/*\
	Perte relativa al database dei Punti\
*/\
\
database_simulatore=OlCreateDatabasePunti(path_simulator,app_hostname,app_hostnameS,shr_usr_key,shr_usr_keyS,root_oltree);\
if(database_simulatore==NULL)\
        printf("\n\nErrore database simulatore non allocato\n\n");\
	\
/*\
	Controllo il tipo di database per vedere se lo scada e' abilitato\
	Se e' cosi' attivero' la interface per password\
*/\
database_type=OlDatabasePuntiType(database_simulatore);\
if(database_type<0)\
	{\
	printf("TopLevelShellMain:\n");\
	printf("\n\nErrore database simulatore di tipo non corretto\n");\
	printf("Controllare campo hostname nel Context.ctx\n");\
	}\
time(&time_end);\
printf("\n\n Creaz. OldatabasePunti time=%ld\n",(time_end-time_start));\
time(&time_start);\
if((database_type==DB_XLSCADA_CLIENT) || (database_type==DB_XLCLIENTS) )	\
	scada_on=True;\
else\
	scada_on=False;\
if(database_type==DB_XLSIMUL)\
	{\
	pert=OlCreatePert("DB_XLSIMUL",shr_usr_key);\
	printf("creato oggetto PERT per simulatore locale\n");\
	}\
else\
	pert=OlCreatePert("DB_XLCLIENTS",shr_usr_key);	\
if(pert==NULL)\
	        printf("\n\nErrore oggetto pert non allocato\n\n");\
time(&time_end);\
printf("\n\n Creaz. OlPert time=%ld\n",(time_end-time_start));\
#endif\
#endif\
\
\
if((ris=creaOlDbPunti(path_simulator,rtrn))<=0)\
   fprintf(stderr,"WARNING: Impossible to create Pointdatabase\n");\
\
time(&time_start);\
OpenStartupPage();\
XlUnsetCursor(rtrn);\
WM_DELETE_WINDOW=XmInternAtom(XtDisplay(topLevelShellMain),"WM_DELETE_WINDOW",False);\
XmAddWMProtocolCallback(topLevelShellMain,WM_DELETE_WINDOW,activateCB_pushButtonQuit,NULL);\
time(&time_end);\
printf("\n\n Ultimo pezzo time=%ld\n",(time_end-time_start));\
\
\
#ifndef DESIGN_TIME\
printf("DEBUG:topLevelShellMain:ris=%d\n",ris);\
if(database_simulatore==NULL )\
   {\
    if(ris<0)\
       {\
       \
       fprintf(stderr,"\n\n********************    WARNING    *********************\n"); \
       fprintf(stderr,"\n\nWARNING Sked path simulator=%s \n Context path simulator=%s\n\n",path_sim_correct,path_simulator);\
       fprintf(stderr,"\n\n********************************************************\n");\
       \
       strcpy(WarningString,WARNINGSTRING);\
       popup_Warning_sim(WarningString,rtrn); \
       }\
   else popup_errorDialog("ERROR: Connection with simulator failed",rtrn);\
   }\
#endif\
\
\
return(rtrn);\

*topLevelShellMain.auxdecl: /* effettua la combine del database della applicazione con quello del display \
   in modo da non perdere il settaggio delle risorse derivanti dai file delle risorse\
   $XAPPLRESDIR/run_time\
*/\
int setApplResDb()\
{\
   /* XrmDatabase ApplDb; variabile globale */\
\
   XrmCombineDatabase( UxDisplay->db, &ApplDb ,True );\
   XrmSetDatabase(UxDisplay, ApplDb );\
   XrmPutFileDatabase(ApplDb,"ApplDb.res");\
}\
\
/* carica il Db delle risorse definite nel context\
*/\
int LoadCtxResDb()\
{\
\
   XrmDatabase CtxDb;\
\
   if( (CtxDb=XrmGetFileDatabase(nome_file_context) ) == NULL )\
   {\
      printf("Error on GetFileDatabase %s - Exit.\n",nome_file_context);\
      return(False);\
   }\
\
   /* il merge permette di aggiungere le nuove risorse lette dal context\
      con quelle definite di default dal file delle risorse della applicazione\
      (ved. $XAPPLRESDIR/run_time )\
   */     \
   XrmMergeDatabases(CtxDb,&UxDisplay->db);\
\
\
   XtVaGetApplicationResources(UxTopLevel,&descrizione_impianto,res_descrizione,\
                               XtNumber(res_descrizione),NULL);\
\
   XtVaGetApplicationResources(UxTopLevel,&path_ico,res_pathico,\
                               XtNumber(res_pathico),NULL);\
\
\
   XtVaGetApplicationResources(UxTopLevel,&path_pagine,res_pathpagine,\
                               XtNumber(res_pathpagine),NULL);\
\
\
   XtVaGetApplicationResources(UxTopLevel,&path_simulator,res_pathsimulator,\
                               XtNumber(res_pathsimulator),NULL);\
\
   XtVaGetApplicationResources(UxTopLevel,&hostname,res_hostname,\
                               XtNumber(res_hostname),NULL);\
\
   XtVaGetApplicationResources(UxTopLevel,&hostnameS,res_hostnameS,\
                               XtNumber(res_hostnameS),NULL);\
\
   XtVaGetApplicationResources(UxTopLevel,&numero_display,res_numero_display,\
                               XtNumber(res_numero_display),NULL);\
\
   XtVaGetApplicationResources(UxTopLevel,&cdisp,res_elenco_display,\
                               XtNumber(res_elenco_display),NULL);\
\
   return(True);\
}\
\
\
\
\
/* apertura pagina di startup\
   - il nome della pagina proviene dai parametri di lancio di MMI\
   - per le pagine tipo teleperm permetto al max due pagine in startup\
*/\
void OpenStartupPage() \
{\
   int i,j;\
   char app_char[200];\
   int teletipo=0;\
\
   /* invio pagina di startup su tutti i display/screen configurati\
   */\
   if( alldisp && num_startup_pages )\
   {\
      printf("Apertura della pagina di startup su tutti i display configurati: [%s]\n",startup_pages[0]);\
\
      /* scorre la lista delle pagina per verificare che esiste quella di startup\
      */\
      for(i=0;i<totale_pagine;i++)\
      {\
         /* effettua la verifica case-insensitive\
         */\
         if( strcasecmp(startup_pages[0],pagine[i].nome)==0)\
         {\
printf("trovata pagina di startp [%s]\n",pagine[i].nome);\
\
            for(j=0;j<numero_display;j++)\
            {\
              printf("apro sul display %s\n",elenco_display[display_selezionato].nome);\
               display_selezionato = j;\
               OpenNewPage(i+1);\
            }\
            display_selezionato = 0;\
            return;\
         }\
      }\
     display_selezionato = 0;\
     return;\
   }\
\
   /* scorre la lista di pagine di startup, perche' possono essere piu' di una\
   */      \
   for(j=0;j<num_startup_pages;j++)\
   {\
\
      printf("Apertura della pagina: [%s]\n",startup_pages[j]);\
\
      /* scorre la lista delle pagina per verificare che esiste quella di startup\
      */\
      for(i=0;i<totale_pagine;i++)\
      {   \
         /* effettua la verifica case-insensitive\
         */  \
         if( strcasecmp(startup_pages[j],pagine[i].nome)==0)\
         {\
            if( strcmp(pagine[i].tipo,"Teleperm") == 0 )\
            {\
               if( teletipo <  elenco_display[0].num_screens)\
                  OpenNewPage(i+1);\
               teletipo++;\
            }\
            else\
               OpenNewPage(i+1);\
            break;\
         }\
      }\
\
      /* se non trova la pagina nella lista, emette uno warning e prosegue\
      */\
      if(i==totale_pagine)\
      {\
         sprintf(app_char,"Error opening page [%s]",startup_pages[j]);\
         popup_errorDialog(app_char,topLevelShellMain);\
      }\
   }\
}\
\
\
/* apertura di una nuova pagina\
*/\
void OpenNewPage(int item_sel)\
{\
   int num_pagina,i;\
   char message[200];\
   int def_scr;\
\
   /* setta il cursore di wait \
   */\
   XlSetCursor(topLevelShellMain,cursor_waitMain);\
\
   def_scr = XDefaultScreen( XtDisplay( topLevelShellMain ) );\
 \
   /* recupero l'indice di pagina nella lista da aprire e lo metto in num_pagina,\
      tengo conto del filtro sulla lista che puo' limitare la lista stessa\
      alla visualizzazione di un tipo di pagine e quindi il numero di item presenti       \
   */   \
   if(modo_elenco==MODO_ELENCO_ALL)\
      num_pagina=item_sel-1;\
\
   if(modo_elenco==MODO_ELENCO_SINOTTICI)\
   {\
      for(i=0;i<totale_pagine;i++)\
      {\
         if(strcmp(pagine[i].tipo,"Sinottico")==0)\
            --item_sel;\
	 if(item_sel<=0)\
	    break;\
      }\
\
      num_pagina=i;\
   }\
\
\
   if(modo_elenco==MODO_ELENCO_STAZIONI)\
   {\
      for(i=0;i<totale_pagine;i++)\
      {\
         if(strcmp(pagine[i].tipo,"Stazioni")==0)\
	    --item_sel;\
	 if(item_sel<=0)\
	    break;\
      }\
      num_pagina=i;\
   }\
\
   /* se la pagina e' gia aperta non faccio niente\
   */\
   if( PaginaAperta(elenco_display[display_selezionato].topLevel,\
                pagine[num_pagina].nome) )\
      return;\
\
   /* verifico che la pagina possa essere aperta\
   */ \
   if(!CanOpenPage(topLevelShellMain))\
	return;\
\
   printf("Apro la Pagina %d %s sul display %s\n",\
	num_pagina,pagine[num_pagina].nome,\
	elenco_display[display_selezionato].nome);\
\
   sprintf(message,"Open page %s on %s \n",pagine[num_pagina].nome,\
	elenco_display[display_selezionato].nome);\
\
   show_warning(message);\
\
   /* in base al tipo di pagina apro la pagina sulla shell opportuna\
   */\
   if((strcmp(pagine[num_pagina].tipo,"Sinottico")==0)||\
	(strcmp(pagine[num_pagina].tipo,"Regolazione")==0))\
   {\
      if(create_topLevelShell1(pagine[num_pagina].nome,\
  	 elenco_display[display_selezionato].topLevel,\
	 elenco_display[display_selezionato].nome,\
	 pagine[num_pagina].sfondo)==NULL)\
	 popup_errorDialog("Error opening page",topLevelShellMain);\
   }\
   else if( (strcmp(pagine[num_pagina].tipo,"Stazioni")==0) )\
   {\
      if(popup_topLevelShellStaz(pagine[num_pagina].nome,\
 	 elenco_display[display_selezionato].topLevel,\
	 elenco_display[display_selezionato].nome,\
	 pagine[num_pagina].sfondo)==NULL)\
	 popup_errorDialog("Error opening page",topLevelShellMain);\
   }\
   else if((strcmp(pagine[num_pagina].tipo,"Teleperm")==0) ) \
   {      \
      if( elenco_display[0].scr[def_scr].wteleperm == NULL )\
      { \
         elenco_display[0].scr[def_scr].wteleperm = create_teleperm(elenco_display[0].scr[def_scr].topLevel,\
                                  &pagine[num_pagina],elenco_display[0].scr[def_scr].nome);\
\
         if( elenco_display[0].scr[def_scr].wteleperm < 1)\
            	 popup_errorDialog("Error opening page",topLevelShellMain);\
         else\
         {\
            UxPopupInterface( elenco_display[0].scr[def_scr].wteleperm , no_grab);\
            XRaiseWindow( XtDisplay(elenco_display[0].scr[def_scr].wteleperm),\
                       XtWindow(elenco_display[0].scr[def_scr].wteleperm) );\
         }\
      }\
      else\
      {\
         if( elenco_display[0].num_screens > 1)\
         { \
            if( def_scr > 0 )\
               def_scr --;\
            else\
               def_scr ++;\
            elenco_display[0].scr[def_scr].wteleperm = create_teleperm(elenco_display[0].scr[def_scr].topLevel,\
                                  &pagine[num_pagina],elenco_display[0].scr[def_scr].nome);\
\
            if( elenco_display[0].scr[def_scr].wteleperm < 1 )\
            	 popup_errorDialog("Error opening page",topLevelShellMain);\
            else\
            {\
               UxPopupInterface( elenco_display[0].scr[def_scr].wteleperm , no_grab);\
               XRaiseWindow( XtDisplay(elenco_display[0].scr[def_scr].wteleperm ),\
                          XtWindow(elenco_display[0].scr[def_scr].wteleperm ));\
            }\
        }\
      }\
   }\
 \
   /* reset del cursore di wait\
   */\
   XlUnsetCursor(topLevelShellMain);\
\
}\
\
\
/* leggo le risorse numDisplay e displayList daldatabase delle risorse\
   ( sono specificate nel Context ) e setto il menu relatico ai display\
*/\
Boolean setta_elenco_display(Widget w)\
{\
   int i,lun;\
\
\
\
#ifndef DESIGN_TIME\
     \
   elenco_display=(AN_DISPLAY*)XtCalloc(numero_display+1,sizeof(AN_DISPLAY));\
 \
   if( elenco_display == NULL )\
   {\
       printf("\nError in  XtCalloc of elenco_display .\nExit.");\
       exit(1);\
   }\
\
   /* distruggo il men dei display, che ricostruisco in base \
      ai display specificati nel context\
   */ \
   XtDestroyWidget(menu2_p1_bDisplay);\
\
   numero_display=numero_display+1;\
\
   for(i=0;i<numero_display;i++)\
   {\
\
      /* se non sono stati specificati dei display, \
         uso solo qiello lacale, altrimenti costruisco una lista\
         ed aggiorno il menu\
      */ \
      if(i==0)\
      {\
         strcpy(elenco_display[i].nome,"Local display");\
 	 elenco_display[i].wb= XtVaCreateManagedWidget( elenco_display[i].nome,\
                        xmPushButtonWidgetClass, menu2_pDisplay,\
                        RES_CONVERT( XmNbackground, "#50a050" ),\
                        RES_CONVERT( XmNlabelString, "Local display" ),\
			XmNfontList,FontLabel,\
                        NULL );\
      }\
      else\
      {\
         sscanf(cdisp,"%s",elenco_display[i].nome);\
         cdisp+=(strlen(elenco_display[i].nome)+1);\
	 elenco_display[i].wb= XtVaCreateManagedWidget( elenco_display[i].nome,\
                        xmPushButtonWidgetClass, menu2_pDisplay,\
                        RES_CONVERT( XmNbackground, "#50a050" ),\
                        RES_CONVERT( XmNlabelString, elenco_display[i].nome ),\
			XmNfontList,FontLabel,\
                        NULL );\
      }\
\
      XtAddCallback(  elenco_display[i].wb, XmNactivateCallback,\
                        activateCB_setta_Display,\
                        (XtPointer) UxTopLevelShellMainContext );\
   }\
\
#endif\
\
   return(True);\
}\
\
\
/* \
   callback legata ai bottoni creati con la setta_elenco_display\
   che effettua lo switch da un display ad un'altro\
*/\
\
static void  activateCB_setta_Display(w, UxClientData, UxCallbackArg )\
        Widget          w;\
        XtPointer       UxClientData, UxCallbackArg;\
{\
int i;\
char message[200];\
\
\
        for(i=0;i<numero_display;i++)\
                if(w==elenco_display[i].wb)\
			{\
                        display_selezionato=i;\
			break;\
			}\
	sprintf(message,"Set default display on %s:0.0\n",\
		elenco_display[display_selezionato].nome);\
	show_warning(message);\
}\
\
Boolean crea_elenco_display(XtAppContext app_context)\
{\
   int i;\
   Display *display;\
   char testo_display[100];\
   int zero=0;\
   char message[200];\
   char *appo;\
   char strappo[255];\
   int default_scr,other_scr;\
\
/* inizializzazione Local Display \
*/\
#ifndef DESIGN_TIME\
   display = NULL;\
   elenco_display[0].topLevel=topLevelShellMain;\
   display = XtDisplay(elenco_display[0].topLevel);\
   elenco_display[0].num_screens = XScreenCount( display );   \
   default_scr = XDefaultScreen( display );\
#endif\
\
   if(default_scr == 0)\
      other_scr = 1;\
   else\
      other_scr = default_scr - 1;\
\
   printf("DISPLAY NAME %s\n", display->display_name);\
   printf("TOTAL SCREEN NUMBER%d\n",elenco_display[0].num_screens );        \
   printf("DEFAULT SCREEN %d\n", default_scr );        \
   printf("OTHER SCREEN %d\n", other_scr );        \
\
/* gestione multiscreen per TELEPERM\
   utilizzo il LOCAL DISPLAY come base per il doppio screen\
*/\
\
   if( elenco_display[0].num_screens > 2 )\
       printf("Teleperm use only two screens. You have on your station %d screen \n",elenco_display[0].num_screens);\
\
/* gestione degli screen.\
   Per teleperm gestisco due screen relativi al display locale, quello dove e' stato lanciato LEGOMMI    \
*/   \
   if( elenco_display[0].num_screens > 0 )\
   {\
      /* alloco memoria per tutti gli screen  */\
      /* Correzione baco:  se 1! screen -> alloca comunque other_screen */\
      if(elenco_display[0].num_screens==1)\
         {\
         elenco_display[0].scr = (AN_SCREEN *)XtCalloc(2,sizeof(AN_SCREEN));\
         if( elenco_display[0].scr == NULL)\
           {\
           printf("ERROR: malloc failed elenco_display[0].scr \n");\
           exit(1);\
           }\
         }\
      else\
         {\
         elenco_display[0].scr = (AN_SCREEN *)XtCalloc(elenco_display[0].num_screens,sizeof(AN_SCREEN));\
         if( elenco_display[0].scr == NULL)\
            {\
            printf("ERROR: malloc failed elenco_display[0].scr \n");\
            exit(1);\
            }         \
         }\
      \
      /* inizializzo  lo screen di default */\
      display = NULL;\
      elenco_display[0].scr[default_scr].topLevel = topLevelShellMain;\
      display = XtDisplay(elenco_display[0].scr[default_scr].topLevel);\
      strcpy(elenco_display[0].scr[default_scr].nome,display->display_name); \
      elenco_display[0].scr[default_scr].wteleperm = NULL;\
\
      if( elenco_display[0].num_screens > 1 )\
      {\
         /* inizializzo l'altro screen \
         */\
         /* creo un display ed una topLevel su ogni screen, che sara' usata come parent per le finestre \
          la creazione del display la faccio con XOpenDisplay e la shell con una XtAppCreateShell \
         */\
         display = NULL;\
\
         /* se il default screen e' lo 0 devo sotituire indice di screen con 1 \
          e viceversa\
         */\
         strcpy(strappo,elenco_display[0].scr[default_scr].nome);\
         appo = strrchr(strappo,'.');\
         if( appo != NULL )\
            (*appo) = 0;\
\
         sprintf(elenco_display[0].scr[other_scr].nome,"%s.%d",strappo,other_scr);\
            \
         printf("OTHER_SCRE NAME = %s\n",elenco_display[0].scr[other_scr].nome);\
                 \
         /* creo il display sull'altro screen*/	\
         display=XtOpenDisplay(app_context,elenco_display[0].scr[other_scr].nome,"run_time","run_time",\
                                      NULL,0,&zero,NULL);\
\
         /* se il display e' stato creato creo anche la topLevel */\
         if( display == NULL )\
         {\
             sprintf(testo_display,"%s: display:screen unreachable",\
		                    elenco_display[0].scr[default_scr].nome);\
	     popup_errorDialog(testo_display,topLevelShellMain);\
         }\
         else\
         {\
	     elenco_display[0].scr[other_scr].topLevel=XtAppCreateShell("topLevelShellMain","run_time",\
	        	 applicationShellWidgetClass,display,NULL,0);\
         }\
         \
         elenco_display[0].scr[other_scr].wteleperm = NULL;\
\
      }\
      else\
      {\
          strcpy( elenco_display[0].scr[other_scr].nome, "" );\
          elenco_display[0].scr[other_scr].topLevel = NULL;\
          elenco_display[0].scr[other_scr].wteleperm = NULL;\
      }\
   }   \
\
\
/* attualmente per gli altri display non gestisco screen\
   per cui teleperm NON DEVE funzionare su altri display diversi da quello di partenza di LEGOMMI\
*/\
   for(i=1;i<numero_display;i++)\
   {\
        display = NULL;\
        /* set dei display/screen. Nel context deve essere indicato nella lista display\
           anche lo screen da utilizzare es. serverX:0.0 serverX:0.1 serverY:0.0.\
           Comunque funziona anche se si specifica solo serverX:0 assumendo il default screen          \
        */\
\
	printf("Apro Display %d %s\n",i,elenco_display[i].nome);\
	sprintf(testo_display,"%s",elenco_display[i].nome);\
	sprintf(message,"Open display on %s\n",elenco_display[i].nome);\
\
	show_warning(message);\
\
	display=XtOpenDisplay(app_context,testo_display,"run_time","run_time",\
                NULL,0,&zero,NULL);\
        \
        /* verifico perche' e' fallita la XtOpenDisplay\
        */\
	if(display==NULL)\
	{\
                 strcpy(strappo,elenco_display[i].nome);\
                 appo = strrchr(strappo,':');\
                 if( appo != NULL )\
                   *appo = 0;\
\
                /* l'host non  raggiungibile o e' sconosciuto   \
                */\
		if( test_host(strappo) != 1)\
		     sprintf(testo_display,"connection to %s refused by server",\
			elenco_display[i].nome);\
		else\
		     sprintf(testo_display,"%s: display:screen unreachable",\
			elenco_display[i].nome);\
\
		popup_errorDialog(testo_display,topLevelShellMain);\
		set_something(elenco_display[i].wb,XmNsensitive,False);\
\
	}\
	else\
        { \
            /* creo la shell sul display specificato. \
               Nota il parametro ApplClass LegoMMI e' diverso dal main dove c'e' run_time \
            */\
	    elenco_display[i].topLevel=XtAppCreateShell("topLevelShellMain","run_time",\
		applicationShellWidgetClass,display,NULL,0);\
        }\
   }\
\
\
/* imposto per default il display Locale\
*/\
   display_selezionato=0;\
\
}\
\
\
\
Boolean InserisciItem()\
{\
int i;\
char riga[300];\
XmString str;\
\
XmListDeleteAllItems(listPage);\
for(i=0;i<totale_pagine;i++)\
	{\
	if((modo_elenco==MODO_ELENCO_ALL)||\
		((modo_elenco==MODO_ELENCO_SINOTTICI)&&\
		(strcmp("Sinottico",pagine[i].tipo)==0))||\
		((modo_elenco==MODO_ELENCO_STAZIONI)&&\
		(strcmp("Stazioni",pagine[i].tipo)==0)))\
	   {\
            sprintf(riga,"%d) %s - refresh = %d msec. - file[%s.rtf] - (%s)",\
               i+1,pagine[i].descrizione,pagine[i].t_refresh*100,pagine[i].nome,\
               pagine[i].tipo);\
            str=XmStringCreateLtoR(riga, XmSTRING_DEFAULT_CHARSET);\
            XmListAddItem(listPage,str,0);\
            XmStringFree(str);\
            }		       \
	}\
\
}\
\
void openPage_proc()\
{\
int item_sel;\
int *items;\
int num;\
\
if(XmListGetSelectedPos(listPage,&items,&num))\
	{\
	RtRecord(items);\
	printf("Selezionato item n = %d\n",items[0]);\
	OpenNewPage(items[0]);\
	XtFree(items);\
	}\
else\
	popup_errorDialog("No item selected",listPage);\
}\
\
void quit_proc()\
{\
	/*\
		Chiudo tutte le pagine\
	*/\
	if(!CloseAllPages(elenco_display,numero_display))\
		popup_errorDialog("Error closing pages",UxTopLevel);\
	/*\
		eventuale terminazione del processo Xaing\
	*/\
	StopAing();\
	/*\
 	chiusura perturbazioni\
	*/\
	OlDestroyPert(pert);\
\
	/*\
		chiusura databasePunti\
	*/\
	OlDestroyDatabasePunti(database_simulatore);\
	/*\
		chiusura del file di log\
	*/\
	if(fpLog!=NULL)\
		fclose(fpLog);\
	exit(0);\
}\
\
\
/*\
Funzione per chiamata esterna creazione OldatabasePunti\
*/\
int creaOlDbPunti(path_sim,Padre)\
char *path_sim;\
Widget Padre;\
{\
int res;\
   _UxCtopLevelShellMain           *UxSaveCtx = UxTopLevelShellMainContext;\
\
\
   UxTopLevelShellMainContext = (_UxCtopLevelShellMain *) UxGetContext( Padre );\
   \
   res=topLevelShellMain_createOlDbPunti(Padre,&UxEnv,path_sim);\
\
   UxTopLevelShellMainContext = UxSaveCtx;\
   return res;\
}\

*topLevelShellMain_createOlDbPunti.class: method
*topLevelShellMain_createOlDbPunti.name: createOlDbPunti
*topLevelShellMain_createOlDbPunti.parent: topLevelShellMain
*topLevelShellMain_createOlDbPunti.methodType: int
*topLevelShellMain_createOlDbPunti.methodArgs: unsigned char *path_sim;\

*topLevelShellMain_createOlDbPunti.methodBody: int ris;\
\
/*\
	Perte relativa al database dei Punti\
*/\
printf("Ricarico il database dei punti\n");\
database_simulatore=OlCreateDatabasePunti(path_sim,app_hostname,app_hostnameS,shr_usr_key,shr_usr_keyS,root_oltree,port_daemon,&ris);\
if(database_simulatore==NULL)\
        printf("\n\nErrore database simulatore non allocato\n\n");\
	\
/*\
	Controllo il tipo di database per vedere se lo scada e' abilitato\
	Se e' cosi' attivero' la interface per password\
*/\
database_type=OlDatabasePuntiType(database_simulatore);\
if(database_type<0)\
	{\
	printf("TopLevelShellMain:\n");\
	printf("\n\nErrore database simulatore di tipo non corretto\n");\
	printf("Controllare campo hostname nel Context.ctx\n");\
	}\
\
\
\
if((database_type==DB_XLSCADA_CLIENT) || (database_type==DB_XLCLIENTS) )	\
	scada_on=True;\
else\
	scada_on=False;\
if(database_type==DB_XLSIMUL)\
	{\
	pert=(OlPertObject)OlCreatePert("DB_XLSIMUL",shr_usr_key);\
	printf("creato oggetto PERT per simulatore locale\n");\
	}\
else\
	pert=(OlPertObject)OlCreatePert("DB_XLCLIENTS",shr_usr_key);	\
if(pert==NULL)\
	        printf("\n\nErrore oggetto pert non allocato\n\n");\
\
if(database_simulatore==NULL)\
   return ris;\
else\
   return 1;
*topLevelShellMain_createOlDbPunti.arguments: path_sim
*topLevelShellMain_createOlDbPunti.path_sim.def: "unsigned char", "*%path_sim%"

*topLevelShellMain.static: true
*topLevelShellMain.name: topLevelShellMain
*topLevelShellMain.parent: NO_PARENT
*topLevelShellMain.x: 116
*topLevelShellMain.y: 169
*topLevelShellMain.width: 860
*topLevelShellMain.height: 630
*topLevelShellMain.iconName: "LegoMMI"
*topLevelShellMain.title: "LegoMMI"
*topLevelShellMain.minHeight: 500
*topLevelShellMain.minWidth: 500
*topLevelShellMain.mwmDecorations: MWM_DECOR_TITLE | MWM_DECOR_RESIZEH | MWM_DECOR_MINIMIZE
*topLevelShellMain.deleteResponse: "do_nothing"

*form4.class: form
*form4.static: true
*form4.name: form4
*form4.parent: topLevelShellMain
*form4.resizePolicy: "resize_none"
*form4.unitType: "pixels"
*form4.x: 30
*form4.y: 20
*form4.width: 860
*form4.height: 630
*form4.background: "#50a050"
*form4.borderWidth: 5
*form4.borderColor: "#3d7a3d"

*labelMain.class: label
*labelMain.static: true
*labelMain.name: labelMain
*labelMain.parent: form4
*labelMain.x: 0
*labelMain.y: 0
*labelMain.width: 840
*labelMain.height: 70
*labelMain.bottomAttachment: "attach_none"
*labelMain.bottomOffset: 0
*labelMain.rightAttachment: "attach_form"
*labelMain.leftAttachment: "attach_form"
*labelMain.fontList: "newcenturyschlbk_bold24"
*labelMain.labelString: "Lego-MMI - hostname"
*labelMain.background: "#50a050"
*labelMain.bottomShadowColor: "#285128"

*pushButtonQuit.class: pushButton
*pushButtonQuit.static: true
*pushButtonQuit.name: pushButtonQuit
*pushButtonQuit.parent: form4
*pushButtonQuit.x: 710
*pushButtonQuit.y: 480
*pushButtonQuit.width: 110
*pushButtonQuit.height: 60
*pushButtonQuit.background: "#50a050"
*pushButtonQuit.labelString: "Quit"
*pushButtonQuit.activateCallback: {\
extern swidget create_questionDialogQuit();\
/*create_questionDialogQuit(topLevelShellMain,scada_on);*/\
/*\
ELIMINO LA PASSWORD IN USCITA DA MMI\
*/\
create_questionDialogQuit(topLevelShellMain,False);\
}
*pushButtonQuit.bottomAttachment: "attach_form"
*pushButtonQuit.bottomOffset: 15
*pushButtonQuit.leftAttachment: "attach_none"
*pushButtonQuit.rightAttachment: "attach_form"
*pushButtonQuit.rightOffset: 15
*pushButtonQuit.recomputeSize: "false"

*panedWindow1.class: panedWindow
*panedWindow1.static: true
*panedWindow1.name: panedWindow1
*panedWindow1.parent: form4
*panedWindow1.x: 0
*panedWindow1.y: 90
*panedWindow1.width: 863
*panedWindow1.height: 470
*panedWindow1.bottomAttachment: "attach_widget"
*panedWindow1.bottomOffset: 10
*panedWindow1.rightAttachment: "attach_form"
*panedWindow1.rightOffset: 0
*panedWindow1.topAttachment: "attach_widget"
*panedWindow1.topOffset: 0
*panedWindow1.topWidget: "labelMain"
*panedWindow1.bottomWidget: "pushButtonQuit"
*panedWindow1.leftAttachment: "attach_form"
*panedWindow1.background: "#50a050"

*formDisplay.class: form
*formDisplay.static: true
*formDisplay.name: formDisplay
*formDisplay.parent: panedWindow1
*formDisplay.resizePolicy: "resize_none"
*formDisplay.x: 3
*formDisplay.y: 0
*formDisplay.width: 857
*formDisplay.height: 123
*formDisplay.background: "#50a050"

*menuDisplay.class: rowColumn
*menuDisplay.static: true
*menuDisplay.name: menuDisplay
*menuDisplay.parent: formDisplay
*menuDisplay.rowColumnType: "menu_option"
*menuDisplay.subMenuId: "menu2_pDisplay"
*menuDisplay.x: 530
*menuDisplay.y: 70
*menuDisplay.background: "#50a050"
*menuDisplay.labelString: "Display name: "
*menuDisplay.leftAttachment: "attach_none"
*menuDisplay.leftOffset: 0
*menuDisplay.rightAttachment: "attach_form"
*menuDisplay.rightOffset: 30

*menu2_pDisplay.class: rowColumn
*menu2_pDisplay.static: true
*menu2_pDisplay.name: menu2_pDisplay
*menu2_pDisplay.parent: menuDisplay
*menu2_pDisplay.rowColumnType: "menu_pulldown"
*menu2_pDisplay.background: "#50a050"

*menu2_p1_bDisplay.class: pushButton
*menu2_p1_bDisplay.static: true
*menu2_p1_bDisplay.name: menu2_p1_bDisplay
*menu2_p1_bDisplay.parent: menu2_pDisplay
*menu2_p1_bDisplay.background: "#50a050"
*menu2_p1_bDisplay.labelString: "Default Display"
*menu2_p1_bDisplay.activateCallback.source: public
*menu2_p1_bDisplay.activateCallback: 

*menuTipoPagina.class: rowColumn
*menuTipoPagina.static: true
*menuTipoPagina.name: menuTipoPagina
*menuTipoPagina.parent: formDisplay
*menuTipoPagina.rowColumnType: "menu_option"
*menuTipoPagina.subMenuId: "menu2_p1"
*menuTipoPagina.labelString: "Tipo Pagina: "
*menuTipoPagina.x: 20
*menuTipoPagina.y: 70
*menuTipoPagina.background: "#50a050"
*menuTipoPagina.leftOffset: 30

*menu2_p1.class: rowColumn
*menu2_p1.static: true
*menu2_p1.name: menu2_p1
*menu2_p1.parent: menuTipoPagina
*menu2_p1.rowColumnType: "menu_pulldown"
*menu2_p1.background: "#50a050"

*menu2_AllPages.class: pushButton
*menu2_AllPages.static: true
*menu2_AllPages.name: menu2_AllPages
*menu2_AllPages.parent: menu2_p1
*menu2_AllPages.labelString: "All pages"
*menu2_AllPages.background: "#50a050"
*menu2_AllPages.activateCallback: modo_elenco=MODO_ELENCO_ALL;\
InserisciItem();

*menu2_p1_b2.class: separator
*menu2_p1_b2.static: true
*menu2_p1_b2.name: menu2_p1_b2
*menu2_p1_b2.parent: menu2_p1

*menu2_Sinottici.class: pushButton
*menu2_Sinottici.static: true
*menu2_Sinottici.name: menu2_Sinottici
*menu2_Sinottici.parent: menu2_p1
*menu2_Sinottici.labelString: "Sinottici"
*menu2_Sinottici.background: "#50a050"
*menu2_Sinottici.activateCallback: modo_elenco=MODO_ELENCO_SINOTTICI;\
InserisciItem();

*menu2_Stazioni.class: pushButton
*menu2_Stazioni.static: true
*menu2_Stazioni.name: menu2_Stazioni
*menu2_Stazioni.parent: menu2_p1
*menu2_Stazioni.labelString: "Stazioni"
*menu2_Stazioni.background: "#50a050"
*menu2_Stazioni.activateCallback: modo_elenco=MODO_ELENCO_STAZIONI;\
InserisciItem();

*labelDescrizione.class: label
*labelDescrizione.static: true
*labelDescrizione.name: labelDescrizione
*labelDescrizione.parent: formDisplay
*labelDescrizione.x: 90
*labelDescrizione.y: 10
*labelDescrizione.width: 720
*labelDescrizione.height: 40
*labelDescrizione.leftOffset: 30
*labelDescrizione.rightAttachment: "attach_form"
*labelDescrizione.rightOffset: 30
*labelDescrizione.leftAttachment: "attach_form"
*labelDescrizione.foreground: "#000000"
*labelDescrizione.background: "#50a050"
*labelDescrizione.borderWidth: 1
*labelDescrizione.fontList: "newcenturyschlbk_roman14"
*labelDescrizione.labelString: "Descrizione dell'impianto"

*formList.class: form
*formList.static: true
*formList.name: formList
*formList.parent: panedWindow1
*formList.resizePolicy: "resize_none"
*formList.x: 30
*formList.y: 200
*formList.width: 810
*formList.height: 210
*formList.background: "#50a050"

*labelPageList.class: label
*labelPageList.static: true
*labelPageList.name: labelPageList
*labelPageList.parent: formList
*labelPageList.x: 110
*labelPageList.y: 10
*labelPageList.width: 740
*labelPageList.height: 30
*labelPageList.labelString: "Page List"
*labelPageList.fontList: "newcenturyschlbk_roman14"
*labelPageList.background: "#50a050"
*labelPageList.leftOffset: 20
*labelPageList.rightAttachment: "attach_form"
*labelPageList.rightOffset: 20
*labelPageList.leftAttachment: "attach_form"

*scrolledWindowList.class: scrolledWindow
*scrolledWindowList.static: true
*scrolledWindowList.name: scrolledWindowList
*scrolledWindowList.parent: formList
*scrolledWindowList.scrollingPolicy: "application_defined"
*scrolledWindowList.x: 0
*scrolledWindowList.y: 0
*scrolledWindowList.width: 820
*scrolledWindowList.height: 320
*scrolledWindowList.bottomAttachment: "attach_form"
*scrolledWindowList.rightAttachment: "attach_form"
*scrolledWindowList.topAttachment: "attach_widget"
*scrolledWindowList.leftAttachment: "attach_form"
*scrolledWindowList.background: "#50a050"
*scrolledWindowList.bottomOffset: 10
*scrolledWindowList.leftOffset: 20
*scrolledWindowList.rightOffset: 20
*scrolledWindowList.topOffset: 10
*scrolledWindowList.topWidget: "labelPageList"

*listPage.class: list
*listPage.static: true
*listPage.name: listPage
*listPage.parent: scrolledWindowList
*listPage.background: "#ffffff"
*listPage.createCallback: {\
char *elenco_pagine;\
int numero_pagine;\
int i;\
char *descrizione,*tipo,*tipo_db;\
int x,y,w,h,t;\
char pagina[200],risorsa[100],appo[100];\
char riga[400];\
XrmDatabase RisDb;\
XrmValue value;\
int dim[4];\
#ifndef DESIGN_TIME\
static XtResource res_elenco_pagine[]= {\
                                {\
                                XtNpage_list,\
                                XtCPage_list,\
                                XmRString,\
                                sizeof(char *),\
                                0,\
                                XmRString,\
                                ""\
                                }\
                                };\
static XtResource res_numero_pagine[]= {\
                                {\
                                XtNpag_num,\
                                XtCPag_num,\
                                XmRInt,\
                                sizeof(int),\
                                0,\
                                XmRImmediate,\
                                0\
                                }\
                                };\
#endif\
/*\
	Carica il database contenente il context\
*/\
if(!CaricaContextBin())\
{\
\
\
#ifndef DESIGN_TIME\
XtVaGetApplicationResources(UxWidget,&numero_pagine,res_numero_pagine,\
                               XtNumber(res_numero_pagine),NULL);\
XtVaGetApplicationResources(UxWidget,&elenco_pagine,res_elenco_pagine,\
                               XtNumber(res_elenco_pagine),NULL);\
pagine=(AN_PAG*)XtCalloc(numero_pagine+1,sizeof(AN_PAG));\
#endif\
\
printf("Leggo l'elenco delle pagine %s %d\n",elenco_pagine,numero_pagine);\
#ifndef DESIGN_TIME\
totale_pagine=numero_pagine;\
for(i=0;i<numero_pagine;i++)\
        {\
        sscanf(elenco_pagine,"%s",pagina);\
        elenco_pagine+=(strlen(pagina)+1);\
	printf("Pagine %d = %s\n",i,pagina);\
	strcpy(pagine[i].nome,pagina);\
        }\
for(i=0;i<numero_pagine;i++)\
        {\
         sprintf(risorsa,"%s.top_descrizione", pagine[i].nome);\
         XrmGetResource(UxDisplay->db, risorsa, (char *)NULL, &tipo_db, &value);\
         strncpy(pagine[i].descrizione,value.addr,(int)value.size);\
\
         sprintf(risorsa,"%s.top_tipo",pagine[i].nome);\
         XrmGetResource(UxDisplay->db, risorsa, (char *)NULL, &tipo_db, &value);\
         strncpy(pagine[i].tipo,value.addr,(int)value.size);\
\
         sprintf(risorsa,"%s.refresh_freq",pagine[i].nome);\
         XrmGetResource(UxDisplay->db, risorsa, (char *)NULL, &tipo_db, &value);\
         strncpy(appo,value.addr,(int)value.size);\
         pagine[i].t_refresh=atoi(appo);\
\
         sprintf(risorsa,"%s.gerarchia",pagine[i].nome);\
         XrmGetResource(UxDisplay->db, risorsa, (char *)NULL, &tipo_db, &value);\
         strncpy(pagine[i].gerarchia,value.addr,(int)value.size);\
\
	\
	/*\
		carica il file di background\
	*/\
	sprintf(riga,"%s/%s",XlGetenv("LEGOMMI_PAGINE"),pagine[i].nome);\
	if(!leggi_file_bck(UxWidget,riga,"bkg",&(pagine[i].sfondo),dim))\
                        printf("Errore lettura file di background\n");\
	}\
#endif\
SalvaContextBin();\
}\
InserisciItem();\
\
\
}
*listPage.defaultActionCallback: {\
int item_sel;\
XmListCallbackStruct *callb;\
int num_pagina,i;\
\
\
callb=(XmListCallbackStruct *)UxCallbackArg;\
item_sel=(int)(callb->item_position);\
OpenNewPage(item_sel);\
\
}
*listPage.selectionPolicy: "single_select"
*listPage.height: 320
*listPage.listSizePolicy: "constant"
*listPage.x: -20
*listPage.y: 40

*menuOpenPage.class: rowColumn
*menuOpenPage.static: true
*menuOpenPage.name: menuOpenPage
*menuOpenPage.parent: listPage
*menuOpenPage.rowColumnType: "menu_popup"
*menuOpenPage.menuAccelerator: "<KeyUp>F4"
*menuOpenPage.background: "#50a050"

*buttonOpenPage.class: pushButton
*buttonOpenPage.static: true
*buttonOpenPage.name: buttonOpenPage
*buttonOpenPage.parent: menuOpenPage
*buttonOpenPage.labelString: "Open page"
*buttonOpenPage.background: "#50a050"
*buttonOpenPage.activateCallback: openPage_proc();
*buttonOpenPage.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"

*form5.class: form
*form5.static: true
*form5.name: form5
*form5.parent: panedWindow1
*form5.resizePolicy: "resize_none"
*form5.x: 0
*form5.y: 440
*form5.width: 860
*form5.height: 300
*form5.background: "#50a050"

*labelMessage.class: label
*labelMessage.static: true
*labelMessage.name: labelMessage
*labelMessage.parent: form5
*labelMessage.x: 110
*labelMessage.y: 10
*labelMessage.width: 740
*labelMessage.height: 30
*labelMessage.leftOffset: 20
*labelMessage.rightAttachment: "attach_form"
*labelMessage.rightOffset: 20
*labelMessage.leftAttachment: "attach_form"
*labelMessage.topOffset: 0
*labelMessage.labelString: "Message Box"
*labelMessage.fontList: "newcenturyschlbk_roman14"
*labelMessage.background: "#50a050"

*scrolledWindowMessage.class: scrolledWindow
*scrolledWindowMessage.static: true
*scrolledWindowMessage.name: scrolledWindowMessage
*scrolledWindowMessage.parent: form5
*scrolledWindowMessage.scrollingPolicy: "application_defined"
*scrolledWindowMessage.x: 0
*scrolledWindowMessage.y: 0
*scrolledWindowMessage.visualPolicy: "variable"
*scrolledWindowMessage.scrollBarDisplayPolicy: "static"
*scrolledWindowMessage.shadowThickness: 0
*scrolledWindowMessage.bottomAttachment: "attach_form"
*scrolledWindowMessage.leftOffset: 20
*scrolledWindowMessage.rightAttachment: "attach_form"
*scrolledWindowMessage.rightOffset: 20
*scrolledWindowMessage.leftAttachment: "attach_form"
*scrolledWindowMessage.topAttachment: "attach_widget"
*scrolledWindowMessage.topOffset: 10
*scrolledWindowMessage.topWidget: "labelMessage"
*scrolledWindowMessage.background: "#50a050"
*scrolledWindowMessage.foreground: "#fefefe"

*scrolledTextMessage.class: scrolledText
*scrolledTextMessage.static: true
*scrolledTextMessage.name: scrolledTextMessage
*scrolledTextMessage.parent: scrolledWindowMessage
*scrolledTextMessage.width: 810
*scrolledTextMessage.height: 70
*scrolledTextMessage.background: "#fefefe"
*scrolledTextMessage.editMode: "multi_line_edit"
*scrolledTextMessage.editable: "false"
*scrolledTextMessage.wordWrap: "true"
*scrolledTextMessage.cursorPositionVisible: "false"
*scrolledTextMessage.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"\


*menuMessageList.class: rowColumn
*menuMessageList.static: true
*menuMessageList.name: menuMessageList
*menuMessageList.parent: scrolledTextMessage
*menuMessageList.rowColumnType: "menu_popup"

*buttonClearMessage.class: pushButton
*buttonClearMessage.static: true
*buttonClearMessage.name: buttonClearMessage
*buttonClearMessage.parent: menuMessageList
*buttonClearMessage.labelString: "Clear List"
*buttonClearMessage.background: "#50a050"
*buttonClearMessage.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"\

*buttonClearMessage.activateCallback: {\
XmTextSetString( scrolledTextMessage, "" );\
}

