/*-----------------------------------------------------------
 * This is the project main program file for Xt generated 
 * code. You may add application dependent source code 
 * at the appropriate places. 
 * 			     
 * Do not modify the statements preceded by the dollar
 * sign ($), these statements will be replaced with
 * the appropriate source code when the main program is  
 * generated.    
 *
 * $Date: 92/04/23 14:57:13 $  		$Revision: 1.11.37.1.45.1 $ 
 *-----------------------------------------------------------*/ 
 /*
        Inizio sezione per SCCS
*/
/*
   modulo config.prj
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)config.prj	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)config.prj	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
#ifdef XOPEN_CATALOG  
#include <locale.h>
#endif
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <Xm/Xm.h>

/*---------------------------------------------------- 
 * UxXt.h needs to be included only when compiling a 
 * stand-alone application. 
 *---------------------------------------------------*/
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif /* DESIGN_TIME */

#include "config.h"
#include "CAP.h"		/* Serve per la gestione delle licenze software*/
// Guag2024 #include "skey_manager3.h"	/* Serve per la gestione delle licenze hardware*/

#include <sqlite3.h>

#define COMMANDFLAG   "-c"
#define COMMCOMPREG   "compreg"
#define COMMCOMPALL   "compall"
#define COMMCREATASK   "creatask"

XtAppContext	UxAppContext;
Widget		UxTopLevel;
Display		*UxDisplay;
int		UxScreen;
extern void istanzia_widget_parent_parent();
extern void istanzia_widget_parent();
extern void istanzia_widget();
extern void select_zone();
extern void move_or_resize();
extern void XlSetSelectRunTime();
extern void proc_deselect();
extern void proc_SelPort();
extern void proc_DeleteConn();
extern int crea_context();
extern void UxInitGraphics(); 

XmString errorString; /* compound string per visualizzazione errori */
int error_level;
char *error_msg[] = {
                       "File Context.ctx not found, no Context Loaded"
                   };

XtActionsRec actions[] = {
                            { "istanzia_widget_parent_parent",istanzia_widget_parent_parent },
                            { "istanzia_widget_parent",istanzia_widget_parent },
                            { "istanzia_widget",istanzia_widget },
                            { "select_zone",select_zone},
                            { "move_or_resize",move_or_resize} 
                         };

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/


extern int pagedit_context_init();
extern swidget errorDialog;
extern swidget create_errorDialog();

extern int compile_all_reg();
extern int compile_all_pag();
extern void destroy_conn();

extern FILE *fpLogMsg;
extern Boolean StateInterfaceMode; 

char Context_Path[255];

int  comm_found;

int  flag_demo = 0;		/*  0 --> disponibile licenza tipo full
                                    1 --> disponibile licenza tipo demo  */

#ifndef DESIGN_TIME
 
//Guag2024 
//PAGEDIT_CONTEXT *pagedit = NULL;

#endif

GC       xorGC;
Arg      args[10];
Cardinal lnargs;

XrmDatabase defdb;

sqlite3 *db;

main(argc,argv)
	int 	argc;
	char 	*argv[];
{
	/*-----------------------------------------------------------
	 * Declarations.
         * The default identifier - mainIface will only be declared 
	 * if the interface function is global and of type swidget.
	 * To change the identifier to a different name, modify the
	 * string mainIface in the file "xtmain.dat". If "mainIface"
         * is declared, it will be used below where the return value
	 * of  PJ_INTERFACE_FUNCTION_CALL will be assigned to it.
         *----------------------------------------------------------*/ 


  	Widget mainIface;

	/*---------------------------------
	 * Interface function declaration
	 *--------------------------------*/	

 	Widget  create_topLevelShell();

	

	/*---------------------
	 * Initialize program
	 *--------------------*/

char str_flag[5];
char str_comm[20];
char Buffer[ENELINFO_LEN + 1];
int indiceSocieta; //serve per gestire eventualmente comportamenti differenziati
char messaggioErrore[1024];
int errorCode;
int richiestaScrittura;

/*
   Si controlla se esiste la licenza per l'attivazione del programma.
   Prima la chiave hardware ed eventualmente quella software
*/
       // Controllo chiave hardware

/* Guag2024       richiestaScrittura=KEYWRITE;
       errorCode=skey_verifica_e_carica3(richiestaScrittura, KEYLABEL, KEYPASSWD, 
                                         LICCONFIG, &indiceSocieta);
       if(errorCode != 0)
         {
          skey_stampa_errore(errorCode, &messaggioErrore);
          printf("CONFIG:%s \n", messaggioErrore);
          //provo l'eventuale chiave software
          if(CAP(CONFIG_LIC, Buffer) == -1) {
             exit(1);
          }
          if(manageLicenseNoTime(Buffer,"config",&flag_demo) == -1)
             exit(1);
          }
*/

       comm_found=0;
       if((argc==2)||(argc>3)) {
          fprintf(stderr,
          "Error!!! Number of parameters on command line\n\t");
          fprintf(stderr,
          "<config> <-c> <command> [compreg, creatask, compall]\n");
          exit(1);
       }
       if(argc>1) {
/*
   La config e' lanciata con un comando. Si controlla se la sintassi e'
   corretta.
*/
         strcpy(str_flag,argv[1]);
         if(strcmp(str_flag,COMMANDFLAG)!=0) {
            fprintf(stderr,
            "Error on command line!!!\n");
            fprintf(stderr,
            "<config> <-c> <command>\n");
            exit(1);
         }
         else {
            strcpy(str_comm,argv[2]);
            if(strcmp(str_comm,COMMCOMPREG)!=0) {
               if(strcmp(str_comm,COMMCOMPALL)!=0) {
                  if(strcmp(str_comm,COMMCREATASK)!=0) {
                     fprintf(stderr,
                     "Command %s not found!!!\n",str_comm);
                     exit(1);
                     }
                  else {
                     comm_found=3;
                  }
               }
               else {
                  comm_found=2;
               } 
            }
            else {
               comm_found=1;
            }
         }
      }
if(comm_found!=0)
   printf("%s\t%s\t%s\n",argv[0],argv[1],argv[2]);

#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif
	testata("config",SccsID);

  	UxTopLevel = XtAppInitialize(&UxAppContext, "Config",
				     NULL, 0, &argc, argv, NULL, NULL, 0);

	UxDisplay = XtDisplay(UxTopLevel);
	UxScreen = XDefaultScreen(UxDisplay);

	/* We set the geometry of UxTopLevel so that dialogShells
	   that are parented on it will get centered on the screen
	   (if defaultPosition is true). */

	XtVaSetValues(UxTopLevel,
			XtNx, 0,
			XtNy, 0,
			XtNwidth, DisplayWidth(UxDisplay, UxScreen),
			XtNheight, DisplayHeight(UxDisplay, UxScreen),
			NULL);

	/*-------------------------------------------------------
	 * Insert initialization code for your application here 
	 *------------------------------------------------------*/

         crea_gc_xor(&xorGC);            /* definizione gc in Xor function */

          
         UxInitGraphics(); 

         XtAppAddActions(UxAppContext,actions,XtNumber(actions));

         XlSetSelectRunTime((caddr_t)proc_deselect);
         XlSetSelectPort((caddr_t)proc_SelPort);
	 XdSetDeleteConn((caddr_t)proc_DeleteConn);

#ifndef DESIGN_TIME

         /* se nella directory di lancio della config
            non esiste un context ne creo uno di default (con path relativi)
         */

         if( !file_exist("Context.ctx") )
         {

            printf("Context.ctx file not found\n");

            if( crea_context("Context.ctx","./",
                 "./", "Default Context.ctx",
                 "./", "./",
                 "./", "./",
                 "", "","") != TRUE )
            {
               printf("Error on default 'Context.ctx' creation\n");
               return(FALSE);
            }
            else
               printf("Default Context.ctx created.\n");

         }
         else
             printf("Context.ctx file found\n");

/* MAURIC
         pagedit_context_init("Context.ctx",&pagedit);
*/
#endif

         defdb = XtDatabase(UxDisplay);

         errorDialog = create_errorDialog();


	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = create_topLevelShell();

        if(comm_found!=0) {
           switch (comm_found)
              {
              case 1:
                 compile_all_reg();
                 fclose(fpLogMsg);
                 break;
              case 2:
                 compile_all_pag();
                 fclose(fpLogMsg);
                 break;
              case 3:
                 info_top_schemi();
                 fclose(fpLogMsg);
                 break;
              default:
                 break;
              }
           exit(0);   
        }

	UxPopupInterface(mainIface, no_grab);

	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/

  	XtAppMainLoop (UxAppContext);

}
