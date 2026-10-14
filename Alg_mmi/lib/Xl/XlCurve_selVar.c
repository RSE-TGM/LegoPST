
/*******************************************************************************
       XlCurve_selVar.c
       (Generated from interface file XlCurve_selVar.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/SelectioB.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#include <XlCurve.h>

#include "sim_param.h"
#include "sim_types.h"
#include "f22_circ.h"
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULT_F22_STR   "libera"

extern swidget create_warningDialog1();
extern void confVar_agg_name_descr();

/*
extern char path_source_f22[MAXPATHNAME];
*/


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
	Widget	UxselectVar;
	Widget	UxUxParent;
	Widget	Uxwid_parent;
	unsigned char	*Uxnome;
	unsigned char	*Uxdescriz;
	unsigned char	*Uxpath_source_f22;
} _UxCselectVar;

static _UxCselectVar           *UxSelectVarContext;
#define selectVar               UxSelectVarContext->UxselectVar
#define UxParent                UxSelectVarContext->UxUxParent
#define wid_parent              UxSelectVarContext->Uxwid_parent
#define nome                    UxSelectVarContext->Uxnome
#define descriz                 UxSelectVarContext->Uxdescriz
#define path_source_f22         UxSelectVarContext->Uxpath_source_f22



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_selectVar();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void activateCB_ok(w,client_data,cbs)
Widget w;
XtPointer client_data;
XmSelectionBoxCallbackStruct *cbs;
{

int lungh;
char str_glob[100];
char *value;
char nome_start[CHAR_LEGO_COD+1],str_app[CHAR_LEGO_COD+1],descr_app[100],*str_app1;

switch (cbs->reason) {
 case XmCR_NO_MATCH:
 {
printf("SELECTION_BOX: XmCR_NO_MATCH:!!!!!\n");
   create_warningDialog1(selectVar,8);
   break;
 }
 case XmCR_OK:
 {
   XmStringGetLtoR(cbs->value,XmSTRING_DEFAULT_CHARSET,&value);
   strcpy(str_glob,value);
   XmStringFree(value);
   strncpy(nome_start,str_glob,CHAR_LEGO_COD);
   nome_start[CHAR_LEGO_COD]='\0';
   strcpy(descr_app,(str_glob+CHAR_LEGO_COD));

printf("activateCB_ok: stringa_sel=%s\tnome_start=%s\tdescr_app=%s\n",str_glob,nome_start,descr_app);

   if((str_app1=strchr(nome_start,' '))!=NULL) {
      lungh=(strlen(nome_start)-strlen(str_app1));
printf("activateCB_ok: lungh=%d\n",lungh);
      strncpy(str_app,nome_start,lungh);
      str_app[lungh]='\0';
   }
   else {
      strcpy(str_app,nome_start);
   }

   strcpy(nome,str_app);
   /* strcpy(nome,nome_start);  */
printf("activateCB_ok: nome=%s\tnome_start=%s\n",nome,nome_start);
   strcpy(descriz,descr_app);

printf("activateCB_ok: nome=%s\tdescriz=%s\n",nome,descriz);

   confVar_agg_name_descr(wid_parent);

   UxDestroyInterface(selectVar);
   break;
 }
 default:
 {
printf("SELECTION_BOX: default!!!!!\n");
   create_warningDialog1(selectVar,8);
   break;
 }
}
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	cancelCB_selectVar( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCselectVar           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSelectVarContext;
	UxSelectVarContext = UxContext =
			(_UxCselectVar *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxThisWidget);
	}
	UxSelectVarContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_selectVar()
{
	Widget		_UxParent;


	/* Creation of selectVar */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = XtVaCreatePopupShell( "selectVar_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 790,
			XmNy, 640,
			XmNwidth, 500,
			XmNheight, 400,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "selectVar",
			XmNiconName, "selectVar",
			NULL );

	}

	selectVar = XtVaCreateManagedWidget( "selectVar",
			xmSelectionBoxWidgetClass,
			_UxParent,
			XmNwidth, 500,
			XmNheight, 400,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNlistLabelString, "Variables" ),
			XmNdialogType, XmDIALOG_SELECTION,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( selectVar, XmNcancelCallback,
		(XtCallbackProc) cancelCB_selectVar,
		(XtPointer) UxSelectVarContext );
	XtAddCallback( selectVar, XmNokCallback,
		(XtCallbackProc) activateCB_ok,
		(XtPointer) UxSelectVarContext );

	UxPutContext( selectVar, (char *) UxSelectVarContext );


	XtAddCallback( selectVar, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSelectVarContext);


	return ( selectVar );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_selectVar( _UxUxParent, _Uxwid_parent, _Uxnome, _Uxdescriz, _Uxpath_source_f22 )
	Widget	_UxUxParent;
	Widget	_Uxwid_parent;
	unsigned char	*_Uxnome;
	unsigned char	*_Uxdescriz;
	unsigned char	*_Uxpath_source_f22;
{
	Widget                  rtrn;
	_UxCselectVar           *UxContext;

	UxSelectVarContext = UxContext =
		(_UxCselectVar *) UxNewContext( sizeof(_UxCselectVar), False );

	UxParent = _UxUxParent;
	wid_parent = _Uxwid_parent;
	nome = _Uxnome;
	descriz = _Uxdescriz;
	path_source_f22 = _Uxpath_source_f22;

	{
		F22CIRC_VAR *nome_descr;
		int num_var,num_var_select;
		int count;
		PUNT_FILE_F22 punt_f22;
		
		XmString *str;
		int i;
		char nome_descr_str[100],str_app_load[CHAR_LEGO_COD];
		rtrn = _Uxbuild_selectVar();

		if(f22_open_file_new(path_source_f22,&punt_f22)==0) {
		      UxPopupInterface(rtrn,no_grab);
		      create_warningDialog1(rtrn,5);
		   }
		   else {
		      if(f22_leggo_nomi_var(punt_f22,&nome_descr,&num_var)==0) {
		         UxPopupInterface(rtrn,no_grab);
		         create_warningDialog1(rtrn,6);
		         f22_close_file(punt_f22);
		      }
		      else {
		/*
			Lettura dei nomi delle variabili andata a buon fine. Caricamento nell'interfaccia.
		*/
		         f22_close_file(punt_f22);
		         count=0;
		         strncpy(str_app_load,nome_descr[count].nomevar,strlen(DEFAULT_F22_STR));
		         str_app_load[strlen(DEFAULT_F22_STR)]='\0';
		         while((count<num_var)&&(strcmp(str_app_load,DEFAULT_F22_STR)!=0)) {
		            count++;
		            if(count!=num_var) {
		               strncpy(str_app_load,nome_descr[count].nomevar,strlen(DEFAULT_F22_STR));
		               str_app_load[strlen(DEFAULT_F22_STR)]='\0';
		            }
		         }
		         num_var_select=count;
		printf("SELECTION_BOX: num_var_select = %d\n",num_var_select);
		         str = (XmString *)XtMalloc(num_var_select * sizeof(XmString));
		         for(i=0;i<num_var_select;i++) {                                           
		            strcpy(nome_descr_str,nome_descr[i].nomevar);
		            strcat(nome_descr_str,nome_descr[i].descvar);
		            str[i]=XmStringCreateSimple(nome_descr_str);
		         }
		         XtVaSetValues(rtrn,XmNlistItems,str,XmNlistItemCount,num_var_select,XmNmustMatch,True,NULL);
		         XtSetSensitive(
		              XmSelectionBoxGetChild(rtrn,XmDIALOG_HELP_BUTTON),False);
		         for(i=0;i<num_var_select;i++)
		            XmStringFree(str[i]);
		         XtFree(str);
		         UxPopupInterface(rtrn,no_grab);
		      }
		   }                
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

