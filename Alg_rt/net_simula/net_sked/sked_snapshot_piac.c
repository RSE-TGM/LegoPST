/**********************************************************************
*
*       C Source:               sked_snapshot_piac.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Apr  2 14:50:38 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sked_snapshot_piac.c-15 %  (%full_filespec: sked_snapshot_piac.c-15:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)sked_snapshot_piac.c	5.3\t2/20/96";
/*
   modulo sked_snapshot_piac.c
   tipo 
   release 5.3
   data 2/20/96
   reserved @(#)sked_snapshot_piac.c	5.3
*/
# include <stdio.h>
# include <errno.h>
#if defined UNIX
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
# include <stdlib.h>
#endif
#if defined VMS
# include"vmsipc.h"
#endif
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "sked.h"
# include "comandi.h"
# include "dispatcher.h"
# include "libnet.h"
# include <Rt/RtDbPunti.h>
# include <Rt/RtMemory.h>

#define TESTO_PROVA "Testo di prova dell'area spare in snapshot S.d.I. 01/97"

/*
     Variabili esterne
*/
extern RtDbPuntiOggetto dbpunti;
extern RtDbPuntiOggetto dbpunti_ext;
extern int    id_msg_sked;	/* id coda messaggi  da processi read */
extern int    id_msg_snap;	/* id coda messaggi  da processi ausiliari */
extern int    id_msg_monit;	/* id coda messaggi  da monit */
extern int    id_msg_banco;	/* id coda messaggi  da banco */
extern int    _MAX_SNAP_SHOT;
extern int    _MAX_BACK_TRACK;
extern int    _MAX_CAMPIONI;
extern int    _NUM_VAR;
extern int    _MAX_PERTUR;
extern int    _SPARE_SNAP;
extern int    _PERT_CLEAR;
extern int    tipo_sked;
extern int    stato_sked;
extern int    nmod;             /* numero dei modelli */
extern int    numero_slave;	/* numero di nodi SLAVE, solo per MASTER  */
extern SNTAB *snapshot_hea;	/* area shm per tabella snapshot */
extern int    size_area_dati;   /* size in byte area dati di tutte le task */
extern float *area_dati;        /* Area dati di tutte le task (e in 
                                   tutte le macchine)*/
extern double tempo_sim;	/* tempo della simulazione */
extern int    fp_slave[MAX_MODEL];      /* */
extern int    fp_ordini[MAX_MODEL];     /* puntatori dei socket a cui spedire
                                         * i comandi, solo per sked master */
extern int    fp_com[MAX_MODEL];        /* 0 -> task sulla stessa macchina 1
                                         * -> task su macchine diverse fp ->
                                         * socket scrittura */
extern int    slot_snap[MAX_VAL_SNAP];	/* indirizzi delle variabili
                                         * caratteristiche degli
                                         * snapshot */
/*
   Variabili globali
*/
HEADER_REGISTRAZIONI hreg_snap;
int         tot_snap;	/* numero degli snapshot registrati */
int         fp_master;	/* puntatore al socket del MAIN solo per
int         id_sh;	/* identificatore sh_var */
VARIABILI  *variabili;	/* database variabili di tutti i modelli */
char       *ind_sh_top;	/* puntatore inizio shared memory sh_var */
STATO_CR    stato_cr;
int   size_area_spare;    /* Definiscono le varie size dello snap */
int   size_blocco_snap;
int   size_parte1_snap; 

#if defined MFFR
SNAP_SKED  sommari_snapshot;
#endif
#if defined BACKTRACK
extern int ins_tacca_bktk_piac();
extern int del_tacca_bktk_piac();
#endif


extern int      modo_backtrack; /* 1 se slave fa SAVEBKTK 0 per SAVESNAP */
extern int      attiva_listaci;

extern  int converti_tempo(float,long  *,long  *,long  *,long  *,long  *,long  *);

sked_snapshot_piac(azione, numero)
   int             azione;
   short           numero;
{
   int             i, ret;
   MSG_NET         messaggio_net;
   MSG_AUS         messaggio_aus;
   PACCHETTO_NET  *p_snap;
   FILE           *fp;
   FILE           *fp_snap;	/* descrittore del file snapshot.dat */
   int size_task;
   int ini_task;
   int ii,jj;
   int nleft;
   int nwrite;
   int dati_complessivi;
   int numero_spedizioni;
   div_t  num_sped;
int   dati_scritti;       /* Numero dei dati inviati in rete (slave) */
TIPO_PERT *pert_tmp;
   /*
        parametri lego
   */
#if defined UNIX
   extern int _N000;
   extern int _N001;
   extern int _N002;
   extern int _N003;
   extern int _N004;
   extern int _N005;
   extern int _N007;
   extern int _M001;
   extern int _M002;
   extern int _M003;
   extern int _M004;
   extern int _M005;
#endif

/* 
   Allocazione delle variabili per la trasmissione in rete:
   non viene piu' effettuata dinamicamente.
*/
   p_snap = (PACCHETTO_NET *) malloc(sizeof(PACCHETTO_NET));

/* Manda il messaggio di SNAPSHOT alle macchine SLAVE  */
   if (tipo_sked == MASTER)
   {
      messaggio_net.header_net.tipo = COMSIM;
      messaggio_net.header_net.codice = azione;
      messaggio_net.header_net.lun = 0;
      for (i = 0; i < nmod; i++)
	 if (fp_ordini[i] > 0)
	 {
	    if (writen(fp_ordini[i], &messaggio_net.header_net,
		       sizeof(HEADER_NET)) < 0)
	    {
	       sked_stato(STATO_ERRORE);
	       fprintf(stderr, "Impossibile com %d\n", i);
	       sked_errore();
	    }
	 }
   }

/* 
   Calcolo dimensioni di alcuni blocchi del file di snapshot 
   1) blocco iniziale  2) corpo dello snapshot 3) area_spare
*/
   size_area_spare  =  _SPARE_SNAP * AREA_SPARE;
   size_parte1_snap =  _MAX_SNAP_SHOT * sizeof(SNTAB) + 
                       sizeof(HEADER_REGISTRAZIONI);
   size_blocco_snap =  size_area_dati    +  sizeof(float)     +
#if defined MFFR
                       sizeof(SNAP_SKED) +
#endif
                       DIM_SNAP_AUS      +
                       size_area_spare * sizeof(char)         +
                       2*_MAX_PERTUR * sizeof(TIPO_PERT);
   /*printf("\t size_parte1_snap=%d size_blocco_snap=%d size_area_spare=%d\n",
              size_parte1_snap,size_blocco_snap,size_area_spare);*/
/*    
   IMPORTANTE: lo slave in caso di backtrack usa la funzione
   sked_snapshot_piac. 
*/
   switch (azione)
   {
   case LOADSNAP:
      {
      if (tipo_sked == MASTER)
	 {
/*       Lettura del file snap.dat */
         ret = snap_load_piac(numero);
         sprintf(messaggio_aus.ausiliario.messaggio, "%d", ret);
         if (atoi(messaggio_aus.ausiliario.messaggio) == (-1))
            fprintf(stderr, "Impossibile caricare lo SNAPSHOT");
         }
/*    Master trasmette i dati e le pert dei modelli non residenti agli slave */
      sked_raccogli_datisnap(MASTER, SLAVE, p_snap, modo_backtrack);
/*
      rimozione pert remote in fase di LOADIC: sia master che slave al
      termine della trasmissione-ricezione dei dati.
*/
      rm_pert_remote();
      break;
      }
   case SAVESNAP:
      {
/*    Gli slave trasmettono dati e pert dei modelli residenti al master */
      sked_raccogli_datisnap(SLAVE, MASTER, p_snap, modo_backtrack);
      if (tipo_sked == MASTER)
         {
/*       Scrittura del file snap.dat */
         ret = snap_save_piac(numero);
         sprintf(messaggio_aus.ausiliario.messaggio, "%d", ret);
         if (atoi(messaggio_aus.ausiliario.messaggio) == (-1))
           fprintf(stderr, "Impossibile salvare lo SNAPSHOT\n");
         else
           tot_snap++;
/*
         rimozione pert remote in fase di SAVEIC: riguarda solo il master
         che ha ricevuto le pert degli slave.
*/
         rm_pert_remote();
         }
      break;
      }
   case DELSNAP:
      {
      if (snapshot_hea[numero - 1].stat == 1)
        ret = snap_del_piac(numero);
      sprintf(messaggio_aus.ausiliario.messaggio, "%d", ret);
      if (atoi(messaggio_aus.ausiliario.messaggio) == (-1))
         fprintf(stderr, "Impossibile eliminare lo SNAPSHOT");
      }
   }   /* Fine switch */

   /* libera la memoria */
   free(p_snap);
}




/*
 * Legge da file la tabella degli snapshot e la carica in shared memory; se
 * il file non esiste lo crea. 
 */

snap_tab_read_piac()
{
   int             i;
   int             size_snap, size_file;
   int             offset;
   char            zero = '0';
   FILE           *fp_snap;	/* descrittore del file snapshot.dat */
   int             dim_zero;
   div_t           dim_snap;

   /* Caricamento shared memory sezione snapshot */
   tot_snap = 0;
   if (!(fp_snap = fopen("snapshot.dat", "r")))
   {
      /* Il file di snapshot non esiste e viene creato vuoto  */
      fprintf(stderr, "Creazione del nuovo file snapshot.dat\n");
      fp_snap = fopen("snapshot.dat", "w");
/*    Prima di scrivere l'area tabellare degli snapshot aggiunge un header */
      sked_prepara_header_reg(&hreg_snap);
      fwrite(&hreg_snap,sizeof(HEADER_REGISTRAZIONI),1,fp_snap);
      for (i = 1; i <= _MAX_SNAP_SHOT; i++)
      {
	 snapshot_hea[i - 1].stat = 0;
	 snapshot_hea[i - 1].prog = i;
	 snapshot_hea[i - 1].pos  = 0;
	 snapshot_hea[i - 1].mod  = 0;
         snapshot_hea[i - 1].forzato = 0;
         snapshot_hea[i - 1].tempo = 0;
         strcpy(snapshot_hea[i - 1].val, "0\00"); 
	 strcpy(snapshot_hea[i - 1].descr, " >>>>    SNAPSHOT FREE    <<<<\00");
	 strcpy(snapshot_hea[i - 1].datasn, "00/00/00\00");
	 strcpy(snapshot_hea[i - 1].temposn, "0\00");
	 fwrite(&snapshot_hea[i - 1], sizeof(SNTAB), 1, fp_snap);
         /*printf("Scritto snap: [i=%d] mod=%d\n",i,snapshot_hea[i-1].mod);*/
      }
/*    
      Dimensione relativa all'area dati di uno snap. Contiene:
         1) area effettiva variabili     2) tempo effettuazione snap
         3) SNAP_SKED                    4) DIM_SNAP_AUS 
         5) Area spare parametrica       6) Area complessiva pert
*/

      size_area_spare  = _SPARE_SNAP * AREA_SPARE;
      size_snap = _MAX_SNAP_SHOT * (size_area_dati      + sizeof(float)    +
#if defined MFFR
				   sizeof(SNAP_SKED)   +
#endif
				   DIM_SNAP_AUS        +  
                                   size_area_spare * sizeof(char)         +
                                   2*_MAX_PERTUR*sizeof(TIPO_PERT) );
/*/
      printf("\n\t Parametri creazione file snapshot.dat\n");
      printf("\t header=%d+SNTAB=%d+size_parte_snap=%d ==> sizefile=%d \n\n",
         sizeof(HEADER_REGISTRAZIONI),
         _MAX_SNAP_SHOT*sizeof(SNTAB),
         size_snap,
         sizeof(HEADER_REGISTRAZIONI)+_MAX_SNAP_SHOT*sizeof(SNTAB)+size_snap);
*/
      printf("\t ==> size file=%d\n",
          sizeof(HEADER_REGISTRAZIONI)+_MAX_SNAP_SHOT*sizeof(SNTAB)+size_snap);

      dim_snap = div(size_snap, sizeof(float));
      if (dim_snap.rem == 0)
      {
	 size_file = size_snap / sizeof(float);
	 dim_zero = sizeof(float);
      } else
      {
	 size_file = size_snap;
	 dim_zero = sizeof(char);
      }
      for (i = 0; i < size_file; i++)
	 fwrite(&zero, dim_zero, 1, fp_snap);
      load_stato_cr(&stato_cr);
      stato_cr.last_snap_save = 0;
      stato_cr.last_snap_load = 0;
      save_stato_cr(&stato_cr);
   } else
   {
/*    Il file esiste, viene letto l'header */
      fread(&hreg_snap,sizeof(HEADER_REGISTRAZIONI),1,fp_snap);
      /*StampaHeader(hreg_snap);*/
/*    Vengono lette le tabella degli snap  */
      for (i = 1; i <= _MAX_SNAP_SHOT; i++)
      {
	 fread(&snapshot_hea[i - 1], sizeof(SNTAB), 1, fp_snap);
	 /*
	 if (snapshot_hea[i - 1].stat == 1)
           printf("snap_shared prog=%d stat=%d pos=%d mod=%d %s data=%s t=%s\n",               snapshot_hea[i - 1].prog,  snapshot_hea[i - 1].stat, 
               snapshot_hea[i - 1].pos,   snapshot_hea[i - 1].mod, 
               snapshot_hea[i - 1].descr, snapshot_hea[i - 1].datasn, 
               snapshot_hea[i - 1].temposn); 
	  */
	 if (snapshot_hea[i - 1].stat == 1)
	    ++tot_snap;
      }
      fclose(fp_snap);
   }
}



/*
 * Salva la copia della shared memory in coda al file. La posizione
 * passata va da 0 per il primo snap a MAX_SNAPSHOT-1. 
 * Gli updown non sono salvati (ne' come perturbazioni ne' come valore)
 * e continuano nella simulazione.
 */
int snap_save_piac(int posizione)
{
char      *app;
char      *punt;
int       offset,offset_tab;
int       i;
char      prova[30];
float     tempo;
char *area_spare;
FILE      *fp_snap;
TIPO_PERT *pert_tmp,*pert_tmp_att;  /* perturbazioni attive e in attesa   */
TIPO_PERT *pert_file,*pert_file_att; /* perturbazioni salvate su file      */
int ii,jj;
HEADER_REGISTRAZIONI hdreg_tmp;
SNTAB sn;

/*
   RtDbPGetHeaderReg(dbpunti_ext,&hdreg_tmp);
   printf("DBPUNTI_EXT con = [%d %d %d %d %d %d %d]\n",
           hdreg_tmp.simulator.max_snap_shot ,
           hdreg_tmp.simulator.max_back_track,
           hdreg_tmp.simulator.max_campioni  ,
           hdreg_tmp.simulator.num_var       ,
           hdreg_tmp.simulator.max_pertur    ,
           hdreg_tmp.simulator.spare_snap    ,
           hdreg_tmp.simulator.pert_clear);
*/
   if (posizione >= _MAX_SNAP_SHOT)
       {
       fprintf(stderr, "snap_save_piac:gia' stati salvati _MAX_SNAP_SHOT\n");
       return (-1);
       }
/*
   printf("snap_save_piac, SAVE: Salvataggio snap [%s] [num %d]  \n",
	  snapshot_hea[posizione].descr, posizione + 1);
*/
   pert_file     = (TIPO_PERT*)calloc(_MAX_PERTUR,sizeof(TIPO_PERT));
   pert_file_att = (TIPO_PERT*)calloc(_MAX_PERTUR,sizeof(TIPO_PERT));

/*
   Nel caso distribuito, ricevute le perturbazioni residenti nelle
   macchine slave bisogna provvedere a salvare l'area pert globale.
*/
   if((pert_tmp=RtDbPGetPuntPert(dbpunti))==NULL)
       {
       printf("snap_save: non riesco ad agganciare le perturbazioni\n");
       return(-1);
       }
   else
       memcpy(pert_file,pert_tmp,_MAX_PERTUR*sizeof(TIPO_PERT));
   if((pert_tmp_att=RtDbPGetPuntPertAtt(dbpunti))==NULL)
       {
       printf("snap_save: non aggancio le perturbazioni in attesa\n");
       return(-1);
       }
   else
       memcpy(pert_file_att,pert_tmp_att,_MAX_PERTUR*sizeof(TIPO_PERT));


   if (!(fp_snap = fopen("snapshot.dat", "r+")))
   {
      fprintf(stderr, "Il file di snapshot non esiste\n");
      exit(0);
   }

/* 0) Registrazione tabella dello snapshot */
   offset = sizeof(HEADER_REGISTRAZIONI) + posizione * sizeof(SNTAB);
   offset_tab = offset;
   fseek(fp_snap, offset, 0);
   fwrite(&snapshot_hea[posizione], sizeof(SNTAB), 1, fp_snap);

/* 1) Area dati */
   offset = size_parte1_snap + posizione * size_blocco_snap;
   fseek(fp_snap, offset, 0);
   punt = (char *) malloc(size_area_dati);
   memcpy(punt, area_dati, size_area_dati);

/*
   A differenza di prima, quando gli  updown venivano  negati nel valore 
   della variabile che finiva su file ma non in memoria, ora si aggiunge
   l'eliminazione dal file della perturbazione PERT_UP corrispondente in
   modo da non salvarla con l'area  delle perturbazioni per gli snapshot.
   Questa  funzione gestisce l'intervento sui dati da salvare sia per le
   variabili del master che per lo slave.
*/
   remove_updown_file_snap((float *) punt, pert_file,pert_file_att);
   fwrite(punt, size_area_dati, 1, fp_snap);
   free(punt);

/* 2) Registra il tempo */
   RtDbPGetTime(dbpunti,&tempo);
   fwrite(&tempo, 1, sizeof(float) , fp_snap);

/* 3) DIM_SNAP_AUS */
   app = (char *) calloc(DIM_SNAP_AUS,sizeof(char));
   fwrite(app, DIM_SNAP_AUS, 1, fp_snap);

#if defined MFFR
/* 4) Sommari */
   /*printf("\t L'area sommari snap scritta circa a offset=%d\n",
           offset+size_area_dati+DIM_SNAP_AUS);*/
   fwrite(&sommari_snapshot, sizeof(SNAP_SKED), 1, fp_snap);
   printf ("sked_snapshot_piac: scritti somm. val = %d fr=%d mf =%d\n",
            sommari_snapshot.somm_snappati,
            sommari_snapshot.readfr.ul_fr,
            sommari_snapshot.readmf.ul_mf);
   memcpy(&prova[0], &sommari_snapshot, 30);
   prova[29] = 0;
   /*
    * memcpy(app,&sommari_snapshot,DIM_SNAP_AUS); app[DIM_SNAP_AUS-1]=0;
    * printf("--------------------------> <%s>\n",app); 
    */
#endif

/* 5) Area libera */
   if ( size_area_spare > 0 )
      {
      area_spare = (char*)calloc(size_area_spare,sizeof(char));
      memcpy(area_spare,TESTO_PROVA,strlen(TESTO_PROVA)*sizeof(char));
      }
   fwrite(area_spare, sizeof(char), size_area_spare, fp_snap);
   if ( size_area_spare > 0 )
      free(area_spare);

/* 6) Area pert : */
/* In realta' non sarebbe necessario fare un fseek */
   offset = size_parte1_snap  + (posizione + 1) * size_blocco_snap - 
                2*_MAX_PERTUR * sizeof(TIPO_PERT) ; 
   fseek(fp_snap, offset, 0);
   fwrite(pert_file,     sizeof(TIPO_PERT), _MAX_PERTUR, fp_snap);
   fwrite(pert_file_att, sizeof(TIPO_PERT), _MAX_PERTUR, fp_snap);
   /*
    * MostraPert(pert_file,1,_MAX_PERTUR);
    * MostraPert(pert_file_att,1,_MAX_PERTUR);
    */
   StampePertFile(pert_file, pert_file_att, 1, 0);


/* Carica i valori caratteristici degli slot */
   if ( attiva_listaci==1 )
      {
      for (i = 0; i < MAX_VAL_SNAP; i++)
         if (slot_snap[i] >= 0)
            snap_slot_piac(i + 1, slot_snap[i], posizione + 1);
      fseek(fp_snap, offset_tab, 0);
      fwrite(&snapshot_hea[posizione], sizeof(SNTAB), 1, fp_snap);
      }

   fclose(fp_snap);

/* Aggiornamenti */
   stato_cr.last_snap_save = posizione + 1;
   save_stato_cr(&stato_cr);

   free(pert_file);
   free(pert_file_att);
   return (posizione);
}



/*
 * Copia nella shared memory lo snapshot. La posizione passata 
 * va da 1 per il primo snap a  MAX_SNAPSHOT. 
 */
int snap_load_piac(int posizione)
{
char  app[DIM_SNAP_AUS];
FILE *fp_snap;	/* descrittore del file snapshot.dat */
int  i;
int  offset;
char tempo[TEMPO_SNAP];
int  ore = 0;
int  minuti = 0;
int  secondi = 0;
int  giorno = 1;
int  mese = 1;
int  anno = 2000;
float tempo_file;
char *area_spare;
TIPO_PERT *pert_tmp,*pert_tmp_att;
TIPO_PERT *pert_mem;

   printf("snap_load_piac: Caricamento snap %s [num %d] \n",
	  snapshot_hea[posizione - 1].descr, posizione);

   pert_tmp     = (TIPO_PERT*)calloc(_MAX_PERTUR,sizeof(TIPO_PERT));
   pert_tmp_att = (TIPO_PERT*)calloc(_MAX_PERTUR,sizeof(TIPO_PERT));

   if (!(fp_snap = fopen("snapshot.dat", "r+")))
   {
      fprintf(stderr, "Il file di snapshot non esiste\n");
      exit(0);
   }

/* 1) Area dati */
   offset = size_parte1_snap + (posizione - 1) * size_blocco_snap;
   fseek(fp_snap, offset, 0);
   fread(area_dati, size_area_dati, 1, fp_snap);

/* Copia il database nel database di LEGOGRAF */
   RtDbPCopyDb(dbpunti_ext,dbpunti);

/* 2) Lettura senza  setting del Tempo */
   fread(&tempo_file, 1, sizeof(float), fp_snap);

/* 3) DIM_SNAP_AUS */
   fread(app, DIM_SNAP_AUS, 1, fp_snap);

/* 4) Sommari */
#if defined MFFR
   fread(&sommari_snapshot, sizeof(SNAP_SKED), 1, fp_snap);
   printf ("sked_snapshot_piac: letti somm. val = %d fr=%d mf =%d\n",
            sommari_snapshot.somm_snappati,
            sommari_snapshot.readfr.ul_fr,
            sommari_snapshot.readmf.ul_mf);
#endif

/* 5) Area libera */
   area_spare = (char*)calloc(size_area_spare,sizeof(char));
   fread(area_spare, sizeof(char), size_area_spare, fp_snap);
   if ( size_area_spare > 0 )
      {
      /*printf("\t Letto area spare=%s\n",area_spare);*/
      free(area_spare);
      }

/* 6) Area pert */
   fread(pert_tmp,     sizeof(TIPO_PERT), _MAX_PERTUR, fp_snap);
   fread(pert_tmp_att, sizeof(TIPO_PERT), _MAX_PERTUR, fp_snap);
   /* 
    * MostraPert(pert_tmp,1,_MAX_PERTUR);
    * MostraPert(pert_tmp_att,1,_MAX_PERTUR);
    */ 
   StampePertFile(pert_tmp, pert_tmp_att, 0, 0);

/* Copio l'area letta (potrei leggerla direttamente */
   if((pert_mem=RtDbPGetPuntPert(dbpunti))==NULL)
       {
       printf("\t L'area PERT letta non la posso copiare\n");
       return(-1);
       }
   else
       memcpy(pert_mem,pert_tmp,_MAX_PERTUR*sizeof(TIPO_PERT));

   if((pert_mem=RtDbPGetPuntPertAtt(dbpunti))==NULL)
       {
       printf("\t L'area PERT att letta non la posso copiare\n");
       return(-1);
       }
   else
       memcpy(pert_mem,pert_tmp_att,_MAX_PERTUR*sizeof(TIPO_PERT));

/* Aggiornamenti */
   stato_cr.last_snap_load = posizione;
   save_stato_cr(&stato_cr);

/* Inserisce la tacca nell'ultimo backtrack registrato */
#if defined BACKTRACK
   converti_tempo((float) (tempo_sim / 1000),
		  (long*)&ore, (long*)&minuti, (long*)&secondi, 
                  (long*)&giorno, (long*)&mese, (long*)&anno);
   sprintf(tempo, "%2d:%2d:%2d", ore, minuti, secondi);
   if (!strncmp(tempo, snapshot_hea[posizione - 1].temposn, 8))
      del_tacca_bktk_piac();
   else
      ins_tacca_bktk_piac(1);
#endif

   fclose(fp_snap);
   free(pert_tmp);
   free(pert_tmp_att);
   return (posizione);
}



/*
 * Elimina lo snapshot identificato da posizione modificando il solo Header.
 * La posizione va conteggiata da 1 a MAX_SNAPSHOT. 
 */
snap_del_piac(posizione)
   int             posizione;
{
   FILE           *fp_snap;	/* descrittore del file snapshot.dat */
   int             offset;

   printf("snap_del_piac: Eliminazione snap %s [num %d]\n",
	  snapshot_hea[posizione - 1].descr, posizione);
   snapshot_hea[posizione - 1].stat = 0;
   snapshot_hea[posizione - 1].mod = 0;
   memset(snapshot_hea[posizione - 1].descr, 0, SNAP_DESC);
   strcpy(snapshot_hea[posizione - 1].descr, " >>>>    SNAPSHOT FREE    <<<<\00");
   memset(snapshot_hea[posizione - 1].datasn, 0, DATA_SNAP);
   strcpy(snapshot_hea[posizione - 1].datasn, "00/00/00\00");
   memset(snapshot_hea[posizione - 1].temposn, 0, TEMPO_SNAP);
   strcpy(snapshot_hea[posizione - 1].temposn, "0\00");
   if (!(fp_snap = fopen("snapshot.dat", "r+")))
   {
      fprintf(stderr, "Il file di snapshot non esiste\n");
      exit(0);
   }
   offset = sizeof(HEADER_REGISTRAZIONI) + (posizione - 1) * sizeof(SNTAB);
   fseek(fp_snap, offset, 0);
/* Registra l'header modificato dello snapshot eliminato */
   fwrite(&snapshot_hea[posizione - 1], sizeof(SNTAB), 1, fp_snap);
   fclose(fp_snap);
   --tot_snap;

   return (posizione);
}



/*
 * riempie la tabella dei valori caratteristici di ogni singolo snapshot 
 */
snap_slot_piac(int pos, int variabile, int num_snap)
{
int    i;
FILE  *fp_snap;
int    offset;
float  app;
int    inizio, fine;


   if (num_snap > 0)
   {
      inizio = num_snap - 1;
      fine   = num_snap;
   } else
   {
      inizio = 0;
      fine   = _MAX_SNAP_SHOT;
   }

   if (!(fp_snap = fopen("snapshot.dat", "r")))
   {
      fprintf(stderr, "Il file di snapshot non esiste\n");
      exit(0);
   }
/*
   printf("\n\n snap_slot_piac inizio=%d fine=%d\n",inizio,fine);
*/
   for (i = inizio; i < fine; i++)
      if (snapshot_hea[i].stat == 1)
        {
        offset = sizeof(HEADER_REGISTRAZIONI) + _MAX_SNAP_SHOT*sizeof(SNTAB) +
                 i * ( size_area_dati +  sizeof(float)  +
#if defined MFFR
                       sizeof(SNAP_SKED)                +
#endif
                       DIM_SNAP_AUS                     +
                       size_area_spare  * sizeof(char)  +
                       2*_MAX_PERTUR * sizeof(TIPO_PERT)  )                  +
                 variabili[variabile].addr * sizeof(float);

         fseek(fp_snap, offset, 0);
         fread(&app, 1, sizeof(float), fp_snap);
	 snapshot_hea[i].val[pos - 1] = app;
         }
   fclose(fp_snap);
   return(0);
}


export_db()
{
int             i, ret;
MSG_NET         messaggio_net;
PACCHETTO_NET *p_snap;


/*    Manda il messaggio di SNAPSHOT alle macchine SLAVE  */
      if (tipo_sked == MASTER)
         {
         messaggio_net.header_net.tipo = COMSIM;
         messaggio_net.header_net.codice = LOADSNAP;
         messaggio_net.header_net.lun = 0;
         for (i = 0; i < nmod; i++)
            if (fp_ordini[i] > 0)
               {
               if (writen(fp_ordini[i], &messaggio_net.header_net,
                          sizeof(HEADER_NET)) < 0)
                  {
                  sked_stato(STATO_ERRORE);
                  fprintf(stderr, "Impossibile com %d\n", i);
                  sked_errore();
                  }
               }
         }
/*    Scambio dati degli snap fra master e slave */
      p_snap = (PACCHETTO_NET *) malloc(sizeof(PACCHETTO_NET));
      sked_raccogli_datisnap(MASTER, SLAVE, p_snap, modo_backtrack);
      free(p_snap);
}


/*
    Routine di scambio dati di uno snapshot (o backtrack) tramite scritture
    in rete fra i processi net_sked e net_read.
    Sono inviati dapprima i dati e poi le perturbazioni.
    Parametri: 
       trasmittente = tipo del processo net_sked trasmittente: MASTER o SLAVE
       ricevente    = tipo della macchina alla quale il processo net_sked
                      trasmittente invia i dati: MASTER o SLAVE
       pc           = messaggio di rete.
    Nelle fasi di LOAD e' net_sked MASTER a spedire alla macchina SLAVE
    mentre durante i SAVE sono i processi net_sked SLAVE a spedire i dati 
    alla macchina MASTER.
    In particolare se si tratta di macchina MASTER la comunicazione (attesa
    messaggio da net_read o scrittura in rete) riguarda i modelli NON 
    residenti per cui quelli con fp_com != 0 mentre in caso di processi su
    macchina SLAVE i modelli residenti (che invieranno i dati al MASTER). 
    N.B. Le perturbazioni si spediscono a pacchetti di TIPO_PERT per cui
         ovviamente MAX_DATI_NET deve potrli contenere.

*/
int sked_raccogli_datisnap(int trasmittente, int ricevente, 
                           PACCHETTO_NET *pc, int modo_bktk)
{
MSG_NET         messaggio_net;
int size_task;
int ini_task;
int i,ii,jj;
int nleft,nleft_att;
int nwrite;
int dati_complessivi;
int numero_spedizioni;
div_t  num_sped;
int   dati_scritti=-1;       /* Numero dei dati inviati in rete (slave) */
int fp_trasmittente;
TIPO_PERT *pert_tmp,*pert_tmp_att,*pert_sped;
int nblocchileft,nblocchiwrite,nblocchipert;
float *pert_float;
int ret;


#if defined DEBUG_TRASMISSIONE
         printf("\t sked_raccogli_datisnap tipo_sk=%d T=%d R=%d (M=%d S=%d)\n",
                tipo_sked,trasmittente,ricevente,MASTER,SLAVE);
/*
         for (i = 0; i < nmod; i++)
             printf("\t %d) fp_com=%d    fp_ordini=%d    fp_slave=%d \n",
                     i,fp_com[i],fp_ordini[i],fp_slave[i]);
*/
#endif
         if ( tipo_sked == trasmittente )
            {
            /*
             * Il processo trasmittente invia dapprima i dati dei modelli 
             * che sono quelli dei modelli non residenti se si tratta del
             * master o quelli dei modelli residenti se si tratta dello slave.
             * In seguito invia le perturbazioni.
             * Si controlla se i dati siano  da  trasmettere suddivisi in 
             * pacchetti di MAX_DATI_NET float.
             */
            for (i = 0; i < nmod; i++)
               if (
                    ((fp_com[i] != 0) && (tipo_sked ==  MASTER)) ||
                    ((fp_com[i] == 0) && (tipo_sked ==  SLAVE))    
                  )
                  {
/* FASE A
                  SPEDIZIONI IN RETE DELLE PERTURBAZIONI
*/
/* 1
                  Dimensioni, offset e campi del pacchetto dati : 
                  alcuni momentaneamente a -1
*/
                  size_task = RtDbPSizeModelli(dbpunti,i);
                  ini_task = RtDbPInizioModelli(dbpunti,i);
                  pc->header_net.sorg     = (-1);
                  pc->header_net.dest     = (-1);
                  pc->header_net.tasksorg = i;
                  pc->header_net.taskdest = (-1);
                  pc->header_net.tipo     = DATISNAP;
                  pc->header_net.flag     = (-1);
/* 2
                  Calcolo del numero delle spedizioni in rete dei dati.
                  Il campo codice viene sfruttato per indicare il numero
                  delle spedizioni calcolato.
*/
                  dati_complessivi = size_task / sizeof(float);
                  num_sped = div(dati_complessivi,MAX_DATI_NET);
                  if ( num_sped.rem == 0 )
                     {
                     numero_spedizioni = dati_complessivi/MAX_DATI_NET;
                     }
                  else
                     {
                     numero_spedizioni = num_sped.quot + 1;
                     }
#if defined DEBUG_TRASMISSIONE
                  printf("\tDATI:numero_spedizioni=%d rem=%d quot=%d\n",
                            numero_spedizioni,num_sped.rem,num_sped.quot);
#endif
                  pc->header_net.codice   = numero_spedizioni;
/* 3
                  Ciclo delle spedizioni in rete dei dati agli sked slave.
*/
                  nleft = size_task;
                  nwrite = 0;
                  for(ii=0;ii<numero_spedizioni;ii++)
                     {
/*                   Calcolo della size ed assegnamento pacchetto */
                     if( nleft > MAX_DATI_NET*sizeof(float) )
                         pc->header_net.lun = MAX_DATI_NET*sizeof(float);
                     else
                         pc->header_net.lun = nleft;
                     memcpy(&(pc->dato[0]),
                            &area_dati[ini_task+nwrite/sizeof(float)],
                            pc->header_net.lun);
                     nwrite += pc->header_net.lun;
                     nleft  -= pc->header_net.lun;
#if defined DEBUG_TRASMISSIONE
                     printf("\t DEST=%d Invio dati n.%d lun=%d (t. cicli=%d)\n",
                             pc->header_net.tasksorg,
                             ii+1,pc->header_net.lun,
                             numero_spedizioni);
                     printf("\t        byte spediti=%d byte mancanti=%d\n",
                             nwrite,nleft);
#endif
#if defined DEBUG_VALORI
/*                   Stampe valori spediti */
                     for(jj=0;jj< pc->header_net.lun/sizeof(float);jj++)
                        printf("%d) [%f]\n",jj,pc->dato[jj]);
#endif
/*                  
                     Scrittura in rete :
                     se slave trasmette al master,  altrimenti al socket 
                     corrispondente alla macchina cui risiede il modello
                     i-esimo di cui si stanno inviando i dati.
*/
                     if ( trasmittente== SLAVE )
                          fp_trasmittente = fp_master;
                     else
                          fp_trasmittente = fp_com[i];
#if defined DEBUG_TRASMISSIONE
                     printf("\t Dati su nodo i=%d con: ",i);
                     printf("fp_trasm.=%d fp_com[i]=%d (fp_master=%d)\n",
                             fp_trasmittente,fp_com[i],fp_master);
#endif
/*
                     Se si tratta di uno slave in modalita' di snapshot
                     elimino gli updown dai dati. Questa fase ora viene fatta
                     direttamente sull'area salvata dal master.
		     if ( (modo_bktk == 0)&&(tipo_sked==SLAVE) )
		        remove_updown_slave(
                               (float *) (&(pc->dato[0])), i);
*/
                     if ( (dati_scritti = writen(fp_trasmittente, pc,
                                                 sizeof(HEADER_NET) +
                                                 pc->header_net.lun)) < 0)
                        {
                        fprintf(stderr,"Errore scrittura su nodo %d\n", i);
                        perror("--> ");
                        if (stato_sked < STATO_ERRORE)
                           {
                           sked_stato(STATO_ERRORE);
                           sked_errore();
                           }
                        }
                     } /* Fine ciclo delle spedizioni in rete */
                  if( nleft!=0 )
                     fprintf(stderr,"Spedizione incompleta: nleft=%d\n",nleft);
/* FASE 2
                  SPEDIZIONI IN RETE DELLE PERTURBAZIONI
*/
/* 1
                  Si recupera l'area delle perturbazioni attive e in attesa 
                  facendone un unico pacchetto lungo 2*_MAX_PERTUR 
*/
                  if((pert_tmp=RtDbPGetPuntPert(dbpunti))==NULL)
                      {
                      nleft = 0;
                      fprintf(stderr,
        "sked_raccogli_datisnap: perturbazioni non disponibili\n");
                      }
                  if((pert_tmp_att=RtDbPGetPuntPertAtt(dbpunti))==NULL)
                      {
                      nleft_att = 0;
                      fprintf(stderr,
        "sked_raccogli_datisnap: perturbazioni in attesa non disponibili\n");
                      }
                  pert_sped = (TIPO_PERT*)calloc(2*_MAX_PERTUR,
                                                 sizeof(TIPO_PERT));
                  memcpy(&pert_sped[0],pert_tmp,
                         _MAX_PERTUR*sizeof(TIPO_PERT));
                  memcpy(&pert_sped[_MAX_PERTUR],pert_tmp_att,
                         _MAX_PERTUR*sizeof(TIPO_PERT));

/* 2
                  Calcolo quante spedizioni sono necessarie per inviare
                  2*_MAX_PERTUR gruppi di sizeof(TIPO_PERT). Ne invio
                  gruppi nblocchipert di TIPO_PERT alla volta (oppure
                  tutti assieme se basta una spedizione).
*/
                  num_sped = div(2*_MAX_PERTUR*sizeof(TIPO_PERT),
                                    MAX_DATI_NET*sizeof(float));
                  if ( num_sped.rem == 0 )
                        {
                        numero_spedizioni = num_sped.quot;
                        }
                  else
                        {
                        numero_spedizioni = num_sped.quot+1;
                        }
                  nblocchipert = (int)( (int)(MAX_DATI_NET*sizeof(float)) / 
                                        (int)(sizeof(TIPO_PERT)));
#if defined DEBUG_TRASMISSIONE
                  printf("\tPERT:numero_spedizioni=%d rem=%d quot=%d",
                          numero_spedizioni,num_sped.rem,num_sped.quot);
                  printf("  nblocchipert=%d\n", nblocchipert);
#endif

/* 3
                  Ciclo delle spedizioni in rete delle pert agli sked slave.
*/
                  pc->header_net.codice = numero_spedizioni;
                  nblocchiwrite         = 0;
                  nblocchileft          = 2*_MAX_PERTUR;
                  pc->header_net.lun    = nblocchipert* sizeof(TIPO_PERT);
                  for(ii=0;ii<numero_spedizioni;ii++)
                     {
                     if(ii==numero_spedizioni-1)
                        {
                        pc->header_net.lun = nblocchileft*sizeof(TIPO_PERT);
                        }
                     ret = ConvPertFloat(&pert_float,
                                   &pert_tmp[nblocchiwrite],pc->header_net.lun);
                     if( ret!=pc->header_net.lun )
                        {
                        printf("\t ConvPertFloat ritorna size=%d ma lun=%d\n",
                                 ret,pc->header_net.lun);
                        pc->header_net.lun = ret;
                        }
                     memcpy(&(pc->dato[0]),
                            pert_float,pc->header_net.lun);
                     nblocchiwrite += pc->header_net.lun/sizeof(TIPO_PERT);
                     nblocchileft  -= pc->header_net.lun/sizeof(TIPO_PERT);
#if defined DEBUG_TRASMISSIONE
                     printf("\t DEST=%d Invio pert n.%d lun=%d (%d pert) (t. cicli=%d)\n",
                             pc->header_net.tasksorg,
                             ii+1,pc->header_net.lun,
                             pc->header_net.lun/sizeof(TIPO_PERT),
                             numero_spedizioni);
                     printf("\t        blocchi: spediti=%d  mancanti=%d\n",
                             nblocchiwrite,nblocchileft);
#endif
#if defined DEBUG_PERTURBAZIONI
                     printf("\t Inizio stampe perturbazioni (%d) \n", 
                             pc->header_net.lun/sizeof(float));
                     for(jj=0;jj< pc->header_net.lun/sizeof(float);jj++)
                        {
                           printf("%d) [%f]\n",jj,pc->dato[jj]);
                        }
#endif
/*
                     Scrittura in rete :
                     se slave trasmette al master,  altrimenti al socket
                     corrispondente alla macchina cui risiede il modello
                     i-esimo di cui si stanno inviando i dati.
*/
                     if ( trasmittente== SLAVE )
                          fp_trasmittente = fp_master;
                     else
                          fp_trasmittente = fp_com[i];
#if defined DEBUG_TRASMISSIONE
                     printf("\t Pert su nodo i=%d con: ",i);
                     printf(" fp_trasm.=%d fp_com[i]=%d (fp_master=%d)\n",
                             fp_trasmittente,fp_com[i],fp_master);
#endif
                     if ( (dati_scritti = writen(fp_trasmittente, pc,
                                                 sizeof(HEADER_NET) +
                                                 pc->header_net.lun)) < 0)
                        {
                        fprintf(stderr,"Errore scrittura su nodo %d\n", i);
                        perror("--> ");
                        if (stato_sked < STATO_ERRORE)
                           {
                           sked_stato(STATO_ERRORE);
                           sked_errore();
                           }
                        }
                     } /* Fine ciclo delle spedizioni in rete */
                  if( nblocchileft!=0 )
                     fprintf(stderr,"Spedizione incompleta: nleft=%d\n",nleft);
                  free(pert_sped);
                  } /* Fine if su fp_com[i] e tipo_sked  */
            }
         else
            if( tipo_sked == ricevente )
            {
            for (i = 0; i < nmod; i++)
                if (
                    ((fp_com[i] != 0) && (tipo_sked ==  MASTER)) ||
                    ((fp_com[i] == 0) && (tipo_sked ==  SLAVE))    
                   )
                    {
/*
                    Disponibilita' di tutti i dati.
                    Attesa ritorno-recezione da parte di net_read,
                    ora si attende solo la size di HEADER_NET.
*/
                    if (msg_rcv(id_msg_sked,&messaggio_net,sizeof(HEADER_NET),
                                DATISNAP,
                                !IPC_NOWAIT, TIMEOUT_SKED) == TIMEOUT_SCADUTO)
                        {
                        if (stato_sked < STATO_ERRORE)
                           {
                           sked_stato(STATO_ERRORE);
                           sked_errore();
                           }
                        }
                    else
                       printf("Disponibilita' di tutti i dati mod %d\n",i);
                    }
            }
          else
            fprintf(stderr,
                    "\t sked_raccogli_datisnap : errore caso non pervisto\n");
}




sked_prepara_header_reg(HEADER_REGISTRAZIONI *h_reg)
{
int nmodelli;
int size_head;
int size_dati;
int mod;
int stati,ingressi,derivate,dati;
FILE *fh;
NUMERI_MODELLI *modelli;

   nmodelli = RtDbPNumeroModelli(dbpunti);

/* 
   Preparazione header 
*/

/* 1) Si recuperano i puntatori alle aree dati di interesse */
   modelli = RtDbPGetPuntHeadmodelli( dbpunti);

/* 2) Si carica l'header */
   memcpy(h_reg->num_modelli,modelli,sizeof(NUMERI_MODELLI)*MAX_MODEL);

   h_reg->size_area_dati = RtDbPSizeData(dbpunti);
   h_reg->simulator.max_snap_shot  = _MAX_SNAP_SHOT;
   h_reg->simulator.max_back_track = _MAX_BACK_TRACK;
   h_reg->simulator.max_campioni   = _MAX_CAMPIONI;
   h_reg->simulator.num_var        = _NUM_VAR;
   h_reg->simulator.max_pertur     = _MAX_PERTUR;
   h_reg->simulator.spare_snap     = _SPARE_SNAP;
   h_reg->simulator.pert_clear     = _PERT_CLEAR;

   return(0);
}


/*
     Provvisoria per 2 modelli.
*/
int  StampaHeader(HEADER_REGISTRAZIONI hreg)
{
int mod;

     printf("Simulator fornisce (%d %d %d %d %d %d %d)\n",
             hreg.simulator.max_snap_shot,
             hreg.simulator.max_back_track,
             hreg.simulator.max_campioni,
             hreg.simulator.num_var,
             hreg.simulator.max_pertur,
             hreg.simulator.spare_snap,
             hreg.simulator.pert_clear);
     return(0);
}


int  StampePertFile(TIPO_PERT *pert, TIPO_PERT *pert_att, 
                    int save, int bktk)
{
int i;
int almeno_una=0;
char *bk="backtrack.dat";
char *sn="snapshot.dat";
char *sv="salvate";
char *rd="lette";
char *s;
char *f;

   s=sv;
   f=bk;
   if ( bktk==0 )
      f=sn;
   if ( save==0 )
      s=rd;
   printf("Perturbazioni %s file %s : \n %s %30s \n",
            s,f,"ATTIVE","ATTESA");
   for(i=0;i<_MAX_PERTUR;i++)
      {
      if( 
         (pert[i].tipo!=-1)     ||
         (pert_att[i].tipo!=-1)
        )
         {
         if( pert[i].tipo!=-1 )
            printf(" pos=%2d tipo=%2d ind=%2d ",
                   pert[i].posizione,
                   pert[i].tipo,
                   pert[i].indirizzo);
         else
            printf(" %s ","---------------------");
         if( pert_att[i].tipo!=-1 )
            printf("%8s pos=%2d tipo=%2d ind=%2d\n",
                   " ",
                   pert_att[i].posizione,
                   pert_att[i].tipo,
                   pert_att[i].indirizzo);
         else
            printf("%8s %s\n"," ","---------------------");
         }
      if( i==_MAX_PERTUR-1 )
         {
         printf(" %s ","*********************");
         printf("%8s %s\n", " ","*********************");
         }
      }
   return(0);
}


int ConvPertFloat(float **p_float, TIPO_PERT *p, int dimensioni)
{
int i,k,nelem,cicli;
div_t rapp;
int size=0;


        rapp = div(dimensioni,sizeof(float));
        if ( rapp.rem == 0 )
           {
           nelem = rapp.quot;
           }
        else
           {
           nelem = rapp.quot+1;
           }
        (*p_float) = (float*)calloc(nelem,sizeof(float));
        cicli = dimensioni/sizeof(TIPO_PERT);
        k=0;
        for(i=0;i<cicli;i++)
           {
#if defined REPLAY
           (*p_float)[k] = (float)p[i].runtime_replay;k++;size+=sizeof(float);
#endif
           (*p_float)[k] = (float)p[i].posizione;k++;size+=sizeof(float);
           (*p_float)[k] = (float)p[i].tipo;k++;size+=sizeof(float);
           (*p_float)[k] = (float)p[i].indirizzo;k++;size+=sizeof(float);
           (*p_float)[k] = (float)p[i].esaminata;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].valore_delta;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].rateo_durata;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].t;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].period;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].phase;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].meanvalue;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].wide;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].t_null_var;k++;size+=sizeof(float);
           (*p_float)[k] = p[i].post;k++;size+=sizeof(float);
           }
        /*printf("\t\t ConvPertFloat con size=%d\n",size);*/
        return(size);
}
/* Fine file */
