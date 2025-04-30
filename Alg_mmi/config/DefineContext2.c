
/*******************************************************************************
       DefineContext2.c
       (Generated from interface file DefineContext2.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo DefineContext2.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)DefineContext2.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <unistd.h>
#ifndef LINUX
#include <sys/access.h>
#endif
#include "config.h"

int operaz;

extern Arg args[];
extern Cardinal lnargs;
extern PAGEDIT_CONTEXT *pagedit;

void display_context_data( );
void aggiorna_ctx( );
extern void TopMenuSetInsensitive( );
extern void TopMenuSetSensitive( );


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
	Widget	UxDefineContext;
	Widget	Uxform7;
	Widget	UxPagesLabel;
	Widget	UxPagesPath;
	Widget	UxObjectLibraryLabel;
	Widget	UxObjectLibraryPath;
	Widget	UxAnimateIconLibraryLabel;
	Widget	UxAnimateIconLibraryPath;
	Widget	UxSimulatorLabel;
	Widget	UxSimulatorPath;
	Widget	UxConfirmButton;
	Widget	UxNoConfirmButton;
	Widget	UxDisplayLabel;
	Widget	UxProjectDescriptionLabel;
	Widget	UxProjectDescription;
	Widget	UxDisplayList;
	Widget	UxDisplayLabel1;
	Widget	UxHostname;
	Widget	UxHostnameS;
	Widget	UxDisplayLabel2;
	PAGEDIT_CONTEXT	*Uxctxappo;
	unsigned char	UxContextName[150];
	unsigned char	*UxnomeCtx;
	int	Uxi_op;
} _UxCDefineContext;

static _UxCDefineContext       *UxDefineContextContext;
#define DefineContext           UxDefineContextContext->UxDefineContext
#define form7                   UxDefineContextContext->Uxform7
#define PagesLabel              UxDefineContextContext->UxPagesLabel
#define PagesPath               UxDefineContextContext->UxPagesPath
#define ObjectLibraryLabel      UxDefineContextContext->UxObjectLibraryLabel
#define ObjectLibraryPath       UxDefineContextContext->UxObjectLibraryPath
#define AnimateIconLibraryLabel UxDefineContextContext->UxAnimateIconLibraryLabel
#define AnimateIconLibraryPath  UxDefineContextContext->UxAnimateIconLibraryPath
#define SimulatorLabel          UxDefineContextContext->UxSimulatorLabel
#define SimulatorPath           UxDefineContextContext->UxSimulatorPath
#define ConfirmButton           UxDefineContextContext->UxConfirmButton
#define NoConfirmButton         UxDefineContextContext->UxNoConfirmButton
#define DisplayLabel            UxDefineContextContext->UxDisplayLabel
#define ProjectDescriptionLabel UxDefineContextContext->UxProjectDescriptionLabel
#define ProjectDescription      UxDefineContextContext->UxProjectDescription
#define DisplayList             UxDefineContextContext->UxDisplayList
#define DisplayLabel1           UxDefineContextContext->UxDisplayLabel1
#define Hostname                UxDefineContextContext->UxHostname
#define HostnameS               UxDefineContextContext->UxHostnameS
#define DisplayLabel2           UxDefineContextContext->UxDisplayLabel2
#define ctxappo                 UxDefineContextContext->Uxctxappo
#define ContextName             UxDefineContextContext->UxContextName
#define nomeCtx                 UxDefineContextContext->UxnomeCtx
#define i_op                    UxDefineContextContext->Uxi_op



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	creat_DefineContext();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void display_context_data(PAGEDIT_CONTEXT *ctx)
{
   char *listaDisplay;
   int i;

/* creo le compound string dei dati e setto
   le label della interfaccia con i valori */

   
   XmTextFieldSetString( ProjectDescription, ctx->descrizione );
   XmTextFieldSetString( PagesPath,XlConvDirVms(ctx->pages));
   XmTextFieldSetString( ObjectLibraryPath, XlConvDirVms(ctx->objectLibrary));
   XmTextFieldSetString( AnimateIconLibraryPath,XlConvDirVms( ctx->animatedIconLibrary));
   XmTextFieldSetString( SimulatorPath, XlConvDirVms(ctx->simulator));
   XmTextFieldSetString( Hostname, ctx->hostname);
   XmTextFieldSetString( HostnameS, ctx->hostnameS);

   if( (listaDisplay = alloca_memoria(ctxappo->num_display*MAXCHARDISPLAY,sizeof(char)) ) == NULL)
          return;

   for(i=0;i<ctxappo->num_display;i++)
   {
       strcat(listaDisplay,ctx->elenco_display[i]);
       strcat(listaDisplay," ");
   }

   XmTextFieldSetString( DisplayList, listaDisplay);
  
}

void aggiorna_ctx(PAGEDIT_CONTEXT *ctx)
{
      char *listaDisplay=NULL,*save=NULL;
      char appo[30];
      int i,ndisplay = 0;

     
      strcpy(ctx->descrizione,XmTextFieldGetString(ProjectDescription));
      strcpy(ctx->pages,XlUnconvDirVms(XmTextFieldGetString(PagesPath)));
      strcpy(ctx->objectLibrary,XlUnconvDirVms(XmTextFieldGetString(ObjectLibraryPath)));
      strcpy(ctx->animatedIconLibrary,XlUnconvDirVms(XmTextFieldGetString(AnimateIconLibraryPath)));
      strcpy(ctx->simulator,XlUnconvDirVms(XmTextFieldGetString(SimulatorPath)));
      strcpy(ctx->hostname,XmTextFieldGetString(Hostname));
      strcpy(ctx->hostnameS,XmTextFieldGetString(HostnameS));
      listaDisplay =  XmTextFieldGetString(DisplayList);
  
      save = listaDisplay;


      listaDisplay[strlen(listaDisplay)+1] = '\0';

      while( (sscanf(listaDisplay,"%s",appo)) > 0)
      {
         printf("listaDisplay = %s\n",listaDisplay);
         printf("letto appo=%s\n",appo);


         if(ndisplay == 0)
         {
            if((ctx->elenco_display = (char**)alloca_memoria(1,sizeof(char *))) == NULL)
               return;
         }
         else
            ctx->elenco_display = realloc(ctx->elenco_display,(ndisplay+1)*sizeof(char *));

         if( (ctx->elenco_display[ndisplay] = alloca_memoria(strlen(appo)+2,sizeof(char))) == NULL)
            return;

         strcpy(ctx->elenco_display[ndisplay],appo);

         listaDisplay += strlen(appo)+1;

         while( *listaDisplay == ' ')
            listaDisplay++;


         ndisplay++;
      }
      ctx->num_display = ndisplay;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	mapCB_form7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCDefineContext       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDefineContextContext;
	UxDefineContextContext = UxContext =
			(_UxCDefineContext *) UxGetContext( UxWidget );
	{
	
	}
	UxDefineContextContext = UxSaveCtx;
}

static	void	activateCB_ConfirmButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCDefineContext       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDefineContextContext;
	UxDefineContextContext = UxContext =
			(_UxCDefineContext *) UxGetContext( UxWidget );
	{
	        {
	        extern void richiedi_change_database();
	        extern int setPageditEnv();
	        extern int display_resources();
	        extern int new_save_context();
	        extern swidget ContextSelection;
	        extern PAGEDIT_CONTEXT *pagedit;
	        extern int display_resources();
	        PAGEDIT_CONTEXT pagappo;
	        char *listaDisplay,appo[30];
	        int num_display = 0;
	        FILE *fp;
	        char mesg[200];
	
	        switch(operaz)
	        {
	           case LOAD_CONTEXT:
	           break;
	           case DEF_CONTEXT:
	              strcpy(pagappo.descrizione,XmTextFieldGetString(ProjectDescription));
	              strcpy(pagappo.pages,XlUnconvDirVms(XmTextFieldGetString(PagesPath)));
	              strcpy(pagappo.objectLibrary,XlUnconvDirVms(XmTextFieldGetString(ObjectLibraryPath)));
	              strcpy(pagappo.animatedIconLibrary,XlUnconvDirVms(XmTextFieldGetString(AnimateIconLibraryPath)));
	              strcpy(pagappo.simulator,XlUnconvDirVms(XmTextFieldGetString(SimulatorPath)));
	              strcpy(pagappo.hostname,XmTextFieldGetString(Hostname));
	              strcpy(pagappo.hostnameS,XmTextFieldGetString(HostnameS));
	              listaDisplay =  XmTextFieldGetString(DisplayList);
	
	
	              while( (sscanf(listaDisplay,"%s",appo)) > 0)
	              {
	                 num_display++;
	                 listaDisplay += strlen(appo);
	                 printf("display name %s num.%d\n",appo,num_display);
	              }
	
	              if( (fp=fopen(ContextName,"w")) == NULL)
	              {
	                 printf("errore fopen %s\n",ContextName);
	                 return;
	              }
	
	/* 21-10-96 Carlo
	
	test accesso direttori 
	*/
	
	if( verifica_direttori_context(&pagappo) != True )
	   return;
	
	
	
	
	        /* salvo su file il context nuovo */
	              fprintf(fp,"%s","*objectLibraries:");
	              fprintf(fp,"%s",pagappo.objectLibrary);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*animatedIconLibraries:");
	              fprintf(fp,"%s",pagappo.animatedIconLibrary);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*numDisplay:");
	              fprintf(fp,"%d",num_display);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*displayList:");
	              fprintf(fp,"%s",listaDisplay);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*pag_num:");
	              fprintf(fp,"%d",0);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*page_list:");
	              fprintf(fp,"%s","");
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*iconlib_num:");
	              fprintf(fp,"%d",0);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*iconlib_list:");
	              fprintf(fp,"%s","");
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*iconlib_label:");
	              fprintf(fp,"%s","");
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*simulator:");
	              fprintf(fp,"%s",pagappo.simulator);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*pages:");
	              fprintf(fp,"%s",pagappo.pages);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*description:");
	              fprintf(fp,"%s",pagappo.descrizione);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*hostName:");
	              fprintf(fp,"%s",pagappo.hostname);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s","*hostNameS:");
	              fprintf(fp,"%s",pagappo.hostnameS);
	              fprintf(fp,"%s","\n");
	              fprintf(fp,"%s:","*nextTagPag: 00");
	              fprintf(fp,"%s","\n");
	              
	              fclose(fp);
	
	        /* effettuo la load del Context appena creato */
	              if(ContextLoad(ContextName,&pagedit))
	              {
	                 display_resources(pagedit);
	                 TopMenuSetSensitive();
	                 richiedi_change_database();
	              }
	              else
	              {
	                 sprintf(mesg,"Context File Not Found %s.\n",pagedit->fileContext);
	                 show_message(mesg);
	                 TopMenuSetInsensitive();
	              }
	           break;
	           case MOD_CONTEXT:
	           {
	/* provo a non farlo 22-10-96
	              char *appo,*start;
	
	              appo = start = ContextName;
	
	              while(*appo != '\0')
	              {
	                 if( *appo == '/')
	                 {
	                    appo++;
	                    start =  appo;
	                 }
	                 else
	                    appo++;
	              }
	
	             strcpy(ctxappo->fileContext,start);
	*/
	             aggiorna_ctx(ctxappo);
	
	/* 21-10-96 Carlo 
	    verifico l'accessibilita dei direttori
	*/
	if( verifica_direttori_context( ctxappo ) != True)
	   return;             
	
	
	        /* salvo il context modificato */
	             new_save_context(ctxappo);
	        /* ed effettuo la load del nuovo context */
	             if(ContextLoad(ContextName,&pagedit))
	             {
	                display_resources(pagedit);
	                TopMenuSetSensitive();
	                richiedi_change_database();
	             }
	           }
	           break;
	           }
	
	           UxDestroyInterface(DefineContext);
	        }
	        
	
	}
	UxDefineContextContext = UxSaveCtx;
}

static	void	activateCB_NoConfirmButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCDefineContext       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDefineContextContext;
	UxDefineContextContext = UxContext =
			(_UxCDefineContext *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(DefineContext);
	}
	UxDefineContextContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_DefineContext()
{


	/* Creation of DefineContext */
	DefineContext = XtVaCreatePopupShell( "DefineContext",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 548,
			XmNy, 122,
			XmNwidth, 600,
			XmNheight, 550,
			NULL );
	UxPutContext( DefineContext, (char *) UxDefineContextContext );


	/* Creation of form7 */
	form7 = XtVaCreateManagedWidget( "form7",
			xmFormWidgetClass,
			DefineContext,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNx, 100,
			XmNy, 150,
			XmNwidth, 560,
			XmNheight, 470,
			NULL );
	XtAddCallback( form7, XmNmapCallback,
		(XtCallbackProc) mapCB_form7,
		(XtPointer) UxDefineContextContext );

	UxPutContext( form7, (char *) UxDefineContextContext );


	/* Creation of PagesLabel */
	PagesLabel = XtVaCreateManagedWidget( "PagesLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 110,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Pages" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( PagesLabel, (char *) UxDefineContextContext );


	/* Creation of PagesPath */
	PagesPath = XtVaCreateManagedWidget( "PagesPath",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 110,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( PagesPath, (char *) UxDefineContextContext );


	/* Creation of ObjectLibraryLabel */
	ObjectLibraryLabel = XtVaCreateManagedWidget( "ObjectLibraryLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 150,
			XmNwidth, 170,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Object Library" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( ObjectLibraryLabel, (char *) UxDefineContextContext );


	/* Creation of ObjectLibraryPath */
	ObjectLibraryPath = XtVaCreateManagedWidget( "ObjectLibraryPath",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 150,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( ObjectLibraryPath, (char *) UxDefineContextContext );


	/* Creation of AnimateIconLibraryLabel */
	AnimateIconLibraryLabel = XtVaCreateManagedWidget( "AnimateIconLibraryLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 200,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Animated Icon Library" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( AnimateIconLibraryLabel, (char *) UxDefineContextContext );


	/* Creation of AnimateIconLibraryPath */
	AnimateIconLibraryPath = XtVaCreateManagedWidget( "AnimateIconLibraryPath",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 200,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( AnimateIconLibraryPath, (char *) UxDefineContextContext );


	/* Creation of SimulatorLabel */
	SimulatorLabel = XtVaCreateManagedWidget( "SimulatorLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 250,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Simulator" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( SimulatorLabel, (char *) UxDefineContextContext );


	/* Creation of SimulatorPath */
	SimulatorPath = XtVaCreateManagedWidget( "SimulatorPath",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 250,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( SimulatorPath, (char *) UxDefineContextContext );


	/* Creation of ConfirmButton */
	ConfirmButton = XtVaCreateManagedWidget( "ConfirmButton",
			xmPushButtonWidgetClass,
			form7,
			XmNx, 30,
			XmNy, 480,
			XmNwidth, 70,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Ok" ),
			NULL );
	XtAddCallback( ConfirmButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_ConfirmButton,
		(XtPointer) UxDefineContextContext );

	UxPutContext( ConfirmButton, (char *) UxDefineContextContext );


	/* Creation of NoConfirmButton */
	NoConfirmButton = XtVaCreateManagedWidget( "NoConfirmButton",
			xmPushButtonWidgetClass,
			form7,
			XmNx, 120,
			XmNy, 480,
			XmNwidth, 70,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			NULL );
	XtAddCallback( NoConfirmButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_NoConfirmButton,
		(XtPointer) UxDefineContextContext );

	UxPutContext( NoConfirmButton, (char *) UxDefineContextContext );


	/* Creation of DisplayLabel */
	DisplayLabel = XtVaCreateManagedWidget( "DisplayLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 300,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Display List" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( DisplayLabel, (char *) UxDefineContextContext );


	/* Creation of ProjectDescriptionLabel */
	ProjectDescriptionLabel = XtVaCreateManagedWidget( "ProjectDescriptionLabel",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 70,
			XmNwidth, 130,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Description" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( ProjectDescriptionLabel, (char *) UxDefineContextContext );


	/* Creation of ProjectDescription */
	ProjectDescription = XtVaCreateManagedWidget( "ProjectDescription",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 70,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( ProjectDescription, (char *) UxDefineContextContext );


	/* Creation of DisplayList */
	DisplayList = XtVaCreateManagedWidget( "DisplayList",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 300,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( DisplayList, (char *) UxDefineContextContext );


	/* Creation of DisplayLabel1 */
	DisplayLabel1 = XtVaCreateManagedWidget( "DisplayLabel1",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 350,
			XmNwidth, 180,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Simulator machine" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( DisplayLabel1, (char *) UxDefineContextContext );


	/* Creation of Hostname */
	Hostname = XtVaCreateManagedWidget( "Hostname",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 350,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( Hostname, (char *) UxDefineContextContext );


	/* Creation of HostnameS */
	HostnameS = XtVaCreateManagedWidget( "HostnameS",
			xmTextFieldWidgetClass,
			form7,
			XmNx, 210,
			XmNy, 400,
			XmNwidth, 340,
			XmNheight, 30,
			XmNmarginHeight, 1,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( HostnameS, (char *) UxDefineContextContext );


	/* Creation of DisplayLabel2 */
	DisplayLabel2 = XtVaCreateManagedWidget( "DisplayLabel2",
			xmLabelWidgetClass,
			form7,
			XmNx, 20,
			XmNy, 400,
			XmNwidth, 170,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Scada machine" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( DisplayLabel2, (char *) UxDefineContextContext );


	XtAddCallback( DefineContext, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxDefineContextContext);


	return ( DefineContext );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	creat_DefineContext( _UxnomeCtx, _Uxi_op )
	unsigned char	*_UxnomeCtx;
	int	_Uxi_op;
{
	Widget                  rtrn;
	_UxCDefineContext       *UxContext;

	UxDefineContextContext = UxContext =
		(_UxCDefineContext *) UxNewContext( sizeof(_UxCDefineContext), False );

	nomeCtx = _UxnomeCtx;
	i_op = _Uxi_op;

	{
		operaz = i_op;
		strcpy(ContextName,nomeCtx);
		rtrn = _Uxbuild_DefineContext();

		if(i_op == MOD_CONTEXT)
		{
		
		   ctxappo = pagedit; 
		   display_context_data(ctxappo);
		
		}
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

