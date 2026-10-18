/**********************************************************************
*
*       C Source:               lg1.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 30 13:59:10 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: lg1.c,2 %  (%full_filespec: 1,csrc,lg1.c,2 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)lg1.c	2.26\t5/12/95";
/*
        Fine sezione per SCCS
*/

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

#include <stdio.h>
#include <stdlib.h>
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
#include <ctype.h>
#include "errore.h"

#include "filtri.h"        /* include per la gestione dei filtri */
#include "fontfallback.h"

#ifndef GRAPHICS_VAR
#define GRAPHICS_VAR
int graphics_on = True;          /* pagina grafica visualizzata True/False */
#endif
char tipo_del_modello;   /* vale P se modello di processo
                            R se modello di regolazione */ 
ERR_LEVEL err_level;
int       changes_in_F01=False;

Display   *display;
Window    root;
GC        gost_win_gc;


char      path_legocad[1024];
char      nome_modello[20];
Widget    topwidget;

extern swidget create_selectModelBox();
extern swidget create_messageDB();
extern swidget create_questionDB();
extern swidget create_instantiationDlg();
extern swidget instantiationDlg;
extern swidget create_BlockSelectionBox();
extern swidget create_EditDescrDialog1();
extern int lg1_init_arg();

/* Function prototypes for missing functions */
int make_pixmap_cursor(Widget, const char*, const char*, const char*);
int create_gost_win_gc();
int lg1_start_modello();

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

 	Widget  create_lg1MainWindow();

	

	/*---------------------
	 * Initialize program
	 *--------------------*/

#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "Topologia",
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

/* il primo argomento e'  la path
   il secondo indica se si vuole la grafica o no
*/

   if(argc < 3)
   {
     printf("\n\nparametri insufficienti -> lg1 'path modello' 'display grafico (0/1)'\n");
     printf("parametro opzionale 'tipo del modello (P/R): processo o regolazione");
     exit(-1);
   }

 
   graphics_on = atoi(argv[2]);

   if( (graphics_on != False) && (graphics_on != True))
   {
     printf("\n\n secondo parametro errato -> lg1 'path modello' 'display grafico (0/1)'\n");
     exit(-1);
   }


   printf("Visualizzazione Grafica %d\n",graphics_on);
   if(argc > 3)  /* sulla linea di comando e' stato specificato
                    se modello di processo o di regolazione */
	tipo_del_modello=argv[3][0];
   else   /* di default il tipo del modello e' settato a
             processo (P)  */
	tipo_del_modello='P';

   if(tipo_del_modello != 'P' && tipo_del_modello!= 'R' &&
      tipo_del_modello != 'p' && tipo_del_modello!= 'r')
	{
	printf("\n\n terzo parametro errato -> lg1 path display_grafico tipo_modello(P/R)");
	exit(-1); 
	}
 
   lg1_init_arg(argv[1]);

	/*-------------------------------------------------------
	 * Insert initialization code for your application here 
	 *------------------------------------------------------*/
        topwidget=UxTopLevel;


        create_selectModelBox();
        create_messageDB();
        create_questionDB();
        create_instantiationDlg(); 
        create_BlockSelectionBox();
        create_EditDescrDialog1();


	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = create_lg1MainWindow();

	UxPopupInterface(mainIface, no_grab);

        make_pixmap_cursor( UxGetWidget(mainIface), "busy", "white", "red");
        make_pixmap_cursor( UxGetWidget(mainIface), "move", "white", "red");

        display = XtDisplay( UxGetWidget(mainIface) );
        root    = RootWindow( display, DefaultScreen(display) );

        create_gost_win_gc();

        lg1_start_modello();


	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/

  	XtAppMainLoop (UxAppContext);

}
