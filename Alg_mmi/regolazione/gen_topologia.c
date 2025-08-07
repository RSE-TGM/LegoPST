/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
/*
   modulo gen_topologia.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)gen_topologia.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gen_topologia.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/************************************************************************
     gen_topologia.c
     Programma per la generazione della topologia di uno schema di
                     regolazione in CAD CONTROLLO
     rielaborato e commentato per REGOMMI da S.d.I
     Acquisisce un file binario <nome>.reg e restituisce un file
     ASCII <nome>.top
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <top_icon.h>
#if defined UNIX
#include <memory.h>
#endif



extern char *OlEstrRegTaskName(char *, char *);
extern void XlOpenFileErrComp(char *);
extern void XlErrComp(char *, char *, char *, char *);
extern void XlSetFileErrComp(char *);

/*
  Dichiarazione delle variabili che vengono caricate dalla lettura
  del file di input contenete i dati topologici
*/

        char pag_ds[LUNDESC];
        int short icn_nm;         /* numero di icone */
        char icn_sigl[NICON][5];  /* sigla icona  */
        char pr_nome[NICON][9];   /* tipo icona + sigla progressiva  
						  icona (4+4) */        
        char pr_desc[NICON][LUNDESC];
        char pr_type[NICON][NVAR_IN_ICON][3];   /* tipo variabile (IN,UA,US) */
        int short pr_v_fl[NICON][NVAR_IN_ICON]; /* flag variabile connessa */
        char pr_idvin[NICON][NVAR_IN_ICON][7];  /* sigla icona collegata */

        int short pr_nuvto[NICON],  /* numero variabili totali nella icona */ 
		  pr_nuvin[NICON],  /* numero variabili di ingresso 
					nella icona */ 
                  pr_nuvot[NICON],  /* numero variabili di uscita */
                  pr_nuvdt[NICON];  /* numero variabili - dati */

        char pr_var[NICON][NVAR_IN_ICON][9];   /* nome variabile */
        char pr_vds[NICON][NVAR_IN_ICON][LUNDESC];  /* descrizione variabile */
        char pr_viorg[NICON][NVAR_IN_ICON][9]; /* nome originale variabili */

char *minuscolo (char *);
char *maiuscolo (char *);


char id_task[3];
char tag_reg[5];

/* GUAG2025: Array statici per le liste invece di allocazione dinamica */
/* Dimensionati come NICON+200 per avere una base logica con margine di sicurezza */
static char static_listai_buffer[NICON+200][100];
static char static_listau_buffer[NICON+200][100]; 
static char static_listac_buffer[NICON+200][80];

int main(argc, argv)
int argc;
char *argv[];
{
char schema[5];
int short pfix, pcix, rtd;
FILE *fp, *fopen();


int short level[NICON], iord, iordp, ford, hlevel;
char *listai[500], *listau[500], *listac[500];
int ico, ing, usc;
char icon_sigl[5];

int short i, j, k, n;
void strcop();
char stringa[80];
char cmpon[5];        
float dummy;
char dummy_char[9];

int ret;
int diff;
int ii;
char pathtask[250];
char filein[250];
char fileout[250];
char filerr[250];
char *punt=NULL;
char s1[5]={'\0'};
char s2[5]={'\0'};
char s3[5]={'\0'};


int read_topologia(char *);



        if(argc != 5)
           {
           fprintf(stderr,
                   "USO CORRETTO: gen_topologia <schema> <task_dir> <work_dir> <filerr>\n");
           exit(2);
           }
	strcpy(schema,argv[argc - 4]); 	
	strcpy(pathtask,argv[argc - 3]); 	
	strcpy(filein,argv[argc - 2]); 	
	strcpy(filerr,argv[argc - 1]); 	

        strcpy(id_task,OlEstrRegTaskName(pathtask,dummy_char));
        maiuscolo(id_task);

#if defined UNIX
	strcat(filein,"/");
#endif
        strcat(filein,schema);
	strcpy(fileout,filein); 	
        strcat(filein,".reg");
        strcat(fileout,".top");


/*      Il nome dello schema viene messo maiuscolo */
	strcpy(schema,maiuscolo(schema)); 	

/*      Se piu' corto di 4 caratteri si aggiungono degli "_" */
        diff = 4 - strlen(schema);
        for(i=0;i<diff;i++)
            strcat(schema,"_");

/*      Lettura topologia da file e controllo dell'esecuzione */
        ret = read_topologia(filein);
        if(ret<0)
            {
            XlOpenFileErrComp(filerr);
            XlErrComp(NULL,"gen_topologia","ERRORE",
                      "read_topologia: Non si puo' aprire il file <shema.reg>");
            XlSetFileErrComp(NULL);
            exit(-1);
            }

/*
        Compongo tag_reg per inserirlo a fianco del nome dello schema,
        dopo essermi assicurato che esistano delle variabili per quell'icona
        tale tag viene presa, 2 caratteri, dalle variabili e per gli altri
        2 dall'id_task
*/
        for(ii=0;ii<icn_nm;ii++)
           {
           if(pr_nuvto[ii]>0)
              break;
            }
        if(ii<icn_nm)
           {
           strncpy(tag_reg,&pr_var[ii][0][4],2);
           tag_reg[2]='\0';
           strcat(tag_reg,id_task);
           }
        else
           strcpy(tag_reg,"");

       /*
       printf("gen_topologia restituisce tag_reg=%s| (ii=%d pr_nuvto[ii]=%d)\n",
               tag_reg,ii,pr_nuvto[ii]);
       */

        for (i=0, iord=0; i<icn_nm; i++)
           {
           if(i >= NICON) {
               fprintf(stderr,"ERROR: i=%d supera NICON=%d!\n", i, NICON);
               exit(-1);
           }
           level[i] = -1;
           strcop(cmpon, 0, 4, pr_nome[i]);
           if ( (strcmp(cmpon, "DTRI")==0) || (strcmp(cmpon, "DTRL")==0) ||
                (strcmp(cmpon, "dtri")==0) || (strcmp(cmpon, "dtrl")==0)  )
              {
              iord++;
              level[i] = 0;
              }
           }

        hlevel = 0;
        int max_iterations = 1000; /* GUAG2025: Protezione contro loop infiniti */
        int iteration_count = 0;
        while (iord!=icn_nm)
           {
           iteration_count++;
           if(iteration_count > max_iterations) {
               fprintf(stderr,"ERROR: Loop infinito rilevato (iterazioni=%d), uscita forzata\n", iteration_count);
               break;
           }
           for (i=0, iordp=iord; i<icn_nm; i++)
              {
              if (level[i]==-1)
                 {
                 ford=0;
                 for (j=0; j<pr_nuvto[i]; j++)
                    if (strcmp(pr_type[i][j], "IN")==0 && pr_v_fl[i][j]==1)
                       for (k=0; k<icn_nm; k++)
                          {
                          strcop(icon_sigl, 4, 4, pr_nome[k]);
                          if (strcmp(pr_idvin[i][j], icon_sigl)==0)
                             if (level[k]==hlevel || level[k]==-1)
                                ford = -1;
                          }
                 if (ford==0)
                    {
                    iord++;
                    level[i]=hlevel;
                    }
                 }
              }
           if(iord==iordp)
              {
              if (hlevel==0 && iord>0)
                 ;
              else
                 break;
              }           
           hlevel++;
           }
        if (iord!=icn_nm)
           {
           sprintf(s1,"%d",icn_nm); 
           sprintf(s2,"%d",iord); 
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_topologia","ERRORE ","----->iord!=icn_nm<-----");
           XlErrComp(NULL,"gen_topologia","ERRORE, SCHEMA=",schema);
           XlErrComp(NULL,"gen_topologia","ERRORE, ICN_NM=",s1);
           XlErrComp(NULL,"gen_topologia","ERRORE, IORD  =",s2);
           for(k=0;k<(icn_nm-1);k++)
              {
              sprintf(s3,"%d",level[k]); 
              XlErrComp(NULL,"gen_topologia","ERRORE,CMP_ID=",pr_nome[k]);
              XlErrComp(NULL,"gen_topologia","ERRORE,CMP_DS=",pr_desc[k]);
              XlErrComp(NULL,"gen_topologia","ERRORE,CMP_ID=",s3);
              s3[0] = '\0';
              }
           XlSetFileErrComp(NULL);
           exit(-1);
           }  

/*      Apertura del file di uscita */
        fp = fopen(fileout, "w");
        if (fp == NULL)
           {
           sprintf(stringa, "Errore in apertura del file %s\n", fileout);
           XlOpenFileErrComp(filerr);
           XlErrComp(NULL,"gen_topologia","ERRORE ", stringa);
           XlSetFileErrComp(NULL);
           exit(-1);
           }
        
/*      Scrittura della parte relativa ai nomi dei blocchi */
        fprintf(fp, "****\n");
        hlevel = 0;
        iord = 0;
        while (iord!=icn_nm)
           {
           for (i=0; i<icn_nm; i++)
              {
              if (level[i]==hlevel)
                 {
                  strcop(icon_sigl, 4, 4, pr_nome[i]);
                  fprintf(fp, "%4.4s      BLOCCO (%2.2s  ) - %s\n", pr_nome[i], 
                          icon_sigl, pr_desc[i]);
                  iord++;
                 }   
              }
           hlevel++;
           }

/*      Scrittura della descrizione */
        fprintf(fp, "****\n");
        fprintf(fp, "%s%s  %s\n", schema,tag_reg, pag_ds);

/*      Scrittura in sequenza delle uscite e degli ingressi dei vari blocchi */
        hlevel = 0;
        iord   = 0;
        ico    = -1;
        ing    = -1;
        usc    = -1;
        while (iord!=icn_nm)
           {
           for (i=0; i<icn_nm; i++)
              {
              if (level[i]==hlevel)
                 {
                 fprintf(fp, "****\n");
                 strcop(icon_sigl, 4, 4, pr_nome[i]);
                 fprintf(fp, "%s  BL.-%2.2s  - **** MODREG %4.4s - %s\n", 
                         pr_nome[i], icon_sigl, pr_nome[i], pr_desc[i]);
                 for (j=0; j<pr_nuvto[i]; j++)
                    {
                    if (strcmp(pr_type[i][j], "IN") == 0)
                       {
                        if (pr_v_fl[i][j] != 0)                     
                           {
                           ico++;
                           listac[ico] = static_listac_buffer[ico];
                           sprintf(listac[ico], "%s%s  --CO-- %s", 
                                   pr_viorg[i][j],id_task, pr_vds[i][j]);

                             sprintf(pr_vds[i][j], "#%s%s#....................<===%s      BL.-%2.2s  -",
                                     pr_viorg[i][j],id_task, pr_idvin[i][j],
                                     pr_idvin[i][j]);
                            }
                         else
                            {
                            ing++;
                            listai[ing] = static_listai_buffer[ing];
                            sprintf(listai[ing], "%s%s  --IN-- %s", 
                                    pr_var[i][j],id_task, pr_vds[i][j]);
                            }
                        }
                     else
                        {
                        usc++;
                        listau[usc] = static_listau_buffer[usc];
                        sprintf(listau[usc], "%s%s  --UA-- %s", 
                                pr_var[i][j],id_task, pr_vds[i][j]);
                        }
                     fprintf(fp, "%s%s  --%s-- %s\n", pr_var[i][j],id_task,
                             pr_type[i][j], pr_vds[i][j]);
                     }
                 iord++;
                 fprintf(fp, "****\n");
                 }   
              }
           hlevel++;
           }

/*
        Elenco degli ingressi di tutti i moduli e degli ingressi collegati
        ( da liste preparate nella parte relativa ai singoli blocchi )
*/
        fprintf(fp, "****      >>>>>>INGRESSI DI TUTTI I MODULI<<<<<<\n");
 
        if (ing != -1)
           for (i=0; i<=ing; i++)
               fprintf(fp, "%s\n", listai[i]);

        fprintf(fp, "****\n");

        if (ico != -1)
           for (i=0; i<=ico; i++)
               fprintf(fp, "%s\n", listac[i]);

        fprintf(fp, "****\n");
        fprintf(fp, "****      >>>>>>ELENCO VARIABILI COMPATTATE<<<<<<\n");
        dummy = 0.;
        if (usc != -1)
           for (i=0; i<=usc; i++)
               fprintf(fp, "%8.8s   %f\n", listau[i], dummy);

        if (ing != -1)
           for (i=0; i<=ing; i++)
               fprintf(fp, "%8.8s   %f\n", listai[i], dummy);

        fprintf(fp, "\n");

        fclose(fp);
        /*printf("Fine regolare di gen_topologia, prodotto file\n <%s>\n",fileout);*/

        exit(0);
}




/*
     Copia su stra n caratteri di stringa a partire 
     dalla vlocazione i.
*/
void strcop(stra, i, n, stringa)
char stringa[];
char stra[];
int n, i;
{
int k, j;

	for (j=0, k=i; j<n; k++, j++)
           {
           stra[j] = stringa[k];
           if (k == strlen(stringa))
               break;
           }
        stra[j] = '\0';
}





/*
    Legge dal file (schema.reg) il cui nome viene passato come 
    parametro le variabili globali (definite ad inizio file)
    necessarie alla creazione del file (schema.top).
*/
#if defined OLD_READ_TOPOLOGIA
int read_topologia(char *nome_file)
{
/*  
   Variabili lette ma non utilizzate per generare la topologia.
   In questo modo il codice seguente e' identico a quello usato 
   nel file gen_f14.
*/
float pr_val[NICON][NVAR_IN_ICON];
int   fl_macro[NICON];
char  var_assoc[NICON][NVAR_IN_ICON][9];
float val_assoc[NICON][NVAR_IN_ICON];
short pr_ndat[NICON]; 
/* 
  Altre variabili.
*/  
FILE *ff=NULL;
int ret=1;
int i,j,k;
int size;
TOP_ICON rec;
char stringa[100];


    printf("gen_topologia : USO VERSIONE OLD_READ_TOPOLOGIA di read_topologia\n");
/*  Apertura del file della topologia  */
    if( !(ff=fopen(nome_file,"r")) )
       {
       fprintf(stderr,"read_topologia : Non si puo' aprire il file <%s> \n",
               nome_file);
       return(-1);
       }

/*  Lettura della descrizione della pagina di regolazione */
    fread(pag_ds,LUNDESC,sizeof(char),ff);

/*  Lettura TOP_ICON, caricamento delle variabili globali corrispondenti */
    i = 0;
    while(ret==1)
       {
       ret = fread(&rec,sizeof(TOP_ICON),1,ff);
       if(ret!=1)
         break;
       
       /* GUAG2025: Controllo limiti array per evitare buffer overflow */
       if(i >= NICON)
         {
         fprintf(stderr,"ERRORE: Troppi icone nel file %s (max %d)\n", nome_file, NICON);
         fclose(ff);
         return(-1);
         }
       
       icn_nm++;
       pr_nuvto[i] = rec.pr_nuvto;
       pr_nuvin[i] = rec.pr_nuvin;
       pr_nuvot[i] = rec.pr_nuvot;
       pr_nuvdt[i] = rec.pr_nuvdt;
       strcpy(icn_sigl[i],rec.icn_sigl);
       strcpy(pr_nome[i],rec.pr_nome);
       strcpy(pr_desc[i],rec.pr_desc);
       /* GUAG2025: Controllo limiti variabili per icona */
       if(pr_nuvto[i] > NVAR_IN_ICON)
         {
         fprintf(stderr,"ERRORE: Troppi variabili nell'icona %d del file %s (max %d)\n", i, nome_file, NVAR_IN_ICON);
         fclose(ff);
         return(-1);
         }
       
       for(k=0;k<pr_nuvto[i];k++)
          {
          pr_v_fl[i][k] = rec.pr_v_fl[k];
          strcpy(pr_type[i][k],rec.pr_type[k]);
          strcpy(pr_idvin[i][k],rec.pr_idvin[k]);
          strcpy(pr_vds[i][k],rec.pr_vds[k]);
          strcpy(pr_viorg[i][k],rec.pr_viorg[k]);
          strcpy(pr_var[i][k],rec.pr_var[k]);
          strcpy(stringa,rec.pr_val[k]);
          pr_val[i][k] = (float)atof(stringa);
/*
          Se il collegamento e' con un'icona di interfaccia 
          elimino il collegamento 
*/
          if( !strcmp(rec.pr_idvin[k],"INTE") )
             {
             printf("Resetto il flag della variabile %s collegata con icona di inetrfaccia  (icona=%d,var. numero=%d)\n",
                     pr_viorg[i][k],i,k);
             strcpy(pr_var[i][k],pr_viorg[i][k]);
             pr_v_fl[i][k] = 0;
             }
          }
       fl_macro[i] = rec.fl_macro;
       pr_ndat[i]  = rec.pr_ndat;
       
       /* GUAG2025: Controllo limiti dati per icona */
       if(pr_ndat[i] > NVAR_IN_ICON)
         {
         fprintf(stderr,"ERRORE: Troppi dati nell'icona %d del file %s (max %d)\n", i, nome_file, NVAR_IN_ICON);
         fclose(ff);
         return(-1);
         }
         
       for(k=0;k<pr_ndat[i];k++)
          {
          strcpy(var_assoc[i][k],rec.var_assoc[k]);
          strcpy(stringa,rec.val_assoc[k]);
          val_assoc[i][k] = (float)atof(stringa);
          }
       i++;
       }

    fclose(ff);
    return(1);
}
#endif

/* Fine file */
