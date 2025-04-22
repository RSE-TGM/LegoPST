/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
/*
	Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)ts_chdir.c	5.1\t11/7/95";
/*
   modulo ts_chdir.c
   tipo 
   release 5.1
   data 11/7/95
   reserved @(#)ts_chdir.c	5.1
*/
int ts_chdir_()
{
   return( chdir( getenv("WORK_DIR") ) );
}
