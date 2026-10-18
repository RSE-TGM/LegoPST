/**********************************************************************
*
*       C Source:               dati.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Fri Apr 12 10:37:40 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: dati.c,2 %  (%full_filespec: 1,csrc,dati.c,2 %)";
#endif
#ifndef DESIGN_TIME
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)dati.c	2.15\t4/26/95";
/*
        Fine sezione per SCCS
*/
#endif

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

#ifdef XOPEN_CATALOG
#include <locale.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xm/Xm.h>

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
Boolean graphics_on; /* indica se si desidera la rappresentazione dello
                     schema associato o meno */

#include "dati.h"
#include <stdlib.h>
#include <unistd.h>
#include "fontfallback.h"

/* Missing function declarations */
int read_file_f03(void);
int read_file_f14(int neqsis, int nu);

int stato;
/*
extern long int neqsis,nu;
Micheletti 28-3-95 
*/
extern int neqsis,nu;

Widget topwidget;

int main(int argc, char *argv[])
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

 	Widget  popup_dati_mainWS();

	

	/*---------------------
	 * Initialize program
	 *--------------------*/

#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "Dati",
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
        topwidget=UxTopLevel;
        if(argc >1)
                {
                if(chdir(argv[1]))
                        {
                        printf("Cannot open model %s",argv[1]);
                        exit(1);
                        }
/*
  Legge il file f03 da cui ricava tutte le informazioni relative
  a blocchi e variabili
*/
                if(read_file_f03())
                        {
                        printf("\nCannot open file f03");
                        exit(1);
                        }
/*
  Legge il file f14 da cui ricava i valori delle variabili
*/
                if(read_file_f14(neqsis,nu))
                        {
                        printf("Cannot open file f14");
                        exit(1);
                        }
                stato=SCELTO;
                }
        if(argc > 2) /* e' stato passato il parametro che specifica
                        se si desidera lo schema grafico */
                graphics_on=atoi(argv[2]);
        else
                graphics_on=True;



	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = popup_dati_mainWS();

	UxPopupInterface(mainIface, no_grab);

	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/

  	XtAppMainLoop (UxAppContext);

}
