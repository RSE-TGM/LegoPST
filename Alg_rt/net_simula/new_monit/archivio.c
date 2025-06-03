/**********************************************************************
*
*       C Source:               archivio.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Nov  8 17:52:55 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: archivio.c-10 %  (%full_filespec: archivio.c-10:csrc:1 %)";
#endif

/*
 * archivi.c
 */
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>
#include <sys/statfs.h>
// #if defined AIX || defined LINUX
// #include <sys/statfs.h>
// #else if defined OSF1
// #include <sys/param.h>
// #include <sys/mount.h>
// #endif  

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"
//#include "banco_globals.h"

#include "parametri.h"
#include "archivi.h"


int SD_archive (int, ARCHIVE_REQ *);
int add_message (Widget, char*, int);


/**********************************************************/
int free_disk()
{
int spazio;
struct statfs buf;
// char *curr_path;
//    curr_path = (char *)get_current_dir_name();
   char curr_path[256];
   if (getcwd(curr_path, sizeof(curr_path)) == NULL)
        perror("getcwd() error");
    
   statfs(curr_path,&buf);
   spazio=(int)buf.f_bavail*4 ;   
	return (spazio/1000); /* in MB */
}
/**********************************************************/
int check_data_save_req()
{
	if ((archsavetfin <= archsavetinit) ||
		 (disco_libero < disco_richiesto))
		return(-1);
	return(0);

}
/**********************************************************/
int richiesta_save_arch () 
{
char *messaggio;
char *testo = "SAVE ARCHIVIO :";
int esito;

	esito = 0;
	archive_req.t_init = archsavetinit;
	archive_req.t_end = archsavetfin;
	strcpy (archive_req.commento,comm_arch_save);
	messaggio = (char *)malloc (strlen(testo) + strlen(comm_arch_save) + 10);

	if (SD_archive (BANCO, &archive_req) > 0)
		sprintf (messaggio,"%s %s",testo,archive_req.commento);
	else
		{
		sprintf (messaggio,"%s %s %s",
                    testo,archive_req.commento,FAILED);
		esito = -1;
		}
	add_message (areaMessaggi,messaggio,LIVELLO_1);
	free(messaggio);
	return (esito);
}
