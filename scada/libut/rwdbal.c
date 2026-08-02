/**********************************************************************
*
*       C Source:               rwdbal.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Jul  1 10:37:16 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: rwdbal.c-3 %  (%full_filespec: rwdbal.c-3:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
/*-> rwdbal

        routine per la lettura e scrittura del data base 
        di sistema allocato in modo variabile
		  (utilizzato cosi' dai configuratori)
		  vengono passate due strutture:
			  db	contiene le dimensioni del data base
			  ut	contiene gli elementi del data base da leggere
				  	   o scrivere
			  il data base deve essere gia' stato allocato con la
			  funzione allocdb
				
			  flag =0 lettura data base
				    =1 scrittura data base
*/

#include <osf1.h>
#include <stdio.h>
#include <string.h>

#include "ansiscr.h"
#include "g0strdb.inc"
#include "g2ptrdb.inc"
#include "dconf.inc"
#include <stdlib.h>

extern void rwgerarc(short, DB_HEADER *, DB_HEADER *);
void iodb(FILE *, short, char *, int, int);

void rwdbal (flag,db,ut)
DB_HEADER *db, *ut;
short flag ;
{
char fnome[FILENAME_MAX+1];
FILE *fp ;
short lung ;
unsigned long int offset;
		
strcpy(fnome,&conf[RTF][0]) ;          

/*
Se sono in lettura apro fdbdp.rtf altrimenti 
faccio il dump del db su fdbdp.rtf.dump
*/
if ( flag ) {
	printf(" SCRITTURA DATA BASE SU DISCO \n");
        strcat(fnome,FDBDP) ;
        strcat(fnome,".DUMP") ;
        fp=fopen(fnome,"wb+") ;
   }
else {
	printf(" LETTURA DATA BASE\n ");
        strcat(fnome,FDBDP) ;
        fp=fopen(fnome,"rb+") ;
   }

if(!fp){perror(" fdbdp "); exit(0); }

offset=ldb_header;
if(ut->dimaa)
{
 	fseek(fp,offset,0);
   iodb(fp,flag,(char *)dbaav,sizeof(S_DBAAV),db->dimaa);        
  	iodb(fp,flag,(char *)dbaaf,sizeof(S_DBAAF),db->dimaa);        
   iodb(fp,flag,(char *)dbaau,sizeof(S_DBAAU),db->dimaa);        
   iodb(fp,flag,(char *)dbaaal,sizeof(S_DBALL),db->dimaa);
 	iodb(fp,flag,(char *)dbaafa,sizeof(S_DBAAFA),db->dimaa);
}
offset=offset+((long)t_dbaa*(long)db->dimaa);
if(ut->dimas)
{
	fseek(fp,offset,0);
   iodb(fp,flag,(char *)dbasv,sizeof(S_DBASV),db->dimas);        
   iodb(fp,flag,(char *)dbasf,sizeof(S_DBASF),db->dimas);        
   iodb(fp,flag,(char *)dbasu,sizeof(S_DBASU),db->dimas);        
   iodb(fp,flag,(char *)dbasal,sizeof(S_DBALL),db->dimas);
   iodb(fp,flag,(char *)dbasde,sizeof(DES),db->dimas);
}
offset=offset+(long)t_dbas*(long)db->dimas;
if(ut->dimac)
{
	fseek(fp,offset,0);
   iodb(fp,flag,(char *)dbacv,sizeof(S_DBACV),db->dimac);        
   iodb(fp,flag,(char *)dbacf,sizeof(S_DBACF),db->dimac);        
   iodb(fp,flag,(char *)dbacu,sizeof(S_DBACU),db->dimac);        
   iodb(fp,flag,(char *)dbacal,sizeof(S_DBALL),db->dimac);
}
offset=offset+(long)t_dbac*(long)db->dimac;
if(ut->dimad)
{
	fseek(fp,offset,0);
   iodb(fp,flag,(char *)dbadv,sizeof(S_DBADV),db->dimad);        
   iodb(fp,flag,(char *)dbadf,sizeof(S_DBADF),db->dimad);        
   iodb(fp,flag,(char *)dbadu,sizeof(S_DBADU),db->dimad);        
   iodb(fp,flag,(char *)dbadal,sizeof(S_DBALL),db->dimad);
}
offset=offset+(long)t_dbad*(long)db->dimad;
if(ut->dimao)
{
	fseek(fp,offset,0);
   iodb(fp,flag,(char *)dbaof,sizeof(S_DBAOF),db->dimao);        
   iodb(fp,flag,(char *)dbaou,sizeof(S_DBAOU),db->dimao);        
   iodb(fp,flag,(char *)dbaom,sizeof(S_DBAOM),db->dimao);
}
offset=offset+(long)t_dbao*(long)db->dimao;
if(ut->dimda)
{
	fseek(fp,offset,0);
	iodb(fp,flag,(char *)dbda,sizeof(S_DBDA),db->dimda);
	iodb(fp,flag,(char *)dbdatr,sizeof(S_DBDATR),db->dimda);
   iodb(fp,flag,(char *)dbdau,sizeof(S_DBDAU),db->dimda); 
	iodb(fp,flag,(char *)dbdaco,sizeof(S_CALIN),db->dimda);
}
offset=offset+(long)t_dbda*(long)db->dimda;
if(ut->dimds)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbds,sizeof(S_DBDS),db->dimds);
 	iodb(fp,flag,(char *)dbdstr,sizeof(S_DBDSTR),db->dimds);
 	iodb(fp,flag,(char *)dbdsu,sizeof(S_DBDSU),db->dimds);
 	iodb(fp,flag,(char *)dbdsco,sizeof(S_CALIN),db->dimds);
   iodb(fp,flag,(char *)dbdsde,sizeof(S_DCALC),db->dimds);
}
offset=offset+(long)t_dbds*(long)db->dimds;
if(ut->dimdc)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbdc,sizeof(S_DBDC),db->dimdc);
 	iodb(fp,flag,(char *)dbdctr,sizeof(S_DBDCTR),db->dimdc);
 	iodb(fp,flag,(char *)dbdcu,sizeof(S_DBDCU),db->dimdc);
 	iodb(fp,flag,(char *)dbdcco,sizeof(S_CALIN),db->dimdc);
}
offset=offset+(long)t_dbdc*(long)db->dimdc;
if(ut->dimdd)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbdd,sizeof(S_DBDD),db->dimdd);
 	iodb(fp,flag,(char *)dbddtr,sizeof(S_DBDDTR),db->dimdd);
 	iodb(fp,flag,(char *)dbddu,sizeof(S_DBDDU),db->dimdd);
 	iodb(fp,flag,(char *)dbddco,sizeof(S_CALIN),db->dimdd);
}
offset=offset+(long)t_dbdd*(long)db->dimdd;
if(ut->dimdo)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbdo,sizeof(S_DBDO),db->dimdo);
 	iodb(fp,flag,(char *)dbdou,sizeof(S_DBDOU),db->dimdo);
 	iodb(fp,flag,(char *)dbdom,sizeof(S_DBDOM),db->dimdo);
}
offset=offset+(long)t_dbdo*(long)db->dimdo;
if(ut->dimor)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbosc,sizeof(S_DBOSC),db->dimor);
 	iodb(fp,flag,(char *)dbosa,sizeof(S_DBOSA),db->dimor);
 	iodb(fp,flag,(char *)dbosp,sizeof(S_DBOSP),db->dimor);
 	iodb(fp,flag,(char *)dbto,sizeof(S_DBTO),db->dimor);
 	iodb(fp,flag,(char *)dbou,sizeof(S_DBOU),db->dimor);
 	iodb(fp,flag,(char *)dboft,sizeof(S_DBOFT),db->dimor);
 	iodb(fp,flag,(char *)dbode,sizeof(S_DESORG),db->dimor);
}
offset=offset+(long)t_dbor*(long)db->dimor;
if(ut->dimst)
{
	fseek(fp,offset,0);
 	iodb(fp,flag,(char *)dbsttr,sizeof(S_DBSTTR),db->dimst);
 	iodb(fp,flag,(char *)dbstu,sizeof(S_DBSTU),db->dimst);
 	iodb(fp,flag,(char *)dbstv,sizeof(S_DBSTV),(db->dimst*g0lunst));
}
fclose(fp);
//scrxy(23,10); printf("                              ");
//clrscr();
//scrxy(0,0);

/* Legge le gerarchie allarmi */
rwgerarc(flag,db,ut);

return;
}
/*
	iodb legge o scrive la tabella passata
*/
/* Stile prototipo, NON K&R: il prototipo (riga 46) dichiara
   void iodb(FILE *, short, char *, int, int) mentre il K&R dichiarava
   "short n, size, flag" promossi a int -> i due lati non concordavano.
   size/n diventano int come da prototipo (i chiamanti passano sizeof(...) e
   dimensioni intere). Vedi docs/KR_PROTOTYPE_AUDIT.md. */
void iodb(FILE *fp, short flag, char *ptr, int size, int n)
{
	short i;
	for(i=0;i<n;i++)
		{
			if(flag) fwrite(ptr,size,1,fp);
			else		fread(ptr,size,1,fp);
			ptr=ptr+size;
		}
}
