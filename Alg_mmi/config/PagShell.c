
/*******************************************************************************
       PagShell.c
       (Generated from interface file PagShell.i)
*******************************************************************************/

#include <stdio.h>

#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>
#include <Xl/XlIconReg.h>

//#include "XlPrint.h"
//#include "other.h"

Widget XlPrintSetup(Widget);
Widget PrintSetup(Widget);



/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo PagShell.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)PagShell.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>

#include <draw.h>
#include <Xd/Xd.h>
#include <Xd/XdLista.h>
#include <Xd/XdUndo.h>
#include <Ol/OlCompiler.h>
#include <Xl/XlDispReg.h>
#include <Xl/XlPort.h>
#include "libutilx.h"
#include "message.h"
#include "config.h"

PAGINA *pagine = NULL; /* lista delle pagine */

int NUM_ELENCHI;

char **elenco_nomi;
char **elenco_classi;


extern OlDatabaseTopologiaObject dbtopologia;
extern Boolean CompilerOk;
extern void get_child();
extern Boolean PrintPag();

extern int flag_demo;

int PrintClosePag();
void close_page();
extern void SetItemString();
extern void EliminaDraget(PAGINA *pag);

//extern  void *		UxNewContext();


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
	Widget	UxPagShell;
	Widget	UxPagMainWindow;
	Widget	UxPagForm1;
	Widget	UxPagScrolledWindow;
	Widget	UxPagDrawingArea;
	Widget	Uxmenu3;
	Widget	Uxmenu3_p1_title;
	Widget	Uxmenu3separ1;
	Widget	Uxmenu3Edit;
	Widget	Uxmenu3separ0;
	Widget	Uxmenu3Refresh;
	Widget	Uxmenu3Undo;
	Widget	Uxmenu3DuplicateAll;
	Widget	Uxmenu3DeleteAll;
	Widget	Uxmenu3CutAll;
	Widget	Uxmenu3CopyAll;
	Widget	Uxmenu3PasteAll;
	Widget	Uxmenu3separ;
	Widget	Uxmenu3Foreground;
	Widget	Uxmenu3FCut;
	Widget	Uxmenu3FCopy;
	Widget	Uxmenu3FPaste;
	Widget	Uxmenu3PForeground;
	Widget	Uxmenu3Background;
	Widget	Uxmenu3BCut;
	Widget	Uxmenu3BCopy;
	Widget	Uxmenu3BPaste;
	Widget	Uxmenu3BPutBottom;
	Widget	Uxmenu3BPutTop;
	Widget	Uxmenu3BModifyObject;
	Widget	Uxmenu3PBackground;
	Widget	UxgrePagPane;
	Widget	UxFileTmp;
	Widget	UxSaveTmp;
	Widget	UxFileSep;
	Widget	UxStampa;
	Widget	UxStampa_Setup;
	Widget	UxFileTmp_b6;
	Widget	UxCloseTmp;
	Widget	Uxmenu4_top_b1;
	Widget	UxAddObj;
	Widget	UxIcon;
	Widget	UxPagPane_top_b1;
	Widget	UxFEdit;
	Widget	UxFUndo;
	Widget	UxFDuplicateAll;
	Widget	UxFDeleteAll;
	Widget	UxFCutAll;
	Widget	UxFCopyAll;
	Widget	UxFPasteAll;
	Widget	UxFsepar0;
	Widget	UxEditForeground;
	Widget	UxFgCut;
	Widget	UxFgCopy;
	Widget	UxFgPaste;
	Widget	UxFEditForeground;
	Widget	UxEditBackground;
	Widget	UxBgCut;
	Widget	UxBgCopy;
	Widget	UxBgPaste;
	Widget	UxBgPutBottom;
	Widget	UxBgPutTop;
	Widget	UxraggruppaButton;
	Widget	UxunraggruppaButton;
	Widget	UxFEditBackground;
	Widget	UxPagPane_top_b2;
	Widget	UxConfigure;
	Widget	UxEdit;
	Widget	UxPagPane_top_b3;
	Widget	UxDrawBackground;
	Widget	UxFDraw;
	Widget	UxPagPane_top_b6;
	Widget	UxCompile;
	Widget	UxCompilePag;
	Widget	UxCompileReg;
	Widget	UxCompile_b5;
	Widget	UxViewError;
	Widget	UxCompile_b4;
	Widget	UxPagPane_top_b4;
	Widget	UxOption;
	Widget	UxGrid;
	Widget	UxWriteRes;
	Widget	UxZoPane;
	Widget	UxZoom100TB;
	Widget	UxZoom200TB;
	Widget	UxZoom300TB;
	Widget	UxZoom400TB;
	Widget	UxSetZ;
	Widget	Uxb_SettaSnapDraw;
	Widget	UxConnectMode;
	Widget	UxSetVisualMode;
	Widget	UxRefreshDrawing;
	Widget	UxPagPane_top_b5;
	PAGINA	*Uxactual_page;
	WidgetClass	Uxistanzia_widget_class;
	swidget	*Uxelenco_editor;
	swidget	Uxwset_snap;
	XdLista	Uxlista_draget;
	XdLista	Uxlista_draget_undo;
	XdListaUndo	Uxlista_liste_undo;
	ST_DRAW_WID	Uxst_draw;
	Boolean	Uxediting_background;
	swidget	Uxdraw_edit;
	unsigned char	*Uxnome_pagina;
	int	Uxindice;
	int	Uxtipop;
} _UxCPagShell;

static _UxCPagShell            *UxPagShellContext;
#define PagShell                UxPagShellContext->UxPagShell
#define PagMainWindow           UxPagShellContext->UxPagMainWindow
#define PagForm1                UxPagShellContext->UxPagForm1
#define PagScrolledWindow       UxPagShellContext->UxPagScrolledWindow
#define PagDrawingArea          UxPagShellContext->UxPagDrawingArea
#define menu3                   UxPagShellContext->Uxmenu3
#define menu3_p1_title          UxPagShellContext->Uxmenu3_p1_title
#define menu3separ1             UxPagShellContext->Uxmenu3separ1
#define menu3Edit               UxPagShellContext->Uxmenu3Edit
#define menu3separ0             UxPagShellContext->Uxmenu3separ0
#define menu3Refresh            UxPagShellContext->Uxmenu3Refresh
#define menu3Undo               UxPagShellContext->Uxmenu3Undo
#define menu3DuplicateAll       UxPagShellContext->Uxmenu3DuplicateAll
#define menu3DeleteAll          UxPagShellContext->Uxmenu3DeleteAll
#define menu3CutAll             UxPagShellContext->Uxmenu3CutAll
#define menu3CopyAll            UxPagShellContext->Uxmenu3CopyAll
#define menu3PasteAll           UxPagShellContext->Uxmenu3PasteAll
#define menu3separ              UxPagShellContext->Uxmenu3separ
#define menu3Foreground         UxPagShellContext->Uxmenu3Foreground
#define menu3FCut               UxPagShellContext->Uxmenu3FCut
#define menu3FCopy              UxPagShellContext->Uxmenu3FCopy
#define menu3FPaste             UxPagShellContext->Uxmenu3FPaste
#define menu3PForeground        UxPagShellContext->Uxmenu3PForeground
#define menu3Background         UxPagShellContext->Uxmenu3Background
#define menu3BCut               UxPagShellContext->Uxmenu3BCut
#define menu3BCopy              UxPagShellContext->Uxmenu3BCopy
#define menu3BPaste             UxPagShellContext->Uxmenu3BPaste
#define menu3BPutBottom         UxPagShellContext->Uxmenu3BPutBottom
#define menu3BPutTop            UxPagShellContext->Uxmenu3BPutTop
#define menu3BModifyObject      UxPagShellContext->Uxmenu3BModifyObject
#define menu3PBackground        UxPagShellContext->Uxmenu3PBackground
#define grePagPane              UxPagShellContext->UxgrePagPane
#define FileTmp                 UxPagShellContext->UxFileTmp
#define SaveTmp                 UxPagShellContext->UxSaveTmp
#define FileSep                 UxPagShellContext->UxFileSep
#define Stampa                  UxPagShellContext->UxStampa
#define Stampa_Setup            UxPagShellContext->UxStampa_Setup
#define FileTmp_b6              UxPagShellContext->UxFileTmp_b6
#define CloseTmp                UxPagShellContext->UxCloseTmp
#define menu4_top_b1            UxPagShellContext->Uxmenu4_top_b1
#define AddObj                  UxPagShellContext->UxAddObj
#define Icon                    UxPagShellContext->UxIcon
#define PagPane_top_b1          UxPagShellContext->UxPagPane_top_b1
#define FEdit                   UxPagShellContext->UxFEdit
#define FUndo                   UxPagShellContext->UxFUndo
#define FDuplicateAll           UxPagShellContext->UxFDuplicateAll
#define FDeleteAll              UxPagShellContext->UxFDeleteAll
#define FCutAll                 UxPagShellContext->UxFCutAll
#define FCopyAll                UxPagShellContext->UxFCopyAll
#define FPasteAll               UxPagShellContext->UxFPasteAll
#define Fsepar0                 UxPagShellContext->UxFsepar0
#define EditForeground          UxPagShellContext->UxEditForeground
#define FgCut                   UxPagShellContext->UxFgCut
#define FgCopy                  UxPagShellContext->UxFgCopy
#define FgPaste                 UxPagShellContext->UxFgPaste
#define FEditForeground         UxPagShellContext->UxFEditForeground
#define EditBackground          UxPagShellContext->UxEditBackground
#define BgCut                   UxPagShellContext->UxBgCut
#define BgCopy                  UxPagShellContext->UxBgCopy
#define BgPaste                 UxPagShellContext->UxBgPaste
#define BgPutBottom             UxPagShellContext->UxBgPutBottom
#define BgPutTop                UxPagShellContext->UxBgPutTop
#define raggruppaButton         UxPagShellContext->UxraggruppaButton
#define unraggruppaButton       UxPagShellContext->UxunraggruppaButton
#define FEditBackground         UxPagShellContext->UxFEditBackground
#define PagPane_top_b2          UxPagShellContext->UxPagPane_top_b2
#define Configure               UxPagShellContext->UxConfigure
#define Edit                    UxPagShellContext->UxEdit
#define PagPane_top_b3          UxPagShellContext->UxPagPane_top_b3
#define DrawBackground          UxPagShellContext->UxDrawBackground
#define FDraw                   UxPagShellContext->UxFDraw
#define PagPane_top_b6          UxPagShellContext->UxPagPane_top_b6
#define Compile                 UxPagShellContext->UxCompile
#define CompilePag              UxPagShellContext->UxCompilePag
#define CompileReg              UxPagShellContext->UxCompileReg
#define Compile_b5              UxPagShellContext->UxCompile_b5
#define ViewError               UxPagShellContext->UxViewError
#define Compile_b4              UxPagShellContext->UxCompile_b4
#define PagPane_top_b4          UxPagShellContext->UxPagPane_top_b4
#define Option                  UxPagShellContext->UxOption
#define Grid                    UxPagShellContext->UxGrid
#define WriteRes                UxPagShellContext->UxWriteRes
#define ZoPane                  UxPagShellContext->UxZoPane
#define Zoom100TB               UxPagShellContext->UxZoom100TB
#define Zoom200TB               UxPagShellContext->UxZoom200TB
#define Zoom300TB               UxPagShellContext->UxZoom300TB
#define Zoom400TB               UxPagShellContext->UxZoom400TB
#define SetZ                    UxPagShellContext->UxSetZ
#define b_SettaSnapDraw         UxPagShellContext->Uxb_SettaSnapDraw
#define ConnectMode             UxPagShellContext->UxConnectMode
#define SetVisualMode           UxPagShellContext->UxSetVisualMode
#define RefreshDrawing          UxPagShellContext->UxRefreshDrawing
#define PagPane_top_b5          UxPagShellContext->UxPagPane_top_b5
#define actual_page             UxPagShellContext->Uxactual_page
#define istanzia_widget_class   UxPagShellContext->Uxistanzia_widget_class
#define elenco_editor           UxPagShellContext->Uxelenco_editor
#define wset_snap               UxPagShellContext->Uxwset_snap
#define lista_draget            UxPagShellContext->Uxlista_draget
#define lista_draget_undo       UxPagShellContext->Uxlista_draget_undo
#define lista_liste_undo        UxPagShellContext->Uxlista_liste_undo
#define st_draw                 UxPagShellContext->Uxst_draw
#define editing_background      UxPagShellContext->Uxediting_background
#define draw_edit               UxPagShellContext->Uxdraw_edit
#define nome_pagina             UxPagShellContext->Uxnome_pagina
#define indice                  UxPagShellContext->Uxindice
#define tipop                   UxPagShellContext->Uxtipop


/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxPagShellMenuPost( wgt, client_data, event )
	Widget		wgt;
	XtPointer	client_data;
	XEvent		*event;

{
	Widget	menu = (Widget) client_data;
	int 	which_button;

	XtVaGetValues( menu, XmNwhichButton, &which_button, NULL );

	if ( event->xbutton.button == which_button )
	{
		XmMenuPosition( menu, (XButtonPressedEvent *) event );
		XtManageChild( menu );
	}
}


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_PagShell();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*--------------------------------------------------------------------
 *
 * iconlib_buttCB
 *
 * Description:
 *               chiama la crea finestra delle icone di libreria
 *               callback agganciata ai bottoni iconlib sulla pagia
 *
 * Parameter:
 *               Widget w
 *               int ind_butt
 *               XmAnyCallbackStruct *call_data
 *
 *-------------------------------------------------------------------*/

void iconlib_buttCB(Widget w,int ind_butt, XmAnyCallbackStruct *call_data)
{
#ifndef DESIGN_TIME
   _UxCPagShell      *UxSaveCtx, *UxContext;
   swidget UxThisWidget;
#endif 

   extern PAGEDIT_CONTEXT *pagedit;
   extern swidget create_IconShell();
   swidget wid;
   char nomelib[100];

#ifndef DESIGN_TIME
   UxSaveCtx = UxPagShellContext;
   UxThisWidget = w;
   UxSaveCtx = UxPagShellContext;
   UxPagShellContext = UxContext = (_UxCPagShell *) UxGetContext( UxThisWidget  );
#endif


   strcpy(nomelib,pagedit->iconlib_list[ind_butt]);
   wid = create_IconShell( nomelib );
   UxPopupInterface(wid,no_grab);


#ifndef DESIGN_TIME
   UxPagShellContext = UxSaveCtx;
#endif

}


/*--------------------------------------------------------------------
 *
 * add_objlib_button
 *
 * Description:
 *               aggiunge bottoni di attivazione iconlib su
 *               una pagina e ne definisce anche la callback
 *
 * Parameter:
 *               swidget wid
 *               
 *-------------------------------------------------------------------*/

void add_objlib_button(swidget wid)
{
   extern PAGEDIT_CONTEXT *pagedit;
   swidget *iconlibButt;
   int i,j,num_butt;
   extern Arg args[];
   extern Cardinal lnargs;

   
   XtDestroyWidget(Icon);

   num_butt = pagedit->iconlib_num;
   if((iconlibButt = (swidget *)alloca_memoria( num_butt, sizeof(swidget) )) == NULL)
      return;

   for(i=0;i<num_butt;i++)
   {
      
      lnargs = 0;
      XtSetArg(args[lnargs],XmNsensitive, True); lnargs++;
      iconlibButt[i] = XmCreatePushButton (AddObj,pagedit->iconlib_label[i],args, lnargs);

#ifndef DESIGN_TIME
   UxPutContext (iconlibButt[i] ,(char *)UxPagShellContext);
#endif      
      XtAddCallback(iconlibButt[i], XmNactivateCallback, (XtCallbackProc)iconlib_buttCB,(XtPointer)i);

   }
   XtManageChildren(iconlibButt, num_butt);

   libera_memoria((char*)iconlibButt);

}



/*--------------------------------------------------------------------
 *
 * assign_name
 *
 * Description:
 *               assegna un nome alla nuova istanza di un oggetto
 *               - attualmente sviluppata in modo semplicistico -
 *
 * Parameter:
 *               PAGINA *pag;
 *               WidgetClass wcl
 *               char *nuovo_nome;
 *
 *-------------------------------------------------------------------*/
void assign_name(PAGINA *pag,int dove,Widget parent,char *nuovo_nome)
{
   Cardinal num_child;
   char *name_parent;
   char nome_figlio[100];
 
   if(dove == SUDRAWING)
   {
      sprintf(nuovo_nome,"%d",pag->next_num);
      strcat(nuovo_nome,"w");
   }
   else
   {
      name_parent = XtName(parent);
      strcpy(nuovo_nome,name_parent);       
      sprintf(nome_figlio,"%d",pag->next_num);
      strcat(nuovo_nome,nome_figlio);
      strcat(nuovo_nome,"c");
   }
   pag->next_num++;
}

/*-----------------------------------
 * salva_indice_editor
 *
 *-----------------------------------*/ 

void salva_indice_editor(swidget edit)
{
   int num_editor;
   int i;

   num_editor = 1;
  
   i=0;
   if(elenco_editor != NULL)
   {
      while(  elenco_editor[i] != NULL)
      {
         i++;
         num_editor++;
      }

      elenco_editor = (swidget *)XtRealloc(elenco_editor, sizeof(swidget)* (num_editor+1));
      elenco_editor[num_editor-1] = edit; 
      elenco_editor[num_editor] = NULL;
   }
}

/*-------------------------------
 *
 * get_lista
 * ricava la lista dei draget che descrivono il background 
 * della pagina.
 *--------------------------------*/

XdLista get_lista(Widget w)
{
#ifndef DESIGN_TIME
   XdLista ret_lista;
   _UxCPagShell     *UxSaveCtx, *UxContext;

    UxSaveCtx =  UxPagShellContext ;
    UxPagShellContext = UxContext =
               (_UxCPagShell *) UxGetContext( w );

   ret_lista= lista_draget;
 
   UxPagShellContext  = UxSaveCtx;

   return(ret_lista);
#endif
}

XdLista get_lista_undo(Widget w)
{
#ifndef DESIGN_TIME
   XdLista ret_lista;
   _UxCPagShell     *UxSaveCtx, *UxContext;

    UxSaveCtx =  UxPagShellContext ;
    UxPagShellContext = UxContext =
               (_UxCPagShell *) UxGetContext( w );

   ret_lista= lista_draget_undo;
 
   UxPagShellContext  = UxSaveCtx;

   return(ret_lista);
#endif
}


void CreaUndoList(w)
Widget w;
{
#ifndef DESIGN_TIME
        _UxCPagShell   *UxSaveCtx, *UxContext;

        UxSaveCtx = UxPagShellContext;
        UxPagShellContext = UxContext =
                        (_UxCPagShell *) UxGetContext( w );
#endif
if(lista_draget_undo != NULL)
        XtFree(lista_draget_undo);

lista_draget_undo=XdCreateLista();
#ifndef DESIGN_TIME
 UxPagShellContext = UxSaveCtx;
#endif
}

XdListaUndo get_lista_liste_undo(Widget w)
{
#ifndef DESIGN_TIME
   XdListaUndo ret_lista;
   _UxCPagShell     *UxSaveCtx, *UxContext;

    UxSaveCtx =  UxPagShellContext ;
    UxPagShellContext = UxContext =
               (_UxCPagShell *) UxGetContext( w );

   ret_lista= lista_liste_undo;
 
   UxPagShellContext  = UxSaveCtx;

   return(ret_lista);
#endif
}


/*
 restituisce o setta una risorsa tra quelle definite
 nella struttura descrittiva delle risorse per il widget di disegno
 (ST_DRAW_WID) definita in draw.h (in legommi/include)
*/
int gs_draw_ris(Widget w,enum ris_draw_wid ris_num, void *ret,Boolean set)
{
int *retint;
GC *retGC;
Widget *retWid;
float *retfloat;
Region *retreg;

#ifndef DESIGN_TIME
        _UxCPagShell   *UxSaveCtx, *UxContext;
        UxSaveCtx = UxPagShellContext;
        UxPagShellContext = UxContext =
                        (_UxCPagShell *) UxGetContext( w );
#endif
switch(ris_num)
        {
	case DR_REGION:
	retreg=ret;
	if(set)
                st_draw.region=*retreg;
        else
                *retreg=st_draw.region;
        break;
        case DR_LINE_STYLE:
        retint=ret;
        if(set)
                st_draw.line_style=*retint;
        else
                *retint=st_draw.line_style;
        break;
        case DR_LINE_WIDTH:
        retint=ret;
        if(set)
                st_draw.line_width=*retint;
        else
                *retint=st_draw.line_width;
        break;
        case DR_DEF_LINE_STYLE:
        retint=ret;
        if(set)
                st_draw.def_line_style=*retint;
        else
                *retint=st_draw.def_line_style;
        break;
        case DR_DEF_LINE_WIDTH:
        retint=ret;
        if(set)
                st_draw.def_line_width=*retint;
        else
                *retint=st_draw.def_line_width;
        break;
        case DR_GC:
        retGC=ret;
        if(set)
                st_draw.gc=*retGC;
        else
                *retGC= st_draw.gc;
        break;
        case DR_GC_BG:
        retGC=ret;
        if(set)
                st_draw.gc_bg=*retGC;
        else
                *retGC= st_draw.gc_bg;
        break;
        case DR_DEF_GC:
        retGC=ret;
        if(set)
                st_draw.def_gc=*retGC;
        else
                *retGC= st_draw.def_gc;
        break;
        case DR_DEF_GC_BG:
        retGC=ret;
        if(set)
                st_draw.def_gc_bg=*retGC;
        else
                *retGC= st_draw.def_gc_bg;
        break;
        case DR_STEP:
        retint=ret;
        if(set)
                st_draw.step= *retint;
        else
                *retint=st_draw.step;
        break;
        case DR_GRID_ON:
        retint=ret;
        if(set)
                st_draw.grid_on= *retint;
        else
                *retint=st_draw.grid_on;
        break;
        case DR_DEF_FILLED:
        retint=ret;
        if(set)
                st_draw.def_filled= *retint;
        else
                *retint=st_draw.def_filled;
        break;
        case DR_DEF_ZOOM:
        retfloat=ret;
        if(set)
                st_draw.def_zoom=*retfloat;
        else
                *retfloat=st_draw.def_zoom;
        break;
        case DR_BOTT_FILL:
        retWid=ret;
        if(set)
                st_draw.bott_fill= *retWid;
        else
                *retWid= st_draw.bott_fill;
        break;
        case DR_BOTT_BORDER:
        retWid=ret;
        if(set)
                st_draw.bott_border= *retWid;
        else
                *retWid= st_draw.bott_border;
        break; 
        case DR_DEF_BOTT_FILL:
        retWid=ret;
        if(set)
                st_draw.def_bott_fill= *retWid;
        else
                *retWid= st_draw.def_bott_fill;
        break;
        case DR_DEF_BOTT_BORDER:
        retWid=ret;
        if(set)
                st_draw.def_bott_border= *retWid;
        else
                *retWid= st_draw.def_bott_border;
        break;
        case DR_BOTT_ARROW:
        retWid=ret;
        if(set)
                st_draw.bott_arrow= *retWid;
        else
                *retWid= st_draw.bott_arrow;
        break;
        }
#ifndef DESIGN_TIME
 UxPagShellContext = UxSaveCtx;
#endif
return((int)ret);
}



/*
 reset_drawing_background 
 setta a False il flag che indica l'utilizzo in corso
 del Draw per disegnare il background
*/
void reset_drawing_background(Widget w)
{
#ifndef DESIGN_TIME

   _UxCPagShell     *UxSaveCtx, *UxContext;

    UxSaveCtx =  UxPagShellContext ;
    UxPagShellContext = UxContext =
               (_UxCPagShell *) UxGetContext( w );

   editing_background = False;
   draw_edit=NULL;
   UxPagShellContext  = UxSaveCtx;
   
   
#endif
}

/*------------------------------------------------------
 Ritorna l'informazione se il background e' in editing
--------------------------------------------------------*/
Boolean is_drawing_background(Widget w)
{
#ifndef DESIGN_TIME

   _UxCPagShell     *UxSaveCtx, *UxContext;

    UxSaveCtx =  UxPagShellContext ;
    UxPagShellContext = UxContext =
               (_UxCPagShell *) UxGetContext( w );

   return(editing_background);
 
   UxPagShellContext  = UxSaveCtx;

   
#endif
}


void close_page(char *pagname)
{
   extern void abilitaMoveMenu();
   extern int Already_Selected;
   extern Widget PagOpen[];
   extern int PagOpenId[];
   extern unsigned short npagopen; 
   PAGINA *pappo=NULL,*prec=NULL;
   extern XrmDatabase defdb;
   int i;
   Widget shell;

/* cerco la pagina da chiudere tremite il nome */
   pappo = pagine;
   while( strcmp(pappo->nomepag,pagname) )
   {
      prec = pappo;
      pappo = pappo->pagsucc;
   }



   UxDisplay->db = defdb;

   shell = pappo->topwidget;

/* ho trovato la pagina e aggiorno le liste delle pagine */

   for(i=0;i<npagopen;i++)
   {
      if( PagOpen[i] == pappo->topwidget)
      {
         PagOpen[i] = NULL;
         PagOpenId[i]= 100;
      }
   }
   npagopen--; 

   if(pappo == pagine)
      pagine = pappo->pagsucc;
   else
      prec->pagsucc = pappo->pagsucc;  

   EliminaDraget(pappo);
   

/* aggiorno il flag di ??? */
   Already_Selected = False;    


/* set item attivi menu' della main interface */  
   abilitaMoveMenu();


   UxDestroyInterface(shell);
pagina_free(pappo);
}

void SetItemString(XmString *Xstring,char *nome,char *tipo,char *descr,int in_use,
	char *tag)
{
   char *Cstring;
   int len;
   
   len = strlen(nome)+strlen(tipo)+strlen(descr)+40;
   Cstring = XtCalloc(len,sizeof(char));
   if(strcmp(tipo,TIPO_REGOLAZIONE)==0)
	{
	if(in_use == SCHEME_IN_USE)
   	   sprintf(Cstring,"%s    - tipo %s [in use][%s]- %s ",
			nome,tipo,tag,descr);
	else
	   sprintf(Cstring,"%s    - tipo %s [not in use][%s]- %s ",
		nome,tipo,tag,descr);
	}
   else
	sprintf(Cstring,"%s    - tipo %s - %s ",nome,tipo,descr);
   *Xstring = (XmString )XmStringCreateSimple( Cstring );
   XtFree(Cstring);  
}

void PageMenuSetInterfaceModeOn()
{
   set_something(SaveTmp,XmNsensitive,(void*) False);
   set_something(ConnectMode,XmNsensitive,(void*) False);
   set_connect(actual_page->drawing,(int)1);
}

void PageMenuSetInterfaceModeOff()
{
   set_something(SaveTmp,XmNsensitive,(void*) True);
}

void SetButtonClosePage(Boolean Stato)
{
   if( (Stato == False) || (Stato == True))
      set_something(CloseTmp,XmNsensitive,(void*) Stato);
}

Boolean PagGetConnectMode(Widget w)
{

#ifndef DESIGN_TIME
  Boolean Stato= False;

  _UxCPagShell     *UxSaveCtx, *UxContext;

   UxSaveCtx =  UxPagShellContext ;
   UxPagShellContext = UxContext =
              (_UxCPagShell *) UxGetContext( w );

   Stato = XmToggleButtonGetState(ConnectMode);
 
   UxPagShellContext  = UxSaveCtx;

   return(Stato);  
#endif
}

Widget gs_WSetSnap(Widget w,Widget wsnap,Boolean set)
{
Widget wret=NULL;
#ifndef DESIGN_TIME


  _UxCPagShell     *UxSaveCtx, *UxContext;

   UxSaveCtx =  UxPagShellContext ;
   UxPagShellContext = UxContext =
              (_UxCPagShell *) UxGetContext( w );

   if(set)
	wset_snap=wsnap;
   else
	wret=wset_snap;
 
   UxPagShellContext  = UxSaveCtx;

   return(wret);  
#endif
}

void RefreshDrawingArea(Widget cw)
{
int i,j;
Cardinal nchild,nnipoti;
WidgetList child,nipoti;


XClearArea(XtDisplay(cw),XtWindow(cw),
		0,0,0,0,True);
get_child(cw,&child,&nchild);
for(i=0;i<nchild;i++)
	{
	XClearArea(XtDisplay(child[i]),XtWindow(child[i]),0,0,0,0,True);
	if(XlIsXlComposite(child[i]))
		{
		get_child(child[i],&nipoti,&nnipoti);
		for(j=0;j<nnipoti;j++)
		   XClearArea(XtDisplay(nipoti[j]),XtWindow(nipoti[j]),
			0,0,0,0,True);
		}		
	}
}

int PrintClosePag( unsigned char *nome_pag )
{
   PrintPag( actual_page->drawing );

   close_page( nome_pag );
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	popupCB_PagShell( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	printf("Popup Callback Is Called drawing = %d\n",PagDrawingArea);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	createCB_menu3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxContext = UxPagShellContext;
	{
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3Edit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   extern swidget create_Resource();
	   swidget redit;
	   Widget widsel,reswid,edchild;
	   WidgetList child;
	   Cardinal nchild;
	   int i;
	
#ifndef DESIGN_TIME
	   widsel = XlGetSelectedWidget( actual_page->drawing);
	   if(widsel)
	   {
	
	      if( XlIsXlComposite(widsel) )
	      {
	         get_child(widsel,&child,&nchild);
	         for(i=0;i<nchild;i++)
	         {
	            get_something(child[i],XlNwEdit, (void*) &edchild);
	            if(edchild != NULL)
	            {
	               printf("Cannot open Editor when Children are in Edit\n");
	               SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",
	                      "Selected Widget Children Is In Edit, Can't Open Editor",
	                       NULL,False,NULL,False,NULL);
	               return;
	            }
	         }   
	      }
	      else if( XlIsXlComposite(XtParent(widsel)) )
	      {
	         get_something( XtParent(widsel),XlNwEdit, (void*) &edchild);
	         if(edchild != NULL)
	         {
	            printf("Cannot open Editor when Parent is in Edit\n");
	            SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",
	                   "Selected Widget Parent Is In Edit, Can't Open Editor",
	                    NULL,False,NULL,False,NULL);
	           return;
	         }      
	      }
	       
	
	      get_something(widsel,XlNwEdit, (void*) &reswid);
	
	
	      if(reswid == NULL)
	      {
	         redit = create_Resource(actual_page,dbtopologia);
	         salva_indice_editor(redit);
	         UxPopupInterface(redit,no_grab);
	      }
	      else
	      { 
	         XRaiseWindow(XtDisplay(reswid),XtWindow(reswid));
	      }
	   }
	
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3Refresh( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	RefreshDrawingArea(actual_page->drawing);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3Undo( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	extern int undo_draget_paste();
	
	undo_draget_paste( actual_page->drawing);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3DuplicateAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   int i;
	   float f_zoom;
	   extern float get_def_zoom();   
	   extern int draget_duplicate();
	   extern int copy_new();
	   WidgetList wsel;
	   Cardinal nwsel;
	
	   int dx,dy,Step;
	/**
	 Se vi sono in selezione oggetti icon reg
	 annulla l'operazione di duplicazione
	 (la duplicazione non gestisce gli schemi
	  di regolazione)
	**/
	   if( get_selected( actual_page,&wsel,&nwsel) == True)
		{
		for(i=0;i<nwsel;i++)
			{
	/*
	 Se la pagina non e' di libreria impedisce il duplicate di porte o
	 display di regolazione
	*/
			if( XlIsIconReg(wsel[i]) ||
			    (PagGetType(actual_page) != TYPE_LIBRERIA &&
			    (XlIsPort(wsel[i]) || XlIsDispReg(wsel[i]))))
				{
				libera_memoria((char*)wsel);
				return;
				}
	
			}
		libera_memoria((char*)wsel);
		}
	   f_zoom= get_def_zoom(actual_page->drawing);
	   Step=get_step(actual_page->drawing);
	   Step = Step * f_zoom;
		
	   if(Step == 1)
	   {
	      dx = DELTAX;
	      dy = DELTAY;
	   }
	   else
	   {
	      dx = Step;
	      dy = Step;
	   }
	   draget_duplicate(actual_page->drawing,dx,dy);
	   copy_new(actual_page);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3DeleteAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	
	   extern int delete_widget();
	   extern int DrawDelete();
	
	   if(WidSelectedAreInEdit(actual_page) == False)
	   {
	      delete_widget(actual_page);
	      DrawDelete(actual_page->drawing,NULL);
	   }
	   else
	      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget Is In Edit",NULL,False,NULL,False,NULL);
	  
	 
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3CutAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_cut();
	
	   extern int select_connections();
	   extern Widget ClipBoardForCopy;
	   extern int WriteBackground();
	   WidgetList wsel;
	   Cardinal nwsel; 
	   extern int fcut();
	
	
	   /* verifico che gli widget non siano in edit */
	   if(WidSelectedAreInEdit(actual_page) == False)
	   {
	      if( get_selected( actual_page,&wsel,&nwsel) == True)
	         select_connections(nwsel,wsel,NULL,WCOPY);
	      fcut(actual_page);
	      draget_cut(actual_page->drawing,ClipBoardForCopy);
	
	      if(nwsel)
	         delete_widget(actual_page);
	   }
	   else
	      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget is In Edit",NULL,False,NULL,False,NULL);
	
	   
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3CopyAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcopy();
	   extern int draget_copy();
	   extern int select_connections(); 
	   extern Widget ClipBoardForCopy;
	   WidgetList wsel;
	   Cardinal nwsel;
	
	
	
	   if( get_selected( actual_page,&wsel,&nwsel) == True)
	      select_connections(nwsel,wsel,NULL,WCOPY);
	   fcopy(actual_page);
	   draget_copy(actual_page->drawing,ClipBoardForCopy);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3PasteAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int draget_paste();
	  extern int fpaste();
	
	  fpaste(actual_page);
	  draget_paste(actual_page->drawing); 
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3FCut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcut();
	
	   fcut(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3FCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcopy();
	
	   fcopy(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3FPaste( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int fpaste();
	
	  fpaste(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BCut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_cut();
	   extern Widget ClipBoardForCopy;
	
	   draget_cut(actual_page->drawing,ClipBoardForCopy);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_copy();
	   extern Widget ClipBoardForCopy;
	
	   draget_copy(actual_page->drawing,ClipBoardForCopy);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BPaste( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int draget_paste();
	
	  draget_paste(actual_page->drawing); 
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BPutBottom( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int DrawPutBottom();
	
	   DrawPutBottom(actual_page->drawing);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BPutTop( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int DrawPutTop();
	
	   DrawPutTop(actual_page->drawing);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_menu3BModifyObject( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   printf("Funzione non abilitata\n");
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_SaveTmp( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   extern PAGEDIT_CONTEXT *pagedit;
	   extern Boolean pagina_save();
	   extern int WriteBackground();
	   char fileback[MAXLENFILENAME];
	
	   extern Boolean CompressData();
	
	   CompressData(actual_page);
	
	
#ifndef DESIGN_TIME
	
	      if( (!strcmp(actual_page->geom.tipo,TIPO_SINOTTICO)) ||
	          (!strcmp(actual_page->geom.tipo,TIPO_STAZIONI))  ||
	          (!strcmp(actual_page->geom.tipo,TIPO_REGOLAZIONE)) ||
	          (!strcmp(actual_page->geom.tipo,TIPO_TELEPERM)) )
	      {
	         if( (pagina_save(actual_page,TIPO_PAGINA,1)) != False)
	         { 
	            strcpy(fileback,pagedit->pages);
	            strcat(fileback,"/"); 
	            strcat(fileback,actual_page->filebkg);
	            strcpy(fileback,XlConvPathVms(fileback));
	            WriteBackground(actual_page->drawing,fileback);
	         }
	      }
	      else if( !strcmp(actual_page->geom.tipo,TIPO_LIBRERIA) )
	      {
	         pagina_save(actual_page,TIPO_LIBRARY,1);
	      }
	   
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_Stampa( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifdef XPRINTER_USED
	char str[200];
	PrintPag(actual_page->drawing);
	sprintf(str,"Printing Page %s",actual_page->nomepag);
	SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo",str,NULL,
		False,NULL,False,NULL); 
#else
	SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo","Print not allowed for current version",NULL,
		False,NULL,False,NULL);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_Stampa_Setup( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifdef XPRINTER_USED
#ifndef DESIGN_TIME
	PrintSetup(actual_page->drawing);
#endif
#else
	SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo","Printer use not allowed for this version"
		 ,NULL,False,NULL,False,NULL);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_CloseTmp( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int gest_conf_op();
	
	   gest_conf_op(CLOSE_PAGE,"Do you really want close %s page ?",actual_page->nomepag);
	
#endif
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FUndo( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	
	extern int undo_draget_paste();
	
	undo_draget_paste( actual_page->drawing );
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FDuplicateAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   int i;
	   float f_zoom;
	   extern float get_def_zoom();   
	   extern int draget_duplicate();
	   extern int copy_new();
	   WidgetList wsel;
	   Cardinal nwsel;
	
	   int dx,dy,Step;
	/**
	 Se vi sono in selezione oggetti icon reg
	 annulla l'operazione di duplicazione
	 (la duplicazione non gestisce gli schemi
	  di regolazione)
	**/
	   if( get_selected( actual_page,&wsel,&nwsel) == True)
		{
		for(i=0;i<nwsel;i++)
			{
	/*
	 Se la pagina non e' di libreria impedisce il duplicate di porte o
	 display di regolazione
	*/
			if( XlIsIconReg(wsel[i]) ||
			    (PagGetType(actual_page) != TYPE_LIBRERIA &&
			    (XlIsPort(wsel[i]) || XlIsDispReg(wsel[i]))))
				{
				libera_memoria((char*)wsel);
				return;
				}
	
			}
		libera_memoria((char*)wsel);
		}
	   f_zoom= get_def_zoom(actual_page->drawing);
	   Step=get_step(actual_page->drawing);
	   Step = Step * f_zoom;
		
	   if(Step == 1)
	   {
	      dx = DELTAX;
	      dy = DELTAY;
	   }
	   else
	   {
	      dx = Step;
	      dy = Step;
	   }
	   draget_duplicate(actual_page->drawing,dx,dy);
	   copy_new(actual_page);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FDeleteAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	
	   extern int delete_widget();
	   extern int DrawDelete();
	
	   if(WidSelectedAreInEdit(actual_page) == False)
	   {
	      delete_widget(actual_page);
	      DrawDelete(actual_page->drawing,NULL);
	   }
	   else
	      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget Is In Edit",NULL,False,NULL,False,NULL);
	      
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FCutAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_cut();
	
	   extern int fcut();
	   extern int select_connections();
	   extern Widget ClipBoardForCopy;
	   extern int WriteBackground();
	   WidgetList wsel;
	   Cardinal nwsel;
	   
	   /* verifico che gli widget non siano in edit */
	   if(WidSelectedAreInEdit(actual_page) == False)
	   {
	
	      if( get_selected( actual_page,&wsel,&nwsel) == True) 
	         select_connections(nwsel,wsel,NULL,WCOPY);
	      fcut(actual_page);
	      draget_cut(actual_page->drawing,ClipBoardForCopy);
	/*      WriteBackground(ClipBoard,"ClipDb.bkg"); */
	      if(nwsel)
	         delete_widget(actual_page);
	  }
	   else
	      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget is In Edit",NULL,False,NULL,False,NULL);
	
	   
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FCopyAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcopy();
	   extern int draget_copy();
	   extern int select_connections(); 
	   extern Widget ClipBoardForCopy;
	   WidgetList wsel;
	   Cardinal nwsel;
	
	
	
	   if( get_selected( actual_page,&wsel,&nwsel) == True)
	      select_connections(nwsel,wsel,NULL,WCOPY);
	   fcopy(actual_page);
	   draget_copy(actual_page->drawing,ClipBoardForCopy);
	
	   
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FPasteAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int draget_paste();
	  extern int fpaste();
	
	  fpaste(actual_page);
	  draget_paste(actual_page->drawing); 
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FgCut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcut();
	
	   fcut(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FgCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int fcopy();
	
	   fcopy(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FgPaste( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int fpaste();
	
	  fpaste(actual_page);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_BgCut( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_cut();
	   extern Widget ClipBoardForCopy;
	
	   draget_cut(actual_page->drawing,ClipBoardForCopy);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_BgCopy( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int draget_copy();
	   extern Widget ClipBoardForCopy;
	
	   draget_copy(actual_page->drawing,ClipBoardForCopy);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_BgPaste( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	  extern int draget_paste();
	
	  draget_paste(actual_page->drawing); 
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_BgPutBottom( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int DrawPutBottom();
	
	   DrawPutBottom(actual_page->drawing);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_BgPutTop( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int DrawPutTop();
	
	   DrawPutTop(actual_page->drawing);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_raggruppaButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawGroup(actual_page->drawing);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_unraggruppaButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawUngroup(actual_page->drawing);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_Edit( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	   extern swidget create_Resource();
	   swidget redit;
	   Widget widsel,reswid,edchild;
	   WidgetList child;
	   Cardinal nchild;
	   int i; 
	
#ifndef DESIGN_TIME
	   widsel = XlGetSelectedWidget( actual_page->drawing);
	   if(widsel)
	   {
	      if( XlIsXlComposite(widsel) )
	      {
	         get_child(widsel,&child,&nchild);
	         for(i=0;i<nchild;i++)
	         {
	            get_something(child[i],XlNwEdit, (void*) &edchild);
	            if(edchild != NULL)
	            {
	               printf("Cannot open Editor when Children are in Edit\n");
	               SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",
	                      "Selected Widget Children Is In Edit, Can't Open Editor",
	                       NULL,False,NULL,False,NULL);
	               return;
	            }
	         }   
	      }
	      else if( XlIsXlComposite(XtParent(widsel)) )
	      {
	         get_something( XtParent(widsel),XlNwEdit, (void*) &edchild);
	         if(edchild != NULL)
	         {
	            printf("Cannot open Editor when Parent is in Edit\n");
	            SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",
	                   "Selected Widget Parent Is In Edit, Can't Open Editor",
	                    NULL,False,NULL,False,NULL);
	            return;
	         }      
	      }
	
	      get_something(widsel,XlNwEdit, (void*) &reswid);
	      if(reswid == NULL)
	      {
	         redit = create_Resource(actual_page,dbtopologia);
	         salva_indice_editor(redit);
	         UxPopupInterface(redit,no_grab);
	      }
	      else
	      { 
	         XRaiseWindow(XtDisplay(reswid),XtWindow(reswid));
	      }
	   }
	   else
	      show_message("Nothing selected\n");
#endif
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_FDraw( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   
	if(draw_edit == NULL)
	   {
	   draw_edit = create_drawShell(PagDrawingArea,actual_page->nomepag);
	   UxPopupInterface(draw_edit,no_grab);
	   }
	else
	   XRaiseWindow(XtDisplay(draw_edit),XtWindow(draw_edit)); 
	/*
	 setta la variabile per indicare che il background e' in fase di editing
	*/
	   editing_background=True;
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_CompilePag( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	extern int compile_page();
	    
	      compile_page(actual_page);
#endif
#ifdef MAURIZIO
#ifndef DESIGN_TIME
	   extern Cardinal lnargs;    
	   extern Arg args[];
	   extern OlCompilerObject compilatore_severe;
	   int err_level = 0;
	   WidgetList children;
	   Cardinal num_children;
	   char outname[105],mesg[200];
	   int g,m,a,o,min,s;
	
	
	   if(compilatore_severe == NULL)
		{
		SetMsg(NULL,NULL,ERRMSG,"ConfigInfo",
	               "Topology database not defined",
	                NULL,False,NULL,False,NULL);
		return;
		}
	
	   if( getenv(ENVPAG) == NULL)
	      return;
	
	   lnargs = 0;
	   XtSetArg( args[lnargs],XmNchildren,&children);lnargs++; 
	   XtSetArg(args[lnargs],XmNnumChildren,&num_children);lnargs++;
	   XtGetValues(actual_page->drawing,args,lnargs);
	
	   OlSetCompiler(children,num_children,compilatore_severe);
	
	   strcpy( outname, getenv(ENVPAG));
	   strcat(outname,"/");
	   strcat(outname,actual_page->nomepag);
	   strcat(outname,".rtf");      
	
	   if( (err_level = XlCompilaWidget(actual_page->db,outname,children,num_children)) == False)   
	       {	 
	          sprintf(mesg, "Error at Compile Time on Page  %s\n",actual_page->nomepag);
	          show_message(mesg); 
	       }  
	       else
	       {
		/*       
		 setto la data della compilazione
	   	*/
	   	data(&g,&m,&a);
	   	ora(&o,&min,&s);
	   	sprintf(actual_page->compilationDate,"%d/%d/%d %d:%d:%d",g,m,a,o,min,s);
	        sprintf(mesg, "Page Compile Ok %s\n ",actual_page->nomepag);
	        show_message(mesg); 
	       }  
#endif
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_CompileReg( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	   extern int CompReg();
	   int macrobl=0;
	  
	   CompReg(actual_page,macrobl);
#endif
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_ViewError( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char error_file[100],comando[200];
	
	if( getenv(ENVPAG) == NULL)
	   return;
	
#ifndef VMS
	strcpy(error_file,XlGetenv(ENVPAG));
	strcat(error_file,"/");
#else
	strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
	strcat(error_file,actual_page->nomepag);
	strcat(error_file,RTF_EXT);
	strcat(error_file,ERR_EXT);
	
	strcpy(comando,"edit_f01   ");
	strcat(comando,error_file);
	strcat(comando,"  False &");
	
	if( system(NULL) != 0)
	   system( comando );  
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_Compile_b4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char error_file[100],comando[200];
	
	if( getenv(ENVPAG) == NULL)
	   return;
	
#ifndef VMS
	strcpy(error_file,XlGetenv(ENVPAG));
	strcat(error_file,"/");
#else
	strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
	strcat(error_file,actual_page->nomepag);
	strcat(error_file,REG_EXT);
	strcat(error_file,ERR_EXT);
	
	strcpy(comando,"edit_f01   ");
	strcat(comando,error_file);
	strcat(comando,"  False &");
	
	if( system(NULL) != 0)
	   system( comando );  
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Grid( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	extern int set_grid_on();
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	set_grid_on(actual_page->drawing,pcall->set);
	XClearArea(UxDisplay,XtWindow(actual_page->drawing),0,0,0,0,True);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_WriteRes( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	extern Boolean WriteResource();
	
	WriteResource(actual_page);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Zoom100TB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawSetZoom(actual_page->drawing,1.0);
	PaginaSetZoom(actual_page->drawing,1.0);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Zoom200TB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawSetZoom(actual_page->drawing,2.0);
	PaginaSetZoom(actual_page->drawing,2.0);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Zoom300TB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawSetZoom(actual_page->drawing,3.0);
	PaginaSetZoom(actual_page->drawing,3.0);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Zoom400TB( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	DrawSetZoom(actual_page->drawing,4.0);
	PaginaSetZoom(actual_page->drawing,4.0);
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_b_SettaSnapDraw( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	
	
	extern swidget create_Set_Snap();
	Widget wsnap;
	
	wsnap=create_Set_Snap(actual_page, PagShell);
	UxPopupInterface( wsnap ,no_grab);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	valueChangedCB_ConnectMode( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	set_connect(actual_page->drawing,pcall->set);
#endif
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_SetVisualMode( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	
	extern swidget popup_topRegoDisplayMode();
	popup_topRegoDisplayMode(actual_page->drawing);
	
	}
	UxPagShellContext = UxSaveCtx;
}

static	void	activateCB_RefreshDrawing( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
	RefreshDrawingArea(actual_page->drawing);
	}
	UxPagShellContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_PagShell()
{
	Widget		menu3_shell;
	Widget		menu3Foreground_shell;
	Widget		menu3Background_shell;
	Widget		FileTmp_shell;
	Widget		AddObj_shell;
	Widget		FEdit_shell;
	Widget		EditForeground_shell;
	Widget		EditBackground_shell;
	Widget		Configure_shell;
	Widget		DrawBackground_shell;
	Widget		Compile_shell;
	Widget		Option_shell;
	Widget		ZoPane_shell;


	/* Creation of PagShell */
	PagShell = XtVaCreatePopupShell( "PagShell",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNtitle, "Pagina",
			XmNallowShellResize, TRUE,
			XmNheight, 121,
			XmNx, 82,
			XmNy, 46,
			NULL );
	XtAddCallback( PagShell, XmNpopupCallback,
		(XtCallbackProc) popupCB_PagShell,
		(XtPointer) UxPagShellContext );

	UxPutContext( PagShell, (char *) UxPagShellContext );


	/* Creation of PagMainWindow */
	PagMainWindow = XtVaCreateManagedWidget( "PagMainWindow",
			xmMainWindowWidgetClass,
			PagShell,
			XmNunitType, XmPIXELS,
			XmNx, 80,
			XmNy, 120,
			XmNheight, 121,
			NULL );
	UxPutContext( PagMainWindow, (char *) UxPagShellContext );


	/* Creation of PagForm1 */
	PagForm1 = XtVaCreateManagedWidget( "PagForm1",
			xmFormWidgetClass,
			PagMainWindow,
			NULL );
	UxPutContext( PagForm1, (char *) UxPagShellContext );


	/* Creation of PagScrolledWindow */
	PagScrolledWindow = XtVaCreateManagedWidget( "PagScrolledWindow",
			xmScrolledWindowWidgetClass,
			PagForm1,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 0,
			XmNy, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNnavigationType, XmNONE,
			XmNtraversalOn, FALSE,
			NULL );
	UxPutContext( PagScrolledWindow, (char *) UxPagShellContext );


	/* Creation of PagDrawingArea */
	PagDrawingArea = XtVaCreateManagedWidget( "PagDrawingArea",
			xmDrawingAreaWidgetClass,
			PagScrolledWindow,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 4,
			XmNy, -6,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNnavigationType, XmNONE,
			XmNtraversalOn, FALSE,
			NULL );
	UxPutContext( PagDrawingArea, (char *) UxPagShellContext );


	/* Creation of menu3 */
	menu3_shell = XtVaCreatePopupShell ("menu3_shell",
			xmMenuShellWidgetClass, PagDrawingArea,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu3 = XtVaCreateWidget( "menu3",
			xmRowColumnWidgetClass,
			menu3_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "<KeyUp>F4",
			NULL );
	UxPutContext( menu3, (char *) UxPagShellContext );

	createCB_menu3( menu3,
			(XtPointer) UxPagShellContext, (XtPointer) NULL );


	/* Creation of menu3_p1_title */
	menu3_p1_title = XtVaCreateManagedWidget( "menu3_p1_title",
			xmLabelWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );
	UxPutContext( menu3_p1_title, (char *) UxPagShellContext );


	/* Creation of menu3separ1 */
	menu3separ1 = XtVaCreateManagedWidget( "menu3separ1",
			xmSeparatorWidgetClass,
			menu3,
			NULL );
	UxPutContext( menu3separ1, (char *) UxPagShellContext );


	/* Creation of menu3Edit */
	menu3Edit = XtVaCreateManagedWidget( "menu3Edit",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Resource" ),
			NULL );
	XtAddCallback( menu3Edit, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3Edit,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3Edit, (char *) UxPagShellContext );


	/* Creation of menu3separ0 */
	menu3separ0 = XtVaCreateManagedWidget( "menu3separ0",
			xmSeparatorGadgetClass,
			menu3,
			NULL );
	UxPutContext( menu3separ0, (char *) UxPagShellContext );


	/* Creation of menu3Refresh */
	menu3Refresh = XtVaCreateManagedWidget( "menu3Refresh",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Refresh window" ),
			NULL );
	XtAddCallback( menu3Refresh, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3Refresh,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3Refresh, (char *) UxPagShellContext );


	/* Creation of menu3Undo */
	menu3Undo = XtVaCreateManagedWidget( "menu3Undo",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Undo draw" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			NULL );
	XtAddCallback( menu3Undo, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3Undo,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3Undo, (char *) UxPagShellContext );


	/* Creation of menu3DuplicateAll */
	menu3DuplicateAll = XtVaCreateManagedWidget( "menu3DuplicateAll",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Duplicate" ),
			NULL );
	XtAddCallback( menu3DuplicateAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3DuplicateAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3DuplicateAll, (char *) UxPagShellContext );


	/* Creation of menu3DeleteAll */
	menu3DeleteAll = XtVaCreateManagedWidget( "menu3DeleteAll",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( menu3DeleteAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3DeleteAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3DeleteAll, (char *) UxPagShellContext );


	/* Creation of menu3CutAll */
	menu3CutAll = XtVaCreateManagedWidget( "menu3CutAll",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( menu3CutAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3CutAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3CutAll, (char *) UxPagShellContext );


	/* Creation of menu3CopyAll */
	menu3CopyAll = XtVaCreateManagedWidget( "menu3CopyAll",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( menu3CopyAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3CopyAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3CopyAll, (char *) UxPagShellContext );


	/* Creation of menu3PasteAll */
	menu3PasteAll = XtVaCreateManagedWidget( "menu3PasteAll",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( menu3PasteAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3PasteAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3PasteAll, (char *) UxPagShellContext );


	/* Creation of menu3separ */
	menu3separ = XtVaCreateManagedWidget( "menu3separ",
			xmSeparatorGadgetClass,
			menu3,
			NULL );
	UxPutContext( menu3separ, (char *) UxPagShellContext );


	/* Creation of menu3Foreground */
	menu3Foreground_shell = XtVaCreatePopupShell ("menu3Foreground_shell",
			xmMenuShellWidgetClass, menu3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu3Foreground = XtVaCreateWidget( "menu3Foreground",
			xmRowColumnWidgetClass,
			menu3Foreground_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu3Foreground, (char *) UxPagShellContext );


	/* Creation of menu3FCut */
	menu3FCut = XtVaCreateManagedWidget( "menu3FCut",
			xmPushButtonWidgetClass,
			menu3Foreground,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( menu3FCut, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3FCut,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3FCut, (char *) UxPagShellContext );


	/* Creation of menu3FCopy */
	menu3FCopy = XtVaCreateManagedWidget( "menu3FCopy",
			xmPushButtonWidgetClass,
			menu3Foreground,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( menu3FCopy, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3FCopy,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3FCopy, (char *) UxPagShellContext );


	/* Creation of menu3FPaste */
	menu3FPaste = XtVaCreateManagedWidget( "menu3FPaste",
			xmPushButtonWidgetClass,
			menu3Foreground,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( menu3FPaste, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3FPaste,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3FPaste, (char *) UxPagShellContext );


	/* Creation of menu3PForeground */
	menu3PForeground = XtVaCreateManagedWidget( "menu3PForeground",
			xmCascadeButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Foreground" ),
			XmNsubMenuId, menu3Foreground,
			NULL );
	UxPutContext( menu3PForeground, (char *) UxPagShellContext );


	/* Creation of menu3Background */
	menu3Background_shell = XtVaCreatePopupShell ("menu3Background_shell",
			xmMenuShellWidgetClass, menu3,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu3Background = XtVaCreateWidget( "menu3Background",
			xmRowColumnWidgetClass,
			menu3Background_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu3Background, (char *) UxPagShellContext );


	/* Creation of menu3BCut */
	menu3BCut = XtVaCreateManagedWidget( "menu3BCut",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( menu3BCut, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BCut,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BCut, (char *) UxPagShellContext );


	/* Creation of menu3BCopy */
	menu3BCopy = XtVaCreateManagedWidget( "menu3BCopy",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( menu3BCopy, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BCopy,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BCopy, (char *) UxPagShellContext );


	/* Creation of menu3BPaste */
	menu3BPaste = XtVaCreateManagedWidget( "menu3BPaste",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( menu3BPaste, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BPaste,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BPaste, (char *) UxPagShellContext );


	/* Creation of menu3BPutBottom */
	menu3BPutBottom = XtVaCreateManagedWidget( "menu3BPutBottom",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Put Bottom" ),
			NULL );
	XtAddCallback( menu3BPutBottom, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BPutBottom,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BPutBottom, (char *) UxPagShellContext );


	/* Creation of menu3BPutTop */
	menu3BPutTop = XtVaCreateManagedWidget( "menu3BPutTop",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Put Top" ),
			NULL );
	XtAddCallback( menu3BPutTop, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BPutTop,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BPutTop, (char *) UxPagShellContext );


	/* Creation of menu3BModifyObject */
	menu3BModifyObject = XtVaCreateManagedWidget( "menu3BModifyObject",
			xmPushButtonWidgetClass,
			menu3Background,
			RES_CONVERT( XmNlabelString, "Modify Object" ),
			NULL );
	XtAddCallback( menu3BModifyObject, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu3BModifyObject,
		(XtPointer) UxPagShellContext );

	UxPutContext( menu3BModifyObject, (char *) UxPagShellContext );


	/* Creation of menu3PBackground */
	menu3PBackground = XtVaCreateManagedWidget( "menu3PBackground",
			xmCascadeButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Background" ),
			XmNsubMenuId, menu3Background,
			NULL );
	UxPutContext( menu3PBackground, (char *) UxPagShellContext );


	/* Creation of grePagPane */
	grePagPane = XtVaCreateManagedWidget( "grePagPane",
			xmRowColumnWidgetClass,
			PagMainWindow,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( grePagPane, (char *) UxPagShellContext );


	/* Creation of FileTmp */
	FileTmp_shell = XtVaCreatePopupShell ("FileTmp_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FileTmp = XtVaCreateWidget( "FileTmp",
			xmRowColumnWidgetClass,
			FileTmp_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( FileTmp, (char *) UxPagShellContext );


	/* Creation of SaveTmp */
	SaveTmp = XtVaCreateManagedWidget( "SaveTmp",
			xmPushButtonWidgetClass,
			FileTmp,
			RES_CONVERT( XmNlabelString, "Save" ),
			XmNsensitive, TRUE,
			NULL );
	XtAddCallback( SaveTmp, XmNactivateCallback,
		(XtCallbackProc) activateCB_SaveTmp,
		(XtPointer) UxPagShellContext );

	UxPutContext( SaveTmp, (char *) UxPagShellContext );


	/* Creation of FileSep */
	FileSep = XtVaCreateManagedWidget( "FileSep",
			xmSeparatorWidgetClass,
			FileTmp,
			NULL );
	UxPutContext( FileSep, (char *) UxPagShellContext );


	/* Creation of Stampa */
	Stampa = XtVaCreateManagedWidget( "Stampa",
			xmPushButtonWidgetClass,
			FileTmp,
			RES_CONVERT( XmNlabelString, "Print" ),
			NULL );
	XtAddCallback( Stampa, XmNactivateCallback,
		(XtCallbackProc) activateCB_Stampa,
		(XtPointer) UxPagShellContext );

	UxPutContext( Stampa, (char *) UxPagShellContext );


	/* Creation of Stampa_Setup */
	Stampa_Setup = XtVaCreateManagedWidget( "Stampa_Setup",
			xmPushButtonWidgetClass,
			FileTmp,
			RES_CONVERT( XmNlabelString, "Printer Setup..." ),
			NULL );
	XtAddCallback( Stampa_Setup, XmNactivateCallback,
		(XtCallbackProc) activateCB_Stampa_Setup,
		(XtPointer) UxPagShellContext );

	UxPutContext( Stampa_Setup, (char *) UxPagShellContext );


	/* Creation of FileTmp_b6 */
	FileTmp_b6 = XtVaCreateManagedWidget( "FileTmp_b6",
			xmSeparatorWidgetClass,
			FileTmp,
			NULL );
	UxPutContext( FileTmp_b6, (char *) UxPagShellContext );


	/* Creation of CloseTmp */
	CloseTmp = XtVaCreateManagedWidget( "CloseTmp",
			xmPushButtonWidgetClass,
			FileTmp,
			RES_CONVERT( XmNlabelString, "Close" ),
			NULL );
	XtAddCallback( CloseTmp, XmNactivateCallback,
		(XtCallbackProc) activateCB_CloseTmp,
		(XtPointer) UxPagShellContext );

	UxPutContext( CloseTmp, (char *) UxPagShellContext );


	/* Creation of menu4_top_b1 */
	menu4_top_b1 = XtVaCreateManagedWidget( "menu4_top_b1",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "File" ),
			XmNsubMenuId, FileTmp,
			RES_CONVERT( XmNmnemonic, "F" ),
			NULL );
	UxPutContext( menu4_top_b1, (char *) UxPagShellContext );


	/* Creation of AddObj */
	AddObj_shell = XtVaCreatePopupShell ("AddObj_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	AddObj = XtVaCreateWidget( "AddObj",
			xmRowColumnWidgetClass,
			AddObj_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( AddObj, (char *) UxPagShellContext );


	/* Creation of Icon */
	Icon = XtVaCreateManagedWidget( "Icon",
			xmPushButtonWidgetClass,
			AddObj,
			RES_CONVERT( XmNlabelString, "Icon" ),
			NULL );
	UxPutContext( Icon, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b1 */
	PagPane_top_b1 = XtVaCreateManagedWidget( "PagPane_top_b1",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "Add Icon" ),
			RES_CONVERT( XmNmnemonic, "A" ),
			XmNsubMenuId, AddObj,
			NULL );
	UxPutContext( PagPane_top_b1, (char *) UxPagShellContext );


	/* Creation of FEdit */
	FEdit_shell = XtVaCreatePopupShell ("FEdit_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	FEdit = XtVaCreateWidget( "FEdit",
			xmRowColumnWidgetClass,
			FEdit_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( FEdit, (char *) UxPagShellContext );


	/* Creation of FUndo */
	FUndo = XtVaCreateManagedWidget( "FUndo",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Undo draw" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			NULL );
	XtAddCallback( FUndo, XmNactivateCallback,
		(XtCallbackProc) activateCB_FUndo,
		(XtPointer) UxPagShellContext );

	UxPutContext( FUndo, (char *) UxPagShellContext );


	/* Creation of FDuplicateAll */
	FDuplicateAll = XtVaCreateManagedWidget( "FDuplicateAll",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Duplicate" ),
			NULL );
	XtAddCallback( FDuplicateAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_FDuplicateAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( FDuplicateAll, (char *) UxPagShellContext );


	/* Creation of FDeleteAll */
	FDeleteAll = XtVaCreateManagedWidget( "FDeleteAll",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( FDeleteAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_FDeleteAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( FDeleteAll, (char *) UxPagShellContext );


	/* Creation of FCutAll */
	FCutAll = XtVaCreateManagedWidget( "FCutAll",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( FCutAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_FCutAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( FCutAll, (char *) UxPagShellContext );


	/* Creation of FCopyAll */
	FCopyAll = XtVaCreateManagedWidget( "FCopyAll",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( FCopyAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_FCopyAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( FCopyAll, (char *) UxPagShellContext );


	/* Creation of FPasteAll */
	FPasteAll = XtVaCreateManagedWidget( "FPasteAll",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( FPasteAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_FPasteAll,
		(XtPointer) UxPagShellContext );

	UxPutContext( FPasteAll, (char *) UxPagShellContext );


	/* Creation of Fsepar0 */
	Fsepar0 = XtVaCreateManagedWidget( "Fsepar0",
			xmSeparatorGadgetClass,
			FEdit,
			NULL );
	UxPutContext( Fsepar0, (char *) UxPagShellContext );


	/* Creation of EditForeground */
	EditForeground_shell = XtVaCreatePopupShell ("EditForeground_shell",
			xmMenuShellWidgetClass, FEdit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	EditForeground = XtVaCreateWidget( "EditForeground",
			xmRowColumnWidgetClass,
			EditForeground_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( EditForeground, (char *) UxPagShellContext );


	/* Creation of FgCut */
	FgCut = XtVaCreateManagedWidget( "FgCut",
			xmPushButtonWidgetClass,
			EditForeground,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( FgCut, XmNactivateCallback,
		(XtCallbackProc) activateCB_FgCut,
		(XtPointer) UxPagShellContext );

	UxPutContext( FgCut, (char *) UxPagShellContext );


	/* Creation of FgCopy */
	FgCopy = XtVaCreateManagedWidget( "FgCopy",
			xmPushButtonWidgetClass,
			EditForeground,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( FgCopy, XmNactivateCallback,
		(XtCallbackProc) activateCB_FgCopy,
		(XtPointer) UxPagShellContext );

	UxPutContext( FgCopy, (char *) UxPagShellContext );


	/* Creation of FgPaste */
	FgPaste = XtVaCreateManagedWidget( "FgPaste",
			xmPushButtonWidgetClass,
			EditForeground,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( FgPaste, XmNactivateCallback,
		(XtCallbackProc) activateCB_FgPaste,
		(XtPointer) UxPagShellContext );

	UxPutContext( FgPaste, (char *) UxPagShellContext );


	/* Creation of FEditForeground */
	FEditForeground = XtVaCreateManagedWidget( "FEditForeground",
			xmCascadeButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Foreground" ),
			XmNsubMenuId, EditForeground,
			NULL );
	UxPutContext( FEditForeground, (char *) UxPagShellContext );


	/* Creation of EditBackground */
	EditBackground_shell = XtVaCreatePopupShell ("EditBackground_shell",
			xmMenuShellWidgetClass, FEdit,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	EditBackground = XtVaCreateWidget( "EditBackground",
			xmRowColumnWidgetClass,
			EditBackground_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( EditBackground, (char *) UxPagShellContext );


	/* Creation of BgCut */
	BgCut = XtVaCreateManagedWidget( "BgCut",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Cut" ),
			NULL );
	XtAddCallback( BgCut, XmNactivateCallback,
		(XtCallbackProc) activateCB_BgCut,
		(XtPointer) UxPagShellContext );

	UxPutContext( BgCut, (char *) UxPagShellContext );


	/* Creation of BgCopy */
	BgCopy = XtVaCreateManagedWidget( "BgCopy",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Copy" ),
			NULL );
	XtAddCallback( BgCopy, XmNactivateCallback,
		(XtCallbackProc) activateCB_BgCopy,
		(XtPointer) UxPagShellContext );

	UxPutContext( BgCopy, (char *) UxPagShellContext );


	/* Creation of BgPaste */
	BgPaste = XtVaCreateManagedWidget( "BgPaste",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Paste" ),
			NULL );
	XtAddCallback( BgPaste, XmNactivateCallback,
		(XtCallbackProc) activateCB_BgPaste,
		(XtPointer) UxPagShellContext );

	UxPutContext( BgPaste, (char *) UxPagShellContext );


	/* Creation of BgPutBottom */
	BgPutBottom = XtVaCreateManagedWidget( "BgPutBottom",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Put Bottom" ),
			NULL );
	XtAddCallback( BgPutBottom, XmNactivateCallback,
		(XtCallbackProc) activateCB_BgPutBottom,
		(XtPointer) UxPagShellContext );

	UxPutContext( BgPutBottom, (char *) UxPagShellContext );


	/* Creation of BgPutTop */
	BgPutTop = XtVaCreateManagedWidget( "BgPutTop",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Put Top" ),
			NULL );
	XtAddCallback( BgPutTop, XmNactivateCallback,
		(XtCallbackProc) activateCB_BgPutTop,
		(XtPointer) UxPagShellContext );

	UxPutContext( BgPutTop, (char *) UxPagShellContext );


	/* Creation of raggruppaButton */
	raggruppaButton = XtVaCreateManagedWidget( "raggruppaButton",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Group" ),
			NULL );
	XtAddCallback( raggruppaButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_raggruppaButton,
		(XtPointer) UxPagShellContext );

	UxPutContext( raggruppaButton, (char *) UxPagShellContext );


	/* Creation of unraggruppaButton */
	unraggruppaButton = XtVaCreateManagedWidget( "unraggruppaButton",
			xmPushButtonWidgetClass,
			EditBackground,
			RES_CONVERT( XmNlabelString, "Ungroup" ),
			NULL );
	XtAddCallback( unraggruppaButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_unraggruppaButton,
		(XtPointer) UxPagShellContext );

	UxPutContext( unraggruppaButton, (char *) UxPagShellContext );


	/* Creation of FEditBackground */
	FEditBackground = XtVaCreateManagedWidget( "FEditBackground",
			xmCascadeButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Background" ),
			XmNsubMenuId, EditBackground,
			NULL );
	UxPutContext( FEditBackground, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b2 */
	PagPane_top_b2 = XtVaCreateManagedWidget( "PagPane_top_b2",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "Edit" ),
			XmNsubMenuId, FEdit,
			NULL );
	UxPutContext( PagPane_top_b2, (char *) UxPagShellContext );


	/* Creation of Configure */
	Configure_shell = XtVaCreatePopupShell ("Configure_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Configure = XtVaCreateWidget( "Configure",
			xmRowColumnWidgetClass,
			Configure_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( Configure, (char *) UxPagShellContext );


	/* Creation of Edit */
	Edit = XtVaCreateManagedWidget( "Edit",
			xmPushButtonWidgetClass,
			Configure,
			RES_CONVERT( XmNlabelString, "Resource Editor" ),
			NULL );
	XtAddCallback( Edit, XmNactivateCallback,
		(XtCallbackProc) activateCB_Edit,
		(XtPointer) UxPagShellContext );

	UxPutContext( Edit, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b3 */
	PagPane_top_b3 = XtVaCreateManagedWidget( "PagPane_top_b3",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "Configure" ),
			XmNsubMenuId, Configure,
			NULL );
	UxPutContext( PagPane_top_b3, (char *) UxPagShellContext );


	/* Creation of DrawBackground */
	DrawBackground_shell = XtVaCreatePopupShell ("DrawBackground_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	DrawBackground = XtVaCreateWidget( "DrawBackground",
			xmRowColumnWidgetClass,
			DrawBackground_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( DrawBackground, (char *) UxPagShellContext );


	/* Creation of FDraw */
	FDraw = XtVaCreateManagedWidget( "FDraw",
			xmPushButtonWidgetClass,
			DrawBackground,
			RES_CONVERT( XmNlabelString, "Draw" ),
			NULL );
	XtAddCallback( FDraw, XmNactivateCallback,
		(XtCallbackProc) activateCB_FDraw,
		(XtPointer) UxPagShellContext );

	UxPutContext( FDraw, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b6 */
	PagPane_top_b6 = XtVaCreateManagedWidget( "PagPane_top_b6",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "DrawBackground" ),
			XmNsubMenuId, DrawBackground,
			NULL );
	UxPutContext( PagPane_top_b6, (char *) UxPagShellContext );


	/* Creation of Compile */
	Compile_shell = XtVaCreatePopupShell ("Compile_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Compile = XtVaCreateWidget( "Compile",
			xmRowColumnWidgetClass,
			Compile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( Compile, (char *) UxPagShellContext );


	/* Creation of CompilePag */
	CompilePag = XtVaCreateManagedWidget( "CompilePag",
			xmPushButtonWidgetClass,
			Compile,
			RES_CONVERT( XmNlabelString, "Page" ),
			NULL );
	XtAddCallback( CompilePag, XmNactivateCallback,
		(XtCallbackProc) activateCB_CompilePag,
		(XtPointer) UxPagShellContext );

	UxPutContext( CompilePag, (char *) UxPagShellContext );


	/* Creation of CompileReg */
	CompileReg = XtVaCreateManagedWidget( "CompileReg",
			xmPushButtonWidgetClass,
			Compile,
			RES_CONVERT( XmNlabelString, "Regolation" ),
			NULL );
	XtAddCallback( CompileReg, XmNactivateCallback,
		(XtCallbackProc) activateCB_CompileReg,
		(XtPointer) UxPagShellContext );

	UxPutContext( CompileReg, (char *) UxPagShellContext );


	/* Creation of Compile_b5 */
	Compile_b5 = XtVaCreateManagedWidget( "Compile_b5",
			xmSeparatorWidgetClass,
			Compile,
			NULL );
	UxPutContext( Compile_b5, (char *) UxPagShellContext );


	/* Creation of ViewError */
	ViewError = XtVaCreateManagedWidget( "ViewError",
			xmPushButtonWidgetClass,
			Compile,
			RES_CONVERT( XmNlabelString, "Page Error View" ),
			NULL );
	XtAddCallback( ViewError, XmNactivateCallback,
		(XtCallbackProc) activateCB_ViewError,
		(XtPointer) UxPagShellContext );

	UxPutContext( ViewError, (char *) UxPagShellContext );


	/* Creation of Compile_b4 */
	Compile_b4 = XtVaCreateManagedWidget( "Compile_b4",
			xmPushButtonWidgetClass,
			Compile,
			RES_CONVERT( XmNlabelString, "Regolation Error View " ),
			NULL );
	XtAddCallback( Compile_b4, XmNactivateCallback,
		(XtCallbackProc) activateCB_Compile_b4,
		(XtPointer) UxPagShellContext );

	UxPutContext( Compile_b4, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b4 */
	PagPane_top_b4 = XtVaCreateManagedWidget( "PagPane_top_b4",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "Compile" ),
			XmNsubMenuId, Compile,
			XmNsensitive, TRUE,
			NULL );
	UxPutContext( PagPane_top_b4, (char *) UxPagShellContext );


	/* Creation of Option */
	Option_shell = XtVaCreatePopupShell ("Option_shell",
			xmMenuShellWidgetClass, grePagPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	Option = XtVaCreateWidget( "Option",
			xmRowColumnWidgetClass,
			Option_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( Option, (char *) UxPagShellContext );


	/* Creation of Grid */
	Grid = XtVaCreateManagedWidget( "Grid",
			xmToggleButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Grid" ),
			NULL );
	XtAddCallback( Grid, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Grid,
		(XtPointer) UxPagShellContext );

	UxPutContext( Grid, (char *) UxPagShellContext );


	/* Creation of WriteRes */
	WriteRes = XtVaCreateManagedWidget( "WriteRes",
			xmPushButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Write Resource" ),
			NULL );
	XtAddCallback( WriteRes, XmNactivateCallback,
		(XtCallbackProc) activateCB_WriteRes,
		(XtPointer) UxPagShellContext );

	UxPutContext( WriteRes, (char *) UxPagShellContext );


	/* Creation of ZoPane */
	ZoPane_shell = XtVaCreatePopupShell ("ZoPane_shell",
			xmMenuShellWidgetClass, Option,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	ZoPane = XtVaCreateWidget( "ZoPane",
			xmRowColumnWidgetClass,
			ZoPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( ZoPane, (char *) UxPagShellContext );


	/* Creation of Zoom100TB */
	Zoom100TB = XtVaCreateManagedWidget( "Zoom100TB",
			xmToggleButtonWidgetClass,
			ZoPane,
			RES_CONVERT( XmNlabelString, "100% (no Zoom)" ),
			NULL );
	XtAddCallback( Zoom100TB, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Zoom100TB,
		(XtPointer) UxPagShellContext );

	UxPutContext( Zoom100TB, (char *) UxPagShellContext );


	/* Creation of Zoom200TB */
	Zoom200TB = XtVaCreateManagedWidget( "Zoom200TB",
			xmToggleButtonWidgetClass,
			ZoPane,
			RES_CONVERT( XmNlabelString, "200%" ),
			NULL );
	XtAddCallback( Zoom200TB, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Zoom200TB,
		(XtPointer) UxPagShellContext );

	UxPutContext( Zoom200TB, (char *) UxPagShellContext );


	/* Creation of Zoom300TB */
	Zoom300TB = XtVaCreateManagedWidget( "Zoom300TB",
			xmToggleButtonWidgetClass,
			ZoPane,
			RES_CONVERT( XmNlabelString, "300%" ),
			NULL );
	XtAddCallback( Zoom300TB, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Zoom300TB,
		(XtPointer) UxPagShellContext );

	UxPutContext( Zoom300TB, (char *) UxPagShellContext );


	/* Creation of Zoom400TB */
	Zoom400TB = XtVaCreateManagedWidget( "Zoom400TB",
			xmToggleButtonWidgetClass,
			ZoPane,
			RES_CONVERT( XmNlabelString, "400%" ),
			NULL );
	XtAddCallback( Zoom400TB, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Zoom400TB,
		(XtPointer) UxPagShellContext );

	UxPutContext( Zoom400TB, (char *) UxPagShellContext );


	/* Creation of SetZ */
	SetZ = XtVaCreateManagedWidget( "SetZ",
			xmCascadeButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Zoom" ),
			XmNsubMenuId, ZoPane,
			NULL );
	UxPutContext( SetZ, (char *) UxPagShellContext );


	/* Creation of b_SettaSnapDraw */
	b_SettaSnapDraw = XtVaCreateManagedWidget( "b_SettaSnapDraw",
			xmPushButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Set/View Snap..." ),
			NULL );
	XtAddCallback( b_SettaSnapDraw, XmNactivateCallback,
		(XtCallbackProc) activateCB_b_SettaSnapDraw,
		(XtPointer) UxPagShellContext );

	UxPutContext( b_SettaSnapDraw, (char *) UxPagShellContext );


	/* Creation of ConnectMode */
	ConnectMode = XtVaCreateManagedWidget( "ConnectMode",
			xmToggleButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Connect Mode" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( ConnectMode, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_ConnectMode,
		(XtPointer) UxPagShellContext );

	UxPutContext( ConnectMode, (char *) UxPagShellContext );


	/* Creation of SetVisualMode */
	SetVisualMode = XtVaCreateManagedWidget( "SetVisualMode",
			xmPushButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Set visual mode" ),
			NULL );
	XtAddCallback( SetVisualMode, XmNactivateCallback,
		(XtCallbackProc) activateCB_SetVisualMode,
		(XtPointer) UxPagShellContext );

	UxPutContext( SetVisualMode, (char *) UxPagShellContext );


	/* Creation of RefreshDrawing */
	RefreshDrawing = XtVaCreateManagedWidget( "RefreshDrawing",
			xmPushButtonWidgetClass,
			Option,
			RES_CONVERT( XmNlabelString, "Refresh window" ),
			NULL );
	XtAddCallback( RefreshDrawing, XmNactivateCallback,
		(XtCallbackProc) activateCB_RefreshDrawing,
		(XtPointer) UxPagShellContext );

	UxPutContext( RefreshDrawing, (char *) UxPagShellContext );


	/* Creation of PagPane_top_b5 */
	PagPane_top_b5 = XtVaCreateManagedWidget( "PagPane_top_b5",
			xmCascadeButtonWidgetClass,
			grePagPane,
			RES_CONVERT( XmNlabelString, "Option" ),
			XmNsubMenuId, Option,
			NULL );
	UxPutContext( PagPane_top_b5, (char *) UxPagShellContext );


	XtAddCallback( PagShell, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxPagShellContext);

	XtAddEventHandler(PagDrawingArea, ButtonPressMask,
			False, (XtEventHandler) _UxPagShellMenuPost, (XtPointer) menu3 );
	XmMainWindowSetAreas( PagMainWindow, grePagPane, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, PagForm1 );

	return ( PagShell );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_PagShell( _Uxnome_pagina, _Uxindice, _Uxtipop )
	unsigned char	*_Uxnome_pagina;
	int	_Uxindice;
	int	_Uxtipop;
{
	Widget                  rtrn;
	_UxCPagShell            *UxContext;

	UxPagShellContext = UxContext =
		(_UxCPagShell *) UxNewContext( sizeof(_UxCPagShell), False );

	nome_pagina = _Uxnome_pagina;
	indice = _Uxindice;
	tipop = _Uxtipop;

	{
		extern Boolean StateInterfaceMode;
		extern void AllPagWidSetTranslation();
		extern void PostOpenPage();
		extern int PagGetType();
		/* 
		  Interfaccia di appoggio per la gestione della clipboard
		*/
		extern Widget ClipBoardUndo;
		XdLista get_lista_undo();
		XdListaUndo get_lista_liste_undo();
		void CreaUndoList();
		PAGINA *pappo;
		AN_OBJ *lista_oggetti = NULL;
		char titolo[100];
		
		wset_snap= NULL;
		elenco_editor = NULL;
		lista_draget = NULL;
		draw_edit=NULL;
		editing_background=False; /* indica che non e' stato ancora richiamato
		                             l'editor del background */
		printf("indice pagina chiamata %d\n",indice);
		if( pagina_init(nome_pagina,&actual_page,tipop) == False)
		{
		   printf("errore nella inizializzazione della pagina\n");
		   return(NULL);
		}
		
		pagina_load_file(actual_page,tipop);
		
		UxDisplay->db = actual_page->db;
		
		if(  pagina_getres(actual_page,&lista_oggetti) == False)
		{
		   printf("non ci sono risorse nel file della pagina\n");
		}
		printf("ho teminato di caricare la pagina=%s\n",actual_page->filepag); 
		lista_draget = XdCreateLista();
		lista_draget_undo=NULL;
		lista_liste_undo=undoCreateLists();
		
		/*
		 Inizializza a valori di default alcuni campi della
		 struttura descrivente gli attributi del disegno
		*/
		init_st_draw(&st_draw);
		rtrn = _Uxbuild_PagShell();

		/* inizializzo la struttura PAGINA e carico le resource della medesima*/
		
		actual_page->topwidget = (Widget )rtrn;
		actual_page->drawing = (Widget )PagDrawingArea;
		actual_page->dispreg_visual_mode = DISPREG_TAG;
		actual_page->port_visual_mode = PORT_MANAGED;
		actual_page->interfaceport_visual_mode = INTERFACE_PORT_MANAGED;
		
		
		pagina_setta_resource(actual_page);
		pagina_create_widget(actual_page,lista_oggetti);
		
		/*
		   Caricamento dei draget presenti all'apertura della pagina nella struttura dedicata
		   all'operazione di 'undo'.
		*/
		CreaUndoList(actual_page->drawing);
		undo_draget_copy(actual_page->drawing,ClipBoardUndo);
		undoListAdd(get_lista_liste_undo(actual_page->drawing),get_lista_undo(actual_page->drawing));
		
		if( tipop == TIPO_LIBRARY)
		{
		   set_something(PagPane_top_b4,XmNsensitive,(void*) False);
		   set_something(PagPane_top_b6,XmNsensitive,(void*) False);
		   set_something(FEditForeground,XmNsensitive,(void*) False);
		   set_something(ConnectMode,XmNsensitive,(void*) False);
		}
		else if ( PagGetType(actual_page) == TYPE_REGOLAZIONE)
			{
			/**  la risorsa snap e' gestita a livello di pagina
			DrawSetSnap((Widget )PagDrawingArea,4);
			**/
			set_something(FUndo,XmNsensitive,(void*) False);
			}
		else if ( PagGetType(actual_page) != TYPE_REGOLAZIONE)
			{
		/*
		 rende insensitive i bottoni tipici solo di pagine regolazione:
			Connect Mode
			Compile Regolation
			Regolation Error View
			Set Visual Mode
		*/
		        set_something(ConnectMode,XmNsensitive,(void*) False);
			set_something(CompileReg,XmNsensitive,(void*) False);
			set_something(Compile_b4,XmNsensitive,(void*) False);
			set_something(SetVisualMode,XmNsensitive,(void*) False);
			}
		
		
		/* aggiorno la lista delle pagine */
		pappo = pagine;
		if( pappo == NULL)
		   pagine = actual_page;
		else
		{
		  while(pagine->pagsucc != NULL)
		      pagine = pagine->pagsucc;
		
		  pagine->pagsucc = actual_page;
		  pagine = pappo;
		}
		
		AllPagWidSetTranslation( actual_page );
		
		
		/* adesso setto i bottoni del menu delle iconlibrary */
		add_objlib_button(rtrn);
		
		
		if(lista_oggetti != NULL)
		   libera_memoria((char*)lista_oggetti);
		
		if(tipop == TIPO_PAGINA)
		   strcpy(titolo,"Page : ");
		else if(tipop == TIPO_LIBRARY)
		   strcpy(titolo,"Library : ");
		
		strcat(titolo,actual_page->nomepag);
		
		set_something(actual_page->topwidget,XmNtitle,(void*) titolo);
		/***
		if(!CompilerOk)
		   set_something(PagPane_top_b4,XmNsensitive,(void*) False);
		***/ 
		PostOpenPage(actual_page->drawing);
		
		if(StateInterfaceMode)
		{
		   PageMenuSetInterfaceModeOn();
		/**
		   set_something(SaveTmp,XmNsensitive,(void*) False);
		   set_something(Option_b2,XmNsensitive,(void*) False);
		   set_connect(actual_page->drawing,(int)1);   
		*/
		}
		else
		   PageMenuSetInterfaceModeOff();
		
		/*
		  Nel caso in cui la licenza disponibile e' di tipo 'demo', vengono
		  disabilitate alcune funzionalita'; compilazione e
		  salvataggio delle pagine. 
		*/
		   if(flag_demo == 1) {
		      XtVaSetValues(PagPane_top_b4, XmNsensitive, False, NULL);
		      XtVaSetValues(SaveTmp, XmNsensitive, False, NULL);
		   }
		
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

