/**********************************************************************
*
*       C Source:               f22_func.c
*       Subsystem:              3
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Mon Jul  6 14:34:15 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: f22_func.c-11 %  (%full_filespec: f22_func.c-11:csrc:3 %)";
#endif
/*
   modulo f22_func.c
   tipo 
   release 5.7
   data 2/6/96
   reserved @(#)f22_func.c	5.7
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)f22_func.c	5.7\t2/6/96";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "sim_param.h"
#include "sim_types.h"
#include "f22_circ.h"
#include "grsf22.h"
#include <Rt/RtMemory.h>

/*
 *    macro per la lettura del file
 */

#define scrivi(_punt_,_size_,_item_,_fp_)   fwrite(_punt_,_size_,_item_,_fp_)
#define leggi(_punt_,_size_,_item_,_fp_)   fread(_punt_,_size_,_item_,_fp_)
#define fluscia(_fp_)     fflush(_fp_)
#define sposta(_fp_,_off_)  fseek(_fp_, _off_, SEEK_SET)
#define sposta_rel(_fp_,_off_,_rel_)  fseek(_fp_, _off_, _rel_)
#define sposta_rel_new(_fp_,_off_)  fseek(_fp_, _off_, SEEK_CUR)

/*  
   Prototyping delle funzioni contenute nel file 
*/
static int ComparaCampioni(SCIRC_SEL_DATI *,SCIRC_SEL_DATI *);

int zero=0;


/*
    Apro in r il file e assegno il puntatore
*/
int f22_open_file(char *nome, PUNT_FILE_F22 *punt)
{
char  *nome_file;

if(nome==NULL)
	return(0);

nome_file=(char*)malloc(strlen(nome)+15);
sprintf(nome_file,"%s.dat",nome);

punt->fp = (FILE *)NULL;
punt->fphd = (FILE *)NULL;

punt->fp = fopen(nome_file,"r");
free(nome_file);

if ( punt->fp != (FILE *)NULL) 
        {
	return(1);
        }
else
        {
	return(0);
        }
}



/*
    Apro in r il file e assegno il puntatore
*/
int f22_open_file_new(char *nome, PUNT_FILE_F22 *punt)
{
char  *nome_file;

if(nome==NULL)
        return(0);

nome_file=(char*)malloc(strlen(nome)+15);
sprintf(nome_file,"%s",nome);

punt->fp = (FILE *)NULL;
punt->fphd = (FILE *)NULL;

punt->fp = fopen(nome_file,"r");
free(nome_file);

if ( punt->fp != (FILE *)NULL)
        {
        return(1);
        }
else
        {
        return(0);
        }
}





/*
    Apro in r+ i files e assegno i puntatori
*/
int f22_open_plus_file(char *nome, PUNT_FILE_F22 *punt)
{
char *nome_file;

if(nome==NULL)
	return(0);

nome_file=(char*)malloc(strlen(nome)+15);
sprintf(nome_file,"%s.dat",nome);

punt->fp = (FILE *)NULL;
punt->fp = fopen(nome_file,"r+");
free(nome_file);
if( punt->fp != (FILE *)NULL) 
	return(1);
else
        {
	return(0);
        }
}




int f22_leggo_header(F22CIRC_HD *header, PUNT_FILE_F22 punt, 
                     int RileggiF22Par, SIMULATOR *simpar)
{
HEADER_REGISTRAZIONI hdreg;
int  offset=sizeof(HEADER_REGISTRAZIONI);
static int prima_volta=1;
/*
 Se e' settato a 1 il parametro  RileggiF22Par la funzione esegue
 una lettura dei parametri dimensionabili nel file Simulator
 e salva i parametri letti nella struttura simpar
 Altrimenti viene skippata la dimensione occupata da tali
 parametri.
*/
if ( RileggiF22Par==1)
   {
   if (simpar == NULL)
	return(0);
   fprintf(stderr,"RileggiF22Par=%d\n",RileggiF22Par);
   if(sposta(punt.fp,0)!=0)
	return(0);
   leggi(&hdreg,sizeof(HEADER_REGISTRAZIONI),1,punt.fp); 
   fprintf(stderr,"parametri f22: %d - %d - %d -%d - %d - %d - %d\n",
           hdreg.simulator.max_snap_shot,
           hdreg.simulator.max_back_track,
           hdreg.simulator.max_campioni,
           hdreg.simulator.num_var,
           hdreg.simulator.max_pertur,
           hdreg.simulator.spare_snap,
           hdreg.simulator.pert_clear);
   simpar->max_snap_shot  = hdreg.simulator.max_snap_shot;
   simpar->max_back_track = hdreg.simulator.max_back_track;
   simpar->max_campioni   = hdreg.simulator.max_campioni;
   simpar->num_var        = hdreg.simulator.num_var;
   simpar->max_pertur     = hdreg.simulator.max_pertur;
   simpar->spare_snap     = hdreg.simulator.spare_snap;
   simpar->pert_clear     = hdreg.simulator.pert_clear;
   }
else
   if(sposta(punt.fp,offset)!=0)
	return(0);
/*
 Lettura della parte header vera e propria
*/
if(leggi(header,sizeof(F22CIRC_HD),1,punt.fp)<=0) { 
/*
   printf("header: p_iniz=%d - p_fine=%d - num_campioni=%d num_var_graf=%d\n",
		header->p_iniz,header->p_fine,header->num_campioni,
		header->num_var_graf);
*/
   return(0);
}
else {
/*
   printf("header: p_iniz=%d - p_fine=%d - num_campioni=%d num_var_graf=%d\n",
                header->p_iniz,header->p_fine,header->num_campioni,
                header->num_var_graf);
*/
   return(1);
}
}

int f22_close_file(PUNT_FILE_F22 punt)
{
fclose(punt.fp);
return(1);
}

/***
	Questa funzione viene utilizzata per la lettura del file
	circolare dall'oggetto grafico XlGrafico.
	Non e' assolutamente ottimizzata.
	Per ottimizzarla si potrebbe leggere, per cercare l'ultimo
	valore > di tempo, leggere un campione ogni 20 ( 20 e' un esempio)
	per individuare la zona del file e quindi, individuata la zona 
	del file di interesse, leggere i campioni uno dopo l'altro.

***/
int f22_leggi_ultimo(PUNT_FILE_F22 punt,F22CIRC_HD header,
		float *tempo,float *dati)
{
F22CIRC_T t_22;
float t_save;
int campione;
int offset,offheader;
int size_campione = (header.num_var_graf ) *sizeof(float) + sizeof(F22CIRC_T);

/*
printf("f22_leggi_ultimo   t = %f inizio = %d fine = %d num_var = %d\n", 
		*tempo, header.p_iniz,header.p_fine,
		header.num_var_graf);
*/

if(header.p_fine == header.p_iniz)
	return(1);

offheader = sizeof(HEADER_REGISTRAZIONI) + sizeof(F22CIRC_HD) +
            sizeof(F22CIRC_VAR) * header.num_var_graf;
campione  = header.p_iniz;
offset    = offheader + (campione-1) * size_campione;
sposta(punt.fp,offset);
leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
if(t_22.tempo > *tempo)
	{
	leggi(dati,sizeof(float),header.num_var_graf,punt.fp);
	memcpy(tempo,&(t_22.tempo),sizeof(float));
	return(1);
	}

campione  =  header.p_fine;
offset    =  offheader + (campione-1) * size_campione;
sposta(punt.fp,offset);
leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
leggi(dati,sizeof(float),header.num_var_graf,punt.fp);
if(t_22.tempo <= *tempo)
		return(0);
t_save = t_22.tempo ;
while(1)
	{
	offset=offheader + (campione-1) * size_campione;
	sposta(punt.fp,offset);
	leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
	if(t_22.tempo <= *tempo)
		{
		memcpy(tempo,&t_save,sizeof(float));
		return(1);
		}
	t_save = t_22.tempo;
	leggi(dati,sizeof(float),header.num_var_graf,punt.fp);
        /*
	 * calcolo campione successivo
         */
	if(campione == header.p_iniz)
		return(0);
	-- campione;
	if(campione == 0)
		{
		if(header.p_iniz < header.p_fine)
			return(0);
		campione = header.num_campioni;
		}
	}
}

/*
 Legge i dati in f22 a partire dal tempo notificato come parametro in
 avanti. 
 (Se viene restituito un dato solo con tempo = -1 significa che non 
 sono stati acquisiti nuovi dati posteriori al tempo richiesto.)
 Viene restituito il umero di campioni letti.
*/
int f22_leggi_campioni(PUNT_FILE_F22 punt,  /* struttura contenente i 
					       puntatori a file f22 e
					       al suo header */
		F22CIRC_HD header,	   /* struttura header del file f22 */ 
		float *tempo,              /* tempo a partire dal quale si 
                                              desidera acquisire nuovi campioni 						*/
		SCIRC_SEL_DATI **dati,     /* buffer dati che viene allocato
                                              internamente a questa funzione
                                              e che restituisce i dati letti
                                            */
		int p1, int p2, int p3, int p4)  /* indici delle misure 
						    che si desidera acquisire */
{
int campione;
int size_campione = (header.num_var_graf ) *sizeof(float) + sizeof(F22CIRC_T);
int offheader     = sizeof(HEADER_REGISTRAZIONI) + sizeof(F22CIRC_HD) +
                    sizeof(F22CIRC_VAR) * header.num_var_graf;
int offset;
int camp;
static F22CIRC_HD header_save;
static int p1_save = -1;
static int p2_save = -1;
static int p3_save = -1;
static int p4_save = -1;
float tempo_prec=-1;
float *valori;
F22CIRC_T t_22;

valori = (float*)malloc(sizeof(float)*header.num_var_graf);
/*
printf("f22_leggi_campioni t > %f sizeof(F22CIRC_T) = %d   %d-%d-%d-%d\n", 
		*tempo,sizeof(F22CIRC_T),
		p1,p2,p3,p4);
*/

/*
	annullo i puntatori
*/
*dati = NULL;
camp = 0; /* contatore dei campioni letti */
/*
 Se l'header non e' variato significa che il tempo e' rimasto invariato
 rispetto alla acquisizione precedente: la routine esce riportando un
 valore con tempo pari a -1 (dati non acquisiti)
*/
if((memcmp(&header,&header_save,sizeof(F22CIRC_HD))==0) && 
    (p1 == p1_save) && (p2 == p2_save) &&(p3 == p3_save) &&(p4 == p4_save))
	{
	memcpy(&header_save,&header,sizeof(F22CIRC_HD));
	p1_save = p1;
	p2_save = p2;
	p3_save = p3;
	p4_save = p4;
	/*
		riempie l'ultimo campione  (tappo)
	*/
	++camp;
	*dati = (SCIRC_SEL_DATI *) 
		realloc(*dati, sizeof(SCIRC_SEL_DATI) * camp);
	(*dati)[camp -1].tempo.tempo = -1;
        free(valori);
	return(0);
	}

/*
 punta alla fine del file
*/
campione=header.p_fine;

if(header.p_fine == header.p_iniz) {
        free(valori);
	return(0);
}

memcpy(&header_save,&header,sizeof(F22CIRC_HD));

while(1)
	{
/*
 SI sposta sul campione da leggere
*/
	offset=offheader+(campione-1) * size_campione;
	sposta(punt.fp,offset);
/*
 Legge il tempo
*/
	leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
/*
 Legge i valori
*/
	leggi(valori,sizeof(float),header.num_var_graf,punt.fp);
/*
printf("t_22.tempo=%f  campione=%d\n",t_22.tempo,campione);
*/

	if(t_22.tempo <= *tempo)
		break;
	/*
		sistema i valori nei vettori
	*/
	++camp;
	*dati = (SCIRC_SEL_DATI *) 
		realloc(*dati, sizeof(SCIRC_SEL_DATI) * camp);
	(*dati)[camp-1].tempo.tempo = t_22.tempo;
	if(p1>=0)
	 (*dati)[camp -1].mis[0] = valori[ p1];
	else
	 (*dati)[camp -1].mis[0]= 0;
	if(p2>=0)
	 (*dati)[camp -1].mis[1] = valori[ p2];
	else
	 (*dati)[camp -1].mis[1] = 0;
	if(p3>=0)
	 (*dati)[camp -1].mis[2] = valori[ p3];
	else
	 (*dati)[camp -1].mis[2] = 0;
	if(p4>=0)
	 (*dati)[camp -1].mis[3] = valori[ p4];
	else
	 (*dati)[camp -1].mis[3] = 0;

        /*
	 * si posiziona sul campione precedente
         */
	if(campione == header.p_iniz)
		break;
	-- campione;
	if(campione == 0)
		campione = header.num_campioni;
if( tempo_prec==t_22.tempo )
    printf("    tempo_prec==t_22.tempo=%f campione=%d\n",t_22.tempo,campione);
tempo_prec = t_22.tempo;

	}

free(valori);

if(camp>0)
	{
/*
 Riordina i campioni (che sono ordinati dal piu' recente al
 piu' vecchio in modo inverso (dal piu' vecchio al piu'
 recente
*/
	qsort(*dati,camp,sizeof(SCIRC_SEL_DATI),(__compar_fn_t)ComparaCampioni);
	*tempo = (*dati)[camp-1].tempo.tempo;
/*
	printf("Ultimo tempo leggo = %f\n",*tempo);
*/

	}

/*
	riempie l'ultimo campione  (tappo = tempo settato a -1)
*/
++camp;
*dati = (SCIRC_SEL_DATI *) realloc(*dati, sizeof(SCIRC_SEL_DATI) * camp);
(*dati)[camp -1].tempo.tempo = -1;
/* 
 ritorna il numero effettivo di campioni
 letti (escluso il tappo) */
return(camp-1);
}



/*
 Legge tutti i campioni presenti in f22circ e restituisce il numero di
 questi.
 (Se viene restituito un dato solo con tempo = -1 significa che non
  sono presenti campioni nel file.)
*/
int f22_leggi_tutti_camp(PUNT_FILE_F22 punt, F22CIRC_HD header,
                         SCIRC_SEL_NEW *dati,int *ind_var_sel)
{
int campione;
int size_campione = (header.num_var_graf ) *sizeof(float) + sizeof(F22CIRC_T);
int offheader     = sizeof(HEADER_REGISTRAZIONI) + sizeof(F22CIRC_HD) +
                    sizeof(F22CIRC_VAR) * header.num_var_graf;
int offset;
int camp,i;
float tempo_prec=-1;
float *valori;
F22CIRC_T t_22;


printf("f22_leggi_tutti_camp: *****************************\n");
printf("f22_leggi_tutti_camp: Indici variabili selez: ");
   for(i=0;i<NUMMAXVAR;i++)
      printf("%d  ",ind_var_sel[i]);
   printf("\n");

   valori = (float*)calloc(header.num_var_graf,sizeof(float));
   camp = 0; /* contatore dei campioni letti */
	
   if(header.p_iniz==header.p_fine) {
/*
   Non ci sono campioni nel file f22circ. Caricamento nella prima 
   posizione del buffer il tappo.
*/
      ++camp;
      dati[camp -1].tempo.tempo = -1;
      free(valori);

printf("f22_leggi_tutti_camp: Nessun campione letto!\n"); 

      return(0);
   }

/*
   Esistono campioni nel file f22circ. Posizionamento sul primo.
*/
   campione=header.p_iniz;
   offset=offheader+(campione-1) * size_campione;
   sposta(punt.fp,offset);

   while(1) {
/*
   Lettura del tempo e dei valori.
*/
      leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
      leggi(valori,sizeof(float),header.num_var_graf,punt.fp);

/*
   Caricamento dei valori nel buffer.
*/
      ++camp;
      dati[camp-1].tempo.tempo = t_22.tempo;
      for(i=0;i<NUMMAXVAR;i++) {
	 if(ind_var_sel[i]>=0)
	    dati[camp -1].mis[i] = valori[ind_var_sel[i]];
	 else
	    dati[camp -1].mis[i] = 0.0;
      }
      if(campione == header.p_fine)
         break;
      campione++;
      if(campione == header.num_campioni) {
         campione=1;
         sposta(punt.fp,offheader);
      }
         
      if( tempo_prec==t_22.tempo )
         printf("    tempo_prec==t_22.tempo=%f campione=%d\n",t_22.tempo,campione);
      tempo_prec = t_22.tempo;

   }

   free(valori);

/*
   Riempimento dell'ultimo campione  (tappo = tempo settato a -1)
*/
   ++camp;
   dati[camp -1].tempo.tempo = -1;

printf("f22_leggi_tutti_camp: *****************************\n");

return(camp-1);
}




/*
 Legge i dati in f22 a partire dal tempo notificato come parametro in
 avanti.
 (Se viene restituito un dato solo con tempo = -1 significa che non
 sono stati acquisiti nuovi dati posteriori al tempo richiesto.)
 Viene restituito il numero di campioni letti.
*/
int f22_leggi_new(PUNT_FILE_F22 punt, F22CIRC_HD header,
                  float *tempo, SCIRC_SEL_NEW **dati,int *ind_var_sel)
{
int campione;
int size_campione = (header.num_var_graf ) *sizeof(float) + sizeof(F22CIRC_T);
int offheader     = sizeof(HEADER_REGISTRAZIONI) + sizeof(F22CIRC_HD) +
                    sizeof(F22CIRC_VAR) * header.num_var_graf;
int offset;
int camp,i;
int ind_iniz,ind_fin;
Boolean old=True;
static F22CIRC_HD header_save;
static int ind_var_sel_save[NUMMAXVAR];
float tempo_prec=-1;
float *valori;
F22CIRC_T t_22;
SCIRC_SEL_NEW dato_temp;


printf("f22_leggi_new: *****************************\n");
printf("f22_leggi_new: Ultimo tempo letto = %f\n",*tempo);
printf("f22_leggi_new: Indici variabili selez: ");
for(i=0;i<NUMMAXVAR;i++)
   printf("%d  ",ind_var_sel[i]);
printf("\n");

valori = (float*)calloc(header.num_var_graf,sizeof(float));

/*
 Inizializzazioni
*/
*dati = NULL;
camp = 0; /* contatore dei campioni letti */
/*
 Se l'header non e' variato significa che il tempo e' rimasto invariato
 rispetto alla acquisizione precedente: la routine esce riportando un
 valore con tempo pari a -1 (dati non acquisiti)
*/
for(i=0;i<NUMMAXVAR;i++) {
   if(ind_var_sel[i]!=ind_var_sel_save[i])
      old=False;
}      

if((memcmp(&header,&header_save,sizeof(F22CIRC_HD))==0) &&
    (old==True))
        {
/*
	Se l'header corrente e' uguale a quello della lettura precedente
	e se le variabili selezionate non sono cambiate,non si esegue
	la lettura del file f22.
*/

printf("f22_leggi_new: Nessun campione letto!\n"); 

/*
   Nessun campione letto. Si carica nella struttura 'dati' solo il tappo.
*/
        ++camp;
        *dati = (SCIRC_SEL_NEW *)
                realloc(*dati, sizeof(SCIRC_SEL_NEW) * camp);
        (*dati)[camp -1].tempo.tempo = -1;
        free(valori);
        return(0);
        }


/*
 punta alla fine del file
*/
campione=header.p_fine;

if(header.p_fine == header.p_iniz)
        return(0);

memcpy(&header_save,&header,sizeof(F22CIRC_HD));
for(i=0;i<NUMMAXVAR;i++)
   ind_var_sel_save[i]=ind_var_sel[i];

/*
 SI sposta sul campione da leggere
*/
        offset=offheader+(campione-1) * size_campione;
        sposta(punt.fp,offset);
while(1)
        {
/*
 SI sposta sul campione da leggere
*/
        offset=offheader+(campione-1) * size_campione;
        sposta(punt.fp,offset);
/*
 Legge il tempo
*/
        leggi(&t_22,sizeof(F22CIRC_T),1,punt.fp);
/*
 Legge i valori
*/
        leggi(valori,sizeof(float),header.num_var_graf,punt.fp);
/*
printf("f22_leggi_new: t_22.tempo=%f  campione=%d\n",t_22.tempo,campione);
*/

        if(t_22.tempo <= *tempo)
                break;
        /*
                sistema i valori nei vettori
        */
        ++camp;
        *dati = (SCIRC_SEL_NEW *)
                realloc(*dati, sizeof(SCIRC_SEL_NEW) * camp);
        (*dati)[camp-1].tempo.tempo = t_22.tempo;
	for(i=0;i<NUMMAXVAR;i++) {
	   if(ind_var_sel[i]>=0)
	      (*dati)[camp -1].mis[i] = valori[ind_var_sel[i]];
	   else
	      (*dati)[camp -1].mis[i] = 0.0;
	}
        /*
         * si posiziona sul campione precedente
         */
        if(campione == header.p_iniz)
                break;
        -- campione;
        if(campione == 0)
                campione = header.num_campioni;
           
        if( tempo_prec==t_22.tempo )
           printf("    tempo_prec==t_22.tempo=%f campione=%d\n",t_22.tempo,campione);
        tempo_prec = t_22.tempo;

        }

free(valori);

if(camp>0)
        {
/*
 Riordina i campioni (che sono ordinati dal piu' recente al
 piu' vecchio in modo inverso (dal piu' vecchio al piu'
 recente
*/
        ind_iniz=0;
        ind_fin=camp-1;
        while(ind_iniz<ind_fin) {
           memcpy(&dato_temp,&(*dati)[ind_iniz],sizeof(SCIRC_SEL_NEW));
           memcpy(&(*dati)[ind_iniz],&(*dati)[ind_fin],sizeof(SCIRC_SEL_NEW));
           memcpy(&(*dati)[ind_fin],&dato_temp,sizeof(SCIRC_SEL_NEW));
           ind_iniz++;
           ind_fin--;
        }
        *tempo = (*dati)[camp-1].tempo.tempo;

printf("Primo tempo letto = %f\n",(*dati)[0].tempo.tempo);
printf("Ultimo tempo letto = %f\n",*tempo);

        }

/*
        riempie l'ultimo campione  (tappo = tempo settato a -1)
*/
++camp;
*dati = (SCIRC_SEL_NEW *) realloc(*dati, sizeof(SCIRC_SEL_NEW) * camp);
(*dati)[camp -1].tempo.tempo = -1;

printf("f22_leggi_new: *****************************\n");

/*
 ritorna il numero effettivo di campioni
 letti (escluso il tappo) */


return(camp-1);
}










static int ComparaCampioni(SCIRC_SEL_DATI *p1,SCIRC_SEL_DATI *p2)
{

if(p1->tempo.tempo > p2->tempo.tempo)
	return(1);

if(p1->tempo.tempo < p2->tempo.tempo)
	return(-1);

return(0);
}

int f22_stampa_campioni(SCIRC_SEL_DATI *dati)
{
int i;

i = 0;
while(1)
	{
	if(dati[i].tempo.tempo<0)
		{
		-- i;
		if(i>0)
		printf("t = %f  val %f %f %f %f\n",
		dati[i].tempo.tempo, dati[i].mis[0],dati[i].mis[1],
		   dati[i].mis[2],dati[i].mis[3]);
		return(1);
		}
	i++;
	}
}


int f22_leggo_nomi_var(PUNT_FILE_F22 file_f22, F22CIRC_VAR **var, int *num)
{
static F22CIRC_HD header;
int i;

if(f22_leggo_header(&header, file_f22, zero, NULL)==0)
	return(-1);

*var=(F22CIRC_VAR *)malloc(header.num_var_graf *sizeof(F22CIRC_VAR));
   
*num = header.num_var_graf;

sposta(file_f22.fp,sizeof(HEADER_REGISTRAZIONI)+sizeof(F22CIRC_HD));
if(leggi(*var,sizeof(F22CIRC_VAR),header.num_var_graf,file_f22.fp)<=0)
	return(0);

/*
 *  for(i=0;i<header.num_var_graf;i++)
 *      printf("%d) [%s]-[%s]\n",i,(*var)[i].nomevar,(*var)[i].descvar);
 */

return(1);
}



float f22_last_t(char *nome_file_f22)
{
PUNT_FILE_F22 file_f22;
static float tempo = -1;
F22CIRC_HD header;
SCIRC_SEL_DATI *dati_f22 = NULL;

if(f22_open_file(nome_file_f22, &file_f22)==0)
        return(-1.0);
if(f22_leggo_header(&header,file_f22, zero, NULL)==0)
        return(-1.0);
f22_leggi_campioni(file_f22,header, &tempo,&dati_f22,-1,-1,-1,-1);
free(dati_f22);

return(tempo);
}


/*
    Apre dei nuovi file f22 circolari chiamandoli
    con estensione aggiuntiva "_new"
*/
int f22_create_new_file(char *nome, PUNT_FILE_F22 *punt)
{
char *nome_file_hd, *nome_file;

if(nome==NULL)
	return(0);

nome_file=(char*)malloc(strlen(nome)+15);
nome_file_hd=(char*)malloc(strlen(nome)+15);
sprintf(nome_file,"%s.dat",nome);
sprintf(nome_file_hd,"%s.hd",nome);

/*
printf("   Nuovo file header [%s]\n   Nuovo file data [%s]\n",
        nome_file_hd,nome_file);
*/

#if defined VMS
punt->fp = 0;
punt->fphd = 0;
/* Creato con gli stessi parametri di net_pf22 */
punt->fp = open(nome_file, O_RDWR | O_NDELAY | O_APPEND | O_CREAT, 0,
                "ctx=rec", "rfm=var", "shr=get", "shr=put");
punt->fphd = open(nome_file_hd, O_RDWR | O_NDELAY | O_APPEND | O_CREAT, 0,
                "ctx=rec", "rfm=var", "shr=get", "shr=put");

free(nome_file);
free(nome_file_hd);
if((punt->fp !=0)&&(punt->fphd != 0))
	return(1);
else
	return(0);
#else
punt->fp = (FILE *)NULL;
punt->fphd = (FILE *)NULL;
punt->fp = fopen(nome_file,"w");
punt->fphd = fopen(nome_file_hd,"w");
free(nome_file);
free(nome_file_hd);
if(( punt->fp != (FILE *)NULL) && ( punt->fphd != (FILE *)NULL))
	return(1);
else
	return(0);
#endif
}




/*
    Apertura del file "f22circ.dat". Questa funzione e' utilizzata in
    sostituzione della f22_create_new_file() nel caso in cui non e' previsto
    il file di header "f22circ.hd". 
*/
int f22_create_file_dat(char *nome, PUNT_FILE_F22 *punt)
{
char *nome_file;

if(nome==NULL)
	return(0);

nome_file=(char*)malloc(strlen(nome)+15);
sprintf(nome_file,"%s.dat",nome);

/*
printf("   Nuovo file data [%s]\n", nome_file);
*/
punt->fp = (FILE *)NULL;
punt->fphd = (FILE *)NULL;
punt->fp = fopen(nome_file,"w");
free(nome_file);
if(punt->fp != (FILE *)NULL)
	return(1);
else
	return(0);
}
