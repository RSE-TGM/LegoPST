/**********************************************************************
*
*       C Source:               SD_dummy_totale.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed May 21 17:32:20 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: SD_dummy_totale.c-4 %  (%full_filespec: SD_dummy_totale.c-4:csrc:1 %)";
#endif

#include <stdio.h>
#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Xm.h>

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"

#include "option.h"
#include "archivi.h"
#include "bistrutt.h"
/*
 * dummy function
 */
/****************************************************************/
int SD_editic (a,p,c)
int a;
SNTAB *p;
int c;
{
int i;
SNTAB *pp;

	pp = p;
	printf ("dummy function SD_editic\n");

	for (i=0; i<MAX_SNAP_SHOT; i++,pp++)
		{
		pp->stat = 1; /* occupato */
		pp->pos  = i;
		pp->prog = i;
		pp->mod  = 0;
		pp->forzato = 0;
		strcpy (pp->descr,"<<< DESCRIZIONE SNAPSHOT >>>>");
		strcpy (pp->datasn,"06oct95");
		strcpy (pp->temposn,"11:55:22");
		}
	pp = p;
	for (i=5; i<15; i++)
		(p+i)->stat = 0;
	
	return(1);
}
/****************************************************************/
int SD_editbt (a,p,c)
int a;
BKTAB *p;
int c;
{
int i;
BKTAB *pp;

   pp = p;
   printf ("dummy function SD_editbt\n");

/*   for (i=0; i<MAX_BACK_TRACK; i++,pp++)   */
	for (i=0; i<30; i++)
      {
      pp = p+i;
      pp->stat = 0;
      }

   for (i=30; i<=70; i++)
      {
		pp = p+i;
      pp->stat = 1; /* occupato */
      pp->pos  = i;
      pp->prog = i;
      pp->mod  = 0;
		if ((i/10)*10 == i)
			pp->forzato = 1;
		else
      	pp->forzato = 0;
		pp->tempo = (float)i;
      strcpy (pp->descr,"<<< DESCRIZIONE BACKTRACK >>>>");
      strcpy (pp->datasn,"16oct95");
      strcpy (pp->temposn,"12:00:00");
      }
	for ( ; i<MAX_BACK_TRACK; i++)
		{
		pp = p+i;
      pp->stat = 0;
		}
   return(1);
}
/****************************************************************/
int SD_loadic (a,n,c)
int a;
int *n;
int *c;
{
	printf ("SD_loadic : dummy function\n");
	return(1);
}
/****************************************************************/
int SD_loadbt (a,n,c)
int a;
int *n;
int c;
{
	printf ("SD_loadbt : dummy function\n");
	return(1);
}
/****************************************************************/
int SD_removeic (a,n)
int a;
int *n;
{
int p;

	p = *n;
	printf ("dummy function SD_removeic ic n. %d\n",p);

	return(1);
}
/****************************************************************/
int SD_saveic (a,n,s,c)
int a;
int *n;
char *s;
char *c;
{
	printf ("SD_saveic : dummy function commento %s\n",s);
	return(1);
}
/****************************************************************/
int SD_savebt (a,n,s,c)
int a;
int *n;
char *s;
int c;
{
   printf ("SD_savebt : dummy function commento %s\n",s);
   return(1);
}
/****************************************************************/
int SD_snapcopy (a,n)
int a;
int *n;
{
int p;

	p = *n;
	printf ("dummy function SD_snapcopy ic n. %d\n",p);

	return(1);
}
/****************************************************************/
int SD_snappaste (a,n)
int a;
int *n;
{
int p;

	p = *n;
	printf ("dummy function SD_snappaste ic n. %d\n",p);

	return(1);
}
/****************************************************************/
int SD_run(a)
int a;
{
	printf ("dummy function SD_run\n");
	return(1);
}
/****************************************************************/
int SD_freeze(a)
int a;
{
   printf ("dummy function SD_freeze\n");
   return(1);
}
/****************************************************************/
int SD_backtrack(a)
int a;
{
   printf ("dummy function SD_backtrack\n");
   return(1);
}
/****************************************************************/
int SD_replay(a)
int a;
{
   printf ("dummy function SD_replay\n");
   return(1);
}
/****************************************************************/
int SD_clear(a)
int a;
{
	printf ("dummy function SD_clear\n");
   return(1);
}
/****************************************************************/
int SD_optsave (processo, dati)
int processo;
char *dati;
{
	if (write_options(dati) == 0)
		return(1);
	return(0);
}
/****************************************************************/
int SD_optload0 (processo, dati)
int processo;
char *dati;
{
	if (lettura_opzioni(dati) == 0)
		return (1);
	return (0);
}
/****************************************************************/
int SD_archive (processo, dati)
int processo;
ARCHIVE_REQ *dati;
{
	printf ("SD_archive tinit = %f\n",dati->t_init);
	printf ("SD_archive tinit = %f\n",dati->t_end);
	printf ("SD_archive comm = %s\n",dati->commento);
	return(1);
}
/****************************************************************/
/******
int SD_aggancio(processo)
int processo;
{
	printf ("DUMMY function SD_aggancio\n");
	return (1);
}
********/
/****************************************************************/
int SD_stato (processo, dati)
int processo;
RICHIESTA_STAT *dati;
{
	printf ("DUMMY function SD_stato\n");
	return (1);
}
/****************************************************************/
/********
int from_dispatcher(a,b,c,d,e,f)
int a;
char *b;
char *c;
char *d;
int *e;
int f;
{
	*e = 0;

	return (0);
}
**********/
/****************************************************************/
int SD_sessione (processo, dati)
int processo;
char *dati;
{
   printf ("DUMMY function SD_sessione:%s:\n",dati);
   return (1);
}
/****************************************************************/
int SD_snapview0 (processo, dati)
int processo;
char *dati;
{
SNAP_VIEW *p = (SNAP_VIEW *)dati;
int i;

	for (i=0; i<10; i++)
		{
		p->var_val[i].valore = (float)i;
		}
	p->record = 10;
   printf ("DUMMY function SD_snapview\n");
   return (1); 
}
/****************************************************************/
int SD_btrecstep0 (processo, dati)
int processo;
float *dati;
{
float p;

	p = *dati;
   printf ("DUMMY function SD_btcrstep:%f:\n",p);
   return (1);
}
/****************************************************************/
int SD_lbtreg (processo)
int processo;
{
	printf ("DUMMY function SD_lbtreg\n");
	return (70);
}
