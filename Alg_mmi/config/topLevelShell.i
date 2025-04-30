! UIMX ascii 2.5 key: 7521                                                      

*topLevelShell.class: topLevelShell
*topLevelShell.gbldecl: /*\
        Inizio sezione per SCCS\
*/\
/*\
   modulo topLevelShell.i\
   tipo \
   release 5.1\
   data 11/13/95\
   reserved @(#)topLevelShell.i	5.1\
*/\
/*\
        Variabile per identificazione della versione\
*/\
/*\
        Fine sezione per SCCS\
*/\
\
#include <stdio.h>\
#include <stdlib.h>\
#include <unistd.h>\
#include <sys/access.h>\
#include <string.h>\
#include <Xm/Xm.h>\
#include <Xm/List.h>\
#include <Xd/Xd.h>\
#include <Xd/XdLista.h>\
#include <Ol/OlTree.h>\
\
#include "config.h"\
\
#include "message.h"\
\
#ifdef XPRINTER_USED\
#include "libutilx.h"\
#endif\
\
XtAppContext CompileAppContext = NULL;\
Display *CompileDisplay;\
Widget CompileTopLevel;\
Widget ClipBoardForCopy; /* interfaccia usata per le\
                             funzionalita' di copy and\
                             paste */\
PAGEDIT_CONTEXT *pagedit;\
extern Arg args[];\
extern Cardinal lnargs;\
extern swidget create_PagDialog();\
extern swidget create_ClipBoard();\
int Already_Selected =False;\
extern int gestMessageBox();   /* gestione conferma operazioni */\
extern int pagedit_context_init();\
extern OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *, char *, int , char *,char *,char *);\
/* lista widget e indici pagine aperte \
*/\
Widget PagOpen[MAXPAGOPEN];\
int    PagOpenId[MAXPAGOPEN];\
unsigned short npagopen;\
\
OlCompilerObject compilatore_severe;\
OlDatabaseTopologiaObject dbtopologia;\
OlDatabasePuntiObject     dbpunti;\
\
\
Boolean OperationConfirm;  /* richiede conferma cancellazione pagina */\
\
Boolean CompilerOk = True;  /* gestisce il menu di compila nelle pagine */ \
\
FILE *fpLogMsg;\
\
Boolean ContextLoaded;      /* True se c'e' un Context caricato */\
Boolean ContextModified;    /* True se un Context e' stato modificato */\
\
Boolean StateInterfaceMode = False;    /* flag interfaccia connessione tra pagine */\
\
extern char  Context_Path[];\
\
extern int flag_demo;\
\
OlTreeObject root_oltree;\
\
/*  */\
void add_item( );\
int add_library_item( );\
void removefromlist( );\
void removefromlibrarylist( );\
void setContextTitle( );\
void disabilitaMoveMenu( );\
void abilitaMoveMenu( );\
void TopMenuSetInsensitive( );\
\
void TopMenuSetSensitive( );\
\
int display_resources( );\
int gest_conf_op( );\
Boolean LibraryIsOpen( );\
int open_page( );\
int modify_page( );\
int duplicate_page( );\
int del_page( );\
\
int show_message( );\
\
int LibraryDeselectAllItem( );\
int PageDeselectAllItem( );\
int clear_topLevel( );\
\
Boolean AlignResPageContext();\
void PrintAllPages();\
int CheckOlTree();\
\

*topLevelShell.ispecdecl: int PaginaSelezionata;\
swidget CompileBoard;\

*topLevelShell.ispeclist: PaginaSelezionata, CompileBoard
*topLevelShell.ispeclist.PaginaSelezionata: "int", "%PaginaSelezionata%"
*topLevelShell.ispeclist.CompileBoard: "swidget", "%CompileBoard%"
*topLevelShell.funcdecl: swidget create_topLevelShell()\

*topLevelShell.funcname: create_topLevelShell
*topLevelShell.funcdef: "swidget", "<create_topLevelShell>(%)"
*topLevelShell.icode: int appo,shr_usr_key=10000;\
\
char *getenv();\
char varrtf[MAXLENFILENAME];\
char varcc[MAXLENFILENAME];\
char varsub[MAXLENFILENAME];\
char varcom[MAXLENFILENAME];\
char mesg[200];\
\
npagopen = 0;\
fpLogMsg = fopen("config.log","w");\
\
\

*topLevelShell.fcode: if(ContextLoad("Context.ctx",&pagedit))\
{\
   display_resources(pagedit);\
   TopMenuSetSensitive();\
}\
else\
{\
      sprintf(mesg,"Context File Not Found %s.\n",pagedit->fileContext);\
      show_message(mesg);\
\
      TopMenuSetInsensitive();\
} \
\
/*\
 Creazione (senza visualizzazione) della\
 clipboard per copy and paste\
*/\
ClipBoardForCopy=create_ClipBoard();\
\
if(getenv(ENVRTF) != NULL)\
{\
   strcpy(varrtf,XlGetenv(ENVRTF));\
   strcat(varrtf,"/variabili.rtf");\
   strcpy(varrtf,XlConvPathVms(varrtf));\
}\
else\
   strcpy(varrtf,"variabili.rtf");\
\
if( getenv(ENVCTX) != NULL)\
{\
   strcpy(varcc,XlGetenv(ENVCTX));\
   strcat(varcc,"/FileDefVarCC.edf");\
   strcpy(varcc,XlConvPathVms(varcc)); \
}\
else\
   strcpy(varcc,"FileDefVarCC.edf");\
\
if( getenv(ENVRTF) != NULL)\
{\
   strcpy(varsub,XlGetenv(ENVRTF));\
   strcat(varsub,"/FileSubSystem.edf");\
   strcpy(varsub,XlConvPathVms(varsub));\
}\
else\
   strcpy(varsub,"FileSubSystem.edf");\
\
\
if( getenv(ENVCTX) != NULL)\
{\
   strcpy(varcom,XlGetenv(ENVCTX));\
   strcat(varcom,"/FileComponent.edf");\
   strcpy(varcom,XlConvPathVms(varcom));\
}\
else\
   strcpy(varcom,"FileComponent.edf");\
\
if(getenv("SHR_USR_KEY") != NULL)\
   shr_usr_key = atoi(getenv("SHR_USR_KEY"));\
\
\
if( (dbtopologia = OlCreateDatabaseTopologia("olrtf",varrtf,shr_usr_key,\
		                         varsub,varcom,varcc)) == NULL)\
{\
   sprintf(mesg,"Topology Database not Loaded\n");\
   show_message(mesg);\
\
   /* disabilita menu' compile */\
   CompilerOk = False;\
\
}\
else\
{\
   compilatore_severe = OlCreateCompiler("olsevere",dbtopologia);\
\
   if( compilatore_severe == NULL )\
   {\
      sprintf(mesg,"Compiler not Loaded\n");\
      show_message(mesg);      \
      /* disabilita menu compile */\
      CompilerOk = False;\
   } \
}\
\
/*\
   Nel caso in cui la licenza disponibile e' di tipo 'demo', vengono \
   disabilitate alcune funzionalita': compilazione e salvataggio delle pagine.\
*/\
   if(flag_demo == 1)\
      XtVaSetValues(menu2_top_b3, XmNsensitive, False, NULL);\
\
return(rtrn);\

*topLevelShell.auxdecl: /*****************************************************************\
\
  Function:    add_item\
\
  Parameter:   XmString cstring;   stringa da inserire nella lista\
               int pos;            posizione dell'item nella lista\
\
  Description:  con pos 0 aggiunge in fondo\
                        1 in cima\
                        qualsiasi altro valore nella posizione\
                        relativa\
\
****************************************************************/\
\
void add_item(XmString cstring,int pos)\
{\
   XmListAddItem(ListPagine,cstring,pos);\
   XmListSetBottomPos(ListPagine,0);\
   XmUpdateDisplay(ListPagine);\
}\
\
void modify_item(XmString newstring, int pos)\
{\
   XmListReplaceItemsPos(ListPagine,&newstring,1,(pos+1));\
   XmUpdateDisplay(ListPagine);\
}\
\
int add_library_item(XmString cstring,int pos)\
{\
   XmListAddItem(LibraryList,cstring,pos);\
   XmListSetBottomPos(LibraryList,0);\
   XmUpdateDisplay(LibraryList);\
}\
\
void modify_library_item(XmString newstring,int pos)\
{\
   XmListReplaceItemsPos(LibraryList,&newstring,1,(pos+1));\
   XmUpdateDisplay(LibraryList);\
}\
\
void removefromlist(int pos)\
{\
   XmListDeletePos(ListPagine,pos);\
   XmUpdateDisplay(ListPagine);\
}\
\
void removefromlibrarylist(int pos)\
{\
   XmListDeletePos(LibraryList,pos);\
   XmUpdateDisplay(LibraryList);\
}\
\
/* setta la label con il nome del Context attivo */ \
void setContextTitle(PAGEDIT_CONTEXT *ped)\
{\
   XmString mstring;\
\
   mstring = XmStringCreateSimple(ped->fileContext);\
   set_something(ContextLbl,XmNlabelString,mstring);\
   XmStringFree(mstring);\
}\
\
void disabilitaMoveMenu()\
{\
/** bottoni che non esistono piu'\
   set_something(menu2_top_b1,XmNsensitive,False);\
   set_something(MovePage,XmNsensitive,False);\
   set_something(MoveIcLib,XmNsensitive,False);\
**/\
}\
\
void abilitaMoveMenu()\
{\
/**** bottoni che non esistono piu'\
   if(npagopen == 0)\
   {\
      set_something(menu2_top_b1,XmNsensitive,True);\
      set_something(MovePage,XmNsensitive,True);\
      set_something(MoveIcLib,XmNsensitive,True);\
   }\
******/\
}\
\
void TopMenuSetInsensitive()\
{\
   set_something(menu1_top_b5,XmNsensitive,False);\
   set_something(menu1_top_b6,XmNsensitive,False);\
/*   set_something(menu2_top_b1,XmNsensitive,False);  non esiste piu'*/\
   set_something(menu2_top_b2,XmNsensitive,False);\
   set_something(menu2_top_b3,XmNsensitive,False);\
}\
\
void TopMenuSetSensitive()\
{\
   set_something(menu1_top_b5,XmNsensitive,True);\
   set_something(menu1_top_b6,XmNsensitive,True);\
/*   set_something(menu2_top_b1,XmNsensitive,True); non esiste piu' */ \
   set_something(menu2_top_b2,XmNsensitive,True);\
   set_something(menu2_top_b3,XmNsensitive,True);\
}\
\
\
/*------------------------------------------------------------------\
 * display resources\
 *\
 * effettua il display nella interfaccia delle risorse caricate\
 * rimuove la lista delle pagine e delle librerie \
 *  e mette le nuove liste\
 *-----------------------------------------------------------------*/\
\
int display_resources(PAGEDIT_CONTEXT *paged)\
{\
   XmString item,mstring;\
   int i,j,pos = 0;\
   Cardinal num_item;\
   PAGINA *pag;\
   AN_OBJ *obj;\
   typedef struct page_save_st{\
        char tagPag[DIMTAG+1];\
        char nomepag[MAXCHAR_PAGNAME];\
        } PAG_SAVE;\
   PAG_SAVE *pag_save;\
   char base_dir[255];\
\
\
   printf("\nexecute display_resource\n");\
\
   setContextTitle(paged);\
\
/* elimino la vecchia lista delle pagine e delle librerie */\
   XmListDeleteAllItems(ListPagine);\
   XmListDeleteAllItems(LibraryList);\
\
   if(paged->pag_num>0)\
       pag_save=(PAG_SAVE*)XtCalloc(paged->pag_num,sizeof(PAG_SAVE));\
/* recupero la lista delle pagine */\
   for(i=0;i<paged->pag_num;i++)\
   {\
     \
      /*  CAPPE  if( pagina_init(paged->page_list[i],&pag,TIPO_PAGINA) == True ) \
      {\
         pagina_load_file(pag,TIPO_PAGINA);     \
         GetResPaginaDb(pag,pag->db,True);\
	 printf("display_resources: prima di AddResPageContext!!\n");\
	 AddResPageContext(pag, paged, i);\
         SetItemString(&item,pag->nomepag,pag->geom.tipo,pag->geom.descriz,\
                pag->in_use,pag->tagPag);\
         add_item(item,0);\
         XmStringFree(item); \
         if(strcmp(pag->geom.tipo,TIPO_REGOLAZIONE)==0)\
                {\
                strcpy(pag_save[i].nomepag,pag->nomepag);\
                strcpy(pag_save[i].tagPag,pag->tagPag);\
                }\
         else\
                {\
                strcpy(pag_save[i].nomepag,pag->nomepag);\
                strcpy(pag_save[i].tagPag,"--");\
                }\
\
         pagina_free(pag);\
      }\
      else\
      {\
         printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",paged->page_list[i]);\
         continue;\
      }        CAPPE!!!!  */\
      \
      /* Nuova situazione! Recupero delle informazioni sulle pagine dal context CAPPE */\
      SetItemString(&item,paged->page_list[i],paged->res_page[i]->type_page,paged->res_page[i]->descr_page,\
                paged->res_page[i]->in_use,paged->res_page[i]->tagPag);\
      add_item(item,0);\
/* MAURIC */\
      XmStringFree(item);\
      if(strcmp(paged->res_page[i]->type_page,TIPO_REGOLAZIONE)==0)\
		{\
		strcpy(pag_save[i].nomepag,paged->page_list[i]);\
                strcpy(pag_save[i].tagPag,paged->res_page[i]->tagPag);\
		}\
      else\
		{\
		strcpy(pag_save[i].nomepag,paged->page_list[i]);\
                strcpy(pag_save[i].tagPag,"--");\
		}\
      printf("display_resources: Indice pagina=%d\n",i);\
   }\
   /* new_save_context(paged);  CAPPE!!! */\
\
    for(i=0;i<paged->pag_num;i++)\
        {\
	if(strcmp(pag_save[i].tagPag,"--")!=0)\
           for(j=i+1;j<paged->pag_num;j++)\
                {\
                if(strcmp(pag_save[j].tagPag,pag_save[i].tagPag)==0)\
                        {\
			char message[200];\
                        sprintf(message,"regolation pages %s and %s have the same tag [%s]\n",\
			pag_save[j].nomepag,pag_save[i].nomepag,pag_save[i].tagPag);\
			SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",message,\
				NULL,False,NULL,False,NULL);\
                        }\
                }\
        }\
\
   if(paged->pag_num>0)\
     XtFree(pag_save);\
    \
/* recupero la lista delle icone */\
   for(i=0;i<paged->iconlib_num;i++)\
   {\
      if(pagina_init(paged->iconlib_list[i],&pag,TIPO_LIBRARY) == True)      \
      {\
      pagina_load_file(pag,TIPO_LIBRARY);\
      GetResPaginaDb(pag,pag->db,True);\
      SetItemString(&item,pag->nomepag,pag->geom.tipo,pag->geom.descriz,\
                pag->in_use,pag->tagPag);\
      add_library_item(item,0);\
      pagina_free(pag);\
      XmStringFree(item);\
      }\
      else\
      {\
         printf("\n\n\tERRORE INIZIALIZZAZIONE LIBRERIA %s\n",paged->iconlib_list[i]);\
         continue;\
      }\
\
\
   }\
\
/* creo le compound string dei dati e setto\
   le label della interfaccia con i valori */\
   mstring = XmStringCreateSimple(paged->descrizione);\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textDescription1, args, lnargs );\
   XmStringFree(mstring);\
\
/*\
   mstring = XmStringCreateSimple(XlConvDirVms(paged->project));\
*/\
printf("DEBUG ContextPath = %s\n",Context_Path);\
\
   strcpy(base_dir,Context_Path);\
   getwd(base_dir);\
   mstring = XmStringCreateSimple(base_dir);\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textProject, args, lnargs );\
   XmStringFree(mstring);\
\
   mstring = XmStringCreateSimple(XlConvDirVms(paged->pages));\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textPages, args, lnargs );\
   XmStringFree(mstring);\
\
   mstring = XmStringCreateSimple(XlConvDirVms(paged->objectLibrary));\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textObjectLibraries, args, lnargs );\
   XmStringFree(mstring);\
\
\
   mstring = XmStringCreateSimple(XlConvDirVms(paged->animatedIconLibrary));\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textAnimatedIconLibraries, args, lnargs );\
   XmStringFree(mstring);\
\
   mstring = XmStringCreateSimple(XlConvDirVms(paged->simulator));\
   lnargs=0;\
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;\
   XtSetValues( textSimulator, args, lnargs );\
   XmStringFree(mstring);\
}\
\
\
\
/*------------------------------------------------\
 *\
 * gest_conf_op\
 *\
 *  gestione conferma in risposta ad operazioni\
 *  e messaggistica in generale\
 *\
 -------------------------------------------------*/\
\
int gest_conf_op( int tipo_op, char *msg, char *par )\
{\
   extern swidget create_messageBoxDialog1();\
   swidget wid;\
\
   wid = create_messageBoxDialog1(tipo_op,msg,par);\
   UxPopupInterface(wid,no_grab);\
}\
\
Boolean LibraryIsOpen(char *nome_libreria)\
{\
   return(False);\
}\
\
\
\
/*------------------------------------------------------------\
 * open_page\
 *\
 * apertura di una pagina selezionata dalla lista\
 *\
 *------------------------------------------------------------*/\
\
int open_page(int tipo)\
{\
   extern XrmDatabase defdb;\
   extern swidget create_PagShell();\
   swidget wid;\
   int *selected,selcount,ind_pagina,i;\
   extern Boolean PagIsOpen();\
\
/* permetto di aprire contemporaneamente al massimo MAXPAGOPEN */\
   if(npagopen == MAXPAGOPEN)\
      return;\
\
/* da utilizzare se si desidera aprire una sola pagina\
\
   if(Already_Selected == True)\
      return;\
**********************************/\
\
/* identifico l'item selezionato */\
   if(tipo == TIPO_PAGINA)\
   {\
      if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) == False )\
         return;\
   }\
   else if(tipo == TIPO_LIBRARY)\
   {\
      if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) == False )\
         return;\
   }\
\
   if(selcount > 1)\
      return;\
   else if(selcount ==0)\
      return;\
\
   ind_pagina = selected[0] - 1;\
\
/* non apro due volte la stessa pagina */\
 \
   if( (tipo==TIPO_PAGINA) && (PagIsOpen(pagedit->page_list[ind_pagina]) == True))\
   {\
      RaisePagina(pagedit->page_list[ind_pagina]);\
/****\
      SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);\
****/\
      return;\
   }\
   else if( (tipo==TIPO_LIBRARY) && (PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True))\
   {\
      RaisePagina(pagedit->iconlib_list[ind_pagina]);\
/****\
      SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);\
*****/\
      return;\
   }\
\
\
/* con questi dati creo la pagina e ne faccio il popup */\
\
   if( tipo == TIPO_PAGINA )\
      wid = create_PagShell(pagedit->page_list[ind_pagina],ind_pagina,tipo);\
   else\
      wid = create_PagShell(pagedit->iconlib_list[ind_pagina],ind_pagina,tipo);\
\
   UxPopupInterface(wid,no_grab);\
\
/* aggiorno la lista delle pagine aperte */\
   PagOpen[npagopen] = wid;\
   PagOpenId[npagopen] = ind_pagina;\
   npagopen++;\
\
/**\
   Already_Selected=True;\
**/\
\
   disabilitaMoveMenu();\
   UxDisplay->db = defdb;\
}\
\
/*------------------------------------------------\
 * modify_page\
 *\
 * modifica dati di tipo geometry  della pagina\
 * selezionata\
 *-----------------------------------------------*/\
\
int modify_page(int tipo)\
{\
   extern swidget create_PagShell();\
   extern Boolean PagIsOpen();\
   swidget wid;\
   int *selected,selcount,ind_pagina;\
\
   if(Already_Selected == True)\
      return;\
\
/* identifico l'item selezionato */\
   if(tipo == TIPO_PAGINA )\
      if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )\
         return;\
\
   if(tipo == TIPO_LIBRARY )\
      if( XmListGetSelectedPos(LibraryList,&selected,&selcount) == False )\
         return;\
\
   if(selcount > 1)\
      return;\
   else if(selcount ==0)\
      return;\
\
\
   ind_pagina = selected[0] - 1;\
\
/* con questi dati creo la pagina e ne faccio il popup */\
   \
   if( tipo == TIPO_PAGINA )\
      if( PagIsOpen(pagedit->page_list[ind_pagina]) == True) \
      {\
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);\
         return;\
      }\
      else\
        wid=create_PagDialog(pagedit->page_list[ind_pagina],MODIFY_PAGE);\
\
   if( tipo == TIPO_LIBRARY)\
      if( PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True) \
      {\
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Library Is Already Open",NULL,False,NULL,False,NULL);\
         return;\
      } \
      else\
         wid=create_PagDialog(pagedit->iconlib_list[ind_pagina],MODIFY_LIBRARY);\
\
   UxPopupInterface(wid,XtGrabNone);\
}\
\
\
/*------------------------------------------------\
 * duplicate_page\
 *\
 * duplicazione dei dati della pagina selezionata\
 *--------------------------------------------------*/\
int duplicate_page()\
{\
\
   extern swidget create_PagDialog();\
   swidget wid;\
   int *selected,selcount,ind_pagina;\
\
   if(Already_Selected == True)\
      return;\
\
/* identifico l'item selezionato */\
   if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )\
      return;\
\
   if(selcount > 1)\
      return;\
   else if(selcount ==0)\
      return;\
\
\
   ind_pagina = selected[0] - 1;\
\
/* con questi dati creo la pagina e ne faccio il popup */\
   wid = create_PagDialog(pagedit->page_list[ind_pagina],COPY_PAGE);\
   UxPopupInterface(wid,XtGrabNone);\
\
}\
/*---------------------------------------------\
 * delete_page\
 *\
 * cancellazione della pagina selezionata\
 *\
 *----------------------------------------------*/\
int del_page(int tipo)\
{\
   extern swidget create_PagShell();\
   int *selected,selcount,ind_pagina;\
\
   if(Already_Selected == True)\
      return;\
\
/* identifico l'item selezionato */\
   if(tipo == TIPO_PAGINA)\
   {\
      if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )\
      {\
         printf("return value XmListGetSelectedPos is False\n");\
         return;\
      }\
   }\
   else if(tipo == TIPO_LIBRARY)\
   {\
      if( XmListGetSelectedPos(LibraryList,&selected,&selcount) == False )\
      {\
         printf("return value XmListGetSelectedPos is False\n");\
         return;\
      }\
   }\
   \
\
  \
   if(selcount > 1)\
      return;\
   else if(selcount ==0)\
      return;\
\
\
   ind_pagina = selected[0] - 1;\
\
   if(tipo == TIPO_PAGINA)\
   {\
      if(PagIsOpen(pagedit->page_list[ind_pagina]) == True)\
      {\
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Open" ,NULL,False,NULL,False,NULL);\
         return;\
      }\
   }\
   else if(tipo == TIPO_LIBRARY)\
   {\
      if(PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True)\
      {\
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Library Is Open" ,NULL,False,NULL,False,NULL);\
         return;\
      }\
   }\
\
/* gestisco una interfaccia di conferma cancellazione */\
   if(tipo == TIPO_PAGINA)\
      gest_conf_op(DELETE_PAGE,"Do you want to delete %s page?",pagedit->page_list[ind_pagina]);\
   else\
      gest_conf_op(DELETE_LIBRARY,"Do you want to delete %s library?",pagedit->iconlib_list[ind_pagina]);\
/**\
   Already_Selected=False;\
**/\
}\
\
/*----------------------------------------------------------\
 * Visualizzazione messaggi nella paned window dei messaggi\
 *---------------------------------------------------------*/\
\
int show_message(char *mess)\
{\
  extern FILE *fpLogMsg;\
  char   *str, *new_str;\
\
  if(fpLogMsg!=NULL)\
        fprintf(fpLogMsg,"%s",mess);\
\
  str = XmTextGetString( scrolledText1 );\
  RtRecord(str);\
  new_str = (char*) alloca_memoria ( ( strlen(mess)+strlen(str)+1 ),sizeof(char *) );\
  strcpy( new_str, str );\
\
  strcat( new_str, mess );\
  XmTextSetString( scrolledText1, new_str );\
  XmTextShowPosition( scrolledText1,strlen(new_str)-1);\
  libera_memoria( str );\
  libera_memoria( new_str );\
  \
  XmUpdateDisplay(scrolledText1);\
\
}\
\
int LibraryDeselectAllItem()\
{\
   XmListDeselectAllItems(LibraryList);\
} \
\
int PageDeselectAllItem()\
{\
   XmListDeselectAllItems(ListPagine);\
}\
\
int clear_topLevel()\
{\
   PAGEDIT_CONTEXT *appo;\
\
   pagedit_context_init("None",&appo);\
   strcpy(appo->fileContext,"BAD CONTEXT"); \
   display_resources(appo);   \
   TopMenuSetInsensitive();\
}\
\
\
/*\
	Funzione che esegue l'allineamento del context\
*/\
Boolean AlignResPageContext()\
{\
\
PAGINA *pag;\
XmString	item;\
int		i;\
char		*tipo,risorsa[100],val_ris_ger[50],filepag[MAXLENFILENAME];\
XrmValue	value;\
\
	XmListDeleteAllItems(ListPagine);\
	for(i=0;i<pagedit->pag_num;i++)\
	{\
		if( pagina_init(pagedit->page_list[i],&pag,TIPO_PAGINA) == True )\
		{\
		   printf("AlignResPageContext: prima di pagina pagina_load_file\n");\
		   pagina_load_file(pag,TIPO_PAGINA);\
         	   GetResPaginaDb(pag,pag->db,True);\
		   /* Gestione dell'eventualita' che nel file .pag la risorsa 'gerarchia'\
		      sia ancora rappresentata con il vecchio formato [][][][][][].	*/\
\
		   /*   Costruzione del path del file .pag corrente.	          	*/\
		   strcpy(filepag,getenv(ENVPAG));\
                   strcat(filepag,"/");\
                   strcat(filepag,pagedit->page_list[i]);\
                   strcat(filepag,PAG_EXT);\
\
		   sprintf(risorsa,"%s.gerarchia",pagedit->page_list[i]);\
		   if(XrmGetResource(pag->db, risorsa, (char *)NULL, &tipo, &value)!=NULL) {\
		      /* La risorsa 'gerarchia' e' presente nel file.pag corrente */  \
		      strncpy(val_ris_ger,value.addr,(int)value.size);\
		      if(!strcmp(val_ris_ger,"[][][][][][]")) {\
			 /* La risorsa gerarchia e' stata salvata con il vecchio formato.\
			    Sostituzione.						  */ \
		         strcpy(val_ris_ger,"-1,-1,-1,-1,-1,-1");\
		         XrmPutStringResource(&pag->db,"*gerarchia",val_ris_ger);\
		         XrmPutFileDatabase(pag->db,filepag);\
		      }\
		   }\
		   else {\
		      /* La risorsa 'gerarchia' non e' presente nel file .pag corrente.\
			 Inserimento con il valore -1,-1,-1,-1,-1,-1			*/\
		      strcpy(val_ris_ger,"-1,-1,-1,-1,-1,-1");\
		      XrmPutStringResource(&pag->db,"*gerarchia",val_ris_ger);\
		      XrmPutFileDatabase(pag->db,filepag);\
		   }\
		   strcpy(pagedit->res_page[i]->hier, val_ris_ger);\
         	   strcpy(pagedit->res_page[i]->descr_page, pag->geom.descriz);\
		   strcpy(pagedit->res_page[i]->type_page, pag->geom.tipo);\
		   pagedit->res_page[i]->refresh_freq = pag->refreshFreq;\
		   pagedit->res_page[i]->in_use = pag->in_use;\
		   strcpy(pagedit->res_page[i]->tagPag,pag->tagPag);\
		   SetItemString(&item,pagedit->page_list[i],pagedit->res_page[i]->type_page,\
				 pagedit->res_page[i]->descr_page,pagedit->res_page[i]->in_use,\
				 pagedit->res_page[i]->tagPag);\
		   add_item(item,0);\
/* MAURIC */\
                   XmStringFree(item);\
		   pagina_free(pag);\
		}\
		else\
		   printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",pagedit->page_list[i]);\
	}\
	printf("AlignResPageContext: prima di new_save_context\n");\
	if(new_save_context( pagedit )!=True)\
		SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Save of Context failed.",\
		       NULL,False,NULL,False,NULL);\
\
}\
\
\
\
/*\
	Funzione per la gestione della voce di menu' "Print all pages"\
*/\
\
void PrintAllPages()\
{\
\
extern int PrintClosePag();\
int i;\
char str[200];\
swidget wid;\
\
 if(pagedit->pag_num != 0) \
 {\
    if(npagopen == 0) \
    {\
       def_cursore(topLevelShell, OROLOGIO);\
       for(i=0; i<pagedit->pag_num; i++) \
       {\
\
           wid = create_PagShell(pagedit->page_list[i], i, TIPO_PAGINA);\
           PagOpen[npagopen] = wid;\
           PagOpenId[npagopen] = i;\
           npagopen++;\
          PrintClosePag(pagedit->page_list[i]);\
       }\
       undef_cursore(topLevelShell);\
       sprintf(str,"Printing Pages %s",topLevelShell);\
       SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",str,NULL,\
		False,NULL,False,NULL);	\
   }\
   else\
   {\
        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","All pages must be closed",NULL,\
           False,NULL,False,NULL);\
   }\
}\
else \
{\
    SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","No pages in ListPages",NULL,\
       False,NULL,False,NULL);\
}\
\
}\
\
\
\
int CheckOlTree(OlDatabaseTopologiaObject dbtopologia)\
{\
\
   FILE *fpLogOlTree;\
   int i,j,hier_pb,problems=0,alarm_state=0,fnomi_err=0;\
   int pos_info,cont_root=0,curr_lev;\
   int num_pages;\
/*\
   OlTreeObject root;\
*/\
   char msg[200],path_log[200],fnomirtf[200],root_hierarchy[50],root_name[MAXCHAR_PAGNAME];\
\
   /* Apertura del file di log 'OlTree.log' */\
   strcpy(path_log,XlGetenv(ENVCTX));\
   strcat(path_log,"/OlTree.log");\
   if((fpLogOlTree=fopen(path_log,"w"))!=NULL) {\
      /* Controllo dell'esistenza del file fnomi.rtf */\
      if(XlGetenv(ENVRTF)==NULL) {\
         printf("Imposssibile trovare file fnomi.rtf !!!\n");\
         show_message("Impossible to find fnomi.rtf!! Only hierarchy will be checked.\n");\
         fnomi_err=-1;\
      }\
      else {\
         strcpy(fnomirtf,XlGetenv(ENVRTF));\
         strcat(fnomirtf,"/fnomi.rtf");\
         if(access(fnomirtf,F_OK)!=0) {\
            /* fnomi.rtf non trovato */\
            show_message("fnomi.rtf not found!! Only hierarchy will be checked.\n");\
            fnomi_err=-1;\
         }\
         else {\
            fnomi_err=0;\
         }\
      }      \
      /* Pagine presenti attualmente nel context */\
      num_pages=pagedit->pag_num;\
      /* Allocazione della radice dell'albero (OlTree) */\
      for(j=0; j<num_pages; j++) {\
         /* Controllo che la pagina corrente sia di tipo SINOTTICO o STAZIONE */\
         if((strcmp(pagedit->res_page[j]->type_page,TIPO_SINOTTICO)) ||\
           (strcmp(pagedit->res_page[j]->type_page,TIPO_STAZIONI))) {\
            curr_lev=OlTreeGetLevel(pagedit->res_page[j]->hier);\
            if(curr_lev==0) {\
               /* Trovata una pagina con le caratteristiche della radice */\
               if(cont_root!=0) {\
               /* Esistono piu' radici. Errore!! */\
		  show_message("HIERARCHY PROBLEM!! More pages have root requirements!\n");\
		  sprintf(msg,"Page: %s\tHierarchy: %s\n",root_name,root_hierarchy);\
		  show_message(msg);\
		  sprintf(msg,"Page: %s\tHierarchy: %s\n",\
			  pagedit->page_list[j],pagedit->res_page[j]->hier);\
		  show_message(msg);\
                  fclose(fpLogOlTree);\
                  return(-2);\
               }\
               else {\
		  strcpy(root_hierarchy,pagedit->res_page[j]->hier);\
		  strcpy(root_name,pagedit->page_list[j]);\
                  pos_info=j;\
                  cont_root++;\
               }\
            }\
         }\
      }\
      if(cont_root==0) {\
         /* Non esistono pagine con le caratteristiche della root */\
	 show_message("HIERARCHY PROBLEM!! A page with root requirements is not present!\n");\
         fclose(fpLogOlTree);\
         return(-2);\
      }\
      if((root_oltree=OlCreateTree(dbtopologia,pos_info,pagedit->res_page[pos_info]->hier,&alarm_state))==NULL) {\
         /* Errore nell'allocazione della radice di OlTree */\
         show_message("HIERARCHY PROBLEM!! Error in root allocation\n");\
         fclose(fpLogOlTree);\
         return(-2);\
      }\
      else {\
         printf("RADICE ALLOCATA!! %s\n",pagedit->res_page[pos_info]->hier); \
         if((alarm_state==-1) && (fnomi_err==0)) {\
            /* Radice caricata correttamente ma problemi sugli allarmi */\
            problems=-1;\
            sprintf(msg,"ALARM PROBLEM!! Page %s. Hierarchy value=%s\nWARNING Alarm description variable not found in fnomi.rtf\n\n",pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier );\
         }\
         else {\
            if(fnomi_err==0)\
               sprintf(msg,"HIERARCHY/ALARM OK!! Page root=%s. Hierarchy value=%s\n\n",\
                       pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier);\
            else\
               sprintf(msg,"HIERARCHY OK/ALARM NOT CHECKED!! Page root=%s. Hierarchy value=%s\n\n",\
                       pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier);\
         }\
         printf("%s",msg);\
         fprintf(fpLogOlTree,"%s",msg);\
      }\
\
      /* Caricamento dell'albero (OlTree) */\
      for(i=1; i<NUM_MAX_LIV; i++) {\
         printf("Build_OlTree: Caricamento del livello=%d\n", i);\
         for(j=0; j<num_pages; j++) {\
            /* Controllo che la pagina corrente sia di tipo SINOTTICO o STAZIONE */\
            if((strcmp(pagedit->res_page[j]->type_page,TIPO_SINOTTICO)) ||\
               (strcmp(pagedit->res_page[j]->type_page,TIPO_STAZIONI))) {\
               curr_lev=OlTreeGetLevel(pagedit->res_page[j]->hier);\
               if(curr_lev==i) {\
                  /* Trovata una pagina di livello i. Inserimento. */\
                  if((hier_pb=OlTreeAddPage(dbtopologia,root_oltree,curr_lev,j,pagedit->res_page[j]->hier))<0) {\
                     /* Riscontrati problemi sull'oggetto OlTree */\
                     switch (hier_pb) {\
                        case -1:\
                           problems=-1;\
                           sprintf(msg,"HIERARCHY PROBLEM!! Page %s without father.\tHierarchy value=%s\n\n",\
                                   pagedit->page_list[j],pagedit->res_page[j]->hier);\
                           break;\
                        case -2:\
                           problems=-1;\
                           sprintf(msg,"HIERARCHY PROBLEM!! Page %s. A page with hierarchy value %s already present\n\n",\
                                   pagedit->page_list[j],pagedit->res_page[j]->hier );\
                           break;\
                        case -3:\
                           /* Pagina inserita in gerarchia ma errori negli allarmi */\
                           if(fnomi_err==0) {\
                              problems=-1;\
                              sprintf(msg,"ALARM PROBLEM!! Page %s. Hierarchy value=%s\n\tAlarm description variable not found in fnomi.rtf\n\n",\
                                      pagedit->page_list[j],pagedit->res_page[j]->hier );\
                           }\
                           else {\
                              sprintf(msg,"HIERARCHY OK/ALARM NOT CHECKED!!Page %s. Hierarchy value=%s\n\n",\
                                      pagedit->page_list[j],pagedit->res_page[j]->hier );\
                           }\
                           break;\
                     }\
                     fprintf(fpLogOlTree,"%s",msg);\
                  }\
                  else { \
                     /* Pagina corrente inserita in gerarchia e allarmi corretti */\
                     sprintf(msg,"HIERARCHY/ALARM OK!! Page %s. Hierarchy value=%s\n\n",\
                             pagedit->page_list[j],pagedit->res_page[j]->hier );\
                     fprintf(fpLogOlTree,"%s",msg);\
                  }\
\
               }\
            }\
         }\
      }\
      fclose(fpLogOlTree);\
   }\
   else {\
      /* Errore nell'apertura del file OlTree.log */\
      show_message("Error opening file OlTree.log!!\n");\
      return(-2);\
   }\
   if (problems==-1)\
      return(-1);\
   else\
      return(0);\
\
}				 
*topLevelShell.name.source: public
*topLevelShell.static: false
*topLevelShell.name: topLevelShell
*topLevelShell.parent: NO_PARENT
*topLevelShell.x: 285
*topLevelShell.y: 180
*topLevelShell.width: 675
*topLevelShell.height: 789
*topLevelShell.iconName: "LEGOMMI - Config"

*mainWindow.class: mainWindow
*mainWindow.static: true
*mainWindow.name: mainWindow
*mainWindow.parent: topLevelShell
*mainWindow.unitType: "pixels"
*mainWindow.x: 5
*mainWindow.y: 10
*mainWindow.width: 650
*mainWindow.height: 630

*menu2.class: rowColumn
*menu2.static: true
*menu2.name: menu2
*menu2.parent: mainWindow
*menu2.rowColumnType: "menu_bar"
*menu2.menuAccelerator: "<KeyUp>F10"

*menu1File1.class: rowColumn
*menu1File1.static: true
*menu1File1.name: menu1File1
*menu1File1.parent: menu2
*menu1File1.rowColumnType: "menu_pulldown"

*menu1Define.class: pushButton
*menu1Define.static: true
*menu1Define.name: menu1Define
*menu1Define.parent: menu1File1
*menu1Define.labelString: "Define Context"
*menu1Define.activateCallback: {\
   extern swidget creat_DefineContext();\
  /*\
	swidget swid;\
        swid = creat_ContextSelection(DEF_CONTEXT);\
  */\
   UxPopupInterface(creat_DefineContext(pagedit->fileContext,MOD_CONTEXT),no_grab);\
\
}

*menu1Align.class: pushButton
*menu1Align.static: true
*menu1Align.name: menu1Align
*menu1Align.parent: menu1File1
*menu1Align.labelString: "Align Context"
*menu1Align.activateCallback: {\
extern swidget create_messageBoxDialog1();\
swidget wid;\
if(npagopen == 0) {\
   wid = create_messageBoxDialog1(ALIGN_CONTEXT,"Do you want to align context?",NULL);\
   UxPopupInterface(wid,no_grab);\
}\
\
}

*separatore3.class: separator
*separatore3.static: true
*separatore3.name: separatore3
*separatore3.parent: menu1File1

*menu1Check.class: pushButton
*menu1Check.static: true
*menu1Check.name: menu1Check
*menu1Check.parent: menu1File1
*menu1Check.labelString: "Check Hierarchy"
*menu1Check.activateCallback: {\
\
   int check_result;\
   check_result=CheckOlTree(dbtopologia);\
   switch (check_result) {\
      case 0:\
         OlDestroyTree(root_oltree);  \
         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking OK!",NULL,False,NULL,False,NULL);\
         break;\
      case -1:\
         OlDestroyTree(root_oltree);\
         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking failed! See logfile OlTree.log",NULL,False,NULL,False,NULL);\
         break;\
      case -2:\
         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking failed! See window messages!!",NULL,False,NULL,False,NULL);\
         break;\
   } \
\
}

*separatore4.class: separatorGadget
*separatore4.static: true
*separatore4.name: separatore4
*separatore4.parent: menu1File1

*print_entry.class: cascadeButton
*print_entry.static: true
*print_entry.name: print_entry
*print_entry.parent: menu1File1
*print_entry.labelString: "Print ..."
*print_entry.subMenuId: "print_menu"

*menu1PrintSet.class: pushButton
*menu1PrintSet.static: true
*menu1PrintSet.name: menu1PrintSet
*menu1PrintSet.parent: menu1File1
*menu1PrintSet.labelString: "Printer Setup..."
*menu1PrintSet.activateCallback: {\
#ifdef XPRINTER_USED\
\
   extern Widget PrintSetup();\
\
   PrintSetup(topLevelShell);\
#else\
   SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Printer use not allowed for this version",NULL,False,NULL,False,NULL);    \
#endif\
}

*separatore6.class: separator
*separatore6.static: true
*separatore6.name: separatore6
*separatore6.parent: menu1File1

*menu1Exit1.class: pushButton
*menu1Exit1.static: true
*menu1Exit1.name: menu1Exit1
*menu1Exit1.parent: menu1File1
*menu1Exit1.labelString: "Exit"
*menu1Exit1.activateCallback: {\
   extern Boolean ContextModified;\
\
   if( ContextModified )\
      gest_conf_op(EXIT_WITH_SAVE,"EXIT - Context is not Saved. Do you Want To Save ?",pagedit);\
   else\
      gest_conf_op(EXIT_NO_CHANGE,"EXIT - Do you really want to Exit?",NULL);\
\
}

*print_menu.class: rowColumn
*print_menu.static: true
*print_menu.name: print_menu
*print_menu.parent: menu1File1
*print_menu.rowColumnType: "menu_pulldown"

*all_but.class: pushButton
*all_but.static: true
*all_but.name: all_but
*all_but.parent: print_menu
*all_but.labelString: "All Pages"
*all_but.activateCallback: {\
#ifdef XPRINTER_USED\
extern swidget create_messageBoxDialog1();\
swidget wid;\
wid = create_messageBoxDialog1(PRINT_ALL,"Print all pages. Are you sure?",NULL);\
UxPopupInterface(wid,no_grab);\
#else\
   \
SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Print not allowed for current version",NULL,\
       False,NULL,False,NULL);\
\
#endif\
\
}\


*select_but.class: pushButton
*select_but.static: true
*select_but.name: select_but
*select_but.parent: print_menu
*select_but.labelString: "Selected Pages"
*select_but.activateCallback: {\
#ifdef XPRINTER_USED\
extern int PrintClosePag();\
int *selected, selcount, i;\
char str[200];\
swidget wid;\
\
if(XmListGetSelectedPos(UxGetWidget(ListPagine), &selected, &selcount) != FALSE) \
{\
    /* Almeno una pagina e' stata selezionata */\
    if(npagopen == 0) \
    {\
       def_cursore(topLevelShell, OROLOGIO); \
       for(i=0; i<selcount; i++) \
       {\
\
          wid = create_PagShell(pagedit->page_list[selected[i]-1], (selected[i]-1),\
                TIPO_PAGINA);\
          PagOpen[npagopen] = wid;\
          PagOpenId[npagopen] = (selected[i]-1);\
          npagopen++;\
          PrintClosePag(pagedit->page_list[selected[i]-1]);\
       }\
\
       undef_cursore(topLevelShell);\
       sprintf(str,"Printing Pages %s",topLevelShell);\
       SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",str,NULL,\
		False,NULL,False,NULL);\
   }\
   else \
   {\
       SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","All pages must be closed",NULL,\
        False,NULL,False,NULL);\
   }\
}\
else\
{\
    SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","No pages selected",NULL,\
        False,NULL,False,NULL);\
}\
#else\
\
SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","Print not allowed for current version",\
       NULL,False,NULL,False,NULL);\
#endif\
\
}

*menu1Create1.class: rowColumn
*menu1Create1.static: true
*menu1Create1.name: menu1Create1
*menu1Create1.parent: menu2
*menu1Create1.rowColumnType: "menu_pulldown"

*Page.class: pushButton
*Page.static: true
*Page.name: Page
*Page.parent: menu1Create1
*Page.labelString: "Page"
*Page.activateCallback: {\
   extern swidget create_PagDialog();\
   Widget wid;\
   printf("\nCreate Page function called\n");\
\
   wid = (Widget )create_PagDialog((char *)NULL,CREATE_PAGE);\
\
   UxPopupInterface(wid,no_grab);\
   \
}

*Regolation.class: pushButton
*Regolation.static: true
*Regolation.name: Regolation
*Regolation.parent: menu1Create1
*Regolation.labelString: "Regolation"
*Regolation.activateCallback: {\
   extern swidget create_PagDialog();\
   Widget wid;\
   printf("\nCreate regolation function called\n");\
\
   wid = (Widget )create_PagDialog((char *)NULL,CREATE_REGOLATION);\
\
   UxPopupInterface(wid,no_grab);\
   \
}

*Library.class: pushButton
*Library.static: true
*Library.name: Library
*Library.parent: menu1Create1
*Library.labelString: "Library"
*Library.activateCallback: {\
   extern swidget create_PagDialog();\
   Widget wid;\
   printf("\nCreate library function called\n");\
\
   wid = (Widget )create_PagDialog((char *)NULL,CREATE_LIBRARY);\
\
   UxPopupInterface(wid,no_grab);\
   \
}

*StandardLibrary.class: pushButton
*StandardLibrary.static: true
*StandardLibrary.name: StandardLibrary
*StandardLibrary.parent: menu1Create1
*StandardLibrary.labelString: "Standard Library"
*StandardLibrary.activateCallback: {\
\
   extern void library_gen();\
   \
\
\
   library_gen(STD_LIB_NAME,STD_LIB_NAME);\
  \
}\


*menu1Edit1.class: rowColumn
*menu1Edit1.static: true
*menu1Edit1.name: menu1Edit1
*menu1Edit1.parent: menu2
*menu1Edit1.rowColumnType: "menu_pulldown"

*menu1Open.class: pushButton
*menu1Open.static: true
*menu1Open.name: menu1Open
*menu1Open.parent: menu1Edit1
*menu1Open.labelString: "Open"
*menu1Open.activateCallback: {\
  int *selected,selcount;\
\
  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   \
     open_page(TIPO_PAGINA);\
  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )\
     open_page(TIPO_LIBRARY);   \
\
\
}  

*menu1Modify.class: pushButton
*menu1Modify.static: true
*menu1Modify.name: menu1Modify
*menu1Modify.parent: menu1Edit1
*menu1Modify.labelString: "Modify"
*menu1Modify.activateCallback: {\
  int *selected,selcount;\
\
  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   \
     modify_page(TIPO_PAGINA);\
  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )\
     modify_page(TIPO_LIBRARY);   \
\
\
}

*menu1Delete1.class: pushButton
*menu1Delete1.static: true
*menu1Delete1.name: menu1Delete1
*menu1Delete1.parent: menu1Edit1
*menu1Delete1.labelString: "Delete"
*menu1Delete1.activateCallback: {\
  int *selected,selcount;\
\
  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   \
     {\
     del_page(TIPO_PAGINA);\
     }\
  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )\
     del_page(TIPO_LIBRARY);   \
\
\
}

*menu1Import.class: rowColumn
*menu1Import.static: true
*menu1Import.name: menu1Import
*menu1Import.parent: menu2
*menu1Import.rowColumnType: "menu_pulldown"

*ImportPage.class: pushButton
*ImportPage.static: true
*ImportPage.name: ImportPage
*ImportPage.parent: menu1Import
*ImportPage.labelString: "Page"
*ImportPage.activateCallback: {\
   extern swidget  create_PageSelection();\
   swidget swid;\
\
   swid = create_PageSelection(IMPORT_PAGE);\
\
   UxPopupInterface(swid,no_grab);\
}

*ImportLibrary.class: pushButton
*ImportLibrary.static: true
*ImportLibrary.name: ImportLibrary
*ImportLibrary.parent: menu1Import
*ImportLibrary.labelString: "Library"
*ImportLibrary.activateCallback: {\
   extern swidget  create_PageSelection();\
   swidget swid;\
\
   swid = create_PageSelection(IMPORT_LIB);\
\
   UxPopupInterface(swid,no_grab);\
}

*ImportIcon.class: pushButton
*ImportIcon.static: true
*ImportIcon.name: ImportIcon
*ImportIcon.parent: menu1Import
*ImportIcon.labelString: "Icon File"
*ImportIcon.activateCallback: {\
 \
}

*menu1Compile.class: rowColumn
*menu1Compile.static: true
*menu1Compile.name: menu1Compile
*menu1Compile.parent: menu2
*menu1Compile.rowColumnType: "menu_pulldown"

*CompileAll.class: pushButton
*CompileAll.static: true
*CompileAll.name: CompileAll
*CompileAll.parent: menu1Compile
*CompileAll.labelString: "All Page"
*CompileAll.activateCallback: {\
       compile_all_pag();\
#if defined MAURIZIO\
           /*compilazione di tutte le pagine nel context */\
           extern Cardinal lnargs;\
           extern Arg args[];\
           extern OlCompilerObject compilatore_severe;\
           extern XrmDatabase defdb;\
           int err_level = 0;\
           WidgetList children;\
           Cardinal num_children;\
           char outname[105],mesg[200];\
           int i,j;\
           PAGINA *pag=NULL;\
           AN_OBJ *lista_oggetti=NULL;\
           WidgetList lista;\
           int g,m,a,o,min,s;\
\
	   XEvent event;\
	   XtAppContext AppContMom;\
	   Widget TopLevelMom;\
	   Display *DisplayMom;\
	   Display *AppDisplay;\
	   Widget AppTopLevel;\
	   int numero=0;\
\
           if( getenv(ENVPAG) == NULL)\
              return;\
\
	   AppTopLevel=UxTopLevel;\
	   AppDisplay=UxDisplay;\
\
	   if(CompileAppContext == NULL)\
		{\
	   	AppContMom= XtCreateApplicationContext();\
	   	DisplayMom=XtOpenDisplay(AppContMom,NULL,"configMom","ConfigMom",NULL,0,&numero,NULL);\
	   	UxTopLevel=XtAppCreateShell("configMom",\
                		"ConfigMom",applicationShellWidgetClass, \
				DisplayMom, NULL, 0);\
	        UxDisplay=DisplayMom;\
		CompileAppContext = AppContMom;\
		CompileDisplay = DisplayMom;\
		CompileTopLevel = UxTopLevel;\
		}\
	   else\
		{\
		AppContMom=CompileAppContext;\
		DisplayMom = CompileDisplay;\
		UxTopLevel = CompileTopLevel;\
		UxDisplay=DisplayMom;\
		}\
\
           for(i=0;i<pagedit->pag_num;i++)\
           {\
	    XdLista lista_save;\
            Widget ClipSave;\
#ifndef DESIGN_TIME\
	    extern Widget ClipBoard;\
#endif\
	    lista_save = clip_get_lista();\
            ClipSave = ClipBoard;\
\
               CompileBoard = create_ClipBoard();\
\
               if( pagina_init( pagedit->page_list[i], &pag,TIPO_PAGINA) == False)\
                  return;\
		\
	       printf("Compilazione pagina [%s]\n",pag->nomepag);\
\
               if( pagina_load_file(pag,TIPO_PAGINA)  && pagina_getres(pag,&lista_oggetti) )\
               {\
\
                  if(pag->num_widget > 0)\
                  {\
                     if( (lista = alloca_memoria(pag->num_widget,sizeof(Widget))) == NULL )\
                        return;\
\
                     UxDisplay->db = pag->db;\
\
                     for(j=0;j<pag->num_widget;j++)\
                        lista[j] = XtCreateWidget(lista_oggetti[j].nome,lista_oggetti[j].classe,CompileBoard,NULL,0);\
		     pag->drawing=CompileBoard;\
\
                     OlSetCompiler(lista,pag->num_widget,compilatore_severe);\
\
                     strcpy(outname,getenv(ENVPAG));\
                     strcat(outname,"/");\
                     strcat(outname,pag->nomepag);\
                     strcat(outname,".rtf");\
		     /*\
			richiamo la routine PostOpenPage2 per settare le\
			risorse, prelevate da Connessioni.reg, nelle \
			eventuali icone di interffaccia\
		     */\
		     PostOpenPage2(CompileBoard,pag->nomepag,pag->db);\
\
                     printf("Chiamo il compilatore e metto il risultato in %s\n",outname);\
\
                     if( (err_level = XlCompilaWidget(pag->db,outname,lista,pag->num_widget) ) == False)\
                     {\
                        sprintf(mesg,"Error at Compile Time on Page %s\n",pag->nomepag);\
                        printf("error at Compile time mesg = %s\n",mesg);\
                        show_message(mesg);\
                     }\
                     else\
                     {\
                        /*       \
	 			setto la data della compilazione\
   			*/\
                        data(&g,&m,&a);\
   			ora(&o,&min,&s);\
   			sprintf(pag->compilationDate,\
				"%d/%d/%d %d:%d:%d",g,m,a,o,min,s);	\
			/*\
        			forzo il salvataggio della pagina\
   			*/\
#ifndef DESIGN_TIME\
   			pagina_save(pag,TIPO_PAGINA,0);\
#endif\
			sprintf(mesg,"Page Compiled  %s,  Ok\n",pag->nomepag);\
                        printf("Page Compiled  %s\n",mesg);\
                        show_message(mesg);\
                     }\
\
                     for(j=0;j<pag->num_widget;j++)\
                        XtDestroyWidget(lista[j]);\
\
                     libera_memoria(lista);\
                 }\
		 pagina_free(pag);\
	         libera_memoria(lista_oggetti);\
              }\
	      else\
              	 libera_memoria(pag);\
\
                UxDestroyInterface(CompileBoard);\
	   	XSync(XtDisplay(UxTopLevel),False);\
      		while(XtAppPending(AppContMom))\
                {\
                XtAppNextEvent(AppContMom,&event);\
                XtDispatchEvent(&event);\
                }\
clip_set_lista(lista_save);\
           ClipBoard =ClipSave;\
             UxDisplay->db = NULL;\
           }\
\
	   UxTopLevel = AppTopLevel;\
	   UxDisplay = AppDisplay;\
	   UxDisplay->db = defdb;\
           \
	\
           show_message("\nCompilation End\n");\
#endif        \
\
}

*CompileAllRegola.class: pushButton
*CompileAllRegola.static: true
*CompileAllRegola.name: CompileAllRegola
*CompileAllRegola.parent: menu1Compile
*CompileAllRegola.labelString: "All Regolation Schema"
*CompileAllRegola.activateCallback: {\
compile_all_reg();\
}

*menu1Compile_b4.class: separator
*menu1Compile_b4.static: true
*menu1Compile_b4.name: menu1Compile_b4
*menu1Compile_b4.parent: menu1Compile

*CompileTask.class: pushButton
*CompileTask.static: true
*CompileTask.name: CompileTask
*CompileTask.parent: menu1Compile
*CompileTask.labelString: "Regolation Task"
*CompileTask.activateCallback: {\
info_top_schemi();\
}

*ViewErrTask.class: pushButton
*ViewErrTask.static: true
*ViewErrTask.name: ViewErrTask
*ViewErrTask.parent: menu1Compile
*ViewErrTask.labelString: "View Error Task"
*ViewErrTask.activateCallback: {\
#ifndef DESIGN_TIME\
char error_file[100],comando[200],appoggio[200];\
char nome_task[100];\
extern int estrai_nome(char *, char*);\
\
if( getenv(ENVPAG) == NULL)\
   return;\
\
#ifndef VMS\
strcpy(error_file,XlGetenv(ENVPAG));\
strcat(error_file,"/");\
strcpy(appoggio,XlGetenv(ENVPAG));\
#else\
strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));\
strcpy(appoggio,XlConvDirVms(XlGetenv(ENVPAG)));\
#endif\
OlEstrRegTaskName(appoggio,nome_task);\
strcat(error_file,nome_task);\
strcat(error_file,REG_EXT);\
strcat(error_file,ERR_EXT);\
\
strcpy(comando,"edit_f01   ");\
strcat(comando,error_file);\
strcat(comando,"  False &");\
\
if( system(NULL) != 0)\
   system( comando );  \
#endif\
}

*InterfaceMode.class: rowColumn
*InterfaceMode.static: true
*InterfaceMode.name: InterfaceMode
*InterfaceMode.parent: menu2
*InterfaceMode.rowColumnType: "menu_pulldown"

*Mode.class: toggleButton
*Mode.static: true
*Mode.name: Mode
*Mode.parent: InterfaceMode
*Mode.labelString: "Interface Mode"
*Mode.valueChangedCallback: {\
  extern void  set_interface();\
   \
  if(StateInterfaceMode == True)\
  {\
     if(npagopen > 0)\
        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Close all the page first." ,NULL,False,NULL,False,NULL);\
     else \
     {\
        StateInterfaceMode = False;\
        set_interface(StateInterfaceMode);\
        if(ContextLoaded)\
           TopMenuSetSensitive();\
     }\
  } \
  else\
  {\
     if(npagopen > 0)\
        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Close all the page first." ,NULL,False,NULL,False,NULL);\
     else \
     {\
        StateInterfaceMode = True;\
        set_interface(StateInterfaceMode);\
        if(ContextLoaded)\
           TopMenuSetInsensitive();\
     }\
  } \
  set_something(Mode,XmNset,StateInterfaceMode);\
}

*menu1_top_b4.class: cascadeButton
*menu1_top_b4.static: true
*menu1_top_b4.name: menu1_top_b4
*menu1_top_b4.parent: menu2
*menu1_top_b4.labelString: "File"
*menu1_top_b4.mnemonic: "F"
*menu1_top_b4.subMenuId: "menu1File1"

*menu1_top_b5.class: cascadeButton
*menu1_top_b5.static: true
*menu1_top_b5.name: menu1_top_b5
*menu1_top_b5.parent: menu2
*menu1_top_b5.labelString: "Create"
*menu1_top_b5.subMenuId: "menu1Create1"
*menu1_top_b5.mnemonic: "C"

*menu1_top_b6.class: cascadeButton
*menu1_top_b6.static: true
*menu1_top_b6.name: menu1_top_b6
*menu1_top_b6.parent: menu2
*menu1_top_b6.labelString: "Edit"
*menu1_top_b6.mnemonic: "E"
*menu1_top_b6.subMenuId: "menu1Edit1"

*menu2_top_b2.class: cascadeButton
*menu2_top_b2.static: true
*menu2_top_b2.name: menu2_top_b2
*menu2_top_b2.parent: menu2
*menu2_top_b2.labelString: "Import"
*menu2_top_b2.subMenuId: "menu1Import"

*menu2_top_b3.class: cascadeButton
*menu2_top_b3.static: true
*menu2_top_b3.name: menu2_top_b3
*menu2_top_b3.parent: menu2
*menu2_top_b3.labelString: "Compile"
*menu2_top_b3.subMenuId: "menu1Compile"
*menu2_top_b3.sensitive: "false"

*menu2_top_b1.class: cascadeButtonGadget
*menu2_top_b1.static: true
*menu2_top_b1.name: menu2_top_b1
*menu2_top_b1.parent: menu2
*menu2_top_b1.labelString: "Interface Mode"
*menu2_top_b1.subMenuId: "InterfaceMode"

*panedWindow1.class: panedWindow
*panedWindow1.static: true
*panedWindow1.name: panedWindow1
*panedWindow1.parent: mainWindow

*form8.class: form
*form8.static: true
*form8.name: form8
*form8.parent: panedWindow1
*form8.resizePolicy: "resize_none"
*form8.x: 3
*form8.y: 3
*form8.width: 671
*form8.height: 315
*form8.paneMaximum: 500
*form8.paneMinimum: 200
*form8.allowResize: "true"

*form5.class: form
*form5.static: true
*form5.name: form5
*form5.parent: form8
*form5.resizePolicy: "resize_none"
*form5.x: 0
*form5.y: 50
*form5.width: 649
*form5.height: 267
*form5.rightAttachment: "attach_form"

*LabelProject.class: label
*LabelProject.static: true
*LabelProject.name: LabelProject
*LabelProject.parent: form5
*LabelProject.x: 20
*LabelProject.y: 50
*LabelProject.width: 150
*LabelProject.height: 15
*LabelProject.alignment: "alignment_beginning"
*LabelProject.labelString: "Base directory :"
*LabelProject.topOffset: 20
*LabelProject.topAttachment: "attach_form"

*labelSimulator.class: label
*labelSimulator.static: true
*labelSimulator.name: labelSimulator
*labelSimulator.parent: form5
*labelSimulator.x: 20
*labelSimulator.y: 170
*labelSimulator.width: 110
*labelSimulator.height: 15
*labelSimulator.alignment: "alignment_beginning"
*labelSimulator.labelString: "Simulator :"
*labelSimulator.bottomAttachment: "attach_none"
*labelSimulator.bottomOffset: 0
*labelSimulator.topAttachment: "attach_form"
*labelSimulator.topOffset: 170

*labelAnimateIconLibraries.class: label
*labelAnimateIconLibraries.static: true
*labelAnimateIconLibraries.name: labelAnimateIconLibraries
*labelAnimateIconLibraries.parent: form5
*labelAnimateIconLibraries.x: 20
*labelAnimateIconLibraries.y: 140
*labelAnimateIconLibraries.width: 170
*labelAnimateIconLibraries.height: 15
*labelAnimateIconLibraries.alignment: "alignment_beginning"
*labelAnimateIconLibraries.labelString: "Animated Icon Libraries :"
*labelAnimateIconLibraries.bottomAttachment: "attach_none"
*labelAnimateIconLibraries.bottomOffset: 8
*labelAnimateIconLibraries.bottomWidget: ""
*labelAnimateIconLibraries.topAttachment: "attach_form"
*labelAnimateIconLibraries.topOffset: 140

*labelObjectLibraries.class: label
*labelObjectLibraries.static: true
*labelObjectLibraries.name: labelObjectLibraries
*labelObjectLibraries.parent: form5
*labelObjectLibraries.x: 20
*labelObjectLibraries.y: 110
*labelObjectLibraries.width: 140
*labelObjectLibraries.height: 15
*labelObjectLibraries.alignment: "alignment_beginning"
*labelObjectLibraries.labelString: "Object Libraries :"
*labelObjectLibraries.topOffset: 110
*labelObjectLibraries.topAttachment: "attach_form"

*LabelDescription.class: label
*LabelDescription.static: true
*LabelDescription.name: LabelDescription
*LabelDescription.parent: form5
*LabelDescription.height: 15
*LabelDescription.alignment: "alignment_beginning"
*LabelDescription.labelString: "Description :"
*LabelDescription.topOffset: 50
*LabelDescription.leftOffset: 20
*LabelDescription.leftAttachment: "attach_form"
*LabelDescription.topAttachment: "attach_form"
*LabelDescription.x: 20
*LabelDescription.y: 50
*LabelDescription.width: 100

*textProject.class: label
*textProject.static: true
*textProject.name: textProject
*textProject.parent: form5
*textProject.x: 180
*textProject.y: 50
*textProject.width: 300
*textProject.height: 15
*textProject.labelString: ""
*textProject.topAttachment: "attach_form"
*textProject.topOffset: 20
*textProject.leftAttachment: "attach_form"
*textProject.leftOffset: 200
*textProject.rightAttachment: "attach_form"
*textProject.rightOffset: 10
*textProject.alignment: "alignment_beginning"

*textObjectLibraries.class: label
*textObjectLibraries.static: true
*textObjectLibraries.name: textObjectLibraries
*textObjectLibraries.parent: form5
*textObjectLibraries.x: 180
*textObjectLibraries.y: 140
*textObjectLibraries.width: 300
*textObjectLibraries.height: 15
*textObjectLibraries.labelString: ""
*textObjectLibraries.topOffset: 110
*textObjectLibraries.topAttachment: "attach_form"
*textObjectLibraries.leftAttachment: "attach_form"
*textObjectLibraries.leftOffset: 200
*textObjectLibraries.rightAttachment: "attach_form"
*textObjectLibraries.rightOffset: 10
*textObjectLibraries.alignment: "alignment_beginning"

*textAnimatedIconLibraries.class: label
*textAnimatedIconLibraries.static: true
*textAnimatedIconLibraries.name: textAnimatedIconLibraries
*textAnimatedIconLibraries.parent: form5
*textAnimatedIconLibraries.x: 220
*textAnimatedIconLibraries.y: 140
*textAnimatedIconLibraries.width: 419
*textAnimatedIconLibraries.height: 15
*textAnimatedIconLibraries.labelString: ""
*textAnimatedIconLibraries.topAttachment: "attach_form"
*textAnimatedIconLibraries.topOffset: 140
*textAnimatedIconLibraries.leftAttachment: "attach_form"
*textAnimatedIconLibraries.leftOffset: 200
*textAnimatedIconLibraries.rightAttachment: "attach_form"
*textAnimatedIconLibraries.rightOffset: 10
*textAnimatedIconLibraries.alignment: "alignment_beginning"

*textSimulator.class: label
*textSimulator.static: true
*textSimulator.name: textSimulator
*textSimulator.parent: form5
*textSimulator.x: 180
*textSimulator.y: 200
*textSimulator.width: 300
*textSimulator.height: 15
*textSimulator.labelString: ""
*textSimulator.topOffset: 170
*textSimulator.topAttachment: "attach_form"
*textSimulator.leftAttachment: "attach_form"
*textSimulator.leftOffset: 200
*textSimulator.rightAttachment: "attach_form"
*textSimulator.rightOffset: 10
*textSimulator.alignment: "alignment_beginning"

*labelPages.class: label
*labelPages.static: true
*labelPages.name: labelPages
*labelPages.parent: form5
*labelPages.x: 20
*labelPages.y: 80
*labelPages.width: 80
*labelPages.height: 15
*labelPages.alignment: "alignment_beginning"
*labelPages.labelString: "Pages :"
*labelPages.leftAttachment: "attach_form"
*labelPages.leftOffset: 20
*labelPages.topAttachment: "attach_form"
*labelPages.topOffset: 80

*textPages.class: label
*textPages.static: true
*textPages.name: textPages
*textPages.parent: form5
*textPages.x: 180
*textPages.y: 110
*textPages.width: 300
*textPages.height: 15
*textPages.labelString: ""
*textPages.topAttachment: "attach_form"
*textPages.topOffset: 80
*textPages.rightAttachment: "attach_form"
*textPages.rightOffset: 10
*textPages.leftAttachment: "attach_form"
*textPages.leftOffset: 200
*textPages.alignment: "alignment_beginning"

*textDescription1.class: label
*textDescription1.static: true
*textDescription1.name: textDescription1
*textDescription1.parent: form5
*textDescription1.x: 200
*textDescription1.y: 50
*textDescription1.width: 300
*textDescription1.height: 15
*textDescription1.labelString: ""
*textDescription1.rightAttachment: "attach_form"
*textDescription1.rightOffset: 10
*textDescription1.leftAttachment: "attach_form"
*textDescription1.leftOffset: 200
*textDescription1.alignment: "alignment_beginning"

*ContextLbl.class: label
*ContextLbl.static: true
*ContextLbl.name: ContextLbl
*ContextLbl.parent: form8
*ContextLbl.x: 160
*ContextLbl.y: 10
*ContextLbl.width: 350
*ContextLbl.height: 40
*ContextLbl.fontList: "-adobe-courier-medium-o-normal--24-240-75-75-m-150-iso8859-1"
*ContextLbl.labelString: "NO CONTEXT LOADED"
*ContextLbl.alignment: "alignment_center"
*ContextLbl.leftAttachment: "attach_form"
*ContextLbl.rightAttachment: "attach_form"
*ContextLbl.rightOffset: 10
*ContextLbl.leftOffset: 10
*ContextLbl.topAttachment: "attach_form"
*ContextLbl.topOffset: 10

*form4.class: form
*form4.static: true
*form4.name: form4
*form4.parent: panedWindow1
*form4.resizePolicy: "resize_none"
*form4.x: 0
*form4.y: 95
*form4.width: 796
*form4.height: 300
*form4.paneMinimum: 100

*scrolledWindow4.class: scrolledWindow
*scrolledWindow4.static: true
*scrolledWindow4.name: scrolledWindow4
*scrolledWindow4.parent: form4
*scrolledWindow4.scrollingPolicy: "application_defined"
*scrolledWindow4.x: 5
*scrolledWindow4.y: 22
*scrolledWindow4.visualPolicy: "variable"
*scrolledWindow4.scrollBarDisplayPolicy: "static"
*scrolledWindow4.shadowThickness: 0
*scrolledWindow4.bottomAttachment: "attach_form"
*scrolledWindow4.bottomOffset: 5
*scrolledWindow4.leftAttachment: "attach_form"
*scrolledWindow4.leftOffset: 5
*scrolledWindow4.rightAttachment: "attach_form"
*scrolledWindow4.rightOffset: 5
*scrolledWindow4.topAttachment: "attach_form"
*scrolledWindow4.topOffset: 25

*ListPagine.class: scrolledList
*ListPagine.name.source: public
*ListPagine.static: false
*ListPagine.name: ListPagine
*ListPagine.parent: scrolledWindow4
*ListPagine.width: 659
*ListPagine.height: 191
*ListPagine.selectionPolicy: "extended_select"
*ListPagine.singleSelectionCallback: {\
int *selected,selcount;\
\
LibraryDeselectAllItem();\
XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount);\
      \
if(selcount > 0)\
   PaginaSelezionata = selected[0];\
else\
   PaginaSelezionata = 0;\
\
}
*ListPagine.doubleClickInterval: 300
*ListPagine.defaultActionCallback: {\
open_page(TIPO_PAGINA);\
}

*menu1.class: rowColumn
*menu1.static: true
*menu1.name: menu1
*menu1.parent: ListPagine
*menu1.rowColumnType: "menu_popup"
*menu1.menuAccelerator: "<KeyUp>F4"

*menu1_p1_title.class: label
*menu1_p1_title.static: true
*menu1_p1_title.name: menu1_p1_title
*menu1_p1_title.parent: menu1
*menu1_p1_title.labelString: "Edit"

*menu1_p1_b5.class: separatorGadget
*menu1_p1_b5.static: true
*menu1_p1_b5.name: menu1_p1_b5
*menu1_p1_b5.parent: menu1

*PBOpen.class: pushButton
*PBOpen.static: true
*PBOpen.name: PBOpen
*PBOpen.parent: menu1
*PBOpen.labelString: "Open"
*PBOpen.activateCallback: {\
   open_page(TIPO_PAGINA);\
}\


*PBModify.class: pushButton
*PBModify.static: true
*PBModify.name: PBModify
*PBModify.parent: menu1
*PBModify.labelString: "Modify"
*PBModify.activateCallback: {\
   modify_page(TIPO_PAGINA);\
}

*PBDuplicate.class: pushButton
*PBDuplicate.static: true
*PBDuplicate.name: PBDuplicate
*PBDuplicate.parent: menu1
*PBDuplicate.labelString: "Duplicate"
*PBDuplicate.activateCallback: {\
   duplicate_page();\
}

*PBDelete.class: pushButton
*PBDelete.static: true
*PBDelete.name: PBDelete
*PBDelete.parent: menu1
*PBDelete.labelString: "Delete"
*PBDelete.activateCallback: {\
   del_page(TIPO_PAGINA);\
}

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: form4
*label4.x: 4
*label4.y: 2
*label4.width: 154
*label4.height: 20
*label4.labelString: "Pages List"
*label4.alignment: "alignment_beginning"

*form9.class: form
*form9.static: true
*form9.name: form9
*form9.parent: panedWindow1
*form9.resizePolicy: "resize_none"
*form9.x: 3
*form9.y: 460
*form9.width: 662
*form9.height: 200
*form9.paneMinimum: 100

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: form9
*label5.x: 6
*label5.y: 4
*label5.width: 175
*label5.height: 18
*label5.labelString: "Libraries List"
*label5.alignment: "alignment_beginning"

*scrolledWindow5.class: scrolledWindow
*scrolledWindow5.static: true
*scrolledWindow5.name: scrolledWindow5
*scrolledWindow5.parent: form9
*scrolledWindow5.scrollingPolicy: "application_defined"
*scrolledWindow5.x: 0
*scrolledWindow5.y: 0
*scrolledWindow5.visualPolicy: "variable"
*scrolledWindow5.scrollBarDisplayPolicy: "static"
*scrolledWindow5.shadowThickness: 0
*scrolledWindow5.bottomAttachment: "attach_form"
*scrolledWindow5.bottomOffset: 5
*scrolledWindow5.leftOffset: 5
*scrolledWindow5.rightAttachment: "attach_form"
*scrolledWindow5.rightOffset: 5
*scrolledWindow5.topOffset: 25
*scrolledWindow5.leftAttachment: "attach_form"
*scrolledWindow5.topAttachment: "attach_form"

*LibraryList.class: scrolledList
*LibraryList.name.source: public
*LibraryList.static: false
*LibraryList.name: LibraryList
*LibraryList.parent: scrolledWindow5
*LibraryList.width: 654
*LibraryList.height: 66
*LibraryList.selectionPolicy: "single_select"
*LibraryList.singleSelectionCallback: {\
PageDeselectAllItem(ListPagine);\
\
}
*LibraryList.visibleItemCount: 11
*LibraryList.defaultActionCallback: {\
PageDeselectAllItem();\
open_page(TIPO_LIBRARY);\
}

*menu6.class: rowColumn
*menu6.static: true
*menu6.name: menu6
*menu6.parent: LibraryList
*menu6.rowColumnType: "menu_popup"
*menu6.menuAccelerator: "<KeyUp>F4"

*menu6_p1_title.class: label
*menu6_p1_title.static: true
*menu6_p1_title.name: menu6_p1_title
*menu6_p1_title.parent: menu6
*menu6_p1_title.labelString: "Edit"

*separ1.class: separatorGadget
*separ1.static: true
*separ1.name: separ1
*separ1.parent: menu6

*OpenLibrary.class: pushButton
*OpenLibrary.static: true
*OpenLibrary.name: OpenLibrary
*OpenLibrary.parent: menu6
*OpenLibrary.labelString: "Open"
*OpenLibrary.activateCallback: {\
   open_page(TIPO_LIBRARY);\
}

*ModifyLibrary.class: pushButton
*ModifyLibrary.static: true
*ModifyLibrary.name: ModifyLibrary
*ModifyLibrary.parent: menu6
*ModifyLibrary.labelString: "Modify"
*ModifyLibrary.activateCallback: {\
   modify_page(TIPO_LIBRARY);\
}

*DeleteLibrary.class: pushButton
*DeleteLibrary.static: true
*DeleteLibrary.name: DeleteLibrary
*DeleteLibrary.parent: menu6
*DeleteLibrary.labelString: "Delete"
*DeleteLibrary.activateCallback: {\
   del_page(TIPO_LIBRARY);\
} 

*form3.class: form
*form3.static: true
*form3.name: form3
*form3.parent: panedWindow1
*form3.resizePolicy: "resize_none"
*form3.x: 3
*form3.y: 577
*form3.width: 668
*form3.height: 178

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: form3
*label8.x: 15
*label8.y: 9
*label8.width: 110
*label8.height: 25
*label8.alignment: "alignment_beginning"
*label8.labelString: "Messages"
*label8.rightAttachment: "attach_none"
*label8.rightOffset: 5
*label8.topOffset: 5
*label8.leftAttachment: "attach_form"
*label8.leftOffset: 5
*label8.topAttachment: "attach_form"

*scrolledWindow1.class: scrolledWindow
*scrolledWindow1.static: true
*scrolledWindow1.name: scrolledWindow1
*scrolledWindow1.parent: form3
*scrolledWindow1.scrollingPolicy: "application_defined"
*scrolledWindow1.x: 10
*scrolledWindow1.y: 40
*scrolledWindow1.visualPolicy: "variable"
*scrolledWindow1.scrollBarDisplayPolicy: "static"
*scrolledWindow1.shadowThickness: 0
*scrolledWindow1.bottomAttachment: "attach_form"
*scrolledWindow1.bottomOffset: 10
*scrolledWindow1.leftAttachment: "attach_form"
*scrolledWindow1.leftOffset: 10
*scrolledWindow1.rightAttachment: "attach_form"
*scrolledWindow1.rightOffset: 10
*scrolledWindow1.topAttachment: "attach_form"
*scrolledWindow1.topOffset: 40

*scrolledText1.class: scrolledText
*scrolledText1.static: true
*scrolledText1.name: scrolledText1
*scrolledText1.parent: scrolledWindow1
*scrolledText1.width: 620
*scrolledText1.height: 81
*scrolledText1.columns: 60
*scrolledText1.editMode: "multi_line_edit"
*scrolledText1.editable: "false"
*scrolledText1.resizeHeight: "true"
*scrolledText1.resizeWidth: "true"

*menu5.class: rowColumn
*menu5.static: true
*menu5.name: menu5
*menu5.parent: scrolledText1
*menu5.rowColumnType: "menu_popup"

*menu5_p1_b1.class: pushButton
*menu5_p1_b1.static: true
*menu5_p1_b1.name: menu5_p1_b1
*menu5_p1_b1.parent: menu5
*menu5_p1_b1.labelString: "Clear Message"
*menu5_p1_b1.activateCallback: {\
\
   XmTextSetString(scrolledText1,(char *)"");\
\
}

