
/*******************************************************************************
	TopSim.c
	(Generated from interface file TopSim.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SeparatoG.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo TopSim.i
   tipo 
   release 1.24
   data 8/29/95
   reserved @(#)TopSim.i	1.24
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <string.h>
#if !defined(LINUX)
#include <sys/mode.h>
#endif
#include <sys/stat.h>
#include <X11/Intrinsic.h> 
#include <Xm/Xm.h>
#include <X11/cursorfont.h> 
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif


#ifndef LIBUTILX
#include <libutilx.h>
#define LIBUTILX
#endif

#include "questionMsgDb.h"
#include "topsim.h"
#include "lg1_topsim.h"

Cursor waitCursor,normalCursor;
Boolean sccsstate=False;

SIMULATORE *simulatore=NULL;
Boolean simulator_changed=False;
swidget SimSelection=NULL;
swidget ModelDefinitionInterface=NULL;

Find_struct parametri_userlist;

Arg argq[15];
Cardinal narg;

/* numero di finestre di connessione aperte */
int fin_conn_open = 0;

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxTopSimMenuPost( wgt, client_data, event, ctd )
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
		XmMenuPosition( menu, event );
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
	Widget	Uxmenu1;
	Widget	UxMainFile;
	Widget	UxMainFileCreate;
	Widget	UxMainFileOpen;
	Widget	UxMainFileRemove;
	Widget	UxMainFileSave;
	Widget	UxMainFileSaveAs;
	Widget	UxMainFileExit;
	Widget	Uxmenu1_top_b1;
	Widget	UxMainEdit;
	Widget	UxMainEditModify;
	Widget	UxMainEditDelete;
	Widget	UxMainEditDeleteConn;
	Widget	UxMainEditAdd;
	Widget	UxMainEditConnect;
	Widget	Uxmenu1_top_b2;
	Widget	UxMainUtilities;
	Widget	UxMainUtilitiesCopy;
	Widget	UxMainUtilitiesView;
	Widget	Uxmenu1_top_b3;
	Widget	UxMainConfigure;
	Widget	UxMainConfigureSCCS;
	Widget	Uxmenu1_top_b4;
	Widget	UxMainCompi;
	Widget	UxMainNetCompi;
	Widget	UxMainView;
	Widget	Uxmenu1_top_b5;
	Widget	UxSCCS;
	Widget	Uxmenu1_p6_b1;
	Widget	Uxmenu1_top_b6;
	Widget	UxpanedWindow2;
	Widget	Uxform2;
	Widget	UxTitolo;
	Widget	UxNomeSimulatore;
	Widget	UxscrolledWindow2;
	Widget	UxListaModelli;
	Widget	UxSCCSStateLabel;
	Widget	UxFindForm;
	Widget	Uxform1;
	Widget	UxscrolledWindow1;
	Widget	UxscrolledText1;
	Widget	Uxmenu2;
	Widget	Uxmenu2_p1_b1;
	Widget	Uxmenu2_p1_b3;
	Widget	Uxmenu2_p1_b2;
	Widget	Uxlabel1;
	swidget	UxConnWind1;
	swidget	UxVarWind1;
	swidget	UxConnWind2;
	swidget	UxVarWind2;
	unsigned char	UxSimulatoreCorrente[100];
} _UxCTopSim;

#define menu1                   UxTopSimContext->Uxmenu1
#define MainFile                UxTopSimContext->UxMainFile
#define MainFileCreate          UxTopSimContext->UxMainFileCreate
#define MainFileOpen            UxTopSimContext->UxMainFileOpen
#define MainFileRemove          UxTopSimContext->UxMainFileRemove
#define MainFileSave            UxTopSimContext->UxMainFileSave
#define MainFileSaveAs          UxTopSimContext->UxMainFileSaveAs
#define MainFileExit            UxTopSimContext->UxMainFileExit
#define menu1_top_b1            UxTopSimContext->Uxmenu1_top_b1
#define MainEdit                UxTopSimContext->UxMainEdit
#define MainEditModify          UxTopSimContext->UxMainEditModify
#define MainEditDelete          UxTopSimContext->UxMainEditDelete
#define MainEditDeleteConn      UxTopSimContext->UxMainEditDeleteConn
#define MainEditAdd             UxTopSimContext->UxMainEditAdd
#define MainEditConnect         UxTopSimContext->UxMainEditConnect
#define menu1_top_b2            UxTopSimContext->Uxmenu1_top_b2
#define MainUtilities           UxTopSimContext->UxMainUtilities
#define MainUtilitiesCopy       UxTopSimContext->UxMainUtilitiesCopy
#define MainUtilitiesView       UxTopSimContext->UxMainUtilitiesView
#define menu1_top_b3            UxTopSimContext->Uxmenu1_top_b3
#define MainConfigure           UxTopSimContext->UxMainConfigure
#define MainConfigureSCCS       UxTopSimContext->UxMainConfigureSCCS
#define menu1_top_b4            UxTopSimContext->Uxmenu1_top_b4
#define MainCompi               UxTopSimContext->UxMainCompi
#define MainNetCompi            UxTopSimContext->UxMainNetCompi
#define MainView                UxTopSimContext->UxMainView
#define menu1_top_b5            UxTopSimContext->Uxmenu1_top_b5
#define SCCS                    UxTopSimContext->UxSCCS
#define menu1_p6_b1             UxTopSimContext->Uxmenu1_p6_b1
#define menu1_top_b6            UxTopSimContext->Uxmenu1_top_b6
#define panedWindow2            UxTopSimContext->UxpanedWindow2
#define form2                   UxTopSimContext->Uxform2
#define Titolo                  UxTopSimContext->UxTitolo
#define NomeSimulatore          UxTopSimContext->UxNomeSimulatore
#define scrolledWindow2         UxTopSimContext->UxscrolledWindow2
#define ListaModelli            UxTopSimContext->UxListaModelli
#define SCCSStateLabel          UxTopSimContext->UxSCCSStateLabel
#define FindForm                UxTopSimContext->UxFindForm
#define form1                   UxTopSimContext->Uxform1
#define scrolledWindow1         UxTopSimContext->UxscrolledWindow1
#define scrolledText1           UxTopSimContext->UxscrolledText1
#define menu2                   UxTopSimContext->Uxmenu2
#define menu2_p1_b1             UxTopSimContext->Uxmenu2_p1_b1
#define menu2_p1_b3             UxTopSimContext->Uxmenu2_p1_b3
#define menu2_p1_b2             UxTopSimContext->Uxmenu2_p1_b2
#define label1                  UxTopSimContext->Uxlabel1
#define ConnWind1               UxTopSimContext->UxConnWind1
#define VarWind1                UxTopSimContext->UxVarWind1
#define ConnWind2               UxTopSimContext->UxConnWind2
#define VarWind2                UxTopSimContext->UxVarWind2
#define SimulatoreCorrente      UxTopSimContext->UxSimulatoreCorrente

static _UxCTopSim	*UxTopSimContext;

Widget	TopSim;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_TopSim();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

int create_sccsS01();
Boolean edit_sccsS01();

int distruggi_interfaccie( swidget wid )
{
    if( wid == ConnWind1 )
    {
       UxDestroyInterface(ConnWind1);
       UxDestroyInterface(VarWind1);
    }
    else if( wid == ConnWind2 )
    {
       UxDestroyInterface(ConnWind2);
       UxDestroyInterface(VarWind2);
    }
    else if( wid == VarWind1 )
    {
       UxDestroyInterface(ConnWind1);
       UxDestroyInterface(VarWind1);
    }
    else if( wid == VarWind2 )
    {
       UxDestroyInterface(ConnWind2);
       UxDestroyInterface(VarWind2);
    }
}

set_wait_cursor()
{
   XDefineCursor(UxDisplay,XtWindow(TopSim),waitCursor);
   XmUpdateDisplay(UxTopLevel);
}

set_normal_cursor()
{
   XDefineCursor(UxDisplay,XtWindow(TopSim),normalCursor);
}

/*** 
   inizializzazione e lettura del modello 
***/

int init_modello(MODELLO *task)
{
   extern Boolean get_working_directory();
   char path[STRLONG],savepath[STRLONG];
   char appo[STRLONG];
   int num_blocchi=0;

/*
   strcpy(appo,task->nome);
   tominus(appo);
   sprintf(path,"%s/legocad/%s",getenv("LEGOCAD_USER"),appo);
*/
   printf("init_modello local_path=%s",task->local_path);
   sprintf(path,"%s",task->local_path);

   get_working_directory(savepath);
 
   if(chdir(path) == -1)
   {
     printf("CANNOT OPEN MODEL %s\n",path);
     return(FALSE);
   }
 
   if( read_f01(task) == FALSE )
   {
      printf("CANNOT READ F01 OF MODEL %s\n",path);
      task->num_blocchi=0;
      return(FALSE);
   }

   if(chdir(savepath) == -1)
   {
     printf("CANNOT RETURN TO SIMULATOR DIRECTORY %s\n",savepath);
     return(FALSE);
   }

   return(TRUE);
}



/*
 imposta il menu di edit 
*/
setta_edit_menu()
{
   Cardinal nitem;

/* setto gli item di menu attivi / non attivi */

   narg=0;
   XtSetArg(argq[narg],XmNselectedItemCount,&nitem);narg++;
   XtGetValues(ListaModelli,argq,narg);

   if(nitem == 0)
   {
      narg=0;
      XtSetArg(argq[narg],XmNsensitive,False);narg++;
      XtSetValues(MainEditModify,argq,narg);
      XtSetValues(MainEditDelete,argq,narg);
      XtSetValues(MainEditConnect,argq,narg);
      XtSetValues(MainEditDeleteConn,argq,narg);
   }
   else if( nitem == 1)
   {   
      narg=0;
      XtSetArg(argq[narg],XmNsensitive,True);narg++;
      XtSetValues(MainEditModify,argq,narg);
      XtSetValues(MainEditDelete,argq,narg);
      XtSetValues(MainEditDeleteConn,argq,narg);
      narg=0;
      XtSetArg(argq[narg],XmNsensitive,False);narg++;
      XtSetValues(MainEditConnect,argq,narg);
   }
   else if( nitem > 1)
   {
      narg=0;
      XtSetArg(argq[narg],XmNsensitive,False);narg++;
      XtSetValues(MainEditModify,argq,narg);
      XtSetValues(MainEditDeleteConn,argq,narg);

      if( nitem > 2)
      {
         narg=0;
         XtSetArg(argq[narg],XmNsensitive,False);narg++;
         XtSetValues(MainEditConnect,argq,narg);
      }
      else if( nitem == 2)
      {
         narg=0;
         XtSetArg(argq[narg],XmNsensitive,True);narg++;
         XtSetValues(MainEditConnect,argq,narg);
      }
   }
}

/***
   crea la lista delle task del simulatore selezionato
***/ 
void set_listbl(SIMULATORE *sim)
{
   MODELLO *mod;
   char str[200];
   XmString item;
   int i;

   XmListDeleteAllItems(ListaModelli);

   for(i=0;i<sim->nmodelli;i++)
   {
      mod = sim->modelli[i];
      sprintf(str,"%s  -  %s",mod->nome,mod->descr);
      item = XmStringCreateSimple(str);
      XmListAddItem(ListaModelli,item,0);      
   } 
}

esiste_task(MODELLO *task)
{
   int i,len;
   char appo1[STRLONG],appo2[STRLONG];

   for(i=0;i<simulatore->nmodelli;i++)
   {

      len=strlen(task->local_path);
      if( task->local_path[len-1] =='/')
         sprintf(appo1,"%s",task->local_path);
      else
         sprintf(appo1,"%s/",task->local_path);

      len=strlen(simulatore->modelli[i]->local_path);
      if( simulatore->modelli[i]->local_path[len-1] =='/')
         sprintf(appo2,"%s",simulatore->modelli[i]->local_path);
      else
         sprintf(appo2,"%s/",simulatore->modelli[i]->local_path);
      tominus(appo1);
      tominus(appo2);
      if( strcmp(appo1,appo2) == NULL)
         return(TRUE);
   }
   return(FALSE);
}

int add_task(MODELLO *task)
{
   extern swidget create_messageDialog();
   extern MODELLO *alloca_modello();
   MODELLO *newmod;
   swidget wid;


   if( esiste_task(task) )
      return(FALSE);

   if( (newmod = alloca_modello()) == NULL)
   {
      printf("errore allocazione modelli\n");
      return(FALSE);
   }

   memcpy(newmod,task,sizeof(MODELLO));

   if( init_modello( newmod ) == FALSE )
   {
      printf("ERRORE la init_modello [%s] e' terminata restituendo un FALSE\n",newmod->nome);
      return(FALSE);
   }
   else
   {
      simulatore->modelli[simulatore->nmodelli] = newmod;
      simulatore->nmodelli++;
      set_listbl(simulatore);
   }

   return(TRUE);
}

int del_all_task_conn()
{
   extern int task_delete_all_connections();
   int i,*lpos,nsel;

   get_model_selected(ListaModelli,&lpos,&nsel);

   if(nsel >0) 
      for(i=0;i<nsel;i++)   
         task_delete_all_connections( simulatore->modelli[lpos[i]-1] );

}

/***
   restituisce l'id dello widget della altra varWin
***/

void getSecVarWin(swidget win1,swidget *win2)
{
printf(" getSecVarWin  VarWind1=%d VarWind2=%d\n",VarWind1,VarWind2);

  if( win1 == VarWind1 )
      *win2 = VarWind2;
   else if( win1 == VarWind2 )
      *win2 = VarWind1;
   else
      *win2 = NULL; 
printf("ricevo win1=%d, restituisco win2=%d\n",win1,*win2);
}


/***
  visualizza un messaggio nella window Messages
***/  
void show_message(char *mesg)
{
   XmTextInsert(scrolledText1,XmTextGetLastPosition(scrolledText1),mesg);
   XmTextShowPosition(scrolledText1,XmTextGetLastPosition(scrolledText1));
}


/***
  clear dei messaggi nella window Messages
***/
int clear_message()
{
   UxPutText(scrolledText1,"");
}


/***
   setta la directory corrente
***/ 
void set_working_directory(char *path)
{
   strcpy(simulatore->path,path);
   chdir(path);
}

/***
   recupera il path della directory corrente
***/
Boolean get_working_directory(char *path)
{
   char comando[200];
   FILE *fp;

   strcpy(comando,"pwd > NomeActDir.tmp");
   system(comando);
   if( (fp = fopen("NomeActDir.tmp","r") ) == NULL)
   {
      fclose(fp);
      return(False);
   }

   fscanf(fp,"%s",path);
   fclose(fp);

   strcpy(comando,"rm NomeActDir.tmp");
   system(comando);

   if(strlen(path) == 0)
      return(False);

   return(True); 

}

/***
   verifica che il nome del direttorio contenuto 
   nella stringa non contenga caratteri non validi
   ATTUALMENTE verifica solo che non ci siano blank
***/
Boolean verifica_nomedir(char *nome)
{
   if( strchr(nome,' ') != NULL )
      return(False);

   return(True);
}


Boolean verifica_lenght(char *str,int len)
{
   if( strlen(str) > len)
      return(False);

   return(True);
     
}


void ResetSimulatorActive()
{
   SimSelection = NULL;
}


/***
   recupera lo stato del bottone relativo ad SCCS ON/OFF
   e visualizza nella main window il messaggio associato
***/
void aggiorna_sccsstate()
{
   XmString xstring;

   sccsstate = XmToggleButtonGetState(MainConfigureSCCS);

   if(sccsstate == True)
      xstring  = XmStringCreateSimple("SCCS Active");
   else
      xstring  = XmStringCreateSimple("SCCS Not Active");

   narg=0;
   XtSetArg(argq[narg],XmNlabelString,xstring);narg++;
   XtSetValues(SCCSStateLabel,argq,narg);  
}


get_model_selected(swidget ListaMod,int **positions,int *count )
{
   XmListGetSelectedPos(ListaMod,positions,count); 
}


/***
   Imposta le coordinate x,y dove posizionare lo
   widget 
***/
    
set_wind_position(Widget wid,Position x,Position y)
{
   narg=0;
   XtSetArg(argq[narg],XmNx,x);narg++;
   XtSetArg(argq[narg],XmNy,y);narg++;
   XtSetValues(wid,argq,narg);
}


/***
   set insensitive dell main menu 
***/
void filepane_setinsensitive(TIPO_OP operaz)
{
    narg=0;
    XtSetArg(argq[narg],XmNsensitive,False);narg++;

    switch(operaz)
    {
       case CREASIM:
           XtSetValues(MainFileOpen,argq,narg);
           XtSetValues(MainFileRemove,argq,narg);

           narg=0;
           XtSetArg(argq[narg],XmNsensitive,True);narg++;
           XtSetValues(MainEditAdd,argq,narg);


       break;  
       case OPENSIM:

           XtSetValues(MainFileCreate,argq,narg);
           XtSetValues(MainFileRemove,argq,narg);

           narg=0;
           XtSetArg(argq[narg],XmNsensitive,True);narg++;
           XtSetValues(MainEditAdd,argq,narg);

       break;
       case REMOVESIM:
           XtSetValues(MainFileCreate,argq,narg);
           XtSetValues(MainFileOpen,argq,narg);
       break;
    } 
}


/***
   set sensitive del main menu
***/
 
void filepane_setsensitive()
{
    narg=0;
    XtSetArg(argq[narg],XmNsensitive,True);narg++;

    XtSetValues(MainFileCreate,argq,narg);
    XtSetValues(MainFileOpen,argq,narg);
    XtSetValues(MainFileRemove,argq,narg);
}


/***
   visualizza il nome del simulatore selezionato
   nella main window
***/

void set_simnome(char *nome)
{
   XmString xstring;
  
   xstring = XmStringCreateSimple(nome);
   narg=0;
   XtSetArg(argq[narg],XmNlabelString,xstring);narg++;
   XtSetValues(NomeSimulatore,argq,narg);
}




/***
   visualizza il nome del simulatore e la lista delle task
   del simulatore selezionato
***/
void display_data(SIMULATORE *sim)
{

   set_simnome(sim->nome);   

   set_listbl(sim);

}

/***
   open del simulatrore selezionato 
***/
Boolean open_simulator(char *path)
{
   extern int read_S01data();
   extern void libera_simulatore();
   extern SIMULATORE * alloca_simulatore();
   char filename[100];
   char wdir[100];

   if(path == NULL)
      return(False);

   strcpy(filename,path);
   strcat(filename,"/S01");       

   if( simulatore != NULL)
      libera_simulatore(simulatore);

   if( (simulatore = alloca_simulatore()) != NULL  )
   {

      if( read_S01data( filename, simulatore ) != True)
         return( False );

      display_data(simulatore);

      strcpy(SimulatoreCorrente,filename);

      get_working_directory(wdir);

      set_working_directory(path);

      simulator_changed = False; 
      return(True);
   }
   else
      return(False);
   
} 

/***
***/
Boolean writeS01_vuoto(char *path,char *nome)
{
   char *filename;
   FILE *fp;

   filename = XtMalloc( (strlen(path) + strlen("/S01") +2) );
   strcpy(filename,path);
   strcat(filename,"/S01"); 
    
   if( (fp = fopen(filename,"w")) == NULL)
   {
      printf("Cannot write S01 file\n");
      return(False);
   } 

   fprintf(fp,"%s\n","****");
   fprintf(fp,"%s\n",nome);  
   fprintf(fp,"%s\n","****");
   fclose(fp);
   XtFree(filename); 
   return(True);
}

/***
   crea la directory SCCS nel direttorio indicato
***/
void crea_sccsdir(char *pathname)
{
   char comando[256],appo[256];

   sprintf(comando,"mkdir %s/SCCS",pathname);
   system(comando);   

   chdir(pathname);

   sprintf(comando,"sccs create S01");
   system(comando);

}

/***
   crea un nuovo simulatore 
***/
Boolean create_simulator(char *path)
{
   char comando[100];
   char *nome,*point;

   if( path != NULL )
   {

      /* ricerco l'ultimo slash e recupero il nome del simulatore */
      point = strrchr(path,'/'); 
      point++;

      /* il nome del simulatore non puo' essere pi' lungo di 8 ch */
      if( verifica_lenght(point,0) || !verifica_lenght(point,8))
      { 
         printf("Error. Simulator Name too long or invalid\n");
         return(False);
      } 

      if( (nome = XtMalloc( strlen(point) )) == NULL)
      {
         printf("create_simulator: Error in Malloc\n");
         return(False);
      }
      strcpy(nome,point);     
  
      if( verifica_nomedir(nome) == False)
      {
         printf("il nome digitato contiene caratteri non validi\n");
         return(False);
      }

      strcpy(comando,"mkdir ");
      strcat(comando,path); 
      system(comando);

      /* creo un file S01 */
      if( writeS01_vuoto(path,nome) == False)
      { 
         printf("Error Writing file S01\n");
         XtFree(nome);
         return(False);
      }      

      /* se richiesto crea anche la directory SCCS */
      if(sccsstate)
      {
         crea_sccsdir(path);
         create_sccsS01();
         edit_sccsS01(" ");
      }

      XtFree(nome);
      return(True);
   }
   else
      return(False);
}

/***
   rimuove un simulatore 
***/

Boolean remove_simulator(char *path)
{
/* ATTENZIONE QUESTA ROUTINE NON VERIFICA SE SI STA
   FACENDO LA CANCELLAZIONE DELLA DIRECTORY 
   ATTUALE OVE SI E' POSIZIONATI!!! 
   IN QUESTO CASO POSSONO VERIFICARSI PROBLEMI
*/
 
   char comando[200],filename[100];
   FILE *fp; 


   strcpy(filename,path);
   strcat(filename,"/S01");
 
   if( (fp = fopen (filename,"r")) == NULL)
   {
      printf("\n\tERROR - File %s is not in directory \n\tThis directory does not appear as Simulator Directory", filename);
      return(False);
   }  
   fclose(fp);

/* elimino tutti i file,sottodirectory  e la directory 'path'
   senza richiedere ultriore conferma all'operatore */

   strcpy(comando,"rm -rf ");
   strcat(comando,path);
   system(comando);
   UxDestroyInterface(SimSelection);
}

/***
   aggiunge una task al simulatore
***/

Boolean aggiungi_task(MODELLO *task)
{
   extern MODELLO *alloca_modello();

   /* devo incrementare il numero dei modelli del simulatore 
      e allocare un nuovo MODELLO 
   */
   simulatore->modelli[simulatore->nmodelli] = alloca_modello();
   simulatore->nmodelli++;
}


compatta_modelli(int indice_start)
{
   int i;

   for(i=indice_start;i<=simulatore->nmodelli-2;i++)
   {       
       simulatore->modelli[i] = simulatore->modelli[i+1];
   }
   simulatore->modelli[simulatore->nmodelli-1] = 0;
}

/***
   rimuove una task ad un simulatore
***/

Boolean rimuovi_task(MODELLO *task)
{
   int i;
   extern int task_delete_all_connections();

   
   task_delete_all_connections( task );

   for(i=0;i<simulatore->nmodelli;i++)
   {
      if( simulatore->modelli[i] == task )
      {
         libera_modello(task);
         simulatore->modelli[i] = NULL;
         compatta_modelli(i);
         simulatore->nmodelli--;
      }
   }
}

confermata_remove_task()
{
   int *lpos,nsel;

   get_model_selected(ListaModelli,&lpos,&nsel);

   if(nsel >0)    
   {
      rimuovi_task(simulatore->modelli[lpos[0]-1]);
      XmListDeletePos(ListaModelli,lpos[0]);
   }

   if( nsel > 1)   
      confermata_remove_task();

   setta_edit_menu();    
}



/*---------------------------------------------------------------------------*/
/*************************  COMANDI SCCS *************************************/
/*---------------------------------------------------------------------------*/
/***
   UNEDIT SCCS
***/
Boolean unedit_sccsS01()
{
   char comando[256],path[256];

   get_working_directory(path); 
   chdir(path); 

   strcpy(comando,"sccs unedit S01");
   system(comando);

   return(True);

}

/***
   INFO SCCS
***/
Boolean info_sccsS01()
{
   extern swidget create_ViewS01();
   char comando[256],path[256];
   swidget sw;

   get_working_directory(path); 
   chdir(path);  

   strcpy(comando,"sccs info > info");
   system(comando);

   sw=create_ViewS01("info");
   UxPopupInterface(sw,no_grab);
   return(True);

}

/***
   GET SCCS
***/
Boolean get_sccsS01(char *vers)
{
   char comando[256],path[256];

   get_working_directory(path); 
   chdir(path);     

   sprintf(comando,"sccs get -r%s S01",vers);
   printf("Invio il comando %s\n",comando);
   system(comando);
   return(True);
}

/*** 
   EDIT SCCS
***/
Boolean edit_sccsS01(char *vers)
{
   char comando[256],path[256];;

   get_working_directory(path); 
   chdir(path);   

   sprintf(comando,"sccs edit -r%s S01",vers);
   printf("Invio il comando %s\n",comando);
   system(comando);
   return(True);
}

/***
   DELTA SCCS
***/
Boolean delta_sccsS01()
{
   char comando[256],path[100];

   get_working_directory(path); 
   chdir(path);   
   save_S01(path);

   strcpy(comando,"sccs delta -ycommento S01");
   printf("Invio il comando %s\n",comando);
   system(comando);

   return(True);
}

/***
   CREATE SCCS
***/
int create_sccsS01()
{
   char comando[256],path[256],sccsdir[256];

   get_working_directory(path); 
   chdir(path);   

   sprintf(comando,"mkdir %s/SCCS",path);
   system(comando);
 
   strcpy(comando,"sccs create S01");
   printf("Invio il comando %s\n",comando);
   system(comando);

   return(True);
}

/*---------------------------------------------------------------------------*/
/************************* FINE COMANDI SCCS *********************************/
/*---------------------------------------------------------------------------*/

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_MainFileCreate( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_SimulatorSelection();
	   extern void simulatorselection_settitle(); 
	   extern swidget create_msgToUser();
	
	   if( fin_conn_open )
	   {
	      create_msgToUser(0,"Connections Window Already Open. Close First");
	      return;
	   }
	
	   if( SimSelection == NULL)
	   {
	      SimSelection = create_SimulatorSelection(CREASIM);
	      UxPopupInterface(SimSelection,no_grab);
	      filepane_setinsensitive(CREASIM);
	   }
	   else
	      XRaiseWindow( UxDisplay, XtWindow(XtParent(SimSelection)) );
	  
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainFileOpen( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_SimulatorSelection();
	   extern swidget create_msgToUser();
	
	   if( fin_conn_open)
	   {
	      create_msgToUser(0,"Connections Window Already Open. Close First");
	      return;
	   }
	
	   if( SimSelection == NULL )
	   {
	      SimSelection = create_SimulatorSelection(OPENSIM);
	      UxPopupInterface( SimSelection,no_grab);
	      filepane_setinsensitive(OPENSIM);
	   }
	   else
	     XRaiseWindow( UxDisplay,XtWindow( XtParent(SimSelection)) );
	
	   if( simulator_changed )
	     create_msgToUser(0,"WARNING Current Model Not Save");
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainFileRemove( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_SimulatorSelection();
	
	   if( SimSelection == NULL )
	   {
	      SimSelection = create_SimulatorSelection(REMOVESIM);
	      UxPopupInterface(SimSelection,no_grab);
	      filepane_setinsensitive(REMOVESIM);
	   }
	   else
	     XRaiseWindow( UxDisplay, XtWindow(XtParent(SimSelection)) );
	
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainFileSave( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_SCCSGEST();
	   extern int exist_sccsdir();
	   extern swidget create_msgToUser();
	   swidget wid;
	   char path[100];
	   Boolean Model_Save;
	
	/* ancora non viene testata la gestione
	   con SCCS  per il momento si suppone 
	   SCCS ON */
	 
	printf("callback save \n");
	
	/***
	   aggiorna_sccsstate();
	***/
	
	   get_working_directory(path);
	
	/***
	   if(sccsstate && exist_sccsdir(path) )
	   {
	      wid = create_SCCSGEST(SAVESIM,path);
	      UxPopupInterface(wid,no_grab);
	   }
	   else
	***/
	
	   {
	      set_wait_cursor();
	      printf("chiamo la save_S01\n");
	
	      Model_Save = save_S01(path);
	
	      if( !Model_Save )
	         create_msgToUser(0,"ERROR  MODEL NOT SAVED. VERIFY PERMISSION ON S01");
	      else
	         if( check_delta_time() != TRUE )
	            create_msgToUser(0,"WARNING This Model Have ACTIVATION INTERVAL Not Correct. MODEL SAVE OK");
	         else
	            create_msgToUser(0,"MODEL SAVE OK");
	
	       set_normal_cursor();
	
	   } 
	   simulator_changed = False;
	
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainFileExit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	extern swidget create_questionDialog();
	swidget wid;
	
	wid = create_questionDialog(MainFileExit,BYEBYE);
	UxPopupInterface(wid,no_grab);
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainEditModify( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_ModelDefinitionWindow();
	   int *lpos,nsel;
	
	   get_model_selected(ListaModelli,&lpos,&nsel);
	
	   if( ModelDefinitionInterface == NULL ) 
	   {
	      if( nsel == 1 )
	      {
	         printf("simulatore->modelli[lpos-1]=%d\n",simulatore->modelli[lpos[0]-1]);
	         ModelDefinitionInterface  =  create_ModelDefinitionWindow(simulatore->modelli[lpos[0]-1],MODTASK);
	         UxPopupInterface(ModelDefinitionInterface,no_grab);
	      }
	      else
	         show_message("\nYou must select One model.\n");
	   }
	   else 
	   {
	       XRaiseWindow( UxDisplay, XtWindow(ModelDefinitionInterface) );
	   }
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainEditDelete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   swidget wid;
	   int *position,count=0;
	
	   get_model_selected(ListaModelli,&position,&count);
	   if( count )
	   {
	      wid = create_questionDialog(MainEditDelete,DELTASK);
	      UxPopupInterface(wid,no_grab);
	   }
	   else
	      show_message("\nYou must select One model.\n");
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainEditDeleteConn( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   int *position,count=0;
	   swidget wid;
	
	
	   get_model_selected(ListaModelli,&position,&count);
	   if( count )
	   {
	       wid = create_questionDialog(MainEditDelete,DELALLCONN);
	      UxPopupInterface(wid,no_grab);
	   }
	   else
	      show_message("\nYou must select One model.\n");
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainEditAdd( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_ModelDefinitionWindow();
	   int lpos,nsel;
	   char *app=NULL;
	
	
	   if( ModelDefinitionInterface == NULL ) 
	   {
	      ModelDefinitionInterface  =  create_ModelDefinitionWindow(app,ADDTASK);
	      UxPopupInterface(ModelDefinitionInterface,no_grab);
	   }
	   else 
	      XRaiseWindow( UxDisplay, XtWindow(ModelDefinitionInterface) );
	
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainEditConnect( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	extern swidget create_msgToUser();
	extern swidget create_ModelConnessionWindow();
	extern swidget create_topVarWin1();
	extern void set_connwind_closure();
	char *str_type[20];
	XrmValue value;
	char oldcolor[50];
	int *position,count;
	MODELLO *mod1,*mod2;
	int ind_mod1,ind_mod2;
	
	if( fin_conn_open )
	{
	   create_msgToUser(0,"Connections Window Already Open. Close First");
	   return;
	}
	
	/* recupero i due modelli selezionati */
	get_model_selected(ListaModelli,&position,&count);
	if( count != 2)
	   return;
	
	printf("position[0]-1 = %d\n",position[0]-1);
	printf("position[1]-1 = %d\n",position[1]-1);
	
	mod1 = simulatore->modelli[position[0]-1];
	mod2 = simulatore->modelli[position[1]-1];
	
	ind_mod1=position[0]-1;
	ind_mod2=position[1]-1;
	
	printf("mod1 = %s ind_mod1=%d\n",mod1->nome,ind_mod1);
	printf("mod2 = %s ind_mod2=%d\n",mod2->nome,ind_mod2);
	
	/* recupero il colore di background della applicazione */
	//CMC XrmGetResource( (UxDisplay->db) ,"topsim.background","Topsim.Background",str_type,&value);
	strncpy(oldcolor,value.addr,(int)value.size);
	
	/* setto il background della shell di connessione 1 */
	//CMC XrmPutStringResource( &(UxDisplay->db) ,"*background",COLWINDCONN1);
	
	/* creo le interfacce */
	ConnWind1 = create_ModelConnessionWindow(mod1);
	UxPopupInterface(ConnWind1,no_grab);
	
	sleep(1);
	
	VarWind1 = create_topVarWin1(mod1,ind_mod1,position[0]-1);
	UxPopupInterface(VarWind1,no_grab);
	set_connwind_closure(VarWind1);
	
	/* setto la loro posizione */
	set_wind_position(ConnWind1,XCONN1,YCONN1);
	set_wind_position(VarWind1,XCONN1,YVAR);
	
	/* setto il background della shell di connessione 2 */
	//CMC XrmPutStringResource(&(UxDisplay->db),"*background",COLWINDCONN2);
	
	/* creo la Connession Window 2 */
	ConnWind2 = create_ModelConnessionWindow(mod2);
	UxPopupInterface(ConnWind2,no_grab);
	VarWind2 = create_topVarWin1(mod2,ind_mod2,position[1]-1);
	UxPopupInterface(VarWind2,no_grab);
	set_connwind_closure(VarWind2);
	
	/* setto la posizione */
	set_wind_position(ConnWind2,XCONN2,YCONN2);
	set_wind_position(VarWind2,XCONN2,YVAR);
	
	/* rispristino il colore di background iniziale */
	//CMC XrmPutStringResource( &(UxDisplay->db) ,"*background",oldcolor);
	
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainUtilitiesCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	swidget swid;
	extern swidget create_CopyS01(); 
	
	swid = create_CopyS01();
	UxPopupInterface(swid,no_grab);
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainUtilitiesView( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	extern swidget create_ViewS01();
	swidget wid;
	
	wid = create_ViewS01(SimulatoreCorrente);
	UxPopupInterface(wid,no_grab);
	}
	UxTopSimContext = UxSaveCtx;
}

static void	valueChangedCB_MainConfigureSCCS( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	aggiorna_sccsstate();
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainNetCompi( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	      set_wait_cursor();
	      system("net_compi > compi.out"); 
	      show_message("Compilation Done.\nYou can see output results on file net_compi.out.\n");
	      set_normal_cursor();
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_MainView( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	extern swidget create_ViewS01();
	swidget wid;
	
	wid = create_ViewS01("net_compi.out");
	UxPopupInterface(wid,no_grab);
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_menu1_p6_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   extern swidget create_SCCSGESTnew();
	   swidget sw;
	
	   sw = create_SCCSGESTnew(simulatore->path);
	   UxPopupInterface(sw,no_grab);
	}
	UxTopSimContext = UxSaveCtx;
}

static void	multipleSelectionCB_ListaModelli( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	Cardinal nitem;
	
	/* setto gli item di menu attivi / non attivi */
	
	narg=0;
	XtSetArg(argq[narg],XmNselectedItemCount,&nitem);narg++;
	XtGetValues(ListaModelli,argq,narg);
	
	if(nitem == 0)
	{
	   narg=0;
	   XtSetArg(argq[narg],XmNsensitive,False);narg++;
	   XtSetValues(MainEditModify,argq,narg);
	   XtSetValues(MainEditDelete,argq,narg);
	   XtSetValues(MainEditConnect,argq,narg);
	   XtSetValues(MainEditDeleteConn,argq,narg);
	}
	else if( nitem == 1)
	{
	   narg=0;
	   XtSetArg(argq[narg],XmNsensitive,True);narg++;
	   XtSetValues(MainEditModify,argq,narg);
	   XtSetValues(MainEditDelete,argq,narg);
	   XtSetValues(MainEditDeleteConn,argq,narg);
	   narg=0;
	   XtSetArg(argq[narg],XmNsensitive,False);narg++;
	   XtSetValues(MainEditConnect,argq,narg);
	}
	else if( nitem > 1)
	{
	   narg=0;
	   XtSetArg(argq[narg],XmNsensitive,False);narg++;
	   XtSetValues(MainEditModify,argq,narg);
	   XtSetValues(MainEditDeleteConn,argq,narg);
	
	   if( nitem > 2)
	   {
	      narg=0;
	      XtSetArg(argq[narg],XmNsensitive,False);narg++;
	      XtSetValues(MainEditConnect,argq,narg);
	   }
	   else if( nitem == 2)
	   {
	      narg=0;
	      XtSetArg(argq[narg],XmNsensitive,True);narg++;
	      XtSetValues(MainEditConnect,argq,narg);
	   }
	}
	
	
	}
	UxTopSimContext = UxSaveCtx;
}

static void	activateCB_menu2_p1_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCTopSim              *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopSimContext;
	UxTopSimContext = UxContext =
			(_UxCTopSim *) UxGetContext( UxWidget );
	{
	   clear_message();
	}
	UxTopSimContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_TopSim()
{
	Widget	TopSim_shell;
	Widget	MainFile_shell;
	Widget	MainEdit_shell;
	Widget	MainUtilities_shell;
	Widget	MainConfigure_shell;
	Widget	MainCompi_shell;
	Widget	SCCS_shell;
	Widget	menu2_shell;

	TopSim_shell = XtVaCreatePopupShell( "TopSim_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 350,
			XmNy, 210,
			XmNwidth, 590,
			XmNheight, 680,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "TopSim",
			XmNiconName, "TopSim",
			NULL );

	TopSim = XtVaCreateManagedWidget( "TopSim",
			xmMainWindowWidgetClass, TopSim_shell,
			XmNheight, 680,
			XmNwidth, 590,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( TopSim, (char *) UxTopSimContext );

	menu1 = XtVaCreateManagedWidget( "menu1",
			xmRowColumnWidgetClass, TopSim,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu1, (char *) UxTopSimContext );

	MainFile_shell = XtVaCreatePopupShell ("MainFile_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	MainFile = XtVaCreateWidget( "MainFile",
			xmRowColumnWidgetClass, MainFile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( MainFile, (char *) UxTopSimContext );

	MainFileCreate = XtVaCreateManagedWidget( "MainFileCreate",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Create Sim" ),
			NULL );

	UxPutContext( MainFileCreate, (char *) UxTopSimContext );

	MainFileOpen = XtVaCreateManagedWidget( "MainFileOpen",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Open Sim" ),
			NULL );

	UxPutContext( MainFileOpen, (char *) UxTopSimContext );

	MainFileRemove = XtVaCreateManagedWidget( "MainFileRemove",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Remove Sim" ),
			NULL );

	UxPutContext( MainFileRemove, (char *) UxTopSimContext );

	MainFileSave = XtVaCreateManagedWidget( "MainFileSave",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Save S01" ),
			NULL );

	UxPutContext( MainFileSave, (char *) UxTopSimContext );

	MainFileSaveAs = XtVaCreateManagedWidget( "MainFileSaveAs",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Save As.." ),
			NULL );

	UxPutContext( MainFileSaveAs, (char *) UxTopSimContext );

	MainFileExit = XtVaCreateManagedWidget( "MainFileExit",
			xmPushButtonWidgetClass, MainFile,
			RES_CONVERT( XmNlabelString, "Exit" ),
			NULL );

	UxPutContext( MainFileExit, (char *) UxTopSimContext );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, MainFile,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxTopSimContext );

	MainEdit_shell = XtVaCreatePopupShell ("MainEdit_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	MainEdit = XtVaCreateWidget( "MainEdit",
			xmRowColumnWidgetClass, MainEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( MainEdit, (char *) UxTopSimContext );

	MainEditModify = XtVaCreateManagedWidget( "MainEditModify",
			xmPushButtonWidgetClass, MainEdit,
			RES_CONVERT( XmNlabelString, "Modify Task" ),
			NULL );

	UxPutContext( MainEditModify, (char *) UxTopSimContext );

	MainEditDelete = XtVaCreateManagedWidget( "MainEditDelete",
			xmPushButtonWidgetClass, MainEdit,
			RES_CONVERT( XmNlabelString, "Delete Task" ),
			NULL );

	UxPutContext( MainEditDelete, (char *) UxTopSimContext );

	MainEditDeleteConn = XtVaCreateManagedWidget( "MainEditDeleteConn",
			xmPushButtonWidgetClass, MainEdit,
			RES_CONVERT( XmNlabelString, "Delete All Task Connections" ),
			NULL );

	UxPutContext( MainEditDeleteConn, (char *) UxTopSimContext );

	MainEditAdd = XtVaCreateManagedWidget( "MainEditAdd",
			xmPushButtonWidgetClass, MainEdit,
			RES_CONVERT( XmNlabelString, "Add Task" ),
			NULL );

	UxPutContext( MainEditAdd, (char *) UxTopSimContext );

	MainEditConnect = XtVaCreateManagedWidget( "MainEditConnect",
			xmPushButtonWidgetClass, MainEdit,
			RES_CONVERT( XmNlabelString, "Connect Task" ),
			NULL );

	UxPutContext( MainEditConnect, (char *) UxTopSimContext );

	menu1_top_b2 = XtVaCreateManagedWidget( "menu1_top_b2",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, MainEdit,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu1_top_b2, (char *) UxTopSimContext );

	MainUtilities_shell = XtVaCreatePopupShell ("MainUtilities_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	MainUtilities = XtVaCreateWidget( "MainUtilities",
			xmRowColumnWidgetClass, MainUtilities_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( MainUtilities, (char *) UxTopSimContext );

	MainUtilitiesCopy = XtVaCreateManagedWidget( "MainUtilitiesCopy",
			xmPushButtonWidgetClass, MainUtilities,
			RES_CONVERT( XmNlabelString, "Copy S01" ),
			NULL );

	UxPutContext( MainUtilitiesCopy, (char *) UxTopSimContext );

	MainUtilitiesView = XtVaCreateManagedWidget( "MainUtilitiesView",
			xmPushButtonWidgetClass, MainUtilities,
			RES_CONVERT( XmNlabelString, "View S01" ),
			NULL );

	UxPutContext( MainUtilitiesView, (char *) UxTopSimContext );

	menu1_top_b3 = XtVaCreateManagedWidget( "menu1_top_b3",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, MainUtilities,
			RES_CONVERT( XmNlabelString, "Utilities" ),
			NULL );

	UxPutContext( menu1_top_b3, (char *) UxTopSimContext );

	MainConfigure_shell = XtVaCreatePopupShell ("MainConfigure_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	MainConfigure = XtVaCreateWidget( "MainConfigure",
			xmRowColumnWidgetClass, MainConfigure_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( MainConfigure, (char *) UxTopSimContext );

	MainConfigureSCCS = XtVaCreateManagedWidget( "MainConfigureSCCS",
			xmToggleButtonGadgetClass, MainConfigure,
			XmNvisibleWhenOff, TRUE,
			XmNset, FALSE,
			RES_CONVERT( XmNlabelString, "SCCS" ),
			NULL );

	UxPutContext( MainConfigureSCCS, (char *) UxTopSimContext );

	menu1_top_b4 = XtVaCreateManagedWidget( "menu1_top_b4",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, MainConfigure,
			RES_CONVERT( XmNlabelString, "Configure" ),
			NULL );

	UxPutContext( menu1_top_b4, (char *) UxTopSimContext );

	MainCompi_shell = XtVaCreatePopupShell ("MainCompi_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	MainCompi = XtVaCreateWidget( "MainCompi",
			xmRowColumnWidgetClass, MainCompi_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( MainCompi, (char *) UxTopSimContext );

	MainNetCompi = XtVaCreateManagedWidget( "MainNetCompi",
			xmPushButtonWidgetClass, MainCompi,
			RES_CONVERT( XmNlabelString, "net_compi" ),
			NULL );

	UxPutContext( MainNetCompi, (char *) UxTopSimContext );

	MainView = XtVaCreateManagedWidget( "MainView",
			xmPushButtonWidgetClass, MainCompi,
			RES_CONVERT( XmNlabelString, "View results" ),
			NULL );

	UxPutContext( MainView, (char *) UxTopSimContext );

	menu1_top_b5 = XtVaCreateManagedWidget( "menu1_top_b5",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, MainCompi,
			RES_CONVERT( XmNlabelString, "Compile" ),
			NULL );

	UxPutContext( menu1_top_b5, (char *) UxTopSimContext );

	SCCS_shell = XtVaCreatePopupShell ("SCCS_shell",
			xmMenuShellWidgetClass, menu1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	SCCS = XtVaCreateWidget( "SCCS",
			xmRowColumnWidgetClass, SCCS_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( SCCS, (char *) UxTopSimContext );

	menu1_p6_b1 = XtVaCreateManagedWidget( "menu1_p6_b1",
			xmPushButtonWidgetClass, SCCS,
			RES_CONVERT( XmNlabelString, "SCCS Interface" ),
			NULL );

	UxPutContext( menu1_p6_b1, (char *) UxTopSimContext );

	menu1_top_b6 = XtVaCreateManagedWidget( "menu1_top_b6",
			xmCascadeButtonWidgetClass, menu1,
			XmNsubMenuId, SCCS,
			RES_CONVERT( XmNlabelString, "SCCS" ),
			NULL );

	UxPutContext( menu1_top_b6, (char *) UxTopSimContext );

	panedWindow2 = XtVaCreateManagedWidget( "panedWindow2",
			xmPanedWindowWidgetClass, TopSim,
			NULL );

	UxPutContext( panedWindow2, (char *) UxTopSimContext );

	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass, panedWindow2,
			XmNpaneMinimum, 400,
			XmNheight, 510,
			XmNwidth, 570,
			XmNy, 10,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form2, (char *) UxTopSimContext );

	Titolo = XtVaCreateManagedWidget( "Titolo",
			xmLabelWidgetClass, form2,
			XmNrightOffset, 5,
			XmNleftOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "helvetica_oblique14" ),
			RES_CONVERT( XmNlabelString, "SIMULATOR TOPOLOGY" ),
			XmNheight, 40,
			XmNwidth, 290,
			XmNy, 10,
			XmNx, 130,
			NULL );

	UxPutContext( Titolo, (char *) UxTopSimContext );

	NomeSimulatore = XtVaCreateManagedWidget( "NomeSimulatore",
			xmLabelWidgetClass, form2,
			XmNtopOffset, 5,
			XmNtopWidget, Titolo,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNfontList, UxConvertFontList( "helvetica_bold18" ),
			RES_CONVERT( XmNlabelString, "NOME DEL SIMULATORE" ),
			XmNheight, 40,
			XmNwidth, 420,
			XmNy, 50,
			XmNx, 5,
			NULL );

	UxPutContext( NomeSimulatore, (char *) UxTopSimContext );

	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass, form2,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 140,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomOffset, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 132,
			XmNx, 4,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow2, (char *) UxTopSimContext );

	ListaModelli = XtVaCreateManagedWidget( "ListaModelli",
			xmListWidgetClass, scrolledWindow2,
			XmNselectionPolicy, XmMULTIPLE_SELECT,
			XmNheight, 381,
			XmNwidth, 564,
			NULL );

	UxPutContext( ListaModelli, (char *) UxTopSimContext );

	SCCSStateLabel = XtVaCreateManagedWidget( "SCCSStateLabel",
			xmLabelWidgetClass, form2,
			XmNalignment, XmALIGNMENT_CENTER,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNlabelType, XmSTRING,
			XmNfontList, UxConvertFontList( "helvetica12" ),
			RES_CONVERT( XmNlabelString, "SCCS Active" ),
			XmNheight, 31,
			XmNwidth, 106,
			XmNy, 95,
			XmNx, 235,
			NULL );

	UxPutContext( SCCSStateLabel, (char *) UxTopSimContext );

	FindForm = XtVaCreateManagedWidget( "FindForm",
			xmFormWidgetClass, form2,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNtopWidget, scrolledWindow2,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftPosition, 0,
			XmNheight, 32,
			XmNwidth, 566,
			XmNy, 472,
			XmNx, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( FindForm, (char *) UxTopSimContext );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, panedWindow2,
			XmNpaneMinimum, 100,
			XmNheight, 27,
			XmNwidth, 590,
			XmNy, 619,
			XmNx, 0,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form1, (char *) UxTopSimContext );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, form1,
			XmNtopOffset, 25,
			XmNrightOffset, 5,
			XmNleftOffset, 5,
			XmNbottomOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 10,
			XmNx, 11,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxTopSimContext );

	scrolledText1 = XtVaCreateManagedWidget( "scrolledText1",
			xmTextWidgetClass, scrolledWindow1,
			XmNpendingDelete, FALSE,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNeditable, FALSE,
			XmNheight, 105,
			XmNwidth, 562,
			NULL );

	UxPutContext( scrolledText1, (char *) UxTopSimContext );

	menu2_shell = XtVaCreatePopupShell ("menu2_shell",
			xmMenuShellWidgetClass, scrolledText1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2 = XtVaCreateWidget( "menu2",
			xmRowColumnWidgetClass, menu2_shell,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu2, (char *) UxTopSimContext );

	menu2_p1_b1 = XtVaCreateManagedWidget( "menu2_p1_b1",
			xmLabelGadgetClass, menu2,
			RES_CONVERT( XmNlabelString, "MESSAGE" ),
			NULL );

	UxPutContext( menu2_p1_b1, (char *) UxTopSimContext );

	menu2_p1_b3 = XtVaCreateManagedWidget( "menu2_p1_b3",
			xmSeparatorGadgetClass, menu2,
			NULL );

	UxPutContext( menu2_p1_b3, (char *) UxTopSimContext );

	menu2_p1_b2 = XtVaCreateManagedWidget( "menu2_p1_b2",
			xmPushButtonWidgetClass, menu2,
			RES_CONVERT( XmNmnemonic, "C" ),
			RES_CONVERT( XmNlabelString, "Clear Window" ),
			NULL );

	UxPutContext( menu2_p1_b2, (char *) UxTopSimContext );

	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass, form1,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Messages" ),
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomWidget, scrolledWindow1,
			XmNbottomOffset, 5,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 18,
			XmNwidth, 143,
			XmNy, 4,
			XmNx, 7,
			NULL );

	UxPutContext( label1, (char *) UxTopSimContext );

	XtAddCallback( TopSim, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainFileCreate, XmNactivateCallback,
			activateCB_MainFileCreate,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainFileOpen, XmNactivateCallback,
			activateCB_MainFileOpen,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainFileRemove, XmNactivateCallback,
			activateCB_MainFileRemove,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainFileSave, XmNactivateCallback,
			activateCB_MainFileSave,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainFileExit, XmNactivateCallback,
			activateCB_MainFileExit,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainEditModify, XmNactivateCallback,
			activateCB_MainEditModify,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainEditDelete, XmNactivateCallback,
			activateCB_MainEditDelete,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainEditDeleteConn, XmNactivateCallback,
			activateCB_MainEditDeleteConn,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainEditAdd, XmNactivateCallback,
			activateCB_MainEditAdd,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainEditConnect, XmNactivateCallback,
			activateCB_MainEditConnect,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainUtilitiesCopy, XmNactivateCallback,
			activateCB_MainUtilitiesCopy,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainUtilitiesView, XmNactivateCallback,
			activateCB_MainUtilitiesView,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainConfigureSCCS, XmNvalueChangedCallback,
			valueChangedCB_MainConfigureSCCS,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainNetCompi, XmNactivateCallback,
			activateCB_MainNetCompi,
			(XtPointer) UxTopSimContext );

	XtAddCallback( MainView, XmNactivateCallback,
			activateCB_MainView,
			(XtPointer) UxTopSimContext );

	XtAddCallback( menu1_p6_b1, XmNactivateCallback,
			activateCB_menu1_p6_b1,
			(XtPointer) UxTopSimContext );

	XtAddCallback( ListaModelli, XmNmultipleSelectionCallback,
			multipleSelectionCB_ListaModelli,
			(XtPointer) UxTopSimContext );

	XtAddCallback( menu2_p1_b2, XmNactivateCallback,
			activateCB_menu2_p1_b2,
			(XtPointer) UxTopSimContext );


	XtAddEventHandler( scrolledText1, ButtonPressMask,
			False, _UxTopSimMenuPost, menu2 );
	XmMainWindowSetAreas( TopSim, menu1, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, panedWindow2 );

	return ( TopSim );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_TopSim()
{
	Widget                  rtrn;
	_UxCTopSim              *UxContext;

	UxTopSimContext = UxContext =
		(_UxCTopSim *) XtMalloc( sizeof(_UxCTopSim) );

	{
		Widget userlist_find_form;
		
		SimSelection = NULL; /* inizializzo */
		ModelDefinitionInterface = NULL;
		ConnWind1=VarWind1=ConnWind2=VarWind2=NULL;
		rtrn = _Uxbuild_TopSim();

		/*** ATTENZIONE --- istruzioni per rilasciare versione ancora in sviluppo  ****/
		
		   /* rendo inattivi i bottoni per gestione SCCS */
		   set_something_val(MainConfigureSCCS,XmNsensitive, (XtArgVal) False);
		   set_something_val(menu1_p6_b1,XmNsensitive, (XtArgVal) False); 
		
		/*** FINE ****/
		
		   narg=0;
		   XtSetArg(argq[narg],XmNsensitive,False);narg++;
		   XtSetValues(MainConfigureSCCS,argq,narg);
		
		
		/* Inserimento find di libreria */
		   parametri_userlist.ID_lista = UxGetWidget(ListaModelli);
		   parametri_userlist.num_elem = 100;
		   parametri_userlist.vis_elem = 1;
		   parametri_userlist.pos      = 0;
		
		   narg=0;
		   XtSetArg(argq[narg],XmNleftAttachment,XmATTACH_FORM);
		   narg++;
		   XtSetArg(argq[narg],XmNleftOffset,0);
		   narg++;
		   XtSetArg(argq[narg],XmNrightAttachment,XmATTACH_FORM);
		   narg++;
		   XtSetArg(argq[narg],XmNrightOffset,0);
		   narg++;
		   XtSetArg(argq[narg],XmNbottomAttachment,XmATTACH_FORM);
		   narg++;
		   XtSetArg(argq[narg],XmNbottomOffset,0);
		   narg++;
		   XtSetArg(argq[narg],XmNtopAttachment,XmATTACH_FORM);
		   narg++;
		   XtSetArg(argq[narg],XmNtopOffset,0);
		   narg++;
		
		   userlist_find_form = find_kit(UxGetWidget(FindForm),argq,narg,
		                                                 &parametri_userlist);
		
		
		   narg=0;
		   XtSetArg(argq[narg],XmNsensitive,False);narg++;
		   XtSetValues(MainEditModify,argq,narg);
		   XtSetValues(MainEditDelete,argq,narg);
		   XtSetValues(MainEditConnect,argq,narg);
		   XtSetValues(MainEditDeleteConn,argq,narg);
		   XtSetValues(MainEditAdd,argq,narg);
		
		   aggiorna_sccsstate();
		
		   show_message("\n\nWelcome in  TOPSIM  Simulator Topology Editor\n");
		   show_message("Property of Enel S.p.A. - C.R.A. Researce Center - Milan (ITALY)\n");
		   show_message("Developed by S.d.I. S.r.l. Milan (ITALY)\n\n");        
		
		   waitCursor = XCreateFontCursor(UxDisplay,XC_watch);
		   normalCursor = XCreateFontCursor(UxDisplay,XC_hand1);
		
		
		   return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_TopSim()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_TopSim();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

