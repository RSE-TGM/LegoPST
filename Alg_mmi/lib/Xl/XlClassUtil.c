/**********************************************************************
*
*       C Source:               XlClassUtil.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jun 11 11:52:22 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlClassUtil.c-3.1.3.1.3.1.1 %  (%full_filespec: XlClassUtil.c-3.1.3.1.3.1.1:csrc:1 %)";
#endif
/*
   modulo XlClassUtil.c
   tipo 
   release 5.2
   data 1/5/96
   reserved @(#)XlClassUtil.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlClassUtil.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS
*/
/*-------------------------------------------------------------------*
 *                                                                   *
 * Module Name:   XlClassUtil.c                                    *
 *                                                                   *
 * Date: 14 Gennaio 1994        Ver: 1.0                             *
 *-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <X11/Core.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlCompositeP.h>
#include <Xl/XlLedP.h>
#include <Xl/XlCaiP.h>
#include <Xl/XlCaiAllP.h>
#include <Xl/XlCaiVarP.h>
#include <Xl/XlBottoneP.h>
#include <Xl/XlIndicP.h>
#include <Xl/XlIndicErrP.h>
#include <Xl/XlLabelP.h>
#include <Xl/XlAllarmiP.h>
#include <Xl/XlBitmapP.h>
#include <Xl/XlIconP.h>
#include <Xl/XlSelettoreP.h>
#include <Xl/XlSetValoreP.h>
#include <Xl/XlChangePageP.h>
#include <Xl/XlGraficoP.h>
#include <Xl/XlPortP.h>
#include <Xl/XlIconRegP.h>
#include <Xl/XlDispRegP.h>
#include <Xl/XlSincroP.h>
#include <Xl/XlSincroOldP.h>
#include <Xl/XlPictographP.h>
#include <Xl/XlIndicTelepP.h>
#include <Xl/XlOperableKeysP.h>
#include <Xl/XlPixvar.h>
#include <Xl/XlPixvarP.h>
#include <Xl/XlCurveP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

/* array contenente le classi di widget trattate da legommi */
 static WidgetClass classi[]= {
		         (WidgetClass)&xlCoreClassRec,
			 (WidgetClass)&xlBottoneClassRec,
			 (WidgetClass)&xlLedClassRec,
			 (WidgetClass)&xlCaiClassRec,
			 (WidgetClass)&xlCaiAllClassRec,
			 (WidgetClass)&xlCaiVarClassRec,
			 (WidgetClass)&xlPortClassRec,
			 (WidgetClass)&xlIndicClassRec,
			 (WidgetClass)&xlIndicErrClassRec,
			 (WidgetClass)&xlLabelClassRec,
			 (WidgetClass)&xlBitmapClassRec,
			 (WidgetClass)&xlSelettoreClassRec,
			 (WidgetClass)&xlSetValoreClassRec,
			 (WidgetClass)&xlChangePageClassRec,
			 (WidgetClass)&xlIconClassRec,
			 (WidgetClass)&xlIconRegClassRec,
			 (WidgetClass)&xlDispRegClassRec,
			 (WidgetClass)&xlManagerClassRec,
			 (WidgetClass)&xlCompositeClassRec,
			 (WidgetClass)&xlAllarmiClassRec,
			 (WidgetClass)&xlGraficoClassRec,
			 (WidgetClass)&xlSincroClassRec,
			 (WidgetClass)&xlSincroOldClassRec,
			 (WidgetClass)&xlPictographClassRec,
			 (WidgetClass)&xlIndicTelepClassRec,
			 (WidgetClass)&xlOperableKeysClassRec,
			 (WidgetClass)&xlCurveClassRec,
			 (WidgetClass)&xlPixvarClassRec
		      };

Boolean XlSetSelectPort(Boolean *);
char *XlGetClassName(WidgetClass);
WidgetClass XlGetClassWid(char *);
Boolean XlSetSelectRunTime(void *);

/*------------------------------------------------------------------
 * Function Name:   XlGetClassName                                 *
 *                                                                 *
 * Parameter:                                                      *
 *      WidgetClass classe;          classe                        *
 *                                                                 *
 * Description:   ritorna il puntatore all classe in forma ASCII   *
 *                                                                 *
 * Date: 14 Gennaio 1994        Ver: 1.0                           *
 *-----------------------------------------------------------------*/

char *XlGetClassName(WidgetClass classe)
{
   return( ((CoreWidgetClass)classe)->core_class.class_name );
}


/*------------------------------------------------------------------
 * Function Name:   XlGetClassWid                                  *
 *                                                                 *
 * Parameter:                                                      *
 *      char *str_classe_name;          classe                     *
 *                                                                 *
 * Description:   trasforma la stringa di descrizione della classe *
 *                 nel puntatore alla classe                       *
 *                                                                 *
 * Date: 14 Gennaio 1994        Ver: 1.0                           *
 *-----------------------------------------------------------------*/

WidgetClass XlGetClassWid(char *str_class_name)
{
   int i,num_classi;

   num_classi=XtNumber(classi);
   for(i=0; i<num_classi; i++)
   {
      if(strcmp(str_class_name,((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)
      return(classi[i]);
   }
   return(NULL);
}

void XlRedisplay(Widget w)
{
XlCoreWidget cw= (XlCoreWidget)w;
XlManagerWidget mw= (XlManagerWidget)w;
XlCoreWidgetClass core_class;
XlManagerWidgetClass manager_class;
int save_trasp;
WidgetList child;
int i,num_child;

if(XlIsXlCore(w))
	{
/*
 per evitare l'alloopamento setta l'attributo di trasparent
 a False in modo che non venga richiamato il ridisegno del
 background (infatti XlRedisplay viene richiamato proprio
 dal ridisegno del background per visualizzare in foreground
 gli oggetti Xl
*/
	save_trasp= cw->xlcore.trasparent;
	cw->xlcore.trasparent= False;
	core_class=(XlCoreWidgetClass) XtClass(w);
	core_class->core_class.expose(w,NULL,NULL);
	cw->xlcore.trasparent= save_trasp;
	}
else
	{
	save_trasp= mw->xlmanager.trasparent;
	mw->xlmanager.trasparent= False;
	manager_class=(XlManagerWidgetClass) XtClass(w);
	manager_class->core_class.expose(w,NULL,NULL);
	mw->xlmanager.trasparent= save_trasp;
	}
/*
	se l'oggetto e' un XlComposite chiama il Redisplay dei figli
*/
if(XlIsXlComposite(w))
	{
	get_something (w,XmNchildren, (void*) &child);
	get_something (w,XmNnumChildren, (void*) &num_child);
	for(i=0;i<num_child;i++)
		XlRedisplay(child[i]);
	}
}



Boolean XlSetSelectRunTime(void * selectRunTime)
{
   int i,num_classi;

   num_classi=XtNumber(classi);
   for(i=0; i<num_classi; i++)
   {
   if(
(strcmp("XlCore",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Bottone",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Led",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Cai",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("CaiAll",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("CaiVar",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Port",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("DispReg",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Indic",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("IndicErr",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Label",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Bitmap",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Icon",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Selettore",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("Sincro",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)||
(strcmp("SincroOld",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL) ||
(strcmp("Pictograph",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL) ||
(strcmp("IndicTelep",((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL) ||
(strcmp(XlCPixvar,((CoreWidgetClass)(classi[i]))->core_class.class_name)==NULL)
)
      
    ((XlCoreWidgetClass)(classi[i]))->xlcore_class.selectRunTime=selectRunTime;
   else
    ((XlManagerWidgetClass)(classi[i]))->
		xlmanager_class.selectRunTime=selectRunTime;

   }
return(True);
}


Boolean XlSetSelectPort(Boolean * selectPort)
{
	xlPortClassRec.xlport_class.selectPort=selectPort;
	return (True);
}
