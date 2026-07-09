/*
 *  umis - visualizza / imposta le unita' di misura LegoPST
 *
 *  Va lanciato dalla directory della simulazione: init_umis() cerca il
 *  file delle unita' in quest'ordine:
 *    1) ./uni_misc.cfg               testo per-simulazione
 *    2) ./uni_misc.dat               binario legacy per-directory
 *    3) $HOME/defaults/uni_misc.dat  binario globale per-utente
 *
 *  uso:
 *    umis -l                  lista parsabile della tabella:
 *                               #file <path> <cfg|dat>
 *                               <tipo> <lettera> <sel> <u0|u1|...> <A> <B>
 *                             (A,B = coefficienti dell'unita' selezionata:
 *                              val_visualizzato = A*val_MKS + B)
 *    umis <tipo> <unita'>     seleziona <unita'> per <tipo> e salva nel
 *                             file TESTO per-simulazione ./uni_misc.cfg
 *                             (creato se assente). <tipo> = codice esteso
 *                             (es. PRESSION) o lettera (es. P).
 *    umis -g <tipo> <unita'>  come sopra ma salva sul file da cui le
 *                             unita' sono state lette (comportamento del
 *                             dialogo Defaults di graphics).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "uni_mis.h"

extern S_UNI_MIS uni_mis[];
extern void init_umis();
extern void agg_umis();
extern int cerca_num_umis();
extern int crea_umis_cfg_locale();
extern const char *umis_file_attivo();
extern int umis_file_is_cfg();

static void rtrim(char *s)
{
int l;
for(l=strlen(s); l>0 && (s[l-1]==' '||s[l-1]=='\t'); l--)
        s[l-1]='\0';
}

static int usage()
{
fprintf(stderr,"uso:  umis -l\n");
fprintf(stderr,"      umis [-g] <tipo|lettera> <unita'>\n");
fprintf(stderr,"  -l  lista tipi di misura, unita' disponibili e selezione corrente\n");
fprintf(stderr,"  -g  salva sul file da cui le unita' sono state lette\n");
fprintf(stderr,"      (default: crea/aggiorna ./uni_misc.cfg per-simulazione)\n");
return(1);
}

/*
   lista della tabella, leggibile e parsabile (animate.tcl umis_load):
     #file <path> <cfg|dat>
     # TIPO ...                                  (intestazione colonne)
     <tipo> <lettera> <sel> <u0|[u1]|...> <A> <B>
   L'unita' SELEZIONATA e' racchiusa tra [ ]; A e B sono i coefficienti
   dell'unita' selezionata (val_vis = A*val_MKS + B).
*/
static void lista()
{
int i,j,num_umis,primo;
char um[L_NOMI_UMIS+1];
char units[16*(L_NOMI_UMIS+3)];

printf("#file %s %s\n",umis_file_attivo(),umis_file_is_cfg()?"cfg":"dat");
printf("# TIPO     L sel  unita' (selezionata tra [ ])    A[sel]     B[sel]\n");
num_umis=cerca_num_umis();
for(i=0;i<num_umis;i++)
        {
        units[0]='\0';
        primo=1;
        for(j=0;j<N_TIPI_UMIS;j++)
                {
                strncpy(um,uni_mis[i].codm[j],L_NOMI_UMIS);
                um[L_NOMI_UMIS]='\0';
                rtrim(um);
                if(um[0]=='\0')
                        continue;
                if(!primo)
                        strcat(units,"|");
                if(j==uni_mis[i].sel)
                        {
                        strcat(units,"[");
                        strcat(units,um);
                        strcat(units,"]");
                        }
                else
                        strcat(units,um);
                primo=0;
                }
        printf("%-9s %c %2d   %-30s %10.6g %10.6g\n",
               uni_mis[i].codice,uni_mis[i].type,uni_mis[i].sel,units,
               uni_mis[i].A[uni_mis[i].sel],
               uni_mis[i].B[uni_mis[i].sel]);
        }
}

int main(int argc, char **argv)
{
int i,j,itipo,isel,num_umis,global_save;
char um[L_NOMI_UMIS+1];
char *tipo,*unita;

global_save=0;
tipo=unita=NULL;
if(argc<2)
        exit(usage());

init_umis();

for(i=1;i<argc;i++)
        {
        if(strcmp(argv[i],"-l")==0)
                {
                lista();
                exit(0);
                }
        else if(strcmp(argv[i],"-g")==0)
                global_save=1;
        else if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0)
                exit(usage());
        /* NB: ogni altro token e' posizionale: l'unita' "---" inizia con '-' */
        else if(tipo==NULL)
                tipo=argv[i];
        else if(unita==NULL)
                unita=argv[i];
        else
                exit(usage());
        }
if(tipo==NULL || unita==NULL)
        exit(usage());

/* individua il tipo di misura: codice esteso o lettera singola */
num_umis=cerca_num_umis();
itipo=(-1);
for(i=0;i<num_umis;i++)
        {
        if(strcasecmp(tipo,uni_mis[i].codice)==0 ||
           (strlen(tipo)==1 && toupper(tipo[0])==(int)uni_mis[i].type))
                {
                itipo=i;
                break;
                }
        }
if(itipo<0)
        {
        fprintf(stderr,"umis: tipo di misura '%s' sconosciuto (usare 'umis -l')\n",tipo);
        exit(1);
        }

/* individua l'unita' richiesta tra quelle previste per il tipo */
isel=(-1);
for(j=0;j<N_TIPI_UMIS;j++)
        {
        strncpy(um,uni_mis[itipo].codm[j],L_NOMI_UMIS);
        um[L_NOMI_UMIS]='\0';
        rtrim(um);
        if(um[0]=='\0')
                continue;
        if(strcasecmp(unita,um)==0)
                {
                isel=j;
                break;
                }
        }
if(isel<0)
        {
        fprintf(stderr,"umis: unita' '%s' non valida per %s (usare 'umis -l')\n",unita,uni_mis[itipo].codice);
        exit(1);
        }

uni_mis[itipo].sel=isel;
if(global_save || umis_file_is_cfg())
        agg_umis();             /* salva dove init_umis ha letto */
else
        {
        if(!crea_umis_cfg_locale())     /* crea ./uni_misc.cfg per-sim */
                {
                fprintf(stderr,"umis: impossibile scrivere il file per-simulazione\n");
                exit(1);
                }
        }
strncpy(um,uni_mis[itipo].codm[isel],L_NOMI_UMIS);
um[L_NOMI_UMIS]='\0';
rtrim(um);
printf("%s -> %s  (file: %s)\n",uni_mis[itipo].codice,um,umis_file_attivo());
exit(0);
}
