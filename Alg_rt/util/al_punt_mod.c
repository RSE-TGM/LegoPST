/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include <Rt/Rt.h>
#include <Rt/RtDbPunti.h>  /* inclusione header pubblico */
#define  ARGV_PATHS02	"-pathS02"
#define  ARGV_N_MOD     "-nummod"
SIMULATOR *simpar;     /* Struttura con parametri */    
int main(argc,argv)
int argc;
char **argv;

{
RtDbPuntiOggetto dbpunti;
int i;
int valore=-1;
			/* Parametri letti dal file Simulator */
int _MAX_SNAP_SHOT;
int _MAX_BACK_TRACK;
int _MAX_CAMPIONI;
int _NUM_VAR;
int _MAX_PERTUR;
int _SPARE_SNAP;
int _PERT_CLEAR;
char _PATH_S02[FILENAME_MAX];
int _NUM_MOD;


if (argc<9) { /* da sistemare */
 
  fprintf(stderr,"\n Sintassi di al_punt_mod non corretta\n");
  fprintf(stderr,"\t Numero parametri non corretto \n");
  exit(1);
  }
/*
Inserisco argomenti passati da al_punt_mod
*/
for(i=0;i<argc-1;i++)
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
      if(strcmp(argv[i],ARGV_PATHS02)==0)    /* path dove c'e' l'S02 */
                  strcpy(_PATH_S02,argv[i+1]);
      if(strcmp(argv[i],ARGV_N_MOD)==0)       /* numero modello in input */
                  _NUM_MOD     = atoi(argv[i+1]);
      }
   printf("all_punt_mod  con = [%d %d %d %d %d %d %d %s %d]\n",
           _MAX_SNAP_SHOT , _MAX_BACK_TRACK,
           _MAX_CAMPIONI  , _NUM_VAR       ,
           _MAX_PERTUR    , _SPARE_SNAP    , _PERT_CLEAR ,
	   _PATH_S02 ,     _NUM_MOD);

simpar = (SIMULATOR*)calloc(1,sizeof(SIMULATOR));

simpar->max_snap_shot  = _MAX_SNAP_SHOT;
simpar->max_back_track = _MAX_BACK_TRACK;
simpar->max_campioni   = _MAX_CAMPIONI;
simpar->num_var        = _NUM_VAR;
simpar->max_pertur     = _MAX_PERTUR;
simpar->spare_snap     = _SPARE_SNAP;
simpar->pert_clear     = _PERT_CLEAR;

strcat(_PATH_S02,"/S02_OSF1");

dbpunti=RtCreateDbPunti(NULL,_PATH_S02,DB_PUNTI_SHARED,simpar);

valore=RtDbPInizioModelli(dbpunti,_NUM_MOD);

printf("OFFSET=%d \n",valore);

} 	/* end main */
