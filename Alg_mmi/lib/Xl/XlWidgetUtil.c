/*********************************************************************
*
*       C Source:               XlWidgetUtil.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Thu Aug 28 09:22:34 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlWidgetUtil.c-4 %  (%full_filespec: XlWidgetUtil.c-4:csrc:1 %)";
#endif
/*
   modulo XlWidgetUtil.c
   tipo 
   release 5.4
   data 1/24/96
   reserved @(#)XlWidgetUtil.c	5.4
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlWidgetUtil.c	5.4\t1/24/96";
/*
        Fine sezione per SCCS
*/
/*-------------------------------------------------------------------*
 *                                                                   *
 * Module Name:   XlWidgetUtil.c                                     *
 *                                                                   *
 * Date: 20 Aprile 1994        Ver: 1.0                              *
 *-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xl/XlP.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlIndicP.h>
#include <Xl/XlManagerP.h>
#include <Xl/XlPictographP.h>
#include <Xl/XlGraficoP.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlIndicTelepP.h>
#include <Xl/XlPixvarP.h>

#include "wait.bmp"
#include "wait_mask.bmp"
#include "err.bmp"
#include "err_mask.bmp"


/*
	variabile globale per la gestione del blink
*/
int num_blink=0;
int blink_allocati=0;
typedef struct lista_blink{
	int ind;
	Widget w;
	blinkcallbackproc  funzione;
	int counter;	/*contatore*/
	float freq;	/*freq di blink*/
	Boolean stato;  /*stato di Blink dell' oggetto (On o Off)*/
	} LISTA_BLINK;
LISTA_BLINK *lista_blink=NULL;


Boolean done(value,toVal)       
XlAnimatedColor *value;
XrmValue *toVal;
{                                               
   static XlAnimatedColor static_val;                   

   if( toVal->addr != NULL )                    
   {                                            
      if( toVal->size < sizeof(XlAnimatedColor) )          
      {                                         
         toVal->size = sizeof(XlAnimatedColor);  
         return( False);                        
      }                                         
      *(XlAnimatedColor *)(toVal->addr) = *(value);         
   }                                            
   else                                         
   {                                            
      static_val = *(value);                    
      toVal->addr = (XtPointer)&static_val;     
   }                                            
   toVal->size = sizeof(XlAnimatedColor);                  
   return( True);                               
}

/*------------------------------------------------------------------
 * Function Name:   XlGetSelectedWidget				   *
 *								   *
 * Parameter:							   *
 *	Widget DrawA;		Drawing Area nella quale           *
 *				cercare l'oggetto selezionato      *
 *								   *
 * Description:	  ritorna l'ID del primo oggetto trovato 	   *
 *			selezionato all'interno di una delle	   *
 *			Drawing Area, altrimenti ritorna NULL;	   *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

Widget XlGetSelectedWidget (Widget DrawA)
{
int i,j;
int NumFigli,NumFigliC;
Widget *Figli,*FigliC;
Arg argDA[2],argC[2],argW[1];
Boolean Sel;

/* Settaggio argomenti per Drawing Area */
XtSetArg (argDA[0], XmNchildren, &Figli);
XtSetArg (argDA[1], XmNnumChildren, &NumFigli);

/* Settaggio argomenti per Composite */
XtSetArg (argC[0], XmNchildren, &FigliC);
XtSetArg (argC[1], XmNnumChildren, &NumFigliC);

/* Settaggio argomenti per Widget */
XtSetArg (argW[0], XlNselected, &Sel);

XtGetValues (DrawA, argDA, 2);

/* Controllo di tutti i figli della Drawing Area */
for ( i=0; i<NumFigli; i++)
	{
	Sel = False;
	/* Prende l'attributo Selected dell'oggetto puntato */
	XtGetValues (Figli[i], argW, 1);
	if (Sel)
		return (Figli [i]);
	else
		{
		/* Controlla se l'oggetto puntato e' Composite */
		if ( XlIsXlComposite (Figli[i]) )
			{
			XtGetValues (Figli[i], argC, 2);

			/* Controllo di tutti i figli del Composite */
			for (j=0; j<NumFigliC; j++)
				{
				Sel = False;
				/* Prende l'attributo Selected
					dell'oggetto puntato */
				XtGetValues (FigliC[j], argW, 1);
				if (Sel)
					return (FigliC[j]);
				}
			}
		}
	}
return (NULL);
}

/*------------------------------------------------------------------
 * Function Name:   XlIsGraficoArch                                *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' di classe Grafico   *
 *		   	e di tipo da archivio, altrimenti          *
 *			ritorna False;				   *
 *								   *
 * Date: 26 Luglio 1994		Ver: 2.1			   *
 *-----------------------------------------------------------------*/
Boolean XlIsGraficoArch (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;
int Tipo;
Arg arg[3];

/*  Se l'oggetto in input e' uguale a NULL ritorna immediatamente NULL  */
if (Oggetto==NULL)
  return (False);

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );

if ( (strncmp(NomeClasseOggetto,"Grafico",7)==0))
	{
	XtSetArg (arg[0], XlNgraficoType, &Tipo);
	XtGetValues (Oggetto, arg, 1);

	if(Tipo == ARCH_GRAF )
		{
		XtFree(NomeClasseOggetto);
  		return (True);
		}
	}

XtFree(NomeClasseOggetto);
return(False);
}

/*------------------------------------------------------------------
 * Function Name:   XlIsGraficoSingle                              *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' di classe Grafico   *
 *		   	e di tipo ad una variabile, altrimenti     *
 *			ritorna False;				   *
 *								   *
 * Date: 26 Luglio 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/
Boolean XlIsGraficoSingle (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;
int Tipo;
Arg arg[3];

/*  Se l'oggetto in input e' uguale a NULL ritorna immediatamente NULL  */
if (Oggetto==NULL)
  return (False);

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );

if ( (strncmp(NomeClasseOggetto,"Grafico",7)==0))
	{
	XtSetArg (arg[0], XlNgraficoType, &Tipo);
	XtGetValues (Oggetto, arg, 1);

	if(Tipo == PLOT_GRAF )
		{
		XtFree(NomeClasseOggetto);
  		return (True);
		}
	}

XtFree(NomeClasseOggetto);
return(False);
}

/*------------------------------------------------------------------
 * Function Name:   XlIsAnimateIcon                                *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' un'icona animata    *
 *			(Bitmap o DisegnoVettoriale),altrimenti    *
 *			ritorna False;				   *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

Boolean XlIsAnimateIcon (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;
int EIcona,EBitmap;
char *ConfrontoIcona="Icon";
char *ConfrontoBitmap="Bitmap";

/*  Se l'oggetto in input e' uguale a NULL ritorna immediatamente NULL  */
if (Oggetto==NULL)
  return (False);

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );

/* Controlla se la classe trovata corrisponde con quella di una 
	Icona o con quella di un Bitmap */
EIcona = strcmp (NomeClasseOggetto, ConfrontoIcona);
EBitmap = strcmp (NomeClasseOggetto, ConfrontoBitmap);
XtFree(NomeClasseOggetto);
if ( (EIcona == 0) || (EBitmap == 0) )
	return (True);
else return (False);
}

/*------------------------------------------------------------------
 * Function Name:   XlIsIndic                                      *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' un indicatore       *
 *		  o un indicatore con errore altrimenti            *
 *                ritorna False			                   *
 *								   *
 * Date: 17 Giugno 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

Boolean XlIsIndic (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );

if(strncmp (NomeClasseOggetto, "Indic",5)==0)
        {
        XtFree(NomeClasseOggetto);
	return (True);
        }
else 
        {
        XtFree(NomeClasseOggetto);
	return (False);
        }
}


/*------------------------------------------------------------------
 * Function Name:   XlIsIndicTelep                                      *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' un indicatore       *
 *		  o un indicatore con errore altrimenti            *
 *                ritorna False			                   *
 *								   *
 * Date: 17 Giugno 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

Boolean XlIsIndicTelep (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );

if(strncmp (NomeClasseOggetto, "IndicTelep",10)==0)
        {
        XtFree(NomeClasseOggetto);
	return (True);
        }
else 
        {
        XtFree(NomeClasseOggetto);
	return (False);
        }
}


/*------------------------------------------------------------------
 * Function Name:   XlIsCurve                                      *
 *                                                                 *
 * Parameter:                                                      *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 * 								   *
 * Description:	  ritorna True se l'Oggetto e' un indicatore       *
 *		  o un indicatore con errore altrimenti            *
 *                ritorna False			                   *
 *								   *
 * Date: 05 Novembre 1997		Ver: 1.0		   *
 *-----------------------------------------------------------------*/

Boolean XlIsCurve (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );


if(strcmp(NomeClasseOggetto,"Curve")==0)
        {
        XtFree(NomeClasseOggetto);
	return (True);
        }
else 
        {
        XtFree(NomeClasseOggetto);
	return (False);
        }
}

/*------------------------------------------------------------------
 * Function Name:   XlIsCai                                      *
 *                                                                 *
 * Parameter:                                                      *
 *      Widget Oggetto;         ID dell'oggetto da esaminare       *
 *                                                                 *
 * Description:   ritorna True se l'Oggetto e' un Cai       *
 *                o un indicatore con errore altrimenti            *
 *                ritorna False                                    *
 *                                                                 *
 * Date: 23 Aprile 1997         Ver: 1.0                           *
 *-----------------------------------------------------------------*/

Boolean XlIsCai (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );
if((strcmp (NomeClasseOggetto, "Cai")==0) )
        {
        XtFree(NomeClasseOggetto);
        return (True);
        }
else
        {
        XtFree(NomeClasseOggetto);
        return (False);
        }
}
/*------------------------------------------------------------------
 * Function Name:   XlIsCaiAll                                     *
 *                                                                 *
 * Parameter:                                                      *
 *      Widget Oggetto;         ID dell'oggetto da esaminare       *
 *                                                                 *
 * Description:   ritorna True se l'Oggetto e' un CaiAll           *
 *                altrimenti ritorna False                         *
 *                                                                 *
 * Date: 21 Luglio 1997         Ver: 1.0                           *
 *-----------------------------------------------------------------*/

Boolean XlIsCaiAll (Widget Oggetto)
{
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );
if((strcmp (NomeClasseOggetto, "CaiAll")==0) )
        {
        XtFree(NomeClasseOggetto);
        return (True);
        }
else
        {
        XtFree(NomeClasseOggetto);
        return (False);
        }
}

/*------------------------------------------------------------------
 * Function Name:   XlIsInEdit                                     *
 *                                                                 *
 * Parameter:                                                      *
 *      Widget wid;             ID dell'oggetto da esaminare       *
 *                                                                 *
 * Description:      ritorna true se l'attributo XlNinEdit         *
 *                     dell'oggetto passato come parametro e       *
 *                     settato; ritorna false se non lo e' o       *
 *                     se lo Widget passato come parametro e'      *
 *                     uguale a NULL                               *
 *                                                                 *
 * Date: 18 Maggio 1994          Ver: 1.0                          *
 *-----------------------------------------------------------------*/

Boolean XlIsInEdit (Widget wid)
{
Arg arg[2];
Boolean Valore=False;

if (wid != NULL)
  {
  XtSetArg (arg[0], XlNinEdit, &Valore);
  XtGetValues (wid, arg, 1);
  }
return (Valore);
}

/*------------------------------------------------------------------
 * Function Name:   XlGetFirstVarGraf				   *
 *								   *
 * Parameter:							   *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna un puntatore alla stringa di descrizione *
 *			di una variabile di definizione del Grafico*
 *                      se non viene trovata nessuna variabile     *
 *                      configurata                                *
 *			viene ritornato un puntatore a NULL;	   *
 *			L'Oggetto deve essere di tipo Grafico      *
 *			altrimenti la routine non puo' funzionare  *
 *								   *
 * Date: 26 Luglio 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

char *XlGetFirstVarGraf (Widget Oggetto)
{
Arg arg[4];
char *Ritorno;

if (XlIsGraficoSingle (Oggetto))
  {
  XtSetArg (arg[0], XlNvarName1, &Ritorno);
  XtGetValues (Oggetto, arg, 1);
  return ( XtNewString (Ritorno) );
  }
else return (NULL);
}

/*------------------------------------------------------------------
 * Function Name:   XlGetFirstVcc				   *
 *								   *
 * Parameter:							   *
 *	Widget Oggetto;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna un puntatore alla stringa di descrizione *
 *			di una variabile di cambio colore,se non   *
 * 			viene trovata nessuna variabile configurata*
 *			viene ritornato un puntatore a NULL;	   *
 *			L'Oggetto deve essere una icona animata    *
 *                      o un indicatore
 *			altrimenti la routine non puo' funzionare  *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/

char *XlGetFirstVcc (Widget Oggetto)
{
Arg arg[4];
int narg;
char *NomeClasseOggetto;
WidgetClass ClasseOggetto;
char *ConfrontoIcona="Icon";
char *ConfrontoIconReg="IconReg";
char *ConfrontoDispReg="DispReg";
char *ConfrontoBitmap="Bitmap";
char *ConfrontoIndic="Indic";
int EIcona,EBitmap,EIndic,EIconReg,EDispReg;
char *IconaVcc1,*IconaVcc2,*IconaVcc3,*IconaVcc4;
char *BitmapVcc1;

EIcona = 1;
EBitmap = 1;

/* Prende la Classe ed il Nome della classe dell'Oggetto */
ClasseOggetto = XtClass (Oggetto);
NomeClasseOggetto = XlGetClassName (ClasseOggetto);
/*
NomeClasseOggetto = XtNewString ( XlGetClassName (ClasseOggetto) );
*/

/* Controlla se la classe trovata corrisponde con quella di una
        Icona (4 variabili di cambio colore ),
        con quella di un Bitmap o con un Indicatore 
        (1 variabile cambio colore) */

EIcona = strncmp (NomeClasseOggetto,ConfrontoIcona,strlen(ConfrontoIcona));
EIconReg = strncmp (NomeClasseOggetto,ConfrontoIconReg,strlen(ConfrontoIconReg));
EDispReg = strncmp (NomeClasseOggetto,ConfrontoDispReg,strlen(ConfrontoDispReg));
EBitmap = strncmp (NomeClasseOggetto,ConfrontoBitmap,strlen(ConfrontoBitmap));
EIndic = strncmp (NomeClasseOggetto,ConfrontoIndic,strlen(ConfrontoIndic));

narg = 0;

printf("DEBUG: EIndic=%d\n",EIndic);

if ((EIcona == 0) || (EIconReg == 0))
	{
	XtSetArg (arg[narg], XlNvarInputCambioColore1, &IconaVcc1); narg++;
	XtSetArg (arg[narg], XlNvarInputCambioColore2, &IconaVcc2); narg++;
	XtSetArg (arg[narg], XlNvarInputCambioColore3, &IconaVcc3); narg++;
	XtSetArg (arg[narg], XlNvarInputCambioColore4, &IconaVcc4); narg++;
	}
else if (EBitmap == 0 || EIndic == 0 || EDispReg == 0)
	{
	XtSetArg (arg[narg], XlNvarInputCambioColore1, &BitmapVcc1); narg++;
	}

/* Prende gli argomenti interessati  */
XtGetValues (Oggetto, arg, narg);

if ((EIcona == 0) || (EIconReg == 0))
	{
	if ( (IconaVcc1 != NULL)&&(IconaVcc1[0] != '\0') ) 
		return ( XtNewString (IconaVcc1) );
	if ( (IconaVcc2 != NULL)&&(IconaVcc2[0] != '\0') ) 
		return ( XtNewString (IconaVcc2) );
	if ( (IconaVcc3 != NULL)&&(IconaVcc3[0] != '\0') ) 
		return ( XtNewString (IconaVcc3) );
	if ( (IconaVcc4 != NULL)&&(IconaVcc4[0] != '\0') ) 
		return ( XtNewString (IconaVcc4) );
	}
else if (EBitmap == 0 || EIndic == 0 || EDispReg == 0)
	{
        printf("DEBUG:str=%s \n",XtNewString (BitmapVcc1));
	if ( (BitmapVcc1!=NULL)&&(BitmapVcc1[0]!='\0') ) 
		return ( XtNewString (BitmapVcc1) );
	}
return ("");
}

/*------------------------------------------------------------------
 * Function Name:   XlGetConfinfoCore				   *
 *								   *
 * Parameter:							   *
 *	Widget wid;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna un puntatore alla struttura confinfo di  *
 *		    XlCore					   *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/
XlConfInfo *XlGetConfinfoCore (Widget wid)
{
if(XlIsXlCore(wid))
         return (xlCoreClassRec.xlcore_class.confinfo);
else
	 return (xlManagerClassRec.xlmanager_class.confinfo);
}

/*------------------------------------------------------------------
 * Function Name:   XlGetConfinfoWid				   *
 *								   *
 * Parameter:							   *
 *	Widget wid;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna un puntatore alla struttura confinfo     *
 *		    relativa all'oggetto selezionato               *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/
XlConfInfo *XlGetConfinfoWid (Widget wid)
{
XlCoreWidgetClass wClasseCore;
XlManagerWidgetClass wClasseManager;
if(XlIsXlCore(wid))
	{
	wClasseCore = (XlCoreWidgetClass)XtClass(wid);
	return (wClasseCore->xlcore_class.confinfo);
	}
else
	{
	wClasseManager = (XlManagerWidgetClass)XtClass(wid);
	return (wClasseManager->xlmanager_class.confinfo);
	}
}

/*------------------------------------------------------------------
 * Function Name:   XlGetNumConfinfoWid				   *
 *								   *
 * Parameter:							   *
 *	Widget wid;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna il numero delle confinfo                 *
 *		    relative all'oggetto selezionato               *
 *								   *
 * Date: 20 Aprile 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/
int XlGetNumConfinfoWid (Widget wid)
{
XlCoreWidgetClass wClasseCore;
XlManagerWidgetClass wClasseManager;
if(XlIsXlCore(wid))
	{
	wClasseCore = (XlCoreWidgetClass)XtClass(wid);
	return (wClasseCore->xlcore_class.num_confinfo);
	}
else
	{
	wClasseManager = (XlManagerWidgetClass)XtClass(wid);
	return (wClasseManager->xlmanager_class.num_confinfo);
	}
}

/*------------------------------------------------------------------
 * Function Name:   XlGetZoom                               *
 *                                                                 *
 * Parameter:                                                      *
 *      Widget wid;             ID dell'oggetto da esaminare       *
 *                                                                 *
 * Desription:    ritorna un intero corrispondente
 *                allo zoom in % (200 = 2 volte).
 *                                                                 *
 * Date: 21 Luglio 1994         Ver: 1.0                           *
 *-----------------------------------------------------------------*/
int XlGetZoom (Widget wid)
{
XlCoreWidget cw;
XlManagerWidget mw;
if(XlIsXlManager(wid))
	{
	mw= (XlManagerWidget) wid;
	return(mw->xlmanager.fatt_zoom);
	}
else
	{
	cw = (XlCoreWidget) wid;
	return(cw->xlcore.fatt_zoom);
	}
}

/*------------------------------------------------------------------
 * Function Name:   XlGetRefreshFreq				   *
 *								   *
 * Parameter:							   *
 *	Widget wid;		ID dell'oggetto da esaminare	   *
 *								   *
 * Desription:	  ritorna un intero corrispondente, in decimi di   *
 *                  secondo, al tempo di refresh della pagina      *
 *                  di dive e' collocato lo widget                 *
 *								   *
 * Date: 21 Luglio 1994		Ver: 1.0			   *
 *-----------------------------------------------------------------*/
int XlGetRefreshFreq (Widget wid)
{
Display *Displ;
XrmDatabase DbWid;
char *tipo;
XrmValue Value;
int Numero;
Boolean Ritorno;

/*  Ricavo il display associato allo widget  */
Displ = XtDisplay ( XtParent (wid) );

/*  Ricavo il database delle risorse dal display  */
DbWid = XrmGetDatabase (Displ);

/*  Leggo la risorsa dal database  */
Ritorno = XrmGetResource (DbWid,XlNrefreshFreq,XlCRefreshFreq,&tipo,&Value);

/*  Se la lettura da database ha avuto successo memorizzo il valore letto
    nella variabile di ritorno  */
if (Ritorno)
  Numero = atoi (Value.addr);
else Numero = 0;

return (Numero);
}

/*------------------------------------------------------------------
 * Function Name:   XlFlagToPixel				   *
 *								   *
 * Parameter:							   *
 *	CAMBIO_COLORE cambio_colore; struttura del cambio colore   *
 *								   *
 * Desription:	  ritorna il pixel in funzione del valore del flag *
 *								   *
 * Date: 21 Ottobre 1994	Ver: 1.0			   *
 *-----------------------------------------------------------------*/
Pixel XlFlagToPixel(cambio_colore)
CAMBIO_COLORE cambio_colore;
{
float valore = cambio_colore.input.valore;


if(OlIsDigital(cambio_colore.input.pdb.tipo,cambio_colore.input.pdb.origin))
        /* punto digitale */
        {
	/* fuori scansione */
	if(cambio_colore.input.flag.flag_digitale.fs == 1)
                return(cambio_colore.fuoriScansione);

	/* automatico */
	if(cambio_colore.input.flag.flag_digitale.au == 1)
                return(cambio_colore.automatico);

	/* fuori attendibilita' */
	if(cambio_colore.input.flag.flag_digitale.fa == 1)
                return(cambio_colore.fuoriAttendib);

	/* stimato */
	if(cambio_colore.input.flag.flag_digitale.st == 1)
                return(cambio_colore.stimato);

	if(cambio_colore.input.flag.flag_digitale.sl == 1)
		return(cambio_colore.digitaleOn);
	else
		return(cambio_colore.basso_alto);
	}
else
	/* punto analogico */
	{
	/* fuori scansione */
	if(cambio_colore.input.flag.flag_analogico.fs == 1)
                return(cambio_colore.fuoriScansione);

	/* automatico */
	if(cambio_colore.input.flag.flag_analogico.au == 1)
                return(cambio_colore.automatico);

	/* fuori attendibilita' */
	if(cambio_colore.input.flag.flag_analogico.fa == 1)
                return(cambio_colore.fuoriAttendib);

	/* stimato */
	if(cambio_colore.input.flag.flag_analogico.st == 1)
                return(cambio_colore.stimato);

	if(cambio_colore.input.flag.flag_analogico.hs == 1)
		return(cambio_colore.altissimo);
	if(cambio_colore.input.flag.flag_analogico.ls == 1)
		return(cambio_colore.bassissimo);
	if(cambio_colore.input.flag.flag_analogico.hc == 1)
		return(cambio_colore.alto_altissimo);
	if(cambio_colore.input.flag.flag_analogico.lc == 1)
		return(cambio_colore.bassissimo_basso);
	return(cambio_colore.basso_alto);
	}
}


/*------------------------------------------------------------------
 * Function Name:   XlFlagToGC	        			   *
 *								   *
 * Parameter:							   *
 *	CAMBIO_COLORE cambio_colore; struttura del cambio colore   *
 *								   *
 * Desription:	  ritorna il GC  ini  funzione del valore del flag *
 *								   *
 * Date: 21 Ottobre 1994	Ver: 1.0			   *
 *-----------------------------------------------------------------*/
GC XlFlagToGC(cambio_colore)
CAMBIO_COLORE cambio_colore;
{
float valore = cambio_colore.input.valore;

if(OlIsDigital(cambio_colore.input.pdb.tipo,cambio_colore.input.pdb.origin))
        /* punto digitale */
        {
	/* fuori scansione */
	if(cambio_colore.input.flag.flag_digitale.fs == 1)
                return(cambio_colore.fuoriScansione_gc);

	/* automatico */
	if(cambio_colore.input.flag.flag_digitale.au == 1)
                return(cambio_colore.automatico_gc);

	/* fuori attendibilita' */
	if(cambio_colore.input.flag.flag_digitale.fa == 1)
                return(cambio_colore.fuoriAttendib_gc);

	/* stimato */
	if(cambio_colore.input.flag.flag_digitale.st == 1)
                return(cambio_colore.stimato_gc);

	if(cambio_colore.input.flag.flag_digitale.sl == 1)
		return(cambio_colore.digitaleOn_gc);
	else
		return(cambio_colore.basso_alto_gc);
	}
else
	/* punto analogico */
	{
	/* fuori scansione */
	if(cambio_colore.input.flag.flag_analogico.fs == 1)
                return(cambio_colore.fuoriScansione_gc);

	/* automatico */
	if(cambio_colore.input.flag.flag_analogico.au == 1)
                return(cambio_colore.automatico_gc);

	/* fuori attendibilita' */
	if(cambio_colore.input.flag.flag_analogico.fa == 1)
                return(cambio_colore.fuoriAttendib_gc);

	/* stimato */
	if(cambio_colore.input.flag.flag_analogico.st == 1)
                return(cambio_colore.stimato_gc);

	if(cambio_colore.input.flag.flag_analogico.hs == 1)
		return(cambio_colore.altissimo_gc);
	if(cambio_colore.input.flag.flag_analogico.ls == 1)
		return(cambio_colore.bassissimo_gc);
	if(cambio_colore.input.flag.flag_analogico.hc == 1)
		return(cambio_colore.alto_altissimo_gc);
	if(cambio_colore.input.flag.flag_analogico.lc == 1)
		return(cambio_colore.bassissimo_basso_gc);
	return(cambio_colore.basso_alto_gc);
	}
}


void XlGetCambioColoreGC(w,cambio_colore,font,clip,modo)
Widget w;
CAMBIO_COLORE *cambio_colore;
Font font;
Pixmap clip;
Boolean modo;  /* true per la stampante */
{
XlCoreWidget cw = (XlCoreWidget) w;
XGCValues values;
unsigned long valuemask;


valuemask = GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle | GCFunction;
if(clip != NULL)
	{
	valuemask =  valuemask | GCClipMask;
	values.clip_mask=clip;
	}


#if defined XPRINTER_USED
if(modo)
	values.background =XlPrintGetPixel(w,cw->core.background_pixel);
else
	values.background = cw->core.background_pixel;
#else
values.background = cw->core.background_pixel;
#endif
values.line_width = 0;
values.line_style = LineSolid;
values.function=GXcopy;


#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->automatico);
        cambio_colore->automatico_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->automatico;
        cambio_colore->automatico_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->automatico;
cambio_colore->automatico_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->stimato);
        cambio_colore->stimato_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->stimato;
        cambio_colore->stimato_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->stimato;
cambio_colore->stimato_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->fuoriScansione);
        cambio_colore->fuoriScansione_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->fuoriScansione;
        cambio_colore->fuoriScansione_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->fuoriScansione;
cambio_colore->fuoriScansione_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
        cambio_colore->fuoriAttendib_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->fuoriAttendib;
        cambio_colore->fuoriAttendib_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->fuoriAttendib;
cambio_colore->fuoriAttendib_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->digitaleOn);
        cambio_colore->digitaleOn_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->digitaleOn;
        cambio_colore->digitaleOn_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->digitaleOn;
cambio_colore->digitaleOn_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->bassissimo);
        cambio_colore->bassissimo_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->bassissimo;
        cambio_colore->bassissimo_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->bassissimo;
cambio_colore->bassissimo_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->bassissimo_basso);
        cambio_colore->bassissimo_basso_gc = XCreateGC(XlDisplay(w),
		XlWindow(w), valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->bassissimo_basso;
        cambio_colore->bassissimo_basso_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->bassissimo_basso;
cambio_colore->bassissimo_basso_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->basso_alto);
        cambio_colore->basso_alto_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->basso_alto;
        cambio_colore->basso_alto_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->basso_alto;
cambio_colore->basso_alto_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->alto_altissimo);
        cambio_colore->alto_altissimo_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->alto_altissimo;
        cambio_colore->alto_altissimo_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->alto_altissimo;
cambio_colore->alto_altissimo_gc = XtGetGC(w, valuemask, &values);
#endif

#if defined XPRINTER_USED
if(modo)
	{
	values.foreground = XlPrintGetPixel(w,cambio_colore->altissimo);
        cambio_colore->altissimo_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
	}
else
	{
	values.foreground = cambio_colore->altissimo;
        cambio_colore->altissimo_gc = XtGetGC(w, valuemask, &values);
	}
#else
values.foreground = cambio_colore->altissimo;
cambio_colore->altissimo_gc = XtGetGC(w, valuemask, &values);
#endif

}

void XlFreeCambioColoreGC(w,cambio_colore,modoPrint)
Widget w;
CAMBIO_COLORE cambio_colore;
Boolean modoPrint;
{

if(cambio_colore.automatico_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.automatico_gc);
	else
        	XtReleaseGC(w,cambio_colore.automatico_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.automatico_gc);
#endif
if(cambio_colore.stimato_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.stimato_gc);
	else
        	XtReleaseGC(w,cambio_colore.stimato_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.stimato_gc);
#endif
if(cambio_colore.fuoriScansione_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.fuoriScansione_gc);
	else
        	XtReleaseGC(w,cambio_colore.fuoriScansione_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.fuoriScansione_gc);
#endif
if(cambio_colore.fuoriAttendib_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.fuoriAttendib_gc);
	else
        	XtReleaseGC(w,cambio_colore.fuoriAttendib_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.fuoriAttendib_gc);
#endif
if(cambio_colore.digitaleOn_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.digitaleOn_gc);
	else
        	XtReleaseGC(w,cambio_colore.digitaleOn_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.digitaleOn_gc);
#endif
if(cambio_colore.bassissimo_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.bassissimo_gc);
	else
        	XtReleaseGC(w,cambio_colore.bassissimo_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.bassissimo_gc);
#endif
if(cambio_colore.bassissimo_basso_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.bassissimo_basso_gc);
	else
        	XtReleaseGC(w,cambio_colore.bassissimo_basso_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.bassissimo_basso_gc);
#endif
if(cambio_colore.basso_alto_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.basso_alto_gc);
	else
        	XtReleaseGC(w,cambio_colore.basso_alto_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.basso_alto_gc);
#endif
if(cambio_colore.alto_altissimo_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.alto_altissimo_gc);
	else
        	XtReleaseGC(w,cambio_colore.alto_altissimo_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.alto_altissimo_gc);
#endif
if(cambio_colore.altissimo_gc)
#if defined XPRINTER_USED
	{
	if(modoPrint)
		XFreeGC(XlDisplay(w),cambio_colore.altissimo_gc);
	else
        	XtReleaseGC(w,cambio_colore.altissimo_gc);
	}
#else
        	XtReleaseGC(w,cambio_colore.altissimo_gc);
#endif
}





void XlInitializeWidgetValue(Widget w)
{
XlConfInfo *confinfo;
int num_confinfo,i;
PUNT_VARINP varinp;
PUNT_VAROUT varout;
ALL_HIERARCHY varall;
	
	confinfo=XlGetConfinfoWid(w);
	num_confinfo=XlGetNumConfinfoWid(w);
	for(i=0;i<num_confinfo;i++)
	  switch(confinfo[i].confinfo_type){
             case XlRTipoVarInp:
             case XlRTipoVarInp+LOCKED_PAG:
             case XlRTipoVarReg:
             case XlRTipoVarReg+LOCKED_PAG:
		{
		memcpy(&varinp,(char*)(confinfo[i].compiled_offset+(char *)w),
                        confinfo[i].compiled_size);
		varinp.valore = 0.;
		varinp.pdb.nmod= 0;
		varinp.pdb.indice = -1;
		varinp.pdb.indice_rel = -1;
		varinp.pdb.tipo = 0;
		varinp.unimis.a =1.;
		varinp.unimis.b =0.;
		memset(&(varinp.flag),0,sizeof(FLAG_MMI));
		memcpy((char*)(confinfo[i].compiled_offset+(char *)w),
                        &varinp, confinfo[i].compiled_size);
		break;
		}
             case XlRTipoVarOut:
             case XlRTipoVarOut+LOCKED_PAG:
		{
		memcpy(&varout,(char*)(confinfo[i].compiled_offset+(char *)w),
                        confinfo[i].compiled_size);
		varout.value = 0.;
		varout.pdb.nmod= 0;
		varout.pdb.indice = -1;
		varout.pdb.indice_rel = -1;
		varout.pdb.tipo = 0;
		varout.unimis.a =1.;
		varout.unimis.b =0.;
		memcpy((char*)(confinfo[i].compiled_offset+(char *)w),
                        &varout, confinfo[i].compiled_size);
		break;
		}
	  case XlRGerarchia:
		{
		/* carico la risorsa da confinfo */
		/*memcpy(&varall,(char*)(confinfo[i].compiled_offset+(char *)w),
                        confinfo[i].compiled_size);
                */
		varall.input_valore=0;
		strcpy(varall.str_input,"");

		/* scarico la risorsa inizializzata in confinfo */
/********************
		memcpy((char*)(confinfo[i].compiled_offset+(char *)w),
                        &varall, confinfo[i].compiled_size);
*********************/
		memcpy((char*)(confinfo[i].compiled_offset+(char *)w),
                        &varall, sizeof(ALL_HIERARCHY));
		break;
		}
	  default:
	  	break;
	  }
}

/*
	la funzione ritorna True se il flag di forzato e' attivo
*/
Boolean XlFlagFzIsOn(cambio_colore)
CAMBIO_COLORE cambio_colore;
{
if(OlIsDigital(cambio_colore.input.pdb.tipo,cambio_colore.input.pdb.origin))
        /* punto digitale */
        {
	if(cambio_colore.input.flag.flag_digitale.fz == 1)
                return(True);
	else
                return(False);
	}
else
	{
	if(cambio_colore.input.flag.flag_analogico.fz == 1)
                return(True);
	else
                return(False);
	}
}

/*------------------------------------------------------------------
 * Function Name:   XlFlagRiIsOn				   *
 *								   *
 * Parameter:							   *
 *	CAMBIO_COLORE cambio_colore; struttura del cambio colore   *
 *								   *
 * Desription:	  ritorna True se il flag ri e' settato            *
 *								   *
 * Date: 20 Dicembre 1994	Ver: 1.0			   *
 *-----------------------------------------------------------------*/
Boolean XlFlagRiIsOn(cambio_colore)
CAMBIO_COLORE cambio_colore;
{

if(OlIsDigital(cambio_colore.input.pdb.tipo,cambio_colore.input.pdb.origin))
        /* punto digitale */
        {
	if(cambio_colore.input.flag.flag_digitale.ri == 1)
                return(True);
	else
		return(False);
	}
else
        /* punto analogico */
        {
	if(cambio_colore.input.flag.flag_analogico.ri == 1)
                return(True);
	else
		return(False);
	}
}


/*------------------------------------------------------------------
 * Function Name:   XlAddBlink				           *
 *								   *
 * Parameter:							   *
 *             Widget:   oggetto a cui aggiungere il blink         *
 *								   *
 * Desription:	  ritorna True se l'operazione e' stata eseguita   *
 *								   *
 * Date: 27 Novembre 1995	Ver: 2.0
 *
 * (Rispetto alla versione precedente � stato aggiunto il parametro *
 * frequenza)			   *
 *-----------------------------------------------------------------*/

//Boolean XlAddBlink(Widget w, int ind, float frequenza, blinkcallbackproc funzione)
Boolean XlAddBlink(Widget w, int ind, float frequenza, void * funzione)
{
int i;
int contatore_comune;
Boolean stato_comune,freq_comune_found;

contatore_comune=0;
stato_comune=False;	/*stato dei blink alla=freq*/
freq_comune_found=False;/*blink con la stessa freq. trovato*/	

if (frequenza==0)
	{
	fprintf(stderr,"Error: Blink Frequency cannot be 0\n");
	return(False);
	}
	/* prima allocazione */
if(blink_allocati==0)
	{
	blink_allocati=100;
	lista_blink=(LISTA_BLINK*)
		XtMalloc(blink_allocati*sizeof(LISTA_BLINK));
	if(lista_blink==NULL)
        {
	        fprintf(stderr,"Error: malloc of lista_blink\n");
		return(False);
        }

	for(i=0;i<blink_allocati;i++)
		{
		lista_blink[i].w=NULL;
		lista_blink[i].ind= -1;
		lista_blink[i].freq=-1;/*frequenza di blink*/
		lista_blink[i].counter=-1; /*contatore*/
		lista_blink[i].stato=-1; /*stato*/
		}
	}
/*
Sincronizza il blink degli oggetti aventi la stessa frequenza		
*/


for(i=0;i<blink_allocati;i++)
	{
	if(frequenza==lista_blink[i].freq)
		{
		contatore_comune=lista_blink[i].counter;          
		stato_comune=lista_blink[i].stato;
		freq_comune_found=True;
		}
	}
	
		
for(i=0;i<blink_allocati;i++)
	{

	if(lista_blink[i].w==NULL)
		{
		++num_blink;
		lista_blink[i].w=w;
		lista_blink[i].ind=ind;
		lista_blink[i].funzione=funzione;
		lista_blink[i].freq=frequenza;/*frequenza di blink*/

/*
Verifica se esistono altri oggetti con la stessa frequenza
*/	
		if(freq_comune_found)
			{
			/*counter=a quello degli oggetti con la stessa freq*/
			lista_blink[i].counter=contatore_comune;
			lista_blink[i].stato=stato_comune;
			}
		else			
			{
/*
counter=semiperiodo/BLANK_RATE(in ms):(cio� counter=n� di volte che XlBlink 
viene invocata senza cambiare lo stato  di Blink dell' oggetto)
*/
			lista_blink[i].counter=1000/(2*frequenza*BLINK_RATE);
			lista_blink[i].stato=False;/*Blink Off*/
			}
		return(True);
		}/*end if lista_blink[i].w==NULL*/
	}/*end for */
	

	/* manca spazio */
	lista_blink=(LISTA_BLINK*)XtRealloc(lista_blink,
			(blink_allocati+100)*sizeof(LISTA_BLINK));
	if(lista_blink==NULL)
        {
           fprintf(stderr,"Error: in realloc of lista_blink\n");
	   return(False);
        }


	for(i=blink_allocati;i<blink_allocati+100;i++)
			{
			lista_blink[i].w=NULL;
			lista_blink[i].ind= -1;
			lista_blink[i].freq=-1;
			lista_blink[i].counter=-1;
			lista_blink[i].stato=-1;
			}
	++num_blink;
	lista_blink[blink_allocati].w=w;
	lista_blink[blink_allocati].ind=ind;
	lista_blink[blink_allocati].funzione=funzione;
	lista_blink[blink_allocati].freq=frequenza; /*frequenza di blink*/
	if(freq_comune_found)
		{
		lista_blink[blink_allocati].counter=contatore_comune;
		lista_blink[blink_allocati].stato=stato_comune;
		}
	else
		{	
		lista_blink[blink_allocati].stato=False; /* Blink Off */
		lista_blink[blink_allocati].counter=1000/(2*frequenza*BLINK_RATE);
		}

	blink_allocati += 100;
	return(True);
}

/*------------------------------------------------------------------
 * Function Name:   XlRemoveBlink			           *
 *								   *
 * Parameter:							   *
 *             Widget:   oggetto a cui aggiungere il blink         *
 *								   *
 * Desription:	  ritorna True se l'operazione e' stata eseguita   *
 *								   *
 * Date: 27 Novembre 1995	Ver: 2.0			   *
 *-----------------------------------------------------------------*/
Boolean XlRemoveBlink(Widget w,int ind)
{
int i;

	--num_blink;
	for(i=0;i<blink_allocati;i++)
		if((lista_blink[i].w==w)&&(lista_blink[i].ind==ind))
			{
			lista_blink[i].w=NULL;
			lista_blink[i].ind= -1;
			lista_blink[i].freq= -1;
			lista_blink[i].counter= -1;
			lista_blink[i].stato= -1;
			return(True);
			}
	return(False);
}
/*------------------------------------------------------------------
 * Function Name:   XlGetBlink			           *
 *								   *
 * Parameter:							   *
 *             Widget:   oggetto da cercare nella lista_blink         *
 *								   *
 * Desription:	  ritorna True se l'operazione e' stata eseguita   *
 *								   *
 * Date: 27 Novembre 1995	Ver: 2.0			   *
 *-----------------------------------------------------------------*/
float XlGetBlink(Widget w,int ind)
{
/*
fuzione utilizzata per vedere se un oggetto � gi� stato inserito
nella lista_blink.Restituisce la frequenza dell' oggetto se trovato,
altrimenti restituisce 0*/
register int i;


	for(i=0;i<blink_allocati;i++)
		{
		if((lista_blink[i].w==w)&&(lista_blink[i].ind==ind))
			{
			return((float)(lista_blink[i].freq));
			}
		}
		return(0);
}

/*------------------------------------------------------------------
 * Function Name:   XlBlink      			           *
 *								   *
 *								   *
 * Desription:	  richiama la funzione di blink per gli oggetti in *
 *                lista                                            *
 *								   *
 * Date: 27 Novembre 1995	Ver: 2.0			   *
 *-----------------------------------------------------------------*/
void XlBlink(caddr_t client_data,XtIntervalId * id)
{
register int i;
XtIntervalId timer_id;
static On=False;

XlIndicClassRec *wIndicClass;

   
   for(i=0;i<blink_allocati;i++)
       if(lista_blink[i].w!=NULL)
       	  {
       	  if((lista_blink[i].counter)==0)          /*Se count==0 On=!On*/
       	  	{
       	  	
       	  	lista_blink[i].counter=1000/(2*(lista_blink[i].freq)*BLINK_RATE)-1;
		if(lista_blink[i].counter<0) lista_blink[i].counter=0;
/*
Cambia stato di Blink 
*/       	  	 
       	  	lista_blink[i].stato=!lista_blink[i].stato;
       	        lista_blink[i].funzione(lista_blink[i].w,lista_blink[i].ind,lista_blink[i].stato);
       	  		
       	  	}
       	  else	{
       	  	--(lista_blink[i].counter);	/*dec counter*/
       	  	}
/*
Invoca la funzione di blink relativa all' oggetto in esame
*/       	  	
	  }  
 
 /*prossima invocazione di XlBlink tra BLINK_RATE millisecondi */
 
     timer_id = XtAppAddTimeOut (
            XtWidgetToApplicationContext ((Widget)client_data),
            (unsigned long) BLINK_RATE, XlBlink, (Widget)client_data);
}


/*
	ritorna True se l'oggetto e' una sotto classe di XlCore
	o XlManager
	False per tutti gli altri oggetti Motif
*/

Boolean XlIsXlWidget(Widget w)
{
if(XtIsSubclass(w,&xlCoreClassRec))
        return(True);
if(XtIsSubclass(w,&xlManagerClassRec))
        return(True);

return(False);
}




Cursor XlCreateWaitCursor(Widget wid)
{
Pixmap shape,mask;
XColor color_1,color_2;
Colormap colormap;
Cursor cursor;

shape = XCreatePixmapFromBitmapData(XtDisplay(wid),
                RootWindow(XtDisplay(wid),DefaultScreen(XtDisplay(wid))),
                wait_bits,wait_width,wait_height,1,0,1);
mask = XCreatePixmapFromBitmapData(XtDisplay(wid),
                RootWindow(XtDisplay(wid),DefaultScreen(XtDisplay(wid))),
                wait_mask_bits,wait_mask_width,
                wait_mask_height,1,0,1);
colormap = DefaultColormap ( XtDisplay(wid),
                        DefaultScreen (XtDisplay(wid)) );
XParseColor(XtDisplay(wid),colormap,"red",&color_1);
XParseColor(XtDisplay(wid),colormap,"black",&color_2);
cursor=XCreatePixmapCursor(XtDisplay(wid),shape,mask,&color_1,&color_2,
        wait_x_hot,wait_y_hot);
XFreePixmap(XtDisplay(wid),shape);
XFreePixmap(XtDisplay(wid),mask);
return(cursor);
}

Cursor XlCreateErrCursor(Widget wid)
{
Pixmap shape,mask;
XColor color_1,color_2;
Colormap colormap;
Cursor cursor;

shape = XCreatePixmapFromBitmapData(XtDisplay(wid),
                RootWindow(XtDisplay(wid),DefaultScreen(XtDisplay(wid))),
                err_bits,err_width,err_height,1,0,1);
mask = XCreatePixmapFromBitmapData(XtDisplay(wid),
                RootWindow(XtDisplay(wid),DefaultScreen(XtDisplay(wid))),
                err_mask_bits,err_mask_width,
                err_mask_height,1,0,1);
colormap = DefaultColormap ( XtDisplay(wid),
                        DefaultScreen (XtDisplay(wid)) );
XParseColor(XtDisplay(wid),colormap,"white",&color_1);
XParseColor(XtDisplay(wid),colormap,"red",&color_2);
cursor=XCreatePixmapCursor(XtDisplay(wid),shape,mask,&color_1,&color_2,
        err_x_hot,err_y_hot);
XFreePixmap(XtDisplay(wid),shape);
XFreePixmap(XtDisplay(wid),mask);
return(cursor);
}

void XlSetCursor(Widget wid, Cursor cursor)
{
XDefineCursor(XtDisplay(wid),XtWindow(wid),cursor);
XmUpdateDisplay(wid);
}

void XlUnsetCursor(Widget wid)
{
XUndefineCursor(XtDisplay(wid),XtWindow(wid));
}

Boolean XlFreeCursor(Widget wid, Cursor cursor)
{
if(cursor == NULL)
	return(False);
XFreeCursor(XtDisplay(wid),cursor);
}
/*-----------------------------------------------------
 *
 * hierarchy_format_new
 *
 * Elabora l' ingresso da risorsa con gerarchia scritta in formato interno, ad es.:
 * (3,-1,-1,-1,-1,-1\0) 
 * e fornisce una stringa formattata in formato UI, nel seguente modo, vedi (SCADA Marcello):
 * ([3][-1]...[-1]\0)
 * Ritorna una stringa col formato voluto se tutto OK altrimenti ritorna \0 (NULL)
 *-----------------------------------------------------*/

char *hierarchy_format_new(char *result ,char * position)
{

char    *string, *string1, str_liv[10] ;
int     lungh, int_liv, i;


string=position;
strcpy(result,"[");
i=0;
       
while ((string1=strchr(string, ',')) != NULL) {
        i++;
        lungh = strlen(string) - strlen(string1);
        strncpy(str_liv, string, lungh);
        str_liv[lungh] = '\0';
        string = (string1+1);
        /* strcpy(string, (string1+1)); */
        strcat(result,str_liv);
        strcat(result,"][");
}

if(i==0) {
        printf("hierarchy_format_new: Error in hierarchy resource!\nCharacter ',' not found!\n");
        return(GERARCHIA_KO);
}
else {
        if(i!=(NUMLIVELLI-1)) {
                printf("hierarchy_format_new: Error in hierarchy resource!\nNumber of levels different than six!\n");
                return(GERARCHIA_KO);
        }
        else {
                strcat(result,string);
                strcat(result,"]");
                /*printf("result=%s", result);*/ /* debug */
        }
}
return(result);
}
/*-----------------------------------------------------
 *
 * format_hierarchy_new
 *
 * Elabora l' ingresso della gerarchia scritta in formato UI: 
 * ([0] [-1]..[-1]) 
 * e fornisce una stringa formattata nel seguente modo interno con terminatore finale \0 :
 * (0,0,0,-1,-1,-1\0)
 * Ritorna una stringa col formato voluto se tutto OK
 * altrimenti ritorna NULL
 *-----------------------------------------------------*/

char *format_hierarchy_new(char *result,char * position)
{
char *p,appo[256];
unsigned int lung;
int contr_lung;
register int i;

/*
printf("format_hierarchy:position=%s\n",position);
*/
/*
Ricerco la coppia di caratteri []. Se esiste, un livello non e' stato qualificato
*/
if(strstr(position,"[]")!=NULL)
        {
        printf("Attenzione!! Input gerarchia non corretto\n");
        return(GERARCHIA_KO);
        }
/*
Controllo per verificare quanti []
ci sono
*/
i=0;
p=strstr(position,"][");
while(p)
        {
        *p++;
        p=strstr(p,"][");
        i++;
        };
if(i!=5)
        {
        printf("format_hierachy_new 1: Errore nella gerarchia\n");
        return(GERARCHIA_KO);
        }
/*
Controllo lunghezza stringa
*/
if(strlen(position)>40)
        {
        printf("format_hierachy_new 2: Errore nella gerarchia\n");
        return(GERARCHIA_KO);
        }
/*
Prima parte analisi stringa differente per differente strtok su alpha
*/
strcpy(appo,position);
p=strtok(appo,"]");     /*leggo il primo valore*/
/*
printf("DEBUG format_hierarchy_new !! primo p=%s\n",p);
*/
/*
Ora *p=[ mentre *(p+1)=prima cifra o car non valido
*/
if((*p=='[')&&(*(p+1)==0)) {  /*Se non c' e' alcun valore metti NULL*/
	printf("Attenzione!! Input gerarchia non corretto\n");
        return(GERARCHIA_KO);
}
else
{
        sscanf(p+1,"%d",&contr_lung);
        if((contr_lung>254)||(contr_lung< -1))
                 {
                 printf("Valore in input gerarchia troppo grande\n");
                 return(GERARCHIA_KO);
                 }
/*
Controllo del num caratteri destinati ad ogni livello
*/
        if((lung=strlen(p+1))>3)
                {
                printf("Attenzione!!! input gerarchia non corretto\n");
                return(GERARCHIA_KO);
                }
        strcpy(result,p+1);
        strcat(result,",");
}/*end if *p==[*/
/*
Scandisco i rimanenti token
*/
i=0;
while ((p = strtok((char *)NULL,"]["  ))!= NULL) {
/*
        printf("DEBUG format_hierarchy_new !! p=%s\n",p);
*/
        if(lung=strlen(p)>3)    /*contr. no. car gerarchia*/
                {
                printf("Attenzione!!! Input gerarchia non corretto\n");
                return(GERARCHIA_KO);
                }
        i++;
        sscanf(p,"%d",&contr_lung);
       /* printf("Valore contr_lung=%d\n",contr_lung);*/
        if((contr_lung>254)||(contr_lung<-1))
               {
                printf("Valore in input gerarchia troppo grande\n");
                return(GERARCHIA_KO);
                }
        strcat(result, p);
        if(i<5)
                strcat(result, ",");
}/*end while*/
        result[strlen(result)] = '\0';
/*
printf("format_hierarchy:result=%s\n",result);
*/
return(result);
}


/*************************************************************
 *  Convertitore da Stringa a Pixel
 *  converte la stringa "#fffffffff" nel formato Pixel
 *************************************************************/

typedef struct Xl_Colore
                {
                char *colorname;
                XColor xcol;
		Display *disp;
                int screen;
                } XlColore;

typedef struct Xl_Lista_Colori
                {
                int num_colors;
                XlColore *xlcolor;
                } XlListaColori;

XlListaColori xllista_colori={0,NULL};



Boolean XlCvtStrToPixel (disp,Stringa,Pix,
                red,green, 
                blue)
Display *disp;
char *Stringa;
Pixel *Pix;
unsigned short *red, *green, *blue;
{
int i;
Colormap colmap;
XColor xcol;

/*
 esamina se il colore e' gia' presente in lista
*/
for(i=0;i<xllista_colori.num_colors;i++)
        {
        if( (disp == xllista_colori.xlcolor[i].disp) &&
            (DefaultScreen(disp) == xllista_colori.xlcolor[i].screen) &&
            (strcmp(xllista_colori.xlcolor[i].colorname,Stringa)==NULL) )
                {
                *Pix =xllista_colori.xlcolor[i].xcol.pixel;
                *red=xllista_colori.xlcolor[i].xcol.red;
                *green=xllista_colori.xlcolor[i].xcol.green;
                *blue=xllista_colori.xlcolor[i].xcol.blue;
                return(True);
                }
        }
/*
 Se il colore non e' presente lo carica in lista
*/
colmap = DefaultColormap ( disp,
             DefaultScreen (disp) );
if ( !XParseColor (disp , colmap , Stringa , &xcol) )
           {
           xcol.pixel = 0xffff;
           printf("Errore XParseColor: colore [%s] inesistente\n",Stringa);
           return(False);
           }
if(!XAllocColor(disp,colmap,&xcol))
           {
           xcol.pixel = 0xffff;

           printf("Errore XAllocColor: impossibile allocare il nuovo colore %s\n",Stringa);
           return(False);
           }

xllista_colori.num_colors++;
xllista_colori.xlcolor=(XlColore *)XtRealloc(xllista_colori.xlcolor,
                                 xllista_colori.num_colors *
                                                  sizeof(XlColore));
xllista_colori.xlcolor[i].colorname=XtNewString(Stringa);
xllista_colori.xlcolor[i].disp=disp;
xllista_colori.xlcolor[i].screen=DefaultScreen(disp);
xllista_colori.xlcolor[i].xcol=xcol;

*Pix = xcol.pixel;
*red=xcol.red;
*green=xcol.green;
*blue=xcol.blue;
return(True);
}



/*************************************************************
 *  Convertitore risorsa AnimatedColor
 *  converte la risorsa dal formato stringa "#fffffffff  2 1"
 *  a formato AnimatedPixel
 *  Pixel + blink frequency + TrasparentOn
 *  E' ammessa l'assenza della blink frequency.
 *************************************************************/

Boolean XlCvtAnimatedColor ( Display *display,
                                    XrmValue *args,
                                    Cardinal *num_args,
                                    XrmValuePtr fromVal,
                                    XrmValuePtr toVal,
                                    XtPointer *converter_data
                                  )
{
   unsigned short dummy;
   char str_pixel[30];
   float blink_rate;
   Boolean trasparent;
   int str_read;
   static XlAnimatedColor AnCol;

   if( *num_args != 0 )
      fprintf(stderr,"XlrCvtAnimatedColor WARNING extra arguments received\n");


   str_read = sscanf( (char *)fromVal->addr,"%s %f %d",str_pixel,&blink_rate,&trasparent);

   if( str_read < 1 )
   {
      fprintf(stderr,"Error in converter  XlrCvtAnimatedColor\n");
      return(False);
   }
   else if( str_read == 1 )
   {
      XlCvtStrToPixel (display,str_pixel,&(AnCol.colore),&dummy,&dummy,&dummy);
      AnCol.blink_rate = 0;
      AnCol.trasparent = False;
      done( &AnCol, toVal);
   }
   else if( str_read == 2)
   {
      XlCvtStrToPixel (display,str_pixel,&(AnCol.colore),&dummy,&dummy,&dummy);
      AnCol.blink_rate = blink_rate;
      AnCol.trasparent = False;
      done( &AnCol, toVal);
   }
   else if( str_read == 3)
   {
      XlCvtStrToPixel (display,str_pixel,&(AnCol.colore),&dummy,&dummy,&dummy);
      AnCol.blink_rate = blink_rate;
      AnCol.trasparent = trasparent;
      done( &AnCol, toVal);
   }

   return(True);
}


/* XlValueToBlinkPixmap
 * restituisce indice del pixmap,colore e frequenza di blink
 * in base al valore assunto dalla variabile di cambio colore
 * (la quale e'composta da 6 variabili massimo con possibilita'
 *  di uso dei flag per le variabili scada)
 */
int XlValueToBlinkPixmap(Widget w,int *indpix,
                        Pixel *pix,float *brate)
{
   XlPixvarWidget cw = (XlPixvarWidget) w;
   int i,j;
   Boolean found=False;
   XlPictographAnimaz *prg;
   unsigned short varcalc,valflag,mask;
   unsigned short shiftbit;
   int valprg;
   int origin,offset,valore;
   XlPixvarClassRec *wPixvarClass;

   varcalc = 0;
   valflag = 0;
   valprg = 0;
   origin = -1;
   offset = -1;
   valore = 0;
   mask = 0x1;

   if( w == NULL)
   {
      *indpix =0;
      *pix = 0;
      *brate = 0.0;
      return(False);
   }

   /* ciclo per tutti i sei contribuenti della variabile cambio colore
   */
   for(i=0;i<6;i++)
   {

     valore = 0;
     valflag = 0;
     offset = -1;
     origin = -1;

     /* la variabile non e' stata configurata - 
        setto il valore a 0 e l'origine come SIMULATA
     */
     if( cw->pixvar.cambio_colore[0].pict_var[i].input.pdb.indice == -1 )    
        valore = 0;
     else
     {
        /* determino se variabile scada o simulatore
        */
        origin = cw->pixvar.cambio_colore[0].pict_var[i].input.pdb.origin;
        
        /* nel caso di variabile simulata utilizzo il valore della variabile
        */
        if( origin == SIMULAID )
           valore = (int)cw->pixvar.cambio_colore[0].pict_var[i].input.valore;
        else
        {
           /* nel caso di variabile scada utilizzo il bit nella posizione specificata
              dall'offset (definito dall'utente)
           */
           offset = cw->pixvar.cambio_colore[0].pict_var[i].FlagOffset;
	   valflag = cw->pixvar.cambio_colore[0].pict_var[i].input.flag.flag_valore;
           shiftbit = mask<<(offset);
           valore = valflag & shiftbit ; 

/*
printf("var = %s\n",cw.pixvar.cambio_colore[0].pict_var[i].str_input);
printf("offset = %d\n",offset);
printf("valflag = %d\n",valflag);
printf("shiftbit = %d\n",shiftbit);
printf("valflag & shiftbit = %d\n",valore);
*/

        }
     }


     /* costruisco il byte di stato che andra' confrontato
        con quello specificato nel .prg dal configuratore
        se il valore e' >0 setto a 1 il bit nella posizione 
        iesima della variabile
     */
     if( valore > 0 )
       varcalc = varcalc | (mask<<i);
     else
       varcalc = varcalc & (~(mask<<i));
   }

/*
printf("valore variabile calcolata = %d\n",varcalc);
*/

   wPixvarClass=(XlPixvarClassRec *)XtClass(w);
   cw->pixvar.program = &(wPixvarClass->xlpixvar_class.programs[cw->pixvar.index_program]);
   prg = cw->pixvar.program;

   if( prg == NULL)     
   {
      *indpix =0;
      *pix = 0;
      *brate = 0.0;
      return(False);
   }

   /* confronto il valore calcolato con i valori contenuti
    * nella lista dei valori in prg (caricati dal file <nome_program>.prg) 
   */
   i=0;
   while( prg->row[i].idvar != -1)
   {
        for(j=0;j<prg->row[i].nval;j++)
        {
/*
           printf("prg->row[%d].val[%d]=%d\n",i,j,prg->row[i].val[j]);
*/
           if( varcalc == prg->row[i].val[j] )
           {
/*
              printf("coincidente al pixmap=%d\n",prg->row[i].idvar);
*/
              *indpix = prg->row[i].idvar;
              *pix = prg->row[i].animaz.colore;
              *brate = prg->row[i].animaz.blink_rate;
              found = True;
              break;
           }
       }
     i++;
   }

   if( !found )
   {
      /* restituisco il colore e la frequenza configurata come default
      */
      *indpix = 1;
      *pix = cw->pixvar.cambio_colore[0].defaultColor.colore;
      *brate = cw->pixvar.cambio_colore[0].defaultColor.blink_rate;
   }

   return(True);
}


/* XlValueToBlinkPixel
 * restituisce il colore e la frequenza di blink in base
 * allo stato dei flag della variabile scada o al valore
 * della variabile simulata
 * ritorna True se il valore del flag e' compreso nella lista
 * ritorna False altrimenti (con il colore e la blink freq. 
 *                            relativa allo stato normale)
*/
int XlValueToBlinkPixel(Widget cw,int idzone,
                        Pixel *pix,float *brate)
{
   XlPictographWidget w= (XlPictographWidget)cw;
   int i,j;
   Boolean found=False;
   XlPictographAnimaz *prg;
   unsigned short varcalc,valflag,mask;
   unsigned short shiftbit;
   int valprg;
   int origin,offset,valore;
   XlPictographClassRec *wPictographClass;
   
   varcalc = 0;
   valflag = 0;
   valprg = 0;
   origin = -1;
   offset = -1;
   valore = 0;
   mask = 0x1;

   if( cw == NULL)
      return(False);

   /* ciclo per tutti i sei contribuenti 
   */
   for(i=0;i<6;i++)
   {

     valore = 0;
     valflag = 0;
     offset = -1;
     origin = -1;

     /* la variabile non e' stata configurata - 
        setto il valore a 0 e l'origine come SIMULATA
     */
     if( w->pictograph.cambio_colore[idzone].pict_var[i].input.pdb.indice == -1 )    
        valore = 0;
     else
     {
        /* determino se variabile scada o simulatore
        */
        origin = w->pictograph.cambio_colore[idzone].pict_var[i].input.pdb.origin;
        
        /* nel caso di variabile simulata utilizzo il valore della variabile
        */
        if( origin == SIMULAID )
           valore = (int)w->pictograph.cambio_colore[idzone].pict_var[i].input.valore;
        else
        {
           /* nel caso di variabile scada utilizzo il bit nella posizione specificata
              dall'offset (definito dall'utente)
           */
           offset = w->pictograph.cambio_colore[idzone].pict_var[i].FlagOffset;
	   valflag = w->pictograph.cambio_colore[idzone].pict_var[i].input.flag.flag_valore;
           shiftbit = mask<<(offset);
           valore = valflag & shiftbit ; 

/*
printf("var = %s\n",w.pictograph.cambio_colore[idzone].pict_var[i].str_input);
printf("offset = %d\n",offset);
printf("valflag = %d\n",valflag);
printf("shiftbit = %d\n",shiftbit);
printf("valflag & shiftbit = %d\n",valore);
*/

        }
     }


     /* costruisco il byte di stato che andra' confrontato
        con quello specificato nel .prg dal configuratore
        se il valore e' >0 setto a 1 il bit nella posizione 
        iesima della variabile
     */
     if( valore > 0 )
       varcalc = varcalc | (mask<<i);
     else
       varcalc = varcalc & (~(mask<<i));
   }

/*
Modifica by Fabio
*/
 wPictographClass=(XlPictographClassRec *)XtClass(w);
   w->pictograph.program = &(wPictographClass->xlpictograph_class.programs[w->pictograph.index_program]);
   prg = w->pictograph.program;

   if( prg == NULL)     
      return(False);

   /* confronto il valore calcolato con i valori contenuti
    * nella lista dei valori in prg (caricati dal file <nome_program>.prg) 
   */
   i=0;
   while( prg->row[i].idvar != -1)
   {
     if( prg->row[i].idvar == idzone)
     {
        for(j=0;j<prg->row[i].nval;j++)
           if( varcalc == prg->row[i].val[j] )
           {
              *pix = prg->row[i].animaz.colore;
              *brate = prg->row[i].animaz.blink_rate;
              found = True;
              break;
           }
     }
     i++;
   }

   if( found )
      if( (*brate) > 0 )
         return(True);
      else
         return(False);
   else
   {
      /* restituisco il colore e la frequenza di stato normale
      */
      *pix = w->pictograph.cambio_colore[idzone].defaultColor.colore;
      *brate = w->pictograph.cambio_colore[idzone].defaultColor.blink_rate;
      if( (*brate) > 0 )
         return(True);
      else 
         return(False);
   }
   return(False);
}

typedef struct Xl_Font
                {
                char *fontname;
                XFontStruct *font_info;
                int counter;
		char dispname[255];
                } XlFont;

typedef struct Xl_Lista_Font
                {
                int num_font;
                XlFont *xlfont;
                } XlListaFont;
XlListaFont xllista_font={0,NULL};

XFontStruct *XlGetFont(disp,fontname)
Display *disp;
char *fontname;
{
int i;
XFontStruct *font_info;
char hostname[255];
char *appo;

/*
 esamina se il font e' gia' presente in lista
*/
strcpy(hostname,DisplayString(disp));
   appo = strrchr(hostname,':');
if( appo != NULL)
  (*appo) = 0;
i=0;
for(i=0;i<xllista_font.num_font;i++)
        {
        if( (strcmp(hostname ,xllista_font.xlfont[i].dispname)==NULL) &&
            (strcmp(xllista_font.xlfont[i].fontname,fontname)==NULL) )
                {
                return(xllista_font.xlfont[i].font_info);
                }
        }
/*
 Se il font non e' presente lo carica in lista
*/
if((font_info = XLoadQueryFont(disp,fontname)) == NULL)
        {
        return(NULL);
        }
xllista_font.num_font++;
xllista_font.xlfont=(XFontStruct *)XtRealloc(xllista_font.xlfont,xllista_font.num_font * sizeof(XlFont));
xllista_font.xlfont[i].fontname=XtNewString(fontname);
xllista_font.xlfont[i].font_info=font_info;
xllista_font.xlfont[i].counter=i;
strcpy(xllista_font.xlfont[i].dispname,""); /* pulizia per DEBUG con 3rd */
strcpy(xllista_font.xlfont[i].dispname,hostname);
return(font_info);
}

