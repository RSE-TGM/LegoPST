/* @(#)35	1.10  com/config/xtmain.tem, aic, aic322, 9239322 9/30/92 16:31:05 */
/*
 *  COMPONENT_NAME: AIC     AIXwindows Interface Composer
 *  
 *  ORIGINS: 58
 *  
 *  
 *                   Copyright IBM Corporation 1991, 1992
 *  
 *                         All Rights Reserved
 *  
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation, and that the name of IBM not be
 *   used in advertising or publicity pertaining to distribution of the
 *   software without specific, written prior permission.
 *  
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
 * $Date: 92/06/25 11:00:40 $  		$Revision: 1.11.37.1.43.2 $ 
 *-----------------------------------------------------------*/ 
#include <stdio.h>

#ifdef XOPEN_CATALOG
#include <locale.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>
#include <sqlite3.h>


/*---------------------------------------------------- 
 * UxXt.h needs to be included only when compiling a 
 * stand-alone application. 
 *---------------------------------------------------*/
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif /* DESIGN_TIME */

XtAppContext	UxAppContext;
Widget		UxTopLevel;
Display		*UxDisplay;
int		UxScreen;

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/
#ifndef DESIGN_TIME
#include "sim_param.h"
#include "parametri.h"
#include "option.h"
OPTIONS_FLAGS options;
Boolean *IcProt;    /* punta alla struttura protezione IC */

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "bistrutt.h"
#include "cursore.h"
XrmDatabase Xdb; 		/* database delle risorse		*/
int inizializzazione;

RICHIESTA_STAT richiesta_stat;
STATISTICHE    statistiche; 
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"
#include "sked.h"
#endif

#include "preset_messaggi.h"

/* aggiunto a mano 8apr97 */
int    _MAX_SNAP_SHOT;
int    _MAX_BACK_TRACK;
int    _MAX_CAMPIONI;
int    _NUM_VAR;
int    _MAX_PERTUR;
int    _SPARE_SNAP;
int    _PERT_CLEAR;
float  _TIME_BACK_TRACK=120.0;   /* intervallo di default fra 2 BT */
/* fine aggiunte */
                         
char *FILES_PATH;

sqlite3 *db;

int main_init(int,char*[]);
int attiva_timer_mainIface (Widget);


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

 	Widget  create_masterMenu(void);
	

	/*---------------------
	 * Initialize program
	 *--------------------*/
/* aggiunto a mano 8apr97 fm */
int i;
   for(i=0;i<argc-1;i++)
      {
      if(strcmp(argv[i],ARGV_N_SNAP)==0)
                  _MAX_SNAP_SHOT  = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_BKTK)==0)
                  _MAX_BACK_TRACK = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_CAMP_CR)==0)
                  _MAX_CAMPIONI   = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_VAR_CR)==0)
                  _NUM_VAR        = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_PERT_ACTIVE)==0)
                  _MAX_PERTUR     = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_SPARE)==0)
                  _SPARE_SNAP     = atoi(argv[i+1]);
      if(strcmp(argv[i],ARGV_N_PERTCL)==0)
                  _PERT_CLEAR     = atoi(argv[i+1]);
      if(strcmp(argv[i],TIME_N_BKTK)==0)
                  sscanf(argv[i+1],"%f",&(_TIME_BACK_TRACK)) ;
      }
// fprintf (stderr,"SNAP = %d\n",_MAX_SNAP_SHOT);
// fprintf (stderr,"BKTK = %d\n",_MAX_BACK_TRACK);
// fprintf (stderr,"CAMP_CR = %d\n",_MAX_CAMPIONI);
// fprintf (stderr,"VAR_CR = %d\n",_NUM_VAR);
// fprintf (stderr,"PERT = %d\n",_MAX_PERTUR);
// fprintf (stderr,"SPARE = %d\n",_SPARE_SNAP);
// fprintf (stderr,"PERTCL = %d\n",_PERT_CLEAR);
// fprintf (stderr,"TIME_BKTK = %f\n",_TIME_BACK_TRACK);

/* fine aggiunte */
#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "banco",
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
        if (main_init(argc, argv) < 0) {
           system ("killsim");
           exit (1);
        }
	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = create_masterMenu();

	UxPopupInterface(mainIface, no_grab);
    
     /*    connessioni (mainIface);  */   /* aggancia le risorse necessarie e si connette 
                                al dispatcher o al bridge */
	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/
        attiva_timer_mainIface (mainIface);

  	XtAppMainLoop (UxAppContext);

}
