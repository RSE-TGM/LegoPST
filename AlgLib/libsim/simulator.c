/**********************************************************************
*
*       C Source:               simulator.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:36:49 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: simulator.c-5 %  (%full_filespec: simulator.c-5:csrc:1 %)";
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
#include <Rt/RtMemory.h>
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>





int   AlgRicercaFile();
int   AlgEstrazWords(int);
int   EstrazWords();
int   ReadSimulator(int);


#define FSNAP "snapshot.dat"
#define FBKTK "backtrack.dat"
#define FF22C "f22circ.dat"
/* dove finisce un f22circ.dat incompatibile, messo da parte da ControlParam */
#define FF22C_SCARTATO "f22circ.dat.incompatibile"
/* dove finiscono snapshot.dat e backtrack.dat salvati con un'altra topologia */
#define FSNAP_SCARTATO "snapshot.dat.incompatibile"
#define FBKTK_SCARTATO "backtrack.dat.incompatibile"
#define WARN  "WARNING"
#define SEVR  "SEVERE"
char nome_proc[50];
/*
                                 *********************
                                 METTERE IN UN INCLUDE
                                 ********************* 
*/
extern int _MAX_SNAP_SHOT;
extern int _MAX_BACK_TRACK;
extern int _MAX_CAMPIONI;
extern int _NUM_VAR;
extern int _MAX_PERTUR;
extern int _SPARE_SNAP;
extern int _PERT_CLEAR;
#define NOMEFILESIM "Simulator"
#define ALTRE_RICERCHE 3
#define MAX_STRINGHE 10

typedef struct algoricerca_st
        {
        char path[FILENAME_MAX];
        }ALGORICERCA;
ALGORICERCA algoritmo[ALTRE_RICERCHE]=
            {
            "~/",
            "~/defaults/",
            "$LEGO/procedure/",
            };

char stringhe[MAX_STRINGHE][20]=
     {
     "MAX_SNAP_SHOT",
     "MAX_BACK_TRACK",
     "MAX_CAMPIONI",
     "NUM_VAR",
     "",
     "",
     "",
     "",
     "",
     ""
     };


extern RtDbPuntiOggetto dbpunti;



/* ****************************************
 * Dedicato alla lettura del file Simulator
 * **************************************** */
/*
      Questa funzione provvede a caricare le variabili d'environment
      a partire da file Simulator.
      Se queste non sono definite esegue le seguenti operazioni:
      1) richiama la routine di ricerca e di copia del file
      2) lettura 
      3) setting delle variabili d'environment
      4) caricamento delle variabili definite.
      Se modo vale 1 forza la lettura del file.
*/
int   ReadSimulator(int modo)
{
int ret;

      /*printf("\t ReadSimulator con parametro %d\n",modo);*/
/*    Non e' in modalita' di forzamento */         
      if( modo != 1 )
        {
        ret = EstrazWords();
        if( ret>=0 )
          {
          return(ret);
          }
        printf("\t Fallita EstrazWords (%d)\n",ret);
        }

      ret = AlgRicercaFile();
      if( ret>=0 )
      ret = AlgEstrazWords(ret);
      if( ret<0 )
          fprintf(stderr,"Non trovato o non corretto il file Simulator\n");
}



/*
      Funzione di ricerca del file Simulator.
      Ricerca: se non viene trovato nella directory locale al simulatore
      viene ricercato, secondo l'ordine di ricerca fissato nella struttura
      algoritmo di tipo ALGORICERCA, e poi ricopiato localmente.
      Codici di ritorno:
		 0   file localE
	 	 1   file in altra posizione , ricopiato localmente
		-1   file non trovato
*/
int   AlgRicercaFile()
{
FILE *fp=NULL;
int ret,i,j,ii,lhome;
int errore=0;
char appoggio[FILENAME_MAX];
char nome_file[FILENAME_MAX];
char comando[2*FILENAME_MAX];
char *punt;


/*    Ricerca nella directory locale */
      fp = (FILE*)fopen(NOMEFILESIM,"r");
      if ( fp!=NULL )
         ret = 0;
      else
         {
/*       Altre ricerche a partire da HOME directory */
         for(i=0;i<ALTRE_RICERCHE;i++)
            {
            switch(algoritmo[i].path[0])
               {
               case '~' :
                  {
                  strcpy(nome_file,getenv("HOME"));
                  lhome = strlen(nome_file);
                  strcpy(&nome_file[lhome],&algoritmo[i].path[1]);
                  strcat(nome_file,NOMEFILESIM);
                  break;
                  }
               case '$' :
                  {
                  strcpy(appoggio,&algoritmo[i].path[1]);
                  ii=0;
                  while( (appoggio[ii]!='/') && (ii<strlen(appoggio)) )
                      ii++;
                  appoggio[ii] = 0;
                  strcpy(nome_file,getenv(appoggio));
                  punt = strstr(algoritmo[i].path,"/");
                  strcat(nome_file,punt);
                  strcat(nome_file,NOMEFILESIM);
                  strcat(nome_file,".tpl");
                  lhome = strlen(nome_file);
                  break;
                  }
               default :
                  {
                  printf("Errore sintassi ricerca file\n");
                  errore = 1;
                  break;
                  }
               }
            if( errore == 1 )
                break;

            printf("%d)\n",i);
            printf("Cerco [%s] (%d)\n",nome_file,strlen(nome_file));
            fp = (FILE*)fopen(nome_file,"r");
            if ( fp!=NULL )
               {
               strcpy(comando,"cp ");
               strcat(comando,nome_file);
               strcat(comando," ");
               strcat(comando,NOMEFILESIM);
               system(comando);
               /*printf("TROVATO [%s] (%d)\n",nome_file,strlen(nome_file));*/
               break;
               }
            }
         if( errore )
             return(-1);
         if( i==ALTRE_RICERCHE )
            ret = -1;
         else
            ret = 1;
         }
      if( ret>=0 )
          fclose(fp);
      return(ret);
}


/*
      Setta le variabili d'ambiente leggendo il file.
      Controlli:
       1 conta le righe del file che corrispondono alle variabili da settare
       2 controlla per ciascuna riga la presenza dei caratteri "." e ":"
       3 elimina i blank e i ritorni a capo
*/
int   AlgEstrazWords(int posfile)
{
FILE *ff;
int i=0,ret=0,nrighe=0;
char riga[FILENAME_MAX];
char app1[FILENAME_MAX];
char app2[FILENAME_MAX];
char *punt1,*punt2,*app_punt,*val;
char **venv;




      ff = fopen(NOMEFILESIM,"r");
      if( ff==NULL )
         exit(fprintf(stderr,"NON ESISTE\n"));
/*    Conto le righe */
      nrighe=0;
      while( fgets(riga,FILENAME_MAX,ff) )
         {
         if( riga[0] != '#' )
           nrighe++;
         }
      rewind(ff);
      venv = cdim2(nrighe,FILENAME_MAX*sizeof(char));
      i=0;
      while( fgets(riga,FILENAME_MAX,ff) )
         {
         if( riga[0] == '#' )
             continue;
         punt1=strstr(riga,".");
         punt2=strstr(riga,":");
         if( (punt1==NULL) || (punt2==NULL) )
             {
             printf("File %s in formato non corretto alla riga %d\n",
                     NOMEFILESIM,i+1);
             ret = -1;
             break;
             }
         punt1++;
         app_punt = punt2;
         punt2++;
         *app_punt = 0;
         while( *punt1 == ' ' )
             punt1++;
         while( *punt2 == ' ' )
             punt2++;
         strcpy(app1,punt1);
         strcpy(app2,punt2);
         if( app2[strlen(app2)-1] == '\n' )
             app2[strlen(app2)-1] = 0;
         while( app1[strlen(app1)-1] == ' ' )
             app1[strlen(app1)-1] = 0;
         while( app2[strlen(app2)-1] == ' ' )
             app2[strlen(app2)-1] = 0;
         sprintf(venv[i],"%s=%s",app1,app2);
         /*
         printf("venv=[%s] (%d)\n",venv[i],strlen(venv[i]));
         */
         putenv(venv[i]);

         i++;
         if( i>nrighe )
           {
           printf("Errori in fase di letture [%s]\n",NOMEFILESIM);
           ret = -1;
           break;
           }
         }

/*    Fisso le variabili */
      for(i=0;i<nrighe;i++)
         {
         strcpy(app1,venv[i]);
         punt1 = strstr(app1,"=");
         *punt1 = 0;
         /*printf("variabile=[%s]=[%s]\n",app1,getenv(app1));*/
         }

      ret = EstrazWords();
/*
       val = getenv("MAX_SNAP_SHOT");
       if(val != NULL)
          _MAX_SNAP_SHOT=atoi(val);

       val = getenv("MAX_BACK_TRACK");
       if(val != NULL)
          _MAX_BACK_TRACK=atoi(val);

       val = getenv("MAX_CAMPIONI");
       if(val != NULL)
          _MAX_CAMPIONI=atoi(val);

       val = getenv("NUM_VAR");
       if(val != NULL)
          _NUM_VAR=atoi(val);

       val = getenv("MAX_PERTUR");
       if(val != NULL)
          _MAX_PERTUR=atoi(val);

       val = getenv("SPARE_SNAP");
       if(val != NULL)
          _SPARE_SNAP=atoi(val);
*/

      return(ret);
}


/*
      Legge le variabili d'ambiente e le fissa nelle define.
*/
int   EstrazWords()
{
char *val;

       val = getenv("MAX_SNAP_SHOT");
       if(val != NULL)
          _MAX_SNAP_SHOT=atoi(val);
       else
          return(-1);

       val = getenv("MAX_BACK_TRACK");
       if(val != NULL)
          _MAX_BACK_TRACK=atoi(val);
       else
          return(-1);

       val = getenv("MAX_CAMPIONI");
       if(val != NULL)
          _MAX_CAMPIONI=atoi(val);
       else
          return(-1);

       val = getenv("NUM_VAR");
       if(val != NULL)
          _NUM_VAR=atoi(val);
       else
          return(-1);

       val = getenv("MAX_PERTUR");
       if(val != NULL)
          _MAX_PERTUR=atoi(val);
       else
          return(-1);

       val = getenv("SPARE_SNAP");
       if(val != NULL)
          _SPARE_SNAP=atoi(val);
       else
          return(-1);

      printf("\t EstrazWordFile %d %d %d %d %d %d\n",
            _MAX_SNAP_SHOT, _MAX_BACK_TRACK, _MAX_CAMPIONI, 
            _NUM_VAR, _MAX_PERTUR,_SPARE_SNAP);

      return(0);
}


/*
    Questa routine controlla che i parametri letti dal file Simulator
    siano coerenti con quelli slvati nel file delle registrazioni
    attive. 
    File controllati e codice d'errore:        
      _______ >  f22circ.dat
      | _____ >  backtrack.dat
      | | ___ >  snapshot.dat
      | | |  

0     0 0 0
1     0 0 1
2     0 1 0
3     0 1 1
4     1 0 0
5     1 0 1
6     1 1 0
7     1 1 1
*/
/*
   La topologia ATTUALE del simulatore, letta da S02_LINUX come la legge il
   database dei punti (RtCreateDbPunti, file_top) - senza toccare la memoria
   condivisa, che a questo punto puo' non esserci ancora. Per ogni task le
   quattro grandezze (stati, ingressi, derivate, dati), e in *size la
   dimensione in byte dell'area dati di tutte le task, cioe' quella che
   snapshot.dat e backtrack.dat registrano nell'intestazione.
   Ritorna il numero di task, o -1 se il file non si legge (e allora il
   controllo della topologia non si fa).
*/
static int TopologiaAttuale(NUMERI_MODELLI *num, int *size)
{
FILE  *fp;
S02   *top;
int    i,nmod;

    *size = 0;
    if( (fp = fopen("S02_LINUX","r"))==NULL )
        return(-1);
    top = (S02*)calloc(1,sizeof(S02));
    if( top==NULL || fread(&(top->nmod),sizeof(int),1,fp)!=1 ||
        top->nmod<=0 || top->nmod>MAX_MODEL ||
        fread(&(top->model[0]),sizeof(MODEL),top->nmod,fp)!=(size_t)top->nmod )
        {
        fclose(fp);
        free(top);
        return(-1);
        }
    fclose(fp);
    nmod = top->nmod;
    for(i=0;i<nmod;i++)
        {
        num[i].stati    = top->model[i].punt_ivin  - top->model[i].punt_ivus;
        num[i].ingressi = top->model[i].punt_ider  - top->model[i].punt_ivin;
        num[i].derivate = top->model[i].punt_idati - top->model[i].punt_ider;
        num[i].dati     = top->model[i].punt_ifine - top->model[i].punt_idati;
        *size += (num[i].stati + num[i].ingressi + num[i].derivate + num[i].dati)
                 * (int)sizeof(float);
        }
    free(top);
    return(nmod);
}

/*
   snapshot.dat o backtrack.dat salvato con un'ALTRA topologia: dopo una
   modifica del modello (kUpSim, una task ricompilata) le variabili non sono
   piu' quelle, e caricare uno stato da quel file vuol dire ripartire da
   valori sfalsati, senza che niente lo dica. Lo si e' visto con snaf14 su una
   demo: uno snapshot di settembre con 56 variabili in piu' dava valori senza
   senso, e net_sked lo avrebbe caricato allo stesso modo.

   Il controllo usa quello che il file registra nell'intestazione: la
   dimensione dell'area dati e, task per task, stati, ingressi, derivate e
   dati. Si confronta anche il dettaglio perche' una variabile spostata da una
   task all'altra lascia uguale il totale.

   Un file cosi' non si puo' usare in nessun caso, quindi lo si mette da parte
   come f22circ.dat (rinominato, non cancellato: lo si ritrova), e la
   simulazione parte con uno nuovo. Lo fa il primo processo che se ne
   accorge; rename() e' atomica, e chi arriva dopo non trova piu' il file.
   Ritorna 1 se lo ha messo da parte (o se lo aveva gia' fatto un altro).
*/
static int ScartaSeAltraTopologia(char *file, char *scartato,
                                  HEADER_REGISTRAZIONI *hd)
{
NUMERI_MODELLI num[MAX_MODEL];
int   nmod,size,i,diverso=0;
FILE *fpedf;
FILE *fp;

    nmod = TopologiaAttuale(num,&size);
    if( nmod<0 || hd->size_area_dati<=0 )
        return(0);                    /* niente da confrontare */
    if( hd->size_area_dati != size )
        diverso = 1;
    for(i=0;i<nmod && !diverso;i++)
        if( hd->num_modelli[i].stati    != num[i].stati    ||
            hd->num_modelli[i].ingressi != num[i].ingressi ||
            hd->num_modelli[i].derivate != num[i].derivate ||
            hd->num_modelli[i].dati     != num[i].dati )
            diverso = 1;
    if( !diverso )
        return(0);

    if( rename(file,scartato)==0 )
        {
        fpedf = fopen(PAREDF,"a");
        if( fpedf!=NULL )
            {
            fprintf(fpedf,"RIMEDIO: %s salvato con un'altra topologia (area dati "
                          "%d byte, ora %d): rinominato in %s, ne verra' creato "
                          "uno nuovo\n",file,hd->size_area_dati,size,scartato);
            fclose(fpedf);
            }
        printf("%s: %s salvato con un'altra topologia (area dati %d byte, ora %d): "
               "rinominato in %s, ne verra' creato uno nuovo\n",
               nome_proc,file,hd->size_area_dati,size,scartato);
        return(1);
        }
    if( (fp = fopen(file,"r"))==NULL )
        return(1);                    /* messo da parte da un altro, un attimo fa */
    fclose(fp);
    return(0);
}

int ControlParam(int processo)
{
int i,ret;
int errorifile=0;
int errorishm=0;
FILE *fp;
FILE *fpedf;
char file[100];
HEADER_REGISTRAZIONI hdfile;
RtDbPuntiOggetto dbpunti_ext;
RtErroreOggetto  errore;
int scrivi=0;
int esnap=0,ebktk=0,ef22=0;




/*  Scritture iniziali su file */
    NomeProcesso(processo,nome_proc);
/*
    parametri.out ricomincia a ogni avvio. Il dispatcher e' il primo processo
    a partire (net_startup, net_startup_headless, startup): lui mette da parte
    il file dell'avvio precedente come parametri.out.prec e ne apre uno nuovo,
    gli altri ci scrivono in coda come prima. Fino a ottobre 2026 si scriveva
    sempre in coda, e il file cresceva per sempre: in una demo ne e' arrivato
    uno con 131 avvii della macchina di chi l'aveva confezionata, e righe
    SEVERE vecchie di giorni sembravano dell'avvio appena fatto.
*/
    if( processo==DISPATCHER )
       {
       time_t ora = time(NULL);
       rename(PAREDF,PAREDF ".prec");
       fpedf = fopen(PAREDF,"w");
       if( fpedf!=NULL )
          {
          fprintf(fpedf,"Avvio del %s",ctime(&ora));
          fclose(fpedf);
          }
       }
    fpedf = fopen(PAREDF,"a");
    fprintf(fpedf,"\n");
    fprintf(fpedf,"\t ===> %-12s <===\n", nome_proc);
    fflush(fpedf);
    fclose(fpedf);

    if( processo==DISPATCHER )
       scrivi=1;
    else
       scrivi=0;
/*  Controllo del file snapshot.dat */
    strcpy(file,FSNAP);
    ret = FileExist(file,scrivi);
    if( ret==0 )
      {
      fp = fopen(file, "r");
      fread(&hdfile,sizeof(HEADER_REGISTRAZIONI),1,fp);
      fclose(fp);
      if( !ScartaSeAltraTopologia(FSNAP,FSNAP_SCARTATO,&hdfile) )
         esnap = ConfrontoPar(hdfile,processo,file);
      }
    else
      printf("%s non esiste %s\n",nome_proc,file);

/*  Controllo del file backtrack.dat */
    strcpy(file,FBKTK);
    ret = FileExist(file,scrivi);
    if( ret==0 )
      {
      fp = fopen(file, "r");
      fread(&hdfile,sizeof(HEADER_REGISTRAZIONI),1,fp);
      fclose(fp);
      if( !ScartaSeAltraTopologia(FBKTK,FBKTK_SCARTATO,&hdfile) )
         ebktk = ConfrontoPar(hdfile,processo,file);
      }

/*  Controllo del file f22circ.dat */
    strcpy(file,FF22C);
    ret = FileExist(file,scrivi);
    if( ret==0 )
      {
      fp = fopen(file, "r");
      fread(&hdfile,sizeof(HEADER_REGISTRAZIONI),1,fp);
/*
printf("\t\t\t [%s]\n",nome_proc);
printf("\t\t ControlParam da f22circ.dat %d %d %d %d %d %d %d\n",
          hdfile.simulator.max_snap_shot,
          hdfile.simulator.max_back_track,
          hdfile.simulator.max_campioni,
          hdfile.simulator.num_var,
          hdfile.simulator.max_pertur,
          hdfile.simulator.spare_snap,
          hdfile.simulator.pert_clear);
*/
      fclose(fp);
      ef22 = ConfrontoPar(hdfile,processo,file);
      }
/*
   Un f22circ.dat incompatibile non ferma piu' l'avvio: lo si mette da parte
   e la simulazione parte, perche' net_prepf22 ne crea uno nuovo quando non
   lo trova.

   Il file porta nell'intestazione i parametri con cui e' nato (numero di
   campioni, numero di variabili). Se non sono quelli in uso - la task
   lanciata prima da una FMU e poi da legopc, un Simulator cambiato, una
   demo confezionata con dentro la corsa di un altro - ConfrontoPar lo segna
   SEVERE, e fino a ottobre 2026 l'avvio finiva con
       Errori in fase di Startup (sk=4 disp=4 monit=4 shm=0)
   lasciando una directory in cui la simulazione non partiva piu' finche'
   qualcuno non cancellava il file a mano, senza che niente dicesse quale.

   Si puo' fare perche' f22circ.dat e' solo la registrazione circolare per i
   grafici: non e' uno stato da cui si riparte. snapshot.dat e backtrack.dat
   invece lo sono, e per i PARAMETRI diversi l'errore resta - buttarli e' una
   decisione di chi usa il simulatore, non di questa funzione. Salvo un caso:
   salvati con un'altra TOPOLOGIA non si possono usare comunque, e vengono
   messi da parte (ScartaSeAltraTopologia, sopra).

   Lo fa il PRIMO processo che se ne accorge, chiunque sia: dispatcher,
   net_sked, il banco e net_prepf22 passano tutti di qui prima di aprire il
   file. rename() e' atomica: chi arriva dopo non trova piu' niente e
   prosegue. Rinominato e non cancellato, cosi' le vecchie registrazioni si
   possono ancora recuperare; una copia messa da parte in precedenza viene
   sostituita.
*/
    if( ef22!=0 )
      {
      if( rename(FF22C,FF22C_SCARTATO)==0 )
         {
         fpedf = fopen(PAREDF,"a");
         if( fpedf!=NULL )
            {
            fprintf(fpedf,"RIMEDIO: %s incompatibile rinominato in %s: "
                          "ne verra' creato uno nuovo\n",FF22C,FF22C_SCARTATO);
            fclose(fpedf);
            }
         printf("%s: %s non compatibile con i parametri in uso: "
                "rinominato in %s, ne verra' creato uno nuovo\n",
                nome_proc,FF22C,FF22C_SCARTATO);
         ef22 = 0;
         }
      else if( (fp = fopen(FF22C,"r"))==NULL )
         {
         /* gia' messo da parte da un altro processo, un attimo fa */
         ef22 = 0;
         }
      else
         fclose(fp);
      }
    errorifile = esnap + 2*ebktk + 4*ef22;
/*
    if ( errorifile!=0 )
       printf("\t\t ControlParam ritorna=%d (sn=%d bk=%d f22=%d)\n",
                 errorifile,esnap,ebktk,ef22);
*/
    return(errorifile);
}

int FileExist(char *nome_file, int scrivi)
{
int    i;
FILE  *fp;
FILE  *fpedf;


    if( (fp = fopen(nome_file, "r")) !=NULL )
        {
        fclose(fp);
        return(0);
        }
    else
        {
        if( scrivi==1 )
            {
            fpedf = fopen(PAREDF,"a");
            fprintf(fpedf,"WARNING  File %-15s non trovato\n",nome_file);
            fflush(fpedf);
            fclose(fpedf);
            }
        return(1);
        }
}


/*
    Confronta i parametri letti da simulator con quelli letti
    dall'header.
    Se un parametro vale NOPARAM significa che non e' stato
    passato al processo.
*/
int ConfrontoPar(HEADER_REGISTRAZIONI hdfile, int processo, 
                 char *nome_file)
{
int  i,ret=0;
FILE *fpedf;
int proc;
char msg[10];
int rmsnap=0,rmbktk=0,rmf22c=0;

/*
   printf("\t PARAMETRI=%d %d %d %d %d %d %d\n",
           _MAX_SNAP_SHOT,_MAX_BACK_TRACK,
           _MAX_CAMPIONI,_NUM_VAR,
           _MAX_PERTUR,_SPARE_SNAP,_PERT_CLEAR);
*/
   if ( 
        ( (NOPARAM == _MAX_SNAP_SHOT )  ||
          (hdfile.simulator.max_snap_shot  == _MAX_SNAP_SHOT ) ) &&  
        ( (NOPARAM == _MAX_BACK_TRACK)  ||                  
          (hdfile.simulator.max_back_track == _MAX_BACK_TRACK) ) &&
        ( (NOPARAM == _MAX_CAMPIONI  )  ||                   
          (hdfile.simulator.max_campioni   == _MAX_CAMPIONI  ) ) &&
        ( (NOPARAM == _NUM_VAR       )  ||                
          (hdfile.simulator.num_var        == _NUM_VAR       ) ) &&
        ( (NOPARAM == _MAX_PERTUR    )  ||              
          (hdfile.simulator.max_pertur     == _MAX_PERTUR    ) ) &&
        ( (NOPARAM == _PERT_CLEAR    )  ||             
          (hdfile.simulator.pert_clear     == _PERT_CLEAR    ) ) &&
        ( (NOPARAM == _SPARE_SNAP    )  ||             
          (hdfile.simulator.spare_snap     == _SPARE_SNAP    ) ) 
      )
      {
      ret = 0;
      }
   else
      {
      ret = 0;
      fpedf = fopen(PAREDF,"a");
      if ( 
           ( NOPARAM != _MAX_SNAP_SHOT )  &&
           ( hdfile.simulator.max_snap_shot  != _MAX_SNAP_SHOT  ) 
         )
         {
         if( (strcmp(nome_file,FSNAP)==0) || (strcmp(nome_file,FBKTK)==0) )
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_SNAP,hdfile.simulator.max_snap_shot,_MAX_SNAP_SHOT);
         }
      if ( 
           ( NOPARAM != _MAX_BACK_TRACK )  &&
           ( hdfile.simulator.max_back_track != _MAX_BACK_TRACK )
         )
         {
         if( (strcmp(nome_file,FSNAP)==0) || (strcmp(nome_file,FBKTK)==0) )
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_BKTK, hdfile.simulator.max_back_track,_MAX_BACK_TRACK);
         }
      if ( 
           ( NOPARAM != _MAX_CAMPIONI )  &&
           ( hdfile.simulator.max_campioni   != _MAX_CAMPIONI   ) 
         )
         {
         if( strcmp(nome_file,FF22C)==0) 
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_CAMP_CR, hdfile.simulator.max_campioni,_MAX_CAMPIONI);
         }
      if ( 
           ( NOPARAM != _NUM_VAR )  &&
           ( hdfile.simulator.num_var        != _NUM_VAR        )
         )
         {
         if( strcmp(nome_file,FF22C)==0) 
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_VAR_CR, hdfile.simulator.num_var,_NUM_VAR);
         }
      if ( 
           ( NOPARAM != _MAX_PERTUR )  &&
           ( hdfile.simulator.max_pertur     != _MAX_PERTUR     )
         )
         {
         if( (strcmp(nome_file,FSNAP)==0) || (strcmp(nome_file,FBKTK)==0) )
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_PERT_ACTIVE, hdfile.simulator.max_pertur,_MAX_PERTUR);
         }
      if ( 
           ( NOPARAM != _SPARE_SNAP )  &&
           ( hdfile.simulator.spare_snap     != _SPARE_SNAP     )
         )
         {
         if( (strcmp(nome_file,FSNAP)==0) || (strcmp(nome_file,FBKTK)==0) )
            {
            strcpy(msg,SEVR);
            ret = 1;
            }
         else
            strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_SPARE, hdfile.simulator.spare_snap,_SPARE_SNAP);
         }
      if ( 
           ( NOPARAM != _PERT_CLEAR )  &&
           ( hdfile.simulator.pert_clear     != _PERT_CLEAR     )
         )
         {
         strcpy(msg,WARN);
         fprintf(fpedf,"%-7s: %-13s  %-18s %5d  Simulator:[%d]\n",
                 msg,nome_file,
              ARGV_N_PERTCL, hdfile.simulator.pert_clear,_PERT_CLEAR);
         }
      fflush(fpedf);
      fclose(fpedf);
      }
   return(ret);
}



int    NomeProcesso(int processo, char *nome_proc)
{

     switch(processo)
        {
        case SKED:
             {
             strcpy(nome_proc,"net_sked");
             break;
             }
        case DISPATCHER:
             {
             strcpy(nome_proc,"dispatcher");
             break;
             }
        case MONIT:
             {
             strcpy(nome_proc,"net_monit");
             break;
             }
        case NET_PREPF22_CIRC:
             {
             strcpy(nome_proc,"net_prepf22_circ");
             break;
             }
        case NEW_MONIT:
             {
             strcpy(nome_proc,"new_monit");
             break;
             }
        case SNAF14:
             {
             strcpy(nome_proc,"snaf14");
             break;
             }
        default :
             {
             strcpy(nome_proc,"PROCESSO SCONOSCIUTO");
             break;
             }
        }
     return(0);
}
/* Fine file letture_dati.c */
