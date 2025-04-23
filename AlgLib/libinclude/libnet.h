/**********************************************************************
*
*       C Header:               libnet.h
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Sat Nov 30 15:09:20 2002 %
*
**********************************************************************/
/*
   modulo libnet.h
   tipo 
   release 5.1
   data 11/16/95
   reserved @(#)libnet.h	5.1
*/

/*
 *******************************************************************
 *   Define  per settare lo standard di trasmissione in rete       *
 *   ULTRIX - AIX - VMS						   *
 *******************************************************************
*/

#define         SCO_AIX         1
#define         SCO_SCO         1
#define         SCO_ULTRIX      2
#define         SCO_OSF1        2
#define         SCO_LINUX       2
#define         SCO_VMS         3
#define         AIX_AIX         1
#define         AIX_SCO         1
#define         AIX_ULTRIX      2
#define         AIX_OSF1        2
#define         AIX_LINUX       2
#define         AIX_VMS         3
#define         ULTRIX_AIX      4
#define         ULTRIX_SCO      4
#define         ULTRIX_ULTRIX   5
#define         ULTRIX_OSF1     5
#define         ULTRIX_LINUX    5
#define         ULTRIX_VMS      6
#define         VMS_AIX         7
#define         VMS_SCO         7
#define         VMS_ULTRIX      8
#define         VMS_OSF1        8
#define         VMS_LINUX       8
#define         VMS_VMS         9
#define         OSF1_AIX        4
#define         OSF1_SCO        4
#define         OSF1_OSF1       5
#define         OSF1_LINUX      5
#define         OSF1_ULTRIX     5
#define         OSF1_VMS        6
#define         LINUX_AIX       4
#define         LINUX_SCO       4
#define         LINUX_OSF1      5
#define         LINUX_LINUX     5
#define         LINUX_ULTRIX    5
#define         LINUX_VMS       6
#define         M68000_VMS      10
#define         VMS_M68000      11
#define         M68000_ULTRIX   12
#define         M68000_OSF1     12
#define         M68000_LINUX    12
#define         ULTRIX_M68000   13
#define         M68000_M68000   14

#define 	TRASMISSIONE	0
#define		RICEZIONE	1


/* definisce la massima dimensione di un pacchetto da spedire in rete */
//#if defined UNIX
#define MAX_DIM_BUFFER_RETE	9999999
//#endif
//#if defined VMS
//#define MAX_DIM_BUFFER_RETE     5000
//#endif

/* conversioni di float fra DEC Station e Vax Station e viceversa */
void utov_float();
void vtou_float();

/* conversioni di float e int fra DEC Station e AIX e viceversa */
void atou_float();
void atou_int();
void atou_short();
void utoa_float();
void utoa_int();
void utoa_short();

void mtou_float();
void mtou_int();
void mtou_short();
void mtov_float();
void utom_float();
void utom_int();
void utom_short();
void vtom_float();

/* conversioni di float e int fra DEC Station e AIX e viceversa */
void atov_float();
void atov_int();
void atov_short();
void vtoa_float();
void vtoa_int();
void vtoa_short();

/* routin di conversione in funzione delle variabili di enviroment
        OS              ->      sistema operativo
        FORMATO_DATI    ->      formato dati desiderato         */
float *converti_float();
float *converti_float_f();
int *converti_int();
int *converti_int_f();
short *converti_short();
short *converti_short_f();
