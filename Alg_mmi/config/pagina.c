/**********************************************************************
*
*       C Source:               pagina.c
*       Subsystem:              8
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Jun  3 15:02:29 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: pagina.c-12 %  (%full_filespec: pagina.c-12:csrc:8 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)pagina.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*----------------------------------------------------------------------- 
 *
 * Pagina.c 
 *
 * Contents: modulo contenente le routine di gestione della pagina
 *           
 *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <Xm/Xm.h>
#include <X11/Core.h>
#include <Xl/XlCore.h>
#include <Xl/XlManager.h>
#include <Xl/Xl.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlPort.h>
#include <Xl/XlDispReg.h>
#include <Ol/OlConn.h>
#include <Xd/Xd.h>
#include <Xd/XdLista.h>
#include <Xd/XdUndo.h>

#include "UxXt.h"

#include "config.h"
#include "message.h"
#include  "libutilx.h"

#define DEFAULT_REFRESHFREQ 10
extern Display *UxDisplay;
extern Cardinal lnargs;
extern Arg args[];

extern void add_def_translation();


extern int estrai_nome(char *, char *);
extern int elimina_da_lista(char *, int, char *);
extern char *alloca_memoria();

extern XdLista get_lista();
extern XdLista get_lista_undo();
extern XdListaUndo get_lista_liste_undo();

//extern void show_message();
extern Boolean XdSetDeleteConn(void * );
extern int OlFindConnectionByPort(OlConnObject , char *, char* ,char *);
extern void undoListDeleteAll(XdListaUndo);


Boolean WidSelectedAreInEdit(PAGINA *pag)
{
   extern int get_selected();
   WidgetList wgtsel;
   Cardinal nwgtsel; 
   int i;
   
   if( get_selected(pag,&wgtsel,&nwgtsel)  == True)
   {
      for(i=0;i<nwgtsel;i++)
      {
         if( XlIsInEdit(wgtsel[i]) ) {
            libera_memoria((char*)wgtsel);
            return(True);
         }
         else {
            libera_memoria((char*)wgtsel);
            return(False);
         }
      }
   }
   return(False);
}
/******************************** end of file ****************************/

/*-------------------------------------------------
 *
 * pagina_create_widget
 *
 * effettua la creazione ed il Manage di tutti
 * gli oggetti della pagina 
 * lettura del background e attivazione 
 * relativa funzione di redraw.
 *
 *-----------------------------------------------*/

int pagina_create_widget(PAGINA *pag, AN_OBJ *listaobj)
{
  extern PAGEDIT_CONTEXT *pagedit;
  extern int ReadBackground();
  extern int append_redraw(); 
  char filebkg[MAXLENFILENAME];
   int i;

  if (listaobj != NULL)
  {
  for(i=0;i<pag->num_widget;i++)
     {
        XtCreateManagedWidget(listaobj[i].nome,listaobj[i].classe,pag->drawing,NULL,0);
     }
  }
  strcpy(filebkg,pagedit->pages);
  strcat(filebkg,"/");
  strcat(filebkg,pag->nomepag);
  strcat(filebkg,BKG_EXT);
  ReadBackground(pag->drawing,filebkg);
  append_redraw(pag->drawing);
}


/*----------------------------------------
 * 
 * pag_wgeometry
 *
 * write sul file della pagina delle 
 * geometry definite dall'utente                          
 *
 *----------------------------------------*/

void pag_wgeometry(FILE *fp,PAGINA *pag)
{
   char risorsa[100];

   sprintf(risorsa,"*%s: %s\n",XlNnomePag,pag->nomepag);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %d\n",XlNxPag,pag->geom.x);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %d\n",XlNyPag,pag->geom.y);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %d\n",XlNwidthPag,pag->geom.width);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %d\n",XlNheightPag,pag->geom.height);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %s\n",XlNtipoPag,pag->geom.tipo);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s: %s\n",XlNdescrPag,pag->geom.descriz);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s:    %d\n",XlNwidthDraw,pag->geom.draw_width);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s:   %d\n",XlNheightDraw,pag->geom.draw_height);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s:   %s\n",XlNbackgroundDraw,pag->geom.background);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s:   %s\n",XlNgerarchia,pag->gerarchia);
   fprintf(fp,"%s",risorsa);

   sprintf(risorsa,"*%s:   %d\n",XlNrefreshFreq,pag->refreshFreq);
   fprintf(fp,"%s",risorsa);

   if(pag->tagPag!=NULL)
      sprintf(risorsa,"*%s: %s\n",XlNtagPag,pag->tagPag);
   else
      sprintf(risorsa,"*%s: %s\n",XlNtagPag,"");

   fprintf(fp,"%s",risorsa);

   if(pag->num_widget > 0)
   {
      sprintf(risorsa,"*%s: %s\n",XlNnextTagReg,pag->nextTagRegol);
      fprintf(fp,"%s",risorsa);

      sprintf(risorsa,"*%s: %d\n",XlNnumTagReg,pag->numTagReg);
      fprintf(fp,"%s",risorsa);
   }
   else
   {
      sprintf(risorsa,"*%s: %s\n",XlNnextTagReg,"00");
      fprintf(fp,"%s",risorsa);

      sprintf(risorsa,"*%s: %d\n",XlNnumTagReg,0);
      fprintf(fp,"%s",risorsa);

      sprintf(risorsa,"*%s: %d\n",XlNschemeInUse,pag->in_use);
      fprintf(fp,"%s",risorsa);
   }
}


/*----------------------------------------------------
 *
 * pagina_save
 *
 * salva sul file delle risorse i dati della pagina
 * modo ->   modo=1 pagina aperta; modo=0=pagina aperta nella clipboard
 *
 *----------------------------------------------------*/

Boolean pagina_save(PAGINA *pag,int tipo,int modo)
{
   extern XrmDatabase CompressDb; /* data base di appoggio compresso */
   extern void get_child();
   WidgetList children;
   Cardinal num_children=0;
   int i,j,num,read;
   WidgetClass classe;
   String nome_widget,nome_classe;
   char appo[200],*lista,nappo[100];
   char path[MAXLENFILENAME],filename[MAXLENFILENAME];
   FILE *fp;
   char stres[50];
   int num_label = 0;
   int dispM,portM,inteM;
   
   lista = NULL;

   if(pag == NULL)
      return(False);

   if(modo == 1)
	{
   	GetRegoDisplayMode(pag->drawing,&dispM,&portM,&inteM);
   	SetRegoDisplayMode(pag->drawing,
		DISPREG_TAG,PORT_MANAGED,INTERFACE_PORT_MANAGED);
   	XSync(XtDisplay(pag->drawing),False);
	}


   if((tipo == TIPO_PAGINA) )
   {
      if( XlGetenv(ENVPAG) != NULL )
      {
         strcpy(path,XlGetenv(ENVPAG));
         strcpy(filename,path);
         strcat(filename,"/");
         strcat(filename,pag->filepag);
         strcpy(filename,XlConvPathVms(filename));
      }
      else
         return(False);
   }
   else if(tipo == TIPO_LIBRARY)
   {
      if( XlGetenv(ENVLIB) != NULL )
      {
         strcpy(path,XlGetenv(ENVLIB));
         strcpy(filename,path);
         strcat(filename,"/");
         strcat(filename,pag->filepag);
         strcpy(filename,XlConvPathVms(filename));
      }
      else
         return(False);
   }
   
   get_child(pag->drawing,&children,&num_children);

   pag->num_widget = num_children;

   if(num_children == 0)
   {
   if(modo!=0)
   	PutResPaginaDb(pag,&CompressDb);
   PutResPaginaDb(pag,&pag->db);
   if(modo==1)
 	{
   	XrmPutFileDatabase(CompressDb,XlConvPathVms(filename));
   	XrmDestroyDatabase(CompressDb);
   	CompressDb=NULL;
	}
   else
   	XrmPutFileDatabase(pag->db,XlConvPathVms(filename));
   return(True);
   }


   if(num_children) 
   {
   if((lista = alloca_memoria(num_children*(L_NOME_OGGETTI+L_CLASSE_OGGETTI+5),sizeof(char))) == NULL )
               return(False);
   for(j=0;j<num_children;j++)
         {
         classe = XtClass(children[j]);
         nome_classe = XlGetClassName(classe);
         nome_widget = XtName(children[j]);
    /*
		elimino dagli oggetti le label delle porte
		di interfaccia
    */
         if(strcmp(nome_widget,NOME_LABEL_PORTE_INTERFACCIA)==0)
		{
		++ num_label;
		continue;
		}

         strcat(lista," ");
         strcat(lista,nome_widget);
         strcat(lista," ");
         strcat(lista,nome_classe);
         }
   /*
	setto il campo elenco_wid della struttura pag
   */
   XtFree(pag->elenco_wid);
   pag->elenco_wid=XtNewString(lista);
   }
   else
      {
      XtFree(pag->elenco_wid);
      pag->elenco_wid=XtNewString("");
      }

   /*
	elimino dal totale dei widget le label delle porte di interfaccia 
   */
   num_children -= num_label;
   pag->num_widget = num_children;

  
   /*
	salvo i dati contenuti nella struttura pag nei database
	CompressDb e in pag->db
   */
   PutResPaginaDb(pag,&pag->db);
   if(modo == 1)
	{
   	PutResPaginaDb(pag,&CompressDb);
   	XrmPutFileDatabase(CompressDb,XlConvPathVms(filename));
   	XrmDestroyDatabase(CompressDb);
   	CompressDb=NULL;
	}
   else
	{
   	XrmPutFileDatabase(pag->db,XlConvPathVms(filename));
	}
   
   if(lista != NULL)
      libera_memoria(lista);
   
   if(modo == 1)
   	SetRegoDisplayMode(pag->drawing,dispM,portM,inteM);

   return(True);
}


/*------------------------------------------
 * AddTransCompositeWid
 *
 * definisce le opportune translation ad uno
 * widget di tipo composite e ai suoi figli in
 * modo ricorsivo
 *
 *------------------------------------------*/

void AddTransCompositeWid(Widget wid,char* stringa)
{
   extern void get_child();
   WidgetList children;
   Cardinal nchildren;
   int i;

   add_def_translation(wid,stringa);
   if(XtIsComposite(wid))
	{
   	get_child(wid,&children,&nchildren);
	for(i=0;i<nchildren;i++)
		AddTransCompositeWid(children[i],stringa);
	}
}

/*------------------------------------------
 * AddTransWid
 *
 * definisce le opportune translation ad uno
 * widget posto su una data drawing area 
 *
 *------------------------------------------*/

void AddTransWid(PAGINA *pag,Widget wid)
{
   extern void get_child();
   char stringa[200];
   WidgetList children,nipoti,figli_nipoti;
   Cardinal nchildren,nnipoti,nfigli_nipoti;
   int i,j,k;


   sprintf(stringa,MOVE_WIDGET_TRANS,pag);
   add_def_translation(wid,stringa);

   if( XlIsXlManager(wid) )
   {
      get_child(wid,&children,&nchildren);

/*
      sprintf(stringa,MOVE_WIDGET_TRANS,pag);
*/
      for(i=0;i<nchildren;i++)
      {
         add_def_translation(children[i],stringa);
/* a sua volta puo' essere un XlManager */
         if(XlIsXlManager(children[i]))
            AddTransWid(pag,children[i]);
/* a sua volta puo' essere un XtComposite */
	AddTransCompositeWid(children[i],stringa);
      }
   }
}



/*-------------------------------------
 *
 * AllPagWidSetTransation
 *
 * set delle translation su tutti gli widget della pagina
 *
 *-------------------------------------*/

void AllPagWidSetTranslation(PAGINA *pagina)
{
   extern void get_child();
   extern void set_draw_translations();
   char stringa[200];
   int i,j;
   WidgetList children;
   Cardinal num_children;


   i=0;
   sprintf(stringa,SELECT_ZONE_TRANS,pagina,i);
   add_def_translation(pagina->drawing,stringa);

   get_child(pagina->drawing,&children,&num_children);
   for(i=0;i<num_children;i++)
      AddTransWid(pagina,children[i]);

/* set translation per interfaccia draw */
   set_draw_translations(pagina->drawing);
}

/*
 PaginaSetZoom
 setta la risorsa di zoom su tutti gli oggetti Xl della
 pagina
*/
void PaginaSetZoom(drawing,fatt_zoom)
Widget drawing;
float fatt_zoom;

{
extern void get_child();

WidgetList children;
int i;
int perc_zoom; /* valore in percento dello zoom */
Cardinal num_children;
perc_zoom= 100.0 * fatt_zoom;
get_child(drawing,&children,&num_children);
for(i=0;i<num_children;i++)
      set_something(children[i],XlNfattZoom,(char*) perc_zoom);
}

/*--------------------------------------------------------
 *
 * GetPaginaFromWidget
 *
 * restituisce il puntatore alla pagina conoscendo 
 * la drawing area 
 *
 *--------------------------------------------------------*/

PAGINA *GetPaginaFromWidget(Widget drawing)
{
   extern PAGINA *pagine;
   PAGINA *pag;

   pag = pagine;
   while(pag != NULL)
   {
      if( pag->drawing == drawing)
         return(pag);

      pag = pag->pagsucc;
   }
}


PAGINA *GetPaginaFromName(char *nome_pagina)
{
   extern PAGINA *pagine;
   PAGINA *pappo;

   pappo = pagine;      

   if(pagine != NULL)
   {
      while(pagine != NULL)
      {
         if(!strcmp(pagine->nomepag,nome_pagina))
         {
            pagine = pappo;
            return(pagine);
         }
       
         pagine = pagine->pagsucc;
      }
   }

   return(NULL);
}

/*-----------------------------------
 * RaisePagina
 *
 * effettua il Raise della pagina
 *----------------------------------*/
Boolean RaisePagina(char *nome_pagina)
{
   extern PAGINA *pagine;
   PAGINA *pappo;

   pappo = pagine;      

   if(pagine != NULL)
   {
      while(pagine != NULL)
      {
         if(!strcmp(pagine->nomepag,nome_pagina))
         {
            XRaiseWindow(XtDisplay(pagine->topwidget), XtWindow(pagine->topwidget));
            pagine = pappo;
            return(True);
         }
         pagine = pagine->pagsucc;
      }
   }

   pagine = pappo;
   return(False);
}

/*-------------------------------------
 *
 * PagIsOpen
 *
 * verifica lo stato di una pagina
 * True  = aperta
 * False = chousa
 *
 *-------------------------------------*/
Boolean PagIsOpen(char *nome_pagina)
{
   extern PAGINA *pagine;
   PAGINA *pappo;

   pappo = pagine;      

   if(pagine != NULL)
   {
      while(pagine != NULL)
      {
         if(!strcmp(pagine->nomepag,nome_pagina))
         {
            pagine = pappo;
            return(True);
         }
       
         pagine = pagine->pagsucc;
      }
   }

   pagine = pappo;

   return(False);
}



/*--------------------------------------
 * 
 * do_cancella
 *
 * dato il nome di una pagina 
 * permette di eliminarla    
 *
 *---------------------------------------*/

void do_cancella(char *nome_pag,int tipo)
{
   extern Boolean ContextModified;
   extern PAGEDIT_CONTEXT *pagedit;
   extern void removefromlist();
   XmString pagname;
   FILE *fp;
   char comando[100];
   int pos,i,j;
   char pathname[MAXLENFILENAME];
   char nome_filepag[MAXLENFILENAME];
   char nome_filebkg[MAXLENFILENAME];
   Boolean FilePagExists = True;
   Boolean FileBkgExists = True;
   PAGINA *pag;
   AN_OBJ *obj;
   char nome_file_errori[100];
   char nome_task[50];
   int in_list;
   char file_conn[200]; /* Path name del file Connessioni.reg */
   extern OlConnObject conn_obj; /* oggetto OlConn per la gestione 
				    delle connessioni */
   extern Boolean StateInterfaceMode;
   Boolean PagConnect=False;
   char mesg[200];

   switch(tipo)
   {
      case TIPO_PAGINA:
         if( XlGetenv(ENVPAG) == NULL)
            return;
	/*
		controllo se la pagina, se di tipo regolazione,
		risulta connessa con un'altra pagina
	*/
   	/*
        	creo l'oggetto connessioni
   	*/
#ifndef VMS
   	strcpy(file_conn,XlGetenv(ENVPAG));
   	strcat(file_conn,"/");
#else
   	strcpy(file_conn,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
   	strcat(file_conn,"Connessioni.reg");
   	if(!StateInterfaceMode)
   		conn_obj=OlCreateConn(file_conn);
	/*
		verifico che la pagina non sia connessa
	*/
	PagConnect=OlPagIsConnected(conn_obj,nome_pag);
   	/*
        	distruggo l'oggetto conn
   	*/
   	if(!StateInterfaceMode)
   		OlDestroyConn(conn_obj);
	if(PagConnect)
		{
        	SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",
			"Selected page is connected with an other page",
			NULL,False,NULL,False,NULL);
        	return;
		}

         strcpy(pathname,XlGetenv(ENVPAG));
         strcat(pathname,"/");

         strcpy(nome_filepag,pathname);
         strcat(nome_filepag,nome_pag);
         strcat(nome_filepag,PAG_EXT);
         strcpy(nome_filepag,XlConvPathVms(nome_filepag));

         strcpy(nome_filebkg,pathname);
         strcat(nome_filebkg,nome_pag);
         strcat(nome_filebkg,BKG_EXT); 
         strcpy(nome_filebkg,XlConvPathVms(nome_filebkg));

/* verifico la esistenza del file della pagina */
         if( (fp = fopen(nome_filepag,"r")) == NULL)
         {
            FilePagExists = False;
         }
         else
         {
            fclose(fp);
         }

         if( (fp = fopen(nome_filebkg,"r")) == NULL)
         {
            FileBkgExists = False;
         }
         else
         {
            fclose(fp);
         }
 
/* verifico che la pagina sia chiusa */
         if(PagIsOpen(nome_pag))
            return;


/* rimuovo il file della pagina,
   aggiorno la struttura PAGEDIT_CONTEXT,
   rimuovo l'item reltivo dalla lista,
   delle pagine 
*/ 

         pagina_init(nome_pag,&pag,TIPO_PAGINA);
         pagina_load_file(pag,TIPO_PAGINA);
         pagina_getres(pag, &obj);

         if(PagGetType(pag) == TYPE_REGOLAZIONE)
            {
            estrai_nome(XlGetenv(ENVPAG),nome_task);
#ifndef VMS
            strcpy(nome_file_errori,XlGetenv(ENVPAG));
            strcat(nome_file_errori,"/");
#else
            strcpy(nome_file_errori,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
            strcat(nome_file_errori,nome_task);
            strcat(nome_file_errori,REG_EXT);
            elimina_da_lista(nome_pag,TIPO_PAGINA,nome_file_errori);
            }

         for(i=0;i<pagedit->pag_num;i++)
         {

            if(!strcmp(pagedit->page_list[i],nome_pag))
            {
               if(FilePagExists)
               {
/* versione solo unix 
                  strcpy(comando,"rm  ");
                  strcat(comando,nome_filepag);
                  system(comando);
*/
                  XlRemove(nome_filepag);
               }

               if(FileBkgExists)
               {
/* versione solo unix
                  strcpy(comando,"rm  ");
                  strcat(comando,nome_filebkg);
                  system(comando);
*/
                  XlRemove(nome_filebkg);
               }

/* cancello il file della pagina */

               pos = i+1; 
               pagedit->pag_num--;

               if(pagedit->pag_num == 0)
               {
                  libera_memoria( (char*)pagedit->page_list);
                  pagedit->page_list=NULL;
		  libera_memoria( (char*)pagedit->res_page);
		  pagedit->res_page = NULL;
               }
               else
               {
                  for(j=i;j<pagedit->pag_num;j++) {
                     strcpy(pagedit->page_list[j],pagedit->page_list[j+1]);
		     memcpy(pagedit->res_page[j], pagedit->res_page[j+1],
			    sizeof(INFO_PAGE));
		  }
		  libera_memoria( (char*)pagedit->res_page[pagedit->pag_num]);   
               }
               break;
            }   
         }
         libera_memoria((char*)obj);
         pagina_free(pag);
         removefromlist(pos);

      break; /* end case TIPO_PAGINA  */
      case TIPO_LIBRARY:

         if( XlGetenv(ENVLIB) == NULL)
            return;

         strcpy(pathname,XlGetenv(ENVLIB));
         strcat(pathname,"/");

         strcpy(nome_filepag,pathname);
         strcat(nome_filepag,nome_pag);
         strcat(nome_filepag,LIB_EXT);
         strcpy(nome_filepag,XlConvPathVms(nome_filepag));


/* verifico la esistenza del file della pagina */
         if( (fp = fopen(nome_filepag,"r")) == NULL)
         {
            FilePagExists = False;
         }

         fclose(fp);

/* verifico che la pagina sia chiusa */
         if(PagIsOpen(nome_pag))
            return;


/* rimuovo il file della libreria,
   aggiorno la struttura PAGEDIT_CONTEXT,
   rimuovo l'item reltivo dalla lista,
   delle pagine 
*/ 

         for(i=0;i<pagedit->iconlib_num;i++)
         {
            if(!strcmp(pagedit->iconlib_list[i],nome_pag))
            {
               if(FilePagExists)
               {
/*
                  strcpy(comando,"rm  ");
                  strcat(comando,nome_filepag);
                  system(comando);
*/
                  XlRemove(nome_filepag);
               }

               for(j=i;j<pagedit->iconlib_num;j++)
               {
                  if(j<(pagedit->iconlib_num-1))
                  {
                     strcpy(pagedit->iconlib_list[j],pagedit->iconlib_list[j+1]);
                     strcpy(pagedit->iconlib_label[j],pagedit->iconlib_label[j+1]);
                  }
               } 

/* cancello il file della pagina */

               pos = i+1; 
               pagedit->iconlib_num--;

               if(pagedit->iconlib_num == 0)
               {
                  libera_memoria( (char*)pagedit->iconlib_list);
                  pagedit->iconlib_list=NULL;
               }
               break;
            }   
         }

         removefromlibrarylist(pos);

      break; /* end case TIPO_LIBRARY */
   } /* end switch */
 
   /* aggiungo il salvataggio del file context 
      dopo avere cancellato una pagina o una libreria
   */
   if( new_save_context( pagedit ) != True )
   {
      sprintf(mesg,"Error: Cannot save file Context \n");
      show_message(mesg);
   }
   ContextModified = True;
}

/*----------------------------------------------
 *
 *  cvtPixelFromString
 *
 *  converti la stringa in Pixel Value
 *
 *-----------------------------------------------*/

Boolean cvtPixelFromString(Widget ogget,char *Stringa,Pixel *Pix)
{
        unsigned long piani[1];
        Colormap colmap;
        XColor xcol;


        colmap = DefaultColormap ( XtDisplay(ogget),
                        DefaultScreen (XtDisplay(ogget)) );
        if ( !XParseColor (XtDisplay(ogget) , colmap , Stringa , &xcol) )
           {
           return(False);
           }
        if(!XAllocColor(XtDisplay(ogget),colmap,&xcol))
           {
           return(False);
           }
        *Pix = xcol.pixel;
        return(True);
}

/*---------------------------------------
 * 
 * pagina_setta_resource
 *
 * set della risorse impostate 
 *
 *-----------------------------------------*/

void pagina_setta_resource(PAGINA *pag)
{
   Pixel pix;

   set_something(pag->topwidget,XmNx,(char*) pag->geom.x);
   set_something(pag->topwidget,XmNy,(char*) pag->geom.y);
   set_something(pag->topwidget,XmNwidth,(char*) pag->geom.width);
   set_something(pag->topwidget,XmNheight,(char*) pag->geom.height);
   set_something(pag->drawing,XmNwidth,(char*) pag->geom.draw_width);
   set_something(pag->drawing,XmNheight,(char*) pag->geom.draw_height);
   DrawSetSnap(pag->drawing,pag->snap_pag);

   /*
    esamina se la risorsa colore di sfondo contiene il nome
    di un file .gif
    */
   pag->sfondo=0;
   if(strstr(pag->geom.background,".gif"))
	{
	Pixmap bitmap_norm;
/*
 In pag->geom.background e' contenuto il nome del file .GIF da
 caricare come sfondo del sinottico
 Il file viene ricercato prima nella directory corrente e poi
 nel path precisato da LEGOMMI_ICO
*/
	if(XlCreatePixmapFromGIF(pag->drawing,pag->geom.background,
		&pag->sfondo)==True)
		set_something(pag->drawing,XmNbackgroundPixmap,(void*) pag->sfondo);
	}
   else if( cvtPixelFromString(pag->drawing,pag->geom.background,&pix) )
      set_something(pag->drawing,XmNbackground,(void*) pix);

}



int dbcopy(XrmDatabase *actualDb,XrmDatabase *newDb,Widget drawing)
{
int i,j;
char str_risorsa_sorg[200],str_risorsa_dest[200];
char str_rischild_sorg[200],str_rischild_dest[200];
WidgetList ClipChild,CompChild;
Cardinal nClipChild,nCompChild; 
WidgetList wgtsel;
Cardinal nwgtsel;
extern void get_child();

	
      get_child(drawing,&wgtsel,&nwgtsel);

      for(i=0;i<nwgtsel;i++)
      {
         strcpy(str_risorsa_sorg, XtName(wgtsel[i]) );
         strcat(str_risorsa_sorg,".");

         strcpy(str_risorsa_dest,"*");
         strcat(str_risorsa_dest, XtName(wgtsel[i]) );
         strcat(str_risorsa_dest,".");
 
         XlTransferResource( actualDb,newDb,
		wgtsel[i],str_risorsa_sorg,str_risorsa_dest );

	/* devo trattare anche i children dei composite */
         if( XlIsXlComposite(wgtsel[i]) || XlIsIconReg(wgtsel[i]) )
         {

            get_child(wgtsel[i],&CompChild,&nCompChild);

            for(j=0;j<nCompChild;j++)
            {
               strcpy(str_rischild_sorg, XtName(CompChild[j]) );
               strcat(str_rischild_sorg,".");

               strcpy(str_rischild_dest,"*");
               strcat(str_rischild_dest, XtName(CompChild[j]) );
               strcat(str_rischild_dest,".");

               XlTransferResource( actualDb,newDb,CompChild[j],
			str_rischild_sorg,str_rischild_dest );
            }
         } 

       }
}

extern Widget ClipBoardForCopy;
int fcopy(PAGINA *pag)
{
   extern int get_selected();
   extern void get_child();
   extern XrmDatabase CuttedDb;
   WidgetList wgtsel,ClipChild,CompChild;
   Cardinal nwgtsel,nClipChild,nCompChild; 
   int i,j;
   char str_risorsa_sorg[200],str_risorsa_dest[200];
   char str_rischild_sorg[200],str_rischild_dest[200];
   FILE *fp;
   /* reset CuttedDb e ClipBoard */
   if(CuttedDb != NULL) 
   {
      get_child(ClipBoardForCopy,&ClipChild,&nClipChild);
      for(i=0;i<nClipChild;i++)
      {
         XtDestroyWidget(ClipChild[i]);
      }

      XrmDestroyDatabase(CuttedDb);
      CuttedDb = NULL;
   }

   /* get selected from pagina */
   if( get_selected(pag,&wgtsel,&nwgtsel) == True)
   {

      /* for all selected, Transfert Resource from pag to ClipBoard */
      for(i=0;i<nwgtsel;i++)
      {
         strcpy(str_risorsa_sorg, XtName(wgtsel[i]) );
         strcat(str_risorsa_sorg,".");

         strcpy(str_risorsa_dest,"*");
         strcat(str_risorsa_dest, XtName(wgtsel[i]) );
         strcat(str_risorsa_dest,".");
 
         XlTransferResource( &pag->db,&CuttedDb,wgtsel[i],str_risorsa_sorg,str_risorsa_dest );

	/* devo trattare anche i children dei composite */
         if( XlIsXlComposite(wgtsel[i]) || XlIsIconReg(wgtsel[i]) )
         {

         get_child(wgtsel[i],&CompChild,&nCompChild);

            for(j=0;j<nCompChild;j++)
            {
               strcpy(str_rischild_sorg, XtName(CompChild[j]) );
               strcat(str_rischild_sorg,".");

               strcpy(str_rischild_dest,"*");
               strcat(str_rischild_dest, XtName(CompChild[j]) );
               strcat(str_rischild_dest,".");

               XlTransferResource( &pag->db,&CuttedDb,CompChild[j],str_rischild_sorg,str_rischild_dest );
            }
         } 

         XrmPutFileDatabase(CuttedDb,"ClipDb.pag");
         UxDisplay->db = CuttedDb;
         XtCreateManagedWidget ( XtName(wgtsel[i]),XtClass(wgtsel[i]),ClipBoardForCopy,NULL,0);
         UxDisplay->db = pag->db;

      }
   if(nwgtsel)
	libera_memoria((char*)wgtsel);
   }
}

/*--- seguono la definizione fdi una struttura e
      una serie di piccole routine utilizzate
      nel paste
 ----*/

typedef struct {
                  char WidOldName[20];
                  char WidNewName[20];
                  int  delta;
               } FROMCLIP;

void get_idwid(char *nome,char *id)
{
   char *appo;

   strcpy(id,nome);
   appo = strchr(id,'w');
   *appo = 0;
}

void get_idchild(char *nome,char *id)
{
   char *appo;

   strcpy(id,nome);
   appo = strchr(id,'w');
   appo++;
   strcpy(id,appo);
   id[strlen(id)-1]=0;
}

void get_nomewid(char *nome,char *id)
{
   char *appo;

   strcpy(id,nome);
   appo = strchr(id,'w');
   appo++;
   *appo = 0;
}

void get_nomechild(char *nome,char *id)
{
   char *appo;

   strcpy(id,nome);
   appo = strchr(id,'w');
   appo++;
   strcpy(id,appo);
}
 
/*-----------------------------------------------------
 *
 * fpaste - prende gli widget presenti nella clipboard
 *          e li trasferisce e ricrea nella pagina
 *          Questa versione tiene conto anche delle 
 *          eventuali connessioni tra gli widget in 
 *          clipboard
 *------------------------------------------------------*/

int fpaste(PAGINA *pag)
{
   extern void PagSetTagReg();
   extern void get_child();
   extern XrmDatabase CuttedDb;
   WidgetList ClipChild,CompChild,newChild;
   Cardinal nClipChild,nCompChild,nnewChild;
   Widget new_wid;
   int i,j,kk,h,n,num_figli=0,TipoIco;
   char str_risorsa_sorg[200],str_risorsa_dest[200],new_name[200],new_child_name[200];
   char str_rischild_sorg[200],str_rischild_dest[200];
   char *lista_figli=NULL,nome_classe[100],appo[10],nome_figlio[100];
   char WidNameVecchio[20],WidNameNuovo[20];
   char ChildNameVecchio[20],ChildNameNuovo[20];
   char *PortConnectWith;
   char ParentPortName[20];
   Boolean Test;
   FROMCLIP *aggiorna;
   int nporteinmodifica=0;
   char appo1[20];
   char appo2[20];
   char reg_name[20],port_name[20];
   int deltachild;
   float f_zoom;
   int perc_zoom;
   extern float get_def_zoom();


   /* get children della ClipBoard */
   UxDisplay->db = CuttedDb;
   get_child(ClipBoardForCopy,&ClipChild,&nClipChild);
   UxDisplay->db = pag->db;

   f_zoom= get_def_zoom(pag->drawing);
   perc_zoom = f_zoom * 100.0;



   /* per tutti i children children della 
      clipboard faccio la  XlTransferResource */
   if(nClipChild)
   {

/* alloco record sufficienti per verificare la situazione 
   del cut & paste delle regolazioni 
*/
      aggiorna = XtMalloc( (Cardinal) (sizeof(FROMCLIP) * nClipChild) );

      desel_all(pag);

      for(i=0;i<nClipChild;i++)
      {

         /* recupero il nome dello Widget in ClipBoard */
         strcpy(aggiorna[i].WidOldName,XtName(ClipChild[i]) );

         strcpy(str_risorsa_sorg, XtName(ClipChild[i]) );
         strcat(str_risorsa_sorg,".");

         /* assegno un nuovo nome al nuovo widget */
         assign_name(pag,SUDRAWING,pag->drawing,new_name);
         strcpy(str_risorsa_dest,"*");
         strcat(str_risorsa_dest, new_name );
         strcat(str_risorsa_dest,".");

         strcpy(aggiorna[i].WidNewName,new_name);

         /* recupero l'id degli widget dal nome */
         get_idwid(aggiorna[i].WidOldName,appo1);
         get_idwid(aggiorna[i].WidNewName,appo2);
         

         /* calcolo il delta tra il vecchio indice e il nuovo */
         aggiorna[i].delta = abs(atoi(appo1) - atoi(appo2));
	/* esamina quante sono le porte della icon reg in esame */
         get_child(ClipChild[i],&CompChild,&nCompChild);
         for(kk=0; kk< nCompChild; kk++)
		{
	 	if( XlIsPort(CompChild[kk]) )
           		 nporteinmodifica++;
		}

         XlTransferResource( &CuttedDb, &pag->db, ClipChild[i],str_risorsa_sorg,str_risorsa_dest );

         XlSetResourceByName(&pag->db,new_name,XlNnome,new_name);

         /* devo trasferire le risorse dei figli e aggiornare la lista dei children del XlComposite */
         if( XlIsXlComposite(ClipChild[i]) || XlIsIconReg(ClipChild[i]) )
         {

            /* inizializzazione */
            lista_figli = NULL;
            num_figli = 0;


            UxDisplay->db = CuttedDb;
            get_child(ClipChild[i],&CompChild,&nCompChild);

            /******* Gestione Verifica Connessione ****/

            for(j=0;j<nCompChild;j++)
            {
                  if(XlIsPort(CompChild[j]))
                  {
                     /* adesso verifico che, se uno widget nella clipboard
                        e' connesso, sia presente in clipboard anche lo widget
                        a cui e' connesso */
                     if(XlPortIsConnected(CompChild[j]))
                     {
                         get_something(CompChild[j],XlNportNameConnect, (void*) &PortConnectWith);
                         /* ho trovato una porta connessa 
                            adesso devo verificare se esiste nella ClipBoard
                            la porta a cui e' agganciata (connessa)
                            per fare cio' e' sufficiente verificare che
                            esiste lo widget parent della porta agganciata */

                         /* recupero il nome e.s. 13w */
                         get_nomewid(PortConnectWith,ParentPortName);

                         Test = False;
                         for(h=0;h<nClipChild;h++)
                         {
                            /* e verifico se nella clipBoard c'e lo widget con quel nome */
                            if( !strcmp( XtName(ClipChild[h]), ParentPortName) )
                            {
                               Test = True;
                            }
                         }
                         if(!Test)
                         {
			/*
				La porta di Connessione non e' in ClipBoard
			*/
                            XlSetResourceByWidget(&CuttedDb,CompChild[j],XlNportNameConnect,"");
			set_something(CompChild[j],XlNportNameConnect,(void*) "");
                         }
                          
                     }
		  }
            }


            UxDisplay->db = pag->db;

            if(nCompChild > 0)
            {
               /* devo resettare la listaChildren del XlComposite perche' devo cambiare nome 
                  ai suoi children il numero dei children invece resta inalterato */
            

               /* genero i figli ed aggiorno la lista dei figli */
               for(j=0;j<nCompChild;j++)
               {
                  strcpy(ChildNameVecchio,XtName(CompChild[j]));

                  /* assegno il nome corretto al figlio */
 
                  strcpy(new_child_name,new_name);
                  sprintf(nome_figlio,"%d",pag->next_num);
                  strcat(new_child_name,nome_figlio);
                  strcat(new_child_name,"c");
                  pag->next_num++;

                  strcpy(ChildNameNuovo,new_child_name);


                  strcpy(nome_classe , XlGetClassName(XtClass(CompChild[j])) );

                  /* alloco spazione per la lista dei figli */
                  if( j==0 )
                  {
                     if( (lista_figli = alloca_memoria((strlen(new_child_name)+strlen(nome_classe)+8 ),sizeof(char))) == NULL)
                        return(False);  
                  }
                  else
                  {
                     if( (lista_figli = realloc( lista_figli,sizeof(char)*(strlen(lista_figli)+strlen(new_child_name)+strlen(nome_classe)+8))) == NULL)
                        return(False);
                  }

                  if(j == 0)
                     strcpy(lista_figli,new_child_name);
                  else
                  {
                     strcat(lista_figli," ");
                     strcat(lista_figli,new_child_name);
                  }

                  strcat(lista_figli," ");
                  strcat(lista_figli,nome_classe);

                  strcpy(str_rischild_sorg, XtName(CompChild[j]) );
                  strcat(str_rischild_sorg,".");

                  strcpy(str_rischild_dest,"*");
                  strcat(str_rischild_dest,new_child_name );
                  strcat(str_rischild_dest,".");

                  
                  /* trasferisco le risorse dei figli */
                  XlTransferResource( &CuttedDb, &pag->db, CompChild[j],str_rischild_sorg,str_rischild_dest );
                  XlSetResourceByName(&pag->db,new_child_name,XlNnome,new_child_name);

                  num_figli++;
         
               }
                
               sprintf(appo,"%d",num_figli);
                
               XlSetResourceByName(&pag->db,new_name,"numFigli",appo);
               XlSetResourceByName(&pag->db,new_name,"listChildren",lista_figli);
            }
         
         }

         UxDisplay->db = pag->db;
         new_wid = XtCreateManagedWidget( new_name,XtClass(ClipChild[i]),pag->drawing,NULL,0);
         AddTransWid(pag,new_wid);
         set_something(new_wid,XlNnome,(void*) new_name);
         set_something(new_wid,XlNselected,(void*) True);

	set_something(new_wid,XlNfattZoom,(void*) perc_zoom);

         if(XlIsIconReg(new_wid))
         {
/*
            printf("Ho istanziato e creato una ICONREG \n");
*/
            get_something(new_wid,XlNiconRegType, (void*) &TipoIco);
            if(TipoIco == NORMAL_ICONREG)
            {
               PagSetTagReg(pag,new_wid);
            }
         }
         

         pag->num_widget++;
         UxDisplay->db = CuttedDb;
      } 
   }
   
   XrmPutFileDatabase(CuttedDb,"PClipDb.pag");
   UxDisplay->db = pag->db;


/* cerco i nuovi Widget nella drawing area */
   get_child(pag->drawing,&newChild,&nnewChild);

/* per ogni iconreg */
   for(i=0;i<nnewChild;i++)
   {

/* scorro la lista vecchio-nuovo per vedere se questo widget e' quello 
   di cui ho fatto il paste */
       for(j=0;j<nClipChild;j++)
       {
/* se trovo la iconreg dentro la lista vecchio-nuovo */
          if( !strcmp(XtName(newChild[i]) , aggiorna[j].WidNewName ) )
          {
/* prendo le porte della iconreg trovata */
               get_child(newChild[i],&CompChild,&nCompChild);

/* e cerco le porte connesse */ 
               for(h=0;h<nCompChild;h++)
               {
                  if( XlIsPort(CompChild[h]) && XlPortIsConnected(CompChild[h]) )
                  {
		/*
			trovate porte connesse
		*/
/* la porta connessa avra' come riferimento di connessione il vecchio nome della porta */
                     get_something(CompChild[h],XlNportNameConnect, (void*) &PortConnectWith); 

/* ricavo il vecchio nome della porta connessa */
                     get_nomewid(PortConnectWith,appo1);
 
/* e cerco nella lista nuovo-vecchio il vecchio nome per ricavarne il nuovo */
                     for(n=0; n<nClipChild; n++)
                     {
                        if(!strcmp(appo1,aggiorna[n].WidOldName) )
                        {
                        /*   
                          printf("\tPaste: ho trovato oldname = %s newname = %s\n\n",aggiorna[n].WidOldName,aggiorna[n].WidNewName);
                         */
/* adesso devo generare il nuovo nome della porta connessa  e sostituirlo nella nuova iconreg */

/* calcolo adesso il numero della porta da connettere il quale viene ricavato
   aggiungendo all'id iconreg la differenza tra il numero di porta e l'id iconreg 
   della vecchia connessione .
   esempio vecchia connessione 0w4c adesso l'id iconreg e' 16w percui a
   0w devo sostituire 16w e a 4c devo sostituire 16+(4-0)=20c
*/
/* in appo1 va l'id della iconreg es. 0 */
                          get_idwid(PortConnectWith,appo1); 

/* in appo1 va l'id della porta es. 4 */
                          get_idchild(PortConnectWith,appo2);
                          
/* in deltachild va il delta tra id della iconreg e id della porta */  
                          deltachild = atoi(appo2) - atoi(appo1); 
                         
/* adesso costruisco il nome della nuova connessione */
 
                          sprintf(appo1,"%s%dc",aggiorna[n].WidNewName,atoi(aggiorna[n].WidNewName)+deltachild); 
                                                         
                          set_something(CompChild[h],XlNportNameConnect, (void*) appo1); 
                          XlSetResourceByWidget(&pag->db,CompChild[h],XlNportNameConnect,appo1);
                        }
                        
                     }
                  }
               }
           }
       }
   }
if(nClipChild)
	{
	XtFree(aggiorna);
	}
/*
 Se la pagina e' di regolazione ed e' in connect mode
 setta la modalita' di connect mode anche per i nuovi
 oggetti inseriti sulla pagina con il paste
*/
if(PagGetType(pag) == TYPE_REGOLAZIONE)
   if( PagGetConnectMode(pag->drawing) == True )
       set_connect(pag->drawing,True);
}

/*----------------------------------------------
 * fcut
 *
 * copy in clipboard dei widget selezionati  
 * la cancellazione degli widget
 * avviene dal chiamante
 *
 *----------------------------------------------*/
void fcut(PAGINA *pag)
{
   extern int get_selected();
   extern int delete_widget();
   WidgetList wgtsel;
   Cardinal numsel,i;


   get_selected(pag,&wgtsel,&numsel);

/* verifico che gli widget non siano in edit */
   if(WidSelectedAreInEdit(pag))
   {
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget is In Edit",NULL,False,NULL,False,NULL);
      return;
   }

/* cut del foreground (widget) */
   fcopy(pag);

}


int gestMessageBox(char *message)
{

}

void PagSetDefaultValue(PAGINA *pagina)
{
    pagina->geom.x = XDEF;
    pagina->geom.y = YDEF;
    pagina->geom.width = MIN_WIDTH*3;
    pagina->geom.height = MIN_HEIGHT*3;
    pagina->geom.draw_width =   MIN_WIDTH*3;
    pagina->geom.draw_height =  MIN_HEIGHT*3;
    strcpy(pagina->geom.background,BACKGROUND_DEF);
    pagina->in_use =  SCHEME_NOT_IN_USE;
}

#ifdef NUOVA_VERS
/**** per future implementazioni ***/
int PagGetTypeNew(PAGINA *pag)
{
   return(pag->geom.tipo_pag);
}
#endif

void PaginaSetTagPag(PAGEDIT_CONTEXT *ctx,PAGINA *pag)
{
   strcpy(pag->tagPag,ctx->nextTagPag);
}


/*
 Setta il tag della icona di regolazione secondo lo schema
 <progressivo dell'icona di regolazione><progressivo schema>
 Incrementa il progressivo dell'icona
*/
void PagSetTagReg(PAGINA *pag,Widget wid)
{
   char tagReg[(DIMTAG*2)+1];
   char StrRes[100];
   
/* setto la tag del regolatore allo widget tipo iconreg */
   strcpy(tagReg,pag->nextTagRegol);
   strcat(tagReg,pag->tagPag);
   lnargs=0;
   XtSetArg(args[lnargs],XlNtagName,tagReg);lnargs++;
   XtSetValues(wid,args,lnargs);
   sprintf(StrRes,"*%s.%s",XtName(wid),XlNtagName);
   XrmPutStringResource(&pag->db,StrRes,tagReg);

/* assegno e ricalcolo la prossima tag di regolazione */
   TagPagCalcNext(pag->nextTagRegol,pag->nextTagRegol);
   pag->numTagReg++;
}


void PostOpenPage(Widget DrawingArea)
{
   extern void get_child();
   WidgetList Wlist,Wlistf;
   Cardinal nwid,nwidf;
   PAGINA *pagMom;
   int i,j;
   Boolean icone = False;
   Arg arg[2];
   int tipo_porta,num_conn;
   int tipoPag;
   char app_char2[100];
   char *app_char1;
   CONNESSIONE *connessione;
   char file_conn[200]; /* Path name del file Connessioni.reg */
   extern OlConnObject conn_obj; /* oggetto OlConn per la gestione 
				    delle connessioni */
   extern Boolean StateInterfaceMode;
   extern void PostOpenPage2();

   /*
	se la pagina e' di libreria ritorna
   */
   pagMom = GetPaginaFromWidget(DrawingArea);
   tipoPag = PagGetType(pagMom);
   if(tipoPag != TYPE_REGOLAZIONE )
	return;
   
PostOpenPage2(DrawingArea,pagMom->nomepag,pagMom->db);
   
#if defined DA_TOGLIERE_20_04_95

   get_child(DrawingArea,&Wlist,&nwid);
   for(i=0;i<nwid;i++)
      if(XlIsInterfaceIconReg(Wlist[i]))
      {
         icone=True;
         break;
      }

   if(!icone)
      return;
   /*
        sono state trovate delle icone di regolazione di interfaccia
   */
   /*
        creo l'oggetto con
   */
#ifndef VMS
   strcpy(file_conn,XlGetenv(ENVPAG));
   strcat(file_conn,"/");
#else
   strcpy(file_conn,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
   strcat(file_conn,"Connessioni.reg");
   if(!StateInterfaceMode)
   	conn_obj=OlCreateConn(file_conn);
   /*
        verifico le eventuali connessioni di interfaccia
   */
   for(i=0;i<nwid;i++)
      if(XlIsInterfaceIconReg(Wlist[i]))
      {
      /*
        cerco i figli dell'icona
      */
      get_child(Wlist[i],&Wlistf,&nwidf);
      for(j=0;j<nwidf;j++)
        if(XlIsInterfacePort(Wlistf[j]))
           {
           /* verifico la porta di interfaccia */
           XtSetArg(arg[0],XlNportNameConnect,&app_char1);
           XtSetArg(arg[1],XlNtipoPort,&tipo_porta);
           XtGetValues(Wlistf[j],arg,2);
           pagMom = GetPaginaFromWidget(DrawingArea);
           if((num_conn=OlFindConnectionByPort(conn_obj,XtName(Wlistf[j]),
                         XtName(Wlist[i]),pagMom->nomepag))<0)
		{
		/*
		   la connessione non e' stata trovata
		   nel file, quindi sconnetto la porta
		*/
		XtSetArg(arg[0],XlNportNameConnect,"");
           	XtSetValues(Wlistf[j],arg,1);
		XlSetResourceByWidget(&(pagMom->db),(Widget)Wlistf[j],
			XlNportNameConnect,"");
		}
	   else
		{
		/*
			la connessione e' stata trovata e quindi
			la setto nella porta
		*/
		connessione = OlGetConn(conn_obj,num_conn);
                if(connessione != NULL)
		  {
		  if((tipo_porta == INPUT_PORT_A)||(tipo_porta == INPUT_PORT_D))
			  sprintf(app_char2,"%s %s %s %s",
				  connessione->u_nome_porta_int,
				  connessione->u_nome_pagina,
				  connessione->i_nome_porta_int,
                                  connessione->i_nome_pagina);
		  else
			  sprintf(app_char2,"%s %s %s %s",
				  connessione->i_nome_porta_int,
				  connessione->i_nome_pagina,
				  connessione->u_nome_porta_int,
                                  connessione->u_nome_pagina);
		  XtSetArg(arg[0],XlNportNameConnect,app_char2);
           	  XtSetValues(Wlistf[j],arg,1);
		  XlSetResourceByWidget(&(pagMom->db),(Widget)Wlistf[j],
			XlNportNameConnect,app_char2);
		  }
		}
           }
      }
   /*
        distruggo l'oggetto conn
   */
   if(!StateInterfaceMode)
   	OlDestroyConn(conn_obj);
#endif

}

void PostOpenPage2(Widget DrawingArea, char * nomePagina, XrmDatabase db)
{
   extern void get_child();
   WidgetList Wlist,Wlistf;
   Cardinal nwid,nwidf;
   PAGINA *pagMom;
   int i,j;
   Boolean icone = False;
   Arg arg[2];
   int tipo_porta,num_conn;
   int tipoPag;
   char app_char2[100];
   char *app_char1;
   CONNESSIONE *connessione;
   char file_conn[200]; /* Path name del file Connessioni.reg */
   extern OlConnObject conn_obj; /* oggetto OlConn per la gestione 
				    delle connessioni */
   extern Boolean StateInterfaceMode;

   get_child(DrawingArea,&Wlist,&nwid);
   for(i=0;i<nwid;i++)
      if(XlIsInterfaceIconReg(Wlist[i]))
      {
         icone=True;
         break;
      }

   if(!icone)
      return;
   /*
        sono state trovate delle icone di regolazione di interfaccia
   */
   /*
        creo l'oggetto con
   */
#ifndef VMS
   strcpy(file_conn,XlGetenv(ENVPAG));
   strcat(file_conn,"/");
#else
   strcpy(file_conn,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
   strcat(file_conn,"Connessioni.reg");
   if(!StateInterfaceMode)
   	conn_obj=OlCreateConn(file_conn);
   /*
        verifico le eventuali connessioni di interfaccia
   */
   for(i=0;i<nwid;i++)
      if(XlIsInterfaceIconReg(Wlist[i]))
      {
      /*
        cerco i figli dell'icona
      */
      get_child(Wlist[i],&Wlistf,&nwidf);
      for(j=0;j<nwidf;j++)
        if(XlIsInterfacePort(Wlistf[j]))
           {
           /* verifico la porta di interfaccia */
           XtSetArg(arg[0],XlNportNameConnect,&app_char1);
           XtSetArg(arg[1],XlNtipoPort,&tipo_porta);
           XtGetValues(Wlistf[j],arg,2);
           if((num_conn=OlFindConnectionByPort(conn_obj,XtName(Wlistf[j]),
                         XtName(Wlist[i]),nomePagina))<0)
		{
		/*
		   la connessione non e' stata trovata
		   nel file, quindi sconnetto la porta
		*/
		XtSetArg(arg[0],XlNportNameConnect,"");
           	XtSetValues(Wlistf[j],arg,1);
		XlSetResourceByWidget(&db,(Widget)Wlistf[j],
			XlNportNameConnect,"");
		}
	   else
		{
		/*
			la connessione e' stata trovata e quindi
			la setto nella porta
		*/
		connessione = OlGetConn(conn_obj,num_conn);
                if(connessione != NULL)
		  {
		  if((tipo_porta == INPUT_PORT_A)||(tipo_porta == INPUT_PORT_D))
			  sprintf(app_char2,"%s %s %s %s",
				  connessione->u_nome_porta_int,
				  connessione->u_nome_pagina,
				  connessione->i_nome_porta_int,
                                  connessione->i_nome_pagina);
		  else
			  sprintf(app_char2,"%s %s %s %s",
				  connessione->i_nome_porta_int,
				  connessione->i_nome_pagina,
				  connessione->u_nome_porta_int,
                                  connessione->u_nome_pagina);
		  XtSetArg(arg[0],XlNportNameConnect,app_char2);
           	  XtSetValues(Wlistf[j],arg,1);
		  XlSetResourceByWidget(&db,(Widget)Wlistf[j],
			XlNportNameConnect,app_char2);
		  }
		}
           }
      }
   /*
        distruggo l'oggetto conn
   */
   if(!StateInterfaceMode)
   	OlDestroyConn(conn_obj);

}

void DisplayResPagina(PAGINA *pag)
{
int num_res=XtNumber(confinfo_pag);
char *app_char;
Dimension app_dim;
Position app_pos;
int app_int;
int i;

if(pag==NULL)
	return;

for(i=0;i<num_res;i++)
	{
	switch(confinfo_pag[i].confinfo_type){
	    case XlRTextAlloc:
		{
		app_char=(char*)pag + confinfo_pag[i].compiled_offset;
		break;
		}
	    case XlRText:
		{
		memcpy(&app_char,
			(char*)pag + confinfo_pag[i].compiled_offset,
			confinfo_pag[i].compiled_size);
		break;
		}
	    case XlRInt:
		{
		app_char = (char*)pag + confinfo_pag[i].compiled_offset;
		memcpy(&app_int,app_char,confinfo_pag[i].compiled_size);
		break;
		}
	    case XlRPosition:
		{
		app_char = (char*)pag + confinfo_pag[i].compiled_offset;
		memcpy(&app_pos,app_char,confinfo_pag[i].compiled_size);
		break;
		}
	    case XlRDimension:
		{
		app_char = (char*)pag + confinfo_pag[i].compiled_offset;
		memcpy(&app_dim,app_char,confinfo_pag[i].compiled_size);
		break;
		}
	   } /* switch */
	}  /* for */
}

/*
 Ricerca la pagina tra tutte le pagine descritte nel
 Context.ctx e caricate gia' in memoria
*/
INFO_PAGE* GetInfoPaginaFromName(char *nome_pag)
{
int i;
extern PAGEDIT_CONTEXT *pagedit;
for(i=0;i<pagedit->pag_num;i++)
	{
        if(!strcmp(pagedit->page_list[i],nome_pag))
            {
	    return(pagedit->res_page[i]);
	    }
	}
return(NULL);
}

Boolean GetInUseFromName(char *nome_pag)
{
Boolean ret;
INFO_PAGE *info_pag;
/*
 Ricerca la pagina tra tutte le pagine descritte nel
 Context.ctx e caricate gia' in memoria
*/
info_pag=GetInfoPaginaFromName(nome_pag);
if(info_pag == NULL)
	{
	printf("\n Pagina non trovata in lista");
	return(False);
	}

if(info_pag->in_use ==1)
   	return(True);
else
   	return(False);
}

/*
 Estrae il tipo di una pagina cercandolo
 nella lista delle INFO relative alle
 pagine.
*/
char *GetInfoTypeFromName(char *nome_pag)
{
INFO_PAGE *info_pag;
info_pag=GetInfoPaginaFromName(nome_pag);
if(info_pag == NULL)
	{
	printf("\n Pagina non trovata in lista");
	return(False);
	}
return(info_pag->type_page);
}



/*
 Estrae il tipo di una pagina
 (utilizzabile anche per pagine
 ancora da importare)
*/
char * GetTypeFromName(char *pag)
{
/*
   extern void show_message();
*/
   char filename[MAXLENFILENAME];
   char mesg[200];
   XrmDatabase db; /* data base di appoggio */
   Boolean retval;
   char *tipo=NULL;
   XrmValue value;
   static char ret[30];

   if( XlGetenv(ENVPAG) != NULL )
      {
         strcpy(filename,XlGetenv(ENVPAG));
         strcat(filename,"/");
         strcat(filename,pag);
         strcat(filename,".pag");
         strcpy(filename,XlConvPathVms(filename));
      }
      else
         return(NULL);

/* load delle risorse */
   strcpy(filename,XlConvPathVms(filename));
   if( (db = XrmGetFileDatabase(filename)) == NULL )
   {
      sprintf(mesg,"Warning XrmGetFileDatabase Failed on page %s \n",filename);
      show_message(mesg);
      return(NULL);
   }

/*
        prelevo la risorsa
*/
  retval = XrmGetResource(db, XlNtipoPag, XlCTipoPag,&tipo, &value);
  if(!retval)
        {
        XrmDestroyDatabase(db);
        return(TIPO_SINOTTICO);
        }

  strcpy(ret,value.addr);

  XrmDestroyDatabase(db);

  return(ret);

}

/*
	a partire dal nome della pagina sistema le tag di regolazione.
	se tagIn == NULL viene utilizzata la risorsa tagPag della pagina
	altrimenti viene utilizzata tagIn che diventa anche la nuova tagPag
*/

Boolean SettaTagInPag(char * nomefile, char * tagIn)
{
Widget CompileBoard;
extern XrmDatabase defdb;
PAGINA *pag=NULL;
AN_OBJ *lista_oggetti=NULL;
WidgetList lista;
Widget wid;
int j;
char *tag;
char apptag[10];

defdb = UxDisplay->db;

CompileBoard = (Widget)create_ClipBoard();

if( pagina_init( nomefile, &pag,TIPO_PAGINA) == False)
           return(False);

if( pagina_load_file(pag,TIPO_PAGINA)  && pagina_getres(pag,&lista_oggetti) )
        {
	if(tagIn != NULL)
		{
		strcpy(pag->tagPag,tagIn);
		XrmPutStringResource(&pag->db,"*tagPag",tagIn);
		}
        UxDisplay->db = pag->db;
        pag->drawing = CompileBoard;
	if(pag->num_widget > 0)
            {

            for(j=0;j<pag->num_widget;j++)
		{
                wid = XtCreateWidget(lista_oggetti[j].nome,
                                        lista_oggetti[j].classe,
                                        CompileBoard,NULL,0);
		if(XlIsIconReg(wid) && !XlIsInterfaceIconReg(wid))
			{
			get_something(wid,XlNtagName, (void*) &tag);
			strcpy(apptag,tag);
			strcpy(&apptag[2],pag->tagPag);
			set_something(wid,XlNtagName,(void*) apptag);
			XlSetResourceByWidget(&(pag->db),wid,XlNtagName,apptag);
			}
		}
	    }
	}
 pagina_save(pag,TIPO_PAGINA,0);
 UxDestroyInterface(CompileBoard);
 pagina_free(pag);


UxDisplay->db = defdb;
}

void EliminaDraget(PAGINA *pag)
{
XdListaUndo  l_undo;
XdLista      ListaDraget;
XdLista      ListaDragetUndo;
extern void proc_DeleteConn();
/*
        elimina i draget
*/
   XdSetDeleteConn((caddr_t)NULL);
   DrawDeleteAll(pag->drawing,NULL);
   XdSetDeleteConn((caddr_t)proc_DeleteConn);
   ListaDraget=get_lista(pag->drawing);
   if(ListaDraget!=NULL)
        XdDestroyLista(ListaDraget);

   ListaDragetUndo=get_lista_undo(pag->drawing);
   if(ListaDragetUndo!=NULL)
        XtFree(ListaDragetUndo);

   l_undo=get_lista_liste_undo(pag->drawing);
   if(l_undo!=NULL)
      undoListDeleteAll(l_undo);

}
