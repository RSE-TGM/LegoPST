#define N000 160
#define N003 N000*50
#define N004 N000*25 
#define NXY N000*50
#define NUU N000*25
#define NDATI N000*500
#define DIMCONTROL 50
#define NUMVAR NXY+NUU
#define NUMPUNTI NUMVAR+1

typedef struct {
		float   xy[NXY];
		float   uu[NUU];
		float   dati[NDATI];
		float   cnxy[NXY];
		float   cnuu[NUU];
		int     nxy;
		int 	nuu;
		int 	ndati;
		float 	tempo;
		float 	dtint;
		char 	nomexy[NXY][9];
		char 	nomeuu[NUU][9];
		int     control[DIMCONTROL];
		char    descrxy[N003][100];
		char    descruu[N004][100];

} MEMORIASHARED;

#define MAXL 100  // massima lunghezza della linea letta dai file
#define MAXPATH 512  // massima lunghezza di un percorso di file
                     // (era MAXL: 100 caratteri non bastano, i percorsi
                     //  venivano troncati in silenzio)
#define MAXBLO 500  // massimo blocchi (era 100: LPS ne ha 134, IPS 111, e senza controllo si andava in segmentation fault)
#define MAXVAR 100   // massimo variabili per blocco
#define MAXPORTE 10  // massimo numero di porte
#define MAXIDPORTA 3  // massimo lunghezza nome della porta
#define MAXVARPORTA 20  // massimo numero di variabli per ogni porta
#define MAXFILEI5 10 // massino numero dei file i5 per ogno modulo lego
#define MAXREMARK 500   // massimo numero di annotazioni *REMARK*
#define MAXTESTO 64     // massima lunghezza del testo di un'annotazione
#define US 1
#define USC 5
#define UA 2
#define ING 3
#define CO 4  // ingresso connesso

#define LEGGI 0
#define SCRIVI 1



typedef struct {
		int  num;
		char nome[MAXVARPORTA][5]; //nome varibili della porta
		int  tipo[MAXVARPORTA]; // negativo:ingresso positivo:uscita
		int  stato; // 0 free CO connesso
		int  iconn; // indice blocco connesso alla porta 
		char bloconn[5]; // blocco connesso alla porta
		int  ibloconn; // puntatore al nome del blocco connesso
		char idbloconn[MAXIDPORTA]; // nome della porta del blocco connesso
		int  okport; // definizione incompleta : 0 ok;1
} PORTA;

typedef struct {
		char  idporta[MAXPORTE][MAXIDPORTA]; //identificatori delle porte
		int   numporte; // numero di porte
		PORTA *porta[MAXPORTE]; // variabili delle porte
} PORTE;


typedef struct {
		int  numvar;
		char firstline[80];		// Prima linea della zona blocco
		char varblo[MAXVAR][5];	// Nome variabile del blocco
		int  tipo[MAXVAR];		// 1=UA, 2=US, 3=ING, 4=CO ingresso connesso
		char varconn[MAXVAR][5];// nome della variabile della blocco di uscita di provenienza
		char bloconn[MAXVAR][5];// nome del blocco  di uscita di provenienza
		char modconn[MAXVAR][5];// nome del modulo del blocco di uscita di provenienza
		int	 posx;
		int	 posy;
		int  pag;
} BLOF01;

typedef struct {
	    char	nomemodello[9];
		int		totblo;
		char	nomemod[MAXBLO][5];
	    char	nomeblo[MAXBLO][5];
		char	descrblo[MAXBLO][70];
		char	nomelibr[MAXBLO][100];
		char    nomei5[MAXBLO][10];
		PORTE	*porte[MAXBLO];
		BLOF01	*blo[MAXBLO];
		int		totpag;
} HEADF01;


/* Annotazione di testo del disegno legocad: il record "1 *REMARK* x y testo"
   di macroblocks.dat. Nel .tom diventa un elemento @com_0 della libreria
   remark, che e' il suo equivalente. */
typedef struct {
		int  x;
		int  y;
		char testo[MAXTESTO];
		char nome[5];		/* nome dell'elemento nel .tom */
} REMARK;
