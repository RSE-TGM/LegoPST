
/*******************************************************************************
       XlCurve_confVar.c
       (Generated from interface file XlCurve_confVar.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <stdlib.h>

#include <XlCurve.h>

#include "sim_param.h"
#include "sim_types.h"
#include "f22_circ.h"
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

extern swidget create_warningDialog1();
extern swidget create_selectVar();
extern Boolean convStringToPix();
extern Boolean VerifStrFloat();

/* extern char *varInfoApp[NUMMAXVAR];
extern char *colStringApp[NUMMAXVAR]; */

Boolean RecoveryLoadInfo();
Boolean RestoreInfo();
void confVar_agg_name_descr();


/* char nome_var[CHAR_LEGO_COD];
char descr_var[71]; */


static	int _UxIfClassId;
int	UxconfVar_agg_text_field_Id = -1;
char*	UxconfVar_agg_text_field_Name = "agg_text_field";

#ifndef confVar_agg_text_field
#define confVar_agg_text_field( UxThis, pEnv ) \
	((void(*)())UxMethodLookup(UxThis, UxconfVar_agg_text_field_Id,\
			UxconfVar_agg_text_field_Name)) \
		( UxThis, pEnv )
#endif


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
	Widget	UxconfVar;
	Widget	Uxform4;
	Widget	Uxform5;
	Widget	UxpushButok;
	Widget	UxpushButdelete;
	Widget	UxpushButcancel;
	Widget	Uxform6;
	Widget	Uxlabel1;
	Widget	UxpushButton1;
	Widget	UxtextField1;
	Widget	Uxform7;
	Widget	Uxlabel2;
	Widget	UxpushButton2;
	Widget	UxtextField2;
	Widget	Uxlabel3;
	Widget	UxtextField3;
	Widget	Uxform8;
	Widget	Uxlabel4;
	Widget	Uxlabel5;
	Widget	Uxlabel6;
	Widget	UxtextField4;
	Widget	UxtextField5;
	Widget	UxtextField6;
	Widget	Uxlabel7;
	Widget	UxoptionMenu_p1;
	Widget	UxoptionMenuEng;
	Widget	UxoptionMenuPc;
	Widget	UxoptionMenuPu;
	Widget	UxoptionMenu1;
	Widget	UxlabelTit;
	unsigned char	Uxnome_var[9];
	unsigned char	Uxdescr_var[71];
	swidget	UxUxParent;
	unsigned char	*UxsingleVarInfo;
	unsigned char	*UxsingleColString;
	unsigned char	*Uxpath_f22;
	int	Uxnum_trend;
} _UxCconfVar;

static _UxCconfVar             *UxConfVarContext;
#define confVar                 UxConfVarContext->UxconfVar
#define form4                   UxConfVarContext->Uxform4
#define form5                   UxConfVarContext->Uxform5
#define pushButok               UxConfVarContext->UxpushButok
#define pushButdelete           UxConfVarContext->UxpushButdelete
#define pushButcancel           UxConfVarContext->UxpushButcancel
#define form6                   UxConfVarContext->Uxform6
#define label1                  UxConfVarContext->Uxlabel1
#define pushButton1             UxConfVarContext->UxpushButton1
#define textField1              UxConfVarContext->UxtextField1
#define form7                   UxConfVarContext->Uxform7
#define label2                  UxConfVarContext->Uxlabel2
#define pushButton2             UxConfVarContext->UxpushButton2
#define textField2              UxConfVarContext->UxtextField2
#define label3                  UxConfVarContext->Uxlabel3
#define textField3              UxConfVarContext->UxtextField3
#define form8                   UxConfVarContext->Uxform8
#define label4                  UxConfVarContext->Uxlabel4
#define label5                  UxConfVarContext->Uxlabel5
#define label6                  UxConfVarContext->Uxlabel6
#define textField4              UxConfVarContext->UxtextField4
#define textField5              UxConfVarContext->UxtextField5
#define textField6              UxConfVarContext->UxtextField6
#define label7                  UxConfVarContext->Uxlabel7
#define optionMenu_p1           UxConfVarContext->UxoptionMenu_p1
#define optionMenuEng           UxConfVarContext->UxoptionMenuEng
#define optionMenuPc            UxConfVarContext->UxoptionMenuPc
#define optionMenuPu            UxConfVarContext->UxoptionMenuPu
#define optionMenu1             UxConfVarContext->UxoptionMenu1
#define labelTit                UxConfVarContext->UxlabelTit
#define nome_var                UxConfVarContext->Uxnome_var
#define descr_var               UxConfVarContext->Uxdescr_var
#define UxParent                UxConfVarContext->UxUxParent
#define singleVarInfo           UxConfVarContext->UxsingleVarInfo
#define singleColString         UxConfVarContext->UxsingleColString
#define path_f22                UxConfVarContext->Uxpath_f22
#define num_trend               UxConfVarContext->Uxnum_trend



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_confVar();

/*******************************************************************************
Declarations of methods
*******************************************************************************/

static void	_confVar_agg_text_field();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

Boolean RecoveryLoadInfo(w)
Widget w;
{
_UxCconfVar  *UxSaveCtx, *UxContext;
int i,length,size;
char str_app[MAXLENINFOVAR],str_single_info[100],lab_titolo[10];
char *str_app1,*p;
char *str_end[NUMINFOVAR];
XmString compound;
Widget Button, Pane;
WidgetList ListaButton;

printf("RecoveryLoadInfo: ENTRATO!!!\n");

   UxSaveCtx = UxConfVarContext;
   UxConfVarContext = UxContext = (_UxCconfVar *) UxGetContext(w);
/*
	Setting della label titolo.
*/
   sprintf(lab_titolo,"Trend %d",num_trend);
   compound=XmStringCreateSimple(lab_titolo);
   XtVaSetValues(labelTit,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   strcpy(str_app,singleVarInfo);
   if(strcmp(str_app,"")!=0) {

printf("RecoveryLoadInfo: str_curr=%s\n",str_app);

/*
	Recupero le singole informazioni. Non inserisco controlli sulla correttezza di cio'
	che viene estratto. Tali controlli sono fatti precedentemente.
*/
      i=0;
      while((str_app1=strchr(str_app, '|')) != NULL) {
         length=strlen(str_app)-strlen(str_app1);
         strncpy(str_single_info, str_app, length);
         str_single_info[length] = '\0';
         size=strlen(str_single_info);
         p=(char *)XtCalloc(size,sizeof(char));
         strcpy(p,str_single_info);
         str_end[i]=p;

printf("RecoveryLoadInfo: str_end[%d]=%s\n",i,str_end[i]);

         i++;
         strcpy(str_app,(str_app1+1));
      }
/*
	Caricamento delle singole informazioni nell'interfaccia.
*/
      XmTextFieldSetString(textField1,singleColString);   
      XmTextFieldSetString(textField2,str_end[0]);
      XmTextFieldSetString(textField3,str_end[2]);
      XmTextFieldSetString(textField4,str_end[3]);
      XmTextFieldSetString(textField5,str_end[4]);
      XmTextFieldSetString(textField6,str_end[5]);
/*
	Setto il valore del tipo di unita' di misura nell'option menu.
*/
      get_something(optionMenu1,XmNsubMenuId, (void*) &Pane);
      get_something(Pane,XmNchildren, (void*) &ListaButton);
      if(strcmp(str_end[6],PERCENT)==0)
         Button = ListaButton[1];
      else if(strcmp(str_end[6],PERUNIT)==0)
         Button = ListaButton[2];
      else
         Button = ListaButton[0];

      set_something_val(optionMenu1,XmNmenuHistory,(XtArgVal)Button);	
/*
	Libero la struttura str_end allocata dinamicamente.
*/
      for(i=0;i<NUMINFOVAR;i++)
         XtFree(str_end[i]);
   }
   else {
/*
	La variabile corrente non e' ancora stata configurata. Carico stringhe vuote
	nell'interfaccia.
*/
      XmTextFieldSetString(textField1,singleColString);
      XmTextFieldSetString(textField2,"");
      XmTextFieldSetString(textField3,"");
      XmTextFieldSetString(textField4,"");
      XmTextFieldSetString(textField5,"");
      XmTextFieldSetString(textField6,"");

      get_something(optionMenu1,XmNsubMenuId, (void*) &Pane);
      get_something(Pane,XmNchildren, (void*) &ListaButton);
      Button = ListaButton[0];
      set_something_val(optionMenu1,XmNmenuHistory, (XtArgVal)Button);
   }
   UxConfVarContext = UxSaveCtx;
printf("RecoveryLoadInfo: USCITO!!!\n");

}


Boolean RestoreInfo(w)
Widget w;
{
int i,num_empty;
char *single_info[NUMINFOVAR];
char all_info[MAXLENINFOVAR];
char *color_str;
Pixel pix;
Widget WgtSelMenu, Pane;
WidgetList ListaButton;
_UxCconfVar  *UxSaveCtx, *UxContext;

printf("RestoreInfo: ENTRATO!!!\n");


   UxSaveCtx = UxConfVarContext;
   UxConfVarContext = UxContext = (_UxCconfVar *) UxGetContext(w);
/*
	Recupero delle singole informazioni da interfaccia.
*/
   color_str=XmTextFieldGetString(textField1);   
   single_info[0]=XmTextFieldGetString(textField2);
   single_info[1]=XmTextFieldGetString(textField2);
   single_info[2]=XmTextFieldGetString(textField3);
   single_info[3]=XmTextFieldGetString(textField4);
   single_info[4]=XmTextFieldGetString(textField5);
   single_info[5]=XmTextFieldGetString(textField6);

   get_something(optionMenu1,XmNmenuHistory, (void*) &WgtSelMenu);
   get_something(optionMenu1,XmNsubMenuId, (void*) &Pane);
   get_something(Pane,XmNchildren, (void*) &ListaButton);
   single_info[6] = (char *)calloc(4,sizeof(char));
   if(WgtSelMenu == ListaButton[0])
      strcpy(single_info[6],ENGINEER);
   else if(WgtSelMenu == ListaButton[1])
      strcpy(single_info[6],PERCENT);
   else
      strcpy(single_info[6],PERUNIT);
/*
   Controlli sui valori caricati nell'interfaccia.
*/
printf("RestoreInfo: prima dei controlli sui dati inseriti\n"); 
   num_empty=0;
   for(i=0;i<(NUMINFOVAR-1);i++) {
printf("RestoreInfo: single_info[%d]=%s\n",i,single_info[i]);
      if(strcmp(single_info[i],"")==0)
         num_empty++;
   }
   if(num_empty!=(NUMINFOVAR-1)) {
/*
	La variabile corrente e' stata configurata. Controllo sulla correttezza dei valori.
*/
      if((strlen(single_info[0])>CHAR_LEGO_COD)||(strcmp(single_info[0],"")==0)) {
         create_warningDialog1(confVar,0);
         return(False);
      }
      if((strcmp(single_info[3],"")==0)||(VerifStrFloat(single_info[3])==False)) {
         create_warningDialog1(confVar,1);
         return(False);
      }
      if((strcmp(single_info[4],"")==0)||(VerifStrFloat(single_info[4])==False)) {
         create_warningDialog1(confVar,7);
         return(False);
      }
      if(atof(single_info[3])>=atof(single_info[4])) {
         create_warningDialog1(confVar,2);
         return(False);
      }   
      if(strcmp(single_info[5],"")==0) {
         create_warningDialog1(confVar,3);
         return(False);
      }
      if(convStringToPix(confVar,color_str,&pix)==False) {
         create_warningDialog1(confVar,4);
         return(False);
      }
/*
	Controlli andati a buon fine. Compongo la stringa con le informazioni separate da
	pipes.
*/
      strcpy(all_info,single_info[0]);
      strcat(all_info,"|");
      XtFree(single_info[0]);
      for(i=1;i<NUMINFOVAR;i++) {
         if(strcmp(single_info[i],"")==0)
            strcpy(single_info[i],"---");
         strcat(all_info,single_info[i]);
         XtFree(single_info[i]);
         strcat(all_info,"|");
      }
      strcpy(singleVarInfo,all_info);
      strcpy(singleColString,color_str);
      printf("RestoreInfo: singleVarInfo=%s\n",singleVarInfo);
  }
  else {
/*
	I campi informazione non sono stati caricati. Salvo una stringa nulla. Variabile non
	configurata.
*/
      strcpy(singleVarInfo,"");
  }
  UxConfVarContext = UxSaveCtx;

printf("RestoreInfo: USCITO!!!\n");

return(True);
}


void confVar_agg_name_descr(w)
Widget w;
{
printf("confVar_agg_name_descr: nome_var=%s\tdescr_var=%s\n",nome_var,descr_var);
   confVar_agg_text_field(w,&UxEnv);
}

/*******************************************************************************
       The following are method functions.
*******************************************************************************/

static void	Ux_agg_text_field( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	printf("confVar_agg_text_field: nome_var=%s\tdescr_var=%s\n",nome_var,descr_var);
	
	XmTextFieldSetString(textField2,nome_var);
	XmTextFieldSetString(textField3,descr_var);
	
	/* XtVaSetValues(textField2,XmNtextValue,nome_var,NULL);
	XtVaSetValues(textField3,XmNtextValue,descr_var,NULL);   */
}

static void	_confVar_agg_text_field( UxThis, pEnv )
	swidget	UxThis;
	Environment *	pEnv;
{
	_UxCconfVar             *UxSaveCtx = UxConfVarContext;

	UxConfVarContext = (_UxCconfVar *) UxGetContext( UxThis );
	if (pEnv)
		pEnv->_major = NO_EXCEPTION;
	Ux_agg_text_field( UxThis, pEnv );
	UxConfVarContext = UxSaveCtx;
}


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushButok( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfVar             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfVarContext;
	UxConfVarContext = UxContext =
			(_UxCconfVar *) UxGetContext( UxWidget );
	{
	if(RestoreInfo(confVar)==True)
	   UxDestroyInterface(confVar);
	}
	UxConfVarContext = UxSaveCtx;
}

static	void	activateCB_pushButdelete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfVar             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfVarContext;
	UxConfVarContext = UxContext =
			(_UxCconfVar *) UxGetContext( UxWidget );
	{
	/*
		Delete del contenuto di tutti i text fields.
	*/
	      XmTextFieldSetString(textField1,"");   
	      XmTextFieldSetString(textField2,"");
	      XmTextFieldSetString(textField3,"");
	      XmTextFieldSetString(textField4,"");
	      XmTextFieldSetString(textField5,"");
	      XmTextFieldSetString(textField6,"");
	
	}
	UxConfVarContext = UxSaveCtx;
}

static	void	activateCB_pushButcancel( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfVar             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfVarContext;
	UxConfVarContext = UxContext =
			(_UxCconfVar *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(confVar);
	}
	UxConfVarContext = UxSaveCtx;
}

static	void	activateCB_pushButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfVar             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfVarContext;
	UxConfVarContext = UxContext =
			(_UxCconfVar *) UxGetContext( UxWidget );
	{
	create_selectVar(NO_PARENT,confVar,nome_var,descr_var,path_f22);
	}
	UxConfVarContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_confVar()
{
	Widget		_UxParent;
	Widget		optionMenu_p1_shell;


	/* Creation of confVar */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	confVar = XtVaCreatePopupShell( "confVar",
			xmDialogShellWidgetClass,
			_UxParent,
			XmNwidth, 630,
			XmNheight, 230,
			XmNx, 500,
			XmNy, 450,
			NULL );
	UxPutContext( confVar, (char *) UxConfVarContext );


	/* Creation of form4 */
	form4 = XtVaCreateWidget( "form4",
			xmFormWidgetClass,
			confVar,
			XmNwidth, 1015,
			XmNheight, 180,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 20,
			XmNy, 10,
			XmNunitType, XmPIXELS,
			XmNdialogStyle, XmDIALOG_MODELESS,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( form4, (char *) UxConfVarContext );


	/* Creation of form5 */
	form5 = XtVaCreateManagedWidget( "form5",
			xmFormWidgetClass,
			form4,
			XmNwidth, 995,
			XmNheight, 45,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 130,
			XmNborderWidth, 1,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			NULL );
	UxPutContext( form5, (char *) UxConfVarContext );


	/* Creation of pushButok */
	pushButok = XtVaCreateManagedWidget( "pushButok",
			xmPushButtonWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 5,
			XmNheight, 35,
			XmNwidth, 100,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushButok, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButok,
		(XtPointer) UxConfVarContext );

	UxPutContext( pushButok, (char *) UxConfVarContext );


	/* Creation of pushButdelete */
	pushButdelete = XtVaCreateManagedWidget( "pushButdelete",
			xmPushButtonWidgetClass,
			form5,
			XmNx, 269,
			XmNy, 5,
			XmNheight, 35,
			XmNwidth, 100,
			RES_CONVERT( XmNlabelString, "Delete" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, pushButok,
			XmNtopOffset, 5,
			NULL );
	XtAddCallback( pushButdelete, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButdelete,
		(XtPointer) UxConfVarContext );

	UxPutContext( pushButdelete, (char *) UxConfVarContext );


	/* Creation of pushButcancel */
	pushButcancel = XtVaCreateManagedWidget( "pushButcancel",
			xmPushButtonWidgetClass,
			form5,
			XmNx, 180,
			XmNy, 5,
			XmNheight, 35,
			XmNwidth, 100,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, pushButdelete,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushButcancel, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButcancel,
		(XtPointer) UxConfVarContext );

	UxPutContext( pushButcancel, (char *) UxConfVarContext );


	/* Creation of form6 */
	form6 = XtVaCreateManagedWidget( "form6",
			xmFormWidgetClass,
			form4,
			XmNwidth, 180,
			XmNheight, 80,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 45,
			XmNborderWidth, 1,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNtopOffset, 10,
			NULL );
	UxPutContext( form6, (char *) UxConfVarContext );


	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			form6,
			XmNx, 10,
			XmNy, 10,
			XmNheight, 25,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Color" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label1, (char *) UxConfVarContext );


	/* Creation of pushButton1 */
	pushButton1 = XtVaCreateManagedWidget( "pushButton1",
			xmPushButtonWidgetClass,
			form6,
			XmNx, 10,
			XmNy, 50,
			XmNwidth, 35,
			XmNheight, 35,
			RES_CONVERT( XmNlabelString, "..." ),
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label1,
			XmNsensitive, FALSE,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( pushButton1, (char *) UxConfVarContext );


	/* Creation of textField1 */
	textField1 = XtVaCreateManagedWidget( "textField1",
			xmTextFieldWidgetClass,
			form6,
			XmNwidth, 120,
			XmNx, 69,
			XmNy, 49,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, pushButton1,
			XmNheight, 35,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label1,
			XmNvalue, "",
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( textField1, (char *) UxConfVarContext );


	/* Creation of form7 */
	form7 = XtVaCreateManagedWidget( "form7",
			xmFormWidgetClass,
			form4,
			XmNwidth, 410,
			XmNheight, 80,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 195,
			XmNy, 45,
			XmNborderWidth, 1,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNtopOffset, 10,
			NULL );
	UxPutContext( form7, (char *) UxConfVarContext );


	/* Creation of label2 */
	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass,
			form7,
			XmNx, 50,
			XmNy, 10,
			XmNheight, 25,
			XmNwidth, 90,
			RES_CONVERT( XmNlabelString, "Var Lego" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label2, (char *) UxConfVarContext );


	/* Creation of pushButton2 */
	pushButton2 = XtVaCreateManagedWidget( "pushButton2",
			xmPushButtonWidgetClass,
			form7,
			XmNx, 10,
			XmNy, 45,
			XmNwidth, 35,
			XmNheight, 35,
			RES_CONVERT( XmNlabelString, "..." ),
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label2,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton2,
		(XtPointer) UxConfVarContext );

	UxPutContext( pushButton2, (char *) UxConfVarContext );


	/* Creation of textField2 */
	textField2 = XtVaCreateManagedWidget( "textField2",
			xmTextFieldWidgetClass,
			form7,
			XmNwidth, 100,
			XmNx, 69,
			XmNy, 49,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, pushButton2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label2,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( textField2, (char *) UxConfVarContext );


	/* Creation of label3 */
	label3 = XtVaCreateManagedWidget( "label3",
			xmLabelWidgetClass,
			form7,
			XmNx, 159,
			XmNy, 9,
			XmNheight, 25,
			XmNwidth, 140,
			RES_CONVERT( XmNlabelString, "Description" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 15,
			XmNleftWidget, label2,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label3, (char *) UxConfVarContext );


	/* Creation of textField3 */
	textField3 = XtVaCreateManagedWidget( "textField3",
			xmTextFieldWidgetClass,
			form7,
			XmNwidth, 250,
			XmNx, 3199,
			XmNy, 49,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, textField2,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label3,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightOffset, 0,
			NULL );
	UxPutContext( textField3, (char *) UxConfVarContext );


	/* Creation of form8 */
	form8 = XtVaCreateManagedWidget( "form8",
			xmFormWidgetClass,
			form4,
			XmNwidth, 390,
			XmNheight, 80,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 615,
			XmNy, 45,
			XmNborderWidth, 1,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNtopOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 10,
			XmNleftWidget, form7,
			NULL );
	UxPutContext( form8, (char *) UxConfVarContext );


	/* Creation of label4 */
	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass,
			form8,
			XmNx, 10,
			XmNy, 10,
			XmNheight, 25,
			XmNwidth, 90,
			RES_CONVERT( XmNlabelString, "Low Lim" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label4, (char *) UxConfVarContext );


	/* Creation of label5 */
	label5 = XtVaCreateManagedWidget( "label5",
			xmLabelWidgetClass,
			form8,
			XmNx, 119,
			XmNy, 9,
			XmNheight, 25,
			XmNwidth, 90,
			RES_CONVERT( XmNlabelString, "High Lim" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, label4,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label5, (char *) UxConfVarContext );


	/* Creation of label6 */
	label6 = XtVaCreateManagedWidget( "label6",
			xmLabelWidgetClass,
			form8,
			XmNx, 219,
			XmNy, 9,
			XmNheight, 25,
			XmNwidth, 90,
			RES_CONVERT( XmNlabelString, "Unit Mis" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, label5,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( label6, (char *) UxConfVarContext );


	/* Creation of textField4 */
	textField4 = XtVaCreateManagedWidget( "textField4",
			xmTextFieldWidgetClass,
			form8,
			XmNwidth, 90,
			XmNx, 9,
			XmNy, 49,
			XmNheight, 35,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label4,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( textField4, (char *) UxConfVarContext );


	/* Creation of textField5 */
	textField5 = XtVaCreateManagedWidget( "textField5",
			xmTextFieldWidgetClass,
			form8,
			XmNwidth, 90,
			XmNx, 119,
			XmNy, 49,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, textField4,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( textField5, (char *) UxConfVarContext );


	/* Creation of textField6 */
	textField6 = XtVaCreateManagedWidget( "textField6",
			xmTextFieldWidgetClass,
			form8,
			XmNwidth, 90,
			XmNx, 219,
			XmNy, 49,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, textField5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label6,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 0,
			NULL );
	UxPutContext( textField6, (char *) UxConfVarContext );


	/* Creation of label7 */
	label7 = XtVaCreateManagedWidget( "label7",
			xmLabelWidgetClass,
			form8,
			XmNx, 230,
			XmNy, 9,
			XmNheight, 25,
			XmNwidth, 90,
			RES_CONVERT( XmNlabelString, "Unit Type" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 5,
			XmNleftWidget, label6,
			XmNtopOffset, 10,
			NULL );
	UxPutContext( label7, (char *) UxConfVarContext );


	/* Creation of optionMenu_p1 */
	optionMenu_p1_shell = XtVaCreatePopupShell ("optionMenu_p1_shell",
			xmMenuShellWidgetClass, form8,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	optionMenu_p1 = XtVaCreateWidget( "optionMenu_p1",
			xmRowColumnWidgetClass,
			optionMenu_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNancestorSensitive, TRUE,
			XmNshadowThickness, 0,
			XmNwidth, 43,
			NULL );
	UxPutContext( optionMenu_p1, (char *) UxConfVarContext );


	/* Creation of optionMenuEng */
	optionMenuEng = XtVaCreateManagedWidget( "optionMenuEng",
			xmPushButtonWidgetClass,
			optionMenu_p1,
			RES_CONVERT( XmNlabelString, "ENG" ),
			XmNmarginHeight, 2,
			XmNmarginWidth, 2,
			XmNx, 0,
			XmNy, 0,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			NULL );
	UxPutContext( optionMenuEng, (char *) UxConfVarContext );


	/* Creation of optionMenuPc */
	optionMenuPc = XtVaCreateManagedWidget( "optionMenuPc",
			xmPushButtonWidgetClass,
			optionMenu_p1,
			RES_CONVERT( XmNlabelString, "PC" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			NULL );
	UxPutContext( optionMenuPc, (char *) UxConfVarContext );


	/* Creation of optionMenuPu */
	optionMenuPu = XtVaCreateManagedWidget( "optionMenuPu",
			xmPushButtonWidgetClass,
			optionMenu_p1,
			RES_CONVERT( XmNlabelString, "PU" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			NULL );
	UxPutContext( optionMenuPu, (char *) UxConfVarContext );


	/* Creation of optionMenu1 */
	optionMenu1 = XtVaCreateManagedWidget( "optionMenu1",
			xmRowColumnWidgetClass,
			form8,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, optionMenu_p1,
			XmNx, 289,
			XmNy, 39,
			XmNwidth, 80,
			XmNheight, 35,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 3,
			XmNleftWidget, textField6,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, label7,
			XmNancestorSensitive, TRUE,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNspacing, 0,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 0,
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightOffset, 0,
			NULL );
	UxPutContext( optionMenu1, (char *) UxConfVarContext );


	/* Creation of labelTit */
	labelTit = XtVaCreateManagedWidget( "labelTit",
			xmLabelWidgetClass,
			form4,
			XmNx, 370,
			XmNy, 10,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Trend" ),
			XmNheight, 30,
			XmNwidth, 150,
			XmNtopOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftPosition, 0,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			XmNleftOffset, 432,
			NULL );
	UxPutContext( labelTit, (char *) UxConfVarContext );

	XtVaSetValues(optionMenu1,
			XmNmenuHistory, optionMenuPc,
			NULL );


	XtAddCallback( confVar, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxConfVarContext);


	return ( confVar );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_confVar( _UxUxParent, _UxsingleVarInfo, _UxsingleColString, _Uxpath_f22, _Uxnum_trend )
	swidget	_UxUxParent;
	unsigned char	*_UxsingleVarInfo;
	unsigned char	*_UxsingleColString;
	unsigned char	*_Uxpath_f22;
	int	_Uxnum_trend;
{
	Widget                  rtrn;
	_UxCconfVar             *UxContext;
	static int		_Uxinit = 0;

	UxConfVarContext = UxContext =
		(_UxCconfVar *) UxNewContext( sizeof(_UxCconfVar), False );

	UxParent = _UxUxParent;
	singleVarInfo = _UxsingleVarInfo;
	singleColString = _UxsingleColString;
	path_f22 = _Uxpath_f22;
	num_trend = _Uxnum_trend;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewClassId();
		UxconfVar_agg_text_field_Id = UxMethodRegister( _UxIfClassId,
					UxconfVar_agg_text_field_Name,
					_confVar_agg_text_field );
		_Uxinit = 1;
	}

	rtrn = _Uxbuild_confVar();
	UxPutClassCode( confVar, _UxIfClassId );

	/*
		Chiamata alla funzione che estrae le singole informazioni dalla stringa
		infoVarChange[i] e le carica nell'interfaccia.
	*/
	
	   RecoveryLoadInfo(rtrn);
	
	UxPopupInterface(rtrn,no_grab);
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

