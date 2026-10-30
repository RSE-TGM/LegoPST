
/*******************************************************************************
       PagShell.c
       (Generated from interface file PagShell.i)
*******************************************************************************/

#include <stdio.h>
#include <string.h>

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
#include <Xm/ScrollBar.h>
#include <Xm/Form.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>
#include <unistd.h>
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
void PagImprontaTogli();
void PagImprontaAggiorna(PAGINA *, int, Boolean);
void PagChiudiRichiesta();
void PagStatusMsg(Widget, int, char *);
//extern void SetItemString();
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
return(ret != NULL);
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


/*----------------------------------------------------------------------
 * Pagine modificate e chiusura (ottobre 2026).
 *
 * Non c'era nessun flag di "pagina modificata": le modifiche passano da
 * troppi punti (move, resize, risorse, icone aggiunte o cancellate,
 * sfondo, connessioni) per inseguirle una per una. Si usa un'impronta:
 * all'apertura e dopo ogni salvataggio si fotografa quello che il Save
 * scriverebbe (database delle risorse, elenco degli oggetti, dati della
 * pagina, disegno di sfondo); alla chiusura la si rifa' e si confronta.
 * Le righe del database si ordinano: Xrm non le scrive in ordine fisso.
 *
 * La chiusura passa sempre da PagChiudiRichiesta, sia da File -> Close
 * sia dalla X del window manager. Prima la X lasciava il default Motif
 * (XmDESTROY): la shell spariva ma la pagina restava nella lista
 * "pagine" e in PagOpen[], e riaprendola config andava in crash.
 *----------------------------------------------------------------------*/

#define IMPR_PAG 0          /* parte salvata da pagina_save (.pag) */
#define IMPR_BKG 1          /* parte salvata da WriteBackground (.bkg) */
#define MAX_IMPRONTE 64

static struct {
   PAGINA *pag;
   char *parte[2];
} impronte[MAX_IMPRONTE];

static int cmp_righe(const void *a, const void *b)
{
   return strcmp(*(char * const *)a, *(char * const *)b);
}

/* legge il file (e lo cancella); restituisce testa + contenuto,
   con le righe ordinate se ordina */
static char *leggi_impronta(char *nome, Boolean ordina, char *testa)
{
   FILE *fp;
   long len;
   char *buf, *out, **righe, *r;
   int n = 0, i;

   if( (fp = fopen(nome,"r")) == NULL )
      return(XtNewString(testa));
   fseek(fp,0L,SEEK_END);
   len = ftell(fp);
   rewind(fp);
   buf = XtMalloc(len+1);
   len = fread(buf,1,len,fp);
   buf[len] = 0;
   fclose(fp);
   unlink(nome);

   out = XtMalloc(len + strlen(testa) + 2);
   strcpy(out,testa);
   if(!ordina)
   {
      strcat(out,buf);
      XtFree(buf);
      return(out);
   }

   for(i=0;i<len;i++)
      if(buf[i] == '\n')
         n++;
   righe = (char **)XtMalloc((n+1)*sizeof(char *));
   n = 0;
   for(r = strtok(buf,"\n"); r != NULL; r = strtok(NULL,"\n"))
      righe[n++] = r;
   qsort(righe,n,sizeof(char *),cmp_righe);
   for(i=0;i<n;i++)
   {
      strcat(out,righe[i]);
      strcat(out,"\n");
   }
   XtFree((char *)righe);
   XtFree(buf);
   return(out);
}

static char *file_impronta(char *nome)
{
   int fd;

   strcpy(nome,"/tmp/config_imprXXXXXX");
   if( (fd = mkstemp(nome)) < 0 )
      return(NULL);
   close(fd);
   return(nome);
}

/* l'impronta di una parte della pagina: NULL se non si riesce a farla */
static char *PagImpronta(PAGINA *pag, int parte)
{
   extern void pag_wgeometry();
   extern Boolean PutResPaginaDb();
   extern void WriteBackground();
   extern void get_child();
   char nome[64], nome2[64], *a, *b, *c, *tutto;
   XrmDatabase dbpag = NULL;
   WidgetList children;
   Cardinal nchildren = 0, i;
   FILE *fp;

   if(pag == NULL || pag->drawing == NULL)
      return(NULL);

   if(parte == IMPR_BKG)
   {
      if(file_impronta(nome) == NULL)
         return(NULL);
      WriteBackground(pag->drawing,nome);
      a = leggi_impronta(nome,False,"[bkg]\n");
/* le prime 4 righe (x_min_d ... y_max_d) sono il riquadro del disegno in
   pixel dello zoom corrente: cambiano con lo zoom, non con il disegno */
      for(b = a + strlen("[bkg]\n"), i = 0; i < 4 && (c = strchr(b,'\n')) != NULL; i++)
         b = c + 1;
      memmove(a + strlen("[bkg]\n"), b, strlen(b) + 1);
      return(a);
   }

/* database delle risorse degli oggetti */
   if(file_impronta(nome) == NULL)
      return(NULL);
   if(pag->db != NULL)
      XrmPutFileDatabase(pag->db,nome);
   a = leggi_impronta(nome,True,"[db]\n");

/* dati della pagina (intestazione del .pag) */
   if(file_impronta(nome) == NULL || file_impronta(nome2) == NULL)
   {
      XtFree(a);
      return(NULL);
   }
   if( (fp = fopen(nome,"w")) != NULL )
   {
      pag_wgeometry(fp,pag);
      fclose(fp);
   }
   PutResPaginaDb(pag,&dbpag);
   if(dbpag != NULL)
   {
      XrmPutFileDatabase(dbpag,nome2);
      XrmDestroyDatabase(dbpag);
   }
   b = leggi_impronta(nome2,False,"");
   if( (fp = fopen(nome,"a")) != NULL )
   {
      fputs(b,fp);
      fclose(fp);
   }
   XtFree(b);
   b = leggi_impronta(nome,True,"[pag]\n");

/* gli oggetti presenti: cancellati e aggiunti */
   get_child(pag->drawing,&children,&nchildren);
   c = XtMalloc(nchildren*(L_NOME_OGGETTI+L_CLASSE_OGGETTI+5) + 16);
   strcpy(c,"[obj]\n");
   for(i=0;i<nchildren;i++)
   {
      strncat(c,XtName(children[i]),L_NOME_OGGETTI);
      strcat(c," ");
      strncat(c,XlGetClassName(XtClass(children[i])),L_CLASSE_OGGETTI);
      strcat(c,"\n");
   }

   tutto = XtMalloc(strlen(a)+strlen(b)+strlen(c)+1);
   strcpy(tutto,a);
   strcat(tutto,b);
   strcat(tutto,c);
   XtFree(a);
   XtFree(b);
   XtFree(c);
   return(tutto);
}

static int PagImprontaIndice(PAGINA *pag, Boolean crea)
{
   int i;

   if(pag == NULL)
      return(-1);
   for(i=0;i<MAX_IMPRONTE;i++)
      if(impronte[i].pag == pag)
         return(i);
   if(!crea)
      return(-1);
   for(i=0;i<MAX_IMPRONTE;i++)
      if(impronte[i].pag == NULL)
      {
         impronte[i].pag = pag;
         impronte[i].parte[0] = impronte[i].parte[1] = NULL;
         return(i);
      }
   return(-1);
}

/* fotografa la pagina com'e' ora (parte: IMPR_PAG, IMPR_BKG, -1 tutte);
   crea: False per aggiornare solo una pagina gia' registrata (aperta) */
void PagImprontaAggiorna(PAGINA *pag, int parte, Boolean crea)
{
   int i, p;

   if( (i = PagImprontaIndice(pag,crea)) < 0 )
      return;
   for(p=0;p<2;p++)
      if(parte < 0 || parte == p)
      {
         XtFree(impronte[i].parte[p]);
         impronte[i].parte[p] = PagImpronta(pag,p);
      }
}

void PagImprontaTogli(PAGINA *pag)
{
   int i;

   if( (i = PagImprontaIndice(pag,False)) < 0 )
      return;
   XtFree(impronte[i].parte[0]);
   XtFree(impronte[i].parte[1]);
   impronte[i].pag = NULL;
   impronte[i].parte[0] = impronte[i].parte[1] = NULL;
}

/* True se la pagina e' cambiata dall'apertura o dall'ultimo salvataggio */
Boolean PagModificata(PAGINA *pag)
{
   int i, p;
   char *ora;
   Boolean diversa = False;

   if( (i = PagImprontaIndice(pag,False)) < 0 )
      return(False);
   for(p=0;p<2 && !diversa;p++)
   {
      if(impronte[i].parte[p] == NULL)
         continue;
      ora = PagImpronta(pag,p);
      if(ora != NULL && strcmp(ora,impronte[i].parte[p]) != 0)
      {
         diversa = True;
/* diagnostica: CONFIG_IMPRONTE=dir scrive le due impronte che differiscono */
         if(getenv("CONFIG_IMPRONTE") != NULL)
         {
            char f[MAXLENFILENAME];
            FILE *fp;
            sprintf(f,"%s/%s_%d_prima.txt",getenv("CONFIG_IMPRONTE"),pag->nomepag,p);
            if( (fp = fopen(f,"w")) != NULL ) { fputs(impronte[i].parte[p],fp); fclose(fp); }
            sprintf(f,"%s/%s_%d_ora.txt",getenv("CONFIG_IMPRONTE"),pag->nomepag,p);
            if( (fp = fopen(f,"w")) != NULL ) { fputs(ora,fp); fclose(fp); }
         }
      }
      XtFree(ora);
   }
   return(diversa);
}

/* salva la pagina (.pag e, se non e' una libreria, .bkg): e' File -> Save */
Boolean PagSalva(PAGINA *pag)
{
   extern PAGEDIT_CONTEXT *pagedit;
   extern Boolean pagina_save();
   extern void WriteBackground();
   extern Boolean CompressData();
   char fileback[MAXLENFILENAME];
   Boolean ok = False;

   if(pag == NULL)
      return(False);

   CompressData(pag);

   if( (!strcmp(pag->geom.tipo,TIPO_SINOTTICO)) ||
       (!strcmp(pag->geom.tipo,TIPO_STAZIONI))  ||
       (!strcmp(pag->geom.tipo,TIPO_REGOLAZIONE)) ||
       (!strcmp(pag->geom.tipo,TIPO_TELEPERM)) )
   {
      if( (pagina_save(pag,TIPO_PAGINA,1)) != False)
      {
         strcpy(fileback,pagedit->pages);
         strcat(fileback,"/");
         strcat(fileback,pag->filebkg);
         strcpy(fileback,XlConvPathVms(fileback));
         WriteBackground(pag->drawing,fileback);
         ok = True;
      }
   }
   else if( !strcmp(pag->geom.tipo,TIPO_LIBRERIA) )
   {
      ok = pagina_save(pag,TIPO_LIBRARY,1);
   }

   if(ok)
      PagImprontaAggiorna(pag,-1,False);
   return(ok);
}

static PAGINA *PagDaNome(char *nome)
{
   PAGINA *p;

   for(p=pagine; p != NULL; p=p->pagsucc)
      if(!strcmp(p->nomepag,nome))
         return(p);
   return(NULL);
}

/* risposte della finestra "pagina modificata"; client_data = nome pagina */
#define CHIUDI_SALVA   1
#define CHIUDI_SCARTA  2

static void chiudi_risposta(Widget w, XtPointer client_data, int azione)
{
   char *nome = (char *)client_data;
   PAGINA *pag = PagDaNome(nome);
   Widget dialog = w;

   while(dialog != NULL && !XtIsSubclass(dialog,xmMessageBoxWidgetClass))
      dialog = XtParent(dialog);
   if(dialog != NULL)
      XtDestroyWidget(XtParent(dialog));

   if(pag != NULL && azione != 0)
   {
      if(azione == CHIUDI_SALVA && !PagSalva(pag))
         PagStatusMsg(pag->drawing,2,"Save failed: page not closed");
      else
         close_page(pag->nomepag);
   }
}

static void chiudi_salva(Widget w, XtPointer cd, XtPointer cb)  { chiudi_risposta(w,cd,CHIUDI_SALVA); }
static void chiudi_scarta(Widget w, XtPointer cd, XtPointer cb) { chiudi_risposta(w,cd,CHIUDI_SCARTA); }
static void chiudi_annulla(Widget w, XtPointer cd, XtPointer cb) { chiudi_risposta(w,cd,0); }
static void chiudi_libera(Widget w, XtPointer cd, XtPointer cb)  { XtFree((char *)cd); }

/* File -> Close e la X della finestra: se la pagina e' cambiata chiede
   Save / Discard / Cancel, altrimenti chiude subito */
void PagChiudiRichiesta(PAGINA *pag)
{
   extern Boolean StateInterfaceMode;
   Widget dialog, scarta;
   XmString msg, ok, annulla, lab;
   char testo[400], *nome;
   Arg args[6];
   int n;
   Boolean salvabile;

   if(pag == NULL)
      return;

   if( !PagModificata(pag) )
   {
      close_page(pag->nomepag);
      return;
   }

   salvabile = !StateInterfaceMode && flag_demo != 1;
   if(salvabile)
      sprintf(testo,"Page %s has unsaved changes.\n\nSave them before closing?",
              pag->nomepag);
   else
      sprintf(testo,"Page %s has unsaved changes,\nbut saving is disabled%s.\n\nDiscard them and close?",
              pag->nomepag, StateInterfaceMode ? " in Interface Mode" : "");

   msg = XmStringCreateLtoR(testo,XmFONTLIST_DEFAULT_TAG);
   ok = XmStringCreateSimple("Save");
   annulla = XmStringCreateSimple("Cancel");
   n = 0;
   XtSetArg(args[n],XmNmessageString,msg); n++;
   XtSetArg(args[n],XmNokLabelString,ok); n++;
   XtSetArg(args[n],XmNcancelLabelString,annulla); n++;
   XtSetArg(args[n],XmNdialogStyle,XmDIALOG_FULL_APPLICATION_MODAL); n++;
   XtSetArg(args[n],XmNdefaultButtonType,XmDIALOG_CANCEL_BUTTON); n++;
   dialog = XmCreateQuestionDialog(pag->topwidget,"PagChiudi",args,n);
   XmStringFree(msg);
   XmStringFree(ok);
   XmStringFree(annulla);
   XtVaSetValues(XtParent(dialog),XmNtitle,"Close page",NULL);

   XtUnmanageChild(XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON));
   if(!salvabile)
      XtSetSensitive(XmMessageBoxGetChild(dialog,XmDIALOG_OK_BUTTON),False);

/* il terzo bottone: Motif lo mette dopo OK, cioe' Save | Discard | Cancel */
   lab = XmStringCreateSimple("Discard");
   scarta = XtVaCreateManagedWidget("Discard",xmPushButtonWidgetClass,dialog,
                                    XmNlabelString,lab,NULL);
   XmStringFree(lab);

   nome = XtNewString(pag->nomepag);
   XtAddCallback(dialog,XmNokCallback,chiudi_salva,nome);
   XtAddCallback(dialog,XmNcancelCallback,chiudi_annulla,nome);
   XtAddCallback(scarta,XmNactivateCallback,chiudi_scarta,nome);
   XtAddCallback(dialog,XmNdestroyCallback,chiudi_libera,nome);

   XtManageChild(dialog);
}

/*----------------------------------------------------------------------
 * Uscita da config con pagine modificate aperte (ottobre 2026).
 *
 * PagUscitaControllo: se ci sono pagine aperte con modifiche non salvate
 * mostra l'elenco e chiede Save all / Discard / Cancel. Con Save all o
 * Discard richiama prosegui(1) (l'utente ha gia' deciso: non serve
 * ridomandare "vuoi uscire?"); con Cancel non fa niente. Restituisce
 * False se non c'e' niente da chiedere: il chiamante prosegue da se'.
 *----------------------------------------------------------------------*/
static void (*uscita_prosegui)(int) = NULL;

static void uscita_risposta(Widget w, XtPointer cd, XtPointer cb)
{
   int azione = (int)(long)cd;
   Widget dialog = w;
   PAGINA *p;
   char msg[200];

   while(dialog != NULL && !XtIsSubclass(dialog,xmMessageBoxWidgetClass))
      dialog = XtParent(dialog);
   if(dialog != NULL)
      XtDestroyWidget(XtParent(dialog));

   if(azione == 0)
      return;

   if(azione == CHIUDI_SALVA)
      for(p=pagine; p != NULL; p=p->pagsucc)
         if( PagModificata(p) && !PagSalva(p) )
         {
            sprintf(msg,"Save of page %s failed: exit cancelled",p->nomepag);
            PagStatusMsg(p->drawing,2,msg);
            return;
         }

   if(uscita_prosegui != NULL)
      (*uscita_prosegui)(1);
}

Boolean PagUscitaControllo(Widget parent, void (*prosegui)(int))
{
   extern Boolean StateInterfaceMode;
   PAGINA *p;
   Widget dialog, scarta;
   XmString msg, ok, annulla, lab;
   char *testo;
   int n, quante = 0, len = 300;
   Arg args[6];
   Boolean salvabile;

   for(p=pagine; p != NULL; p=p->pagsucc)
      if( PagModificata(p) )
      {
         quante++;
         len += strlen(p->nomepag) + 4;
      }
   if(quante == 0)
      return(False);

   uscita_prosegui = prosegui;
   salvabile = !StateInterfaceMode && flag_demo != 1;

   testo = XtMalloc(len);
   strcpy(testo, quante == 1 ? "This page has unsaved changes:\n\n"
                             : "These pages have unsaved changes:\n\n");
   n = 0;
   for(p=pagine; p != NULL; p=p->pagsucc)
      if( PagModificata(p) )
      {
         strcat(testo,"   ");
         strcat(testo,p->nomepag);
         strcat(testo,"\n");
      }
   if(salvabile)
      strcat(testo,"\nSave them before exiting?");
   else
      strcat(testo, StateInterfaceMode ?
         "\nSaving is disabled in Interface Mode.\nDiscard them and exit?" :
         "\nSaving is disabled.\nDiscard them and exit?");

   msg = XmStringCreateLtoR(testo,XmFONTLIST_DEFAULT_TAG);
   XtFree(testo);
   ok = XmStringCreateSimple("Save all");
   annulla = XmStringCreateSimple("Cancel");
   n = 0;
   XtSetArg(args[n],XmNmessageString,msg); n++;
   XtSetArg(args[n],XmNokLabelString,ok); n++;
   XtSetArg(args[n],XmNcancelLabelString,annulla); n++;
   XtSetArg(args[n],XmNdialogStyle,XmDIALOG_FULL_APPLICATION_MODAL); n++;
   XtSetArg(args[n],XmNdefaultButtonType,XmDIALOG_CANCEL_BUTTON); n++;
   dialog = XmCreateQuestionDialog(parent,"PagUscita",args,n);
   XmStringFree(msg);
   XmStringFree(ok);
   XmStringFree(annulla);
   XtVaSetValues(XtParent(dialog),XmNtitle,"Exit",NULL);

   XtUnmanageChild(XmMessageBoxGetChild(dialog,XmDIALOG_HELP_BUTTON));
   if(!salvabile)
      XtSetSensitive(XmMessageBoxGetChild(dialog,XmDIALOG_OK_BUTTON),False);

   lab = XmStringCreateSimple("Discard");
   scarta = XtVaCreateManagedWidget("Discard",xmPushButtonWidgetClass,dialog,
                                    XmNlabelString,lab,NULL);
   XmStringFree(lab);

   XtAddCallback(dialog,XmNokCallback,uscita_risposta,(XtPointer)(long)CHIUDI_SALVA);
   XtAddCallback(dialog,XmNcancelCallback,uscita_risposta,(XtPointer)0);
   XtAddCallback(scarta,XmNactivateCallback,uscita_risposta,(XtPointer)(long)CHIUDI_SCARTA);

   XtManageChild(dialog);
   return(True);
}

/* la X del window manager: come File -> Close */
static void chiusura_wm_pagina(Widget w, XtPointer client_data, XtPointer cb)
{
   PAGINA *p;

   for(p=pagine; p != NULL; p=p->pagsucc)
      if(p->topwidget == w)
      {
         PagChiudiRichiesta(p);
         return;
      }
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


   PagImprontaTogli(pappo);
   UxDestroyInterface(shell);
pagina_free(pappo);
}

int SetItemString(XmString *Xstring,char *nome,char *tipo,char *descr,int in_use,
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
   set_something_val(SaveTmp,XmNsensitive,(XtArgVal)  False);
   set_something_val(ConnectMode,XmNsensitive,(XtArgVal) False);
   set_connect(actual_page->drawing,(int)1);
}

void PageMenuSetInterfaceModeOff()
{
   set_something_val(SaveTmp,XmNsensitive,(XtArgVal) True);
}

void SetButtonClosePage(Boolean Stato)
{
   if( (Stato == False) || (Stato == True))
      set_something_val(CloseTmp,XmNsensitive,(XtArgVal) Stato);
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
	   extern void draget_duplicate();
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
	   extern void DrawDelete();
	
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
	   extern void draget_cut();
	
	   extern int select_connections();
	   extern Widget ClipBoardForCopy;
	   extern void WriteBackground();
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
	   extern void draget_copy();
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
	  extern void draget_paste();
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
	   extern void draget_cut();
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
	   extern void draget_copy();
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
	  extern void draget_paste();
	
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
	   extern void DrawPutBottom();
	
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
	   extern void DrawPutTop();
	
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
#ifndef DESIGN_TIME
	   PagSalva(actual_page);
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
	   PagChiudiRichiesta(actual_page);
	
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
	   extern void draget_duplicate();
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

/*
	La riga di stato in fondo alla pagina (PagStatus, ottobre 2026): le
	notifiche che non devono fermare il lavoro con un popup, in primo
	luogo i clic rifiutati nel collegare le porte, che prima finivano solo
	su stderr. Il colore dice il livello (0 verde: fatto; 1 giallo:
	rifiutato, con un bip; 2 rosso: errore) e dopo qualche secondo torna
	normale; il testo resta fino alla notifica successiva.
	w e' un qualunque widget della pagina.
*/
#define PAGSTATUS_MS_OK      6000
#define PAGSTATUS_MS_AVVISO 12000

static void PagStatusNormale(XtPointer client_data, XtIntervalId *id)
{
	Widget lab = (Widget) client_data;
	Pixel bg, fg;

	XtVaGetValues(XtParent(lab), XmNbackground, &bg,
	              XmNforeground, &fg, NULL);
	XtVaSetValues(lab, XmNbackground, bg, XmNforeground, fg,
	              XmNuserData, (XtPointer) 0, NULL);
}

static void PagStatusDistrutto(Widget lab, XtPointer cd, XtPointer cb)
{
	XtPointer id = NULL;

	XtVaGetValues(lab, XmNuserData, &id, NULL);
	if(id != NULL)
		XtRemoveTimeOut((XtIntervalId) id);
}

void PagStatusMsg(Widget w, int livello, char *msg)
{
	static char *sfondo[] = { "PaleGreen", "Yellow", "Red" };
	static char *testo[]  = { "Black", "Black", "White" };
	Widget shell, lab;
	XtPointer id = NULL;
	XmString xs;

	if(w == NULL || msg == NULL)
		return;
	if(livello < 0) livello = 0;
	if(livello > 2) livello = 2;
	for(shell = w; shell != NULL && !XtIsShell(shell);
	    shell = XtParent(shell))
		;
	lab = (shell != NULL) ? XtNameToWidget(shell, "*PagStatus") : NULL;
	if(lab == NULL)
		{
		fprintf(stderr, "config: %s\n", msg);
		return;
		}
	xs = XmStringCreateSimple(msg);
	XtVaSetValues(lab, XmNlabelString, xs,
	        XtVaTypedArg, XmNbackground, XmRString,
	            sfondo[livello], strlen(sfondo[livello]) + 1,
	        XtVaTypedArg, XmNforeground, XmRString,
	            testo[livello], strlen(testo[livello]) + 1,
	        NULL);
	XmStringFree(xs);
	if(livello > 0)
		XBell(XtDisplay(lab), 0);
	XtVaGetValues(lab, XmNuserData, &id, NULL);
	if(id != NULL)
		XtRemoveTimeOut((XtIntervalId) id);
	id = (XtPointer) XtAppAddTimeOut(XtWidgetToApplicationContext(lab),
	        livello == 0 ? PAGSTATUS_MS_OK : PAGSTATUS_MS_AVVISO,
	        PagStatusNormale, (XtPointer) lab);
	XtVaSetValues(lab, XmNuserData, id, NULL);
	XmUpdateDisplay(lab);
}

/*
	Zoom con Ctrl + rotella del mouse (ottobre 2026), in aggiunta a
	Option -> Zoom: rotella su, gradino successivo (fino a 400%); rotella
	giu', precedente (fino a 100%). Il gradino si cambia "premendo" la voce
	del menu, cosi' passa dalla stessa callback e il menu resta giusto; poi
	si spostano le barre di scorrimento perche' il punto sotto il puntatore
	resti fermo.

	La rotella arriva come tasto 4/5 alla finestra piu' interna sotto il
	puntatore, cioe' spesso a un'icona, che la ignora: per questo non c'e'
	un gestore sulla drawing area ma un dispatcher degli eventi, che prende
	solo Ctrl + tasto 4/5 dentro una pagina e lascia passare tutto il resto
	(la rotella senza Ctrl continua a scorrere la pagina).
*/
static XtEventDispatchProc PagZoomDispatcherPrec = NULL;      /* ButtonPress */
static XtEventDispatchProc PagZoomDispatcherPrecRil = NULL;   /* ButtonRelease */

/* sposta la barra in modo che il punto della drawing area a coordinata
   pos_da finisca a distanza pos_clip dal bordo della finestra visibile */
static void PagZoomScorri(Widget sb, int pos_da, int pos_clip)
{
	int val, slider, inc, pageinc, min = 0, max = 0;

	if(sb == NULL || !XtIsManaged(sb))
		return;
	XtVaGetValues(sb, XmNminimum, &min, XmNmaximum, &max, NULL);
	XmScrollBarGetValues(sb, &val, &slider, &inc, &pageinc);
	val = min + pos_da - pos_clip;
	if(val > max - slider) val = max - slider;
	if(val < min) val = min;
	XmScrollBarSetValues(sb, val, slider, inc, pageinc, True);
}

/* torna True se l'evento era per lo zoom di una pagina, e l'ha usato */
static Boolean PagZoomRotella(Widget w, XEvent *ev)
{
	static float gradini[] = { 1.0, 2.0, 3.0, 4.0 };
	_UxCPagShell *ctx;
	Widget shell, toggle[4], clip = NULL, hsb = NULL, vsb = NULL;
	Window figlio;
	int i, attuale = 0, nuovo, dax, day, clx, cly;
	float zoom, rapporto;
	extern float get_def_zoom();

	for(shell = w; shell != NULL && !XtIsShell(shell);
	    shell = XtParent(shell))
		;
	if(shell == NULL || strcmp(XtName(shell), "PagShell") != 0)
		return(False);
	ctx = (_UxCPagShell *) UxGetContext(shell);
	if(ctx == NULL || ctx->UxPagDrawingArea == NULL)
		return(False);
	/* la pagina deve averlo nel menu (non e' cosi' per ogni tipo) */
	toggle[0] = ctx->UxZoom100TB; toggle[1] = ctx->UxZoom200TB;
	toggle[2] = ctx->UxZoom300TB; toggle[3] = ctx->UxZoom400TB;
	if(!XtIsSensitive(toggle[0]))
		return(True);

	zoom = get_def_zoom(ctx->UxPagDrawingArea);
	for(i = 0; i < 4; i++)
		if(zoom > gradini[i] - 0.01)
			attuale = i;
	nuovo = attuale + (ev->xbutton.button == Button4 ? 1 : -1);
	if(nuovo < 0 || nuovo > 3)
		return(True);

	/* il punto sotto il puntatore, nella drawing area e nella finestra
	   visibile (clip window), prima dello zoom */
	XtVaGetValues(ctx->UxPagScrolledWindow, XmNclipWindow, &clip,
	        XmNhorizontalScrollBar, &hsb, XmNverticalScrollBar, &vsb, NULL);
	XTranslateCoordinates(ev->xbutton.display, ev->xbutton.root,
	        XtWindow(ctx->UxPagDrawingArea),
	        ev->xbutton.x_root, ev->xbutton.y_root, &dax, &day, &figlio);
	clx = dax; cly = day;
	if(clip != NULL)
		XTranslateCoordinates(ev->xbutton.display, ev->xbutton.root,
		        XtWindow(clip), ev->xbutton.x_root, ev->xbutton.y_root,
		        &clx, &cly, &figlio);

	XmToggleButtonSetState(toggle[nuovo], True, True);

	rapporto = gradini[nuovo] / zoom;
	PagZoomScorri(hsb, (int)(dax * rapporto), clx);
	PagZoomScorri(vsb, (int)(day * rapporto), cly);
	return(True);
}

static Boolean PagZoomDispatcher(XEvent *ev)
{
	if((ev->type == ButtonPress || ev->type == ButtonRelease) &&
	   (ev->xbutton.button == Button4 || ev->xbutton.button == Button5) &&
	   (ev->xbutton.state & ControlMask))
		{
		Widget w = XtWindowToWidget(ev->xany.display, ev->xany.window);
		Boolean usato = False;

		if(w != NULL)
			{
			/* al rilascio non si fa niente, ma non deve arrivare
			   ad altri, se la pressione e' stata usata qui */
			if(ev->type == ButtonPress)
				usato = PagZoomRotella(w, ev);
			else
				{
				Widget sh;
				for(sh = w; sh != NULL && !XtIsShell(sh);
				    sh = XtParent(sh))
					;
				usato = (sh != NULL &&
				         strcmp(XtName(sh), "PagShell") == 0);
				}
			}
		if(usato)
			return(True);
		}
	if(ev->type == ButtonRelease)
		return((*PagZoomDispatcherPrecRil)(ev));
	return((*PagZoomDispatcherPrec)(ev));
}

static void PagZoomInstalla(Widget w)
{
	if(PagZoomDispatcherPrec != NULL)
		return;
	PagZoomDispatcherPrec = XtSetEventDispatcher(XtDisplay(w),
	        ButtonPress, PagZoomDispatcher);
	PagZoomDispatcherPrecRil = XtSetEventDispatcher(XtDisplay(w),
	        ButtonRelease, PagZoomDispatcher);
}

/*
	Edit -> Disconnect interface (e la stessa voce nel menu del tasto
	destro): toglie le connessioni verso altri schemi delle icone di
	interfaccia selezionate, scollega_interfacce in othercnf.c. Funziona
	anche fuori da Interface Mode (ottobre 2026).
*/
static	void	activateCB_Disconnect( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCPagShell            *UxSaveCtx, *UxContext;

	/*
	   il contesto della pagina da cui arriva il clic: senza, actual_page
	   sarebbe quella dell'ultimo contesto attivo, cioe' un'altra pagina
	   quando ce n'e' piu' d'una aperta
	*/
	UxSaveCtx = UxPagShellContext;
	UxPagShellContext = UxContext =
			(_UxCPagShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	extern void scollega_interfacce();

	if(WidSelectedAreInEdit(actual_page) == False)
	   scollega_interfacce(actual_page);
	else
	   SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget Is In Edit",NULL,False,NULL,False,NULL);
#endif
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
	   extern void DrawDelete();
	
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
	   extern void draget_cut();
	
	   extern int fcut();
	   extern int select_connections();
	   extern Widget ClipBoardForCopy;
	   extern void WriteBackground();
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
	   extern void draget_copy();
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
	  extern void draget_paste();
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
	   extern void draget_cut();
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
	   extern void draget_copy();
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
	  extern void draget_paste();
	
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
	   extern void DrawPutBottom();
	
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
	   extern void DrawPutTop();
	
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
	extern void compile_page();
	    
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
	               "PagShell.c: Topology database not defined",
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


	/* Creation of PagStatus: la riga di stato, vedi PagStatusMsg */
	{
	Widget PagStatus;
	PagStatus = XtVaCreateManagedWidget( "PagStatus",
			xmLabelWidgetClass,
			PagForm1,
			RES_CONVERT( XmNlabelString, " " ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNrecomputeSize, FALSE,
			XmNmarginHeight, 4,
			XmNmarginWidth, 8,
			XmNuserData, (XtPointer) 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( PagStatus, (char *) UxPagShellContext );
	XtAddCallback( PagStatus, XmNdestroyCallback,
		(XtCallbackProc) PagStatusDistrutto, NULL );
#ifndef DESIGN_TIME
	XlPortSetMsgHandler( PagStatusMsg );
	PagZoomInstalla( PagShell );
#endif

	/* Creation of PagScrolledWindow */
	PagScrolledWindow = XtVaCreateManagedWidget( "PagScrolledWindow",
			xmScrolledWindowWidgetClass,
			PagForm1,
			XmNscrollingPolicy, XmAUTOMATIC,
			XmNx, 0,
			XmNy, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNbottomWidget, PagStatus,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNnavigationType, XmNONE,
			XmNtraversalOn, FALSE,
			NULL );
	UxPutContext( PagScrolledWindow, (char *) UxPagShellContext );
	}


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

	{
	Widget w = XtVaCreateManagedWidget( "menu3Disconnect",
			xmPushButtonWidgetClass,
			menu3,
			RES_CONVERT( XmNlabelString, "Disconnect interface" ),
			NULL );
	XtAddCallback( w, XmNactivateCallback,
		(XtCallbackProc) activateCB_Disconnect,
		(XtPointer) UxPagShellContext );
	UxPutContext( w, (char *) UxPagShellContext );
	}


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

	{
	Widget w = XtVaCreateManagedWidget( "FDisconnect",
			xmPushButtonWidgetClass,
			FEdit,
			RES_CONVERT( XmNlabelString, "Disconnect interface" ),
			NULL );
	XtAddCallback( w, XmNactivateCallback,
		(XtCallbackProc) activateCB_Disconnect,
		(XtPointer) UxPagShellContext );
	UxPutContext( w, (char *) UxPagShellContext );
	}


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
		printf("create_PagShell: indice pagina chiamata %d\n",indice);
		if( pagina_init(nome_pagina,&actual_page,tipop) == False)
		{
		   printf("create_PagShell: errore nella inizializzazione della pagina\n");
		   return(NULL);
		}
		
		pagina_load_file(actual_page,tipop);
		
		UxDisplay->db = actual_page->db;
		
		if(  pagina_getres(actual_page,&lista_oggetti) == False)
		{
		   printf("create_PagShell: non ci sono risorse nel file della pagina\n");
		}
		printf("create_PagShell: ho terminato di caricare la pagina=%s\n",actual_page->filepag); 
		lista_draget = XdCreateLista();
		lista_draget_undo=NULL;
		lista_liste_undo=undoCreateLists();
		
		/*
		 Inizializza a valori di default alcuni campi della
		 struttura descrivente gli attributi del disegno
		*/
		init_st_draw(&st_draw);
		rtrn = _Uxbuild_PagShell();
		/* la X del window manager come File -> Close (vedi PagChiudiRichiesta) */
		XtVaSetValues(rtrn, XmNdeleteResponse, XmDO_NOTHING, NULL);
		XmAddWMProtocolCallback(rtrn,
			XmInternAtom(XtDisplay(rtrn),"WM_DELETE_WINDOW",False),
			(XtCallbackProc) chiusura_wm_pagina, NULL);

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
		   set_something_val(PagPane_top_b4,XmNsensitive,(XtArgVal) False);
		   set_something_val(PagPane_top_b6,XmNsensitive,(XtArgVal) False);
		   set_something_val(FEditForeground,XmNsensitive,(XtArgVal) False);
		   set_something_val(ConnectMode,XmNsensitive,(XtArgVal) False);
		}
		else if ( PagGetType(actual_page) == TYPE_REGOLAZIONE)
			{
			/**  la risorsa snap e' gestita a livello di pagina
			DrawSetSnap((Widget )PagDrawingArea,4);
			**/
			set_something_val(FUndo,XmNsensitive,(XtArgVal) False);
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
		        set_something_val(ConnectMode,XmNsensitive,(XtArgVal) False);
			set_something_val(CompileReg,XmNsensitive,(XtArgVal) False);
			set_something_val(Compile_b4,XmNsensitive,(XtArgVal) False);
			set_something_val(SetVisualMode,XmNsensitive,(XtArgVal) False);
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
		
		set_something_val(actual_page->topwidget,XmNtitle,(XtArgVal) titolo);
		/***
		if(!CompilerOk)
		   set_something_val(PagPane_top_b4,XmNsensitive, (XtArgVal) False);
		***/ 
		PostOpenPage(actual_page->drawing);
		
		if(StateInterfaceMode)
		{
		   PageMenuSetInterfaceModeOn();
		/**
		   set_something_val(SaveTmp,XmNsensitive, (XtArgVal) False);
		   set_something_val(Option_b2,XmNsensitive, (XtArgVal) False);
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
		
		
		/* la pagina com'e' all'apertura, per sapere alla chiusura se e' cambiata */
		PagImprontaAggiorna(actual_page, -1, True);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

