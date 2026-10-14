/**********************************************************************
*
*       C Source:               XlWidgetRes.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Wed Jun  4 12:09:38 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlWidgetRes.c-4 %  (%full_filespec: XlWidgetRes.c-4:csrc:1 %)";
#endif
/*
   modulo XlWidgetRes.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlWidgetRes.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlWidgetRes.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/

/*----------------------------------------------------------------------
 *       							       *
 *  XlGetResWidget						       *
 *   								       * 
 *---------------------------------------------------------------------*/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xl/Xl.h>
#include <Xl/XlP.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlWidgetRes.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

static Boolean GetResWid(Widget,XlWidgetResList* ,int *);
static char* PixelToString(Widget,Pixel);
static int Compara(XlWidgetRes* ,XlWidgetRes *);
static int ComparaVar(XlWidgetRes* ,XlWidgetRes *);
static int OutRiga(char *,FILE *,int);
static Boolean PrWid(XlWidgetResList ,int ,char *,FILE *, int);
static Boolean PrVar(XlWidgetResList ,int ,FILE *);
static char *GetValore(XlWidgetResList ,int ,char *,char *);
static void scomponi(char *,char *,char *,char *);


Boolean XlGetResWidget(Widget drawing, XlWidgetResList *lista, int *num_res)
{
Boolean ret = True;
Arg args[10];
Cardinal lnargs;
WidgetList figli,nipoti;
Cardinal num_figli,num_nipoti;
int i,j;

    /*
	inizializza il vettore lista
    */
    *lista=NULL;
    *num_res=0;
    lnargs=0;
    XtSetArg(args[lnargs],XmNnumChildren,&num_figli);lnargs++;
    XtSetArg(args[lnargs],XmNchildren,&figli);lnargs++;
    XtGetValues(drawing,args,lnargs);
    for(i=0;i<num_figli;i++)
	{
	GetResWid(figli[i],lista,num_res);
	if(XlIsXlComposite(figli[i]))
	   {
	   lnargs=0;
    	   XtSetArg(args[lnargs],XmNnumChildren,&num_nipoti);lnargs++;
    	   XtSetArg(args[lnargs],XmNchildren,&nipoti);lnargs++;
    	   XtGetValues(figli[i],args,lnargs);
	   for(j=0;j<num_nipoti;j++)
		GetResWid(nipoti[j],lista,num_res);
	   }
	}
qsort(*lista,*num_res,sizeof(XlWidgetRes),Compara);
return(ret);
}

static Boolean GetResWid(Widget w,XlWidgetResList *lista, int *num_res)
{
XlWidgetRes *pres;
XlConfInfo *confinfo,*confinfo_wid;
XlCoreClassRec * wxlcoreclass;
XlManagerClassRec * wxlmanagerclass;
int i,num_confinfo,k,num_conf_core,num_conf_manager,num_conf_wid;
char opzione[250];
float app_float;
int app_int;
Position app_pos;
Dimension app_dim;
char *riga_app, *token;
Pixel app_pix;

	/*
		prendo le confinfo
	*/
	if(XlIsXlCore(w))
        {
        wxlcoreclass=(XlCoreClassRec*)XtClass(w);
        num_conf_wid=wxlcoreclass->xlcore_class.num_confinfo;
	num_conf_core=xlCoreClassRec.xlcore_class.num_confinfo;
	num_confinfo=num_conf_wid+num_conf_core;
	confinfo=(XlConfInfo*)XtMalloc(num_confinfo*sizeof(XlConfInfo));
	memcpy(&confinfo[0],xlCoreClassRec.xlcore_class.confinfo,
		num_conf_core*sizeof(XlConfInfo));
	memcpy(&confinfo[num_conf_core],
		wxlcoreclass->xlcore_class.confinfo,
		num_conf_wid*sizeof(XlConfInfo));
        }
	else
        {
        wxlmanagerclass=(XlManagerClassRec*)XtClass(w);
        num_conf_wid=wxlmanagerclass->xlmanager_class.num_confinfo;
	num_conf_manager=xlManagerClassRec.xlmanager_class.num_confinfo;
	num_confinfo=num_conf_wid+num_conf_manager;
	confinfo=(XlConfInfo*)XtMalloc(num_confinfo*sizeof(XlConfInfo));
	memcpy(&confinfo[0],xlManagerClassRec.xlmanager_class.confinfo,
		num_conf_manager*sizeof(XlConfInfo));
	memcpy(&confinfo[num_conf_manager],
		wxlmanagerclass->xlmanager_class.confinfo,
		num_conf_wid*sizeof(XlConfInfo));
        }


	/*
		setta una riga della lista con solo il nome del Widget
		e il campo type a -2 per gli oggetti di primo livello e
		-1 per quelli figli di un composite
	*/
	++(*num_res);
	(*lista)=
           (XlWidgetRes *)XtRealloc((*lista),(*num_res)*sizeof(XlWidgetRes));
	pres=(XlWidgetRes*)(&(*lista)[(*num_res)-1]);
	memset(pres,0,sizeof(XlWidgetRes));
	strcpy(pres->nome_wid,XtName(w));
	strcpy(pres->nome_classe,XlGetClassName(XtClass(w)));
	if(XlIsXlComposite(XtParent(w)))
		{
		strcpy(pres->nome_wid_padre,XtName(XtParent(w)));
		pres->type= -1;
		}
	else
		{
		pres->nome_wid_padre[0]=0;
		pres->type= -2;
		}
	


	for(i=0;i<num_confinfo;i++)
        {
	++(*num_res);
	(*lista)=
           (XlWidgetRes *)XtRealloc((*lista),(*num_res)*sizeof(XlWidgetRes));
	pres=(XlWidgetRes*)(&(*lista)[(*num_res)-1]);
	memset(pres,0,sizeof(XlWidgetRes));
	strcpy(pres->nome_wid,XtName(w));
	strcpy(pres->nome_classe,XlGetClassName(XtClass(w)));
	if(XlIsXlComposite(XtParent(w)))
		strcpy(pres->nome_wid_padre,XtName(XtParent(w)));
	else
		pres->nome_wid_padre[0]=0;
	pres->type=confinfo[i].confinfo_type;
	pres->group=confinfo[i].confinfo_group;
	strcpy(pres->name,confinfo[i].confinfo_name);
        switch(confinfo[i].confinfo_type){
                case XlRTipoVarInp:
                case XlRTipoVarOut:
                case XlRTipoVarReg:
                case XlRTipoVarInp+LOCKED_PAG:
                case XlRTipoVarOut+LOCKED_PAG:
                case XlRTipoVarReg+LOCKED_PAG:
                case XlRTipoVarPort:
                case XlRGerarchia:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &riga_app);
			strcpy(pres->valore,riga_app);
                        break;
                        }
                case XlRText:
                case XlRFile:
                case XlRComponent:
                case XlRSubSystem:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &riga_app);
			strcpy(pres->valore,riga_app);
                        break;
                        }
                case XlRFloat:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_float);
                        sprintf(pres->valore,"%f",app_float);
                        break;
                        }
                case XlRInt:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_int);
                        sprintf(pres->valore,"%d",app_int);
                        break;
                        }
                case XlRPosition:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_pos);
                        sprintf(pres->valore,"%d",app_pos);
                        break;
                        }
                case XlRColor:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_pix);
                        sprintf(pres->valore,"%s",PixelToString(w,app_pix));
                        break;
                        }
                case XlRDimension:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_dim);
                        sprintf(pres->valore,"%d",app_dim);
                        break;
                        }
                case XlROption:
                        {
                        get_something(w,confinfo[i].resource_name, (void*) &app_int);
                        strcpy(opzione,confinfo[i].enum_options);
                        token=strtok(opzione,",");
                        for(k=0;k<app_int;k++)
                                token=strtok(NULL,",");
                        sprintf(pres->valore,"%s",token);
                        break;
                        }
		}
                                                                     
	}
/*
	libero le confinfo
*/
XtFree(confinfo);
}

Boolean XlPrintResWidget(XlWidgetResList lista, int num_res,char *nome)
{
int i,j;
FILE *fp=NULL;
Boolean su_file;
char testo[1000];
char nome_file[300];


if(nome_file==NULL)
	su_file=False;
else
	su_file=True;

if(su_file)
  	{
	sprintf(nome_file,"%s.info",nome);
	fp=fopen(nome_file,"w");
	if(fp==(FILE*)NULL)
		su_file=False;
	}

/*
	intestazione
*/
if(su_file)
{
OutRiga(
"****************************************************************************",
fp,0);
OutRiga(" ",fp,0);
OutRiga(" ",fp,0);
sprintf(testo,"                        Pagina  %s",nome);
OutRiga(testo,fp,0);
OutRiga(" ",fp,0);
OutRiga(" ",fp,0);
OutRiga(
"****************************************************************************",
fp,0);
OutRiga(" ",fp,0);
}
for(i=0;i<num_res;i++)
   if(lista[i].type== -2)
	{
	PrWid(lista,num_res,lista[i].nome_wid,fp,0);
	for(j=0;j<num_res;j++)
   		if((lista[j].type== -1)&&
			(strcmp(lista[j].nome_wid_padre,lista[i].nome_wid)==0))
				PrWid(lista,num_res,lista[j].nome_wid,fp,5);
	}

PrVar(lista,num_res,fp);
if(su_file)
	fclose(fp);
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

static int Compara(XlWidgetRes *p1,XlWidgetRes *p2)
{
int ret;

ret=strcmp(p1->nome_classe,p2->nome_classe);
if(ret!=0)
	return(ret);

ret=strcmp(p1->nome_wid,p2->nome_wid);
if(ret!=0)
	return(ret);

if(p1->type>p2->type)
	return(1);
if(p1->type<p2->type)
	return(-1);
return(0);
}



static int OutRiga(char * testo,FILE *fp,int off)
{
int i=0;

for(i=0;i<off;i++)
	if(fp==NULL)
		printf(" ");
	else
		fprintf(fp," ");
	
if(fp==NULL)
	printf("%s\n",testo);
else
	fprintf(fp,"%s\n",testo);
}

static Boolean PrWid(XlWidgetResList lista,int num_res,
		char * nome,FILE *fp,int off)
{
char testo[1000];
int i;

for(i=0;i<num_res;i++)
  if((lista[i].type<0)&&(strcmp(lista[i].nome_wid,nome)==0))
	{
	sprintf(&testo[0],"Widget Class=%s Widget Name=%s",
		lista[i].nome_classe,lista[i].nome_wid);
	OutRiga(testo,fp,off);
	break;
	}
sprintf(&testo[0],"%s=%s - %s=%s",
	XlDx0,GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
	XlDy0,GetValore(lista,num_res,lista[i].nome_wid,XlDy0));
OutRiga(testo,fp,off+4);
sprintf(&testo[0],"%s=%s - %s=%s",
	XlDwidth0,GetValore(lista,num_res,lista[i].nome_wid,XlDwidth0),
	XlDheight0,GetValore(lista,num_res,lista[i].nome_wid,XlDheight0));
OutRiga(testo,fp,off+4);
for(i=0;i<num_res;i++)
  if((lista[i].type>0)&&(strcmp(lista[i].nome_wid,nome)==0)&&
	(lista[i].group!=XlRGrGeom))
	{
	sprintf(&testo[0],"%s = %s", lista[i].name,
		GetValore(lista,num_res,lista[i].nome_wid,lista[i].name));
	OutRiga(testo,fp,off+4);
	}

/*
	ultima riga
*/
OutRiga("      ",fp,0);
}


static char *GetValore(XlWidgetResList lista,int num_res,
		char *nome_wid,char *nome_res)
{
char *ret=" ";
int i;
for(i=0;i<num_res;i++)
  if((strcmp(lista[i].name,nome_res)==0)
	&&(strcmp(lista[i].nome_wid,nome_wid)==0))
		return(lista[i].valore);

return(ret);
}

static Boolean PrVar(XlWidgetResList lista,int num_res,FILE *fp)
{
char testo[1000];
int i;

/*
	ordina per variabili
*/
qsort(lista,num_res,sizeof(XlWidgetRes),ComparaVar);

OutRiga("      ",fp,0);
OutRiga("      ",fp,0);
OutRiga("Elenco Variabili      ",fp,0);
OutRiga("  ",fp,0);
OutRiga("Variabili di input",fp,0);
for(i=0;i<num_res;i++)
   if((lista[i].type == XlRTipoVarInp)&&(strcmp(lista[i].valore,"")!=0))
	{
	if(lista[i].nome_wid_padre[0] == 0)
	   sprintf(&testo[0],"%s {%s - %s (%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0));
	else
	   sprintf(&testo[0],"%s {%s - %s (%s,%s) %s(%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0),
		lista[i].nome_wid_padre,
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDy0));
	OutRiga(testo,fp,2);
	}

OutRiga("  ",fp,0);
OutRiga("Variabili di output",fp,0);
for(i=0;i<num_res;i++)
   if((lista[i].type == XlRTipoVarOut)&&(strcmp(lista[i].valore,"")!=0))
	{
	if(lista[i].nome_wid_padre[0] == 0)
	   sprintf(&testo[0],"%s {%s - %s (%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0));
	else
	   sprintf(&testo[0],"%s {%s - %s (%s,%s) %s(%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0),
		lista[i].nome_wid_padre,
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDy0));
	OutRiga(testo,fp,2);
	}

OutRiga("  ",fp,0);
OutRiga("Variabili di regolazione",fp,0);
for(i=0;i<num_res;i++)
   if(((lista[i].type == XlRTipoVarReg)||
	(lista[i].type == XlRTipoVarReg+LOCKED_PAG))
		&&(strcmp(lista[i].valore,"")!=0))
	{
	if(lista[i].nome_wid_padre[0] == 0)
	   sprintf(&testo[0],"%s {%s - %s (%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0));
	else
	   sprintf(&testo[0],"%s {%s - %s (%s,%s) %s(%s,%s)}", 
		lista[i].valore,lista[i].nome_classe,lista[i].nome_wid,
		GetValore(lista,num_res,lista[i].nome_wid,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid,XlDy0),
		lista[i].nome_wid_padre,
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDx0),
		GetValore(lista,num_res,lista[i].nome_wid_padre,XlDy0));
	OutRiga(testo,fp,2);
	}
}

static int ComparaVar(XlWidgetRes *p1,XlWidgetRes *p2)
{
char variabile_1[20],blocco_1[20],modello_1[20];
char variabile_2[20],blocco_2[20],modello_2[20];
int ret;
/*
	Ordino in funzione del tipo, prima chiave
*/
if(p1->type>p2->type)
	return(1);
if(p1->type<p2->type)
	return(-1);
/*
	Ordino in funzione del valore, seconda chiave
*/
if((p1->type == XlRTipoVarInp)||
   (p1->type == XlRTipoVarOut)||
   (p1->type == XlRTipoVarReg))
	{
	scomponi(p1->valore,variabile_1,blocco_1,modello_1);
	scomponi(p2->valore,variabile_2,blocco_2,modello_2);

	ret=strcmp(modello_1,modello_2);
	if(ret!=0)
		return(ret);
	
	ret=strcmp(blocco_1,blocco_2);
	if(ret!=0)
		return(ret);
	
	ret=strcmp(variabile_1,variabile_2);
	if(ret!=0)
		return(ret);
	}	
return(0);
}

static void scomponi(char *stringa,char *variabile,char *blocco,char *modello)
{
char app_stringa[300];
char *app_char,*app_char2;

sprintf(variabile," ");
sprintf(blocco," ");
sprintf(modello," ");
if(stringa==NULL)
	return;
if(stringa[0]==' ')
	return;
if(strcmp(stringa,"")==0)
	return;

strcpy(app_stringa,stringa);
app_char=strstr(app_stringa," ");
if(app_char!=NULL)
	{
	*app_char=0;
	strcpy(variabile,app_stringa);
	}

strcpy(app_stringa,stringa);
app_char=strstr(app_stringa," ");
++app_char;
app_char2=strstr(app_char," ");
if(app_char2!=NULL)
	{
	*app_char2=0;
	strcpy(blocco,app_char);
	}

strcpy(app_stringa,stringa);
app_char=strstr(app_stringa," ");
++app_char;
app_char=strstr(app_char," ");
++app_char;
app_char2=strstr(app_char," ");
if(app_char2!=NULL)
	{
	*app_char2=0;
	strcpy(modello,app_char);
	}
}
