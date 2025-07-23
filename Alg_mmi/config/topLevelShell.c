
/*******************************************************************************
       topLevelShell.c
       (Generated from interface file topLevelShell.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/PanedW.h>
#include <Xm/CascadeBG.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

#include <Rt/RtMemory.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo topLevelShell.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)topLevelShell.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
#include <unistd.h>
#ifndef LINUX
//#include <sys/access.h>
#endif
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/List.h>
#include <Xd/Xd.h>
#include <Xd/XdLista.h>
#include <Ol/OlTree.h>

#include "config.h"

#include "message.h"

/*
#ifdef XPRINTER_USED
#include "libutilx.h"
#endif
*/
#include "libutilx.h"

#ifndef DEBUG_MALLOC
void *RtRecord(void *punt);
#endif

// XtAppContext CompileAppContext = NULL;
extern XtAppContext CompileAppContext ;
Display *CompileDisplay;
Widget CompileTopLevel;



Widget ClipBoardForCopy; /* interfaccia usata per le
                             funzionalita' di copy and
                             paste */
PAGEDIT_CONTEXT *pagedit;
extern Arg args[];
extern Cardinal lnargs;
extern swidget create_PagDialog();
extern swidget create_ClipBoard();
int Already_Selected =False;
extern int gestMessageBox();   /* gestione conferma operazioni */
extern int pagedit_context_init();
extern char *OlEstrRegTaskName(char *, char *);
extern  void *		UxNewContext();
extern XdLista clip_get_lista();

/* lista widget e indici pagine aperte 
*/
Widget PagOpen[MAXPAGOPEN];
int    PagOpenId[MAXPAGOPEN];
unsigned short npagopen;

OlCompilerObject compilatore_severe;
OlDatabaseTopologiaObject dbtopologia;
OlDatabasePuntiObject     dbpunti;


Boolean OperationConfirm;  /* richiede conferma cancellazione pagina */

Boolean CompilerOk = True;  /* gestisce il menu di compila nelle pagine */ 

FILE *fpLogMsg;

Boolean ContextLoaded;      /* True se c'e' un Context caricato */
Boolean ContextModified;    /* True se un Context e' stato modificato */

Boolean StateInterfaceMode = False;    /* flag interfaccia connessione tra pagine */

extern char  Context_Path[];

extern int flag_demo;

OlTreeObject root_oltree;

/*  */
void add_item( );
int add_library_item( );
void removefromlist( );
void removefromlibrarylist( );
void setContextTitle( );
void disabilitaMoveMenu( );
void abilitaMoveMenu( );
void TopMenuSetInsensitive( );

void TopMenuSetSensitive( );

int display_resources( );
int gest_conf_op( );
Boolean LibraryIsOpen( );
int open_page();
int modify_page( );
int duplicate_page( );
int del_page( );

int show_message( );

int LibraryDeselectAllItem( );
int PageDeselectAllItem( );
int clear_topLevel( );

Boolean AlignResPageContext();
void PrintAllPages();
int CheckOlTree();
extern void compile_all_pag();
extern int info_top_schemi();
//extern int SetItemString();
extern OlDatabaseTopologiaObject OlCreateDatabaseTopologia(char *, char *, int , char *,char *,char *);

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
	Widget	UxmainWindow;
	Widget	Uxmenu2;
	Widget	Uxmenu1File1;
	Widget	Uxmenu1Define;
	Widget	Uxmenu1Align;
	Widget	Uxseparatore3;
	Widget	Uxmenu1Check;
	Widget	Uxseparatore4;
	Widget	Uxprint_menu;
	Widget	Uxall_but;
	Widget	Uxselect_but;
	Widget	Uxprint_entry;
	Widget	Uxmenu1PrintSet;
	Widget	Uxseparatore6;
	Widget	Uxmenu1Exit1;
	Widget	Uxmenu1_top_b4;
	Widget	Uxmenu1Create1;
	Widget	UxPage;
	Widget	UxRegolation;
	Widget	UxLibrary;
	Widget	UxStandardLibrary;
	Widget	Uxmenu1_top_b5;
	Widget	Uxmenu1Edit1;
	Widget	Uxmenu1Open;
	Widget	Uxmenu1Modify;
	Widget	Uxmenu1Delete1;
	Widget	Uxmenu1_top_b6;
	Widget	Uxmenu1Import;
	Widget	UxImportPage;
	Widget	UxImportLibrary;
	Widget	UxImportIcon;
	Widget	Uxmenu2_top_b2;
	Widget	Uxmenu1Compile;
	Widget	UxCompileAll;
	Widget	UxCompileAllRegola;
	Widget	Uxmenu1Compile_b4;
	Widget	UxCompileTask;
	Widget	UxViewErrTask;
	Widget	Uxmenu2_top_b3;
	Widget	UxInterfaceMode;
	Widget	UxMode;
	Widget	Uxmenu2_top_b1;
	Widget	UxpanedWindow1;
	Widget	Uxform8;
	Widget	Uxform5;
	Widget	UxLabelProject;
	Widget	UxlabelSimulator;
	Widget	UxlabelAnimateIconLibraries;
	Widget	UxlabelObjectLibraries;
	Widget	UxLabelDescription;
	Widget	UxtextProject;
	Widget	UxtextObjectLibraries;
	Widget	UxtextAnimatedIconLibraries;
	Widget	UxtextSimulator;
	Widget	UxlabelPages;
	Widget	UxtextPages;
	Widget	UxtextDescription1;
	Widget	UxContextLbl;
	Widget	Uxform4;
	Widget	UxscrolledWindow4;
	Widget	Uxmenu1;
	Widget	Uxmenu1_p1_title;
	Widget	Uxmenu1_p1_b5;
	Widget	UxPBOpen;
	Widget	UxPBModify;
	Widget	UxPBDuplicate;
	Widget	UxPBDelete;
	Widget	Uxlabel4;
	Widget	Uxform9;
	Widget	Uxlabel5;
	Widget	UxscrolledWindow5;
	Widget	Uxmenu6;
	Widget	Uxmenu6_p1_title;
	Widget	Uxsepar1;
	Widget	UxOpenLibrary;
	Widget	UxModifyLibrary;
	Widget	UxDeleteLibrary;
	Widget	Uxform3;
	Widget	Uxlabel8;
	Widget	UxscrolledWindow1;
	Widget	UxscrolledText1;
	Widget	Uxmenu5;
	Widget	Uxmenu5_p1_b1;
	int	UxPaginaSelezionata;
	swidget	UxCompileBoard;

	Widget	UxfindBlock;
        Find_struct     Uxparametri_ulist;
        Arg     Uxargb[15];
        Cardinal        Uxnargb;


} _UxCtopLevelShell;

static _UxCtopLevelShell       *UxTopLevelShellContext;
#define mainWindow              UxTopLevelShellContext->UxmainWindow
#define menu2                   UxTopLevelShellContext->Uxmenu2
#define menu1File1              UxTopLevelShellContext->Uxmenu1File1
#define menu1Define             UxTopLevelShellContext->Uxmenu1Define
#define menu1Align              UxTopLevelShellContext->Uxmenu1Align
#define separatore3             UxTopLevelShellContext->Uxseparatore3
#define menu1Check              UxTopLevelShellContext->Uxmenu1Check
#define separatore4             UxTopLevelShellContext->Uxseparatore4
#define print_menu              UxTopLevelShellContext->Uxprint_menu
#define all_but                 UxTopLevelShellContext->Uxall_but
#define select_but              UxTopLevelShellContext->Uxselect_but
#define print_entry             UxTopLevelShellContext->Uxprint_entry
#define menu1PrintSet           UxTopLevelShellContext->Uxmenu1PrintSet
#define separatore6             UxTopLevelShellContext->Uxseparatore6
#define menu1Exit1              UxTopLevelShellContext->Uxmenu1Exit1
#define menu1_top_b4            UxTopLevelShellContext->Uxmenu1_top_b4
#define menu1Create1            UxTopLevelShellContext->Uxmenu1Create1
#define Page                    UxTopLevelShellContext->UxPage
#define Regolation              UxTopLevelShellContext->UxRegolation
#define Library                 UxTopLevelShellContext->UxLibrary
#define StandardLibrary         UxTopLevelShellContext->UxStandardLibrary
#define menu1_top_b5            UxTopLevelShellContext->Uxmenu1_top_b5
#define menu1Edit1              UxTopLevelShellContext->Uxmenu1Edit1
#define menu1Open               UxTopLevelShellContext->Uxmenu1Open
#define menu1Modify             UxTopLevelShellContext->Uxmenu1Modify
#define menu1Delete1            UxTopLevelShellContext->Uxmenu1Delete1
#define menu1_top_b6            UxTopLevelShellContext->Uxmenu1_top_b6
#define menu1Import             UxTopLevelShellContext->Uxmenu1Import
#define ImportPage              UxTopLevelShellContext->UxImportPage
#define ImportLibrary           UxTopLevelShellContext->UxImportLibrary
#define ImportIcon              UxTopLevelShellContext->UxImportIcon
#define menu2_top_b2            UxTopLevelShellContext->Uxmenu2_top_b2
#define menu1Compile            UxTopLevelShellContext->Uxmenu1Compile
#define CompileAll              UxTopLevelShellContext->UxCompileAll
#define CompileAllRegola        UxTopLevelShellContext->UxCompileAllRegola
#define menu1Compile_b4         UxTopLevelShellContext->Uxmenu1Compile_b4
#define CompileTask             UxTopLevelShellContext->UxCompileTask
#define ViewErrTask             UxTopLevelShellContext->UxViewErrTask
#define menu2_top_b3            UxTopLevelShellContext->Uxmenu2_top_b3
#define InterfaceMode           UxTopLevelShellContext->UxInterfaceMode
#define Mode                    UxTopLevelShellContext->UxMode
#define menu2_top_b1            UxTopLevelShellContext->Uxmenu2_top_b1
#define panedWindow1            UxTopLevelShellContext->UxpanedWindow1
#define form8                   UxTopLevelShellContext->Uxform8
#define form5                   UxTopLevelShellContext->Uxform5
#define LabelProject            UxTopLevelShellContext->UxLabelProject
#define labelSimulator          UxTopLevelShellContext->UxlabelSimulator
#define labelAnimateIconLibraries UxTopLevelShellContext->UxlabelAnimateIconLibraries
#define labelObjectLibraries    UxTopLevelShellContext->UxlabelObjectLibraries
#define LabelDescription        UxTopLevelShellContext->UxLabelDescription
#define textProject             UxTopLevelShellContext->UxtextProject
#define textObjectLibraries     UxTopLevelShellContext->UxtextObjectLibraries
#define textAnimatedIconLibraries UxTopLevelShellContext->UxtextAnimatedIconLibraries
#define textSimulator           UxTopLevelShellContext->UxtextSimulator
#define labelPages              UxTopLevelShellContext->UxlabelPages
#define textPages               UxTopLevelShellContext->UxtextPages
#define textDescription1        UxTopLevelShellContext->UxtextDescription1
#define ContextLbl              UxTopLevelShellContext->UxContextLbl
#define form4                   UxTopLevelShellContext->Uxform4
#define scrolledWindow4         UxTopLevelShellContext->UxscrolledWindow4
#define menu1                   UxTopLevelShellContext->Uxmenu1
#define menu1_p1_title          UxTopLevelShellContext->Uxmenu1_p1_title
#define menu1_p1_b5             UxTopLevelShellContext->Uxmenu1_p1_b5
#define PBOpen                  UxTopLevelShellContext->UxPBOpen
#define PBModify                UxTopLevelShellContext->UxPBModify
#define PBDuplicate             UxTopLevelShellContext->UxPBDuplicate
#define PBDelete                UxTopLevelShellContext->UxPBDelete
#define label4                  UxTopLevelShellContext->Uxlabel4
#define form9                   UxTopLevelShellContext->Uxform9
#define label5                  UxTopLevelShellContext->Uxlabel5
#define scrolledWindow5         UxTopLevelShellContext->UxscrolledWindow5
#define menu6                   UxTopLevelShellContext->Uxmenu6
#define menu6_p1_title          UxTopLevelShellContext->Uxmenu6_p1_title
#define separ1                  UxTopLevelShellContext->Uxsepar1
#define OpenLibrary             UxTopLevelShellContext->UxOpenLibrary
#define ModifyLibrary           UxTopLevelShellContext->UxModifyLibrary
#define DeleteLibrary           UxTopLevelShellContext->UxDeleteLibrary
#define form3                   UxTopLevelShellContext->Uxform3
#define label8                  UxTopLevelShellContext->Uxlabel8
#define scrolledWindow1         UxTopLevelShellContext->UxscrolledWindow1
#define scrolledText1           UxTopLevelShellContext->UxscrolledText1
#define menu5                   UxTopLevelShellContext->Uxmenu5
#define menu5_p1_b1             UxTopLevelShellContext->Uxmenu5_p1_b1
#define PaginaSelezionata       UxTopLevelShellContext->UxPaginaSelezionata
#define CompileBoard            UxTopLevelShellContext->UxCompileBoard

#define findBlock               UxTopLevelShellContext->UxfindBlock
#define parametri_ulist         UxTopLevelShellContext->Uxparametri_ulist
#define argb                    UxTopLevelShellContext->Uxargb
#define nargb                   UxTopLevelShellContext->Uxnargb



/*******************************************************************************
       The following function is an event-handler for posting menus.
*******************************************************************************/

static void	_UxtopLevelShellMenuPost(Widget wgt,XtPointer client_data, XEvent*event )
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

Widget	topLevelShell;
Widget	ListPagine;
Widget	LibraryList;

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_topLevelShell();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*****************************************************************

  Function:    add_item

  Parameter:   XmString cstring;   stringa da inserire nella lista
               int pos;            posizione dell'item nella lista

  Description:  con pos 0 aggiunge in fondo
                        1 in cima
                        qualsiasi altro valore nella posizione
                        relativa

****************************************************************/

void add_item(XmString cstring,int pos)
{
   XmListAddItem(ListPagine,cstring,pos);
   XmListSetBottomPos(ListPagine,0);
   XmUpdateDisplay(ListPagine);
}

void modify_item(XmString newstring, int pos)
{
   XmListReplaceItemsPos(ListPagine,&newstring,1,(pos+1));
   XmUpdateDisplay(ListPagine);
}

int add_library_item(XmString cstring,int pos)
{
   XmListAddItem(LibraryList,cstring,pos);
   XmListSetBottomPos(LibraryList,0);
   XmUpdateDisplay(LibraryList);
}

void modify_library_item(XmString newstring,int pos)
{
   XmListReplaceItemsPos(LibraryList,&newstring,1,(pos+1));
   XmUpdateDisplay(LibraryList);
}

void removefromlist(int pos)
{
   XmListDeletePos(ListPagine,pos);
   XmUpdateDisplay(ListPagine);
}

void removefromlibrarylist(int pos)
{
   XmListDeletePos(LibraryList,pos);
   XmUpdateDisplay(LibraryList);
}

/* setta la label con il nome del Context attivo */ 
void setContextTitle(PAGEDIT_CONTEXT *ped)
{
   XmString mstring;

   mstring = XmStringCreateSimple(ped->fileContext);
   set_something_val(ContextLbl,XmNlabelString,(XtArgVal) mstring);
   XmStringFree(mstring);
}

void disabilitaMoveMenu()
{
/** bottoni che non esistono piu'
   set_something_val(menu2_top_b1,XmNsensitive, (XtArgVal) False);
   set_something_val(MovePage,XmNsensitive, (XtArgVal) False);
   set_something_val(MoveIcLib,XmNsensitive, (XtArgVal) False);
**/
}

void abilitaMoveMenu()
{
/**** bottoni che non esistono piu'
   if(npagopen == 0)
   {
      set_something_val(menu2_top_b1,XmNsensitive, (XtArgVal) True);
      set_something_val(MovePage,XmNsensitive, (XtArgVal) True);
      set_something_val(MoveIcLib,XmNsensitive, (XtArgVal) True);
   }
******/
}

void TopMenuSetInsensitive()
{
   set_something_val(menu1_top_b5,XmNsensitive,(XtArgVal) False);
   set_something_val(menu1_top_b6,XmNsensitive,(XtArgVal) False);
/*   set_something_val(menu2_top_b1,XmNsensitive, (XtArgVal) False);  non esiste piu'*/
   set_something_val(menu2_top_b2,XmNsensitive,(XtArgVal) False);
   set_something_val(menu2_top_b3,XmNsensitive,(XtArgVal) False);
}

void TopMenuSetSensitive()
{
   set_something_val(menu1_top_b5,XmNsensitive,(XtArgVal) True);
   set_something_val(menu1_top_b6,XmNsensitive,(XtArgVal) True);
/*   set_something_val(menu2_top_b1,XmNsensitive, (XtArgVal) True); non esiste piu' */ 
   set_something_val(menu2_top_b2,XmNsensitive,(XtArgVal) True);
   set_something_val(menu2_top_b3,XmNsensitive,(XtArgVal) True);
}


/*------------------------------------------------------------------
 * display resources
 *
 * effettua il display nella interfaccia delle risorse caricate
 * rimuove la lista delle pagine e delle librerie 
 *  e mette le nuove liste
 *-----------------------------------------------------------------*/

int display_resources(PAGEDIT_CONTEXT *paged)
{
   XmString item,mstring;
   int i,j,pos = 0;
   Cardinal num_item;
   PAGINA *pag;
   AN_OBJ *obj;
   typedef struct page_save_st{
        char tagPag[DIMTAG+1];
        char nomepag[MAXCHAR_PAGNAME];
        } PAG_SAVE;
   PAG_SAVE *pag_save;
   char base_dir[255];


   printf("\nexecute display_resource\n");

   setContextTitle(paged);

/* elimino la vecchia lista delle pagine e delle librerie */
   XmListDeleteAllItems(ListPagine);
   XmListDeleteAllItems(LibraryList);

   if(paged->pag_num>0)
       pag_save=(PAG_SAVE*)XtCalloc(paged->pag_num,sizeof(PAG_SAVE));
/* recupero la lista delle pagine */
   for(i=0;i<paged->pag_num;i++)
   {
     
      /*  CAPPE  if( pagina_init(paged->page_list[i],&pag,TIPO_PAGINA) == True ) 
      {
         pagina_load_file(pag,TIPO_PAGINA);     
         GetResPaginaDb(pag,pag->db,True);
	 printf("display_resources: prima di AddResPageContext!!\n");
	 AddResPageContext(pag, paged, i);
         SetItemString(&item,pag->nomepag,pag->geom.tipo,pag->geom.descriz,
                pag->in_use,pag->tagPag);
         add_item(item,0);
         XmStringFree(item); 
         if(strcmp(pag->geom.tipo,TIPO_REGOLAZIONE)==0)
                {
                strcpy(pag_save[i].nomepag,pag->nomepag);
                strcpy(pag_save[i].tagPag,pag->tagPag);
                }
         else
                {
                strcpy(pag_save[i].nomepag,pag->nomepag);
                strcpy(pag_save[i].tagPag,"--");
                }

         pagina_free(pag);
      }
      else
      {
         printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",paged->page_list[i]);
         continue;
      }        CAPPE!!!!  */
      
      /* Nuova situazione! Recupero delle informazioni sulle pagine dal context CAPPE */
      SetItemString(&item,paged->page_list[i],paged->res_page[i]->type_page,paged->res_page[i]->descr_page,
                paged->res_page[i]->in_use,paged->res_page[i]->tagPag);
      add_item(item,0);
/* MAURIC */
      XmStringFree(item);
      if(strcmp(paged->res_page[i]->type_page,TIPO_REGOLAZIONE)==0)
		{
		strcpy(pag_save[i].nomepag,paged->page_list[i]);
                strcpy(pag_save[i].tagPag,paged->res_page[i]->tagPag);
		}
      else
		{
		strcpy(pag_save[i].nomepag,paged->page_list[i]);
                strcpy(pag_save[i].tagPag,"--");
		}
      printf("display_resources: Indice pagina=%d\n",i);
   }
   /* new_save_context(paged);  CAPPE!!! */

    for(i=0;i<paged->pag_num;i++)
        {
	if(strcmp(pag_save[i].tagPag,"--")!=0)
           for(j=i+1;j<paged->pag_num;j++)
                {
                if(strcmp(pag_save[j].tagPag,pag_save[i].tagPag)==0)
                        {
			char message[200];
                        sprintf(message,"regolation pages %s and %s have the same tag [%s]\n",
			pag_save[j].nomepag,pag_save[i].nomepag,pag_save[i].tagPag);
			SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",message,
				NULL,False,NULL,False,NULL);
                        }
                }
        }

   if(paged->pag_num>0)
     XtFree((char*)pag_save);
    
/* recupero la lista delle icone */
   for(i=0;i<paged->iconlib_num;i++)
   {
      if(pagina_init(paged->iconlib_list[i],&pag,TIPO_LIBRARY) == True)      
      {
      pagina_load_file(pag,TIPO_LIBRARY);
      GetResPaginaDb(pag,pag->db,True);
      SetItemString(&item,pag->nomepag,pag->geom.tipo,pag->geom.descriz,
                pag->in_use,pag->tagPag);
      add_library_item(item,0);
      pagina_free(pag);
      XmStringFree(item);
      }
      else
      {
         printf("\n\n\tERRORE INIZIALIZZAZIONE LIBRERIA %s\n",paged->iconlib_list[i]);
         continue;
      }


   }

/* creo le compound string dei dati e setto
   le label della interfaccia con i valori */

   mstring = XmStringCreateSimple(paged->descrizione);
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textDescription1, args, lnargs );
   XmStringFree(mstring);

/*
   mstring = XmStringCreateSimple(XlConvDirVms(paged->project));
*/
printf("DEBUG ContextPath = %s\n",Context_Path);

   strcpy(base_dir,Context_Path);
   getwd(base_dir);
   mstring = XmStringCreateSimple(base_dir);
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textProject, args, lnargs );
   XmStringFree(mstring);

   mstring = XmStringCreateSimple(XlConvDirVms(paged->pages));
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textPages, args, lnargs );
   XmStringFree(mstring);

   mstring = XmStringCreateSimple(XlConvDirVms(paged->objectLibrary));
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textObjectLibraries, args, lnargs );
   XmStringFree(mstring);


   mstring = XmStringCreateSimple(XlConvDirVms(paged->animatedIconLibrary));
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textAnimatedIconLibraries, args, lnargs );
   XmStringFree(mstring);

   mstring = XmStringCreateSimple(XlConvDirVms(paged->simulator));
   lnargs=0;
   XtSetArg(args[lnargs],XmNlabelString,mstring);lnargs++;
   XtSetValues( textSimulator, args, lnargs );
   XmStringFree(mstring);
}



/*------------------------------------------------
 *
 * gest_conf_op
 *
 *  gestione conferma in risposta ad operazioni
 *  e messaggistica in generale
 *
 -------------------------------------------------*/

int gest_conf_op( int tipo_op, char *msg, char *par )
{
   extern swidget create_messageBoxDialog1();
   swidget wid;

   wid = create_messageBoxDialog1(tipo_op,msg,par);
   UxPopupInterface(wid,no_grab);
}

Boolean LibraryIsOpen(char *nome_libreria)
{
   return(False);
}



/*------------------------------------------------------------
 * open_page
 *
 * apertura di una pagina selezionata dalla lista
 *
 *------------------------------------------------------------*/

int open_page(int tipo)
{
   extern XrmDatabase defdb;
   extern swidget create_PagShell();
   swidget wid;
   int *selected,selcount,ind_pagina,i;
   extern Boolean PagIsOpen();

/* permetto di aprire contemporaneamente al massimo MAXPAGOPEN */
   if(npagopen == MAXPAGOPEN)
      return(0);

/* da utilizzare se si desidera aprire una sola pagina

   if(Already_Selected == True)
      return;
**********************************/

/* identifico l'item selezionato */
   if(tipo == TIPO_PAGINA)
   {
      if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) == False )
         return(0);
   }
   else if(tipo == TIPO_LIBRARY)
   {
      if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) == False )
         return(0);
   }

   if(selcount > 1)
      return(0);
   else if(selcount ==0)
      return(0);

   ind_pagina = selected[0] - 1;

/* non apro due volte la stessa pagina */
 
   if( (tipo==TIPO_PAGINA) && (PagIsOpen(pagedit->page_list[ind_pagina]) == True))
   {
      RaisePagina(pagedit->page_list[ind_pagina]);
/****
      SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);
****/
      return(0);
   }
   else if( (tipo==TIPO_LIBRARY) && (PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True))
   {
      RaisePagina(pagedit->iconlib_list[ind_pagina]);
/****
      SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);
*****/
      return(0);
   }


/* con questi dati creo la pagina e ne faccio il popup */

   if( tipo == TIPO_PAGINA )
   {
printf("create_PagShell ind_pagina=%d tipo=%d \n",ind_pagina,tipo);
      wid = create_PagShell(pagedit->page_list[ind_pagina],ind_pagina,tipo);
   }
   else
   {
      wid = create_PagShell(pagedit->iconlib_list[ind_pagina],ind_pagina,tipo);
   }

   UxPopupInterface(wid,no_grab);

/* aggiorno la lista delle pagine aperte */
   PagOpen[npagopen] = wid;
   PagOpenId[npagopen] = ind_pagina;
   npagopen++;

/**
   Already_Selected=True;
**/

   disabilitaMoveMenu();
   UxDisplay->db = defdb;
}

/*------------------------------------------------
 * modify_page
 *
 * modifica dati di tipo geometry  della pagina
 * selezionata
 *-----------------------------------------------*/

int modify_page(int tipo)
{
   extern swidget create_PagShell();
   extern Boolean PagIsOpen();
   swidget wid;
   int *selected,selcount,ind_pagina;

   if(Already_Selected == True)
      return(0);

/* identifico l'item selezionato */
   if(tipo == TIPO_PAGINA )
      if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )
         return(0);

   if(tipo == TIPO_LIBRARY )
      if( XmListGetSelectedPos(LibraryList,&selected,&selcount) == False )
         return(0);

   if(selcount > 1)
      return(0);
   else if(selcount ==0)
      return(0);


   ind_pagina = selected[0] - 1;

/* con questi dati creo la pagina e ne faccio il popup */
   
   if( tipo == TIPO_PAGINA )
      if( PagIsOpen(pagedit->page_list[ind_pagina]) == True) 
      {
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Already Open",NULL,False,NULL,False,NULL);
         return(0);
      }
      else
        wid=create_PagDialog(pagedit->page_list[ind_pagina],MODIFY_PAGE);

   if( tipo == TIPO_LIBRARY)
      if( PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True) 
      {
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Library Is Already Open",NULL,False,NULL,False,NULL);
         return(0);
      } 
      else
         wid=create_PagDialog(pagedit->iconlib_list[ind_pagina],MODIFY_LIBRARY);

   UxPopupInterface(wid,XtGrabNone);
}


/*------------------------------------------------
 * duplicate_page
 *
 * duplicazione dei dati della pagina selezionata
 *--------------------------------------------------*/
int duplicate_page()
{

   extern swidget create_PagDialog();
   swidget wid;
   int *selected,selcount,ind_pagina;

   if(Already_Selected == True)
      return(0);

/* identifico l'item selezionato */
   if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )
      return(0);

   if(selcount > 1)
      return(0);
   else if(selcount ==0)
      return(0);


   ind_pagina = selected[0] - 1;

/* con questi dati creo la pagina e ne faccio il popup */
   wid = create_PagDialog(pagedit->page_list[ind_pagina],COPY_PAGE);
   UxPopupInterface(wid,XtGrabNone);

}
/*---------------------------------------------
 * delete_page
 *
 * cancellazione della pagina selezionata
 *
 *----------------------------------------------*/
int del_page(int tipo)
{
   extern swidget create_PagShell();
   int *selected,selcount,ind_pagina;

   if(Already_Selected == True)
      return(0);

/* identifico l'item selezionato */
   if(tipo == TIPO_PAGINA)
   {
      if( XmListGetSelectedPos(ListPagine,&selected,&selcount) == False )
      {
         printf("return value XmListGetSelectedPos is False\n");
         return(0);
      }
   }
   else if(tipo == TIPO_LIBRARY)
   {
      if( XmListGetSelectedPos(LibraryList,&selected,&selcount) == False )
      {
         printf("return value XmListGetSelectedPos is False\n");
         return(0);
      }
   }
   

  
   if(selcount > 1)
      return(0);
   else if(selcount ==0)
      return(0);


   ind_pagina = selected[0] - 1;

   if(tipo == TIPO_PAGINA)
   {
      if(PagIsOpen(pagedit->page_list[ind_pagina]) == True)
      {
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Page Is Open" ,NULL,False,NULL,False,NULL);
         return(0);
      }
   }
   else if(tipo == TIPO_LIBRARY)
   {
      if(PagIsOpen(pagedit->iconlib_list[ind_pagina]) == True)
      {
         SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Library Is Open" ,NULL,False,NULL,False,NULL);
         return(0);
      }
   }

/* gestisco una interfaccia di conferma cancellazione */
   if(tipo == TIPO_PAGINA)
      gest_conf_op(DELETE_PAGE,"Do you want to delete %s page?",pagedit->page_list[ind_pagina]);
   else
      gest_conf_op(DELETE_LIBRARY,"Do you want to delete %s library?",pagedit->iconlib_list[ind_pagina]);
/**
   Already_Selected=False;
**/
}

/*----------------------------------------------------------
 * Visualizzazione messaggi nella paned window dei messaggi
 *---------------------------------------------------------*/

int show_message(char *mess)
{
  extern FILE *fpLogMsg;
  char   *str, *new_str;

  if(fpLogMsg!=NULL)
        fprintf(fpLogMsg,"%s",mess);

  str = XmTextGetString( scrolledText1 );
  RtRecord(str);
  new_str = (char*) alloca_memoria ( ( strlen(mess)+strlen(str)+1 ),sizeof(char *) );
  strcpy( new_str, str );

  strcat( new_str, mess );
  XmTextSetString( scrolledText1, new_str );
  XmTextShowPosition( scrolledText1,strlen(new_str)-1);
  libera_memoria( (char*)str );
  libera_memoria( (char*)new_str );
  
  XmUpdateDisplay(scrolledText1);

}

int LibraryDeselectAllItem()
{
   XmListDeselectAllItems(LibraryList);
} 

int PageDeselectAllItem()
{
   XmListDeselectAllItems(ListPagine);
}

int clear_topLevel()
{
   PAGEDIT_CONTEXT *appo;

   pagedit_context_init("None",&appo);
   strcpy(appo->fileContext,"BAD CONTEXT"); 
   display_resources(appo);   
   TopMenuSetInsensitive();
}


/*
	Funzione che esegue l'allineamento del context
*/
Boolean AlignResPageContext()
{

PAGINA *pag;
XmString	item;
int		i;
char		*tipo,risorsa[100],val_ris_ger[50],filepag[MAXLENFILENAME];
XrmValue	value;

	XmListDeleteAllItems(ListPagine);
	for(i=0;i<pagedit->pag_num;i++)
	{
		if( pagina_init(pagedit->page_list[i],&pag,TIPO_PAGINA) == True )
		{
		   printf("AlignResPageContext: prima di pagina pagina_load_file\n");
		   pagina_load_file(pag,TIPO_PAGINA);
         	   GetResPaginaDb(pag,pag->db,True);
		   /* Gestione dell'eventualita' che nel file .pag la risorsa 'gerarchia'
		      sia ancora rappresentata con il vecchio formato [][][][][][].	*/

		   /*   Costruzione del path del file .pag corrente.	          	*/
		   strcpy(filepag,getenv(ENVPAG));
                   strcat(filepag,"/");
                   strcat(filepag,pagedit->page_list[i]);
                   strcat(filepag,PAG_EXT);

		   sprintf(risorsa,"%s.gerarchia",pagedit->page_list[i]);
		   if(XrmGetResource(pag->db, risorsa, (char *)NULL, &tipo, &value)!=NULL) {
		      /* La risorsa 'gerarchia' e' presente nel file.pag corrente */  
		      strncpy(val_ris_ger,value.addr,(int)value.size);
		      if(!strcmp(val_ris_ger,"[][][][][][]")) {
			 /* La risorsa gerarchia e' stata salvata con il vecchio formato.
			    Sostituzione.						  */ 
		         strcpy(val_ris_ger,"-1,-1,-1,-1,-1,-1");
		         XrmPutStringResource(&pag->db,"*gerarchia",val_ris_ger);
		         XrmPutFileDatabase(pag->db,filepag);
		      }
		   }
		   else {
		      /* La risorsa 'gerarchia' non e' presente nel file .pag corrente.
			 Inserimento con il valore -1,-1,-1,-1,-1,-1			*/
		      strcpy(val_ris_ger,"-1,-1,-1,-1,-1,-1");
		      XrmPutStringResource(&pag->db,"*gerarchia",val_ris_ger);
		      XrmPutFileDatabase(pag->db,filepag);
		   }
		   strcpy(pagedit->res_page[i]->hier, val_ris_ger);
         	   strcpy(pagedit->res_page[i]->descr_page, pag->geom.descriz);
		   strcpy(pagedit->res_page[i]->type_page, pag->geom.tipo);
		   pagedit->res_page[i]->refresh_freq = pag->refreshFreq;
		   pagedit->res_page[i]->in_use = pag->in_use;
		   strcpy(pagedit->res_page[i]->tagPag,pag->tagPag);
		   SetItemString(&item,pagedit->page_list[i],pagedit->res_page[i]->type_page,
				 pagedit->res_page[i]->descr_page,pagedit->res_page[i]->in_use,
				 pagedit->res_page[i]->tagPag);
		   add_item(item,0);
/* MAURIC */
                   XmStringFree(item);
		   pagina_free(pag);
		}
		else
		   printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",pagedit->page_list[i]);
	}
	printf("AlignResPageContext: prima di new_save_context\n");
	if(new_save_context( pagedit )!=True)
		SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Save of Context failed.",
		       NULL,False,NULL,False,NULL);

}



/*
	Funzione per la gestione della voce di menu' "Print all pages"
*/

void PrintAllPages()
{

extern int PrintClosePag();
int i;
char str[200];
swidget wid;

   extern swidget create_PagShell();

 if(pagedit->pag_num != 0) 
 {
    if(npagopen == 0) 
    {
       def_cursore(topLevelShell, OROLOGIO);
       for(i=0; i<pagedit->pag_num; i++) 
       {

           wid = create_PagShell(pagedit->page_list[i], i, TIPO_PAGINA);
           PagOpen[npagopen] = wid;
           PagOpenId[npagopen] = i;
           npagopen++;
          PrintClosePag(pagedit->page_list[i]);
       }
       undef_cursore(topLevelShell);
       sprintf(str,"Printing Pages %s",topLevelShell);
       SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",str,NULL,
		False,NULL,False,NULL);	
   }
   else
   {
        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","All pages must be closed",NULL,
           False,NULL,False,NULL);
   }
}
else 
{
    SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","No pages in ListPages",NULL,
       False,NULL,False,NULL);
}

}



int CheckOlTree(OlDatabaseTopologiaObject dbtopologia)
{

   FILE *fpLogOlTree;
   int i,j,hier_pb,problems=0,alarm_state=0,fnomi_err=0;
   int pos_info,cont_root=0,curr_lev;
   int num_pages;
/*
   OlTreeObject root;
*/
   char msg[200],path_log[200],fnomirtf[200],root_hierarchy[50],root_name[MAXCHAR_PAGNAME];

   /* Apertura del file di log 'OlTree.log' */
   strcpy(path_log,XlGetenv(ENVCTX));
   strcat(path_log,"/OlTree.log");
   if((fpLogOlTree=fopen(path_log,"w"))!=NULL) {
      /* Controllo dell'esistenza del file fnomi.rtf */
      if(XlGetenv(ENVRTF)==NULL) {
         printf("Imposssibile trovare file fnomi.rtf !!!\n");
         show_message("Impossible to find fnomi.rtf!! Only hierarchy will be checked.\n");
         fnomi_err=-1;
      }
      else {
         strcpy(fnomirtf,XlGetenv(ENVRTF));
         strcat(fnomirtf,"/fnomi.rtf");
         if(access(fnomirtf,F_OK)!=0) {
            /* fnomi.rtf non trovato */
            show_message("fnomi.rtf not found!! Only hierarchy will be checked.\n");
            fnomi_err=-1;
         }
         else {
            fnomi_err=0;
         }
      }      
      /* Pagine presenti attualmente nel context */
      num_pages=pagedit->pag_num;
      /* Allocazione della radice dell'albero (OlTree) */
      for(j=0; j<num_pages; j++) {
         /* Controllo che la pagina corrente sia di tipo SINOTTICO o STAZIONE */
         if((strcmp(pagedit->res_page[j]->type_page,TIPO_SINOTTICO)) ||
           (strcmp(pagedit->res_page[j]->type_page,TIPO_STAZIONI))) {
            curr_lev=OlTreeGetLevel(pagedit->res_page[j]->hier);
            if(curr_lev==0) {
               /* Trovata una pagina con le caratteristiche della radice */
               if(cont_root!=0) {
               /* Esistono piu' radici. Errore!! */
		  show_message("HIERARCHY PROBLEM!! More pages have root requirements!\n");
		  sprintf(msg,"Page: %s\tHierarchy: %s\n",root_name,root_hierarchy);
		  show_message(msg);
		  sprintf(msg,"Page: %s\tHierarchy: %s\n",
			  pagedit->page_list[j],pagedit->res_page[j]->hier);
		  show_message(msg);
                  fclose(fpLogOlTree);
                  return(-2);
               }
               else {
		  strcpy(root_hierarchy,pagedit->res_page[j]->hier);
		  strcpy(root_name,pagedit->page_list[j]);
                  pos_info=j;
                  cont_root++;
               }
            }
         }
      }
      if(cont_root==0) {
         /* Non esistono pagine con le caratteristiche della root */
	 show_message("HIERARCHY PROBLEM!! A page with root requirements is not present!\n");
         fclose(fpLogOlTree);
         return(-2);
      }
      if((root_oltree=OlCreateTree(dbtopologia,pos_info,pagedit->res_page[pos_info]->hier,&alarm_state))==NULL) {
         /* Errore nell'allocazione della radice di OlTree */
         show_message("HIERARCHY PROBLEM!! Error in root allocation\n");
         fclose(fpLogOlTree);
         return(-2);
      }
      else {
         printf("RADICE ALLOCATA!! %s\n",pagedit->res_page[pos_info]->hier); 
         if((alarm_state==-1) && (fnomi_err==0)) {
            /* Radice caricata correttamente ma problemi sugli allarmi */
            problems=-1;
            sprintf(msg,"ALARM PROBLEM!! Page %s. Hierarchy value=%s\nWARNING Alarm description variable not found in fnomi.rtf\n\n",pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier );
         }
         else {
            if(fnomi_err==0)
               sprintf(msg,"HIERARCHY/ALARM OK!! Page root=%s. Hierarchy value=%s\n\n",
                       pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier);
            else
               sprintf(msg,"HIERARCHY OK/ALARM NOT CHECKED!! Page root=%s. Hierarchy value=%s\n\n",
                       pagedit->page_list[pos_info],pagedit->res_page[pos_info]->hier);
         }
         printf("%s",msg);
         fprintf(fpLogOlTree,"%s",msg);
      }

      /* Caricamento dell'albero (OlTree) */
      for(i=1; i<NUM_MAX_LIV; i++) {
         printf("Build_OlTree: Caricamento del livello=%d\n", i);
         for(j=0; j<num_pages; j++) {
            /* Controllo che la pagina corrente sia di tipo SINOTTICO o STAZIONE */
            if((strcmp(pagedit->res_page[j]->type_page,TIPO_SINOTTICO)) ||
               (strcmp(pagedit->res_page[j]->type_page,TIPO_STAZIONI))) {
               curr_lev=OlTreeGetLevel(pagedit->res_page[j]->hier);
               if(curr_lev==i) {
                  /* Trovata una pagina di livello i. Inserimento. */
                  if((hier_pb=OlTreeAddPage(dbtopologia,root_oltree,curr_lev,j,pagedit->res_page[j]->hier))<0) {
                     /* Riscontrati problemi sull'oggetto OlTree */
                     switch (hier_pb) {
                        case -1:
                           problems=-1;
                           sprintf(msg,"HIERARCHY PROBLEM!! Page %s without father.\tHierarchy value=%s\n\n",
                                   pagedit->page_list[j],pagedit->res_page[j]->hier);
                           break;
                        case -2:
                           problems=-1;
                           sprintf(msg,"HIERARCHY PROBLEM!! Page %s. A page with hierarchy value %s already present\n\n",
                                   pagedit->page_list[j],pagedit->res_page[j]->hier );
                           break;
                        case -3:
                           /* Pagina inserita in gerarchia ma errori negli allarmi */
                           if(fnomi_err==0) {
                              problems=-1;
                              sprintf(msg,"ALARM PROBLEM!! Page %s. Hierarchy value=%s\n\tAlarm description variable not found in fnomi.rtf\n\n",
                                      pagedit->page_list[j],pagedit->res_page[j]->hier );
                           }
                           else {
                              sprintf(msg,"HIERARCHY OK/ALARM NOT CHECKED!!Page %s. Hierarchy value=%s\n\n",
                                      pagedit->page_list[j],pagedit->res_page[j]->hier );
                           }
                           break;
                     }
                     fprintf(fpLogOlTree,"%s",msg);
                  }
                  else { 
                     /* Pagina corrente inserita in gerarchia e allarmi corretti */
                     sprintf(msg,"HIERARCHY/ALARM OK!! Page %s. Hierarchy value=%s\n\n",
                             pagedit->page_list[j],pagedit->res_page[j]->hier );
                     fprintf(fpLogOlTree,"%s",msg);
                  }

               }
            }
         }
      }
      fclose(fpLogOlTree);
   }
   else {
      /* Errore nell'apertura del file OlTree.log */
      show_message("Error opening file OlTree.log!!\n");
      return(-2);
   }
   if (problems==-1)
      return(-1);
   else
      return(0);

}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_menu1Define( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget creat_DefineContext();
	  /*
		swidget swid;
	        swid = creat_ContextSelection(DEF_CONTEXT);
	  */
	   UxPopupInterface(creat_DefineContext(pagedit->fileContext,MOD_CONTEXT),no_grab);
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Align( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	extern swidget create_messageBoxDialog1();
	swidget wid;
	if(npagopen == 0) {
	   wid = create_messageBoxDialog1(ALIGN_CONTEXT,"Do you want to align context?",NULL);
	   UxPopupInterface(wid,no_grab);
	}
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Check( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	
	   int check_result;
	   check_result=CheckOlTree(dbtopologia);
	   switch (check_result) {
	      case 0:
	         OlDestroyTree(root_oltree);  
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking OK!",NULL,False,NULL,False,NULL);
	         break;
	      case -1:
	         OlDestroyTree(root_oltree);
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking failed! See logfile OlTree.log",NULL,False,NULL,False,NULL);
	         break;
	      case -2:
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy checking failed! See window messages!!",NULL,False,NULL,False,NULL);
	         break;
	   } 
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_all_but( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
#ifdef XPRINTER_USED
	extern swidget create_messageBoxDialog1();
	swidget wid;
	wid = create_messageBoxDialog1(PRINT_ALL,"Print all pages. Are you sure?",NULL);
	UxPopupInterface(wid,no_grab);
#else
	   
	SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Print not allowed for current version",NULL,
	       False,NULL,False,NULL);
	
#endif
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_select_but( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
#ifdef XPRINTER_USED
	extern int PrintClosePag();
	int *selected, selcount, i;
	char str[200];
	swidget wid;
	
	extern swidget create_PagShell();
	   
	if(XmListGetSelectedPos(UxGetWidget(ListPagine), &selected, &selcount) != FALSE) 
	{
	    /* Almeno una pagina e' stata selezionata */
	    if(npagopen == 0) 
	    {
	       def_cursore(topLevelShell, OROLOGIO); 
	       for(i=0; i<selcount; i++) 
	       {
	
	          wid = create_PagShell(pagedit->page_list[selected[i]-1], (selected[i]-1),
	                TIPO_PAGINA);
	          PagOpen[npagopen] = wid;
	          PagOpenId[npagopen] = (selected[i]-1);
	          npagopen++;
	          PrintClosePag(pagedit->page_list[selected[i]-1]);
	       }
	
	       undef_cursore(topLevelShell);
	       sprintf(str,"Printing Pages %s",topLevelShell);
	       SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo",str,NULL,
			False,NULL,False,NULL);
	   }
	   else 
	   {
	       SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","All pages must be closed",NULL,
	        False,NULL,False,NULL);
	   }
	}
	else
	{
	    SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","No pages selected",NULL,
	        False,NULL,False,NULL);
	}
#else
	
	SetMsg(topLevelShell,NULL,WRNMSG,"Config Info","Print not allowed for current version",
	       NULL,False,NULL,False,NULL);
#endif
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1PrintSet( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
#ifdef XPRINTER_USED
	
	   extern Widget PrintSetup();
	
	   PrintSetup(topLevelShell);
#else
	   SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Printer use not allowed for this version",NULL,False,NULL,False,NULL);    
#endif
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Exit1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern Boolean ContextModified;
	
	   if( ContextModified )
	      gest_conf_op(EXIT_WITH_SAVE,"EXIT - Context is not Saved. Do you Want To Save ?",(char*)pagedit);
	   else
	      gest_conf_op(EXIT_NO_CHANGE,"EXIT - Do you really want to Exit?",(char*)NULL);
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_Page( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget create_PagDialog();
	   Widget wid;
	   printf("\nCreate Page function called\n");
	
	   wid = (Widget )create_PagDialog((char *)NULL,CREATE_PAGE);
	
	   UxPopupInterface(wid,no_grab);
	   
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_Regolation( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget create_PagDialog();
	   Widget wid;
	   printf("\nCreate regolation function called\n");
	
	   wid = (Widget )create_PagDialog((char *)NULL,CREATE_REGOLATION);
	
	   UxPopupInterface(wid,no_grab);
	   
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_Library( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget create_PagDialog();
	   Widget wid;
	   printf("\nCreate library function called\n");
	
	   wid = (Widget )create_PagDialog((char *)NULL,CREATE_LIBRARY);
	
	   UxPopupInterface(wid,no_grab);
	   
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_StandardLibrary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	
	   extern void library_gen();
	   
	
	
	   library_gen(STD_LIB_NAME,STD_LIB_NAME);
	  
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Open( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	  int *selected,selcount;
	
	  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   
	     open_page(TIPO_PAGINA);
	  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )
	     open_page(TIPO_LIBRARY);   
	
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Modify( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	  int *selected,selcount;
	
	  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   
	     modify_page(TIPO_PAGINA);
	  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )
	     modify_page(TIPO_LIBRARY);   
	
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu1Delete1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	  int *selected,selcount;
	
	  if( XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount) != False )   
	     {
	     del_page(TIPO_PAGINA);
	     }
	  else if( XmListGetSelectedPos(UxGetWidget(LibraryList),&selected,&selcount) != False )
	     del_page(TIPO_LIBRARY);   
	
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_ImportPage( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget  create_PageSelection();
	   swidget swid;
	
	   swid = create_PageSelection(IMPORT_PAGE);
	
	   UxPopupInterface(swid,no_grab);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_ImportLibrary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   extern swidget  create_PageSelection();
	   swidget swid;
	
	   swid = create_PageSelection(IMPORT_LIB);
	
	   UxPopupInterface(swid,no_grab);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_ImportIcon( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	 
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_CompileAll( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	       compile_all_pag();
#if defined MAURIZIO
	           /*compilazione di tutte le pagine nel context */
	           extern Cardinal lnargs;
	           extern Arg args[];
	           extern OlCompilerObject compilatore_severe;
	           extern XrmDatabase defdb;
	           int err_level = 0;
	           WidgetList children;
	           Cardinal num_children;
	           char outname[105],mesg[200];
	           int i,j;
	           PAGINA *pag=NULL;
	           AN_OBJ *lista_oggetti=NULL;
	           WidgetList lista;
	           int g,m,a,o,min,s;
	
		   XEvent event;
		   XtAppContext AppContMom;
		   Widget TopLevelMom;
		   Display *DisplayMom;
		   Display *AppDisplay;
		   Widget AppTopLevel;
		   int numero=0;
	
	           if( getenv(ENVPAG) == NULL)
	              return;
	
		   AppTopLevel=UxTopLevel;
		   AppDisplay=UxDisplay;
	
		   if(CompileAppContext == NULL)
			{
		   	AppContMom= XtCreateApplicationContext();
		   	DisplayMom=XtOpenDisplay(AppContMom,NULL,"configMom","ConfigMom",NULL,0,&numero,NULL);
		   	UxTopLevel=XtAppCreateShell("configMom",
	                		"ConfigMom",applicationShellWidgetClass, 
					DisplayMom, NULL, 0);
		        UxDisplay=DisplayMom;
			CompileAppContext = AppContMom;
			CompileDisplay = DisplayMom;
			CompileTopLevel = UxTopLevel;
			}
		   else
			{
			AppContMom=CompileAppContext;
			DisplayMom = CompileDisplay;
			UxTopLevel = CompileTopLevel;
			UxDisplay=DisplayMom;
			}
	
	           for(i=0;i<pagedit->pag_num;i++)
	           {
		    XdLista lista_save;
	            Widget ClipSave;
#ifndef DESIGN_TIME
		    extern Widget ClipBoard;
#endif
		    lista_save = clip_get_lista();
	            ClipSave = ClipBoard;
	
	               CompileBoard = create_ClipBoard();
	
	               if( pagina_init( pagedit->page_list[i], &pag,TIPO_PAGINA) == False)
	                  return;
			
		       printf("Compilazione pagina [%s]\n",pag->nomepag);
	
	               if( pagina_load_file(pag,TIPO_PAGINA)  && pagina_getres(pag,&lista_oggetti) )
	               {
	
	                  if(pag->num_widget > 0)
	                  {
	                     if( (lista = alloca_memoria(pag->num_widget,sizeof(Widget))) == NULL )
	                        return;
	
	                     UxDisplay->db = pag->db;
	
	                     for(j=0;j<pag->num_widget;j++)
	                        lista[j] = XtCreateWidget(lista_oggetti[j].nome,lista_oggetti[j].classe,CompileBoard,NULL,0);
			     pag->drawing=CompileBoard;
	
	                     OlSetCompiler(lista,pag->num_widget,compilatore_severe);
	
	                     strcpy(outname,getenv(ENVPAG));
	                     strcat(outname,"/");
	                     strcat(outname,pag->nomepag);
	                     strcat(outname,".rtf");
			     /*
				richiamo la routine PostOpenPage2 per settare le
				risorse, prelevate da Connessioni.reg, nelle 
				eventuali icone di interffaccia
			     */
			     PostOpenPage2(CompileBoard,pag->nomepag,pag->db);
	
	                     printf("Chiamo il compilatore e metto il risultato in %s\n",outname);
	
	                     if( (err_level = XlCompilaWidget(pag->db,outname,lista,pag->num_widget) ) == False)
	                     {
	                        sprintf(mesg,"Error at Compile Time on Page %s\n",pag->nomepag);
	                        printf("error at Compile time mesg = %s\n",mesg);
	                        show_message(mesg);
	                     }
	                     else
	                     {
	                        /*       
		 			setto la data della compilazione
	   			*/
	                        data(&g,&m,&a);
	   			ora(&o,&min,&s);
				if ( a < 10 )
				   {
	   			      sprintf(pag->compilationDate, "%d/%d/0%d %d:%d:%d",g,m,a,o,min,s);	
				   }
				   else
				   {
	   			      sprintf(pag->compilationDate, "%d/%d/%d %d:%d:%d",g,m,a,o,min,s);	
				   }
				/*
	        			forzo il salvataggio della pagina
	   			*/
#ifndef DESIGN_TIME
	   			pagina_save(pag,TIPO_PAGINA,0);
#endif
				sprintf(mesg,"Page Compiled  %s,  Ok\n",pag->nomepag);
	                        printf("Page Compiled  %s\n",mesg);
	                        show_message(mesg);
	                     }
	
	                     for(j=0;j<pag->num_widget;j++)
	                        XtDestroyWidget(lista[j]);
	
	                     libera_memoria((char*)lista);
	                 }
			 pagina_free(pag);
		         libera_memoria((char*)lista_oggetti);
	              }
		      else
	              	 libera_memoria((char*)pag);
	
	                UxDestroyInterface(CompileBoard);
		   	XSync(XtDisplay(UxTopLevel),False);
	      		while(XtAppPending(AppContMom))
	                {
	                XtAppNextEvent(AppContMom,&event);
	                XtDispatchEvent(&event);
	                }
	clip_set_lista(lista_save);
	           ClipBoard =ClipSave;
	             UxDisplay->db = NULL;
	           }
	
		   UxTopLevel = AppTopLevel;
		   UxDisplay = AppDisplay;
		   UxDisplay->db = defdb;
	           
		
	           show_message("\nCompilation End\n");
#endif        
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_CompileAllRegola( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	compile_all_reg();
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_CompileTask( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	info_top_schemi();
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_ViewErrTask( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	char error_file[100],comando[200],appoggio[200];
	char nome_task[100];
	extern int estrai_nome(char *, char*);
	
	if( getenv(ENVPAG) == NULL)
	   return;
	
#ifndef VMS
	strcpy(error_file,XlGetenv(ENVPAG));
	strcat(error_file,"/");
	strcpy(appoggio,XlGetenv(ENVPAG));
#else
	strcpy(error_file,XlConvDirVms(XlGetenv(ENVPAG)));
	strcpy(appoggio,XlConvDirVms(XlGetenv(ENVPAG)));
#endif
	OlEstrRegTaskName(appoggio,nome_task);
	strcat(error_file,nome_task);
	strcat(error_file,REG_EXT);
	strcat(error_file,ERR_EXT);
	
	strcpy(comando,"edit_f01   ");
	strcat(comando,error_file);
	strcat(comando,"  False &");
	
	if( system(NULL) != 0)
	   system( comando );  
#endif
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	valueChangedCB_Mode( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	  extern void  set_interface();
	   
	  if(StateInterfaceMode == True)
	  {
	     if(npagopen > 0)
	        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Close all the page first." ,NULL,False,NULL,False,NULL);
	     else 
	     {
	        StateInterfaceMode = False;
	        set_interface(StateInterfaceMode);
	        if(ContextLoaded)
	           TopMenuSetSensitive();
	     }
	  } 
	  else
	  {
	     if(npagopen > 0)
	        SetMsg(topLevelShell,NULL,WRNMSG,"ConfigInfo","Close all the page first." ,NULL,False,NULL,False,NULL);
	     else 
	     {
	        StateInterfaceMode = True;
	        set_interface(StateInterfaceMode);
	        if(ContextLoaded)
	           TopMenuSetInsensitive();
	     }
	  } 
	  set_something_val(Mode,XmNset,(XtArgVal) StateInterfaceMode);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	singleSelectionCB_ListPagine( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	int *selected,selcount;
	
	LibraryDeselectAllItem();
	XmListGetSelectedPos(UxGetWidget(ListPagine),&selected,&selcount);
	      
	if(selcount > 0)
	   PaginaSelezionata = selected[0];
	else
	   PaginaSelezionata = 0;
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	defaultActionCB_ListPagine( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	open_page(TIPO_PAGINA);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_PBOpen( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   open_page(TIPO_PAGINA);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_PBModify( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   modify_page(TIPO_PAGINA);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_PBDuplicate( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   duplicate_page();
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_PBDelete( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   del_page(TIPO_PAGINA);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	singleSelectionCB_LibraryList( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	PageDeselectAllItem(ListPagine);
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	defaultActionCB_LibraryList( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	PageDeselectAllItem();
	open_page(TIPO_LIBRARY);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_OpenLibrary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   open_page(TIPO_LIBRARY);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_ModifyLibrary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   modify_page(TIPO_LIBRARY);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_DeleteLibrary( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	   del_page(TIPO_LIBRARY);
	}
	UxTopLevelShellContext = UxSaveCtx;
}

static	void	activateCB_menu5_p1_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCtopLevelShell       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxTopLevelShellContext;
	UxTopLevelShellContext = UxContext =
			(_UxCtopLevelShell *) UxGetContext( UxWidget );
	{
	
	   XmTextSetString(scrolledText1,(char *)"");
	
	}
	UxTopLevelShellContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_topLevelShell()
{
	Widget		menu1File1_shell;
	Widget		print_menu_shell;
	Widget		menu1Create1_shell;
	Widget		menu1Edit1_shell;
	Widget		menu1Import_shell;
	Widget		menu1Compile_shell;
	Widget		InterfaceMode_shell;
	Widget		menu1_shell;
	Widget		menu6_shell;
	Widget		menu5_shell;


	/* Creation of topLevelShell */
	topLevelShell = XtVaCreatePopupShell( "topLevelShell",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 285,
			XmNy, 180,
			XmNwidth, 675,
			XmNheight, 789,
			XmNiconName, "LEGOMMI - Config",
			NULL );
	UxPutContext( topLevelShell, (char *) UxTopLevelShellContext );


	/* Creation of mainWindow */
	mainWindow = XtVaCreateManagedWidget( "mainWindow",
			xmMainWindowWidgetClass,
			topLevelShell,
			XmNunitType, XmPIXELS,
			XmNx, 5,
			XmNy, 10,
			XmNwidth, 650,
			XmNheight, 630,
			NULL );
 /* 	mainWindow = XtVaCreateManagedWidget( "mainWindow",
			xmMainWindowWidgetClass,
			topLevelShell,
			NULL ); 
 */	
	UxPutContext( mainWindow, (char *) UxTopLevelShellContext );


	/* Creation of menu2 */
	menu2 = XtVaCreateManagedWidget( "menu2",
			xmRowColumnWidgetClass,
			mainWindow,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( menu2, (char *) UxTopLevelShellContext );


	/* Creation of menu1File1 */
	menu1File1_shell = XtVaCreatePopupShell ("menu1File1_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1File1 = XtVaCreateWidget( "menu1File1",
			xmRowColumnWidgetClass,
			menu1File1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu1File1, (char *) UxTopLevelShellContext );


	/* Creation of menu1Define */
	menu1Define = XtVaCreateManagedWidget( "menu1Define",
			xmPushButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Define Context" ),
			NULL );
	XtAddCallback( menu1Define, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Define,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Define, (char *) UxTopLevelShellContext );


	/* Creation of menu1Align */
	menu1Align = XtVaCreateManagedWidget( "menu1Align",
			xmPushButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Align Context" ),
			NULL );
	XtAddCallback( menu1Align, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Align,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Align, (char *) UxTopLevelShellContext );


	/* Creation of separatore3 */
	separatore3 = XtVaCreateManagedWidget( "separatore3",
			xmSeparatorWidgetClass,
			menu1File1,
			NULL );
	UxPutContext( separatore3, (char *) UxTopLevelShellContext );


	/* Creation of menu1Check */
	menu1Check = XtVaCreateManagedWidget( "menu1Check",
			xmPushButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Check Hierarchy" ),
			NULL );
	XtAddCallback( menu1Check, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Check,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Check, (char *) UxTopLevelShellContext );


	/* Creation of separatore4 */
	separatore4 = XtVaCreateManagedWidget( "separatore4",
			xmSeparatorGadgetClass,
			menu1File1,
			NULL );
	UxPutContext( separatore4, (char *) UxTopLevelShellContext );


	/* Creation of print_menu */
	print_menu_shell = XtVaCreatePopupShell ("print_menu_shell",
			xmMenuShellWidgetClass, menu1File1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	print_menu = XtVaCreateWidget( "print_menu",
			xmRowColumnWidgetClass,
			print_menu_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( print_menu, (char *) UxTopLevelShellContext );


	/* Creation of all_but */
	all_but = XtVaCreateManagedWidget( "all_but",
			xmPushButtonWidgetClass,
			print_menu,
			RES_CONVERT( XmNlabelString, "All Pages" ),
			NULL );
	XtAddCallback( all_but, XmNactivateCallback,
		(XtCallbackProc) activateCB_all_but,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( all_but, (char *) UxTopLevelShellContext );


	/* Creation of select_but */
	select_but = XtVaCreateManagedWidget( "select_but",
			xmPushButtonWidgetClass,
			print_menu,
			RES_CONVERT( XmNlabelString, "Selected Pages" ),
			NULL );
	XtAddCallback( select_but, XmNactivateCallback,
		(XtCallbackProc) activateCB_select_but,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( select_but, (char *) UxTopLevelShellContext );


	/* Creation of print_entry */
	print_entry = XtVaCreateManagedWidget( "print_entry",
			xmCascadeButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Print ..." ),
			XmNsubMenuId, print_menu,
			NULL );
	UxPutContext( print_entry, (char *) UxTopLevelShellContext );


	/* Creation of menu1PrintSet */
	menu1PrintSet = XtVaCreateManagedWidget( "menu1PrintSet",
			xmPushButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Printer Setup..." ),
			NULL );
	XtAddCallback( menu1PrintSet, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1PrintSet,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1PrintSet, (char *) UxTopLevelShellContext );


	/* Creation of separatore6 */
	separatore6 = XtVaCreateManagedWidget( "separatore6",
			xmSeparatorWidgetClass,
			menu1File1,
			NULL );
	UxPutContext( separatore6, (char *) UxTopLevelShellContext );


	/* Creation of menu1Exit1 */
	menu1Exit1 = XtVaCreateManagedWidget( "menu1Exit1",
			xmPushButtonWidgetClass,
			menu1File1,
			RES_CONVERT( XmNlabelString, "Exit" ),
			NULL );
	XtAddCallback( menu1Exit1, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Exit1,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Exit1, (char *) UxTopLevelShellContext );


	/* Creation of menu1_top_b4 */
	menu1_top_b4 = XtVaCreateManagedWidget( "menu1_top_b4",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "File" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, menu1File1,
			NULL );
	UxPutContext( menu1_top_b4, (char *) UxTopLevelShellContext );


	/* Creation of menu1Create1 */
	menu1Create1_shell = XtVaCreatePopupShell ("menu1Create1_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1Create1 = XtVaCreateWidget( "menu1Create1",
			xmRowColumnWidgetClass,
			menu1Create1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu1Create1, (char *) UxTopLevelShellContext );


	/* Creation of Page */
	Page = XtVaCreateManagedWidget( "Page",
			xmPushButtonWidgetClass,
			menu1Create1,
			RES_CONVERT( XmNlabelString, "Page" ),
			NULL );
	XtAddCallback( Page, XmNactivateCallback,
		(XtCallbackProc) activateCB_Page,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( Page, (char *) UxTopLevelShellContext );


	/* Creation of Regolation */
	Regolation = XtVaCreateManagedWidget( "Regolation",
			xmPushButtonWidgetClass,
			menu1Create1,
			RES_CONVERT( XmNlabelString, "Regolation" ),
			NULL );
	XtAddCallback( Regolation, XmNactivateCallback,
		(XtCallbackProc) activateCB_Regolation,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( Regolation, (char *) UxTopLevelShellContext );


	/* Creation of Library */
	Library = XtVaCreateManagedWidget( "Library",
			xmPushButtonWidgetClass,
			menu1Create1,
			RES_CONVERT( XmNlabelString, "Library" ),
			NULL );
	XtAddCallback( Library, XmNactivateCallback,
		(XtCallbackProc) activateCB_Library,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( Library, (char *) UxTopLevelShellContext );


	/* Creation of StandardLibrary */
	StandardLibrary = XtVaCreateManagedWidget( "StandardLibrary",
			xmPushButtonWidgetClass,
			menu1Create1,
			RES_CONVERT( XmNlabelString, "Standard Library" ),
			NULL );
	XtAddCallback( StandardLibrary, XmNactivateCallback,
		(XtCallbackProc) activateCB_StandardLibrary,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( StandardLibrary, (char *) UxTopLevelShellContext );


	/* Creation of menu1_top_b5 */
	menu1_top_b5 = XtVaCreateManagedWidget( "menu1_top_b5",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Create" ),
			XmNsubMenuId, menu1Create1,
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	UxPutContext( menu1_top_b5, (char *) UxTopLevelShellContext );


	/* Creation of menu1Edit1 */
	menu1Edit1_shell = XtVaCreatePopupShell ("menu1Edit1_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1Edit1 = XtVaCreateWidget( "menu1Edit1",
			xmRowColumnWidgetClass,
			menu1Edit1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu1Edit1, (char *) UxTopLevelShellContext );


	/* Creation of menu1Open */
	menu1Open = XtVaCreateManagedWidget( "menu1Open",
			xmPushButtonWidgetClass,
			menu1Edit1,
			RES_CONVERT( XmNlabelString, "Open" ),
			NULL );
	XtAddCallback( menu1Open, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Open,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Open, (char *) UxTopLevelShellContext );


	/* Creation of menu1Modify */
	menu1Modify = XtVaCreateManagedWidget( "menu1Modify",
			xmPushButtonWidgetClass,
			menu1Edit1,
			RES_CONVERT( XmNlabelString, "Modify" ),
			NULL );
	XtAddCallback( menu1Modify, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Modify,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Modify, (char *) UxTopLevelShellContext );


	/* Creation of menu1Delete1 */
	menu1Delete1 = XtVaCreateManagedWidget( "menu1Delete1",
			xmPushButtonWidgetClass,
			menu1Edit1,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( menu1Delete1, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu1Delete1,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu1Delete1, (char *) UxTopLevelShellContext );


	/* Creation of menu1_top_b6 */
	menu1_top_b6 = XtVaCreateManagedWidget( "menu1_top_b6",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Edit" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			XmNsubMenuId, menu1Edit1,
			NULL );
	UxPutContext( menu1_top_b6, (char *) UxTopLevelShellContext );


	/* Creation of menu1Import */
	menu1Import_shell = XtVaCreatePopupShell ("menu1Import_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1Import = XtVaCreateWidget( "menu1Import",
			xmRowColumnWidgetClass,
			menu1Import_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu1Import, (char *) UxTopLevelShellContext );


	/* Creation of ImportPage */
	ImportPage = XtVaCreateManagedWidget( "ImportPage",
			xmPushButtonWidgetClass,
			menu1Import,
			RES_CONVERT( XmNlabelString, "Page" ),
			NULL );
	XtAddCallback( ImportPage, XmNactivateCallback,
		(XtCallbackProc) activateCB_ImportPage,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ImportPage, (char *) UxTopLevelShellContext );


	/* Creation of ImportLibrary */
	ImportLibrary = XtVaCreateManagedWidget( "ImportLibrary",
			xmPushButtonWidgetClass,
			menu1Import,
			RES_CONVERT( XmNlabelString, "Library" ),
			NULL );
	XtAddCallback( ImportLibrary, XmNactivateCallback,
		(XtCallbackProc) activateCB_ImportLibrary,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ImportLibrary, (char *) UxTopLevelShellContext );


	/* Creation of ImportIcon */
	ImportIcon = XtVaCreateManagedWidget( "ImportIcon",
			xmPushButtonWidgetClass,
			menu1Import,
			RES_CONVERT( XmNlabelString, "Icon File" ),
			NULL );
	XtAddCallback( ImportIcon, XmNactivateCallback,
		(XtCallbackProc) activateCB_ImportIcon,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ImportIcon, (char *) UxTopLevelShellContext );


	/* Creation of menu2_top_b2 */
	menu2_top_b2 = XtVaCreateManagedWidget( "menu2_top_b2",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Import" ),
			XmNsubMenuId, menu1Import,
			NULL );
	UxPutContext( menu2_top_b2, (char *) UxTopLevelShellContext );


	/* Creation of menu1Compile */
	menu1Compile_shell = XtVaCreatePopupShell ("menu1Compile_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1Compile = XtVaCreateWidget( "menu1Compile",
			xmRowColumnWidgetClass,
			menu1Compile_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu1Compile, (char *) UxTopLevelShellContext );


	/* Creation of CompileAll */
	CompileAll = XtVaCreateManagedWidget( "CompileAll",
			xmPushButtonWidgetClass,
			menu1Compile,
			RES_CONVERT( XmNlabelString, "All Page" ),
			NULL );
	XtAddCallback( CompileAll, XmNactivateCallback,
		(XtCallbackProc) activateCB_CompileAll,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( CompileAll, (char *) UxTopLevelShellContext );


	/* Creation of CompileAllRegola */
	CompileAllRegola = XtVaCreateManagedWidget( "CompileAllRegola",
			xmPushButtonWidgetClass,
			menu1Compile,
			RES_CONVERT( XmNlabelString, "All Regolation Schema" ),
			NULL );
	XtAddCallback( CompileAllRegola, XmNactivateCallback,
		(XtCallbackProc) activateCB_CompileAllRegola,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( CompileAllRegola, (char *) UxTopLevelShellContext );


	/* Creation of menu1Compile_b4 */
	menu1Compile_b4 = XtVaCreateManagedWidget( "menu1Compile_b4",
			xmSeparatorWidgetClass,
			menu1Compile,
			NULL );
	UxPutContext( menu1Compile_b4, (char *) UxTopLevelShellContext );


	/* Creation of CompileTask */
	CompileTask = XtVaCreateManagedWidget( "CompileTask",
			xmPushButtonWidgetClass,
			menu1Compile,
			RES_CONVERT( XmNlabelString, "Regolation Task" ),
			NULL );
	XtAddCallback( CompileTask, XmNactivateCallback,
		(XtCallbackProc) activateCB_CompileTask,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( CompileTask, (char *) UxTopLevelShellContext );


	/* Creation of ViewErrTask */
	ViewErrTask = XtVaCreateManagedWidget( "ViewErrTask",
			xmPushButtonWidgetClass,
			menu1Compile,
			RES_CONVERT( XmNlabelString, "View Error Task" ),
			NULL );
	XtAddCallback( ViewErrTask, XmNactivateCallback,
		(XtCallbackProc) activateCB_ViewErrTask,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ViewErrTask, (char *) UxTopLevelShellContext );


	/* Creation of menu2_top_b3 */
	menu2_top_b3 = XtVaCreateManagedWidget( "menu2_top_b3",
			xmCascadeButtonWidgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Compile" ),
			XmNsubMenuId, menu1Compile,
			XmNsensitive, FALSE,
			NULL );
	UxPutContext( menu2_top_b3, (char *) UxTopLevelShellContext );


	/* Creation of InterfaceMode */
	InterfaceMode_shell = XtVaCreatePopupShell ("InterfaceMode_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	InterfaceMode = XtVaCreateWidget( "InterfaceMode",
			xmRowColumnWidgetClass,
			InterfaceMode_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( InterfaceMode, (char *) UxTopLevelShellContext );


	/* Creation of Mode */
	Mode = XtVaCreateManagedWidget( "Mode",
			xmToggleButtonWidgetClass,
			InterfaceMode,
			RES_CONVERT( XmNlabelString, "Interface Mode" ),
			NULL );
	XtAddCallback( Mode, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_Mode,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( Mode, (char *) UxTopLevelShellContext );


	/* Creation of menu2_top_b1 */
	menu2_top_b1 = XtVaCreateManagedWidget( "menu2_top_b1",
			xmCascadeButtonGadgetClass,
			menu2,
			RES_CONVERT( XmNlabelString, "Interface Mode" ),
			XmNsubMenuId, InterfaceMode,
			NULL );
	UxPutContext( menu2_top_b1, (char *) UxTopLevelShellContext );


	/* Creation of panedWindow1 */
	panedWindow1 = XtVaCreateManagedWidget( "panedWindow1",
			xmPanedWindowWidgetClass,
			mainWindow,
			NULL );
	UxPutContext( panedWindow1, (char *) UxTopLevelShellContext );


	/* Creation of form8 */
	form8 = XtVaCreateManagedWidget( "form8",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 3,
			XmNy, 3,
			XmNwidth, 671,
			XmNheight, 200,
			XmNpaneMaximum, 500,
			XmNpaneMinimum, 200,
			XmNallowResize, TRUE,
			NULL );
	UxPutContext( form8, (char *) UxTopLevelShellContext );


	/* Creation of form5 */
	form5 = XtVaCreateManagedWidget( "form5",
			xmFormWidgetClass,
			form8,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 50,
			XmNwidth, 649,
			XmNheight, 250,
			XmNrightAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( form5, (char *) UxTopLevelShellContext );


	/* Creation of LabelProject */
	LabelProject = XtVaCreateManagedWidget( "LabelProject",
			xmLabelWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 50,
			XmNwidth, 150,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Base directory :" ),
			XmNtopOffset,0 ,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( LabelProject, (char *) UxTopLevelShellContext );


	/* Creation of labelSimulator */
	labelSimulator = XtVaCreateManagedWidget( "labelSimulator",
			xmLabelWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 170,
			XmNwidth, 110,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Simulator :" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNbottomOffset, 0,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 125,
			NULL );
	UxPutContext( labelSimulator, (char *) UxTopLevelShellContext );


	/* Creation of labelAnimateIconLibraries */
	labelAnimateIconLibraries = XtVaCreateManagedWidget( "labelAnimateIconLibraries",
			xmLabelWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 140,
			XmNwidth, 170,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Animated Icon Libraries :" ),
			XmNbottomAttachment, XmATTACH_NONE,
			XmNbottomOffset, 8,
			XmNbottomWidget, NULL,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 100,
			NULL );
	UxPutContext( labelAnimateIconLibraries, (char *) UxTopLevelShellContext );


	/* Creation of labelObjectLibraries */
	labelObjectLibraries = XtVaCreateManagedWidget( "labelObjectLibraries",
			xmLabelWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 110,
			XmNwidth, 140,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Object Libraries :" ),
			XmNtopOffset, 75,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( labelObjectLibraries, (char *) UxTopLevelShellContext );


	/* Creation of LabelDescription */
	LabelDescription = XtVaCreateManagedWidget( "LabelDescription",
			xmLabelWidgetClass,
			form5,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Description :" ),
			XmNtopOffset, 25,
			XmNleftOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNx, 20,
			XmNy, 50,
			XmNwidth, 100,
			NULL );
	UxPutContext( LabelDescription, (char *) UxTopLevelShellContext );


	/* Creation of textProject */
	textProject = XtVaCreateManagedWidget( "textProject",
			xmLabelWidgetClass,
			form5,
			XmNx, 180,
			XmNy, 50,
			XmNwidth, 300,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( textProject, (char *) UxTopLevelShellContext );


	/* Creation of textObjectLibraries */
	textObjectLibraries = XtVaCreateManagedWidget( "textObjectLibraries",
			xmLabelWidgetClass,
			form5,
			XmNx, 180,
			XmNy, 140,
			XmNwidth, 300,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopOffset, 75,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( textObjectLibraries, (char *) UxTopLevelShellContext );


	/* Creation of textAnimatedIconLibraries */
	textAnimatedIconLibraries = XtVaCreateManagedWidget( "textAnimatedIconLibraries",
			xmLabelWidgetClass,
			form5,
			XmNx, 220,
			XmNy, 140,
			XmNwidth, 419,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 100,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( textAnimatedIconLibraries, (char *) UxTopLevelShellContext );


	/* Creation of textSimulator */
	textSimulator = XtVaCreateManagedWidget( "textSimulator",
			xmLabelWidgetClass,
			form5,
			XmNx, 180,
			XmNy, 200,
			XmNwidth, 300,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopOffset, 125,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( textSimulator, (char *) UxTopLevelShellContext );


	/* Creation of labelPages */
	labelPages = XtVaCreateManagedWidget( "labelPages",
			xmLabelWidgetClass,
			form5,
			XmNx, 20,
			XmNy, 80,
			XmNwidth, 80,
			XmNheight, 15,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Pages :" ),
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 50,
			NULL );
	UxPutContext( labelPages, (char *) UxTopLevelShellContext );


	/* Creation of textPages */
	textPages = XtVaCreateManagedWidget( "textPages",
			xmLabelWidgetClass,
			form5,
			XmNx, 180,
			XmNy, 110,
			XmNwidth, 300,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 50,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( textPages, (char *) UxTopLevelShellContext );


	/* Creation of textDescription1 */
	textDescription1 = XtVaCreateManagedWidget( "textDescription1",
			xmLabelWidgetClass,
			form5,
			XmNx, 200,
			XmNy, 50,
			XmNwidth, 300,
			XmNheight, 15,
			RES_CONVERT( XmNlabelString, "" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 200,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 25,
			NULL );
	UxPutContext( textDescription1, (char *) UxTopLevelShellContext );


	/* Creation of ContextLbl */
	ContextLbl = XtVaCreateManagedWidget( "ContextLbl",
			xmLabelWidgetClass,
			form8,
			XmNx, 160,
			XmNy, 10,
			XmNwidth, 350,
			XmNheight, 20,
			XmNfontList, UxConvertFontList( "-adobe-courier-medium-o-normal--24-240-75-75-m-150-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "NO CONTEXT LOADED" ),
			XmNalignment, XmALIGNMENT_CENTER,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			NULL );
	UxPutContext( ContextLbl, (char *) UxTopLevelShellContext );


	/* Creation of form4 */
	form4 = XtVaCreateManagedWidget( "form4",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 0,
			XmNy, 55,
			XmNwidth, 796,
			XmNheight, 400,
			XmNpaneMinimum, 100,
			NULL );
	UxPutContext( form4, (char *) UxTopLevelShellContext );


	/* Creation of scrolledWindow4 */
	scrolledWindow4 = XtVaCreateManagedWidget( "scrolledWindow4",
			xmScrolledWindowWidgetClass,
			form4,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 5,
			XmNy, 22,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 50,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 25,
			NULL );
	UxPutContext( scrolledWindow4, (char *) UxTopLevelShellContext );

        findBlock = XtVaCreateManagedWidget( "findBlock",
                        xmRowColumnWidgetClass, form4,
                        XmNresizable, TRUE,
                        XmNpacking, XmPACK_TIGHT,
                        XmNnumColumns, 2,
                        XmNleftOffset, 5,
                        XmNrightOffset, 30,
                        XmNrightAttachment, XmATTACH_FORM,
                        XmNorientation, XmHORIZONTAL,
                        XmNleftAttachment, XmATTACH_FORM,
                        XmNbottomOffset, 5,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNheight, 20,
                        XmNwidth, 300,
                        XmNy, 270,
                        XmNx, 30,
                        NULL );
	UxPutContext( findBlock, (char *) UxTopLevelShellContext );


	/* Creation of ListPagine */
	ListPagine = XtVaCreateManagedWidget( "ListPagine",
			xmListWidgetClass,
			scrolledWindow4,
			XmNwidth, 659,
			XmNheight, 191,
			XmNselectionPolicy, XmEXTENDED_SELECT,
			XmNdoubleClickInterval, 300,
			NULL );
	XtAddCallback( ListPagine, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_ListPagine,
		(XtPointer) UxTopLevelShellContext );
	XtAddCallback( ListPagine, XmNdefaultActionCallback,
		(XtCallbackProc) defaultActionCB_ListPagine,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ListPagine, (char *) UxTopLevelShellContext );


	/* Creation of menu1 */
	menu1_shell = XtVaCreatePopupShell ("menu1_shell",
			xmMenuShellWidgetClass, ListPagine,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1 = XtVaCreateWidget( "menu1",
			xmRowColumnWidgetClass,
			menu1_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "<KeyUp>F4",
			NULL );
	UxPutContext( menu1, (char *) UxTopLevelShellContext );


	/* Creation of menu1_p1_title */
	menu1_p1_title = XtVaCreateManagedWidget( "menu1_p1_title",
			xmLabelWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );
	UxPutContext( menu1_p1_title, (char *) UxTopLevelShellContext );


	/* Creation of menu1_p1_b5 */
	menu1_p1_b5 = XtVaCreateManagedWidget( "menu1_p1_b5",
			xmSeparatorGadgetClass,
			menu1,
			NULL );
	UxPutContext( menu1_p1_b5, (char *) UxTopLevelShellContext );


	/* Creation of PBOpen */
	PBOpen = XtVaCreateManagedWidget( "PBOpen",
			xmPushButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "Open" ),
			NULL );
	XtAddCallback( PBOpen, XmNactivateCallback,
		(XtCallbackProc) activateCB_PBOpen,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( PBOpen, (char *) UxTopLevelShellContext );


	/* Creation of PBModify */
	PBModify = XtVaCreateManagedWidget( "PBModify",
			xmPushButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "Modify" ),
			NULL );
	XtAddCallback( PBModify, XmNactivateCallback,
		(XtCallbackProc) activateCB_PBModify,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( PBModify, (char *) UxTopLevelShellContext );


	/* Creation of PBDuplicate */
	PBDuplicate = XtVaCreateManagedWidget( "PBDuplicate",
			xmPushButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "Duplicate" ),
			NULL );
	XtAddCallback( PBDuplicate, XmNactivateCallback,
		(XtCallbackProc) activateCB_PBDuplicate,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( PBDuplicate, (char *) UxTopLevelShellContext );


	/* Creation of PBDelete */
	PBDelete = XtVaCreateManagedWidget( "PBDelete",
			xmPushButtonWidgetClass,
			menu1,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( PBDelete, XmNactivateCallback,
		(XtCallbackProc) activateCB_PBDelete,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( PBDelete, (char *) UxTopLevelShellContext );


	/* Creation of label4 */
	label4 = XtVaCreateManagedWidget( "label4",
			xmLabelWidgetClass,
			form4,
			XmNx, 4,
			XmNy, 2,
			XmNwidth, 154,
			XmNheight, 20,
			RES_CONVERT( XmNlabelString, "Pages List" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label4, (char *) UxTopLevelShellContext );


	/* Creation of form9 */
	form9 = XtVaCreateManagedWidget( "form9",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 3,
			XmNy, 460,
			XmNwidth, 662,
			XmNheight, 200,
			XmNpaneMinimum, 100,
			NULL );
	UxPutContext( form9, (char *) UxTopLevelShellContext );


	/* Creation of label5 */
	label5 = XtVaCreateManagedWidget( "label5",
			xmLabelWidgetClass,
			form9,
			XmNx, 6,
			XmNy, 4,
			XmNwidth, 175,
			XmNheight, 18,
			RES_CONVERT( XmNlabelString, "Libraries List" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label5, (char *) UxTopLevelShellContext );


	/* Creation of scrolledWindow5 */
	scrolledWindow5 = XtVaCreateManagedWidget( "scrolledWindow5",
			xmScrolledWindowWidgetClass,
			form9,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 0,
			XmNy, 0,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 5,
			XmNleftOffset, 5,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 5,
			XmNtopOffset, 25,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( scrolledWindow5, (char *) UxTopLevelShellContext );


	/* Creation of LibraryList */
	LibraryList = XtVaCreateManagedWidget( "LibraryList",
			xmListWidgetClass,
			scrolledWindow5,
			XmNwidth, 654,
			XmNheight, 66,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNvisibleItemCount, 11,
			NULL );
	XtAddCallback( LibraryList, XmNsingleSelectionCallback,
		(XtCallbackProc) singleSelectionCB_LibraryList,
		(XtPointer) UxTopLevelShellContext );
	XtAddCallback( LibraryList, XmNdefaultActionCallback,
		(XtCallbackProc) defaultActionCB_LibraryList,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( LibraryList, (char *) UxTopLevelShellContext );


	/* Creation of menu6 */
	menu6_shell = XtVaCreatePopupShell ("menu6_shell",
			xmMenuShellWidgetClass, LibraryList,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu6 = XtVaCreateWidget( "menu6",
			xmRowColumnWidgetClass,
			menu6_shell,
			XmNrowColumnType, XmMENU_POPUP,
			XmNmenuAccelerator, "<KeyUp>F4",
			NULL );
	UxPutContext( menu6, (char *) UxTopLevelShellContext );


	/* Creation of menu6_p1_title */
	menu6_p1_title = XtVaCreateManagedWidget( "menu6_p1_title",
			xmLabelWidgetClass,
			menu6,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );
	UxPutContext( menu6_p1_title, (char *) UxTopLevelShellContext );


	/* Creation of separ1 */
	separ1 = XtVaCreateManagedWidget( "separ1",
			xmSeparatorGadgetClass,
			menu6,
			NULL );
	UxPutContext( separ1, (char *) UxTopLevelShellContext );


	/* Creation of OpenLibrary */
	OpenLibrary = XtVaCreateManagedWidget( "OpenLibrary",
			xmPushButtonWidgetClass,
			menu6,
			RES_CONVERT( XmNlabelString, "Open" ),
			NULL );
	XtAddCallback( OpenLibrary, XmNactivateCallback,
		(XtCallbackProc) activateCB_OpenLibrary,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( OpenLibrary, (char *) UxTopLevelShellContext );


	/* Creation of ModifyLibrary */
	ModifyLibrary = XtVaCreateManagedWidget( "ModifyLibrary",
			xmPushButtonWidgetClass,
			menu6,
			RES_CONVERT( XmNlabelString, "Modify" ),
			NULL );
	XtAddCallback( ModifyLibrary, XmNactivateCallback,
		(XtCallbackProc) activateCB_ModifyLibrary,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( ModifyLibrary, (char *) UxTopLevelShellContext );


	/* Creation of DeleteLibrary */
	DeleteLibrary = XtVaCreateManagedWidget( "DeleteLibrary",
			xmPushButtonWidgetClass,
			menu6,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );
	XtAddCallback( DeleteLibrary, XmNactivateCallback,
		(XtCallbackProc) activateCB_DeleteLibrary,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( DeleteLibrary, (char *) UxTopLevelShellContext );


	/* Creation of form3 */
	form3 = XtVaCreateManagedWidget( "form3",
			xmFormWidgetClass,
			panedWindow1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 3,
			XmNy, 577,
			XmNwidth, 668,
			XmNheight, 178,
			NULL );
	UxPutContext( form3, (char *) UxTopLevelShellContext );


	/* Creation of label8 */
	label8 = XtVaCreateManagedWidget( "label8",
			xmLabelWidgetClass,
			form3,
			XmNx, 15,
			XmNy, 9,
			XmNwidth, 110,
			XmNheight, 25,
			XmNalignment, XmALIGNMENT_BEGINNING,
			RES_CONVERT( XmNlabelString, "Messages" ),
			XmNrightAttachment, XmATTACH_NONE,
			XmNrightOffset, 5,
			XmNtopOffset, 5,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 5,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( label8, (char *) UxTopLevelShellContext );


	/* Creation of scrolledWindow1 */
	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass,
			form3,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNx, 10,
			XmNy, 40,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 40,
			NULL );
	UxPutContext( scrolledWindow1, (char *) UxTopLevelShellContext );


	/* Creation of scrolledText1 */
	scrolledText1 = XtVaCreateManagedWidget( "scrolledText1",
			xmTextWidgetClass,
			scrolledWindow1,
			XmNwidth, 620,
			XmNheight, 81,
			XmNcolumns, 60,
			XmNeditMode, XmMULTI_LINE_EDIT ,
			XmNeditable, FALSE,
			XmNresizeHeight, TRUE,
			XmNresizeWidth, TRUE,
			NULL );
	UxPutContext( scrolledText1, (char *) UxTopLevelShellContext );


	/* Creation of menu5 */
	menu5_shell = XtVaCreatePopupShell ("menu5_shell",
			xmMenuShellWidgetClass, scrolledText1,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu5 = XtVaCreateWidget( "menu5",
			xmRowColumnWidgetClass,
			menu5_shell,
			XmNrowColumnType, XmMENU_POPUP,
			NULL );
	UxPutContext( menu5, (char *) UxTopLevelShellContext );


	/* Creation of menu5_p1_b1 */
	menu5_p1_b1 = XtVaCreateManagedWidget( "menu5_p1_b1",
			xmPushButtonWidgetClass,
			menu5,
			RES_CONVERT( XmNlabelString, "Clear Message" ),
			NULL );
	XtAddCallback( menu5_p1_b1, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu5_p1_b1,
		(XtPointer) UxTopLevelShellContext );

	UxPutContext( menu5_p1_b1, (char *) UxTopLevelShellContext );


	XtAddCallback( topLevelShell, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxTopLevelShellContext);

	XtAddEventHandler(ListPagine, ButtonPressMask,
			False, (XtEventHandler) _UxtopLevelShellMenuPost, (XtPointer) menu1 );
	XtAddEventHandler(LibraryList, ButtonPressMask,
			False, (XtEventHandler) _UxtopLevelShellMenuPost, (XtPointer) menu6 );
	XtAddEventHandler(scrolledText1, ButtonPressMask,
			False, (XtEventHandler) _UxtopLevelShellMenuPost, (XtPointer) menu5 );
	XmMainWindowSetAreas( mainWindow, menu2, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, panedWindow1 );

	return ( topLevelShell );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_topLevelShell()
{
	Widget                  rtrn;
	_UxCtopLevelShell       *UxContext;

	UxTopLevelShellContext = UxContext =
		(_UxCtopLevelShell *) UxNewContext( sizeof(_UxCtopLevelShell), False );


	{
                Widget block_find_form;

		int appo,shr_usr_key=10000;
		
		char *getenv();
		char varrtf[MAXLENFILENAME];
		char varcc[MAXLENFILENAME];
		char varsub[MAXLENFILENAME];
		char varcom[MAXLENFILENAME];
		char mesg[200];
		
		npagopen = 0;
		fpLogMsg = fopen("config.log","w");
		rtrn = _Uxbuild_topLevelShell();


		/* creo il find_kit */
               /* Inserimento find di libreria */
                   parametri_ulist.ID_lista = UxGetWidget(ListPagine);
                   parametri_ulist.num_elem = 1000;
                   parametri_ulist.vis_elem = 1;
                   parametri_ulist.pos      = 0;

                   nargb=0;
                   XtSetArg(argb[nargb],XmNleftAttachment,XmATTACH_FORM);
                   nargb++;
                   XtSetArg(argb[nargb],XmNleftOffset,0);
                   nargb++;
                   XtSetArg(argb[nargb],XmNrightAttachment,XmATTACH_FORM);
                   nargb++;
                   XtSetArg(argb[nargb],XmNrightOffset,0);
                   nargb++;
                   XtSetArg(argb[nargb],XmNbottomAttachment,XmATTACH_FORM);
                   nargb++;
                   XtSetArg(argb[nargb],XmNbottomOffset,0);
                   nargb++;
                   XtSetArg(argb[nargb],XmNtopAttachment,XmATTACH_FORM);
                   nargb++;
                   XtSetArg(argb[nargb],XmNtopOffset,0);
                   nargb++;

                   block_find_form = find_kit(UxGetWidget(findBlock),argb,nargb,
                                                                 &parametri_ulist);



		/***** fine ***/
		

		if(ContextLoad("Context.ctx",&pagedit))
		{
		   display_resources(pagedit);
		   TopMenuSetSensitive();
		}
		else
		{
		      sprintf(mesg,"Context File Not Found %s.\n",pagedit->fileContext);
		      show_message(mesg);
		
		      TopMenuSetInsensitive();
		} 
		
		/*
		 Creazione (senza visualizzazione) della
		 clipboard per copy and paste
		*/
		ClipBoardForCopy=create_ClipBoard();
		
		if(getenv(ENVRTF) != NULL)
		{
		   strcpy(varrtf,XlGetenv(ENVRTF));
		   strcat(varrtf,"/variabili.rtf");
		   strcpy(varrtf,XlConvPathVms(varrtf));
		}
		else
		   strcpy(varrtf,"variabili.rtf");
		
		if( getenv(ENVCTX) != NULL)
		{
		   strcpy(varcc,XlGetenv(ENVCTX));
		   strcat(varcc,"/FileDefVarCC.edf");
		   strcpy(varcc,XlConvPathVms(varcc)); 
		}
		else
		   strcpy(varcc,"FileDefVarCC.edf");
		
		if( getenv(ENVRTF) != NULL)
		{
		   strcpy(varsub,XlGetenv(ENVRTF));
		   strcat(varsub,"/FileSubSystem.edf");
		   strcpy(varsub,XlConvPathVms(varsub));
		}
		else
		   strcpy(varsub,"FileSubSystem.edf");
		
		
		if( getenv(ENVCTX) != NULL)
		{
		   strcpy(varcom,XlGetenv(ENVCTX));
		   strcat(varcom,"/FileComponent.edf");
		   strcpy(varcom,XlConvPathVms(varcom));
		}
		else
		   strcpy(varcom,"FileComponent.edf");
		
		if(getenv("SHR_USR_KEY") != NULL)
		   shr_usr_key = atoi(getenv("SHR_USR_KEY"));
		
		
		if( (dbtopologia = OlCreateDatabaseTopologia("olrtf",varrtf,shr_usr_key,
				                         varsub,varcom,varcc)) == NULL)
		{
		   sprintf(mesg,"Topology Database not Loaded\n");
		   show_message(mesg);
		
		   /* disabilita menu' compile */
		   CompilerOk = False;
		
		}
		else
		{
		   compilatore_severe = OlCreateCompiler("olsevere",dbtopologia);
		
		   if( compilatore_severe == NULL )
		   {
		      sprintf(mesg,"Compiler not Loaded\n");
		      show_message(mesg);      
		      /* disabilita menu compile */
		      CompilerOk = False;
		   } 
		}
		
		/*
		   Nel caso in cui la licenza disponibile e' di tipo 'demo', vengono 
		   disabilitate alcune funzionalita': compilazione e salvataggio delle pagine.
		*/
		   if(flag_demo == 1)
		      XtVaSetValues(menu2_top_b3, XmNsensitive, False, NULL);
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

