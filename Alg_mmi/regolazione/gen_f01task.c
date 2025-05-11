/**********************************************************************
*
*       C Source:               gen_f01task.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Feb 18 17:01:14 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: gen_f01task.c-5 %  (%full_filespec: gen_f01task.c-5:csrc:2 %)";
#endif
/*
   modulo gen_f01task.c
   tipo 
   release 5.2
   data 2/20/96
   reserved @(#)gen_f01task.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gen_f01task.c	5.2\t2/20/96";
/*
        Fine sezione per SCCS
*/
/***********************************************************************
     programma per la generazione della topologia di un task di
                     regolazione in CAD CONTROLLO

21/10/94
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <top_icon.h>


/*       VARIABILI RELATIVE A CONNESSIONI FRA SCHEMI LETTE DA FILE */
char int_sche[NSCHEMI][NINTE][9];   /* Vettorializzato NSCHEMI [NINTE]*/
char int_conn[NSCHEMI][NINTE][3];   /* Vettorializzato NSCHEMI [NINTE]*/
char int_varl[NSCHEMI][NINTE][10];  /* Vettorializzato NSCHEMI [NINTE]*/
char int_varr[NSCHEMI][NINTE][10];  /* Vettorializzato NSCHEMI [NINTE]*/

char int_sigl[NSCHEMI][NINTE][5];   /* Vettorializzato NSCHEMI */
int short int_nm[NSCHEMI];          /* Vettorializzato NSCHEMI */

int short sch_nm;
char sch_id[NSCHEMI][5];
char sch_uso[NSCHEMI][2];           /* Vettorializzato NSCHEMI */
char sch_ds[NSCHEMI][54]; 



char fpath[200];
char work_dir[200];

/*
   Prototyping funzioni
*/
int rd_connessioni(char *nomefile);
int completa_nome(char *, char *);
char *strupr(),*strlwr();
void array();
char *getpul (char *, int , FILE *);

void XlOpenFileErrComp(char *nomeFile);
void XlErrComp(char* ,char *,char *,char *);
void XlSetFileErrComp(char *);

char con_sche[NSCHEMI][NINTE][9];   /* Vettorializzato NSCHEMI e letto
                                       da schemi.dsc contiene il nome
                                       completo <schema><progress><id_task>
                                       dello schema a cui e' connessa
                                       la variabile riferita a quella
                                       icona di interfaccia */
char con_varl[NSCHEMI][NINTE][9];   /* Vettorializzato NSCHEMI */
char con_varr[NSCHEMI][NINTE][9];   /* Vettorializzato NSCHEMI */

int main(argc, argv)
int argc;
char *argv[];
{

	char linea[256];
        char task[9];
        char nomefilef01[FILENAME_MAX];
        char nomefile[FILENAME_MAX];
	char nomelib[FILENAME_MAX];
	char nomecon[FILENAME_MAX];
        FILE *fp1, *fptask, *fplib, *fpcon;
	fpos_t posit1, posit2, oldposit;
        char pag_ds[54];
	char blocco[7];
	char linea1[256];	
	char descr[54];
	char stringa[FILENAME_MAX];

	int short errore,errore_con;
	int short nint;
	int short trovato;
        int short i, ii, j, k, l, n, inizio,lun,tmp;

int ret;
int schemi_usati;
int diff;
int conta;
char nome_file[FILENAME_MAX];
char fileschemi[FILENAME_MAX];
char filerr[FILENAME_MAX];
char schema4[5];
char schema8[9];
char appoggio[20];
char messaggio[FILENAME_MAX]={'\0'};
                  
char msg_gen[FILENAME_MAX]={"Errore: generare i modelli degli schemi:"};
char msg_sch[100]={'\0'};

FILE *fout;
char tag_des[NSCHEMI][5];              /* Contiene <proga schemar><id_task>i
                                          e viene letto dal file schema.reg
                                          prima della fase di scrittura della
                                          intestazione */

        tmp=0;
        if (argc != 6)
           {
           fprintf(stderr,
"USO CORRETTO: gen_f01task  <task> <fpath> <work_dir> <fileschemi> <filerr>\n");
           exit(2);
           }
	strcpy(task,strupr(argv[1])); 	 	
	strcpy(fpath,argv[2]); 
	strcpy(work_dir,argv[3]); 

	strcpy(fileschemi,argv[4]); 	 	
	strcpy(filerr,argv[5]); 	 	


/*      Lettura del file con le connessioni */
        ret = rd_connessioni(fileschemi);
        if (ret<0)
           {
           fprintf(stderr,"Fallita lettura file connessioni");

           strcpy(messaggio,"Fallita lettura file connessioni ");
           strcat(messaggio,fileschemi);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);

           exit(-1);
           }

/*	sprintf(nomefilef01,"%s/f01.dat",fpath);*/
	strcpy(nomefilef01,fpath);
#if defined UNIX
	strcat(nomefilef01,"/");
#endif
	strcat(nomefilef01,"f01.dat");

	fptask = fopen(nomefilef01, "w");
        if (fptask == NULL)
           {
           fprintf(stderr,"Errore in apertura %s\n",nomefilef01);

           strcpy(messaggio,"Errore in apertura file ");
           strcat(messaggio,nomefilef01);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);

           exit(-1);
           }
        
/*      Nomi files temporanei */
	strcpy(nomelib,work_dir); 
	strcpy(nomecon,work_dir); 
#if defined UNIX
	strcat(nomelib,"/");
	strcat(nomecon,"/");
#endif
        strcat(nomelib,"ing_lib.tmp"); 
        strcat(nomecon,"ing_con.tmp"); 

        fplib = fopen(nomelib, "w");
        if (fplib == NULL)
           {
           strcpy(messaggio,"Errore in apertura file ");
           strcat(messaggio,nomelib);
           fprintf(stderr,"%s\n",messaggio);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);

           fclose(fptask);
           exit(-1);
           }

	fpcon = fopen(nomecon,"w");
        if (fpcon == NULL)
           {
           strcpy(messaggio,"Errore in apertura file ");
           strcat(messaggio,nomecon);
           fprintf(stderr,"%s\n",messaggio);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);

           fclose(fptask);
           fclose(fplib);

           exit(-1);
           }

        fprintf(fptask, "****\n");

/*      
        INTESTAZIONE DEL FILE
 	Ciclo di lettura degli schemi. Si usano solo gli schemi 
	che hanno il flag SCH_USO = S. Si scrivono nel file f01.dat
        nome e descrizione di ogni schema usato.
*/
	for (i=0; i<sch_nm; i++)	
           {
/*         Controllo uso dello schema */
           if( sch_uso[i][0]!='S' )
               continue;

	   strupr(sch_id[i]);

/*         Controllo lunghezza nome */
           completa_nome(sch_id[i],schema4);

	   fprintf(fptask,"%4.4s      BLOCCO (%4.4s) - %-53s\n",
		   schema4, schema4, sch_ds[i]);
           schemi_usati++;
	   }   /* Fine ciclo intestazione file */

        fprintf(fptask, "****\n%-8.8s\n",task);

	errore = 0;
	errore_con = 0;
	
/*      Apertura file di uscita per connessioni dello schema */
        fout = fopen("Conn_task.edf","w");

/*      CICLO DI SCANSIONE DI TUTTI GLI SCHEMI */
	for (i=0; i<sch_nm; i++)	
           {
/*         Controllo uso dello schema */
           if( sch_uso[i][0]!='S' )
               continue;
           /* 
           debug
           printf("\n  trattamento schema %d %4.4s ",i+1,sch_id[i]); 
           debug
           */
	   fprintf(fptask,"****\n");

/*         Controllo lunghezza nome */
           completa_nome(sch_id[i],schema4);

/*         
           Lettura da schema.dat della tag_des.
*/
	   strcpy(nomefile,work_dir);
#if defined UNIX
	   strcat(nomefile,"/");
#endif
	   strcpy(appoggio,sch_id[i]);
	   strlwr(appoggio);
	   strcat(nomefile,appoggio);
	   strcat(nomefile,".dat");
           fp1 = fopen(nomefile, "r");
           if (fp1 == NULL)
               {
               fprintf(stderr,
                       "Errore: generare modello dello schema %4.4s\n",
                        sch_id[i]);
               /* Per file errori generazione f01 */
               strcpy(msg_sch,sch_id[i]);
               strcat(msg_sch," ");
               errore++;
               continue;
               }

           /* Lettura schema.dat solo delle prime 4 righe */
           conta=0;
	   /*while( (fgets(linea,150,fp1) != NULL) && (conta!=4) )*/
	   while( (getpul(linea,200,fp1) != NULL) && (conta!=4) )
              {
              conta++;
              if( conta==4 )
                 {
                 completa_nome(sch_id[i],schema4);
                 if( strncmp(schema4,linea,4)!=0 )
                    {
                    fprintf(stderr,"\t\t [%s]\n",schema4);
                    fprintf(stderr,
                            "Errore in schema.dat in lineai (lun=%d):\n<%s>",
                            strlen(linea),linea);
                    strcpy(msg_sch,sch_id[i]);
                    strcat(msg_sch," La linea 4 e' scorretta. ");
                    errore++;
                    fclose(fp1);
                    continue;
                    }
                 else
                    {
                    strncpy(tag_des[i],&linea[4],4);
                    tag_des[i][4]='\0';
                    if ( strlen(tag_des[i])!=4 )
                       {
                       fprintf(stderr,
                            "Errore tag_scorretta %s\n",tag_des);
                       strcpy(msg_sch,sch_id[i]);
                       strcat(msg_sch," tag_des scorretta:");
                       strcat(msg_sch,tag_des[i]);
                       errore++;
                       fclose(fp1);
                       continue;
                       }
                    }
                 }
              }
           fclose(fp1);


/* 1) INTESTAZIONE SCHEMA I-ESIMO  */
	   fprintf(fptask,"%4.4s%4.4s  BL.-%4.4s- **** REGOL. %4.4s - %s\n",
                   schema4, tag_des[i], schema4, schema4, sch_ds[i]);

/*         Apertura del file schema.dat (esistenza gia' testata) */
	   strcpy(nomefile,work_dir);
#if defined UNIX
	   strcat(nomefile,"/");
#endif
           strcpy(appoggio,sch_id[i]);
           strlwr(appoggio);
	   strcat(nomefile,appoggio);
	   strcat(nomefile,".dat");
           fp1 = fopen(nomefile, "r");
/* 2) VARIABILI D'USCITA */
	   /*while( fgets(linea,150,fp1) != NULL )*/
	   while( getpul(linea,200,fp1) != NULL )
              {
              if(strncmp(linea,"****",4) == 0)
                 {
                 continue;
                 }
              else if(strncmp(&linea[10],"--UA--",6) == 0)
                      {
                      fprintf(fptask,"%s\n",linea);		
                      fgetpos(fp1,&oldposit); /* SALVO POSIZIONE */
                      }
		    else if(strncmp(&linea[10],"--IN--",6) == 0)
                            {
                            break;
                            }
              }


/* 3) VARIABILI D'INGRESSO */
	   fsetpos(fp1,&oldposit);

	   sch_id[i][4]='\0';	


/* 4) ICONE INTERFACCIA */
 	   /*nint = -1;*/
 	   nint = 0;
/*
           Ciclo per ogni icona di interfaccia dello schema a (di indice i)
           Significato variabili (riferite allo schema)
           int_nm[i] : numero delle icone di interfaccia
           nint      : numero delle icone di interfaccia connesse
*/
           for(j=0;j<int_nm[i];j++)
              {   
              trovato = 0;
/*            Test sulla connessione dell'icona di interfaccia */
              if( !strcmp(int_conn[i][j],"NO") )
                  continue;
/*
              Si cerca con un ciclo ii) su tutti gli schemi se lo schema 
              connesso (int_sche[i]) alla icona di interfaccia
              connessa (int_conn[i][j]=SI dello schema considerato
              sch_id[i]) e' fra quelli esistenti (sch_id[ii]).
*/
              /*for(ii=0; ii<=sch_nm; ii++)*/
              for(ii=0; ii<sch_nm; ii++)
                 {
                 strupr(sch_id[i]);
/*  ATTENZIONE IL TEST DEVE ESSERE PORTATO A 8 CARATTERI */
                 completa_nome(sch_id[ii],schema4);
                 /*
                 debug
                 printf("\n Confronto ii=%d i=%d j=%d <%s> con <%s>\n",
                        ii,i,j,schema4,int_sche[i][j]);
                 debug
                 */
                 if (strncasecmp(schema4,int_sche[i][j],4) ==0)
                    {
                    trovato=1;
                    /*
                    debug
                    printf("\n   TROVATO=%s|\n",int_sche[i][j]);
                    debug
                    */
                    break;
                    }
                 }  /* Fine ciclo ii */
/*            Se non e' stato trovato e' un errore */
              if(!trovato)
                 {
                 sprintf(messaggio,
	           "Var. (locale) <%s> dello schema <%s> connessa a schema <%s> non trovato\n",
	            int_varl[i][j],sch_id[i],int_sche[i][j]);
                 fprintf(stderr,messaggio);
                 errore_con++;
                 goto ERREXIT;
                 }
/*            Controllo lunghezza nome schema connesso */
              completa_nome(int_sche[i][j],con_sche[i][j]);
/*            Riempimento buffer */
              strupr(con_sche[i][j]);
              strcpy(con_varl[i][j],int_varl[i][j]);
              strcpy(con_varr[i][j],int_varr[i][j]);
              nint++;
              }  /* Fine ciclo (j) sulle icona di interfaccia */

/*            Scrittura connessioni  su file di output */
              fprintf(fout,
                  "   ******Connessioni schema n.%d %s (icone interfaccia %d,connesse=%d)\n",
                      i,sch_id[i],int_nm[i],nint);	
              /*
	      printf("   ******Connessioni schema n.%d %s: (icone interfaccia %d,connesse=%d)\n",
                      i,sch_id[i],int_nm[i],nint);	
              */
	      for (j=0; j<int_nm[i]; j++)
	         {
                 fprintf(fout,
                         "   icona <%d>, var. locale: <%s> connessa a schema: <%s> (varr:%s)\n",
                        j,con_varl[i][j],con_sche[i][j],con_varr[i][j]);
                 /*
                 printf("   icona <%d>, var. locale: <%s> connessa a schema: <%s> (varr:%s)\n",
                        j,con_varl[i][j],con_sche[i][j],con_varr[i][j]);
                 */
	         }	

/* 5) CONNESSIONI FRA GLI SCHEMI */
/*
           Lettura del file schema.dat
*/
	   /*while ( fgets(linea,150,fp1) != NULL ) */
	   while ( getpul(linea,200,fp1) != NULL ) 
              {
              /*
              debug
              printf("  linea=%6.6s\n",linea);
              debug
              */
              if (strncmp(linea,"****",4) == 0)
                 {
                 continue;
                 }
              else if (strncmp(&linea[10],"--IN--",6) == 0)
                 {
/*               Per ogni icona di interfaccia cerco la connessione */
                 trovato = 0;
		 /*for ( j=0; j<=nint; j++ )*/         /*  <= ???? */
		 for ( j=0; j<int_nm[i]; j++ )
                    {
                    /*
                    debug
                    printf("    Confronto con_varl[i=%d][j=%d]=%s con =%8.8s\n",
                            i,j,con_varl[i][j],linea);
                    debug
                    */
                    if (strncmp(linea,con_varl[i][j],8) == 0)
                       {
                       /*
                       debug
                       printf("    la variabile e'  <%8.8s>\n", linea);   	
                       debug
                       */
/*
                       sprintf(linea1,
				"%8.8s  --CO--%s",linea,&linea[16]);
			       	fprintf(fpcon,"%s\n",linea1);
*/
		       fprintf(fpcon,"%8.8s  --CO--%s\n",linea,&linea[16]);
/*
				sprintf(linea1,
"%8.8s  --IN-- #%8.8s#....................<===%8.8s%  BL.-%4.4s-",
con_varr[i][j], linea, con_sche[i][j],  con_sche[i][j]); 
				fprintf(fptask,"%s\n",linea1);
*/
		       fprintf(fptask,
"%8.8s  --IN-- #%8.8s#....................<===%8.8s%  BL.-%4.4s-\n",
con_varr[i][j], linea, con_sche[i][j],  con_sche[i][j]);
                       trovato=1;
                       break;
                       } 	
                    }
                    if (!trovato)
                       {
                       fprintf(fptask,"%s\n",linea);
                       fprintf(fplib,"%s\n",linea);
                       }		
                 }
              }
	   fprintf(fptask,"****\n");
	   fclose(fp1);
        }  /* Fine del ciclo su tutti gli schemi */

        fclose(fout);
	if (errore)
           {
           goto ERREXIT;
           }
	fprintf(fptask,"****      >>>>>>INGRESSI DI TUTTI I MODULI<<<<<<\n");

/* STRANO, MA CON FFLUSH/REWIND NON FUNZIONA!
   DEVO CHIUDERE E RIAPRIRE (Pentolini)       */	
        fclose(fplib);
        fplib=fopen(nomelib,"r");
        while ( getpul(linea,200,fplib) != NULL )
           {
           fprintf(fptask,"%s\n",linea);
           }	
        fprintf(fptask,"****\n");
        fclose(fplib);
        remove(nomelib);   
        fclose(fpcon);
        fpcon=fopen(nomecon,"r");
        while ( getpul(linea,200,fpcon) != NULL )
           {
           fprintf(fptask,"%s\n",linea);
           }
        fclose(fpcon);
        remove(nomecon);	 
        fclose(fptask);
        /*printf("Fine corretta gen_f01task\n");*/
        exit(0);

ERREXIT:
        if( (errore>0) )
           {
           strcat(msg_gen,msg_sch);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",msg_gen);
           XlSetFileErrComp(NULL);
           }
        if ( (errore_con>0) )
           {
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_f01task","ERRORE\n ",messaggio);
           XlSetFileErrComp(NULL);
           }
        fclose(fp1);
        fclose(fplib);
        fclose(fpcon);
        remove(nomefilef01);
        remove(nomelib);   
        remove(nomecon);   
        printf("\nFine scorretta gen_f01task\n"); 

        exit(-1);
}



/*
	ARRAY
	CERCA LA VARIABILE LOCALE DI INGRESSO, CHE SCOMPARIRA' PERCHE' 
	CONNESSA AD ALTRO SCHEMA, TRA LE VARIABILI CAMBIO COLORE. SE LA 
	TROVA LA SOSTITUISCE CON QUELLA REMOTA 
*/
void array( varloc, varrem, num, vcc)
char varloc[6];
char varrem[8];
int  short num;
char vcc[][7];
{
	int short ii;

	for (ii=0; ii< num; ii++)
	{
		if (strncmp(varloc, vcc[ii],6)==0)
		{
			strcpy(vcc[ii], varrem);
		}
	}
}



/*
    Converte in caratteri maiuscoli.
*/
char * strupr(ss)
char ss[];
{
        int i;
        for(i=0; i<strlen(ss); i++)
            if(ss[i]<123 && ss[i]>96)
               ss[i] = ss[i] - 32;
	return(ss);        	
}


/*
    Converte in caratteri minuscoli.
*/
char * strlwr(ss)
char ss[];
{
        int i;
        for(i=0; i<strlen(ss); i++)
            if(ss[i]<91 && ss[i]>64)
               ss[i] = ss[i] + 32;
        return(ss);
}



/*
    Lettura delle variabili relative alle connessioni fra schemi.
*/
int rd_connessioni(char *input_file)
{
DESC_SCHEMA *schema;
FILE *ff;
int num_sch;
int diff;
int i,k;

    if( (ff=fopen(input_file,"r"))==NULL )
       {
       fprintf(stderr,"gen_f01task:Non si puo' aprire il file %s\n",input_file);
       return(-1);
       }

    fread(&num_sch,sizeof(int),1,ff);
    if( num_sch==0 )
       {
       fprintf(stderr,"Nessuno schema usato nel task \n");
       fclose(ff);
       return(-1);
       }

    schema = (DESC_SCHEMA*)calloc(sizeof(DESC_SCHEMA),num_sch);
    for(i=0;i<num_sch;i++)
       {
       fread(&schema[i],sizeof(DESC_SCHEMA)-sizeof(ICON_INTERF*),1,ff);
       /*
       printf("%d) nicn_interf=%d  uso=%s|  id_sch=%s| desc=%s|\n",i,
               schema[i].schm.num_icn_interf,schema[i].schm.uso_schema,
               schema[i].schm.id_schema,schema[i].schm.des_schema);
       */
       if( schema[i].schm.num_icn_interf>0 )
            {
            schema[i].icn_interf = (ICON_INTERF*)calloc(sizeof(ICON_INTERF),
                                              schema[i].schm.num_icn_interf);
            fread(&schema[i].icn_interf[0],sizeof(ICON_INTERF),
                  schema[i].schm.num_icn_interf,ff);
            /*
            for(k=0;k<schema[i].schm.num_icn_interf;k++)
               {
               printf("  %d.%d) sigla=%s| int_conn=%s| int_sche=%s| ",
                       i,k,
                       schema[i].icn_interf[k].int_icnsigl,
                       schema[i].icn_interf[k].int_conn,
                       schema[i].icn_interf[k].clg.int_sche);
               printf("varl=%s| varr=%s|\n",
                       schema[i].icn_interf[k].clg.int_varl,
                       schema[i].icn_interf[k].clg.int_varr);
               }
            */
            }
       }

/* Chiusura file */
   fclose(ff);

/* Assegnamento dei valori letti alle variabili globali */
   sch_nm = (short)num_sch;
   for(i=0;i<num_sch;i++)
      {
      strcpy(sch_id[i],schema[i].schm.id_schema);
      strcpy(sch_ds[i],schema[i].schm.des_schema);
      strcpy(sch_uso[i],schema[i].schm.uso_schema);
      int_nm[i] = (short)schema[i].schm.num_icn_interf;

      for(k=0;k<int_nm[i];k++)
         {
         strcpy(int_sigl[i][k],schema[i].icn_interf[k].int_icnsigl);
         strcpy(int_conn[i][k],schema[i].icn_interf[k].int_conn);
         strcpy(int_sche[i][k],schema[i].icn_interf[k].clg.int_sche);
         strcpy(int_varr[i][k],schema[i].icn_interf[k].clg.int_varr);
         strcpy(int_varl[i][k],schema[i].icn_interf[k].clg.int_varl);
         }
      }

/* Liberazione delle strutture */
   for(i=0;i<num_sch;i++)
      {
      if( schema[i].schm.num_icn_interf>0 )
         {
         /*
         printf("%d) Libero lo schema <%d icone interf> con ",
                 i,schema[i].schm.num_icn_interf);
         printf("int_conn[0]=%s|  int_conn[1]=%s|  sid_schema=%s|\n",
                 schema[i].icn_interf[0].int_conn,
                 schema[i].icn_interf[1].int_conn,
                 schema[i].schm.id_schema);
         */
         free(schema[i].icn_interf);
         }
      }
   free(schema);
   return(0);
}


/*
    Se il nome di ingresso e' inferiore come lunghezza
    a 4 caratteri la funzione lo completa accodando un
    numero sufficiente di caratteri "_"
*/
int completa_nome(char *nomein, char *nomeout)
{
int i,k,diff;

    strcpy(nomeout,nomein);
    diff = 4 - strlen(nomein);
    if( diff>0 )
       {
       for(k=0;k<diff;k++)
           strcat(nomeout,"_");
       return(-1);
       }
    return(1);
}
