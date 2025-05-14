#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAP.h"		/* Serve per la gestione delle licenze software*/
#include "skey_manager3.h"	/* Serve per la gestione delle licenze hardware*/


int main(int argc, char **argv)
{
char   ProgramID[64];
char   NewProgramID[64];
char   Buffer[ENELINFO_LEN + 1];
int indiceSocieta; //serve per gestire eventualmente comportamenti differenziati
char messaggioErrore[1024];
int errorCode;
int richiestaScrittura;

   if(argc!=2)
   {
      fprintf(stderr,
      "Error!!! Number of parameters on command line\n\t");
      fprintf(stderr,
      "<check_license> <ProgramID>\n");
      exit(1);
   }
   else
   {
//     if (strlen(argv[1]) != PROGRAMID_LEN) {
//        fprintf ( stderr ,"check_license: ProgramID length != %d \n",PROGRAMID_LEN) ;
//        exit(1);
//     }
     strcpy(ProgramID,argv[1]);
   }


/*
   Si controlla se esiste la licenza per l'attivazione del programma.
   Prima la chiave hardware ed eventualmente quella software
*/


#define LICRUNTIME    "ALGRUNTIME"
#define LICMMI    "ALGMMI"
#define LICCONFIG    "ALGCONFIG"
#define LICSCADA    "ALGSCADA"
#define LICLEGOCAD    "ALGLEGOCAD"


// traduco le chiavi esterne in interne

       if (!strcmp(ProgramID, "algrt"))
       {
       	strcpy(NewProgramID, LICRUNTIME);
       }
       else if (!strcmp(ProgramID, "alglc"))
       {
       	strcpy(NewProgramID, LICLEGOCAD);
       }
       else if (!strcmp(ProgramID, "algcf"))
       {
       	strcpy(NewProgramID, LICCONFIG);
       }
       else if (!strcmp(ProgramID, "algsc"))
       {
       	strcpy(NewProgramID, LICSCADA);
       }
       else if (!strcmp(ProgramID, "algmm"))
       {
       	strcpy(NewProgramID, LICMMI);
       }
       else // probabilmente è proprio quello che si vuole
       {
       	strcpy(NewProgramID, ProgramID);
       }



       // Controllo chiave hardware

#if defined YESSMARTKEY
       richiestaScrittura=KEYWRITE;
       errorCode=skey_verifica_e_carica3(richiestaScrittura, KEYLABEL, KEYPASSWD,
                                         NewProgramID, &indiceSocieta);
#else
       errorCode=-1;
#endif

       if(errorCode == 0)
         {
	    	fprintf(stderr, "##################################################\n");
		fprintf(stderr, "#                                                #\n");
		fprintf(stderr, "#                 LICENSE OK!!!                  #\n");
		fprintf(stderr, "#                                                #\n");
		fprintf(stderr, "##################################################\n");
		exit(0);
	 }
	 else
         {
#if defined YESSMARTKEY
          	skey_stampa_errore(errorCode, &messaggioErrore);
          	printf("CONFIG:%s \n", messaggioErrore);
#endif
          	//provo l'eventuale chiave software
          	if(CAP(CONFIG_LIC, Buffer) == -1)
		{
			fprintf(stderr, "##################################################\n");
			fprintf(stderr, "#                                                #\n");
			fprintf(stderr, "#              LICENSE NOT FOUND!!!              #\n");
			fprintf(stderr, "#                                                #\n");
			fprintf(stderr, "##################################################\n");             		
			exit(1);
          	}
	 	if(CAP(ProgramID, Buffer) == -1)
	 	{
			fprintf(stderr, "##################################################\n");
			fprintf(stderr, "#                                                #\n");
			fprintf(stderr, "#              LICENSE NOT FOUND!!!              #\n");
			fprintf(stderr, "#                                                #\n");
			fprintf(stderr, "##################################################\n");             		
			exit(1);
	 	}
	 }


/*
printf("check_license DEBUG: ProgramID = %s\n",ProgramID);
*/

/*
printf("check_license DEBUG: ENELinfo = %s\n",Buffer); 
*/
   if (Buffer[0] == '\0') {
	fprintf(stderr, "##################################################\n");
	fprintf(stderr, "#                                                #\n");
	fprintf(stderr, "#              LICENSE NOT FOUND!!!              #\n");
	fprintf(stderr, "#                                                #\n");
	fprintf(stderr, "##################################################\n");             		
        exit(1);
   }
   else {
	fprintf(stderr, "##################################################\n");
	fprintf(stderr, "#                                                #\n");
	fprintf(stderr, "#                 LICENSE OK!!!                  #\n");
	fprintf(stderr, "#                                                #\n");
	fprintf(stderr, "##################################################\n");
	exit(0);
   }  


}
