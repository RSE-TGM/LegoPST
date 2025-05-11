/**********************************************************************
*
*       C Source:               OlCompiler.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Wed Oct 23 15:08:31 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: OlCompiler.c-16.1.3 %  (%full_filespec: OlCompiler.c-16.1.3:csrc:1 %)";
#endif
/*
   modulo OlCompiler.c
   tipo 
   release 5.2
   data 2/5/96
   reserved @(#)OlCompiler.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)OlCompiler.c	5.2\t2/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  OlCompiler.c - Compilatore per oggetti figli di XlCore e XlManager
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <Xl/XlResources.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlDispReg.h>
#include <Ol/OlCompilerP.h>


void XlErrComp(char* ,char *,char *,char *);

#include "libutilx.h"
#include "utile.h"

/*
	define per l'accesso al database; il nome dell'oggetto compilatore
	deve essere <compilatore>
*/
#define NUMMODELLI (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.nummodelli)\
			(compilatore->olcompiler.database)
#define NUMVARIABILI (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.numvariabili)\
			(compilatore->olcompiler.database)
#define TAGTOIND(nome,indirizzo,tipo) (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.tagtoind)\
			(compilatore->olcompiler.database,nome,indirizzo,tipo)
#define TAGTOINDS(nome,indirizzo,tipo) (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.tagtoinds)\
			(compilatore->olcompiler.database,nome,indirizzo,tipo)
#define PROGTOVAR(prog) (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.progtovar)\
			(compilatore->olcompiler.database,prog)

#define INDTOMOD(ind) (olDatabaseTopologiaClassRec.oldatabaseTopologia_class.indtomod)\
			(compilatore->olcompiler.database,ind)

/* lista delle risorse  */
static XlResources resources[]= {
        {
        XlNnome,
        XlCNome,
        XmRString,
        sizeof(char *),
        XtOffsetOf(OlCompilerRec,olcompiler.nome_compilatore),
        XmRString,
        "olcheck"
        },
	};

/* dichiarazioni funzioni varie */
/* dichiarazione di procedure varie */
static int separa_str();
static Boolean controlla_confinfo();
static int exec_dbmftc(char*,char*);


/* typedef per separa_str() */
typedef struct {
 char stringa[100];
 int lun_stringa;
 } STRIN_ST;

         
/* dichiarazione dei metodi (methods) */
static Boolean InitializeCompiler();
static Boolean CompileVar();
static Boolean CompileConfinfo();


/*
	Dichiarazioni di funzioni varie
*/
static void ConvertiVarReg();
static Widget VarRegIsConnect();
static void GetTagPorta();

/* Inizializzazione del class record */
OlCompilerClassRec olCompilerClassRec = {
  { /* compiler fields */
    /* initialize               */      InitializeCompiler,
    /* compilevar               */      CompileVar,
    /* compileconfinfo          */      CompileConfinfo,
  }
};

ObjectClass olCompilerObjectClass = (ObjectClass) &olCompilerClassRec;

extern char *OlEstrRegTaskName(char *, char *);


static Boolean CompileVar(OlCompilerObject compilatore,char *label,
	PUNT_DB * pdb, int tipo)
{
int ret= False;

if(compilatore==NULL)
	return(ret);
	/*
		Verifica se la label e' definita
	*/
if(label==NULL)
	return(ret);
	/*
		se il compilatore e' di tipo COMP_CHECK 
		termina la compilazione
	*/
if(compilatore->olcompiler.tipo_comp==COMP_CHECK)
	return(True);
/*printf("CompileVar:label=%s tipo=%d origin=%d\n",label,tipo,pdb->origin);*/
/*
Chiamo la funzione TAGTOIND per var SCADA  (pdb.origin==1)
e la TAGTOINDS per variabili di tipo SIMULATORE (pdb.origin==0)
*/
	if(pdb->origin==SIMULAID)
		return(TAGTOIND(label,&(pdb->indice),&(pdb->tipo)));
	else if(pdb->origin==SCADAID) 
		return(TAGTOINDS(label,&(pdb->indice),&(pdb->tipo)));
	else 
		return(False);

return(True);
}

static Boolean CompileConfinfo(OlCompilerObject compilatore,Widget w,
			XlConfInfo *confinfo, int num_confinfo,
			XtResourceList resources, int num_resources,
			char **risorsa_compiled)
{
PUNT_DB pdb;
PUNT_VAROUT varout;
PUNT_VARINP varinp;
int i,k;
char *var,*var_ret;
char *nome_var;
char app_char[1000];
char descr_obj[100];
WidgetClass wclass;
int app_int;
STRIN_ST strin [15];
char strin0_save[10];
char riga_compiled[1000];
Boolean ret=True;
char Modello[MAX_LUN_NOME_MODELLO+1];
char Blocco[MAX_LUN_NOME_BLOCCO+1];
int SCADA_DEBUG=0;
int ret_sys;
char comm_dbmftc[100];
char tag_dbm[100];


if( getenv("SCADA_DEBUG") != NULL )
   SCADA_DEBUG = atoi( getenv("SCADA_DEBUG") );

for(i=0; i<15; i++) {
   strin[i].stringa[0]=0;
   strin[i].lun_stringa = 0;
}
riga_compiled[0]=0;
strcpy(riga_compiled,"# ");


if(!controlla_confinfo(compilatore,w,confinfo,num_confinfo,
		resources,num_resources))
	ret=False;
else
 if((compilatore->olcompiler.tipo_comp==COMP_WARN)||
	(compilatore->olcompiler.tipo_comp==COMP_CHECK))
		return(True);
	
wclass=XtClass(w);
sprintf(descr_obj, "CompileConfinfo(%s,%s)", 
	  wclass->core_class.class_name,XtName(w));
/*
printf("CompileConfinfo DEBUG: descr_obj = %s\n", descr_obj);
*/

for(i=0;i<num_confinfo;i++)
        {
        switch(confinfo[i].confinfo_type){
          case XlRTipoVarInp:
          case XlRTipoVarInp+LOCKED_PAG:
          case XlRTipoVarReg:
          case XlRTipoVarReg+LOCKED_PAG:
             {
	     /*
		verifico che la variabile non sia di tipo XlRTipoVarReg
		di un oggetto XlPort
	     */
	     if((confinfo[i].confinfo_type==XlRTipoVarReg) && XlIsPort(w))
		break;

             /*
                identifica il nome della risorsa
             */
             for(k=0;k< num_resources;k++)
                if(!strcmp(resources[k].resource_name,
                        confinfo[i].resource_name))
                           break;
             /*
                estrae la label della variabile
             */
/*
printf("OlCompiler DEBUG: resource_str = %s resource_size = %d\n",
        *((char**)((char*)w+resources[k].resource_offset)), resources[k].resource_size);
printf("OlCompiler DEBUG: resource_name = %s resource_offset = %d\n",
       resources[k].resource_name, resources[k].resource_offset);
*/
             memcpy(&var,
                (char*)((char*)w+resources[k].resource_offset),
                resources[k].resource_size);
             strcpy(app_char,var);
/*
printf("OlCompiler DEBUG: app_char = #%s# lungh_app_char = %d\n", app_char,
        strlen(app_char));
*/
             separa_str(app_char,strlen(app_char),7,strin);
             /*
                carica dall'oggetto la struttura PUNT_VARINP
             */
             memcpy(&varinp,(char*)(confinfo[i].compiled_offset+(char*)w),
                        confinfo[i].compiled_size);
             /*
                richiama il metodo CompilaVar dell'oggetto compilatore
             */
             varinp.pdb.nmod= -1;
	     varinp.pdb.indice= -1;
	     varinp.pdb.tipo= -1;
/* 
Se il terzo token della variabile e' SUPERVIS -> origin=1 cioe' var SCADA
altrimenti  origin=0 cioe' var di provenienza SIMULATORE
*/
             if((!strcmp(strin[2].stringa,SUPERVIS)) || 
		(!strcmp(strin[1].stringa,SUPERVIS)))
             	varinp.pdb.origin=SCADAID;
	     else 
		varinp.pdb.origin=SIMULAID;
	     varinp.valore= -1;
	     varinp.tipo_elab= INP_NORM;
             varinp.unimis.a=1;
             varinp.unimis.b=0;
	     sprintf(varinp.unimis.descr,"--");
	     /*
		riempe il campo pdb.nmod
	     */
             if(varinp.pdb.indice!= -1)
	     	varinp.pdb.nmod=INDTOMOD(varinp.pdb.indice);

             /*
		verifica se il tipo e' XlRTipoVarReg
	     */
             if((confinfo[i].confinfo_type == XlRTipoVarReg)||
		(confinfo[i].confinfo_type == XlRTipoVarReg+LOCKED_PAG))
                if((strin[0].lun_stringa!=0)&&(strin[0].stringa[0]!=' '))
		    {
		    char * app_strin;

		    strin[0].stringa[2]=0;
		    strcpy(strin0_save,strin[0].stringa);
		    app_strin = strin[0].stringa;
/*
 Modifica la tag della variabile di regolazione in base ad eventuali
 collegamenti (se variabile di input collegata) e comunque la completa
 aggiungendo la tag del modulo di regolazione e i 2 caratteri identificativi
 della task
*/
		    ConvertiVarReg(compilatore,w,&app_strin);
		    if(app_strin!=NULL)
		    	strcpy(strin[0].stringa,app_strin);
printf("\n 1) strin[0].stringa = %s\n", strin[0].stringa);
		    strcpy(strin[3].stringa,"NOP");
		    strcpy(strin[4].stringa,"1.0");
		    strcpy(strin[5].stringa,"0.0");
		    strcpy(strin[6].stringa,"---");
		    }

             if((strin[0].lun_stringa!=0)&&(strin[0].stringa[0]!=' '))
               if(!CompileVar (compilatore,strin[0].stringa,&(varinp.pdb),
			confinfo[i].confinfo_type))
                  {

if ( getenv("RISOLVI_REGOLAZIONE") && (varinp.pdb.origin == SIMULAID) )
{
   strcpy(comm_dbmftc,"dbmftc2 REG_INT_CONN_DB ");
   strcat(comm_dbmftc, strin[0].stringa);
/*
printf("CompileConfinfo DEBUG: comm_dbmftc = %s\n", comm_dbmftc);
*/
   ret_sys=exec_dbmftc(comm_dbmftc,tag_dbm);
/*
printf("CompileConfinfo DEBUG: ret_sys = %d\n", ret_sys);
*/
   switch (ret_sys) {
      case 0:
/*
printf("CompileConfinfo DEBUG: tag_dbm = #%s#\n", tag_dbm);
*/
         if(!CompileVar (compilatore,tag_dbm,&(varinp.pdb), confinfo[i].confinfo_type))
            {
               XlErrComp((char*)w,"CompileConfinfo","Input tag sostituita per regolazione not found",tag_dbm);
               varout.pdb.nmod= -1;
               varout.pdb.indice= -1;
               varout.pdb.tipo= -1;
               varout.pdb.origin= -1;
               varout.tipo_pert= -1;
               varout.value= -1;
               varout.rateodurata= -1;
               varout.unimis.a=1;
               varout.unimis.b=0;
               sprintf(varout.unimis.descr,"--");
               ret = False;
            }
         break;
      case 1:
         XlErrComp((char*)w,"CompileConfinfo","External program input parameter error","dbmftc2");
         ret=False;
         break;
      case 2:
         XlErrComp((char*)w,"CompileConfinfo","Database not found","REG_INT_CONN_DB");
         ret=False;
         break;
      case 3:
/*
printf("CompileConfinfo DEBUG: sono in case 3\n");
*/
         XlErrComp((char*)w,"CompileConfinfo","Input tag not found",strin[0].stringa);
         ret=False;
         break;
      case -1:
         XlErrComp((char*)w,"CompileConfinfo","External program not found","dbmftc2");
         ret=False;
         break;
   }

}
else
{
		  XlErrComp((char*)w,"CompileConfinfo","Input tag not found",strin[0].stringa);
                  if( SCADA_DEBUG && (varinp.pdb.origin==SCADAID))
                  {
                     CompileVar (compilatore," ",&(varinp.pdb), confinfo[i].confinfo_type);
                  }
                  else
                  {
                     varinp.pdb.nmod= -1;
   		     varinp.pdb.indice= -1;
      		     varinp.pdb.tipo= -1;
                     varinp.pdb.origin=-1;
   		     varinp.valore= -1;
		     varinp.tipo_elab= -1;
		     varinp.unimis.a=1;
		     varinp.unimis.b=0;
		     sprintf(varinp.unimis.descr,"---");
                    ret=False;
                  }
                  }
}
	     /*
		riempe il campo pdb.nmod che sara' calcolato da INDTOMOD per
		variabili di origine SIMULATORE e sara' posto a 0 per
		variabili di tipo SCADA (essendo sempre =0 il numero di
		modelli per lo SCADA
	     */
             if((varinp.pdb.indice!= -1)&&(varinp.pdb.origin==SIMULAID)) /*simulatore */
	     	varinp.pdb.nmod=INDTOMOD(varinp.pdb.indice);
	     else if((varinp.pdb.indice!= -1)&&(varinp.pdb.origin==SCADAID)) /*scada*/
		varinp.pdb.nmod=0;
	     /*
		riempe il campo tipo_elab
	     */
	
	  if(strcmp(strin[1].stringa,SUPERVIS))	/* se 2� token !=SUPERVIS */
	     {
             if(varinp.pdb.indice!= -1)
		{
		if(strcmp(strin[3].stringa,"NOP")==0)
			varinp.tipo_elab=INP_NORM;
		else
			varinp.tipo_elab=INP_NEG;
		}
	     /*
		carica le unita' di misura
	     */
	     if(varinp.pdb.indice!= -1)
		{
		varinp.unimis.a=atof(strin[4].stringa);
		varinp.unimis.b=atof(strin[5].stringa);
		sprintf(varinp.unimis.descr,"%s",strin[6].stringa);
		}

	     }/* end if secondo token != SUPRVIS */
             /*
                 Ricarica nell'oggetto la struttura compilata
             */
             memcpy((char*)(confinfo[i].compiled_offset+(char*)w),
                        &varinp, confinfo[i].compiled_size);
             /* 
		carico la risorsa compiled_str
	     */
	     sprintf(app_char,"%d %d %d %d %d %f %f %s | ",
		varinp.pdb.nmod,varinp.pdb.indice,
		varinp.pdb.tipo,varinp.pdb.origin,
		varinp.tipo_elab,
		varinp.unimis.a,varinp.unimis.b,varinp.unimis.descr);
	     strcat(riga_compiled,app_char);
             break;
             }
          case XlRTipoVarOut:
          case XlRTipoVarOut+LOCKED_PAG:
             {
             /*
                identifica il nome della risorsa
             */
             for(k=0;k< num_resources;k++)
                if(!strcmp(resources[k].resource_name,
                        confinfo[i].resource_name))
                           break;
             /*
                estrae la label della variabile
             */
             memcpy(&var,
                (char*)((char*)w+resources[k].resource_offset),
                resources[k].resource_size);
	     strcpy(app_char,var);
             separa_str(app_char,strlen(app_char),9,strin);
             /*
                carica dall'oggetto la struttura PUNT_VAROUT
             */
             memcpy(&varout,(char*)(confinfo[i].compiled_offset+(char*)w),
                        confinfo[i].compiled_size);
             /*
		inizializza i valori
             */
	     varout.pdb.nmod= -1;
	     varout.pdb.indice= -1;
	     varout.pdb.tipo= -1;
	     varout.pdb.origin= SIMULAID;   /* attualmente non esistono output
						per SCADA	*/
	     varout.tipo_pert= -1;
	     varout.value= -1;
	     varout.rateodurata= -1;
             varout.unimis.a=1;
             varout.unimis.b=0;
	     sprintf(varout.unimis.descr,"---");
	     /*
		Riempe il campo tipo_pert
	     */
	     varout.tipo_pert= -2;
	     if(!strcmp(strin[3].stringa,"PERT_SCALINO"))
		varout.tipo_pert=PERT_SCALINO;
	     if(!strcmp(strin[3].stringa,"PERT_RAMPA"))
		varout.tipo_pert=PERT_RAMPA;
	     if(!strcmp(strin[3].stringa,"PERT_IMPULSO"))
		varout.tipo_pert=PERT_IMPULSO;
	     if(!strcmp(strin[3].stringa,"PERT_UP_DOWN"))
		varout.tipo_pert=PERT_UP;
	     if(!strcmp(strin[3].stringa,"PERT_NOT"))
		varout.tipo_pert=PERT_NOT;
	     if(!strcmp(strin[3].stringa,"PERT_MALFUNCTION"))
		varout.tipo_pert=PERT_MALFUNCTION;
	     if(!strcmp(strin[3].stringa,"PERT_MALFUNCTION_STOP"))
		varout.tipo_pert=PERT_MALFUNCTION_STOP;

	     if((!strcmp(strin[3].stringa,"NULL"))||(strin[0].lun_stringa==0))
		varout.tipo_pert= -1;
	     if(varout.tipo_pert == -2)
		{
		XlErrComp((char*)w,"CompileConfinfo","Pert not found",strin[3].stringa);
		ret=False;
		varout.tipo_pert= -1;
		}
	     /*
		Riempe il campo value
	     */
	     varout.value=atof(strin[4].stringa);
	     /*
		Riempe il campo rateodurata
	     */
	     varout.rateodurata=atof(strin[5].stringa);
	     /*
		carica le unita' di misura
	     */
	     if(varout.pdb.indice!= -1)
		{
		varout.unimis.a=atof(strin[6].stringa);
		varout.unimis.b=atof(strin[7].stringa);
		sprintf(varout.unimis.descr,"%s",strin[8].stringa);
		}
             /*
                richiama il metodo CompilaVar dell'oggetto compilatore
		per la compilazione del campo pdb.indice
             */
             if((strin[0].lun_stringa!=0)&&(varout.tipo_pert>0))
               if(!CompileVar(compilatore,strin[0].stringa,&(varout.pdb),
			confinfo[i].confinfo_type))
                  {
                  XlErrComp((char*)w,"CompileConfinfo","Output tag not found",strin[0].stringa);
                  varout.pdb.nmod= -1;
                  varout.pdb.indice= -1;
                  varout.pdb.tipo= -1;
                  varout.pdb.origin= -1;
                  varout.tipo_pert= -1;
                  varout.value= -1;
                  varout.rateodurata= -1;
                  varout.unimis.a=1;
                  varout.unimis.b=0;
                  sprintf(varout.unimis.descr,"--");
                  ret = False;
                  }
		/*
		IMPORTANTE !!!
		Origine variabile di output sempre simulatore
		*/
		varout.pdb.origin=SIMULAID;

	     /*
		Quando ci sara' SCADA ci sara' da introdurre anche
		INDTOMODS
		riempe il campo pdb.nmod
	     */
             if(varout.pdb.indice!= -1)
	     	varout.pdb.nmod=INDTOMOD(varout.pdb.indice);
	     /*
		riempe il campo pdb.tipo (non ancora utilizzato)
	     */
		
             /*
                 Ricarica nell'oggetto la struttura compilata
             */
                 memcpy((char*)(confinfo[i].compiled_offset+(char*)w),
                        &varout, confinfo[i].compiled_size);
             /* 
		carico la risorsa compiled_str
	     */
	     sprintf(app_char,"%d %d %d %d %d %f %f %f %f %s | ",
		varout.pdb.nmod,varout.pdb.indice,varout.pdb.tipo,
		varout.pdb.origin,varout.tipo_pert,varout.value,
		varout.rateodurata,
		varout.unimis.a,varout.unimis.b,varout.unimis.descr);
	     strcat(riga_compiled,app_char);
             break;
             }
          default:
                break;
        }
        }

if(riga_compiled[0]!=0)
	{
	*risorsa_compiled=(char*)malloc(strlen(riga_compiled)+1);
	strcpy(*risorsa_compiled,riga_compiled);
	}
return(ret);
}


static Boolean InitializeCompiler(OlCompilerObject compilatore) 
{
	if(!strcmp(compilatore->olcompiler.nome_compilatore,"olwarn"))
		{
                compilatore->olcompiler.tipo_comp=COMP_WARN;
                return(True);
                }
	if(!strcmp(compilatore->olcompiler.nome_compilatore,"olcheck"))
		{
                compilatore->olcompiler.tipo_comp=COMP_CHECK;
                return(True);
                }
	if(!strcmp(compilatore->olcompiler.nome_compilatore,"olsevere"))
		{
                compilatore->olcompiler.tipo_comp=COMP_SEVERE;
                return(True);
                }
        XlWarning("OlCompiler","InitializeCompiler","Compilatore inesistente");
	return(False);
}

Boolean OlCompilerSetConn(OlCompilerObject compilatore, OlConnObject conn)
{
if (compilatore == NULL)
	return(False);
if (conn == NULL)
	return(False);

compilatore->olcompiler.connessioni=conn;

return(True);
}

/*
	Creazione compilatore
*/
OlCompilerObject OlCreateCompiler(char *nome_compilatore, 
		OlDatabaseTopologiaObject database)
{
OlCompilerObject compilatore=NULL;
VARIABILI *var;
int nres=((Cardinal) (sizeof(resources) / sizeof(resources[0]))); /* numero di 
								risorse */
int k;

	compilatore=(OlCompilerRec*) XtMalloc(sizeof (OlCompilerRec));
	if(compilatore==NULL)
		{
		XlWarning("OlCompiler","XlCreateCompiler",
			 "Impossibile allocare compilatore oggetti");
		return(NULL);
		}
	/*
		allocazione e riempimento risorse 
	*/
	for(k=0;k<nres;k++)
	  {
	  if(!strcmp(resources[k].resource_name,"nome"))
	    {
	    if(nome_compilatore==NULL)
		{
	        compilatore->olcompiler.nome_compilatore=(char*)
			XtMalloc(strlen(resources[k].default_addr)+1);
		strcpy(compilatore->olcompiler.nome_compilatore,
			resources[k].default_addr);
		}
	    else
		{
	        compilatore->olcompiler.nome_compilatore=(char*)
			XtMalloc(strlen(nome_compilatore)+1);
		strcpy(compilatore->olcompiler.nome_compilatore,
			nome_compilatore);
		}
	    }
	  }
	/*
		Registrazione database
	*/
	if(strcmp(compilatore->olcompiler.nome_compilatore,"olsevere")==0)
	  {
	  if(database!=NULL)
		{
		compilatore->olcompiler.database=database;
		}
	  else
		{
		XlWarning("OlCompiler","XlCreateCompiler",
				"Database non definito");
		XtFree((char *)compilatore);
		return(NULL);
		}
	  }
	else
	  compilatore->olcompiler.database=NULL;
	
	
	/*
		inizializzazione del compilatore
	*/
	if(
	 (olCompilerClassRec.olcompiler_class.initialize_compiler)(compilatore))
		return(compilatore);
	else
		{
		XlWarning("OlCompiler","XlCreateCompiler",
			 "Compilatore non allocato");
		XtFree((char *)compilatore);
		return(NULL);
		}
}

/*
	Distruzione compilatore
*/
Boolean OlDestroyCompiler(OlCompilerObject compilatore )
{

if(compilatore==NULL)
	{
	XlWarning("OlCompiler","OlDestroyCompiler",
                         "Compilatore non allocato");
	return(False);
	}


if(compilatore->olcompiler.nome_compilatore)
	XtFree((char *) compilatore->olcompiler.nome_compilatore);

XtFree((char *)compilatore);

return(True);
}

static int separa_str(
 char riga[],
 int lun,
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i,j;

   for (s=riga, i=0; i<nstr; i++) {
      if((s = strtok( s, " \t")) == NULL) {
         for(j=i; j<nstr; j++) {
            strin[j].stringa[0]= 0;
            strin[j].lun_stringa = 0;
         }
         break;
      }
      else {
         if(strin[i].stringa==(char*)NULL) {
            sprintf(strin[i].stringa," ");
            strin[i].lun_stringa = strlen(strin[i].stringa);
         }
         else {
            strcpy(strin[i].stringa,s);
            strin[i].lun_stringa = strlen(s);
         }
      }
/*
printf("separa_str DEBUG: strin[%d] = %s\n", i, strin[i].stringa);
*/
      s = (char*)NULL;
   }
}


int exec_dbmftc(comando,tag_dbm)
char *comando;
char *tag_dbm;
{
FILE *fp;
int retval,exit_stat;
char riga[101];
signal(SIGCHLD,(SIG_DFL) );
fp=popen(comando,"r");
while(fgets(riga,100,fp)!=NULL)
        {
/*
printf("exec_dbmftc DEBUG: riga = #%s# lungh_riga = %d\n",riga, strlen(riga));
*/
        strcpy(tag_dbm,riga);
        if(tag_dbm[strlen(tag_dbm)-1] == '\n')
           tag_dbm[strlen(tag_dbm)-1] = '\0';
        }
retval=pclose(fp);
/*
printf("\n FINE COMANDO retval=%d exit status= %d\n",retval,WEXITSTATUS(retval));
*/
return(WEXITSTATUS(retval));
}


static Boolean controlla_confinfo(OlCompilerObject compilatore,Widget w,
			XlConfInfo *confinfo, int num_confinfo,
			XtResourceList resources, int num_resources)
{
WidgetClass wclass;
char descr_obj[100];
char app_char[300];
Boolean ret=True;
STRIN_ST strin [10];
int i,k;
char *var;



/*
	momentaneamente
*/
return(ret);

	wclass=XtClass(w);
	sprintf(descr_obj, "controlla_confinfo(%s,%s)", 
		wclass->core_class.class_name,XtName(w));

for(i=0;i<num_confinfo;i++)
  {
  switch(confinfo[i].confinfo_type){
    case XlRTipoVarInp:
    case XlRTipoVarReg:
	{
       	/*
       	identifica il nome della risorsa
       	*/
       	for(k=0;k< num_resources;k++)
       	   if(!strcmp(resources[k].resource_name,confinfo[i].resource_name))
                      	break;
        /*
        estrae la label della variabile
        */
        memcpy(&var,(char*)((char*)w+resources[k].resource_offset),
               	resources[k].resource_size);
        strcpy(app_char,var);
        separa_str(app_char,strlen(app_char),3,strin);
	if(!strcmp(strin[0].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
	       "\n* nome della variabile di ingresso  non definita");
	   ret=False;
	   }
	if(!strcmp(strin[1].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
		    "\n* nome del modello non definito");
	   ret=False;
	   }
	if(!strcmp(strin[2].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
	    "\n* tipo di operazione sulla variabile di ingresso non definita");
	   ret=False;
	   }
	if(compilatore->olcompiler.tipo_comp!=COMP_WARN&& !ret)
		return(ret);
	}
    case XlRTipoVarOut:
	{
       	/*
       	identifica il nome della risorsa
       	*/
       	for(k=0;k< num_resources;k++)
       	   if(!strcmp(resources[k].resource_name,confinfo[i].resource_name))
                      	break;
        /*
        estrae la label della variabile
        */
        memcpy(&var,(char*)((char*)w+resources[k].resource_offset),
               	resources[k].resource_size);
        strcpy(app_char,var);
        separa_str(app_char,strlen(app_char),3,strin);
	if(!strcmp(strin[0].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
	       "\n* nome della variabile di uscita  non definita");
	   ret=False;
	   }
	if(!strcmp(strin[1].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
		    "\n* nome del modello non definito");
	   ret=False;
	   }
	if(!strcmp(strin[2].stringa," "))
	   {
	   XlWarning("OlCompiler",descr_obj,
	    "\n* tipo di perturbazione non definita");
	   ret=False;
	   }
	if(compilatore->olcompiler.tipo_comp!=COMP_WARN&& !ret)
		return(ret);
	}
     }
   }
return(ret);
}

/*
 ConvertiVarReg esegue 2 operazioni sulla tag passata
 come parametro
 1) modifica in caso di variabile connessa tramite
    porta di input ad altro vlocchetto
 2) completamento del nome della variabile aggiungendo
    tag progressiva dell'icona di regolazione e primi
    2 caratteri del nome della task.
 Il puntatore s in uscita bviene assegnato tramita assegnamento
 a  variabile statica interna. Non deve essere fatta la free
 ma la copia se si utilizza il risultato in chiamate successive.
*/

static void ConvertiVarReg(OlCompilerObject compilatore, Widget w, char **s)
{
char *app1_char;
static char app2_char[20];
Widget portC;
char tag_task[3];
char task_dir[FILENAME_MAX];
char dummy_char[10];
char *tagSave;
char *appo=NULL;
/*
 Verifica se nell'icona di regolazione in esame (w) e'
 presente una porta che e' associata alla tag
 passata con il parametro s.
 la funzione VarRegIsConnect ritorna l'indice di
 widget dell'eventuale porta connessa; altrimenti
 ritorna NULL
*/
	portC=VarRegIsConnect(w, *s);
	if(portC!=NULL)
		{
/*
 Salva il puntatore al nome della tag cercata perche'
 la funzione successiva lo modifica metytendolo a NULL
 se non trova la tag: per proseguire nel caso di
 porte di interfaccia non connesse viene salvata la 
 tag cercata
*/
		tagSave=*s;
/*
 Ricava la tag della porta connessa alla porta in esame:
 se la tag viene trovata regolarmente, internamente a
 GetTagPorta viene chiamata a sua volta la funzione
 ConvertiVarReg per comporre il nome completo della
 variabile (aggiungendo tag della icona e primi due caratteri
 della task. In questo modo la tag e' stata
 convertita e quindi si esce dalla funzione ConvertiVarReg
*/
		GetTagPorta(compilatore, portC,s);
		if(*s!=NULL) 
			return;
/*
 Non si e' trovata una tag relativa alla porta connessa:
 siamo nel caso in cui la connessione era verso una IconReg
 di interfaccia che non e' stata connessa sullo schema
 di provenienza.
 Ci si comporta come se la porta non fosse collegata
 (variabile non connessa).
*/
		*s=tagSave;
printf("ConvertiVarReg [%s] la tag %s e' connessa ad una int non connessa \n",XtName(w), *s);
		}
/*
 La Tag in esame non e' connessa tramite porta ad
 alcuna altra variabile: si completa il nome
 della variabile con tag della icona ospite e
 caratteri inziali della task.
*/
        if(XlIsIconReg(w))
           get_something(w,XlNtagName, (void*) &app1_char);
        else
          if(XlIsDispReg(w))
             get_something(XtParent(w),XlNtagName, (void*) &app1_char);
          else
	     return;

/*
    Accodo i primi due caratteri del direttorio della task
    eventualmente filtrati 
*/
        strcpy(task_dir,XlGetenv("LEGOMMI_PAG"));
        appo = OlEstrRegTaskName(task_dir,dummy_char);
        if( appo !=NULL )
            {
            strcpy(tag_task,appo);
            tomaius(tag_task);
            free(appo);
            }
        else
            strcpy(tag_task,"");

	sprintf(app2_char,"%s%s%s",*s,app1_char,tag_task);
        *s=app2_char;
}

/*
 Verifica se nell'icona di regolazione in esame (w) e'
 presente una porta che e' associata alla tag
 passata con il parametro tag.
 la funzione VarRegIsConnect ritorna l'indice di
 widget dell'eventuale porta connessa; altrimenti
 ritorna NULL
*/
static Widget VarRegIsConnect(Widget w, char * tag)
{
Widget ico;
WidgetList figli;
Cardinal num_figli;
int i;
char *tagPorta;

if(XlIsIconReg(w))
	ico = w;
else
	ico = XtParent(w);
/*
	ricavo i figli dell'icona
*/
XtVaGetValues( ico, XmNnumChildren, &num_figli,
                        XmNchildren, &figli, NULL );
/*
 Esamina tra le porte se ne esiste una:
	1) connessa
	2) con tag pari a quella cercata
*/
for(i=0;i<num_figli;i++)
	if(XlIsPort(figli[i]))
	   {
	   if((XlPortIsConnected(figli[i]))&&(XlPortIsInput(figli[i])))
		{
		XtVaGetValues(figli[i],XlNportName,&tagPorta,NULL);
		if(strcmp(tagPorta,tag)==0)
			return(figli[i]);
		}
           }
return(NULL);
}
/*
 Ricava la tag della porta connessa ad una
 porta di cui viene passato l'indice di widget
 Se la tag viene trovata viene chiamata la
 funzione  ConvertiVarReg per completare
 il nome della variabile.
*/
static void GetTagPorta(OlCompilerObject compilatore, Widget w, char **s)
{
Widget drawing;
WidgetList figli;
Cardinal num_figli;
int i;
Widget porta;
char *nomeP;
char *tag;
char *prog;
static char app_char[20];
/*
 ricava il nome della porta connessa
*/
XtVaGetValues(w,XlNportNameConnect,&nomeP,NULL);
/*
 individua l'indice di widget della porta 
 connessa (porta)
*/
drawing =XtParent(XtParent(w));
XtVaGetValues( drawing, XmNnumChildren, &num_figli,
                        XmNchildren, &figli, NULL );
for(i=0;i<num_figli;i++)
	if((porta=XtNameToWidget(figli[i],nomeP))!=NULL)
		break;
/*
	se la porta connessa e' figlia di una icona di interfaccia 
	esamina il contenuto del file Connessioni per trovare la
        tag della porta connessa sull'altro schema.
*/
if(XlIsInterfaceIconReg(XtParent(porta)))
	{
	XtVaGetValues(w,XlNportName,&tag,NULL);
	XtVaGetValues(XtParent(w),XlNtagName,&prog,NULL);
	sprintf(app_char,"%s%s",tag,prog);
	*s = OlFindConnectedTag(compilatore->olcompiler.connessioni,app_char);
	return;
	}
/*
 In caso di normale icona di regolazione ricava
 la tag della porta connessa
*/
XtVaGetValues(porta,XlNportName,&tag,NULL);
*s=tag;
ConvertiVarReg(compilatore,XtParent(porta),s);
}
