#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>
#include <errno.h>
#include "sim_param.h"


int manageLicense(Buffer, program, demoFlag, licenseTime)
char   *Buffer;
char   *program;
int    *demoFlag;
double *licenseTime;
{
char   *pstr;
char   appTime[10];
char   licenseType[10];


   if (Buffer[0] == '\0') {
      fprintf(stderr, "##################################################\n");
      fprintf(stderr, "        %s: LICENSE NOT FOUND!!!\n",program);
      fprintf(stderr, "       MA VA BENE LO STESSO\n");
      fprintf(stderr, "#################################################\n");
     //       GUAG 2024
     //       return ( -1 ) ;
      return ( 0 ) ;
   }
/*  
printf("manageLicense DEBUG: Buffer = %s|\n", Buffer);
*/
/*
   Si recupera la tipologia della licenza: 'full' o 'demo' e il tempo
   limite di durata della licenza.
*/
   if ((pstr = strtok(Buffer, "-" ))!= NULL) {
      strcpy(licenseType,pstr);
      while ((pstr = strtok((char *)NULL, "-"))!= NULL) {
         strcpy(appTime,pstr);
      }
   }
   else {
      fprintf(stderr, " Error!!! License string: separator '-' not found\n");
      return(-1);
   }
   if((strcmp(licenseType, LICENSE_DEMO) != 0) &&
      (strcmp(licenseType, LICENSE_FULL) != 0)) {
      fprintf(stderr, " Error!!! License type specified\n");
      return(-1);
   }
/*
printf("manageLicense DEBUG: appTime = %s\n",appTime);
*/
   *licenseTime = atof(appTime);
   if(*licenseTime == 0.0) {
      fprintf(stderr, " Error!!! license time specified!!!\n");
      return(-1);
   }
   if(strcmp(licenseType, LICENSE_DEMO) == 0)
      *demoFlag = 1;

   fprintf(stderr, "##################################################\n");
   fprintf(stderr, "     %s: LICENSE      --->  OK\n",program);
   fprintf(stderr, "     %s: LICENSE TYPE --->  %s\n",program,licenseType);

   if(*demoFlag == 1)
      fprintf(stderr, "     %s: DEMO TIME    --->  %lf (min)\n",program,(*licenseTime/60.0));

   fprintf(stderr, "##################################################\n");
   return(0);

}



int manageLicenseNoTime(Buffer, program, demoFlag)
char   *Buffer;
char   *program;
int    *demoFlag;
{
char   licenseType[10];

/*
   Si controlla se la licenza esiste.
*/
   if (Buffer[0] == '\0') {
      fprintf(stderr, "##################################################\n");
      fprintf(stderr, "        %s: LICENSE NOT FOUND!!!\n",program);
      fprintf(stderr, "       MA VA BENE LO STESSO\n");
      fprintf(stderr, "#################################################\n");
	         //       GUAG 2024
		 //            //       return ( -1 ) ;
      return ( 0 ) ;
   }
/*  
printf("manageLicenseNoTime DEBUG: Buffer = %s\n",Buffer);
printf("manageLicenseNoTime DEBUG: LunBuffer = %d\n", strlen(Buffer));
printf("manageLicenseNoTime DEBUG: LunDefine = %d\n", strlen(LICENSE_FULL));
*/

/*
   Si controlla la tipologia della licenza: 'full' o 'demo'.
*/
   if((strcmp(Buffer,LICENSE_FULL)!=0) && (strcmp(Buffer,LICENSE_DEMO)!=0)) {
      fprintf(stderr, " Error!!! License type specified\n");
      return(-1);
   }
   else {
      if(strcmp(Buffer,LICENSE_DEMO) == 0) {
         strcpy(licenseType,LICENSE_DEMO);
         *demoFlag = 1; 
      }
      else {
         strcpy(licenseType,LICENSE_FULL);
      }
   }
   fprintf(stderr, "##################################################\n");
   fprintf(stderr, "     %s: LICENSE      --->  OK\n",program);
   fprintf(stderr, "     %s: LICENSE TYPE --->  %s\n",program,licenseType);
   fprintf(stderr, "##################################################\n");
   return(0);

}



int getTimeLicense(tempo)
double *tempo;
{
struct timeval tp;
struct timezone tzp;
int    ret;

   ret = gettimeofday(&tp,&tzp);
   if(ret == -1) {
      printf("getTimeLicense - errore gettimeofday n. %d\n",errno);
      return(ret);
   }
   else {
      *tempo = (double)tp.tv_sec;
/*
printf("getTimeLicense DEBUG: tempo = %lf\ttp.tv_sec = %d\n",
       *tempo, tp.tv_sec);
*/
   }

   return(0);

}



int setTimeLicense(flag_demo, tempo_license)
int    flag_demo;
double *tempo_license;
{
double tempo_corr;
int    ret;


   if(flag_demo == 0) {
/*
   Licenza di tipo 'full'
*/
      *tempo_license = *tempo_license * LICENSE_TIME_FACT;
   }
   else {
/*
   Licenza di tipo 'demo'
*/
      ret = getTimeLicense(&tempo_corr);
      if(ret == 0)
         *tempo_license = *tempo_license + tempo_corr;
      else
         return(-1);
   }

   return(0);

}
