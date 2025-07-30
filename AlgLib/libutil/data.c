/**********************************************************************
*
*       C Source:               data.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed May 16 18:07:11 2001 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: data.c-2 %  (%full_filespec: data.c-2:csrc:2 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)data.c	5.1\t11/10/95";
/*
   modulo data.c
   tipo 
   release 5.1
   data 11/10/95
   reserved @(#)data.c	5.1
*/
#include <stdio.h>
#include <time.h>

/*

	restituisce la data corrente nel in tre interi nel formato
	
		gg mm aa

*/

int     data (giorno, mese, anno)
int    *giorno;
int    *mese;
int    *anno;
{
struct tm  *strutt_tempo;
int     tempo_time;
// GUAG2025 init della data
    *giorno=*mese=1;
    *anno=1970;
    time ((time_t *)&tempo_time);

    strutt_tempo = localtime ((time_t *)&tempo_time);
    if (strutt_tempo == NULL)
	return (-1);

    *giorno = strutt_tempo -> tm_mday;
    *mese = strutt_tempo -> tm_mon + 1;
    *anno = strutt_tempo -> tm_year+1900; // l'anno è a partire dal 1900 !!
    return (1);
}

/* N.B. consapevolmente questa funzione fornira' un risultato
        errato tra cent'anni...
*/
int     data2 (giorno, mese, anno)
int    *giorno;
int    *mese;
int    *anno;
{
struct tm  *strutt_tempo;
int     tempo_time;

    time ((time_t *)&tempo_time);

    strutt_tempo = localtime ((time_t *)&tempo_time);
    if (strutt_tempo == NULL)
	return (-1);

    *giorno = strutt_tempo -> tm_mday;
    *mese = strutt_tempo -> tm_mon + 1;
    if( strutt_tempo -> tm_year < 100 )
	    *anno = strutt_tempo -> tm_year;
    else
	    *anno = strutt_tempo -> tm_year - 100;

    return (1);
}
