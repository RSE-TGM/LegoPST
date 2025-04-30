/**********************************************************************
*
*       C Source:               regolazione.c
*       Subsystem:              8
*       Description:
*       %created_by:    mauric %
*       %date_created:  Fri Mar  6 14:53:08 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: regolazione.c-9 %  (%full_filespec: regolazione.c-9:csrc:8 %)";
#endif
/* modulo regolazione.c
   tipo 
   release 5.9
   data 3/19/96
   reserved @(#)regolazione.c	5.9
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)regolazione.c	5.9\t3/19/96";
/*
        Fine sezione per SCCS
*/
/*
 regolazione.c 
 Contiene routines tipiche della applicazione REGOMMI
 dove si utilizzano strutture particolari tipiche
 della regolazione in ambiente LEGO
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <UxXt.h>
#include "config.h"
#include "message.h"
#include "forme.h"     /* per topologia schemi regolazione */
#include "top_icon.h"  /* per topologia schemi regolazione */
#include <Xl/Xl.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xd/Xd.h>
#include <Ol/OlForm.h>
#include <Ol/OlForm_regol.h>
#include <Xm/DrawingA.h>
#include <Ol/OlConn.h>

#include <sys/types.h>
#include <sys/stat.h>

/*
   Variabili esterne
*/
extern Display *UxDisplay;
extern int UxScreen;
extern Arg args[];
extern Cardinal lnargs;
extern GC xorGC;
extern swidget topLevelShell;
extern Boolean StateInterfaceMode;
extern OlConnObject conn_obj; /* oggetto OlConn (gestione connessioni) */


/*  
   Funzioni per la costruzione dei macromoduli
*/
int scrivi_connessioni(PAGINA *, char * , char *, char *);
extern int OlFindConnectionByPort(OlConnObject , char *, char* ,char *);



/*  ***** FUNZIONI PER LA GESTIONE I/O PER MACROMODULI ***** */
/*
1) scrivi_connessioni
*/

/*  
     Raccoglie dalle icone di interfaccia della drawing area
     informazioni relative alle connessioni in un vettore di 
     strutture (tipo VAR_CONN) che viene scritto su file.
     Tali informazioni vengono lette dal programma gen_macromod.
*/
int scrivi_connessioni(PAGINA *pg, char *nome_file, char *file_err,
                       char *file_connessioni)
{
VAR_CONN *v_cn=NULL;
FILE *fp;
WidgetList Wlist,child;
Widget wid_icon_conn;
Widget wid_porta_conn;
Cardinal nwid,num_child;
int i,j,k,h;
int n_var;
int narg;
Arg arg[8];
char *str_tag;
char *nome_var;
char *nome_porta;
char *porta_conn;
char messaggio[200];
int n_conn;
int n_connessione;
int interf_port;
char var_conn_icn_interf[5];


/* Figli della darwing area */
   get_child(pg->drawing,&Wlist,&nwid);

/* Creo l'oggetto Conn */
   if(!StateInterfaceMode)
      conn_obj=OlCreateConn(file_connessioni);

/* Conto le variabili connesse all'esterno dello schema */
   n_conn = 0;
   interf_port = 0;
   for(i=0;i<nwid;i++)
      {
      if(XlIsInterfaceIconReg(Wlist[i]))
         {
         interf_port = 1;
         get_child(Wlist[i],&child,&num_child);
         for(j=0;j<num_child;j++)
             {
             if(XlIsPort(child[j]))
                {
/*              Test sulla connessione ad un'altro schema */
                if( (n_connessione=
                     OlFindConnectionByPort(conn_obj, XtName(child[j]),
                                            XtName(Wlist[i]),pg->nomepag))>=0)
                    {
/*                  Distruggo l'oggetto conn */
                    if(!StateInterfaceMode)
                       OlDestroyConn(conn_obj);
                    sprintf(messaggio,
                            "Lo schema %s ha icone di interfaccia connesse",
                            pg->nomepag);
                    XlOpenFileErrComp(file_err);
                    XlErrComp(NULL,"Write schema_macro.reg","ATTENZIONE\n ",
                              messaggio);
                    XlSetFileErrComp(NULL);
                    return(-1);
                    }
/*              Ricavo il nome della porta e quello della porta connessa */
                narg=0;
                XtSetArg(arg[narg],XlNportName,&nome_porta);narg++;
                XtSetArg(arg[narg],XlNportNameConnect,&porta_conn);narg++;
                XtGetValues(child[j],arg,narg);
                if( strlen(porta_conn)!=0  )
                    n_conn++;
                }
             }
         }
      }
   /*printf("\n   scrivi_connessioni nwid=%d n_conn=%d\n",nwid,n_conn);*/

/* Distruggo l'oggetto conn */
   if(!StateInterfaceMode)
      OlDestroyConn(conn_obj);

/* Contollo sul numero di connessioni */
   if( (interf_port==0) )
      {
      fprintf(stderr,
             "ATTENZIONE schema senza icone di interfaccia, non si genera macromodulo\n");
      sprintf(messaggio, "Lo schema %s non ha icone di interfaccia ",
              pg->nomepag);
      XlOpenFileErrComp(file_err);
      XlErrComp(NULL,"Write schema_macro.reg","ATTENZIONE\n ",
                messaggio);
      XlSetFileErrComp(NULL);
      return(-1);
      }
   if( (n_conn==0)||(n_conn>M_CONN) )
      {
      fprintf(stderr,
              "ATTENZIONE scrivi_connessioni: n. connessioni errato (%d)\n",n_conn);
      sprintf(messaggio, "Lo schema %s ha un numero errato (%d) di connessioni",
              pg->nomepag,n_conn);
      XlOpenFileErrComp(file_err);
      XlErrComp(NULL,"Write schema_macro.reg","ATTENZIONE\n ",
                messaggio);
      XlSetFileErrComp(NULL);
      return(-1);
      }
  
/* Alloco la struttura da scrivere su file */
   v_cn = (VAR_CONN*)calloc(n_conn,sizeof(VAR_CONN));
  
/* Carico i nomi delle variabili connesse ad un icona di interfaccia */
   n_var = 0;
   for(i=0;i<nwid;i++)
      {
      if(XlIsInterfaceIconReg(Wlist[i]))
         {
/*       Carico i figli dell'icona di regolazione considerata */
         get_child(Wlist[i],&child,&num_child);
         for(j=0;j<num_child;j++)
             {
             if(XlIsPort(child[j]))   /* Non c'e differenza tra in ed out */
                {
/*              Ricavo il nome della porta e quello della porta connessa */
                narg=0;
                XtSetArg(arg[narg],XlNportName,&nome_porta);narg++;
                XtSetArg(arg[narg],XlNportNameConnect,&porta_conn);narg++;
                XtGetValues(child[j],arg,narg);

/*              Porta connessa : risalgo alla variabile che le e' collegata */
                /*
                printf("porta_conn=%s| nome_porta=%s|",porta_conn,nome_porta);
                */

/*              Test sulla connessione ad un'altro schema */
                if( strlen(porta_conn)!=0  )
                    {
/*                  Ricavo l'icona connessa e la porta che e' connessa */
                    wid_icon_conn=get_wid_icon_conn(pg->drawing,
                                                    porta_conn);
                    wid_porta_conn=get_wid_port(wid_icon_conn,
                                                    porta_conn);

/*                  Ricavo la tag della variabile connessa */
                    narg=0;
                    XtSetArg(arg[narg],XlNtagName,&str_tag);narg++;
                    XtGetValues(wid_icon_conn,arg,narg);
                    /*
                    printf("Tag icona conn=%s|\n",str_tag);
                    */

/*                  Ricavo il nome della variabile associata alla porta */
                    narg=0;
                    XtSetArg(arg[narg],XlNportName,&nome_var);narg++;
                    XtGetValues(wid_porta_conn,arg,narg);
                    /*
                    printf("nome variabile associata =%s|\n",nome_var);
                    */
                    strcpy(var_conn_icn_interf,nome_var);
                    strncat(var_conn_icn_interf,str_tag,2);
                    var_conn_icn_interf[5] = '\0';

/*                  Carico la struttura descrittiva della connessione */
                    copy_n_car(v_cn[n_var].nome_var_conn,var_conn_icn_interf,4);
                    n_var++;
                    }
                }
             }       /* Fine ciclo su tutti i figli di una icon_reg */
         }
      }              /* Fine ciclo su tutti i figli della drawing */



/*
   printf("Riepilogo : numero_connessioni = %d\n",n_conn);
   for(i=0;i<n_conn;i++)
      printf("v_cn[%d]=%s|\n",i,v_cn[i].nome_var_conn);
*/

/* Scrittura su file */
   fp = fopen(nome_file,"w");
   if( fp==NULL)
      {
      fprintf(stderr,"ATTENZIONE scrivi_connessioni, non si scrive il file %s\n",
              nome_file);
      if( v_cn!=NULL )
         free(v_cn);
      return(-1);

      }
   fwrite(&n_conn,sizeof(int),1,fp);
   fwrite(v_cn,sizeof(VAR_CONN),n_conn,fp);
   fclose(fp);

/* Libero la struttura scritta su file */
   if( v_cn!=NULL )
      free(v_cn);

   /*fprintf(stderr,"Scritto il file %s\n",nome_file);*/

   return(1);
}

/* Fine file */
