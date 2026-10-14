/**********************************************************************
*
*       C Source:               XlUtilBkg.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Apr 28 10:22:32 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlUtilBkg.c-10 %  (%full_filespec: XlUtilBkg.c-10:csrc:1 %)";
#endif
/*
   modulo XlUtilBkg.c
   tipo 
   release 5.2
   data 2/16/96
   reserved @(#)XlUtilBkg.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlUtilBkg.c	5.2\t2/16/96";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <X11/Shell.h>
#include <Mrm/MrmAppl.h>                /* Motif Toolkit and MRM */
#include <Xm/MainW.h>
#include <Xm/BulletinB.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xl/Xl.h>
#include <Xl/XlUtilBkg.h>
#include <Xl/XlDrawingUtil.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

static Boolean setta_gc();
static Boolean IntersezioneRettangoli();




/*
 IntersezioneRettangoli === XdIntersectRectangle
 ritorna True se due rettangoli (di cui si passano posizione e ampiezze)
 si intersecano
*/
Boolean IntersezioneRettangoli(int x1,int y1,int w1,int h1,
                           int x2,int y2,int w2,int h2)
{
/*
 coordinate dei centri dei due rettangoli
*/
int c1x,c2x; /* ascisse dei due centri */
int c1y,c2y; /* ordinate dei due centri */
/*
 calcolo coordinate centri
*/
c1x=x1+w1/2;
c2x=x2+w2/2;
c1y=y1+h1/2;
c2y=y2+h2/2;
/*
 I due rettangoli si intersecano se si intersecano le loro
 proiezioni sugli assi delle ascisse e delle coordinate.
 Le proiezioni si intersecano se la distanza tra i centri
 e' inferiore alla somma della meta' delle due ampiezze.
*/
if(((c2x-c1x)*(c2x-c1x)<=(w1/2+w2/2)*(w1/2+w2/2))&&
        ((c2y-c1y)*(c2y-c1y)<=(h1/2+h2/2)*(h1/2+h2/2)))
        return(True);
else
        return(False);
}




Boolean leggi_file_bck(Widget w, char * _nome_file, char *ext,BCK_OBJ **bck_obj,		int *dimensioni)
{
FILE *fp;
char *icon_dir;
char path_icon[300];
char riga[100];
char nome_file[FILENAME_MAX];
char colore[100];
int x1,y1,x2,y2,sa,fa,np;
int topx,topy;
int wi_c,st_c,fill;
char fg_c[14], bg_c[14], fg_f[14];
int tipo;
int i;
BCK_OBJ *b=NULL;
int num_b=0;
int num_obj=0;
int min_x;
int min_y;
int max_x;
int max_y;
int width,height;
Boolean tipo_icona=False;
int border[4]; /* top.bottom,left,right */
/*
	tabella dei colori di cambio colore per le icone animate
*/
static char* tabella_colori[4]= { "#dddddddddddd", "#bbbbbbbbbbbb",
                                  "#999999999999", "#777777777777"};


/*
	inizializzo il valore di ritorno
*/
*bck_obj=b;

/*
	verifico se si tratta di una icona
*/
if(strcmp(ext,"ico")==0)
	{
	tipo_icona=True;
	/*
		carico i bordi
	*/
	border[0] = dimensioni[0]; /* top */
	border[1] = dimensioni[1]; /* bottom */
	border[2] = dimensioni[2]; /* left */
	border[3] = dimensioni[3]; /* right */
	}

/*
	aggiungo l'estensione
*/
sprintf(nome_file,"%s.%s",_nome_file,ext);

icon_dir=getenv("LEGOMMI_ICO");
if(icon_dir==NULL)
        return(False);
path_icon[0]=0;
strcpy(path_icon,icon_dir);
strcat(path_icon,"/");
strcat(path_icon,nome_file);
if((fp=fopen(path_icon,"r"))==NULL)
	{
	if((fp=fopen(nome_file,"r"))==NULL)
		return(False);
	}

/*
	legge le coordinate massime e minime 
*/
fscanf(fp,"%s %d",riga,&min_x);
fscanf(fp,"%s %d",riga,&min_y);
fscanf(fp,"%s %d",riga,&max_x);
fscanf(fp,"%s %d",riga,&max_y);

dimensioni[0]=min_x;
dimensioni[1]=min_y;
dimensioni[2]=max_x;
dimensioni[3]=max_y;

/*
	legge il numero degli oggetti
*/
fscanf(fp,"%s %d",riga,&num_obj);
b=(BCK_OBJ*)XtCalloc((num_obj+1),sizeof(BCK_OBJ));
if(strcmp(riga,"num_d"))
	fprintf(stderr,
	   "lettura_bkg: la prima riga deve contenere il numero degli oggetti");


width=max_x-min_x;
height=max_y-min_y;

/***
printf("\nmin_x=%d min_y=%d max_x=%d max_y=%d\n",
		min_x,min_y,max_x,max_y);
*****/

if(!tipo_icona)
	{
	min_x=0;
	min_y=0;
	}
else
	{
	/*
		modifico i valori di min_x e min_y 
		in funzione dei bordi
	*/
	min_x -= border[2];
	min_y -= border[0];
	}

num_b=0;
while(num_b<num_obj)
	{
	fscanf(fp,"%s %d",riga,&wi_c);
/*
 Controlla se era una riga di endgroup:
 in tal caso legge la riga successiva
*/
/* carlo 05-12-97 
	if(riga[0] == 'e')
*/
        while( riga[0] == 'e' )
		fscanf(fp,"%s %d",riga,&wi_c);

	b[num_b].wi_c=wi_c;

	fscanf(fp,"%s %d",riga,&st_c);
	b[num_b].st_c=st_c;

        fscanf(fp,"%s %s",riga,fg_c);
	if(fg_c[0]!='@')
		{
		XlCvtStrToPixel(XtDisplay(w),fg_c,&(b[num_b].fg_c),&(b[num_b].fg_c_red),
				&(b[num_b].fg_c_green),&(b[num_b].fg_c_blue));
		b[num_b].fg_c_cc= -999;
		}
	else
		{
		b[num_b].fg_c_cc= -(atoi)(&fg_c[1]);
		XlCvtStrToPixel(XtDisplay(w),tabella_colori[b[num_b].fg_c_cc*(-1)-1],
			&(b[num_b].fg_c),&(b[num_b].fg_c_red),
			&(b[num_b].fg_c_green),&(b[num_b].fg_c_blue));
		}

        fscanf(fp,"%s %s",riga,bg_c);
	if(bg_c[0]!='@')
                {
                XlCvtStrToPixel(XtDisplay(w),bg_c,&(b[num_b].bg_c),
			&(b[num_b].bg_c_red),&(b[num_b].bg_c_green),&(b[num_b].bg_c_blue));
                b[num_b].bg_c_cc= -999;
                }
        else
                {
                b[num_b].bg_c_cc= -(atoi)(&bg_c[1]);
		XlCvtStrToPixel(XtDisplay(w),tabella_colori[b[num_b].bg_c_cc*(-1)-1],
			&(b[num_b].bg_c),
			&(b[num_b].bg_c_red),&(b[num_b].bg_c_green),&(b[num_b].bg_c_blue));
                }

        fscanf(fp,"%s %s",riga,fg_f);
	if(fg_f[0]!='@')
                {
                XlCvtStrToPixel(XtDisplay(w),fg_f,&(b[num_b].fg_f),
			&(b[num_b].fg_f_red),&(b[num_b].fg_f_green),&(b[num_b].fg_f_blue));
                b[num_b].fg_f_cc= -999;
                }
        else
                {
                b[num_b].fg_f_cc= -(atoi)(&fg_f[1]);
		XlCvtStrToPixel(XtDisplay(w),tabella_colori[b[num_b].fg_f_cc*(-1)-1],
			&(b[num_b].fg_f),
			&(b[num_b].fg_f_red),&(b[num_b].fg_f_green),&(b[num_b].fg_f_blue));
                }

	fscanf(fp,"%s %d\n",riga,&fill);
	b[num_b].fill=fill;

	fscanf(fp,"%s",riga);
	switch(riga[0])
		{
		case 'l':
			{
			tipo=TIPO_LINEA;
			fscanf(fp,"%d %d %d %d",
				&(b[num_b].dati[0]), &(b[num_b].dati[1]),
				&(b[num_b].dati[2]), &(b[num_b].dati[3]));
			b[num_b].dati[0] -= min_x;
			b[num_b].dati[1] -= min_y;
			b[num_b].dati[2] -= min_x;
			b[num_b].dati[3] -= min_y;
			break;
			}
		case 'c':
			{
			tipo=TIPO_CERCHIO;
			fscanf(fp,"%d %d %d %d %d %d",
				&x1,&y1,&x2,&y2,
				&(b[num_b].dati[4]), &(b[num_b].dati[5]));
        		b[num_b].dati[0]= (x1 < x2) ? x1 : x2;
        		b[num_b].dati[1]= (y1 < y2) ? y1 : y2;
			b[num_b].dati[2]=abs(x1-x2);
			b[num_b].dati[3]=abs(y1-y2);
			b[num_b].dati[0] -= min_x;
			b[num_b].dati[1] -= min_y;
			break;
			}
		case 'r':
			{
			tipo=TIPO_RETTANGOLO;
			fscanf(fp,"%d %d %d %d",&x1,&y1,&x2,&y2);
                        b[num_b].dati[0]= (x1 < x2) ? x1 : x2;
                        b[num_b].dati[1]= (y1 < y2) ? y1 : y2;
                        b[num_b].dati[2]=abs(x1-x2);
                        b[num_b].dati[3]=abs(y1-y2);
			b[num_b].dati[0] -= min_x;
			b[num_b].dati[1] -= min_y;
			break;
			}
		case 'p':
			{
			tipo=TIPO_POLIGONO;
			fscanf(fp,"%d",&np);
			b[num_b].dati[0]=np;
			(b[num_b].points)=
				(XPoint *)XtCalloc(np,sizeof(XPoint));
			for(i=0;i<np;i++)
				{
				fscanf(fp,"%d %d",&x1,&y1);
				b[num_b].points[i].x=(short)x1;
				b[num_b].points[i].y=(short)y1;
				b[num_b].points[i].x -= min_x;
				b[num_b].points[i].y -= min_y;
				}
			break;
			}
		case 'C':
			{
			tipo=TIPO_CONNESSIONE;
			fscanf(fp,"%d",&np);
			b[num_b].dati[0]=np;
			(b[num_b].points)=
				(XPoint *)XtCalloc(np,sizeof(XPoint));
			for(i=0;i<np;i++)
				{
				fscanf(fp,"%d %d",&x1,&y1);
				b[num_b].points[i].x=(short)x1;
				b[num_b].points[i].y=(short)y1;
				b[num_b].points[i].x -= min_x;
				b[num_b].points[i].y -= min_y;
				}
			break;
			}
		case 'g': /* lettura del gruppo */
			{
			fscanf(fp,"%d %d %d %d %d",&np,&x1,&y1,&x2,&y2);
			continue;
			}
		default:
			{
			tipo= -1;
			printf(stderr,"tipo sconosciuto\n");
			return(-1);
			}
		}
	b[num_b].tipo=tipo;
	num_b++;
	}
/*
printf("setto a -1 il tipo dell'oggetto di indice %d della pagina %s\n",num_obj,_nome_file);
*/
b[num_obj].tipo= -1;
*bck_obj=b;
fclose(fp);
/****************
XSync(XtDisplay(w),False);
****************************/
return(1);
}

Boolean disegna_bck(Widget w,BCK_OBJ *b,XmDrawingAreaCallbackStruct *str,
		Region *region,Boolean modoPrint)
{
int num_b=0;
int i;
static GC gc=NULL;
static GC gc_fill=NULL;
static Display *LocalDisplay=NULL;
XGCValues values;
unsigned long valuemask=GCSubwindowMode;
int str_count;
XRectangle rect;
/*
	se la lista e' vuota ritorna senza errore
   printf("LISTA BKG VUOTA\n");
*/
if(b==NULL)
	return(True);

/* se non c'e' nessun disegno ritorno subito
   printf("BKG SENZA DISEGNI\n");
*/
   if(b[0].tipo == -1 )
      return(True);

/*
        per la modalita' stampante annullo i vecchi GC
*/
if(modoPrint)
        {
        if(gc!=NULL)
                XFreeGC(XtDisplay(w),gc);
        if(gc_fill!=NULL)
                XFreeGC(XtDisplay(w),gc_fill);
        gc=NULL;
        gc_fill=NULL;
        LocalDisplay=NULL;
        }

/*
	registra il display la prima volta
*/
if(LocalDisplay==NULL)
	LocalDisplay=XtDisplay(w);

/*
	verifica il cambiamento del display
*/
if(LocalDisplay!=XtDisplay(w))
	{
/*
printf("\n\nLocalDisplay =%s XtDisplay(w)=%s\n",DisplayString(LocalDisplay),DisplayString(XtDisplay(w)));
*/
	XFreeGC(LocalDisplay,gc);
	XFreeGC(LocalDisplay,gc_fill);
	LocalDisplay=XtDisplay(w);
	gc=NULL;
	gc_fill=NULL;
	}

/*
	se e' necessario creo i gc
*/
if(gc==NULL)
	{
	values.subwindow_mode= IncludeInferiors; 
	gc=XCreateGC(XlDisplay(w),XlWindow(w), valuemask,&values);
	gc_fill=XCreateGC(XlDisplay(w),XlWindow(w), valuemask,&values);
	}

/*
printf("DISEGNA background %d %d %d %d count=%d\n",
	str->event->xexpose.x,str->event->xexpose.y,
	str->event->xexpose.width,str->event->xexpose.height,
		str->event->xexpose.count);
*/
/*
	crea o aggiorna la regione di ridisegno
*/
if(!modoPrint)
        {
	if((*region)==NULL)
		(*region)=XCreateRegion();

	XtAddExposureToRegion(str->event,(*region));
	}
if(modoPrint)
	str_count = 0;
else
	str_count = str->event->xexpose.count;

if(str_count== 0)
   {
   if(!modoPrint)
	{
   	XSetRegion(XtDisplay(w),gc,(*region));
   	XSetRegion(XtDisplay(w),gc_fill,(*region));
	XClipBox(*region,&rect);
	}
   num_b=0;
   while(b[num_b].tipo>0)
      {
   /*
      printf("Oggetto %d -> %d wi_c=%d st_c=%d fill=%d\n",
	   num_b,b[num_b].tipo,b[num_b].wi_c,b[num_b].st_c,b[num_b].fill);
   */
      if(num_b == 0)
      	setta_gc(w,gc,gc_fill,b[num_b],1);
      else
      	setta_gc(w,gc,gc_fill,b[num_b],0);



      switch(b[num_b].tipo)
	   {
	   case TIPO_LINEA:
		   {
   /*
		   printf("linea %d %d %d %d\n", b[num_b].dati[0],
		   b[num_b].dati[1],b[num_b].dati[2],b[num_b].dati[3]);
   */
		   XDrawLine(XlDisplay(w),XlWindow(w),gc,
			   b[num_b].dati[0],b[num_b].dati[1],
			   b[num_b].dati[2],b[num_b].dati[3]);
		   break;
		   }
	   case TIPO_RETTANGOLO:
		   {
   /*
		   printf("%d %d %d %d\n", b[num_b].dati[0],b[num_b].dati[1],
		   b[num_b].dati[2],b[num_b].dati[3]);
   */
		   if(b[num_b].fill==1)
		      XFillRectangle(XlDisplay(w),XlWindow(w),gc_fill,
		           b[num_b].dati[0],b[num_b].dati[1],
                           b[num_b].dati[2],b[num_b].dati[3]);
		   XDrawRectangle(XlDisplay(w),XlWindow(w),gc,
		           b[num_b].dati[0],b[num_b].dati[1],
                           b[num_b].dati[2],b[num_b].dati[3]);
		   break;
		   }
	   case TIPO_CERCHIO:
		   {
   /*
		   printf("%d %d %d %d %d %d\n",
			   b[num_b].dati[0],b[num_b].dati[1],
			   b[num_b].dati[2],b[num_b].dati[3],
			   b[num_b].dati[4],b[num_b].dati[5]);
   */
                   if(b[num_b].fill==1)
		      XFillArc(XlDisplay(w),XlWindow(w),gc_fill,
			   b[num_b].dati[0],b[num_b].dati[1],
                           b[num_b].dati[2],b[num_b].dati[3],
                           b[num_b].dati[4],b[num_b].dati[5]);
		   XDrawArc(XlDisplay(w),XlWindow(w),gc,
			   b[num_b].dati[0],b[num_b].dati[1],
                           b[num_b].dati[2],b[num_b].dati[3],
                           b[num_b].dati[4],b[num_b].dati[5]);
		   break;
		   }
	   case TIPO_POLIGONO:
		   {
   /*
		   for(i=0;i<b[num_b].dati[0];i++)
		   printf("%d %d\n",b[num_b].dati[i*2+1],b[num_b].dati[i*2+2]);
   */
                   if(b[num_b].fill==1)
		       XFillPolygon(XlDisplay(w),XlWindow(w),gc_fill,
			    b[num_b].points,b[num_b].dati[0],
			    Complex,CoordModeOrigin);
		   XDrawLines(XlDisplay(w),XlWindow(w),gc,
			   b[num_b].points,b[num_b].dati[0],CoordModeOrigin);
		   break;
		   }
	   case TIPO_CONNESSIONE:
		   {
   /*
		   for(i=0;i<b[num_b].dati[0];i++)
		   printf("%d %d\n",b[num_b].dati[i*2+1],b[num_b].dati[i*2+2]);
   */
                   if(b[num_b].fill==1)
		       XFillPolygon(XlDisplay(w),XlWindow(w),gc_fill,
			    b[num_b].points,b[num_b].dati[0],
			    Complex,CoordModeOrigin);
		   XDrawLines(XlDisplay(w),XlWindow(w),gc,
			   b[num_b].points,b[num_b].dati[0],CoordModeOrigin);
		   break;
		   }
	   }
     ++num_b;
     }
   if(!modoPrint)
	{
	Cardinal lnargs;
	Arg args[5];
	WidgetList children;
	int i;
	int rect_pos;
	Cardinal  num_children=0;
	Position x,y;
	Dimension width,height;
/*
 esamina se qualche oggetto Xl si trova nella regione di ridisegno
*/
   if(!XtIsComposite(w))
        {
        children = NULL;
        num_children = 0;
	}
    else
	{
        lnargs=0;
        XtSetArg(args[lnargs],XmNnumChildren,&num_children);lnargs++;
        XtSetArg(args[lnargs],XmNchildren,&children);lnargs++;
        XtGetValues(w,args,lnargs);
	}
for(i=0;i<num_children;i++)
   	{
/*
 Controllo su manged necessario per la presenza nel caso di richiamo
 di Plot o Aing di campi testo non managed utilizzati per l'attivazione
 del VariableEditor
*/
	if(!XtIsManaged(children[i]))
		continue;
        lnargs = 0;
   	XtSetArg(args[lnargs],XmNx,&x);lnargs++;
   	XtSetArg(args[lnargs],XmNy,&y);lnargs++;
   	XtSetArg(args[lnargs],XmNwidth,&width);lnargs++;
   	XtSetArg(args[lnargs],XmNheight,&height);lnargs++;
   	XtGetValues(children[i],args,lnargs);
   	if(IntersezioneRettangoli(x,y,width,height,
                rect.x,rect.y,rect.width,rect.height))
/*
 forza il ridisegno dell'oggetto
*/
		{
        	XlRedisplay(children[i]);
		}
	}

   	XDestroyRegion((*region));
   	(*region)=NULL;
	}
   }
if(modoPrint)
        {
        if(gc!=NULL)
                XFreeGC(XlDisplay(w),gc);
        if(gc_fill!=NULL)
                XFreeGC(XlDisplay(w),gc_fill);
        gc=NULL;
        gc_fill=NULL;
        LocalDisplay=NULL;
        }
}
   
Boolean libera_lista_bck(BCK_OBJ *bck_obj)
{
int num_b=0;
int i;

if(bck_obj == NULL)
	return(True);
num_b=0;
while(bck_obj[num_b].tipo>0)
	{
	if((bck_obj[num_b].tipo==TIPO_POLIGONO)||
		(bck_obj[num_b].tipo==TIPO_CONNESSIONE))
			XtFree(bck_obj[num_b].points);
	++num_b;
	}
XtFree(bck_obj);
return (1);
}

static Boolean setta_gc(Widget w, GC gc, GC gc_fill,BCK_OBJ obj,int clear_gc)
{
static unsigned int line_width = -1;
static int line_style = -1;
static Pixel fg_c = -1;
static Pixel bg_c = -1;
static Pixel fg_f = -1;
XGCValues values;
unsigned long valuemask=0;

if(clear_gc == 1)
	{
	line_width = -1;
	line_style = -1;
	fg_c = -1;
	bg_c = -1;
	fg_f = -1;
	}


/**
printf("setta_gc %d %d\n",line_width,line_style);
**/

if(line_width!=obj.wi_c)
	{
	valuemask = valuemask | GCLineWidth;
	line_width=obj.wi_c;
	values.line_width = line_width;
	}

if(line_style!=obj.st_c)
	{
	valuemask = valuemask | GCLineStyle;
	line_style=obj.st_c;
	values.line_style = line_style;
	}

if(fg_c!=obj.fg_c)
	{
	valuemask = valuemask | GCForeground;
	fg_c=XlPrintGetPixel(w,obj.fg_c);
	values.foreground = fg_c;
	}

if(bg_c!=obj.bg_c)
	{
	valuemask = valuemask | GCBackground;
	bg_c=XlPrintGetPixel(w,obj.bg_c);
	values.background = bg_c;
	}

if(valuemask!=0)
	XChangeGC(XlDisplay(w),gc,valuemask,&values);

if(fg_f!=obj.fg_f)
	{
	valuemask = GCForeground;
	fg_f=XlPrintGetPixel(w,obj.fg_f);
	values.foreground = fg_f;
	XChangeGC(XlDisplay(w),gc_fill,valuemask,&values);
	}
}


Boolean disegna_bck_icon(Widget w,BCK_OBJ *b,
		Pixel c1,Pixel c2,Pixel c3,Pixel c4,Boolean modoPrint)
{
int num_b=0;
int zoom;
int i;
static GC gc=NULL;
static GC gc_fill=NULL;
static Display *LocalDisplay=NULL;
XGCValues values;
unsigned long valuemask=0;
unsigned short red,green,blue;
XPoint *app_point;
Position offX,offY;
/*
	tabella dei colori di cambio colore per le icone animate
*/
static char* tabella_colori[4]= { "#dddddddddddd", "#bbbbbbbbbbbb",
                                  "#999999999999", "#777777777777"};

if(c1== -1)
	XlCvtStrToPixel(XtDisplay(w),tabella_colori[0],&c1,&red,&green,&blue);
if(c2== -1)
	XlCvtStrToPixel(XtDisplay(w),tabella_colori[1],&c2,&red,&green,&blue);
if(c3== -1)
	XlCvtStrToPixel(XtDisplay(w),tabella_colori[2],&c3,&red,&green,&blue);
if(c4== -1)
	XlCvtStrToPixel(XtDisplay(w),tabella_colori[3],&c4,&red,&green,&blue);


/*
	se la lista e' vuota ritorna senza errore
*/
if(b==NULL)
	return(True);


/*
	per la modalita' stampante annullo i vecchi GC
*/
if(modoPrint)
	{
	if(gc!=NULL)
        	XFreeGC(XtDisplay(w),gc);
	if(gc_fill!=NULL)
        	XFreeGC(XtDisplay(w),gc_fill);
        gc=NULL;
        gc_fill=NULL;
	LocalDisplay=NULL;
	offX=XlPrintOffsetX(w);
	offY=XlPrintOffsetY(w);
	}
else
	{
	offX=0;
	offY=0;
	}
/*
        registra il display la prima volta
*/
if(LocalDisplay==NULL)
        LocalDisplay=XlDisplay(w);

/*
        verifica il cambiamento del display
*/
if(LocalDisplay!=XlDisplay(w))
        {
        XFreeGC(LocalDisplay,gc);
        XFreeGC(LocalDisplay,gc_fill);
        LocalDisplay=XlDisplay(w);
        gc=NULL;
        gc_fill=NULL;
        }
/*
        se e' necessario creo i gc
*/
if(gc==NULL)
        {
        gc=XCreateGC(XlDisplay(w),XlWindow(w), valuemask,&values);
        gc_fill=XCreateGC(XlDisplay(w),XlWindow(w), valuemask,&values);
        }

/**********
printf("DISEGNA background icona %d %d %d %d\n",c1,c2,c3,c4);
**********/
/*
	crea o aggiorna la regione di ridisegno
*/

   num_b=0;
   if(XlIsXlComposite(XtParent(w)))
   	zoom=XlGetZoom(XtParent(w));
   else
	zoom=XlGetZoom(w);

   while(b[num_b].tipo>0)
      {
/**
      printf("Oggetto %d -> %d wi_c=%d st_c=%d fill=%d\n",
	   num_b,b[num_b].tipo,b[num_b].wi_c,b[num_b].st_c,b[num_b].fill);
**/

/********
printf("c1 =%d c2 = %d c3 =%d c4 = %d\n",c1,c2,c3,c4);
printf("b[%d].fg_c_cc = %d\n",num_b,b[num_b].fg_c_cc);

      if(b[num_b].fg_c_cc == -1) 
      {
        if( c1 == -2 ) 
        {
           ++num_b;
           continue;
        }
        else 
           b[num_b].fg_c=c1;
     }
      

      if(b[num_b].fg_c_cc== -2) 
      {
           if( c2 == -2 ) 
           {
              ++num_b;
              continue;
           }
           else
             b[num_b].fg_c=c2;
      }

      if(b[num_b].fg_c_cc== -3) 
      {
           if( c3 == -2 ) 
           {
              ++num_b;
              continue;
           }
           else
             b[num_b].fg_c=c3;
      }

      if(b[num_b].fg_c_cc== -4) 
      {
           if( c4 == -2 ) 
           {
              ++num_b;
              continue;
           }
           else
              b[num_b].fg_c=c4;
      }
****/
      if(b[num_b].fg_c_cc== -1) b[num_b].fg_c=c1;
      if(b[num_b].fg_c_cc== -2) b[num_b].fg_c=c2;
      if(b[num_b].fg_c_cc== -3) b[num_b].fg_c=c3;
      if(b[num_b].fg_c_cc== -4) b[num_b].fg_c=c4;


      if(b[num_b].bg_c_cc== -1) b[num_b].bg_c=c1;
      if(b[num_b].bg_c_cc== -2) b[num_b].bg_c=c2;
      if(b[num_b].bg_c_cc== -3) b[num_b].bg_c=c3;
      if(b[num_b].bg_c_cc== -4) b[num_b].bg_c=c4;

      if(b[num_b].fg_f_cc== -1) b[num_b].fg_f=c1;
      if(b[num_b].fg_f_cc== -2) b[num_b].fg_f=c2;
      if(b[num_b].fg_f_cc== -3) b[num_b].fg_f=c3;
      if(b[num_b].fg_f_cc== -4) b[num_b].fg_f=c4;

      if(num_b == 0)
      	setta_gc(w,gc,gc_fill,b[num_b],1);
      else
      	setta_gc(w,gc,gc_fill,b[num_b],0);


      switch(b[num_b].tipo)
	   {
	   case TIPO_LINEA:
		   {
   /*
		   printf("linea %d %d %d %d\n", b[num_b].dati[0],
		   b[num_b].dati[1],b[num_b].dati[2],b[num_b].dati[3]);
   */
		   XDrawLine(XlDisplay(w),XlWindow(w),gc,
			   (b[num_b].dati[0]*zoom)/100+offX,
			   (b[num_b].dati[1]*zoom)/100+offY,
			   (b[num_b].dati[2]*zoom)/100+offX,
			   (b[num_b].dati[3]*zoom)/100+offY);
		   break;
		   }
	   case TIPO_RETTANGOLO:
		   {
   /*
		   printf("%d %d %d %d\n", b[num_b].dati[0],b[num_b].dati[1],
		   b[num_b].dati[2],b[num_b].dati[3]);
   */
		   if(b[num_b].fill==1)
		      XFillRectangle(XlDisplay(w),XlWindow(w),gc_fill,
		           (b[num_b].dati[0]*zoom)/100+offX,
			   (b[num_b].dati[1]*zoom)/100+offY,
                           (b[num_b].dati[2]*zoom)/100,
		           (b[num_b].dati[3]*zoom)/100);
		   XDrawRectangle(XlDisplay(w),XlWindow(w),gc,
		           (b[num_b].dati[0]*zoom)/100+offX,
			   (b[num_b].dati[1]*zoom)/100+offY,
                           (b[num_b].dati[2]*zoom)/100,
			   (b[num_b].dati[3]*zoom)/100);
		   break;
		   }
	   case TIPO_CERCHIO:
		   {
   /*
		   printf("%d %d %d %d %d %d\n",
			   b[num_b].dati[0],b[num_b].dati[1],
			   b[num_b].dati[2],b[num_b].dati[3],
			   b[num_b].dati[4],b[num_b].dati[5]);
   */
                   if(b[num_b].fill==1)
		      XFillArc(XlDisplay(w),XlWindow(w),gc_fill,
			   (b[num_b].dati[0]*zoom)/100+offX,
		      	   (b[num_b].dati[1]*zoom)/100+offY,
                           (b[num_b].dati[2]*zoom)/100,
			   (b[num_b].dati[3]*zoom)/100,
                           b[num_b].dati[4],b[num_b].dati[5]);
		   XDrawArc(XlDisplay(w),XlWindow(w),gc,
			   (b[num_b].dati[0]*zoom)/100+offX,
			   (b[num_b].dati[1]*zoom)/100+offY,
                           (b[num_b].dati[2]*zoom)/100,
			   (b[num_b].dati[3]*zoom)/100,
                           b[num_b].dati[4], b[num_b].dati[5]);
		   break;
		   }
	   case TIPO_POLIGONO:
		   {
   /*
		   for(i=0;i<b[num_b].dati[0];i++)
		   printf("%d %d\n",b[num_b].dati[i*2+1],b[num_b].dati[i*2+2]);
   */
		   app_point=(XPoint*)XtMalloc
				(b[num_b].dati[0]*sizeof(XPoint));
		   for(i=0;i<b[num_b].dati[0];i++)
			{
			app_point[i].x=(b[num_b].points[i].x*zoom)/100+offX;
			app_point[i].y=(b[num_b].points[i].y*zoom)/100+offY;
			}
                   if(b[num_b].fill==1)
		       XFillPolygon(XlDisplay(w),XlWindow(w),gc_fill,
			    app_point,b[num_b].dati[0],
			    Complex,CoordModeOrigin);
		   XDrawLines(XlDisplay(w),XlWindow(w),gc,
			   app_point,b[num_b].dati[0],CoordModeOrigin);
		   XtFree(app_point);
		   break;
		   }
	   }
     ++num_b;
     }

if(modoPrint)
	{
	if(gc!=NULL)
        	XFreeGC(XlDisplay(w),gc);
	if(gc_fill!=NULL)
        	XFreeGC(XlDisplay(w),gc_fill);
        gc=NULL;
        gc_fill=NULL;
	LocalDisplay=NULL;
	}
}
   

Boolean ruota_bck(int *width, int *height,BCK_OBJ *b,
		int rotate, int ass_rotate,
		int top, int bottom, int left, int right)
{
int num_b=0;
int app_int1,app_int2,np,i;




/*
        se la lista e' vuota ritorna senza errore
*/
if(b==NULL)
        return(True);

if((rotate==ROTATE_90)||(rotate==ROTATE_270))
	{
	app_int1= *width;
	*width = *height;
	*height = app_int1;
	}

while(b[num_b].tipo>0)
      {
      switch(b[num_b].tipo)
           {
           case TIPO_LINEA:
                   {
		   gest_trasf_coor_ico(&(b[num_b].dati[0]),&(b[num_b].dati[1]),
			*width,*height,rotate,ass_rotate,
			top,bottom,left,right);
		   gest_trasf_coor_ico(&(b[num_b].dati[2]),&(b[num_b].dati[3]),
			*width,*height,rotate,ass_rotate,
			top,bottom,left,right);
                   break;
                   }
           case TIPO_RETTANGOLO:
                   {
		   gest_trasf_wh_ico(&(b[num_b].dati[0]),&(b[num_b].dati[1]),
			&(b[num_b].dati[2]),&(b[num_b].dati[3]),
			*width,*height,rotate,ass_rotate,
			top,bottom,left,right);
                   break;
                   }
           case TIPO_CERCHIO:
                   {
		   gest_trasf_wh_ico(&(b[num_b].dati[0]),&(b[num_b].dati[1]),
			&(b[num_b].dati[2]),&(b[num_b].dati[3]),
			*width,*height,rotate,ass_rotate,
			top,bottom,left,right);
		   gest_trasf_arc_ico(&(b[num_b].dati[4]),&(b[num_b].dati[5]),
				rotate,ass_rotate);
                   break;
                   }
           case TIPO_POLIGONO:
                   {
		   np=b[num_b].dati[0];
		   for(i=0;i<np;i++)
                      {
	              app_int1=(int)b[num_b].points[i].x;	
	              app_int2=(int)b[num_b].points[i].y;	
		      gest_trasf_coor_ico(&app_int1,&app_int2,
			*width,*height,rotate,ass_rotate,
			top,bottom,left,right);
		      b[num_b].points[i].x=(short)app_int1;
		      b[num_b].points[i].y=(short)app_int2;
		      } 
                   break;
                   }
           }
     ++num_b;
     }
}

/*
	verifica se lo sfondo contiene la variabile cambio colore numC
*/
Boolean XlEsisteCambioColore(BCK_OBJ *b, int numC)
{
int num_b=0;
int controllo = numC*(-1);

if(b==NULL)
        return(False);

while(b[num_b].tipo>0)
      	{
	if(b[num_b].fg_c_cc== controllo) return(True);
	if(b[num_b].bg_c_cc== controllo) return(True);
	if(b[num_b].fg_f_cc== controllo) return(True);
	++num_b;
      	}
return(False);
}
