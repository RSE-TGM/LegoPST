! UIMX ascii 2.5 key: 7103                                                      

*PagShell.class: topLevelShell
*PagShell.gbldecl: /*\
        Inizio sezione per SCCS\
*/\
/*\
   modulo PagShell.i\
   tipo \
   release 5.1\
   data 11/13/95\
   reserved @(#)PagShell.i	5.1\
*/\
/*\
        Variabile per identificazione della versione\
*/\
/*\
        Fine sezione per SCCS\
*/\
\
#include <stdio.h>\
#include <stdlib.h>\
#include "config.h"\
#include <draw.h>\
#include <Xd/Xd.h>\
#include <Xd/XdLista.h>\
#include <Xd/XdUndo.h>\
#include <Ol/OlCompiler.h>\
#include <Xl/XlDispReg.h>\
#include <Xl/XlPort.h>\
#include "message.h"\
\
PAGINA *pagine = NULL; /* lista delle pagine */\
\
int NUM_ELENCHI;\
\
char **elenco_nomi;\
char **elenco_classi;\
\
\
extern OlDatabaseTopologiaObject dbtopologia;\
extern Boolean CompilerOk;\
extern void get_child();\
extern Boolean PrintPag();\
\
extern int flag_demo;\
\
int PrintClosePag();\
void close_page();\

*PagShell.ispecdecl: PAGINA *actual_page; /*pagina corrente */\
WidgetClass istanzia_widget_class;\
swidget *elenco_editor;\
swidget wset_snap;\
XdLista lista_draget;\
XdLista lista_draget_undo;\
XdListaUndo lista_liste_undo;\
\
/* struttura contenente gli attributi del disegno \
   quali snap, fill ecc.. */\
ST_DRAW_WID st_draw;\
Boolean editing_background;\
swidget draw_edit;\

*PagShell.ispeclist: actual_page, istanzia_widget_class, elenco_editor, wset_snap, lista_draget, lista_draget_undo, lista_liste_undo, st_draw, editing_background, draw_edit
*PagShell.ispeclist.actual_page: "PAGINA", "*%actual_page%"
*PagShell.ispeclist.istanzia_widget_class: "WidgetClass", "%istanzia_widget_class%"
*PagShell.ispeclist.elenco_editor: "swidget", "*%elenco_editor%"
*PagShell.ispeclist.wset_snap: "swidget", "%wset_snap%"
*PagShell.ispeclist.lista_draget: "XdLista", "%lista_draget%"
*PagShell.ispeclist.lista_draget_undo: "XdLista", "%lista_draget_undo%"
*PagShell.ispeclist.lista_liste_undo: "XdListaUndo", "%lista_liste_undo%"
*PagShell.ispeclist.st_draw: "ST_DRAW_WID", "%st_draw%"
*PagShell.ispeclist.editing_background: "Boolean", "%editing_background%"
*PagShell.ispeclist.draw_edit: "swidget", "%draw_edit%"
*PagShell.funcdecl: swidget create_PagShell(char *nome_pagina,int indice,int tipop)\

*PagShell.funcname: create_PagShell
*PagShell.funcdef: "swidget", "<create_PagShell>(%)"
*PagShell.argdecl: unsigned char *nome_pagina;\
int indice;\
int tipop;
*PagShell.arglist: nome_pagina, indice, tipop
*PagShell.arglist.nome_pagina: "unsigned char", "*%nome_pagina%"
*PagShell.arglist.indice: "int", "%indice%"
*PagShell.arglist.tipop: "int", "%tipop%"
*PagShell.icode: extern Boolean StateInterfaceMode;\
extern void AllPagWidSetTranslation();\
extern void PostOpenPage();\
extern int PagGetType();\
/* \
  Interfaccia di appoggio per la gestione della clipboard\
*/\
extern Widget ClipBoardUndo;\
XdLista get_lista_undo();\
XdListaUndo get_lista_liste_undo();\
void CreaUndoList();\
PAGINA *pappo;\
AN_OBJ *lista_oggetti = NULL;\
char titolo[100];\
\
wset_snap= NULL;\
elenco_editor = NULL;\
lista_draget = NULL;\
draw_edit=NULL;\
editing_background=False; /* indica che non e' stato ancora richiamato\
                             l'editor del background */\
printf("indice pagina chiamata %d\n",indice);\
if( pagina_init(nome_pagina,&actual_page,tipop) == False)\
{\
   printf("errore nella inizializzazione della pagina\n");\
   return(NULL);\
}\
\
pagina_load_file(actual_page,tipop);\
\
UxDisplay->db = actual_page->db;\
\
if(  pagina_getres(actual_page,&lista_oggetti) == False)\
{\
   printf("non ci sono risorse nel file della pagina\n");\
}\
printf("ho teminato di caricare la pagina=%s\n",actual_page->filepag); \
lista_draget = XdCreateLista();\
lista_draget_undo=NULL;\
lista_liste_undo=undoCreateLists();\
\
/*\
 Inizializza a valori di default alcuni campi della\
 struttura descrivente gli attributi del disegno\
*/\
init_st_draw(&st_draw);\

*PagShell.fcode: /* inizializzo la struttura PAGINA e carico le resource della medesima*/\
\
actual_page->topwidget = (Widget )rtrn;\
actual_page->drawing = (Widget )PagDrawingArea;\
actual_page->dispreg_visual_mode = DISPREG_TAG;\
actual_page->port_visual_mode = PORT_MANAGED;\
actual_page->interfaceport_visual_mode = INTERFACE_PORT_MANAGED;\
\
\
pagina_setta_resource(actual_page);\
pagina_create_widget(actual_page,lista_oggetti);\
\
/*\
   Caricamento dei draget presenti all'apertura della pagina nella struttura dedicata\
   all'operazione di 'undo'.\
*/\
CreaUndoList(actual_page->drawing);\
undo_draget_copy(actual_page->drawing,ClipBoardUndo);\
undoListAdd(get_lista_liste_undo(actual_page->drawing),get_lista_undo(actual_page->drawing));\
\
if( tipop == TIPO_LIBRARY)\
{\
   set_something(PagPane_top_b4,XmNsensitive,False);\
   set_something(PagPane_top_b6,XmNsensitive,False);\
   set_something(FEditForeground,XmNsensitive,False);\
   set_something(ConnectMode,XmNsensitive,False);\
}\
else if ( PagGetType(actual_page) == TYPE_REGOLAZIONE)\
	{\
	/**  la risorsa snap e' gestita a livello di pagina\
	DrawSetSnap((Widget )PagDrawingArea,4);\
	**/\
	set_something(FUndo,XmNsensitive,False);\
	}\
else if ( PagGetType(actual_page) != TYPE_REGOLAZIONE)\
	{\
/*\
 rende insensitive i bottoni tipici solo di pagine regolazione:\
	Connect Mode\
	Compile Regolation\
	Regolation Error View\
	Set Visual Mode\
*/\
        set_something(ConnectMode,XmNsensitive,False);\
	set_something(CompileReg,XmNsensitive,False);\
	set_something(Compile_b4,XmNsensitive,False);\
	set_something(SetVisualMode,XmNsensitive,False);\
	}\
\
\
/* aggiorno la lista delle pagine */\
pappo = pagine;\
if( pappo == NULL)\
   pagine = actual_page;\
else\
{\
  while(pagine->pagsucc != NULL)\
      pagine = pagine->pagsucc;\
\
  pagine->pagsucc = actual_page;\
  pagine = pappo;\
}\
\
AllPagWidSetTranslation( actual_page );\
\
\
/* adesso setto i bottoni del menu delle iconlibrary */\
add_objlib_button(rtrn);\
\
\
if(lista_oggetti != NULL)\
   libera_memoria(lista_oggetti);\
\
if(tipop == TIPO_PAGINA)\
   strcpy(titolo,"Page : ");\
else if(tipop == TIPO_LIBRARY)\
   strcpy(titolo,"Library : ");\
\
strcat(titolo,actual_page->nomepag);\
\
set_something(actual_page->topwidget,XmNtitle,titolo);\
/***\
if(!CompilerOk)\
   set_something(PagPane_top_b4,XmNsensitive,False);\
***/ \
PostOpenPage(actual_page->drawing);\
\
if(StateInterfaceMode)\
{\
   PageMenuSetInterfaceModeOn();\
/**\
   set_something(SaveTmp,XmNsensitive,False);\
   set_something(Option_b2,XmNsensitive,False);\
   set_connect(actual_page->drawing,(int)1);   \
*/\
}\
else\
   PageMenuSetInterfaceModeOff();\
\
/*\
  Nel caso in cui la licenza disponibile e' di tipo 'demo', vengono\
  disabilitate alcune funzionalita'; compilazione e\
  salvataggio delle pagine. \
*/\
   if(flag_demo == 1) {\
      XtVaSetValues(PagPane_top_b4, XmNsensitive, False, NULL);\
      XtVaSetValues(SaveTmp, XmNsensitive, False, NULL);\
   }\
\
\
return(rtrn);\

*PagShell.auxdecl: /*--------------------------------------------------------------------\
 *\
 * iconlib_buttCB\
 *\
 * Description:\
 *               chiama la crea finestra delle icone di libreria\
 *               callback agganciata ai bottoni iconlib sulla pagia\
 *\
 * Parameter:\
 *               Widget w\
 *               int ind_butt\
 *               XmAnyCallbackStruct *call_data\
 *\
 *-------------------------------------------------------------------*/\
\
iconlib_buttCB(Widget w,int ind_butt, XmAnyCallbackStruct *call_data)\
{\
#ifndef DESIGN_TIME\
   _UxCPagShell      *UxSaveCtx, *UxContext;\
   swidget UxThisWidget;\
#endif \
\
   extern PAGEDIT_CONTEXT *pagedit;\
   extern swidget create_IconShell();\
   swidget wid;\
   char nomelib[100];\
\
#ifndef DESIGN_TIME\
   UxSaveCtx = UxPagShellContext;\
   UxThisWidget = w;\
   UxSaveCtx = UxPagShellContext;\
   UxPagShellContext = UxContext = (_UxCPagShell *) UxGetContext( UxThisWidget  );\
#endif\
\
\
   strcpy(nomelib,pagedit->iconlib_list[ind_butt]);\
   wid = create_IconShell( nomelib );\
   UxPopupInterface(wid,no_grab);\
\
\
#ifndef DESIGN_TIME\
   UxPagShellContext = UxSaveCtx;\
#endif\
\
}\
\
\
/*--------------------------------------------------------------------\
 *\
 * add_objlib_button\
 *\
 * Description:\
 *               aggiunge bottoni di attivazione iconlib su\
 *               una pagina e ne definisce anche la callback\
 *\
 * Parameter:\
 *               swidget wid\
 *               \
 *-------------------------------------------------------------------*/\
\
add_objlib_button(swidget wid)\
{\
   extern PAGEDIT_CONTEXT *pagedit;\
   swidget *iconlibButt;\
   int i,j,num_butt;\
   extern Arg args[];\
   extern Cardinal lnargs;\
\
   \
   XtDestroyWidget(Icon);\
\
   num_butt = pagedit->iconlib_num;\
   if((iconlibButt = (swidget *)alloca_memoria( num_butt, sizeof(swidget) )) == NULL)\
      return;\
\
   for(i=0;i<num_butt;i++)\
   {\
      \
      lnargs = 0;\
      XtSetArg(args[lnargs],XmNsensitive, True); lnargs++;\
      iconlibButt[i] = XmCreatePushButton (AddObj,pagedit->iconlib_label[i],args, lnargs);\
\
#ifndef DESIGN_TIME\
   UxPutContext (iconlibButt[i] ,(char *)UxPagShellContext);\
#endif      \
      XtAddCallback(iconlibButt[i], XmNactivateCallback, iconlib_buttCB,i);\
\
   }\
   XtManageChildren(iconlibButt, num_butt);\
\
   libera_memoria(iconlibButt);\
\
}\
\
\
\
/*--------------------------------------------------------------------\
 *\
 * assign_name\
 *\
 * Description:\
 *               assegna un nome alla nuova istanza di un oggetto\
 *               - attualmente sviluppata in modo semplicistico -\
 *\
 * Parameter:\
 *               PAGINA *pag;\
 *               WidgetClass wcl\
 *               char *nuovo_nome;\
 *\
 *-------------------------------------------------------------------*/\
void assign_name(PAGINA *pag,int dove,Widget parent,char *nuovo_nome)\
{\
   Cardinal num_child;\
   char *name_parent;\
   char nome_figlio[100];\
 \
   if(dove == SUDRAWING)\
   {\
      sprintf(nuovo_nome,"%d",pag->next_num);\
      strcat(nuovo_nome,"w");\
   }\
   else\
   {\
      name_parent = XtName(parent);\
      strcpy(nuovo_nome,name_parent);       \
      sprintf(nome_figlio,"%d",pag->next_num);\
      strcat(nuovo_nome,nome_figlio);\
      strcat(nuovo_nome,"c");\
   }\
   pag->next_num++;\
}\
\
/*-----------------------------------\
 * salva_indice_editor\
 *\
 *-----------------------------------*/ \
\
void salva_indice_editor(swidget edit)\
{\
   int num_editor;\
   int i;\
\
   num_editor = 1;\
  \
   i=0;\
   if(elenco_editor != NULL)\
   {\
      while(  elenco_editor[i] != NULL)\
      {\
         i++;\
         num_editor++;\
      }\
\
      elenco_editor = (swidget *)XtRealloc(elenco_editor, sizeof(swidget)* (num_editor+1));\
      elenco_editor[num_editor-1] = edit; \
      elenco_editor[num_editor] = NULL;\
   }\
}\
\
/*-------------------------------\
 *\
 * get_lista\
 * ricava la lista dei draget che descrivono il background \
 * della pagina.\
 *--------------------------------*/\
\
XdLista get_lista(Widget w)\
{\
#ifndef DESIGN_TIME\
   XdLista ret_lista;\
   _UxCPagShell     *UxSaveCtx, *UxContext;\
\
    UxSaveCtx =  UxPagShellContext ;\
    UxPagShellContext = UxContext =\
               (_UxCPagShell *) UxGetContext( w );\
\
   ret_lista= lista_draget;\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   return(ret_lista);\
#endif\
}\
\
XdLista get_lista_undo(Widget w)\
{\
#ifndef DESIGN_TIME\
   XdLista ret_lista;\
   _UxCPagShell     *UxSaveCtx, *UxContext;\
\
    UxSaveCtx =  UxPagShellContext ;\
    UxPagShellContext = UxContext =\
               (_UxCPagShell *) UxGetContext( w );\
\
   ret_lista= lista_draget_undo;\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   return(ret_lista);\
#endif\
}\
\
\
void CreaUndoList(w)\
Widget w;\
{\
#ifndef DESIGN_TIME\
        _UxCPagShell   *UxSaveCtx, *UxContext;\
\
        UxSaveCtx = UxPagShellContext;\
        UxPagShellContext = UxContext =\
                        (_UxCPagShell *) UxGetContext( w );\
#endif\
if(lista_draget_undo != NULL)\
        XtFree(lista_draget_undo);\
\
lista_draget_undo=XdCreateLista();\
#ifndef DESIGN_TIME\
 UxPagShellContext = UxSaveCtx;\
#endif\
}\
\
XdListaUndo get_lista_liste_undo(Widget w)\
{\
#ifndef DESIGN_TIME\
   XdListaUndo ret_lista;\
   _UxCPagShell     *UxSaveCtx, *UxContext;\
\
    UxSaveCtx =  UxPagShellContext ;\
    UxPagShellContext = UxContext =\
               (_UxCPagShell *) UxGetContext( w );\
\
   ret_lista= lista_liste_undo;\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   return(ret_lista);\
#endif\
}\
\
\
/*\
 restituisce o setta una risorsa tra quelle definite\
 nella struttura descrittiva delle risorse per il widget di disegno\
 (ST_DRAW_WID) definita in draw.h (in legommi/include)\
*/\
int gs_draw_ris(Widget w,enum ris_draw_wid ris_num, void *ret,Boolean set)\
{\
int *retint;\
GC *retGC;\
Widget *retWid;\
float *retfloat;\
Region *retreg;\
\
#ifndef DESIGN_TIME\
        _UxCPagShell   *UxSaveCtx, *UxContext;\
        UxSaveCtx = UxPagShellContext;\
        UxPagShellContext = UxContext =\
                        (_UxCPagShell *) UxGetContext( w );\
#endif\
switch(ris_num)\
        {\
	case DR_REGION:\
	retreg=ret;\
	if(set)\
                st_draw.region=*retreg;\
        else\
                *retreg=st_draw.region;\
        break;\
        case DR_LINE_STYLE:\
        retint=ret;\
        if(set)\
                st_draw.line_style=*retint;\
        else\
                *retint=st_draw.line_style;\
        break;\
        case DR_LINE_WIDTH:\
        retint=ret;\
        if(set)\
                st_draw.line_width=*retint;\
        else\
                *retint=st_draw.line_width;\
        break;\
        case DR_DEF_LINE_STYLE:\
        retint=ret;\
        if(set)\
                st_draw.def_line_style=*retint;\
        else\
                *retint=st_draw.def_line_style;\
        break;\
        case DR_DEF_LINE_WIDTH:\
        retint=ret;\
        if(set)\
                st_draw.def_line_width=*retint;\
        else\
                *retint=st_draw.def_line_width;\
        break;\
        case DR_GC:\
        retGC=ret;\
        if(set)\
                st_draw.gc=*retGC;\
        else\
                *retGC= st_draw.gc;\
        break;\
        case DR_GC_BG:\
        retGC=ret;\
        if(set)\
                st_draw.gc_bg=*retGC;\
        else\
                *retGC= st_draw.gc_bg;\
        break;\
        case DR_DEF_GC:\
        retGC=ret;\
        if(set)\
                st_draw.def_gc=*retGC;\
        else\
                *retGC= st_draw.def_gc;\
        break;\
        case DR_DEF_GC_BG:\
        retGC=ret;\
        if(set)\
                st_draw.def_gc_bg=*retGC;\
        else\
                *retGC= st_draw.def_gc_bg;\
        break;\
        case DR_STEP:\
        retint=ret;\
        if(set)\
                st_draw.step= *retint;\
        else\
                *retint=st_draw.step;\
        break;\
        case DR_GRID_ON:\
        retint=ret;\
        if(set)\
                st_draw.grid_on= *retint;\
        else\
                *retint=st_draw.grid_on;\
        break;\
        case DR_DEF_FILLED:\
        retint=ret;\
        if(set)\
                st_draw.def_filled= *retint;\
        else\
                *retint=st_draw.def_filled;\
        break;\
        case DR_DEF_ZOOM:\
        retfloat=ret;\
        if(set)\
                st_draw.def_zoom=*retfloat;\
        else\
                *retfloat=st_draw.def_zoom;\
        break;\
        case DR_BOTT_FILL:\
        retWid=ret;\
        if(set)\
                st_draw.bott_fill= *retWid;\
        else\
                *retWid= st_draw.bott_fill;\
        break;\
        case DR_BOTT_BORDER:\
        retWid=ret;\
        if(set)\
                st_draw.bott_border= *retWid;\
        else\
                *retWid= st_draw.bott_border;\
        break; \
        case DR_DEF_BOTT_FILL:\
        retWid=ret;\
        if(set)\
                st_draw.def_bott_fill= *retWid;\
        else\
                *retWid= st_draw.def_bott_fill;\
        break;\
        case DR_DEF_BOTT_BORDER:\
        retWid=ret;\
        if(set)\
                st_draw.def_bott_border= *retWid;\
        else\
                *retWid= st_draw.def_bott_border;\
        break;\
        case DR_BOTT_ARROW:\
        retWid=ret;\
        if(set)\
                st_draw.bott_arrow= *retWid;\
        else\
                *retWid= st_draw.bott_arrow;\
        break;\
        }\
#ifndef DESIGN_TIME\
 UxPagShellContext = UxSaveCtx;\
#endif\
return(ret);\
}\
\
\
\
/*\
 reset_drawing_background \
 setta a False il flag che indica l'utilizzo in corso\
 del Draw per disegnare il background\
*/\
reset_drawing_background(Widget w)\
{\
#ifndef DESIGN_TIME\
\
   _UxCPagShell     *UxSaveCtx, *UxContext;\
\
    UxSaveCtx =  UxPagShellContext ;\
    UxPagShellContext = UxContext =\
               (_UxCPagShell *) UxGetContext( w );\
\
   editing_background = False;\
   draw_edit=NULL;\
   UxPagShellContext  = UxSaveCtx;\
   \
   \
#endif\
}\
\
/*------------------------------------------------------\
 Ritorna l'informazione se il background e' in editing\
--------------------------------------------------------*/\
Boolean is_drawing_background(Widget w)\
{\
#ifndef DESIGN_TIME\
\
   _UxCPagShell     *UxSaveCtx, *UxContext;\
\
    UxSaveCtx =  UxPagShellContext ;\
    UxPagShellContext = UxContext =\
               (_UxCPagShell *) UxGetContext( w );\
\
   return(editing_background);\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   \
#endif\
}\
\
\
void close_page(char *pagname)\
{\
   extern void abilitaMoveMenu();\
   extern int Already_Selected;\
   extern Widget PagOpen[];\
   extern int PagOpenId[];\
   extern unsigned short npagopen; \
   PAGINA *pappo=NULL,*prec=NULL;\
   extern XrmDatabase defdb;\
   int i;\
   Widget shell;\
\
/* cerco la pagina da chiudere tremite il nome */\
   pappo = pagine;\
   while( strcmp(pappo->nomepag,pagname) )\
   {\
      prec = pappo;\
      pappo = pappo->pagsucc;\
   }\
\
\
\
   UxDisplay->db = defdb;\
\
   shell = pappo->topwidget;\
\
/* ho trovato la pagina e aggiorno le liste delle pagine */\
\
   for(i=0;i<npagopen;i++)\
   {\
      if( PagOpen[i] == pappo->topwidget)\
      {\
         PagOpen[i] = NULL;\
         PagOpenId[i]= 100;\
      }\
   }\
   npagopen--; \
\
   if(pappo == pagine)\
      pagine = pappo->pagsucc;\
   else\
      prec->pagsucc = pappo->pagsucc;  \
\
   EliminaDraget(pappo);\
   \
\
/* aggiorno il flag di ??? */\
   Already_Selected = False;    \
\
\
/* set item attivi menu' della main interface */  \
   abilitaMoveMenu();\
\
\
   UxDestroyInterface(shell);\
pagina_free(pappo);\
}\
\
SetItemString(XmString *Xstring,char *nome,char *tipo,char *descr,int in_use,\
	char *tag)\
{\
   char *Cstring;\
   int len;\
   \
   len = strlen(nome)+strlen(tipo)+strlen(descr)+40;\
   Cstring = XtCalloc(len,sizeof(char));\
   if(strcmp(tipo,TIPO_REGOLAZIONE)==0)\
	{\
	if(in_use == SCHEME_IN_USE)\
   	   sprintf(Cstring,"%s    - tipo %s [in use][%s]- %s ",\
			nome,tipo,tag,descr);\
	else\
	   sprintf(Cstring,"%s    - tipo %s [not in use][%s]- %s ",\
		nome,tipo,tag,descr);\
	}\
   else\
	sprintf(Cstring,"%s    - tipo %s - %s ",nome,tipo,descr);\
   *Xstring = (XmString )XmStringCreateSimple( Cstring );\
   XtFree(Cstring);  \
}\
\
PageMenuSetInterfaceModeOn()\
{\
   set_something(SaveTmp,XmNsensitive,False);\
   set_something(ConnectMode,XmNsensitive,False);\
   set_connect(actual_page->drawing,(int)1);\
}\
\
PageMenuSetInterfaceModeOff()\
{\
   set_something(SaveTmp,XmNsensitive,True);\
}\
\
void SetButtonClosePage(Boolean Stato)\
{\
   if( (Stato == False) || (Stato == True))\
      set_something(CloseTmp,XmNsensitive,Stato);\
}\
\
Boolean PagGetConnectMode(Widget w)\
{\
\
#ifndef DESIGN_TIME\
  Boolean Stato= False;\
\
  _UxCPagShell     *UxSaveCtx, *UxContext;\
\
   UxSaveCtx =  UxPagShellContext ;\
   UxPagShellContext = UxContext =\
              (_UxCPagShell *) UxGetContext( w );\
\
   Stato = XmToggleButtonGetState(ConnectMode);\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   return(Stato);  \
#endif\
}\
\
Widget gs_WSetSnap(Widget w,Widget wsnap,Boolean set)\
{\
Widget wret=NULL;\
#ifndef DESIGN_TIME\
\
\
  _UxCPagShell     *UxSaveCtx, *UxContext;\
\
   UxSaveCtx =  UxPagShellContext ;\
   UxPagShellContext = UxContext =\
              (_UxCPagShell *) UxGetContext( w );\
\
   if(set)\
	wset_snap=wsnap;\
   else\
	wret=wset_snap;\
 \
   UxPagShellContext  = UxSaveCtx;\
\
   return(wret);  \
#endif\
}\
\
RefreshDrawingArea(Widget cw)\
{\
int i,j;\
Cardinal nchild,nnipoti;\
WidgetList child,nipoti;\
\
\
XClearArea(XtDisplay(cw),XtWindow(cw),\
		0,0,0,0,True);\
get_child(cw,&child,&nchild);\
for(i=0;i<nchild;i++)\
	{\
	XClearArea(XtDisplay(child[i]),XtWindow(child[i]),0,0,0,0,True);\
	if(XlIsXlComposite(child[i]))\
		{\
		get_child(child[i],&nipoti,&nnipoti);\
		for(j=0;j<nnipoti;j++)\
		   XClearArea(XtDisplay(nipoti[j]),XtWindow(nipoti[j]),\
			0,0,0,0,True);\
		}		\
	}\
}\
\
int PrintClosePag( unsigned char *nome_pag )\
{\
   PrintPag( actual_page->drawing );\
\
   close_page( nome_pag );\
} \

*PagShell.static: true
*PagShell.name: PagShell
*PagShell.parent: NO_PARENT
*PagShell.title: "Pagina"
*PagShell.allowShellResize: "true"
*PagShell.popupCallback: {\
printf("Popup Callback Is Called drawing = %d\n",PagDrawingArea);\
\
}
*PagShell.height: 121
*PagShell.x: 82
*PagShell.y: 46

*PagMainWindow.class: mainWindow
*PagMainWindow.static: true
*PagMainWindow.name: PagMainWindow
*PagMainWindow.parent: PagShell
*PagMainWindow.unitType: "pixels"
*PagMainWindow.x: 80
*PagMainWindow.y: 120
*PagMainWindow.height: 121

*PagForm1.class: form
*PagForm1.static: true
*PagForm1.name: PagForm1
*PagForm1.parent: PagMainWindow

*PagScrolledWindow.class: scrolledWindow
*PagScrolledWindow.static: true
*PagScrolledWindow.name: PagScrolledWindow
*PagScrolledWindow.parent: PagForm1
*PagScrolledWindow.scrollingPolicy: "automatic"
*PagScrolledWindow.x: 0
*PagScrolledWindow.y: 0
*PagScrolledWindow.bottomAttachment: "attach_form"
*PagScrolledWindow.leftAttachment: "attach_form"
*PagScrolledWindow.rightAttachment: "attach_form"
*PagScrolledWindow.topAttachment: "attach_form"
*PagScrolledWindow.navigationType: "none"
*PagScrolledWindow.traversalOn: "false"

*PagDrawingArea.class: drawingArea
*PagDrawingArea.static: true
*PagDrawingArea.name: PagDrawingArea
*PagDrawingArea.parent: PagScrolledWindow
*PagDrawingArea.resizePolicy: "resize_none"
*PagDrawingArea.x: 4
*PagDrawingArea.y: -6
*PagDrawingArea.marginHeight: 0
*PagDrawingArea.marginWidth: 0
*PagDrawingArea.navigationType: "none"
*PagDrawingArea.traversalOn: "false"

*menu3.class: rowColumn
*menu3.static: true
*menu3.name: menu3
*menu3.parent: PagDrawingArea
*menu3.rowColumnType: "menu_popup"
*menu3.menuAccelerator: "<KeyUp>F4"
*menu3.createCallback: {\
\
}

*menu3Foreground.class: rowColumn
*menu3Foreground.static: true
*menu3Foreground.name: menu3Foreground
*menu3Foreground.parent: menu3
*menu3Foreground.rowColumnType: "menu_pulldown"

*menu3FCut.class: pushButton
*menu3FCut.static: true
*menu3FCut.name: menu3FCut
*menu3FCut.parent: menu3Foreground
*menu3FCut.labelString: "Cut"
*menu3FCut.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcut();\
\
   fcut(actual_page);\
#endif\
}

*menu3FCopy.class: pushButton
*menu3FCopy.static: true
*menu3FCopy.name: menu3FCopy
*menu3FCopy.parent: menu3Foreground
*menu3FCopy.labelString: "Copy"
*menu3FCopy.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcopy();\
\
   fcopy(actual_page);\
#endif\
}

*menu3FPaste.class: pushButton
*menu3FPaste.static: true
*menu3FPaste.name: menu3FPaste
*menu3FPaste.parent: menu3Foreground
*menu3FPaste.labelString: "Paste"
*menu3FPaste.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int fpaste();\
\
  fpaste(actual_page);\
#endif\
}

*menu3Background.class: rowColumn
*menu3Background.static: true
*menu3Background.name: menu3Background
*menu3Background.parent: menu3
*menu3Background.rowColumnType: "menu_pulldown"

*menu3BCut.class: pushButton
*menu3BCut.static: true
*menu3BCut.name: menu3BCut
*menu3BCut.parent: menu3Background
*menu3BCut.labelString: "Cut"
*menu3BCut.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_cut();\
   extern Widget ClipBoardForCopy;\
\
   draget_cut(actual_page->drawing,ClipBoardForCopy);\
#endif\
}

*menu3BCopy.class: pushButton
*menu3BCopy.static: true
*menu3BCopy.name: menu3BCopy
*menu3BCopy.parent: menu3Background
*menu3BCopy.labelString: "Copy"
*menu3BCopy.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_copy();\
   extern Widget ClipBoardForCopy;\
\
   draget_copy(actual_page->drawing,ClipBoardForCopy);\
#endif\
}

*menu3BPaste.class: pushButton
*menu3BPaste.static: true
*menu3BPaste.name: menu3BPaste
*menu3BPaste.parent: menu3Background
*menu3BPaste.labelString: "Paste"
*menu3BPaste.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int draget_paste();\
\
  draget_paste(actual_page->drawing); \
#endif\
}

*menu3BPutBottom.class: pushButton
*menu3BPutBottom.static: true
*menu3BPutBottom.name: menu3BPutBottom
*menu3BPutBottom.parent: menu3Background
*menu3BPutBottom.labelString: "Put Bottom"
*menu3BPutBottom.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int DrawPutBottom();\
\
   DrawPutBottom(actual_page->drawing);\
#endif\
}

*menu3BPutTop.class: pushButton
*menu3BPutTop.static: true
*menu3BPutTop.name: menu3BPutTop
*menu3BPutTop.parent: menu3Background
*menu3BPutTop.labelString: "Put Top"
*menu3BPutTop.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int DrawPutTop();\
\
   DrawPutTop(actual_page->drawing);\
#endif\
}

*menu3BModifyObject.class: pushButton
*menu3BModifyObject.static: true
*menu3BModifyObject.name: menu3BModifyObject
*menu3BModifyObject.parent: menu3Background
*menu3BModifyObject.labelString: "Modify Object"
*menu3BModifyObject.activateCallback: {\
   printf("Funzione non abilitata\n");\
}

*menu3_p1_title.class: label
*menu3_p1_title.static: true
*menu3_p1_title.name: menu3_p1_title
*menu3_p1_title.parent: menu3
*menu3_p1_title.labelString: "Edit"

*menu3separ1.class: separator
*menu3separ1.static: true
*menu3separ1.name: menu3separ1
*menu3separ1.parent: menu3

*menu3Edit.class: pushButton
*menu3Edit.static: true
*menu3Edit.name: menu3Edit
*menu3Edit.parent: menu3
*menu3Edit.labelString: "Resource"
*menu3Edit.activateCallback: {\
   extern swidget create_Resource();\
   swidget redit;\
   Widget widsel,reswid,edchild;\
   WidgetList child;\
   Cardinal nchild;\
   int i;\
\
#ifndef DESIGN_TIME\
   widsel = XlGetSelectedWidget( actual_page->drawing);\
   if(widsel)\
   {\
\
      if( XlIsXlComposite(widsel) )\
      {\
         get_child(widsel,&child,&nchild);\
         for(i=0;i<nchild;i++)\
         {\
            get_something(child[i],XlNwEdit,(char*)&edchild);\
            if(edchild != NULL)\
            {\
               printf("Cannot open Editor when Children are in Edit\n");\
               SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",\
                      "Selected Widget Children Is In Edit, Can't Open Editor",\
                       NULL,False,NULL,False,NULL);\
               return;\
            }\
         }   \
      }\
      else if( XlIsXlComposite(XtParent(widsel)) )\
      {\
         get_something( XtParent(widsel),XlNwEdit,(char*)&edchild);\
         if(edchild != NULL)\
         {\
            printf("Cannot open Editor when Parent is in Edit\n");\
            SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",\
                   "Selected Widget Parent Is In Edit, Can't Open Editor",\
                    NULL,False,NULL,False,NULL);\
           return;\
         }      \
      }\
       \
\
      get_something(widsel,XlNwEdit,(char*)&reswid);\
\
\
      if(reswid == NULL)\
      {\
         redit = create_Resource(actual_page,dbtopologia);\
         salva_indice_editor(redit);\
         UxPopupInterface(redit,no_grab);\
      }\
      else\
      { \
         XRaiseWindow(XtDisplay(reswid),XtWindow(reswid));\
      }\
   }\
\
#endif\
}

*menu3separ0.class: separatorGadget
*menu3separ0.static: true
*menu3separ0.name: menu3separ0
*menu3separ0.parent: menu3

*menu3Refresh.class: pushButton
*menu3Refresh.static: true
*menu3Refresh.name: menu3Refresh
*menu3Refresh.parent: menu3
*menu3Refresh.labelString: "Refresh window"
*menu3Refresh.activateCallback: {\
RefreshDrawingArea(actual_page->drawing);\
}

*menu3Undo.class: pushButton
*menu3Undo.static: true
*menu3Undo.name: menu3Undo
*menu3Undo.parent: menu3
*menu3Undo.labelString: "Undo draw"
*menu3Undo.mnemonic: "U"
*menu3Undo.activateCallback: {\
extern int undo_draget_paste();\
\
undo_draget_paste( actual_page->drawing);\
\
}

*menu3DuplicateAll.class: pushButton
*menu3DuplicateAll.static: true
*menu3DuplicateAll.name: menu3DuplicateAll
*menu3DuplicateAll.parent: menu3
*menu3DuplicateAll.labelString: "Duplicate"
*menu3DuplicateAll.activateCallback: {\
   int i;\
   float f_zoom;\
   extern float get_def_zoom();   \
   extern int draget_duplicate();\
   extern int copy_new();\
   WidgetList wsel;\
   Cardinal nwsel;\
\
   int dx,dy,Step;\
/**\
 Se vi sono in selezione oggetti icon reg\
 annulla l'operazione di duplicazione\
 (la duplicazione non gestisce gli schemi\
  di regolazione)\
**/\
   if( get_selected( actual_page,&wsel,&nwsel) == True)\
	{\
	for(i=0;i<nwsel;i++)\
		{\
/*\
 Se la pagina non e' di libreria impedisce il duplicate di porte o\
 display di regolazione\
*/\
		if( XlIsIconReg(wsel[i]) ||\
		    (PagGetType(actual_page) != TYPE_LIBRERIA &&\
		    (XlIsPort(wsel[i]) || XlIsDispReg(wsel[i]))))\
			{\
			libera_memoria(wsel);\
			return;\
			}\
\
		}\
	libera_memoria(wsel);\
	}\
   f_zoom= get_def_zoom(actual_page->drawing);\
   Step=get_step(actual_page->drawing);\
   Step = Step * f_zoom;\
	\
   if(Step == 1)\
   {\
      dx = DELTAX;\
      dy = DELTAY;\
   }\
   else\
   {\
      dx = Step;\
      dy = Step;\
   }\
   draget_duplicate(actual_page->drawing,dx,dy);\
   copy_new(actual_page);\
\
}

*menu3DeleteAll.class: pushButton
*menu3DeleteAll.static: true
*menu3DeleteAll.name: menu3DeleteAll
*menu3DeleteAll.parent: menu3
*menu3DeleteAll.labelString: "Delete"
*menu3DeleteAll.activateCallback: {\
#ifndef DESIGN_TIME\
\
   extern int delete_widget();\
   extern int DrawDelete();\
\
   if(WidSelectedAreInEdit(actual_page) == False)\
   {\
      delete_widget(actual_page);\
      DrawDelete(actual_page->drawing,NULL);\
   }\
   else\
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget Is In Edit",NULL,False,NULL,False,NULL);\
  \
 \
#endif\
}

*menu3CutAll.class: pushButton
*menu3CutAll.static: true
*menu3CutAll.name: menu3CutAll
*menu3CutAll.parent: menu3
*menu3CutAll.labelString: "Cut"
*menu3CutAll.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_cut();\
\
   extern int select_connections();\
   extern Widget ClipBoardForCopy;\
   extern int WriteBackground();\
   WidgetList wsel;\
   Cardinal nwsel; \
   extern int fcut();\
\
\
   /* verifico che gli widget non siano in edit */\
   if(WidSelectedAreInEdit(actual_page) == False)\
   {\
      if( get_selected( actual_page,&wsel,&nwsel) == True)\
         select_connections(nwsel,wsel,NULL,WCOPY);\
      fcut(actual_page);\
      draget_cut(actual_page->drawing,ClipBoardForCopy);\
\
      if(nwsel)\
         delete_widget(actual_page);\
   }\
   else\
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget is In Edit",NULL,False,NULL,False,NULL);\
\
   \
#endif\
}

*menu3CopyAll.class: pushButton
*menu3CopyAll.static: true
*menu3CopyAll.name: menu3CopyAll
*menu3CopyAll.parent: menu3
*menu3CopyAll.labelString: "Copy"
*menu3CopyAll.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcopy();\
   extern int draget_copy();\
   extern int select_connections(); \
   extern Widget ClipBoardForCopy;\
   WidgetList wsel;\
   Cardinal nwsel;\
\
\
\
   if( get_selected( actual_page,&wsel,&nwsel) == True)\
      select_connections(nwsel,wsel,NULL,WCOPY);\
   fcopy(actual_page);\
   draget_copy(actual_page->drawing,ClipBoardForCopy);\
#endif\
}

*menu3PasteAll.class: pushButton
*menu3PasteAll.static: true
*menu3PasteAll.name: menu3PasteAll
*menu3PasteAll.parent: menu3
*menu3PasteAll.labelString: "Paste"
*menu3PasteAll.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int draget_paste();\
  extern int fpaste();\
\
  fpaste(actual_page);\
  draget_paste(actual_page->drawing); \
#endif\
}

*menu3separ.class: separatorGadget
*menu3separ.static: true
*menu3separ.name: menu3separ
*menu3separ.parent: menu3

*menu3PForeground.class: cascadeButton
*menu3PForeground.static: true
*menu3PForeground.name: menu3PForeground
*menu3PForeground.parent: menu3
*menu3PForeground.labelString: "Foreground"
*menu3PForeground.subMenuId: "menu3Foreground"

*menu3PBackground.class: cascadeButton
*menu3PBackground.static: true
*menu3PBackground.name: menu3PBackground
*menu3PBackground.parent: menu3
*menu3PBackground.labelString: "Background"
*menu3PBackground.subMenuId: "menu3Background"

*grePagPane.class: rowColumn
*grePagPane.static: true
*grePagPane.name: grePagPane
*grePagPane.parent: PagMainWindow
*grePagPane.rowColumnType: "menu_bar"
*grePagPane.menuAccelerator: "<KeyUp>F10"

*FileTmp.class: rowColumn
*FileTmp.static: true
*FileTmp.name: FileTmp
*FileTmp.parent: grePagPane
*FileTmp.rowColumnType: "menu_pulldown"

*SaveTmp.class: pushButton
*SaveTmp.static: true
*SaveTmp.name: SaveTmp
*SaveTmp.parent: FileTmp
*SaveTmp.labelString: "Save"
*SaveTmp.activateCallback: {\
   extern PAGEDIT_CONTEXT *pagedit;\
   extern Boolean pagina_save();\
   extern int WriteBackground();\
   char fileback[MAXLENFILENAME];\
\
   extern Boolean CompressData();\
\
   CompressData(actual_page);\
\
\
#ifndef DESIGN_TIME\
\
      if( (!strcmp(actual_page->geom.tipo,TIPO_SINOTTICO)) ||\
          (!strcmp(actual_page->geom.tipo,TIPO_STAZIONI))  ||\
          (!strcmp(actual_page->geom.tipo,TIPO_REGOLAZIONE)) ||\
          (!strcmp(actual_page->geom.tipo,TIPO_TELEPERM)) )\
      {\
         if( (pagina_save(actual_page,TIPO_PAGINA,1)) != False)\
         { \
            strcpy(fileback,pagedit->pages);\
            strcat(fileback,"/"); \
            strcat(fileback,actual_page->filebkg);\
            strcpy(fileback,XlConvPathVms(fileback));\
            WriteBackground(actual_page->drawing,fileback);\
         }\
      }\
      else if( !strcmp(actual_page->geom.tipo,TIPO_LIBRERIA) )\
      {\
         pagina_save(actual_page,TIPO_LIBRARY,1);\
      }\
   \
#endif\
}
*SaveTmp.sensitive: "true"

*FileSep.class: separator
*FileSep.static: true
*FileSep.name: FileSep
*FileSep.parent: FileTmp

*Stampa.class: pushButton
*Stampa.static: true
*Stampa.name: Stampa
*Stampa.parent: FileTmp
*Stampa.labelString: "Print"
*Stampa.activateCallback: {\
#ifdef XPRINTER_USED\
char str[200];\
PrintPag(actual_page->drawing);\
sprintf(str,"Printing Page %s",actual_page->nomepag);\
SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo",str,NULL,\
	False,NULL,False,NULL); \
#else\
SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo","Print not allowed for current version",NULL,\
	False,NULL,False,NULL);\
#endif\
}

*Stampa_Setup.class: pushButton
*Stampa_Setup.static: true
*Stampa_Setup.name: Stampa_Setup
*Stampa_Setup.parent: FileTmp
*Stampa_Setup.labelString: "Printer Setup..."
*Stampa_Setup.activateCallback: {\
#ifdef XPRINTER_USED\
#ifndef DESIGN_TIME\
PrintSetup(actual_page->drawing);\
#endif\
#else\
SetMsg(actual_page->drawing,NULL,WRNMSG,"ConfigInfo","Printer use not allowed for this version"\
	 ,NULL,False,NULL,False,NULL);\
#endif\
}

*FileTmp_b6.class: separator
*FileTmp_b6.static: true
*FileTmp_b6.name: FileTmp_b6
*FileTmp_b6.parent: FileTmp

*CloseTmp.class: pushButton
*CloseTmp.static: true
*CloseTmp.name: CloseTmp
*CloseTmp.parent: FileTmp
*CloseTmp.labelString: "Close"
*CloseTmp.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int gest_conf_op();\
\
   gest_conf_op(CLOSE_PAGE,"Do you really want close %s page ?",actual_page->nomepag);\
\
#endif\
\
}

*AddObj.class: rowColumn
*AddObj.static: true
*AddObj.name: AddObj
*AddObj.parent: grePagPane
*AddObj.rowColumnType: "menu_pulldown"

*Icon.class: pushButton
*Icon.static: true
*Icon.name: Icon
*Icon.parent: AddObj
*Icon.labelString: "Icon"
*Icon.activateCallback.source: public
*Icon.activateCallback: 

*FEdit.class: rowColumn
*FEdit.static: true
*FEdit.name: FEdit
*FEdit.parent: grePagPane
*FEdit.rowColumnType: "menu_pulldown"

*FUndo.class: pushButton
*FUndo.static: true
*FUndo.name: FUndo
*FUndo.parent: FEdit
*FUndo.labelString: "Undo draw"
*FUndo.mnemonic: "U"
*FUndo.activateCallback: {\
\
extern int undo_draget_paste();\
\
undo_draget_paste( actual_page->drawing );\
\
}

*FDuplicateAll.class: pushButton
*FDuplicateAll.static: true
*FDuplicateAll.name: FDuplicateAll
*FDuplicateAll.parent: FEdit
*FDuplicateAll.labelString: "Duplicate"
*FDuplicateAll.activateCallback: {\
   int i;\
   float f_zoom;\
   extern float get_def_zoom();   \
   extern int draget_duplicate();\
   extern int copy_new();\
   WidgetList wsel;\
   Cardinal nwsel;\
\
   int dx,dy,Step;\
/**\
 Se vi sono in selezione oggetti icon reg\
 annulla l'operazione di duplicazione\
 (la duplicazione non gestisce gli schemi\
  di regolazione)\
**/\
   if( get_selected( actual_page,&wsel,&nwsel) == True)\
	{\
	for(i=0;i<nwsel;i++)\
		{\
/*\
 Se la pagina non e' di libreria impedisce il duplicate di porte o\
 display di regolazione\
*/\
		if( XlIsIconReg(wsel[i]) ||\
		    (PagGetType(actual_page) != TYPE_LIBRERIA &&\
		    (XlIsPort(wsel[i]) || XlIsDispReg(wsel[i]))))\
			{\
			libera_memoria(wsel);\
			return;\
			}\
\
		}\
	libera_memoria(wsel);\
	}\
   f_zoom= get_def_zoom(actual_page->drawing);\
   Step=get_step(actual_page->drawing);\
   Step = Step * f_zoom;\
	\
   if(Step == 1)\
   {\
      dx = DELTAX;\
      dy = DELTAY;\
   }\
   else\
   {\
      dx = Step;\
      dy = Step;\
   }\
   draget_duplicate(actual_page->drawing,dx,dy);\
   copy_new(actual_page);\
\
}

*FDeleteAll.class: pushButton
*FDeleteAll.static: true
*FDeleteAll.name: FDeleteAll
*FDeleteAll.parent: FEdit
*FDeleteAll.labelString: "Delete"
*FDeleteAll.activateCallback: {\
#ifndef DESIGN_TIME\
\
   extern int delete_widget();\
   extern int DrawDelete();\
\
   if(WidSelectedAreInEdit(actual_page) == False)\
   {\
      delete_widget(actual_page);\
      DrawDelete(actual_page->drawing,NULL);\
   }\
   else\
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget Is In Edit",NULL,False,NULL,False,NULL);\
      \
#endif\
}

*FCutAll.class: pushButton
*FCutAll.static: true
*FCutAll.name: FCutAll
*FCutAll.parent: FEdit
*FCutAll.labelString: "Cut"
*FCutAll.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_cut();\
\
   extern int fcut();\
   extern int select_connections();\
   extern Widget ClipBoardForCopy;\
   extern int WriteBackground();\
   WidgetList wsel;\
   Cardinal nwsel;\
   \
   /* verifico che gli widget non siano in edit */\
   if(WidSelectedAreInEdit(actual_page) == False)\
   {\
\
      if( get_selected( actual_page,&wsel,&nwsel) == True) \
         select_connections(nwsel,wsel,NULL,WCOPY);\
      fcut(actual_page);\
      draget_cut(actual_page->drawing,ClipBoardForCopy);\
/*      WriteBackground(ClipBoard,"ClipDb.bkg"); */\
      if(nwsel)\
         delete_widget(actual_page);\
  }\
   else\
      SetMsg(NULL,NULL,WRNMSG,"ConfigWarning","Selected Widget is In Edit",NULL,False,NULL,False,NULL);\
\
   \
#endif\
}

*FCopyAll.class: pushButton
*FCopyAll.static: true
*FCopyAll.name: FCopyAll
*FCopyAll.parent: FEdit
*FCopyAll.labelString: "Copy"
*FCopyAll.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcopy();\
   extern int draget_copy();\
   extern int select_connections(); \
   extern Widget ClipBoardForCopy;\
   WidgetList wsel;\
   Cardinal nwsel;\
\
\
\
   if( get_selected( actual_page,&wsel,&nwsel) == True)\
      select_connections(nwsel,wsel,NULL,WCOPY);\
   fcopy(actual_page);\
   draget_copy(actual_page->drawing,ClipBoardForCopy);\
\
   \
#endif\
}\
\


*FPasteAll.class: pushButton
*FPasteAll.static: true
*FPasteAll.name: FPasteAll
*FPasteAll.parent: FEdit
*FPasteAll.labelString: "Paste"
*FPasteAll.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int draget_paste();\
  extern int fpaste();\
\
  fpaste(actual_page);\
  draget_paste(actual_page->drawing); \
#endif\
}

*Fsepar0.class: separatorGadget
*Fsepar0.static: true
*Fsepar0.name: Fsepar0
*Fsepar0.parent: FEdit

*FEditForeground.class: cascadeButton
*FEditForeground.static: true
*FEditForeground.name: FEditForeground
*FEditForeground.parent: FEdit
*FEditForeground.labelString: "Foreground"
*FEditForeground.subMenuId: "EditForeground"

*FEditBackground.class: cascadeButton
*FEditBackground.static: true
*FEditBackground.name: FEditBackground
*FEditBackground.parent: FEdit
*FEditBackground.labelString: "Background"
*FEditBackground.subMenuId: "EditBackground"

*EditForeground.class: rowColumn
*EditForeground.static: true
*EditForeground.name: EditForeground
*EditForeground.parent: FEdit
*EditForeground.rowColumnType: "menu_pulldown"

*FgCut.class: pushButton
*FgCut.static: true
*FgCut.name: FgCut
*FgCut.parent: EditForeground
*FgCut.labelString: "Cut"
*FgCut.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcut();\
\
   fcut(actual_page);\
#endif\
}

*FgCopy.class: pushButton
*FgCopy.static: true
*FgCopy.name: FgCopy
*FgCopy.parent: EditForeground
*FgCopy.labelString: "Copy"
*FgCopy.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int fcopy();\
\
   fcopy(actual_page);\
#endif\
}

*FgPaste.class: pushButton
*FgPaste.static: true
*FgPaste.name: FgPaste
*FgPaste.parent: EditForeground
*FgPaste.labelString: "Paste"
*FgPaste.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int fpaste();\
\
  fpaste(actual_page);\
#endif\
}

*EditBackground.class: rowColumn
*EditBackground.static: true
*EditBackground.name: EditBackground
*EditBackground.parent: FEdit
*EditBackground.rowColumnType: "menu_pulldown"

*BgCut.class: pushButton
*BgCut.static: true
*BgCut.name: BgCut
*BgCut.parent: EditBackground
*BgCut.labelString: "Cut"
*BgCut.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_cut();\
   extern Widget ClipBoardForCopy;\
\
   draget_cut(actual_page->drawing,ClipBoardForCopy);\
#endif\
}

*BgCopy.class: pushButton
*BgCopy.static: true
*BgCopy.name: BgCopy
*BgCopy.parent: EditBackground
*BgCopy.labelString: "Copy"
*BgCopy.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int draget_copy();\
   extern Widget ClipBoardForCopy;\
\
   draget_copy(actual_page->drawing,ClipBoardForCopy);\
#endif\
}

*BgPaste.class: pushButton
*BgPaste.static: true
*BgPaste.name: BgPaste
*BgPaste.parent: EditBackground
*BgPaste.labelString: "Paste"
*BgPaste.activateCallback: {\
#ifndef DESIGN_TIME\
  extern int draget_paste();\
\
  draget_paste(actual_page->drawing); \
#endif\
}

*BgPutBottom.class: pushButton
*BgPutBottom.static: true
*BgPutBottom.name: BgPutBottom
*BgPutBottom.parent: EditBackground
*BgPutBottom.labelString: "Put Bottom"
*BgPutBottom.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int DrawPutBottom();\
\
   DrawPutBottom(actual_page->drawing);\
#endif\
}

*BgPutTop.class: pushButton
*BgPutTop.static: true
*BgPutTop.name: BgPutTop
*BgPutTop.parent: EditBackground
*BgPutTop.labelString: "Put Top"
*BgPutTop.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int DrawPutTop();\
\
   DrawPutTop(actual_page->drawing);\
#endif\
}

*raggruppaButton.class: pushButton
*raggruppaButton.static: true
*raggruppaButton.name: raggruppaButton
*raggruppaButton.parent: EditBackground
*raggruppaButton.labelString: "Group"
*raggruppaButton.activateCallback: {\
DrawGroup(actual_page->drawing);\
}\


*unraggruppaButton.class: pushButton
*unraggruppaButton.static: true
*unraggruppaButton.name: unraggruppaButton
*unraggruppaButton.parent: EditBackground
*unraggruppaButton.labelString: "Ungroup"
*unraggruppaButton.activateCallback: {\
DrawUngroup(actual_page->drawing);\
}\


*Configure.class: rowColumn
*Configure.static: true
*Configure.name: Configure
*Configure.parent: grePagPane
*Configure.rowColumnType: "menu_pulldown"

*Edit.class: pushButton
*Edit.static: true
*Edit.name: Edit
*Edit.parent: Configure
*Edit.labelString: "Resource Editor"
*Edit.activateCallback: {\
   extern swidget create_Resource();\
   swidget redit;\
   Widget widsel,reswid,edchild;\
   WidgetList child;\
   Cardinal nchild;\
   int i; \
\
#ifndef DESIGN_TIME\
   widsel = XlGetSelectedWidget( actual_page->drawing);\
   if(widsel)\
   {\
      if( XlIsXlComposite(widsel) )\
      {\
         get_child(widsel,&child,&nchild);\
         for(i=0;i<nchild;i++)\
         {\
            get_something(child[i],XlNwEdit,(char*)&edchild);\
            if(edchild != NULL)\
            {\
               printf("Cannot open Editor when Children are in Edit\n");\
               SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",\
                      "Selected Widget Children Is In Edit, Can't Open Editor",\
                       NULL,False,NULL,False,NULL);\
               return;\
            }\
         }   \
      }\
      else if( XlIsXlComposite(XtParent(widsel)) )\
      {\
         get_something( XtParent(widsel),XlNwEdit,(char*)&edchild);\
         if(edchild != NULL)\
         {\
            printf("Cannot open Editor when Parent is in Edit\n");\
            SetMsg(NULL,NULL,WRNMSG,"ConfigWarning",\
                   "Selected Widget Parent Is In Edit, Can't Open Editor",\
                    NULL,False,NULL,False,NULL);\
            return;\
         }      \
      }\
\
      get_something(widsel,XlNwEdit,(char*)&reswid);\
      if(reswid == NULL)\
      {\
         redit = create_Resource(actual_page,dbtopologia);\
         salva_indice_editor(redit);\
         UxPopupInterface(redit,no_grab);\
      }\
      else\
      { \
         XRaiseWindow(XtDisplay(reswid),XtWindow(reswid));\
      }\
   }\
   else\
      show_message("Nothing selected\n");\
#endif\
\
}

*DrawBackground.class: rowColumn
*DrawBackground.static: true
*DrawBackground.name: DrawBackground
*DrawBackground.parent: grePagPane
*DrawBackground.rowColumnType: "menu_pulldown"

*FDraw.class: pushButton
*FDraw.static: true
*FDraw.name: FDraw
*FDraw.parent: DrawBackground
*FDraw.labelString: "Draw"
*FDraw.activateCallback: {\
#ifndef DESIGN_TIME\
   \
if(draw_edit == NULL)\
   {\
   draw_edit = create_drawShell(PagDrawingArea,actual_page->nomepag);\
   UxPopupInterface(draw_edit,no_grab);\
   }\
else\
   XRaiseWindow(XtDisplay(draw_edit),XtWindow(draw_edit)); \
/*\
 setta la variabile per indicare che il background e' in fase di editing\
*/\
   editing_background=True;\
#endif\
}

*Compile.class: rowColumn
*Compile.static: true
*Compile.name: Compile
*Compile.parent: grePagPane
*Compile.rowColumnType: "menu_pulldown"

*CompilePag.class: pushButton
*CompilePag.static: true
*CompilePag.name: CompilePag
*CompilePag.parent: Compile
*CompilePag.labelString: "Page"
*CompilePag.activateCallback: {\
#ifndef DESIGN_TIME\
extern int compile_page();\
    \
      compile_page(actual_page);\
#endif\
#ifdef MAURIZIO\
#ifndef DESIGN_TIME\
   extern Cardinal lnargs;    \
   extern Arg args[];\
   extern OlCompilerObject compilatore_severe;\
   int err_level = 0;\
   WidgetList children;\
   Cardinal num_children;\
   char outname[105],mesg[200];\
   int g,m,a,o,min,s;\
\
\
   if(compilatore_severe == NULL)\
	{\
	SetMsg(NULL,NULL,ERRMSG,"ConfigInfo",\
               "Topology database not defined",\
                NULL,False,NULL,False,NULL);\
	return;\
	}\
\
   if( getenv(ENVPAG) == NULL)\
      return;\
\
   lnargs = 0;\
   XtSetArg( args[lnargs],XmNchildren,&children);lnargs++; \
   XtSetArg(args[lnargs],XmNnumChildren,&num_children);lnargs++;\
   XtGetValues(actual_page->drawing,args,lnargs);\
\
   OlSetCompiler(children,num_children,compilatore_severe);\
\
   strcpy( outname, getenv(ENVPAG));\
   strcat(outname,"/");\
   strcat(outname,actual_page->nomepag);\
   strcat(outname,".rtf");      \
\
   if( (err_level = XlCompilaWidget(actual_page->db,outname,children,num_children)) == False)   \
       {	 \
          sprintf(mesg, "Error at Compile Time on Page  %s\n",actual_page->nomepag);\
          show_message(mesg); \
       }  \
       else\
       {\
	/*       \
	 setto la data della compilazione\
   	*/\
   	data(&g,&m,&a);\
   	ora(&o,&min,&s);\
   	sprintf(actual_page->compilationDate,"%d/%d/%d %d:%d:%d",g,m,a,o,min,s);\
        sprintf(mesg, "Page Compile Ok %s\n ",actual_page->nomepag);\
        show_message(mesg); \
       }  \
#endif\
#endif\
}

*CompileReg.class: pushButton
*CompileReg.static: true
*CompileReg.name: CompileReg
*CompileReg.parent: Compile
*CompileReg.labelString: "Regolation"
*CompileReg.activateCallback: {\
#ifndef DESIGN_TIME\
   extern int CompReg();\
   int macrobl=0;\
  \
   CompReg(actual_page,macrobl);\
#endif\
\
}

*Compile_b5.class: separator
*Compile_b5.static: true
*Compile_b5.name: Compile_b5
*Compile_b5.parent: Compile

*ViewError.class: pushButton
*ViewError.static: true
*ViewError.name: ViewError
*ViewError.parent: Compile
*ViewError.labelString: "Page Error View"
*ViewError.activateCallback: {\
#ifndef DESIGN_TIME\
char error_file[100],comando[200];\
\
if( getenv(ENVPAG) == NULL)\
   return;\
\
#ifndef VMS\
strcpy(error_file,XlGetenv(ENVPAG));\
strcat(error_file,"/");\
#else\
strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));\
#endif\
strcat(error_file,actual_page->nomepag);\
strcat(error_file,RTF_EXT);\
strcat(error_file,ERR_EXT);\
\
strcpy(comando,"edit_f01   ");\
strcat(comando,error_file);\
strcat(comando,"  False &");\
\
if( system(NULL) != 0)\
   system( comando );  \
#endif\
}

*Compile_b4.class: pushButton
*Compile_b4.static: true
*Compile_b4.name: Compile_b4
*Compile_b4.parent: Compile
*Compile_b4.labelString: "Regolation Error View "
*Compile_b4.activateCallback: {\
#ifndef DESIGN_TIME\
char error_file[100],comando[200];\
\
if( getenv(ENVPAG) == NULL)\
   return;\
\
#ifndef VMS\
strcpy(error_file,XlGetenv(ENVPAG));\
strcat(error_file,"/");\
#else\
strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));\
#endif\
strcat(error_file,actual_page->nomepag);\
strcat(error_file,REG_EXT);\
strcat(error_file,ERR_EXT);\
\
strcpy(comando,"edit_f01   ");\
strcat(comando,error_file);\
strcat(comando,"  False &");\
\
if( system(NULL) != 0)\
   system( comando );  \
#endif\
}\


*Option.class: rowColumn
*Option.static: true
*Option.name: Option
*Option.parent: grePagPane
*Option.rowColumnType: "menu_pulldown"

*Grid.class: toggleButton
*Grid.static: true
*Grid.name: Grid
*Grid.parent: Option
*Grid.labelString: "Grid"
*Grid.valueChangedCallback: {\
#ifndef DESIGN_TIME\
extern int set_grid_on();\
XmToggleButtonCallbackStruct *pcall;\
pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;\
set_grid_on(actual_page->drawing,pcall->set);\
XClearArea(UxDisplay,XtWindow(actual_page->drawing),0,0,0,0,True);\
#endif\
}\


*WriteRes.class: pushButton
*WriteRes.static: true
*WriteRes.name: WriteRes
*WriteRes.parent: Option
*WriteRes.labelString: "Write Resource"
*WriteRes.activateCallback: {\
extern Boolean WriteResource();\
\
WriteResource(actual_page);\
}

*SetZ.class: cascadeButton
*SetZ.static: true
*SetZ.name: SetZ
*SetZ.parent: Option
*SetZ.labelString: "Zoom"
*SetZ.subMenuId: "ZoPane"

*b_SettaSnapDraw.class: pushButton
*b_SettaSnapDraw.static: true
*b_SettaSnapDraw.name: b_SettaSnapDraw
*b_SettaSnapDraw.parent: Option
*b_SettaSnapDraw.labelString: "Set/View Snap..."
*b_SettaSnapDraw.activateCallback: {\
\
\
extern swidget create_Set_Snap();\
Widget wsnap;\
\
wsnap=create_Set_Snap(actual_page, PagShell);\
UxPopupInterface( wsnap ,no_grab);\
\
}

*ConnectMode.class: toggleButton
*ConnectMode.static: true
*ConnectMode.name: ConnectMode
*ConnectMode.parent: Option
*ConnectMode.labelString: "Connect Mode"
*ConnectMode.mnemonic: "C"
*ConnectMode.valueChangedCallback: {\
#ifndef DESIGN_TIME\
XmToggleButtonCallbackStruct *pcall;\
pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;\
set_connect(actual_page->drawing,pcall->set);\
#endif\
}\


*SetVisualMode.class: pushButton
*SetVisualMode.static: true
*SetVisualMode.name: SetVisualMode
*SetVisualMode.parent: Option
*SetVisualMode.labelString: "Set visual mode"
*SetVisualMode.activateCallback: {\
\
extern swidget popup_topRegoDisplayMode();\
popup_topRegoDisplayMode(actual_page->drawing);\
\
}

*RefreshDrawing.class: pushButton
*RefreshDrawing.static: true
*RefreshDrawing.name: RefreshDrawing
*RefreshDrawing.parent: Option
*RefreshDrawing.labelString: "Refresh window"
*RefreshDrawing.activateCallback: {\
RefreshDrawingArea(actual_page->drawing);\
}

*ZoPane.class: rowColumn
*ZoPane.static: true
*ZoPane.name: ZoPane
*ZoPane.parent: Option
*ZoPane.rowColumnType: "menu_pulldown"
*ZoPane.radioBehavior: "true"

*Zoom100TB.class: toggleButton
*Zoom100TB.static: true
*Zoom100TB.name: Zoom100TB
*Zoom100TB.parent: ZoPane
*Zoom100TB.labelString: "100% (no Zoom)"
*Zoom100TB.valueChangedCallback: {\
DrawSetZoom(actual_page->drawing,1.0);\
PaginaSetZoom(actual_page->drawing,1.0);\
}

*Zoom200TB.class: toggleButton
*Zoom200TB.static: true
*Zoom200TB.name: Zoom200TB
*Zoom200TB.parent: ZoPane
*Zoom200TB.labelString: "200%" 
*Zoom200TB.valueChangedCallback: {\
DrawSetZoom(actual_page->drawing,2.0);\
PaginaSetZoom(actual_page->drawing,2.0);\
}

*Zoom300TB.class: toggleButton
*Zoom300TB.static: true
*Zoom300TB.name: Zoom300TB
*Zoom300TB.parent: ZoPane
*Zoom300TB.labelString: "300%"
*Zoom300TB.valueChangedCallback: {\
DrawSetZoom(actual_page->drawing,3.0);\
PaginaSetZoom(actual_page->drawing,3.0);\
}

*Zoom400TB.class: toggleButton
*Zoom400TB.static: true
*Zoom400TB.name: Zoom400TB
*Zoom400TB.parent: ZoPane
*Zoom400TB.labelString: "400%"
*Zoom400TB.valueChangedCallback: {\
DrawSetZoom(actual_page->drawing,4.0);\
PaginaSetZoom(actual_page->drawing,4.0);\
}

*menu4_top_b1.class: cascadeButton
*menu4_top_b1.static: true
*menu4_top_b1.name: menu4_top_b1
*menu4_top_b1.parent: grePagPane
*menu4_top_b1.labelString: "File"
*menu4_top_b1.subMenuId: "FileTmp"
*menu4_top_b1.mnemonic: "F"

*PagPane_top_b1.class: cascadeButton
*PagPane_top_b1.static: true
*PagPane_top_b1.name: PagPane_top_b1
*PagPane_top_b1.parent: grePagPane
*PagPane_top_b1.labelString: "Add Icon"
*PagPane_top_b1.mnemonic: "A"
*PagPane_top_b1.subMenuId: "AddObj"

*PagPane_top_b2.class: cascadeButton
*PagPane_top_b2.static: true
*PagPane_top_b2.name: PagPane_top_b2
*PagPane_top_b2.parent: grePagPane
*PagPane_top_b2.labelString: "Edit"
*PagPane_top_b2.subMenuId: "FEdit"

*PagPane_top_b3.class: cascadeButton
*PagPane_top_b3.static: true
*PagPane_top_b3.name: PagPane_top_b3
*PagPane_top_b3.parent: grePagPane
*PagPane_top_b3.labelString: "Configure"
*PagPane_top_b3.subMenuId: "Configure"

*PagPane_top_b6.class: cascadeButton
*PagPane_top_b6.static: true
*PagPane_top_b6.name: PagPane_top_b6
*PagPane_top_b6.parent: grePagPane
*PagPane_top_b6.labelString: "DrawBackground"
*PagPane_top_b6.subMenuId: "DrawBackground"

*PagPane_top_b4.class: cascadeButton
*PagPane_top_b4.static: true
*PagPane_top_b4.name: PagPane_top_b4
*PagPane_top_b4.parent: grePagPane
*PagPane_top_b4.labelString: "Compile"
*PagPane_top_b4.subMenuId: "Compile"
*PagPane_top_b4.sensitive: "true"

*PagPane_top_b5.class: cascadeButton
*PagPane_top_b5.static: true
*PagPane_top_b5.name: PagPane_top_b5
*PagPane_top_b5.parent: grePagPane
*PagPane_top_b5.labelString: "Option"
*PagPane_top_b5.subMenuId: "Option"

