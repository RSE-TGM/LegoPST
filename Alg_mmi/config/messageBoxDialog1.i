! UIMX ascii 2.5 key: 4642                                                      

*messageBoxDialog1.class: messageBoxDialog
*messageBoxDialog1.gbldecl: /*\
        Inizio sezione per SCCS\
*/\
/*\
   modulo messageBoxDialog1.i\
   tipo \
   release 5.1\
   data 11/13/95\
   reserved @(#)messageBoxDialog1.i	5.1\
*/\
/*\
        Variabile per identificazione della versione\
*/\
/*\
        Fine sezione per SCCS\
*/\
\
#include <stdio.h>\
#include <string.h>\
#include <Xm/Xm.h>\
#include "config.h"\
extern Arg args[];\
extern Cardinal lnargs;\
extern PAGEDIT_CONTEXT *pagedit;\
extern Boolean StateInterfaceMode;
*messageBoxDialog1.ispecdecl: char parametro[MAXLENFILENAME];
*messageBoxDialog1.ispeclist: parametro
*messageBoxDialog1.ispeclist.parametro: "unsigned char", "%parametro%[150]"
*messageBoxDialog1.funcdecl: swidget create_messageBoxDialog1(int id_op,char *message,char *param)\

*messageBoxDialog1.funcname: create_messageBoxDialog1
*messageBoxDialog1.funcdef: "swidget", "<create_messageBoxDialog1>(%)"
*messageBoxDialog1.argdecl: int id_op;\
unsigned char *message;\
unsigned char *param;
*messageBoxDialog1.arglist: id_op, message, param
*messageBoxDialog1.arglist.id_op: "int", "%id_op%"
*messageBoxDialog1.arglist.message: "unsigned char", "*%message%"
*messageBoxDialog1.arglist.param: "unsigned char", "*%param%"
*messageBoxDialog1.icode: /*\
  Questa interfaccia gestisce le operazioni di \
  EXIT_WITH_SAVE, EXIT_NO_CHANGE, DELETE_PAGE, DELETE_LIBRARY,\
  CLOSE_PAGE, KILL_SIMULATOR\
\
  riceve in input l'ID dell'operazione, il messaggio \
  da visualizzare e un parametro char* che puo'\
  essere in base all'ID dell'operazione il puntatore\
  al context o ad una pagina.   \
*/\
XmString Msg,Msg1,Msg2,Msg3;\
char *intern_msg;\
int num_elem;\
\
if(param != NULL)\
   strcpy(parametro,param);
*messageBoxDialog1.fcode: if( param != NULL)\
   num_elem = strlen(message)+strlen(param) +2 ;\
else\
   num_elem = strlen(message)+2 ;\
\
if(num_elem <= 0)\
   return(NULL);\
\
if((intern_msg = (char *)alloca_memoria(  num_elem ,sizeof(char))) == NULL)\
   return(NULL);\
  \
\
if(param != NULL)\
   sprintf(intern_msg,message,param); \
else\
   strcpy(intern_msg,message);\
\
Msg = XmStringCreateSimple(intern_msg);\
\
lnargs=0;\
XtSetArg(args[lnargs],XmNmessageString,Msg);lnargs++;\
\
if( (id_op == EXIT_WITH_SAVE) || (id_op == KILL_SIMULATOR) ||\
    (id_op == SAVE_AND_LOAD_NEWCTX) || \
    (id_op == SAVE_AND_LOAD_OLDCTX) || (id_op == EXIT_NO_CHANGE) ||\
    (id_op == ALIGN_CONTEXT)  )\
{\
   Msg1 = XmStringCreateSimple("Yes");\
   XtSetArg(args[lnargs],XmNokLabelString,Msg1);lnargs++;\
   Msg2 = XmStringCreateSimple("No");\
   XtSetArg(args[lnargs],XmNcancelLabelString,Msg2);lnargs++;\
   Msg3 = XmStringCreateSimple("Cancel");\
   XtSetArg(args[lnargs],XmNhelpLabelString,Msg3);lnargs++;\
}\
XtSetValues(rtrn,args,lnargs);\
\
XmStringFree(Msg);\
libera_memoria((char*)intern_msg);\
if( (id_op == EXIT_WITH_SAVE) || (id_op == KILL_SIMULATOR) ||\
    (id_op == SAVE_AND_LOAD_NEWCTX) ||\
    (id_op == SAVE_AND_LOAD_OLDCTX) || (id_op == EXIT_NO_CHANGE) ||\
    (id_op == ALIGN_CONTEXT)  )\
{\
   XmStringFree(Msg1);\
   XmStringFree(Msg2);\
   XmStringFree(Msg3);\
}\
\
return(rtrn);
*messageBoxDialog1.auxdecl:
*messageBoxDialog1.static: true
*messageBoxDialog1.name: messageBoxDialog1
*messageBoxDialog1.parent: NO_PARENT
*messageBoxDialog1.defaultShell: topLevelShell
*messageBoxDialog1.unitType: "pixels"
*messageBoxDialog1.x: 350
*messageBoxDialog1.y: 300
*messageBoxDialog1.width: 550
*messageBoxDialog1.height: 260
*messageBoxDialog1.allowOverlap: "false"
*messageBoxDialog1.dialogStyle: "dialog_full_application_modal"
*messageBoxDialog1.dialogTitle: "Message"
*messageBoxDialog1.msgDialogType: "dialog_question"
*messageBoxDialog1.messageAlignment: "alignment_center"
*messageBoxDialog1.noResize: "true"
*messageBoxDialog1.cancelCallback: {\
extern FILE *fpLogMsg;\
\
switch(id_op)\
{\
   case EXIT_WITH_SAVE:\
      fclose(fpLogMsg);\
      exit(0);\
   break;\
}\
}
*messageBoxDialog1.okCallback: {\
{\
#ifndef DESIGN_TIME\
   extern void save_conn();\
   extern void destroy_conn();  \
#endif\
   extern swidget creat_DefineContext();\
   extern void close_page();\
   extern int new_save_context();\
   extern Boolean chiudi_simul;\
   extern void change_database(); \
   extern void richiedi_change_database(); \
   extern FILE *fpLogMsg;\
   extern swidget ContextSelection1;\
   extern Boolean AlignResPageContext();\
   extern void PrintAllPages();\
\
   switch(id_op)\
   {\
      case SAVE_AND_LOAD_OLDCTX:\
         printf("richiedo il save del context\n se non viene accordato non faccio nienete\n");\
         printf("se viene accordato chiamo la LOAD_CONTEXT \n");                 \
   \
         if( new_save_context(pagedit) == False)\
         {\
            printf("Non riesco a salvare la situazione\n");\
            printf("provare a copiare il file Context precedente\n");\
            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");\
         }   \
         else\
         {\
            printf("\n\n\tTento di caricare il Context %s\n",parametro);\
            if( ContextLoad(parametro,&pagedit))\
            {     \
               printf("Load del Context %s\n",parametro);\
               display_resources(pagedit); \
               richiedi_change_database();\
               UxDestroyInterface(UxWidget);\
               UxDestroyInterface(ContextSelection1);\
            }  \
            else          \
               printf("Non sono riuscito a caricare il Context %s\n",parametro);\
         }\
      break;\
      case SAVE_AND_LOAD_NEWCTX:\
         printf("richiedo il save del context\n se non viene accordato non faccio nienete\n");\
         printf("se viene accordato chiamo la LOAD_CONTEXT \n");                 \
   \
         if( new_save_context(pagedit) == False)\
         {\
            printf("Non riesco a salvare la situazione\n");\
            printf("provare a copiare il file Context precedente\n");\
            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");\
         }  \
         else\
         { \
             UxPopupInterface( creat_DefineContext(parametro,DEF_CONTEXT), no_grab);\
         }\
      break;\
      case ALIGN_CONTEXT:\
	 printf("Allineamento delle risorse presenti nel context alle corrispondenti nei files .pag\n");\
	 AlignResPageContext();\
      break; \
\
      case PRINT_ALL:\
	 PrintAllPages();\
      break;\
\
      case EXIT_WITH_SAVE:\
         if( StateInterfaceMode )\
         {\
             save_conn();\
             destroy_conn();\
         }\
         else if( new_save_context(param) == False)\
         {\
            printf("Non riesco a salvare la situazione\n");\
            printf("provare a copiare il file Context precedente\n");\
            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");\
         }   \
         fclose(fpLogMsg);\
         exit(0);\
      break;\
      case EXIT_NO_CHANGE:\
         if( StateInterfaceMode )\
             destroy_conn();\
         fclose(fpLogMsg);\
         exit(0);\
      break;\
      case DELETE_PAGE:\
         do_cancella(param,TIPO_PAGINA);	\
      break;\
      case DELETE_LIBRARY:\
         do_cancella(param,TIPO_LIBRARY);\
      break;\
      case CLOSE_PAGE:\
         close_page(param);\
      break;\
       case KILL_SIMULATOR:\
         change_database();\
      break;\
  }\
}\
\
}
*messageBoxDialog1.helpCallback: {\
switch(id_op)\
{\
   case EXIT_WITH_SAVE:\
      UxDestroyInterface(messageBoxDialog1);\
   break;\
}\
}

