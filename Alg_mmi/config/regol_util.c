/**********************************************************************
*
*       C Source:               regolazione.c
*       Subsystem:              8
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue May 30 10:17:02 2006 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: regol_util.c-10 %  (%full_filespec: regol_util.c-10:csrc:1 %)";
#endif
/* modulo regolazione.c
   tipo 
   release 5.9
   data 3/19/96
   reserved @(#)regolazione.c	5.9
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)regolazione.c	5.9\t3/19/96";
/*
        Fine sezione per SCCS
*/
/*
 regolazione.c 
 Contiene routines tipiche della applicazione REGOMMI
 dove si utilizzano strutture particolari tipiche
 della regolazione in ambiente LEGO
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <UxXt.h>
#include "config.h"
#include "message.h"
#include "forme.h"     /* per topologia schemi regolazione */
#include "top_icon.h"  /* per topologia schemi regolazione */
#include <Xl/Xl.h>
#include <Xl/XlPort.h>
#include <Xl/XlIconReg.h>
#include <Xd/Xd.h>
#include <Ol/OlForm.h>
#include <Ol/OlForm_regol.h>
#include <Xm/DrawingA.h>
#include <Ol/OlConn.h>
#if defined VMS
#include <stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if defined VMS
#include <unixlib.h>
#endif

#include "libutilx.h"
#include "utile.h"

typedef struct s_porte {
	int tipo;
	char *nome_porta; /* nome (LEGO) della porta */
	Boolean connessa;
	char *porta_conn; /* nome (LEGOMMI) della porta cui e' connessa */
	char *icona_conn; /* nome (LEGO) della icona connessa */
	char *label_sost; /* label (2char.) della variabile che
				 deve sostituire il
                                 nome della variabile (caso di porta di
				 ingresso collegata) 
				 N.B.: va completata con
                                 la tag dell'icona (icona_conn)
				 per avere il nome
                                 completo di 6 caratteri.
				*/
	} S_PORTE;
/*
   Funzioni esterne
*/

/*  
   Funzioni di appoggio
*/
int rd_f14 (int , char *, char *, char *, int *, char *);
int  setta_valori_14(int, char *, PAGINA *);
int PropagaValConn(Widget , char *, char *, char *, PAGINA *);
int PropagaValoreFraPorte(Widget, Widget, char *, PAGINA *);
int estrai_nome(char *, char *);
int controllo_exe(char *);
int salva_data(PAGINA *);
int TestMacro(char *, char*, char*, char*);
int LeggiListe(char *, char *);
int Widgetcmp(Widget * , Widget  *);
int FileNameInproc(char *, char *, char *);
extern int FileNameLoc(char *, char *, char *);
extern int EstrWorkFile(char *, char *, char *);

/*  
   Funzioni per la gestione di lista_schemi.dat o lista_macro.dat
*/
int gest_lista_schemi(char *, char *, char *, char *, PAGINA *, char *, int);
int eli_schema(LST_SCHM *, char* , int, char * );
int ins_schema(LST_SCHM *, int , char* , char* );
int schema_presente(LST_SCHM *, char *, int );
int rd_lst_schemi(LST_SCHM **, char *, char *);
int wr_lst_schemi(LST_SCHM *lst, int ,char *);
int compara_lst(LST_SCHM * , LST_SCHM *);
void elimina_da_lista(char *, int, char *); 
int trova_in_lista(char *, char *, int, int);


/*  
   Funzioni di appoggio
*/
char *getpul (char *, int , FILE *);
int nome_underscore(char *, char *, int );
int  MessaggiErrore(int , int , char *);

void get_child(Widget,WidgetList *,Cardinal *);
Widget get_wid_icon_conn(Widget,char *);
Widget get_wid_port(Widget,char *);


VARS *varsch;
char *lista_moduletti="lista_moduli_reg.dat";
char *lista_macro="lista_macro.dat";
char *lista_schemi="lista_schemi.dat";
char *lib_schemi="schemi_reg.a";
char *lib_macro="macro_reg.a";
/*
     Variabili con i pathname definiti dal Context.
*/
char workfile1[FILENAME_MAX];
char envfile1[FILENAME_MAX];
char rtffile1[FILENAME_MAX];



/*
   Struttura per leggere la lista dei moduletti degli
   schemi e dei macromoduli durante la compilazione
   delle pagine.
*/
LST_SCHM *ls_moduletti=NULL;
LST_SCHM *ls_macromod=NULL;
LST_SCHM *ls_schemireg=NULL;
int n_mcro1,n_mdlt,n_schm;



/*
 is_tag_in_port
 ritorna True se la tag passata (primi 2 caratteri) coincide con
 la tag di una delle porte presenti all'interno dell'icona di 
 regolazione wid.
*/
Boolean is_tag_in_port(Widget wid, char *tag)
{
Widget *childcomp;
int j;
char *nome_porta;
Cardinal num_childcomp;
Arg arg[4];

/*  Setto la cella del vettore degli argomenti al di fuori
	del ciclo for  */
XtSetArg( arg[0], XlNportName, &nome_porta);

/* recupero i figli di XlComposite */
get_something (wid, XmNchildren, (void*) &childcomp);
get_something (wid, XmNnumChildren, (void*) &num_childcomp);

/*  Controllo tutti i figli della IconReg  */
for(j=0;j<num_childcomp;j++)
  {
  /*  Se il figlio e' una porta ne controllo la tag  */
  if ( XlIsPort(childcomp[j]) )
    {
    XtGetValues( childcomp[j], arg, 1);
    if( strncmp(nome_porta, tag, 2) == 0 )
      return(True);
    }
  }
return(False);
}

/*
 get_info_porte
 ricava dall'oggetto icon reg le informazioni
 relative alle sue porte memorizzandole in un
 vettore di strutture allocato internamente alla
 funzione
 wid = indice di widget della icona di regolazione
 num_port = punta ad un'intero nel quale viene restituito 
 il numero di porte presenti.
*/
S_PORTE * get_info_porte(Widget wid, int *num_porte)
{
int j;
Arg arg[4];
int ind;
S_PORTE *p_port;
Widget wid_icon_conn;
Widget wid_porta_conn;
extern Widget get_wid_icon_conn();
extern Widget get_wid_port();
WidgetList childcomp;
Cardinal num_childcomp;

/* recupero i figli di XlComposite */
get_child(wid,&childcomp,&num_childcomp);

/*
 Si alloca le strutture per memorizzare le informazioni relative
 alle porte; il numero massimo di porte e' pari al numero dei figli
 del composite (IconReg).
 Viene allocato un vettore di strutture dimensionato a num_childcomp.
*/
ind=0;
p_port=(S_PORTE *)XtCalloc(num_childcomp, sizeof(S_PORTE));
for(j=0;j<num_childcomp;j++)
         {
	 if(XlIsPort(childcomp[j]))
		{
		XtSetArg(arg[0],XlNportNameConnect,&p_port[ind].porta_conn);
		XtSetArg(arg[1],XlNportName,&p_port[ind].nome_porta);
		XtSetArg(arg[2],XlNtipoPort,&p_port[ind].tipo);
		XtGetValues(childcomp[j],arg,3);
/*
 se la porta e' connessa ricava il nome della icona di regolazione
 cui e' connessa
*/
		if(strlen(p_port[ind].porta_conn)!=0)
			{
			p_port[ind].connessa=True;
/*
 Ricava l'indice di Widget della icona connessa
*/
			wid_icon_conn=get_wid_icon_conn(XtParent(wid),
				p_port[ind].porta_conn);
/*
 Ricava la tag della icona connessa
*/
			XtSetArg(arg[0],XlNtagName,&p_port[ind].icona_conn);
			XtGetValues(wid_icon_conn,arg,1);
/*
 Ricava l'indice di widget della porta connessa
*/
			wid_porta_conn=get_wid_port(wid_icon_conn,
					p_port[ind].porta_conn);
/*
 Ricava la tag della variabile associata alla porta connessa
*/
			XtSetArg(arg[0],XlNportName,&p_port[ind].label_sost);
			XtGetValues(wid_porta_conn,arg,1);
			}
		else
			{
			p_port[ind].connessa=False;
			}
		ind++;
		}
         }
*num_porte=ind;
return(p_port);
}


/* 
  Creazione topologia schemi di regolazione 
  Valori ritornati: 
   n   Compilazione corretta, modulo inserito in lista_schemi.dat 
      (lista_macro.dat). n e' il numero di schemi inseriti nella lista.
   0   Compilazione corretta, modulo non inserito (gia' presente) in 
      lista_schemi.dat (lista_macro.dat).
  -1  Compilazione fallita.
*/
int compila_reg(PAGINA *pag, int compile_all, int macrobl)
{
static int no_error=1;
int inserimenti_in_lista = 0;
FILE *fp;
Boolean trovata;
int i,j,k,h;
int tipo_icon_reg;
int num_var;
int nmax_var;
int num_dat;
int num_porte;
S_PORTE *p_porte;
char *buffer;
char *punt;
char *appoggio=NULL;
char *str_tag, *str_modul, *str_descr;
char *str_input_value;
TOP_ICON rec;  /* record con le informazioni relative all'icona */
Arg arg[10];
int narg=0;
int nval;
int ret;
int ret_sy;
WidgetList children;
WidgetList children_get;
Cardinal num_children;
OlFormObject form;
char stringa[MAX_L_TAGS];
char tag_tipo[MAX_L_TAGS];
char scelta[10];
char comando[500];
char schema_min[5];
char nome_pagina_reg[500];
char nome_file_top[500];
char nome_file_macro[500];
char nome_file_err[500];
char nome_task_err[500];
char nome_libut_reg[500];
char nome_vms[500];
char file_conn[500];  /* Path name di Connessioni.reg */
char app[500];
char nome_task[9];
char work_pag[500];
char messaggio[500];
char path_pwd[500];
char desc_pagina[LUNDESC];  /* N.B. desc_pagina in config.h e' dimensionato 200
                                    mentre topsch scrive su file 54 caratteri
                                    ( LUNDESC) percio' anche sul file .reg
                                    scrivo  54 caratteri */
int in_use;                 /* Da PagGetInUse passata a gen_stazionario */
char from[100],to[100];     /* Nomi per operazioni sui files */
char tipo_codice[20];
char tipo_lista[20];


   if(PagGetType(pag) != TYPE_REGOLAZIONE)
      {
      sprintf(messaggio,"%s is not a regulation page",pag->nomepag);
      if ( !compile_all )
         printf("%s\n",messaggio);
      return(-2);
      }

   nmax_var=0;
/*
   printf("\n\n   Richiamata compila_reg per lo schema %s \n\n",
           pag->nomepag);
*/
/*
   Se si tratta della compilazione di una sola pagina si caricano le
   i pathname d'ambiente.
*/
      ret = EstrWorkFile(workfile1,envfile1,rtffile1);
      if ( ret!=0 )
         {
         fprintf(stderr,"Non sono definite le variabili d'ambiente\n");
         return(-1);
         }

/******************
   0. APERTURA FILE per compilazione schemi.
   Il nome e' quello della pagina con l'estensione .reg
******************/
/*
   Costruisce il path name della pagina in base alla variabile
   di environment LEGOCAD_USER
*/
   if ( strlen(pag->nomepag)>4 )
      fprintf(stderr,"ATTENZIONE nome pagina lungo <%d> caratteri\n",
           strlen(pag->nomepag));
   strncpy(schema_min,pag->nomepag,4);
   schema_min[4] = '\0';
   tominus(schema_min);

/* 
   Costruzione dei path dei files utilizzati 
*/
   if( XlGetenv(WORK_PAG)!=NULL )
     {
     strcpy(work_pag,XlGetenv(WORK_PAG));
#if defined VMS
     strcpy(work_pag,XlConvDirVms(XlGetenv(WORK_PAG)));
#endif
     }
   else 
     {
     strcpy(work_pag,"");
     strcpy(messaggio,"Non e' stata definita la variabile di ambiente LEGOMMI_WORK: ");
     strcat(messaggio," si assegna la variabile ENVPAG,proc");
     printf("%s\n",messaggio);
     strcpy(work_pag,XlGetenv(ENVPAG));
     strcat(work_pag,"/proc");
#if defined VMS
     strcpy(work_pag,XlConvDirVms(work_pag));
#endif
     if ( !XlPutenv(WORK_PAG,work_pag) )
        printf("Fallita la Putenv di WORK_PAG\n");
     }
/* File degli errori di compilazione pagina (nome pagina tale quale) */
   strcpy(nome_file_err,XlGetenv(ENVPAG));
   strcat(nome_file_err,"/");
   strcpy(app,XlGetenv(ENVPAG));
#if defined VMS
   strcpy(nome_file_err,XlConvDirVms(XlGetenv(ENVPAG)));
   strcpy(app,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
   strcpy(nome_task_err,nome_file_err);
   if( estrai_nome(app, nome_task)<0 )
     {
     fprintf(stderr,"nome estratto non corretto=%s\n",nome_task);
     strcpy(messaggio,"Nome task superiore agli 8 caratteri");
     goto FINE;
     }
   strcat(nome_task_err,nome_task);
   strcat(nome_task_err,".reg");

   strcat(nome_file_err,pag->nomepag);
   strcat(nome_file_err,".reg");

/* Nome file per le connessioni */
   strcpy(file_conn,XlGetenv(ENVPAG));
   strcat(file_conn,"/");
   strcat(file_conn,"Connessioni.reg");

/* Path libut_reg */
   if( XlGetenv("LEGOCAD_USER")==NULL )
     {
     strcpy(messaggio,"Non e' stata definita LEGOCAD_USER");
     goto FINE;
     }

   strcpy(nome_libut_reg,XlGetenv("LEGOCAD_USER"));
#if defined UNIX
   strcat(nome_libut_reg,"/legocad/libut_reg");
#endif

/* Files "schema.top" "schema_macro.reg" e "schema.reg"  */
   strcpy(nome_pagina_reg,work_pag);
#if defined UNIX
   strcat(nome_pagina_reg,"/");
#endif
   strcat(nome_pagina_reg,schema_min);
   strcpy(nome_file_top,nome_pagina_reg);
   strcat(nome_file_top,".top");
   strcpy(nome_file_macro,nome_pagina_reg);
   strcat(nome_file_macro,"_macro.reg");
   strcat(nome_pagina_reg,".reg");
/*
   printf("---------------------------\n");
   printf("nome_pagina_reg=%s\n",nome_pagina_reg);
   printf("nome_file_err  =%s\n",nome_file_err);
   printf("nome_task_err  =%s\n",nome_task_err);
   printf("nome_file_top  =%s\n",nome_file_top);
   printf("nome_file_macro=%s\n",nome_file_macro);
   printf("nome_libut_reg =%s\n",nome_libut_reg);
   printf("---------------------------\n");
*/
   if ( strlen(pag->geom.descriz)>=LUNDESC )
      {
      strncpy(desc_pagina,pag->geom.descriz,LUNDESC-1);
      desc_pagina[LUNDESC-1] = '\0';
      }
   else
      strcpy(desc_pagina,pag->geom.descriz);

/* 
   Eventuale creazione del direttorio di lavoro.
   path_pwd memorizza la posizione corrente che in realta' e'
   coincidente con ENVPAG
*/
   getcwd(path_pwd,sizeof(path_pwd));
   chdir(XlGetenv(ENVPAG));
   system("make -f $LEGO_BIN/cad_maketask proc/first");
   chdir(path_pwd);

   fp=fopen(nome_pagina_reg,"w+");
   if(fp==NULL)
      {
      strcpy(messaggio,"Cannot open regulation output file ");
      strcat(messaggio,nome_pagina_reg);
      printf("%s\n",messaggio);
      return(-1);
      }

/*
   Setta il file per la registrazione degli errori che ha come nome
   <nome_schema>.reg_err, scrive l'header del file e lo chiude
*/
   XlSetFileErrComp(nome_file_err);
   XlHeaderFileErrComp(nome_file_err);
   XlSetFileErrComp(NULL);




/********************
     1.SCRITTURA DESCRIZIONE SU FILE :
*******************/
fwrite(desc_pagina,LUNDESC,1,fp);

get_child(pag->drawing,&children_get,&num_children);

/*
 Copia l'array dei widget appartenenti alla pagina
 e li ordina alfabeticamente secondo il nome del
 widget (w1, w2, w25, ....) in modo che non muti
 da una compilazione alla successiva l'ordine dei
 records da cui viene generato il codice fortran
 ed i files relativi allo schema
*/
children=(WidgetList)XtMalloc(num_children * sizeof(Widget));
memcpy(children,children_get,num_children * sizeof(Widget));
qsort(children, num_children, sizeof(Widget), (__compar_fn_t)Widgetcmp);

if( num_children==0 )
      {
      strcpy(messaggio,"Page without any icon");
      goto FINE;
      }

/*
    Leggo una volta la lista dei moduletti e poi
    controllo che i moduletti che vengono istanziati
    stiano nella lista.
*/

/* Legge la lista dei moduletti e la tiene in memoria */
   ret = LeggiListe(nome_file_err,work_pag);
   if ( ret==-1 )
      {
      sprintf(messaggio,"Non trovata oppure vuota la lista dei moduletti\n");
      goto FINE;
      }

for(i=0;i<num_children;i++)
   {
   /*printf("\n children trattato = %s", XtName(children[i]));*/
   if( XlIsIconReg(children[i]) )
      {
	memset(&rec,0,sizeof(TOP_ICON));
/***********************
   2. PREPARAZIONE RECORD PER SCRITTURA SU FILE
   recupera i campi contenuti in icon reg
   prepara il record da salvare su (rec)
************************/
        narg=0;
	XtSetArg(arg[narg],XlNtagName,&str_tag);narg++;
	XtSetArg(arg[narg],XlNmodulName,&str_modul);narg++;
	XtSetArg(arg[narg],XlNobjectForm,&form);narg++;
	XtSetArg(arg[narg],XlNinputValue,&str_input_value);narg++;
	XtSetArg(arg[narg],XlNiconDescr,&str_descr);narg++;
	XtSetArg(arg[narg],XlNiconRegType,&tipo_icon_reg);narg++;
    	XtGetValues(children[i],arg,narg);
/*
        printf("\n\n\nstr_tag=%s| STR_MODUL=%s|\n",
                str_tag,str_modul);
        printf("\nstr_input_value=%s|\n",str_input_value);
        printf("str_descr=%s|\n",str_descr);
        printf("tipo_icon_reg=%d|\n",tipo_icon_reg);
*/
/*      
        Gestione flags per macromodulo (1), 0 se moduletto di regolazione.
        Il riconoscimento si basa sulla presenza nella lista.
        Un macromodulo non puo' contenerne un altro.
*/
        rec.fl_macro = TestMacro(str_modul,nome_file_err,work_pag,messaggio);
        if ( rec.fl_macro<0 )
           {
/*
           sprintf(messaggio,"Flag associato all'icona non corretto (%d)",
                   rec.fl_macro); 
*/
           goto FINE;
           }

/* carica i campi nel record */
	strcpy(rec.icn_sigl,str_tag);

	strcpy(rec.pr_nome,str_modul);
	strcat(rec.pr_nome,str_tag);

	strcpy(rec.pr_desc,str_descr);

/*
 Controlla se e' una icona di tipo interfaccia
 Verificare se e' corretto 
*/
	if(tipo_icon_reg != NORMAL_ICONREG)
		{
                /*printf("ICONA DI INTERFACCIA LA SALTO\n\n\n");*/
		rec.pr_nuvto=0;
		rec.pr_nuvin=0;
		rec.pr_nuvot=0;
		rec.pr_nuvdt=0;
		continue;
		}
		
	if(form == NULL)
		{
		fprintf(stderr,"Errore: form non allocata \n");
                strcpy(messaggio,"Compila_reg form non allocata");
                goto FINE;
		}

/* legge dalla form l'elenco delle variabili */
	buffer = (char *) OlGetTags(form,0);
        punt = buffer;

/* legge dalla form l'elenco delle variabili */

/*
  richiede il vettore di strutture contenente le informazioni 
  relative alle porte
*/
	p_porte=get_info_porte(children[i],&num_porte);

/* ricava il numero di variabili */

	memcpy(&num_var,punt,sizeof(int));
	punt += sizeof(int);

        rec.pr_nuvto = num_var;

/* 
  assegnamento nomi e tipi delle variabili
*/

	for(k=0;k<num_var;k++)
          {
/* copia nella stringa la tag (2 caratteri sigla + descrizione del tipo) */

          strcpy(stringa,punt);

/*        Leggo i valori dalla stringa */

          /*fprintf(stderr,"   EstrTagVal per variabile %2.2s%s\n",
                  stringa,rec.icn_sigl);*/
          appoggio = EstrTagVal(form,OL_FORM_INPUT_VALUE,
                                stringa,2,str_input_value);
          if( appoggio!=NULL )
	      {
              rec.pr_val[k] = atof(appoggio);  /* Lo metto come float su file */
              XtFree(appoggio);
	      appoggio=NULL;
	      }
          else
	      {
              fprintf(stderr,
          "compila_reg: EstrTagVal ritorna valore errato alla variabile \n");
              MessaggiErrore(8, 5, messaggio);
              goto FINE;
	      }
/*
  Compone il nome originario della variabile
*/
          strncpy(rec.pr_viorg[k],stringa,2);
          strcat(rec.pr_viorg[k],str_tag);
          strcpy(rec.pr_var[k],rec.pr_viorg[k]);
/*
 ricerca tra le porte della icona una porta con lo stesso nome
 della tag
*/
	  trovata=False;
	  for(j=0;j<num_porte; j++)
		{
		if(strncmp(stringa,p_porte[j].nome_porta,2)==0)
	            {
/*
 Se la porta e' di input ed e' connessa la variabile ad essa associata
 deve prendere il nome della variabile della porta cui e' connessa
*/
		    if((p_porte[j].tipo == INPUT_PORT_A ||
		        p_porte[j].tipo == INPUT_PORT_D) &&
			p_porte[j].connessa)
		    	{
/*
 compone il nome della variabile presente sulla porta connessa
*/
			strncpy(rec.pr_var[k],p_porte[j].label_sost,2);
	  		rec.pr_var[k][2]='\0';
			strcat(rec.pr_var[k],p_porte[j].icona_conn);
/*
 copia il nome (XXYY) della icona collegata
*/
			strcpy(rec.pr_idvin[k],p_porte[j].icona_conn);
			rec.pr_v_fl[k]=1; /* setta il flag di variabile connessa */
			trovata=True;
			break;
			}
		    }
		}
	    if(!trovata) rec.pr_v_fl[k]=0;
            strncpy(tag_tipo,&stringa[OFF_INI_TIPO+2],LUN_TIPO-4);
            tag_tipo[LUN_TIPO-4] = '\0';
            strcpy(rec.pr_type[k],tag_tipo);
            punt += MAX_L_TAGS;
	    }
       OlNumTipiVar(buffer,&rec.pr_nuvin,&rec.pr_nuvot,&rec.pr_nuvdt);

       /*printf("rec.pr_nuvin=%d rec.pr_nuvot=%d rec.pr_nuvdt=%d\n",
              rec.pr_nuvin,rec.pr_nuvot,rec.pr_nuvdt);*/
       if(buffer!=NULL)
	{
         XtFree(buffer);
	buffer = NULL;
	}

/* legge dalla form l'elenco dei dati */
       buffer = (char *) OlGetTags(form,1);
       punt = buffer;

/* ricava il numero dei dati */
       memcpy(&num_dat,punt,sizeof(int));
       punt += sizeof(int);

        for(k=0;k<num_dat;k++)
          {

/*        Leggo le tag dei dati */

          strcpy(rec.var_assoc[k],punt);

/*        Leggo il valore corrispondente alla tag */ 

          /*fprintf(stderr,"   EstrTagVal per dato %s\n",rec.var_assoc[k]);*/
          appoggio = EstrTagVal(form,OL_FORM_INPUT_DATA,rec.var_assoc[k],
                                strlen(rec.var_assoc[k]), str_input_value);
          if( appoggio!=NULL )
	      {
              rec.val_assoc[k] = atof(appoggio); /* Lo metto float su file */
              XtFree(appoggio);
	      }
          else
	      {
              fprintf(stderr,
                      "compila_reg: EstrTagVal ritorna valore dato errato\n");
              MessaggiErrore(8, 5, messaggio);
              goto FINE;
	      }

          punt += MAX_L_TAGS;
          }
        rec.pr_nuvto = num_var;
        rec.pr_ndat = num_dat;
        if(buffer!=NULL)
	   {
           XtFree(buffer);
	   buffer = NULL;
	   }


/* EstrDescr restituisce le descrizioni delle variabili */
       buffer = EstrDescr(form,0,str_input_value);
       punt = buffer;
       memcpy(&num_var,punt,sizeof(int));
       punt += sizeof(int);
       for(k=0;k<num_var;k++)
          {
          /*printf("PUNTV=%s\n",punt);*/
          strcpy(rec.pr_vds[k],punt);
          punt += MAX_L_DESCR;
          }
       if(buffer!=NULL)
	   {
           XtFree(buffer);
	   buffer = NULL;
	   }

      XtFree((char *)p_porte);

/* EstrDescr restituisce le descrizioni dei dati */
       buffer = EstrDescr(form,1,str_input_value);
       punt = buffer;
       memcpy(&num_dat,punt,sizeof(int));
       punt += sizeof(int);
       for(k=0;k<num_dat;k++)
          {
          /*printf("PUNTD=%s\n",punt);*/
          strcpy(rec.des_assoc[k],punt);
          punt += MAX_L_DESCR;
          }
       if(buffer!=NULL)
	   {
           XtFree(buffer);
	   buffer = NULL;
	   }

/* Assegno per ora in modo statico i flags delle variabili per i macromoduli */
/*
        for(k=0;k<num_var;k++)
          {
          if ( strstr(rec.pr_vds[k],"USCITA OR")!=NULL )
              {
              printf("Assegno I alla variabile=%s\n",rec.pr_var[k]);
              strcpy(rec.flag_est_var[k],"I");
              }
          else
              strcpy(rec.flag_est_var[k],"E");
          }
        for(k=0;k<num_dat;k++)
          strcpy(rec.flag_est_dat[k],"E");
*/
/*      Aggiorno il numero totale di variabili */
        nmax_var += rec.pr_nuvto;

/****
 FINE PREPARAZIONE DEL RECORD
*****/
/********************
 3.SCRITTURA DEL RECORD SU FILE :
*******************/
      fwrite(&rec,sizeof(TOP_ICON),1,fp);
      }
   }
fclose(fp);
XtFree((char *)children);
/* Libero la struttura con la lista dei moduletti */
if ( ls_moduletti!=NULL )
   free(ls_moduletti);
if ( ls_schemireg!=NULL )
   free(ls_schemireg);
if ( ls_macromod!=NULL )
   free(ls_macromod);


/*   Controllo dell'esistenza di tutti gli eseguibili necessari */
     if( (ret=controllo_exe(app))!=0 )
         {
         sprintf(messaggio,
                 "Manca l'eseguibile <%s> in $LEGOMMI_BIN\n",app);
         goto FINE;
         }

/********
 Attivazione dei programmi di elaborazione dei dati topologici
 e non relativi allo schema
********/
/*
 1. Genera il file di descrizione della topologia del modello 
    calcolando la sequenza corretta di gestione dei singoli 
    blocchetti
    	in ingresso:
			nome dello schema (minuscolo)
			nome directory di lavoro
			nome file degli errori
        in uscita:
			file <schema.top>
    L'attivita' e' gestita dal programma:
		gen_topologia
*/
   strcpy(comando,"gen_topologia");
   strcat(comando," ");
   strcat(comando,schema_min);
   strcat(comando," ");
   strcat(comando,XlGetenv(ENVPAG));
   strcat(comando," ");
   strcat(comando,work_pag);
   strcat(comando," ");
   strcat(comando,nome_file_err);
   ret_sy = system (comando);
#if defined UNIX
   ret = W_EXITSTATUS(ret_sy);
#else
   ret = ret_sy;
#endif
   if( ret!=0 )
      {
      MessaggiErrore(1,ret,messaggio);
      goto FINE;
      }

/*
 2.   Creazione del fortran dello schema e delle informazioni tipo
      file f01. UNIX e VMS usano due eseguibili diversi.
      Se si tratta di un macromodulo viene generato il codice tramite
      l'eseguibile gen_macromod.
             in ingresso (come nomi logici in VMS, parametri in UNIX):	
                        nome dello schema
                        directory di lavoro
             in uscita:
                        file <schema.f>
	                file <schema_01.dat>
		        file <schema.dat>
*/
   strcpy(tipo_codice,"gen_sche");
   strcpy(tipo_lista,lista_schemi);

   strcpy(comando,tipo_codice);
#if defined UNIX
   strcat(comando," ");
   strcat(comando,schema_min);
   strcat(comando," ");
   strcat(comando,work_pag);
#endif
#if defined VMS
   XlPutenv("SCHEMA_REG",schema_min);
   XlPutenv("PATH_U_CC",work_pag);
#endif
   ret_sy = system (comando);
#if defined UNIX
   ret = W_EXITSTATUS(ret_sy);
#else
   ret = ret_sy;
#endif
   if( ret!=0 )
     {
     MessaggiErrore(2+macrobl,ret,messaggio);
     goto FINE;
     }


#if defined UNIX
/* 
 3.  Gestione del file lista_schemi.dat o di lista_macro.dat.
     Vengono esclusi gli schemi non in uso, in VMS e' gestito
     da gen_stazionario.
*/
   if (PagGetInUse(pag)==True)
      {
      in_use = 1;
      ret = gest_lista_schemi(work_pag,path_pwd,pag->nomepag,desc_pagina,
                              pag,nome_file_err,macrobl);
      if (ret==1)
         {
         sprintf(messaggio,"File %s aggiornato",tipo_lista);
         if ( compile_all!=1 )
            printf("%s\n",messaggio);
         else
            inserimenti_in_lista = 1;
         }
      }
   else
      in_use = 0;
#endif



/*
 4.  Creo la libreria degli schemi 
     (escludo schemi non in uso e macroblocchi)
*/
   chdir(work_pag);

#if defined UNIX
   sprintf(comando,"%s.o",schema_min);
   if ( !XlPutenv("LIBRERIA","schemi_reg.a") )
        printf("Fallita la Putenv di NOME_LIB\n");
   if ( !XlPutenv("LISTA",comando) )
        printf("Fallita la Putenv di LISTA\n");
/*
   printf("in_use = %d\n",pag->in_use);
*/
   if ( PagGetInUse(pag)==True )
         strcpy(comando,"make -f $LEGO_BIN/makelibr");
   else
         strcpy(comando,"make -f $LEGO_BIN/makelibr_pgnouse");
   printf("\t\t\t comando=%s\n",comando);
   ret_sy = system (comando);
   ret = W_EXITSTATUS(ret_sy);
   if( ret != 0 )
         {
         sprintf(messaggio,"makelibr:terminata non correttamente (%d)",
                 ret);
         goto FINE;
         }
#endif

/*
 5. Richiama l'eseguibile gen_f14 che  genera il file f14.dat solo
    se non si tratta di un macromodulo.
	in ingresso:
                     schema
		     pathname della libut_reg
		     directory di lavoro
		     file degli errori
	in uscita:   
                     file <f14.dat>
                     file <lg3_reg.f>
*/
   strcpy(comando,"gen_f14");
   strcat(comando," ");
   strcat(comando,schema_min);
   strcat(comando," ");
   strcat(comando,nome_libut_reg);
   strcat(comando," ");
   strcat(comando,work_pag);
   strcat(comando," ");
   strcat(comando,nome_file_err);
   ret_sy = system (comando);
#if defined UNIX
   ret = W_EXITSTATUS(ret_sy);
#else
   ret = ret_sy;
#endif
   if( ret!=0 )
     {
     MessaggiErrore(4,ret,messaggio);
     goto FINE;
     }



/*
 6. In UNIX richiama l'eseguibile gen_stazionario mentre in VMS lancia
    un file .com. 
    Viene attivato stando nella directory di lavoro.
	in ingresso:  
                        schema (minuscolo)
		        pathname della libut_reg (per accedere a libreg.a)
                        directory di lavoro
			nome file degli errori (senza estensione reg)
                        flag pagina in uso
			lg3_reg.dat (usato internamente)
                        f14.dat (aperto internamente da lg3reg)
	in uscita:      
                        schema_14.dat
			schema_04.dat
ATTENZIONE: per gestire la non compilazione in VMS va passato il flag indicante
            che la pagina e' o meno in uso
        Operazioni eseguite :
           a) compila lg3_reg.f (locale)
           b) crea l'eseguibile lg3reg
           c) trasferimento eseguibile nel direttorio di lavoro
           d) esecuzione lg3reg calcolando in tal modo lo stazionario.
           e) eliminazione di lg3reg e di lg3reg.* 
*/
   strcpy(comando,"gen_stazionario");
   strcat(comando," ");
   strcat(comando,schema_min);
   strcat(comando," ");
   strcat(comando,nome_libut_reg);
   strcat(comando," ");
   strcat(comando,work_pag);
   strcat(comando," ");
   strcat(comando,nome_file_err);
   strcat(comando," ");
   sprintf(&comando[strlen(comando)],"%d",in_use);
   ret_sy = system (comando);
#if defined UNIX
   ret = W_EXITSTATUS(ret_sy);
#else
   ret = ret_sy;
#endif
   if( ret!=0 )
     {
     MessaggiErrore(5,ret,messaggio);
     goto FINE;
     }

/*
    Si riposiziona NEL DIRETTORIO CORRENTE
*/
   chdir(path_pwd);

/* 
   Lettura del file14 schema e caricamento dei valori di regime 
*/
   ret =  rd_f14(nmax_var,nome_libut_reg,work_pag,schema_min,
                 &nval,nome_file_err);
   if(ret>=0)
      {
      ret = setta_valori_14(nval,nome_file_err,pag);
      XtFree((char *)varsch);
      if( ret<0 )
         {
         strcpy(messaggio,"Fallita acquisizione valori di regime");
         goto FINE;
         }
      }
   else
      {
      strcpy(messaggio,"Fallita lettura f14 di regime ");
      goto FINE;
      }

/* Gestione messaggi */
   if( !compile_all )
     {
     strcpy(messaggio,"Compilation correct of the pag ");
     strcat(messaggio,pag->nomepag);
     if( in_use==0 )
        strcat(messaggio," which is not in use");
     strcat(messaggio,"\n");
     printf("%s\n",messaggio);
     }

/* Se la pagina non e' in uso il file schema.f si rinomina schema.nf */
   chdir(work_pag);
   if ( PagGetInUse(pag)==False )
      {
      strcpy(from,schema_min);
      strcat(from,".f");
      strcpy(to,schema_min);
      strcat(to,".nf");
      XlRename(from,to);
      strcpy(from,schema_min);
      strcat(from,".o");
      XlRemove(from);
      }
   chdir(path_pwd);

   salva_data(pag);

   return(inserimenti_in_lista);

FINE:
   chdir(path_pwd);
/* Messaggio errore e chiusura del file */
   no_error = 0;
   printf("%s\n",messaggio);
   XlOpenFileErrComp(nome_file_err);
   XlErrComp(NULL,"Write .reg file ","ERRORE\n ",messaggio);
   XlErrComp(NULL,"Write .reg file - Termine-","ERRORE\n ",
             "Fine non corretta compilazione pagina");
   XlSetFileErrComp(NULL);
   return(-1); 
}





/*
    Setto la data della creazione della regolazione dopo
    una compilazione:
    Se di regolazione:
       - fa un touch dei file finali di questa compilazione
         ./proc/nomepag_04.dat  ./proc/nomepag_14.dat
         per evitarne una successiva nuova ricompilazione.
       - fa un touch del file ./nomepag.rtf  nel caso che
         come sinottico sia gia' stata compilata. 
*/
int salva_data(PAGINA *pag)
{
int g,m,a,o,min,s;
char comando[FILENAME_MAX*2];
char file1[FILENAME_MAX];
char file2[FILENAME_MAX];
char firtf[FILENAME_MAX];
int ret1=-1,ret2=-2;


   data(&g,&m,&a);
   ora(&o,&min,&s);
/*
 Modifica con comando sed la stringa che descrive la data di compilazione
 dello schema di regolazione
*/
   sprintf(pag->regolationDate,"%d\\/%d\\/%d %d:%d:%d",g,m,a,o,min,s);
   sprintf(comando,"sed \"s/\\*regolationDate\:.*$/\\*regolationDate\: %s/\" %s.pag > %s.sed",
		pag->regolationDate,pag->nomepag,pag->nomepag);

   system(comando);
#ifdef SALVAPAG
   sprintf(comando,"mv %s.sed %s.pag",pag->nomepag,pag->nomepag);
   system(comando);
#endif

/* 
   Modifico la data dei files prodotti dalla compilazione
   allo scopo di averli  piu' recenti del file pagina.pag
   durante il confronto fatto in sede di  compilazione di
   tutte le pagine con i files   ./proc/pagina_14.dat   e 
   ./proc/pagina_04.dat.
*/

/* Ricavo i pathname dei file */
   FileNameInproc(pag->nomepag,"_04.dat",file1);
   FileNameInproc(pag->nomepag,"_14.dat",file2);

/* Modifica data */
   ret1=tempo_file_touch(file1);
   ret2=tempo_file_touch(file2);
   if( (ret1!=0)||(ret2!=0) ) {
       printf("Fallito touch per <%s><%s>\n",file1,file2);
   }

   return(1);
}



/*  ***** FUNZIONI GESTIONE LETTURA F14 E PROPAGAZIONE VALORI  ***** */
/*
1) rd_f14
2) setta_valori_14
3) PropagaValConn
4) PropagaValoreFraPorte
*/
/*
   Legge il file f14 di regime (schema_14.dat) e assegna i
   i valori alle icone.
*/
int rd_f14 (int num, char *lib_path, char *fpath, char *nomepag, 
            int *valori_letti, char *filerr)
{
FILE *f14;
int  cont_ast = 0;
int  nv = 0;
int  diff;
int  i = 0;
char schema[MAXCHAR_PAGNAME];
char appogg[MAXCHAR_PAGNAME];
char riga[256];
char messaggio[FILENAME_MAX];
char fil14[FILENAME_MAX];
char file_da[FILENAME_MAX];
char file_a[FILENAME_MAX];

/* Allocazione struttura ed apertura file */
   strcpy (schema, nomepag);
   diff = 4 - strlen(schema);
   if( diff>0 )
      for(i=0;i<diff;i++)
         strcat(schema,"_");

   strcpy (fil14,fpath);
#if defined UNIX
   strcat(fil14,"/");
#endif
   strcpy(appogg,schema);
   tominus(appogg);
   strcat(fil14,appogg);
   strcat(fil14,"_14.dat");


   varsch = (VARS*)XtCalloc(num,sizeof(VARS));
   f14 = fopen (fil14, "r");
   if (!f14)
      {
      XtFree((char *)varsch);
      strcpy(messaggio,"Errore in apertura del file ");
      strcat(messaggio,fil14);
      XlOpenFileErrComp(filerr);
      XlErrComp(NULL,"Lettura schema_14.dat","ERRORE\n ",messaggio);
      XlSetFileErrComp(NULL);
      return(-1);
      }

/* Lettura schema_14.dat */
   while( getpul (riga, 255, f14) )
      {
      /*printf("(%d)riga=%s\n",strlen(riga),riga);*/
      if (riga[0] == '*')
         {
         if (++cont_ast >= 6)
            break;
         }
      else
         {
         riga[24] = '\0';
         copy_n_car (varsch[nv].nome, riga+4, 8);
         for(i=0;(varsch[nv].nome[i]!=' ')&&(i<strlen(varsch[nv].nome));i++);
         varsch[nv].nome[i] = '\0';
         strcpy(varsch[nv].fval,riga+14);
         /*printf("-------->%s=%s|\n",varsch[nv].nome,varsch[nv].fval);*/
         nv++;
         }
      }
   fclose (f14);

   qsort (varsch, nv, sizeof(VARS), (__compar_fn_t)strcmp);
   (*valori_letti) = nv;

   return(1);
}



/*
     Carica i valori letti da file nello stringone e setta poi
     lo stringone modificato tra le risorse dell'oggetto.
*/
int setta_valori_14 (int nv14, char *filerr, PAGINA *pag)
{
OlFormObject form;
Cardinal num_children;
WidgetList children;
Arg arg[10];
int ret;
int lun;
int i,k,h;
int narg=0;
int num_var=0;
int inserimenti=0;
int propagazioni=0;
int non_propagazioni=0;
char *str_tag,*str_modul;
char *str_inp_value;
char *stringone;
char *buffer=NULL;
char *punt;
char var_f14[MAX_L_TAGS];
char tag[MAX_L_TAGS];
char valore[MAX_L_TAGS];
char messaggio[FILENAME_MAX]={'\0'};
char app1[FILENAME_MAX]={'\0'};
char app2[FILENAME_MAX]={'\0'};



/*  Ciclo su tutti i child */
    get_child(pag->drawing,&children,&num_children);

/*  Ciclo su tutte le icone di regolazione */
    for(i=0;i<num_children;i++)
       {
       if( XlIsIconReg(children[i]) )
          {
/*        Nome della icona */
          narg=0;
          XtSetArg(arg[narg],XlNtagName,&str_tag);narg++;
          XtSetArg(arg[narg],XlNmodulName,&str_modul);narg++;
          XtSetArg(arg[narg],XlNobjectForm,&form);narg++;
          XtSetArg(arg[narg],XlNinputValue,&str_inp_value);narg++;
          XtGetValues(children[i],arg,narg);

          stringone=XtNewString(str_inp_value);

          /*printf("====================\nstringone=%s=\n",stringone);*/

          if( strcmp(stringone,"no value")!=0 )
             {
/*           Salto le icone di interfaccia */
             if( XlIsInterfaceIconReg(children[i])==True ) 
                {
                /*printf(" icona di interfaccia\n");*/
                continue;
                }
/*           Recupera le tags della form e il loro numero */
             if( buffer!=NULL)
		{
                free(buffer);
		buffer = NULL;
		}
             buffer = (char*)OlGetTags(form,0);
             punt = buffer;
   	     memcpy(&num_var,punt,sizeof(int));
             punt += sizeof(int);
/*           Ciclo su tutte le variabili dell'icona */
             for(k=0;k<num_var;k++)
               {
               strcpy(tag,punt);
               punt += MAX_L_TAGS;
/*             Compongo il nome della variabile che potrebbe 
               essere scritta su f14.dat dello schema */
               strncpy(var_f14,tag,2);
               var_f14[2] = '\0';
               strcat(var_f14,str_tag);
               var_f14[strlen(var_f14)] = '\0';
/*
               Le variabili connesse hanno lo stesso nome della
               variabile cui sono connesse e non sono sostituite
               nel caricamento dei valori da f14 ma successivamente 
               propagate 
*/
               for(h=0;h<nv14;h++)
                  { 
/*
 confronta solo i primi 6 caratteri del nome perche' gli ultimi 2
 vengono utilizzati per memorizzare l'id della task di regolazione
*/
                  if( !strncmp(var_f14,varsch[h].nome,6) )
                       {
                       /*printf("VAR=%s| (%s)\n",var_f14,varsch[h].fval);*/
                       strcpy(valore,varsch[h].fval);
                       break;
                       }
                  }
               if(h<nv14)
                  {
/*                printf("h=%d nv14=%d\n",h,nv14); */
                  char *stringone_input;
                  stringone_input = stringone;  //GUAG2025 copio il puntatore per liberare memoria dopo
                  ret = InsTagVal(form,OL_FORM_INPUT_VALUE, tag, 2, &stringone, valore);
                  // infatti stringone dopo la chiamata a InsTagVal potrebbe essere duplicato, quindi lo libero dopo
                  if(ret == 2 && stringone_input != NULL) XtFree(stringone_input);
                  if(ret<0)
                     {
                     strcpy(messaggio,
                           "setta_valori_14: non ritrovata nella form la tag ");
                     strcat(messaggio,tag);
                     XlOpenFileErrComp(filerr);
                     XlErrComp(NULL,"Acquisizione valori di regime",
                               "ERRORE\n ",messaggio);
                     XlSetFileErrComp(NULL);
                     if(stringone!=NULL)
                        XtFree(stringone);
                     return(-1);
                     }
                  inserimenti++;
/*                Ad inserimento effettuato si setta la risorsa */
                  narg=0;
                  XtSetArg(arg[narg],XlNinputValue,stringone);narg++;
                  XtSetValues(children[i],arg,narg);


/*                Si setta il data base */
                  /* printf("Si setta il data base\n"); */
                  XlSetResourceByWidget(&pag->db,children[i],XlNinputValue,
                                        stringone);


/*                Propaga il valore alla eventuale porta connessa */
                  ret = PropagaValConn(children[i],var_f14,tag,filerr,pag);
                  if (ret==1)
                     {
                     propagazioni++;
                     }
                  if (ret==-2)
                     {
                     strcpy(messaggio,"Variabile connessa non propagata");
                     printf("%s\n",messaggio);
                     non_propagazioni++;
                     strcpy(messaggio,"setta_valori:variabili non propagate  ");
                     sprintf(app1,"%d\00",non_propagazioni);
                     strcat(messaggio,app1);
                     XlOpenFileErrComp(filerr);
                     XlErrComp(NULL,"Acquisizione valori di regime",
                               "ATTENZIONE\n ",messaggio);
                     XlSetFileErrComp(NULL);
                     }
                  }
               }
		if(buffer!=NULL)
			{
             		XtFree(buffer);
			buffer = NULL;
			}
             }
          if(stringone!=NULL)
             XtFree(stringone);
          }
       }  /* Fine ciclo sui figli */
       if( inserimenti!=nv14 )
           {
           fprintf(stderr,"ATTENZIONE setta_valori_14: letti da schema_14 %d valori ma riconosciuti ed inseriti nello schema %d\n",nv14,inserimenti);
           strcpy(app1,"setta_valori_14: letti dal file schema_14.dat");
           strcpy(app2,"valori ma inseriti nello schema");
           sprintf(messaggio,"%s %d %s %d",app1,nv14,app2,inserimenti);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"Acquisizione valori di regime",
                     "ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);
           return(-1);
           }
       printf("Propagazioni=%d\n",propagazioni);
       return(1);
}

/*
    Data un icona di regolazione, il nome di una variabile associata 
    ad una sua porta verifica se la porta e' connessa ed in tal caso
    propaga questo valore alla variabile associata alla porta cui e'
    connessa.
*/   
int PropagaValConn(Widget wid_in, char *nome, char *tag_form, char *file_err,PAGINA *pagina)
{
int j,h;
int tipo;
int narg;
int ret;
Arg arg[10];
WidgetList childcomp;
Widget wid_icon_conn;
Widget wid_porta_conn;
Cardinal num_childcomp;
char *nome_porta;
char *porta_conn;
char *str_modul;
char nome_da_cercare[10];
int num_porte;
char messaggio[100];


/*   Un ingresso non viene propagato */
     if( !strcmp(&tag_form[4],DEF_IN) )
         return(-1);

/*   Ciclo su tutte le porte figlie della icon_reg */
     get_child(wid_in,&childcomp,&num_childcomp);
     for(j=0;j<num_childcomp;j++)
         {
         if(XlIsPort(childcomp[j]))
            {
            narg=0;
            XtSetArg(arg[narg],XlNportNameConnect,&porta_conn);narg++;
            XtSetArg(arg[narg],XlNportName,&nome_porta);narg++;
                    XtSetArg(arg[narg],XlNmodulName,&str_modul);narg++;
            XtSetArg(arg[narg],XlNtipoPort,&tipo);narg++;
            XtGetValues(childcomp[j],arg,narg);
/* 
            Se la porta dell'icona e' quella della variabile ed
            e' connessa avviene la propagazione ricavando l'oggetto
            dell'icona connessa e l'oggetto porta collegata di
            questa icona.
*/
            if( (strncmp(nome_porta, nome,2)==0) 
                &&  (strlen(porta_conn)!=0)       )
                {
                wid_icon_conn=get_wid_icon_conn(XtParent(wid_in),
                                                porta_conn);
/*              Caso particolare : connessione con icona di interfaccia */
                if( XlIsInterfaceIconReg(wid_icon_conn)==True )
                    {
                    printf("Non propago la variabile <%s> ad una porta di interfaccia\n",nome);
                    return(-1);
                    }
                else
                    {
                    wid_porta_conn=get_wid_port(wid_icon_conn, porta_conn);
                    ret = PropagaValoreFraPorte(childcomp[j],wid_porta_conn,
                                                file_err,pagina);
                    if( ret<0 )
                       {
                       strcpy(messaggio,
                              "PropagaValConn: non propago la variabile ");
                       strcat(messaggio,nome);
                       XlOpenFileErrComp(file_err);
                       XlErrComp(NULL,"Acquisizione valori di regime",
                                 "ATTENZIONE\n ",messaggio);
                       XlSetFileErrComp(NULL);
                       }
                    return(ret);
                    }
                }
            }
         }
      return(-1);
}


/*
   Propaga il valore della stringa di input da una 
   porta sorgente ad una destinataria.
*/
int PropagaValoreFraPorte(Widget sorg, Widget dest, char *file_err, PAGINA *pag)
{
int val1,val2;
int narg;
int ret;
Arg arg[8];
OlFormObject form;
char *nome_var;
char *str_modul;
char * valore;
char *value, *nuovo_value;
char messaggio[100];

        /*
           PORTA SORGENTE
        */
/*      Ricavo il nome della variabile associata alla porta sorgente */
        narg=0;
        XtSetArg(arg[narg],XlNportName,&nome_var);narg++;
        XtGetValues(sorg,arg,narg);

/*      Ricavo form e stringone dell'icona sorgente */
        narg=0;
        XtSetArg(arg[narg],XlNobjectForm,&form);narg++;
        XtSetArg(arg[narg],XlNinputValue,&value);narg++;
        XtGetValues(XtParent(sorg),arg,narg);

/*      Estraggo il valore della variabile da propagare */
        valore = EstrTagVal(form,OL_FORM_INPUT_VALUE,nome_var,2,value);
	if(valore==NULL)
	      {
              fprintf(stderr,
                  "PropagaValoreFraPorte: EstrTagVal ritorna valore errato\n");
              MessaggiErrore(9, 5, messaggio);
              XlOpenFileErrComp(file_err);
              XlErrComp(NULL,"Acquisizione valori di regime",
                        "ATTENZIONE\n ",messaggio);
              XlSetFileErrComp(NULL);
              return(-2);
	      }
        
        /*
           PORTA DESTINATARIA
        */
/*      Ricavo il nome della variabile associata alla porta destinataria */
        narg=0;
        XtSetArg(arg[narg],XlNportName,&nome_var);narg++;
        XtGetValues(dest,arg,narg);

/*      Ricavo form e stringone dell'icona destinataria */
        narg=0;
        XtSetArg(arg[narg],XlNobjectForm,&form);narg++;
        XtSetArg(arg[narg],XlNinputValue,&value);narg++;
	XtSetArg(arg[narg],XlNmodulName,&str_modul);narg++;
        XtGetValues(XtParent(dest),arg,narg);
/*
 Duplica il valore letto poiche' la funzione InsTagValue ne
 modifica il valore
*/
	nuovo_value=XtNewString(value);

/*      Inserisco il valore della variabile da propagare */
        char *nuovo_value_input;
        nuovo_value_input = nuovo_value; //GUAG2025 copio il puntatore per liberare memoria dopo
        ret = InsTagVal(form,OL_FORM_INPUT_VALUE,nome_var,2,&nuovo_value,valore);
        if(valore!=NULL) XtFree(valore);     
        if(ret == 2 && nuovo_value_input != NULL) XtFree(nuovo_value_input);
	     if (ret<0)
           {
           /*
           fprintf(stderr,
              "PropagaValoreFraPorte: non esiste la variabile destinataria \n");
           */
           strcpy(messaggio,
                "PropagaValoreFraPorte: non esiste la variabile destinataria");
           XlOpenFileErrComp(file_err);
           XlErrComp(NULL,"Acquisizione valori di regime",
                     "ATTENZIONE\n ",messaggio);
           XlSetFileErrComp(NULL);
           return(-2);
           }

/*      Setto risorse e data base modificato */
        narg=0;
        XtSetArg(arg[narg],XlNinputValue,nuovo_value);narg++;
        XtSetValues(XtParent(dest),arg,narg);
        /*printf("PropagaValoreFraPorte : XlSetResourceByWidget\n");*/
        XlSetResourceByWidget(&pag->db,XtParent(dest),XlNinputValue,nuovo_value);
/*
 Libera il valore dopo che e' stato assegnato all'oggetto
*/
	XtFree(nuovo_value);

        return(1);
}




/*  ***** FUNZIONI DI GESTIONE STRINGHE ***** */
/*
1) getpul
*/
/*
   Legge una riga di testo da un file ed elimina gli
   eventuali spazi e/o caratteri di controllo alla fine 
   della riga letta.
*/
char *getpul (char *str, int n, FILE *fp)
{
char *ptr;
if (! fgets (str, n, fp))
   return NULL;
ptr = str + strlen(str) - 1;
for ( ; ptr >= str && (unsigned)*ptr <= 32; )
   *(ptr--) = '\0';
return (str);
}



/*
  Non accetta nomi di direttory suoperiori agli 8 caratteri
*/
int estrai_nome(char *nome_path, char *nome_out)
{
int i;
char appoggio[200]={'\0'};

   strcpy(nome_out,"");
   /*printf("NOMEIN=%s\n",nome_path);*/
   for(i=strlen(nome_path);i>0;i--)
      {
#if defined UNIX
      if( nome_path[i]=='/' )
#endif
#if defined VMS
      if( nome_path[i]=='.' )
#endif
          {
          strcpy(appoggio,&nome_path[i+1]);
          if(strlen(appoggio)>9)
             {
             strncpy(nome_out,appoggio,8);
             nome_out[8]='\0';
             }
          else
             strcpy(nome_out,appoggio);
#if defined VMS
          nome_out[strlen(nome_out)-1]='\0';
#endif
#if defined UNIX
          nome_out[strlen(nome_out)]='\0';
#endif
          break;
          }
     }
   if(i==0)
     {
     printf("Estrazione nome task scorretta ");
     return(-1);
     }
   return(1);
}



/*  ***** FUNZIONI GESTIONE LISTA SCHEMI ***** 
1) int gest_lista_schemi
2) eli_schema
3) ins_schema
4) schema_presente
5) rd_lst_schemi
6) wr_lst_schemi
7) compara_lst
*/


/*
    Gestisce l'aggiornamento del file lista_schemi.dat o lista_moduli.dat
    al termine della fase di compilazione della pagina.
    Se il flag macro vale 1 gestiscle la lista_macro.dat.
*/
int gest_lista_schemi(char *dir_work, char *dir_corr, char *nome_schema, 
                      char *desc_schema, PAGINA *pag, char *filerr, int macro)
{
char file_lista[20];
LST_SCHM *lista;
int posiz,ii;
int numero_schemi;
char nome_4[5];
FILE *ff;
char messaggio[200];
char nome_dello_schema[5];
char descrizione[300];
int *selected,selcount,ind_pagina;


   strcpy(nome_dello_schema,nome_schema);
   if (strlen(nome_dello_schema)>4 )
      nome_dello_schema[4] = '\0';
    
/* Completo il nome se piu' corto di 4 caratteri */
   nome_underscore(nome_dello_schema,nome_4,4);

/* Posizionamento direttorio di lavoro */
   chdir(dir_work);

   strcpy(file_lista,lista_schemi);

/* Presenza della lista o eventuale creazione */
   if( !(ff=fopen(file_lista,"r")) )
       {
       XlOpenFileErrComp(filerr);
       XlErrComp(NULL,"gest_lista_schemi","WARNING",messaggio);
       sprintf(messaggio,
              "%s does not exist, will create empty to insert this scheme",
              file_lista);
       XlSetFileErrComp(NULL);
       ff=fopen(file_lista,"w");
       fclose(ff);
       }

/* Lettura della lista_schemi.dat e caricamento struttura LST_SCHM */
   numero_schemi= rd_lst_schemi(&lista,filerr,file_lista);
   if (numero_schemi<0)
     {
     if (lista != NULL)
        {
        free(lista);
	lista=NULL;
        }
     chdir(dir_corr);
     return(-1);
     }

/* Test schema gia' inserito */
   posiz = schema_presente(lista,nome_4,numero_schemi);
   if (posiz != -1 )
     {
     if (lista != NULL)
        {
        free(lista);
	lista=NULL;
        }
     chdir(dir_corr);
     return(-1);
     }

/* Elimino i blank iniziali da desc */
   strcpy(descrizione,desc_schema);
   ii=0;
   while( descrizione[ii]==' ' && descrizione[ii]!=NULL )
         ii++;

/* Inserimento schema nella lista */
   ins_schema(lista,numero_schemi,nome_4,&desc_schema[ii]);
   numero_schemi++;

/* Riscrittura completa del file */
   wr_lst_schemi(lista,numero_schemi,file_lista);

/* Liberazione strutture */
   if (lista != NULL)
      {
      free(lista);
      lista=NULL;
      }

/* Posizionamento direttorio di lavoro */
   chdir(dir_corr);

   return(1);
}



/*
    Elimina uno schema identificato dal suo nome dalla lista
    Ritorni:
            -1 numero scorretto di schemi in lista
            -2 manca lo schema da eliminare nella lista
             1 eliminazione corretta
*/
int eli_schema(LST_SCHM *lst, char* name, int ntot, char *filerr)
{
char msg[100]={'\0'};
int pos;

    if (ntot<=0)
      {
      fprintf(stderr,"Numero schemi <%d> scorretto\n",ntot);
      sprintf(msg,"Numero schemi <%d> scorretto\n",ntot);
      XlOpenFileErrComp(filerr);
      XlErrComp(NULL,"eli_schema","ERRORE\n ",msg);
      XlSetFileErrComp(NULL);
      return(-1);
      }

/*  Controllo presenza schema nella lista, se pos=-1 e' assente */
    pos = schema_presente(lst,name,ntot);

/*  Eliminazione, il flag again=-1 impedira' la scrittura */
    if( (pos !=-1)&&(pos<ntot) )
       {
       lst[pos].again = -1;
       return(1);
       }
    else
       return(-2);
}



/*
    Inserisce uno schema nella lista cercando la posizione
    libera. E' quella con 1 nel campo again della struttura
    che contiene nschemi piu' uno.
    Ritorni:
            1 Non viene inserito perche' gia' presente
            0 Inserito nella struttura
*/
int ins_schema(LST_SCHM *lst, int nsche, char* name, char* descr)
{
int i;
int pos;
char app_name[100];


/*  Controllo presenza, -1 e' assente */
    pos = schema_presente(lst,name,nsche);
    if(pos!=-1)
      {
      return(1);
      }

    strcpy(app_name,name);
    for(i=0;i<=nsche;i++)
       if (lst[i].again == 1)
          {
          pos = i;
          break;
          }
    tomaius(app_name);
    strcpy(lst[pos].name,app_name);
    strcpy(lst[pos].descr,descr);
    lst[pos].again = 0;
    return(0);
}




/*
    Cerca uno schema in una lista restituendone la
    posizione oppure -1 se e' assente.
*/
int schema_presente(LST_SCHM *lst, char *nome, int nsche)
{
int ret;
int i;
char nome_maiuscolo[10];

    if( nsche<=0 )
       return(-1);
    strcpy(nome_maiuscolo,nome);
    tomaius(nome_maiuscolo);
    for(i=0;i<nsche;i++)
       if( !strcmp(lst[i].name,nome_maiuscolo) )
          {
          return(i);
          }

    return(-1);
}



/*
    Scrittura del file lista_schemi.dat (o lista_macro.dat) 
    con ordinamento alfabetico rispetto ai nomi (che sono maiuscoli) 
    degli schemi.
*/
int wr_lst_schemi(LST_SCHM *lst, int nsche, char *file_lst)
{
int i;
FILE *fp;

/*  Apertura file in scrittura */
    fp=fopen(file_lst,"w");

/*  Ordinamento */
    qsort(lst,nsche,sizeof(LST_SCHM),(__compar_fn_t)compara_lst);

/*  Scrittura */
    for(i=0;i<nsche;i++)
       if (lst[i].again==0)
          {
          fprintf(fp,"%s %s\n",lst[i].name,lst[i].descr);
          }

/*  Chiusura */
    fclose(fp);
}


/*
    Funzione di confronto personalizzata su un campo
    della struttura LST_SCHM usata per qsort nel caso 
    di ordinamento alfabetico sul campo nome della struttura.
*/
int compara_lst(LST_SCHM * uno, LST_SCHM * due)
{
int ret;

    ret=strcmp(uno->name,due->name);
    if ( ret==1 )
       return(1);
    else
       if (ret==0)
          return(0);
       else
          return(-1);
}




/*
   Legge il file lista_schemi.dat (o lista_macro.dat) e carica 
   l'elenco delle informazioni (nome e descrizione) nella struttura
   apposita riservando in coda un campo libero.
   Ritorna il numero degli schemi effettivamente presenti 
   oppure -1 se non esiste il file.
*/
int rd_lst_schemi(LST_SCHM **lista, char *filerr, char *file_lst)
{
int i;
int nsche;
int name_lun;
int dimensione;
int max_schemi;
char riga[256];
char messagg[256];
FILE *ff;
LST_SCHM *lst;

/*
    printf(" ATTENZIONE SONO IN rd_lst_schm (file %s)\n",file_lst);
*/
    if( !(ff=fopen(file_lst,"r")) )
       {
/*
       sprintf(messagg,"Non si puo' aprire il file %s",file_lst);
       XlOpenFileErrComp(filerr);
       XlErrComp(NULL,"rd_lst_schemi","ERRORE",messagg);
       XlSetFileErrComp(NULL);
*/
       return(-1);
       }

/*  Struttura dati schemi */
    lst = (LST_SCHM*)calloc(MAX_SHE_INLIST,sizeof(LST_SCHM));
    dimensione = MAX_SHE_INLIST*sizeof(LST_SCHM);
    max_schemi = MAX_SHE_INLIST;

/*  Lettura, riallocazioni, caricamento */
    i=0;
    while( fgets(riga,255,ff)!=NULL )
      {
/*    Eventuale riallocazione */
      if( i==max_schemi )
         {
         dimensione += AGG_SCHE*sizeof(LST_SCHM);
         max_schemi += AGG_SCHE;
         /*
         printf("\n\nRealloc (%s) dim=%d num=%d i=%d\n",
                 file_lst,dimensione,max_schemi,i);
         */
         lst = (LST_SCHM*)realloc(lst,max_schemi*sizeof(LST_SCHM));
         }
/*    Controllo sulla riga letta */
      while( (riga[strlen(riga)-1] == '\n') ||
             (riga[strlen(riga)-1] == ' ')     )
           {
           riga[strlen(riga)-1] = '\0';
           if (strlen(riga)==0)
              break;
           }
       if (strlen(riga)==0)
         continue;

/*
      Azzero i campi
*/
      memset(&lst[i],0,sizeof(LST_SCHM));

/*    Lettura del nome */
      name_lun = 4;
      strncpy(lst[i].name,riga,name_lun);
      lst[i].name[strlen(lst[i].name)] = '\0';

/*   
      Lettura della descrizione eliminando blank iniziali,
      finali e ritorni a capo
*/
      while( riga[name_lun]==' ' && riga[name_lun]!=NULL )
         name_lun++;
      if( name_lun<strlen(riga) )
        {
        strcpy(lst[i].descr,&riga[name_lun]);
        while( (lst[i].descr[strlen(lst[i].descr)-1] == '\n') ||
               (lst[i].descr[strlen(lst[i].descr)-1] == ' ')     )
           lst[i].descr[strlen(lst[i].descr)-1] = '\0';
        }
      i++;
      }
    nsche = i;
    if (nsche==0)
       {
       /* fprintf(stderr,"Non ci sono schemi nella lista\n"); */
       }

/*  
    Uscita: solo le informazioni sugli schemi esistenti
    con in piu' una posizione vuota 
*/
    (*lista) = (LST_SCHM*)calloc((nsche+1),sizeof(LST_SCHM));
    if (nsche>0)
       memcpy((*lista),lst,nsche*sizeof(LST_SCHM));
    (*lista)[nsche].again = 1;


/*  Chiusura file */
    if (lst != NULL)
       {
       free(lst);
       lst=NULL;
       }

    fclose(ff);

    return(nsche);
}




/*
    Cancella dalla lista lo schema trattato; rimuove anche i files
    dello schema presenti nel direttorio di lavoro; elimina il modulo 
    dalla libreria.
*/
void elimina_da_lista(char *nome_pag, int tipo, char *filerr)
{
LST_SCHM *lista;
int numero_schemi;
int posizione;
int i,lun,ret;
int tipo_lista;
int in_lista;
int pg_comp;
char nome_schema_4[5];
char path_cwd[FILENAME_MAX];
char path_lst[FILENAME_MAX];
char messaggio[FILENAME_MAX];
char comando[FILENAME_MAX];
char rmfile[FILENAME_MAX];
char nome_4[FILENAME_MAX];
char nome_schema[FILENAME_MAX];
char work[FILENAME_MAX];
char file_lista[FILENAME_MAX];
char lib_lista[FILENAME_MAX];
PAGINA *pag;
AN_OBJ *obj;




   if (nome_pag==NULL)
      return;

/* Cambio directory */
   getcwd(path_cwd,sizeof(path_cwd));
   if( XlGetenv(WORK_PAG)!=NULL )
     strcpy(work,XlGetenv(WORK_PAG));
   else
     {
     strcpy(work,"");
     strcpy(work,XlGetenv(ENVPAG));
     strcat(work,"/proc");
     }
   chdir(work);

   tipo_lista = trova_in_lista(nome_pag,filerr,0,0);
   if ( tipo_lista<0 )
      {
      chdir(path_cwd);
      return;
      }
   else if( tipo_lista==1 )
            {
            strcpy(file_lista,lista_schemi);
            strcpy(lib_lista,lib_schemi);
            }
        else
            {
            strcpy(file_lista,lista_macro);
            strcpy(lib_lista,lib_macro);
            }

/* Carica la pagina */
   pagina_init(nome_pag,&pag,TIPO_PAGINA);
   pagina_load_file(pag,TIPO_PAGINA);
   pagina_getres(pag,&obj);

   if(PagGetType(pag) != TYPE_REGOLAZIONE)
      {
      fprintf(stderr,"Non e' di tipo regolazione !!!\n");
      chdir(path_cwd);
      pagina_free(pag);
      return;
      }

/* Schema in uso e quindi in lista */
   if ( PagGetInUse(pag)==True )
      in_lista = 1;
   else
      if ( PagGetInUse(pag)==False )
          in_lista = 0;
      else
          {
          printf("PagGetInUse failed!!!\n");
/*
          XlOpenFileErrComp(filerr);
          XlErrComp(NULL,"elimina_da_lista","ERRORE\n ","PagGetInUse failed");
          XlSetFileErrComp(NULL);
*/
          chdir(path_cwd);
          pagina_free(pag);
          return;
          }

/* Nomi */
   strcpy(nome_schema,nome_pag);
   if( strlen(nome_schema)>4 )
      nome_schema[4] = '\0';
   nome_underscore(nome_schema,nome_4,4);

/*
   Se elimino una pagina mai compilata la mancanza di file non 
   deve dare errori.
   Una pagina compilata non manca del file schema.reg
*/
   tominus(nome_schema);
   strcpy(rmfile,nome_schema);
   strcat(rmfile,".reg");
#if defined VMS
	pg_comp=1;
#else
   if ( file_no_exe_filter(rmfile)==1 )
      pg_comp=1;
   else
      pg_comp=0;
#endif

/* Eliminazione dalla lista se il modulo e' in use ed esiste schema.reg */
   if ( (in_lista==1)&&(pg_comp==1) )
       {
       numero_schemi= rd_lst_schemi(&lista,filerr,file_lista);

/*     Conrolli sulla lista schemi */
       if ( numero_schemi<=0 )
          {
          sprintf(messaggio,"Error during deleting:\n%s ",file_lista);
          if ( numero_schemi==0 )
            strcat(messaggio,"is empty");
          else
             if ( numero_schemi==-1 )
               strcat(messaggio,"is missing");
             else
               strcat(messaggio,"Bad schemes number.");
          printf("%s\n",messaggio);
/*
          XlOpenFileErrComp(filerr);
          XlErrComp(NULL,"elimina_da_lista","ERRORE\n ",messaggio);
          XlSetFileErrComp(NULL);
*/
          chdir(path_cwd);
          pagina_free(pag);
          return;
          }

/*     Eliminazione schema */
       ret = eli_schema(lista,nome_4,numero_schemi,filerr);

/*     Conrolli sulla cancellazione e segnalazioni errore */
       if ( ret<=0 )
          {
          strcpy(messaggio,"Error during deleting into the list: ");
          if(ret==-1)
             strcat(messaggio,"bad scheme number");
          else
            if ( (ret==-2) )
               strcat(messaggio,"missing scheme");
          strcat(messaggio,";other files are not removed");

          printf("%s\n",messaggio);
/*
          XlOpenFileErrComp(filerr);
          XlErrComp(NULL,"elimina_da_lista","ERRORE\n ",messaggio);
          XlSetFileErrComp(NULL);
*/
          chdir(path_cwd);
          pagina_free(pag);
          return;
          }

/*     Riscrittura lista e cancellazione oggetto dalla libreria */
       wr_lst_schemi(lista,numero_schemi,file_lista);
       sprintf(comando,"ar -dv %s ",lib_lista);
       tominus(nome_schema);
       strcat(comando,nome_schema);
       strcat(comando,".o");
       system(comando);
       }    /* Fine if in_lista==1 */

/* Cancellazione files */
   if (nome_schema!=NULL)
      {
      tominus(nome_schema);
      strcpy(rmfile,nome_schema);
      strcat(rmfile,".*");
      XlRemove(rmfile);
/*    File _01.dat  e file nomepagina.reg_err oppure schema_macro.reg */
      if ( tipo_lista==1 )
         {
         strcpy(rmfile,nome_schema);
         strcat(rmfile,"_01.dat");
         XlRemove(rmfile);
         }
      else
         {
         strcpy(rmfile,nome_schema);
         strcat(rmfile,"_macro.reg");
         XlRemove(rmfile);
         }
      strcpy(rmfile,"../");
      strcat(rmfile,nome_pag);
      strcat(rmfile,REG_EXT);
      strcat(rmfile,ERR_EXT);
      XlRemove(rmfile);
/*    File _04.dat e _14.dat oppure schemainfo.dat */
      if ( tipo_lista==1 )
         {
         tominus(nome_4);
         strcpy(rmfile,nome_4);
         strcat(rmfile,"_14.dat");
         XlRemove(rmfile);
         strcpy(rmfile,nome_4);
         strcat(rmfile,"_04.dat");
         XlRemove(rmfile);
         }
      else
         {
         tominus(nome_4);
         strcpy(rmfile,nome_4);
         strcat(rmfile,"info.dat");
         XlRemove(rmfile);
         }
      }

/* Mi ricolloco nel direttorio di partenza */
   chdir(path_cwd);

/* Liberazione struttura */
   if ( (in_lista==1)&&(pg_comp==1) )
      if (lista != NULL)
         {
         free(lista);
         lista=NULL;
         }

/* Liberazione struttura pagina */
   pagina_free(pag);
}


/*
    Se nome NULL errore;
    Se nome piu' lungo di lunghezza errore;
    Se nome tra 0 e lunghezza lo completa con _ (che sostituiscono
    gli eventuali blank) e lo termina con 0.
    Nome out deve essere dichiarato char[lunghezza+1] !!!
*/
int nome_underscore(char *nome_in, char *nome_out, int lunghezza)
{
int ret,i,lun;


   lun = strlen(nome_in);

/* Controlli */
   if( lun==0 )
      return(-1);
   if( lun>lunghezza )
      {
      printf("nome_underscore: nome in ingresso troppo lungo\n");
      return(-1);
      }

/* Gestione */
   strcpy(nome_out,nome_in);
   if( lun==lunghezza)
      {
      for(i=0;i<lunghezza;i++)
         if ( nome_out[i]==' ' )
           nome_out[i] = '_';
      }
   else
      {
      for(i=lun;i<lunghezza;i++)
         strcat(nome_out,"_"); 
      }
   
    nome_out[lunghezza] = '\0';

    return(1);
}


/*
   Verifica la presenza di alcuni files.
N.B. 
Sostituito controllo gen_macromod con gen_f01task
*/
int controllo_exe(char *msg_out)
{
int ret1=99;
int ret2=99;
int ret3=99;
int ret4=99;
int ret5=99;
int ret_1=99;
char mem[200];
   struct stat st;

 
   strcpy(msg_out,"");

/* Posizionamenti */
   getcwd(mem,sizeof(mem));
   chdir(XlGetenv("LEGOMMI_BIN"));

   ret1 = stat( "gen_topologia", &st );
   ret2 = stat( "gen_sche", &st );
   ret3 = stat( "gen_f01task", &st );
   ret4 = stat( "gen_f14", &st );
   ret5 = stat( "gen_stazionario", &st );

/* Ritorni */
   chdir(mem);
   /*printf(" %d  %d  %d  %d %d \n",ret1,ret2,ret3,ret4,ret5);*/
   if( (ret1==0)&&(ret2==0)&&(ret3==0)&&(ret4==0)&&(ret5==0) )
       return(0);
   else
      {
      if( ret1==-1 )
         {
         strcpy(msg_out,"gen_topologia");
         return(-1);
         }
      if( ret2==-1 )
         {
         strcpy(msg_out,"gen_sche");
         return(-2);
         }
      if( ret3==-1 )
         {
         strcpy(msg_out,"gen_f01task");
         return(-3);
         }
      if( ret4==-1 )
         {
         strcpy(msg_out,"gen_f14");
         return(-4);
         }
      if( ret5==-1 )
         {
         strcpy(msg_out,"gen_stazionario");
         return(-5);
         }
      }
}



/*
    Determina se una pagina identificata dal nome sta nella
    lista_schemi.dat (1) o nella lista_macro.dat (2) o nella
    lista dei moduletti lista_moduli_reg.dat (3). 
    Quest'ultimo controllo viene fatto solo quando controllo_macro 
    vale 1 cioe' in caso di attribuzione, durante il ciclo di
    compilazione, del flag fl_macro. In questo caso le liste 
    vengono lette comunque.
    Questa routine viene chiamata da TestMacro e da elimina_schema.
*/
int trova_in_lista(char *nome, char *filerr, int controllo_macro, int in_comp)
{
LST_SCHM *lista;
int ret;
int numero_schemi;
int numero_macro;
int in_lista_moduletti=1;
int in_lista_schemi=1;
int in_lista_macro=1;
char nome_4[10];
char messaggio[100];


/* Nomi */
   nome_underscore(nome,nome_4,4);

/* Controllo sulla lista dei moduletti gia' letta in precedenza */
   if (controllo_macro==1)
      {
      if ( -1 == schema_presente(ls_moduletti,nome_4,n_mdlt) )
            in_lista_moduletti = 0;
      ret =3;
      }

/*
   Viene letta la lista degli schemi solo in caso di cancellazione
   perche' potrebbe non essere mai stata letta.
   Nella compilazione normale si usa la lista che e' gia' stata letta.
*/
   if ( in_comp==0 )
      {
      numero_schemi= rd_lst_schemi(&lista,filerr,lista_schemi);
      if ( -1 == schema_presente(lista,nome_4,numero_schemi) )
         in_lista_schemi = 0;
      if ( numero_schemi>0 )
	 if (lista != NULL)
            {
            free(lista);
	    lista=NULL;
            }
      }
   else
      {
      numero_schemi=n_schm;
      if ( -1 == schema_presente(ls_schemireg,nome_4,numero_schemi) )
         in_lista_schemi = 0;
      }

/*
   Viene letta la lista dei macromoduli solo in caso di cancellazione
   perche' potrebbe non essere mai stata letta.
   Nella compilazione normale si usa la lista che e' gia' stata letta.
*/
   if ( in_comp==0 )
      {
      numero_macro= rd_lst_schemi(&lista,filerr,lista_macro);
      if ( -1 == schema_presente(lista,nome_4,numero_macro) )
         in_lista_macro = 0;
      if ( numero_schemi>0 )
	 if (lista != NULL)
            {
            free(lista);
	    lista=NULL;
            }
      }
   else
      {
      numero_macro= n_mcro1;
      if ( -1 == schema_presente(ls_macromod,nome_4,numero_macro) )
         in_lista_macro = 0;
      }

/* Risultati */
   if ( (in_lista_macro==0)&&(in_lista_schemi==0)&&(controllo_macro==0) )
      {
      ret=-1;
      sprintf(messaggio,"Non trovata la pagina <%s> nelle liste",nome_4);
      }
   if ( (in_lista_macro==1)&&(in_lista_schemi==1) )
      {
      ret=-2;
      sprintf(messaggio,"La pagina <%s> sta' in entrambe le liste",nome_4);
      }
   if ( in_lista_schemi==1 )
      ret=1;
   if ( in_lista_macro==1 )
      ret=2;
  
   if (ret<0)
      printf("%s\n",messaggio);
   return(ret);
}
   


/*
      Legge la lista dei moduletti, degli schemi e dei
      macromoduli  allocando le relative strutture.
      Errore solo in caso in cui la lista dei moduletti
      non esista o sia vuota.
*/
int   LeggiListe(char *filerr, char *work_path)
{
char nome_file[FILENAME_MAX];
char path_pwd[FILENAME_MAX];


/*    Si posiziona nel direttorio di lavoro */
      getcwd(path_pwd,sizeof(path_pwd));
      chdir(work_path);

/*    Pathname della lista dei moduletti */
      strcpy(nome_file,XlGetenv("LEGOCAD_USER"));
#if defined UNIX
      strcat(nome_file,"/legocad/libut_reg/libreg/");
#endif
      strcat(nome_file,lista_moduletti);


/*    Memorizzazione liste */
      n_mdlt= rd_lst_schemi(&ls_moduletti,filerr,nome_file);
      n_mcro1= rd_lst_schemi(&ls_macromod,filerr,lista_macro);
      n_schm= rd_lst_schemi(&ls_schemireg,filerr,lista_schemi);

/*    Ritorno directory di partenza */
      chdir(path_pwd);
      /*
      printf("Finito LeggiListe con n_mcr=%d n_schm=%d n_mdlt=%d\n",
              n_mcro,n_schm,n_mdlt);
      */

      if ( n_mdlt<0 )
         return(-1);
      else
         return(1);
}



/*
    Riconosce se un moduletto che si sta processando
    durante la compilazione sta nella lista_macro.dat
*/
int TestMacro(char *nome_cercato, char *filerr, char *work_path, char *outmsg)
{
int  ret;
char path_pwd[FILENAME_MAX];


/* Si posiziona nel direttorio di lavoro */
   getcwd(path_pwd,sizeof(path_pwd));
   chdir(work_path);

/*
   Controllo la presenza del moduletto semplice o macro 
   nella lista dei moduletti o nelle altre liste.
*/
   ret = trova_in_lista(nome_cercato, filerr,1,1);

   switch(ret)
     {
     case -2:
        {
        /* Sta in lista_macro e lista_schemi (-2). ERRORE */
        strcpy(outmsg,"Presenza contemporanea in lista_macro e lista_schemi");
        ret = -2;
        break;
        }
     case -1:
        {
        /* Non puo' accadere. ERRORE */
        strcpy(outmsg,"Assenza sia da in lista_macro che da lista_schemi");
        ret = -1;
        break;
        }
     case 1:
        {
        /* Sta in lista_schemi.dat. ERRORE */
        strcpy(outmsg,"Moduletto in lista_schemi ! ");
        ret = -3;
        break;
        }
     case 2:
        {
        /* Sta in lista_macro.dat. Il flag sara' 1 */
        ret = 1;
        break;
        }
     case 3:
        {
        /* Sta in lista_moduli_reg.dat, Il flag sara' 0 */
        ret = 0;
        break;
        }
     default :
        {
        /* ERRORE */
        strcpy(outmsg,"Attribuzione flag icona : Errore non codificato");
        ret = -4;
        break;
        }
     }
    chdir(path_pwd);
    return(ret);
}



/*
    Da usare nella qsort per ordinamento alfabetico widget
*/
int Widgetcmp(Widget *w1, Widget *w2)
{
int num1, num2;
char *str1,*str2;

     str1=XtName(*w1);
     str2=XtName(*w2);
     sscanf(str1,"%d",&num1);
     sscanf(str2,"%d",&num2);

     return(num1-num2);
}



/* 
   Descrizione dei vari messaggi d'errore
*/
int  MessaggiErrore(int ind, int caso, char *msg_out)
{
struct MSG_EXE_ST 
{
char *eseg;
char *msg2;
char *msg3;
char *msg4;
char *solu;
};

static struct MSG_EXE_ST msg[9]= 
   {
   "gen_topologia", "<schema> <path> <filerr>", 
   "Caso non previsto", "Caso non previsto", "-",

   "gen_sche", "<schema> <work_path>", 
   "Caso non previsto", "Caso non previsto", "-",

   "vecchio gen_macromod", "        ", 
   "editare il file degli errori err_macro.edf", "Caso non previsto", "-",

   "gen_f14", "<schema> <path libut_reg> <work_path> <filerr>", 
   "Caso non previsto", "Caso non previsto", "-",
 
   "gen_stazionario", "<schema> <path_lib> <work_path> <filerr>", 
   "Compilazione fallita di lg3reg.f", "Calcolo rgime (lg3reg) fallito", "-",

   "gen_f01task", "<task> <path task> <work_dir> <fileschemi> <file_err>", 
   "Caso non previsto", "Caso non previsto", "-",

   "gen_f14task", "<task> <path task> <work_dir> <fileschemi> <file_err>", 
   "Caso non previsto", "Caso non previsto", "-",

   "compila_reg", "EstrTagVal fallisce la lettura del valore", 
   "di una variabile o di un dato", "-", "Controllare la form",

   "compila_reg", "PropagaValoreFraPorte fallisce la lettura del valore", 
   "nella stringa a causa di EstrTagVal", "-", "Controllare la form",
    };

     switch(caso)
         {
         case 1:
            {
            sprintf(msg_out,"MANCA ESEGUIBILE %s\n",msg[ind-1].eseg);
            break;
            }
         case 2:
            {
            sprintf(msg_out,
                   "PARAMETRI SCORRETTI %s; USARE %s",msg[ind-1].eseg,msg[ind-1].msg2);
            break;
            }
         case 3:
            {
            sprintf(msg_out,
                   "Errori in %s, %s",msg[ind-1].eseg,msg[ind-1].msg3);
            break;
            }
         case 4:
            {
            sprintf(msg_out,
                   "Errori in %s, %s",msg[ind-1].eseg,msg[ind-1].msg4);
            break;
            }
         case 5:
            {
            sprintf(msg_out,
                   "Errori in %s, %s %s\n   %s",msg[ind-1].eseg,msg[ind-1].msg2,
                                              msg[ind-1].msg3,msg[ind-1].solu);
            break;
            }
         default :
            {
            sprintf(msg_out,
                   "%s non e' terminato corretteamente",msg[ind-1].eseg);
            break;
            }
         }
}



/*
 get_wid_port
 restituisce l'indice di widget della porta dato il widget dell'icona
 ed il nome della porta
*/
Widget get_wid_port(Widget wid_icon,char *nome_porta)
{
Widget wid_port;
wid_port=XtNameToWidget(wid_icon,nome_porta);
return(wid_port);
}




/*
 get_wid_icon_conn
 ricava l'indice di widget dell'icona a cui e' connesso
 la porta a partire dal nome della porta connessa e dallo
 indice di widget della drawing area
*/
Widget get_wid_icon_conn(Widget wid,char *nome_porta_conn)
{
char *pstr;
char *nome_icon_conn;
Widget wid_icon_conn;
/*
 - in base al nome della porta connessa viene ricavato il nome
   della icon reg che la contiene (== nome della porta troncato
   al carattere 'w').
 - l'indice di widget della icona di regolazione collegata si
   ricava utilizzando la funzione XtNameToWidget
*/
/* ricava il nome dell'icona partendo da quello della porta */
nome_icon_conn=XtNewString(nome_porta_conn);
pstr=strchr(nome_icon_conn,'w');
pstr++;
*pstr='\0';

/*printf("nome icon conn = %s\n",nome_icon_conn);*/

wid_icon_conn=XtNameToWidget(wid, nome_icon_conn);
/*printf(" wid_icon_conn = %d\n",wid_icon_conn);*/
XtFree(nome_icon_conn);
return(wid_icon_conn);
}



/*--------------------------------------------------------------
 *
 *  get_child
 *
 *  restituisce la lista e il numero dei children del widget wid
 *
 *--------------------------------------------------------------*/
void get_child(Widget wid,WidgetList *childList,Cardinal *numChild)
{
Cardinal lnargs;
Arg args[5];
    /*
        verifico che l'oggetto sia un composite altrimenti ritorno
        childList = NULL e numChild = 0;
    */
    if(!XtIsComposite(wid))
        {
        *childList = NULL;
        *numChild = 0;
        return;
        }
    lnargs=0;
    XtSetArg(args[lnargs],XmNnumChildren,numChild);lnargs++;
    XtSetArg(args[lnargs],XmNchildren,childList);lnargs++;
    XtGetValues(wid,args,lnargs);
}



/*
    Data una pagina e una tag ricava il
    pathname del file relativo alla pagina
    contenuto nella sottodirectory ./proc
    corrente e lo restituisce nel parametro fileout.
*/
int FileNameInproc(char *pgname, char *tag, char *fileout)
{
char work_file[300];
char *appoggio;
char *nomepagina;
char *nome_4;
int lun,ret;
struct stat buf;
int stat_ret;


   lun        = strlen(pgname) + strlen(workfile1);
   appoggio   = XtMalloc((lun+1)*sizeof(char));
   nomepagina = XtMalloc((lun+1)*sizeof(char));
/* tiene conto della lunghezza della tag (7 caratteri)
   con abbondanza per eventuali modifiche
*/   
nome_4     = XtMalloc((lun+21)*sizeof(char));

   strcpy(nomepagina,workfile1);
   strcpy(appoggio,pgname);
   tominus(appoggio);
   strcat(nomepagina,appoggio);

/* Nomi di pagine piu' corti di 4 caratteri */
   if(   (strlen(pgname)<4) &&
       ( (!strcmp(tag,"_04.dat")) || (!strcmp(tag,"_14.dat")) )  )
      {
      strcpy(appoggio,pgname);
      nome_underscore(appoggio,nome_4,4);
      strcpy(appoggio,workfile1);
      strcat(appoggio,nome_4);
      strcpy(nome_4,appoggio);
      printf("nome_4=%s\n",nome_4);
      }
   else
      strcpy(nome_4,nomepagina);


/* Compongo con il nome pagina e la tag desiderata (nomi minuscoli) */
   strcat(nome_4,tag);

   strcpy(fileout,nome_4);
   /*
   printf("\t FileNameInproc restituisce=%s  (workfile=%s)\n",
             fileout,workfile);
   */

   stat_ret=stat(fileout,&buf);

   XtFree(nomepagina);
   XtFree(nome_4);
   XtFree(appoggio);

   return(stat_ret);
}




/* Fine file */
