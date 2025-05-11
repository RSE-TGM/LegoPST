/**********************************************************************
*
*       C Source:               OlPert.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Wed Oct 23 15:22:48 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlPert.c-3.1.2 %  (%full_filespec: OlPert.c-3.1.2:csrc:1 %)";
#endif
/*
   modulo OlPert.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)OlPert.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlPert.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  OlPert.c - oggetto per la gestione delle perturbazioni degli
 *             oggetti figli di XlCore e XlManager
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Ol/OlPertP.h>
#include <Xl/XlP.h>

/* dichiarazioni funzioni varie */
/* dichiarazione di procedure varie */
static int separa_str();

/* typedef per separa_str() */
typedef struct {
 char *stringa;
 int lun_stringa;
 } STRIN_ST;

         
/* dichiarazione dei metodi (methods) */
static Boolean InitializePert();
static Boolean SendPert();



/* Inizializzazione del class record */
OlPertClassRec olPertClassRec = {
  { /* pert fields */
    /* initialize               */      InitializePert,
    /* sendpert                 */      SendPert,
  }
};

ObjectClass olPertObjectClass = (ObjectClass) &olPertClassRec;


static Boolean InitializePert(OlPertObject pert) 
{
	/*
		si aggancia o crea la coda di messaggi
		per l'invio delle perturbazioni
	*/
/*
Se il tipo di simulatore e' XL==DB_XLSIMUL mmi agganciato a shm sked,
la perturbazione va su ID_MSG_PERT. Se la modalita' e' client-server 
la perturbazione va su ID_MSG_PERT_MMI.
*/
	if(strcmp(pert->olpert.nome_database,"DB_XLSIMUL"))
		{
		pert->olpert.id_msg=
			msg_create(pert->olpert.shr_key+ID_MSG_PERT_MMI,0);
		}
	else
		pert->olpert.id_msg=
			msg_create(pert->olpert.shr_key+ID_MSG_PERT,0);
	if(pert->olpert.id_msg<=0)
		{
		XlWarning("OlPert","OlInitializePert",
			 "Impossibile agganciare la coda delle perturbazioni");
		return(False);
		}

	return(True);
}


/*
	Creazione pert
*/
OlPertObject OlCreatePert(char *nome_database, int shr_key)
{
OlPertRec *pert;

	pert=(OlPertRec*) XtMalloc(sizeof (OlPertRec));
	if(pert==NULL)
		{
		XlWarning("OlPert","OlCreatePert",
			 "Impossibile allocare oggetto pert");
		return(NULL);
		}
	/*
		allocazione e riempimento risorse 
	*/
	if(nome_database==NULL)
	        pert->olpert.nome_database=NULL;
	    else
		{
	        pert->olpert.nome_database=(char*)
			XtMalloc(strlen(nome_database)+1);
		strcpy(pert->olpert.nome_database,
			nome_database);
		}
	pert->olpert.shr_key=shr_key;
	/*
		inizializzazione dell'oggetto pert 
	*/
	if(
	 (olPertClassRec.olpert_class.initialize_pert)(pert))
		return(pert);
	else
		{
		XlWarning("OlPert","OlCreatePert",
			 "Pert non allocato");
		XtFree((char *)pert);
		return(NULL);
		}
}

/*
	Distruzione pert
*/
Boolean OlDestroyPert(OlPertObject pert )
{

if(pert==NULL)
	{
	XlWarning("OlPert","OlDestroyPert",
                         "Pert non allocato");
	return(False);
	}

/*
Elimino anche coda per PERTURBAZIONI
Se il tipo di simulatore e' XL==DB_XLSIMUL mmi agganciato a shm sked,
la perturbazione va su ID_MSG_PERT. Se la modalita' e' client-server
la perturbazione va su ID_MSG_PERT_MMI.
*/

        if(strcmp(pert->olpert.nome_database,"DB_XLSIMUL"))
                {
                if(msg_close(pert->olpert.id_msg)<0)
                    fprintf(stderr,"WARNING: Impossible to del queue with key=%X\n",pert->olpert.shr_key+ID_MSG_PERT_MMI);
                }
/******
        else
                {
                if(msg_close(pert->olpert.id_msg)<0)
                    fprintf(stderr,"WARNING: Impossible to del queue with key=%X\n",pert->olpert.shr_key+ID_MSG_PERT);
                }

*******/




if(pert->olpert.nome_database)
	XtFree((char *)pert->olpert.nome_database);

XtFree((char *)pert);

return(True);
}

static int separa_str(
 char riga[],
 int lun,
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;
for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, " \t");
  if(strin[i].stringa==(char*)NULL)
     {
     strin[i].stringa=(char*)malloc(3);
     sprintf(strin[i].stringa," ");
     strin[i].lun_stringa = strlen(strin[i].stringa);
     }
  else
     {
     strin[i].lun_stringa = ( s ? strlen( s) : 0 );
     }
  s = (char*)NULL; }
}

static Boolean SendPert(OlPertObject pert,PUNT_VAROUT punt)
{
/**
printf("Richiamto metodo SendPert nome_database=%s\n",pert->olpert.nome_database);
printf("shr_key = %d id_msg = %d\n",
	pert->olpert.shr_key,pert->olpert.id_msg);
printf("tipo_pert = %d  value = %f r/d = %f \n",punt.tipo_pert,punt.value,
			punt.rateodurata);
printf("pdb.nmod = %d pdb.indice = %d pdb.tipo = %d\n",
	punt.pdb.nmod,punt.pdb.indice,punt.pdb.tipo);
**/
pert_snd(pert->olpert.id_msg,punt.tipo_pert,punt.pdb.indice,punt.value,
		punt.rateodurata,0.,0.,0.,0.,0.,0.);
}
