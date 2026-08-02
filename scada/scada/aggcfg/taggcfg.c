/**********************************************************************
*
*       C Source:               taggcfg.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Fri Dec 13 14:32:32 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: taggcfg.c-4 %  (%full_filespec: taggcfg.c-4:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
/* 
   TAGGCFG.C
	   il task permette di aggiornare i files operativi dei
   	punti e le pagine video provenienti da config.

   Creazione (a partire da "tlink.c"):
      13.10.92  -  GM.Furlan
         Utilizzo Rete Ethernet - Gestione in singolo

   20 Novembre 1994
      Inserimento gestione aggiornamento MMI Window
      Messaggi WinDFile WinIFile.
*/

#include <osf1.h> 
#include <io.h> 
#include <dos.h> 
#include <stdio.h> 
#include <string.h>
#if !defined OSF1 && !defined LINUX
#include <direct.h>
#endif
#include <stdlib.h> 
#if defined OSF1 || defined LINUX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "maxpar.inc"
#include "dconf.inc"
#include "mesqueue.h"
#include "g2comdb.inc"
#include "g1tipdb.inc"
#include "comunic.inc"
#include "diagnodi.inc"
#include "messcada.inc"
#include "mesprocv.inc"
#include "pscserr.inc"                   
#include "netmsg.h"
#include "netstr.h"
#include "aggcfg.inc"

#if defined OSF1 || defined LINUX
char release_slave[4] = {0};
#else
extern	char release_slave[];
#endif
extern   DB_HEADER h_db;

S_VOUUT_CONFIG stcfg;      		/* struttura messaggio di stato config */
short stvid[max_video];          /* stato dei video */

// Function declarations
int diagnet(short pdb, SPO_STATO *mess, short *porta);
int bitvalue(short *buffer, short indice);
void pscserr(int stop_task, int task_id, int routine_id, int error_code, int sys_halt);
void wai(int delay);
void CloseConn(void);
void scd(void);
void sce(void);
void InvSlave(short *mess, short lmsg);
void wrdb(int header);
int wbyte(int fd, char *buffer, int offset, int length);
void NetMarteEnd(void);
int fcloseall(void);
void RestIDT(void);
void rwdbal(int flag, DB_HEADER *db1, DB_HEADER *db2);
int DosRemove(char *filename);
int DosRename(char *oldname, char *newname);
void to_ascii(char *filename);
void readdb(DB_HEADER *db);
void allocdb(int flag, DB_HEADER *db);
void RestartScada(void);

void taggcfg()	
{
short lung;
short num_mes;                   // numero di messaggi (file *.RTF) inviati
short	i, lungd, ier;
short varstvid[max_video];   /* tabella variazione stato dei video */
#if defined OSF1 || defined LINUX
char  *pathdef="./pscstmp";     // direttorio temporaneo
#else
char  *pathdef="c:/pscstmp";     // direttorio temporaneo
#endif
char  fnome[FILENAME_MAX+1], fdest[FILENAME_MAX+1], filerx;
char  flmmi;                     // flag ricezione file mmi
long  offset, offlista;
FILE  *fp;
DB_HEADER u_db;
short errorx;							/* variabile per test errori trasferimento */
short point;
short portaRet;

QUEUE_PACKET packr;		         /* pacchetto ricezione messaggio   	  */
QUEUE_PACKET packmmi;		      /* pacchetto accodamento verso mmi    */
QUEUE_PACKET packcon;   		   /* pacchetto accodamento verso config */
char stato;                      /* stato della config  */
char *strscfg[]={"CONFIG ON  ",
                 "           "}; /* stringa di stato della config  */
/* codice colori stringa di stato della config  */
short colscfg[]={0x17,           /* sfondo rosso, scritta bianca   */
                 0x0  };         /* sfondo nero, scritta nera      */

RX_STATOSIS ssys;						/* risposta stato sistema				 */
TX_ACK		conack;					/* ack per config							 */

#if defined OSF1 || defined LINUX
mkdir(pathdef,S_IRWXU|S_IRGRP|S_IROTH);                  /* creazione direttorio temporaneo */
#else
mkdir(pathdef);                  /* creazione direttorio temporaneo */
#endif

packr.que=c_aggcfgi;
packr.flg=MSG_WAIT;
packr.amsg=(char *)&mconfig;

packmmi.flg=MSG_WAIT;
packmmi.wto= 0;
packmmi.que=c_mmio;
	  
packcon.wto=0;
packcon.que=c_aggcfgo;
packcon.flg=MSG_WAIT;

conack.nodo=0;		  		/*	inizializzazione messaggio ack	*/
conack.mess=MESSACK;

errorx=0;               /* flag errore in scrittura su disco */ 
filerx=0;               /* flag di ricezione files in corso  */
flmmi=0;                /* flag ricezione file mmi           */
num_mes=0;              /* numero di messaggi inviati        */

LOOP:
packr.wto=0;
packr.que=c_aggcfgi;
dequeue(&packr);
#if !defined OSF1 && !defined LINUX	// Per tappare un buco del caricamento della configurazione
fwai(0.02);	// apparentemente arrivava un messaggio duplicato
#endif

switch((unsigned char)mconfig.i.tipomess)
{
case STATOPORTA:
      if((stato=diagnet(config,(SPO_STATO*)&mconfig,&portaRet))>-1)    // variazione di stato della config
      {
      short lungst;
      // invio stringa di stato config ai video
      stcfg.indice=vouut;
      stcfg.zona=0;
      stcfg.video=1;
      stcfg.x=0;
      stcfg.y=0;
      stcfg.colore=colscfg[stato];
      stcfg.zoom=0;
      lungst=strlen(strscfg[0]);
      stcfg.lungh=lungst;
		strncpy((char *)stcfg.buff,strscfg[stato],lungst); // stringa
      stcfg.buff[lungst]=-1;    // tappo
      stcfg.buff[lungst+1]=-1;    // tappo
	   packmmi.amsg=(char *)&stcfg;
	   packmmi.lmsg=sizeof(S_VOUUT_CONFIG);
      for (i=0;i<num_video;i++)
      {
         // clear tabella variazione stato dei video 
         varstvid[i]=0;
         // memorizzazione stato video 
         stvid[i]=(dbdd[(stvide1)+i] & mask_sl);
         stcfg.nodo=i;
         enqueue(&packmmi);
      }   
		num_mes=0;
		flmmi=0;
    }
   break;

   case STATOSIS: 							// richiesta stato sistema
   case STATOSISES: 							// richiesta stato sistema
	  if(!sys_master) ssys.mess=NO_STATOSIS;		// sono slave
	  else  ssys.mess=STATOSIS;
	  ssys.nodo=0;
	  ssys.status=0;
	  ssys.rete=0;
	  packcon.amsg=(char*)&ssys;
	  packcon.lmsg=sizeof(ssys);
     if (SYS_DUALE)
         {
	  	   point=stwdb;
	  	   if(bitvalue(&dbdd[sysab],g2di_sl))
	  	      {
	  		   ssys.status=MASK_IDEN;			// sistema B
			   point=stwda;						// duale A
	         }
	      if(bitvalue(&dbdd[master],g2di_sl)) ssys.status=ssys.status | MASK_MASTER;
	      if(bitvalue(&dbdd[point],g2di_sl)) ssys.status=ssys.status | MASK_WDDUALE;
         if(mconfig.i.tipomess==STATOSIS)
   	      memcpy(ssys.rel_duale,release_slave,4);
         else
            {
      	      memcpy(ssys.rel_duale,release_slave,2);
      	      memcpy(&ssys.rel_duale[2],&release_slave[2],2);
            }
         }
     else
         {
	      ssys.status=MASK_MASTER;				// singolo A Master
	      memset(ssys.rel_duale,0,4);
         }
     if(mconfig.i.tipomess==STATOSIS)
  	      memcpy(ssys.rel_propria,h_db.release,4);
     else
     {
        memcpy(ssys.rel_duale,release_slave,2);
        memcpy(&ssys.rel_duale[2],&release_slave[2],2);
     }
	  ssys.n_video=num_video;
	  memcpy(ssys.stvideo,stvid,num_video*2);
	  enqueue(&packcon);
 	break;

	case FORZAMASTER:
     pscserr(STOP_TASK,TASK_AGGCFG,ROU_AGGFMA,0,SYS_HALT);
	break;

  case FILE_MMI:         /* messaggio MMI: reindirizzo su coda MMI */

  flmmi=1;              /* setting del flag di arrivo messaggio MMI  */
  /*
    messaggio verso mmi con indice a 40 
  */
  uconf.buff_mmi.indice=40;			
  uconf.buff_mmi.micro=1;
  uconf.buff_mmi.indfile=mconfig.mmi.ind_file;
  uconf.buff_mmi.indblocco=mconfig.mmi.ind_blocco;
  memcpy(uconf.buff_mmi.blocco,(char *)&mconfig.mmi.bl_dsk[0],512);
  packmmi.amsg=(char *)&uconf.buff_mmi;
  packmmi.lmsg=sizeof(uconf.buff_mmi);
  packcon.amsg=(char*)&conack;
  packcon.lmsg=sizeof(conack);
  for (i=0;i<num_video;i++)
     {
     // controllo variazione stato video
     if( !varstvid[i] )  // nessuna variazione stato video (l'eventuale
     {                   // variazione e' segnalata dal task "scgev")
      if( !stvid[i] )      // video on line
        {
	        uconf.buff_mmi.nodo=i;
	        uconf.buff_mmi.video=1;			
		     enqueue(&packmmi);				 // invio a mmi messaggio
			  if(i && !(i % 2)) wai(2);
		  }
     }
     else      // variazione stato video: chiusura comunicazione
     {
		  CloseConn();
		  goto LOOP;				// fine trasmissione
     }
  }
  if(mconfig.mmi.ack) enqueue(&packcon); 			 // se richiesto invio a config ack
  break;

  case mWinIFile:         		/* messaggio File Window MMI	  */
  case mWinDFile:         		/* messaggio File Window MMI	  */

  packmmi.amsg=packr.amsg;
  packmmi.lmsg=packr.lmsg;
//
//	Se si tratta del primo messaggio individuo le postazioni MMI
//	in linea a cui inviarlo. Se, durante la trasmissione dei successivi
//	messaggi uno o piu' mmi si scollegano interrompo l'intero aggiornamento
//
   if(mconfig.winFile.nome[0])		// prima trasmissione
	{
		scd();
	   for (i=0;i<num_video;i++)
   	{
	     varstvid[i]=0;       							// clear tabella variazione stato dei video 
   	  stvid[i]=(dbdd[(stvide1)+i] & mask_sl); // memorizzazione stato video 
	   }
  		sce();
	}
	for (i=0;i<num_video;i++)
   {
     // controllo variazione stato video
		if( !varstvid[i] )  // nessuna variazione stato video (l'eventuale
      {                   // variazione e' segnalata dal task "scgev")
      	if( !stvid[i] )      // video on line
      	{
	        mconfig.winFile.nodo=i;
		     enqueue(&packmmi);				 // invio a mmi messaggio
			  if(i && !(i % 2)) wai(2);
		 	}
     	}
		else      // variazione stato video: chiusura comunicazione
    	{
		  CloseConn();
		  goto LOOP;				// fine trasmissione
    	}
	}
   if(mconfig.winFile.bAck)
   {
		packcon.amsg=(char*)&conack;
		packcon.lmsg=sizeof(conack);
  		enqueue(&packcon); 			 // se richiesto invio a config ack
	}
  break;

 case TESTA_FILE :
// _visch('I');
//	printf(" TestaFile %d",num_mes);
   if (SYS_DUALE)
      {
      if(mconfig.i.aggtype!=AGG_MASTER) 
         InvSlave((short *)&mconfig,packr.lmsg);
      }

  if(mconfig.i.aggtype==AGG_SLAVE && sys_master) break;
  num_mes++;
  if(mconfig.i.nome[0]==file_iniz)
  {
    if(!strncmp(&mconfig.i.nome[1],file_rev,strlen(file_rev)))
    {
       strcpy(h_db.release,&mconfig.i.nome[3]);
       wrdb(wr_header);
       break;
    }
    /*
	   inizio trasmissione configurazione reset variabile errorx
   	azzeramento file con nome files precedente trasferimento
    */
    offlista=0;
	 errorx=0;
	 if(filerx) { fclose(fp); filerx=0; }  // se file aperto provvedo alla sua chiusura
	 fp=fopen(LISTA,"wb+");
	 if(!fp) ++errorx;
	 else	  fclose(fp);
	 break;
  }
  /*
    apro il file indicato
  */
  if(filerx) { fclose(fp); filerx=0; }  // se file aperto provvedo alla sua chiusura 
  filerx= 1;                             
  errorx++;                             // incremento ricevuto
  memset(fnome,0,sizeof(fnome));
  lung=strlen(pathdef);						// lunghezza path di defaut
  strcpy(fnome,pathdef);			  
  for(i=0; i<strlen(fnome); i++) if(fnome[i]=='/') fnome[i]='\\';
  fnome[lung]=0x5c;
 
  offset= 0;
  memcpy(&fnome[lung+1],mconfig.i.nome,lun_fnome);
  fp=fopen(LISTA,"rb+");
  if(wbyte(fileno(fp),mconfig.i.nome,offlista,lun_fnome)==-1)
	     pscserr(STOP_TASK,TASK_AGGCFG,ROU_AGGFIL,0,SYS_CONT);
  offlista=offlista+lun_fnome;
  fclose(fp);
  fp=fopen(fnome,"wb+");
  if(fp==NULL) { perror(""); errorx=1;
					  pscserr(STOP_TASK,TASK_AGGCFG,ROU_AGGFIL,1,SYS_CONT);}
 break;

 case DATI_FILE:
//  _visch('D');
//	printf("Dati File %d",num_mes);
   if (SYS_DUALE)
      {
      if(mconfig.d.aggtype!=AGG_MASTER) 
         InvSlave((short *)&mconfig,packr.lmsg);
      }
  if(mconfig.d.aggtype==AGG_SLAVE && sys_master) break;
  num_mes++;
  /*
   	scrivo i dati inviati
  */
  if(wbyte(fileno(fp),mconfig.d.dati,offset,mconfig.d.numbyte)==-1) 
		 pscserr(STOP_TASK,TASK_AGGCFG,ROU_AGGFIL,2,SYS_CONT);
  offset += mconfig.d.numbyte;
 break;

 case FINE_FILE:
//  _visch('F');
//	printf("Fine File %d",num_mes);
   if (SYS_DUALE)
      {
      if(mconfig.d.aggtype!=AGG_MASTER) 
         InvSlave((short *)&mconfig,packr.lmsg);
      }
  if(mconfig.d.aggtype==AGG_SLAVE && sys_master) break;
  num_mes++;
  /*
   	fine file
  */
  fclose(fp);
  filerx= 0;
  errorx--;              // file arrivato correttamente decremento contatore
 break;

 case FINE_TX:
//  _visch('X');
//	printf("Fine Tx %d",num_mes);
   if (SYS_DUALE)
      {
      if(mconfig.d.aggtype!=AGG_MASTER) 
         InvSlave((short *)&mconfig,packr.lmsg);
      }
  if(mconfig.d.aggtype==AGG_SLAVE && sys_master) break;
  num_mes++;
 break;

 case EOM:
  printf("****E\n");
  if(flmmi) 
      flmmi=0;
   else
      {
      if  (SYS_DUALE)
         {
	      if(mconfig.x.aggtype!=AGG_MASTER) 
            InvSlave((short *)&mconfig,packr.lmsg);
         }
	   if(mconfig.x.aggtype==AGG_SLAVE && sys_master) break;
	   printf("\nmsg: %d-%d",num_mes,mconfig.x.tipofile);
      if (num_mes!=mconfig.x.tipofile)
      {
         ++errorx;
         pscserr(ERR_TASK,TASK_AGGCFG,ROU_AGGMES,0,SYS_CONT);
      }

    /*
   	fine trasmissione
      eseguo il test sul corretto arrivo di tutti i files
      - se non ci sono errori provvedo:
        1 - alla chiusura i tutti i files
        2 - ritorno al sistema operativo 
        3 - copio il files nuovi nel sottodirettorio di lavoro
        4 - eseguo il restart del sistema.
      - se errore
        1 - non effettuo nessuna operazione
    */
      if(!errorx)  // se non ci sono errori ricopio i nuovi files su quelli operativi 
      {
/*
	attendo completamento trasferimento dati su sistema slave
	prima di provocare la caduta del watch dog
*/
      if (SYS_DUALE)
         {
	   	if(mconfig.x.aggtype!=AGG_MASTER && sys_master)
				{
					while( bitvalue(&dbdd[stwda],g2di_sl)
					&& bitvalue(&dbdd[stwdb],g2di_sl))	wai(2);	// attesa termine operazioni anche su slave
				}
         }
#if defined (AC_INETDUALE) 
			InetEnd();					// fine comunicazioni Inet
#endif
         printf("\nFine TR");     // obbligo la fine delle operazioni di IO
                                  // di tutti i task meno prioritari
      	scd();							// blocco lo scheduler per impedire
      										// a pscswd di intervenire
         if(NET_ABIL)
   	      NetMarteEnd();          // chiusura comunicazione rete Etehernet
      	fcloseall();               // chiudo tutti i files aperti
         RestIDT();              // ripristino IDT DOS (era stata salvata
                                 // da beg_marte con SaveIDT)
      	rwdbal(1,&h_db,&h_db);  // salvo dbs
      
      	memset(fdest,0,sizeof(fdest));	 // preparo path destinazione
      	lungd=strlen(&conf[RTF][0]);
         strcpy(fdest,&conf[RTF][0]);			  
         for(i=0; i<strlen(fdest); i++) if(fdest[i]=='/') fdest[i]='\\';
         fdest[lungd]=0x5c;
      
      	fp=fopen(LISTA,"rb+");
      	for(;;)
      	{
      		if(fread(&fnome[lung+1],lun_fnome,1,fp)!=1) break;
            strncpy(&fdest[lungd+1],&fnome[lung+1],lun_fnome);
      		remove(fdest);				// cancello file
      	   if(rename(fnome,fdest)) 
               perror ("TAGGCFG: rename");
      	}
#if defined OSF1 || defined LINUX
	to_ascii("fvirtprn.rtf");
	to_ascii("fdizst.rtf");
	to_ascii("fdesal.rtf");
#endif
         fclose(fp);
      /*
        		stop sistema: leggo la nuova configurazione	del data base.
				Se il n. di punti aa, as, ac e ad e' il medesimo salvo i valori
				del data base di diagnostica (contengono i contatori degli archivi)
      */
			readdb(&u_db);
			if(u_db.dimaa==h_db.dimaa && u_db.dimas==h_db.dimas
			&& u_db.dimac==h_db.dimac && u_db.dimad==h_db.dimad)
			{
					memset(&u_db,0,sizeof(DB_HEADER));
					u_db.dimad=h_db.dimad;
		      	rwdbal(1,&h_db,&u_db);   // riscrivo i valori degli analogici di diagnostica

			}
         allocdb(1,&h_db);
#if defined OSF1 || defined LINUX
	RestartScada();
#else
      	exit(0);
#endif
      }
   }
   num_mes=0;              /* azzero numero di messaggi inviati */
 break;
 default:         // messaggio sconosiuto
      pscserr(ERR_TASK,TASK_AGGCFG,ROU_AGGSCO,mconfig.i.tipomess,SYS_CONT);
 break;
}
goto LOOP;
}
/*
	Chiusura connessione con Config
*/
void CloseConn()
{
	QUEUE_PACKET pack;

	pack.wto=0;
	pack.que=c_aggcfgo;
	pack.flg=MSG_WAIT;
   pack.lmsg=6;
   pack.amsg=(char*) &uconf;

	uconf.buff[0]=-1;		// il messaggio e' per txnet che chiude la
   uconf.buff[1]=MODOFF;	// comunicazione con config immediatamente		
   uconf.buff[2]=0;			
  	enqueue(&pack);
}

/*
	InvSlave

   Usata solo se il sistema � duale.
	Routine per l'invio del messaggio al sistema slave se il sistema e'
	master.	Nella prim word (n. nodo) viene inserito il codice del
	messaggio RTF_TRASF interpretato dal task duale.
	Se l'accodamento non ha buon esito vien chiusa la connessione con
	la config

	Parametri

	short *	indirizzo pacchetto
	short		lunghezza messaggio

	Ritorno

	nessuno
*/
/* Stile prototipo, NON K&R: il prototipo (riga 79) dichiara
   void InvSlave(short *, short); nel K&R lmsg era promosso a int.
   Vedi docs/KR_PROTOTYPE_AUDIT.md. */
void InvSlave(short *mess, short lmsg)
{
	QUEUE_PACKET	pack;

	if(sys_master)
		{
			if(!bitvalue(&dbdd[slaveiniz],g2di_sl))
				{
					pack.wto=100;
					pack.flg=MSG_WAIT;
					pack.que=c_dua_tx;
					pack.amsg=(char *)mess;
					pack.lmsg=lmsg;
					*mess=RTF_TRASF;
					if(enqueue(&pack)) CloseConn();	// time out
				}
			else CloseConn();					// slave non aggiornato
		}
	return;
}
