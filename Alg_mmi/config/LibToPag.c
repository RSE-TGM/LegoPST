
/*******************************************************************************
       LibToPag.c
       (Generated from interface file LibToPag.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include <Xm/List.h>
#include <Xm/SelectioB.h>

#include "UxXt.h"
#include "libutilx.h"

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo LibToPag.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)LibToPag.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
#include <Xm/Xm.h>
#include "config.h"


/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	int	mumble;
} _UxCLibToPag;

static _UxCLibToPag            *UxLibToPagContext;


Widget	LibToPag;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_LibToPag();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	mapCB_LibToPag( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibToPag            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibToPagContext;
	UxLibToPagContext = UxContext =
			(_UxCLibToPag *) UxGetContext( UxWidget );
	{
	
	}
	UxLibToPagContext = UxSaveCtx;
}

static	void	createCB_LibToPag( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibToPag            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibToPagContext;
	UxContext = UxLibToPagContext;
	{
	extern PAGEDIT_CONTEXT *pagedit;
	XmString stringItem;
	Widget Lista;
	int i;
	
	Lista = XmSelectionBoxGetChild(UxGetWidget(LibToPag),XmDIALOG_LIST);
	for(i=0;i<pagedit->iconlib_num;i++)
	{
	   stringItem = XmStringCreateSimple(pagedit->iconlib_list[i]);
	   XmListAddItem(Lista,stringItem,0);
	   XmStringFree(stringItem);
	}
	
	}
	UxLibToPagContext = UxSaveCtx;
}

static	void	okCallback_LibToPag( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibToPag            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibToPagContext;
	UxLibToPagContext = UxContext =
			(_UxCLibToPag *) UxGetContext( UxWidget );
	{
	/*
	   muovo il file con il nome selezionato 
	   dalla directory della libreria alla directory
	   delle pagine. Le directory le ricavo dalle 
	   variabili di enviroment e le estensioni dei file
	   sono prestabilite (librerie .lib e pagine .pag e .bkg)
	*/  
	
	extern PAGEDIT_CONTEXT *pagedit;
	extern char *extract_string();
	extern void add_item();
	XmString Slib;
	Widget text;
	char pathlib[MAXLENFILENAME],pathpag[MAXLENFILENAME];
	char *lib,comando[200],filelib[MAXLENFILENAME],filepag[MAXLENFILENAME];
	int i,j;
	FILE *fp;
	
	/* recupero la il nome della libreria selezionata */
	 
	text = XmSelectionBoxGetChild(UxGetWidget(LibToPag),XmDIALOG_TEXT);
	get_something(text,XmNvalue, (void*) &lib);
	
	/* costriusco i nomi dei file */ 
	if( getenv(ENVPAG) == NULL)
	   return;
	if( getenv(ENVLIB) == NULL)
	   return;
	
	strcpy(pathpag,getenv(ENVPAG));
	strcpy(pathlib,getenv(ENVLIB));
	
	printf("Path delle pagine %s\n",pathpag);
	printf("Path delle librerie %s\n",pathlib);
	
	strcpy(filelib,pathlib);
	strcat(filelib,"/");
	strcat(filelib,lib);
	strcat(filelib,LIB_EXT);
	
	strcpy(filepag,pathpag);
	strcat(filepag,"/");
	strcat(filepag,lib);
	strcat(filepag,PAG_EXT);
	
	printf("file libreria da convertire %s\n",filelib);
	printf("file pagina da convertire %s\n",filepag);
	
	/* effettuo la system */
	strcpy(comando,"mv ");
	strcat(comando,filelib);
	strcat(comando," ");
	strcat(comando,filepag);
	
	if(system(NULL) == 0)
	{
	   printf("Unix Shell Command Interpeter Not Accessible\n");
	   return;
	}
	
	system(comando);
	
	
	/* compatto la lista delle librerie */
	for(i=0;i<pagedit->iconlib_num;i++)
	{
	   if(!strcmp(pagedit->iconlib_list[i],lib))
	   {
	      for(j=i;j<(pagedit->iconlib_num-1);j++)
	      {
	         strcpy(pagedit->iconlib_list[j],pagedit->iconlib_list[j+1]);   
	         strcpy(pagedit->iconlib_label[j],pagedit->iconlib_label[j+1]);   
	      }
	   }
	}      
	
	pagedit->iconlib_num--;
	
	
	/* aggiungo la pagina nelle liste interne */
	if(pagedit->pag_num > 0)
	{
	   pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1));
	
	   if( pagedit->page_list == NULL )
	      return;
	    
	}
	else
	{
	   if( (pagedit->page_list = (char**)alloca_memoria(1,sizeof(char *))) == NULL)
	      return;
	}
	
	if( (pagedit->page_list[pagedit->pag_num] = (char*)alloca_memoria(strlen(lib)+1,sizeof(char))) == NULL) 
	   return;
	
	strcpy(pagedit->page_list[pagedit->pag_num],lib);
	pagedit->pag_num++;
	
	
	/* aggiorno la scrolled list delle pagine */
	
	Slib = XmStringCreateSimple(lib);
	add_item(Slib,0);
	
	}
	UxLibToPagContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_LibToPag()
{
	Widget		_UxParent;


	/* Creation of LibToPag */
	_UxParent = XtVaCreatePopupShell( "LibToPag_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 570,
			XmNy, 220,
			XmNwidth, 340,
			XmNheight, 250,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "LibToPag",
			NULL );

	LibToPag = XtVaCreateWidget( "LibToPag",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 340,
			XmNheight, 250,
			RES_CONVERT( XmNlistLabelString, "Object Library List" ),
			NULL );
	XtAddCallback( LibToPag, XmNmapCallback,
		(XtCallbackProc) mapCB_LibToPag,
		(XtPointer) UxLibToPagContext );
	XtAddCallback( LibToPag, XmNokCallback,
		(XtCallbackProc) okCallback_LibToPag,
		(XtPointer) UxLibToPagContext );

	UxPutContext( LibToPag, (char *) UxLibToPagContext );

	createCB_LibToPag( LibToPag,
			(XtPointer) UxLibToPagContext, (XtPointer) NULL );


	XtAddCallback( LibToPag, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxLibToPagContext);


	return ( LibToPag );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_LibToPag()
{
	Widget                  rtrn;
	_UxCLibToPag            *UxContext;

	UxLibToPagContext = UxContext =
		(_UxCLibToPag *) UxNewContext( sizeof(_UxCLibToPag), False );


	rtrn = _Uxbuild_LibToPag();

	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

