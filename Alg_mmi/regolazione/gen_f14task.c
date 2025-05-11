/**********************************************************************
*
*       C Source:               gen_f14task.c
*       Subsystem:              2
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Feb 18 18:47:40 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: gen_f14task.c-5 %  (%full_filespec: gen_f14task.c-5:csrc:2 %)";
#endif
/*
   modulo gen_f14task.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)gen_f14task.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gen_f14task.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/************************************************************************
     programma per la generazione del file F14.DAT di un task di
                     regolazione in CAD CONTROLLO

IL FILE F14.DAT DEL TASK VIENE COSTRUITO A PARTIRE DA:
1) I FILE "<schema>_f14.dat" DEGLI SCHEMI USATI NEL TASK DI SIMULAZIONE
2) IL FILE f01.dat DEL TASK CHE CONTIENE INFORMAZIONI SULLE CONNESSIONI
   TRA GLI SCHEMI
3) IL FILE "schema.dsc" CHE CONTENGONO INFORMAZIONI SUI MODULI DI 
   REGOLAZIONE E SUI MACROMODULI

*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <top_icon.h>


/*
    Descrizioni dei moduletti di regolazione
*/
int nrego;
char g_id[NMODULI][5];
char g_ds[NMODULI][54];

char task[9], task_path[FILENAME_MAX], work_path[FILENAME_MAX];
char nomefile[FILENAME_MAX], nomef01[FILENAME_MAX], nomef14[FILENAME_MAX];
char linea[256], linea1[256], linea14[256];
/*
   Funzioni
*/
int rd_moduletti(char *);
short aggcount (short *contavar);
char *minuscolo (char *);
char *maiuscolo (char *);
char *getpul (char *, int, FILE *);
int estrai_nome(char *, char *);

void XlOpenFileErrComp(char *nomeFile);
void XlErrComp(char* ,char *,char *,char *);
void XlSetFileErrComp(char *);


int main (int argc, char *argv[])
{
short contavar;
short trovato;
int contaerr = 0;
FILE *f01, *f14, *f_14;
fpos_t pf01, pf01con, pf01lib, pf14, pf_14[NSCHEMI];
short sch_nm = 0;
char sch_id[NSCHEMI][LUNSIGL];
char sch_ds[NSCHEMI][54]; 
short i, j, k, n, tr;
char stringa[FILENAME_MAX];
char fileschemi[FILENAME_MAX];
char filerr[FILENAME_MAX];
char messaggio[FILENAME_MAX];
char app[FILENAME_MAX];
char appfile[FILENAME_MAX];
int contaerr_in;
int contaerr_bl;
char appoggio[10];



   if (argc != 6)
      {
      fprintf(stderr,
"USO CORRETTO: gen_f14task <task> <task_path> <work_path> <fileschemi> <filerr>\n");
      exit(2);
      }

   strcpy (filerr, argv[5]);        
   strcpy (fileschemi, argv[4]);        
   strcpy (work_path,argv[3]);    
   strcpy (task_path,argv[2]);    
/*
 Il nome della task deve essere portato in maiuscolo
 perche' nel lego viene utilizzato in questo formato
*/
   strcpy (task,maiuscolo(argv[1]));        
#if defined UNIX
   strcat (task_path,"/");
   strcat (work_path,"/");
#endif

/* APERTURA FILE DI TOPOLOGIA DEL TASK: F01.DAT */
   strcpy (nomef01,task_path);
   strcat (nomef01,"f01.dat");
   f01 = fopen (nomef01, "r");
   if (!f01)
      {
      fprintf(stderr,"Errore apertura file %s\n", nomef01);
      strcpy(messaggio,"Errore in apertura file ");
      strcat(messaggio,nomef01);
      XlOpenFileErrComp(filerr);
      XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
      XlSetFileErrComp(NULL);
      exit(-1);
      }

/* LETTURA DEGLI SCHEMI FACENTI PARTE DEL TASK */
   /*printf("\n  schemi usati nel task %s:\n",task);*/
   /*getpul (linea, 100, f01);*/   /* LA PRIMA LINEA E' ****, LA BUTTO VIA! */  
   getpul (linea, 200, f01);   /* LA PRIMA LINEA E' ****, LA BUTTO VIA! */  
   /*while (getpul (linea, 100, f01))*/
   while (getpul (linea, 200, f01))
      {
      if (!strncmp (linea, "****", 4))
         break;
      else
         {
         sprintf (sch_id[sch_nm], "%4.4s\0", linea);
         printf ("  %d %s|\n", sch_nm+1, sch_id[sch_nm]);
         sch_nm++;
         }      
      }


/* LA PROSSIMA LINEA DI F01 E' LA LISTA USCITE DEL PRIMO SCHEMA */

/* APERTURA FILE VALORI INIZIALI DEL TASK: F14.DAT */

   strcpy(nomef14,task_path);
   strcat(nomef14,"f14.dat");
   f14 = fopen (nomef14, "w");
   if (!f14)
      {
      fprintf(stderr,"Errore apertura file %s", nomef14);

      strcpy(messaggio,"Errore in apertura file ");
      strcat(messaggio,nomef14);
      XlOpenFileErrComp(filerr);
      XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
      XlSetFileErrComp(NULL);

      exit(-1);
      }

/* INTESTAZIONE DEL FILE */
   fprintf(f14, "*LG*NOME IMPIANTO =%-8.8s\n", task);
   fprintf(f14, "%s%s\n", "*LG*DATI DI NORMALIZZAZIONE P0=      *H0=      *",
            "W0=      *T0=      *R0=      *");
   fprintf(f14, "*LG*                        L0=      *V0=      *DP0=      *\n");
   fprintf(f14, "*LG*CONDIZIONI INIZIALI VARIABILI DEL SISTEMA (MKS)\n");

/* CONDIZIONI INIZIALI VARIABILI DEL SISTEMA ===> F14.DAT */

   contavar = 0;
   for (i=0;i<sch_nm;i++)   
      {
      /*printf ("\n  trattamento schema %d %4.4s|\n ", i+1, sch_id[i]);*/
      strcpy(nomefile,"");
/*
      sprintf (nomefile, "%s%4.4s%s", task_path, sch_id[i], "_14.dat");
*/
      strcpy(appoggio,sch_id[i]);
      minuscolo(appoggio);
      sprintf (nomefile, "%s%s%s", work_path, appoggio, "_14.dat");
      /*f_14 = fopen (minuscolo(nomefile), "r");*/
      f_14 = fopen (nomefile, "r");
      if (!f_14)
         {
         strcpy(messaggio,"Generare stazionario dello schema ");
         strcat(messaggio,sch_id[i]);
         XlOpenFileErrComp(filerr);
         XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
         contaerr++;
         continue;
         }   

      /*while (getpul (linea, 150, f_14))*/
      while (getpul (linea, 200, f_14))
         if (! strncmp (linea, "*LG*CONDIZIONI", 14))
            break;

      /*while (getpul (linea, 150, f_14))*/
      while (getpul (linea, 200, f_14))
         {
         if (! strncmp (linea, "*LG*CONDIZIONI", 14))
            break;

/*       CERCO LA VARIABILE (LETTA DAL _14.DAT) IN F01.DAT */       
         /*while (getpul (linea1, 150, f01))*/
         while (getpul (linea1, 200, f01))
            {
            /*printf("Confronto 01=%9.9s 14=%9.9s\n",linea1,linea+4);*/
            if (!strncmp (linea1, linea+4, 8))
               break;
            }

/*       SCRITTURA IN F14 */
         sprintf (linea14, " %2.2d %40.40s %6.6s %s",
         aggcount (&contavar), linea+4, linea1+17, linea+45);
         fprintf (f14, "%s\n", linea14);
         }
      fgetpos (f_14, &pf_14[i]);
      fclose(f_14);
      }   /* Fine ciclo for su sch_nm (variabili sistema) */

   if (contaerr)
      {
      fclose (f14);
      remove (nomef14);

      strcpy(app,"");
      sprintf(app,"(%d errori)\00",contaerr);
      strcpy(messaggio,"Non completata la generazione di f14 ");
      strcat(messaggio,app);
      XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
      XlSetFileErrComp(NULL);

      exit(-1);
      }

/* MEMORIZZO INIZIO LISTA INGRESSI */
   /*while (getpul (linea, 150, f01))*/
   while (getpul (linea, 200, f01))
      {
      if (!strncmp (linea+10, ">>>>>>", 6))
         {
         fgetpos (f01, &pf01);
         break;
         }
      }

/* MEMORIZZO POSIZIONE LISTA VARIABILI CONNESSE */
   /*while (getpul (linea, 150, f01))*/
   while (getpul (linea, 200, f01))
      {
      if (!strncmp (linea, "****", 4))
         {
         fgetpos (f01, &pf01con); 
         break;
         }
      }


   contaerr_in = 0;
/* CONDIZIONI INIZIALI VARIABILI DI INGRESSO ===> F14.DAT */
   fprintf (f14, "*LG*CONDIZIONI INIZIALI VARIABILI DI INGRESSO (MKS)\n");
   contavar = 0;
   for(i=0;i<sch_nm;i++)   
      {
      strcpy(nomefile,"");
      strcpy(appoggio,sch_id[i]);
      minuscolo(appoggio);
      sprintf (nomefile, "%s%4.4s%s", work_path, appoggio, "_14.dat");
      /*f_14 = fopen (minuscolo (nomefile), "r");*/
      f_14 = fopen (nomefile, "r");
      if (!f_14)
         {
         fprintf (stderr,"Errore in apertura del file %s\n", nomefile);

         strcpy(messaggio,"Errore in apertura file ");
         strcat(messaggio,nomefile);
         XlOpenFileErrComp(filerr);
         XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
         XlSetFileErrComp(NULL);

         exit(-1);
         }

/*    MI RIPORTO ALLA LISTA DELLE VAR. D'INGRESSO DI _F14 */
      fsetpos (f_14, &pf_14[i]);
      /*while (getpul (linea, 150, f_14))*/
      while (getpul (linea, 200, f_14))
         {
         /*printf("%6.6s|",linea+4);*/
         if (!strncmp (linea, "*LG*DATI FISICI", 15))
            {
            fgetpos (f_14, &pf_14[i]);
            break;
            }

/*       CERCO TRA LE VARIABILI LIBERE */
         fsetpos (f01, &pf01);
         /*getpul (linea1, 150, f01);*/
         getpul (linea1, 200, f01);
         if (!strncmp (linea1, linea+4, 6))
            {
/*          VARIABILE LIBERA, LA SCRIVO IN F14 */
            sprintf (linea14," %2.2d %40.40s %6.6s %s",
               aggcount(&contavar), &linea[4], &linea1[17], &linea[45]);
            fprintf (f14, "%s\n", linea14);
            fgetpos (f01, &pf01);
            }
         else
            {
/*          VARIABILE CONNESSA, LA CERCO IN F14 */
            fsetpos (f01, &pf01con);
            /*getpul (linea1, 150, f01);*/
            getpul (linea1, 200, f01);        
            /*
            debug
            printf("\nCerco tra VARIABILI CONNESSE 01=%8.8s|\n",linea1);
            debug
            */
            if (strncmp (linea1, linea+4, 6))
              {
              fprintf(stderr,"\nErrore:variabile %6.6s non presente in f01.dat",
                       linea+4);

              contaerr_in++;
              sprintf(messaggio,
                    "Errore:variabile %6.6s non presente in f01.dati, file ",linea+4);
              strcpy(app,nomefile);
              if( estrai_nome(app, appfile)==1 )
                 {
                 printf("\nnome estratto=%s\n",appfile);
                 strcat(messaggio,appfile);
                 }
              XlOpenFileErrComp(filerr);
              XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
              XlSetFileErrComp(NULL);
/*
              exit(-1);
*/
              }
            else
               fgetpos (f01, &pf01con);
            }
         }
      }   /* Fine ciclo for su sch_nm (variabili sistema) */


/*********************************************************************
   DATI FISICI: LETTURA DESCRIZIONI DA schemi.dsc
**********************************************************************/
   nrego = 0;
/*
*/
   strcpy (nomefile,fileschemi);
/*
*/
   nrego = rd_moduletti(nomefile);

/*********************************************************************
   DATI FISICI: LETTURA DESCRIZIONI MACROMODULI DA T-.MACRO.TIP 
   if (tfile[0] == 'T')
      nrego += leggi_tip (tfile, "MACRO", g_id+nrego, g_ds+nrego);
**********************************************************************/


/***************************************************************
    DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI 
****************************************************************/

   contaerr_bl = 0;
   fprintf (f14,"*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI\n");
   for (i=0;i<sch_nm;i++)   
      {
      strcpy(appoggio,sch_id[i]);
      minuscolo(appoggio);
      sprintf (nomefile, "%s%4.4s%s", work_path, appoggio, "_14.dat");
      /*f_14 = fopen (minuscolo (nomefile), "r");*/
      f_14 = fopen (nomefile, "r");
      if (!f_14)
         {
         fprintf (stderr, "Errore in apertura del file %s", nomefile);

         strcpy(messaggio,"Errore in apertura file ");
         strcat(messaggio,nomefile);
         XlOpenFileErrComp(filerr);
         XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
         XlSetFileErrComp(NULL);

         exit(-1);
         }
      fsetpos (f_14, &pf_14[i]);
      /*while (getpul (linea, 150, f_14))*/
      while (getpul (linea, 200, f_14))
         {
         if (!strncmp (linea+20, sch_id[i], 4))
/******************************************************
   INTESTAZIONE DELLO SCHEMA
*******************************************************/
            {
            linea[20] = '\0';
            continue;
            }
         else 
/******************************************************
   SCRITTURA BLOCCHI
*******************************************************/
            {   
            fsetpos (f_14, &pf_14[i]);
            /*while (getpul (linea, 150, f_14))*/
            while (getpul (linea, 200, f_14))
               if (!strncmp (linea, "*   -", 5))
                  {
                  fprintf (f14, "%s\n", linea);               
                  /*getpul (linea, 150, f_14);*/
                  getpul (linea, 200, f_14);
                  trovato = 0;
                  for (j = 0; j < nrego; j++)
                     {
                     if (!strncmp (linea+11, g_id[j], 4))
                        {
                        trovato = 1;
                        fprintf (f14, "%23.23s %s\n", linea, g_ds[j]);
                        break;
                        }
                     }
                  if (!trovato)
                     {
                     fprintf (stderr,
                        "Errore: modulo/macro %4.4s inesistente", linea+11);
                     contaerr_bl++;
                     sprintf(messaggio,
                             "Errore(blocchi): modulo/macro %4.4s inesistente", 
                             linea+11);
                     XlOpenFileErrComp(filerr);
                     XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
                     XlSetFileErrComp(NULL);
                     }
                  }
               else if (!strcmp (linea, "*LG*EOF"))

/******************************************************
   FINE DATI DELLO SCHEMA
*******************************************************/

                  break;
               else
                  fprintf (f14, "%s\n", linea);               
            }
         }     /* Fine ciclo lettura file_14 */
      fclose(f_14);      
      }  /* Fine ciclo for su sch_nm (dati fisici e geometrici) */
fprintf(f14,"*LG*EOF\n");
fclose(f14);
fclose(f01);
if (contaerr_bl>0 )
   {
   sprintf(messaggio, "Non sono stati trovati %d moduletti ",
           contaerr_bl);
   XlOpenFileErrComp(filerr);
   XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
   }
if (contaerr_in>0 )
   {
   sprintf(messaggio, "Non sono state trovate %d variabili di ingresso",
           contaerr_in);
   XlOpenFileErrComp(filerr);
   XlErrComp(NULL,"gen_f14task","ERRORE ",messaggio);
   }
if ( (contaerr_bl>0 ) || (contaerr_in>0 ) )
   {
   strcpy(messaggio,"f01.dat ugualmente scritto");
   XlErrComp(NULL,"gen_f14task","ATTENZIONE",messaggio);
   XlSetFileErrComp(NULL);
   printf("\nFine non corretta gen_f14task\n");
   exit(-1);
   }
/*printf("\nFine corretta gen_f14task\n");*/
exit(0);
}



short aggcount (short *contavar)
{
if (*contavar == 99)
   *contavar = 0;
else
   (*contavar)++ ;
return (*contavar);
}




/*
   Legge da file la struttura descrittiva degli schemi con riferimento
   alla parte relativa ai nomi e alle descrizioni dei moduletti caricando
   i vettori g_id, g_ds.
*/
int rd_moduletti(char *input_file)
{
FILE *ff;
MODUL_LST *mod;
DESC_SCHEMA *schema;
int num_sch;
int i,k,h;
int num_reg=0;
int inserimento;

    if( (ff=fopen(input_file,"r"))==NULL )
       {
       fprintf(stderr,"gen_f14task:Non si puo' aprire il file %s\n",input_file);
       return(-1);
       }

/*  Lettura numero degli schemi */
    fread(&num_sch,sizeof(int),1,ff);

/*  Allocazioni vettori strutture */
    schema = (DESC_SCHEMA*)calloc(sizeof(DESC_SCHEMA),num_sch);
    mod = (MODUL_LST*)calloc(sizeof(MODUL_LST),num_sch);

/*  Ciclo di lettura dati singolo schema */
    for(i=0;i<num_sch;i++)
       {
       fread(&schema[i],sizeof(DESC_SCHEMA)-sizeof(ICON_INTERF*),1,ff);

/*     Assegnamento della struttura mod nel vettore */
       memcpy(&mod[i],&schema[i].mdlt,sizeof(MODUL_LST));

       if( schema[i].schm.num_icn_interf>0 )
            {
            schema[i].icn_interf = (ICON_INTERF*)calloc(sizeof(ICON_INTERF),
                                               schema[i].schm.num_icn_interf);
            fread(&schema[i].icn_interf[0],sizeof(ICON_INTERF),
                  schema[i].schm.num_icn_interf,ff);
            }
       }

/*
   Assegnamento dei nomi dei moduletti del singolo schema 
   nel vettore globale 
*/
    for(i=0;i<num_sch;i++)
       {
       for(k=0;k<mod[i].num;k++)
          {
          inserimento = 1;
          for(h=0;h<num_reg;h++)
             {
             if( !strncmp(g_id[h],mod[i].nome[k],4) )
                {
                inserimento = 0;
                break;
                }
             }
          if (inserimento)
             {
             strcpy(g_id[num_reg],mod[i].nome[k]);
             strcpy(g_ds[num_reg],mod[i].desc[k]);
             num_reg++;
             }
          }
       }
/*
       printf("num_reg=%d\n",num_reg);
       for(k=0;k<num_reg;k++)
          printf("%d)finale  NOME=%s| DESC=%s| \n",k,g_id[k],g_ds[k]);
*/

/* Liberazione delle strutture */
   for(i=0;i<num_sch;i++)
      {
      if( schema[i].schm.num_icn_interf>0 )
         {
         /*printf("%d) Libero lo schema <%d> con int_conn[0]=%s|  int_conn[1]=%s|\n",
                 i,schema[i].schm.num_icn_interf,schema[i].icn_interf[0].int_conn,
                 schema[i].icn_interf[1].int_conn);*/
         free(schema[i].icn_interf);
         }
      }
   if( schema!=NULL )
      free(schema);
   if( mod!=NULL )
      free(mod);
   fclose(ff);
   return(num_reg);
}


/*
  Non accetta nomi di direttory suoperiori agli 8 caratteri
*/
int estrai_nome(char *nome_path, char *nome_out)
{
int i;
char appoggio[200];

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
/* Fine file */
