/*
   modulo modallpag.c
   tipo 
   release 1.1
   data 1/8/96
   reserved @(#)modallpag.c	1.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)modallpag.c	1.1\t1/8/96";
/*
        Fine sezione per SCCS
*/
/*                  modallpag.c   S.Balistreri

 *programma di utilita' che consente di modificare per tutte le pagine
 *contenute nel Context.ctx(fornito dall'utente)i colori delle soglie */



#include<stdio.h>
#include<string.h>
#include<stdlib.h>     /*necessario per utilizzo funzione system*/


#define DIM 200        /*dimensione max della linea da leggere da file*/


char comando[200];


main (argc,argv)
int argc;
char *argv[];
{
int y,ritmen1;
int i,t,num,numpag=0,trovato=0,okdir=0,trovnum=0,rewok=0;

char linea[DIM],stringa[60];    /*linea letto da file Context.ctx*/
                                /*stringa letta da linea*/

char colorselez[30];

char appdir[DIM];               /*appdir contiene direttorio
                                 */

char pathcontext[50];           /*direttorio del Context.ctx*/

#ifdef VMS
  char context[]="Context.ctx";  /*stringa da aggiungere al direttorio*/
  char parampath[DIM];
#else
  char context[]="/Context.ctx";
#endif


char keydir[]="*pages:";        /*contenuta nel Context e di seguito
                                 *riporta direttorio pagine
                                 */ 

char keylist[]="*page_list:";   /*nel context individua la lista 
                                 *delle pagine
                                 */    

char keypag[]="*pag_num:";      /*nel context individua il numero
                                 *delle pagine
                                 */

char* ptrstr;                   /*la strstr ritorna puntatore alla stringa*/
char* ptrlist;  
char* ptrpag;                
char* iniziolistpag;                 

FILE *fp;



if (argc==1)
{
  printf ("INSERIRE PATH COMPLETO DIRETTORIO DOVE RISIEDE IL CONTEXT.CTX:\n");
  /* fgets e non gets: gets non ha limite, e un path lungo sforava */
  if (fgets (pathcontext, sizeof(pathcontext) - strlen("/Context.ctx"), stdin) == NULL)
    exit (1);
  pathcontext[strcspn (pathcontext, "\n")] = '\0';

 /* per UNIX
  *esempio pathcontext=/usr/users/pag
  *aggiungo a pathcontext la stringa /Context.ctx
  *e il risultato si trova in pathcontext ed e'
  *pathcontext=/usr/users/pag/Context.ctx
  *per VMS
  *esempio pathcontext=[usr.users.pag]
  *aggiungo a pathcontext la stringa Context.ctx
  *e il risultato si trova in pathcontext ed e'
  *pathcontext=[usr.users.pag]Context.ctx
  */

  strcat(pathcontext,context);     /*in pathcontext ho dir+file*/

  /*apro il file in lettura*/
  if((fp=fopen(pathcontext,"r"))==NULL) {
       printf ("il Context.ctx non esiste nel direttorio specificato\n");
       return(-1);
  }
   
    while ((fgets(linea,DIM-1,fp))!=NULL)
    {
    /* ho letto una linea e devo verificare se esiste parola chiave*/

       ptrstr=strstr(linea,keydir);
       ptrpag=strstr(linea,keypag);

       if(trovnum!=1)
       {
          if(ptrpag!=NULL)
          {
            /*ho trovato parola chiave che identifica il numero delle pagine */
            trovnum=1;
            ptrpag+=strlen(keypag);
            sscanf(ptrpag,"%s",stringa);
            numpag=atoi(stringa);
          }
       }
       if(okdir!=1)
       {
          if (ptrstr!=NULL)
          {
            /*ho trovato parola chiave che identifica il direttorio*/
            okdir=1;
            y=strlen(linea);
            for(i=8,t=0;i<y-1;i++)
            {
               appdir[t]=linea[i];
               t++;
            }
#ifdef VMS
       strcpy(parampath,"\"");
       strcat(parampath,appdir);
       strcat(parampath,"\"");
       strcpy(appdir,parampath);
#endif
          }
       }
       printf("mi fermo prima della rewind\n");
       if ((okdir==1)&&(trovnum==1)&&(rewok!=1))
       {
          rewind(fp);
          rewok=1;
       }
       ptrlist=strstr(linea,keylist);
       if(trovato!=1)
       {
          if ((ptrlist!=NULL)&&(okdir==1)&&(trovnum==1))
          {
             /*ho trovato parola chiave che identifica lista pagine*/
             trovato=1;
             ptrlist+=strlen(keylist);
             iniziolistpag=ptrlist;
             do
             {
                ptrlist=iniziolistpag;  /*riporto il puntatore a inizio lista*/
                ritmen1=menu1();
                if (ritmen1!=0)
                {
                printf("inserire colore desiderato(es formato-->ffffffffffff)o white:\n");
                   scanf("%s",colorselez);
                   for(num=0;num<=numpag;num++)
                   {
                      sscanf(ptrlist,"%s",stringa);
                      ptrlist+=strlen(stringa);
                      ptrlist++;
                     if(stringa[0]!='\\')
                      {
                         /*
                         devo escludere caso "\" prima della
                         lista delle pagine ,vedi *pages_list
                         in Context.ctx
                         */
                         sprintf(comando,"%s %s %s %d %s","modpag",appdir,stringa,ritmen1,colorselez);
                         printf("comando=%s\n",comando);
                         system (comando);
                      }
                   }
                }
             }
          while(ritmen1!=0);
          }
       }
  }/*while*/
  fclose(fp);
                                                                                
    
} /*if argc*/

} /* main */

