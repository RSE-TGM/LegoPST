/**********************************************************************
*
*       C Source:               stato_cr.c
*       Subsystem:              3
*       Description:
*       %created_by:    mauric %
*       %date_created:  Mon Jan 26 16:23:29 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: stato_cr.c-2 %  (%full_filespec: stato_cr.c-2:csrc:3 %)";
#endif
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)stato_cr.c	5.1\t11/7/95";
/*
   modulo stato_cr.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)stato_cr.c	5.1
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sked.h"
#include <Rt/RtMemory.h>

int save_stato_cr(stato_cr)
STATO_CR *stato_cr;
{
FILE *fp;

if(fp=fopen("stato_cr.rtf","w"))
        {
        fwrite(stato_cr,sizeof(STATO_CR),1,fp);
        fclose(fp);
#if defined VMS
        system("purge/noconfirm stato_cr.rtf");
#endif
	return(1);
        }
else
	{
        perror("dispatcher(impossibile salvare file statocr.rtf)");
	return( -1);
	}
}


int load_stato_cr(stato_cr)
STATO_CR *stato_cr;
{
FILE *fp;

if(fp=fopen("stato_cr.rtf","r"))
        {
        fread(stato_cr,sizeof(STATO_CR),1,fp);
        fclose(fp);
	return(1);
        }
        else
        {
        stato_cr->last_snap_save= 0;
        stato_cr->last_snap_load= 0;
#if defined BACKTRACK
        stato_cr->last_bktk_save= 0;
        stato_cr->last_bktk_load= 0;
#endif
	return(-1);
        }
}




int load_stato_cr_arch(stato_cr,path_archive)
STATO_CR *stato_cr;
char     *path_archive;
{
FILE *fp;
char path_stato_cr[FILENAME_MAX];

strcpy(path_stato_cr,path_archive);
strcat(path_stato_cr,"/stato_cr.rtf");

printf("path_stato_cr = %s\n",path_stato_cr);

if(fp=fopen(path_stato_cr,"r"))
        {
        fread(stato_cr,sizeof(STATO_CR),1,fp);
        fclose(fp);
	return(1);
        }
        else
        {
        stato_cr->last_snap_save= 0;
        stato_cr->last_snap_load= 0;
#if defined BACKTRACK
        stato_cr->last_bktk_save= 0;
        stato_cr->last_bktk_load= 0;
#endif
	return(-1);
        }
}
