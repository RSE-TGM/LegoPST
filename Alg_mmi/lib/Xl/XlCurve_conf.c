
/*******************************************************************************
       XlCurve_conf.c
       (Generated from interface file XlCurve_conf.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
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

extern swidget create_confVar();

/*
extern char *varInfoChange[NUMMAXVAR];
extern char *colTrendString[NUMMAXVAR];
extern Boolean change_conf_OK;
char *varInfoApp[NUMMAXVAR];
char *colStringApp[NUMMAXVAR];
char path_source_f22[MAXPATHNAME];
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
	Widget	UxconfRunTime;
	Widget	Uxform2;
	Widget	UxlabTitle;
	Widget	UxrowColumn2;
	Widget	UxpushBut1;
	Widget	UxpushBut6;
	Widget	UxpushBut2;
	Widget	UxpushBut7;
	Widget	UxpushBut3;
	Widget	UxpushBut8;
	Widget	UxpushBut4;
	Widget	UxpushBut9;
	Widget	UxpushBut5;
	Widget	UxpushBut10;
	Widget	Uxform3;
	Widget	UxokBut;
	Widget	UxcancelBut;
	unsigned char	**UxvarInfoApp;
	unsigned char	**UxcolStringApp;
	swidget	UxUxParent;
	int	UxnumCurveConf;
	unsigned char	*UxsorgDati;
	unsigned char	**UxvarInfo;
	unsigned char	**UxcolString;
	int	*Uxconf_OK;
} _UxCconfRunTime;

static _UxCconfRunTime         *UxConfRunTimeContext;
#define confRunTime             UxConfRunTimeContext->UxconfRunTime
#define form2                   UxConfRunTimeContext->Uxform2
#define labTitle                UxConfRunTimeContext->UxlabTitle
#define rowColumn2              UxConfRunTimeContext->UxrowColumn2
#define pushBut1                UxConfRunTimeContext->UxpushBut1
#define pushBut6                UxConfRunTimeContext->UxpushBut6
#define pushBut2                UxConfRunTimeContext->UxpushBut2
#define pushBut7                UxConfRunTimeContext->UxpushBut7
#define pushBut3                UxConfRunTimeContext->UxpushBut3
#define pushBut8                UxConfRunTimeContext->UxpushBut8
#define pushBut4                UxConfRunTimeContext->UxpushBut4
#define pushBut9                UxConfRunTimeContext->UxpushBut9
#define pushBut5                UxConfRunTimeContext->UxpushBut5
#define pushBut10               UxConfRunTimeContext->UxpushBut10
#define form3                   UxConfRunTimeContext->Uxform3
#define okBut                   UxConfRunTimeContext->UxokBut
#define cancelBut               UxConfRunTimeContext->UxcancelBut
#define varInfoApp              UxConfRunTimeContext->UxvarInfoApp
#define colStringApp            UxConfRunTimeContext->UxcolStringApp
#define UxParent                UxConfRunTimeContext->UxUxParent
#define numCurveConf            UxConfRunTimeContext->UxnumCurveConf
#define sorgDati                UxConfRunTimeContext->UxsorgDati
#define varInfo                 UxConfRunTimeContext->UxvarInfo
#define colString               UxConfRunTimeContext->UxcolString
#define conf_OK                 UxConfRunTimeContext->Uxconf_OK



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_confRunTime();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_pushBut1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[0],colStringApp[0],sorgDati,1);
	
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[5],colStringApp[5],sorgDati,6);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[1],colStringApp[1],sorgDati,2);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[6],colStringApp[6],sorgDati,7);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[2],colStringApp[2],sorgDati,3);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[7],colStringApp[7],sorgDati,8);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[3],colStringApp[3],sorgDati,4);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[8],colStringApp[8],sorgDati,9);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[4],colStringApp[4],sorgDati,5);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_pushBut10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	create_confVar(confRunTime,varInfoApp[9],colStringApp[9],sorgDati,10);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_okBut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	int i,length;
	char *str_app1,*str_app2,str_lego1[CHAR_LEGO_COD+1],str_lego2[CHAR_LEGO_COD+1];
	
	*conf_OK=PARAM_CHANGE;
	for(i=0;i<numCurveConf;i++) {
	   str_app1=strchr(varInfo[i], '|');
	   str_app2=strchr(varInfoApp[i], '|');
	   if((str_app1==NULL)||(str_app2==NULL)) {
	      if((str_app1==NULL)&&(str_app2!=NULL)) {
	/*
		Aggiunta una variabile.
	*/
	        *conf_OK=CURVE_CHANGE;
	      }
	      else {
	         if((str_app1!=NULL)&&(str_app2==NULL)) {
	/*
		Eliminata una variabile.
	*/
	            *conf_OK=CURVE_CHANGE;
	         }
	      }           
	   }
	   else {
	      length=strlen(varInfo[i])-strlen(str_app1);
	      strncpy(str_lego1, varInfo[i], length);
	      str_lego1[length]='\0';
	      length=strlen(varInfoApp[i])-strlen(str_app2);
	      strncpy(str_lego2, varInfoApp[i], length);
	      str_lego2[length]='\0';
	printf("okBut: str_lego1=%s\tstr_lego2=%s\n",str_lego1,str_lego2);
	      if(strcmp(str_lego1,str_lego2)!=0)
	/*
		E' cambiato il codice lego di una delle variabili. Leggere tutti i campioni
		dal file f22.
	*/
	         *conf_OK=CURVE_CHANGE;
	   }
	}
	
	for(i=0;i<numCurveConf;i++) {
	   strcpy(varInfo[i],varInfoApp[i]);
	   free(varInfoApp[i]);
	}
	free(varInfoApp);
	
	for(i=0;i<numCurveConf;i++) {
	   strcpy(colString[i],colStringApp[i]);
	   free(colStringApp[i]);
	}
	free(colStringApp);
	
	UxDestroyInterface(UxThisWidget);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

static	void	activateCB_cancelBut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCconfRunTime         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxConfRunTimeContext;
	UxConfRunTimeContext = UxContext =
			(_UxCconfRunTime *) UxGetContext( UxWidget );
	{
	int i;
	
	for(i=0;i<numCurveConf;i++)
	   free(varInfoApp[i]);
	free(varInfoApp);
	
	for(i=0;i<numCurveConf;i++)
	   free(colStringApp[i]);
	free(colStringApp);
	
	UxDestroyInterface(UxThisWidget);
	}
	UxConfRunTimeContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_confRunTime()
{
	Widget		_UxParent;


	/* Creation of confRunTime */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	confRunTime = XtVaCreatePopupShell( "confRunTime",
			xmDialogShellWidgetClass,
			_UxParent,
			XmNwidth, 380,
			XmNheight, 300,
			XmNx, 470,
			XmNy, 560,
			NULL );
	UxPutContext( confRunTime, (char *) UxConfRunTimeContext );


	/* Creation of form2 */
	form2 = XtVaCreateWidget( "form2",
			xmFormWidgetClass,
			confRunTime,
			XmNwidth, 450,
			XmNheight, 250,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 10,
			XmNunitType, XmPIXELS,
			XmNnoResize, TRUE,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( form2, (char *) UxConfRunTimeContext );


	/* Creation of labTitle */
	labTitle = XtVaCreateManagedWidget( "labTitle",
			xmLabelWidgetClass,
			form2,
			XmNx, 85,
			XmNy, 10,
			XmNwidth, 280,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Configuration trends" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--14-140-75-75-m-90-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			XmNleftOffset, 70,
			NULL );
	UxPutContext( labTitle, (char *) UxConfRunTimeContext );


	/* Creation of rowColumn2 */
	rowColumn2 = XtVaCreateManagedWidget( "rowColumn2",
			xmRowColumnWidgetClass,
			form2,
			XmNwidth, 430,
			XmNheight, 140,
			XmNx, 10,
			XmNy, 40,
			XmNnumColumns, 5,
			XmNpacking, XmPACK_COLUMN,
			XmNradioAlwaysOne, FALSE,
			XmNadjustMargin, TRUE,
			XmNmarginHeight, 30,
			XmNmarginWidth, 10,
			XmNnavigationType, XmTAB_GROUP,
			XmNspacing, 20,
			XmNisAligned, FALSE,
			XmNentryAlignment, XmALIGNMENT_CENTER,
			XmNadjustLast, TRUE,
			XmNresizable, FALSE,
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNresizeHeight, FALSE,
			XmNresizeWidth, FALSE,
			XmNtopOffset, 60,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			NULL );
	UxPutContext( rowColumn2, (char *) UxConfRunTimeContext );


	/* Creation of pushBut1 */
	pushBut1 = XtVaCreateManagedWidget( "pushBut1",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 30,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 63,
			RES_CONVERT( XmNlabelString, "Trend 1" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut1, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut1,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut1, (char *) UxConfRunTimeContext );


	/* Creation of pushBut6 */
	pushBut6 = XtVaCreateManagedWidget( "pushBut6",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 20,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 6" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut6, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut6,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut6, (char *) UxConfRunTimeContext );


	/* Creation of pushBut2 */
	pushBut2 = XtVaCreateManagedWidget( "pushBut2",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 100,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 2" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut2, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut2,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut2, (char *) UxConfRunTimeContext );


	/* Creation of pushBut7 */
	pushBut7 = XtVaCreateManagedWidget( "pushBut7",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 180,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 7" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut7, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut7,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut7, (char *) UxConfRunTimeContext );


	/* Creation of pushBut3 */
	pushBut3 = XtVaCreateManagedWidget( "pushBut3",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 260,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 3" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut3, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut3,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut3, (char *) UxConfRunTimeContext );


	/* Creation of pushBut8 */
	pushBut8 = XtVaCreateManagedWidget( "pushBut8",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 340,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 8" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut8, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut8,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut8, (char *) UxConfRunTimeContext );


	/* Creation of pushBut4 */
	pushBut4 = XtVaCreateManagedWidget( "pushBut4",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 100,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 4" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut4, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut4,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut4, (char *) UxConfRunTimeContext );


	/* Creation of pushBut9 */
	pushBut9 = XtVaCreateManagedWidget( "pushBut9",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 180,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 9" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut9, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut9,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut9, (char *) UxConfRunTimeContext );


	/* Creation of pushBut5 */
	pushBut5 = XtVaCreateManagedWidget( "pushBut5",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 260,
			XmNy, 30,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 5" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut5, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut5,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut5, (char *) UxConfRunTimeContext );


	/* Creation of pushBut10 */
	pushBut10 = XtVaCreateManagedWidget( "pushBut10",
			xmPushButtonWidgetClass,
			rowColumn2,
			XmNx, 260,
			XmNy, 80,
			XmNheight, 30,
			XmNwidth, 60,
			RES_CONVERT( XmNlabelString, "Trend 10" ),
			XmNrecomputeSize, FALSE,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--12-120-75-75-m-70-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( pushBut10, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushBut10,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( pushBut10, (char *) UxConfRunTimeContext );


	/* Creation of form3 */
	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass,
			form2,
			XmNwidth, 430,
			XmNheight, 50,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 10,
			XmNy, 190,
			XmNleftOffset, 10,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNborderWidth, 1,
			NULL );
	UxPutContext( form3, (char *) UxConfRunTimeContext );


	/* Creation of okBut */
	okBut = XtVaCreateManagedWidget( "okBut",
			xmPushButtonWidgetClass,
			form3,
			XmNx, 50,
			XmNy, 10,
			XmNheight, 35,
			XmNwidth, 100,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( okBut, XmNactivateCallback,
		(XtCallbackProc) activateCB_okBut,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( okBut, (char *) UxConfRunTimeContext );


	/* Creation of cancelBut */
	cancelBut = XtVaCreateManagedWidget( "cancelBut",
			xmPushButtonWidgetClass,
			form3,
			XmNx, 240,
			XmNy, 10,
			XmNheight, 35,
			XmNwidth, 100,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	XtAddCallback( cancelBut, XmNactivateCallback,
		(XtCallbackProc) activateCB_cancelBut,
		(XtPointer) UxConfRunTimeContext );

	UxPutContext( cancelBut, (char *) UxConfRunTimeContext );


	XtAddCallback( confRunTime, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxConfRunTimeContext);


	return ( confRunTime );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_confRunTime( _UxUxParent, _UxnumCurveConf, _UxsorgDati, _UxvarInfo, _UxcolString, _Uxconf_OK )
	swidget	_UxUxParent;
	int	_UxnumCurveConf;
	unsigned char	*_UxsorgDati;
	unsigned char	**_UxvarInfo;
	unsigned char	**_UxcolString;
	int	*_Uxconf_OK;
{
	Widget                  rtrn;
	_UxCconfRunTime         *UxContext;

	UxConfRunTimeContext = UxContext =
		(_UxCconfRunTime *) UxNewContext( sizeof(_UxCconfRunTime), False );

	UxParent = _UxUxParent;
	numCurveConf = _UxnumCurveConf;
	sorgDati = _UxsorgDati;
	varInfo = _UxvarInfo;
	colString = _UxcolString;
	conf_OK = _Uxconf_OK;

	{
		int i;
		rtrn = _Uxbuild_confRunTime();

		printf("create_confRunTime: ENTRATO!!!\n");
		
		varInfoApp = (unsigned char **)calloc(NUMMAXVAR, sizeof(char *));
		colStringApp = (unsigned char **)calloc(NUMMAXVAR, sizeof(char *));
		
		/* strcpy(path_f22,sorgDati); */
		
		if(numCurveConf<10)
		   XtVaSetValues(pushBut10,XmNsensitive,False,NULL);
		if(numCurveConf<9)
		   XtVaSetValues(pushBut9,XmNsensitive,False,NULL);
		if(numCurveConf<8)
		   XtVaSetValues(pushBut8,XmNsensitive,False,NULL);
		if(numCurveConf<7)
		   XtVaSetValues(pushBut7,XmNsensitive,False,NULL);
		if(numCurveConf<6)
		   XtVaSetValues(pushBut6,XmNsensitive,False,NULL);
		if(numCurveConf<5)
		   XtVaSetValues(pushBut5,XmNsensitive,False,NULL);
		if(numCurveConf<4)
		   XtVaSetValues(pushBut4,XmNsensitive,False,NULL);
		if(numCurveConf<3)
		   XtVaSetValues(pushBut3,XmNsensitive,False,NULL);
		if(numCurveConf<2)
		   XtVaSetValues(pushBut2,XmNsensitive,False,NULL);
		/*
			Inizializzazione delle variabili d'appoggio.
		*/
		for(i=0;i<numCurveConf;i++) {
		   varInfoApp[i]=(unsigned char *)calloc(MAXLENINFOVAR,sizeof(char));
		   strcpy(varInfoApp[i],varInfo[i]);
		}
		
		for(i=0;i<numCurveConf;i++) {
		   colStringApp[i]=(unsigned char *)calloc(15,sizeof(char));
		   strcpy(colStringApp[i],colString[i]);
		
		printf("create_confRunTime: varInfoApp[%d]=%s\n",i,varInfoApp[i]);
		
		}
		
		
		UxPopupInterface(rtrn,no_grab);
		
		printf("create_confRunTime: USCITO!!!\n");
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

