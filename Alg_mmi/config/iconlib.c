/**********************************************************************
*
*       C Source:               iconlib.c
*       Subsystem:              8
*       Description:
*       %created_by:    famgr %
*       %date_created:  Wed Oct  7 11:35:25 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: iconlib.c-3 %  (%full_filespec: iconlib.c-3:csrc:8 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)iconlib.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*----------------------------------------------------------------------- 
 *
 * Iconlib.c 
 *
 * Contents: modulo contenente le routine di gestione delle icon library 
 *           
 *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <Xm/Xm.h>
#include "config.h"

extern Display *UxDisplay;

extern void add_def_translation();


/*-------------------------------------------------------------------------
 * iconlib_init()
 *
 * inizializzazione struttura dati di pagina tipo DRAWING
 *
 * input:
 *       char *fileicon;    nome del file delle risorse della pagina
 * output:
 *       PAGINA *pag;      puntatore alla struttura da allocare
 *
 *------------------------------------------------------------------------*/
int iconlib_init( char *lname, ICONLIB **icon )
{
   extern PAGEDIT_CONTEXT *pagedit;
   int retval;
   char libfile[MAXLENFILENAME];

   strcpy(libfile,pagedit->objectLibrary);
   strcat(libfile,"/");
   strcat(libfile,lname);
   strcat(libfile,LIB_EXT);

   printf("iconlib_init on file %s\n",libfile);

   if( (*icon = (ICONLIB*)alloca_memoria(1,sizeof(ICONLIB))) == NULL)
      return(False);

/* load delle risorse */
   UxDisplay->db =(*icon)->db = XrmGetFileDatabase(XlConvPathVms(libfile));

}


/*------------------------------------------------------------
 * iconlib_getres()
 *
 * load delle resources del Db delle iconlib
 *
 * input:
 *       Widget  widpadre: ID widget del padre
 *       ICONLIB *pagina;  pointer alla struttura dati
 *
 * retvalue:
 *           True
 *           False
 *------------------------------------------------------------*/

int iconlib_getres(ICONLIB *iconlib,AN_OBJ **listaicon)
{
   extern Arg args[];
   extern Cardinal lnargs;
   int i,retval;
   char *tipo,*appo,classname[100];
   XrmValue value;
                        
/* get numero widget sulla iconlib */
   retval = XrmGetResource(iconlib->db, XtNnum_widget, XtCNum_widget, &tipo, &value);
   if( retval == False)
   {
      printf("\nI'm sorry, error on Get Resource\n");
      return(retval);
   }

   iconlib->num_widget = atoi(value.addr);

/* get dimensioni della drawing area */
   retval = 
     XrmGetResource(iconlib->db, XlNheightDraw, XlCHeightDraw, &tipo, &value);
   if( retval == False)
   {
      printf("\nI'm sorry, error on Get Resource\n");
      return(retval);
   }
   iconlib->draw_height = atoi(value.addr);

   retval = 
     XrmGetResource(iconlib->db, XlNwidthDraw, XlCWidthDraw, &tipo, &value);
   if( retval == False)
   {
      printf("\nI'm sorry, error on Get Resource\n");
      return(retval);
   }
   iconlib->draw_width = atoi(value.addr);

/* carico la lista degli widget */
   retval = XrmGetResource(iconlib->db, XtNelenco_wid0, XtCElenco_wid0, &tipo, &value);
   if( retval == False)
   {
      printf("\nI'm sorry, error on Get Resource\n");
      return(retval);
   }

/* inizializzazione dei descrittori degli widget sulla iconlib */

   if( (*listaicon =  (AN_OBJ *)alloca_memoria(iconlib->num_widget,sizeof(AN_OBJ))) == NULL)
      return(False);

   appo = value.addr;

   for(i=0;i<iconlib->num_widget;i++)
   {
      sscanf(appo,"%s",(*listaicon)[i].nome);
      appo += ( strlen( (*listaicon)[i].nome ) +1 );
      sscanf(appo,"%s",classname);
      appo += ( strlen( classname ) +1 );
      (*listaicon)[i].classe = XlGetClassWid(classname);
   }

}


int iconlib_create_widget(ICONLIB *lib,AN_OBJ *listaicon)
{
   int i;
   extern Boolean XlCheckLib();

  for(i=0;i<lib->num_widget;i++)
     XtCreateManagedWidget(listaicon[i].nome,listaicon[i].classe,lib->drawing,NULL,0);
   
  /*
	controllo la consistenza della libreria appena creata
  */
  if(!XlCheckLib(lib->drawing))
	show_message("Error in library\n");
}

/*------------------------------------------------------------
 * iconlib_def_translation()
 *
 * definizione delle translation per le icon library 
 *
 * input:
 *       ICONLIB *plib;  pointer alla struttura dati 
 *
 * retvalue:
 *           True
 *           False
 *------------------------------------------------------------*/
void iconlib_setta_trans(char *trans,WidgetList widlist,Cardinal numwid)
{
   int i;

   for(i=0;i<numwid;i++)
      add_def_translation(widlist[i],trans);

}

void iconlib_def_translation(ICONLIB *iconlib)
{
   extern void get_child();
   char *tipo;
   int i,j,retval;
   XrmValue value;
   WidgetList children,xlwid,xl2wid;
   Cardinal num_children,nxlwid;
   int k;
   Cardinal n2xlwid;
   char stringa[200];

   get_child(iconlib->drawing,&children,&num_children);
printf("numero_children=%d\n",num_children);
   for(i=0;i<num_children;i++)
   {
      sprintf(stringa,ADD_ICON_TRANS,iconlib);
      add_def_translation(children[i],stringa);

      if( XlIsXlManager(children[i]) && XlIsXlComposite(children[i]) )
      {
         get_child(children[i],&xlwid,&nxlwid);
         sprintf(stringa,ADD_ICON_PARENT,iconlib);

         for(j=0;j<nxlwid;j++)
            add_def_translation(xlwid[j],stringa);
      }

      if( XlIsXlManager(children[i]) && !XlIsXlComposite(children[i]) )
      {
         printf("Sono un Cambio pagina o un Set Valore\n");
         get_child(children[i],&xlwid,&nxlwid);

         for(j=0;j<nxlwid;j++)
	    {
            sprintf(stringa,ADD_ICON_PARENT,iconlib);
            add_def_translation(xlwid[j],stringa);
/*
 Gestione per l'oggetto grafici: viene gestito un altro
 livello di figli
*/
	    if(XtIsComposite(xlwid[j]))
		{
          	get_child(xlwid[j],&xl2wid,&n2xlwid);
         	sprintf(stringa,ADD_ICON_PARENT_PARENT,iconlib);
		for(k=0; k< n2xlwid; k++)
            		add_def_translation(xl2wid[k],stringa);
		}
	    }
      }
   
   }

}

/******************************** end of file ****************************/
