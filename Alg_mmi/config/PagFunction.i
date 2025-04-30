! UIMX ascii 2.5 key: 6340                                                      

*PagFunction.class: promptDialog
*PagFunction.gbldecl: /*\
        Inizio sezione per SCCS\
*/\
/*\
   modulo PagFunction.i\
   tipo \
   release 5.1\
   data 11/13/95\
   reserved @(#)PagFunction.i	5.1\
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
#include "config.h"\
#include "message.h"\
extern swidget topLevelShell;
*PagFunction.ispecdecl: int tipo_operaz;\

*PagFunction.ispeclist: tipo_operaz
*PagFunction.ispeclist.tipo_operaz: "int", "%tipo_operaz%"
*PagFunction.funcdecl: swidget creat_PagFunction(int op)\

*PagFunction.funcname: creat_PagFunction
*PagFunction.funcdef: "swidget", "<creat_PagFunction>(%)"
*PagFunction.argdecl: int op;
*PagFunction.arglist: op
*PagFunction.arglist.op: "int", "%op%"
*PagFunction.icode:
*PagFunction.fcode: tipo_operaz = op;\
return(rtrn);\

*PagFunction.auxdecl: /*\
   estrae da fileSpec il nome del file senza path \
   se non ce l'ha ritorna lo stesso puntatore\
   il chiamante deve verificare che non sia NULL \
ATTENZIONE AL TOKEN \
*/\
\
char *extractFileName(char *fileSpec) \
{\
    int i,len;\
    char *NamePointer;\
#ifdef VMS\
    char token = ']';\
    printf("ATTENZIONE cerco il nome del file con separatore ]\n");\
#else\
    char token = '/';\
#endif\
\
\
    len = strlen(fileSpec);\
    \
    for(i=len-1;i>=0;i--)\
       if(fileSpec[i] == token)\
       {\
          NamePointer = &(fileSpec[i+1]);\
          return(NamePointer);\
       }\
    return(fileSpec);\
}
*PagFunction.name.source: public
*PagFunction.static: false
*PagFunction.name: PagFunction
*PagFunction.parent: NO_PARENT
*PagFunction.defaultShell: dialogShell
*PagFunction.dialogType: "dialog_prompt"
*PagFunction.unitType: "pixels"
*PagFunction.x: 240
*PagFunction.y: 375
*PagFunction.width: 540
*PagFunction.height: 255
*PagFunction.dialogStyle: "dialog_primary_application_modal"
*PagFunction.textColumns: 20
*PagFunction.selectionLabelString: "Page Name"
*PagFunction.textString: ""
*PagFunction.okCallback: {\
extern Arg args[];\
extern Cardinal lnargs;\
extern char *extract_string();\
extern PAGEDIT_CONTEXT *pagedit;\
extern void add_item();\
XmString pagname;\
char filename[MAXLENFILENAME];\
char strpagname[MAXCHAR_PAGNAME];\
char pag_inlist[MAXCHAR_PAGNAME],comando[200];\
int i;\
FILE *fp; \
Boolean pathSpec=False;\
char *nomePag;\
\
/* IMPORT PAG devo gestire i casi\
   il file indicato deve essere esistente.\
   puo' essere nella directory LEGOMMI_PAG corrente oppure in una altra\
   se e' non e' nella LEGOMMI_PAG ce lo copio dentro se non c'e' n'e'\
      gia' uno con lo stesso nome\
   non puo' essere gia incluso nel Context corrente \
*/   \
\
if( getenv(ENVPAG) == NULL ) \
   return;\
\
lnargs=0;\
XtSetArg(args[lnargs],XmNtextString,&pagname);lnargs++;\
XtGetValues(UxWidget,args,lnargs);\
\
/* recupero la pagina da importare */\
strcpy(strpagname,extract_string(pagname));\
\
switch(tipo_operaz)\
{\
   case IMPORT_PAGE:\
      /* verifica se il nome del file contiene la path */ \
      nomePag = extractFileName(strpagname);\
      if( strcmp(strpagname,nomePag) )\
          pathSpec = True;\
\
      /* viene richiesto l'import da LEGOMMI_PAG */\
      if(!pathSpec)\
      {\
        /* viene richiesto l'import da LEGOMMI_PAG */\
         strcpy(filename,getenv(ENVPAG));\
         strcat(filename,"/"); \
         strcat(filename,nomePag);\
         strcat(filename,PAG_EXT);\
         if( ( fp = file_exist(filename)) == NULL)\
         {\
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); \
            return;\
         }\
         fclose(fp);\
      }\
      else\
      {\
         /* viene richiesto l'import da una altra directory */\
         /* se per caso e' la stessa il file viene copiato ugualmente */\
\
         strcpy(filename,strpagname); \
         strcat(filename,PAG_EXT);\
         if( ( fp = file_exist(filename)) == NULL)\
         {\
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); \
            return;\
         }\
     \
         if( system(NULL) == 0)\
         {\
            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Unix Command Shell Not Available",NULL,False,NULL,False,NULL); \
            return;\
         }\
         else\
         {\
            strcpy(comando,"cp ");\
            strcat(comando,filename);\
            strcat(comando," "); \
            strcat(comando,getenv(ENVPAG)); \
            strcat(comando,"/"); \
            strcat(comando,nomePag);\
            strcat(comando,PAG_EXT);\
            system(comando);\
\
            strcpy(filename,strpagname); \
            strcat(filename,BKG_EXT);\
            strcpy(comando,"cp ");\
            strcat(comando,filename);\
            strcat(comando," "); \
            strcat(comando,getenv(ENVPAG)); \
            strcat(comando,"/"); \
            strcat(comando,nomePag);\
            strcat(comando,BKG_EXT);\
            system(comando);      }\
      }\
  \
      /* se e' gia presente nel context non termino la operazione */\
      for(i=0;i<pagedit->pag_num;i++)\
      {\
         if(!strcmp(pagedit->page_list[i],nomePag))\
         {\
            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Already Exist",NULL,False,NULL,False,NULL); \
            return;\
         }\
      }\
\
      /* aggiorno la lista delle pagine */      \
      if( (pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1))) == NULL)\
      {\
         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); \
         return;\
      }\
      if( (pagedit->page_list[pagedit->pag_num] = alloca_memoria(strlen(nomePag)+1,sizeof(char) )) == NULL)\
         return;\
\
      strcpy(pagedit->page_list[pagedit->pag_num],nomePag);\
      pagedit->pag_num++;\
\
      pagname = XmStringCreateSimple(nomePag);\
      add_item(pagname,0);         \
\
\
   break;\
}\
 \
UxDestroyInterface(UxWidget);\
\
}
*PagFunction.applyLabelString: ""

