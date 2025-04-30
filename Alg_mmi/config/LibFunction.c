
/*******************************************************************************
       LibFunction.c
       (Generated from interface file LibFunction.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo LibFunction.i
   tipo 
   release 5.2
   data 3/18/96
   reserved @(#)LibFunction.i	5.2
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
extern PAGEDIT_CONTEXT *pagedit;

int library_wrstd( );
void library_gen( );


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
	Widget	Uxform6;
	Widget	UxpushButton2;
	Widget	UxpushButton3;
	Widget	UxpushButton4;
	Widget	Uxlabel1;
	Widget	Uxlabel2;
	Widget	Uxlabel3;
	Widget	UxFPagName;
	Widget	UxFLibName;
	Widget	UxFLabName;
	Widget	Uxseparator1;
	int	UxPaginaSel;
} _UxCLibFunction;

static _UxCLibFunction         *UxLibFunctionContext;
#define form6                   UxLibFunctionContext->Uxform6
#define pushButton2             UxLibFunctionContext->UxpushButton2
#define pushButton3             UxLibFunctionContext->UxpushButton3
#define pushButton4             UxLibFunctionContext->UxpushButton4
#define label1                  UxLibFunctionContext->Uxlabel1
#define label2                  UxLibFunctionContext->Uxlabel2
#define label3                  UxLibFunctionContext->Uxlabel3
#define FPagName                UxLibFunctionContext->UxFPagName
#define FLibName                UxLibFunctionContext->UxFLibName
#define FLabName                UxLibFunctionContext->UxFLabName
#define separator1              UxLibFunctionContext->Uxseparator1
#define PaginaSel               UxLibFunctionContext->UxPaginaSel


Widget	LibFunction;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_LibFunction();
extern Boolean file_exist(char *);
extern void SetItemString();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*-----------------------------------
 *
 * scrittura standard icon library 
 *
 *-----------------------------------*/

int library_wrstd(char *libname)
{
   extern PAGEDIT_CONTEXT *pagedit;
   FILE *fp;
   Boolean heregen=False,already_exists=False;
   char libfile[MAXLENFILENAME];
   extern void WriteTemplateStdLib();

   strcpy(libfile,pagedit->objectLibrary);
   strcat(libfile,"/");
   strcat(libfile,libname);   
   strcat(libfile,LIB_EXT);
   strcpy(libfile,XlConvPathVms(libfile)); 


   if( file_exist(libfile) )
      already_exists = True;

   if( (fp = fopen(libfile,"w")) == NULL)
   {
      SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Standard Library File Not Available For Write",NULL,False,NULL,False,NULL);
//      fclose(fp);
      return(SEVERE);    
   }

   WriteTemplateStdLib(fp);  

   fclose(fp);

   return(already_exists);
}

/*--------------------------------------------------
 * 
 * library_gen
 *
 * routine di generazione icon library
 *
 *---------------------------------------------------*/

void library_gen(char *nmlib,char *label)
{
   extern Boolean ContextModified;
   extern void removefromlist();
   extern PAGEDIT_CONTEXT *pagedit;
   char filelib[MAXLENFILENAME];
   FILE *fp;
   XmString cstring;
   int i;
   if( (XlGetenv(ENVLIB) == NULL) )
      return;
   if(nmlib == NULL)
      return;
/* costruisco la path file name */
   strcpy(filelib,getenv(ENVLIB));
   strcat(filelib,"/");
   strcat(filelib,nmlib);
   strcat(filelib,LIB_EXT);
   strcpy(filelib,XlConvPathVms(filelib));


printf("stai cercando di creare la libreria %s\n",filelib);
/* verifico se e' gia' nel context */
   for(i=0;i<pagedit->iconlib_num;i++)
   {
/* se c'e' deve esserci anche il file */
      if(!strcmp(pagedit->iconlib_list[i],nmlib))
      {
/* se non c'e' il file associato mi inc... */
         if( !file_exist(filelib) )
         {
printf("ed e' nel context ma non esiste, no good thing \n");
            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Error, File Not Found But Is Into Context Library List",NULL,False,NULL,False,NULL);
            return;
         }
         else
         {
/* se c'e' ti dico che cosa la riscrivi a fare */
printf("ed e' nel context ma gia' esiste, no good thing \n");
           SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Proposed Library Already Exist",NULL,False,NULL,False,NULL);
           return;
         }
      }
   }
/* stai creando una libreria che non e' nel context, OK */
/* devi creare il file della libreria e appicciarlo nel context */
/* la libreria da creare non deve proprio esistere */
   if( !file_exist(filelib) )
   {
       printf("il file giustamente non esiste\n");
   }
   else
   {
      SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Proposed Library Already Exist",NULL,False,NULL,False,NULL);
printf("e questo file gia' esiste, no good thing \n");
      return;
   }
/* ti concedo di scrivere la stdlib */
   if( !strcmp(nmlib,STD_LIB_NAME) )
      library_wrstd(STD_LIB_NAME);
   else
   {
/* e ti concedo pure di scrivere una libreria (vuota naturalmente */
      if( (fp = fopen(filelib,"w")) == NULL)
      {
         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Sorry, Cannot Write Library",NULL,False,NULL,False,NULL);
         fclose(fp);
         return;
      }
      fclose(fp);
   }
/* aggiorno il context */
   pagedit->iconlib_num++;
   pagedit->iconlib_list = realloc(pagedit->iconlib_list,(sizeof(char *)*pagedit->iconlib_num));
   if( (pagedit->iconlib_list[pagedit->iconlib_num-1] = alloca_memoria(strlen(nmlib)+1,sizeof(char) )) == NULL )
      return;
   strcpy(pagedit->iconlib_list[pagedit->iconlib_num-1],nmlib);
   pagedit->iconlib_label = realloc(pagedit->iconlib_label,(sizeof(char *)*pagedit->iconlib_num ));
   if( (pagedit->iconlib_label[pagedit->iconlib_num-1] = alloca_memoria(strlen(label)+1,sizeof(char) )) == NULL )
      return;
   strcpy(pagedit->iconlib_label[pagedit->iconlib_num-1],label);

/* aggiusto le liste */
   SetItemString(&cstring,pagedit->iconlib_label[pagedit->iconlib_num-1],"Library","Standard Library",0,NULL);
   add_library_item(cstring,0);

   if( new_save_context(pagedit) != True )
   {
      show_message("Error on save context\n"); 
      ContextModified = True;
   }
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibFunction         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibFunctionContext;
	UxLibFunctionContext = UxContext =
			(_UxCLibFunction *) UxGetContext( UxWidget );
	{
	extern Boolean ContextModified;
	char *pname,*lname,*label,comando[300];
	
	 
	pname = XmTextFieldGetString(FPagName);
	lname = XmTextFieldGetString(FLibName);
	label = XmTextFieldGetString(FLabName);
	
	library_gen(lname,label);
	ContextModified = True;
	UxDestroyInterface(LibFunction);
	
	
	}
	UxLibFunctionContext = UxSaveCtx;
}

static	void	activateCB_pushButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibFunction         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibFunctionContext;
	UxLibFunctionContext = UxContext =
			(_UxCLibFunction *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(LibFunction);
	}
	UxLibFunctionContext = UxSaveCtx;
}

static	void	valueChangedCB_FPagName( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCLibFunction         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxLibFunctionContext;
	UxLibFunctionContext = UxContext =
			(_UxCLibFunction *) UxGetContext( UxWidget );
	{
	XmTextFieldSetString(FLibName,"");
	XmTextFieldSetString(FLabName,"");
	}
	UxLibFunctionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_LibFunction()
{


	/* Creation of LibFunction */
	LibFunction = XtVaCreatePopupShell( "LibFunction",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 450,
			XmNy, 320,
			XmNwidth, 400,
			XmNheight, 250,
			NULL );
	UxPutContext( LibFunction, (char *) UxLibFunctionContext );


	/* Creation of form6 */
	form6 = XtVaCreateManagedWidget( "form6",
			xmFormWidgetClass,
			LibFunction,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNx, 10,
			XmNy, 20,
			XmNwidth, 230,
			XmNheight, 150,
			NULL );
	UxPutContext( form6, (char *) UxLibFunctionContext );


	/* Creation of pushButton2 */
	pushButton2 = XtVaCreateManagedWidget( "pushButton2",
			xmPushButtonWidgetClass,
			form6,
			XmNx, 30,
			XmNy, 200,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Ok" ),
			NULL );
	XtAddCallback( pushButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton2,
		(XtPointer) UxLibFunctionContext );

	UxPutContext( pushButton2, (char *) UxLibFunctionContext );


	/* Creation of pushButton3 */
	pushButton3 = XtVaCreateManagedWidget( "pushButton3",
			xmPushButtonWidgetClass,
			form6,
			XmNx, 160,
			XmNy, 200,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			NULL );
	XtAddCallback( pushButton3, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton3,
		(XtPointer) UxLibFunctionContext );

	UxPutContext( pushButton3, (char *) UxLibFunctionContext );


	/* Creation of pushButton4 */
	pushButton4 = XtVaCreateManagedWidget( "pushButton4",
			xmPushButtonWidgetClass,
			form6,
			XmNx, 280,
			XmNy, 200,
			XmNwidth, 80,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Help" ),
			NULL );
	UxPutContext( pushButton4, (char *) UxLibFunctionContext );


	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			form6,
			XmNx, 30,
			XmNy, 30,
			XmNwidth, 90,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Page Name" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label1, (char *) UxLibFunctionContext );


	/* Creation of label2 */
	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass,
			form6,
			XmNx, 30,
			XmNy, 80,
			XmNwidth, 91,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Library Name" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label2, (char *) UxLibFunctionContext );


	/* Creation of label3 */
	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass,
			form6,
			XmNx, 31,
			XmNy, 130,
			XmNwidth, 81,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Label Name" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label3, (char *) UxLibFunctionContext );


	/* Creation of FPagName */
	FPagName = XtVaCreateManagedWidget( "FPagName",
			xmTextFieldWidgetClass,
			form6,
			XmNx, 140,
			XmNy, 30,
			XmNwidth, 181,
			XmNheight, 30,
			NULL );
	XtAddCallback( FPagName, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_FPagName,
		(XtPointer) UxLibFunctionContext );

	UxPutContext( FPagName, (char *) UxLibFunctionContext );


	/* Creation of FLibName */
	FLibName = XtVaCreateManagedWidget( "FLibName",
			xmTextFieldWidgetClass,
			form6,
			XmNx, 140,
			XmNy, 80,
			XmNwidth, 181,
			XmNheight, 30,
			XmNvalue, "std",
			NULL );
	UxPutContext( FLibName, (char *) UxLibFunctionContext );


	/* Creation of FLabName */
	FLabName = XtVaCreateManagedWidget( "FLabName",
			xmTextFieldWidgetClass,
			form6,
			XmNx, 140,
			XmNy, 130,
			XmNwidth, 181,
			XmNheight, 30,
			XmNvalue, "Std",
			NULL );
	UxPutContext( FLabName, (char *) UxLibFunctionContext );


	/* Creation of separator1 */
	separator1 = XtVaCreateManagedWidget( "separator1",
			xmSeparatorWidgetClass,
			form6,
			XmNx, 0,
			XmNy, 180,
			XmNwidth, 400,
			XmNheight, 10,
			NULL );
	UxPutContext( separator1, (char *) UxLibFunctionContext );


	XtAddCallback( LibFunction, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxLibFunctionContext);


	return ( LibFunction );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_LibFunction( _UxPaginaSel )
	int	_UxPaginaSel;
{
	Widget                  rtrn;
	_UxCLibFunction         *UxContext;

	UxLibFunctionContext = UxContext =
		(_UxCLibFunction *) UxNewContext( sizeof(_UxCLibFunction), False );

	PaginaSel = _UxPaginaSel;

	rtrn = _Uxbuild_LibFunction();

	/* se c'e' una pagina selezionata nella lista setto quella */  
	
	if(PaginaSel)
	{
	   XmTextFieldSetString(FPagName,pagedit->page_list[PaginaSel-1]);
	}  
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

