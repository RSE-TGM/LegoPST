
/*******************************************************************************
       PagFunction.c
       (Generated from interface file PagFunction.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SelectioB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo PagFunction.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)PagFunction.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
#include "config.h"
#include "message.h"
extern swidget topLevelShell;


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
	int	Uxtipo_operaz;
	int	Uxop;
} _UxCPagFunction;

static _UxCPagFunction         *UxPagFunctionContext;
#define tipo_operaz             UxPagFunctionContext->Uxtipo_operaz
#define op                      UxPagFunctionContext->Uxop


Widget	PagFunction;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	creat_PagFunction();
extern Boolean file_exist(char *);

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*
   estrae da fileSpec il nome del file senza path 
   se non ce l'ha ritorna lo stesso puntatore
   il chiamante deve verificare che non sia NULL 
ATTENZIONE AL TOKEN 
*/

char *extractFileName(char *fileSpec) 
{
    int i,len;
    char *NamePointer;
#ifdef VMS
    char token = ']';
    printf("ATTENZIONE cerco il nome del file con separatore ]\n");
#else
    char token = '/';
#endif


    len = strlen(fileSpec);
    
    for(i=len-1;i>=0;i--)
       if(fileSpec[i] == token)
       {
          NamePointer = &(fileSpec[i+1]);
          return(NamePointer);
       }
    return(fileSpec);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_PagFunction( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagFunction         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagFunctionContext;
	UxPagFunctionContext = UxContext =
			(_UxCPagFunction *) UxGetContext( UxWidget );
	{
	extern Arg args[];
	extern Cardinal lnargs;
	extern char *extract_string();
	extern PAGEDIT_CONTEXT *pagedit;
	extern void add_item();
	XmString pagname;
	char filename[MAXLENFILENAME];
	char strpagname[MAXCHAR_PAGNAME];
	char pag_inlist[MAXCHAR_PAGNAME],comando[200];
	int i;
	FILE *fp; 
	Boolean pathSpec=False;
	char *nomePag;
	
	/* IMPORT PAG devo gestire i casi
	   il file indicato deve essere esistente.
	   puo' essere nella directory LEGOMMI_PAG corrente oppure in una altra
	   se e' non e' nella LEGOMMI_PAG ce lo copio dentro se non c'e' n'e'
	      gia' uno con lo stesso nome
	   non puo' essere gia incluso nel Context corrente 
	*/   
	
	if( getenv(ENVPAG) == NULL ) 
	   return;
	
	lnargs=0;
	XtSetArg(args[lnargs],XmNtextString,&pagname);lnargs++;
	XtGetValues(UxWidget,args,lnargs);
	
	/* recupero la pagina da importare */
	strcpy(strpagname,extract_string(pagname));
	
	switch(tipo_operaz)
	{
	   case IMPORT_PAGE:
	      /* verifica se il nome del file contiene la path */ 
	      nomePag = extractFileName(strpagname);
	      if( strcmp(strpagname,nomePag) )
	          pathSpec = True;
	
	      /* viene richiesto l'import da LEGOMMI_PAG */
	      if(!pathSpec)
	      {
	        /* viene richiesto l'import da LEGOMMI_PAG */
	         strcpy(filename,getenv(ENVPAG));
	         strcat(filename,"/"); 
	         strcat(filename,nomePag);
	         strcat(filename,PAG_EXT);
	         if( file_exist(filename))
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); 
	            return;
	         }
	         //fclose(fp);
	      }
	      else
	      {
	         /* viene richiesto l'import da una altra directory */
	         /* se per caso e' la stessa il file viene copiato ugualmente */
	
	         strcpy(filename,strpagname); 
	         strcat(filename,PAG_EXT);
	         //if( ( fp = file_exist(filename)) == NULL)
			 if( !file_exist(filename))
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); 
	            return;
	         }
	     
	         if( system(NULL) == 0)
	         {
	            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Unix Command Shell Not Available",NULL,False,NULL,False,NULL); 
	            return;
	         }
	         else
	         {
	            strcpy(comando,"cp ");
	            strcat(comando,filename);
	            strcat(comando," "); 
	            strcat(comando,getenv(ENVPAG)); 
	            strcat(comando,"/"); 
	            strcat(comando,nomePag);
	            strcat(comando,PAG_EXT);
	            system(comando);
	
	            strcpy(filename,strpagname); 
	            strcat(filename,BKG_EXT);
	            strcpy(comando,"cp ");
	            strcat(comando,filename);
	            strcat(comando," "); 
	            strcat(comando,getenv(ENVPAG)); 
	            strcat(comando,"/"); 
	            strcat(comando,nomePag);
	            strcat(comando,BKG_EXT);
	            system(comando);      }
	      }
	  
	      /* se e' gia presente nel context non termino la operazione */
	      for(i=0;i<pagedit->pag_num;i++)
	      {
	         if(!strcmp(pagedit->page_list[i],nomePag))
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Already Exist",NULL,False,NULL,False,NULL); 
	            return;
	         }
	      }
	
	      /* aggiorno la lista delle pagine */      
	      if( (pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1))) == NULL)
	      {
	         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	         return;
	      }
	      if( (pagedit->page_list[pagedit->pag_num] = alloca_memoria(strlen(nomePag)+1,sizeof(char) )) == NULL)
	         return;
	
	      strcpy(pagedit->page_list[pagedit->pag_num],nomePag);
	      pagedit->pag_num++;
	
	      pagname = XmStringCreateSimple(nomePag);
	      add_item(pagname,0);         
	
	
	   break;
	}
	 
	UxDestroyInterface(UxWidget);
	
	}
	UxPagFunctionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_PagFunction()
{
	Widget		_UxParent;


	/* Creation of PagFunction */
	_UxParent = XtVaCreatePopupShell( "PagFunction_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 240,
			XmNy, 375,
			XmNwidth, 540,
			XmNheight, 255,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "PagFunction",
			NULL );

	PagFunction = XtVaCreateWidget( "PagFunction",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNdialogType, XmDIALOG_PROMPT,
			XmNunitType, XmPIXELS,
			XmNwidth, 540,
			XmNheight, 255,
			XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL,
			XmNtextColumns, 20,
			RES_CONVERT( XmNselectionLabelString, "Page Name" ),
			RES_CONVERT( XmNtextString, "" ),
			RES_CONVERT( XmNapplyLabelString, "" ),
			NULL );
	XtAddCallback( PagFunction, XmNokCallback,
		(XtCallbackProc) okCallback_PagFunction,
		(XtPointer) UxPagFunctionContext );

	UxPutContext( PagFunction, (char *) UxPagFunctionContext );


	XtAddCallback( PagFunction, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxPagFunctionContext);


	return ( PagFunction );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	creat_PagFunction( _Uxop )
	int	_Uxop;
{
	Widget                  rtrn;
	_UxCPagFunction         *UxContext;

	UxPagFunctionContext = UxContext =
		(_UxCPagFunction *) UxNewContext( sizeof(_UxCPagFunction), False );

	op = _Uxop;

	rtrn = _Uxbuild_PagFunction();

	tipo_operaz = op;
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

