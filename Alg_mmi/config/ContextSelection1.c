
/*******************************************************************************
       ContextSelection1.c
       (Generated from interface file ContextSelection1.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include <Xm/SelectioB.h>
#include <Xm/FileSB.h>

#include "UxXt.h"
#include "libutilx.h"

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo ContextSelection1.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)ContextSelection1.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
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
	int	Uxid_ope;
	int	Uxiop;
} _UxCContextSelection1;

static _UxCContextSelection1   *UxContextSelection1Context;
#define id_ope                  UxContextSelection1Context->Uxid_ope
#define iop                     UxContextSelection1Context->Uxiop


Widget	ContextSelection1;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_ContextSelection1();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*------------------------------------------------
 * GetSelectedContextName
 * 
 * restituisce il nome del context Selezionato
 * che puo' essere conpleto di path o meno
 * che puo' essere di un file esistente oppure no
 *-----------------------------------------------*/ 

Boolean GetSelectedContextName(char *nomeCtxSel)
{
   Widget WidName; 
   char *nomeSel,*appo;
 
   WidName = XmSelectionBoxGetChild(ContextSelection1,XmDIALOG_TEXT);
   get_something(WidName,XmNvalue, (void*) &nomeSel);

   if(nomeSel != NULL)
   {
      strcpy(nomeCtxSel,nomeSel);

#ifdef VMS
      if( (appo = strstr(nomeCtxSel,";")) != NULL)
         *appo = 0;
#endif      
      return(True);
   }
   else
      return(False); 
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_ContextSelection1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCContextSelection1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxContextSelection1Context;
	UxContextSelection1Context = UxContext =
			(_UxCContextSelection1 *) UxGetContext( UxWidget );
	{
	/* 
	   devo gestire la load di un Context
	   prima verifico se il context esiste
	
	   nella LOAD 
	   se esiste if file richiedo di salvare il context attualmente attivo (se c'e')
	   poi faccio la load vera e propria.
	   se non esiste richiedo di salvare il context attualmente attivo (se c'e')
	   e poi richiamo la maschera di definizione (e sara' quindi lei a desidere
	   se effettuare la load o meno).
	
	*/
	
	extern Boolean ContextLoaded;    /* True se c'e' un context caricato */
	extern Boolean ContextModified;  /* True se c'e' un context non salvato */
	extern Boolean  ContextLoad();
	extern int display_resources();
	extern PAGEDIT_CONTEXT *pagedit;
	extern void TopMenuSetInsensitive();
	extern void TopMenuSetSensitive();
	extern void richiedi_change_database();
	extern swidget create_messageBoxDialog1();
	extern swidget creat_DefineContext();
	
	
	char ContextName[MAXLENFILENAME];
	Boolean ContextNuovo = False;
	Widget Intf;
	
	if(id_ope == 3131)
	{
	   printf("Operazione non ancora abilitata\n");
	}
	
	if( GetSelectedContextName(ContextName) == False )
	{
	   printf("Nessun Context Selezionato, non facio nulla\n"); 
	   return;
	}
	
	if( fopen(ContextName,"r") == NULL)
	   ContextNuovo = True;
	else
	   ContextNuovo = False;
	
	printf("Context Selezionato %s e' nuovo? %d\n",ContextName,ContextNuovo);
	
	
	if(ContextNuovo)
	{
	   if(ContextLoaded && ContextModified)
	   {
	   printf("Vado a definire un nuovo Context salvando il precedente\n ");
	             /* vuoi salvare il Context attivo ? 
	               chiama la maschera di save con parametro SAVE_AND_LOAD_NEWCTX 
	               la quale poi su Ok salva il vecchio context
	               chiama la maschera per definire il nuovo context
	               ed effettua la load del context e setta la variabile globale
	               ContextLoaded = True;
	             */
	      Intf=create_messageBoxDialog1(SAVE_AND_LOAD_NEWCTX,"Do you want save Context?",ContextName);   
	      UxPopupInterface(Intf,no_grab);
	              
	   }
	   else
	   {
	    printf("Vado a definire un nuovo Context senza salvare il precedente\n ");
	            /* chiama la maschera di definizione del context che poi 
	                su Ok effettua la LOAD load del nuovo context  e 
	                setta le variabili globali ContextLoaded = True
	                ContextModified = True
	              */
	      UxPopupInterface( creat_DefineContext(ContextName,DEF_CONTEXT), no_grab);
	   }
	}
	else /* load di un vecchio context */
	{
	   if(ContextLoaded && ContextModified)
	   {                
	             /* vuoi salvare il Context attivo ? 
	               chiama la maschera di save con parametro SAVE_AND_LOAD_OLDCTX
	               la quale poi su Ok salva il vecchio context
	               ed effettua la load del context e setta la variabile globale
	               ContextLoaded = True;
	             */ 
	      Intf=create_messageBoxDialog1(SAVE_AND_LOAD_OLDCTX,"Do you want save previous Context?",ContextName);   
	      UxPopupInterface(Intf,no_grab);
	   }
	   else
	   {
	      /* effettua la load del context e setta la variabile globale
	         ContextLoaded = True;
	      */
	      if(ContextLoad(ContextName,&pagedit) ) 
	      {     
	         display_resources(pagedit); 
	         TopMenuSetSensitive();
	         richiedi_change_database(); 
	         UxDestroyInterface(UxWidget);
	      }
	      else
	      {
	         clear_topLevel(); 
	      } 
	    } 
	   
	}         
	
	
	}
	UxContextSelection1Context = UxSaveCtx;
}

static	void	cancelCB_ContextSelection1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCContextSelection1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxContextSelection1Context;
	UxContextSelection1Context = UxContext =
			(_UxCContextSelection1 *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxWidget);
	}
	UxContextSelection1Context = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ContextSelection1()
{
	Widget		_UxParent;


	/* Creation of ContextSelection1 */
	_UxParent = XtVaCreatePopupShell( "ContextSelection1_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 669,
			XmNy, 65,
			XmNwidth, 304,
			XmNheight, 429,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "ContextSelection1",
			XmNiconName, "ContextSelection1",
			NULL );

	ContextSelection1 = XtVaCreateManagedWidget( "ContextSelection1",
			xmFileSelectionBoxWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 304,
			XmNheight, 429,
			RES_CONVERT( XmNpattern, "*.ctx" ),
			NULL );
	XtAddCallback( ContextSelection1, XmNokCallback,
		(XtCallbackProc) okCallback_ContextSelection1,
		(XtPointer) UxContextSelection1Context );
	XtAddCallback( ContextSelection1, XmNcancelCallback,
		(XtCallbackProc) cancelCB_ContextSelection1,
		(XtPointer) UxContextSelection1Context );

	UxPutContext( ContextSelection1, (char *) UxContextSelection1Context );


	XtAddCallback( ContextSelection1, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxContextSelection1Context);


	return ( ContextSelection1 );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_ContextSelection1( _Uxiop )
	int	_Uxiop;
{
	Widget                  rtrn;
	_UxCContextSelection1   *UxContext;

	UxContextSelection1Context = UxContext =
		(_UxCContextSelection1 *) UxNewContext( sizeof(_UxCContextSelection1), False );

	iop = _Uxiop;

	{
		id_ope = iop;
		rtrn = _Uxbuild_ContextSelection1();

		/* queste righe servono ad individuare un path iniziale 
		   che corrisponde a quella dell'ultimo context utilizzato
		*/
		
		if( getenv(ENVCTX) != NULL)
		   set_something(ContextSelection1,XmNdirSpec,(void*) getenv(ENVCTX));
		if(iop = 3131)
		   set_something(ContextSelection1,XmNdirSpec,(void*) getenv(ENVICO));
		
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

