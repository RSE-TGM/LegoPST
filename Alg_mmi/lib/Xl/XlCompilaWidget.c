/**********************************************************************
*
*       C Source:               XlCompilaWidget.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:28:51 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlCompilaWidget.c-7 %  (%full_filespec: XlCompilaWidget.c-7:csrc:1 %)";
#endif
/*
   modulo XlCompilaWidget.c
   tipo 
   release 5.2
   data 2/16/96
   reserved @(#)XlCompilaWidget.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlCompilaWidget.c	5.2\t2/16/96";
/*
        Fine sezione per SCCS
*/

/*----------------------------------------------------------------------
 *       							       *
 *  XlCompilaWidget						       *
 *  XlCheckLib	     					               *
 *  XlSetFileErrComp						       *
 *  XlHeaderFileErrComp						       *
 *  XlErrComp   						       *
 *   								       * 
 *---------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <pagresdef.h>
#include <Xm/Xm.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlIconRegP.h>
#include <Xl/XlDispReg.h>
#include <Ol/OlCompiler.h>
#include <Ol/OlDatabaseTopologia.h>

static void SetVarReg();
static void UnSetVarReg();

FILE *fpErrCom;
int numErrCom;
int numErrComWidget;


void XlErrComp(Widget w,char *procedura,char *commentoA,char *commentoB)
{
static char nameWidget[100];
Widget parent;

/*
	incrementa il contatore degli errori
*/
if(w != NULL && strcmp(nameWidget,XtName(w))==0)
	{
	++numErrComWidget;
	}
	else
	{
	++numErrCom;
	numErrComWidget=1;
	
	if(w)
		sprintf(nameWidget,"%s",XtName(w));
	fprintf(fpErrCom,"\n\n");
	}

if(w)
	{
	fprintf(fpErrCom,"%4d.%d) Error on %s [%s]\n",
		numErrCom,numErrComWidget,XtName(w),
		XlGetClassName(XtClass(w)));
	fprintf(fpErrCom,"         Position (%d,%d) ",w->core.x,w->core.y);
	parent=XtParent(w);
	if(XlIsXlComposite(parent))
		{
		fprintf(fpErrCom," - parent name %s position (%d,%d)\n",
			XtName(parent),parent->core.x,parent->core.y);
		}
	else
		{
		fprintf(fpErrCom,"\n");
		}
	}
else /* caso non relativo a singolo widget */
	{
	fprintf(fpErrCom,"%4d - Elaboration error\n", numErrCom);
	}
fprintf(fpErrCom,"         Procedure [%s]",procedura);
fprintf(fpErrCom,"         %s",commentoA);
if(commentoB!=NULL)
	fprintf(fpErrCom,"   %s\n\n",commentoB);
else
	fprintf(fpErrCom,"\n\n");
}

/*
   Apre per la prima volta il file degli errori
*/
void XlSetFileErrComp(char *nomeFile)
{
char nome[FILENAME_MAX];
#if defined VMS
char nome_vms[FILENAME_MAX];
#endif

if(nomeFile==NULL)
	fclose(fpErrCom);
else
	{
	sprintf(nome,"%s_err",nomeFile);
#if defined VMS
	strcpy(nome_vms,XlConvPathVms(nome));
	strcpy(nome,nome_vms);
#endif
	if((fpErrCom=fopen(nome,"w"))==NULL)
		fpErrCom=NULL;

	numErrCom=0;
	numErrComWidget=0;
	return;
	}
}


/*
   Apre il file degli errori
*/
void XlOpenFileErrComp(char *nomeFile)
{
char nome[FILENAME_MAX];
#if defined VMS
char nome_vms[FILENAME_MAX];
#endif

if(nomeFile==NULL)
	fclose(fpErrCom);
else
	{
	sprintf(nome,"%s_err",nomeFile);
#if defined VMS
	strcpy(nome_vms,XlConvPathVms(nome));
	strcpy(nome,nome_vms);
#endif
	if((fpErrCom=fopen(nome,"a"))==NULL)
            {
            printf("Il file %s non esiste, lo apro in write\n",nome);
	    if((fpErrCom=fopen(nome,"w"))==NULL)
    		fpErrCom=NULL;
            }

	numErrCom=0;
	numErrComWidget=0;
	return;
	}
}

void XlHeaderFileErrCompTask(char *nomeFile)
{
fprintf(fpErrCom,
	"****************************************************************************\n\n");
fprintf(fpErrCom,"Generazione dei files f01.dat, f14.dat e della task \n");
fprintf(fpErrCom,
	"\n*****************************************************************************\n");
}

void XlHeaderFileErrComp(char *nomeFile)
{
fprintf(fpErrCom,
	"****************************************************************************\n\n");
fprintf(fpErrCom,"Compilazione pagina %s\n",nomeFile);
fprintf(fpErrCom,
	"\n*****************************************************************************\n");
}

Boolean XlCompilaWidget(XrmDatabase RisDb,char * nomeFile,
			WidgetList widget,Cardinal num_widgets)
{
   XlCoreClassRec *wCoreClass;
   XlManagerClassRec *wManagerClass;  
   WidgetList widget_figli;
   Cardinal num_figli;
   int i,j;
   Boolean ret=True;
   char file_conn[200]; /* Path name per Connessioni.reg */
   OlConnObject conn_obj;
   int g,m,a,o,min,s;
   char str_data[100];
   char res_data[100];
   XrmValue value;
   char *tipo;
   char tipo_pag[200];
   char task_dir[FILENAME_MAX];
   char dummy_char[10];
   char tag_task[3];
   char *appo;

   

   XlSetFileErrComp(nomeFile);
   XlHeaderFileErrComp(nomeFile);

   /*
	Setta l'oggetto Connessioni
   */
#ifndef VMS
   strcpy(file_conn,XlGetenv("LEGOMMI_PAG"));
   strcat(file_conn,"/");
#else
   strcpy(file_conn,XlConvDirVms(XlGetenv("LEGOMMI_PAG")));
#endif
   strcat(file_conn,"Connessioni.reg");

   conn_obj=OlCreateConn(file_conn);
   
   OlSetConn(widget,num_widgets,conn_obj);
   

   for(i=0;i<num_widgets;i++)
   {
      if( XlIsXlCore(widget[i]) )
      {
         wCoreClass=(XlCoreClassRec *)XtClass(widget[i]);
         if(!((wCoreClass->xlcore_class.compile)(widget[i],
               wCoreClass->xlcore_class.compilatore)))
	    ret = False;

      if(RisDb!=NULL)
	 {
	 XlSetResourceByWidget(&RisDb,widget[i],XlNcompiled,
		((XlCoreWidget)widget[i])->xlcore.compiled_str);
	 }
      }
      else
      {
         wManagerClass=(XlManagerClassRec *)XtClass(widget[i]);
         if(!((wManagerClass->xlmanager_class.compile)(widget[i],
            wManagerClass->xlmanager_class.compilatore)))
	    ret = False;

      if(RisDb!=NULL)
	{
	 XlSetResourceByWidget(&RisDb,widget[i],XlNcompiled,
		((XlManagerWidget)widget[i])->xlmanager.compiled_str);
	if(XlIsXlComposite(widget[i]))
		{
		XtVaGetValues( widget[i],
                        XmNnumChildren, &num_figli,
                        XmNchildren, &widget_figli, NULL );
		for(j=0;j<num_figli;j++)
		     {
		     if(XlIsXlCore(widget_figli[j]))
	 	           XlSetResourceByWidget(&RisDb,widget_figli[j],XlNcompiled,
		             ((XlCoreWidget)widget_figli[j])->xlcore.compiled_str);
                     else
	                   XlSetResourceByWidget(&RisDb,widget_figli[j],XlNcompiled,
		       ((XlManagerWidget)widget_figli[j])->xlmanager.compiled_str);
		     }
			
		}
	}
      }
   }

OlDestroyConn(conn_obj);

data(&g,&m,&a);
ora(&o,&min,&s);
snprintf(res_data,sizeof(res_data),"*%s",XlNcompilationDate);
snprintf(str_data,sizeof(str_data),"%d/%d/%d %d:%d:%d",g,m,a,o,min,s);
if(RisDb!=NULL)
	XrmPutStringResource(&RisDb,res_data,str_data);

/************
Se pagina di regolazione inserisci anche iniziali
nome task di regolazione (utile per Xaing su porte)
************/
/*
Verifico che la pagine sia di regolazione
*/
if(RisDb!=NULL)
   XrmGetResource(RisDb,XlNtipoPag,(char*)NULL,&tipo,&value);

if( (value.addr == NULL) || (value.size == 0) )
        {
        fprintf(stderr,"Warning: cannot know tipo Page\n");
        ret = False;
        }
else
   {
   strncpy(tipo_pag,value.addr,(int)value.size);
   /*
   Verify se e' di regolazione
   */
   if(!strcmp(tipo_pag,"Regolazione"))
      {
      strcpy(task_dir,XlGetenv("LEGOMMI_PAG"));
      appo = (char *)OlEstrRegTaskName(task_dir,dummy_char);
      if( appo !=NULL )
            {
            strcpy(tag_task,appo);
            tomaius(tag_task);
            free(appo);
            }
      else
            strcpy(tag_task,"");

      /*printf("\nDEBUG:Nome tag_task=%s !!!\n\n",tag_task);*/
      /*
      Scrivi risorsa nel db della pagina
      */
      sprintf(res_data,"*%s",XlNregTaskSigla);
      XrmPutStringResource(&RisDb,res_data,tag_task);
      
      }
   }



if(nomeFile!=NULL)
	XrmPutFileDatabase(RisDb,XlConvPathVms(nomeFile));

for(i=0;i<num_widgets;i++)
   {
    if(XlIsXlComposite(widget[i]))
	{
	XtVaGetValues( widget[i],
                      XmNnumChildren, &num_figli,
                      XmNchildren, &widget_figli, NULL );
	}
    }



XlSetFileErrComp(NULL);

if(ret)
	printf("Compilazione OK\n");
else
	printf("Errori in compilazione\n");

return(ret);
}

/*
	prende in ingresso la drawing area della libreria e controlla
	la consistenza della libreria
*/
Boolean XlCheckLib(Widget drawing)
{
int i;
WidgetList widget_figli;
Cardinal num_figli;
XlIconRegClassRec *wIconRegClass;  
Boolean ret = True;

XtVaGetValues( drawing,XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
/*
	controllo la consistenza delle icone di regolazione 
*/
for (i=0;i<num_figli;i++)
	if(XlIsIconReg(widget_figli[i]))
		{
                if(!XlCheckIconReg(widget_figli[i]))
	    		ret = False;
		}
return(ret);
}

