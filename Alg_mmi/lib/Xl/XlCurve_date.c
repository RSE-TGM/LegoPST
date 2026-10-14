
/*******************************************************************************
       XlCurve_date.c
       (Generated from interface file XlCurve_date.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/DialogS.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <stdlib.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define MULT_STEP 10
#define HOUR_LIMIT 99
#define MINUTE_LIMIT 59

/*
extern Boolean seldata_OK;

extern int hour_sel,minute_sel;
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
	Widget	UxsetDate;
	Widget	Uxformdate;
	Widget	Uxformlab;
	Widget	Uxlabeldd;
	Widget	Uxlabelmm;
	Widget	Uxlabelyy;
	Widget	Uxlabelhr;
	Widget	Uxlabelmn;
	Widget	Uxformarrow;
	Widget	Uxformarrow1;
	Widget	UxarrowButUp1;
	Widget	UxarrowButUp2;
	Widget	Uxlabelval;
	Widget	UxarrowButDown1;
	Widget	UxarrowButDown2;
	Widget	Uxformarrow2;
	Widget	UxarrowButUp3;
	Widget	UxarrowButUp4;
	Widget	Uxlabelval1;
	Widget	UxarrowButDown3;
	Widget	UxarrowButDown4;
	Widget	Uxformarrow3;
	Widget	UxarrowButUp5;
	Widget	UxarrowButUp6;
	Widget	Uxlabelval2;
	Widget	UxarrowButDown5;
	Widget	UxarrowButDown6;
	Widget	Uxformarrow4;
	Widget	UxarrowButUp7;
	Widget	UxarrowButUp8;
	Widget	Uxlabelval3;
	Widget	UxarrowButDown7;
	Widget	UxarrowButDown8;
	Widget	Uxformarrow5;
	Widget	UxarrowButUp9;
	Widget	UxarrowButUp10;
	Widget	Uxlabelval4;
	Widget	UxarrowButDown9;
	Widget	UxarrowButDown10;
	Widget	UxpushButOK;
	Widget	UxpushButCanc;
	swidget	UxUxParent;
	Boolean	*Uxsel_OK;
	int	*Uxhour;
	int	*Uxminute;
} _UxCsetDate;

static _UxCsetDate             *UxSetDateContext;
#define setDate                 UxSetDateContext->UxsetDate
#define formdate                UxSetDateContext->Uxformdate
#define formlab                 UxSetDateContext->Uxformlab
#define labeldd                 UxSetDateContext->Uxlabeldd
#define labelmm                 UxSetDateContext->Uxlabelmm
#define labelyy                 UxSetDateContext->Uxlabelyy
#define labelhr                 UxSetDateContext->Uxlabelhr
#define labelmn                 UxSetDateContext->Uxlabelmn
#define formarrow               UxSetDateContext->Uxformarrow
#define formarrow1              UxSetDateContext->Uxformarrow1
#define arrowButUp1             UxSetDateContext->UxarrowButUp1
#define arrowButUp2             UxSetDateContext->UxarrowButUp2
#define labelval                UxSetDateContext->Uxlabelval
#define arrowButDown1           UxSetDateContext->UxarrowButDown1
#define arrowButDown2           UxSetDateContext->UxarrowButDown2
#define formarrow2              UxSetDateContext->Uxformarrow2
#define arrowButUp3             UxSetDateContext->UxarrowButUp3
#define arrowButUp4             UxSetDateContext->UxarrowButUp4
#define labelval1               UxSetDateContext->Uxlabelval1
#define arrowButDown3           UxSetDateContext->UxarrowButDown3
#define arrowButDown4           UxSetDateContext->UxarrowButDown4
#define formarrow3              UxSetDateContext->Uxformarrow3
#define arrowButUp5             UxSetDateContext->UxarrowButUp5
#define arrowButUp6             UxSetDateContext->UxarrowButUp6
#define labelval2               UxSetDateContext->Uxlabelval2
#define arrowButDown5           UxSetDateContext->UxarrowButDown5
#define arrowButDown6           UxSetDateContext->UxarrowButDown6
#define formarrow4              UxSetDateContext->Uxformarrow4
#define arrowButUp7             UxSetDateContext->UxarrowButUp7
#define arrowButUp8             UxSetDateContext->UxarrowButUp8
#define labelval3               UxSetDateContext->Uxlabelval3
#define arrowButDown7           UxSetDateContext->UxarrowButDown7
#define arrowButDown8           UxSetDateContext->UxarrowButDown8
#define formarrow5              UxSetDateContext->Uxformarrow5
#define arrowButUp9             UxSetDateContext->UxarrowButUp9
#define arrowButUp10            UxSetDateContext->UxarrowButUp10
#define labelval4               UxSetDateContext->Uxlabelval4
#define arrowButDown9           UxSetDateContext->UxarrowButDown9
#define arrowButDown10          UxSetDateContext->UxarrowButDown10
#define pushButOK               UxSetDateContext->UxpushButOK
#define pushButCanc             UxSetDateContext->UxpushButCanc
#define UxParent                UxSetDateContext->UxUxParent
#define sel_OK                  UxSetDateContext->Uxsel_OK
#define hour                    UxSetDateContext->Uxhour
#define minute                  UxSetDateContext->Uxminute



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_setDate();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_arrowButUp2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_dd;
	XmString compound;
	char str_new[5];
	int dd;
	
	XtVaGetValues(labelval,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_dd);
	XmStringFree(compound);
	printf("callback_date: string_dd=%s\n",str_dd);
	dd=atoi(str_dd);
	if(dd<31)
	   dd++;
	else
	   dd=1;
	if(dd<10)
	   sprintf(str_new,"0%d",dd);
	else
	   sprintf(str_new,"%d",dd);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	
	
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButUp7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_hr;
	XmString compound;
	char str_new[5];
	int hr;
	
	XtVaGetValues(labelval3,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_hr);
	XmStringFree(compound);
	printf("callback_date: string_hr=%s\n",str_hr);
	hr=atoi(str_hr);
	if(hr<(100-MULT_STEP))
	   hr=hr+MULT_STEP;
	else
	   hr=hr-(100-MULT_STEP);
	if(hr<10)
	   sprintf(str_new,"0%d",hr);
	else
	   sprintf(str_new,"%d",hr);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval3,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	   
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButUp8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_hr;
	XmString compound;
	char str_new[5];
	int hr;
	
	XtVaGetValues(labelval3,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_hr);
	XmStringFree(compound);
	printf("callback_date: string_hr=%s\n",str_hr);
	hr=atoi(str_hr);
	if(hr<HOUR_LIMIT)
	   hr++;
	else
	   hr=0;
	if(hr<10)
	   sprintf(str_new,"0%d",hr);
	else
	   sprintf(str_new,"%d",hr);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval3,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButDown7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_hr;
	XmString compound;
	char str_new[5];
	int hr;
	
	XtVaGetValues(labelval3,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_hr);
	XmStringFree(compound);
	printf("callback_date: string_dd=%s\n",str_hr);
	hr=atoi(str_hr);
	if(hr!=0)
	   hr--;
	else
	   hr=HOUR_LIMIT;
	if(hr<10)
	   sprintf(str_new,"0%d",hr);
	else
	   sprintf(str_new,"%d",hr);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval3,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButDown8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_hr;
	XmString compound;
	char str_new[5];
	int hr;
	
	XtVaGetValues(labelval3,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_hr);
	XmStringFree(compound);
	printf("callback_date: string_hr=%s\n",str_hr);
	hr=atoi(str_hr);
	if(hr>=10)
	   hr=hr-MULT_STEP;
	else
	   hr=100+(hr-MULT_STEP);
	if(hr<10)
	   sprintf(str_new,"0%d",hr);
	else
	   sprintf(str_new,"%d",hr);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval3,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButUp9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_min;
	XmString compound;
	char str_new[5];
	int min;
	
	XtVaGetValues(labelval4,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_min);
	XmStringFree(compound);
	printf("callback_date: string_min=%s\n",str_min);
	min=atoi(str_min);
	if(min<=(MINUTE_LIMIT-MULT_STEP))
	   min=min+MULT_STEP;
	else
	   min=min-(60-MULT_STEP);
	if(min<10)
	   sprintf(str_new,"0%d",min);
	else
	   sprintf(str_new,"%d",min);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval4,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButUp10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_min;
	XmString compound;
	char str_new[5];
	int min;
	
	XtVaGetValues(labelval4,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_min);
	XmStringFree(compound);
	printf("callback_date: string_dd=%s\n",str_min);
	min=atoi(str_min);
	printf("callback_date: minuti=%d\n",min);
	if(min<MINUTE_LIMIT)
	   min++;
	else
	   min=0;
	printf("callback_date: minuti_after=%d\n",min);
	if(min<10)
	   sprintf(str_new,"0%d",min);
	else
	   sprintf(str_new,"%d",min);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval4,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButDown9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_min;
	XmString compound;
	char str_new[5];
	int min;
	
	XtVaGetValues(labelval4,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_min);
	XmStringFree(compound);
	printf("callback_date: string_dd=%s\n",str_min);
	min=atoi(str_min);
	if(min!=0)
	   min--;
	else
	   min=MINUTE_LIMIT;
	if(min<10)
	   sprintf(str_new,"0%d",min);
	else
	   sprintf(str_new,"%d",min);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval4,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_arrowButDown10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str_min;
	XmString compound;
	char str_new[5];
	int min;
	
	XtVaGetValues(labelval4,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str_min);
	XmStringFree(compound);
	printf("callback_date: string_min=%s\n",str_min);
	min=atoi(str_min);
	if(min>=MULT_STEP)
	   min=min-MULT_STEP;
	else
	   min=60+(min-MULT_STEP);
	if(min<10)
	   sprintf(str_new,"0%d",min);
	else
	   sprintf(str_new,"%d",min);
	compound=XmStringCreateSimple(str_new);
	XtVaSetValues(labelval4,XmNlabelString,compound,NULL);
	XmStringFree(compound);
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_pushButOK( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	String str;
	XmString compound;
	
	XtVaGetValues(labelval3,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
	XmStringFree(compound);
	*hour=atoi(str);
	
	XtVaGetValues(labelval4,XmNlabelString,&compound,NULL);
	XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
	XmStringFree(compound);
	*minute=atoi(str);
	
	printf("datasel: hour_sel=%d\tminute_sel=%d\n",*hour,*minute);
	
	*sel_OK=True;
	
	UxDestroyInterface(UxThisWidget);
	
	
	}
	UxSetDateContext = UxSaveCtx;
}

static	void	activateCB_pushButCanc( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCsetDate             *UxSaveCtx, *UxContext;

	UxSaveCtx = UxSetDateContext;
	UxSetDateContext = UxContext =
			(_UxCsetDate *) UxGetContext( UxWidget );
	{
	UxDestroyInterface(UxThisWidget);
	}
	UxSetDateContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_setDate()
{
	Widget		_UxParent;


	/* Creation of setDate */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	setDate = XtVaCreatePopupShell( "setDate",
			xmDialogShellWidgetClass,
			_UxParent,
			XmNwidth, 270,
			XmNheight, 210,
			XmNx, 480,
			XmNy, 760,
			NULL );
	UxPutContext( setDate, (char *) UxSetDateContext );


	/* Creation of formdate */
	formdate = XtVaCreateWidget( "formdate",
			xmFormWidgetClass,
			setDate,
			XmNwidth, 270,
			XmNheight, 220,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 70,
			XmNy, 110,
			XmNunitType, XmPIXELS,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNnoResize, TRUE,
			XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
			NULL );
	UxPutContext( formdate, (char *) UxSetDateContext );


	/* Creation of formlab */
	formlab = XtVaCreateManagedWidget( "formlab",
			xmFormWidgetClass,
			formdate,
			XmNwidth, 210,
			XmNheight, 30,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 30,
			XmNy, 20,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNleftOffset, 30,
			XmNtopOffset, 20,
			NULL );
	UxPutContext( formlab, (char *) UxSetDateContext );


	/* Creation of labeldd */
	labeldd = XtVaCreateManagedWidget( "labeldd",
			xmLabelWidgetClass,
			formlab,
			XmNx, 5,
			XmNy, 5,
			RES_CONVERT( XmNlabelString, "DD" ),
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNmarginBottom, 4,
			XmNtopOffset, 0,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNwidth, 30,
			NULL );
	UxPutContext( labeldd, (char *) UxSetDateContext );


	/* Creation of labelmm */
	labelmm = XtVaCreateManagedWidget( "labelmm",
			xmLabelWidgetClass,
			formlab,
			XmNx, 40,
			XmNy, 10,
			RES_CONVERT( XmNlabelString, "MM" ),
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNmarginBottom, 4,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNwidth, 30,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, labeldd,
			NULL );
	UxPutContext( labelmm, (char *) UxSetDateContext );


	/* Creation of labelyy */
	labelyy = XtVaCreateManagedWidget( "labelyy",
			xmLabelWidgetClass,
			formlab,
			XmNx, 80,
			XmNy, 10,
			RES_CONVERT( XmNlabelString, "YY" ),
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNmarginBottom, 4,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNwidth, 30,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, labelmm,
			NULL );
	UxPutContext( labelyy, (char *) UxSetDateContext );


	/* Creation of labelhr */
	labelhr = XtVaCreateManagedWidget( "labelhr",
			xmLabelWidgetClass,
			formlab,
			XmNx, 130,
			XmNy, 10,
			RES_CONVERT( XmNlabelString, "Hr" ),
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNmarginBottom, 4,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNwidth, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 20,
			XmNleftWidget, labelyy,
			NULL );
	UxPutContext( labelhr, (char *) UxSetDateContext );


	/* Creation of labelmn */
	labelmn = XtVaCreateManagedWidget( "labelmn",
			xmLabelWidgetClass,
			formlab,
			XmNx, 170,
			XmNy, 0,
			RES_CONVERT( XmNlabelString, "Mn" ),
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNmarginBottom, 4,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			XmNwidth, 30,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, labelhr,
			NULL );
	UxPutContext( labelmn, (char *) UxSetDateContext );


	/* Creation of formarrow */
	formarrow = XtVaCreateManagedWidget( "formarrow",
			xmFormWidgetClass,
			formdate,
			XmNwidth, 210,
			XmNheight, 120,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 30,
			XmNy, 50,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow, (char *) UxSetDateContext );


	/* Creation of formarrow1 */
	formarrow1 = XtVaCreateManagedWidget( "formarrow1",
			xmFormWidgetClass,
			formarrow,
			XmNwidth, 30,
			XmNheight, 110,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 5,
			XmNy, 5,
			XmNtopOffset, 5,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow1, (char *) UxSetDateContext );


	/* Creation of arrowButUp1 */
	arrowButUp1 = XtVaCreateManagedWidget( "arrowButUp1",
			xmArrowButtonWidgetClass,
			formarrow1,
			XmNx, 0,
			XmNy, 0,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButUp1, (char *) UxSetDateContext );


	/* Creation of arrowButUp2 */
	arrowButUp2 = XtVaCreateManagedWidget( "arrowButUp2",
			xmArrowButtonWidgetClass,
			formarrow1,
			XmNx, 0,
			XmNy, 20,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	XtAddCallback( arrowButUp2, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButUp2,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButUp2, (char *) UxSetDateContext );


	/* Creation of labelval */
	labelval = XtVaCreateManagedWidget( "labelval",
			xmLabelWidgetClass,
			formarrow1,
			XmNx, 2,
			XmNy, 39,
			XmNheight, 27,
			XmNwidth, 26,
			RES_CONVERT( XmNlabelString, "00" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNborderWidth, 0,
			RES_CONVERT( XmNborderColor, "#a05050" ),
			RES_CONVERT( XmNbottomShadowColor, "SaddleBrown" ),
			XmNshadowThickness, 0,
			RES_CONVERT( XmNtopShadowColor, "#8b4513" ),
			XmNtopOffset, 38,
			XmNsensitive, TRUE,
			NULL );
	UxPutContext( labelval, (char *) UxSetDateContext );


	/* Creation of arrowButDown1 */
	arrowButDown1 = XtVaCreateManagedWidget( "arrowButDown1",
			xmArrowButtonWidgetClass,
			formarrow1,
			XmNx, 0,
			XmNy, 65,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown1, (char *) UxSetDateContext );


	/* Creation of arrowButDown2 */
	arrowButDown2 = XtVaCreateManagedWidget( "arrowButDown2",
			xmArrowButtonWidgetClass,
			formarrow1,
			XmNx, 0,
			XmNy, 85,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown2, (char *) UxSetDateContext );


	/* Creation of formarrow2 */
	formarrow2 = XtVaCreateManagedWidget( "formarrow2",
			xmFormWidgetClass,
			formarrow,
			XmNwidth, 30,
			XmNheight, 110,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 44,
			XmNy, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow2, (char *) UxSetDateContext );


	/* Creation of arrowButUp3 */
	arrowButUp3 = XtVaCreateManagedWidget( "arrowButUp3",
			xmArrowButtonWidgetClass,
			formarrow2,
			XmNx, 0,
			XmNy, 0,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButUp3, (char *) UxSetDateContext );


	/* Creation of arrowButUp4 */
	arrowButUp4 = XtVaCreateManagedWidget( "arrowButUp4",
			xmArrowButtonWidgetClass,
			formarrow2,
			XmNx, 0,
			XmNy, 20,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButUp4, (char *) UxSetDateContext );


	/* Creation of labelval1 */
	labelval1 = XtVaCreateManagedWidget( "labelval1",
			xmLabelWidgetClass,
			formarrow2,
			XmNx, 2,
			XmNy, 39,
			XmNheight, 27,
			XmNwidth, 26,
			RES_CONVERT( XmNlabelString, "00" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNborderWidth, 0,
			RES_CONVERT( XmNborderColor, "#a05050" ),
			NULL );
	UxPutContext( labelval1, (char *) UxSetDateContext );


	/* Creation of arrowButDown3 */
	arrowButDown3 = XtVaCreateManagedWidget( "arrowButDown3",
			xmArrowButtonWidgetClass,
			formarrow2,
			XmNx, 0,
			XmNy, 65,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown3, (char *) UxSetDateContext );


	/* Creation of arrowButDown4 */
	arrowButDown4 = XtVaCreateManagedWidget( "arrowButDown4",
			xmArrowButtonWidgetClass,
			formarrow2,
			XmNx, 0,
			XmNy, 85,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown4, (char *) UxSetDateContext );


	/* Creation of formarrow3 */
	formarrow3 = XtVaCreateManagedWidget( "formarrow3",
			xmFormWidgetClass,
			formarrow,
			XmNwidth, 30,
			XmNheight, 110,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 83,
			XmNy, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow3, (char *) UxSetDateContext );


	/* Creation of arrowButUp5 */
	arrowButUp5 = XtVaCreateManagedWidget( "arrowButUp5",
			xmArrowButtonWidgetClass,
			formarrow3,
			XmNx, 0,
			XmNy, 0,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButUp5, (char *) UxSetDateContext );


	/* Creation of arrowButUp6 */
	arrowButUp6 = XtVaCreateManagedWidget( "arrowButUp6",
			xmArrowButtonWidgetClass,
			formarrow3,
			XmNx, 0,
			XmNy, 20,
			XmNheight, 20,
			XmNwidth, 30,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButUp6, (char *) UxSetDateContext );


	/* Creation of labelval2 */
	labelval2 = XtVaCreateManagedWidget( "labelval2",
			xmLabelWidgetClass,
			formarrow3,
			XmNx, 2,
			XmNy, 39,
			XmNheight, 27,
			XmNwidth, 26,
			RES_CONVERT( XmNlabelString, "00" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNborderWidth, 0,
			RES_CONVERT( XmNborderColor, "#a05050" ),
			NULL );
	UxPutContext( labelval2, (char *) UxSetDateContext );


	/* Creation of arrowButDown5 */
	arrowButDown5 = XtVaCreateManagedWidget( "arrowButDown5",
			xmArrowButtonWidgetClass,
			formarrow3,
			XmNx, 0,
			XmNy, 65,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown5, (char *) UxSetDateContext );


	/* Creation of arrowButDown6 */
	arrowButDown6 = XtVaCreateManagedWidget( "arrowButDown6",
			xmArrowButtonWidgetClass,
			formarrow3,
			XmNx, 0,
			XmNy, 85,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( arrowButDown6, (char *) UxSetDateContext );


	/* Creation of formarrow4 */
	formarrow4 = XtVaCreateManagedWidget( "formarrow4",
			xmFormWidgetClass,
			formarrow,
			XmNwidth, 30,
			XmNheight, 110,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 132,
			XmNy, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow4, (char *) UxSetDateContext );


	/* Creation of arrowButUp7 */
	arrowButUp7 = XtVaCreateManagedWidget( "arrowButUp7",
			xmArrowButtonWidgetClass,
			formarrow4,
			XmNx, 0,
			XmNy, 0,
			XmNheight, 20,
			XmNwidth, 30,
			NULL );
	XtAddCallback( arrowButUp7, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButUp7,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButUp7, (char *) UxSetDateContext );


	/* Creation of arrowButUp8 */
	arrowButUp8 = XtVaCreateManagedWidget( "arrowButUp8",
			xmArrowButtonWidgetClass,
			formarrow4,
			XmNx, 0,
			XmNy, 20,
			XmNheight, 20,
			XmNwidth, 30,
			NULL );
	XtAddCallback( arrowButUp8, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButUp8,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButUp8, (char *) UxSetDateContext );


	/* Creation of labelval3 */
	labelval3 = XtVaCreateManagedWidget( "labelval3",
			xmLabelWidgetClass,
			formarrow4,
			XmNx, 2,
			XmNy, 39,
			XmNheight, 27,
			XmNwidth, 26,
			RES_CONVERT( XmNlabelString, "00" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNborderWidth, 0,
			RES_CONVERT( XmNborderColor, "#a05050" ),
			NULL );
	UxPutContext( labelval3, (char *) UxSetDateContext );


	/* Creation of arrowButDown7 */
	arrowButDown7 = XtVaCreateManagedWidget( "arrowButDown7",
			xmArrowButtonWidgetClass,
			formarrow4,
			XmNx, 0,
			XmNy, 65,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			NULL );
	XtAddCallback( arrowButDown7, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButDown7,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButDown7, (char *) UxSetDateContext );


	/* Creation of arrowButDown8 */
	arrowButDown8 = XtVaCreateManagedWidget( "arrowButDown8",
			xmArrowButtonWidgetClass,
			formarrow4,
			XmNx, 0,
			XmNy, 85,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			NULL );
	XtAddCallback( arrowButDown8, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButDown8,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButDown8, (char *) UxSetDateContext );


	/* Creation of formarrow5 */
	formarrow5 = XtVaCreateManagedWidget( "formarrow5",
			xmFormWidgetClass,
			formarrow,
			XmNwidth, 30,
			XmNheight, 110,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 171,
			XmNy, 5,
			RES_CONVERT( XmNbackground, "WhiteSmoke" ),
			NULL );
	UxPutContext( formarrow5, (char *) UxSetDateContext );


	/* Creation of arrowButUp9 */
	arrowButUp9 = XtVaCreateManagedWidget( "arrowButUp9",
			xmArrowButtonWidgetClass,
			formarrow5,
			XmNx, 0,
			XmNy, 0,
			XmNheight, 20,
			XmNwidth, 30,
			NULL );
	XtAddCallback( arrowButUp9, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButUp9,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButUp9, (char *) UxSetDateContext );


	/* Creation of arrowButUp10 */
	arrowButUp10 = XtVaCreateManagedWidget( "arrowButUp10",
			xmArrowButtonWidgetClass,
			formarrow5,
			XmNx, 0,
			XmNy, 20,
			XmNheight, 20,
			XmNwidth, 30,
			NULL );
	XtAddCallback( arrowButUp10, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButUp10,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButUp10, (char *) UxSetDateContext );


	/* Creation of labelval4 */
	labelval4 = XtVaCreateManagedWidget( "labelval4",
			xmLabelWidgetClass,
			formarrow5,
			XmNx, 2,
			XmNy, 39,
			XmNheight, 27,
			XmNwidth, 26,
			RES_CONVERT( XmNlabelString, "00" ),
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--20-140-100-100-m-110-iso8859-1" ),
			XmNborderWidth, 0,
			RES_CONVERT( XmNborderColor, "#a05050" ),
			NULL );
	UxPutContext( labelval4, (char *) UxSetDateContext );


	/* Creation of arrowButDown9 */
	arrowButDown9 = XtVaCreateManagedWidget( "arrowButDown9",
			xmArrowButtonWidgetClass,
			formarrow5,
			XmNx, 0,
			XmNy, 65,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			NULL );
	XtAddCallback( arrowButDown9, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButDown9,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButDown9, (char *) UxSetDateContext );


	/* Creation of arrowButDown10 */
	arrowButDown10 = XtVaCreateManagedWidget( "arrowButDown10",
			xmArrowButtonWidgetClass,
			formarrow5,
			XmNx, 0,
			XmNy, 85,
			XmNheight, 20,
			XmNwidth, 30,
			XmNarrowDirection, XmARROW_DOWN,
			NULL );
	XtAddCallback( arrowButDown10, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrowButDown10,
		(XtPointer) UxSetDateContext );

	UxPutContext( arrowButDown10, (char *) UxSetDateContext );


	/* Creation of pushButOK */
	pushButOK = XtVaCreateManagedWidget( "pushButOK",
			xmPushButtonWidgetClass,
			formdate,
			XmNx, 35,
			XmNy, 175,
			RES_CONVERT( XmNlabelString, "OK" ),
			XmNrecomputeSize, TRUE,
			XmNwidth, 60,
			XmNheight, 30,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			XmNmarginHeight, 0,
			XmNmarginBottom, 2,
			XmNmarginTop, 2,
			NULL );
	XtAddCallback( pushButOK, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButOK,
		(XtPointer) UxSetDateContext );

	UxPutContext( pushButOK, (char *) UxSetDateContext );


	/* Creation of pushButCanc */
	pushButCanc = XtVaCreateManagedWidget( "pushButCanc",
			xmPushButtonWidgetClass,
			formdate,
			XmNx, 140,
			XmNy, 175,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNrecomputeSize, TRUE,
			XmNwidth, 110,
			XmNheight, 30,
			XmNfontList, UxConvertFontList( "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1" ),
			XmNmarginHeight, 0,
			XmNmarginBottom, 2,
			XmNmarginTop, 2,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 40,
			XmNleftWidget, pushButOK,
			XmNtopOffset, 175,
			NULL );
	XtAddCallback( pushButCanc, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButCanc,
		(XtPointer) UxSetDateContext );

	UxPutContext( pushButCanc, (char *) UxSetDateContext );

	XtVaSetValues(formdate,
			XmNinitialFocus, pushButOK,
			NULL );


	XtAddCallback( setDate, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxSetDateContext);


	return ( setDate );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_setDate( _UxUxParent, _Uxsel_OK, _Uxhour, _Uxminute )
	swidget	_UxUxParent;
	Boolean	*_Uxsel_OK;
	int	*_Uxhour;
	int	*_Uxminute;
{
	Widget                  rtrn;
	_UxCsetDate             *UxContext;

	UxSetDateContext = UxContext =
		(_UxCsetDate *) UxNewContext( sizeof(_UxCsetDate), False );

	UxParent = _UxUxParent;
	sel_OK = _Uxsel_OK;
	hour = _Uxhour;
	minute = _Uxminute;

	{
		int giorno,mese,anno;
		XmString compound;
		char app[5];
		rtrn = _Uxbuild_setDate();

		data(&giorno,&mese,&anno);
		
		sprintf(app,"%2d",giorno);
		compound=XmStringCreateSimple(app);
		XtVaSetValues(labelval,XmNlabelString,compound,NULL);
		XmStringFree(compound);
		
		sprintf(app,"%2d",mese);
		compound=XmStringCreateSimple(app);
		XtVaSetValues(labelval1,XmNlabelString,compound,NULL);
		XmStringFree(compound);
		
		sprintf(app,"%2d",anno);
		compound=XmStringCreateSimple(app);
		XtVaSetValues(labelval2,XmNlabelString,compound,NULL);
		XmStringFree(compound);
		
		sprintf(app,"%2d",*hour);
		compound=XmStringCreateSimple(app);
		XtVaSetValues(labelval3,XmNlabelString,compound,NULL);
		XmStringFree(compound);
		
		sprintf(app,"%2d",*minute);
		compound=XmStringCreateSimple(app);
		XtVaSetValues(labelval4,XmNlabelString,compound,NULL);
		XmStringFree(compound);
		
		UxPopupInterface(rtrn,no_grab);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

