/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
   modulo autodoc.c
   tipo 
   release 1.10
   data 3/31/95
   reserved @(#)autodoc.c	1.10
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)autodoc.c	1.10\t3/31/95";
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
#include <unistd.h>

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

#include "autodoc.h"
#include "unita.h"
#include "init_unita.h"
#include "fontfallback.h"

Arg args[30];
int nargs;

XmString cstring;

Display *display;
int screen_num;

char nome_modello[9];

char file_tempdati[256], file_tempvar[256];

StructVarianti varianti_modello[5] = {  { "Current", "f14.dat", True },
                                        { "", "", False},
                                        { "", "" ,False},
                                        { "", "" ,False},
                                        { "", "" ,False} };
byte num_varianti = 5;
byte num_file_f14 = 1;

int num_lines_vert = 56, num_lines_horiz = 38;
byte tipo_stampante = PRINTER_ASCII;
char page_prefix_dati[6];
char page_prefix_var[6];

int leggi_file_unita();
int copia_file(char*, char*);

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

 	Widget  create_autodoc_shell();

	

	/*---------------------
	 * Initialize program
	 *--------------------*/

#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "Autodoc",
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

        display = UxDisplay;
        screen_num = UxScreen;
/*
 Se il programma e' stato lanciato con un parametro sulla linea di
 comando esso rappresenta il direttorio del modello
*/
        if(argc >1)
		{
		if(chdir(argv[1]))
			{
			printf("Cannot open model %s", argv[1]);
			exit(1);
			}
		}

     /* Leggi il file delle unita di misura */

        if ( access( FILE_UNITA_MISURA, F_OK) == 0)
           leggi_file_unita();
/***
        tmpnam(file_tempdati);
        tmpnam(file_tempvar);
***/
        strcpy(file_tempdati, "dati.tmp");
        strcpy(file_tempvar, "var.tmp");
     /* Copia i file delle informazioni aggiuntive nei rispettivi file */
     /* temporanei */
        if (access(FILE_INFO_DATI, F_OK) == 0)
           copia_file(FILE_INFO_DATI, file_tempdati);
        else
           fclose(fopen(file_tempdati,"w+")); /* crea il file vuoto */

        if (access(FILE_INFO_VAR, F_OK) == 0)
           copia_file(FILE_INFO_VAR, file_tempvar);
        else
           fclose(fopen(file_tempvar,"w+")); /* crea il file vuoto */

        printf("file temporaneo dati: %s\n", file_tempdati);
        printf("file temporaneo variabili: %s\n", file_tempvar);

	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = create_autodoc_shell();

	UxPopupInterface(mainIface, no_grab);

	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/

  	XtAppMainLoop (UxAppContext);

}
