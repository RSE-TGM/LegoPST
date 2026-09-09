/**********************************************************************
*
*       C Header:               cont_rec.h
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed May 21 17:35:22 1997 %
*
**********************************************************************/
/*
 * cont_rec.h
 */
#ifndef _CONT_REC_INCLUDED
#define _CONT_REC_INCLUDED
/*****************************************************************/
#define FILE_REC_DEFAULT	"recorder.edf"  
#define FILE_DATA_DEFAULT  "f22circ"
#define VIEW_PROGRAM       "graphics"
#define HMI_PROGRAM        "lghmi"
/* Il selettore viene lanciato da DENTRO la simulazione in corso, e il banco
   gira nella dir del simulatore: -insim gli fa disabilitare Open loc path (che
   lo scollegherebbe da questa simulazione) e il pulsante net_startup (che con
   killsim ammazzerebbe proprio questa). */
#define HMI_PROGRAM_OPT    "-insim"

#define NO_MOD				-1 /* per selezione totale */
#define NO_BLOCK        -1 /* per selezione totale */

FLAG_SEL flag_sel;
int nu_var_sel;

#define CARICA		1        /* flag per selezione file     */
#define SALVA 		2        /* flag per selezione file     */

#define SEARCH_UP   0
#define SEARCH_DOWN 1

/*****************************************************************/
#endif
