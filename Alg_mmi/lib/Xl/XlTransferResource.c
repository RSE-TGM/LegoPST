/**********************************************************************
*
*       C Source:               XlTransferResource.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Tue Apr 16 17:15:16 1996 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlTransferResource.c,2 %  (%full_filespec: 1,csrc,XlTransferResource.c,2 %)";
#endif
/*
   modulo XlTransferResource.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlTransferResource.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlTransferResource.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*-------------------------------------------------------------------*
 *                                                                   *
 * Module Name:   XlTransferResource.c                               *
 *                                                                   *
 * Date: 13 Gennaio 1994        Ver: 1.0                             *
 *-------------------------------------------------------------------*/
#include <stdio.h>
#include <Xm/Xm.h>   
#include <Rt/RtMemory.h>   
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


/*-------------------------------------------------------------------------------
 * Function Name:   XlTransferResources                                         *
 *                                                                              *
 * Parameter:                                                                   *
 *      XrmDatabase *pSorgDb;  puntatore al database delle risorse sorgente     *
 *      XrmDatabase *pDestDb;  puntatore al database delle risorse destinazione *
 *      Widget wsorg;          ID widget sorgente                               *
 *      char *str_sorg; stringa dell'oggetto sorgente                           *
 *      char *str_dest; stringa dell'oggetto destinazione                       *
 *                                                                              *
 * Description:   effettua il trasferimento delle risorse risorse               *
 *                dall'oggetto sorgente a quello destinazione.                  *
 *                Le stringhe dell'oggetto sorgente e destinazione              *
 *                devono essere nella forma:                                    * 
 *                                                                              *
 *                se l'oggetto dest e la drawing area				*
 *                      str_sorg = nome_sorg.     				*
 *                oppure se e' un oggetto di tipo composite (Led,etc..)         *
 *                	*nome_padre.nome_dest 					*
 *                                                                              *
 * Date: 13 Gennaio 1993        Ver: 1.0                                        *
 *------------------------------------------------------------------------------*/

XlTransferResource(XrmDatabase *pSorgDb, XrmDatabase *pDestDb, Widget wsorg, char *str_sorg,char *str_dest )
{
   WidgetClass wclass;
   XtResourceList resources;
   Cardinal num_resources;
   XrmValue xrm_value;
   char *str_type;
   char str_risorsa_sorg[150],str_risorsa_dest[150];
   short i,offset_str_sorg,offset_str_dest;

   wclass=XtClass(wsorg);
   XtGetResourceList(wclass,&resources,&num_resources);
   RtRecord(resources);

   strcpy(str_risorsa_sorg,str_sorg);
   offset_str_sorg=strlen(str_risorsa_sorg);

   strcpy(str_risorsa_dest,str_dest);
   offset_str_dest=strlen(str_risorsa_dest);

   for( i=0; i<num_resources; i++)
   {
      strcpy(&str_risorsa_sorg[offset_str_sorg],resources[i].resource_name);
      if(XrmGetResource(*pSorgDb,str_risorsa_sorg,resources[i].resource_class,&str_type,&xrm_value)==True)
      {
         strcpy(&str_risorsa_dest[offset_str_dest],resources[i].resource_name);
         XrmPutResource(pDestDb,str_risorsa_dest,XmRString,&xrm_value);
      }
   }
  
   XtFree(resources);
}
