
/*******************************************************************************
	SelectModelBox.c
	(Generated from interface file SelectModelBox.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/SelectioB.h>
#include <Xm/MainW.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#ifndef DESIGN_TIME
/*
   modulo SelectModelBox.i
   tipo 
   release 1.21
   data 5/12/95
   reserved @(#)SelectModelBox.i	1.21
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include "legomain.h"
#include "lc_errore.h"
extern int model_filter(char *);
extern char path_legocad[];
Boolean attesa_oknew;
char *strret;

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
	Widget	UxSelectModelBox;
	Widget	UxSelectModel;
	Widget	Uxmenu1;
	Widget	Uxmenu1_p3;
	Widget	Uxmenu1_p3_b2;
	Widget	Uxmenu1_p3_b3;
	Widget	Uxmenu1_top_b3;
	Widget	UxWDescr;
	int	Uxtipo_op;
} _UxCSelectModelBox;

#define SelectModelBox          UxSelectModelBoxContext->UxSelectModelBox
#define SelectModel             UxSelectModelBoxContext->UxSelectModel
#define menu1                   UxSelectModelBoxContext->Uxmenu1
#define menu1_p3                UxSelectModelBoxContext->Uxmenu1_p3
#define menu1_p3_b2             UxSelectModelBoxContext->Uxmenu1_p3_b2
#define menu1_p3_b3             UxSelectModelBoxContext->Uxmenu1_p3_b3
#define menu1_top_b3            UxSelectModelBoxContext->Uxmenu1_top_b3
#define WDescr                  UxSelectModelBoxContext->UxWDescr
#define tipo_op                 UxSelectModelBoxContext->Uxtipo_op

static _UxCSelectModelBox	*UxSelectModelBoxContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	SelModelsActivate();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

salva_descrizione()
{
   extern int save_descr();
   char path[200];

   sprintf(path,"%s/%s",path_legocad,UxGetTextString(SelectModel));
   save_descr(path,strret);
}

refresh_lista(swidget w)
{
Arg arg[2];
XmString *modelli;
char **str_modelli;
int num,i;
Widget lista;

#ifndef DESIGN_TIME
_UxCSelectModelBox      *UxSaveCtx, *UxContext;
        UxSaveCtx = UxSelectModelBoxContext;
        UxSelectModelBoxContext = UxContext =
                        (_UxCSelectModelBox *) UxGetContext( w );
#endif

lista = XmFileSelectionBoxGetChild( SelectModel ,XmDIALOG_LIST);


XmListDeleteAllItems( lista );

printf("prima di genera ...\n");

genera_lista_entry(path_legocad, model_filter, &num, &str_modelli );

printf("dopo di genera ...\n");

modelli=(XmString *)malloc(num * sizeof(XmString));
for(i=0;i<num;i++)
	{
	modelli[i]=XmStringCreateSimple(str_modelli[i]);
	free(str_modelli[i]);
	}
free(str_modelli);
XtSetArg(arg[0], XmNlistItems, modelli);
XtSetArg(arg[1], XmNlistItemCount, num);
XtSetValues( SelectModel, arg, 2);
for(i=0;i<num;i++)
	{
	//XtFree(modelli[i]);
	XmStringFree(modelli[i]);
	}

#ifndef DESIGN_TIME
   UxSelectModelBoxContext = UxSaveCtx;
#endif
}

char  *get_select_model_new()
{    
   return(UxGetTextString( SelectModel ));
}

modifi()
{
}

/*** Boolean predproc(display, event, arg)
 ***
 ***    Descrizione:
 ***       Accetta qualsiasi tipo di evento.
 ***/
Boolean predproc(display, event, arg)
Display *display;
XEvent *event;
char *arg;
{
    return(True);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	helpCB_SelectModel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSelectModelBox      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectModelBoxContext;
	UxSelectModelBoxContext = UxContext =
			(_UxCSelectModelBox *) UxGetContext( UxWidget );
	{
	extern char path_legocad[];
	extern char path_modello[];
	char app[100];
	char comando[200];
	
	strcpy( app, path_legocad );
	strcat( app, "/" );
	strcat( app, UxGetTextString( SelectModel ) );
	
	/* chiamo adesso info passandogli la path del modello */
	
	strcpy(comando,"linfo ");
	strcat(comando,app);
	strcat(comando," 2 0");
	
	printf("invio il comando %s\n",comando);
	
	system(comando);
	
	}
	UxSelectModelBoxContext = UxSaveCtx;
}

static void	okCallback_SelectModel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSelectModelBox      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectModelBoxContext;
	UxSelectModelBoxContext = UxContext =
			(_UxCSelectModelBox *) UxGetContext( UxWidget );
	{
	extern char path_legocad[];
	extern int reset_wsel();
	extern char path_modello[];
	extern swidget labelNomeModello;
	extern swidget create_vis_msg();
	extern char *estr_nome_modello();
	extern char *str_toupper();
	extern int tipo_modello;
	
	
	extern swidget create_editText();
	
	char app[FILENAME_MAX];
	char str2[FILENAME_MAX];
	extern int stato;
	char *nome_modello;
	FILE *fp;
	XEvent evento;
	extern Boolean predproc();
	
	mode_t modo = (S_IRWXG | S_IRWXO | S_IRUSR | S_IWUSR | S_IXUSR);
	
	strcpy( app, path_legocad );
	strcat( app, "/" );
	strcpy(str2,UxGetTextString( SelectModel ) );
	if(strlen(str2)==0)
		return;
	strcat( app, str2 );
	
	/*  Si posiziona nel direttorio selezionato dall'utente
	*/
	
	if(chdir(app) == -1)
	{
	   printf("Creazione directory nuovo modello\n");
	   if(mkdir(app,modo) == -1)
	   {
	        printf("SORRY CANNOT CREATE NEW MODEL\n");
	        return;
	   } 
	   if(chdir(app) == -1) 
	   {
	        printf("SORRY CANNOT CREATE NEW MODEL\n");
	        return;
	   }
	/* creo la interfaccia per le introduzione della descrizione */
	
	   WDescr = create_editText(UxWidget,EDIT_DESCR,NULL,&strret,salva_descrizione);
	   attesa_oknew = True; 
	}
	
	
		
	/* Verifica che il path selezionato sia accettabile per essere considerato
	   direttorio utilizzabile per la costruzione del modello
	*/
	
	if(is_path_modello(app))
	{
	   strcpy(path_modello,app);
	   nome_modello=estr_nome_modello(app);
	   str_toupper(nome_modello);
	   switch(tipo_modello)
		{
		case TIPO_PROCESSO:
		sprintf(str2,"Model name: %s (process)",nome_modello);
		break;
		case TIPO_REGOLAZIONE:
		sprintf(str2,"Model name: %s (control)",nome_modello);
		break;
		case TIPO_MIX:
		sprintf(str2,"Model name: %s (mixed)",nome_modello);
		break;
		}
	
		  
	   UxPutLabelString(labelNomeModello,str2);
	
	/* Il modello e' stato scelto: aggiorna lo stato di abilitazione
	   dei bottoni del pulldown menu
	*/
	
	   stato=SCELTO;
	
	/*  update_pulldown();
	*/
	
	   aggiorna_attivi();
	}
		
	if( attesa_oknew == False)    	
	{
	   reset_wsel();
	   UxDestroyInterface( XtParent(UxWidget) );
	}
	
	
	reset_wsel();
	
	/*
	UxDestroyInterface( XtParent(UxWidget) );
	*/
	}
	UxSelectModelBoxContext = UxSaveCtx;
}

static void	cancelCB_SelectModel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSelectModelBox      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectModelBoxContext;
	UxSelectModelBoxContext = UxContext =
			(_UxCSelectModelBox *) UxGetContext( UxWidget );
	{
	extern int reset_wsel();
	/*::
	if( (WDescr != NULL) )
	{
	   XtDestroyWidget(WDescr);
	   WDescr = NULL;
	}
	::*/
	reset_wsel();
	
	UxDestroyInterface( XtParent(UxWidget) );
	
	}
	UxSelectModelBoxContext = UxSaveCtx;
}

static void	activateCB_menu1_p3_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSelectModelBox      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectModelBoxContext;
	UxSelectModelBoxContext = UxContext =
			(_UxCSelectModelBox *) UxGetContext( UxWidget );
	{
	extern char path_legocad[];
	extern char path_modello[];
	extern swidget create_editText();
	extern swidget create_vis_msg();
	char app[100];
	char comando[200];
	char *appo;
	
	char path[200];
	
	sprintf(path,"%s/%s",path_legocad,UxGetTextString(SelectModel));
	printf("path descrizione %s\n",path);
	if( read_descr(path,&appo) == True )
	{
	    create_editText(menu1_p3_b2,EDIT_DESCR,appo,&strret,salva_descrizione );
	}
	else
	   create_vis_msg("Cannot open description file descr.dat!");
	
#ifdef PIPPO
	
	strcpy( app, path_legocad );
	strcat( app, "/" );
	strcat( app, UxGetTextString( SelectModel ) );
	
	/* chiamo adesso info passandogli la path del modello */
	
	strcpy(comando,"linfo ");
	strcat(comando,app);
	strcat(comando," 1 1");
	
	
	system(comando);
#endif
	}
	UxSelectModelBoxContext = UxSaveCtx;
}

static void	activateCB_menu1_p3_b3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSelectModelBox      *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectModelBoxContext;
	UxSelectModelBoxContext = UxContext =
			(_UxCSelectModelBox *) UxGetContext( UxWidget );
	{
	extern swidget create_question_operation();
	swidget wid;
	
	wid = create_question_operation(SelectModel,Q_DELETE_MODEL);
	}
	UxSelectModelBoxContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_SelectModelBox()
{
	Widget	SelectModelBox_shell;
	Widget	menu1_p3_shell;

	SelectModelBox_shell = XtVaCreatePopupShell( "SelectModelBox_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 680,
			XmNy, 250,
			XmNwidth, 330,
			XmNheight, 420,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "SelectModelBox",
			XmNiconName, "SelectModelBox",
			NULL );

	SelectModelBox = XtVaCreateManagedWidget( "SelectModelBox",
			xmMainWindowWidgetClass, SelectModelBox_shell,
			XmNheight, 420,
			XmNwidth, 330,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( SelectModelBox, (char *) UxSelectModelBoxContext );

	SelectModel = XtVaCreateManagedWidget( "SelectModel",
			xmSelectionBoxWidgetClass, SelectModelBox,
			RES_CONVERT( XmNhelpLabelString, "View" ),
			RES_CONVERT( XmNselectionLabelString, "Selected Model" ),
			RES_CONVERT( XmNlistLabelString, "Models" ),
			NULL );

	UxPutContext( SelectModel, (char *) UxSelectModelBoxContext );

	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass, SelectModelBox,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu1, (char *) UxSelectModelBoxContext );

	menu1_p3_shell = XtVaCreatePopupShell ("menu1_p3_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1_p3 = XtVaCreateWidget( "menu1_p3",
			xmRowColumnWidgetClass, menu1_p3_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu1_p3, (char *) UxSelectModelBoxContext );

	menu1_p3_b2 = XtVaCreateManagedWidget( "menu1_p3_b2",
			xmPushButtonWidgetClass, menu1_p3,
			RES_CONVERT( XmNlabelString, "Edit Description" ),
			NULL );

	UxPutContext( menu1_p3_b2, (char *) UxSelectModelBoxContext );

	menu1_p3_b3 = XtVaCreateManagedWidget( "menu1_p3_b3",
			xmPushButtonWidgetClass, menu1_p3,
			RES_CONVERT( XmNlabelString, "Delete Model" ),
			NULL );

	UxPutContext( menu1_p3_b3, (char *) UxSelectModelBoxContext );

	menu1_top_b3 = XtVaCreateManagedWidget( "menu1_top_b3",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, menu1_p3,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu1_top_b3, (char *) UxSelectModelBoxContext );

	XtAddCallback( SelectModelBox, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSelectModelBoxContext );

	XtAddCallback( SelectModel, XmNhelpCallback,
			helpCB_SelectModel,
			(XtPointer) UxSelectModelBoxContext );
	XtAddCallback( SelectModel, XmNokCallback,
			okCallback_SelectModel,
			(XtPointer) UxSelectModelBoxContext );
	XtAddCallback( SelectModel, XmNcancelCallback,
			cancelCB_SelectModel,
			(XtPointer) UxSelectModelBoxContext );

	XtAddCallback( menu1_p3_b2, XmNactivateCallback,
			activateCB_menu1_p3_b2,
			(XtPointer) UxSelectModelBoxContext );

	XtAddCallback( menu1_p3_b3, XmNactivateCallback,
			activateCB_menu1_p3_b3,
			(XtPointer) UxSelectModelBoxContext );


	XmMainWindowSetAreas( SelectModelBox, menu1, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, SelectModel );

	return ( SelectModelBox );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_SelModelsActivate( _Uxtipo_op )
	int	_Uxtipo_op;
{
	Widget                  rtrn;
	_UxCSelectModelBox      *UxContext;

	UxSelectModelBoxContext = UxContext =
		(_UxCSelectModelBox *) XtMalloc( sizeof(_UxCSelectModelBox) );

	tipo_op = _Uxtipo_op;
	{
		Widget wid;
		Arg arg[2];
		XmString *modelli;
		char **str_modelli;
		int num,i;
		
		attesa_oknew = False;
		WDescr =  NULL;
		rtrn = _Uxbuild_SelectModelBox();


printf("prima di genera 2...\n");

		genera_lista_entry(path_legocad, model_filter, &num, &str_modelli );
		

printf("dopo di genera 2...\n");

		modelli=(XmString *)malloc(num * sizeof(XmString));
		for(i=0;i<num;i++)
			{
	printf("CMC2 SelectModelBox %s\n",str_modelli[i]);
			modelli[i]=XmStringCreateSimple(str_modelli[i]);
			free(str_modelli[i]);
			}
		printf("passo\n");
		free(str_modelli);
		printf("passo\n");
		XtSetArg(arg[0], XmNlistItems, modelli);
		XtSetArg(arg[1], XmNlistItemCount, num);
		XtSetValues( SelectModel , arg, 2);
		printf("passo\n");
		for(i=0;i<num;i++)
			{
			//XtFree(modelli[i]);
			XmStringFree(modelli[i]);
			}
		printf("passo\n");
		
		if(tipo_op == SELECT_MODEL)
		{
		   XtUnmanageChild( menu1 );
		}
		else if(tipo_op == REORDER_MODEL)
		{
		   set_something_val( XtParent(SelectModelBox),XmNtitle, (XtArgVal) "DeleteModelBox");
		   wid = XmSelectionBoxGetChild( SelectModel, XmDIALOG_OK_BUTTON );
		   XtUnmanageChild( wid );
		}
		printf("passo\n");
		
		if( proc_legocad_attivi())
		{
		  lc_errore(MODEL_IN_PROGRESS_ERR,"Main");
		  wid = XmSelectionBoxGetChild( SelectModel, XmDIALOG_OK_BUTTON );
		  XtUnmanageChild( wid ); 
		}
		printf("passo\n");
		
		UxPopupInterface(rtrn, no_grab);
		
		printf("esco\n");
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	SelModelsActivate( _Uxtipo_op )
	int	_Uxtipo_op;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_SelModelsActivate( _Uxtipo_op );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

