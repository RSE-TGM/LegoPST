/**********************************************************************
*
*       C Source:               comp_pag_util.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed May 19 10:07:43 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: comp_pag_util.c-8 %  (%full_filespec: comp_pag_util.c-8:csrc:1 %)";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

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

#include "config.h"

#define DEFAULT_REFRESHFREQ 10
extern int path_rel_to_abs(char *,char *);
/*
   Variabili globali.
*/
//Guag2024
//char workfile[200];
//Arg      args[10];
//Cardinal lnargs;


/*-------------------------------------------------------------------------
 *
 * load_nomifile
 *
 * inserisce nella struttura PAGINA i nomi della pagina e dei
 * file ad essa associati  (files <nome_pagina>.pag
 * e <nome_pagina>.bkg contenenti rispettivamente le
 * risorse degli oggetti ed il disegno di background)
 *
 *-----------------------------------------------------------------------*/
void load_nomifile(char *nome_pagina,PAGINA *pag,int tipo)
{
   strcpy(pag->nomepag,nome_pagina);

   if(tipo == TIPO_PAGINA )
   {
      strcpy(pag->filepag,nome_pagina);
      strcat(pag->filepag,PAG_EXT);

      strcpy(pag->filebkg,nome_pagina);
      strcat(pag->filebkg,BKG_EXT);
   }
   else if(tipo ==  TIPO_LIBRARY)
   {
      strcpy(pag->filepag,nome_pagina);
      strcat(pag->filepag,LIB_EXT);
   }
}



/*-------------------------------------------------------------------
 *
 *   alloca_memoria
 *
 *   Input:
 *           int num_elem;    numero elementi da allocare;
 *           int dim_elem;    dimensione singolo elemento;
 *
 *   Return:
 *           char *;          puntatore al primo elemento allocato
 *
 *   Descrizione:
 *                  questa routine alloca  num_elem di dimensione
 *                  dim_elem e inizializza tutto a zero
 *                  e ritorna il puntatore al primo elemento
 *                  Se num_elem == 0 o non riesce ad allocare
 *                  la memoria ritorna NULL.
 *
 *---------------------------------------------------------------------*/
char *alloca_memoria(int num_elem, int dim_elem)
{
   char *punt=NULL;

   if( (dim_elem <= 0) || (num_elem <=0) )
   {
/*
      SetMsg(NULL,MEMERR,ERRMSG,"ERRORE MEMORIA","alloca_memoria parametri errat
i",NULL,False,NULL,False,NULL);
*/
      return(NULL);
   }
   if( (punt = calloc((size_t)num_elem, (size_t)dim_elem)) == NULL )
      printf("ERRORE MEMORIA!!! alloca_memoria errore allocazione\n");
/*
      SetMsg(NULL,MEMERR,ERRMSG,"ERRORE MEMORIA","alloca_memoria errore allocazi
one",NULL,False,NULL,False,NULL);
*/
   return(punt);

}




/*--------------------------------------------------------------------
 *
 *   libera_memoria
 *
 *   Input:
 *           char *punt;         puntatore all'area da liberare
 *
 *   Descrizione:
 *
 *
 *-------------------------------------------------------------------*/

int libera_memoria(char *punt)
{
   if(punt != NULL)
   {
      free(punt);
      return(True);
   }
   return(False);
}




/*-------------------------------------------------------------------------
 * pagina_init()
 *
 * allocazione ed inizializzazione struttura dati PAGINA
 * con i nomi dei file che la compongono.
 * N.B. la struttura PAGINA viene allocata internamente
 *      e deve essere liberata successivamente con la chiamata
 *      libera_memoria.
 *
 * input:
 *       char *nomepag;    nome della pagina;
 * output:
 *       PAGINA *pag;      puntatore alla struttura allocata
 *
 *------------------------------------------------------------------------*/
Boolean pagina_init(char *nomepag,PAGINA **pag ,int tipo)
{

   if( (*pag = alloca_memoria(1,sizeof(PAGINA))) == NULL)
      return(False);
   /*
        setta a "" il campo elenco_wid
   */
   (*pag)->elenco_wid=XtNewString("");
   load_nomifile(nomepag,*pag,tipo);
   return(True);
}



/*-----------------------------------------------------
 *
 * pagina_load_file
 *
 * carica nel campo db della struttura PAGINA
 * il database delle risorse
 * letto dal file <nome_pagina>.pag
 *
 *-----------------------------------------------------*/
Boolean pagina_load_file(PAGINA *pag,int tipo)
{
char filename[MAXLENFILENAME];
char mesg[200];

   if(tipo == TIPO_PAGINA)
   {
      if( XlGetenv(ENVPAG) != NULL )
      {
         strcpy(filename,XlGetenv(ENVPAG));
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
         strcpy(filename,XlGetenv(ENVLIB));
         strcat(filename,"/");
         strcat(filename,pag->filepag);
         strcpy(filename,XlConvPathVms(filename));
      }
      else
         return(False);
   }

/* load delle risorse */
   strcpy(filename,XlConvPathVms(filename));
   if( (pag->db = XrmGetFileDatabase(filename)) == NULL )
   {
      printf("Warning XrmGetFileDatabase Failed on page %s\n",filename);
/*
      sprintf(mesg,"Warning XrmGetFileDatabase Failed on page %s \n",filename);
      show_message(mesg);
*/
      return(False);
   }
   return(True);
}



Boolean PutResPaginaDb(PAGINA *pag,XrmDatabase *db)
{
int num_res=XtNumber(confinfo_pag);
char *app_char;
Dimension app_dim;
Position app_pos;
int app_int;
int i;
char appo[100],risorsa[100];
Boolean ret=True;

/*
        prelevo il valore dello snap
*/
#ifdef MAURIC
pag->snap_pag=get_step(pag->drawing);
#endif
/*
printf("PutResPaginaDb: snap_pag = %d\n",pag->snap_pag);
*/
if(pag==NULL)
        return;

for(i=0;i<num_res;i++)
        {
        sprintf(risorsa,"*%s",confinfo_pag[i].resource_name);
        switch(confinfo_pag[i].confinfo_type){
            case XlRTextAlloc:
                {
                app_char=(char*)pag + confinfo_pag[i].compiled_offset;
                if(app_char!=NULL)
                        XrmPutStringResource(db,risorsa, app_char);
                else
                        XrmPutStringResource(db,risorsa, "");
                break;
                }
            case XlRText:
                {
                memcpy(&app_char,
                        (char*)pag + confinfo_pag[i].compiled_offset,
                        confinfo_pag[i].compiled_size);
                if(app_char!=NULL)
                        XrmPutStringResource(db,risorsa, app_char);
                else
                        XrmPutStringResource(db,risorsa, "");
                break;
                }
            case XlRInt:
                {
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(&app_int,app_char,confinfo_pag[i].compiled_size);
                sprintf(appo,"%d",app_int);
                XrmPutStringResource(db,risorsa,appo);
                break;
                }
            case XlRPosition:
                {
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(&app_pos,app_char,confinfo_pag[i].compiled_size);
                sprintf(appo,"%d",app_pos);
                XrmPutStringResource(db,risorsa,appo);
                break;
                }
            case XlRDimension:
                {
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(&app_dim,app_char,confinfo_pag[i].compiled_size);
                sprintf(appo,"%d",app_dim);
                XrmPutStringResource(db,risorsa,appo);
                break;
                }
           } /* switch */
        }  /* for */
return(ret);
}




Boolean GetResPaginaDb(PAGINA *pag,XrmDatabase db, Boolean modo)
{
char mesg[200];
int num_res=XtNumber(confinfo_pag);
char *app_char,*app_char2;
Dimension app_dim;
Position app_pos;
int app_int;
int i;
char appo[100],risorsa[100];
Boolean retval,ret;
char *tipo=NULL;
XrmValue value;

if(pag==NULL)
        return(False);

/*
        setto i valori di default della struttura pagina
*/
strcpy(pag->geom.tipo,TIPO_DEF);
strcpy(pag->geom.background,BACKGROUND_DEF);
strcpy(pag->gerarchia,"-1,-1,-1,-1,-1,-1");
pag->geom.x = XDEF;
pag->geom.y = YDEF;
pag->geom.width = MIN_WIDTH;
pag->geom.height = MIN_HEIGHT;
pag->geom.draw_width = MIN_WIDTH;
pag->geom.draw_height = MIN_HEIGHT;
pag->refreshFreq = DEFAULT_REFRESHFREQ;
pag->in_use = SCHEME_NOT_IN_USE;
pag->nextTagRegol[0] = ASCII0;
pag->nextTagRegol[1] = ASCII0;
pag->dispreg_visual_mode = 0;
pag->port_visual_mode = 0;
pag->interfaceport_visual_mode = 0;
pag->snap_pag = DEF_REG_SNAP;


for(i=0;i<num_res;i++)
        {
        sprintf(risorsa,"*%s",confinfo_pag[i].resource_name);
        retval = XrmGetResource(db, confinfo_pag[i].resource_name,
                        confinfo_pag[i].resource_class, &tipo, &value);
        if(!retval && modo)
                {
                printf("Resource Not Get %s  on page %s \n",
                       confinfo_pag[i].resource_name,pag->nomepag);
/*
                sprintf(mesg,"Resource Not Get %s  on page %s \n",
                        confinfo_pag[i].resource_name,pag->nomepag);
                show_message(mesg);
*/
                }
        if(retval)
          switch(confinfo_pag[i].confinfo_type){
            case XlRTextAlloc:
                {
                app_char=(char*)pag + confinfo_pag[i].compiled_offset;
                if(value.size<confinfo_pag[i].compiled_size-1)
                        strncpy(app_char, value.addr, value.size);
                else
                        strncpy(app_char, value.addr,
                                confinfo_pag[i].compiled_size-1);
                break;
                }
            case XlRText:
                {
                char **da_lib;

                da_lib =(char **) ((char*)pag +
                        confinfo_pag[i].compiled_offset);
                if(*da_lib)
                        XtFree(*da_lib);
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                app_char2=XtNewString(value.addr);
                memcpy( app_char,
                        &app_char2, confinfo_pag[i].compiled_size);
                break;
                }
            case XlRInt:
                {
                app_int=atoi(value.addr);
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(app_char,&app_int,confinfo_pag[i].compiled_size);
                break;
                }
            case XlRPosition:
                {
                app_pos=atoi(value.addr);
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(app_char,&app_pos,confinfo_pag[i].compiled_size);
                break;
                }
            case XlRDimension:
                {
                app_dim=atoi(value.addr);
                app_char = (char*)pag + confinfo_pag[i].compiled_offset;
                memcpy(app_char,&app_dim,confinfo_pag[i].compiled_size);
                break;
                }
           } /* switch */
        }  /* for */

  /*
   *    verifico la congruenza delle risorse lette
   */
   if((pag->snap_pag <=0) || (pag->snap_pag > MAX_SNAP))
        pag->snap_pag = DEF_REG_SNAP;

return(True);
}





/*------------------------------------------------------------
 * pagina_getres()
 *
 * a)load delle resources descriventi le caratteristiche generali
 *   della pagina (dimensioni,tipo ecc...) dal Db della pagina
 *   stessa.
 * b)allocazione e load dell'array di strutture AN_OBJ che contiene
 *   i nomi e le classi di tutti gli oggetti da creare sulla pagina.
 *
 * input:
 *       PAGINA *pagina;  pointer alla struttura dati
 *                        che descrive le caratteristiche
 *                        generali della pagina.
 * input+output:
 *       AN_OBJ **listaobj; puntatore alla lista di strutture
 *                          (allocata internamente) contenente
 *                          nomi e classi degli oggetti da
 *                          creare sulla pagina.
 *
 * retvalue:
 *           True
 *           False
 *------------------------------------------------------------*/
Boolean pagina_getres(PAGINA *pagina,AN_OBJ **listaobj)
{
   int i,j,retval=True,numero_widget;
   int read,num;
   char *tipo=NULL,*appo,classname[100],nappo[100];
   XrmValue value;
   char mesg[200];

retval=GetResPaginaDb(pagina,pagina->db,True);


/**
   Allocazione e caricamento dell'array di strutture di tipo AN_OBJ
   che contengono i campi stringa nome e classe dei widget da creare sulla
   pagina.
   Tale array viene utilizzato nel ciclo di creazione degli
   oggetti stessi.
**/
   if(pagina->num_widget > 0)
   {

      /* inizializzazione dei descrittori degli widget sulla iconlib */

      if( (*listaobj = alloca_memoria(pagina->num_widget,sizeof(AN_OBJ))) == NULL)
         return(False);


      appo = pagina->elenco_wid;

      for(i=0;i<pagina->num_widget;i++)
      {
         sscanf(appo,"%s",(*listaobj)[i].nome);
         appo += ( strlen( (*listaobj)[i].nome ) +1 );
         sscanf(appo,"%s",classname);
         appo += ( strlen( classname ) +1 );
         (*listaobj)[i].classe = XlGetClassWid(classname);
      }
   }
   else
   {
     pagina->next_num = 1;
     *listaobj = NULL;
   }

   if(!retval)
   {
      printf("Error on Get Resource on page %s \n", pagina->nomepag);
/*
      sprintf(mesg,"Error on Get Resource on page %s \n", pagina->nomepag);
      show_message(mesg);
*/
   }

   return(retval);
}




int PagGetType(PAGINA *pag)
{
    if( !strcmp(pag->geom.tipo,TIPO_SINOTTICO) )
       return(TYPE_SINOTTICO);
    if( !strcmp(pag->geom.tipo,TIPO_STAZIONI) )
       return(TYPE_STAZIONE);
    if( !strcmp(pag->geom.tipo,TIPO_LIBRERIA) )
       return(TYPE_LIBRERIA);
    if( !strcmp(pag->geom.tipo,TIPO_REGOLAZIONE) )
       return(TYPE_REGOLAZIONE);
    if( !strcmp(pag->geom.tipo,TIPO_TELEPERM) )
       return(TYPE_TELEPERM);
}




/*
 pagina_free
 libera il database associato ad una pagina e la memoria occupata
 dalla struttura pagina e dai soui membri
*/
void pagina_free(PAGINA *pag)
{
char **p_testo;
int i;
int num_res=XtNumber(confinfo_pag);
/*
 Distrugge il database delle risorse associato
 alla pagina
*/
if(pag->db!=NULL)
        XrmDestroyDatabase(pag->db);
/*
 libera nella struttura pag i campi che erano stati allocati
 nella lettura delle risorse descrittive della pagina
*/
for(i=0;i<num_res;i++)
        {
        if(confinfo_pag[i].confinfo_type == XlRText)
                {
        p_testo =(char **) ((char*)pag + confinfo_pag[i].compiled_offset);

                if((*p_testo)!= pag->elenco_wid)
                        {
                        printf("\n calcolo errato p_testo !! \n");
                        XtFree(pag->elenco_wid);
                        }
                else
                        XtFree(*p_testo);
                }
        }
/*
 Se la pagina contiene un pixmap di sfondo lo libera
*/
if(pag->sfondo)
        XFreePixmap(XtDisplay(pag->drawing),pag->sfondo);
/*
 libera la struttura pag
*/
free(pag);
}



Boolean PagGetInUse(PAGINA *pag)
{
if(pag->in_use==SCHEME_IN_USE)
        return(True);
else
        return(False);
}



/*
   Recupero il path della directory proc dalla
   variabile d'ambiente WORK_PAG oppure ENVPAG
*/
int EstrWorkFile(char *outfile, char *envfile, char *rtffile)
{
char workfile[200];


   if( XlGetenv(WORK_PAG)!=NULL )
     {
     strcpy(workfile,XlGetenv(WORK_PAG));
     strcpy(envfile,XlGetenv(ENVPAG));
     strcpy(rtffile,XlGetenv(ENVRTF));
     }
   else
     {
     if( XlGetenv(ENVPAG)!=NULL )
        strcpy(envfile,XlGetenv(ENVPAG));
     else
        {
        fprintf(stderr,"non e' definita ENVPAG\n");
        return(-1);
        }
     if( XlGetenv(ENVRTF)!=NULL )
        strcpy(rtffile,XlGetenv(ENVRTF));
     else
        {
        fprintf(stderr,"non e' definita ENVRTF\n");
        return(-1);
        }
/* modifica 21-11-96 carlo
     strcpy(workfile,envfile);
     strcat(workfile,"/proc");
*/
     if( XlGetenv(ENVCTX)!=NULL )
     {
        strcpy(workfile,XlGetenv(ENVCTX));
        strcat(workfile,"/proc");
     }
     if ( !XlPutenv(WORK_PAG,workfile) )
        fprintf(stderr,"EstrWorkFile:Fallita la Putenv di WORK_PAG\n");
     }
#if defined UNIX
     strcat(workfile,"/");
     strcat(envfile,"/");
     strcat(rtffile,"/");
#endif

   strcpy(outfile,workfile);
/*
   printf("EstrWorkFile: ritorna\n   %s\n   %s\n   %s\n\n",
          workfile,envfile,rtffile);
*/
   return(0);
}






/*--------------------
 *
 *
 *
 ----------------------*/
Boolean file_exist(char *filename)
{
   FILE *fp;

   if(  (fp = fopen(filename,"r")) != NULL)
   {
      fclose(fp);
      return(True);
   }
   return(False);
}

// /*--------------------
//  *
//  *
//  *
//  ----------------------*/
//  FILE *file_exists(char *filename)
//  {
//     FILE *fp;
 
//     if(  (fp = fopen(filename,"r")) != NULL)
//     {
//        fclose(fp);
//        return(True);
//     }
//     return(NULL);
//  }

/*--------------------
 *
 *
 *
 ----------------------*/
int setvarenv_appl(char *pag_dir,char *rtf_dir)
{
char curr_dir[FILENAME_MAX];
char nome_path_abs[FILENAME_MAX];

   getwd(curr_dir);
   if(!XlPutenv(ENVCTX,curr_dir)) {
      printf("XlPutenv ENVCTX error!!!\n");
      return(False); 
   }
   if(path_rel_to_abs(pag_dir,nome_path_abs)<0)
   {
   printf("cmp_pag_util.c: error retrieving abs path of=%s\n",pag_dir);
   return(False);
   }

   if(!XlPutenv(ENVPAG,nome_path_abs)) {
      printf("XlPutenv ENVPAG error!!!\n");
      return(False);
   }
  if(path_rel_to_abs(rtf_dir,nome_path_abs)<0)
   {
   printf("cmp_pag_util.c: error retrieving abs path of=%s\n",nome_path_abs);
   return(False);
   }

   if(!XlPutenv(ENVRTF,rtf_dir)) {
      printf("XlPutenv ENVRTF error!!!\n");
      return(False);
   }
/*--------------------
printf("setvarenv_appl:\n\t");
printf("ENVPAG = %s\n",XlGetenv(ENVPAG));
printf("ENVRTF = %s\n",XlGetenv(ENVRTF));
printf("ENVCTX = %s\n",XlGetenv(ENVCTX));
--------------------*/
return(True);
}




