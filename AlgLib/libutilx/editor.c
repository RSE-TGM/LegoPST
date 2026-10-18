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
/*
   modulo editor.c
   tipo 
   release 5.1
   data 11/10/95
   reserved @(#)editor.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)editor.c	5.1\t11/10/95";
/*
        Fine sezione per SCCS
*/
/*   EDITOR.C		
     Editing di un file.

     Consiste in un FormDialog ridimensionabile contenente: un Text widget
     e 3 Push Button

     Permette il passaggio diretto delle label dei Button e delle
     rispettive callback. 

     PARAMETRI:
     padre     	    widget da cui deve dipendere
     testo          conterra' il widget di testo .
     filename	    FILE da editare
     editing        testo editabile si/no 
     font	    font per il Text 
     chiama_funz    funzione di callback da associare al Button specificato
     geometry  	    definito in libutilx.h

  ritorna l'indice di Widget della FormDialog  se l'operazione ha avuto
  successo, altrimenti ritorna 0.
*/

#include <stdio.h>
#if defined UNIX
#include <sys/types.h>
#include <sys/stat.h>
#elif defined VMS
#include <types.h>
#include <stat.h>
#endif
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
#include <Xm/MessageB.h>
#include <Xm/Text.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/DialogS.h>
#include <Xm/Separator.h>
#include <Xm/Xm.h>

#include "libutilx.h"
#include "fontfallback.h"

/* Macro per la creazioni di compound stringhe */
#define CS  			XmSTRING_DEFAULT_CHARSET
#define CREATE_CSTRING(string)  (XmStringCreateLtoR((string),CS))


Widget editor (padre,testo,filename,editing,font,chiama_funz,geometry)
Widget padre, *testo;
char *filename; 
int editing;   /* True = 1,  False = 0  */
char  *font;
Elenco_callback chiama_funz;
Dialog_geometry geometry;
{
   Arg args[20];
   int n;
   char *stringa = NULL, *ptr2;
   FILE *fp;
   Widget finestra, find_form, bottom_widget = NULL;
   Widget button1 = NULL, button2 = NULL, button3 = NULL, sep;
   XFontStruct *finfo;
   XmFontList fontList;
   struct stat info_file;
   Text_find_struct *parametri;
   XmString cstring;

   finfo = FontFallbackLoad(XtDisplay(padre),font);
   fontList = XmFontListCreate (finfo,XmSTRING_DEFAULT_CHARSET);

/* allocazione della struttura utilizzata dalla function find_in_a_text() */
   parametri = (Text_find_struct *) XtMalloc(sizeof(Text_find_struct));   

/* Creazione del Form Dialog  */
   cstring = CREATE_CSTRING(filename);
   n = 0;
   XtSetArg(args[n],XmNdialogTitle,cstring); n++;
   XtSetArg(args[n],XmNdialogStyle,XmDIALOG_MODELESS);n++;
   XtSetArg(args[n],XmNnoResize,False); n++;
   XtSetArg(args[n],XmNautoUnmanage,False); n++;
   XtSetArg(args[n],XmNresizePolicy,XmRESIZE_ANY); n++;
   XtSetArg(args[n],XmNdefaultPosition,geometry.default_pos); n++;
   XtSetArg(args[n],XmNx,geometry.x); n++;
   XtSetArg(args[n],XmNy,geometry.y); n++;
   XtSetArg(args[n],XmNdefaultPosition,geometry.default_pos); n++;
   XtSetArg(args[n],XmNheight,geometry.height); n++;
   XtSetArg(args[n],XmNwidth, geometry.width); n++;
   XtSetArg(args[n],XmNallowOverlap,False); n++;
   XtSetArg(args[n],XmNhorizontalSpacing,15); n++;
   XtSetArg(args[n],XmNverticalSpacing,15);n++;

/* nel campo userData viene memorizzato il puntatore della struttura */
/* parametri. In questo modo, si consentire al programmatore di deallocare */
/* la memoria utilizzata quando si chiude la window di editor. */
   XtSetArg(args[n],XmNuserData, parametri);n++;

   finestra=XmCreateFormDialog(padre,"finestra",args,n);
   XmStringFree(cstring);

   fp = fopen (filename,"r");
   if (fp == NULL)
      fprintf (stderr,"Can't open file %s\n", filename);
   else
   {
 /* Recupera la dimensione del file */
 /* Lettura del file */
      if ( !fstat(fileno(fp),&info_file))
         stringa = (char *) XtMalloc ((unsigned) info_file.st_size+1);
      else
         stringa = (char *) XtMalloc (sizeof(char) * 100000);

      ptr2 = stringa;
      while (fgets(ptr2,140,fp) != NULL)
         ptr2 = ptr2+strlen(ptr2);
   }

/* Creazione dei tre Push Button */

/* Accetta i parametri per il BUTTON 1  */
   if (chiama_funz[0].button_label != NULL)
   {
       cstring = CREATE_CSTRING(chiama_funz[0].button_label);
       n=0;
       XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNbottomOffset,10); n++;
       XtSetArg(args[n],XmNlabelString, cstring ); n++;
       button1=XmCreatePushButton(finestra,"button1",args,n);
       XtManageChild (button1);
       XtAddCallback (button1,XmNactivateCallback,
   	      	      chiama_funz[0].callback,
                     (XtPointer)chiama_funz[0].parametro);
       bottom_widget = button1;
       XmStringFree(cstring);
   }

/*  Accetta i parametri per il BUTTON 2  */
   if (chiama_funz[1].button_label != NULL)
   {
       cstring = CREATE_CSTRING(chiama_funz[1].button_label);
       n=0;
       XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNbottomOffset,10); n++;
       if (button1 != NULL)
       {
          XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET); n++;
          XtSetArg(args[n],XmNleftWidget,button1); n++;
          XtSetArg(args[n],XmNleftOffset,25); n++;
       }
       else
          XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNlabelString, cstring ); n++;
       button2=XmCreatePushButton(finestra,"button2",args,n);
       XtManageChild (button2);
       XtAddCallback (button2,XmNactivateCallback,
                              chiama_funz[1].callback,
                              (XtPointer)chiama_funz[1].parametro);
       XmStringFree(cstring);
       bottom_widget = button2;
   }
	
/*  Accetta i parametri per il BUTTON 3  */
   if (chiama_funz[2].button_label != NULL)
   {
       cstring = CREATE_CSTRING(chiama_funz[2].button_label);
       n=0;
       XtSetArg(args[n],XmNbottomAttachment,XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNbottomOffset,10); n++;
       XtSetArg(args[n],XmNleftAttachment,XmATTACH_WIDGET); n++;
       if (button2 != NULL)
       {
          XtSetArg(args[n],XmNleftWidget,button2); n++;
          XtSetArg(args[n],XmNleftOffset,25); n++;
          XtSetArg(args[n],XmNlabelString, cstring ); n++;
       }
       else
       {
          if (button1 != NULL)
          {
             XtSetArg(args[n],XmNleftWidget,button1); n++;
             XtSetArg(args[n],XmNleftOffset,25); n++;
             XtSetArg(args[n],XmNlabelString, cstring ); n++;
          }
          else
             XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM); n++;
       }
       XtSetArg(args[n],XmNlabelString, cstring ); n++;
       button3 = XmCreatePushButton(finestra,"button3",args,n);
       XtManageChild (button3);
       XtAddCallback (button3,XmNactivateCallback,
	      		      chiama_funz[2].callback,
                     (XtPointer)chiama_funz[2].parametro);
       XmStringFree(cstring);
       bottom_widget = button3;
   }

   if (bottom_widget != NULL)
   {
       n=0;
       XtSetArg(args[n],XmNrightAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNleftAttachment, XmATTACH_FORM); n++;
       XtSetArg(args[n],XmNbottomAttachment, XmATTACH_WIDGET); n++;
       XtSetArg(args[n],XmNbottomWidget, bottom_widget); n++;
       XtSetArg(args[n],XmNbottomOffset, 10); n++;
       sep = XmCreateSeparator(finestra,"separatore",args,n);
       XtManageChild(sep);
   }

   n = 0;
   if (bottom_widget != NULL)
   {
      XtSetArg(args[n],XmNbottomAttachment, XmATTACH_WIDGET); n++;
      XtSetArg(args[n],XmNbottomWidget, sep); n++;
      XtSetArg(args[n],XmNbottomOffset, 20); n++;
   }
   else
      XtSetArg(args[n],XmNbottomAttachment, XmATTACH_FORM); n++;
   find_form = (Widget)find_in_a_text(finestra, args, n, parametri);

/* Creazione del text-widget */
   n=0;
   XtSetArg(args[n],XmNcolumns,110); n++;
   XtSetArg(args[n],XmNrows,60); n++;
   XtSetArg(args[n],XmNeditable,editing); n++;
   XtSetArg(args[n],XmNeditMode,XmMULTI_LINE_EDIT); n++;
   XtSetArg(args[n],XmNfontList,fontList); n++;
   XtSetArg(args[n],XmNscrollingPolicy,XmAUTOMATIC); n++;
   XtSetArg(args[n],XmNvisualPolicy,XmCONSTANT); n++;
   XtSetArg(args[n],XmNbottomAttachment, XmATTACH_WIDGET); n++;
   XtSetArg(args[n],XmNbottomWidget, find_form); n++;
   XtSetArg(args[n],XmNbottomOffset, 10); n++;
   XtSetArg(args[n],XmNtopAttachment,XmATTACH_FORM); n++;
   XtSetArg(args[n],XmNrightAttachment,XmATTACH_FORM); n++;
   XtSetArg(args[n],XmNleftAttachment,XmATTACH_FORM); n++;
   *testo=XmCreateScrolledText(finestra,"testo",args,n);
   XmTextSetString (*testo,stringa);
   XtManageChild(*testo);

   parametri->ID_text = *testo;

   XtManageChild(finestra);
   XtSetKeyboardFocus (finestra, *testo);

/* Dealloca la memoria utilizzata e chiudi il file */
   if ( fp != NULL)
   {
      fclose( fp );
      XtFree( stringa );
   }

   return(finestra);
}
