
/*******************************************************************************
	SimulatorSelection.c
	(Generated from interface file SimulatorSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/FileSB.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo SimulatorSelection.i
   tipo 
   release 1.24
   data 8/29/95
   reserved @(#)SimulatorSelection.i	1.24
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include "topsim.h"
#include "lg1_topsim.h"
#include "questionMsgDb.h"

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
	Widget	UxSimulatorSelection;
	Boolean	Uxnew_simulator;
	unsigned char	*Uxsimulator_path;
	TIPO_OP	Uxidoper;
	TIPO_OP	Uxoperaz;
} _UxCSimulatorSelection;

#define SimulatorSelection      UxSimulatorSelectionContext->UxSimulatorSelection
#define new_simulator           UxSimulatorSelectionContext->Uxnew_simulator
#define simulator_path          UxSimulatorSelectionContext->Uxsimulator_path
#define idoper                  UxSimulatorSelectionContext->Uxidoper
#define operaz                  UxSimulatorSelectionContext->Uxoperaz

static _UxCSimulatorSelection	*UxSimulatorSelectionContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_SimulatorSelection();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

void simulatorselection_settitle(swidget thiswid,char *title)
{
   Arg arg[2];
   Cardinal narg;

printf("Setto il titolo %s\n",title);
   narg=0;
   XtSetArg(arg[narg],XmNtitle,title);narg++;
   XtSetValues(XtParent(thiswid),arg,narg);   
}

char *get_selection()
{
   char *simul;
   simul = UxGetTextString(SimulatorSelection);  
   return(simul);
}

Boolean simulator_isnew(char *simpath)
{
   extern Boolean sccsstate;
   FILE *fp=NULL;
   char filename[300]; 
   Boolean new=False;

   switch( idoper )
   {
      case CREASIM:
      case REMOVESIM:
         /* verifico l'esistenza della directory */  
         strcpy(filename,simpath);
      break;
      case OPENSIM:
         /* verifico l'esistenza della directory */  
         if( sccsstate == True )
            strcpy(filename,simpath);
         else
         {  
            /* verifico l'esistenza del file S01 */  
            strcpy(filename,"");
            sprintf(filename,"%s%s",simpath,"/S01");
         }
      break;
   }


/* verifico se si puo' accedere al file in lettura */

   if( (fp = fopen(filename,"r")) != NULL)
      new = False;
   else
      new = True;   

   fclose(fp);
   return(new);
}

void confermata_remove_simulator()
{
   extern Boolean remove_simultor();
   char *path;
   
   printf("Ho ricevuto conferma della rimozione del simulatore %s\n",get_selection());

   path = XtMalloc( strlen(get_selection()) );
   strcpy(path,get_selection());       

   remove_simulator(path);
   XtFree(path);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	helpCB_SimulatorSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulatorSelection  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulatorSelectionContext;
	UxSimulatorSelectionContext = UxContext =
			(_UxCSimulatorSelection *) UxGetContext( UxWidget );
	{
	swidget wid;
	char *pathfile,*filename;
	
	pathfile = get_selection();
	filename = XtMalloc(strlen(pathfile) + 5);
	
	strcpy(filename,pathfile);
	strcat(filename,"/S01");
	
	wid = create_ViewS01( filename );
	UxPopupInterface(wid,no_grab);
	}
	UxSimulatorSelectionContext = UxSaveCtx;
}

static void	destroyCB_SimulatorSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulatorSelection  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulatorSelectionContext;
	UxSimulatorSelectionContext = UxContext =
			(_UxCSimulatorSelection *) UxGetContext( UxWidget );
	{
	extern void ResetSimulatorActive();
	extern void filepane_setsensitive();
	
	ResetSimulatorActive();
	filepane_setsensitive();
	
	
	}
	UxSimulatorSelectionContext = UxSaveCtx;
}

static void	cancelCB_SimulatorSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulatorSelection  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulatorSelectionContext;
	UxSimulatorSelectionContext = UxContext =
			(_UxCSimulatorSelection *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxWidget);
	}
	UxSimulatorSelectionContext = UxSaveCtx;
}

static void	okCallback_SimulatorSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSimulatorSelection  *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSimulatorSelectionContext;
	UxSimulatorSelectionContext = UxContext =
			(_UxCSimulatorSelection *) UxGetContext( UxWidget );
	{
	extern swidget create_questionDialog();
	extern swidget create_messageDialog();
	extern Boolean open_simulator();
	extern Boolean sccsstate;
	extern SIMULATORE *simulatore;
	
	 
	swidget wid;
	
	/* verifico la selezione effettuata  */
	
	simulator_path = get_selection();
	new_simulator = simulator_isnew(simulator_path);
	
	
	printf("Selezione effettuata %s\n",simulator_path);
	printf("operazione richiesta %d\n",idoper);
	
	/* in base alla quale verifico le operazioni da svolgere */
	switch( idoper )
	{
	   case CREASIM:
	      if( !new_simulator )
	      {
	         printf("Operazione di creazione non concessa, il simulatore gia' esiste\n"); 
	         wid = create_messageDialog(UxWidget,idoper);
	         UxPopupInterface(wid,no_grab);
	      }
	      else
	      {
	          /* open di un simulatore */
	          if( create_simulator(simulator_path) == True)
	          {
	             printf("Create del simulatore andata a buon fine\n");
	
	             /* open del simulatore creato */
	             if( open_simulator(simulator_path) == True)
	                printf("Open del simulatore andata a buon fine\n");
	             else
	                printf("Open del simulatore NON andata a buon fine\n");
	          }
	          else
	             printf("Create del simulatore NON andata a buon fine\n");
	
	           UxDestroyInterface(UxWidget);            
	         } 
	   break;
	   case OPENSIM:
	      if( new_simulator )
	      {
	         wid = create_messageDialog(UxWidget,idoper);
	         UxPopupInterface(wid,no_grab);  
	      }
	      else
	      {
	
	/**** dal 27-7-95 la gestione SCCS viene fatta separatamente
	         if(sccsstate && exist_sccsdir(simulator_path) )
	         {
	            wid = create_SCCSGEST(idoper,simulator_path);
	            UxPopupInterface(wid,no_grab);
	         }
	         else
	***/
	         {
	             /* open di un simulatore */
	             if( open_simulator(simulator_path) == True)
	                printf("Open del simulatore andata a buon fine\n");
	             else
	                printf("Open del simulatore NON andata a buon fine\n");
	
	             UxDestroyInterface(UxWidget);            
	         } 
	      }
	   break;
	   case REMOVESIM:
	      if( new_simulator )
	      {
	printf("Tento di rimuovere un simulatore che risulta nuovo\n");
	         wid = create_messageDialog(UxWidget,idoper);
	         UxPopupInterface(wid,no_grab);
	      }
	      else
	      {
	         printf("selezione =%s , simulatore->path=%s\n",simulator_path,simulatore->path);
	         if( strcmp(simulator_path,simulatore->path) )
	         {
	         wid = create_questionDialog(UxWidget,idoper);
	         UxPopupInterface(wid,no_grab);       
	         }
	         else
	         {
	         wid = create_messageDialog(UxWidget,idoper);
	         UxPopupInterface(wid,no_grab);
	         }
	      }
	   break;
	}
	
	
	
	}
	UxSimulatorSelectionContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_SimulatorSelection()
{
	Widget	SimulatorSelection_shell;

	SimulatorSelection_shell = XtVaCreatePopupShell( "SimulatorSelection_shell",
			xmDialogShellWidgetClass, UxTopLevel,
			XmNx, 467,
			XmNy, 295,
			XmNwidth, 532,
			XmNheight, 372,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "SimulatorSelection",
			NULL );

	SimulatorSelection = XtVaCreateWidget( "SimulatorSelection",
			xmFileSelectionBoxWidgetClass, SimulatorSelection_shell,
			XmNwidth, 532,
			RES_CONVERT( XmNselectionLabelString, "Simulator Selected" ),
			RES_CONVERT( XmNhelpLabelString, "View S01" ),
			RES_CONVERT( XmNfileListLabelString, "Simulator" ),
			XmNfileTypeMask, XmFILE_DIRECTORY,
			XmNdialogType, XmDIALOG_QUESTION,
			XmNheight, 372,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( SimulatorSelection, (char *) UxSimulatorSelectionContext );

	XtAddCallback( SimulatorSelection, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSimulatorSelectionContext );

	XtAddCallback( SimulatorSelection, XmNhelpCallback,
			helpCB_SimulatorSelection,
			(XtPointer) UxSimulatorSelectionContext );
	XtAddCallback( SimulatorSelection, XmNdestroyCallback,
			destroyCB_SimulatorSelection,
			(XtPointer) UxSimulatorSelectionContext );
	XtAddCallback( SimulatorSelection, XmNcancelCallback,
			cancelCB_SimulatorSelection,
			(XtPointer) UxSimulatorSelectionContext );
	XtAddCallback( SimulatorSelection, XmNokCallback,
			okCallback_SimulatorSelection,
			(XtPointer) UxSimulatorSelectionContext );



	return ( SimulatorSelection );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_SimulatorSelection( _Uxoperaz )
	TIPO_OP	_Uxoperaz;
{
	Widget                  rtrn;
	_UxCSimulatorSelection  *UxContext;

	UxSimulatorSelectionContext = UxContext =
		(_UxCSimulatorSelection *) XtMalloc( sizeof(_UxCSimulatorSelection) );

	operaz = _Uxoperaz;
	{
		char inidir[STRLONG];
		XmString xstring;
		
		idoper = operaz;
		
		if( getenv("SKED_USER") != NULL)
		   sprintf(inidir,"%s",getenv("SKED_USER"));
		else
		   if( getwd(inidir) == NULL )
		      sprintf(inidir,"%s","./");
		rtrn = _Uxbuild_SimulatorSelection();

		switch(operaz)
		{
		    case OPENSIM:
		       simulatorselection_settitle(SimulatorSelection,"Open Simulator");
		    break;
		    case REMOVESIM:
		       simulatorselection_settitle(SimulatorSelection,"Remove Simulator");
		    break;
		    case CREASIM:
		       simulatorselection_settitle(SimulatorSelection,"Create Simulator");
		    break;
		}
		
		xstring = XmStringCreateSimple(inidir);
		set_something_val(SimulatorSelection,XmNdirectory, (XtArgVal) xstring);
		
		
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_SimulatorSelection( _Uxoperaz )
	TIPO_OP	_Uxoperaz;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_SimulatorSelection( _Uxoperaz );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

