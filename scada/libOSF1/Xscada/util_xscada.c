/**********************************************************************
*
*       C Source:               util_xscada.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Dec 12 17:03:21 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: util_xscada.c-3.2.1 %  (%full_filespec: util_xscada.c-3.2.1:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#ifdef XSCADA_INTERFACE
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>

#include "tag.h"
#include "deflag.inc"
#include "g2comdb.inc"
#include "comunic.inc"

#include "mesqueue.h"
#include "messcada.inc"
#include "util_xscada.h"


static void SettaDescr(Widget, short, short);
static void Maiuscolo(char *);

int ind_mandb_1 = -1;
int ind_mandb_2 = -1;
int ind_mandb_3 = -1;
int ind_mandb_4 = -1;
int ind_mandb_5 = -1;

int ext_mandb_1 = -1;
int ext_mandb_2 = -1;
int ext_mandb_3 = -1;
int ext_mandb_4 = -1;
int ext_mandb_5 = -1;

CreateFinMandb(Widget labelinfo)
{
ind_mandb_1 = -1;
ind_mandb_2 = -1;
ind_mandb_3 = -1;
ind_mandb_4 = -1;
ind_mandb_5 = -1;
}

int GetSlotByLabel(Widget cw)
{
int ret;
char nome[20];

strcpy(nome,XtName(cw));
ret = atoi(&nome[9]);

return(ret);
}

Widget GetToggleButton(Widget cw,short i)
{
char nome[100];
char nomeChild[100];
char nomeParent[100];
Widget widChild,widParent;

/* Compone il nome del padre  e recupera il Widget */
strcpy(nome,XtName(cw));
strcpy(nomeParent, "rowColumn");
strcat(nomeParent,(nome+9));
widParent=XtNameToWidget(XtParent(cw),nomeParent);

/* Se non trova parent stampa segnalazione di errore ed esce */
if(!widParent)
{
	printf("util_xscada.c [GetToggleButton] widParent NULL !!!\n");
	return NULL;
}

/* Compone il nome del figlio  e recupera il Widget */
strcpy(nomeChild,"toggleButtonf");
sprintf(nomeChild+strlen(nomeChild),"%02d",i);
strcat(nomeChild,"_");
strcat(nomeChild,(nome+9));
widChild=XtNameToWidget(widParent,nomeChild);

/* Se non trova figlio stampa segnalazione di errore ed esce */
if(!widChild)
{
	printf("util_xscada.c [GetToggleButton] widChild NULL !!!\n");
        return NULL;
}

return widChild;
}

void InitFlags(short ext,Widget labelinfo)
{
int i,j;
Widget widChild;
XmString string;
Arg arg[1];

/* Scandisce tutti i bottoni dei flag */
for(i=1,j=0;i<=nToggleButton;j++,i++)
{
	widChild=GetToggleButton(labelinfo,i);

	/* Se non trova figlio stampa segnalazione di errore ed esce */
	if(!widChild)
	{
		printf("util_xscada.c [InitFlags] widChild NULL !!!\n");
		return;
	}

	/* Seleziona la stringa  da scrivere in base al tipo di punto */
	switch(ext)
	{
		case g1tipaa:   /* analogici acquisiti                 */
		case g1tipas:   /* analogici calcolati standard        */
		case g1tipac:   /* analogici calcolati non standard    */
		case g1tipad:   /* analogici di diagnostica            */
			string=XmStringCreateSimple(xfan[j].pFlag);
			break;
		case g1tipao:   /* analogici di uscita                 */
			string=XmStringCreateSimple(xfao[j].pFlag);
			break;

		case g1tipda:   /* digitali  acquisiti                 */
		case g1tipds:   /* digitali  calcolati standard        */
		case g1tipdc:   /* digitali  calcolati non standard    */
		case g1tipdd:   /* digitali  di diagnostica            */
			string=XmStringCreateSimple(xfdi[j].pFlag);
			break;

		case g1tipor:   /* organi                              */
			string=XmStringCreateSimple(xfor[j].pFlag);
			break;

		case g1tipdo:   /* digitali  di uscita                 */
		case g1tipby:   /* aggregato byte                      */
		case g1tipst:   /* stringhe                            */
			string=XmStringCreateSimple("  ");
			break;
	}

	/* Aggiorna la scritta "sul" bottone */	
	XtSetArg(arg[0],XmNlabelString,string);
	XtSetValues(widChild,arg,1);
	XmStringFree(string);
}
}

SetVar(Widget labelinfo)
{
char nome[100];
char nomeLabel[100];
Widget wid;
short point,ext,posiz;
long blocco;
char nome_ricerca[200];
char *app;

strcpy(nome,"textField");
strcpy(nomeLabel,XtName(labelinfo));
strcat(nome,&nomeLabel[9]);
wid = XtNameToWidget(XtParent(labelinfo),nome);
memset(nome_ricerca,0,200);
strcpy(nome_ricerca,XmTextFieldGetString(wid));
/*
	elimina gli eventuali spazi
*/
if((app=strstr(nome_ricerca," "))!=NULL)
	memset(app,0,30);
Maiuscolo(nome_ricerca);
XmTextFieldSetString(wid,nome_ricerca);

/* Scrive le stringhe "sui" bottoni dei flag in base al tipo di punto */
if(!ricerca((short *)nome_ricerca,&point,&ext,&blocco,&posiz,fpp[fnomi]))
	{
	InitFlags(ext,labelinfo);
	SettaDescr(wid,point,ext);
//printf("dopo ricerca [%s]  = %d\n",nome_ricerca,point);
	}

if(strcmp(&nomeLabel[9],"1")==0)
	{
	ind_mandb_1 = point;
	ext_mandb_1 = ext;
	}
if(strcmp(&nomeLabel[9],"2")==0)
	{
	ind_mandb_2 = point;
	ext_mandb_2 = ext;
	}
if(strcmp(&nomeLabel[9],"3")==0)
	{
	ind_mandb_3 = point;
	ext_mandb_3 = ext;
	}
if(strcmp(&nomeLabel[9],"4")==0)
	{
	ind_mandb_4 = point;
	ext_mandb_4 = ext;
	}
if(strcmp(&nomeLabel[9],"5")==0)
	{
	ind_mandb_5 = point;
	ext_mandb_5 = ext;
	}
}

ChangeFlag(Widget labelinfo, Widget Flag)
{
//printf("ChangeFlag labelinfo = [%s] flag = [%s]\n",XtName(labelinfo),XtName(Flag));
}


VisMode(Widget labelinfo)
{
//printf("VisMode [%s]\n",XtName(labelinfo));
}

SetMode(Widget labelinfo)
{
//printf("SetMode [%s]\n",XtName(labelinfo));
}

SetDataBaseFlag(Widget cw,short ext,short point)
{

int i,j;
Widget widChild;

/* Scandisce tutti i bottoni dei flag */
for(i=1,j=0;i<=nToggleButton;i++,j++)
{
        widChild=GetToggleButton(cw,i);

        /* Se non trova figlio stampa segnalazione di errore ed esce */
        if(!widChild)
        {
                printf("util_xscada.c [ToggleButtonStateToFlag] widChild NULL !!!\n");
                return;
        }

	switch (ext) {
	   case (g1tipaa):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			bitset(&dbaaf[point],xfan[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipas):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			bitset(&dbasf[point],xfan[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipac):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			bitset(&dbacf[point],xfan[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipad):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			bitset(&dbadf[point],xfan[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipao):
		{
		if(xfao[j].ind!=-1 && strcmp(xfao[j].pFlag,""))
			bitset(&dbaof[point],xfao[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipda):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			bitset(&dbda[point],xfdi[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipds):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			bitset(&dbds[point],xfdi[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipdc):
			{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			bitset(&dbdc[point],xfdi[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipdd):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			bitset(&dbdd[point],xfdi[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipor):
		{
		if(xfor[j].ind!=-1 && strcmp(xfor[j].pFlag,""))
			bitset(&dbosc[point],xfor[j].ind,XmToggleButtonGetState(widChild));
		break;
		}
	   case (g1tipdo):
	   case (g1tipst):
	   other:
		{
		break;
       		}
	}
}
}

SetApplay(Widget labelinfo)
{
char nome[100];
char nomeLabel[100];
Widget wid_var,wid_val;
char nome_var[100];
char str_val[100];
short lstr;
short point,ext,posiz;
long blocco;
short digVal;
char *app_char;

strcpy(nome,"textFieldSt");
strcpy(nomeLabel,XtName(labelinfo));
strcat(nome,&nomeLabel[9]);
wid_val = XtNameToWidget(XtParent(labelinfo),nome);

strcpy(nome,"textField");
strcpy(nomeLabel,XtName(labelinfo));
strcat(nome,&nomeLabel[9]);
wid_var = XtNameToWidget(XtParent(labelinfo),nome);

memset(nome_var,0,30);
strcpy(nome_var,XmTextGetString(wid_var));
/*
	elimina gli eventuali spazi
*/
if((app_char=strstr(nome_var," "))!=NULL)
	memset(app_char,0,30);

Maiuscolo(nome_var);
strcpy(str_val,XmTextGetString(wid_val));
ricerca((short *)nome_var,&point,&ext,&blocco,&posiz,fpp[fnomi]);
if(ext < 0)
	return;

/* Aggiorna lo stato dei flag nel data base di sistema */
SetDataBaseFlag(labelinfo,ext,point);

/* Aggiorna il valore dei punti nel data base di sistema */
switch (ext) {
   case (g1tipaa):
	{
	dbaav[point] = atof(str_val);
	break;
	}
   case (g1tipas):
	{
	dbasv[point] = atof(str_val);
	break;
	}
   case (g1tipac):
	{
	dbacv[point] = atof(str_val);
	break;
	}
   case (g1tipad):
	{
	dbadv[point] = atof(str_val);
	break;
	}
   case (g1tipao):
	{
	invao(point,atof(str_val));
	break;
	}
   case (g1tipda):
	{
	digVal=atoi(str_val);
	bitset(&dbda[point],g2di_sl,digVal);
	break;
	}
   case (g1tipds):
	{
	digVal=atoi(str_val);
	bitset(&dbds[point],g2di_sl,digVal);
	break;
	}
   case (g1tipdc):
	{
	digVal=atoi(str_val);
	bitset(&dbdc[point],g2di_sl,digVal);
	break;
	}
   case (g1tipdd):
	{
	digVal=atoi(str_val);
	bitset(&dbdd[point],g2di_sl,digVal);
	break;
	}
   case (g1tipdo):
	{
	coman(point);
	break;
	}
   case (g1tipor):
	{
	for(lstr=strlen(str_val);lstr<lun_stato;lstr++)
		str_val[lstr]=' ';
	if(ctrlstato(str_val,&storg[dbode[point].tipo],stat_diz)>=0)
		dbosc[point]=(dbosc[point] & 0xFF00)+ctrlstato(str_val,&storg[dbode[point].tipo],stat_diz);
	break;
	}
   case (g1tipst):
	{
	strncpy(dbstv+(point*g0lunst),str_val,g0lunst);
	break;
	}
   other:
	{
	break;
        }
}
}

ClearToggleButtonState(Widget cw)
{
int i;
Widget widChild;


for(i=1;i<=nToggleButton;i++)
{
        widChild=GetToggleButton(cw,i);

        /* Se non trova figlio stampa segnalazione di errore ed esce */
        if(!widChild)
        {
                printf("util_xscada.c [ClearToggleButtonState] widChild NULL !!!\n");
                return;
        }

	XmToggleButtonSetState(widChild,FALSE,FALSE);
}
}

RefToggleButtonState(Widget cw,short ext,short point)
{
int i,j;
Widget widChild;

/* Scandisce tutti i bottoni dei flag */
for(i=1,j=0;i<=nToggleButton;j++,i++)
{
        widChild=GetToggleButton(cw,i);

        /* Se non trova figlio stampa segnalazione di errore ed esce */
        if(!widChild)
        {
                printf("util_xscada.c [RefToggleButtonState] widChild NULL !!!\n");
                return;
        }

	switch (ext) {
	   case (g1tipaa):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbaaf[point],xfan[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipas):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbasf[point],xfan[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipac):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbacf[point],xfan[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipad):
		{
		if(xfan[j].ind!=-1 && strcmp(xfan[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbadf[point],xfan[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipao):
		{
		if(xfao[j].ind!=-1 && strcmp(xfao[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbaof[point],xfao[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipda):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbda[point],xfdi[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipds):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbds[point],xfdi[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipdc):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbdc[point],xfdi[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipdd):
		{
		if(xfdi[j].ind!=-1 && strcmp(xfdi[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbdd[point],xfdi[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipor):
		{
		if(xfor[j].ind!=-1 && strcmp(xfor[j].pFlag,""))
			XmToggleButtonSetState(widChild,bitvalue(&dbosc[point],xfor[j].ind),FALSE);
		else
			XmToggleButtonSetState(widChild,FALSE,FALSE);
		break;
		}
	   case (g1tipdo):
	   case (g1tipst):
	   other:
		{
		ClearToggleButtonState(cw);
		break;
       		}
	}
}
}

RefSlot(Widget cw,short point, short ext)
{
float valore = -1.0;
short digVal;
Boolean trovato=FALSE;
char str_val[50];
char nome[100];
char nomeLabel[100];
Widget wid;

if (point < 0)
	ext = -1;

switch (ext) {
   case (g1tipaa):
	{
	valore = dbaav[point];
	sprintf(str_val,"%f",valore);
	trovato=TRUE;
	break;
	}
   case (g1tipas):
	{
	valore = dbasv[point];
	sprintf(str_val,"%f",valore);
	trovato=TRUE;
	break;
	}
   case (g1tipac):
	{
	valore = dbacv[point];
	sprintf(str_val,"%f",valore);
	trovato=TRUE;
	break;
	}
   case (g1tipad):
	{
	valore = dbadv[point];
	sprintf(str_val,"%f",valore);
	trovato=TRUE;
	break;
	}
   case (g1tipao):
	{
	sprintf(str_val,"        ");
	trovato=TRUE;
	break;
	}
   case (g1tipda):
	{
	digVal=bitvalue(&dbda[point],g2di_sl);
	sprintf(str_val,"%d",digVal);
	trovato=TRUE;
	break;
	}
   case (g1tipds):
	{
	digVal=bitvalue(&dbds[point],g2di_sl);
	sprintf(str_val,"%d",digVal);
	trovato=TRUE;
	break;
	}
   case (g1tipdc):
	{
	digVal=bitvalue(&dbdc[point],g2di_sl);
	sprintf(str_val,"%d",digVal);
	trovato=TRUE;
	break;
	}
   case (g1tipdd):
	{
	digVal=bitvalue(&dbdd[point],g2di_sl);
	sprintf(str_val,"%d",digVal);
	trovato=TRUE;
	break;
	}
   case (g1tipdo):
	{
	sprintf(str_val,"        ");
	trovato=TRUE;
	break;
	}
   case (g1tipor):
	{
	memcpy(str_val,&stat_diz[(dbosc[point] &  0x00FF) *lun_stato],lun_stato);
	str_val[lun_stato]='\0';
	trovato=TRUE;
	break;
	}
   case (g1tipst):
	{
	sprintf(str_val,"%s",dbstv+(g0lunst*point));
	trovato=TRUE;
	break;
	}
   other:
	{
	trovato=FALSE;
	break;
        }
}

if(!trovato)
{
	sprintf(str_val,"not found");
	ClearToggleButtonState(cw);
}
else
	RefToggleButtonState(cw,ext,point);
	
strcpy(nome,"textFieldSt");
strcpy(nomeLabel,XtName(cw));
strcat(nome,&nomeLabel[9]);
wid = XtNameToWidget(XtParent(cw),nome);
XmTextSetString(wid,str_val);
}

MandbRefresh(Widget labelinfo)
{
/****
printf("MandbRefresh %d %d %d %d %d\n",
	ind_mandb_1,ind_mandb_2,ind_mandb_3,ind_mandb_4,
		GetSlotByLabel(labelinfo));
****/
if(GetSlotByLabel(labelinfo)==1)
	RefSlot(labelinfo,ind_mandb_1,ext_mandb_1);
if(GetSlotByLabel(labelinfo)==2)
	RefSlot(labelinfo,ind_mandb_2,ext_mandb_2);
if(GetSlotByLabel(labelinfo)==3)
	RefSlot(labelinfo,ind_mandb_3,ext_mandb_3);
if(GetSlotByLabel(labelinfo)==4)
	RefSlot(labelinfo,ind_mandb_4,ext_mandb_4);
if(GetSlotByLabel(labelinfo)==5)
	RefSlot(labelinfo,ind_mandb_5,ext_mandb_5);
}

static void SettaDescr(Widget wid, short point, short ext)
{
char *app_descrizione;
char descrizione[SCD_DESCRIZIONE+1];
char nome_var[SCD_SIGLA+1];
char riga[SCD_SIGLA+SCD_DESCRIZIONE+20];
extern long  off_f[] ;          /* definiti nel main mpscs */
extern short nbyte[] ;          /* come sopra */
long tot;

memset(nome_var,0,SCD_SIGLA+1);
strncpy(nome_var,XmTextFieldGetString(wid),SCD_SIGLA);

tot=off_f[ext]+(long)nbyte[ext]*(long)point;
app_descrizione = (char *)malloc(nbyte[ext]);
if(rbyte(fileno(fpp[fdde]),(short*)app_descrizione,tot,nbyte[ext])== -1)
	memset(app_descrizione,0,nbyte[ext]);

app_descrizione[SCD_SIGLA+SCD_DESCRIZIONE]=0;
strcpy(descrizione,&app_descrizione[SCD_SIGLA]);
sprintf(riga,"%s - %s",nome_var,descrizione);
//printf("%s",riga);
XmTextFieldSetString(wid,riga);
free(app_descrizione);
}

static void Maiuscolo(char *testo)
{
int i;
for(i=0;i<strlen(testo);i++)
	testo[i]=toupper(testo[i]);
}

/*
 get_something
    legge un singolo attributo di un Widget
 */
void get_something(w, resource, (void*) value)
    Widget w;
    char *resource, *value;
{
Arg al[1];
XtSetArg(al[0], resource, value);
XtGetValues(w, al, 1);
}

/*
 set_something_val
    setta un singolo attributo di un Widget
 */
void set_something_val(w, resource, (XtArgVal) value)
    Widget w;
    char *resource, *value;
{
Arg al[1];
XtSetArg(al[0], resource, value);
XtSetValues(w, al, 1);
}

static Pixel CvtStrToPixel (Widget ogget,char *Stringa)
{
        unsigned long piani[1];
        Colormap colmap;
        XColor xcol;

        colmap = DefaultColormap ( XtDisplay(ogget),
                        DefaultScreen (XtDisplay(ogget)) );
        if ( !XParseColor (XtDisplay(ogget) , colmap , Stringa , &xcol) )
           {
           printf("Errore XParseColor: colore [%s] inesistente\n",Stringa);
           return(False);
           }
        if(!XAllocColor(XtDisplay(ogget),colmap,&xcol))
           {
           printf("Errore XAllocColor: impossibile allocare un nuovo colore\n");
           return(False);
           }
        return(xcol.pixel);
}

Boolean LoadBitmap
	(Widget w, Pixmap *stip, char * bits, int width, int height)
{
long cnero,cbianco;
Pixel background;

get_something(w,XmNbackground,(void*) &background);
background=CvtStrToPixel(w,"#00007b7b0202");

if((*stip=XCreatePixmapFromBitmapData(XtDisplay(w),
            RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),bits,
            width,height,
	    CvtStrToPixel(w,"#000000000000"),CvtStrToPixel(w,"#50a0a0"),
            DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w)))))== False)
        {
        fprintf(stderr,"\n Errore creazione pixmap");
        exit(1);
        }
}

#endif
