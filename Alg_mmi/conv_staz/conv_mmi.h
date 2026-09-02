#define MAX_LUN_RIGA_ELENCO_WID		4000
#define WIDTH_COMPOSITE		DIM_UNITSTAZ
#define HEIGHT_COMPOSITE	DIM_UNITSTAZ
#define BORDER_COMPOSITE	1
/*  Colori di sfondo. I primi tre sono quelli del faceplate xstaz
    (xstaz.c, sfondo_label/sfondo_window/sfondo_staz): le pagine MMI
    convertite devono somigliare al pannello che l'operatore conosce.
    ATTENZIONE: dichiarare "background" non basta, serve sempre anche
    "inheritBackground: 0" - vedi ScriviSfondoFiglio in conv_mmi.c.  */
#define SFONDO_LABEL   "#cc00e500cb00"	/* verdino chiaro : etichette   */
#define SFONDO_WINDOW  "#cc00e500e500"	/* verdino pallido: pagina      */
#define SFONDO_STAZ    "#d500e500d500"	/* verdino grigio : stazione    */
#define SFONDO_DISPLAY "#10001c002c00"	/* blu notte      : display     */
#define FONT_PICCOLO  "fixed"
#define FONT_GRANDE   "-adobe-times-bold-r-normal--25-180-100-100-p-132-iso8859-1"

/*  Tavolozza del cambio colore: quale usare dipende dallo sfondo su cui
    l'oggetto e' disegnato - vedi ScriviCambioColore in conv_mmi.c.  */
#define CC_SU_FONDO_SCURO	0
#define CC_SU_FONDO_CHIARO	1

struct colore_stato_st{
	char *risorsa;		/* nome della risorsa colore*1 (Xl.h)	*/
	char *su_scuro;		/* valore per un oggetto su fondo scuro	*/
	char *su_chiaro;	/* valore per un oggetto su fondo chiaro*/
	};
typedef struct colore_stato_st COLORE_STATO;

struct conv_colori_st{
	char *old_colore;
	char *colore;
	char *colore_blink;
	};
typedef struct conv_colori_st CONV_COLORI;


static CONV_COLORI conv_colori[]={
	{"NERO","black","white"},
	{"BIANCO","white","black"},
	{"GIALLO","#e400c4008800","#ff00ff000000"},
	{"VERDE","#00008a000000","#0000ff000000"},
	{"ROSSO","#b30000000000","#ff0032000000"},
	{"GRIGIO","#5c005c005c00","#cb00cb00cb00"},
	{"BLU","#000000007600","#0000e500ff00"},
	{NULL,NULL,NULL}
	};

