/**********************************************************************
*
*       C Source:               XlAllarmi_func.c
*       Subsystem:              1
*       Description:
*       %created_by:    famgr %
*       %date_created:  Mon Nov  2 17:13:54 1998 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlAllarmi_func.c-5.1.1 %  (%full_filespec: XlAllarmi_func.c-5.1.1:csrc:1 %)";
#endif
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlAllarmi_func.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/

/*******************************************************************************
	top_allarmi.c

	Associated Resource file: top_allarmi.rf
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ToggleB.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Label.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xm/Xm.h>
#include <Ol/OlDatabasePuntiP.h>
#include <Xl/XlAllarmiP.h>
#include <Xl/label_tonda.bmp>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */


#define MODO_ALLARMI 0
#define MODO_ARCHIVI 1
#define DESCR_MODO_ALLARMI "Alarms"
#define DESCR_MODO_ARCHIVIO "Archives"

#define ALL_DA_RIC_STR  "New (unack.) alarms"
#define ALL_IN_ARCH_STR  "Archived alarms"
#define ALL_DA_STR       "from"
#define ALL_A_STR	     "to"
#define RICONOSCI_STR "ACK"
#define DATA_NOT_AVAILABLE_STR  "Data not available"
#define DATA_STR "DD HH-MM"
#define DESCRIZIONE_STR " Tag                  Description              Type     State    Refer."
#define VALORE_STR  "Value"

#define MODO_CONG_OFF	0
#define MODO_CONG_ON	1
#define DESCR_MODO_CONG_OFF "Cong. Off"
#define DESCR_MODO_CONG_ON "Cong. On"

Arg arg[10];

/*******************************************************************************
	The definition of the context structure:
	If you create multiple instances of your interface, the context
	structure ensures that your callbacks use the variables for the
	correct instance.

	For each swidget in the interface, each argument to the Interface
	function, and each variable in the Instance Specific section of the
	Declarations Editor, there is an entry in the context structure.
	and a #define.  The #define makes the variable name refer to the
	corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	S_XLALLARMI *UxPXlAllarmi;
	Widget	Uxtop_allarmi;
	Widget	Uxform_allarmi;
	Widget	Uxform_tasti;
	Widget	Uxform_tasto_riconosci;
	Widget	Uxtasto_riconosci;
	Widget	UxseparatorGadget1;
	Widget	Uxlabel_da;
	Widget	Uxlabel_a;
	Widget	Uxform_altri_tasti;
	Widget	Uxtasto_archivio;
	Widget	Uxtasto_cong;
	Widget	Uxtasto_sing;
	Widget	UxseparatorGadget2;
	Widget	Uxform_label;
	Widget	Uxlabel_titolo;
	Widget	Uxlabel1;
	Widget	Uxlabel2;
	Widget	Uxform_testo;
	Widget	UxscrollBar_allarmi;
	Widget	UxseparatorGadget3;
	Widget	UxscrolledWindow_allarmi;
	Widget	Uxform_testi_allarmi;
	Widget	Uxlabel_titoli;
	Widget	Uxtesto_1;
	Widget	Uxtesto_2;
	Widget	Uxtesto_3;
	Widget	Uxtesto_4;
	Widget	Uxtesto_5;
	Widget	Uxtesto_6;
	Widget	Uxtesto_7;
	Widget	Uxtesto_8;
	Widget	Uxtesto_9;
	Widget	Uxtesto_10;
	Widget	Uxtesto_11;
	Widget	Uxtesto_12;
	Widget	Uxtesto_13;
	Widget	Uxtesto_14;
	Widget	Uxtesto_15;
	Widget	Uxtesto_16;
	Widget	UxseparatorGadget4;
	Widget	Uxvalore_1;
	Widget	Uxvalore_2;
	Widget	Uxvalore_3;
	Widget	Uxvalore_4;
	Widget	Uxvalore_5;
	Widget	Uxvalore_6;
	Widget	Uxvalore_7;
	Widget	Uxvalore_8;
	Widget	Uxvalore_9;
	Widget	Uxvalore_10;
	Widget	Uxvalore_11;
	Widget	Uxvalore_12;
	Widget	Uxvalore_13;
	Widget	Uxvalore_14;
	Widget	Uxvalore_15;
	Widget	Uxvalore_16;
	Widget	Uxtoggle_1;
	Widget	Uxtoggle_2;
	Widget	Uxtoggle_3;
	Widget	Uxtoggle_4;
	Widget	Uxtoggle_5;
	Widget	Uxtoggle_6;
	Widget	Uxtoggle_7;
	Widget	Uxtoggle_8;
	Widget	Uxtoggle_9;
	Widget	Uxtoggle_10;
	Widget	Uxtoggle_11;
	Widget	Uxtoggle_12;
	Widget	Uxtoggle_13;
	Widget	Uxtoggle_14;
	Widget	Uxtoggle_15;
	Widget	Uxtoggle_16;
	Widget	Uxled_1;
	Widget	Uxled_2;
	Widget	Uxled_3;
	Widget	Uxled_4;
	Widget	Uxled_5;
	Widget	Uxled_6;
	Widget	Uxled_7;
	Widget	Uxled_8;
	Widget	Uxled_9;
	Widget	Uxled_10;
	Widget	Uxled_11;
	Widget	Uxled_12;
	Widget	Uxled_13;
	Widget	Uxled_14;
	Widget	Uxled_15;
	Widget	Uxled_16;
	int	Uxmodo;
	int	Uxmodo_cong;
	int	Uxdb_allarmi;
	int	UxcodiceAll;
	int	Uxall_da;
	int	Uxall_a;
	int	Uxall_da_ric;
	int	Uxall_in_arch;
	DATI_ALLARMI	Uxdati_allarmi[16];
	int	Uxzone_impianto[NUMERO_ZONE];
	Pixel	Uxpix_all[8];
	Pixel	Uxbackground_allarmi;
	Pixel	Uxforeground_allarmi;
	Pixmap	Uxpixmap_all[8];
	Pixmap	Uxpixmap_spento;
	XmFontList Uxfont_normale;
	XmFontList Uxfont_titolo;
	Dimension Uxh_font_normale;
	Dimension Uxw_font_normale;
	Dimension Uxh_font_titolo;
	Dimension Uxw_font_titolo;
	Dimension Uxform_width;
	Dimension Uxform_height;
	Dimension Uxlabel_height;
	Dimension Uxw_form_ric;
	Dimension Uxw_form_arch;
	OlDatabasePuntiObject UxdatabasePunti;
} _UxCtop_allarmi;

#define ALLARMI_CONTEXT	        UxTop_allarmiContext->UxPXlAllarmi

static _UxCtop_allarmi	*UxTop_allarmiContext;


/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_top_allarmi();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

setta_scrollBar(S_XLALLARMI *PuntAllarmi)
{
S_XLALLARMI *pXlAllarmi;
int min,max,val,size;
pXlAllarmi=PuntAllarmi;

/*
	refresh della scroollBar
*/
if(modo==MODO_ARCHIVI)
	{
	min=1;
	if(all_in_arch<=16)
		{
		val=all_da;
		max=2;
		}
	else
		{
		max=all_in_arch;
		val=all_da;
		}
	if(all_in_arch>0)
		size=((float)(max-min))/((float)all_in_arch/16.);
	else
		size=1;
	}
else
	{
	min=1;
	if(all_da_ric<=16)
		{
		val=all_da;
		max=2;
		}
	else
		{
		max=all_da_ric;
		val=all_da;
		}
	if(all_da_ric>0)
		size=((float)(max-min))/((float)all_da_ric/16.);
	else
		size=1;
	}

if(size<1) size=1;
if(size>(max-min))  size=max-min;
if(size+val>max) size=max-val;
if(max<min) max=min;
if(val<min) val=min;
if(val>max) val=max;
XtSetArg(arg[0],XmNmaximum,max);
XtSetArg(arg[1],XmNminimum,min);
XtSetArg(arg[2],XmNvalue,val);
XtSetArg(arg[3],XmNsliderSize,size);
XtSetValues(scrollBar_allarmi,arg,4);
}

setta_colori_led(S_XLALLARMI *PuntAllarmi,Boolean On)
{
S_XLALLARMI *pXlAllarmi;

pXlAllarmi=PuntAllarmi;

if(On==True)
   {
   XtSetArg(arg[0],XmNlabelPixmap,pixmap_spento);
   if(dati_allarmi[0].blink_astr)
      XtSetValues(led_1,arg,1);
   if(dati_allarmi[1].blink_astr)
      XtSetValues(led_2,arg,1);
   if(dati_allarmi[2].blink_astr)
      XtSetValues(led_3,arg,1);
   if(dati_allarmi[3].blink_astr)
      XtSetValues(led_4,arg,1);
   if(dati_allarmi[4].blink_astr)
      XtSetValues(led_5,arg,1);
   if(dati_allarmi[5].blink_astr)
      XtSetValues(led_6,arg,1);
   if(dati_allarmi[6].blink_astr)
      XtSetValues(led_7,arg,1);
   if(dati_allarmi[7].blink_astr)
      XtSetValues(led_8,arg,1);
   if(dati_allarmi[8].blink_astr)
      XtSetValues(led_9,arg,1);
   if(dati_allarmi[9].blink_astr)
      XtSetValues(led_10,arg,1);
   if(dati_allarmi[10].blink_astr)
      XtSetValues(led_11,arg,1);
   if(dati_allarmi[11].blink_astr)
      XtSetValues(led_12,arg,1);
   if(dati_allarmi[12].blink_astr)
      XtSetValues(led_13,arg,1);
   if(dati_allarmi[13].blink_astr)
      XtSetValues(led_14,arg,1);
   if(dati_allarmi[14].blink_astr)
      XtSetValues(led_15,arg,1);
   if(dati_allarmi[15].blink_astr)
      XtSetValues(led_16,arg,1);
   }
else
   {
   if(dati_allarmi[0].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[0].colore_astr]);
      XtSetValues(led_1,arg,1);
      }
   if(dati_allarmi[1].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[1].colore_astr]);
      XtSetValues(led_2,arg,1);
      }
   if(dati_allarmi[2].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[2].colore_astr]);
      XtSetValues(led_3,arg,1);
      }
   if(dati_allarmi[3].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[3].colore_astr]);
      XtSetValues(led_4,arg,1);
      }
   if(dati_allarmi[4].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[4].colore_astr]);
      XtSetValues(led_5,arg,1);
      }
   if(dati_allarmi[5].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[5].colore_astr]);
      XtSetValues(led_6,arg,1);
      }
   if(dati_allarmi[6].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[6].colore_astr]);
      XtSetValues(led_7,arg,1);
      }
   if(dati_allarmi[7].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[7].colore_astr]);
      XtSetValues(led_8,arg,1);
      }
   if(dati_allarmi[8].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[8].colore_astr]);
      XtSetValues(led_9,arg,1);
      }
   if(dati_allarmi[9].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[9].colore_astr]);
      XtSetValues(led_10,arg,1);
      }
   if(dati_allarmi[10].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[10].colore_astr]);
      XtSetValues(led_11,arg,1);
      }
   if(dati_allarmi[11].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[11].colore_astr]);
      XtSetValues(led_12,arg,1);
      }
   if(dati_allarmi[12].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[12].colore_astr]);
      XtSetValues(led_13,arg,1);
      }
   if(dati_allarmi[13].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[13].colore_astr]);
      XtSetValues(led_14,arg,1);
      }
   if(dati_allarmi[14].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[14].colore_astr]);
      XtSetValues(led_15,arg,1);
      }
   if(dati_allarmi[15].blink_astr)
      {
      XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[15].colore_astr]);
      XtSetValues(led_16,arg,1);
      }
   }

}


/* void: non ritorna niente, e nessuno usa il valore (era int per default,
   con un "return;" senza valore). Definita prima di tutte le chiamate. */
void setta_tasti(S_XLALLARMI *PuntAllarmi)
{
S_XLALLARMI *pXlAllarmi;

pXlAllarmi=PuntAllarmi;

if(modo==MODO_ALLARMI)
	{	
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(DESCR_MODO_ARCHIVIO));
	XtSetValues(tasto_archivio,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(DESCR_MODO_ALLARMI));
	XtSetValues(tasto_archivio,arg,1);
	XtSetArg(arg[0],XmNsensitive,False);
	XtSetValues(tasto_sing,arg,1);
	XtSetValues(tasto_cong,arg,1);
	XtSetValues(tasto_riconosci,arg,1);
	}
if(modo==MODO_ARCHIVI)
	return;
if(modo_cong==MODO_CONG_ON)
	{
	XtSetArg(arg[0],XmNsensitive,True);
	XtSetValues(tasto_cong,arg,1);
	XtSetArg(arg[0],XmNsensitive,True);
	XtSetValues(tasto_sing,arg,1);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(DESCR_MODO_CONG_OFF));
	XtSetValues(tasto_cong,arg,1);
	XtSetArg(arg[0],XmNsensitive,False);
	XtSetValues(tasto_riconosci,arg,1);
	XtSetArg(arg[0],XmNsensitive,True);
	XtSetValues(tasto_archivio,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNsensitive,True);
	XtSetValues(tasto_cong,arg,1);
	XtSetArg(arg[0],XmNsensitive,False);
	XtSetValues(tasto_sing,arg,1);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(DESCR_MODO_CONG_ON));
	XtSetValues(tasto_cong,arg,1);
	if(all_da==1)
		XtSetArg(arg[0],XmNsensitive,True);
	else
		XtSetArg(arg[0],XmNsensitive,False);
	XtSetValues(tasto_riconosci,arg,1);
	XtSetArg(arg[0],XmNsensitive,True);
	XtSetValues(tasto_archivio,arg,1);
	}
}

raccogli_dati_allarmi(S_XLALLARMI *PuntAllarmi)
{
int i;
S_XLALLARMI *pXlAllarmi;

pXlAllarmi=PuntAllarmi;


/*
	elimina l'eventuale blink
*/
for(i=0;i<16;i++)
	{
	if(dati_allarmi[i].colore_astr>7)
		{
		dati_allarmi[i].colore_astr -=8;
		dati_allarmi[i].blink_astr=1;
		}
	else
		dati_allarmi[i].blink_astr=0;

	if(dati_allarmi[i].colore_testo>7)
		{
		dati_allarmi[i].colore_testo -=8;
		dati_allarmi[i].blink_testo=1;
		}
	else
		dati_allarmi[i].blink_testo=0;
	}
refresh_allarmi(pXlAllarmi);
setta_tasti(pXlAllarmi);
setta_scrollBar(pXlAllarmi);
}

refresh_allarmi(S_XLALLARMI *PuntAllarmi)
{
char app_testo[200];
S_XLALLARMI *pXlAllarmi;
int dim_scroll;

pXlAllarmi=PuntAllarmi;

sprintf(app_testo,"%s: %d",ALL_DA_RIC_STR,all_da_ric);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
XtSetValues(label1,arg,1);
sprintf(app_testo,"%s: %d",ALL_IN_ARCH_STR,all_in_arch);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
XtSetValues(label2,arg,1);
sprintf(app_testo,"%s: %d",ALL_DA_STR,all_da);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
XtSetValues(label_da,arg,1);
sprintf(app_testo,"%s: %d",ALL_A_STR,all_a);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
XtSetValues(label_a,arg,1);

/* riga 1 */
printf("Descriz. riga1=%s\n",dati_allarmi[0].testo);
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_1,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[0].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[0].colore_testo]);
XtSetValues(testo_1,arg,2);
if(dati_allarmi[0].esiste_valore==1)
	{

	sprintf(app_testo,"%f",dati_allarmi[0].valore);
printf("Valore riga1=%s\n",app_testo);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_1,arg,1);
	}
else
	{
printf("Il valore della riga1 non esiste\n");
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_1,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[0].colore_astr]);
XtSetValues(led_1,arg,1);
/* riga 2 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_2,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[1].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[1].colore_testo]);
XtSetValues(testo_2,arg,2);
if(dati_allarmi[1].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[1].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_2,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_2,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[1].colore_astr]);
XtSetValues(led_2,arg,1);
/* riga 3 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_3,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[2].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[2].colore_testo]);
XtSetValues(testo_3,arg,2);
if(dati_allarmi[2].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[2].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_3,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_3,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[2].colore_astr]);
XtSetValues(led_3,arg,1);
/* riga 4 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_4,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[3].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[3].colore_testo]);
XtSetValues(testo_4,arg,2);
if(dati_allarmi[3].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[3].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_4,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_4,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[3].colore_astr]);
XtSetValues(led_4,arg,1);
/* riga 5 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_5,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[4].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[4].colore_testo]);
XtSetValues(testo_5,arg,2);
if(dati_allarmi[4].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[4].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_5,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_5,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[4].colore_astr]);
XtSetValues(led_5,arg,1);
/* riga 6 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_6,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[5].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[5].colore_testo]);
XtSetValues(testo_6,arg,2);
if(dati_allarmi[5].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[5].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_6,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_6,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[5].colore_astr]);
XtSetValues(led_6,arg,1);
/* riga 7 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_7,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[6].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[6].colore_testo]);
XtSetValues(testo_7,arg,2);
if(dati_allarmi[6].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[6].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_7,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_7,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[6].colore_astr]);
XtSetValues(led_7,arg,1);

/* riga 8 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_8,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[7].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[7].colore_testo]);
XtSetValues(testo_8,arg,2);
if(dati_allarmi[7].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[7].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_8,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_8,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[7].colore_astr]);
XtSetValues(led_8,arg,1);

/* riga 9 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_9,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[8].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[8].colore_testo]);
XtSetValues(testo_9,arg,2);
if(dati_allarmi[8].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[8].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_9,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_9,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[8].colore_astr]);
XtSetValues(led_9,arg,1);

/* riga 10 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_10,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[9].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[9].colore_testo]);
XtSetValues(testo_10,arg,2);
if(dati_allarmi[9].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[9].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_10,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_10,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[9].colore_astr]);
XtSetValues(led_10,arg,1);

/* riga 11 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_11,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[10].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[10].colore_testo]);
XtSetValues(testo_11,arg,2);
if(dati_allarmi[10].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[10].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_11,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_11,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[10].colore_astr]);
XtSetValues(led_11,arg,1);

/* riga 12 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_12,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[11].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[11].colore_testo]);
XtSetValues(testo_12,arg,2);
if(dati_allarmi[11].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[11].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_12,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_12,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[11].colore_astr]);
XtSetValues(led_12,arg,1);

/* riga 13 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_13,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[12].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[12].colore_testo]);
XtSetValues(testo_13,arg,2);
if(dati_allarmi[12].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[12].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_13,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_13,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[12].colore_astr]);
XtSetValues(led_13,arg,1);

/* riga 14 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_14,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[13].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[13].colore_testo]);
XtSetValues(testo_14,arg,2);
if(dati_allarmi[13].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[13].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_14,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_14,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[13].colore_astr]);
XtSetValues(led_14,arg,1);

/* riga 15 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_15,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[14].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[14].colore_testo]);
XtSetValues(testo_15,arg,2);
if(dati_allarmi[14].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[14].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_15,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_15,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[14].colore_astr]);
XtSetValues(led_15,arg,1);
/* riga 16 */
XtSetArg(arg[0],XmNforeground,pix_all[7]);
XtSetValues(valore_16,arg,1);
XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(dati_allarmi[15].testo));
XtSetArg(arg[1],XmNforeground,pix_all[dati_allarmi[15].colore_testo]);
XtSetValues(testo_16,arg,2);
if(dati_allarmi[15].esiste_valore==1)
	{
	sprintf(app_testo,"%f",dati_allarmi[15].valore);
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(app_testo));
	XtSetValues(valore_16,arg,1);
	}
else
	{
	XtSetArg(arg[0],XmNlabelString,XmStringCreateSimple(""));
	XtSetValues(valore_16,arg,1);
	}
XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[dati_allarmi[15].colore_astr]);
XtSetValues(led_16,arg,1);
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_tasto_riconosci( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtop_allarmi         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTop_allarmiContext;
	UxTop_allarmiContext = UxContext =
			(_UxCtop_allarmi *) UxGetContext( UxWidget );
	{
	S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;
	Boolean ret;

	/*
		invia il comando di riconoscimento al database dei punti
        N.B. Violazione TOTALE dei pricipi della OO teoria.
	*/
	ret=(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
                (databasePunti,OL_RICONOSCI_ALLARMI,db_allarmi);

	if(!ret)
        	XlWarning("XlAllarmi","activateCB_tasto_riconosci",
			"Riconoscimento allarmi");

	}
	UxTop_allarmiContext = UxSaveCtx;
}

static void	activateCB_tasto_archivio( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtop_allarmi         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTop_allarmiContext;
	UxTop_allarmiContext = UxContext =
			(_UxCtop_allarmi *) UxGetContext( UxWidget );
	{
	S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;

	
	if((modo==MODO_ALLARMI)&&(all_in_arch==0))
		return;

	if(modo==MODO_ARCHIVI)
	   {
	   modo=MODO_ALLARMI;
	   /*
		   invia il comando di passaggio in modo allarmi
        N.B. Violazione TOTALE dei pricipi della OO teoria.
	   */
	   if(!(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
                  (databasePunti,OL_MODO_ALLARMI,db_allarmi))
        	   XlWarning("XlAllarmi","activateCB_tasto_archivio",
			   "Set modo allarmi");
	   }
	else
	   {
	   modo=MODO_ARCHIVI;
	   /*
		   invia il comando di passaggio in modo allarmi
        N.B. Violazione TOTALE dei pricipi della OO teoria.
	   */
	   if(!(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
                  (databasePunti,OL_MODO_ARCHIVIO,db_allarmi))
        	   XlWarning("XlAllarmi","activateCB_tasto_archivio",
			   "Set modo archivi");
	   }
	setta_tasti(ALLARMI_CONTEXT);
	}
	UxTop_allarmiContext = UxSaveCtx;
}

static void	activateCB_tasto_cong( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtop_allarmi         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTop_allarmiContext;
	UxTop_allarmiContext = UxContext =
			(_UxCtop_allarmi *) UxGetContext( UxWidget );
	{
	int i=0;
	S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;
	
	if(modo_cong==MODO_CONG_OFF)
		{
		modo_cong=MODO_CONG_ON;
		}
	else
		{
		modo_cong=MODO_CONG_OFF;
		}
	setta_tasti(ALLARMI_CONTEXT);
	}
	UxTop_allarmiContext = UxSaveCtx;
}

static void	increment_callback( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCtop_allarmi         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTop_allarmiContext;
	UxTop_allarmiContext = UxContext =
			(_UxCtop_allarmi *) UxGetContext( UxWidget );
	{
	S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;
	Boolean ret;


	if((modo==MODO_ALLARMI)&&(all_a==all_da_ric))
		{
		setta_scrollBar(ALLARMI_CONTEXT);
		return;
		}
	/*
		invia il comando di riconoscimento al database dei punti
        N.B. Violazione TOTALE dei pricipi della OO teoria.
	*/
	ret=(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
                (databasePunti,OL_AVANTI_ALLARMI,db_allarmi);

	if(!ret)
        	XlWarning("XlAllarmi","activateCB_tasto_riconosci",
			"Pagina successiva allarmi/archivi");

	}
	UxTop_allarmiContext = UxSaveCtx;
}


static void     decrement_callback( UxWidget, UxClientData, UxCallbackArg )
        Widget          UxWidget;
        XtPointer       UxClientData, UxCallbackArg;
{
        _UxCtop_allarmi         *UxSaveCtx, *UxContext;

        UxSaveCtx = UxTop_allarmiContext;
        UxTop_allarmiContext = UxContext =
                        (_UxCtop_allarmi *) UxGetContext( UxWidget );
        {
        S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;
	Boolean ret;

	if((modo==MODO_ALLARMI)&&(all_da==1))
		{
		setta_scrollBar(ALLARMI_CONTEXT);
		return;
		}
	/*
		invia il comando di riconoscimento al database dei punti
        N.B. Violazione TOTALE dei pricipi della OO teoria.
	*/
	ret=(olDatabasePuntiClassRec.oldatabasePunti_class.commandAllarmi)
                (databasePunti,OL_INDIETRO_ALLARMI,db_allarmi);

	if(!ret)
        	XlWarning("XlAllarmi","activateCB_tasto_riconosci",
			"Pagina precedente allarmi/archivi");

        }
        UxTop_allarmiContext = UxSaveCtx;
}

static void     valueChanged_callback( UxWidget, UxClientData, UxCallbackArg )
        Widget          UxWidget;
        XtPointer       UxClientData, UxCallbackArg;
{
        _UxCtop_allarmi         *UxSaveCtx, *UxContext;

        UxSaveCtx = UxTop_allarmiContext;
        UxTop_allarmiContext = UxContext =
                        (_UxCtop_allarmi *) UxGetContext( UxWidget );
        {
        S_XLALLARMI *pXlAllarmi=ALLARMI_CONTEXT;

	setta_scrollBar(ALLARMI_CONTEXT);
        }
        UxTop_allarmiContext = UxSaveCtx;
}
/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_top_allarmi(Widget padre,S_XLALLARMI *PuntAllarmi)
{
S_XLALLARMI *pXlAllarmi;
Dimension att_toggle,primo_att,width_testo,width_valore,width_form_all;
Dimension p1,p2,p3,h1,h_bar;
char stringa_pagina[300];
char stringa_titoli[300];
int i;
int prima_zona=1;

ALLARMI_CONTEXT = PuntAllarmi;
pXlAllarmi=PuntAllarmi;

att_toggle=label_height-20+5;
primo_att=51+(label_height-20)/2;
width_testo=(LUN_TESTO_ALLARMI+1+LUN_DATA_ALLARMI)*w_font_normale;
width_valore=10*w_font_normale;
width_form_all=width_testo+width_valore;
h1=(HEIGHT_FORM_TASTI-3*4)/3;
p1=3;
p2=3*2+h1;
p3=3*3+h1*2;
h_bar=label_height*16+5*16;
sprintf(stringa_pagina,"[db %d][zone ",db_allarmi);
for(i=0;i<NUMERO_ZONE;i++)
	{
	if(zone_impianto[i]==1)
		{
		if(prima_zona==1)
			{
			prima_zona=0;
			sprintf(stringa_pagina,"%s%d",stringa_pagina,i);
			}
		else
			{
			sprintf(stringa_pagina,"%s, %d",stringa_pagina,i);
			}
		}
	}
sprintf(stringa_pagina,"%s]",stringa_pagina);
/*
Tolti 3 blank
by Fabio
*/
sprintf(stringa_titoli,"%s %s  %s",DATA_STR,DESCRIZIONE_STR,VALORE_STR); 


	form_allarmi = XtVaCreateManagedWidget( "form_allarmi",
			xmFormWidgetClass, padre,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNx,OFFSET_FORM_ESTERNA,
			XmNy,OFFSET_FORM_ESTERNA,
			XmNwidth,form_width,
			XmNheight,form_height,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, padre,
			XmNtopOffset, OFFSET_FORM_ESTERNA,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftWidget, padre,
			XmNleftOffset, OFFSET_FORM_ESTERNA,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNrightWidget, padre,
			XmNrightOffset, OFFSET_FORM_ESTERNA,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, padre,
			XmNbottomOffset, OFFSET_FORM_ESTERNA,
			XmNunitType, XmPIXELS,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_allarmi, (char *) UxTop_allarmiContext );

	form_tasti = XtVaCreateManagedWidget( "form_tasti",
			xmFormWidgetClass, form_allarmi,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNheight, HEIGHT_FORM_TASTI,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_tasti, (char *) UxTop_allarmiContext );

	form_tasto_riconosci = XtVaCreateManagedWidget( "form_tasto_riconosci",
			xmFormWidgetClass, form_tasti,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNwidth, w_form_ric,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_tasto_riconosci, (char *) UxTop_allarmiContext );

	tasto_riconosci = XtVaCreateManagedWidget( "tasto_riconosci",
			xmPushButtonWidgetClass, form_tasto_riconosci,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNtopOffset, p2,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 15,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, RICONOSCI_STR ),
			XmNheight, h1,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( tasto_riconosci, (char *) UxTop_allarmiContext );

	separatorGadget1 = XtVaCreateManagedWidget( "separatorGadget1",
			xmSeparatorWidgetClass, form_tasto_riconosci,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNorientation, XmVERTICAL,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 150,
			XmNwidth, 10,
			XmNy, 0,
			XmNx, 0,
			NULL );

	UxPutContext( separatorGadget1, (char *) UxTop_allarmiContext );

	label_da = XtVaCreateManagedWidget( "label_da",
			xmLabelWidgetClass, form_tasto_riconosci,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			RES_CONVERT( XmNlabelString, ALL_DA_STR ),
			XmNtopOffset, p1,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNheight, h1,
			XmNfontList, font_normale,
			XmNmarginHeight,5,
			NULL );

	UxPutContext( label_da, (char *) UxTop_allarmiContext );

	label_a = XtVaCreateManagedWidget( "label_a",
			xmLabelWidgetClass, form_tasto_riconosci,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, p3,
			XmNtopAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, ALL_A_STR ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, h1,
			XmNfontList, font_normale,
			XmNmarginHeight,10,
			NULL );

	UxPutContext( label_a, (char *) UxTop_allarmiContext );

	form_altri_tasti = XtVaCreateManagedWidget( "form_altri_tasti",
			xmFormWidgetClass, form_tasti,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNrightWidget, form_tasto_riconosci,
			XmNrightOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_NONE,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 130,
			XmNwidth, w_form_arch,
			XmNy, 10,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_altri_tasti, (char *) UxTop_allarmiContext );

	tasto_archivio = XtVaCreateManagedWidget( "tasto_archivio",
			xmPushButtonWidgetClass, form_altri_tasti,
#ifndef RICONOSCIMENTO_SINGOLO
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, p2,
#else
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, h1,
#endif
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, DESCR_MODO_ARCHIVIO ),
			XmNheight, h1,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( tasto_archivio, (char *) UxTop_allarmiContext );

	tasto_cong = XtVaCreateManagedWidget( "tasto_cong",
			xmPushButtonWidgetClass, form_altri_tasti,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			RES_CONVERT( XmNlabelString, "Cong. Off" ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, p2,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNheight, h1,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( tasto_cong, (char *) UxTop_allarmiContext );

	tasto_sing = XtVaCreateManagedWidget( "tasto_sing",
			xmPushButtonWidgetClass, form_altri_tasti,
			XmNsensitive, FALSE,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			RES_CONVERT( XmNlabelString, "Sing." ),
			XmNtopOffset, p3,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNheight, h1,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( tasto_sing, (char *) UxTop_allarmiContext );

	separatorGadget2 = XtVaCreateManagedWidget( "separatorGadget2",
			xmSeparatorWidgetClass, form_altri_tasti,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNorientation, XmVERTICAL,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 150,
			XmNwidth, 10,
			XmNy, 0,
			XmNx, 0,
			NULL );

	UxPutContext( separatorGadget2, (char *) UxTop_allarmiContext );

	form_label = XtVaCreateManagedWidget( "form_label",
			xmFormWidgetClass, form_tasti,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightWidget, form_altri_tasti,
			XmNrightAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNheight, 110,
			XmNwidth, 370,
			XmNy, 20,
			XmNx, 40,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_label, (char *) UxTop_allarmiContext );

	label_titolo = XtVaCreateManagedWidget( "label_titolo",
			xmLabelWidgetClass, form_label,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, stringa_pagina ),
			XmNfontList, font_titolo,
			NULL );

	UxPutContext( label_titolo, (char *) UxTop_allarmiContext );

	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass, form_label,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			RES_CONVERT( XmNlabelString, ALL_IN_ARCH_STR ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( label2, (char *) UxTop_allarmiContext );

	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass, form_label,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, ALL_DA_RIC_STR ),
			XmNleftOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbottomWidget, label2,
			XmNbottomOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( label1, (char *) UxTop_allarmiContext );

	form_testo = XtVaCreateManagedWidget( "form_testo",
			xmFormWidgetClass, form_allarmi,
			XmNtopOffset, HEIGHT_FORM_TASTI,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_testo, (char *) UxTop_allarmiContext );

	scrollBar_allarmi = XtVaCreateManagedWidget( "scrollBar_allarmi",
			xmScrollBarWidgetClass, form_testo,
			XmNshowArrows, TRUE,
			XmNincrement, 8,
			XmNshadowThickness, 2,
			XmNtopOffset, 47,
			XmNminimum,1,
			XmNmaximum,16,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNtroughColor, trough_allarmi,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_NONE,
			XmNwidth, 15,
			XmNheight, h_bar,
			NULL );

	UxPutContext( scrollBar_allarmi, (char *) UxTop_allarmiContext );

	separatorGadget3 = XtVaCreateManagedWidget( "separatorGadget3",
			xmSeparatorWidgetClass, form_testo,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 10,
			XmNy, 0,
			XmNx, 0,
			NULL );

	UxPutContext( separatorGadget3, (char *) UxTop_allarmiContext );

	scrolledWindow_allarmi = XtVaCreateManagedWidget( "scrolledWindow_allarmi",
			xmScrolledWindowWidgetClass, form_testo,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightWidget, scrollBar_allarmi,
			XmNrightAttachment, XmATTACH_WIDGET,
#ifndef RICONOSCIMENTO_SINGOLO
			XmNleftOffset, 25,
#else
			XmNleftOffset, 45,
#endif
			XmNbottomAttachment, XmATTACH_FORM,
			XmNscrollingPolicy, XmAUTOMATIC,
			NULL );

	UxPutContext( scrolledWindow_allarmi, (char *) UxTop_allarmiContext );

	form_testi_allarmi = XtVaCreateManagedWidget( "form_testi_allarmi",
			xmFormWidgetClass, scrolledWindow_allarmi,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNwidth, width_form_all,
/*
			XmNheight, 490,
			XmNy, 2,
			XmNx, 2,
*/
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );

	UxPutContext( form_testi_allarmi, (char *) UxTop_allarmiContext );

	label_titoli = XtVaCreateManagedWidget( "label_titoli",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNheight,label_height,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNleftAttachment, XmATTACH_FORM,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, stringa_titoli),
			XmNleftOffset, 0,
			XmNheight, 30,
			XmNwidth, 500,
			XmNy, 0,
			XmNx, 0,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( label_titoli, (char *) UxTop_allarmiContext );

	testo_1 = XtVaCreateManagedWidget( "testo_1",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNwidth,width_testo,
			XmNheight,label_height,
			XmNtopWidget, label_titoli,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_NONE,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_1, (char *) UxTop_allarmiContext );

	testo_2 = XtVaCreateManagedWidget( "testo_2",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNwidth,width_testo,
			XmNheight,label_height,
			XmNtopWidget, testo_1,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_2, (char *) UxTop_allarmiContext );

	testo_3 = XtVaCreateManagedWidget( "testo_3",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNtopWidget, testo_2,
			XmNwidth,width_testo,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_3, (char *) UxTop_allarmiContext );

	testo_4 = XtVaCreateManagedWidget( "testo_4",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_3,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_4, (char *) UxTop_allarmiContext );

	testo_5 = XtVaCreateManagedWidget( "testo_5",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_5, (char *) UxTop_allarmiContext );

	testo_6 = XtVaCreateManagedWidget( "testo_6",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_5,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_6, (char *) UxTop_allarmiContext );

	testo_7 = XtVaCreateManagedWidget( "testo_7",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_6,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_7, (char *) UxTop_allarmiContext );

	testo_8 = XtVaCreateManagedWidget( "testo_8",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_7,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_8, (char *) UxTop_allarmiContext );

	testo_9 = XtVaCreateManagedWidget( "testo_9",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_8,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_9, (char *) UxTop_allarmiContext );

	testo_10 = XtVaCreateManagedWidget( "testo_10",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNtopWidget, testo_9,
			XmNwidth,width_testo,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_10, (char *) UxTop_allarmiContext );

	testo_11 = XtVaCreateManagedWidget( "testo_11",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_10,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_11, (char *) UxTop_allarmiContext );

	testo_12 = XtVaCreateManagedWidget( "testo_12",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, testo_11,
			XmNtopOffset, 5,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_12, (char *) UxTop_allarmiContext );

	testo_13 = XtVaCreateManagedWidget( "testo_13",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_12,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_13, (char *) UxTop_allarmiContext );

	testo_14 = XtVaCreateManagedWidget( "testo_14",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopWidget, testo_13,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_14, (char *) UxTop_allarmiContext );

	testo_15 = XtVaCreateManagedWidget( "testo_15",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNwidth,width_testo,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopWidget, testo_14,
			XmNtopOffset, 5,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_15, (char *) UxTop_allarmiContext );

	testo_16 = XtVaCreateManagedWidget( "testo_16",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNtopWidget, testo_15,
			XmNwidth,width_testo,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, DATA_NOT_AVAILABLE_STR ),
			XmNfontList, font_normale,
			NULL );

	UxPutContext( testo_16, (char *) UxTop_allarmiContext );

	separatorGadget4 = XtVaCreateManagedWidget( "separatorGadget4",
			xmSeparatorWidgetClass, form_testi_allarmi,
			XmNtopWidget, label_titoli,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 10,
			XmNwidth, 420,
			XmNy, 30,
			XmNx, 0,
			NULL );

	UxPutContext( separatorGadget4, (char *) UxTop_allarmiContext );

	valore_1 = XtVaCreateManagedWidget( "valore_1",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_1,
			XmNwidth,width_valore,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, label_titoli,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_1, (char *) UxTop_allarmiContext );

	valore_2 = XtVaCreateManagedWidget( "valore_2",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNtopWidget, valore_1,
			XmNtopOffset, 5,
			XmNwidth,width_valore,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftWidget, testo_2,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_2, (char *) UxTop_allarmiContext );

	valore_3 = XtVaCreateManagedWidget( "valore_3",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_3,
			XmNtopWidget, valore_2,
			XmNtopOffset, 5,
			XmNwidth,width_valore,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_3, (char *) UxTop_allarmiContext );

	valore_4 = XtVaCreateManagedWidget( "valore_4",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_4,
			XmNtopWidget, valore_3,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNwidth,width_valore,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_4, (char *) UxTop_allarmiContext );

	valore_5 = XtVaCreateManagedWidget( "valore_5",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_5,
			XmNtopWidget, valore_4,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNwidth,width_valore,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_5, (char *) UxTop_allarmiContext );

	valore_6 = XtVaCreateManagedWidget( "valore_6",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_6,
			XmNtopWidget, valore_5,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNwidth,width_valore,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_6, (char *) UxTop_allarmiContext );

	valore_7 = XtVaCreateManagedWidget( "valore_7",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_7,
			XmNtopWidget, valore_6,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNwidth,width_valore,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_7, (char *) UxTop_allarmiContext );

	valore_8 = XtVaCreateManagedWidget( "valore_8",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_8,
			XmNtopWidget, valore_7,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNwidth,width_valore,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_8, (char *) UxTop_allarmiContext );

	valore_9 = XtVaCreateManagedWidget( "valore_9",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_9,
			XmNwidth,width_valore,
			XmNtopWidget, valore_8,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_9, (char *) UxTop_allarmiContext );

	valore_10 = XtVaCreateManagedWidget( "valore_10",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_10,
			XmNtopWidget, valore_9,
			XmNtopOffset, 5,
			XmNwidth,width_valore,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_10, (char *) UxTop_allarmiContext );

	valore_11 = XtVaCreateManagedWidget( "valore_11",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_11,
			XmNrightAttachment, XmATTACH_FORM,
			XmNwidth,width_valore,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, valore_10,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_11, (char *) UxTop_allarmiContext );

	valore_12 = XtVaCreateManagedWidget( "valore_12",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_12,
			XmNtopWidget, valore_11,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNwidth,width_valore,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_12, (char *) UxTop_allarmiContext );

	valore_13 = XtVaCreateManagedWidget( "valore_13",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_13,
			XmNtopWidget, valore_12,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNwidth,width_valore,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_13, (char *) UxTop_allarmiContext );

	valore_14 = XtVaCreateManagedWidget( "valore_14",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_14,
			XmNtopWidget, valore_13,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNwidth,width_valore,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_14, (char *) UxTop_allarmiContext );

	valore_15 = XtVaCreateManagedWidget( "valore_15",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_15,
			XmNtopWidget, valore_14,
			XmNwidth,width_valore,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_15, (char *) UxTop_allarmiContext );

	valore_16 = XtVaCreateManagedWidget( "valore_16",
			xmLabelWidgetClass, form_testi_allarmi,
			XmNrecomputeSize,False,
			XmNheight,label_height,
			XmNleftWidget, testo_16,
			XmNrightAttachment, XmATTACH_FORM,
			XmNwidth,width_valore,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, valore_15,
			XmNtopOffset, 5,
			XmNtopAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "" ),
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNfontList, font_normale,
			NULL );

	UxPutContext( valore_16, (char *) UxTop_allarmiContext );

	toggle_1 = XtVaCreateManagedWidget( "toggle_1",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, primo_att,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_1, (char *) UxTop_allarmiContext );

	toggle_2 = XtVaCreateManagedWidget( "toggle_2",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_1,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_2, (char *) UxTop_allarmiContext );

	toggle_3 = XtVaCreateManagedWidget( "toggle_3",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_2,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_3, (char *) UxTop_allarmiContext );

	toggle_4 = XtVaCreateManagedWidget( "toggle_4",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_3,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_4, (char *) UxTop_allarmiContext );

	toggle_5 = XtVaCreateManagedWidget( "toggle_5",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_4,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_5, (char *) UxTop_allarmiContext );

	toggle_6 = XtVaCreateManagedWidget( "toggle_6",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_5,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_6, (char *) UxTop_allarmiContext );

	toggle_7 = XtVaCreateManagedWidget( "toggle_7",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_6,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_7, (char *) UxTop_allarmiContext );

	toggle_8 = XtVaCreateManagedWidget( "toggle_8",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 0,
			XmNtopWidget, toggle_7,
			XmNtopOffset, att_toggle,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, -10,
			NULL );

	UxPutContext( toggle_8, (char *) UxTop_allarmiContext );

	toggle_9 = XtVaCreateManagedWidget( "toggle_9",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_8,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_9, (char *) UxTop_allarmiContext );

	toggle_10 = XtVaCreateManagedWidget( "toggle_10",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_9,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_10, (char *) UxTop_allarmiContext );

	toggle_11 = XtVaCreateManagedWidget( "toggle_11",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_10,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_11, (char *) UxTop_allarmiContext );

	toggle_12 = XtVaCreateManagedWidget( "toggle_12",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_11,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_12, (char *) UxTop_allarmiContext );

	toggle_13 = XtVaCreateManagedWidget( "toggle_13",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_12,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_13, (char *) UxTop_allarmiContext );

	toggle_14 = XtVaCreateManagedWidget( "toggle_14",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_13,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_14, (char *) UxTop_allarmiContext );

	toggle_15 = XtVaCreateManagedWidget( "toggle_15",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopWidget, toggle_14,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_15, (char *) UxTop_allarmiContext );

	toggle_16 = XtVaCreateManagedWidget( "toggle_16",
			xmToggleButtonWidgetClass, form_testo,
			XmNtopOffset, att_toggle,
			XmNtopWidget, toggle_15,
			XmNtopPosition, 0,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			XmNx, 0,
			NULL );

	UxPutContext( toggle_16, (char *) UxTop_allarmiContext );

	led_1 = XtVaCreateManagedWidget( "led_1",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, primo_att,
			XmNleftWidget, toggle_1,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_1, (char *) UxTop_allarmiContext );

	led_2 = XtVaCreateManagedWidget( "led_2",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNleftWidget, toggle_2,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, led_1,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_2, (char *) UxTop_allarmiContext );

	led_3 = XtVaCreateManagedWidget( "led_3",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_2,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_3,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_3, (char *) UxTop_allarmiContext );

	led_4 = XtVaCreateManagedWidget( "led_4",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_3,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_4,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_4, (char *) UxTop_allarmiContext );

	led_5 = XtVaCreateManagedWidget( "led_5",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_4,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_5,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_5, (char *) UxTop_allarmiContext );

	led_6 = XtVaCreateManagedWidget( "led_6",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_5,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_6,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_6, (char *) UxTop_allarmiContext );

	led_7 = XtVaCreateManagedWidget( "led_7",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_6,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_7,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_7, (char *) UxTop_allarmiContext );

	led_8 = XtVaCreateManagedWidget( "led_8",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_7,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_8,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_8, (char *) UxTop_allarmiContext );

	led_9 = XtVaCreateManagedWidget( "led_9",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_8,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_9,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_9, (char *) UxTop_allarmiContext );

	led_10 = XtVaCreateManagedWidget( "led_10",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_9,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_10,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_10, (char *) UxTop_allarmiContext );

	led_11 = XtVaCreateManagedWidget( "led_11",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_10,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_11,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_11, (char *) UxTop_allarmiContext );

	led_12 = XtVaCreateManagedWidget( "led_12",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_11,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_12,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_12, (char *) UxTop_allarmiContext );

	led_13 = XtVaCreateManagedWidget( "led_13",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_12,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_13,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_13, (char *) UxTop_allarmiContext );

	led_14 = XtVaCreateManagedWidget( "led_14",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_13,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_14,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_14, (char *) UxTop_allarmiContext );

	led_15 = XtVaCreateManagedWidget( "led_15",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNleftWidget, toggle_15,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNtopWidget, led_14,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_15, (char *) UxTop_allarmiContext );

	led_16 = XtVaCreateManagedWidget( "led_16",
			xmLabelWidgetClass, form_testo,
			XmNrecomputeSize,False,
			XmNtopWidget, led_15,
			XmNtopOffset, att_toggle,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNleftWidget, toggle_16,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNmarginWidth, 0,
			XmNmarginHeight, 0,
			XmNmappedWhenManaged, TRUE,
			XmNlabelType, XmPIXMAP,
			XmNbackground, background_allarmi,
			XmNforeground, foreground_allarmi,
			XmNheight, 20,
			XmNwidth, 20,
			NULL );

	UxPutContext( led_16, (char *) UxTop_allarmiContext );

	XtAddCallback( tasto_riconosci, XmNactivateCallback,
			activateCB_tasto_riconosci,
			(XtPointer) UxTop_allarmiContext );

	XtAddCallback( tasto_archivio, XmNactivateCallback,
			activateCB_tasto_archivio,
			(XtPointer) UxTop_allarmiContext );

	XtAddCallback( tasto_cong, XmNactivateCallback,
			activateCB_tasto_cong,
			(XtPointer) UxTop_allarmiContext );

	XtAddCallback( scrollBar_allarmi, XmNincrementCallback,
			increment_callback,
			(XtPointer) UxTop_allarmiContext );

	XtAddCallback( scrollBar_allarmi, XmNdecrementCallback,
			decrement_callback,
			(XtPointer) UxTop_allarmiContext );

	XtAddCallback( scrollBar_allarmi, XmNvalueChangedCallback,
			valueChanged_callback,
			(XtPointer) UxTop_allarmiContext );

	return (form_allarmi);
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_top_allarmi(Widget padre, S_XLALLARMI *PuntAllarmi)
{
	Widget                  rtrn;
	_UxCtop_allarmi         *UxContext;

	UxTop_allarmiContext = UxContext =
		(_UxCtop_allarmi *) XtMalloc( sizeof(_UxCtop_allarmi) );

	{
	unsigned short dummy;
		int i;
		Widget wid;
		S_XLALLARMI *pXlAllarmi;

		pXlAllarmi=PuntAllarmi;
		
		modo=MODO_ALLARMI;
		modo_cong=MODO_CONG_OFF;
		all_da_ric=0;
		all_in_arch=0;
		all_da=0;
		all_a=0;
		rtrn = _Uxbuild_top_allarmi(padre,PuntAllarmi);
		/*
			alloca i pixel 
		*/
		for(i=0;i<8;i++)
			XlCvtStrToPixel(XtDisplay(form_label),
				tabella_colori_allarmi[i], &pix_all[i],
				&dummy,&dummy,&dummy);
		
		/*
			Carica i bitmap
		*/
		for(i=0;i<8;i++)
		   LoadBitmapLed(rtrn,&pixmap_all[i],
			label_tonda_bits,label_tonda_width,
		        label_tonda_height,
			pix_all[i],background_allarmi);
                LoadBitmapLed(rtrn,&pixmap_spento,
			label_tonda_bits,label_tonda_width,
                        label_tonda_height,
			background_allarmi,background_allarmi);

		XtSetArg(arg[0],XmNlabelPixmap,pixmap_all[0]);
		XtSetArg(arg[1],XmNbackground,background_allarmi);
		XtSetValues(led_1,arg,2);
		XtSetValues(led_2,arg,2);
		XtSetValues(led_3,arg,2);
		XtSetValues(led_4,arg,2);
		XtSetValues(led_5,arg,2);
		XtSetValues(led_6,arg,2);
		XtSetValues(led_7,arg,2);
		XtSetValues(led_8,arg,2);
		XtSetValues(led_9,arg,2);
		XtSetValues(led_10,arg,2);
		XtSetValues(led_11,arg,2);
		XtSetValues(led_12,arg,2);
		XtSetValues(led_13,arg,2);
		XtSetValues(led_14,arg,2);
		XtSetValues(led_15,arg,2);
		XtSetValues(led_16,arg,2);

#ifndef RICONOSCIMENTO_SINGOLO
		XtUnmanageChild(toggle_1);
		XtUnmanageChild(toggle_2);
		XtUnmanageChild(toggle_3);
		XtUnmanageChild(toggle_4);
		XtUnmanageChild(toggle_5);
		XtUnmanageChild(toggle_6);
		XtUnmanageChild(toggle_7);
		XtUnmanageChild(toggle_8);
		XtUnmanageChild(toggle_9);
		XtUnmanageChild(toggle_10);
		XtUnmanageChild(toggle_11);
		XtUnmanageChild(toggle_12);
		XtUnmanageChild(toggle_13);
		XtUnmanageChild(toggle_14);
		XtUnmanageChild(toggle_15);
		XtUnmanageChild(toggle_16);

		XtUnmanageChild(tasto_cong);
		XtUnmanageChild(tasto_sing);
#endif

		/*
			setta i colori della scrollBar
		*/
		XtSetArg(arg[0],XmNhorizontalScrollBar,&wid);
		XtGetValues(scrolledWindow_allarmi,arg,1);
		if(wid!=NULL)
			{
			XtSetArg(arg[0],XmNbackground, background_allarmi);
			XtSetArg(arg[1],XmNforeground, foreground_allarmi);
			XtSetArg(arg[2],XmNtroughColor, trough_allarmi);
			XtSetValues(wid,arg,3);
			}

		return(rtrn);
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_top_allarmi(Widget padre,S_XLALLARMI *PuntAllarmi)
{
	Widget			_Uxrtrn;
	static int		_Uxinit = 0;

	if ( ! _Uxinit )
	{
		UxLoadResources( "top_allarmi.rf" );
		_Uxinit = 1;
	}

	_Uxrtrn = _Ux_create_top_allarmi(padre,PuntAllarmi);

	return ( _Uxrtrn );
}


LoadBitmapLed(Widget w, Pixmap *stip, char * bits, int width, int height,
		Pixel foreground,Pixel background)
{
long cnero,cbianco;


if((*stip=XCreatePixmapFromBitmapData(XtDisplay(w),
            RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),bits,
            width,height,
            foreground, background,
            DefaultDepth(XtDisplay(w),DefaultScreen(XtDisplay(w)))))== False)
        {
        fprintf(stderr,"\n Errore creazione pixmap");
        exit(1);
        }
}
/*
Metodo che permette di ricavare il db allarmi associato alla pagina
Parameters:
         Widget w: oggetto XlAllarmi
Returns:
       No db allarmi richiesto
       -1 se db inammissibile
*/
int XlAllarmiGetDb(Widget w)
{
XlAllarmiWidget cw= (XlAllarmiWidget)w;

if( (cw->allarmi.database_allarmi > 0 ) && (cw->allarmi.database_allarmi <= NUMERO_DATABASE_ALLARMI) )
    return cw->allarmi.database_allarmi;
else
   return -1;
}
/*******************************************************************************
	END OF FILE
*******************************************************************************/

