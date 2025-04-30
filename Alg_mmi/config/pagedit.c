/**********************************************************************
*
*       C Source:               pagedit.c
*       Subsystem:              8
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Sep 22 10:32:31 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: pagedit.c-4.1.6 %  (%full_filespec: pagedit.c-4.1.6:csrc:8 %)";
#endif
/*
   modulo pagedit.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)pagedit.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)pagedit.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*----------------------------------------------------------------------- 
 *
 * Pagedit.c 
 *
 * Contents: modulo contenente le routine di gestione del pagedit_context
 *           
 *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <Xm/Xm.h>

#include "UxXt.h"

#include <libgen.h>
#include "config.h"
#include "message.h"

extern Arg args[];
extern int lnargs;
extern Display *UxDisplay;
extern Widget errorDialog;
extern Boolean file_exist();
extern void TopMenuSetInsensitive();
extern void TopMenuSetSensitive();

//extern void show_message();

extern int path_rel_to_abs(char *,char *);

extern OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *, char *, int , char *,char *,char *);


extern char Context_Path[];

Boolean GetRes = True;

/*--------------------------------------------------------------------
 * pagedit_context_init()
 *
 * allocazione e inizializzazione della struttura dati PAGEDIT_CONTEXT
 *
 * input:
 *        char *filectx;            nome del file context corrente
 *                                  puo' essere compresa nel nome
 *                                  anche la path ma sempre in UNIX-Style
 *        PAGEDIT_CONTEXT **pagctx; puntatore alla struttura
 *
 * retvalue:
 *           True
 *           False
 *-------------------------------------------------------------------*/

int pagedit_context_init(char *filectx,PAGEDIT_CONTEXT **pagctx)
{

   extern char *extractFileName();
   char mesg[100];
   char *appo=NULL,loadfile[250];


/* allocazione e inizializzazione struttura */
   if( ((*pagctx) = (PAGEDIT_CONTEXT *)alloca_memoria( 1, sizeof(PAGEDIT_CONTEXT) ) ) == NULL )
      return(False);

/* verifica esistenza del context file */ 
   if( !file_exist(filectx) )
   {
      GetRes = False;
      return(False);
   }
   else
   {
      GetRes = True;

#ifdef VMS
      appo = strstr(filectx,";");
      if( appo != NULL)
         *appo = 0;
#endif


/* setto la ContextPath dal filename del context (se questo e' comprensivo di path)
   oppure dalla current directory
*/
   if( dirname(filectx) != NULL )
      strcpy(Context_Path,dirname(filectx));
   else
      strcpy(Context_Path,getwd(Context_Path));

 
/* nella struttura inserisco il solo nome senza path */
      strcpy((*pagctx)->fileContext,extractFileName(filectx));
 

/* load delle risorse 
   con conversione della path in formato VMS-style (se necessario) */


      strcpy(loadfile,XlConvPathVms(filectx));
      (*pagctx)->db = XrmGetFileDatabase(loadfile);

      if((*pagctx)->db == NULL)
      {
         SetMsg(NULL,0,INFMSG,"ConfigInfo","No Resource Database Loaded", 
                NULL,False,NULL,False,NULL);
         GetRes = False;
         return(False);
      }

   }
   return(True);
}

/*------------------------------------------------------
 * setPageditEnv
 * 
 * effettua la XlPutenv delle variabili di enviroment
 * (cho sono interne alla pplicazione)
 * relative ai vari direttori impostati nel context
 * se le directory non esistono  o non riesce a
 * effettuare la XlPutenv ritorna False
 * unica eccezione per la directory del simulatore
 * in questo caso viene solo disabilitata la
 * compilazione delle pagine
 *-----------------------------------------------------*/

int setPageditEnv(PAGEDIT_CONTEXT *pedit)
{
   char mesg[300];
   char debug_dir[255];
   char start_dir[255];
   char path_abs[FILENAME_MAX];


   if( getwd(start_dir) == NULL)
   {
      show_message("Error on reading current directory\n");
      return(False);
   }

   

/* DEBUG carlo mi posiziononella directory dove c'e' il context */
   XlChDir(Context_Path);


/* verifica accessibilita' sulle directory interessate */
   if( verifica_direttori_context(pedit) != True )
   {
      show_message("Error on Context\n");
      return(False);
   }


/* set delle veriabili di environment */
   if(path_rel_to_abs(Context_Path,path_abs)<0)
      {
      fprintf(stderr,"pagedit.c.: error retrieving abs path of=%s\n",Context_Path);
      return(False);
      }

   if( !XlPutenv(ENVCTX,path_abs) )
   {
      return(False);
   }

   if(path_rel_to_abs(pedit->pages,path_abs)<0)
      {
      fprintf(stderr,"pagedit.c.: error retrieving abs path of=%s\n",
                                                        pedit->pages);
      return(False);
      }

   if( !XlPutenv(ENVPAG,path_abs) )
   {
      return(False);
   }

   if(path_rel_to_abs(pedit->objectLibrary,path_abs)<0)
      {
      fprintf(stderr,"pagedit.c.: error retrieving abs path of=%s\n",
                                                        pedit->objectLibrary);
      return(False);
      }

   if( !XlPutenv(ENVLIB,path_abs) )
   {
      return(False);
   }

   if(path_rel_to_abs(pedit->animatedIconLibrary,path_abs)<0)
      {
      fprintf(stderr,"pagedit.c.: error retrieving abs path of=%s\n",
                                                        pedit->animatedIconLibrary);
      return(False);
      }
   if( !XlPutenv(ENVICO,path_abs) )
   {
      return(False);
   }

   if(path_rel_to_abs(pedit->simulator,path_abs)<0)
      {
      fprintf(stderr,"pagedit.c.: error retrieving abs path of=%s\n",
                                                        pedit->simulator);
      return(False);
      }

   if( !XlPutenv(ENVRTF,path_abs) )
   {
   return(False);
   }

   if( XlGetenv(ENVCTX) != NULL)
      XlChDir(XlGetenv(ENVCTX));


   return(True);
}

/*------------------------------------------------------------
 *
 * pagedit_context_getres()
 *
 * load delle resources del Db Context
 *
 * input:
 *       PAGEDIT_CONTEXT *pagctx;  pointer alla struttura dati
 *
 * retvalue:
 *           True
 *           False
 *------------------------------------------------------------*/

int pagedit_context_getres(PAGEDIT_CONTEXT *pagctx)
{
   int errcount;
   int i,retval=True;
   char *tipo,appo[800],reserr[1000],appoggio[200];
   XrmValue value;
   char *listaDisplay;
   Boolean errore = False;
   
   Boolean AddResPageContext(PAGEDIT_CONTEXT *, int);

   if(!GetRes)
      return(False);

   errcount=0;
   strcpy(reserr,"Resources substituted with default values :\n");

   if(!errore)
   {
      if(  XrmGetResource(pagctx->db, XlNpages, XlCPages, &tipo, &value) == 0 )
      {
         errcount++;
         strcat(reserr,"Pages \n");
	 strcpy(pagctx->pages,"");
      }
      else
         strncpy(pagctx->pages,value.addr,(int)value.size);
   }
   
   if( !errore )
   {
      if( XrmGetResource(pagctx->db, XlNdescription, XlCDescription, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"Description \n");
         strcpy(pagctx->descrizione,"");
      }
      else
         strncpy(pagctx->descrizione,value.addr,(int)value.size);
   }

   if(!errore )
   {
      if( XrmGetResource(pagctx->db, XlNobjectLibraries, XlCObjectLibraries, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"ObjectLibraries \n");
         strcpy(pagctx->objectLibrary,".");
      }
      else
         strncpy(pagctx->objectLibrary,value.addr,(int)value.size);
   }

   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XlNanimatedIconLibraries, XlCAnimatedIconLibraries, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"AnimatedIconLibraries\n");
         strcpy(pagctx->animatedIconLibrary,".");
      }
      else
         strncpy(pagctx->animatedIconLibrary,value.addr,(int)value.size);
   }


   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XlNsimulator, XlCSimulator, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"Simulator\n");
         strcpy(pagctx->simulator,".");
      }
      else
         strncpy(pagctx->simulator,value.addr,(int)value.size);
   }


/* hostname */
   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XlNhostName, XlCHostName, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"Hostname\n");
         strcpy(pagctx->hostname,"");
      }
      else
         strncpy(pagctx->hostname,value.addr,(int)value.size);
   }
/* hostnameS */
   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XlNhostNameS, XlCHostNameS, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"HostnameS\n");
         strcpy(pagctx->hostnameS,"");
      }
      else
         strncpy(pagctx->hostnameS,value.addr,(int)value.size);
    }
/* get numero delle pagine definite */
   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XtNpag_num, XtCPag_num, &tipo, &value) == 0)
      {
         errcount++;
         strcat(reserr,"PagNum\n");
	 pagctx->pag_num =0;
      }
      else
         pagctx->pag_num = atoi(value.addr);

      if(pagctx->pag_num > 0)
      {
         if( (pagctx->page_list = (char **)alloca_memoria(pagctx->pag_num,sizeof(char *)) ) != NULL )
         {
            /* carico la lista delle pagine */
            if( XrmGetResource(pagctx->db, XtNpage_list, XtCPage_list, &tipo, &value) == 0)
		{
               	errore = True;
		errcount++;
		strcat(reserr,"Error in page list\n");
		}
         }
         else
            errore = True;
      }
   }

   /* Allocazione dell'array di puntatori alle informazioni relative
      alle pagine contenute nel context */
   if( (pagctx->pag_num > 0) && !errore )
   {
      if( (pagctx->res_page = (INFO_PAGE **)alloca_memoria(pagctx->pag_num,sizeof(INFO_PAGE *)) ) == NULL )
         errore = True;
   }

   if( (pagctx->pag_num > 0) && !errore ) 
   {
      for(i=0;i<pagctx->pag_num;i++)
      {
         sscanf(value.addr,"%s",appo);
         if( (pagctx->page_list[i] = (char *)alloca_memoria( strlen(appo)+1, sizeof(char) )) == NULL)
         {
            errore = True;
            break;
         }
         else
         {
            strcpy(pagctx->page_list[i],appo);
            value.addr += (strlen( pagctx->page_list[i] ) +1);
         }
      }
   }

   /* CAPPE */
   if( (pagctx->pag_num > 0) && !errore )
   {
      for(i=0;i<pagctx->pag_num;i++)
      {
         if(AddResPageContext(pagctx, i) == True)
         {
            errore = True;
            break;
         }
      }
   } 

/* get numero delle iconlib definite */
   if(!errore)
   {
      if( XrmGetResource(pagctx->db, XtNiconlib_num, XtCIconlib_num, &tipo, &value) == 0)
	{
        errcount++;
	pagctx->iconlib_num = 0;
        strcat(reserr,"IconlibNum\n");
	}
      else
        pagctx->iconlib_num = atoi(value.addr);
   

      if( (pagctx->iconlib_num>0) && !errore )
      {
         if( (pagctx->iconlib_list  = (char **)alloca_memoria(pagctx->iconlib_num,sizeof(char *))) == NULL)
            errore = True;

         if( (pagctx->iconlib_label = (char **)alloca_memoria(pagctx->iconlib_num,sizeof(char *)) ) == NULL )
            errore = True;

/* carico la lista delle iconlib */
         if(!errore)
         {
            if( XrmGetResource(pagctx->db, XtNiconlib_list, XtCIconlib_list, &tipo, &value) == 0 )
               errore = True;
            else
            {

               for(i=0;i<pagctx->iconlib_num;i++)
               {
                  sscanf(value.addr,"%s",appo);
                  if( (pagctx->iconlib_list[i] = (char *)alloca_memoria( strlen(appo)+1, sizeof(char) )) == NULL )
                  {
                     errore = True;
                     break;
                  }
                  else
                  {
                     strcpy(pagctx->iconlib_list[i],appo);
                     value.addr += (strlen( pagctx->iconlib_list[i] ) +1);
                  }
               }

/* carico la lista delle label delle iconlib da attribuire ai bottoni del menu delle pagine */
               if( XrmGetResource(pagctx->db, XtNiconlib_label, XtCIconlib_label, &tipo, &value) == 0)
                  errore = True;
               else
               {

                  for(i=0;i<pagctx->iconlib_num;i++)
                  {
                     sscanf(value.addr,"%s",appo);
                     if( (pagctx->iconlib_label[i] = (char *)alloca_memoria( strlen(appo)+1, sizeof(char) )) == NULL )
                     {
                        errore = True;
                        break;
                     }
                     else
                     {
                        strcpy(pagctx->iconlib_label[i],appo);
                        value.addr += (strlen( pagctx->iconlib_label[i] ) +1);
                     }
                  }
               }
            }
         }
      }
   }

/* get numero display  */
   if( !errore )
   {
      if( XrmGetResource(pagctx->db, XlNnumDisplay, XlCNumDisplay, &tipo, &value) == 0)
	{
        errcount++;
	pagctx->num_display = 0;
        strcat(reserr,"NumDisplay\n");
	}
      else
         pagctx->num_display = atoi(value.addr);


/* get  display list */
      if( XrmGetResource(pagctx->db, XlNdisplayList, XlCDisplayList, &tipo, &value) == 0)
	{
        errcount++;
	errore = True;
        strcat(reserr,"Error reading display list\n");
	}
      else
      {
         if( (pagctx->num_display > 0) && ! errore )
         {
            if( (listaDisplay = (char *)alloca_memoria(pagctx->num_display*MAXCHARDISPLAY,sizeof(char)) ) == NULL)
               errore = True;
            else
               strcpy(listaDisplay,value.addr);

            if( (pagctx->elenco_display = (char **)alloca_memoria(pagctx->num_display,sizeof(char *))) != NULL)
            {
               for(i=0;i<pagctx->num_display;i++) 
               {
                 if(sscanf(listaDisplay,"%s",appo)>0)
                 {
                    if((pagctx->elenco_display[i] = (char *)alloca_memoria(strlen(appo)+1,sizeof(char))) == NULL)
                       return(False);
                    strcpy(pagctx->elenco_display[i],appo);
                    listaDisplay += strlen(appo)+1;
                    if(*listaDisplay == ' ')
                       listaDisplay++;
                 }
               }
            }
         }
      }
   }

/* get numero TagPag  definite */
   if( !errore)
   {
      if( XrmGetResource(pagctx->db, XlNnextTagPag, XlCNextTagPag, &tipo, &value) == 0)
      {
	 errcount++;
	 strcpy(pagctx->nextTagPag,"00"); 
         strcat(reserr,"nextTagPag");
      }
      else
         strncpy(pagctx->nextTagPag,value.addr,DIMTAG);

   }


   if(errore)
	{
	SetMsg(NULL,NULL,WRNMSG,"Config Error", "Not recoverable error in Context.ctx format",
               NULL,False,NULL,False,NULL);
	return(False);
	}
   if(errcount)
   {
      printf("reserr = %s\n",reserr);
      SetMsg(NULL,NULL,WRNMSG,"Config Error",reserr,NULL,False,NULL,False,NULL);
      new_save_context(pagctx);
      return(True);
   }
   else
      return(True);
   
}

  
/*------------------------------------------------------------------
 * new_save_context
 *
 * salvataggio file Context
 *
 *-----------------------------------------------------------------*/

int new_save_context(PAGEDIT_CONTEXT *pagedt)
{
   int i;
   char *lista=NULL,appo[800],outfile[MAXLENFILENAME], risorsa[100];
   char *lista_display=NULL;


   XrmDestroyDatabase(pagedt->db);
   pagedt->db = XrmGetStringDatabase("");

/* scrivo nel Xrmdb tutte le risorse del context */


   sprintf(appo,"%s",pagedt->descrizione);
   XrmPutStringResource(&pagedt->db,"*description",appo);

   sprintf(appo,"%s",pagedt->pages);
   XrmPutStringResource(&pagedt->db,"*pages",appo);

   sprintf(appo,"%s",pagedt->objectLibrary);
   XrmPutStringResource(&pagedt->db,"*objectLibraries",appo);

   sprintf(appo,"%s",pagedt->animatedIconLibrary);
   XrmPutStringResource(&pagedt->db,"*animatedIconLibraries",appo);

   sprintf(appo,"%s",pagedt->simulator);
   XrmPutStringResource(&pagedt->db,"*simulator",appo);

   sprintf(appo,"%d",pagedt->num_display);
   XrmPutStringResource(&pagedt->db,"*numDisplay",appo);

   if(pagedt->num_display > 0)
   {
      if( ( lista_display = (char *)alloca_memoria(pagedt->num_display*MAXCHARDISPLAY,sizeof(char))) == NULL)
          return(False);

      for(i=0;i<pagedt->num_display;i++)
      {
         strcat(lista_display," ");
         strcat(lista_display,pagedt->elenco_display[i]);
      }
   }
   else
   {
      if( (lista_display = (char *)alloca_memoria(MAXCHARDISPLAY,sizeof(char))) == NULL)
         return(False);
      strcpy(lista_display,"");
   } 

   XrmPutStringResource(&pagedt->db,"*displayList",lista_display);

   sprintf(appo,"%d",pagedt->pag_num);
   XrmPutStringResource(&pagedt->db,"*pag_num",appo);

   sprintf(appo,"%d",pagedt->iconlib_num);
   XrmPutStringResource(&pagedt->db,"*iconlib_num",appo);

   sprintf(appo,"%s",pagedt->nextTagPag);
   XrmPutStringResource(&pagedt->db,"*nextTagPag",appo);

/* scrivo nel Xrmdb la lista delle pagine, delle icon library, delle TagPag */

   if(pagedt->pag_num>0)
   {
      if( (lista = (char *)alloca_memoria( pagedt->pag_num*MAXCHAR_PAGNAME,sizeof(char))) == NULL)
          return(False);
   

      for(i=0;i<pagedt->pag_num;i++)
      {
         strcat(lista," ");
         strcat(lista,pagedt->page_list[i]);
      }

      XrmPutStringResource(&pagedt->db,"*page_list",lista);
      libera_memoria(lista);
      /* CAPPE Caricamento delle nuove risorse nel database */
      for(i=0;i<pagedt->pag_num;i++) {
	 sprintf(risorsa, "*%s.top_descrizione", pagedt->page_list[i]);
	 XrmPutStringResource(&pagedt->db, risorsa, pagedt->res_page[i]->descr_page);
	 sprintf(risorsa, "*%s.top_tipo", pagedt->page_list[i]);
	 XrmPutStringResource(&pagedt->db, risorsa, pagedt->res_page[i]->type_page);
	 sprintf(risorsa, "*%s.tagPag", pagedt->page_list[i]);
	 XrmPutStringResource(&pagedt->db, risorsa, pagedt->res_page[i]->tagPag);
	 sprintf(risorsa, "*%s.refresh_freq", pagedt->page_list[i]);
	 sprintf(appo, "%d", pagedt->res_page[i]->refresh_freq);
	 XrmPutStringResource(&pagedt->db, risorsa, appo);

	 sprintf(risorsa, "*%s.gerarchia", pagedt->page_list[i]);
	 XrmPutStringResource(&pagedt->db, risorsa, pagedt->res_page[i]->hier);

	 sprintf(risorsa, "*%s.schemeInUse", pagedt->page_list[i]);
	 sprintf(appo, "%d", pagedt->res_page[i]->in_use);
	 XrmPutStringResource(&pagedt->db, risorsa, appo);
      } 

   }
   else
	{
        XrmPutStringResource(&pagedt->db,"*page_list"," ");
	}


   if(pagedt->iconlib_num > 0)
   {
      if((lista = (char *)alloca_memoria(pagedt->iconlib_num*MAXCHAR_PAGNAME,sizeof(char))) == NULL)
          return(False);

      for(i=0;i<pagedt->iconlib_num;i++)
      {
         strcat(lista," ");
         strcat(lista,pagedt->iconlib_list[i]);
      }
      sprintf(appo,"%s",lista);
      
      XrmPutStringResource(&pagedt->db,"*iconlib_list",appo);
      libera_memoria(lista);
      
      }
   else
      XrmPutStringResource(&pagedt->db,"*iconlib_list"," ");
 


   if( pagedt->iconlib_num >0 )
   {
      if((lista = (char *)alloca_memoria(pagedt->iconlib_num*MAXCHAR_PAGNAME,sizeof(char))) == NULL)
          return(False);
/****   
   else
      lista = NULL;
****/

      for(i=0;i<pagedt->iconlib_num;i++)
      {
         strcat(lista," ");
         strcat(lista,pagedt->iconlib_label[i]);
      }
      sprintf(appo,"%s",lista);
      XrmPutStringResource(&pagedt->db,"*iconlib_label",appo);
      libera_memoria(lista);

   }
   else
      XrmPutStringResource(&pagedt->db,"*iconlib_label"," ");
   

   sprintf(appo,"%s",pagedt->hostname);
   XrmPutStringResource(&pagedt->db,"*hostName",appo);

   sprintf(appo,"%s",pagedt->hostnameS);
   XrmPutStringResource(&pagedt->db,"*hostNameS",appo);

/* scrivo su file le risorse caricate nel Xrmdb  */

   

   strcpy(outfile,Context_Path);
   strcat(outfile,"/");
   strcat(outfile,pagedt->fileContext);
   XrmPutFileDatabase(pagedt->db,XlConvPathVms(outfile));

   return(True);
}
 
void richiedi_change_database()
{
    extern Widget create_messageBoxDialog1();

    UxPopupInterface( create_messageBoxDialog1(KILL_SIMULATOR,"Do You Want To Reload The Topology Defined in Context ?",(char *)NULL), no_grab);
}   
/*-------------------------------
 *
 * opera lo switch tra database
 *
 *-------------------------------*/

void change_database() 
{
   extern OlDatabaseTopologiaObject dbtopologia;
   extern PAGEDIT_CONTEXT *pagedit;
   extern Boolean  CompilerOk;
   extern Widget topLevelShell;
   extern OlCompilerObject compilatore_severe;

   char filertf[MAXLENFILENAME];
   char varcc[MAXLENFILENAME];
   char varsub[MAXLENFILENAME];
   char varcom[MAXLENFILENAME];
   char varrtf[MAXLENFILENAME];


   CompilerOk = True;
   strcpy(filertf,pagedit->simulator);
   strcat(filertf,"/variabili.rtf");
   strcpy(filertf,XlConvPathVms(filertf));

/* tento di caricare il nuovo database 
   distruggendo il vecchio,
   effettuando la killsim
   creando il nuovo database 
*/
   if( file_exist(filertf) )
      {
      if(dbtopologia != NULL)
         {
         if(OlDestroyDatabaseTopologia(dbtopologia) == False)
             {
             SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Cannot Perform Destroy Simulator Database",NULL,False,NULL,False,NULL);
             CompilerOk = False;
             return;
             }
          }

      strcpy(varrtf,filertf);

      strcpy(varcc,Context_Path);
      strcat(varcc,"/FileDefVarCC.edf");
      strcpy(varcc,XlConvPathVms(varcc));

      strcpy(varsub,pagedit->simulator);
      strcat(varsub,"/FileSubSystem.edf");
      strcpy(varsub,XlConvPathVms(varsub));

      strcpy(varcom,Context_Path);
      strcat(varcom,"/FileComponent.edf");
      strcpy(varcom,XlConvPathVms(varcom));
       
      if( getenv("SHR_USR_KEY") == NULL )
          {
            printf("disabilito la compilazione perche' SHR_USR_KEY non e' definita\n");
            CompilerOk = False;
            return;
          }

      if( (dbtopologia = OlCreateDatabaseTopologia("olrtf",varrtf,atoi(getenv("SHR_USR_KEY")),
                                varsub,varcom,varcc)) == NULL)
          {
          SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Simulator Database Not Active",NULL,False,NULL,False,NULL);
          CompilerOk = False;
          return;
          } 

      OlDestroyCompiler(compilatore_severe);
      compilatore_severe = OlCreateCompiler("olsevere",dbtopologia);
      }
   else
      {
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","File Simulator Topology Database Not Found",NULL,False,NULL,False,NULL);
      CompilerOk = False;
      printf("Il file %s non esiste\n",filertf);
      }

}


/*-----------------------------------------------
 *
 * kill del simulatore correntemente attivo 
 * distruzione del database attivo 
 * creazione nuovo database  
 *---------------------------------------------*/
void subs_database()
{
   extern OlDatabaseTopologiaObject dbtopologia;
   extern PAGEDIT_CONTEXT *pagedit;
   extern Boolean chiudi_simul;
   extern Boolean CompilerOk;
   char filertf[MAXLENFILENAME];
   char varcc[MAXLENFILENAME];
   char varsub[MAXLENFILENAME];
   char varcom[MAXLENFILENAME];
   char varrtf[MAXLENFILENAME];


   if(chiudi_simul == True)
   {
      if( XlGetenv(ENVRTF) != NULL)
      {
         strcpy(filertf,XlGetenv(ENVRTF));
         strcat(filertf,"/variabili.rtf");
      }
      else
         return;

      strcpy(filertf,XlConvPathVms(filertf));
      if( file_exist(filertf) != NULL)
      {
         if(OlDestroyDatabaseTopologia(dbtopologia) == False)
            return;

         system("killsim");
         setPageditEnv(pagedit);

         if( XlGetenv(ENVRTF) != NULL )
         {
         strcpy(varrtf,XlGetenv(ENVRTF));
         strcat(varrtf,"/variabili.rtf");
         strcpy(varrtf,XlConvPathVms(varrtf));

         strcpy(varsub,XlGetenv(ENVRTF));
         strcat(varsub,"/FileSubSystem.edf");
         strcpy(varsub,XlConvPathVms(varsub));
         }

         if( XlGetenv(ENVRTF) != NULL )
         {
         strcpy(varcc,XlGetenv(ENVCTX));
         strcat(varcc,"/FileDefVarCC.edf");
         strcpy(varcc,XlConvPathVms(varcc));

         strcpy(varcom,XlGetenv(ENVCTX));
         strcat(varcom,"/FileComponent.edf");
         strcpy(varcom,XlConvPathVms(varcom));
         }

         if( getenv("SHR_USR_KEY") == NULL)
         {
            CompilerOk = False;
            return;
         }

         if( (dbtopologia = OlCreateDatabaseTopologia("olrtf",varrtf,atoi(getenv("SHR_USR_KEY")),
                                varsub,varcom,varcc)) == NULL)
         {
             SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Database Not Active",NULL,False,NULL,False,NULL);
             /* disabilita menu' compile */
             CompilerOk = False;
         } 
         chiudi_simul = False; 
      }
   }
   else
      CompilerOk = False;

   setPageditEnv(pagedit);
}


/*-------------------------------------
 *  TagPagCalcNext
 *
 * calcola il valore successivo a Tag 
 *
 * N.B. non valuta il raggiungimento di ZZ
 *      se capitasse questa routine non funziona 
 *---------------------------------------*/
/*
 prossimoChar
   restituisce il prossimo carattere o il valore zero
   se il set di caratteri e' esaurito
*/
char prossimoChar(char c)
{
if ((c >= ASCII0 && c < ASCII9) ||
    (c >= ASCIIA && c < ASCIIZ) ) 
	c++;
else
	{
	if (c== ASCII9)
		c= ASCIIA;
	else if (c==ASCIIZ)
		c='\0';
	}
return(c);
}
	
Boolean TagPagOK(char *Tag)
{
if(strlen(Tag)!= 2)
	return(False);
if (
	( (Tag[1] >= ASCII0 && Tag[1] <= ASCII9) ||
          (Tag[1] >= ASCIIA && Tag[1] <= ASCIIZ) )
	&&
	( (Tag[0] >= ASCII0 && Tag[0] <= ASCII9) ||
          (Tag[0] >= ASCIIA && Tag[0] <= ASCIIZ) ) 
   )
	return(True);
else
	return(False);
}


int TagPagCalcNext(char *Tag,char *nextTag)
{
if( ! TagPagOK(Tag) )
	return(2);
nextTag[1]=prossimoChar(Tag[1]);
if(nextTag[1] == '\0') /* tag == 'Z' riparto incrementando il numero a sin */
      {
      nextTag[0]=prossimoChar(Tag[0]);
      if(nextTag[0] == '\0') return(1);
      nextTag[1] = ASCII0;  /* '0' */
      }
nextTag[2]=0; /* chiusura stringa */
return(0);
}

/*--------------------------------------------
 * TagPagSetNew
 *
 * alloca un nuovo elemento nella lista
 * e incrementa di conseguenza il nextTagPag
 *--------------------------------------------*/

int TagPagSetNew(PAGEDIT_CONTEXT *ctx,int tipo)
{
return(TagPagCalcNext(ctx->nextTagPag,ctx->nextTagPag));
}

/*-----------------------------------------
 * nextTagPagGet
 *
 * ritorna la la nextTagPag      
 *-----------------------------------------*/

char *nextTagPagGet(PAGEDIT_CONTEXT *ctx)
{
   return(ctx->nextTagPag);
}


Boolean ContextLoad(char *nomeCtx,PAGEDIT_CONTEXT **pagedit)
{
   extern Boolean ContextLoaded;
   extern Boolean ContextModified;
   PAGEDIT_CONTEXT *appo;


   if( !pagedit_context_init(nomeCtx,pagedit) ) 
   {
      ContextLoaded=False;
      return(ContextLoaded);
   }
   if( !pagedit_context_getres(*pagedit) )
   {
      ContextLoaded=False;
      return(ContextLoaded);
   }
   if( !setPageditEnv(*pagedit) )
   {
      ContextLoaded=False;
      return(ContextLoaded);
   }

   ContextLoaded = True; 
   ContextModified = False; 

   return(ContextLoaded);
}

/* crea il file context 26-9-96 Micheletti 
   in questa versione le directory devono gia' esistere
*/
int crea_context(char *nome_ctx,char *path_filectx, 
                 char *path_prj, char *descr, 
                 char *path_pages, char *path_objlib, 
                 char *path_iconlib, char *path_sim, 
                 char *display_list, char *hostname, char *hostnameS)
{

   FILE *fp;
   char filename[FILENAME_MAX];
   char *appo;
   char appo2[255];
   int num_display=0;

/* verifica la accessibilita la esistenza e accessibilita delle directory
*/

   
   if( access(path_filectx,R_OK|W_OK|X_OK) != 0)
      return( FALSE ); 
   if( access(path_prj,R_OK|W_OK|X_OK) != 0)
      return( FALSE ); 
   if( access(path_pages,R_OK|W_OK|X_OK) != 0)
      return( FALSE ); 
   if( access(path_objlib,R_OK|W_OK|X_OK) != 0)
      return( FALSE ); 
   if( access(path_iconlib,R_OK|W_OK|X_OK) != 0)
      return( FALSE ); 

/* emette solo un messaggio perche' e' lecito che non esista gia'
*/
   if( access(path_sim,R_OK|W_OK|X_OK) != 0)
      printf("Simulator sirectory not found\n");
   
/* costruisco il filename */
   sprintf(filename,"%s/%s",path_filectx,nome_ctx);

/* crea il file context, altrimenti esce con errore */
   if( (fp=fopen(filename,"w")) == NULL)
   {
      printf("Cannot create %s\n",filename);
      return(FALSE);
   }

   /* calcolo il numero di display impostati */
   appo=display_list;
   if( appo != NULL && strlen(appo) )
   {
      /* skip blank  */
      while( *appo == ' ' )
         appo++;
  
      /* conto quante stringhe ci sono in display_list */     
      while( sscanf(appo2,"%s",appo) )
      {
         if( strlen(appo2) )
         {
            num_display++;
            appo += strlen(appo2);
         }

         /* skip blank  */
         while( *appo == ' ' )
            appo++;
      }
      
       
   }

/* scrivo nel file context i valori passati */
   fprintf(fp,"*numTagPag:   0\n");
   fprintf(fp,"*displayList:   %s\n",display_list);
   fprintf(fp,"*objectLibraries:   %s\n",path_objlib);
   fprintf(fp,"*page_list:   \n"); 
   fprintf(fp,"*numDisplay:   %d\n",num_display); 
   fprintf(fp,"*simulator:   %s\n",path_sim);
   fprintf(fp,"*description:   %s\n",descr);
   fprintf(fp,"*iconlib_num:   0\n");

   fprintf(fp,"*iconlib_list:   \n");
   fprintf(fp,"*pag_num:   0\n");
   fprintf(fp,"*animatedIconLibraries:   %s\n",path_iconlib);
   fprintf(fp,"*iconlib_label:   \n");
   fprintf(fp,"*pages:   %s\n",path_pages);
   fprintf(fp,"*nextTagPag:   00\n");
   fprintf(fp,"*hostName:   %s\n",hostname);
   fprintf(fp,"*hostNameS:   %s\n",hostnameS);

   fclose(fp);
   return(TRUE);
}

/* 21-10-96 Carlo 
   verifica_direttori_context controlla gli attributi delle directory
                      essere utilizzate per l'environment 
*/
int verifica_direttori_context(PAGEDIT_CONTEXT *ctx)
{
   char mesg[200];


   

   /* CAPPE if( access(ctx->objectLibrary,R_OK|W_OK|X_OK) != 0 ) */
   if( access(ctx->objectLibrary,R_OK) != 0 )
   {
      sprintf(mesg,"Object Library Directory: %s Not Found \n",
                         ctx->objectLibrary);
      show_message(mesg);
      return(False);
   }


   /* CAPPE if( access(ctx->animatedIconLibrary,R_OK|W_OK|X_OK) != 0) */
   if( access(ctx->animatedIconLibrary,R_OK) != 0)
   {
      sprintf(mesg,"Icon Library Directory: %s Not Found \n",
                        ctx->animatedIconLibrary);
      show_message(mesg);
      return(False);
   }


   if( access(ctx->pages,R_OK|W_OK|X_OK) != 0)
   {
      sprintf(mesg,"Pages Directory: %s Not Found \n",
                       ctx->pages);
      show_message(mesg);
      return(False);
   }



   if( access(ctx->simulator,R_OK|W_OK|X_OK) != 0)
   {
      sprintf(mesg,"Simulator Directory: %s Not Found \n",
                       ctx->simulator);
      show_message(mesg);
   }


   return(True);
}


/* CAPPE: Funzione provvisoria per l'inserimento delle nuove risorse
   nella struttura di tipo PAGEDIT_CONTEXT a partire dalla struttura
   di tipo PAGINA							 */
Boolean AddResPageContext(PAGEDIT_CONTEXT *pagctx, int i)
{
   Boolean	error = False;
   char		*tipo, risorsa[100], appo[100];
   XrmValue 	value;
   int		error_count=0;

   /* Controllo per vedere se il file Context.ctx e' nel vecchio formato */
   
   if((pagctx->res_page[i] = (INFO_PAGE *)alloca_memoria( 1, sizeof(INFO_PAGE) )) == NULL)
   {
      printf("AddResPageContext. Errore nella alloca_memoria!!\n");
      error = True; 
   }
   else {
      /* Caricamento delle risorse relative alla pagina corrente */
      sprintf(risorsa,"%s.top_descrizione", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, (char *)NULL, &tipo, &value) == 0)
      { 
	 /* error = True;
	 error_count++;  CAPPE */
	 strcpy(pagctx->res_page[i]->descr_page,"");
      }
      else
         strncpy(pagctx->res_page[i]->descr_page,value.addr,(int)value.size);
      sprintf(risorsa,"%s.top_tipo", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, NULL, &tipo, &value) == 0)
      {
         /* error = True;
         error_count++; CAPPE */
	 strcpy(pagctx->res_page[i]->type_page,"");
      }
      else
         strncpy(pagctx->res_page[i]->type_page,value.addr,(int)value.size);

      sprintf(risorsa,"%s.tagPag", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, NULL, &tipo, &value) == 0)
      {
         /* error = True;
	 error_count++;  CAPPE */
	 strcpy(pagctx->res_page[i]->tagPag,"");
      }
      else
         strncpy(pagctx->res_page[i]->tagPag,value.addr,(int)value.size);

      sprintf(risorsa,"%s.refresh_freq", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, NULL, &tipo, &value) == 0)
      {
         /* error = True;
	 error_count++; CAPPE */
	 pagctx->res_page[i]->refresh_freq = 0;
      }
      else
      {
         strncpy(appo,value.addr,(int)value.size);
         pagctx->res_page[i]->refresh_freq = atoi(appo);
      }

      sprintf(risorsa,"%s.schemeInUse", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, NULL, &tipo, &value) == 0)
      {
         /* error = True;
	 error_count++; CAPPE */
	 pagctx->res_page[i]->in_use = 0;
      }
      else
      {
         strncpy(appo,value.addr,(int)value.size);
         pagctx->res_page[i]->in_use = atoi(appo);
      }

      sprintf(risorsa,"%s.gerarchia", pagctx->page_list[i]);
      if( XrmGetResource(pagctx->db, risorsa, NULL, &tipo, &value) == 0)
      {
         /* error = True;
	 error_count++; CAPPE */
	 strcpy(pagctx->res_page[i]->hier,"");
      }
      else
         strncpy(pagctx->res_page[i]->hier,value.addr,(int)value.size);
   }
   
   return(error);

}



/* CAPPE. Funzione che allinea i valori delle risorse relative alle
   pagine presenti nel context con i corrispondenti presenti nei files
   .pag.  */
/*Boolean AlignResPageContext(PAGEDIT_CONTEXT *pagctx)
{
PAGINA *pag;
XmString item;
int    i;


   for(i=0;i<pagctx->pag_num;i++)
   {
      if( pagina_init(pagctx->page_list[i],&pag,TIPO_PAGINA) == True )
      {
         pagina_load_file(pag,TIPO_PAGINA);
         GetResPaginaDb(pag,pag->db,True);
         AddResPageContext(pag, pagctx, i);
         SetItemString(&item,pagctx->page_list[i],pagctx->res_page[i]->type_page,pagctx->res_page[i]->descr_page,pagctx->res_page[i]->in_use,pagctx->res_page[i]->tagPag);
         add_item(item,0) ;
	 pagina_free(pag);
      }
      else
         printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",pagctx->page_list[i])
;
   }

   if( new_save_context( pagedit ) != True )
            SetMsg(topLevelShell,NULL,INFMSG,
                       "ConfigInfo","Save of Context failed.",
                       NULL,False,NULL,False,NULL);
   

}   CAPPE */


/******************************** end of file ****************************/
