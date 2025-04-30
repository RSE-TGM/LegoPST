/**********************************************************************
*
*       C Source:               comp_reg.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Mar 31 16:49:07 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: comp_reg.c-3 %  (%full_filespec: comp_reg.c-3:csrc:1 %)";
#endif

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/DrawingA.h>
#include <Xm/Frame.h>

#include <UxXt.h>

#include "config.h"

#include <sqlite3.h>

XtAppContext    UxAppContext;
Widget          UxTopLevel;
Display         *UxDisplay;
int             UxScreen;

sqlite3 *db;


main(argc,argv)
int	argc;
char	*argv[];
{
Widget CompileBoard;
PAGINA *pag=NULL;
AN_OBJ *lista_oggetti=NULL;

Boolean read_db;
int no_regolazione;
int flag=1;
int macrobl=0;
int ret=100;
int ret_int=0;
int j;
char pag_name[20];
char pag_path[FILENAME_MAX],rtf_path[FILENAME_MAX];

   if((argc<2)||(argc>4)) {
      fprintf(stderr,
      "Error!!! Number of parameters on command line\n\t");
      fprintf(stderr,
      "<compreg> <pag_name> <pages_path> <rtf_path>\n");
      exit(2);
   }
   else {
      strcpy(pag_name,argv[1]);
/*
printf("argc = %d\n",argc);
*/
      if(argc>2) {
         strcpy(pag_path,argv[2]);
         strcpy(rtf_path,argv[3]);
         if((setvarenv_appl(pag_path,rtf_path))==False) {
            exit(2); 
         }
      }
   }

   UxTopLevel = XtVaAppInitialize(&UxAppContext, "compreg",
                              NULL, 0, &argc, argv, NULL, NULL);

   UxDisplay = XtDisplay(UxTopLevel);
   UxScreen = XDefaultScreen(UxDisplay);

   read_db=False;
   CompileBoard = (Widget)create_ClipBoardComp();

   if(pagina_init(pag_name,&pag,TIPO_PAGINA)==True) {
      if(pagina_load_file(pag,TIPO_PAGINA)  &&
         pagina_getres(pag,&lista_oggetti)) 
         {
/*
         Bisogna evitare i tentativi di compilazione di un sinottico
         (che ovviamente non e' mai stato compilato come regolazione)
         nonostante un controllo vi sia anche nella routine di
         compilazione
*/
         no_regolazione=0;
         if(PagGetType(pag) != TYPE_REGOLAZIONE)
            {
            no_regolazione=1;
            printf("        no_regolazione=%d\n",no_regolazione);
            }
         read_db=True;
         if( (pag->num_widget > 0) && ( no_regolazione==0) )
            {
            UxDisplay->db = pag->db;
            for(j=0;j<pag->num_widget;j++) {
               XtCreateWidget(lista_oggetti[j].nome,
                              lista_oggetti[j].classe,
                              CompileBoard,NULL,0);
            }
            pag->drawing=CompileBoard;
/*
         Compilazione della pagina di regolazione corrente.
         Il risultato della compilazione viene valutato dal
         programma chiamante.
*/
            ret=compila_reg(pag,flag,macrobl);
            if(ret>=0)
               ret_int=0;
            else if(ret==-2)
               ret_int=1;
            else if(ret==-1)
               ret_int=2;
            libera_memoria((char*)lista_oggetti);
            XtDestroyWidget(CompileBoard);
            if(!read_db)
               pag->db = NULL;
            pagina_free(pag);
/*
printf("Terminato il processo 'compreg'\n\tSystem return value = %d\n",ret_int);
*/
            exit(ret_int);
            }
/*
         Liberazione della struttura list_oggetti
*/
            libera_memoria((char*)lista_oggetti);
         }
         if(!read_db)
            pag->db = NULL;
         pagina_free(pag);
      }
      XtDestroyWidget(CompileBoard);
      exit(2);

   XtAppMainLoop(UxAppContext);

}



