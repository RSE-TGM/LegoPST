/**********************************************************************
*
*       C Source:               OlConn.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Wed Oct 23 15:26:48 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlConn.c-3.1.1 %  (%full_filespec: OlConn.c-3.1.1:csrc:1 %)";
#endif
/*
   modulo OlConn.c
   tipo 
   release 5.4
   data 2/16/96
   reserved @(#)OlConn.c	5.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlConn.c	5.4\t2/16/96";
/*
        Fine sezione per SCCS
*/
/*
 *  OlConn.c
 *  Oggetto per la memorizzazione delle connessioni fra i diversi schemi
 *  di regolazione.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Ol/OlConnP.h>
#include "utile.h"


/* dichiarazioni funzioni varie */
static Boolean CompareConnection(CONNESSIONE *, CONNESSIONE *);
static int separa_str();
static Boolean leggoRigaConn(FILE*, char *);

/* dichiarazioni di strutture ad uso interno  */
/* typedef per separa_str() */
typedef struct {
 char *stringa;
 int lun_stringa;
 } STRIN_ST;


/* dichiarazione dei metodi */
static Boolean InitializeConn();
static Boolean ReadConn();
static Boolean WriteConn();
static int GetNumConns();
static Boolean InsConnection();
static Boolean DelConnection();
static int FindConnection();
static int FindConnectionByPort();


/* Inizializzazione del class record */
OlConnClassRec olConnClassRec = {
  { /* conn fields */
    /* initialize               */      InitializeConn,
    /* read                     */      ReadConn,
    /* write                    */      WriteConn,
    /* get_num_conns            */      GetNumConns,
    /* ins_conn                 */      InsConnection,
    /* del_conn                 */      DelConnection,
    /* find_conn                */     FindConnection,
    /* find_conn_by_port        */     FindConnectionByPort,
  }
};


ObjectClass olConnObjectClass = (ObjectClass) &olConnClassRec;

extern char *OlEstrRegTaskName(char *, char *);


/****************************************************************************
*                                                                           *
*          Metodi dell'oggetto OlConn                                       *
*                                                                           *
*****************************************************************************/

/*
        Metodo InitializeConn  dell'oggetto conn
*/
static Boolean InitializeConn(OlConnObject conn)
{
        /*
	printf("OlConn: metodo InitializeConn\n");
        */

	/*
                annullo il campo del numero delle connessioni e il
                vettore delle connessioni
        */
        conn->olconn.num_conns = 0;
        conn->olconn.connessioni = NULL;


	/*
		leggo le connessioni
	*/
	if(!(olConnClassRec.olconn_class.read)(conn))
                return(False);


return(True);
}


/*
        Metodo ReadConn dell'oggetto conn
*/
static Boolean ReadConn(OlConnObject conn)
{
char riga[MAX_LUN_RIGA_CONN];
STRIN_ST strin [25];
CONNESSIONE  connessione;
int i;

        /*
	printf("OlConn: metodo ReadConn\n");
        */

	/*
		apro il file delle connessioni
	*/
	conn->olconn.fp = fopen(conn->olconn.path_name,"r");

	/*
		Il file non esiste e quindi viene creato vuoto
	*/
	/*if( (int)conn->olconn.fp == (int)NULL )*/
	if( conn->olconn.fp == NULL )
	    {
	    conn->olconn.fp = fopen(conn->olconn.path_name,"w");
	    /*if( (int)conn->olconn.fp == (int)NULL )*/
	    if( conn->olconn.fp == NULL )
		return(False);
	    }
	/*
		leggo le connessioni
	*/
	while(leggoRigaConn(conn->olconn.fp,riga))
		{
		separa_str(riga,22,strin);
/*
		for(i=0;i<22;i++)
			printf("campo[%d]=%s|\n",i,strin[i].stringa);
*/

                strcpy(connessione.u_nome_pagina,strin[1].stringa);
        	strcpy(connessione.u_nome_porta_int,strin[2].stringa);
        	strcpy(connessione.u_nome_icona_int,strin[3].stringa);
        	strcpy(connessione.u_nome_porta,strin[4].stringa);
        	strcpy(connessione.u_nome_icona,strin[5].stringa);
        	strcpy(connessione.u_nome_var,strin[6].stringa);
        	strcpy(connessione.u_nome_modulo,strin[7].stringa);
        	strcpy(connessione.u_nome_tag,strin[8].stringa);
        	strcpy(connessione.u_descrizione,strin[9].stringa);
        	strcpy(connessione.i_nome_pagina,strin[11].stringa);
        	strcpy(connessione.i_nome_porta_int,strin[12].stringa);
        	strcpy(connessione.i_nome_icona_int,strin[13].stringa);
        	strcpy(connessione.i_nome_porta,strin[14].stringa);
        	strcpy(connessione.i_nome_icona,strin[15].stringa);
        	strcpy(connessione.i_nome_var,strin[16].stringa);
        	strcpy(connessione.i_nome_modulo,strin[17].stringa);
        	strcpy(connessione.i_nome_tag,strin[18].stringa);
        	strcpy(connessione.i_descrizione,strin[19].stringa);
        	connessione.valore_variabile = atof(strin[21].stringa);

	        if(!(olConnClassRec.olconn_class.ins_conn)(conn,connessione))
			return(False);
		}
	

	/*
		Chiudo il file
	*/
	fclose(conn->olconn.fp);

return(True);
}


/*
        Metodo WriteConn dell'oggetto conn
*/
static Boolean WriteConn(OlConnObject conn)
{
int tot;
int i;

        /*
	printf("OlConn: metodo WriteConn\n");
        */

	/*
		apro il file in scrittura
	*/
	conn->olconn.fp = fopen(conn->olconn.path_name,"w");
	/*if( (int)conn->olconn.fp == (int)NULL )*/
	if( conn->olconn.fp == NULL )
		return(False);

	tot = conn->olconn.num_conns;

	for(i=0;i<tot;i++)
                {
		fprintf(conn->olconn.fp,
		"uscita|%s|%s|%s|%s|%s|%s|%s|%s|%s|ingresso|%s|%s|%s|%s|%s|%s|%s|%s|%s|valore|%f\n",
		conn->olconn.connessioni[i].u_nome_pagina,
                conn->olconn.connessioni[i].u_nome_porta_int,
                conn->olconn.connessioni[i].u_nome_icona_int,
                conn->olconn.connessioni[i].u_nome_porta,
                conn->olconn.connessioni[i].u_nome_icona,
                conn->olconn.connessioni[i].u_nome_var,
                conn->olconn.connessioni[i].u_nome_modulo,
                conn->olconn.connessioni[i].u_nome_tag,
                conn->olconn.connessioni[i].u_descrizione,
                conn->olconn.connessioni[i].i_nome_pagina,
                conn->olconn.connessioni[i].i_nome_porta_int,
                conn->olconn.connessioni[i].i_nome_icona_int,
                conn->olconn.connessioni[i].i_nome_porta,
                conn->olconn.connessioni[i].i_nome_icona,
                conn->olconn.connessioni[i].i_nome_var,
                conn->olconn.connessioni[i].i_nome_modulo,
                conn->olconn.connessioni[i].i_nome_tag,
                conn->olconn.connessioni[i].i_descrizione,
                conn->olconn.connessioni[i].valore_variabile);
                }

	/*
		Chiudo il file
	*/
	fclose(conn->olconn.fp);

return(True);
}


/*
        Metodo di GetNumConns dell'oggetto conn
*/
static int GetNumConns(OlConnObject conn)
{
return(conn->olconn.num_conns);
}


/*
        Metodo di InsConnection dell'oggetto conn
*/
static Boolean InsConnection(OlConnObject conn,CONNESSIONE connessione)
{
Boolean ret = False;

        /*
		verifico che i campi della connessione siano pieni
	*/
	if(connessione.u_nome_pagina == NULL) return(False);
	if(strlen(connessione.u_nome_pagina) == 0) return(False);

	if(connessione.u_nome_porta_int == NULL) return(False);
	if(strlen(connessione.u_nome_porta_int) == 0) return(False);

	if(connessione.u_nome_icona_int == NULL) return(False);
	if(strlen(connessione.u_nome_icona_int) == 0) return(False);

	if(connessione.u_nome_porta == NULL) return(False);
	if(strlen(connessione.u_nome_porta) == 0) return(False);

	if(connessione.u_nome_icona == NULL) return(False);
	if(strlen(connessione.u_nome_icona) == 0) return(False);

	if(connessione.u_nome_var == NULL) 
		sprintf(connessione.u_nome_var,"-");
	if(strlen(connessione.u_nome_var) == 0) 
		sprintf(connessione.u_nome_var,"-");

	if(connessione.u_nome_modulo == NULL) 
		sprintf(connessione.u_nome_modulo,"-");
	if(strlen(connessione.u_nome_modulo) == 0) 
		sprintf(connessione.u_nome_modulo,"-");

	if(connessione.u_nome_tag == NULL) 
		sprintf(connessione.u_nome_tag,"-");
	if(strlen(connessione.u_nome_tag) == 0) 
		sprintf(connessione.u_nome_tag,"-");

	if(connessione.u_descrizione == NULL) 
		sprintf(connessione.u_descrizione,"-");
	if(strlen(connessione.u_descrizione) == 0) 
		sprintf(connessione.u_descrizione,"-");

	if(connessione.i_nome_pagina == NULL) return(False);
	if(strlen(connessione.i_nome_pagina) == 0) return(False);

	if(connessione.i_nome_porta_int == NULL) return(False);
	if(strlen(connessione.i_nome_porta_int) == 0) return(False);

	if(connessione.i_nome_icona_int == NULL) return(False);
	if(strlen(connessione.i_nome_icona_int) == 0) return(False);

	if(connessione.i_nome_porta == NULL) return(False);
	if(strlen(connessione.i_nome_porta) == 0) return(False);

	if(connessione.i_nome_icona == NULL) return(False);
	if(strlen(connessione.i_nome_icona) == 0) return(False);

	if(connessione.i_nome_var == NULL) 
		sprintf(connessione.i_nome_var,"-");
	if(strlen(connessione.i_nome_var) == 0) 
		sprintf(connessione.i_nome_var,"-");

	if(connessione.i_nome_modulo == NULL) 
		sprintf(connessione.i_nome_modulo,"-");
	if(strlen(connessione.i_nome_modulo) == 0) 
		sprintf(connessione.i_nome_modulo,"-");

	if(connessione.i_nome_tag == NULL) 
		sprintf(connessione.i_nome_tag,"-");
	if(strlen(connessione.i_nome_tag) == 0) 
		sprintf(connessione.i_nome_tag,"-");

	if(connessione.i_descrizione == NULL) 
		sprintf(connessione.i_descrizione,"-");
	if(strlen(connessione.i_descrizione) == 0) 
		sprintf(connessione.i_descrizione,"-");


	/*
		alloco lo spazio per la nuova connessione
	*/

	if(conn->olconn.num_conns == 0)
		{
	        conn->olconn.connessioni = (CONNESSIONE*)XtMalloc(
		      sizeof(CONNESSIONE));
		}
	else
		{
	        conn->olconn.connessioni = (CONNESSIONE*)XtRealloc(
		      (char *) conn->olconn.connessioni,
		      (conn->olconn.num_conns+1)*sizeof(CONNESSIONE));
		}
		
	if(conn->olconn.connessioni != NULL)
		ret = True;
	else
		return(False);
	
        memcpy(&(conn->olconn.connessioni[conn->olconn.num_conns]), 
		&connessione,sizeof(CONNESSIONE));

        if(ret == True)
		++conn->olconn.num_conns;

return(True);
}

/*
	Metodo DelConnection dell'oggetto conn
*/
static Boolean DelConnection(OlConnObject conn, int pos)
{
int tot;
CONNESSIONE *connessioni;

	tot = conn->olconn.num_conns;

	if(tot == 0)
		return(False);

	if((pos<0)||(pos>tot))
		return(False);

	connessioni = (CONNESSIONE*) XtMalloc((tot-1)*sizeof(CONNESSIONE)); 

	if(pos>0)
		memcpy(&connessioni[0],&(conn->olconn.connessioni[0]),
			sizeof(CONNESSIONE)*pos);
        if(pos<tot)
		memcpy(&connessioni[pos],&(conn->olconn.connessioni[pos+1]),
                        sizeof(CONNESSIONE)*(tot-pos-1));

	XtFree((char *)conn->olconn.connessioni);
	
	-- conn->olconn.num_conns;
	
	conn->olconn.connessioni = (CONNESSIONE*)XtMalloc(
			sizeof(CONNESSIONE)*conn->olconn.num_conns);

	if(conn->olconn.connessioni == NULL)
		return(False);

	memcpy(conn->olconn.connessioni,connessioni,
		conn->olconn.num_conns*sizeof(CONNESSIONE));

	XtFree((char *)connessioni);

	return(True);
}

/*
        Metodo FindConnection dell'oggetto conn
*/
static int FindConnection(OlConnObject conn, CONNESSIONE connessione)
{
int ret = -1;
int i,tot;

	tot = (olConnClassRec.olconn_class.get_num_conns)(conn);


	for(i=0;i<tot;i++)
		if(CompareConnection(&connessione,
				&(conn->olconn.connessioni[i])))
			return(i);

        return(ret);
}

/*
        Metodo FindConnectionByPort dell'oggetto conn
*/
static int FindConnectionByPort(OlConnObject conn, 
				char *ind_port,
				char *ind_icon,
				char *nome_pag)
{
int i,tot;

/*
OlPrintConn(conn);

printf("Metodo FindConnectionByPort <%s> <%s> <%s>\n",	
		ind_port,ind_icon,nome_pag);
*/

	tot = (olConnClassRec.olconn_class.get_num_conns)(conn);

	for(i=0;i<tot;i++)
        {
	  if(
	   ((strcmp(ind_port,conn->olconn.connessioni[i].u_nome_porta_int)==0)&&
	   (strcmp(ind_icon,conn->olconn.connessioni[i].u_nome_icona_int)==0)&&
	   (strcmp(nome_pag,conn->olconn.connessioni[i].u_nome_pagina)==0))||
	   ((strcmp(ind_port,conn->olconn.connessioni[i].i_nome_porta_int)==0)&&
	   (strcmp(ind_icon,conn->olconn.connessioni[i].i_nome_icona_int)==0)&&
	   (strcmp(nome_pag,conn->olconn.connessioni[i].i_nome_pagina)==0))
	  )
		{
		printf("  ----> %d\n",i);
		return(i);
		}
        }
		
	printf("  ----> non trovata: porta=%s | icona=%s | pagina=%s |\n",
		ind_port, ind_icon, nome_pag);

        return(-1);
}


/****************************************************************************
*                                                                           *
*                          Funzioni private                                 *
*                                                                           *
*****************************************************************************/

static Boolean CompareConnection(CONNESSIONE *c1, CONNESSIONE *c2)
{
	if(c1->spare == c2->spare)
		return(True);
	else
		return(False);
}

static int separa_str(
 char riga[],
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;
for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, "|\n");
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

static Boolean leggoRigaConn(FILE* fp, char *riga)
{
char c;
int k;
char *ret;

ret=fgets (riga, MAX_LUN_RIGA_CONN, fp);

if(ret == NULL)
        return(False);
else
        return(True);
}

/****************************************************************************
*                                                                           *
*       Funzioni richiamabili dall'esterno per la gestione                  *
*       dell'oggetto OlConn                                                 *
*                                                                           *
*****************************************************************************/

OlConnObject OlCreateConn(char *path)
{
OlConnObject newconn=NULL;

        /*
	printf("OlCreateConn => %s\n",path);
        */

	newconn=(OlConnRec*) XtMalloc (sizeof (OlConnRec));
	if(newconn==NULL)
		{
		XlWarning("OlConn","OlCreateConn",
        		"Impossibile allocare OlConn in memoria\n");
		return(NULL);
		}

	/*
		allocazione della risorsa path
	*/
	if(path == NULL)
		return(NULL);

	newconn->olconn.path_name=(char*)XtMalloc(strlen(path)+1);
	strcpy(newconn->olconn.path_name,path);

	/*
		Alloco, leggendo da file l'elenco delle connessioni
	*/
	if(!(olConnClassRec.olconn_class.initialize)(newconn))
                return(NULL);

return(newconn);
}

void OlDestroyConn(OlConnObject conn)
{
	if(conn == NULL)
		return;


	/*
		libero la struttura dell'oggetto
	*/
	if(conn->olconn.path_name != NULL)
		XtFree((char *)conn->olconn.path_name);

	if(conn->olconn.connessioni != NULL)
		XtFree((char *)conn->olconn.connessioni);

	/* 
		libero l'oggetto
	*/
	XtFree((char *)conn);
}

Boolean OlInsNewConn(OlConnObject conn, CONNESSIONE connessione)
{
Boolean ret = False;

/*
printf("OlInsNewConn\nuscita|%s|%s|%s|%s|%s|%s|%s|%s|%s|ingresso|%s|%s|%s|%s|%s|%s|%s|%s|%s|valore|%f\n",
		connessione.u_nome_pagina, connessione.u_nome_porta_int,
                connessione.u_nome_icona_int, connessione.u_nome_porta,
                connessione.u_nome_icona, connessione.u_nome_var,
                connessione.u_nome_modulo, connessione.u_nome_tag,
                connessione.u_descrizione, connessione.i_nome_pagina,
                connessione.i_nome_porta_int, connessione.i_nome_icona_int,
                connessione.i_nome_porta, connessione.i_nome_icona,
                connessione.i_nome_var, connessione.i_nome_modulo,
                connessione.i_nome_tag, connessione.i_descrizione,
                connessione.valore_variabile);
*/
		
	ret = (olConnClassRec.olconn_class.ins_conn)(conn,connessione);

        if(!ret)
		fprintf(stderr,"Errore inserimento nuova connessione\n");

	return(ret);
}

int OlGetNumConn(OlConnObject conn)
{
	return((olConnClassRec.olconn_class.get_num_conns)(conn));
}

void OlPrintConn(OlConnObject conn)
{
int i,tot;

	tot = OlGetNumConn(conn);
	for(i=0;i<tot;i++)
		OlPrintSingleConn(conn,i);
}

void OlPrintSingleConn(OlConnObject conn,int i)
{
printf("[%d]\nuscita|%s|%s|%s|%s|%s|%s|%s|%s|%s|\ningresso|%s|%s|%s|%s|%s|%s|%s|%s|%s|\nvalore|%f\n",i,
		conn->olconn.connessioni[i].u_nome_pagina, 
	      	conn->olconn.connessioni[i].u_nome_porta_int,
                conn->olconn.connessioni[i].u_nome_icona_int, 
		conn->olconn.connessioni[i].u_nome_porta,
                conn->olconn.connessioni[i].u_nome_icona, 
		conn->olconn.connessioni[i].u_nome_var,
                conn->olconn.connessioni[i].u_nome_modulo, 
		conn->olconn.connessioni[i].u_nome_tag,
                conn->olconn.connessioni[i].u_descrizione, 
		conn->olconn.connessioni[i].i_nome_pagina,
                conn->olconn.connessioni[i].i_nome_porta_int, 
		conn->olconn.connessioni[i].i_nome_icona_int,
                conn->olconn.connessioni[i].i_nome_porta, 
		conn->olconn.connessioni[i].i_nome_icona,
                conn->olconn.connessioni[i].i_nome_var, 
		conn->olconn.connessioni[i].i_nome_modulo,
                conn->olconn.connessioni[i].i_nome_tag, 
		conn->olconn.connessioni[i].i_descrizione,
                conn->olconn.connessioni[i].valore_variabile);
}

Boolean OlDelConnection(OlConnObject conn, int pos)
{
	return((olConnClassRec.olconn_class.del_conn)(conn,pos));
}

int OlFindConnection(OlConnObject conn, CONNESSIONE connessione)
{
	return((olConnClassRec.olconn_class.find_conn)(conn, connessione));
}

/*
	Trova l'indice della connessione in funzione 
		dell'indice della porta, 
		dell'icona di regolazione e 
		del nome della pagina
*/
int OlFindConnectionByPort(OlConnObject conn, char *ind_port, char* ind_icon,
			   char *nome_pag)
{
	return((olConnClassRec.olconn_class.find_conn_by_port)
			(conn, ind_port,ind_icon,nome_pag));
}

Boolean OlSaveConn(OlConnObject conn)
{
	return((olConnClassRec.olconn_class.write)(conn));
}

void OlCreateStructConn(CONNESSIONE *connessione,
                char *u_nome_pagina,
                char *u_nome_porta_int,
                char *u_nome_icona_int,
                char *u_nome_porta,
                char *u_nome_icona,
                char *u_nome_var,
                char *u_nome_modulo,
                char *u_nome_tag,
                char *u_descrizione,
                char *i_nome_pagina,
                char *i_nome_porta_int,
                char *i_nome_icona_int,
                char *i_nome_porta,
                char *i_nome_icona,
                char *i_nome_var,
                char *i_nome_modulo,
                char *i_nome_tag,
                char *i_descrizione,
                float valore_variabile)
{

        strcpy(connessione->u_nome_pagina,u_nome_pagina);
        strcpy(connessione->u_nome_porta_int,u_nome_porta_int);
        strcpy(connessione->u_nome_icona_int,u_nome_icona_int);
        strcpy(connessione->u_nome_porta,u_nome_porta);
        strcpy(connessione->u_nome_icona,u_nome_icona);
        strcpy(connessione->u_nome_var,u_nome_var);
        strcpy(connessione->u_nome_modulo,u_nome_modulo);
        strcpy(connessione->u_nome_tag,u_nome_tag);
        strcpy(connessione->u_descrizione,u_descrizione);
        strcpy(connessione->i_nome_pagina,i_nome_pagina);
        strcpy(connessione->i_nome_porta_int,i_nome_porta_int);
        strcpy(connessione->i_nome_icona_int,i_nome_icona_int);
        strcpy(connessione->i_nome_porta,i_nome_porta);
        strcpy(connessione->i_nome_icona,i_nome_icona);
        strcpy(connessione->i_nome_var,i_nome_var);
        strcpy(connessione->i_nome_modulo,i_nome_modulo);
        strcpy(connessione->i_nome_tag,i_nome_tag);
        strcpy(connessione->i_descrizione,i_descrizione);
        connessione->valore_variabile = valore_variabile;

}


CONNESSIONE *OlGetConn(OlConnObject conn,int i)
{
CONNESSIONE *ret;


	if(i < 0)
		return(NULL);
	if(i > OlGetNumConn(conn))
		return(NULL);
		
	ret = &(conn->olconn.connessioni[i]); 
	
	return(ret);
}

char * OlFindConnectedTag(OlConnObject conn, char *tagIn)
{
int i,tot;
char tag_task[3];
char task_dir[FILENAME_MAX];
char dummy_char[10];
static char tagOut[9];
char *appo=NULL;



tot = (olConnClassRec.olconn_class.get_num_conns)(conn);

/*
    Prelevo i primi due caratteri del direttorio della task
    eventualmente filtrati per inserirli in coda al nome per
    tag out
*/
strcpy(task_dir,XlGetenv("LEGOMMI_PAG"));
appo = OlEstrRegTaskName(task_dir,dummy_char);
if( appo!=NULL )
    {
    strcpy(tag_task,appo);
    tomaius(tag_task);
    free(appo);
    }
else
    strcpy(tag_task,"");
    

for(i=0;i<tot;i++)
       	{
	sprintf(tagOut,"%s%s",conn->olconn.connessioni[i].u_nome_var,
		conn->olconn.connessioni[i].u_nome_tag);
	if(strcmp(tagOut,tagIn)==0)
		{
		sprintf(tagOut,"%s%s%s",conn->olconn.connessioni[i].i_nome_var,
				conn->olconn.connessioni[i].i_nome_tag,tag_task);
		return(tagOut);
		}
	sprintf(tagOut,"%s%s",conn->olconn.connessioni[i].i_nome_var,
		conn->olconn.connessioni[i].i_nome_tag);
	if(strcmp(tagOut,tagIn)==0)
		{
		sprintf(tagOut,"%s%s%s",conn->olconn.connessioni[i].u_nome_var,
			conn->olconn.connessioni[i].u_nome_tag,tag_task);
		return(tagOut);
		}
 	}	
return(NULL);
}

Boolean OlPagIsConnected(OlConnObject conn, char *nomePag)
{
int i,tot;
tot = (olConnClassRec.olconn_class.get_num_conns)(conn);
for(i=0;i<tot;i++)
	{
	if(strcmp(nomePag,conn->olconn.connessioni[i].u_nome_pagina)==0)
		return(True);
	if(strcmp(nomePag,conn->olconn.connessioni[i].i_nome_pagina)==0)
		return(True);
	}
return(False);
}
