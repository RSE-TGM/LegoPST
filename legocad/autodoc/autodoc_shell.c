
/*******************************************************************************
	autodoc_shell.c
	(Generated from interface file autodoc_shell.i)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SeparatoG.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

#include "libutilx.h"

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo autodoc_shell.i
   tipo 
   release 1.12
   data 3/31/95
   reserved @(#)autodoc_shell.i	1.12
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include "autodoc.h"
#include "noto_xbm.h"
#include "UxXt.h"
#include "fontfallback.h"

void lcDestroySwidget(Widget);
void ordina_blocchi(ModelBlockStruct *, int);

 
/************************************************************************/
/* VARIABILI GLOBALI ESTERNE						*/
/************************************************************************/
extern Display *display;
extern int screen_num;

extern char nome_modello[];

extern char page_prefix_var[], page_prefix_dati[];
extern int num_lines_vert, num_lines_horiz;

extern char file_tempdati[], file_tempvar[];
#ifndef DESIGN_TIME
extern byte num_file_f14;
#endif
/************************************************************************/
/* VARIABILI GLOBALI							*/
/************************************************************************/
char *font = "-Adobe-Courier-Bold-R-Normal--14-140-*";
XFontStruct *finfo1;
XmFontList fontList;

Pixmap pixm_noto;

Colormap cmap;
Pixel color_bg_label, color_select_label;

Boolean open_window_unita = False;
Boolean open_window_models = False;
Boolean open_print_config = False;
Boolean open_note_modello = False;
#ifndef DESIGN_TIME
WindowInfoStruct *winfo_list;
#endif

ModelBlockStruct *lista_blocchi;
int num_blocchi;

char *note_modello = NULL;

void copy_n_car(char *,char *,int);
int copia_file(char*, char*);


/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_Uxautodoc_shellMenuPost( wgt, client_data, event, ctd )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;
	Boolean		*ctd;
{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, (XButtonPressedEvent *)event );
		XtManageChild( menu );
	}
}

/*******************************************************************************
	The definition of the context structure:
	If you create multiple instances of your interface, the context
	structure ensures that your callbacks use the variables for the
	correct instance.

	For each swidget in the interface, each argument to the Interface
	function, and each variable in the Instance Specific section of the
	Declarations Editor, there is an entry in the context structure.
	and a #define.  The #define makes the variable name refer to the
	corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	Uxautodoc_shell;
	Widget	UxmainWindow1;
	Widget	Uxmenu_principale;
	Widget	Uxmenu_file;
	Widget	Uxmenu_print;
	Widget	Uxpb_print_data;
	Widget	Uxpb_print_var;
	Widget	Uxpb_print_steady;
	Widget	Uxmenu_cs_print;
	Widget	Uxmenu_pb_save;
	Widget	Uxmenu_pb_quit;
	Widget	Uxmenu1_top_b1;
	Widget	Uxmenu_edit;
	Widget	Uxmenu_pb_data;
	Widget	Uxmenu_pb_variables;
	Widget	Uxmenu1_top_b2;
	Widget	Uxmenu_config;
	Widget	Uxmenu_pb_unit;
	Widget	Uxmenu_pb_models;
	Widget	Uxmenu_pb_printconf;
	Widget	Uxmenu1_top_b3;
	Widget	Uxform_mainWindow1;
	Widget	Uxpb_note_modello;
	Widget	UxlabelGadget1;
	Widget	Uxsnome_modello;
	Widget	UxlabelGadget3;
	Widget	UxscrolledWindow1;
	Widget	Uxopt_menu1;
	Widget	Uxopt_labelG1;
	Widget	Uxopt_separatorG1;
	Widget	Uxopt_pb_data;
	Widget	Uxopt_pb_variables;
} _UxCautodoc_shell;

#define autodoc_shell           UxAutodoc_shellContext->Uxautodoc_shell
#define mainWindow1             UxAutodoc_shellContext->UxmainWindow1
#define menu_principale         UxAutodoc_shellContext->Uxmenu_principale
#define menu_file               UxAutodoc_shellContext->Uxmenu_file
#define menu_print              UxAutodoc_shellContext->Uxmenu_print
#define pb_print_data           UxAutodoc_shellContext->Uxpb_print_data
#define pb_print_var            UxAutodoc_shellContext->Uxpb_print_var
#define pb_print_steady         UxAutodoc_shellContext->Uxpb_print_steady
#define menu_cs_print           UxAutodoc_shellContext->Uxmenu_cs_print
#define menu_pb_save            UxAutodoc_shellContext->Uxmenu_pb_save
#define menu_pb_quit            UxAutodoc_shellContext->Uxmenu_pb_quit
#define menu1_top_b1            UxAutodoc_shellContext->Uxmenu1_top_b1
#define menu_edit               UxAutodoc_shellContext->Uxmenu_edit
#define menu_pb_data            UxAutodoc_shellContext->Uxmenu_pb_data
#define menu_pb_variables       UxAutodoc_shellContext->Uxmenu_pb_variables
#define menu1_top_b2            UxAutodoc_shellContext->Uxmenu1_top_b2
#define menu_config             UxAutodoc_shellContext->Uxmenu_config
#define menu_pb_unit            UxAutodoc_shellContext->Uxmenu_pb_unit
#define menu_pb_models          UxAutodoc_shellContext->Uxmenu_pb_models
#define menu_pb_printconf       UxAutodoc_shellContext->Uxmenu_pb_printconf
#define menu1_top_b3            UxAutodoc_shellContext->Uxmenu1_top_b3
#define form_mainWindow1        UxAutodoc_shellContext->Uxform_mainWindow1
#define pb_note_modello         UxAutodoc_shellContext->Uxpb_note_modello
#define labelGadget1            UxAutodoc_shellContext->UxlabelGadget1
#define snome_modello           UxAutodoc_shellContext->Uxsnome_modello
#define labelGadget3            UxAutodoc_shellContext->UxlabelGadget3
#define scrolledWindow1         UxAutodoc_shellContext->UxscrolledWindow1
#define opt_menu1               UxAutodoc_shellContext->Uxopt_menu1
#define opt_labelG1             UxAutodoc_shellContext->Uxopt_labelG1
#define opt_separatorG1         UxAutodoc_shellContext->Uxopt_separatorG1
#define opt_pb_data             UxAutodoc_shellContext->Uxopt_pb_data
#define opt_pb_variables        UxAutodoc_shellContext->Uxopt_pb_variables

static _UxCautodoc_shell	*UxAutodoc_shellContext;

Widget	slista_blocchi;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_autodoc_shell();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*** void blocco_selezionato(w, flag, call_data)
 ***    Parametri:
 ***       Widget w:
 ***       byte flag : indica se VARIABILI o DATI_GEOMETRICI.
 ***       XmAnyCallbackStruct *call_Data : non utilizzato.
 ***
 ***    Descrizione:
 ***       Callback della del menu EDIT (l'utente ha premuto "Data" o 
 ***       "Variables").
 ***/
void blocco_selezionato(w, flag, call_data)
Widget w;
byte flag;
XmAnyCallbackStruct *call_data;
{
   int num_sel;
   XmStringTable items_sel;
   char *str, *nome_blocco, *descr_blocco;
   WindowInfoStruct *winfo;

/* verifica che sia stato selezionato un blocco dalla lista */
   get_something(UxGetWidget(slista_blocchi),XmNselectedItemCount, (void*) &num_sel);
   
   if ( num_sel == 0 )
   {
      fprintf(stderr,"You must select ONE block!\n");
      return;
   }

   get_something(UxGetWidget(slista_blocchi),XmNselectedItems,(void*) &items_sel);

   str = (char *) extract_string(items_sel[0]);
   nome_blocco = (char *) XtCalloc(9,sizeof(char));
   descr_blocco = (char *) XtCalloc(51,sizeof(char));
   copy_n_car(nome_blocco,str,8);
   copy_n_car(descr_blocco,str+14,50);
   printf("blocco selezionato: %s\n", nome_blocco);

   winfo = new_struct_info(flag);

/* Attiva la window delle informazioni aggiuntive */
   winfo->swindow = create_info_ashell(nome_blocco,descr_blocco,flag,winfo);
}

/*** Pixel get_colore( color_string )
 ***    Parametri:
 ***       char *color_string: terna RGB in formaggio HEX.
 ***
 ***    Descrizione
 ***       Alloca il colore.
 ***/
Pixel get_colore( color_string )
char *color_string;
{
   XColor rgb;

   XParseColor(display, cmap, color_string, &rgb);
   if (XAllocColor( display, cmap, &rgb) != 0)
      printf("non riesco ad allocare!\n");
   return(rgb.pixel);
}

/*** Pixmap crea_pixmap( bits, w, h)
 ***    Parametri:
 ***       char *bits : bitmap 
 ***       int w, h : dimensioni della bitmap;
 ***
 ***    Descrizione:
 ***       crea una pixmap dalla bitmap.
 ***/
Pixmap crea_pixmap(bits, w, h)
char *bits;
short w, h;
{
   Pixmap icona;
   Window  win;
   int screen_num;
   Pixel back;

   screen_num = DefaultScreen(display);

   back = get_colore("#4f9f9f");

   icona = XCreatePixmapFromBitmapData( display, 
                        RootWindow(display, screen_num), bits, w, h,
                        BlackPixel(display,screen_num), back,
                        DefaultDepth(display,screen_num));
   return(icona);
}

/*** WindowInfoStruct *new_struct_info(flag)
 ***    Parametri:
 ***       byte flag: tipo della window delle info aggiuntive
 ***                  (DATI_GEOMETRICI, VARIABILI, VARIABILI_N_F14)
 ***
 ***    Descrizione:
 ***       Aggiunge un item in fondo alla lista winfo_list.
 ***/
WindowInfoStruct *new_struct_info(flag)
byte flag;
{
   WindowInfoStruct *winfo, *winfoprec;

   if (winfo_list == NULL)
   {
      winfo_list = (WindowInfoStruct *) malloc(sizeof(WindowInfoStruct));
      winfo = winfo_list;
   }
   else
   {
      for ( winfo = winfo_list; winfo != NULL ; 
            winfoprec = winfo, winfo = winfo->next );
      winfoprec->next = (WindowInfoStruct *) malloc(sizeof(WindowInfoStruct));
      winfo = winfoprec->next;
   }

   winfo->next = NULL;
   winfo->flag = flag;

   return(winfo);
}
#endif

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	destroyCB_autodoc_shell( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	printf("MI HANNO DISTRUTTO!!!\n");
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_pb_print_data( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	create_ashell_print(DOC_DATI);
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_pb_print_var( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	create_ashell_print(DOC_VARIABILI);
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_pb_print_steady( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	create_ashell_print(DOC_STAZIONARI);
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_menu_pb_save( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	FILE *fp_note;
	
	copia_file(file_tempdati, FILE_INFO_DATI);
	copia_file(file_tempvar, FILE_INFO_VAR);
	
	/* salva le note del modello */
	if ((fp_note = fopen(FILE_NOTE_MODELLO, "w+")) != NULL)
	{
	   if (note_modello == NULL)
	      fputs("", fp_note);
	   else
	      fputs(note_modello, fp_note);
	   fclose(fp_note);
	}
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_menu_pb_quit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	UxDestroySwidget(autodoc_shell);
	
	unlink(file_tempdati);
	unlink(file_tempvar);
	
	exit(0);
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_menu_pb_unit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	if ( !open_window_unita )
	   create_window_unita();
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_menu_pb_models( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	if (!open_window_models)
	   create_window_models();
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_menu_pb_printconf( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	if (!open_print_config)
	   create_dialog_print_config();
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

static void	activateCB_pb_note_modello( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCautodoc_shell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxAutodoc_shellContext;
	UxAutodoc_shellContext = UxContext =
			(_UxCautodoc_shell *) UxGetContext( UxWidget );
	{
	if ( !open_note_modello )
	   create_window_notes(&note_modello, NULL, NULL, &open_note_modello);
	}
	UxAutodoc_shellContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_autodoc_shell()
{
	Widget	menu_file_shell;
	Widget	menu_print_shell;
	Widget	menu_edit_shell;
	Widget	menu_config_shell;
	Widget	opt_menu1_shell;

	autodoc_shell = XtVaCreatePopupShell( "autodoc_shell",
			applicationShellWidgetClass, UxTopLevel,
			XmNdefaultFontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtitle, "LEGOCAD MODEL DOCUMENTATION",
			XmNiconName, "MODEL DOCUMENTATION",
			XmNheight, 450,
			XmNwidth, 650,
			XmNy, 150,
			XmNx, 90,
			NULL );

	UxPutContext( autodoc_shell, (char *) UxAutodoc_shellContext );

	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass, autodoc_shell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNheight, 500,
			XmNwidth, 400,
			XmNy, 0,
			XmNx, 0,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( mainWindow1, (char *) UxAutodoc_shellContext );

	menu_principale = XtVaCreateManagedWidget( "menu_principale",
			xmRowColumnWidgetClass, mainWindow1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNspacing, 20,
			XmNshadowThickness, 2,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu_principale, (char *) UxAutodoc_shellContext );

	menu_file_shell = XtVaCreatePopupShell ("menu_file_shell",
			xmMenuShellWidgetClass, menu_principale,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_file = XtVaCreateWidget( "menu_file",
			xmRowColumnWidgetClass, menu_file_shell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			RES_CONVERT( XmNlabelString, "File" ),
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu_file, (char *) UxAutodoc_shellContext );

	menu_print_shell = XtVaCreatePopupShell ("menu_print_shell",
			xmMenuShellWidgetClass, menu_file,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_print = XtVaCreateWidget( "menu_print",
			xmRowColumnWidgetClass, menu_print_shell,
			RES_CONVERT( XmNlabelString, "Print" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu_print, (char *) UxAutodoc_shellContext );

	pb_print_data = XtVaCreateManagedWidget( "pb_print_data",
			xmPushButtonWidgetClass, menu_print,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Data doc." ),
			NULL );

	UxPutContext( pb_print_data, (char *) UxAutodoc_shellContext );

	pb_print_var = XtVaCreateManagedWidget( "pb_print_var",
			xmPushButtonWidgetClass, menu_print,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "V" ),
			RES_CONVERT( XmNlabelString, "Variables doc." ),
			NULL );

	UxPutContext( pb_print_var, (char *) UxAutodoc_shellContext );

	pb_print_steady = XtVaCreateManagedWidget( "pb_print_steady",
			xmPushButtonWidgetClass, menu_print,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNlabelString, "Steady state(s) doc." ),
			NULL );

	UxPutContext( pb_print_steady, (char *) UxAutodoc_shellContext );

	menu_cs_print = XtVaCreateManagedWidget( "menu_cs_print",
			xmCascadeButtonWidgetClass, menu_file,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNsubMenuId, menu_print,
			RES_CONVERT( XmNlabelString, "Print" ),
			NULL );

	UxPutContext( menu_cs_print, (char *) UxAutodoc_shellContext );

	menu_pb_save = XtVaCreateManagedWidget( "menu_pb_save",
			xmPushButtonWidgetClass, menu_file,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Save info" ),
			NULL );

	UxPutContext( menu_pb_save, (char *) UxAutodoc_shellContext );

	menu_pb_quit = XtVaCreateManagedWidget( "menu_pb_quit",
			xmPushButtonWidgetClass, menu_file,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNmnemonic, "Q" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );

	UxPutContext( menu_pb_quit, (char *) UxAutodoc_shellContext );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass, menu_principale,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, menu_file,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxAutodoc_shellContext );

	menu_edit_shell = XtVaCreatePopupShell ("menu_edit_shell",
			xmMenuShellWidgetClass, menu_principale,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_edit = XtVaCreateWidget( "menu_edit",
			xmRowColumnWidgetClass, menu_edit_shell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu_edit, (char *) UxAutodoc_shellContext );

	menu_pb_data = XtVaCreateManagedWidget( "menu_pb_data",
			xmPushButtonWidgetClass, menu_edit,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNlabelString, "Data" ),
			NULL );

	UxPutContext( menu_pb_data, (char *) UxAutodoc_shellContext );

	menu_pb_variables = XtVaCreateManagedWidget( "menu_pb_variables",
			xmPushButtonWidgetClass, menu_edit,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNmnemonic, "V" ),
			RES_CONVERT( XmNlabelString, "Variables" ),
			NULL );

	UxPutContext( menu_pb_variables, (char *) UxAutodoc_shellContext );

	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonWidgetClass, menu_principale,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			XmNsubMenuId, menu_edit,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu1_top_b2, (char *) UxAutodoc_shellContext );

	menu_config_shell = XtVaCreatePopupShell ("menu_config_shell",
			xmMenuShellWidgetClass, menu_principale,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_config = XtVaCreateWidget( "menu_config",
			xmRowColumnWidgetClass, menu_config_shell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Config" ),
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu_config, (char *) UxAutodoc_shellContext );

	menu_pb_unit = XtVaCreateManagedWidget( "menu_pb_unit",
			xmPushButtonWidgetClass, menu_config,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			RES_CONVERT( XmNlabelString, "Unit ..." ),
			NULL );

	UxPutContext( menu_pb_unit, (char *) UxAutodoc_shellContext );

	menu_pb_models = XtVaCreateManagedWidget( "menu_pb_models",
			xmPushButtonWidgetClass, menu_config,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "M" ),
			RES_CONVERT( XmNlabelString, "Steady state version" ),
			NULL );

	UxPutContext( menu_pb_models, (char *) UxAutodoc_shellContext );

	menu_pb_printconf = XtVaCreateManagedWidget( "menu_pb_printconf",
			xmPushButtonWidgetClass, menu_config,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1" ),
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "P" ),
			RES_CONVERT( XmNlabelString, "Print ..." ),
			NULL );

	UxPutContext( menu_pb_printconf, (char *) UxAutodoc_shellContext );

	menu1_top_b3 = XtVaCreateManagedWidget( "menu1_top_b3",
			xmCascadeButtonWidgetClass, menu_principale,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			XmNsubMenuId, menu_config,
			RES_CONVERT( XmNlabelString, "Config" ),
			NULL );

	UxPutContext( menu1_top_b3, (char *) UxAutodoc_shellContext );

	form_mainWindow1 = XtVaCreateManagedWidget( "form_mainWindow1",
			xmFormWidgetClass, mainWindow1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			NULL );

	UxPutContext( form_mainWindow1, (char *) UxAutodoc_shellContext );

	pb_note_modello = XtVaCreateManagedWidget( "pb_note_modello",
			xmPushButtonWidgetClass, form_mainWindow1,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNforeground, "WHITE" ),
			RES_CONVERT( XmNbackground, "#4F7F7F" ),
			RES_CONVERT( XmNlabelString, "NOTE" ),
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_NONE,
			XmNheight, 30,
			XmNwidth, 70,
			XmNy, 10,
			XmNx, 10,
			NULL );

	UxPutContext( pb_note_modello, (char *) UxAutodoc_shellContext );

	labelGadget1 = XtVaCreateManagedWidget( "labelGadget1",
			xmLabelGadgetClass, form_mainWindow1,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftWidget, NULL,
			XmNfontList, UxConvertFontList( "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			RES_CONVERT( XmNlabelString, "MODEL NAME: " ),
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 15,
			XmNx, 90,
			NULL );

	UxPutContext( labelGadget1, (char *) UxAutodoc_shellContext );

	snome_modello = XtVaCreateManagedWidget( "snome_modello",
			xmLabelGadgetClass, form_mainWindow1,
			XmNrightWidget, pb_note_modello,
			XmNtopOffset, 15,
			XmNtopAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftWidget, labelGadget1,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "< ... >" ),
			XmNheight, 20,
			XmNwidth, 410,
			XmNy, 10,
			XmNx, 230,
			NULL );

	UxPutContext( snome_modello, (char *) UxAutodoc_shellContext );

	labelGadget3 = XtVaCreateManagedWidget( "labelGadget3",
			xmLabelGadgetClass, form_mainWindow1,
			XmNfontList, UxConvertFontList( "" ),
			RES_CONVERT( XmNlabelString, "BLOCKS LIST:" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopWidget, labelGadget1,
			XmNtopOffset, 20,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNheight, 20,
			XmNwidth, 120,
			XmNy, 60,
			XmNx, 10,
			NULL );

	UxPutContext( labelGadget3, (char *) UxAutodoc_shellContext );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, form_mainWindow1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNtopWidget, labelGadget3,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 180,
			XmNx, 50,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxAutodoc_shellContext );

	slista_blocchi = XtVaCreateManagedWidget( "slista_blocchi",
			xmListWidgetClass, scrolledWindow1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Courier-Bold-R-Normal--14-140-*" ),
			XmNheight, 230,
			XmNwidth, 570,
			NULL );

	UxPutContext( slista_blocchi, (char *) UxAutodoc_shellContext );

	opt_menu1_shell = XtVaCreatePopupShell ("opt_menu1_shell",
			xmMenuShellWidgetClass, slista_blocchi,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	opt_menu1 = XtVaCreateWidget( "opt_menu1",
			xmRowColumnWidgetClass, opt_menu1_shell,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( opt_menu1, (char *) UxAutodoc_shellContext );

	opt_labelG1 = XtVaCreateManagedWidget( "opt_labelG1",
			xmLabelGadgetClass, opt_menu1,
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNlabelString, "EDIT" ),
			NULL );

	UxPutContext( opt_labelG1, (char *) UxAutodoc_shellContext );

	opt_separatorG1 = XtVaCreateManagedWidget( "opt_separatorG1",
			xmSeparatorGadgetClass, opt_menu1,
			NULL );

	UxPutContext( opt_separatorG1, (char *) UxAutodoc_shellContext );

	opt_pb_data = XtVaCreateManagedWidget( "opt_pb_data",
			xmPushButtonWidgetClass, opt_menu1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNlabelString, "Data" ),
			NULL );

	UxPutContext( opt_pb_data, (char *) UxAutodoc_shellContext );

	opt_pb_variables = XtVaCreateManagedWidget( "opt_pb_variables",
			xmPushButtonWidgetClass, opt_menu1,
			RES_CONVERT( XmNbackground, "#4f9f9f" ),
			XmNfontList, UxConvertFontList( "-Adobe-Helvetica-Bold-R-Normal--14-140-75-75-P-82-ISO8859-1" ),
			RES_CONVERT( XmNlabelString, "Variables" ),
			NULL );

	UxPutContext( opt_pb_variables, (char *) UxAutodoc_shellContext );

	XtAddCallback( autodoc_shell, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( autodoc_shell, XmNdestroyCallback,
			destroyCB_autodoc_shell,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( pb_print_data, XmNactivateCallback,
			activateCB_pb_print_data,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( pb_print_var, XmNactivateCallback,
			activateCB_pb_print_var,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( pb_print_steady, XmNactivateCallback,
			activateCB_pb_print_steady,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( menu_pb_save, XmNactivateCallback,
			activateCB_menu_pb_save,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( menu_pb_quit, XmNactivateCallback,
			activateCB_menu_pb_quit,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( menu_pb_data, XmNactivateCallback,
			blocco_selezionato,
			(XtPointer) DATI_GEOMETRICI );

	XtAddCallback( menu_pb_variables, XmNactivateCallback,
			blocco_selezionato,
			(XtPointer) VARIABILI );

	XtAddCallback( menu_pb_unit, XmNactivateCallback,
			activateCB_menu_pb_unit,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( menu_pb_models, XmNactivateCallback,
			activateCB_menu_pb_models,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( menu_pb_printconf, XmNactivateCallback,
			activateCB_menu_pb_printconf,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( pb_note_modello, XmNactivateCallback,
			activateCB_pb_note_modello,
			(XtPointer) UxAutodoc_shellContext );

	XtAddCallback( opt_pb_data, XmNactivateCallback,
			blocco_selezionato,
			(XtPointer) DATI_GEOMETRICI );

	XtAddCallback( opt_pb_variables, XmNactivateCallback,
			blocco_selezionato,
			(XtPointer) VARIABILI );


	XtAddEventHandler( slista_blocchi, ButtonPressMask,
			False, _Uxautodoc_shellMenuPost, opt_menu1 );
	XmMainWindowSetAreas( mainWindow1, menu_principale, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, form_mainWindow1 );

	return ( autodoc_shell );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_autodoc_shell()
{
	Widget                  rtrn;
	_UxCautodoc_shell       *UxContext;

	UxAutodoc_shellContext = UxContext =
		(_UxCautodoc_shell *) XtMalloc( sizeof(_UxCautodoc_shell) );

	{
		FILE *fp;
		rtrn = _Uxbuild_autodoc_shell();

		/* recupera la colormappazza */
		   cmap = DefaultColormap(display, screen_num);
		
		   color_bg_label = get_colore("#4f8f8f");
		   color_select_label = get_colore("#4fafaf");
		
		/* leggi la configurazione della stampante */
		    if ((fp = fopen(FILE_PRINT_CONFIG, "r")) != NULL)
		   {
		      fscanf(fp, "%d %d %s %s", &num_lines_vert, &num_lines_horiz,
		                                page_prefix_dati, page_prefix_var);
		      fclose(fp);
		   }
		
		/* recupera la nota del modello */
		   if ((fp = fopen(FILE_NOTE_MODELLO, "r")) != NULL)
		   {
		      copia_testo_nota(fp, &note_modello);
		      fclose(fp);
		   }
		
		/* Creazione Font per Text */
		   finfo1 = FontFallbackLoad(display,font);
		   fontList = XmFontListCreate (finfo1,XmSTRING_DEFAULT_CHARSET);
		
		/* recupera il nome del modello dal file f01.dat */
		   setta_nome_modello();
		   UxPutLabelString(snome_modello,nome_modello);
		
		/* crea la pixmap NOTO */
		   pixm_noto = crea_pixmap(noto_bits, noto_width, noto_height,
					   UxGetWidget(autodoc_shell));
		
		/* recupera la lista dei blocchi */
		   get_blocchi_modello( &lista_blocchi, &num_blocchi );
		   ordina_blocchi(lista_blocchi, num_blocchi);
		
		   update_blocks_list(lista_blocchi, num_blocchi, UxGetWidget(slista_blocchi));
		
		   return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_autodoc_shell()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_autodoc_shell();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

