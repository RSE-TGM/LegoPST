/**********************************************************************
*
*       C Source:               XlPrint.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Thu Oct 24 12:05:28 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlPrint.c-4.2.2 %  (%full_filespec: XlPrint.c-4.2.2:csrc:1 %)";
#endif
/*
   modulo XlPrint.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlPrint.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlPrint.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/

/*----------------------------------------------------------------------
 *                                                                     *
 *  XlPrint                                                            *  
 *                                                                     *
 *---------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <pagresdef.h>
#include <Xm/Xm.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlPrint.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */



static char * PixelToString(Widget ,Pixel );
static Pixel CvtStrToPixel (Widget, char *);
static char *risolvi_font(char *,int);
XtCallbackProc ApplyNewSetup();
XtCallbackProc CancelNewSetup();

#if defined XPRINTER_USED && !defined LINUX
Display *printer=NULL;
/*****************************************************************************
 * Printer Setup Data structure
 *****************************************************************************/
XpPrinterInfo *pPrinterSetupInfo=NULL; /* intialized in main */



#define DEF_FONT_FAMILY         0
#define DEF_FONT_WEIGHT         0
#define DEF_FONT_SLANT          0
#define DEF_FONT_SPACE          0
#define FONT_FATT_SIZE          7.3

static char *family[]={"-courier",
                        "-helvetica",
                        "-new century schoolbook",
                        "-symbol",
                        "-times"};
int num_family=5;

static char *weight[]={"-medium","-bold"};
int num_weight=2;

static char *slant[]={"-r","-i","-o"};
int num_slant=3;

static char *space[]={"-m-","-p-"};
int num_space=2;

#endif

Boolean XlPrintOpen(Widget drawing)
{
#if defined XPRINTER_USED && !defined LINUX
   int iret;
   char comando[200];
   CoreWidget wcore =(CoreWidget)drawing;
   GC gc_print;
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
		| GCLineStyle ;

   iret=XpUnlock(XP_UNLOCK_CODE);

   /*
	apertura della stampante
   */
   if(pPrinterSetupInfo == NULL)
	pPrinterSetupInfo = XpGetPrinterInfo();
   printer = XpOpenPrtWithInfo(pPrinterSetupInfo);
/****
   if(!XpIsOutputSet(printer)) {
                if(!(file=(char *)XbGetOutputFile(XtParent(w),NULL))){
                        XpClosePrinter(printer);
                        return;
                }
                else {
                        XpSetOutputFile(printer,file);
                        free(file);
                }
        }
*****/
   if(printer==NULL)
	return(False);
   XpMatch(XtDisplay(drawing),0,printer);
   XpStartDoc(printer,"sample");
   XpStartPage(printer);
   /*
	stampa la drawing-area
   */
printf("NOME CLASSE DRAWING=%s\n",XlGetClassName(XtClass(drawing)));
   if(!strcmp(XlGetClassName(XtClass(drawing)),"XmDrawingArea"))
	{
printf("ENTRATO in NOT ALARM\n");
        values.line_width = 0;
   	values.line_style = LineSolid;
   	values.foreground = XlPrintGetPixel(drawing, wcore->core.background_pixel);
   	values.background = XlPrintGetPixel(drawing, wcore->core.background_pixel);
   	gc_print = XCreateGC(XlDisplay(drawing), XlWindow(drawing), 
			valuemask, &values);
   	XFillRectangle(XlDisplay(drawing),XlWindow(drawing), gc_print,
		0,0, wcore->core.width,wcore->core.height);
        XFreeGC(XlDisplay(drawing),gc_print);
	}
   return(True);

#else
	/*
		xprinter non disponibile
        */
	return(False);
#endif
}

Boolean XlPrint(Widget drawing)
{
#if defined XPRINTER_USED && !defined LINUX
   XlCoreClassRec *wCoreClass;
   XlManagerClassRec *wManagerClass;
   WidgetList widget,widget_figli;
   Cardinal num_widgets,num_figli;
   int i,j;
   Boolean ret=True;



   /*
	raccoglie i figli della drawing area
   */
   XtVaGetValues( drawing, XmNnumChildren, &num_widgets,
		 XmNchildren, &widget, NULL );

   for(i=0;i<num_widgets;i++)
	if(XlIsXlWidget(widget[i]))
          {
             if( XlIsXlCore(widget[i]) )
             {
                wCoreClass=(XlCoreClassRec *)XtClass(widget[i]);
                if(!((wCoreClass->xlcore_class.print)(widget[i])))
                   ret = False;
             }
             else
             {
                wManagerClass=(XlManagerClassRec *)XtClass(widget[i]);
                if(!((wManagerClass->xlmanager_class.print)(widget[i])))
                   ret = False;
	        if(XlIsXlComposite(widget[i]))
		       ret=XlPrint(widget[i]);
             }
    }


return(ret);
#else
	/*
		xprinter non disponibile
        */
	return(False);
#endif
}

Boolean XlPrintClose()
{
#if defined XPRINTER_USED && !defined LINUX
    /*
	chiusura della stampante
    */
    XpEndPage(printer);
    XpEndDoc(printer);

    XpClosePrinter(printer);

    printer=NULL;
	return(True);
#else
	/*
		xprinter non disponibile
        */
	return(False);
#endif
}


Display *XlDisplay(Widget w)
{
#if defined XPRINTER_USED && !defined LINUX
if(printer!=NULL)
	return(printer);
else
	return(XtDisplay(w));
#else
return(XtDisplay(w));
#endif
}

Window XlWindow(Widget w)
{
#if defined XPRINTER_USED && !defined LINUX
if(printer!=NULL)
	return(0);
else
	return(XtWindow(w));
#else
return(XtWindow(w));
#endif
}

Boolean XlPrinterIsOn()
{
#if defined XPRINTER_USED && !defined LINUX
if(printer!=NULL)
	return(True);
else
	return(False);
#else
return(False);
#endif
}

Position XlPrintOffsetX(Widget w)
{
#if defined XPRINTER_USED && !defined LINUX
Position x,px;
if(!XlPrinterIsOn()) return(0);

XtVaGetValues(w,XmNx,&x,NULL);
if(XlIsXlComposite(XtParent(w)))
	{
	XtVaGetValues(XtParent(w),XmNx,&px,NULL);
	return(x+px);
	}
else
	return(x);

#else
return(0);
#endif
}


Position XlPrintOffsetY(Widget w)
{
#if defined XPRINTER_USED && !defined LINUX
Position y,py;
if(!XlPrinterIsOn()) return(0);

XtVaGetValues(w,XmNy,&y,NULL);
if(XlIsXlComposite(XtParent(w)))
	{
	XtVaGetValues(XtParent(w),XmNy,&py,NULL);
	return(y+py);
	}
else
	return(y);

#else
return(0);
#endif
}


char *XlPrinterGetFont(Widget w,char *fontname,XFontStruct *font_info)
{
int ascent=font_info->max_bounds.ascent;
int descent=font_info->max_bounds.descent;
int width=font_info->max_bounds.lbearing+font_info->max_bounds.rbearing;
int size;
char font[300];
char **f;
int num_f,i;



if((ascent+descent)>width)
	size=width;
else
	size=ascent+descent;

sprintf(font,"-adobe-courier-medium-r-normal--*-%d-300-300-*-*-iso8859-1",
	size);

printf("GetFont [%s]\n        [%s]\n\n",fontname,risolvi_font(fontname,size));

return(risolvi_font(fontname,size));
}

static char *risolvi_font(char *font,int size)
{
#if defined XPRINTER_USED && !defined LINUX
int ret_family,ret_weight,ret_slant,ret_space,ret_size;
int i;
static char ret_font[300];

/*
	per alcuni font viene cablata la size
	
	- fixed 11
*/
if(strcmp(font,"fixed")==0)
	size =11;

ret_family=DEF_FONT_FAMILY;
ret_weight=DEF_FONT_WEIGHT;
ret_slant=DEF_FONT_SLANT;
ret_space=DEF_FONT_SPACE;
ret_size=(int)(FONT_FATT_SIZE*(float)size/20);
ret_size*=20;
printf ("risolvi_font: size=%d - ret_size=%d \n", size,  ret_size);

for(i=0;i<num_family;i++)
        if(strstr(font,family[i])!=NULL)
                {
                ret_family=i;
                break;
                }

for(i=0;i<num_weight;i++)
        if(strstr(font,weight[i])!=NULL)
                {
                ret_weight=i;
                break;
                }

for(i=0;i<num_slant;i++)
        if(strstr(font,slant[i])!=NULL)
                {
                ret_slant=i;
                break;
                }

for(i=0;i<num_space;i++)
        if(strstr(font,space[i])!=NULL)
                {
                ret_space=i;
                break;
                }


/* sprintf(ret_font,"-adobe%s%s%s-normal--*-%d-300-300%s*-iso8859-1", */
sprintf(ret_font,"-adobe%s%s%s-normal--*-%d-*-*%s*-iso8859-1",
        family[ret_family], weight[ret_weight],slant[ret_slant],ret_size,
		space[ret_space]);

printf("risolvi_font: font ritornato = %s\n",ret_font);

return(ret_font);
#endif
}


Pixel XlPrintGetPixel(Widget w,Pixel pix)
{
#if defined XPRINTER_USED && !defined LINUX
Pixel pixel_ret;

if(!XlPrinterIsOn())
	return(pix);

	pixel_ret=CvtStrToPixel (w,PixelToString(w,pix));
	return(pixel_ret);
#else
return(pix);
#endif
}


static char * PixelToString(Widget wid,Pixel pix)
{
Arg args[3];
XColor Colore;
Colormap cmap;
char *vstring;
Display *Displ;

        /*  Ricavo il display dal widget passato come parametro */
        Displ = XtDisplay (wid);
        Colore.pixel=pix;
        XtSetArg (args[0], XmNcolormap, &cmap);
        XtGetValues (wid, args, 1);
        XQueryColor (Displ, cmap, &Colore);
        vstring = (char *)XtCalloc (15, sizeof(char));
        sprintf (vstring,"#%04x%04x%04x",Colore.red,Colore.green,Colore.blue);
        return (vstring);
}


static Pixel CvtStrToPixel (Widget ogget,char *Stringa)
{
        unsigned long piani[1];
        Colormap colmap;
        XColor xcol;
	Pixel pix;


        colmap = DefaultColormap ( XlDisplay(ogget),
                        DefaultScreen (XtDisplay(ogget)) );
        if ( !XParseColor (XlDisplay(ogget) , colmap , Stringa , &xcol) )
           {
           printf("Errore XParseColor: colore [%s] inesistente\n",Stringa);
           return(False);
           }
        if(!XAllocColor(XlDisplay(ogget),colmap,&xcol))
           {
           printf("Errore XAllocColor: impossibile allocare un nuovo colore\n");
           return(False);
           }
        pix = xcol.pixel;
        return(pix);
}

Boolean XlHardCopyWidget(Widget w)
{
#if defined XPRINTER_USED && !defined LINUX
Position x,y;
int xw,yw;
int width,height;
Window win;


/* Problema della stampa dei bitmap (config). Quando un bitmap e' unmanaged,
   ad esempio nella stampa dalla main window, la config cade. Per evitare
   cio' non lo si stampa. PROVVISORIO!!.  */

/* Inizio modifica */
printf("ENTRATO NELLA XLHard \n\n");
 						  
if(XtIsRealized(w) == False)
	return(True); 

/* Fine modifica */


if(!XlPrinterIsOn())
	return(False);

x=XlPrintOffsetX(w);
y=XlPrintOffsetY(w);
printf("XlHard: x=%ld y=%ld \n",x,y);
/*
	ricavo le coordinate assolute della Window del Widget w
*/

XTranslateCoordinates(XtDisplay(w),XtWindow(w),
		RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
		w->core.x,w->core.y,&xw,&yw,&win);


width=DisplayWidth(XtDisplay(w),DefaultScreen(XtDisplay(w)));
height=DisplayHeight(XtDisplay(w),DefaultScreen(XtDisplay(w)));
/*
	il widget e fuori dallo screen e qundi non lo stampo
*/
/****** patch momentanea
if((xw>width)||(yw>height))
	return(True);
***************/


XpPrintWindow(XlDisplay(w),XtDisplay(w),0,XtWindow(w),x,y);

return(True);
#else
return(True);
#endif
}


Boolean XlPrintBackgroundAndBorder(Widget cw, int trasparent)
{
GC gc_print;
CoreWidget wcore =(CoreWidget)cw;
Position offX,offY;
Dimension size;
XGCValues values;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
		| GCLineStyle ;
/*
 se transparent vale 100:
	-> caso particolare di stampa di una window interna
           all'oggetto XlGrafico.
	   e' sempre NON trasparente ed il calcolo dell'offset per la stampa
           va effettuato diversamente (c'e' un parent in piu' da
           considerare
*/

if(trasparent == 100)
	{
	Dimension bordo;
	int offxparent;
	int offyparent;
	get_something(cw,XmNx, (void*) &offX);
        get_something(cw,XmNy, (void*) &offY);
	get_something(cw,XmNborderWidth, (void*) &bordo);
	offX+=bordo;
        offY+=bordo;
        get_something(XtParent(cw),XmNx, (void*) &offxparent);
        get_something(XtParent(cw),XmNy, (void*) &offyparent);
	get_something(XtParent(cw),XmNborderWidth, (void*) &bordo);
	offX+=bordo;
        offY+=bordo;
        offX+=offxparent;
        offY+=offyparent;
	get_something(XtParent(XtParent(cw)),XmNx, (void*) &offxparent);
        get_something(XtParent(XtParent(cw)),XmNy, (void*) &offyparent);
	get_something(XtParent(XtParent(cw)),XmNborderWidth, (void*) &bordo);
	offX+=bordo;
        offY+=bordo;
        offX+=offxparent;
        offY+=offyparent;
	trasparent=0;
	}
else   /* caso normale */
	{
	offX=XlPrintOffsetX(cw);
        offY=XlPrintOffsetY(cw);
	}


	if(trasparent == 0)
	   {
	   values.line_width = 0;
           values.line_style = LineSolid;
	   values.foreground = 
		XlPrintGetPixel(cw, wcore->core.background_pixel);
	   values.background = 
		XlPrintGetPixel(cw, wcore->core.background_pixel);
           gc_print = 
		XCreateGC(XlDisplay(cw), XlWindow(cw), valuemask, &values);
           XFillRectangle(XlDisplay(cw),XlWindow(cw), gc_print,
		offX,offY, wcore->core.width,wcore->core.height);
	   XFreeGC(XlDisplay(cw),gc_print);
	   }

	if(wcore->core.border_width >0)
	   {
	   values.line_width = wcore->core.border_width;
           values.line_style = LineSolid;
           values.foreground = XlPrintGetPixel(cw, wcore->core.border_pixel);
           values.background = XlPrintGetPixel(cw, wcore->core.border_pixel);
           gc_print = 
		XCreateGC(XlDisplay(cw), XlWindow(cw), valuemask, &values);
	   size=wcore->core.border_width;
           XDrawRectangle(XlDisplay(cw),XlWindow(cw), gc_print,
		   offX,offY, 
		   wcore->core.width,wcore->core.height);
	   XFreeGC(XlDisplay(cw),gc_print);
	   }
}


/*****************************************************************************
 * PrintSetup
 *
 *   PrintSetup is called from HandleMenuSelection.  It creates a XbPrinterSetupWidget
 *   dialog box so that the user can change printer options.
 *
 *****************************************************************************/
#if defined XPRINTER_USED && !defined LINUX
XtCallbackProc
ApplyNewSetup(Widget w,
        caddr_t client_data,
        XbPrinterBoxCallbackStruct *pPrinterBoxCallbackStruct)
{
        /* free old setup and get the new one */
        free (pPrinterSetupInfo);
        pPrinterSetupInfo = pPrinterBoxCallbackStruct->xpinfo;
        XtDestroyWidget(XtParent(w));
}


XtCallbackProc
CancelNewSetup(Widget w,
        caddr_t client_data,
        XbPrinterBoxCallbackStruct *pPrinterBoxCallbackStruct)
{
        /* free old setup and get the new one */
        if(pPrinterBoxCallbackStruct->xpinfo)
                free(pPrinterBoxCallbackStruct->xpinfo);
        XtDestroyWidget(XtParent(w));
}

Widget
XlPrintSetup(Widget w)
{
        Arg al[2];
        int ac = 0;
        int iret;
        Widget wPrinterSetupDialog;
        iret=XpUnlock(XP_UNLOCK_CODE);

   	if(pPrinterSetupInfo == NULL)
	pPrinterSetupInfo = XpGetPrinterInfo();

        XtSetArg(al[ac],XbNprinterInfo,pPrinterSetupInfo), ac++;
        XtSetArg(al[ac],XmNtitle,"Printer Setup"), ac++;
        wPrinterSetupDialog = XbCreatePrinterDialog(w,"printerManager",al,ac);
        XtAddCallback(wPrinterSetupDialog, XbNapplyCallback,(XtCallbackProc)ApplyNewSetup,0);
        XtAddCallback(wPrinterSetupDialog, XbNcancelCallback,(XtCallbackProc)CancelNewSetup,0);
        XtManageChild(wPrinterSetupDialog);
        XtPopup(XtParent(wPrinterSetupDialog),XtGrabNone);
return(wPrinterSetupDialog);
}
           
#endif
