/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_sep_s.c	1.2\t6/16/93";
/*
   modulo co_sep_s.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)co_sep_s.c	1.2
*/

/*    SUBROUTINE SEPARA_STR(RIGA,LUN,NSTR,STRIN)
  Conversione f-c:        1991-05-09      Autore: C.Lusso (CISE)
*/
	/* rimuovere se inserita in compi.c */
#include <string.h>

#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include "sim_param.h"
#include "sim_types.h"
#include "xstaz.h"
#include "compstaz.h"
#include "conv_mmi.h"

int separa_str(
 char riga[],
 int lun,
 int nstr,
 STRIN_ST strin[])
{
char *s;
int i;
for (i=0; i<nstr; i++)   strin[i].stringa=NULL;
for (s=riga, i=0; i<nstr; i++) {
  strin[i].stringa = s = strtok( s, " \t");
  if ( s != NULL ) strin[i].lun_stringa = strlen( s);
  else 
	{ strin[i].lun_stringa = 0; return(0);}
  s = NULL; }
} /* FINE "co_separa_str.c" */
