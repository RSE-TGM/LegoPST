
/*******************************************************************************
       messageBoxDialog1.c
       (Generated from interface file messageBoxDialog1.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo messageBoxDialog1.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)messageBoxDialog1.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <string.h>
#include <Xm/Xm.h>
#include "config.h"
extern Arg args[];
extern Cardinal lnargs;
extern PAGEDIT_CONTEXT *pagedit;
extern Boolean StateInterfaceMode;


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
	Widget	UxmessageBoxDialog1;
	unsigned char	Uxparametro[150];
	int	Uxid_op;
	unsigned char	*Uxmessage;
	unsigned char	*Uxparam;
} _UxCmessageBoxDialog1;

static _UxCmessageBoxDialog1   *UxMessageBoxDialog1Context;
#define messageBoxDialog1       UxMessageBoxDialog1Context->UxmessageBoxDialog1
#define parametro               UxMessageBoxDialog1Context->Uxparametro
#define id_op                   UxMessageBoxDialog1Context->Uxid_op
#define message                 UxMessageBoxDialog1Context->Uxmessage
#define param                   UxMessageBoxDialog1Context->Uxparam



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_messageBoxDialog1();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	cancelCB_messageBoxDialog1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmessageBoxDialog1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageBoxDialog1Context;
	UxMessageBoxDialog1Context = UxContext =
			(_UxCmessageBoxDialog1 *) UxGetContext( UxWidget );
	{
	extern FILE *fpLogMsg;
	
	switch(id_op)
	{
	   case EXIT_WITH_SAVE:
	      fclose(fpLogMsg);
	      exit(0);
	   break;
	}
	}
	UxMessageBoxDialog1Context = UxSaveCtx;
}

static	void	okCallback_messageBoxDialog1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmessageBoxDialog1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageBoxDialog1Context;
	UxMessageBoxDialog1Context = UxContext =
			(_UxCmessageBoxDialog1 *) UxGetContext( UxWidget );
	{
	{
#ifndef DESIGN_TIME
	   extern void save_conn();
	   extern void destroy_conn();  
#endif
	   extern swidget creat_DefineContext();
	   extern void close_page();
	   extern int new_save_context();
	   extern Boolean chiudi_simul;
	   extern void change_database(); 
	   extern void richiedi_change_database(); 
	   extern FILE *fpLogMsg;
	   extern swidget ContextSelection1;
	   extern Boolean AlignResPageContext();
	   extern void PrintAllPages();
	
	   switch(id_op)
	   {
	      case SAVE_AND_LOAD_OLDCTX:
	         printf("richiedo il save del context\n se non viene accordato non faccio nienete\n");
	         printf("se viene accordato chiamo la LOAD_CONTEXT \n");                 
	   
	         if( new_save_context(pagedit) == False)
	         {
	            printf("Non riesco a salvare la situazione\n");
	            printf("provare a copiare il file Context precedente\n");
	            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");
	         }   
	         else
	         {
	            printf("\n\n\tTento di caricare il Context %s\n",parametro);
	            if( ContextLoad(parametro,&pagedit))
	            {     
	               printf("Load del Context %s\n",parametro);
	               display_resources(pagedit); 
	               richiedi_change_database();
	               UxDestroyInterface(UxWidget);
	               UxDestroyInterface(ContextSelection1);
	            }  
	            else          
	               printf("Non sono riuscito a caricare il Context %s\n",parametro);
	         }
	      break;
	      case SAVE_AND_LOAD_NEWCTX:
	         printf("richiedo il save del context\n se non viene accordato non faccio nienete\n");
	         printf("se viene accordato chiamo la LOAD_CONTEXT \n");                 
	   
	         if( new_save_context(pagedit) == False)
	         {
	            printf("Non riesco a salvare la situazione\n");
	            printf("provare a copiare il file Context precedente\n");
	            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");
	         }  
	         else
	         { 
	             UxPopupInterface( creat_DefineContext(parametro,DEF_CONTEXT), no_grab);
	         }
	      break;
	      case ALIGN_CONTEXT:
		 printf("Allineamento delle risorse presenti nel context alle corrispondenti nei files .pag\n");
		 AlignResPageContext();
	      break; 
	
	      case PRINT_ALL:
		 PrintAllPages();
	      break;
	
	      case EXIT_WITH_SAVE:
	         if( StateInterfaceMode )
	         {
	             save_conn();
	             destroy_conn();
	         }
	         else if( new_save_context((PAGEDIT_CONTEXT *)param) == False)
	         {
	            printf("Non riesco a salvare la situazione\n");
	            printf("provare a copiare il file Context precedente\n");
	            printf("ed aggiornarlo in seguito manualmente, Good Luck\n");
	         }   
	         fclose(fpLogMsg);
	         exit(0);
	      break;
	      case EXIT_NO_CHANGE:
	         if( StateInterfaceMode )
	             destroy_conn();
	         fclose(fpLogMsg);
	         exit(0);
	      break;
	      case DELETE_PAGE:
	         do_cancella(param,TIPO_PAGINA);	
	      break;
	      case DELETE_LIBRARY:
	         do_cancella(param,TIPO_LIBRARY);
	      break;
	      case CLOSE_PAGE:
	         close_page(param);
	      break;
	       case KILL_SIMULATOR:
	         change_database();
	      break;
	  }
	}
	
	}
	UxMessageBoxDialog1Context = UxSaveCtx;
}

static	void	helpCB_messageBoxDialog1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCmessageBoxDialog1   *UxSaveCtx, *UxContext;

	UxSaveCtx = UxMessageBoxDialog1Context;
	UxMessageBoxDialog1Context = UxContext =
			(_UxCmessageBoxDialog1 *) UxGetContext( UxWidget );
	{
	switch(id_op)
	{
	   case EXIT_WITH_SAVE:
	      UxDestroyInterface(messageBoxDialog1);
	   break;
	}
	}
	UxMessageBoxDialog1Context = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_messageBoxDialog1()
{
	Widget		_UxParent;


	/* Creation of messageBoxDialog1 */
	_UxParent = XtVaCreatePopupShell( "messageBoxDialog1_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 350,
			XmNy, 300,
			XmNwidth, 550,
			XmNheight, 260,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "messageBoxDialog1",
			NULL );

	messageBoxDialog1 = XtVaCreateWidget( "messageBoxDialog1",
			xmMessageBoxWidgetClass,
			_UxParent,
			XmNunitType, XmPIXELS,
			XmNwidth, 550,
			XmNheight, 260,
			XmNallowOverlap, FALSE,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			RES_CONVERT( XmNdialogTitle, "Message" ),
			XmNdialogType, XmDIALOG_QUESTION,
			XmNmessageAlignment, XmALIGNMENT_CENTER,
			XmNnoResize, TRUE,
			NULL );
	XtAddCallback( messageBoxDialog1, XmNcancelCallback,
		(XtCallbackProc) cancelCB_messageBoxDialog1,
		(XtPointer) UxMessageBoxDialog1Context );
	XtAddCallback( messageBoxDialog1, XmNokCallback,
		(XtCallbackProc) okCallback_messageBoxDialog1,
		(XtPointer) UxMessageBoxDialog1Context );
	XtAddCallback( messageBoxDialog1, XmNhelpCallback,
		(XtCallbackProc) helpCB_messageBoxDialog1,
		(XtPointer) UxMessageBoxDialog1Context );

	UxPutContext( messageBoxDialog1, (char *) UxMessageBoxDialog1Context );


	XtAddCallback( messageBoxDialog1, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxMessageBoxDialog1Context);


	return ( messageBoxDialog1 );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_messageBoxDialog1( _Uxid_op, _Uxmessage, _Uxparam )
	int	_Uxid_op;
	unsigned char	*_Uxmessage;
	unsigned char	*_Uxparam;
{
	Widget                  rtrn;
	_UxCmessageBoxDialog1   *UxContext;

	UxMessageBoxDialog1Context = UxContext =
		(_UxCmessageBoxDialog1 *) UxNewContext( sizeof(_UxCmessageBoxDialog1), False );

	id_op = _Uxid_op;
	message = _Uxmessage;
	param = _Uxparam;

	{
		/*
		  Questa interfaccia gestisce le operazioni di 
		  EXIT_WITH_SAVE, EXIT_NO_CHANGE, DELETE_PAGE, DELETE_LIBRARY,
		  CLOSE_PAGE, KILL_SIMULATOR
		
		  riceve in input l'ID dell'operazione, il messaggio 
		  da visualizzare e un parametro char* che puo'
		  essere in base all'ID dell'operazione il puntatore
		  al context o ad una pagina.   
		*/
		XmString Msg,Msg1,Msg2,Msg3;
		char *intern_msg;
		int num_elem;
		
		if(param != NULL)
		   strcpy(parametro,param);
		rtrn = _Uxbuild_messageBoxDialog1();

		if( param != NULL)
		   num_elem = strlen(message)+strlen(param) +2 ;
		else
		   num_elem = strlen(message)+2 ;
		
		if(num_elem <= 0)
		   return(NULL);
		
		if((intern_msg = (char *)alloca_memoria(  num_elem ,sizeof(char))) == NULL)
		   return(NULL);
		  
		
		if(param != NULL)
		   sprintf(intern_msg,message,param); 
		else
		   strcpy(intern_msg,message);
		
		Msg = XmStringCreateSimple(intern_msg);
		
		lnargs=0;
		XtSetArg(args[lnargs],XmNmessageString,Msg);lnargs++;
		
		if( (id_op == EXIT_WITH_SAVE) || (id_op == KILL_SIMULATOR) ||
		    (id_op == SAVE_AND_LOAD_NEWCTX) || 
		    (id_op == SAVE_AND_LOAD_OLDCTX) || (id_op == EXIT_NO_CHANGE) ||
		    (id_op == ALIGN_CONTEXT)  )
		{
		   Msg1 = XmStringCreateSimple("Yes");
		   XtSetArg(args[lnargs],XmNokLabelString,Msg1);lnargs++;
		   Msg2 = XmStringCreateSimple("No");
		   XtSetArg(args[lnargs],XmNcancelLabelString,Msg2);lnargs++;
		   Msg3 = XmStringCreateSimple("Cancel");
		   XtSetArg(args[lnargs],XmNhelpLabelString,Msg3);lnargs++;
		}
		XtSetValues(rtrn,args,lnargs);
		
		XmStringFree(Msg);
		libera_memoria((char*)intern_msg);
		if( (id_op == EXIT_WITH_SAVE) || (id_op == KILL_SIMULATOR) ||
		    (id_op == SAVE_AND_LOAD_NEWCTX) ||
		    (id_op == SAVE_AND_LOAD_OLDCTX) || (id_op == EXIT_NO_CHANGE) ||
		    (id_op == ALIGN_CONTEXT)  )
		{
		   XmStringFree(Msg1);
		   XmStringFree(Msg2);
		   XmStringFree(Msg3);
		}
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

