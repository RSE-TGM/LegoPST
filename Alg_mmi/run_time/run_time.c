/*
        Inizio sezione per SCCS
*/
/*
   modulo run_time.prj
   tipo 
   release 5.2
   data 1/22/96
   reserved @(#)run_time.prj	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)run_time.prj	5.2\t1/22/96";
/*
        Fine sezione per SCCS
*/



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
 * Date: 92/04/23 14:57:13   		Revision: 1.11.37.1.45.1  
 *-----------------------------------------------------------*/ 

#ifdef XOPEN_CATALOG
#include <locale.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

/*---------------------------------------------------- 
 * UxXt.h needs to be included only when compiling a 
 * stand-alone application. 
 *---------------------------------------------------*/
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif /* DESIGN_TIME */

#include <Rt/RtMemory.h>
# include "CAP.h"		/* Serve per la gestione delle licenze */
// Guag2024 #include "skey_manager3.h"	/* Serve per la gestione delle licenze hardware*/
#include <sqlite3.h>
#include "fontfallback.h"

XtAppContext	UxAppContext;
Widget		UxTopLevel;
Display		*UxDisplay;
int		UxScreen;

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/

Widget MainInterface;
char *nome_file_context;
double tempo_license;
int    flag_demo;
int    mmiMaxConErr=3;


sqlite3 *db;

void read_argv(int , char **);

int main(argc,argv)
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

 	Widget  popup_topLevelShellMain();


	


	/*---------------------
	 * Initialize program
	 *--------------------*/

char    Buffer[ENELINFO_LEN + 1];
int    indiceSocieta; //serve per gestire eventualmente comportamenti differenziati
char   messaggioErrore[1024];
int    errorCode;
int    richiestaScrittura;
double  tempo_corr;
int     ret;


#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "run_time",
				     NULL, 0, &argc, argv, NULL, NULL, 0);
  	FontFallbackInstall();  /* font 2-byte -> fallback 8-bit */

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
/*
   Verifica se è definito il valore massimo di errori consecutivi
   in shared memory
*/
	if( getenv("MMI_MAX_CON_ERR") != NULL)
	{
		mmiMaxConErr=atoi(getenv("MMI_MAX_CON_ERR"));
                printf("Settato mmiMaxConErr=%d \n",mmiMaxConErr);
 	}


/*
   Si controlla se esiste la licenza per l'attivazione del programma.
   Prima la chiave hardware ed eventualmente quella software
*/
        // Controllo chiave hardware
/* Guag2024        richiestaScrittura=KEYWRITE;
        errorCode=skey_verifica_e_carica3(richiestaScrittura, KEYLABEL, KEYPASSWD,
                                          LICMMI, &indiceSocieta);
	if(errorCode != 0)
	{
		skey_stampa_errore(errorCode, &messaggioErrore);
		printf("mmi:%s \n", messaggioErrore);

		//provo l'eventuale chiave software
		if(CAP(MMI_LIC, Buffer) == -1)
		{
			exit(1);
		}
		if(manageLicense(Buffer,"mmi",&flag_demo,&tempo_license) == -1)
		{
			exit(1);
		}
	}
	else
	{
		flag_demo=0;
		tempo_license=10000000;
	}
*/
        flag_demo=0;
        tempo_license=10000000;


/*
printf("mmi main DEBUG: flag_demo = %d\ttempo_license = %lf\n",
       flag_demo, tempo_license);
*/

        ret = setTimeLicense(flag_demo, &tempo_license);
        if(ret == -1)
           exit(1);
/*
printf("mmi main DEBUG: flag_demo = %d\ttempo_license = %lf\n",
       flag_demo, tempo_license);
*/
	nome_file_context=(char*)XtMalloc(strlen("Context.ctx")+1);
        strcpy(nome_file_context,"Context.ctx");
        read_argv(argc,argv);
	printf("Apro il file Context %s\n",nome_file_context);
	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = popup_topLevelShellMain();

/*
	UxPopupInterface(mainIface, no_grab);
*/
        
#ifndef DESIGN_TIME

        MainInterface=mainIface;

#endif


	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/
	
  	XtAppMainLoop (UxAppContext);

}
