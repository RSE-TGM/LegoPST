/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)co_float.c	1.2\t6/16/93";
/*
   modulo co_float.c
   tipo 
   release 1.2
   data 6/16/93
   reserved @(#)co_float.c	1.2
*/
#include <stdio.h>
#include <string.h>

#include "sim_param.h"
#include "sim_types.h"
#include "compstaz.inc"

/*
        conv_float
*/
int co_float(lmax,ldec,str,val)
int lmax,ldec;
STRIN_ST *str;
float *val;
{
char *px;
int ndec;

/*printf("\n stringa %s  lung %d ",str->stringa,str->lun_stringa);*/
if (str->lun_stringa > lmax) return(1);
px=strchr(str->stringa,'.');
if (px == NULL) return(1);
px++;
ndec=0;
for (;;)
{
	if (*px == 0) break;
	px++;
	ndec++;
}
/*printf("\n lmax %d  ldec %d  ndec %d",lmax,ldec,ndec);*/
if (ndec > ldec) return(1);
if (!sscanf(str->stringa,"%f",val)) return(1);
return(0);
}

