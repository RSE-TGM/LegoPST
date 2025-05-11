/**********************************************************************
*
*       C Source:               gen_f14.c
*       Subsystem:              2
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jul  3 17:35:16 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: gen_f14.c,2 %  (%full_filespec: 2,csrc,gen_f14.c,2 %)";
#endif
/*
   modulo gen_f14.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)gen_f14.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)gen_f14.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/******************** File: GENF14.C *******************/
/* Generazione del file F14.DAT, partendo dal file.top */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <top_icon.h>

#define errore(stringa)         exit(fprintf(stderr,"Attention !!!\n  Error in process topsch: %s\n",stringa))
#define IRIGA 500
#define URIGA 500

void XlOpenFileErrComp(char *nomeFile);
void XlErrComp(char* ,char *,char *,char *);
void XlSetFileErrComp(char *);
int read_topologia(char *);

char riga[256];
char fpath[50], mpath[50];

int iflag;
char schema[5];
char descr_schema[81];

/* ---  VARIAB. RELATIVE AI BLOCCHI DELLO SCHEMA DA INSERIRE NEL FILE14 --- */

short nbloc;                 /* Numero di blocchi nel file .TOP */
short icn_nm;                /* Numero di icone del catalogo */
short icn_pt[NICON];         /* Puntatore al numero di icona corrispondente */
char blocchi[NICON][9];      /* Nome del blocco ricavato dal file .TOP */
int fl_macro[NICON];         /* Flag (1 se il blocco e` un macromodulo) */
char descr_bloc[NICON][81];  /* Descr. dei blocco ricavato dal file .TOP */
short pr_ndat[NICON];        /* Numero di dati fisici e geom. del blocco */
char var_assoc[NICON][NVAR_IN_ICON][9]; /* Nome dei dati fisici e geom. associati */
float val_assoc[NICON][NVAR_IN_ICON];   /* Valore dei dati fisici e geom. */

/* ---  VARIABILI RELATIVE AI BLOCCHI DI USCITA DA INSERIRE NEL FILE14 --- */  

char ua_variabile[URIGA][9]; /* Nome della variabile di uscita */
float ua_valore[URIGA];      /* Valore della variabile di uscita */
char ua_noto[URIGA][5];      /* Noto relativo alla variabile di uscita */
char ua_descr[URIGA][81];    /* Desc. relativa alla variabile di uscita */
short rigaua;                /* Indice delle var. di uscita del blocco */

/* ---  VARIABILI RELATIVE AI BLOCCHI DI INGRESSO DA INSERIRE NEL FILE14 --- */ 

char in_variabile[IRIGA][9]; /* Nome della variabile di ingresso */
float in_valore[IRIGA];      /* Valore della variabile di ingresso */
char in_noto[IRIGA][5];      /* Noto relativo alla variabile di ingresso */
char in_descr[IRIGA][81];    /* Desc. relativa alla variabile di ingresso */
short rigain;                /* Indice delle var. di ingresso del blocco */

/* ----------  VARIABILI DI APPOGGIO NELLA LETTURA DEL FILE .reg -----------*/
float pr_val[NICON][NVAR_IN_ICON];
char pr_nome[NICON][9];             /* Nome del blocco dello schema */
char icn_sigl[NICON][5];            /* Nome della icone del catalogo */
int pr_nuvin[NICON], pr_nuvot[NICON], pr_nuvdt[NICON];
char pr_var[NICON][NVAR_IN_ICON][9];

/* ----------  VARIABILI DI COMODO NELLA LETTURA DEL FILE .reg -----------*/
/*             Servono per avere un unico codice di lettura per
               gen_f14 e gen_topologia                                    */
int short pr_v_fl[NICON][NVAR_IN_ICON];
int short pr_nuvto[NICON];
char pr_desc[NICON][LUNDESC];
char pr_type[NICON][NVAR_IN_ICON][3];
char pr_idvin[NICON][NVAR_IN_ICON][7];
char pr_vds[NICON][NVAR_IN_ICON][LUNDESC];
char pr_viorg[NICON][NVAR_IN_ICON][9];
/* ------------------- PROTOTIPI DELLE FUNZIONI ------------------------- */

int leggi_top (char *);
int leggi_reg_inf (char *);
int gen_lg3reg (char *, char *);
void f14_variabili (FILE *f14);
void crea_struttura (char *variabile, char *descr, char *noto);
void blocco_reg (FILE *f14, int i);
int blocco_mac (FILE *f14, int i);
char *minuscolo (char *);
char *maiuscolo (char *);
char *copia_n (char *, char *, int );
char *getpul (char *, int, FILE *);

/* ------------------- INIZIO DEL MAIN ------------------------- */

char tag_reg[5];     /* progessivo dello schema e tag della task
                        letti da schema.top per non avere in f14
                        il nome dello schema duplicato */

int main (int argc, char *argv[])
{
int i;
FILE *f14; /* Puntatore al file di output (f14.dat) */

char stringa[200]={'\0'};
char file_top[200];
char file_reg[200];
char file_err[200];
char path[200];
char lib_path[200];
char *punt;
char *ppunt;
int ret;



      if (argc != 5)
         {
         fprintf(stderr,
            "USO CORRETTO: genf14 <schema> <lib_path> <path> <file_err>\n");
         exit(2);
         }
      strcpy(schema,argv[1]);
      strcpy(lib_path,argv[2]);
      strcpy(path,argv[3]);
      strcpy(file_err,argv[4]);
      strcpy(file_reg,path);
#if defined UNIX
      strcat(file_reg,"/");
#endif
      strcat(file_reg,schema);
      strcpy(file_top,file_reg);
      strcat(file_reg,".reg");
      strcat(file_top,".top");

/************   PROVA
float in_valore[IRIGA];  
for(i=0;i<IRIGA;i++)
	in_valore[i]= -9.0;
********/

      iflag = 1;

/*    Lettura del file <path/file>.top */
      ret = leggi_top(file_top);
      if ( ret<0 ) 
         {
         sprintf(stringa,
                 "Errore lettura file topologico <%s> (leggi_top)",file_top);
         XlOpenFileErrComp(file_err);
         XlErrComp(NULL,"gen_f14","ERRORE ", stringa);
         XlSetFileErrComp(NULL);
         exit(-1);
         }

/*    Lettura del file <path/file>.reg */
      ret = leggi_reg_inf (file_reg);
      if ( ret<0 ) 
         {
         sprintf(stringa,
                 "Errore lettura file <%s> (leggi_top)",file_reg);
         strcpy(stringa,"Routine leggi_reg_inf");
         XlOpenFileErrComp(file_err);
         XlErrComp(NULL,"gen_f14","ERRORE ", stringa);
         XlSetFileErrComp(NULL);
         exit(-1);
         }

/*    Apertura f14.dat */
      strcpy(stringa,path);
#ifdef UNIX
      strcat(stringa,"/");
#endif
      strcat(stringa,"f14.dat");
      f14 = fopen (stringa, "w");

      if (!f14)
         {
         strcpy(stringa,"Non si puo' aprire il file f14.dat");
         XlOpenFileErrComp(file_err);
         XlErrComp(NULL,"gen_f14","ERRORE ", stringa);
         XlSetFileErrComp(NULL);
         exit(-1);
         }

/*    Intestazione del file F14.DAT (con normalizzazioni di default) */
      fprintf (f14, "*LG*NOME IMPIANTO =%s\n", schema);
      fprintf (f14, "*LG*DATI DI NORMALIZZAZIONE P0=      *H0=      "); 
      fprintf (f14, "*W0=      *T0=      *R0=      *\n");
      fprintf (f14, "*LG*                        L0=      *V0=      ");
      fprintf (f14, "*DP0=      *\n");

/*    Scrittura delle variabili di uscita e dei valori relativi nel F14.DAT */
      f14_variabili (f14);

/*    Scrittura dei dati fisico/geometrici e dei relativi valori nel F14.DAT */
      fprintf (f14,
            "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI\n");
/*    Non scrive piu' il nome del blocco duplicato ma accoda tag_reg */
      /*fprintf (f14, "*LG*DATI DEL BLOCCO %s%s ", schema, schema);*/
      fprintf (f14, "*LG*DATI DEL BLOCCO %s%s ", schema, tag_reg);
      fprintf (f14, " REGOL. %s - %s\n", schema, descr_schema);

      for (i=0; i<nbloc; i++)
         {
         fprintf(f14, "*  %s\n", descr_bloc[i]);
         if (fl_macro[icn_pt[i]])
            {
            printf ("Gestisco il blocco_mac\n");
            ret = blocco_mac (f14, i);
            if ( ret==-1 )
               {
               sprintf (stringa,"Manca il file %s%-4.4sinfo.dat", mpath, blocchi[i]);
               XlOpenFileErrComp(file_err);
               XlErrComp(NULL,"gen_f14","ERRORE ", stringa);
               XlSetFileErrComp(NULL);
               exit(-1);
               }
            }
         else
            {
            blocco_reg (f14, i);
            }
         }

/*    Aggiunta riga terminatrice e chiusura del file F14.DAT */
      fprintf (f14, "*LG*EOF\n");
      fclose (f14);
      /*fprintf(stderr,"Generato il file f14.dat nel direttorio\n %s\n",path);*/

/*    Genera il file lg3_f */
      ret = gen_lg3reg (lib_path,path);
      if( ret != 1 )
        {
        strcpy(stringa,"Non si genera il file lg3reg.f");
        XlOpenFileErrComp(file_err);
        XlErrComp(NULL,"gen_f14","ERRORE ", stringa);
        XlSetFileErrComp(NULL);
        exit(-1);
        }

      exit(0);
}
/*       ------------------- FINE DEL MAIN -------------------------      */




/*
   Legge il file filetopologia caricando le variabili globali :
   nbloc, blocchi, descr_bloc, ua_variabile, ua_noto, ua_descr,
   riguana, in_variabile, in_noto, in_descr, rigain
*/
int leggi_top (char *input_file)
{
FILE *top;
char *prig = riga;
char *car;
int cont_ast = 0;
int flag_in = 0;


/*top = fopen(minuscolo(input_file), "r");*/
top = fopen(input_file, "r");


if (!top)
    {
    fprintf(stderr,"leggi_top : Non si puo' aprire il file %s\n",input_file);
    return(-1);
    }

while (getpul (riga, 255, top))
   if (riga[0] == '*')
      {
      cont_ast++;
      if (cont_ast > 2 && riga[12] == '>')
         flag_in++;
      if (flag_in >= 2)
         break;
      }
   else if (cont_ast == 2) /* Riga con il nome dello schema */
      {
      copia_n (schema, maiuscolo(riga), 4);
      copia_n (tag_reg,maiuscolo(riga+4), 4);
      /*
      printf("gen_f14 ha letto per tag_reg=%s|\n",tag_reg);
      */
      copia_n (descr_schema, riga+10, 80);
      }
   else if (cont_ast > 2 && riga[12] == '.') /* Riga con nome blocco */
      {
      copia_n (blocchi[nbloc], riga, 8);
      copia_n (descr_bloc[nbloc], riga+36, 80);
      nbloc++;
      }
   else if (flag_in == 1 && riga[12] == 'I') /* Riga con --IN-- */
      {
      crea_struttura (in_variabile[rigain], in_descr[rigain], in_noto[rigain]);
      rigain++;
      }
   else if (riga[12] == 'U') /* Riga con --UA-- */
      {
      crea_struttura (ua_variabile[rigaua], ua_descr[rigaua], ua_noto[rigaua]);
      rigaua++;
      }


fclose (top);
return(1);
}


/*
     Legge le variabili mancanti dal file .reg
*/
int leggi_reg_inf (char *nomefile)
{
short pfix, pcix;
int i, j, k, ret;
int pr_nuvar;
char var_nam[9], seg_nam[9];



/*     Lettura del file */
       ret = read_topologia(nomefile);
       if( ret<0 )
          return(-1);

/*     Ciclo su tutti i blocchi dello schema */
       for (i=0; i<nbloc; i++)
          {
          icn_pt[i] = -1;
          for (j=0; j<icn_nm; j++)
             {
             if (! strncmp (blocchi[i], pr_nome[j], 8))
                {
                /*
                printf("%d)  blocchi=%s=pr_nome  (j=%d)\n",
                       i, blocchi[i],j);
                */
                icn_pt[i] = j;
                break;
                }
             }
          if (icn_pt[i] < 0)
             {
             fprintf(stderr,"Blocco %s non ritrovato fra le icone\n",
                     blocchi[i]);
             return(-1);
             }
/*
          ATTENZIONE pr_var e pr_val, sono gia' letti VETTORIALMENTE dal file;
          Devo usare il vettore di svincolo icn_pt perche' il vettore blocchi 
          e' allineato (dal file .top) mentre i vettori su file.reg non lo sono.
*/
          strcpy (seg_nam, icn_sigl[icn_pt[i]]);
          pr_nuvar = pr_nuvot[icn_pt[i]] + pr_nuvin[icn_pt[i]] + 
                     pr_nuvdt[icn_pt[i]];
          /*
          printf("pr_nuvot[icn_pt[i]]=%d pr_nuvin[icn_pt[i]]=%d pr_nuvdt[icn_pt[i]]=%d\n",
                  pr_nuvot[icn_pt[i]],pr_nuvin[icn_pt[i]],pr_nuvdt[icn_pt[i]]);
          */

/*        Caricamento dei valori di uscita */
          for (j = 0; j < pr_nuvot[icn_pt[i]]; j++)
             {
             sprintf (var_nam, "%-8.8s", pr_var[icn_pt[i]][j]);
             for (k = 0; k < rigaua; k++)
                if (!strncmp (var_nam, ua_variabile[k],6))
                   {
                   /*
                   printf("ua_variabile[%d]=%s| pr_val=%f\n",
                           k,ua_variabile[k],pr_val[icn_pt[i]][j]);
                   */
                   ua_valore[k] = pr_val[icn_pt[i]][j];
                   break;
                   }
             }
/*        Caricamento dei valori di ingresso */
          for (j = pr_nuvot[icn_pt[i]]; j < pr_nuvar; j++)
             {
             sprintf (var_nam, "%-8.8s", pr_var[icn_pt[i]][j]);
             for (k = 0; k < rigain; k++)
                if (!strncmp (var_nam, in_variabile[k],6))
                   {
                   /*
                   printf("in_variabile[%d]=%s| pr_val=%f\n",
	                   k,in_variabile[k],pr_val[icn_pt[i]][j]);
                   */
                   in_valore[k] = pr_val[icn_pt[i]][j];
                   break;
                   }
             }
          } /* Fine ciclo */
       return(1);
}



/*
   Genera il file lg3reg.f 
*/
int gen_lg3reg (char *path_lg3, char *path_work)
{
int diff;
int i;
char *car;
char nome_in[FILENAME_MAX];
char nome_out[FILENAME_MAX];
char schema_[10];
FILE *lg3;
FILE *lg3o;

   strcpy(nome_in,path_lg3);
   strcpy(nome_out,path_work);
#ifdef UNIX
   strcat(nome_in,"/libreg/");  /* A seguito di questa modifica va 
                                   cambiato per VMS */
   strcat(nome_out,"/");
#endif
   strcat(nome_in,"lg3_reg.dat");
#ifdef UNIX
   strcat(nome_out,"lg3reg.f");
#endif
#ifdef VMS
   strcat(nome_out,"lg3reg.for");
#endif

   lg3 = fopen (nome_in, "r");
   lg3o = fopen (nome_out, "w");

if (!lg3)
   {
   fprintf(stderr,"Errore apertura file %s, non si genera %s\n",
           nome_in,nome_out);
   return(-1);
   }
if (!lg3o)
   {
   fprintf(stderr,"Errore in apertura file %s\n",
           nome_out);
   return(-1);
   }

/* Se lo schema e' piu' corto di 4 caratteri si aggiungono "_" */
strcpy(schema_,schema);
diff = 4 - strlen(schema_);
for(i=0;i<diff;i++)
    {
    strcat(schema_,"_");
    }
while (car = getpul(riga, 255, lg3))
   {
   for ( ; *car; car++)
      if (*car == '#' && strlen(car) >= 4)
         strncpy (car, schema_, 4);
   fprintf (lg3o, "%s\n", riga);
   }

fclose (lg3);
fclose (lg3o);
/*
   fprintf(stderr,"Generato il file fortran %s\n",nome_out);
*/
return(1);
}



void f14_variabili (FILE *f14)
{
int i = 0;
char blocco[9];
char stringa[50];

/* Blocco non duplica piu il nome ma vi accoda tag_reg */
/*sprintf (blocco, "%s%s", schema, schema);*/
sprintf (blocco, "%s%s", schema, tag_reg);

fprintf (f14, "*LG*CONDIZIONI INIZIALI VARIABILI DEL SISTEMA (MKS)\n");
for (i = 0; i < rigaua; i++)
   {
   strcpy(stringa,"");
   sprintf(stringa,"%-10.10f",ua_valore[i]);
   stringa[10]='\0';

   fprintf (f14, " %2d %s =", (i + 1) % 100, ua_variabile[i]);

   if( !strncmp(stringa,"INF",3) )
      {
      fprintf(f14, "%s", stringa);
      }
   else
      {
      if( !strncmp(stringa,"NaNQ",4) )
          {
          fprintf(f14, "%s", stringa);
          }
      else
          {
          if (ua_valore[i] < 0)
              fprintf(f14, "%1.3E", ua_valore[i]);
           else
              fprintf(f14, "%1.4E", ua_valore[i]);
          }
      }
   fprintf(f14, "*BL.(%s)=%s* %s\n", blocco, ua_noto[i], ua_descr[i]);
   }

fprintf(f14,"*LG*CONDIZIONI INIZIALI VARIABILI DI INGRESSO\n");
for (i = 0; i < rigain; i++)
   {
   fprintf (f14, " %2d %s =", (i + 1) % 100, in_variabile[i]);
   if (in_valore[i] < 0)
      fprintf(f14, "%1.3E", in_valore[i]);
   else
      fprintf(f14, "%1.4E", in_valore[i]);
   fprintf(f14, "*BL.(%s)=%s* %s\n", blocco, in_noto[i], in_descr[i]);
   }
}



void crea_struttura (char *variabile, char *descr, char *noto)
{
copia_n (variabile, riga, 8);
copia_n (descr, riga+17, 80);
if (riga[12] == 'I')
   strcpy (noto, "NOTO");
else
   strcpy (noto, "    ");
}



void blocco_reg (FILE *f14, int i)
{
int j;
int gf = ( (!strncmp(blocchi[i], "GFUN", 4)) ||
           (!strncmp(blocchi[i], "gfun", 4))    );

fprintf(f14, "*   BLOCCO %-6.6s %3d%s\n", blocchi[i], i+1, descr_bloc[i]);
for (j = 0; j < pr_ndat[icn_pt[i]]; j++)
   {
   fprintf (f14, "    %-6.6s%-2.2s =", var_assoc[icn_pt[i]][j], blocchi[i]+4);
   if (val_assoc[icn_pt[i]][j] < 0)
      fprintf (f14, "%1.3E*", val_assoc[icn_pt[i]][j]);
   else
      fprintf (f14, "%1.4E*", val_assoc[icn_pt[i]][j]);
   if ((j + gf) % 2)
      fprintf (f14, "\n");
   }
if ((pr_ndat[icn_pt[i]] + gf) % 2)
   fprintf (f14, "\n");
}



int blocco_mac (FILE *f14, int i)
{
char minfo[80];
FILE *isp;
int flag1 = 0;
int ndato = 0;
char valore[20];

sprintf (minfo, "%s%-4.4sinfo.dat", mpath, blocchi[i]);
/*isp = fopen (minuscolo(minfo), "r");*/
isp = fopen (minfo, "r");
if (!isp)
    {
    fprintf(stderr,"blocco_mac : Non si puo' aprire il file %-4.4sinfo.dat\n",
            blocchi[i]);
    return(-1);
    }

fprintf(f14, "*   MACMOD %-6.6s %3d%s\n", blocchi[i], i+1, descr_bloc[i]);

while (getpul (riga, 255, isp))
   {
   if (riga[0] == '*')
      {
      flag1++;
      if (flag1 < 2)
         continue;
      }
   else
      {
      if (flag1 < 2)
	 continue;
      strncpy (riga+10, blocchi[i]+4, 2);
      strncpy (riga+35, blocchi[i]+4, 2);

      if (!strncmp (riga+4, var_assoc[icn_pt[i]][ndato], 6))
         {
         if (val_assoc[icn_pt[i]][ndato] < 0)
            sprintf (valore, "%1.3E     ", val_assoc[icn_pt[i]][ndato]);
         else
            sprintf (valore, "%1.4E     ", val_assoc[icn_pt[i]][ndato]);
         strncpy (riga+14, valore, 10);
         ndato++;
         }
      if (strlen(riga) > 25 && !strncmp (riga+29, var_assoc[icn_pt[i]][ndato], 6))
         {
         if (val_assoc[icn_pt[i]][ndato] < 0)
            sprintf (valore, "%1.3E     ", val_assoc[icn_pt[i]][ndato]);
         else
            sprintf (valore, "%1.4E     ", val_assoc[icn_pt[i]][ndato]);
         strncpy (riga+39, valore, 10);
         ndato++;
         }
      }
   fprintf (f14, "%s\n", riga);
   }
fclose (isp);
return(1);
}





/*
   Legge il file nome_file.reg caricando le variabili necessarie 
   alla costruzione del file f14.

   Attenzione qui non deve esserci come in gen_topologia il controllo 
   su INTE relativamente a rec.pr_idvin. 
   Le linee di codice non indentate sono relative a variabili lette ma
   non utilizzate per generare il file f14.dat.
   In questo modo il codice seguente e' identico a quello usato
   nel file gen_topologia.
*/
#if defined OLD_READ_TOPOLOGIA
int read_topologia(char *nome_file)
{
FILE *ff=NULL;
int ret=1;
int i,j,k;
int size;
TOP_ICON rec;
char stringa[100];
int num_var;



    printf("gen_f14: USA LA VERSIONE OLD_READ_TOPOLOGIA per read_topologia\n");
/*  Apertura del file della topologia  */
    if( !(ff=fopen(nome_file,"r")) )
       {
       fprintf(stderr,"read_topologia : Non si puo' aprire il file %s\n",
               nome_file);
       return(-1);
       }

/*  Lettura della descrizione della pagina di regolazione */
    fread(stringa,LUNDESC,sizeof(char),ff);

/*  Lettura TOP_ICON, caricamento delle variabili globali corrispondenti */
    i = 0;
    while(ret==1)
       {
       ret = fread(&rec,sizeof(TOP_ICON),1,ff);
       if(ret!=1)
         break;
       icn_nm++;
       pr_nuvto[i] = rec.pr_nuvto;
       pr_nuvin[i] = rec.pr_nuvin;
       pr_nuvot[i] = rec.pr_nuvot;
       pr_nuvdt[i] = rec.pr_nuvdt;
       strcpy(icn_sigl[i],rec.icn_sigl);
       strcpy(pr_nome[i],rec.pr_nome);
                    strcpy(pr_desc[i],rec.pr_desc);
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
/************* PROVA MARCELLO  */
          if( !strcmp(rec.pr_idvin[k],"INTE") )
             {
             printf("gen_f14: Resetto il flag della variabile %s collegata con icona di inetrfaccia  (icona=%d,var. numero=%d)\n",
                     pr_viorg[i][k],i,k);
             strcpy(pr_var[i][k],pr_viorg[i][k]);
             pr_v_fl[i][k] = 0;
             }
/************* FINE PROVA MARCELLO  */
          }
       fl_macro[i] = rec.fl_macro;
       pr_ndat[i]  = rec.pr_ndat;
/*printf("%d)  pr_nome=%s  pr_ndat=%d\n",i,pr_nome[i],pr_ndat[i]);*/
       for(k=0;k<pr_ndat[i];k++)
          {
          strcpy(var_assoc[i][k],rec.var_assoc[k]);
/*printf("   [%d] var_assoc=%s\n",k,var_assoc[i][k]);*/
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
