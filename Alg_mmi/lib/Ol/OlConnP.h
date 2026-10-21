/**********************************************************************
*
*       C Header:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
/*
   modulo OlConnP.h
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)OlConnP.h	5.1
*/
/*
 * OlConnP.h - definizioni private per l'oggetto OlConn
 */
#ifndef _OlConnP_h
#define _OlConnP_h
/*
 Inclusione degli heder file privati delle classi superiori
	per il momento nessuna
*/

#include <Xm/XmP.h>
#include <Xl/XlP.h>

/*
 Inclusione dell'header file pubblico
*/
#include <Ol/OlConn.h>

typedef Boolean (*OlInitializeConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef Boolean (*OlReadConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef Boolean (*OlWriteConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef int (*OlNumConnsProc) (
#if NeedFunctionPrototypes
#endif
);

typedef Boolean (*OlInsConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef Boolean (*OlDelConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef int (*OlFindConnProc) (
#if NeedFunctionPrototypes
#endif
);

typedef int (*OlFindConnByPortProc) (
#if NeedFunctionPrototypes
#endif
);

/*
 Strutture relative alla nuova Classe
 */
typedef struct {
        OlInitializeConnProc initialize;    /* metodo di inizializzazione DB  */
	OlReadConnProc read;		/* metodo di lettura delle connessioni
					   dal file Connection.reg       */
	OlWriteConnProc write;          /* metodo di scrittura della connessioni
                                           nel file Connection.reg       */ 
	OlNumConnsProc get_num_conns;   /* restituisce il numero di connessioni
					   presenti */
	OlInsConnProc ins_conn;         /* inserisce una nuova connessione */
	OlDelConnProc del_conn;         /* elimina una  connessione */
	OlFindConnProc find_conn;       /* trova una  connessione restituendo
					   l'indice */
	OlFindConnByPortProc find_conn_by_port;  
					/* trova una  connessione  in 
				           funzione dell'indice della porta
					   dell'icona di regolazione e del
					   nome della pagina  */
        } OlConnClassPart;
/*
 Dichiarazione del record completo di classe
 */
typedef struct _OlConnClassRec {
         OlConnClassPart    olconn_class;
        } OlConnClassRec;

extern OlConnClassRec olConnClassRec;

/*
 Nuovi campi per il record del widget OlConn
 */

typedef struct {
        /* risorse  */
        char *path_name;      /* pathname del file di conns             */
	int num_conns;	      /* numero delle conns */
	CONNESSIONE *connessioni;
	FILE *fp;             /* file pointer del file delle connesioni */
	/*
	   com'era il file quando e' stato letto (o scritto l'ultima volta):
	   WriteConn non lo sovrascrive se nel frattempo e' cambiato su disco,
	   per esempio per mano di lgconn
	*/
	long letto_sec;
	long letto_nsec;
	long letto_size;
        } OlConnPart;
/*
 Dichiarazione completa del record per instanziazione
 */
typedef struct _OlConnRec {
        OlConnPart olconn;
        } OlConnRec;

#endif /* _OlConnP_h */
