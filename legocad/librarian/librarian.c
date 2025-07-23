#ifndef DESIGN_TIME
/*
   modulo librarian.prj
   tipo 
   release 2.25
   data 1/18/96
   reserved @(#)librarian.prj	2.25
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)librarian.prj	2.25\t1/18/96";

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
#include <stdio.h>
#include <unistd.h>
#include <lc_errore.h>

#include <libutilx.h>
#include <file_icone.h>

#include "definizioni.h"
XtAppContext	UxAppContext;
Widget		UxTopLevel;
Display		*UxDisplay;
int		UxScreen;

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/

extern swidget mainWindow1;

Widget UxTopLevel;

Display *display;
int screen;

Arg args[20];
Cardinal argcount;

/* D E F I N I Z I O N E   D E I   C O L O R I */
char *names[] = { "white", "black" };
int num_colors = 2;
Pixel apix[10];
short color_values[] = { WHITE,BLACK };
Colormap colormap;

ERR_LEVEL err_level;

/********************************************************************/
/* Strutture riguardanti il geometry management delle varie Dialog...
   Specificare, nell'ordine: { DefaultPosition TRUE/FALSE,XmNx,XmNy,
                               XmNwidth,XmNHeight }                 */
Dialog_geometry geom_attention   = { TRUE, NULL, NULL, NULL, 150 };
/********************************************************************/
Widget attention_wdg;

/***************************************************/
/* Struttura di callback per gli widget di conferma*/
/***************************************************/

static Elenco_callback test_env_callback = {
        {"Create",  conf_test_env, 0 },
        {"Copy All",conf_test_env, 1 },
        {"Exit",    conf_test_env, 2 }};
Widget dialog_test_env;

/* Array di compound strings */
XmString cstrings[500], cstring, cstring_null;

char message[256];

/* VARIABILE GLOBALE DEL NOME DEL MODULO SELEZIONATO */
char nome_modulo[5];
char descr_modulo[81];

char *path_legocad, *path_lego, *path_rego;

/******************************************************/ 

main(argc,argv)
	int 	argc;
	char 	*argv[];
{
        FILE *fp;
        char file_libgraf[256], forausbase[256], file_orig[256];
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

 	Widget  create_mainWindow1();

	

	/*---------------------
	 * Initialize program
	 *--------------------*/

  

#ifdef XOPEN_CATALOG
	setlocale(LC_ALL, "");
#endif

  	UxTopLevel = XtAppInitialize(&UxAppContext, "Librarian",
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

        display = XtDisplay (UxTopLevel);

     /* Ambiente LEGOCAD dell'utente */
        path_legocad = (char *) getenv("LEGOCAD_USER");
        if (path_legocad == NULL)
        {
           printf ("WARNING: You must set the environment variable LEGOCAD_USER.\n");

#ifdef VMS
           path_legocad = "[]";
#else
           path_legocad = ".";
#endif
        }
/*
 Path REGOLIB deve puntare alla directory dove sono presenti le librerie
 standard dei moduli LEGO di REGOLAZIONE
*/
        path_rego = (char *) getenv("REGOLIB");
        if (path_rego == NULL)
	{
           printf ("WARNING: You must set the environment variable REGOLIB.\n");
#ifdef VMS
           path_rego = "[]";
#else
           path_rego = ".";
#endif
	}
/*
 Path PROCLIB deve puntare alla directory dove sono presenti le librerie
 standard dei moduli LEGO di PROCESSO
*/
        path_lego = (char *) getenv("PROCLIB");
        if (path_lego == NULL)
	{
           printf ("WARNING: You must set the environment variable PROCLIB.\n");
#ifdef VMS
           path_lego = "[]";
#else
           path_lego = ".";
#endif
	}
     /* Controllo esistenza del file LIBUT/FORAUSBASE.F */
     /* Se non esiste va copiato */

        percorso_libreria(forausbase, LIBUT, LIB_UTENTE);
        strcat(forausbase, "forausbase.f");

        if (access(forausbase,F_OK))
        {
           percorso_libreria(file_orig, LIBUT, LIB_STANDARD);
           strcat(file_orig,"forausbase.f");

           if ( copia_file(file_orig,forausbase) )
           {
              printf ("WARNING: Can't copy file FORAUSBASE.F. Can't continue.\n"
);
/***
              XtCloseDisplay(display);
              exit(1);
***/
           }
        }

     /* inizializzazione di alcune variabili globali */
        init_vars();
     

	/*----------------------------------------------------------------
	 * Create and popup the first window of the interface.  The 	 
	 * return value can be used in the popdown or destroy functions.
         * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will 
         * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE. 
	 *---------------------------------------------------------------*/

	mainIface = create_mainWindow1();

	UxPopupInterface(mainIface, no_grab);
     /* CONTROLLO DELL'AMBIENTE LEGOCAD */
        if (test_environment() != 0)
        {
           strcpy(message, "The LEGOCAD environment is not correct\n");
           strcat(message, "What do you do?");
           dialog_test_env = (Widget) conferma (UxTopLevel, message,
                                                test_env_callback);
           XtManageChild (dialog_test_env);
        }
        else
        {
           fprintf(stdout,"END OF TEST -> OK!\n");

        /* Aggiornamento file icone grafiche se il file lista_moduli.dat e' */
        /* piu' recente di lista_moduli.dat */
           fp = apri_file_icoproc(CREA_TRUE, LIB_UTENTE);
           fclose(fp);
        }

        set_something_val (UxTopLevel,XmNtitle, (XtArgVal) "LEGO MODULES EDITOR");

        /* Disinnesco dei menu finche' non si sceglie un modulo */
        menu_modulo_non_attivo();

        /* Creazione finestra di warning */
        attention_wdg = (Widget) attention(UxTopLevel,"",CREA,geom_attention);

        /*  Recupera la Colormap e allocca i colori di fg e bg  */
        get_something(UxGetWidget(mainWindow1),XmNcolormap,(void*) &colormap);
        get_pixel(names,apix,num_colors);
    

	/*-----------------------
	 * Enter the event loop 
	 *----------------------*/

  	XtAppMainLoop (UxAppContext);

}
