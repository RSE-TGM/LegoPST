
/*******************************************************************************
       PagDialog.c
       (Generated from interface file PagDialog.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo PagDialog.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)PagDialog.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
#include "config.h"
#ifndef DESIGN_TIME
#include "message.h"
#endif
#include "res_edit.h"

#include "libutilx.h"


#define MAXREGNAME 4
extern swidget topLevelShell;
extern Arg args[];
extern Cardinal lnargs;

/* */

Boolean verifica_nome( );
void riempi_campi( );
void SetOptMenu( );

extern char *tmpnam();
extern void PagSetDefaultValue();
extern Boolean pagina_load_file( );
extern Boolean pagina_init( );
extern Boolean pagina_getres( );
extern int PagGetType( );
extern int show_message();


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
	Widget	UxPagDialog;
	Widget	Uxform2;
	Widget	UxlabelName;
	Widget	UxlabelX;
	Widget	UxlabelY;
	Widget	UxlabelDesc;
	Widget	UxPagDescription;
	Widget	UxPagX;
	Widget	UxPagY;
	Widget	UxPagBackground;
	Widget	UxlabelTitle1;
	Widget	UxOkButton;
	Widget	UxCancelButton;
	Widget	UxlabelTitle2;
	Widget	UxlabelTitle3;
	Widget	UxlabelHeight1;
	Widget	UxlabelWidth1;
	Widget	UxPagHeight;
	Widget	UxPagWidth;
	Widget	UxlabelHeight2;
	Widget	UxDrawHeight;
	Widget	UxlabelWidth2;
	Widget	UxDrawWidth;
	Widget	UxpushButton1;
	Widget	UxlabelTipo;
	Widget	UxPagName;
	Widget	Uxmenu4_p1;
	Widget	Uxmenu4_p1_b2;
	Widget	Uxmenu4_p1_b1;
	Widget	Uxmenu4_p1_b3;
	Widget	Uxmenu4;
	Widget	UxlabelRefreshFrequenzy;
	Widget	UxRefreshFrequenzy;
	Widget	Uxlabel_in_use;
	Widget	Uxmenu_in_use;
	Widget	UxIn_use;
	Widget	UxNot_in_use;
	Widget	Uxmenu7;
	Widget	UxHierarchyLabel;
	Widget	UxHierarchy;
	int	Uxoperazione;
	PAGINA	*Uxpagina;
	AN_OBJ	*Uxlistaobj;
	unsigned char	Uxoldname[100];
	Widget	UxListaWid[2];
	int	UxtipoPag;
	unsigned char	*Uxpname;
	int	Uxoper;
} _UxCPagDialog;

static _UxCPagDialog           *UxPagDialogContext;
#define PagDialog               UxPagDialogContext->UxPagDialog
#define form2                   UxPagDialogContext->Uxform2
#define labelName               UxPagDialogContext->UxlabelName
#define labelX                  UxPagDialogContext->UxlabelX
#define labelY                  UxPagDialogContext->UxlabelY
#define labelDesc               UxPagDialogContext->UxlabelDesc
#define PagDescription          UxPagDialogContext->UxPagDescription
#define PagX                    UxPagDialogContext->UxPagX
#define PagY                    UxPagDialogContext->UxPagY
#define PagBackground           UxPagDialogContext->UxPagBackground
#define labelTitle1             UxPagDialogContext->UxlabelTitle1
#define OkButton                UxPagDialogContext->UxOkButton
#define CancelButton            UxPagDialogContext->UxCancelButton
#define labelTitle2             UxPagDialogContext->UxlabelTitle2
#define labelTitle3             UxPagDialogContext->UxlabelTitle3
#define labelHeight1            UxPagDialogContext->UxlabelHeight1
#define labelWidth1             UxPagDialogContext->UxlabelWidth1
#define PagHeight               UxPagDialogContext->UxPagHeight
#define PagWidth                UxPagDialogContext->UxPagWidth
#define labelHeight2            UxPagDialogContext->UxlabelHeight2
#define DrawHeight              UxPagDialogContext->UxDrawHeight
#define labelWidth2             UxPagDialogContext->UxlabelWidth2
#define DrawWidth               UxPagDialogContext->UxDrawWidth
#define pushButton1             UxPagDialogContext->UxpushButton1
#define labelTipo               UxPagDialogContext->UxlabelTipo
#define PagName                 UxPagDialogContext->UxPagName
#define menu4_p1                UxPagDialogContext->Uxmenu4_p1
#define menu4_p1_b2             UxPagDialogContext->Uxmenu4_p1_b2
#define menu4_p1_b1             UxPagDialogContext->Uxmenu4_p1_b1
#define menu4_p1_b3             UxPagDialogContext->Uxmenu4_p1_b3
#define menu4                   UxPagDialogContext->Uxmenu4
#define labelRefreshFrequenzy   UxPagDialogContext->UxlabelRefreshFrequenzy
#define RefreshFrequenzy        UxPagDialogContext->UxRefreshFrequenzy
#define label_in_use            UxPagDialogContext->Uxlabel_in_use
#define menu_in_use             UxPagDialogContext->Uxmenu_in_use
#define In_use                  UxPagDialogContext->UxIn_use
#define Not_in_use              UxPagDialogContext->UxNot_in_use
#define menu7                   UxPagDialogContext->Uxmenu7
#define HierarchyLabel          UxPagDialogContext->UxHierarchyLabel
#define Hierarchy               UxPagDialogContext->UxHierarchy
#define operazione              UxPagDialogContext->Uxoperazione
#define pagina                  UxPagDialogContext->Uxpagina
#define listaobj                UxPagDialogContext->Uxlistaobj
#define oldname                 UxPagDialogContext->Uxoldname
#define ListaWid                UxPagDialogContext->UxListaWid
#define tipoPag                 UxPagDialogContext->UxtipoPag
#define pname                   UxPagDialogContext->Uxpname
#define oper                    UxPagDialogContext->Uxoper



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_PagDialog();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* verifica che nel nome della pagina non ci siano 
   caratteri non validi per il filename */
 
Boolean verifica_nome(char *nome)
{
   if( (strchr(nome,' ') != NULL) || (strchr(nome,'.') != NULL) )
      return(False);
   else
      return(True);   
}

 void riempi_campi( PAGINA *pag )
{   
   char appo[500], msg[100];

   strcpy(appo,pag->nomepag); 
printf("appo=%s\n",appo);
   XmTextFieldSetString(PagName, appo);
printf("appo=%s\n",appo);

   strcpy(appo,pag->geom.descriz); 
   XmTextFieldSetString(PagDescription, appo);

   sprintf(appo,"%d",pag->geom.x); 
   XmTextFieldSetString(PagX,appo);

   sprintf(appo,"%d",pag->geom.y); 
   XmTextFieldSetString(PagY,appo);

   sprintf(appo,"%d",pag->geom.width); 
   XmTextFieldSetString(PagWidth,appo);

   sprintf(appo,"%d",pag->geom.height); 
   XmTextFieldSetString(PagHeight,appo);

   sprintf(appo,"%d",pag->geom.draw_width); 
   XmTextFieldSetString(DrawWidth,appo);

   sprintf(appo,"%d",pag->geom.draw_height); 
   XmTextFieldSetString(DrawHeight,appo);

   if( (operazione != CREATE_PAGE) && (operazione != CREATE_REGOLATION))
      sprintf(appo,"%d",pag->refreshFreq); 
   else
      sprintf(appo,"%d",10); 

   XmTextFieldSetString(RefreshFrequenzy,appo);

   printf("riempi_campi: pag->refreshFreq=%d\n",pag->refreshFreq);
   printf("riempi_campi: pag->gerarchia=%s\n",pag->gerarchia);
// GUAG2025 gerarchia bug
//    if(operazione == MODIFY_PAGE)
//    	hierarchy_format_new(appo,pag->gerarchia);
//    else
// 	strcpy(appo,"[-1][-1][-1][-1][-1][-1]");
  if(operazione == MODIFY_PAGE)
  {
     if(strlen(pag->gerarchia)>0)
     {
        // Controlla se è già in formato UI (con parentesi quadre)
        if(strchr(pag->gerarchia, '[') != NULL)
        {
           // È già in formato UI, copialo direttamente
           strcpy(appo, pag->gerarchia);
        }
        else
        {
           // È in formato interno (con virgole), convertilo
           hierarchy_format_new(appo, pag->gerarchia);
        }
     }
     else
        strcpy(appo,"[-1][-1][-1][-1][-1][-1]");
  }

   if(!strcmp(GERARCHIA_KO,appo))
   {
      sprintf(msg,"Hierarchy resource not correct! Page:%s\tHierarchy:%s\n",pag->nomepag,pag->gerarchia);
      /* strcpy(msg,"Hierarchy specification not correct!\n"); */
      show_message(msg);
// se GUAG2025 GERARCHIA_KO allora lasciola gerarchia vecchia
//      XmTextFieldSetString(Hierarchy,"");
      XmTextFieldSetString(Hierarchy,pag->gerarchia);
   }
   else {
      XmTextFieldSetString(Hierarchy,appo);
   }
   printf("riempi_campi: DOPO: pag->gerarchia=%s\n",pag->gerarchia);

   XmTextFieldSetString(PagBackground,pag->geom.background);

}

void SetOptMenu(PAGINA *pag)
{
   Widget Pane,Bottone;   
   WidgetList ListaButton;

   /*  menu' tipo pagina */
   get_something(menu4,XmNsubMenuId, (void*) &Pane);
   get_something(Pane,XmNchildren, (void*) &ListaButton);
   
   if(!strcmp(pag->geom.tipo,TIPO_SINOTTICO))
       Bottone = ListaButton[0];
   else if(!strcmp(pag->geom.tipo,TIPO_STAZIONI))
       Bottone = ListaButton[1];
   else if(!strcmp(pag->geom.tipo,TIPO_TELEPERM))
       Bottone = ListaButton[2];
   else  /* mi metto al riparo da eventuali sporcate */
       Bottone = ListaButton[0];

   set_something_val(menu4,XmNmenuHistory,(XtArgVal)Bottone);

  /*  menu' in use */
   get_something_val(menu7,XmNsubMenuId,(XtPointer)&Pane);
   get_something_val(Pane,XmNchildren,(XtPointer)&ListaButton);
   
   if(pag->in_use==SCHEME_IN_USE)
       Bottone = ListaButton[0];
   else if(pag->in_use==SCHEME_NOT_IN_USE)
       Bottone = ListaButton[1];
   else  /* mi metto al riparo da eventuali sporcate */
       Bottone = ListaButton[0];

   set_something_val(menu7,XmNmenuHistory,(XtArgVal)Bottone); 

}


/* setto le dimensioni di default della pagina
   di tipo Telepem
   Le dimensioni sono impostate considerando uno screen 1280x1024
   
*/
void TelepermSetDimension()
{
   char appo[500];
/*
   int screenW=WidthOfScreen( XtScreen(w) );
   int screenH=HeightOfScreen( XtScreen(w) );
*/
   int screenW = 1280;
   int screenH = 1024;

   XmTextFieldSetString(PagX,"0");

   XmTextFieldSetString(PagY,"0");

   sprintf(appo,"%d", screenW ); 
   XmTextFieldSetString(PagWidth,appo);

   sprintf(appo,"%d", screenH ); 
   XmTextFieldSetString(PagHeight,appo);

   sprintf(appo,"%d", screenW ); 
   XmTextFieldSetString(DrawWidth,appo);

   sprintf(appo,"%d", screenH - 96 ); 
   XmTextFieldSetString(DrawHeight,appo);

}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_OkButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagDialog           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagDialogContext;
	UxPagDialogContext = UxContext =
			(_UxCPagDialog *) UxGetContext( UxWidget );
	{
#ifndef DESIGN_TIME
	// extern void  SetItemString();
	int SetItemString(XmString *,char *,char *,char *,int ,char *);
	extern int TagPagSetNew();
	extern void PaginaSetTagPag();
	extern Arg args[];
	extern Cardinal lnargs;
	extern void add_item();
	extern char *extract_string();
	extern PAGEDIT_CONTEXT *pagedit;
	extern Boolean ContextModified;
	extern char *XlConvPathVms();
	extern Boolean XlCopy();
	
	char comando[200],message[500];
	XmString Cpagname,new_item;
	FILE *fp;
	char strRes[100],strVal[100];
	char nomepag[MAXCHAR_PAGNAME];  /* nome della pagina */
	char filepag[MAXLENFILENAME],file1[MAXLENFILENAME],filebkg[MAXLENFILENAME];  
	int i,tipo_pag;
	int WLimit,HLimit,LowLimit;
	Widget WgtOptMenu;
	char stringItem[200];
	PAGINA *pagappo;
	AN_OBJ *obj;
	int tipo_sorg;
	
	/* definisco i limiti sulle dimensioni della pagina in base allo screen */ 
	WLimit = XDisplayWidth(UxDisplay,UxScreen)  - 10;
	HLimit = XDisplayHeight(UxDisplay,UxScreen) - 10;  
	LowLimit = MIN_WIDTH;
	
	/* recupero i dati presenti sulla maschera */
	
	strcpy(nomepag,XmTextFieldGetString(PagName));
	
	/* verifica il nome attribuito alla pagina */
	if( verifica_nome(nomepag) == False)
	{
	   SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Invalid Page Name",
	          NULL,False,NULL,False,NULL);
	   return;
	}
	
printf("activateCB_OkButton:nomepag=%s\n",nomepag);	
	switch(operazione)
	{
	   case CREATE_PAGE:
	      if( XlGetenv(ENVPAG) == NULL)
	         return;
	      strcpy(filepag,getenv(ENVPAG));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,PAG_EXT);
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag));
	
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      pagina->refreshFreq = atoi(XmTextFieldGetString(RefreshFrequenzy));
	/*
	by fabio
	*/
	      format_hierarchy_new(pagina->gerarchia,XmTextFieldGetString(Hierarchy));
	      if( strcmp(GERARCHIA_KO,pagina->gerarchia)==0 )
	            {
	             SetMsg(topLevelShell,NULL,INFMSG,
	                   "ConfigInfo","Hierarchy specification not correct!",
	                    NULL,False,NULL,False,NULL);
	             return;
	              }
	
	
	printf("Pagina gerarc=%s\n",pagina->gerarchia);
	      get_something(menu4,XmNmenuHistory, (void*) &WgtOptMenu);
	      if(WgtOptMenu == menu4_p1_b1)
	         strcpy(pagina->geom.tipo,TIPO_STAZIONI);   
	      else if(WgtOptMenu == menu4_p1_b2)
	         strcpy(pagina->geom.tipo,TIPO_SINOTTICO);
	      else if(WgtOptMenu == menu4_p1_b3)
	         strcpy(pagina->geom.tipo,TIPO_TELEPERM);
	
		
	      pagina->in_use=SCHEME_NOT_IN_USE;   
	      pagina->snap_pag=DEF_SNAP;
	      
	      if( pagina->refreshFreq < 1)
	      {
	          SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Refresh Frequenzy Values Not Acceptable",NULL,False,NULL,False,NULL);
	          return;
	      }
	
	      if( (fp = fopen(filepag,"r")) != NULL)
	      {
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Proposed Page Already Exist",NULL,False,NULL,False,NULL);
	         fclose(fp);
	         return;
	      }
	      else
	      {
	         XlCopy(FILEPAGINADEFAULT,filepag); 
	         fp = fopen(filepag,"a");
	         strcpy(pagina->nomepag,nomepag);
	         strcpy(pagina->filepag,filepag);
	         pag_wgeometry(fp,pagina);
	      }
	/* aggiorno la struttura del CONTEXT: 'page_num', 'page_list' e 'res_page' */ 
	      if(pagedit->pag_num > 0)
	      {
		 printf("OkButton: prima delle realloc per la CREATE_PAGE!!!\n");
	         pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1));
		 pagedit->res_page = realloc(pagedit->res_page, sizeof(INFO_PAGE *)*(pagedit->pag_num+1));
	
	         if(( pagedit->page_list == NULL ) || (pagedit->res_page == NULL))
	            return;
	      }
	      else
	         if(((pagedit->page_list = (char **)alloca_memoria(1,sizeof(char *))) == NULL) ||
			((pagedit->res_page = (INFO_PAGE **)alloca_memoria(1, sizeof(INFO_PAGE *))) == NULL))
	            return;
	      printf("OkButton: prima di allocare la struttura per la nuova pagina!!!\n");
	      if(((pagedit->page_list[pagedit->pag_num] = (char *)alloca_memoria(strlen(nomepag)+1,sizeof(char))) == NULL) ||
		  ((pagedit->res_page[pagedit->pag_num] = (INFO_PAGE *)alloca_memoria(1,sizeof(INFO_PAGE))) == NULL))
	         return;
	      strcpy(pagedit->page_list[pagedit->pag_num],nomepag);
	
	      printf("OkButton: Nomepag:%s Numpag:%d prima dei caricamenti delle nuove info!!!\n", 
			pagedit->page_list[pagedit->pag_num],pagedit->pag_num); 
	      strcpy(pagedit->res_page[pagedit->pag_num]->descr_page, pagina->geom.descriz);
	      strcpy(pagedit->res_page[pagedit->pag_num]->type_page, pagina->geom.tipo);
	      pagedit->res_page[pagedit->pag_num]->refresh_freq = pagina->refreshFreq;
	      strcpy(pagedit->res_page[pagedit->pag_num]->hier, pagina->gerarchia);
	      pagedit->res_page[pagedit->pag_num]->in_use = pagina->in_use;
	      fclose(fp);
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	      strcpy(pagina->filebkg,nomepag);
	      strcpy(pagina->filebkg,BKG_EXT);
	/*      TagPagSetNew(pagedit,TYPE_STAZIONE); */
	/* il Set delle TagPag lo lascio per evitare sporcamenti */ 
	      PaginaSetTagPag(pagedit,pagina);
	
	      strcpy(pagedit->res_page[pagedit->pag_num]->tagPag,pagina->tagPag);
	
	      pagedit->pag_num++;
	 
	/* aggiungo l'item nella lista delle pagine*/
	      SetItemString(&Cpagname,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,pagina->in_use,pagina->tagPag);
	      add_item(Cpagname,0);
	/* MAURIC */
	      pagina_free(pagina);
	      XmStringFree(Cpagname);
	   break; 
	   case CREATE_REGOLATION:
	      if( XlGetenv(ENVPAG) == NULL)
	         return;
	      strcpy(filepag,XlGetenv(ENVPAG));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,PAG_EXT);
	
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag)); 
	
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      pagina->refreshFreq = atoi(XmTextFieldGetString(RefreshFrequenzy));    
	      pagina->numTagReg = 0;
	      strcpy(pagina->nextTagRegol,"00");
	      strcpy(pagina->geom.tipo,TIPO_REGOLAZIONE);
	
	/*
	      CAPPE. Per le pagine di regolazione la risorsa 'gerarchia' viene posta uguale a '-1,-1,-1,-1,-1,-1'
	      (pagine fuori dalla gerarchia)????      
	      strcpy(pagina->gerarchia,"-1,-1,-1,-1,-1,-1");*/ 

		  //GUAG2025  problema gerarchia
	      strcpy(pagina->gerarchia,XmTextFieldGetString(Hierarchy));
	
	
	
	      get_something(menu7,XmNmenuHistory, (void*) &WgtOptMenu);
	      if(WgtOptMenu == In_use)
		  pagina->in_use=SCHEME_IN_USE;
	      else if(WgtOptMenu == Not_in_use)
	             pagina->in_use=SCHEME_NOT_IN_USE; 
	      pagina->snap_pag=DEF_REG_SNAP;
	      if( pagina->refreshFreq < 1)
	      {
	          SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Refresh Frequenzy Values Not Acceptable",NULL,False,NULL,False,NULL);
	          return;
	      }
	
	      if( (fp = fopen(filepag,"r")) != NULL)
	      {
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Proposed Page Already Exist",NULL,False,NULL,False,NULL);
	         fclose(fp);
	         return;
	      }
	      else
	      {
	         XlCopy(FILEPAGINADEFAULT,filepag);
	         fp = fopen(filepag,"a");
	         strcpy(pagina->nomepag,nomepag);
	         strcpy(pagina->filepag,filepag);
	         pag_wgeometry(fp,pagina);
	      }
	/* aggiorno la struttura del CONTEXT */ 
	      if(pagedit->pag_num > 0)
	      {
	         pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1));
		 pagedit->res_page = realloc(pagedit->res_page, sizeof(INFO_PAGE *)*(pagedit->pag_num+1));
	         if(( pagedit->page_list == NULL ) || (pagedit->res_page == NULL))
	            return;
	      }
	      else
	         if(((pagedit->page_list = (char **)alloca_memoria(1,sizeof(char *))) == NULL) ||
		    ((pagedit->res_page = (INFO_PAGE **)alloca_memoria(1, sizeof(INFO_PAGE *))) == NULL))
	            return;
	      if(((pagedit->page_list[pagedit->pag_num] = (char *)alloca_memoria(strlen(nomepag)+1,sizeof(char))) == NULL) ||
	         ((pagedit->res_page[pagedit->pag_num] = (INFO_PAGE *)alloca_memoria(1,sizeof(INFO_PAGE))) == NULL))
	         return;
	      strcpy(pagedit->page_list[pagedit->pag_num],nomepag);
	      TagPagSetNew(pagedit,TYPE_REGOLAZIONE);
	      PaginaSetTagPag(pagedit,pagina);
	
	      strcpy(pagedit->res_page[pagedit->pag_num]->descr_page, pagina->geom.descriz);
	      strcpy(pagedit->res_page[pagedit->pag_num]->type_page, pagina->geom.tipo);
	      pagedit->res_page[pagedit->pag_num]->refresh_freq = pagina->refreshFreq;
	      strcpy(pagedit->res_page[pagedit->pag_num]->hier, pagina->gerarchia);
	      pagedit->res_page[pagedit->pag_num]->in_use = pagina->in_use;
	      strcpy(pagedit->res_page[pagedit->pag_num]->tagPag,pagina->tagPag);
	      pagedit->pag_num++;
	
	      printf("Pagina->tagPag = %s\n",pagina->tagPag);
	      sprintf(strRes,"*%s:   %s\n",XlNtagPag,pagina->tagPag);
	      fprintf(fp,"%s",strRes);
	      fclose(fp);
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	      strcpy(pagina->filebkg,nomepag);
	      strcpy(pagina->filebkg,BKG_EXT);
	/* Aggiungo l'item nella lista delle pagine */
	      SetItemString(&Cpagname,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,pagina->in_use,pagina->tagPag);
	      add_item(Cpagname,0);
	/* MAURIC */
	      pagina_free(pagina);
	      XmStringFree(Cpagname);
	   break;
	   case CREATE_LIBRARY:
	      if( XlGetenv(ENVLIB) == NULL)
	         return;
	      strcpy(filepag,XlGetenv(ENVLIB));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,LIB_EXT);
	
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag));
	    
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      strcpy(pagina->geom.tipo,TIPO_LIBRERIA);
	
	      pagina->snap_pag=DEF_REG_SNAP;
	      pagina->in_use=SCHEME_NOT_IN_USE;
	      if( (fp = fopen(filepag,"r")) != NULL)
	      {
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Proposed Page Already Exist",NULL,False,NULL,False,NULL);
	         fclose(fp);
	         return;
	      }
	      else
	      {
	         XlCopy(FILEPAGINADEFAULT,filepag); 
	         if( (fp = fopen(filepag,"a")) == NULL)
		 {
		     sprintf(message,"Error opening %s",filepag);
		     SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",message,NULL,False,NULL,False,NULL);
		     return;
		 }
	         strcpy(pagina->nomepag,nomepag);
	         strcpy(pagina->filepag,filepag);
	         pag_wgeometry(fp,pagina);
	         fclose(fp);
	/* aggiorno la struttura del CONTEXT */ 
	      if(pagedit->iconlib_num > 0)
	      {
	         pagedit->iconlib_list = realloc(pagedit->iconlib_list,sizeof(char *)*(pagedit->iconlib_num+1));
	         pagedit->iconlib_label = realloc(pagedit->iconlib_label,sizeof(char *)*(pagedit->iconlib_num+1));
	
	         if( pagedit->iconlib_list == NULL )
	            return;
	      }
	      else
	      {  
	         if((pagedit->iconlib_list = (char **)alloca_memoria(1,sizeof(char *))) == NULL)
	            return;
	         if((pagedit->iconlib_label = (char **)alloca_memoria(1,sizeof(char *))) == NULL)
	            return;
	      }
	
	      if( (pagedit->iconlib_list[pagedit->iconlib_num] = (char *)alloca_memoria(strlen(nomepag)+1,sizeof(char))) == NULL)
	         return;
	      if( (pagedit->iconlib_label[pagedit->iconlib_num] = (char *)alloca_memoria(strlen(nomepag)+1,sizeof(char))) == NULL)
	         return;
	      strcpy(pagedit->iconlib_list[pagedit->iconlib_num],nomepag);
	      strcpy(pagedit->iconlib_label[pagedit->iconlib_num],nomepag);
	      pagedit->iconlib_num++;         
	      }
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	/* Aggiungo l'item nella lista delle pagine */
	      SetItemString(&Cpagname,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,pagina->in_use,pagina->tagPag);
	      add_library_item(Cpagname,0);
	/* MAURIC */
	      pagina_free(pagina);
	      XmStringFree(Cpagname);
	   break;
	   case MODIFY_PAGE:
	      if( XlGetenv(ENVPAG) == NULL)
	         return;
	      strcpy(filepag,XlGetenv(ENVPAG));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,PAG_EXT);
	
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag));
	
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      pagina->refreshFreq = atoi(XmTextFieldGetString(RefreshFrequenzy));
	
	/*
	by fabio
	*/
	
	      format_hierarchy_new(pagina->gerarchia,XmTextFieldGetString(Hierarchy));
	      if( strcmp(GERARCHIA_KO,pagina->gerarchia)==0 )
	              {
	                  SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Hierarchy specification not correct!",NULL,False,NULL,False,NULL);
	                  return;
	              }
	
	printf("Gerarchia modificata=%s\n",pagina->gerarchia);
	/* posso cambiare il tipopagina solo se non e' di regolazione */
	
	      if(tipoPag != TYPE_REGOLAZIONE)
	      {
	          get_something(menu4,XmNmenuHistory, (void*) &WgtOptMenu);
	          if(WgtOptMenu == menu4_p1_b1)
	             strcpy(pagina->geom.tipo,TIPO_STAZIONI);   
	          else if(WgtOptMenu == menu4_p1_b2)
	            strcpy(pagina->geom.tipo,TIPO_SINOTTICO);
	          else if(WgtOptMenu == menu4_p1_b3)
	            strcpy(pagina->geom.tipo,TIPO_TELEPERM);
	
	      }
	
	/* posso cambiare il pagina in_use solo se e' di regolazione */
	
	      if(tipoPag == TYPE_REGOLAZIONE)
	      {
	          get_something(menu7,XmNmenuHistory, (void*) &WgtOptMenu);
	          if(WgtOptMenu == In_use)
		      pagina->in_use = SCHEME_IN_USE;
	            else if(WgtOptMenu == Not_in_use)
			pagina->in_use = SCHEME_NOT_IN_USE;
	      }
	
	      if( pagina->refreshFreq < 1)
	      {
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Refresh Frequenzy Values Not Acceptable",NULL,False,NULL,False,NULL);
	         return;
	      }
	 
	      XrmPutStringResource(&pagina->db,"*nomePag",nomepag);
	      XrmPutStringResource(&pagina->db,"*top_descrizione",pagina->geom.descriz);
	      sprintf(strVal,"%s",pagina->geom.tipo);
	      XrmPutStringResource(&pagina->db,"*top_tipo",strVal);
	      sprintf(strVal,"%d",pagina->geom.x);
	      XrmPutStringResource(&pagina->db,"*top_x",strVal);
	      sprintf(strVal,"%d",pagina->geom.y);
	      XrmPutStringResource(&pagina->db,"*top_y",strVal);
	      sprintf(strVal,"%d",pagina->geom.width);
	      XrmPutStringResource(&pagina->db,"*top_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.height);
	      XrmPutStringResource(&pagina->db,"*top_height",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_width);
	      XrmPutStringResource(&pagina->db,"*drawing_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_height);
	      XrmPutStringResource(&pagina->db,"*drawing_height",strVal);
	      XrmPutStringResource(&pagina->db,"*drawing_background",pagina->geom.background);
	      sprintf(strVal,"%d",pagina->in_use);
	      XrmPutStringResource(&pagina->db,"*schemeInUse",strVal);
	      if(operazione ==  MODIFY_PAGE)
	      {
	         sprintf(strVal,"%d",pagina->refreshFreq);
	         printf("pagina->refreshFreq = %d strVal =%s\n",pagina->refreshFreq,strVal);
	         XrmPutStringResource(&pagina->db,"*refresh_freq",strVal);
	         XrmPutStringResource(&pagina->db,"*gerarchia",pagina->gerarchia);
	      }
	      XrmPutFileDatabase(pagina->db,filepag);
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	
	      /* Aggiornamento del context */
	      for(i=0; i<pagedit->pag_num; i++)
	      {
	      printf("MODIFY_PAGE-agg context- Nomepag:%s Nomepag_list:%s  Indice:%d\n",nomepag, pagedit->page_list[i],i);
	         if(!strcmp(pagedit->page_list[i],nomepag))
	         {
	            break;
	         }
	      }
	      printf("MODIFY_PAGE-agg context- Nomepag:%s  Indice:%d\n",pagedit->page_list[i],i);
	      strcpy(pagedit->res_page[i]->descr_page, pagina->geom.descriz);
	      strcpy(pagedit->res_page[i]->type_page, pagina->geom.tipo);
	      pagedit->res_page[i]->refresh_freq = pagina->refreshFreq;
	      strcpy(pagedit->res_page[i]->hier, pagina->gerarchia);
	      pagedit->res_page[i]->in_use = pagina->in_use;
	      strcpy(pagedit->res_page[i]->tagPag,pagina->tagPag);
	
	      /* Aggiungi l'item nella lista delle pagine */
	      SetItemString(&new_item,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,
	                    pagina->in_use,pagina->tagPag);
	      modify_item(new_item,i);
	/* MAURIC */
	      pagina_free(pagina);
	      libera_memoria((char*)listaobj);
	      XmStringFree(new_item); 
	   break;
	   case MODIFY_LIBRARY:
	      if( XlGetenv(ENVLIB) == NULL)
	         return;
	
	      strcpy(filepag,XlGetenv(ENVLIB));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,LIB_EXT);
	
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag));
	 
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      XrmPutStringResource(&pagina->db,"*nomePag",nomepag);
	      XrmPutStringResource(&pagina->db,"*top_descrizione",pagina->geom.descriz);
	      sprintf(strVal,"%s",pagina->geom.tipo);
	      XrmPutStringResource(&pagina->db,"*top_tipo",strVal);
	      sprintf(strVal,"%d",pagina->geom.x);
	      XrmPutStringResource(&pagina->db,"*top_x",strVal);
	      sprintf(strVal,"%d",pagina->geom.y);
	      XrmPutStringResource(&pagina->db,"*top_y",strVal);
	      sprintf(strVal,"%d",pagina->geom.width);
	      XrmPutStringResource(&pagina->db,"*top_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.height);
	      XrmPutStringResource(&pagina->db,"*top_height",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_width);
	      XrmPutStringResource(&pagina->db,"*drawing_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_height);
	      XrmPutStringResource(&pagina->db,"*drawing_height",strVal);
	      XrmPutStringResource(&pagina->db,"*drawing_background",pagina->geom.background);
	      sprintf(strVal,"%d",pagina->in_use);
	      XrmPutStringResource(&pagina->db,"*schemeInUse",strVal);
	      XrmPutFileDatabase(pagina->db,filepag);
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	      /* Ricerca della posizione della pagina nella lista */ 
	      for(i=0;i<pagedit->iconlib_num;i++)
	         if(!strcmp(nomepag,pagedit->iconlib_list[i]))
	            break;
	      /* Aggiungi l'item nella lista delle pagine */
	      SetItemString(&new_item,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,
	                    pagina->in_use,pagina->tagPag);
	      modify_library_item(new_item,i);
	/* MAURIC */
	      pagina_free(pagina);
	      libera_memoria((char*)listaobj);
	      XmStringFree(new_item);
	   break;
	   case COPY_PAGE:
	      if( XlGetenv(ENVPAG) == NULL)
	         return;
	
	      strcpy(filepag,XlGetenv(ENVPAG));
	      strcat(filepag,"/");
	      strcat(filepag,nomepag);
	      strcat(filepag,PAG_EXT);
	
	/* convers. per VMS */
	      strcpy(filepag,XlConvPathVms(filepag));
	      strcpy(pagina->geom.descriz, XmTextFieldGetString(PagDescription));
	      pagina->geom.x =  atoi(XmTextFieldGetString(PagX));
	      pagina->geom.y =  atoi(XmTextFieldGetString(PagY));
	      pagina->geom.width = atoi(XmTextFieldGetString(PagWidth));
	      pagina->geom.height = atoi(XmTextFieldGetString(PagHeight));
	      pagina->geom.draw_width = atoi(XmTextFieldGetString(DrawWidth));
	      pagina->geom.draw_height = atoi(XmTextFieldGetString(DrawHeight));
	      strcpy(pagina->geom.background,XmTextFieldGetString(PagBackground));
	      pagina->refreshFreq = atoi(XmTextFieldGetString(RefreshFrequenzy));
	      format_hierarchy_new(pagina->gerarchia,XmTextFieldGetString(Hierarchy));
	
	      if( (fp = fopen(filepag,"r")) != NULL)
	      {
	         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Proposed Page Already Exist",NULL,False,NULL,False,NULL);
	         return;
	      }
	      else
	      {
	         if( strcmp(GERARCHIA_KO,pagina->gerarchia)==0 )
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,
	                   "ConfigInfo","Hierarchy specification not correct!",
	                    NULL,False,NULL,False,NULL);
	            return;
	         }
	
	         if( pagina->refreshFreq < 1)
	           {
	              SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Refresh Frequenzy Values Not Acceptable",NULL,False,NULL,False,NULL);
	              return;
	           }         
	         
	         if( pagina_init(oldname,&pagappo,TIPO_PAGINA) == True )
	         {
	            pagina_load_file(pagappo,TIPO_PAGINA);
	            pagina_getres(pagappo,&obj);
	            tipo_sorg = PagGetType(pagappo);
	         } 
	
	/* copio il .pag */
	         strcpy(file1,XlGetenv(ENVPAG));
	         strcat(file1,"/");
	         strcat(file1,oldname);
	         strcat(file1,PAG_EXT);
	         strcpy(file1,XlConvPathVms(file1));
	         XlCopy(file1,filepag);
	
	/* copio il .bkg */
	         strcpy(file1,XlGetenv(ENVPAG));
	         strcat(file1,"/");
	         strcat(file1,oldname);
	         strcat(file1,BKG_EXT);
	         strcpy(file1,XlConvPathVms(file1));
	         strcpy(filebkg,XlGetenv(ENVPAG));
	         strcat(filebkg,"/");
	         strcat(filebkg,nomepag);
	         strcat(filebkg,BKG_EXT);
	         strcpy(filebkg,XlConvPathVms(filebkg));
	         XlCopy(file1,filebkg);
	
	      }
	
	      if(pagedit->pag_num > 0)
	      {
	         pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1));
		 pagedit->res_page = realloc(pagedit->res_page, sizeof(INFO_PAGE *)*(pagedit->pag_num+1));
	         if(( pagedit->page_list == NULL ) || (pagedit->res_page == NULL))
	            return;
	      }
	      else
	         if(((pagedit->page_list = (char **)alloca_memoria(1,sizeof(char *))) == NULL) || 
		    ((pagedit->res_page = (INFO_PAGE **)alloca_memoria(1, sizeof(INFO_PAGE *))) == NULL))
	            return;
	
	      if(((pagedit->page_list[pagedit->pag_num] = (char *)alloca_memoria(strlen(nomepag)+1,sizeof(char))) == NULL) ||
	         ((pagedit->res_page[pagedit->pag_num] = (INFO_PAGE *)alloca_memoria(1,sizeof(INFO_PAGE))) == NULL))
	        return;
	
	      XrmPutStringResource(&pagina->db,"*nomePag",nomepag);
	      XrmPutStringResource(&pagina->db,"*top_descrizione",pagina->geom.descriz);
	      sprintf(strVal,"%s",pagina->geom.tipo);
	      XrmPutStringResource(&pagina->db,"*top_tipo",strVal);
	      sprintf(strVal,"%d",pagina->geom.x);
	      XrmPutStringResource(&pagina->db,"*top_x",strVal);
	      sprintf(strVal,"%d",pagina->geom.y);
	      XrmPutStringResource(&pagina->db,"*top_y",strVal);
	      sprintf(strVal,"%d",pagina->geom.width);
	      XrmPutStringResource(&pagina->db,"*top_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.height);
	      XrmPutStringResource(&pagina->db,"*top_height",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_width);
	      XrmPutStringResource(&pagina->db,"*drawing_width",strVal);
	      sprintf(strVal,"%d",pagina->geom.draw_height);
	      XrmPutStringResource(&pagina->db,"*drawing_height",strVal);
	      XrmPutStringResource(&pagina->db,"*drawing_background",pagina->geom.background);
	      sprintf(strVal,"%d",pagina->in_use);
	      XrmPutStringResource(&pagina->db,"*schemeInUse",strVal);
	
	
	      sprintf(strVal,"%d",pagina->refreshFreq);
	      XrmPutStringResource(&pagina->db,"*refresh_freq",strVal);
	/*
	by fabio
	*/
	      sprintf(strVal,"%d",pagina->snap_pag);
	      XrmPutStringResource(&pagina->db,"*snap_pag",strVal);
	      XrmPutStringResource(&pagina->db,"*gerarchia",pagina->gerarchia);
	      strcpy(pagina->nomepag,nomepag);
	      strcpy(pagina->filepag,filepag);
	
	/* se duplico una regolazione devo aggiornare la nuova PagTag */
	      if(tipo_sorg == TYPE_REGOLAZIONE)
	      { 
	         PaginaSetTagPag(pagedit,pagina);
	         sprintf(strVal,"%s",pagina->tagPag);
	         XrmPutStringResource(&pagina->db,"*tagPag",strVal);
	         TagPagSetNew(pagedit,TYPE_REGOLAZIONE);
	      }
	
	      /* CAPPE. Aggiornamento del context */
	      strcpy(pagedit->page_list[pagedit->pag_num],nomepag);
	      strcpy(pagedit->res_page[pagedit->pag_num]->descr_page, pagina->geom.descriz);
	      strcpy(pagedit->res_page[pagedit->pag_num]->type_page, pagina->geom.tipo);
	      pagedit->res_page[pagedit->pag_num]->refresh_freq = pagina->refreshFreq;
	      strcpy(pagedit->res_page[pagedit->pag_num]->hier, pagina->gerarchia);
	      pagedit->res_page[pagedit->pag_num]->in_use = pagina->in_use;
	      strcpy(pagedit->res_page[pagedit->pag_num]->tagPag,pagina->tagPag);
	      pagedit->pag_num++;
	
	
	      XrmPutFileDatabase(pagina->db,XlConvPathVms(filepag));
	
	      SetItemString(&Cpagname,pagina->nomepag,pagina->geom.tipo,pagina->geom.descriz,pagina->in_use,pagina->tagPag);
	      add_item(Cpagname,0);
	      if(tipo_sorg == TYPE_REGOLAZIONE)
	                        SettaTagInPag(nomepag,NULL);
	/* MAURIC */
	      libera_memoria((char*)listaobj);
	      libera_memoria((char*)obj);
	      pagina_free(pagappo);
	      pagina_free(pagina);
	      XmStringFree(Cpagname);
	   break;
	}
	
	
	/* aggiungo il salvataggio del file context 
	by Carlo
	*/
	if( new_save_context( pagedit ) != True )
	{
	    SetMsg(topLevelShell,NULL,INFMSG,
	               "ConfigInfo","Save of Context failed.",
	               NULL,False,NULL,False,NULL);
	    ContextModified = True;
	}
	
	XtDestroyWidget(PagDialog);
	
#endif
	}
	UxPagDialogContext = UxSaveCtx;
}

static	void	activateCB_CancelButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagDialog           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagDialogContext;
	UxPagDialogContext = UxContext =
			(_UxCPagDialog *) UxGetContext( UxWidget );
	{
	/* MAURIC */
	pagina_free(pagina);
	XtDestroyWidget(PagDialog);
	}
	UxPagDialogContext = UxSaveCtx;
}

static	void	activateCB_pushButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagDialog           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagDialogContext;
	UxPagDialogContext = UxContext =
			(_UxCPagDialog *) UxGetContext( UxWidget );
	{
	extern swidget create_ColorViewer();
	swidget ColorInterface;
	char InitColor[100];
	XmString LabelButton;
	
	strcpy(InitColor,XmTextFieldGetString(PagBackground));
	
	LabelButton = XmStringCreateSimple("Page Background");
	
	ListaWid[0] = pushButton1;
	ListaWid[1] = PagBackground;
	ColorInterface = create_ColorViewer(CONFIG_VER,InitColor,
	                                    LabelButton,UxDisplay->db,
					    ListaWid,PagDialog);
	
	if(ColorInterface)
	   UxPopupInterface(ColorInterface,no_grab);
	
	}
	UxPagDialogContext = UxSaveCtx;
}

static	void	activateCB_menu4_p1_b3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPagDialog           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPagDialogContext;
	UxPagDialogContext = UxContext =
			(_UxCPagDialog *) UxGetContext( UxWidget );
	{
	   /* selezioando il tipo teleperm
	      imposto X,Y,Width,Height della pagina
	      con i valori pre-definiti dal tipo teleperm
	      che dovra' occupare l'intero screen
	   */
	
	   TelepermSetDimension( ); 
	
	}
	UxPagDialogContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_PagDialog()
{
	Widget		menu4_p1_shell;
	Widget		menu_in_use_shell;


	/* Creation of PagDialog */
	PagDialog = XtVaCreatePopupShell( "PagDialog",
			topLevelShellWidgetClass,
			UxTopLevel,
			XmNx, 68,
			XmNy, 185,
			XmNheight, 800,
			XmNwidth, 610,
			NULL );
	UxPutContext( PagDialog, (char *) UxPagDialogContext );


	/* Creation of form2 */
	form2 = XtVaCreateManagedWidget( "form2",
			xmFormWidgetClass,
			PagDialog,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNx, 0,
			XmNy, 100,
			XmNwidth, 610,
			XmNheight, 700,
			NULL );
	UxPutContext( form2, (char *) UxPagDialogContext );


	/* Creation of labelName */
	labelName = XtVaCreateManagedWidget( "labelName",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 50,
			XmNwidth, 90,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Nome Pagina" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelName, (char *) UxPagDialogContext );


	/* Creation of labelX */
	labelX = XtVaCreateManagedWidget( "labelX",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 280,
			XmNwidth, 20,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "X" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelX, (char *) UxPagDialogContext );


	/* Creation of labelY */
	labelY = XtVaCreateManagedWidget( "labelY",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 325,
			XmNwidth, 20,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Y" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelY, (char *) UxPagDialogContext );


	/* Creation of labelDesc */
	labelDesc = XtVaCreateManagedWidget( "labelDesc",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 90,
			XmNwidth, 90,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Description" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelDesc, (char *) UxPagDialogContext );


	/* Creation of PagDescription */
	PagDescription = XtVaCreateManagedWidget( "PagDescription",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 85,
			XmNwidth, 400,
			XmNheight, 35,
			NULL );
	UxPutContext( PagDescription, (char *) UxPagDialogContext );


	/* Creation of PagX */
	PagX = XtVaCreateManagedWidget( "PagX",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 280,
			XmNwidth, 120,
			XmNheight, 35,
			XmNvalue, "",
			NULL );
	UxPutContext( PagX, (char *) UxPagDialogContext );


	/* Creation of PagY */
	PagY = XtVaCreateManagedWidget( "PagY",
			xmTextFieldWidgetClass,
			form2,
XmNx, 120,
			XmNy, 320,
			XmNwidth, 120,
			XmNheight, 35,
			NULL );
	UxPutContext( PagY, (char *) UxPagDialogContext );


	/* Creation of PagBackground */
	PagBackground = XtVaCreateManagedWidget( "PagBackground",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 590,
			XmNwidth, 250,
			XmNheight, 35,
			NULL );
	UxPutContext( PagBackground, (char *) UxPagDialogContext );


	/* Creation of labelTitle1 */
	labelTitle1 = XtVaCreateManagedWidget( "labelTitle1",
			xmLabelWidgetClass,
			form2,
			XmNx, 170,
			XmNy, 10,
			XmNwidth, 185,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Data Page Definition" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--18-180-75-75-p-98-iso8859-1" ),
			NULL );
	UxPutContext( labelTitle1, (char *) UxPagDialogContext );


	/* Creation of OkButton */
	OkButton = XtVaCreateManagedWidget( "OkButton",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 20,
			XmNy, 760,
			XmNwidth, 75,
			XmNheight, 35,
			RES_CONVERT( XmNlabelString, "Ok" ),
			NULL );
	XtAddCallback( OkButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_OkButton,
		(XtPointer) UxPagDialogContext );

	UxPutContext( OkButton, (char *) UxPagDialogContext );


	/* Creation of CancelButton */
	CancelButton = XtVaCreateManagedWidget( "CancelButton",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 760,
			XmNwidth, 75,
			XmNheight, 35,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			NULL );
	XtAddCallback( CancelButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_CancelButton,
		(XtPointer) UxPagDialogContext );

	UxPutContext( CancelButton, (char *) UxPagDialogContext );


	/* Creation of labelTitle2 */
	labelTitle2 = XtVaCreateManagedWidget( "labelTitle2",
			xmLabelWidgetClass,
			form2,
			XmNx, 20,
			XmNy, 250,
			XmNwidth, 175,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Window Size and Position" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1" ),
			NULL );
	UxPutContext( labelTitle2, (char *) UxPagDialogContext );


	/* Creation of labelTitle3 */
	labelTitle3 = XtVaCreateManagedWidget( "labelTitle3",
			xmLabelWidgetClass,
			form2,
			XmNx, 20,
			XmNy, 470,
			XmNwidth, 195,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Drawing Area Size and Color" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1" ),
			NULL );
	UxPutContext( labelTitle3, (char *) UxPagDialogContext );


	/* Creation of labelHeight1 */
	labelHeight1 = XtVaCreateManagedWidget( "labelHeight1",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 405,
			XmNwidth, 55,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Height " ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelHeight1, (char *) UxPagDialogContext );


	/* Creation of labelWidth1 */
	labelWidth1 = XtVaCreateManagedWidget( "labelWidth1",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 365,
			XmNwidth, 55,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Width " ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelWidth1, (char *) UxPagDialogContext );


	/* Creation of PagHeight */
	PagHeight = XtVaCreateManagedWidget( "PagHeight",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 400,
			XmNwidth, 121,
			XmNheight, 35,
			NULL );
	UxPutContext( PagHeight, (char *) UxPagDialogContext );


	/* Creation of PagWidth */
	PagWidth = XtVaCreateManagedWidget( "PagWidth",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 360,
			XmNwidth, 120,
			XmNheight, 35,
			NULL );
	UxPutContext( PagWidth, (char *) UxPagDialogContext );


	/* Creation of labelHeight2 */
	labelHeight2 = XtVaCreateManagedWidget( "labelHeight2",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 540,
			XmNwidth, 69,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Height " ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelHeight2, (char *) UxPagDialogContext );


	/* Creation of DrawHeight */
	DrawHeight = XtVaCreateManagedWidget( "DrawHeight",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 540,
			XmNwidth, 125,
			XmNheight, 35,
			NULL );
	UxPutContext( DrawHeight, (char *) UxPagDialogContext );


	/* Creation of labelWidth2 */
	labelWidth2 = XtVaCreateManagedWidget( "labelWidth2",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 500,
			XmNwidth, 71,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Width " ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelWidth2, (char *) UxPagDialogContext );


	/* Creation of DrawWidth */
	DrawWidth = XtVaCreateManagedWidget( "DrawWidth",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 500,
			XmNwidth, 126,
			XmNheight, 35,
			NULL );
	UxPutContext( DrawWidth, (char *) UxPagDialogContext );


	/* Creation of pushButton1 */
	pushButton1 = XtVaCreateManagedWidget( "pushButton1",
			xmPushButtonWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 590,
			XmNwidth, 100,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Background" ),
			NULL );
	XtAddCallback( pushButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButton1,
		(XtPointer) UxPagDialogContext );

	UxPutContext( pushButton1, (char *) UxPagDialogContext );


	/* Creation of labelTipo */
	labelTipo = XtVaCreateManagedWidget( "labelTipo",
			xmLabelWidgetClass,
			form2,
			XmNx, 10,
			XmNy, 140,
			XmNwidth, 90,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Tipo Pagina" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelTipo, (char *) UxPagDialogContext );


	/* Creation of PagName */
	PagName = XtVaCreateManagedWidget( "PagName",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 40,
			XmNwidth, 175,
			XmNheight, 35,
			XmNmaxLength, 4,
//			XmNcolumns, 20,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1" ),
			NULL );
	UxPutContext( PagName, (char *) UxPagDialogContext );


	/* Creation of menu4_p1 */
	menu4_p1_shell = XtVaCreatePopupShell ("menu4_p1_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu4_p1 = XtVaCreateWidget( "menu4_p1",
			xmRowColumnWidgetClass,
			menu4_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu4_p1, (char *) UxPagDialogContext );


	/* Creation of menu4_p1_b2 */
	menu4_p1_b2 = XtVaCreateManagedWidget( "menu4_p1_b2",
			xmPushButtonWidgetClass,
			menu4_p1,
			RES_CONVERT( XmNlabelString, "Sinottico" ),
			NULL );
	UxPutContext( menu4_p1_b2, (char *) UxPagDialogContext );


	/* Creation of menu4_p1_b1 */
	menu4_p1_b1 = XtVaCreateManagedWidget( "menu4_p1_b1",
			xmPushButtonWidgetClass,
			menu4_p1,
			RES_CONVERT( XmNlabelString, "Stazioni" ),
			NULL );
	UxPutContext( menu4_p1_b1, (char *) UxPagDialogContext );


	/* Creation of menu4_p1_b3 */
	menu4_p1_b3 = XtVaCreateManagedWidget( "menu4_p1_b3",
			xmPushButtonWidgetClass,
			menu4_p1,
			RES_CONVERT( XmNlabelString, "Teleperm" ),
			NULL );
	XtAddCallback( menu4_p1_b3, XmNactivateCallback,
		(XtCallbackProc) activateCB_menu4_p1_b3,
		(XtPointer) UxPagDialogContext );

	UxPutContext( menu4_p1_b3, (char *) UxPagDialogContext );


	/* Creation of menu4 */
	menu4 = XtVaCreateManagedWidget( "menu4",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, menu4_p1,
			XmNx, 158,
			XmNy, 139,
			NULL );
	UxPutContext( menu4, (char *) UxPagDialogContext );


	/* Creation of labelRefreshFrequenzy */
	labelRefreshFrequenzy = XtVaCreateManagedWidget( "labelRefreshFrequenzy",
			xmLabelWidgetClass,
			form2,
			XmNx, 8,
			XmNy, 221,
			XmNwidth, 190,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, "Refresh Frequezy (dec. sec.)" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( labelRefreshFrequenzy, (char *) UxPagDialogContext );


	/* Creation of RefreshFrequenzy */
	RefreshFrequenzy = XtVaCreateManagedWidget( "RefreshFrequenzy",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 202,
			XmNy, 217,
			XmNwidth, 40,
			XmNheight, 35,
			XmNvalue, "",
			NULL );
	UxPutContext( RefreshFrequenzy, (char *) UxPagDialogContext );


	/* Creation of label_in_use */
	label_in_use = XtVaCreateManagedWidget( "label_in_use",
			xmLabelWidgetClass,
			form2,
			XmNx, 7,
			XmNy, 173,
			XmNwidth, 145,
			XmNheight, 26,
			RES_CONVERT( XmNlabelString, "Regolation scheme" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			NULL );
	UxPutContext( label_in_use, (char *) UxPagDialogContext );


	/* Creation of menu_in_use */
	menu_in_use_shell = XtVaCreatePopupShell ("menu_in_use_shell",
			xmMenuShellWidgetClass, form2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu_in_use = XtVaCreateWidget( "menu_in_use",
			xmRowColumnWidgetClass,
			menu_in_use_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( menu_in_use, (char *) UxPagDialogContext );


	/* Creation of In_use */
	In_use = XtVaCreateManagedWidget( "In_use",
			xmPushButtonWidgetClass,
			menu_in_use,
			RES_CONVERT( XmNlabelString, "In use" ),
			NULL );
	UxPutContext( In_use, (char *) UxPagDialogContext );


	/* Creation of Not_in_use */
	Not_in_use = XtVaCreateManagedWidget( "Not_in_use",
			xmPushButtonWidgetClass,
			menu_in_use,
			RES_CONVERT( XmNlabelString, "Not in use" ),
			NULL );
	UxPutContext( Not_in_use, (char *) UxPagDialogContext );


	/* Creation of menu7 */
	menu7 = XtVaCreateManagedWidget( "menu7",
			xmRowColumnWidgetClass,
			form2,
			XmNrowColumnType, XmMENU_OPTION,
			XmNsubMenuId, menu_in_use,
			XmNx, 157,
			XmNy, 171,
			NULL );
	UxPutContext( menu7, (char *) UxPagDialogContext );


	/* Creation of HierarchyLabel */
	HierarchyLabel = XtVaCreateManagedWidget( "HierarchyLabel",
			xmLabelWidgetClass,
			form2,
			XmNx, 20,
			XmNy, 640,
			XmNwidth, 120,
			XmNheight, 30,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNfontList, UxConvertFontList( "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1" ),
			RES_CONVERT( XmNlabelString, "Hierarchy Position" ),
			NULL );
	UxPutContext( HierarchyLabel, (char *) UxPagDialogContext );


	/* Creation of Hierarchy */
	Hierarchy = XtVaCreateManagedWidget( "Hierarchy",
			xmTextFieldWidgetClass,
			form2,
			XmNx, 120,
			XmNy, 670,
			XmNwidth, 240,
			XmNheight, 35,
			NULL );
	UxPutContext( Hierarchy, (char *) UxPagDialogContext );


	XtAddCallback( PagDialog, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxPagDialogContext);


	return ( PagDialog );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_PagDialog( _Uxpname, _Uxoper )
	unsigned char	*_Uxpname;
	int	_Uxoper;
{
	Widget                  rtrn;
	_UxCPagDialog           *UxContext;

	UxPagDialogContext = UxContext =
		(_UxCPagDialog *) UxNewContext( sizeof(_UxCPagDialog), False );

	pname = _Uxpname;
	oper = _Uxoper;

	{
		char appo[100],nomepag[100],*nome_tmp;
		char filepag[MAXLENFILENAME],filebkg[MAXLENFILENAME];
		XmString cstring;
		
		operazione = oper;
		
		if(pname != NULL)
		   strcpy(nomepag,pname);
		
		switch(oper)
		{
		   case CREATE_PAGE:
		      strcpy(nomepag,"PAGE");
		      strcpy(filepag,nomepag);
		      strcat(filepag,PAG_EXT);
		      strcpy(filebkg,nomepag);
		      strcat(filebkg,BKG_EXT);
		   break;
		   case CREATE_REGOLATION:
		      strcpy(nomepag,"REGO");
		      strcpy(filepag,nomepag);
		      strcat(filepag,PAG_EXT);
		      strcpy(filebkg,nomepag);
		      strcat(filebkg,BKG_EXT);
		   break;
		   case CREATE_LIBRARY:
		      strcpy(nomepag,"LIB");
		      strcpy(filepag,nomepag);
		      strcat(filepag,LIB_EXT);
		   break;  
		   case COPY_PAGE:
		      strcpy(oldname,nomepag);
		      tmpnam(appo);                /* nome temporaneo */
		      nome_tmp = &appo[5];
		      strcpy(filepag,nomepag);
		      strcat(filepag,PAG_EXT);
		      strcpy(filebkg,nomepag);
		      strcat(filebkg,BKG_EXT);
		   break;
		}
		rtrn = _Uxbuild_PagDialog();

		switch(oper)
		{
		   case CREATE_PAGE:
		      set_something_val(PagName,XmNmaxLength,(XtArgVal)MAXCHAR_PAGNAME);
		      cstring = XmStringCreateSimple("Data Page Definition");
		      set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);
		      pagina_init(nomepag,&pagina,TIPO_PAGINA);
		      set_something_val(menu7,XmNsensitive,(XtArgVal)False); 
		      set_something_val(label_in_use,XmNsensitive,(XtArgVal)False);	
		      pagina_load_file(pagina,TIPO_PAGINA);
		      PagSetDefaultValue(pagina);
		      riempi_campi(pagina);
		      XmStringFree(cstring);
		   break;
		   case CREATE_LIBRARY:
		      set_something_val(PagName,XmNmaxLength,(XtArgVal)MAXCHAR_PAGNAME);
		      cstring = XmStringCreateSimple("Data Library Definition");
		      set_something_val(labelTitle1,XmNlabelString,(XtArgVal) cstring);
		      set_something_val(labelTipo,XmNsensitive,(XtArgVal)False);
		      set_something_val(menu4,XmNsensitive,(XtArgVal)False);
		      XtVaSetValues(menu4_p1_b2,RES_CONVERT(XmNlabelString,"Library"),NULL);
		      set_something_val(menu7,XmNsensitive,(XtArgVal)False);
		      set_something_val(label_in_use,XmNsensitive,(XtArgVal)False);
		      set_something_val(pushButton1,XmNsensitive,(XtArgVal)False);
		      set_something_val(PagBackground,XmNsensitive,(XtArgVal)False);
		      set_something_val(RefreshFrequenzy,XmNsensitive,(XtArgVal)False);
		      set_something_val(labelRefreshFrequenzy,XmNsensitive,(XtArgVal)False);
		      set_something_val(Hierarchy,XmNsensitive,(XtArgVal)False);
		      set_something_val(HierarchyLabel,XmNsensitive,(XtArgVal)False);
		      pagina_init(nomepag,&pagina,TIPO_LIBRARY); 
		      pagina_load_file(pagina,TIPO_LIBRARY);
		      PagSetDefaultValue(pagina);
		      riempi_campi(pagina);
		      XmStringFree(cstring);
		   break;
		   case CREATE_REGOLATION:
		      cstring = XmStringCreateSimple("Data Regolation Definition");
		      set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);
		      set_something_val(PagName,XmNmaxLength,(XtArgVal)MAXREGNAME);    /* nome len max di 4ch */
		      set_something_val(labelTipo,XmNsensitive,(XtArgVal)False);
		      set_something_val(menu4,XmNsensitive,(XtArgVal)False);
		      XtVaSetValues(menu4_p1_b2,RES_CONVERT(XmNlabelString,"Regolazione"),NULL);
		
		      pagina_init(nomepag,&pagina,TIPO_PAGINA);
		      pagina_load_file(pagina,TIPO_PAGINA);
		      PagSetDefaultValue(pagina);
		      riempi_campi(pagina);
		      XmStringFree(cstring);
		      
		   break; 
		   case MODIFY_PAGE:
		      {
		
		          pagina_init(nomepag,&pagina,TIPO_PAGINA);
		          pagina_load_file(pagina,TIPO_PAGINA);
		          pagina_getres(pagina,&listaobj);
		          tipoPag = PagGetType(pagina);
		          switch(tipoPag)
		          {
		             case TYPE_REGOLAZIONE:
		                cstring = XmStringCreateSimple("Data Regolation Definition");
		                set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);
		                set_something_val(labelTipo,XmNsensitive,(XtArgVal)False);
		                set_something_val(menu4,XmNsensitive,(XtArgVal)False);
		                XtVaSetValues(menu4_p1_b2,RES_CONVERT(XmNlabelString,"Regolazione"),NULL);
		
		             break;
		             case TYPE_SINOTTICO:
		             case TYPE_STAZIONE:
		             case TYPE_TELEPERM:      
				set_something_val(menu7,XmNsensitive,(XtArgVal)False);
		      		set_something_val(label_in_use,XmNsensitive,(XtArgVal)False);
		             break;
		
		          }
		
		          riempi_campi(pagina);
		          SetOptMenu(pagina);
		          XmTextFieldSetEditable(PagName,False);
		     } 
		   break;
		   case MODIFY_LIBRARY:
		     cstring = XmStringCreateSimple("Data Library Definition");
		     set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);
		     set_something_val(labelTipo,XmNsensitive,(XtArgVal)False);
		     set_something_val(menu4,XmNsensitive,(XtArgVal)False);
		     XtVaSetValues(menu4_p1_b2,RES_CONVERT(XmNlabelString,"Library"),NULL);
		     set_something_val(menu7,XmNsensitive,(XtArgVal)False);
		     set_something_val(label_in_use,XmNsensitive,(XtArgVal)False);
		     set_something_val(pushButton1,XmNsensitive,(XtArgVal)False);
		     set_something_val(PagBackground,XmNsensitive,(XtArgVal)False);
		     set_something_val(RefreshFrequenzy,XmNsensitive,(XtArgVal)False);
		     set_something_val(labelRefreshFrequenzy,XmNsensitive,(XtArgVal)False);
		     set_something_val(Hierarchy,XmNsensitive,(XtArgVal) False);
		     pagina_init(nomepag,&pagina,TIPO_LIBRARY);
		     pagina_load_file(pagina,TIPO_LIBRARY);
		     pagina_getres(pagina,&listaobj);
		     riempi_campi(pagina);
		     SetOptMenu(pagina);
		     XmTextFieldSetEditable(PagName,False);
		     XmStringFree(cstring);
		   break;
		   case COPY_PAGE:
		      {
		
		          set_something_val(PagName,XmNmaxLength,(XtArgVal)MAXCHAR_PAGNAME);
		          pagina_init(nomepag,&pagina,TIPO_PAGINA);
		          pagina_load_file(pagina,TIPO_PAGINA);
		          pagina_getres(pagina,&listaobj);
		          tipoPag = PagGetType(pagina);
		          switch(tipoPag)
		          {
		             case TYPE_REGOLAZIONE:
		                nome_tmp[4] = 0;
		                set_something_val(PagName,XmNmaxLength,(XtArgVal)MAXREGNAME);    /* nome len max di 4ch */
		                cstring = XmStringCreateSimple("Data Regolation Definition");
		                set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);
		                set_something_val(labelTipo,XmNsensitive,(XtArgVal)False);
		                set_something_val(menu4,XmNsensitive,(XtArgVal)False);
		                XmStringFree(cstring);
		             break; 
		             case TYPE_STAZIONE:
		             case TYPE_SINOTTICO:
			     case TYPE_TELEPERM:
		                cstring = XmStringCreateSimple("Data Page Definition");
		                set_something_val(labelTitle1,XmNlabelString,(XtArgVal)cstring);     
		                set_something_val(menu7,XmNsensitive,(XtArgVal)False);
		                set_something_val(label_in_use,XmNsensitive,(XtArgVal)False);
		                XmStringFree(cstring);
		             break;
		          }
		          riempi_campi(pagina);
		          SetOptMenu(pagina);
		          XmTextFieldSetString(PagName,nome_tmp);
		    }
		   break;
		}
		
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

