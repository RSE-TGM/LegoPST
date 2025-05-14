/**********************************************************************
*
*       C Source:               sna2f14.c
*       Subsystem:              1
*       Description:
*       %created_by:    ciccotel %
*       %date_created:  Tue Oct 22 14:41:42 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: sna2f14.c-8.1.1 %  (%full_filespec: sna2f14.c-8.1.1:csrc:1 %)";
#endif
/*															*/
/* sna2f14.c		v.1.0										*/
/*															*/
/* creato: 14 maggio 1996									*/
/* modif.: 21 maggio 1996									*/
/* autore: Marco Bonasoro (CISE)							*/
/*															*/
/* Realizzazione del nuovo snaf14 per snapshot dimensionati	*/
/* dinamicamente.											*/
/* Questa versione non gestisce la riscrittura degli F04.	*/
/*															*/




#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>
#include <Rt/RtMemory.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.h"
//#include "monit.h"
//#include "conv_mmi.h"

extern int ControlParam(int );

#define MAXLINE 150

RtDbPuntiOggetto	dbpunti;
RtErroreOggetto		errore1;
SNTAB			   *snapshot_hea;	/* area shm per tabella snapshot */
int					size_area_dati;	/* size in byte dell'area dati di tutte le task */
float			   *area_dati;		/* puntatore all'inizio dell'area dati delle task */

VARIABILI     	   *variabili;		/* database variabili di tutti i modelli */
int					shr_usr_key;	/* chiave utente per shared  */

FILE *fp14new, *fp14old;

S02 s02_;

#if defined MFFR
SNAP_SKED			sommari_snapshot;
#endif




void write_f14();

int    _MAX_SNAP_SHOT;
int    _MAX_BACK_TRACK;
int    _MAX_CAMPIONI;
int    _NUM_VAR;
int    _MAX_PERTUR;
int    _SPARE_SNAP;
int    _PERT_CLEAR;



SIMULATOR simpar;

int main(int argc, char **argv)

{
FILE *fp_snap;
int snap_sel, offset;
int num_modelli;
int i, ok_f04_dummy;
int *availSnap;
HEADER_REGISTRAZIONI hreg_snap;
int size_blocco_snap;
int err;

/* lettura s02 */
	read_S02_net();

/* creazione dell'oggetto RtDbPunti */
        for(i=1;i<argc-1;i++)
            {
            if(strcmp(argv[i],ARGV_N_SNAP)==0)
                  _MAX_SNAP_SHOT  = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_BKTK)==0)
                  _MAX_BACK_TRACK = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_CAMP_CR)==0)
                  _MAX_CAMPIONI   = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_VAR_CR)==0)
                  _NUM_VAR        = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_PERT_ACTIVE)==0)
                  _MAX_PERTUR     = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_SPARE)==0)
                  _SPARE_SNAP     = atoi(argv[i+1]);
            if(strcmp(argv[i],ARGV_N_PERTCL)==0)
                  _PERT_CLEAR     = atoi(argv[i+1]);
            }
        printf("\t\t snaf14 [%d %d %d %d %d %d %d]\n",
               _MAX_SNAP_SHOT,
               _MAX_BACK_TRACK,
               _MAX_CAMPIONI,
               _NUM_VAR,
               _MAX_PERTUR,
               _SPARE_SNAP,
               _PERT_CLEAR);

         simpar.max_snap_shot  = _MAX_SNAP_SHOT;
         simpar.max_back_track = _MAX_BACK_TRACK;
         simpar.max_campioni   = _MAX_CAMPIONI;
         simpar.num_var        = _NUM_VAR;
         simpar.max_pertur     = _MAX_PERTUR;
         simpar.spare_snap     = _SPARE_SNAP;
         simpar.pert_clear     = _PERT_CLEAR;
          

         availSnap = (int*)calloc(_MAX_SNAP_SHOT,sizeof(int));

        err = ControlParam("snaf14");
        if ( err!=0 )
           {
           system ("xterm -T SNAF14ERR -e vi parametri.out &");
           exit(fprintf(stderr,
                "Activation and file session parameters are incongruous.\n\n"));
           }
	
	/* decodifica SHR_USR_KEY */
	shr_usr_key = atoi((char *) getenv("SHR_USR_KEY"));

	/* aggancio al database dei punti */
	errore1 = RtCreateErrore(RT_ERRORE_TERMINALE,"net_sked");
#if defined SCO_UNIX
	dbpunti = RtCreateDbPunti(errore1,"S02_SCO",DB_PUNTI_INT,&simpar);
#endif
#if defined OSF1
	dbpunti = RtCreateDbPunti(errore1,"S02_OSF1",DB_PUNTI_INT,&simpar);
#endif
#if defined LINUX
	dbpunti = RtCreateDbPunti(errore1,"S02_LINUX",DB_PUNTI_INT,&simpar);
#endif
#if defined AIX
	dbpunti = RtCreateDbPunti(errore1,"S02_AIX",DB_PUNTI_INT,&simpar);
#endif
#if defined VMS
	dbpunti = RtCreateDbPunti(errore1,"S02_VMS",DB_PUNTI_INT,&simpar);
#endif

/* Breve spiegazione del programma */
      system("clear");
      printf("\n\n\n\n\t\t\t Program snaf14 with parameter reading:\n");
      printf("\n\t MAX_SNAP_SHOT %5d   MAX_BACK_TRACK %5d\n",
               _MAX_SNAP_SHOT,
               _MAX_BACK_TRACK);
      printf("\n\t MAX_CAMPIONI  %5d   NUM_VAR        %5d\n",
               _NUM_VAR,
               _MAX_CAMPIONI);
      printf("\n\t MAX_PERTUR    %5d   SPARE_SNAP     %5d   PERT_CLEAR %1d\n\n",
               _MAX_PERTUR,
               _SPARE_SNAP,
               _PERT_CLEAR);
      printf("\n\t This program writes a new f14 file called f14.s in which\n");
      printf("\n\t variables' values are picked up from the snapshot\n");
      printf("\n\n\n\t press return to continue\n\n");
      getchar();

	/* azzeramento shared memory */
	RtDbPClearData(dbpunti);

	/*  recupero il puntatore all'area degli header degli snapshot */
	snapshot_hea = RtDbPGetPuntSnapshot(dbpunti);
	area_dati = RtDbPPuntData(dbpunti);
	size_area_dati = RtDbPSizeData(dbpunti);

	/* lettura tabella snapshot */
	if (!(fp_snap = fopen("snapshot.dat", "r")))
	{
		printf("\ncannot open snapshot.dat\n");
		exit(-1);
	}
	else	/* scrittura dell'elenco snapshot disponibili */
	{
                fread(&hreg_snap,sizeof(HEADER_REGISTRAZIONI),1,fp_snap);
		printf("\n\n");
		for (i=1; i<= _MAX_SNAP_SHOT; i++)
		{
			availSnap[i-1] = 0;
			fread (&snapshot_hea[i-1], sizeof(SNTAB), 1, fp_snap);
			if (snapshot_hea[i-1].stat == 1)
			{
				availSnap[i-1] = 1;
				printf("%d)   descrizione= %s\n\n",
						snapshot_hea[i-1].prog,
						snapshot_hea[i-1].descr);
/*clever hack*/ snapshot_hea[i-1].pos = i;
			}
		}
	}

	for(;;)
	{
		printf("choose snapshot number:" );
		if(scanf("%d",&snap_sel) > 0)
		{
			/*
			if ((snap_sel >=1) && (snap_sel <= tot_snap))
				break;
			else if (snap_sel > tot_snap)
				printf("\nplease choose a number between 1 and %d\n\n", tot_snap);
			*/
			if ((snap_sel < _MAX_SNAP_SHOT) && (availSnap[snap_sel-1] == 1))
				break;
			else
				printf("\nsnapshot number %d not available: please select from list\n\n", snap_sel);
		}
		else
		{
			printf("\n\n\n\n wrong non numeric input: program terminated\n");
			fflush(stdout);
			exit(-1);
		}
	}
	printf("\n\n chosen number %d\n\n",snap_sel);

	/* lettura di risposta ininfluente (richiesta per compatibilita') */
/**********
	printf("\n Please confirm rewriting of f04.dat [yes=1 no=0]\n(Warning: functionality not available in this release - sorry)\n");
	scanf("%d", &ok_f04_dummy);
************/
  printf("\t Notice: rewriting of f04.dat functionality not available.\n\n");

/* 
   caricamento in shared memory dello snapshot scelto 
*/
        size_blocco_snap = size_area_dati    +  sizeof(float)     +
#if defined MFFR
                           sizeof(SNAP_SKED) +
#endif
                           DIM_SNAP_AUS      +
                           _SPARE_SNAP * AREA_SPARE * sizeof(char)         +
                           2 * _MAX_PERTUR * sizeof(TIPO_PERT);

        offset = _MAX_SNAP_SHOT * sizeof(SNTAB) + sizeof(HEADER_REGISTRAZIONI) +
                 (snapshot_hea[snap_sel - 1].pos - 1) * size_blocco_snap;

	fseek(fp_snap, offset, 0);
	fread(area_dati, size_area_dati, 1, fp_snap);
#if defined MFFR
	fread(&sommari_snapshot, sizeof(SNAP_SKED), 1, fp_snap);
#endif

	fclose(fp_snap);

	/* a questo punto, occorre reperire le informazioni dettagliate */
	/* sul numero di ingressi, dati, uscite dei vari modelli */
	/* dopodiche' bisogna accedere direttamente alla "area_dati" */
	/* per riscrivere l'f14. */
	num_modelli = RtDbPNumeroModelli(dbpunti);
	for (i=0; i<num_modelli; i++)
		write_f14(i);

/*      Per intercettare il return dovuto alla scelta del numero di snapshot */
        getchar();
	printf("\n Please confirm end :  press return\n");
        getchar();
        RtDestroyDbPunti(dbpunti);
        printf("\n*** snaf14 successfull ***\n");
}




int loadAline (char line[MAXLINE], int *len)

{
	int c, k;

	for (k=0; (c= fgetc(fp14old)) != '\n'; k++)
	{
		if (c == EOF)
			return(1);
		line[k] = c;
	}

	line[k] = '\0';
	*len = k;
	return(0);
}




void fixValue (char line[MAXLINE], float val)

{
	char valchar[15];
	char *pline, *lennum;

	pline = line;
	pline += 14;

	/* se il numero e` positivo, si aumenta la precisione */
	if (val < 0)
		sprintf(valchar, "%-9.3E", val);
	else
		sprintf(valchar, "%-9.4E", val);

	/* se il numero e` intero, si aggiunge il punto */
	if ((strchr(valchar, '.') == NULL) && (strchr(valchar, 'E') == NULL))
	{
		lennum = strchr(valchar, ' ');
		memcpy(lennum, ".", 1);
	}

	memcpy(pline, valchar, 10);
}




void write_f14(int nmod)

{
	int modNeqsis, modNu, modInizio;
	float correctVal;
	char filename[256], line[MAXLINE];
	int k, len, iret;

	strcpy(filename, s02_.model[nmod].path);
	strcat(filename, "f14.dat");
	fp14old = fopen(filename, "r");
	if (fp14old == NULL)
	{
		printf("write_f14: cannot open original f14.dat\n");
		fflush(stdout);
		exit(1);
	}

	strcpy(filename, s02_.model[nmod].path);
	strcat(filename, "f14.s");
	fp14new = fopen(filename, "w");
	if (fp14new == NULL)
	{
		printf("write_f14: cannot open new file f14.s\n");
		fflush(stdout);
		exit(1);
	}

	/* copia delle prime 4 righe */
	for (k=0; k<4; k++)
	{
		iret = loadAline(line, &len);
		fprintf(fp14new, "%s\n", line);
	}

	/* recupero informazioni sul modello */
	/*modNeqsis = RtDbPGetStatiModello(dbpunti, nmod);*/
	modNeqsis = s02_.model[nmod].punt_ivin - s02_.model[nmod].punt_ivus;
	/*modNu = RtDbPGetIngressiModello(dbpunti, nmod);*/
	modNu = s02_.model[nmod].punt_ider - s02_.model[nmod].punt_ivin;

	/* riscrittura delle righe corrette per variabili di uscita ed ingresso */
	modInizio = RtDbPInizioModelli(dbpunti, nmod);
	for (k=0; k<modNeqsis; k++)
	{
		iret = loadAline(line, &len);
		correctVal = RtDbPGetValueD(dbpunti, modInizio + k);
		fixValue(line, correctVal);
		fprintf(fp14new, "%s\n", line);
	}

	iret = loadAline(line, &len);		/* commento "CONDIZIONI INIZIALI" */
	fprintf(fp14new, "%s\n", line);

	for (k=0; k<modNu; k++)
	{
		iret = loadAline(line, &len);
		correctVal = RtDbPGetValueD (dbpunti, modInizio + k + modNeqsis);
		fixValue(line, correctVal);
		fprintf(fp14new, "%s\n", line);
	}

	/* copia pedissequa delle righe rimanenti */
	while (loadAline(line, &len) == 0)
		fprintf(fp14new, "%s\n", line);

	fclose(fp14old);
	fclose(fp14new);
}
