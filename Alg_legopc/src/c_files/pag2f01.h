/*********************************************************
 *
 * pag2f01.h
 *
 * Creato:	     3 Febbraio 1995
 * Modificato:  27 Ottobre  1998
 * scritto da:  Marco Bonasoro
 *
 * descrizione:
 * file header per l'applicazione pag2f01, un
 * prototipo di convertitore dal formato .pag al f01.dat.
 * Contiene la definizione di costanti e i typedefs
 *
 * incluso in:
 *   pag2f01.c
 *   i32i5.c
 *
 *********************************************************/


#define PAGLINE			-1	/* .pag line with page item */
#define MODLINE			1	/* .pag line with module item */
#define PORTLINE			2	/* .pag line with port item */

#define MAXNMATCFG		11	/* num of math module configurations */
#define SYSTEM_CONF		0	/* let the system decide: take base */
#define BASE_MAT_CONF		1	/* 'blank' conf. */
#define PLUS_MAT_CONF		2	/* '+' conf. */
#define MINUS_MAT_CONF		3	/* '-' conf. */

// Guag mar 2008 - aumentato il numero dei blocchi
// #define MAXNMODULE		256	/* entries in modlist */
// #define MAXNFPORT			64	/* entries in fportlist */
// #define MAXNMATVAR		64	/* entries in mvlist */
// #define MAXNFISVAR		16	/* max num of physical vars per module */

#define MAXNMODULE		400	/* entries in modlist */
#define MAXNFPORT			100	/* entries in fportlist */
#define MAXNMATVAR		100	/* entries in mvlist */
#define MAXNFISVAR		25	/* max num of physical vars per module */

#define MODNAMENCH		8	/* module name num characters */
#define MODPAGNAMENCH		6	/* module name in .pag num characters */
#define FULLMODNAMEPREFLEN	4	/* length in char of module name prefix */
#define MVNAMENCH			8	/* math var name num characters */
#define MVTYPENCH			2	/* math var type num characters */
#define MVTHROUGHPASSTYPE	"??"	/* string "passage" type (2) for math var */
#define FPNAMENCH			2	/* phys port name num characters */
#define FPPAGNAMENCH		10	/* phys port name num characters */
#define FVNAMENCH			8	/* phys var name num characters */
#define FVTYPENCH			2	/* phys var type num characters */
#define TAGPAGNCH			2	/* page tag num characters */
#define GENERICVARNAMENCH	4	/* non-block-assigned var name num char */
#define COMMENTNCH		256	/* max length of comments */

#define MVSTATUS_ORIG		0	/* original math var */
#define MVSTATUS_SUBST		1	/* substituted mathvar (see subconn field) */

#define FVARNOTINMAT		0	/* phys var doesn't figure in math module */
#define FVAREQMATROLE		1	/* phys var has equal role in math module */
#define FVAROPPMATROLE		-1	/* phys var has opposite role in math module */
#define FVARTRANSINMAT		2	/* phys var doesn't figure in math module but is transmitted */
#define FVARUNDMATROLE		3	/* phys var has undefined role in math module */

#define MAXMULTLINK		1024	/* max items found in follow2 */

// Guag mar 2008 - aumentato il numero dei blocchi
// #define CFTREE_MAXMODUNASS	256	/* max number of mathematically unconfigured modules */
#define CFTREE_MAXMODUNASS	400	/* max number of mathematically unconfigured modules */
#define CFTREE_MODID		0	/* slot for module id */
#define CFTREE_ERASEME		MAXNMATCFG	/* slot for local erasing flag */
#define CFTREE_CFGREJECTED	1	/* single module configuration has been rejected */
#define CFTREE_CFG2TEST		2	/* single module configuration will be "exploded" */
#define CFG_GOOD			1
#define CFG_NOGOOD		0

#define CALL4MAIN			0	/* current function has been called from main */
#define CALL4CHKCONF		1	/* current function has been called from checkconf */
#define CALL4CHKLCONF		2	/* current function has been called from checkLconf */

#define LIMIT2IN			0	/* chain of 2 limit input */
#define LIMIT2OUT			1	/* chain of 2 limit output */

#define NOHEADCHAIN		-1	/* module is not a head-chain */
#define SINGLECHAIN		-2	/* module is a head-chain of length 1 */
#define ENDCHAIN			-1	/* linear chain is terminated */
#define BROKENCHAIN		-2	/* linear chain is interrupted */
#define INSTANTENDCHAIN		-3	/* linear chain is terminated but current module is discarded */
#define ISNOTINSUBNET		-1	/* value in cfLtreeslot if module is not in current subnet */
#define LEVELTOP			1	/* signals to fillLtree to make follow2 */
#define LEVELNOTTOP		0	/* signals to fillLtree not to make follow2 */





typedef struct matVarDesc {
	char mvname[FVNAMENCH+1];
	char fullmvname[MVNAMENCH+1];
	char mvtype[MVTYPENCH+1];
	char mvcomment[COMMENTNCH];
	int status;
	char subconn[MVNAMENCH+1];
} mvdesc;


typedef struct fportDesc {
	char fpname[FPNAMENCH+1];
	char fppname[FPPAGNAMENCH+1];
	char fpcpname[FPPAGNAMENCH+1], fpcmodpn[MODNAMENCH+1];
	int fptype;
	char fvar[MAXNFISVAR][FVNAMENCH+1];

	char cftype[MAXNMATCFG][MAXNFISVAR][FVTYPENCH+1];
	int cfnum[MAXNMATCFG][MAXNFISVAR];
	int curcfnum[MAXNFISVAR];
	int was3[MAXNFISVAR];

	struct fportDesc *connectport;
        char fppconn[FPPAGNAMENCH*2+1];
} fpdesc;


typedef struct moduleDesc {
	char modname[MODNAMENCH+1];
	char fullmodname[MODNAMENCH+1];
	char modpname[MODPAGNAMENCH+1];
	char modcomment[COMMENTNCH];
	char modlibpath[512];
	fpdesc *fplist[MAXNFPORT];
	int fpnum;
	int status, numconfig, cftreeslot, cfLtreeslot, cfL2treeslot;
	int cnstat, cnin, cnout;
	char cfgcomment[MAXNMATCFG][COMMENTNCH];
	mvdesc *mvlist[MAXNMATCFG][MAXNMATVAR];
	int mvnum;
	int tiporegol;
} moddesc;

extern int FPORTNTYPE;
extern int fpvnum[];
