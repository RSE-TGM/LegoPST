/**********************************************************************
*
*       C Source:               pert_show.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jan 22 11:36:33 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: pert_show.c-3 %  (%full_filespec: pert_show.c-3:csrc:1 %)";
#endif
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include "sim_param.h"
# include "sim_types.h"
# include "sim_ipc.h"
# include "comandi.h"
# include "sked.h"
#include <Rt/RtMemory.h>
#include <Rt/RtDbPunti.h>
#include <Rt/RtErrore.h>


extern int fp_com[MAX_MODEL];
extern int nmod;                /* numero di modelli */
extern char *fp_app[MAX_MODEL];
extern int *fp_size[MAX_MODEL];
extern int stato_sked;
extern int tipo_sked;
extern RtDbPuntiOggetto dbpunti;

/*
     In funzione di un vettore di perturbazioni del tipo TIPO_PERT
     lungo MAX_PERTUR e di un parametro che indica se la stampa deve 
     essere piena oppure no stampa le pert esistenti.
*/
int  MostraPert(TIPO_PERT *pert, int full, int _max_pertur)
{
int k,conta=0;
float valore=-9;


     for (k = 0; k < _max_pertur; k++)
       {
       if ( ((pert[k].tipo > 0)&&(pert[k].tipo < MAX_PERT)) &&
            (pert[k].tipo != -1) )
            {
            RtDbPGetValue(dbpunti,pert[k].indirizzo,&valore);
            if ( full==1 )
               printf("Pos = %d tipo = %d ind= %d val= %f rt= %f t= %f ->%f\n",
                    k, pert[k].tipo,
                    pert[k].indirizzo,
                    pert[k].valore_delta,
                    pert[k].rateo_durata,
                    pert[k].t, valore);
            }
       else
            {
            conta++;
/*
            printf("Pos=%d  [%d] tipo=%d ind=%d val=%f rt=%f t=%f->%f\n",
                    k,
                    pert[k].runtime_replay,
                    pert[k].tipo,
                    pert[k].indirizzo,
                    pert[k].valore_delta,
                    pert[k].rateo_durata,
                    pert[k].t, valore);
*/
            }
       }
     if (conta != _max_pertur)
        printf("\t PRESENTI %d PERTURBAZIONI\n\n",_max_pertur-conta);
     else
        printf("\t Non PRESENTI PERTURBAZIONI\n\n");
}


void MostraPertFile(char *nome)
{
TIPO_PERT perturbazione;
FILE      *fp_pert;
char nome_file[FILENAME_MAX];
int k;


         if( nome==NULL )
            strcpy(nome_file,"perturbazioni.dat");
         else
            strcpy(nome_file,nome);
         printf("Apro il file [%s] size TIIPO_PERT=%d\n",
                 nome_file,sizeof(TIPO_PERT));
         system("ls -la perturbazioni.*");

         fp_pert = fopen(nome_file,"r");
         if( fp_pert==NULL )
           exit(fprintf(stderr,"\t Manca il file[%s]\n",nome_file));
         k = 0;
         while (fread(&perturbazione, sizeof(TIPO_PERT), 1, fp_pert))
            {
            /*   fseek(fp_pert, k * sizeof(TIPO_PERT), 0);*/
         printf("\t %2d)  TIPO=%d ind=%d   t=%10.6f reteo=%f valore_delta=%f\n",
                        k+1,
                        perturbazione.tipo,
                        perturbazione.indirizzo,
                        perturbazione.t,
                        perturbazione.rateo_durata,
                        perturbazione.valore_delta);
            k++;
            }
         fclose(fp_pert);
         printf("Fine regolare con persenti %d perturbazioni.\n",k);

}

