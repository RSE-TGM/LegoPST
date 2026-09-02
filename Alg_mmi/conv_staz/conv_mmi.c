#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>

#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "conv_mmi.h"
#include "compstaz.h"


#define MAX_NOME_FILE 512

extern char dir_uscita[];      /* opzione -d: dove scrivere le pagine */
extern FILE *fp_staz[MAX_PAG];
extern int numero_el[MAX_PAG];
extern int last_num[MAX_PAG];
extern int max_x[MAX_PAG];
extern int max_y[MAX_PAG];
extern char nomi_ogg_pag[MAX_PAG][MAX_LUN_RIGA_ELENCO_WID];

/*  Sfondo "vuoto": nessun oggetto disegnato, bounding box degenere. Serve
    perche' la catena di raccolta (kMakeGlobpages/Link_rtf) collega SEMPRE anche
    il .bkg accanto al .rtf, e l'MMI lo legge: senza, resta un link penzolante e
    a video compare "Errore lettura file di background". Per un faceplate lo
    sfondo vero e' il colore dichiarato in *drawing_background, quindi un .bkg
    vuoto e' esattamente cio' che serve.  */
static void ScriviBkgVuoto(char *nome)
{
char nome_file[MAX_NOME_FILE];
FILE *fp;

	if (dir_uscita[0])
		sprintf(nome_file,"%s/%s.bkg",dir_uscita,nome);
	else
		sprintf(nome_file,"%s.bkg",nome);
	if ((fp=fopen(nome_file,"w")) == NULL)
	{
		printf("ATTENZIONE: non riesco a creare %s\n",nome_file);
		return;
	}
	fprintf(fp,"x_min_d 10000\n");
	fprintf(fp,"y_min_d 10000\n");
	fprintf(fp,"x_max_d -10000\n");
	fprintf(fp,"y_max_d -10000\n");
	fprintf(fp,"num_d 0\n");
	fclose(fp);
}

int ApriFileMMI(int num,char* nome,char *descrizione)
{
char nome_file[MAX_NOME_FILE];

	if (dir_uscita[0])
		sprintf(nome_file,"%s/%s.pag",dir_uscita,nome);
	else
		sprintf(nome_file,"%s.pag",nome);
	printf("Apro il file %s numero %d\n",nome_file,num+1);

	fp_staz[num]=fopen(nome_file,"w");
	if (fp_staz[num] == NULL)
	{
		printf("ATTENZIONE: non riesco a creare %s\n",nome_file);
		exit(1);
	}
	numero_el[num]=0;
	nomi_ogg_pag[num][0]=0;
	fprintf(fp_staz[num],"*top_descrizione:    %s\n",descrizione);
	fprintf(fp_staz[num],"*top_x: 0\n");
	fprintf(fp_staz[num],"*top_y: 0\n");
	fprintf(fp_staz[num],"*top_width: %d\n",
			(max_x[num]+3)*WIDTH_COMPOSITE);
	fprintf(fp_staz[num],"*top_height: %d\n",
			(max_y[num]+2)*HEIGHT_COMPOSITE);
	fprintf(fp_staz[num],"*top_tipo: Stazioni\n");
/*  Risorse che kWinContext/kGlobContext estraggono dalla pagina con un grep
    per comporre il Context: senza, la riga nel Context esce monca e la
    pagina non si aggiorna. Valori come nei template di libut_mmi.  */
	fprintf(fp_staz[num],"*refresh_freq:\t10\n");
	fprintf(fp_staz[num],"*schemeInUse:\t0\n");
	fprintf(fp_staz[num],"*tagPag:\t\n");
	fprintf(fp_staz[num],"*drawing_width: %d\n",
			(max_x[num]+3)*WIDTH_COMPOSITE);
	fprintf(fp_staz[num],"*drawing_height: %d\n",
			(max_y[num]+2)*HEIGHT_COMPOSITE);
	fprintf(fp_staz[num],"*drawing_background: %s\n",SFONDO_WINDOW);
	ScriviBkgVuoto(nome);
	return(0);
}

void ChiudiFileMMI()
{
int i;

for(i=0;i<MAX_PAG;i++)
	if(fp_staz[i]!=NULL)
		{
		fprintf(fp_staz[i],
			"*elenco_wid0: %s\n",nomi_ogg_pag[i]);
		fprintf(fp_staz[i],
			"*num_widget: %d\n",numero_el[i]);
		fprintf(fp_staz[i],
			"*nextnum: %d\n",last_num[i]+1);
		fclose(fp_staz[i]);
		}
}

void AggiungiOggetto(int pagina,int num)
{
sprintf(nomi_ogg_pag[pagina],"%s %dw XlComposite",nomi_ogg_pag[pagina],num);
last_num[pagina]=num;
++numero_el[pagina];
}


int ScriviComposite(int pagina, int num, int x, int y, int width, int height)
{

y=max_y[pagina]-y-height+2;

fprintf(fp_staz[pagina],"*%dw.x0:    %d\n",num,x*WIDTH_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.y0:    %d\n",num,y*HEIGHT_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.width0:    %d\n",num,
		width*WIDTH_COMPOSITE-2*BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.height0:    %d\n",num,
		height*HEIGHT_COMPOSITE-2*BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.borderWidth:    %d\n",num,BORDER_COMPOSITE);
fprintf(fp_staz[pagina],"*%dw.background: %s\n",num,SFONDO_STAZ);
fprintf(fp_staz[pagina],"*%dw.inheritBackground: 0\n",num);


}

int RegistraElencoFigliComposite(int pagina,  int num, int num_figli,
	char *elenco_figli)
{
fprintf(fp_staz[pagina],"*%dw.numFigli:   %d\n",num,num_figli);
fprintf(fp_staz[pagina],"*%dw.listChildren:   %s\n",num,elenco_figli);
}

char *RetColore(char *old)
{
int j;
   for (j=0; conv_colori[j].old_colore!=NULL; j++)
        if (!strcmp(old,conv_colori[j].old_colore))
                break;
return(conv_colori[j].colore);
}

/*  Scrive lo sfondo di un oggetto figlio di una stazione.
    La riga inheritBackground NON e' facoltativa: la risorsa vale 1 per
    default (XlCore.c e XlManager.c, lista resources[]) e la Initialize
    del widget copia il background del PADRE sopra quello letto dal file
    (XlCore.c "se inhertiBackground == 1 setta il background del padre").
    Senza spegnerla, "background" e' una risorsa morta: tutta la pagina
    finisce al colore di drawing_background e i colori qui sotto non si
    vedono mai.  */
void ScriviSfondoFiglio(int pagina,int num_w,int cont_f,char *colore)
{
fprintf(fp_staz[pagina],"*%dw%dc.background: %s\n",num_w,cont_f,colore);
fprintf(fp_staz[pagina],"*%dw%dc.inheritBackground: 0\n",num_w,cont_f);
}

/*  Tavolozza del cambio colore.
    Il colore delle cifre di un display non e' normFg: e' il GC scelto dai
    flag del punto (XlWidgetUtil.c, XlFlagToGC) fra le risorse colore*1.
    Se non le scriviamo valgono i default di Xl - giallo, verde, ciano,
    blu, magenta, rosso - pensati per fondo scuro: su fondo chiaro il
    giallo su verdino da' un rapporto di contrasto di 1.2, cioe' invisibile.
    Quindi due tavolozze, scelte in base allo sfondo dell'oggetto.  */
static COLORE_STATO colori_stato[]={
	/* risorsa                   su fondo scuro   su fondo chiaro */
	{"coloreStimato1",         "#ffffffff0000","#7a005c000000"},
	{"coloreBassoAlto1",       "#0000ffff0000","#000064000000"},
	{"coloreAutomatico1",      "#0000ffffffff","#00006a006a00"},
	{"coloreFuoriScansione1",  "#666699009900","#00000000b300"},
	{"coloreFuoriAttendib1",   "#ffff6666ffff","#8b0000008b00"},
	{"coloreBassissimo1",      "#ffff55555555","#b30000000000"},
	{"coloreBassissimoBasso1", "#ffffffff0000","#7a005c000000"},
	{"coloreAltoAltissimo1",   "#ffffffff0000","#7a005c000000"},
	{"coloreAltissimo1",       "#ffff55555555","#b30000000000"},
	{"coloreDigSet1",          "#ffff55555555","#b30000000000"},
	{NULL,NULL,NULL}
	};

void ScriviCambioColore(int pagina,int num_w,int cont_f,int fondo)
{
int j;

for (j=0; colori_stato[j].risorsa!=NULL; j++)
	fprintf(fp_staz[pagina],"*%dw%dc.%s: %s\n",num_w,cont_f,
		colori_stato[j].risorsa,
		(fondo==CC_SU_FONDO_CHIARO) ? colori_stato[j].su_chiaro
					    : colori_stato[j].su_scuro);
}

char *RetColoreBlink(char *old)
{
int j;
   for (j=0; conv_colori[j].old_colore!=NULL; j++)
        if (!strcmp(old,conv_colori[j].old_colore))
                break;
return(conv_colori[j].colore_blink);
}

char *CostruisciRigaInput(char *var, char *mod,int neg)
{
/* `ret' DEVE essere static: la funzione ne restituisce l'indirizzo al
   chiamante, che ci fa sopra una strcpy. Da automatica, era un puntatore a
   uno stack frame gia' distrutto: funzionava per caso con i compilatori
   dell'epoca, con la libc di oggi e' un SIGSEGV dentro strcpy. Tutte le
   chiamate hanno la forma strcpy(dest, CostruisciRiga...()), una per
   espressione, quindi un solo buffer basta. */
static char ret[100];
	
if(neg==0)
	sprintf(ret,"%s BLOCCO %s NOP 1.0 0.0 ---",var,mod);
else
	sprintf(ret,"%s BLOCCO %s NOT 1.0 0.0 ---",var,mod);


return(ret);
}

char *CostruisciRigaOutput(char *var, char *mod,char *pert,char *val)
{
static char ret[100];   /* static per lo stesso motivo di CostruisciRigaInput */
float app_float;

if(val==NULL)
	app_float=0;
else
	app_float=atof(val);
	
if(strcmp(pert,"STEP")==0)
	sprintf(ret,"%s BLOCCO %s PERT_SCALINO %f 0.0 1.0 0.0 ---",
		var,mod,app_float);
if(strcmp(pert,"IMPULSO")==0)
	sprintf(ret,"%s BLOCCO %s PERT_IMPULSO 1.0 0.0 1.0 0.0 ---",var,mod);
if(strcmp(pert,"NEGAZIONE")==0)
	sprintf(ret,"%s BLOCCO %s PERT_NOT 0.0 0.0 1.0 0.0 ---",var,mod);
if(strcmp(pert,"UP_DOWN")==0)
	sprintf(ret,"%s BLOCCO %s PERT_UP_DOWN 0.0 0.0 1.0 0.0 ---",var,mod);


return(ret);
}

void CercoPosMax(FILE *fp)
{
char riga [80];
int lun;
int nriga=0;
STRIN_ST string[10];
int pagina;
int x,y;
int nstr;

for (;;)
  {
  legge_riga( riga, &lun, &nriga);
  if(strncmp(riga,"END_OF_FILE",11)==0)
	break;
  if(strncmp(riga,"PAGINA",6)==0)
	{
        separa_str( riga, lun, nstr=2, string);
	if(string[1].stringa == NULL)
		continue;
	pagina = atoi (string[1].stringa);
        legge_riga( riga, &lun, &nriga);
        separa_str( riga, lun, nstr=3, string);
	x=atoi(string[1].stringa);
	y=atoi(string[2].stringa);
	printf("PAGINA = %d   pos_x=%d pos_y=%d\n",pagina,x,y);
        if(max_x[pagina-1]<x)
		max_x[pagina-1]=x;
        if(max_y[pagina-1]<y)
		max_y[pagina-1]=y;
	}
  }
rewind(fp);
}
