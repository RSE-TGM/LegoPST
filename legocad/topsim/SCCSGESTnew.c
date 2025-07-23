
/*******************************************************************************
	SCCSGESTnew.c
	(Generated from interface file SCCSGESTnew.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo SCCSGESTnew.i
   tipo 
   release 1.2
   data 8/29/95
   reserved @(#)SCCSGESTnew.i	1.2
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#include "topsim.h"
#include "lg1_topsim.h"

extern SIMULATORE *simulatore; 

#define FILEPRS "prs.dat"     /* file di output sccs prs */

#define COMMENTS   1    /* tipo riga commento */ 
#define MRS        2    /* tipo riga mrs */
#define MAXLENCOMM 100  
#define MAXLEN     50

typedef struct {
                  char ver[MAXLEN];
                  char date[MAXLEN];
                  char user[MAXLEN];
                  char mrs[MAXLENCOMM];
                  char comments[MAXLENCOMM];
               } VERSIONI;

/*******************************************************************************
	The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxSCCSGESTnewMenuPost( wgt, client_data, event, ctd )
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
	Widget	UxSCCSGESTnew;
	Widget	Uxform3;
	Widget	UxscrolledWindow3;
	Widget	UxListVersion1;
	Widget	Uxmenu6;
	Widget	Uxmenu6_p1_title;
	Widget	UxSCCSseparator;
	Widget	UxSCCScreate;
	Widget	UxSCCSmenuEdit;
	Widget	UxSCCSgetB;
	Widget	UxSCCSeditB;
	Widget	UxSCCSedit;
	Widget	UxSCCSmenuDelta;
	Widget	UxSCCSdelgetB;
	Widget	UxSCCSdeleditB;
	Widget	UxSCCSsave;
	Widget	UxSCCSunedit;
	Widget	UxSCCSinfo;
	Widget	Uxlabel2;
	Widget	UxcancelButton1;
	unsigned char	Uxversion[20];
	unsigned char	Uxfilname[120];
	VERSIONI	*Uxsvers;
	int	Uxnver;
	unsigned char	*Uxpath;
} _UxCSCCSGESTnew;

#define SCCSGESTnew             UxSCCSGESTnewContext->UxSCCSGESTnew
#define form3                   UxSCCSGESTnewContext->Uxform3
#define scrolledWindow3         UxSCCSGESTnewContext->UxscrolledWindow3
#define ListVersion1            UxSCCSGESTnewContext->UxListVersion1
#define menu6                   UxSCCSGESTnewContext->Uxmenu6
#define menu6_p1_title          UxSCCSGESTnewContext->Uxmenu6_p1_title
#define SCCSseparator           UxSCCSGESTnewContext->UxSCCSseparator
#define SCCScreate              UxSCCSGESTnewContext->UxSCCScreate
#define SCCSmenuEdit            UxSCCSGESTnewContext->UxSCCSmenuEdit
#define SCCSgetB                UxSCCSGESTnewContext->UxSCCSgetB
#define SCCSeditB               UxSCCSGESTnewContext->UxSCCSeditB
#define SCCSedit                UxSCCSGESTnewContext->UxSCCSedit
#define SCCSmenuDelta           UxSCCSGESTnewContext->UxSCCSmenuDelta
#define SCCSdelgetB             UxSCCSGESTnewContext->UxSCCSdelgetB
#define SCCSdeleditB            UxSCCSGESTnewContext->UxSCCSdeleditB
#define SCCSsave                UxSCCSGESTnewContext->UxSCCSsave
#define SCCSunedit              UxSCCSGESTnewContext->UxSCCSunedit
#define SCCSinfo                UxSCCSGESTnewContext->UxSCCSinfo
#define label2                  UxSCCSGESTnewContext->Uxlabel2
#define cancelButton1           UxSCCSGESTnewContext->UxcancelButton1
#define version                 UxSCCSGESTnewContext->Uxversion
#define filname                 UxSCCSGESTnewContext->Uxfilname
#define svers                   UxSCCSGESTnewContext->Uxsvers
#define nver                    UxSCCSGESTnewContext->Uxnver
#define path                    UxSCCSGESTnewContext->Uxpath

static _UxCSCCSGESTnew	*UxSCCSGESTnewContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_SCCSGESTnew();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

int crea_file_prs(char *pathf,char *file)
{
   char comando[255];

   sprintf(comando,"sccs prs %s/%s > %s",pathf,file,FILEPRS);
   system(comando);
}

/* questa routine legge il file generato dalla redirezione su file
   del comando SCCS PRS
   la KEYWORD D  definisce l'inizio di un set di righe relative alla versione xx
   le KEYWORD MRs e COMMENTS  permettono di interpretare le righe ad esse seguenti
                              come mrs o commento
*/

int read_file_prs()
{
   FILE *fp;
   char stringa[251];
   char key[251],spare[251],ver[20],date[20],user[20];
   char *pnl;
   int retval;
   char mrs[251],comments[251];
   int tipo;

   if( (fp=fopen(FILEPRS,"r")) == NULL)
      return(FALSE);

   while( fgets(stringa,80,fp) != NULL )
   {

/* elimino l'eventuale new line sulla stringa */
      pnl = strstr(stringa,"\n");
      if( pnl != NULL)
         *pnl = 0;

/* interpreto le righe */
      if( strlen(stringa) )
      {

/* tutto si basa sul riconoscimento delle KEYWORD */
        sscanf(stringa,"%s",key);

        /* inizia la descrizione di una versione */
        if( !strcmp(key,"D") )
        {
           svers = realloc(svers,sizeof(VERSIONI ) * (nver+1) );
           sscanf(stringa,"%s%s%s%s%s",spare,svers[nver].ver,svers[nver].date,spare,svers[nver].user);
           nver++;
        }
        else if( !strcmp(key,"MRs:") )
           tipo = MRS;
        else if( !strcmp(key,"COMMENTS:") )
           tipo = COMMENTS;
        else
        {
           if( tipo == MRS)
           {
             strcpy(svers[nver].mrs,stringa);
           }
           else if( tipo == COMMENTS )
           {
              strncat(svers[nver].comments,stringa,MAXLENCOMM-strlen(svers[nver].comments) );
           }
           else
             strcpy(filname,stringa);
        }
      }
   }

   fclose(fp);
   return(TRUE);
}


/*
   ogni item viene creato dalla concatenazione di
   ver data user comment 
   devo creare nitem = nver
*/
crea_item()
{
   char result[MAXLENCOMM+(MAXLEN*3)];
   int i;
   XmString xstring;

 
   for(i=nver-1;i>=0;i--)
   {
      strcpy(result,"");
      sprintf(result,"%s %s %s %s",svers[i].ver,svers[i].date,svers[i].user,svers[i].comments);
      xstring = XmStringCreateSimple(result);
      XmListAddItem(ListVersion1,xstring,1); 
   }
}






int get_version_from_cstring(char *strin,char *strout)
{
  int i;
  char *ini=NULL,*fin=NULL;
  Boolean stop = False;

/*
   si suppone che la stringa di input abbia il formato
   "  5.4 data time user"
*/

  for(i=0;i<strlen(strin);i++)
     if( strin[i] == '*' )
        strin[i] = ' ';   

  for(i=0;i<strlen(strin);i++)
  {
     /* setto l'inizio dopo i blank iniziali */
     if( strin[i] != ' ' && !stop )
     {
        ini = &(strin[i]);
        printf("ini =%s\n",ini);
        stop = True;
     }
       
     /* setto la fine al primo blank successivo a inizio */  
     if( ini != NULL && strin[i] == ' ')
     {
        fin = &(strin[i]);
        *fin = 0;
        printf("fin =%s\n",fin);
        break;
     }
  }

/* copio i caratteri individuati */
  strncpy(strout,ini,(fin-ini)+1);

}


int esiste_sccsdir(char *lpath) 
{
   char sccsdir[100];

   strcpy(sccsdir,lpath);
   strcat(sccsdir,"/SCCS");
   if( chdir(sccsdir) != 0 ) /* la directory non e' accessibile */
      return(False);

   chdir(lpath);
   return(True);
}

#ifdef OLD_VERS
void leggi_lS01(FILE *fp)
{
   char str[100],result[200],*appo;
   int n;
   XmString xstring;
   char vers[100];
   char comm[100];
   int reiniz;          /* =1 se riga vuota  =0 se c'e qualcosa*/
   int commento;        /* =0 riga di versione =1 riga di commento */  

/* leggo il file l.S01 generato con il comando sccs get -l S01 
   il quale e' strutturato cosi'
      riga della versione del file
      riga di commento (opzionale) questa riga puo' non esserci
      riga (possono essere anche di piu') con 1 solo char 
*/
   reiniz = 0;
   commento = 0;
   n=0;
   while( fgets(str,80,fp) != NULL)
   {
/* ho letto una riga con 1 solo char */
      if( strlen(str) == 1)
         reiniz = 1;
      else 
         reiniz = 0;

/* creo l'item */ 
      if( reiniz && commento)
      {
            appo = strchr(vers,'\n');
            if( appo )
               *appo = ' ';
            appo = strchr(vers,'\t');
            if( appo )
              *appo = ' ';
            appo = strchr(comm,'\n');
            if( appo )
               *appo = ' ';
            appo = strchr(comm,'\t');
            if( appo )
               *appo = ' ';
 
         sprintf(result,"%s  - %s",vers,comm);

         xstring = XmStringCreateSimple(result);
         XmListAddItem(ListVersion1,xstring,1); 
         strcpy(result,"");

         commento = 0;

      }
       
/* se ho letto qualcosa */  
      if( !reiniz )
      {

/* ho un commento se ho gia' letto la versione */
         if( commento )
            strcpy(comm,str);

/* ho una versione o all'inizio della lettura del file o
   se ho gia' costruito l'item cioe' ho letto una riga vuota
   quindi la prossima riga sara' sicuramente una versione  
*/
         if( !commento ) 
         {  
            strcpy(vers,str);
            commento = 1;
         }

      }
      else
      {
         strcpy(vers,"");
         strcpy(comm,""); 
      }

    }      
}

void riempi_lista()
{
   FILE *fp;
  
   if( (fp = fopen("l.S01","r")) != NULL)
      leggi_lS01(fp);
  
   fclose(fp);
}

#endif  /*OLD VERSIONE */


int rsetta_edit_menu( char *pathq )
{

    extern int check_file_status();
    char pathfile[256];
    int stato;

    sprintf(pathfile,"%s/S01",pathq);

    stato = check_file_status(pathfile);   
 
    switch( stato )
    {
       case FALSE:
          set_something_val(SCCSedit,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSsave,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSunedit,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSinfo,XmNsensitive, (XtArgVal) True);
       break;
       case R_OK:
          set_something_val(SCCSedit,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSsave,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSunedit,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSinfo,XmNsensitive, (XtArgVal) True);
       break;
       case W_OK:
          set_something_val(SCCSedit,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSsave,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSunedit,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSinfo,XmNsensitive, (XtArgVal) True);
       break; 
       case (W_OK+R_OK):
          set_something_val(SCCSedit,XmNsensitive, (XtArgVal) False);
          set_something_val(SCCSsave,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSunedit,XmNsensitive, (XtArgVal) True);
          set_something_val(SCCSinfo,XmNsensitive, (XtArgVal) True);
       break; 
    } 

}

int setta_popup_menu( char *pathf )
{

    char appodir[256];

    sprintf(appodir,"%s/SCCS",pathf);

    if( esiste_sccsdir(pathf) == True )
    { 
      
       set_something_val( SCCScreate,XmNsensitive, (XtArgVal) False );
       set_something_val( SCCSedit,XmNsensitive, (XtArgVal) True );
       set_something_val( SCCSsave,XmNsensitive, (XtArgVal) True );
       set_something_val( SCCSunedit,XmNsensitive, (XtArgVal) True );
       set_something_val( SCCSinfo,XmNsensitive, (XtArgVal) True );

       rsetta_edit_menu(pathf);
  
    }
    else
    {
       set_something_val( SCCScreate,XmNsensitive, (XtArgVal) True );
       set_something_val( SCCSedit,XmNsensitive, (XtArgVal) False );
       set_something_val( SCCSsave,XmNsensitive, (XtArgVal) False );
       set_something_val( SCCSunedit,XmNsensitive, (XtArgVal) False );
       set_something_val( SCCSinfo,XmNsensitive, (XtArgVal) False );
    }
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	singleSelectionCB_ListVersion1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	char cstring[STRLONG];
	XmString item;
	
	
	/* recupero l'item selezionato tramite gli arg della callback */
	
	item = XmStringCopy( ((XmListCallbackStruct *)UxCallbackArg)->item );
	strcpy(cstring,extract_string(item));
	
	printf("Selezione =%s\n",cstring);
	
	/* recupero dalla stringa dell'item la versione */
	get_version_from_cstring(cstring,version);
	
	printf("versione=%s\n",version);
	
	
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCScreate( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   int create_sccsS01();
	
	   printf("sccs create S01\n");
	   create_sccsS01();
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSgetB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	  extern Boolean get_sccsS01();
	  printf("sccs get -r%s S01",version); 
	
	   get_sccsS01(version);
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSeditB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   extern Boolean edit_sccsS01();
	
	   printf("sccs edit -r%s S01\n",version);
	
	   edit_sccsS01(version);
	
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSdelgetB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   extern Boolean delta_sccsS01();
	   extern Boolean get_sccsS01();
	
	   delta_sccsS01();
	   get_sccsS01(version); 
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSdeleditB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   extern Boolean edit_sccsS01();
	   extern Boolean delta_sccsS01();
	
	   delta_sccsS01();
	   edit_sccsS01(version);
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSunedit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   extern Boolean unedit_sccsS01();
	
	   unedit_sccsS01();
	
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_SCCSinfo( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	   extern Boolean info_sccsS01();
	
	   info_sccsS01();
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

static void	activateCB_cancelButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCSCCSGESTnew         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSCCSGESTnewContext;
	UxSCCSGESTnewContext = UxContext =
			(_UxCSCCSGESTnew *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(XtParent(UxWidget));
	}
	UxSCCSGESTnewContext = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_SCCSGESTnew()
{
	Widget	menu6_shell;
	Widget	SCCSmenuEdit_shell;
	Widget	SCCSmenuDelta_shell;

	SCCSGESTnew = XtVaCreatePopupShell( "SCCSGESTnew",
			topLevelShellWidgetClass, UxTopLevel,
			XmNheight, 450,
			XmNwidth, 400,
			XmNy, 360,
			XmNx, 410,
			NULL );

	UxPutContext( SCCSGESTnew, (char *) UxSCCSGESTnewContext );

	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass, SCCSGESTnew,
			XmNheight, 40,
			XmNwidth, 70,
			XmNy, 300,
			XmNx, 170,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form3, (char *) UxSCCSGESTnewContext );

	scrolledWindow3 = XtVaCreateManagedWidget( "scrolledWindow3",
			xmScrolledWindowWidgetClass, form3,
			XmNbottomOffset, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopOffset, 50,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 50,
			XmNx, 20,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow3, (char *) UxSCCSGESTnewContext );

	ListVersion1 = XtVaCreateManagedWidget( "ListVersion1",
			xmListWidgetClass, scrolledWindow3,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNheight, 260,
			XmNwidth, 320,
			NULL );

	UxPutContext( ListVersion1, (char *) UxSCCSGESTnewContext );

	menu6_shell = XtVaCreatePopupShell ("menu6_shell",
			xmMenuShellWidgetClass, ListVersion1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu6 = XtVaCreateWidget( "menu6",
			xmRowColumnWidgetClass, menu6_shell,
			XmNmenuAccelerator, "<KeyUp>F4",
			XmNrowColumnType, XmMENU_POPUP,
			NULL );

	UxPutContext( menu6, (char *) UxSCCSGESTnewContext );

	menu6_p1_title = XtVaCreateManagedWidget( "menu6_p1_title",
			xmLabelWidgetClass, menu6,
			RES_CONVERT( XmNlabelString, "SCCS" ),
			NULL );

	UxPutContext( menu6_p1_title, (char *) UxSCCSGESTnewContext );

	SCCSseparator = XtVaCreateManagedWidget( "SCCSseparator",
			xmSeparatorGadgetClass, menu6,
			NULL );

	UxPutContext( SCCSseparator, (char *) UxSCCSGESTnewContext );

	SCCScreate = XtVaCreateManagedWidget( "SCCScreate",
			xmPushButtonWidgetClass, menu6,
			RES_CONVERT( XmNlabelString, "Create" ),
			NULL );

	UxPutContext( SCCScreate, (char *) UxSCCSGESTnewContext );

	SCCSmenuEdit_shell = XtVaCreatePopupShell ("SCCSmenuEdit_shell",
			xmMenuShellWidgetClass, menu6,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	SCCSmenuEdit = XtVaCreateWidget( "SCCSmenuEdit",
			xmRowColumnWidgetClass, SCCSmenuEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( SCCSmenuEdit, (char *) UxSCCSGESTnewContext );

	SCCSgetB = XtVaCreateManagedWidget( "SCCSgetB",
			xmPushButtonWidgetClass, SCCSmenuEdit,
			RES_CONVERT( XmNlabelString, "Get (Read Only)" ),
			NULL );

	UxPutContext( SCCSgetB, (char *) UxSCCSGESTnewContext );

	SCCSeditB = XtVaCreateManagedWidget( "SCCSeditB",
			xmPushButtonWidgetClass, SCCSmenuEdit,
			RES_CONVERT( XmNlabelString, "Edit (read/write)" ),
			NULL );

	UxPutContext( SCCSeditB, (char *) UxSCCSGESTnewContext );

	SCCSedit = XtVaCreateManagedWidget( "SCCSedit",
			xmCascadeButtonWidgetClass, menu6,
			XmNsubMenuId, SCCSmenuEdit,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( SCCSedit, (char *) UxSCCSGESTnewContext );

	SCCSmenuDelta_shell = XtVaCreatePopupShell ("SCCSmenuDelta_shell",
			xmMenuShellWidgetClass, menu6,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	SCCSmenuDelta = XtVaCreateWidget( "SCCSmenuDelta",
			xmRowColumnWidgetClass, SCCSmenuDelta_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( SCCSmenuDelta, (char *) UxSCCSGESTnewContext );

	SCCSdelgetB = XtVaCreateManagedWidget( "SCCSdelgetB",
			xmPushButtonWidgetClass, SCCSmenuDelta,
			RES_CONVERT( XmNlabelString, "Delget (delta+get)" ),
			NULL );

	UxPutContext( SCCSdelgetB, (char *) UxSCCSGESTnewContext );

	SCCSdeleditB = XtVaCreateManagedWidget( "SCCSdeleditB",
			xmPushButtonWidgetClass, SCCSmenuDelta,
			RES_CONVERT( XmNlabelString, "Deledit (delta+edit)" ),
			NULL );

	UxPutContext( SCCSdeleditB, (char *) UxSCCSGESTnewContext );

	SCCSsave = XtVaCreateManagedWidget( "SCCSsave",
			xmCascadeButtonGadgetClass, menu6,
			XmNsubMenuId, SCCSmenuDelta,
			RES_CONVERT( XmNlabelString, "Save" ),
			NULL );

	UxPutContext( SCCSsave, (char *) UxSCCSGESTnewContext );

	SCCSunedit = XtVaCreateManagedWidget( "SCCSunedit",
			xmPushButtonWidgetClass, menu6,
			RES_CONVERT( XmNlabelString, "Unedit" ),
			NULL );

	UxPutContext( SCCSunedit, (char *) UxSCCSGESTnewContext );

	SCCSinfo = XtVaCreateManagedWidget( "SCCSinfo",
			xmPushButtonWidgetClass, menu6,
			RES_CONVERT( XmNlabelString, "Info" ),
			NULL );

	UxPutContext( SCCSinfo, (char *) UxSCCSGESTnewContext );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, form3,
			XmNbottomOffset, 5,
			XmNbottomWidget, scrolledWindow3,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "SCCS Version of Sim1" ),
			XmNheight, 30,
			XmNwidth, 320,
			XmNy, 10,
			XmNx, 20,
			NULL );

	UxPutContext( label2, (char *) UxSCCSGESTnewContext );

	cancelButton1 = XtVaCreateManagedWidget( "cancelButton1",
			xmPushButtonWidgetClass, form3,
			XmNrightOffset, 20,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopWidget, scrolledWindow3,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNheight, 30,
			XmNwidth, 80,
			XmNy, 370,
			XmNx, 20,
			NULL );

	UxPutContext( cancelButton1, (char *) UxSCCSGESTnewContext );

	XtAddCallback( SCCSGESTnew, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( ListVersion1, XmNsingleSelectionCallback,
			singleSelectionCB_ListVersion1,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCScreate, XmNactivateCallback,
			activateCB_SCCScreate,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSgetB, XmNactivateCallback,
			activateCB_SCCSgetB,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSeditB, XmNactivateCallback,
			activateCB_SCCSeditB,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSdelgetB, XmNactivateCallback,
			activateCB_SCCSdelgetB,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSdeleditB, XmNactivateCallback,
			activateCB_SCCSdeleditB,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSunedit, XmNactivateCallback,
			activateCB_SCCSunedit,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( SCCSinfo, XmNactivateCallback,
			activateCB_SCCSinfo,
			(XtPointer) UxSCCSGESTnewContext );

	XtAddCallback( cancelButton1, XmNactivateCallback,
			activateCB_cancelButton1,
			(XtPointer) UxSCCSGESTnewContext );


	XtAddEventHandler( ListVersion1, ButtonPressMask,
			False, _UxSCCSGESTnewMenuPost, menu6 );

	return ( SCCSGESTnew );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_SCCSGESTnew( _Uxpath )
	unsigned char	*_Uxpath;
{
	Widget                  rtrn;
	_UxCSCCSGESTnew         *UxContext;

	UxSCCSGESTnewContext = UxContext =
		(_UxCSCCSGESTnew *) XtMalloc( sizeof(_UxCSCCSGESTnew) );

	path = _Uxpath;
	{
		char wdir[100];
		
		
		if( get_working_directory(wdir) == FALSE)
		   return(NULL);
		
		/*
		system("sccs get -l S01");
		*/
		
		nver=0;
		svers=NULL;
		rtrn = _Uxbuild_SCCSGESTnew();

		/*
		riempi_lista();
		*/
		crea_file_prs(path,"S01");
		read_file_prs();
		crea_item();
		
		setta_popup_menu(path);
		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_SCCSGESTnew( _Uxpath )
	unsigned char	*_Uxpath;
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_SCCSGESTnew( _Uxpath );

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

