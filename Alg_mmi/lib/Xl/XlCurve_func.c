/**********************************************************************
*
*	C Source:		XlCurve_func.c
*	Instance:		1
*	Description:	
*	%created_by:	lomgr %
*	%date_created:	Fri May 14 11:51:41 2004 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlCurve_func.c-10 %  (%full_filespec: XlCurve_func.c-10:csrc:1 %)";
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

#include "UxXt.h"

#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include <Xm/PushB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <X11/Shell.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/XmP.h>

#include <Xl/XlCurveP.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

/* Prototyping funzioni esterne */
extern Widget create_handleVar();
extern Widget create_setDate();

extern void prepare_draw();
extern void AggTimeAxis();
extern void DrawGridCurve();
extern Boolean DrawCurves();
extern void AggLegendValues();
extern void ResetLegend();
extern void RefreshXlCurve();


/* Prototyping funzioni */
Widget create_xlcurve();
Widget create_legend();
void   SettingVarInfo();
void   SettingColorTrend();
Boolean RecoveryVarInfo();
Boolean RecoveryRangeZoom();
Boolean LoadLegoCodStr();
void ActFisScale();
void CleanFisScale();
void PushedEffect();
void AggLabVal();
int CompareRange();
int calc_start_pos();
int calc_end_pos();

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/
static void  exposeCB_curvefield(wid,cw,call_data)
Widget wid;
XlCurveWidget cw;
XmAnyCallbackStruct *call_data;
{
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
          (_UxCtopLevelCurve *) UxGetContext(wid);
/*
printf("exposeCB_curvefield: ENTRATO!!!!\n");
*/
   XClearArea(XtDisplay(cw),XtWindow(cw),0,0,0,0,True);

   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("exposeCB_curvefield: USCITO!!!!\n");
*/
}



static void  activateCB_pushButton(wid,toggle_act,call_data)
Widget wid;
int *toggle_act;
XmAnyCallbackStruct *call_data;
{

_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
/*
printf("activateCB_pushButton: ENTRATO!!!\n");
printf("activateCB_pushButton: toggle_act=%d\tnum_scale=%d\n",*toggle_act,num_scale_act);
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext( top_curve );
   {
   create_handleVar(wid,top_curve,toggle_act,&num_scale_act);
   }
   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("activateCB_pushButton: USCITO!!!\n");
*/
}


static void  activateCB_timeleft(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
          (_UxCtopLevelCurve *) UxGetContext(wid);
/*
printf("activateCB_timeleft: ENTRATO!!!\n");
*/
   if(obj->curve.s_XLcurve.cur_line_pos_f22!=-1) {
      if(static_status==False) {
         static_status=True;
         PushedEffect(flow);
      }
/*
	Determinazione della nuova posizione finale.
*/
      obj->curve.s_XLcurve.pos_end_visual=obj->curve.s_XLcurve.cur_line_pos_f22;
/*
	Determinazione della nuova posizione iniziale.
*/
      obj->curve.s_XLcurve.pos_start_visual=calc_start_pos(obj->curve.s_XLcurve.buffer_f22,
		obj->curve.s_XLcurve.rangeTimeStart,obj->curve.s_XLcurve.pos_end_visual);
/*
   Chiamata delle funzioni per il tracciamento delle curve.
*/             
      RefreshXlCurve(obj);
   }
   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("activateCB_timeleft: USCITO!!!\n");
*/
}



static void  activateCB_timeright(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int   delta_x_pix,range_x_pix;
int rangeTimeNew,rangeC; 
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
          (_UxCtopLevelCurve *) UxGetContext(wid);
/*
printf("activateCB_timeright: ENTRATO!!!\n");
printf("activateCB_timeright: x_pix_start=%d\tcurs_pos_pixel=%d\n",obj->curve.s_XLcurve.x_pix_start,curs_pos_pixel);
*/

   if(obj->curve.s_XLcurve.cur_line_pos_f22!=-1) {
/*
   La cursor line interseca le curve. Il primo campione e' sulla cursor line.
*/
      obj->curve.s_XLcurve.pos_start_visual=obj->curve.s_XLcurve.cur_line_pos_f22;
/*
   Determinazione della posizione dell'ultimo campione da visualizzare.
*/
      obj->curve.s_XLcurve.pos_end_visual=calc_end_pos(obj->curve.s_XLcurve.buffer_f22,
	obj->curve.s_XLcurve.rangeTimeStart,obj->curve.s_XLcurve.pos_start_visual,
	obj->curve.s_XLcurve.n_last_val);
/*
printf("activateCB_timeright: pos_start_visual=%d\tpos_end_visual=%d\n",
        obj->curve.s_XLcurve.pos_start_visual,obj->curve.s_XLcurve.pos_end_visual);
*/
/*
   Si controlla se l'intervallo di tempo delimitato dal primo e
   dall'ultimo campione e' minore dell'intervallo di visualizzazione
   scelto; in tal caso si ricalcola la posizione iniziale e si passa
   in condizioni dinamiche.
*/
      rangeC=(int)(obj->curve.s_XLcurve.buffer_f22[obj->curve.s_XLcurve.pos_end_visual].tempo.tempo-
        obj->curve.s_XLcurve.buffer_f22[obj->curve.s_XLcurve.pos_start_visual].tempo.tempo);
      if(rangeC<obj->curve.s_XLcurve.rangeTimeStart) {
         if(static_status==True) {
            static_status=False;
            PushedEffect(flow);
         }
         obj->curve.s_XLcurve.pos_start_visual=calc_start_pos(obj->curve.s_XLcurve.buffer_f22,
		obj->curve.s_XLcurve.rangeTimeStart,obj->curve.s_XLcurve.pos_end_visual);
      }
      if((obj->curve.s_XLcurve.pos_end_visual-obj->curve.s_XLcurve.pos_start_visual)==
         obj->curve.s_XLcurve.rangeTimeStart) {
         if(static_status==True) {
            static_status=False;
            PushedEffect(flow);
         }
      }
   }
   else {
/*
	La cursor line non interseca le curve.
*/


      delta_x_pix=obj->curve.s_XLcurve.x_pix_start-curs_pos_pixel;
      range_x_pix=width_curvefield-delta_x_pix;
/*
printf("activateCB_timeright: range_x_pix=%d\trangeTimeStart=%f\n",
        range_x_pix,obj->curve.s_XLcurve.rangeTimeStart);
*/
      rangeTimeNew=(int)((float)obj->curve.s_XLcurve.rangeTimeStart*((float)range_x_pix/
                   (float)width_curvefield));
/*
printf("activateCB_timeright: rangeTimeNew=%d\n",rangeTimeNew);
*/
/*
   Determinazione della posizione del primo campione.
*/
      obj->curve.s_XLcurve.pos_start_visual=0;
/*
   Determinazione della posizione dell'ultimo campione.
*/
      obj->curve.s_XLcurve.pos_end_visual=calc_end_pos(obj->curve.s_XLcurve.buffer_f22,
	rangeTimeNew,obj->curve.s_XLcurve.pos_start_visual,obj->curve.s_XLcurve.n_last_val);
/*
printf("activateCB_timeright: pos_end_visual=%d\n",obj->curve.s_XLcurve.pos_end_visual);
*/
      if(obj->curve.s_XLcurve.pos_end_visual==obj->curve.s_XLcurve.n_last_val) {
         if(static_status==True)
            static_status=False;
      }
   }
/*
   Chiamata delle funzioni per il tracciamento delle curve.
*/ 
   RefreshXlCurve(obj);

   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("activateCB_timeright: USCITO!!!\n");
*/
}


static void  activateCB_decreasetime(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int   i;
int rangeTimeLeft,rangeTimeRight;
float curs_pos_perc;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
          (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_decreasetime: ENTRATO!!!\n");

   if(obj->curve.s_XLcurve.cur_line_pos_f22!=-1) {
/*
   La cursor line interseca le curve. Ha senso fare uno zoom.
*/
      i=0;
      while(obj->curve.s_XLcurve.range_zoom[i]!=obj->curve.s_XLcurve.rangeTimeStart)
         i++;
      if(i!=0) {
/*
   Esiste almeno un range piu' piccolo di quello corrente. Possibilita'
   di zoom. Viene aggiornato il flag statico/dinamico e lo stato del 
   bottone associato.
*/
         if(static_status==False) {
            static_status=True;
            PushedEffect(flow);
         }
         obj->curve.s_XLcurve.rangeTimeStart=obj->curve.s_XLcurve.range_zoom[i-1];
/*
   Determinazione dei nuovi range di tempo a sinistra e a destra della
   cursor line.
*/
/*
printf("activateCB_decreasetime:rangeTimeStart=%d\tcurs_pos_pixel=%d\tcur_line_pos_f22=%d\n",
       obj->curve.s_XLcurve.rangeTimeStart,curs_pos_pixel,obj->curve.s_XLcurve.cur_line_pos_f22);
*/
         curs_pos_perc=curs_pos_pixel/(float)width_curvefield;
/*
printf("activateCB_decreasetime: curs_pos_perc=%f\n",curs_pos_perc);
*/
         rangeTimeLeft=(int)((float)obj->curve.s_XLcurve.rangeTimeStart*curs_pos_perc);
         rangeTimeRight=obj->curve.s_XLcurve.rangeTimeStart-rangeTimeLeft;
/*
   Determinazione della posizione del primo e dell'ultimo campione.
*/
         obj->curve.s_XLcurve.pos_start_visual=calc_start_pos(obj->curve.s_XLcurve.buffer_f22,
	   rangeTimeLeft,obj->curve.s_XLcurve.cur_line_pos_f22);
         obj->curve.s_XLcurve.pos_end_visual=calc_end_pos(obj->curve.s_XLcurve.buffer_f22,
	   rangeTimeRight,obj->curve.s_XLcurve.cur_line_pos_f22,obj->curve.s_XLcurve.n_last_val);
/*
   Tracciamento delle curve.
*/
         RefreshXlCurve(obj);
      }
   }
   UxTopLevelCurveContext = UxSaveCtx;

printf("activateCB_decreasetime: USCITO!!!\n");
}



static void  activateCB_increasetime(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int   i;
int   rangeTimeRight; 
float t_sim,curs_pos_perc;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
          (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_increasetime: ENTRATO!!!\n");

   if(obj->curve.s_XLcurve.cur_line_pos_f22!=-1) {
      i=0;
      while(obj->curve.s_XLcurve.range_zoom[i]!=obj->curve.s_XLcurve.rangeTimeStart)
         i++;
      if(i!=(obj->curve.s_XLcurve.num_range_zoom-1)) {
/*
        Esiste almeno un range piu' grande di quello corrente. Possibilita'
        di zoom. Controllo pero' prima se ha senso allargare la scala.
*/
         t_sim=(int)(obj->curve.s_XLcurve.t_end-obj->curve.s_XLcurve.t_start);
/*
printf("activateCB_increasetime:t_sim=%d\n",t_sim);
i*/
         if((t_sim>obj->curve.s_XLcurve.rangeTimeStart) ||
            (obj->curve.s_XLcurve.rangeTimeStart!=obj->curve.s_XLcurve.rangeTimeDef)){
            obj->curve.s_XLcurve.rangeTimeStart=obj->curve.s_XLcurve.range_zoom[i+1];
            curs_pos_perc=((float)curs_pos_pixel/(float)width_curvefield);
/*
printf("activateCB_increasetime: rangeTimeStart=%d\tcurs_pos_perc=%d\n",
        obj->curve.s_XLcurve.rangeTimeStart,curs_pos_perc);
*/
            rangeTimeRight=(int)((float)obj->curve.s_XLcurve.rangeTimeStart*(1-curs_pos_perc));
            obj->curve.s_XLcurve.pos_end_visual=calc_end_pos(obj->curve.s_XLcurve.buffer_f22,
		 rangeTimeRight,obj->curve.s_XLcurve.cur_line_pos_f22,
                 obj->curve.s_XLcurve.n_last_val);
                 if(obj->curve.s_XLcurve.pos_end_visual==obj->curve.s_XLcurve.n_last_val) {
                    if(static_status==True) {
                       static_status=False;
                       PushedEffect(flow);
                    }
                 }
            obj->curve.s_XLcurve.pos_start_visual=calc_start_pos(obj->curve.s_XLcurve.buffer_f22,
		 obj->curve.s_XLcurve.rangeTimeStart,obj->curve.s_XLcurve.pos_end_visual);
         RefreshXlCurve(obj);
         }
      } 
   }     
   UxTopLevelCurveContext = UxSaveCtx;
printf("activateCB_increasetime: USCITO!!!\n");

}


static void  activateCB_seldata(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
        (_UxCtopLevelCurve *) UxGetContext(wid);
        {
        create_setDate(wid,&obj->curve.s_XLcurve.seldata_OK,
                         &obj->curve.s_XLcurve.hour_sel,
                         &obj->curve.s_XLcurve.minute_sel);
        }
        UxTopLevelCurveContext = UxSaveCtx;
}



static void  activateCB_abstime(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{
   _UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
        (_UxCtopLevelCurve *) UxGetContext(wid);
        {
	PushedEffect(wid);
        if(abs_time_flag==True)
           abs_time_flag=False;
        else
           abs_time_flag=True;
        AggTimeAxis(obj);
        }
        UxTopLevelCurveContext = UxSaveCtx;
}



static void  activateCB_flow(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext=(_UxCtopLevelCurve *) UxGetContext(wid);
        {
        PushedEffect(wid);
	if(static_status==False) {
/*
   Passaggio dinamico->statico. Si arresta semplicemente l'aggiornamento.
*/
           static_status=True;
        }
        else { 
/*
   Passaggio statico->dinamico. Si riattiva l'aggiornamento, calcolando
   prima le nuove posizioni iniziale e finale dei campioni da visualizzare.
*/ 
           static_status=False;
           obj->curve.s_XLcurve.pos_end_visual=
                obj->curve.s_XLcurve.n_last_val;
           obj->curve.s_XLcurve.pos_start_visual=
              calc_start_pos(obj->curve.s_XLcurve.buffer_f22,
                             obj->curve.s_XLcurve.rangeTimeStart,
                             obj->curve.s_XLcurve.pos_end_visual);
           RefreshXlCurve(obj);
        }
        }
        UxTopLevelCurveContext = UxSaveCtx;
}



static void  activateCB_decreasevalue(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int i;
float ref_perc,ref_fis,range;
float lim_min_perc,lim_max_perc,lim_min_curr_perc;
float range_perc,delta_perc;
XmString compound;
String str;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext =
        (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_decreasevalueaxis: ENTRATO!!!\n");

   ref_perc=ref_line_pos/100.0;
   for(i=0;i<NUMMAXVAR;i++) {
      if(obj->curve.s_XLcurve.ind_var_sel[i]!=-1) {
         if((strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) ||
            (strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)) {
            ref_fis=obj->curve.s_XLcurve.min_max_perc.min +
                      (obj->curve.s_XLcurve.min_max_perc.max-obj->curve.s_XLcurve.min_max_perc.min)*ref_perc;
            range=(obj->curve.s_XLcurve.min_max_perc.max-obj->curve.s_XLcurve.min_max_perc.min)/(obj->curve.s_XLcurve.factScaleValue);
            obj->curve.s_XLcurve.min_max_perc.max=ref_fis+(range/2.0);
            obj->curve.s_XLcurve.min_max_perc.min=ref_fis-(range/2.0);
         }
         else {
            ref_fis = obj->curve.s_XLcurve.min_max_oper[i].min +
                      (obj->curve.s_XLcurve.min_max_oper[i].max-obj->curve.s_XLcurve.min_max_oper[i].min)*ref_perc;
            range=(obj->curve.s_XLcurve.min_max_oper[i].max-obj->curve.s_XLcurve.min_max_oper[i].min)/(obj->curve.s_XLcurve.factScaleValue);
            obj->curve.s_XLcurve.min_max_oper[i].max=ref_fis+(range/2.0);
            obj->curve.s_XLcurve.min_max_oper[i].min=ref_fis-(range/2.0);
         }
      }
   }
         
/*
	Aggiornamento delle labels sull'asse dei valori.
*/
   XtVaGetValues(labval1,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_max_perc=atof(str);
   XtVaGetValues(labval6,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_min_perc=atof(str);
   range_perc=(lim_max_perc-lim_min_perc)/(obj->curve.s_XLcurve.factScaleValue);
   delta_perc=range_perc/NUMDIVHEIGHT;
   lim_min_curr_perc=ref_line_pos-(range_perc/2.0);

   AggLabVal(lim_min_curr_perc,delta_perc);

/*
   Tracciamento delle curve.
*/
   XClearArea(XtDisplay(curvefield),XtWindow(curvefield),0,0,0,0,True);
   DrawGridCurve(obj);
   DrawCurves(obj);

   UxTopLevelCurveContext = UxSaveCtx;

printf("activateCB_decreasevalueaxis: USCITO!!!\n");

}



static void  activateCB_rangeup(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int i;
float incr_range,ref_perc;
float delta_perc;
float lim_min_perc,lim_min_curr_perc,lim_max_perc;
XmString compound;
String   str;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
     (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_rangeup: ENTRATO!!!\n");

/*
	Aggiornamento dei limiti delle variabili in base alla traslazione
	richiesta.
*/
/*
printf("activateCB_rangeup: ref_line_pos=%f\n",ref_line_pos);
*/
   ref_perc=ref_line_pos/100.0; 
   for(i=0;i<NUMMAXVAR;i++) {
      if(obj->curve.s_XLcurve.ind_var_sel[i]!=-1) {
         if((strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) ||
            (strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)) {
            incr_range=((obj->curve.s_XLcurve.min_max_perc.max-
                        obj->curve.s_XLcurve.min_max_perc.min)*ref_perc);
            obj->curve.s_XLcurve.min_max_perc.max+=incr_range;
            obj->curve.s_XLcurve.min_max_perc.min+=incr_range;
         }
         else {
            incr_range=((obj->curve.s_XLcurve.min_max_oper[i].max-
                        obj->curve.s_XLcurve.min_max_oper[i].min)*ref_perc);
            obj->curve.s_XLcurve.min_max_oper[i].max+=incr_range;
            obj->curve.s_XLcurve.min_max_oper[i].min+=incr_range;
         }
      }
   }

/*
	Aggiornamento delle labels sull'asse dei valori.
*/
   XtVaGetValues(labval1,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_max_perc=atof(str);
   XtVaGetValues(labval6,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_min_perc=atof(str);
   delta_perc=(lim_max_perc-lim_min_perc)/NUMDIVHEIGHT;
   lim_min_curr_perc=lim_min_perc+ref_line_pos;
/*   
printf("activateCB_rangeup: lim_min_curr_perc=%f\tlim_max_perc=%f\tdelta_perc=%f\n",
	lim_min_curr_perc,lim_max_perc,delta_perc);
*/
   AggLabVal(lim_min_curr_perc,delta_perc);
/*
   Tracciamento delle curve.
*/
   XClearArea(XtDisplay(curvefield),XtWindow(curvefield),0,0,0,0,True);
   DrawGridCurve(obj);
   DrawCurves(obj);

printf("activateCB_rangeup: USCITO!!!\n");

   UxTopLevelCurveContext = UxSaveCtx;
}


static void  activateCB_defaultset(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int i;
XmString compound;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
     (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_defaultset: ENTRATO!!!\n");

/*
	Ripristino dei limiti delle variabili configurate ai valori iniziali.
*/
   
   for(i=0;i<NUMMAXVAR;i++) {
      if(obj->curve.s_XLcurve.ind_var_sel[i]!=-1) {
         if((strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) ||
            (strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)) {
            obj->curve.s_XLcurve.min_max_perc.max=DEF_HL_DISPL;
            obj->curve.s_XLcurve.min_max_perc.min=DEF_LL_DISPL;
         }
         else {
            obj->curve.s_XLcurve.min_max_oper[i].max=obj->curve.s_XLcurve.min_max[i].max;
            obj->curve.s_XLcurve.min_max_oper[i].min=obj->curve.s_XLcurve.min_max[i].min;
         }
      }
   }

/*
	Ripristino delle labels sull'asse dei valori.
*/
   AggLabVal(DEF_LL_DISPL,DEF_DIV_DISPL);
/*
   Tracciamento delle curve.
*/
   XClearArea(XtDisplay(curvefield),XtWindow(curvefield),0,0,0,0,True);
   DrawGridCurve(obj);
   DrawCurves(obj);

   UxTopLevelCurveContext = UxSaveCtx;

printf("activateCB_defaultset: USCITO!!!\n");


}



static void  activateCB_rangedown(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int i;
float decr_range,ref_perc;
float delta_perc;
float lim_min_perc,lim_min_curr_perc,lim_max_perc;
XmString compound;
String str;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
     (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_rangedown: ENTRATO!!!\n");

/*
	Aggiornamento dei limiti delle variabili in base alla traslazione
	richiesta.
*/
/*
printf("activateCB_rangedown: ref_line_pos=%f\n",ref_line_pos);
*/
   ref_perc=ref_line_pos/100.0;
   for(i=0;i<NUMMAXVAR;i++) {
      if(obj->curve.s_XLcurve.ind_var_sel[i]!=-1) {
         if((strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) ||
            (strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)) {
            decr_range=((obj->curve.s_XLcurve.min_max_perc.max-
               obj->curve.s_XLcurve.min_max_perc.min)*(1.0-ref_perc));
            obj->curve.s_XLcurve.min_max_perc.max-=decr_range;
            obj->curve.s_XLcurve.min_max_perc.min-=decr_range;
         }
         else {
            decr_range=((obj->curve.s_XLcurve.min_max_oper[i].max -
               obj->curve.s_XLcurve.min_max_oper[i].min)*(1.0-ref_perc));
            obj->curve.s_XLcurve.min_max_oper[i].max-=decr_range;
            obj->curve.s_XLcurve.min_max_oper[i].min-=decr_range;
         }
      }
   }
/*
	Aggiornamento delle labels sull'asse dei valori.
*/

   XtVaGetValues(labval1,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_max_perc=atof(str);
   XtVaGetValues(labval6,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_min_perc=atof(str);
   delta_perc=(lim_max_perc-lim_min_perc)/NUMDIVHEIGHT;
   lim_min_curr_perc=lim_min_perc-(100.0-ref_line_pos);

   AggLabVal(lim_min_curr_perc,delta_perc);
/*
   Tracciamento delle curve.
*/
   XClearArea(XtDisplay(curvefield),XtWindow(curvefield),0,0,0,0,True);
   DrawGridCurve(obj);
   DrawCurves(obj);

   UxTopLevelCurveContext = UxSaveCtx;
printf("activateCB_rangedown: USCITO!!!\n");
}



static void  activateCB_increasevalue(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

int i;
float ref_fis,ref_perc,range;
float lim_min_perc,lim_max_perc,lim_min_curr_perc;
float delta_perc,range_perc;
XmString compound;
String str;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;

UxSaveCtx = UxTopLevelCurveContext;
UxTopLevelCurveContext = UxContext =
     (_UxCtopLevelCurve *) UxGetContext(wid);

printf("activateCB_increasevalueaxis: ENTRATO!!!\n");

   ref_perc=0.5;
   for(i=0;i<NUMMAXVAR;i++) {
      if(obj->curve.s_XLcurve.ind_var_sel[i]!=-1) {
         if((strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) ||
            (strcmp(obj->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)) {
            ref_fis=obj->curve.s_XLcurve.min_max_perc.min +
               (obj->curve.s_XLcurve.min_max_perc.max-obj->curve.s_XLcurve.min_max_perc.min)*ref_perc;
            range=(obj->curve.s_XLcurve.min_max_perc.max-obj->curve.s_XLcurve.min_max_perc.min)*(obj->curve.s_XLcurve.factScaleValue);
            obj->curve.s_XLcurve.min_max_perc.max=ref_fis+(range*(1.0-(ref_line_pos/100.0))); 
            obj->curve.s_XLcurve.min_max_perc.min=ref_fis-(range*(ref_line_pos/100.0));
         }
         else {
            ref_fis=obj->curve.s_XLcurve.min_max_oper[i].min +
               (obj->curve.s_XLcurve.min_max_oper[i].max-obj->curve.s_XLcurve.min_max_oper[i].min)*ref_perc;
            range=(obj->curve.s_XLcurve.min_max_oper[i].max-obj->curve.s_XLcurve.min_max_oper[i].min)*(obj->curve.s_XLcurve.factScaleValue);
            obj->curve.s_XLcurve.min_max_oper[i].max=ref_fis+(range*(1.0-(ref_line_pos/100.0)));
            obj->curve.s_XLcurve.min_max_oper[i].min=ref_fis-(range*(ref_line_pos/100.0));
         }
      }
   }

/*
	Aggiornamento delle labels sull'asse dei valori.
*/

   XtVaGetValues(labval1,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_max_perc=atof(str);
   XtVaGetValues(labval6,XmNlabelString,&compound,NULL);
   XmStringGetLtoR(compound,XmSTRING_DEFAULT_CHARSET,&str);
   XmStringFree(compound);
   lim_min_perc=atof(str);
   range_perc=(lim_max_perc-lim_min_perc)*(obj->curve.s_XLcurve.factScaleValue);
   delta_perc=range_perc/NUMDIVHEIGHT;
   lim_min_curr_perc=ref_line_pos-(range_perc*(ref_line_pos/100.0));

   AggLabVal(lim_min_curr_perc,delta_perc);
/*
   Tracciamento delle curve.
*/
   XClearArea(XtDisplay(curvefield),XtWindow(curvefield),0,0,0,0,True);
   DrawGridCurve(obj);
   DrawCurves(obj);

   UxTopLevelCurveContext = UxSaveCtx;



printf("activateCB_increasevalueaxis: USCITO!!!\n");
}



static void  activateCB_configur(wid,obj,call_data)
Widget wid;
XlCurveWidget obj;
XmAnyCallbackStruct *call_data;
{

_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

printf("activateCB_configur: ENTRATO!!!\n");

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(wid);
/*
printf("activateCB_configur: numCurves=%d\n",obj->curve.s_XLcurve.numCurves);
*/
   create_confRunTime(obj,obj->curve.s_XLcurve.numCurves,obj->curve.s_XLcurve.curveSource,
                      obj->curve.s_XLcurve.varInfoChange,obj->curve.s_XLcurve.colTrendString,
                      &obj->curve.s_XLcurve.change_conf_OK);
/*
printf("activateCB_configur: varInfoChange=%s\n",obj->curve.s_XLcurve.varInfoChange[0]);
*/
   UxTopLevelCurveContext = UxSaveCtx;


printf("activateCB_configur: USCITO!!!\n");

}
/*******************************************************************************
       End callback functions.
*******************************************************************************/


int calc_start_pos(camp_f22,rangeTime,pos_end_f22)
SCIRC_SEL_NEW *camp_f22;
int rangeTime;
int pos_end_f22;
{

int pos_start_curr;
int rangeTimeCurr;
/*
printf("calc_start_pos: ENTRATO!!!\n");
*/
   pos_start_curr=pos_end_f22;
   rangeTimeCurr=(int)(camp_f22[pos_end_f22].tempo.tempo-camp_f22[pos_start_curr].tempo.tempo);
   while((rangeTimeCurr<rangeTime)&&(pos_start_curr>0)) {
      pos_start_curr--;
      rangeTimeCurr=(int)(camp_f22[pos_end_f22].tempo.tempo-camp_f22[pos_start_curr].tempo.tempo);
   }
/*
printf("calc_start_pos: pos_start=%d\n",pos_start_curr);
printf("calc_start_pos: USCITO!!!\n");
*/
return(pos_start_curr);
}


int calc_end_pos(camp_f22,rangeTime,pos_start_f22,pos_last_val)
SCIRC_SEL_NEW *camp_f22;
int rangeTime;
int pos_start_f22;
int pos_last_val;
{

int pos_end_curr;
int rangeTimeCurr;

printf("calc_end_pos: ENTRATO!!!\n");

   pos_end_curr=pos_start_f22;
   rangeTimeCurr=(int)(camp_f22[pos_end_curr].tempo.tempo-camp_f22[pos_start_f22].tempo.tempo);
   while((rangeTimeCurr<rangeTime)&&(pos_end_curr<pos_last_val)) {
      pos_end_curr++;
      rangeTimeCurr=(int)(camp_f22[pos_end_curr].tempo.tempo-camp_f22[pos_start_f22].tempo.tempo);
   }
/*
printf("calc_end_pos: rangeTime=%d\tn_last_val=%d\n",rangeTime,pos_last_val);
printf("calc_end_pos: rangeTimeCurr=%d\tpos_end_curr=%d\n",rangeTimeCurr,pos_end_curr);
*/
printf("calc_end_pos: USCITO!!!\n");

return(pos_end_curr);
}



void PushedEffect(w)
Widget w;
{

Pixmap arm_color;
Pixmap background_color;
Pixmap bottom_shadow;
Pixmap top_shadow;

        XtVaGetValues(w,XmNarmColor,&arm_color,NULL);
        XtVaGetValues(w,XmNbackground,&background_color,NULL);
        XtVaGetValues(w,XmNtopShadowColor,&top_shadow,NULL);
        XtVaGetValues(w,XmNbottomShadowColor,&bottom_shadow,NULL);

        /* Setto nuovi valori del bottone (scambio bordini e sfondi)*/

        XtVaSetValues(w,XmNbackground,arm_color,NULL);
        XtVaSetValues(w,XmNarmColor,background_color,NULL);
        XtVaSetValues(w,XmNtopShadowColor,bottom_shadow,NULL);
        XtVaSetValues(w,XmNbottomShadowColor,top_shadow,NULL);

}



void AggLabVal(lim_inf_perc,delta_perc)
float lim_inf_perc;
float delta_perc;
{

XmString compound;
char str[5];
/*
printf("AggLabVal: lim_inf_perc=%f\tdelta_perc=%f\n",lim_inf_perc,delta_perc);
*/
   sprintf(str,"%.1f",lim_inf_perc);
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval6,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   sprintf(str,"%.1f",(lim_inf_perc+delta_perc));
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval5,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   sprintf(str,"%.1f",(lim_inf_perc+(2.0*delta_perc)));
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval4,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   sprintf(str,"%.1f",(lim_inf_perc+(3.0*delta_perc)));
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval3,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   sprintf(str,"%.1f",(lim_inf_perc+(4.0*delta_perc)));
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval2,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   sprintf(str,"%.1f",(lim_inf_perc+(5.0*delta_perc)));
   compound=XmStringCreateSimple(str);
   XtVaSetValues(labval1,XmNlabelString,compound,NULL);
   XmStringFree(compound);

}



void SettingVarInfo(w)
Widget w;
{

int i,j;
char str_info[MAXLENINFOVAR];
char *varinfo[NUMINFOVAR];
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("SettingVarInfo: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
        for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {
           if(strcmp(cw->curve.s_XLcurve.varInfoChange[i],"")!=0) {
                 RecoveryVarInfo(varinfo,cw->curve.s_XLcurve.varInfoChange[i]);
                 strcpy(cw->curve.s_XLcurve.var_lego_cod[i],varinfo[0]);
                 if(cw->curve.s_XLcurve.legend) {
                    XtVaSetValues(labelCodList[i+1],
                           RES_CONVERT( XmNlabelString, varinfo[1]),
                           NULL );
                    XtVaSetValues(labelDescrList[i+1],
                           RES_CONVERT( XmNlabelString, varinfo[2]),
                           NULL );
                    XtVaSetValues(labelLowLimList[i+1],
                           RES_CONVERT( XmNlabelString, varinfo[3]),
                           NULL );
                    XtVaSetValues(labelHighLimList[i+1],
                           RES_CONVERT( XmNlabelString, varinfo[4]),
                           NULL );
                    XtVaSetValues(labelUnitList[i+1],
                           RES_CONVERT( XmNlabelString, varinfo[5]),
                           NULL );
                 }
/*
	Caricamento dei limiti (inferiore e superiore), dell'unita' di misura
	e del suo tipo nell'oggetto XlCurve.
*/
                 strcpy(cw->curve.s_XLcurve.unit_fis[i],varinfo[5]);
                 strcpy(cw->curve.s_XLcurve.tipo_unit[i],varinfo[6]);
/*
printf("SettingVarInfo: unit_fis[%d]=%s\n",i,cw->curve.s_XLcurve.unit_fis[i]);
printf("SettingVarInfo: tipo_unit[%d]=%s\n",i,cw->curve.s_XLcurve.tipo_unit[i]);
*/
	         cw->curve.s_XLcurve.min_max[i].min=(float)atof(varinfo[3]);
	         cw->curve.s_XLcurve.min_max[i].max=(float)atof(varinfo[4]);
	         cw->curve.s_XLcurve.min_max_oper[i].min=(float)atof(varinfo[3]);
	         cw->curve.s_XLcurve.min_max_oper[i].max=(float)atof(varinfo[4]);

                 for(j=0;j<NUMINFOVAR;j++)
                    free(varinfo[j]);
           }
           else {
              strcpy(cw->curve.s_XLcurve.var_lego_cod[i],"");
              if(cw->curve.s_XLcurve.legend) {
                 ResetLegend(cw,i);
                 XtVaSetValues(pushButList[i],XmNsensitive,False,NULL);
              }
           }
        }
        UxTopLevelCurveContext = UxSaveCtx;
              
printf("SettingVarInfo: USCITO!!!\n");

}



void SettingColorTrend(w)
Widget w;
{

int i;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("SettingColorTrend: ENTRATO!!!\n");

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
   for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {
      if(convStringToPix(w,cw->curve.s_XLcurve.colTrendString[i],&cw->curve.s_XLcurve.col_trend[i])==True) {
/*
	La conversione e' avvenuta correttamente. Aggiorno il background dei bottoni
	nella legenda. Aggiorno il graphic context per il tracciamento delle curve.
*/
         XtVaSetValues(pushButList[i],XmNbackground,cw->curve.s_XLcurve.col_trend[i],NULL);
         XSetForeground(XtDisplay(w),cw->curve.s_XLcurve.trends_gc[i],
                        cw->curve.s_XLcurve.col_trend[i]);
         XSetForeground(XtDisplay(w),cw->curve.s_XLcurve.trends_fluct_gc[i],
                        cw->curve.s_XLcurve.col_trend[i]);
      }
   } 
   UxTopLevelCurveContext = UxSaveCtx;

printf("SettingColorTrend: USCITO!!!\n");

}



Boolean RecoveryVarInfo(str_end, varInfoComplete)
char *str_end[];
char *varInfoComplete;
{

char str_app[MAXLENINFOVAR], str_single_info[100], *str_app1, *p;
int length,i=0,j=0;

printf("RecoveryVarInfo: ENTRATO!!!\n");

printf("RecoveryVarInfo: varInfoComplete=%s\n",varInfoComplete);

   strcpy(str_app,varInfoComplete);
   while((str_app1=strchr(str_app, '|')) != NULL) {
      length=strlen(str_app)-strlen(str_app1);
      strncpy(str_single_info, str_app, length);
      str_single_info[length] = '\0';
      p=(char *)calloc(length+1,sizeof(char));
      strcpy(p,str_single_info);
      str_end[i]=p;
/*
printf("RecoveryVarInfo: i=%d\tstr_end=%s\n",i,str_end[i]);
*/
      i++;
      strcpy(str_app,(str_app1+1));
   }
   if(i==(NUMINFOVAR-1)) {
/*
   Nella risorsa non e' stato specificato il tipo di unita' di misura.
   Setto il tipo % come default.
*/      
      p=(char *)calloc(4,sizeof(char));
      strcpy(p,PERCENT);
      str_end[i]=p;
      strcat(varInfoComplete,PERCENT);
      strcat(varInfoComplete,"|");
   }

printf("RecoveryVarInfo: varInfoCompleteNew=%s\n",varInfoComplete);

printf("RecoveryVarInfo: USCITO!!!\n");

return(True);
}



Boolean RecoveryRangeZoom(w)
Widget w;
{

int length,i=0;
char str_app[100],str_single_info[10],*str_app1;

XlCurveWidget cw = (XlCurveWidget)w;

printf("RecoveryRangeZoom: ENTRATO!!!\n");

   strcpy(str_app,cw->curve.s_XLcurve.rangeTimeZoom);
   while((str_app1=strchr(str_app, '|')) != NULL) {
      length=strlen(str_app)-strlen(str_app1);
      strncpy(str_single_info, str_app, length);
      str_single_info[length] = '\0';
      cw->curve.s_XLcurve.range_zoom[i]=atoi(str_single_info);
/*
printf("RecoveryRangeZoom: range_zoom[%d]=%d\n",i,cw->curve.s_XLcurve.range_zoom[i]);
*/
      i++;
      strcpy(str_app,(str_app1+1));
   }
   cw->curve.s_XLcurve.num_range_zoom=i;
   qsort(cw->curve.s_XLcurve.range_zoom,cw->curve.s_XLcurve.num_range_zoom,
         sizeof(int),CompareRange);

printf("RecoveryRangeZoom: USCITO!!!\n");

return(True);
}


int CompareRange(ran1,ran2)
int *ran1;
int *ran2;
{

   if(*ran1>*ran2)
      return(1);
   if(*ran1<*ran2)
      return(-1);

return(0);
}


Boolean LoadLegoCodStr(w,legocod,indic)
Widget w;
char   *legocod;
int    indic;
{

char *p;
size_t size;
XlCurveWidget cw = (XlCurveWidget)w;

printf("LoadLegoCodStr: ENTRATO!!!\n");

	size=strlen(legocod);
        p=(char *)calloc(1,size);
        if(p==NULL) {
           XlError("XlCurve","LoadLegoCodStr","calloc error");
           return(False);
        }  
        strcpy(p,legocod);
        cw->curve.s_XLcurve.var_lego_cod[indic]=p;
/*
        printf("LoadLegoCodStr: var_lego_cod[%d]=%s\n",indic,cw->curve.s_XLcurve.var_lego_cod[indic]);
*/
printf("LoadLegoCodStr: USCITO!!!\n");

return(True);
}



void ActFisScale(w,indic)
Widget w;
int indic;
{
Arg arg[10];
float low_lim,high_lim,interv;
char str_displ[30];
XmString compound;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("ActFisScale: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
	low_lim=cw->curve.s_XLcurve.min_max_oper[indic].min;
	high_lim=cw->curve.s_XLcurve.min_max_oper[indic].max;

	interv=(high_lim-low_lim)/3.0;
/*
printf("ActFisScale:low_lim=%f\thigh_lim=%f\tinterv=%f\n",low_lim,high_lim,interv);
*/
	sprintf(str_displ,"%.1f",low_lim);
        compound=XmStringCreateSimple(str_displ);
        XtVaSetValues(labscafis3,XmNlabelString, compound,
                      XmNforeground,cw->curve.s_XLcurve.col_trend[indic],NULL);
        XmStringFree(compound);
	sprintf(str_displ,"%.1f",low_lim+interv);
        compound=XmStringCreateSimple(str_displ);
        XtVaSetValues(labscafis2,XmNlabelString,compound,
                      XmNforeground,cw->curve.s_XLcurve.col_trend[indic],NULL);
        XmStringFree(compound);
	sprintf(str_displ,"%.1f",low_lim+(2*interv));
        compound=XmStringCreateSimple(str_displ);
        XtVaSetValues(labscafis1,XmNlabelString,compound,
                      XmNforeground,cw->curve.s_XLcurve.col_trend[indic],NULL);
        XmStringFree(compound);
	sprintf(str_displ,"%.1f",high_lim);
        compound=XmStringCreateSimple(str_displ);
        XtVaSetValues(labscafis0,XmNlabelString,compound,
                      XmNforeground,cw->curve.s_XLcurve.col_trend[indic],NULL);
        XmStringFree(compound);
        compound=XmStringCreateSimple(cw->curve.s_XLcurve.unit_fis[indic]);
        XtVaSetValues(labunitfis,XmNlabelString,compound,
                      XmNforeground,cw->curve.s_XLcurve.col_trend[indic],NULL);
        XmStringFree(compound);
        UxTopLevelCurveContext = UxSaveCtx;
	
printf("ActFisScale: USCITO!!!\n");
	
}



void CleanFisScale(w)
Widget w;
{
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("CleanFisScale: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
	XtVaSetValues(labscafis0,XmNforeground,cw->curve.s_XLcurve.graphAreaColor,
		      NULL);
	XtVaSetValues(labscafis1,XmNforeground,cw->curve.s_XLcurve.graphAreaColor,
		      NULL);
	XtVaSetValues(labscafis2,XmNforeground,cw->curve.s_XLcurve.graphAreaColor,
		      NULL);
	XtVaSetValues(labscafis3,XmNforeground,cw->curve.s_XLcurve.graphAreaColor,
		      NULL);
	XtVaSetValues(labunitfis,XmNforeground,cw->curve.s_XLcurve.graphAreaColor,
		      NULL);
        UxTopLevelCurveContext = UxSaveCtx;

printf("CleanFisScale: USCITO!!!\n");

}

Widget create_xlcurve(obj)
Widget obj;
{
int     height_row_col,height_legend_area,interv_lab_time;
char    path_bitmap[1024];
char    *xlcurve_icon_path;
Dimension width_curve,height_curve,height_graph_area;
Position  xobj,yobj;
Widget  father_xlcurve;

XlCurveWidget xlcurve_obj = (XlCurveWidget) obj;
_UxCtopLevelCurve      *UxContext;

   UxTopLevelCurveContext = UxContext =
      (_UxCtopLevelCurve *) UxNewContext( sizeof(_UxCtopLevelCurve), False );

printf("create_xlcurve: ENTRATO!!!");

   father_xlcurve=XtParent(xlcurve_obj);

printf("create_xlcurve: father=%s\n",XtName(father_xlcurve));   

   get_something(xlcurve_obj,XmNwidth, (void*) &width_curve);
   get_something(xlcurve_obj,XmNheight, (void*) &height_curve);
   width_curve-=10;
   height_curve-=10;
   

printf("create_xlcurve: width_curve=%d\theight_curve=%d\n",width_curve,height_curve);
/*
   get_something(xlcurve_obj,XmNx,(void*) &xobj);
   get_something(xlcurve_obj,XmNy,(void*) &yobj);
*/   

   top_curve = xlcurve_obj;
   UxPutContext( top_curve, (char *) UxTopLevelCurveContext );
printf("********************************************************\n");
printf("Creazione widgets fissi costituenti l'oggetto XlCurve!!!\n");

	/* Creation of contenitore1 */
        contenitore1 = XtVaCreateManagedWidget( "contenitore1",
                        xmFormWidgetClass,
                        xlcurve_obj,
                        XmNwidth, width_curve,
                        XmNheight, height_curve,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNsensitive, TRUE,
                        XmNx,5,
                        XmNy,5,
                        XmNunitType, XmPIXELS,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.mainColor,
                        NULL );
        UxPutContext( contenitore1, (char *) UxTopLevelCurveContext );
	XtAddCallback(contenitore1,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

        /* Creation of graphicarea */
        graphicarea = XtVaCreateManagedWidget( "graphicarea",
                        xmFormWidgetClass,
                        contenitore1,
                        XmNwidth, 990,
                        XmNheight, 470,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 5,
                        XmNy, 5,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 50,
                        XmNleftAttachment, XmATTACH_FORM,
                        XmNleftOffset, 10,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 345,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 5,
                        NULL );
        UxPutContext( graphicarea, (char *) UxTopLevelCurveContext );
	XtAddCallback(graphicarea,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

        /* Creation of datetime */
        datetime = XtVaCreateManagedWidget( "datetime",
                        xmLabelWidgetClass,
                        graphicarea,
                        XmNx, 450,
                        XmNy, 270,
                        XmNheight, 25,
                        XmNwidth, 170,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 2,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 2,
			XmNrecomputeSize,False,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00.00.00 00:00:00" ),
                        XmNalignment, XmALIGNMENT_CENTER,
                        RES_CONVERT( XmNbackground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( datetime, (char *) UxTopLevelCurveContext );
	XtAddCallback(datetime,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

        /* Creation of sourcedata */
        sourcedata = XtVaCreateManagedWidget( "sourcedata",
                        xmLabelWidgetClass,
                        graphicarea,
                        XmNx, 0,
                        XmNy, 410,
                        XmNheight, 20,
                        XmNwidth, 600,
                        RES_CONVERT( XmNlabelString, "Data source" ),
                        XmNalignment, XmALIGNMENT_BEGINNING,
                        RES_CONVERT( XmNbackground, "WhiteSmoke" ),
                        XmNfontList, UxConvertFontList("-adobe-courier-bold-o-normal--12--120-75-75-m-70-iso8859-1" ),
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 2,
                        XmNleftOffset, 2,
                        NULL );
        UxPutContext( sourcedata, (char *) UxTopLevelCurveContext );
	XtAddCallback(sourcedata,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

        /* Creation of curvefield */
        curvefield = XtVaCreateManagedWidget( "curvefield",
                        xmDrawingAreaWidgetClass,
                        graphicarea,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNwidth, 700,
                        XmNheight, 380,
                        XmNx, 20,
                        XmNy, 44,
                        XmNleftOffset, 20,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.curveFieldColor,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 260,
                        XmNleftAttachment, XmATTACH_FORM,
                        XmNtopOffset, 44,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 60,
                        XmNtopAttachment, XmATTACH_FORM,
                        NULL );
        UxPutContext( curvefield, (char *) UxTopLevelCurveContext );
	XtAddCallback(curvefield,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

        if(xlcurve_obj->curve.s_XLcurve.timePeriodKeys) {
/*
        Si creano i pushbuttons per la variazione della scala dei tempi.
*/
                /* Creation of changetime */
                changetime = XtVaCreateManagedWidget( "changetime",
                        xmFormWidgetClass,
                        graphicarea,
                        XmNwidth, 750,
                        XmNheight, 40,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 20,
                        XmNy, 10,
                        XmNleftOffset, 20,
                        XmNbottomAttachment, XmATTACH_WIDGET,
                        XmNbottomOffset, 2,
                        XmNbottomWidget, curvefield,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 260,
                        XmNleftAttachment, XmATTACH_FORM,
                        NULL );
                UxPutContext( changetime, (char *) UxTopLevelCurveContext );

                /* Creation of increasetime */
                increasetime = XtVaCreateManagedWidget( "increasetime",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 20,
                        XmNy, 7,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNleftOffset, 20,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopOffset, 7,
                        NULL );
                UxPutContext( increasetime, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(increasetime,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_increasetime,
		  	 xlcurve_obj);

                /* Creation of timeleft */
                timeleft = XtVaCreateManagedWidget( "timeleft",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 60,
                        XmNy, 7,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, increasetime,
                        NULL );
                UxPutContext( timeleft, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(timeleft,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_timeleft,
		  	 xlcurve_obj);

                /* Creation of seldata */
                seldata = XtVaCreateManagedWidget( "seldata",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 100,
                        XmNy, 7,
                        XmNwidth, 80,
                        XmNheight, 30,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, timeleft,
                        NULL );
                UxPutContext( seldata, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(seldata,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_seldata,
                        xlcurve_obj);


                /* Creation of timeright */
                timeright = XtVaCreateManagedWidget( "timeright",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 210,
                        XmNy, 7,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, seldata,
                        NULL );
                UxPutContext( timeright, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(timeright,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_timeright,
		  	 xlcurve_obj);

                /* Creation of decreasetime */
                decreasetime = XtVaCreateManagedWidget( "decreasetime",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 250,
                        XmNy, 7,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, timeright,
                        NULL );
                UxPutContext( decreasetime, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(decreasetime,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_decreasetime,
		  	 xlcurve_obj);


                /* Creation of abstime */
                abstime = XtVaCreateManagedWidget( "abstime",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 300,
                        XmNy, 7,
                        XmNwidth, 80,
                        XmNheight, 30,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, decreasetime,
                        NULL );
                UxPutContext( abstime, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(abstime,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_abstime,
                        xlcurve_obj);

                /* Creation of flow */
                flow = XtVaCreateManagedWidget( "flow",
                        xmPushButtonWidgetClass,
                        changetime,
                        XmNx, 410,
                        XmNy, 7,
                        XmNwidth, 80,
                        XmNheight, 30,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 10,
                        XmNleftWidget, abstime,
                        NULL );
                UxPutContext( flow, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(flow,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_flow,
                        xlcurve_obj);

        }
        else {
                set_something_val(curvefield,XmNtopOffset,(XtArgVal) 2);
        }

        if(xlcurve_obj->curve.s_XLcurve.valueRangeKeys) {
/*
        Si creano i pushbuttons per la variazione del range dei
        valori.
*/
                /* Creation of changevalue */
                changevalue = XtVaCreateManagedWidget( "changevalue",
                        xmFormWidgetClass,
                        graphicarea,
                        XmNwidth, 40,
                        XmNheight, 380,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 830,
                        XmNy, 44,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 218,
                        XmNleftWidget, curvefield,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopOffset, 46,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 60,
                        XmNtopAttachment, XmATTACH_FORM,
                        NULL );
                UxPutContext( changevalue, (char *) UxTopLevelCurveContext );

                /* Creation of increasevalue */
                increasevalue = XtVaCreateManagedWidget( "increasevalue",
                        xmPushButtonWidgetClass,
                        changevalue,
                        XmNx, 7,
                        XmNy, 20,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNleftOffset, 7,
                        XmNtopOffset, 20,
                        NULL );
                UxPutContext( increasevalue, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(increasevalue, XmNactivateCallback,
                  	(XtCallbackProc)activateCB_increasevalue,
		  	 xlcurve_obj);

                /* Creation of rangeup */
                rangeup = XtVaCreateManagedWidget( "rangeup",
                        xmPushButtonWidgetClass,
                        changevalue,
                        XmNx, 7,
                        XmNy, 60,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopOffset, 10,
                        XmNtopWidget, increasevalue,
                        NULL );
                UxPutContext( rangeup, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(rangeup,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_rangeup,
		  	 xlcurve_obj);

                /* Creation of defaultset */
                defaultset = XtVaCreateManagedWidget( "defaultset",
                        xmPushButtonWidgetClass,
                        changevalue,
                        XmNx, 7,
                        XmNy, 100,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopOffset, 10,
                        XmNtopWidget, rangeup,
                        NULL );
                UxPutContext( defaultset, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(defaultset,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_defaultset,
		  	 xlcurve_obj);


                /* Creation of rangedown */
                rangedown = XtVaCreateManagedWidget( "rangedown",
                        xmPushButtonWidgetClass,
                        changevalue,
                        XmNx, 7,
                        XmNy, 140,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopOffset, 10,
                        XmNtopWidget, defaultset,
                        NULL );
                UxPutContext( rangedown, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(rangedown,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_rangedown,
		  	 xlcurve_obj);

                /* Creation of decreasevalue */
                decreasevalue = XtVaCreateManagedWidget( "decreasevalue",
                        xmPushButtonWidgetClass,
                        changevalue,
                        XmNx, 7,
                        XmNy, 180,
                        XmNwidth, 30,
                        XmNheight, 30,
                        XmNlabelType, XmPIXMAP,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.keysColor,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopOffset, 10,
                        XmNtopWidget, rangedown,
                        NULL );
                UxPutContext( decreasevalue, (char *) UxTopLevelCurveContext );
		if(xlcurve_obj->xlmanager.config==False)
		   XtAddCallback(decreasevalue,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_decreasevalue,
		  	 xlcurve_obj);
        }
        else {
                set_something_val(curvefield,XmNrightOffset,(XtArgVal)220);
                if(xlcurve_obj->curve.s_XLcurve.timePeriodKeys)
                   set_something_val(changetime,XmNrightOffset,(XtArgVal)220);
        }

        /* Creation of formvalue */
        formvalue = XtVaCreateManagedWidget( "formvalue",
                        xmFormWidgetClass,
                        graphicarea,
                        XmNwidth, 55,
                        XmNheight, 380,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 1010,
                        XmNy, 220,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 130,
                        XmNleftWidget, curvefield,
                        XmNtopOffset, 34,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 50,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( formvalue, (char *) UxTopLevelCurveContext );

        /* Creation of labval1 */
        labval1 = XtVaCreateManagedWidget( "labval1",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 10,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "100.0" ),
                        XmNtopOffset, 2,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNbottomPosition, 4,
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( labval1, (char *) UxTopLevelCurveContext );

        /* Creation of labval2 */
        labval2 = XtVaCreateManagedWidget( "labval2",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 70,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "80.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 23,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( labval2, (char *) UxTopLevelCurveContext );

        /* Creation of labval3 */
        labval3 = XtVaCreateManagedWidget( "labval3",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 210,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "60.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbottomPosition, 42,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( labval3, (char *) UxTopLevelCurveContext );

        /* Creation of labval4 */
        labval4 = XtVaCreateManagedWidget( "labval4",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 310,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "40.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 61,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( labval4, (char *) UxTopLevelCurveContext );

        /* Creation of labval5 */
        labval5 = XtVaCreateManagedWidget( "labval5",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 400,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "20.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 80,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( labval5, (char *) UxTopLevelCurveContext );

        /* Creation of labval6 */
        labval6 = XtVaCreateManagedWidget( "labval6",
                        xmLabelWidgetClass,
                        formvalue,
                        XmNx, 0,
                        XmNy, 520,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "0.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,  
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 100,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        NULL );
        UxPutContext( labval6, (char *) UxTopLevelCurveContext );

	/* Creation of formscafis */
        formscafis = XtVaCreateManagedWidget( "formscafis",
                        xmFormWidgetClass,
                        graphicarea,
                        XmNwidth, 55,
                        XmNheight, 380,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 990,
                        XmNy, 50,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 50,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 50,
                        XmNleftWidget, curvefield,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 34,
                        NULL );
        UxPutContext( formscafis, (char *) UxTopLevelCurveContext );

        /* Creation of labscafis0 */
        labscafis0 = XtVaCreateManagedWidget( "labscafis0",
                        xmLabelWidgetClass,
                        formscafis,
                        XmNx, 0,
                        XmNy, 310,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "0.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 5,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNforeground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( labscafis0, (char *) UxTopLevelCurveContext );

        /* Creation of labscafis1 */
        labscafis1 = XtVaCreateManagedWidget( "labscafis1",
                        xmLabelWidgetClass,
                        formscafis,
                        XmNx, 0,
                        XmNy, 310,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "0.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 33,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNforeground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( labscafis1, (char *) UxTopLevelCurveContext );

        /* Creation of labscafis2 */
        labscafis2 = XtVaCreateManagedWidget( "labscafis2",
                        xmLabelWidgetClass,
                        formscafis,
                        XmNx, 0,
                        XmNy, 400,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "0.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 66,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNforeground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( labscafis2, (char *) UxTopLevelCurveContext );

        /* Creation of labscafis3 */
        labscafis3 = XtVaCreateManagedWidget( "labscafis3",
                        xmLabelWidgetClass,
                        formscafis,
                        XmNx, 0,
                        XmNy, 520,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNalignment, XmALIGNMENT_END,
                        RES_CONVERT( XmNlabelString, "0.0" ),
                        XmNrecomputeSize, FALSE,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,
                        XmNbottomAttachment, XmATTACH_POSITION,
                        XmNbottomPosition, 100,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNforeground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( labscafis3, (char *) UxTopLevelCurveContext );

        /* Creation of labunitfis */
        labunitfis = XtVaCreateManagedWidget( "labunitfis",
                        xmLabelWidgetClass,
                        graphicarea,
                        XmNx, 980,
                        XmNy, 10,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_value,
                        RES_CONVERT( XmNlabelString, "%" ),
                        XmNforeground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 60,
                        XmNleftWidget, curvefield,
                        NULL );
        UxPutContext( labunitfis, (char *) UxTopLevelCurveContext );

        /* Creation of formtime */
        formtime = XtVaCreateManagedWidget( "formtime",
                        xmFormWidgetClass,
                        graphicarea,
                        XmNwidth, 780,
                        XmNheight, 21,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 30,
                        XmNy, 570,
                        XmNleftOffset, 20,
                        XmNtopAttachment, XmATTACH_WIDGET,
                        XmNtopOffset, 10,
                        XmNtopWidget, curvefield,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 210,
                        XmNleftAttachment, XmATTACH_FORM,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        NULL );
        UxPutContext( formtime, (char *) UxTopLevelCurveContext );

        /* Creation of labtime1 */
        labtime1 = XtVaCreateManagedWidget( "labtime1",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 90,
                        XmNy, 10,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNtopOffset, 0,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 17,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime1, (char *) UxTopLevelCurveContext );

        /* Creation of labtime2 */
        labtime2 = XtVaCreateManagedWidget( "labtime2",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 190,
                        XmNy, 0,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 60,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 34,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime2, (char *) UxTopLevelCurveContext );

        /* Creation of labtime3 */
        labtime3 = XtVaCreateManagedWidget( "labtime3",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 320,
                        XmNy, 0,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 60,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 51,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime3, (char *) UxTopLevelCurveContext );

        /* Creation of labtime4 */
        labtime4 = XtVaCreateManagedWidget( "labtime4",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 450,
                        XmNy, 0,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 60,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 68,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime4, (char *) UxTopLevelCurveContext );

        /* Creation of labtime5 */
        labtime5 = XtVaCreateManagedWidget( "labtime5",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 570,
                        XmNy, 0,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 60,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 85,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime5, (char *) UxTopLevelCurveContext );

        /* Creation of labtime6 */
        labtime6 = XtVaCreateManagedWidget( "labtime6",
                        xmLabelWidgetClass,
                        formtime,
                        XmNx, 710,
                        XmNy, 0,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        RES_CONVERT( XmNlabelString, "00:00" ),
                        XmNheight, 21,
                        XmNwidth, 60,
                        XmNrecomputeSize, FALSE,
                        XmNrightAttachment, XmATTACH_POSITION,
                        XmNrightPosition, 100,
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNresizable, FALSE,
                        NULL );
        UxPutContext( labtime6, (char *) UxTopLevelCurveContext );

        /* Creation of labvalun */
        labvalun = XtVaCreateManagedWidget( "labvalun",
                        xmLabelWidgetClass,
                        graphicarea,
                        XmNx, 1070,
                        XmNy, 10,
                        XmNheight, 21,
                        XmNwidth, 55,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_time,  
                        XmNbackground,xlcurve_obj->curve.s_XLcurve.graphAreaColor,
                        RES_CONVERT( XmNlabelString, "%" ),
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNalignment, XmALIGNMENT_CENTER,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 130,
                        XmNleftWidget, curvefield,
                        NULL );
        UxPutContext( labvalun, (char *) UxTopLevelCurveContext );


/*
        Aggiunta dei pixmap sui pushbuttons. Settaggio del background.
*/
        xlcurve_icon_path=getenv("LEGOCAD_ICO");
        if(xlcurve_icon_path==NULL) {
           XlWarning("XlCurve","LoadBitmap","impossibile leggere la variabili LEGOCAD_ICO");
        }
        else {
           if(xlcurve_obj->curve.s_XLcurve.timePeriodKeys) {

printf("create_xlcurve: Aggiunta dei bitmaps sui pushbuttons relativi ai tempi\n");

              XtVaSetValues(changetime,
			    XmNbackground,xlcurve_obj->curve.s_XLcurve.keysColor,NULL);
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/increaseTime.bmp");
              XtVaSetValues(increasetime,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );

              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/timeLeft.bmp");
              XtVaSetValues(timeleft,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/timeRight.bmp");
              XtVaSetValues(timeright,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/decreaseTime.bmp");
              XtVaSetValues(decreasetime,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
           }
           if(xlcurve_obj->curve.s_XLcurve.valueRangeKeys) {

printf("create_xlcurve: Aggiunta dei bitmaps sui pushbuttons relativi ai valori\n");

              XtVaSetValues(changevalue,
                            XmNbackground,xlcurve_obj->curve.s_XLcurve.keysColor,NULL);
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/increaseAxis.bmp");
              XtVaSetValues(increasevalue,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/rangeUp.bmp");
              XtVaSetValues(rangeup,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/rangeDown.bmp");
              XtVaSetValues(rangedown,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/defaultSet.bmp");
              XtVaSetValues(defaultset,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
              strcpy(path_bitmap,xlcurve_icon_path);
              strcat(path_bitmap,"/decreaseAxis.bmp");
              XtVaSetValues(decreasevalue,
                           RES_CONVERT( XmNlabelPixmap, path_bitmap),
                           NULL );
           }
        }

printf("\nFine creazione widgets fissi dell'oggetto XlCurve!!!!!\n");
printf("********************************************************\n");

return(top_curve);

}


Widget create_legend(obj)
Widget obj;
{

int height_row_col,i;
XlCurveWidget xlcurve_obj = (XlCurveWidget)obj;
char nomeWidget[20],nomeApp[5];
Pixmap pixmap;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

printf("create_legend: ENTRATO!!\n");

     UxSaveCtx = UxTopLevelCurveContext;
     UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(obj);
     if(xlcurve_obj->curve.s_XLcurve.legend) {
        /* Creation of legendarea */
        height_row_col=((24 + 3) * xlcurve_obj->curve.s_XLcurve.numCurves);
        legendarea = XtVaCreateManagedWidget( "legendarea",
                        xmFormWidgetClass,
                        contenitore1,
                        XmNwidth, 900,
                        XmNheight, 335,
                        XmNresizePolicy, XmRESIZE_NONE,
                        XmNx, 5,
                        XmNy, 500,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 90,
                        XmNleftAttachment, XmATTACH_FORM,
                        XmNleftOffset, 10,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 5,
                        XmNtopAttachment, XmATTACH_NONE,
                        XmNtopOffset, 480,
                        NULL );
        UxPutContext( legendarea, (char *) UxTopLevelCurveContext );
	XtAddCallback(legendarea,  XmNexposeCallback,
                      (XtCallbackProc)exposeCB_curvefield,
		       xlcurve_obj);

                /* Creation of rowColBut */
                rowColBut = XtVaCreateManagedWidget( "rowColBut",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 24,
                        XmNheight, height_row_col,
                        XmNx, 5,
                        XmNy, 30,
                        XmNleftOffset, 5,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 29,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16,  */
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNpacking, XmPACK_COLUMN,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNradioAlwaysOne, FALSE,
                        XmNisAligned, FALSE,
                        XmNadjustMargin, TRUE,
                        XmNadjustLast, TRUE,
                        XmNnavigationType, XmNONE,
                        XmNorientation, XmVERTICAL,
                        XmNtraversalOn, TRUE,
                        XmNresizable, FALSE,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColBut, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColBut,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                for(i=0;i<xlcurve_obj->curve.s_XLcurve.numCurves;i++) {
                   strcpy(nomeWidget,"button_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   pushButList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmPushButtonWidgetClass,
                        rowColBut,
                        XmNx, 0,
                        XmNy, 0,
                        XmNwidth, 24,
                        XmNheight, 24,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNrecomputeSize, FALSE,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.col_trend[i],
                        NULL );
                   UxPutContext( pushButList[i], (char *) UxTopLevelCurveContext );
                   if(xlcurve_obj->xlmanager.config==False)
                      XtAddCallback(pushButList[i],  XmNactivateCallback,
                           (XtCallbackProc)activateCB_pushButton,
                           &xlcurve_obj->curve.s_XLcurve.toggle_state[i]);
                }
                
printf("create_xlcurve: numCurves=%d!!!\n",xlcurve_obj->curve.s_XLcurve.numCurves);

	height_row_col=(((xlcurve_obj->curve.s_XLcurve.numCurves+1))*27 - 1);
                /* Creation of rowColCod */
                rowColCod = XtVaCreateManagedWidget( "rowColCod",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 280,
                        XmNheight, height_row_col,
                        XmNx, 520,
                        XmNy, 0,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColBut,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16, */
                        XmNmarginHeight, 0,
                        XmNpacking, XmPACK_TIGHT,
                        XmNradioAlwaysOne, FALSE,
                        XmNadjustLast, TRUE,
                        XmNnavigationType, XmNONE,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNisAligned, TRUE,
                        XmNspacing, 3,
                        XmNmarginWidth, 0,
                        XmNresizable, TRUE,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColCod, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColCod,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                labelCodList[0] = XtVaCreateManagedWidget( "labCod",
                        xmLabelWidgetClass,
                        rowColCod,
                        XmNx, 40,
                        XmNy, 20,
                        XmNwidth, 210,
                        XmNheight, 24,
                        XmNmarginWidth, 2,
                        XmNrecomputeSize, FALSE,
                        XmNborderWidth, 0,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                UxPutContext( labelCodList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColdescr */
                rowColdescr = XtVaCreateManagedWidget( "rowColdescr",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 240,
                        XmNheight, height_row_col,
                        XmNx, 277,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16,  */
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftWidget, rowColCod,
                        XmNleftOffset, 5,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColdescr, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColdescr,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label12 */
                labelDescrList[0] = XtVaCreateManagedWidget( "labDescr",
                        xmLabelWidgetClass,
                        rowColdescr,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 250,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelDescrList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColLimLow */
                rowColLimLow = XtVaCreateManagedWidget( "rowColLimLow",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 110,
                        XmNheight, height_row_col,
                        XmNx, 480,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16,  */
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColdescr,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColLimLow, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColLimLow,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label23 */
                labelLowLimList[0] = XtVaCreateManagedWidget( "labLi",
                        xmLabelWidgetClass,
                        rowColLimLow,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelLowLimList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColVal */
                rowColVal = XtVaCreateManagedWidget( "rowColVal",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 110,
                        XmNheight, height_row_col,
                        XmNx, 560,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomOffset, 16,
                        XmNbottonAttachment, XmATTACH_FORM,  */
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColLimLow,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColVal, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColVal,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label34 */
                labelValueList[0] = XtVaCreateManagedWidget( "labVal",
                        xmLabelWidgetClass,
                        rowColVal,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelValueList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColLimHigh */
                rowColLimHigh = XtVaCreateManagedWidget( "rowColLimHigh",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 110,
                        XmNheight, height_row_col,
                        XmNx, 660,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16,   */
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColVal,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColLimHigh, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColLimHigh,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label45 */
                labelHighLimList[0] = XtVaCreateManagedWidget( "labLs",
                        xmLabelWidgetClass,
                        rowColLimHigh,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelHighLimList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColUnit */
                rowColUnit = XtVaCreateManagedWidget( "rowColUnit",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 110,
                        XmNheight, height_row_col,
                        XmNx, 750,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        /* XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 16,  */
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColLimHigh,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        NULL );
                UxPutContext( rowColUnit, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColUnit,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label56 */
                labelUnitList[0] = XtVaCreateManagedWidget( "labUn",
                        xmLabelWidgetClass,
                        rowColUnit,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelUnitList[0], (char *) UxTopLevelCurveContext );

                /* Creation of rowColCursor */
                rowColCursor = XtVaCreateManagedWidget( "rowColCursor",
                        xmRowColumnWidgetClass,
                        legendarea,
                        XmNwidth, 110,
                        XmNheight, height_row_col,
                        XmNx, 850,
                        XmNmarginHeight, 0,
                        XmNmarginWidth, 0,
                        XmNspacing, 3,
                        XmNpacking, XmPACK_TIGHT,
                        XmNresizeHeight, FALSE,
                        XmNresizeWidth, FALSE,
                        XmNradioAlwaysOne, FALSE,
                        XmNnavigationType, XmNONE,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNtopOffset, 2,
                        XmNleftAttachment, XmATTACH_WIDGET,
                        XmNleftOffset, 5,
                        XmNleftWidget, rowColUnit,
                        XmNrightAttachment, XmATTACH_NONE,
                        XmNrightOffset, 10,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                UxPutContext( rowColCursor, (char *) UxTopLevelCurveContext );
	        XtAddCallback(rowColCursor,  XmNexposeCallback,
                              (XtCallbackProc)exposeCB_curvefield,
		              xlcurve_obj);

                /* Creation of label67 */
                labelCursorList[0] = XtVaCreateManagedWidget( "labCur",
                        xmLabelWidgetClass,
                        rowColCursor,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 24,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                UxPutContext( labelCursorList[0], (char *) UxTopLevelCurveContext );

                for(i=1;i<=xlcurve_obj->curve.s_XLcurve.numCurves;i++) {
                   /* Creation of label1 */
                   strcpy(nomeWidget,"label_cod_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelCodList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColCod,
                        XmNx, 40,
                        XmNy, 20,
                        XmNwidth, 200,
                        XmNheight, 22,
                        XmNborderWidth, 1,
                        XmNrecomputeSize, FALSE,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                   UxPutContext( labelCodList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label12 */
                   strcpy(nomeWidget,"label_descr_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelDescrList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColdescr,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 250,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelDescrList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label23 */
                   strcpy(nomeWidget,"label_ll_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelLowLimList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColLimLow,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelLowLimList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label34 */
                   strcpy(nomeWidget,"label_val_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelValueList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColVal,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelValueList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label45 */
                   strcpy(nomeWidget,"label_hl_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelHighLimList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColLimHigh,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelHighLimList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label56 */
                   strcpy(nomeWidget,"label_unit_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelUnitList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColUnit,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelUnitList[i], (char *) UxTopLevelCurveContext );

                   /* Creation of label67 */
                   strcpy(nomeWidget,"label_curs_");
                   sprintf(nomeApp,"%d",i);
                   strcat(nomeWidget,nomeApp);
                   labelCursorList[i] = XtVaCreateManagedWidget( nomeWidget,
                        xmLabelWidgetClass,
                        rowColCursor,
                        XmNx, 40,
                        XmNy, 10,
                        XmNheight, 22,
                        XmNrecomputeSize, FALSE,
                        XmNwidth, 200,
                        XmNborderWidth, 1,
                        RES_CONVERT( XmNlabelString, "" ),
                        XmNbackground, xlcurve_obj->curve.s_XLcurve.legendColor,
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend,
                        NULL );
                   UxPutContext( labelCursorList[i], (char *) UxTopLevelCurveContext );

                }
                if(xlcurve_obj->curve.s_XLcurve.configurationButton) {
                   /* Creation of configur */
                   configur = XtVaCreateManagedWidget( "configur",
                        xmPushButtonWidgetClass,
                        contenitore1,
                        XmNx, 940,
                        XmNy, 520,
                        XmNwidth, 80,
                        XmNheight, 30,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 5,
                        RES_CONVERT( XmNlabelString, "Config" ),
                        XmNtopAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 310,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        RES_CONVERT( XmNbackground, "black" ),
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                   UxPutContext( configur, (char *) UxTopLevelCurveContext );
		   if(xlcurve_obj->xlmanager.config==False)
		      XtAddCallback(configur,  XmNactivateCallback,
                  	(XtCallbackProc)activateCB_configur,
                        xlcurve_obj);
                }
                if(xlcurve_obj->curve.s_XLcurve.saveButton) {
                   /* Creation of save */
                   save = XtVaCreateManagedWidget( "save",
                        xmPushButtonWidgetClass,
                        contenitore1,
                        XmNx, 940,
                        XmNy, 560,
                        XmNwidth, 80,
                        XmNheight, 30,
                        RES_CONVERT( XmNlabelString, "Save" ),
                        XmNleftAttachment, XmATTACH_NONE,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 5,
                        XmNtopAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 280,
                        RES_CONVERT( XmNbackground, "black" ),
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                   UxPutContext( save, (char *) UxTopLevelCurveContext );
                }

                if(xlcurve_obj->curve.s_XLcurve.saveGroupsButton) {
                   /* Creation of savegr */
                   savegr = XtVaCreateManagedWidget( "savegr",
                        xmPushButtonWidgetClass,
                        contenitore1,
                        XmNx, 940,
                        XmNy, 600,
                        XmNwidth, 80,
                        XmNheight, 30,
                        RES_CONVERT( XmNlabelString, "Save gr." ),
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 5,
                        XmNtopAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 250,
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        RES_CONVERT( XmNbackground, "black" ),
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                   UxPutContext( savegr, (char *) UxTopLevelCurveContext );
                }
                if(xlcurve_obj->curve.s_XLcurve.groupsButton) {
                   /* Creation of groups */
                   groups = XtVaCreateManagedWidget( "groups",
                        xmPushButtonWidgetClass,
                        contenitore1,
                        XmNx, 940,
                        XmNy, 640,
                        XmNwidth, 80,
                        XmNheight, 30,
                        RES_CONVERT( XmNlabelString, "Groups" ),
                        XmNleftAttachment, XmATTACH_NONE,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNrightOffset, 5,
                        XmNtopAttachment, XmATTACH_NONE,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 220,
                        RES_CONVERT( XmNbackground, "black" ),
                        RES_CONVERT( XmNforeground, "WhiteSmoke" ),
                        XmNfontList,xlcurve_obj->curve.s_XLcurve.font_list_legend, 
                        NULL );
                   UxPutContext( groups, (char *) UxTopLevelCurveContext );
                }
       }
       else {
          set_something_val(graphicarea,XmNbottomOffset,(XtArgVal)5);
       }
       UxTopLevelCurveContext = UxSaveCtx;

printf("create_legend: Fine creazione widgets costituenti l'oggetto XlCurve!!!\n");

printf("create_legend: USCITO!!!\n");

return(top_curve);

}




