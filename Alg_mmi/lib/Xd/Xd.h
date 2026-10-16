/**********************************************************************
*
*       C Header:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/

/*
 Xd.h
 Definizioni e prototiping di funzioni utilizzate nella gestione
 dei draget
*/
#include <stdio.h>
#ifndef _Xd_h
#define _Xd_h
#include <Xd/XdCore.h>   /* contiene le definizioni di Draget e DragetClass */
#include <Xd/XdLista.h>   /* contiene le definizioni di XdLista */
#include <Xd/XdGeom.h>    /* contiene definizioni di funzioni geometriche
                              di uso generale */
#include <Xl/Xl.h>

#include <Rt/RtMemory.h>
typedef struct s_group_ev {
			int type;
			int n_angle; /* angolo fisso del resize 
					0 - angolo in alto a sinistra
					1 - angolo in alto a destra
					2 - basso sinistra
					3 - basse a destra
					*/
			int xfix;      /* coordinate del punto che rimane
					  fisso */
			int yfix;
			float fatt_x;  /* ridimensionamento 
					  lungo l'asse x */
			float fatt_y; /* ridimensionamento lungo
					  l'asse y */
			} GROUP_EV; 
#define RESIZE_GROUP 1
/*
 Prototiping delle funzioni
*/
extern DragetClass XdClass(Draget);
extern Boolean XdIsSelected(Draget);
extern Boolean XdIsManaged(Draget);
extern Draget XdCreateDraget(Draget,DragetClass, GC, GC);
extern Draget XdDuplicateDraget(Draget,Widget);
extern void XdManageDraget(Draget);
extern void  XdDrawDraget(Draget,XEvent *);
extern Boolean XdPickDraget(Draget,XEvent *);
extern Boolean XdPickExtDraget(Draget,XEvent *);
extern void XdDeselectDraget(Draget,Boolean);
extern void XdSelectDraget(Draget,Boolean);
extern void XdFirstDraw(Draget,XEvent *);
extern void XdFirstPoint(Draget,XEvent *);
extern Boolean XdLastPoint(Draget,XEvent *);
extern char *XdPixelToString(Widget,Pixel);
extern Pixel XdStringToPixel(Widget,String);
extern Boolean _XdInheritPick(Draget, XEvent* , Boolean);
extern void _XdInheritSelect(Draget, Boolean,Boolean);
extern void _XdInheritClassInitialize(Draget);
extern void _XdInheritDeleteRegions(Draget);
extern Boolean XdDragetInRect(Draget, XRectangle *);
extern Draget XdReadRecord(Widget, XdGC *,FILE *);
extern GC XdGetXorGC(Widget );
extern GC XdGetCopyGC(Widget );
extern Boolean XdIsConn(Draget );
extern Boolean XdIsGroup(Draget );
extern Boolean XdGetFilled(Draget );

/*
 Definizioni 
*/
#define TRASLA_CONN  0
#define RESIZE_CONN  1
#endif  /* _Xd_h   */

