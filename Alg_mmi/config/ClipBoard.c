
/*******************************************************************************
       ClipBoard.c
       (Generated from interface file ClipBoard.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/DrawingA.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo ClipBoard.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)ClipBoard.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <Xm/Xm.h>
#include <Xd/Xd.h>
#include "config.h"

#define VMIN 100	/*tipo di calcolo sulle coordinate */
#define VMAX 101 

XrmDatabase CuttedDb = NULL;


/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	int	mumble;
} _UxCClipBoard;

static _UxCClipBoard           *UxClipBoardContext;


Widget	ClipBoard;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_ClipBoard();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*---------------------------------------------------
 * CBQuery
 *
 * verifica la esistenza di oggetti nella ClipBoard
 *--------------------------------------------------*/  
int CBQuery(Widget **CBChild,Cardinal *CBnumChild)
{
   extern void get_child();
   Cardinal lnargs;
   Arg args[3]; 

   *CBnumChild=0;

   lnargs = 0;
   XtSetArg( args[lnargs],XmNchildren,CBChild);lnargs++;
   XtSetArg(args[lnargs],XmNnumChildren,CBnumChild);lnargs++;
   XtGetValues(ClipBoard,args,lnargs);

   if(*CBnumChild)
      return(True);

   return(False);
}



/* 
 * calc
 *
 * calcola tra tutti gli nele valori val il massimo o il minimo 
 * e lo restituisce in ret. 
 * if type = VMIN calcola il minimo
 * if type = VMAX calcola il massimo 
 * 
*/
void calc(int type,Position *ret,Position *val,Cardinal nele )
{
   int i;

   *ret = val[0];
   
   if(type == VMIN)
   {
      for(i=0;i<nele;i++)
        if(val[i] < *ret)
          *ret = val[i];
   }
   else if(type == VMAX) 
   {
      for(i=0;i<nele;i++)
         if(val[i] > *ret)
            *ret = val[i]; 
   }
}

/*--------------------------------------
 *    
 * determina le coordinate massime e minime 
 * tra tutti gli widget presenti nella clipboard
 *  
 --------------------------------------*/
int CBGetCoordWid(Position *minX,Position *minY,Position *maxX,Position *maxY)
{
   extern void get_child();
   Widget *wgtlst;
   Position *x,*y;
   Dimension *w,*h;
   Cardinal numChild,i;

/* recupero gli widget nella CB */

   if(CBQuery(&wgtlst,&numChild))
   {

      if( (x = (Position *)alloca_memoria(numChild,sizeof(Position))) == NULL)
         return(False);

      if( (y = (Position *)alloca_memoria(numChild,sizeof(Position))) == NULL)
         return(False);

      if( (w = (Position *)alloca_memoria(numChild,sizeof(Dimension))) == NULL)
         return(False);

      if( (h = (Position *)alloca_memoria(numChild,sizeof(Dimension))) == NULL)
         return(False);

      for(i=0;i<numChild;i++)
         get_geometry(wgtlst[i],&x[i],&y[i],&w[i],&h[i]);                    

/* calcolo la x e la y minima */
      calc(VMIN,minX,x,numChild);
      calc(VMIN,minY,y,numChild);

      for(i=0;i<numChild;i++)
      {
         x[i] += w[i];
         y[i] += h[i];
      }

/* calcolo la x e la y massima */      
      calc(VMAX,maxX,x,numChild);
      calc(VMAX,maxY,y,numChild);

      libera_memoria((char*)x);
      libera_memoria((char*)y);
      libera_memoria((char*)w);
      libera_memoria((char*)h);
   
      return(True);
   }
   return(False);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_ClipBoard()
{
	Widget		_UxParent;


	/* Creation of ClipBoard */
	_UxParent = XtVaCreatePopupShell( "ClipBoard_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 400,
			XmNy, 220,
			XmNwidth, 570,
			XmNheight, 500,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "ClipBoard",
			XmNiconName, "ClipBoard",
			NULL );

	ClipBoard = XtVaCreateManagedWidget( "ClipBoard",
			xmDrawingAreaWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 570,
			XmNheight, 500,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			NULL );
	UxPutContext( ClipBoard, (char *) UxClipBoardContext );


	XtAddCallback( ClipBoard, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxClipBoardContext);


	return ( ClipBoard );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_ClipBoard()
{
	Widget                  rtrn;
	_UxCClipBoard           *UxContext;

	UxClipBoardContext = UxContext =
		(_UxCClipBoard *) UxNewContext( sizeof(_UxCClipBoard), False );


	{
		CreaClipboardList();
		rtrn = _Uxbuild_ClipBoard();

		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

