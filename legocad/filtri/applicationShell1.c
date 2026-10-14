
/*******************************************************************************
	applicationShell1.c
	(Generated from interface file applicationShell1.i)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
	Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
   modulo applicationShell1.i
   tipo 
   release 2.6
   data 3/20/95
   reserved @(#)applicationShell1.i	2.6
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <Xm/List.h>     
#ifndef FILTRI_H
#include "filtri.h"
#endif          

extern char filedf[];                 /* filename file di editing */
extern FILE *fpedf;                   /* file di edting */
extern FILTRI *filtri[];              /* vettore filtri */
extern short num_filtri;              /* numero di filtri definiti */

int closedf(void);
void aggiorna_edit_shell(FILTRI *filtro);


extern swidget create_exitdialogShell1();  
extern swidget exitdialogShell1;	         /* Quit Application swidget */ 
extern swidget create_opendialogShell2();
extern swidget opendialogShell2;		 /* Editing File Selection */
extern swidget create_editShell1();
extern swidget editShell1;                       /* Edit Filter  */ 

short num_filtro_selez;                          /* indice filtro selezionato */
int operaz;                                      /* tipo operazione menu di EDIT */

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
	Widget	UxfiltriShell1;
	Widget	UxmainWindow1;
	Widget	Uxmenu2;
	Widget	Uxmenu1_p1;
	Widget	Uxmenu1_p1_b3;
	Widget	Uxmenu1_p1_b2;
	Widget	Uxmenu1_top_b1;
	Widget	Uxmenu2_p2;
	Widget	Uxmenu2_p2_b1;
	Widget	Uxmenu2_p2_b2;
	Widget	Uxmenu2_p2_b3;
	Widget	Uxmenu2_top_b1;
	Widget	Uxform1;
	Widget	UxscrolledWindow1;
	Widget	UxscrolledWindow2;
	Widget	UxlabelGadget2;
	Widget	UxlabelGadget1;
	int	Uxselez;
} _UxCfiltriShell1;

#define filtriShell1            UxFiltriShell1Context->UxfiltriShell1
#define mainWindow1             UxFiltriShell1Context->UxmainWindow1
#define menu2                   UxFiltriShell1Context->Uxmenu2
#define menu1_p1                UxFiltriShell1Context->Uxmenu1_p1
#define menu1_p1_b3             UxFiltriShell1Context->Uxmenu1_p1_b3
#define menu1_p1_b2             UxFiltriShell1Context->Uxmenu1_p1_b2
#define menu1_top_b1            UxFiltriShell1Context->Uxmenu1_top_b1
#define menu2_p2                UxFiltriShell1Context->Uxmenu2_p2
#define menu2_p2_b1             UxFiltriShell1Context->Uxmenu2_p2_b1
#define menu2_p2_b2             UxFiltriShell1Context->Uxmenu2_p2_b2
#define menu2_p2_b3             UxFiltriShell1Context->Uxmenu2_p2_b3
#define menu2_top_b1            UxFiltriShell1Context->Uxmenu2_top_b1
#define form1                   UxFiltriShell1Context->Uxform1
#define scrolledWindow1         UxFiltriShell1Context->UxscrolledWindow1
#define scrolledWindow2         UxFiltriShell1Context->UxscrolledWindow2
#define labelGadget2            UxFiltriShell1Context->UxlabelGadget2
#define labelGadget1            UxFiltriShell1Context->UxlabelGadget1
#define selez                   UxFiltriShell1Context->Uxselez

static _UxCfiltriShell1	*UxFiltriShell1Context;

Widget	scrolledList1;
Widget	scrolledList2;

/*******************************************************************************
	Forward declarations of functions that are defined later in this file.
*******************************************************************************/

Widget	create_filtriShell1();

/*******************************************************************************
	Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*==========================================================
|
|  add_items_list:  Aggiunge n items alla lista
|
===========================================================*/

void add_items_list(swidget lista,char *dati[],int n_item)
{
   XmString item;
   int i;


   UxPutVisibleItemCount(lista,n_item);
   if(UxGetWidget(lista) != NULL)
   {
      printf("\neseguo la add_items_list\n");
      for(i=0;i < n_item;i++)
      {
         printf("item = %s\n",dati[i]);
         item = (XmString)XmStringCreateLtoR( dati[i], XmSTRING_DEFAULT_CHARSET); 
         XmListAddItem(UxGetWidget(lista),item,i+1);
         XmStringFree(item);        
      }
   }
   else
      printf("\nadd_items_list lista errata\n");

   printf("/n ho terminato la add item \n");
}

/*========================================================
|
| delete_all_items:  elimina tutti gli items dalla lista
|
==========================================================*/

void delete_all_items(swidget lista)
{
   if(UxGetWidget(lista) != NULL)
   {
      printf("\neseguo la delete_all_item\n");
      XmListDeleteAllItems(UxGetWidget(lista));
   }
   else
      printf("\ndelete_all_item lista non corretta\n");

}

/*==========================================================
|
|  refresh_lista_nomi:  Effettua il refresh della lista de
|                       nomi dei filtri
|
===========================================================*/

void refresh_lista_nomi()
{
   XmString item;
   int i;

   delete_all_items(scrolledList1);

   UxPutVisibleItemCount(scrolledList1,num_filtri);

   printf("\neseguo la refresh_lista_nomi\n");
   for(i=0;i < num_filtri;i++)
   {
      item = (XmString)XmStringCreateLtoR( filtri[i]->nome_filtro, XmSTRING_DEFAULT_CHARSET); 
      XmListAddItem(UxGetWidget(scrolledList1),item,i+1);
      XmStringFree(item);        
   }
}


/*==========================================================
|
|   filtro_init:  alloca la memoria per la struttura di un 
|                 filtro e ne restituisce  il puntatore
|                 
===========================================================*/

FILTRI *filtro_init()
{
   short j;
   FILTRI *ftr;

   printf("\n inizializzazione FILTRO \n");

   ftr = (FILTRI *)(malloc(DIM_FILTRO));
   memset(ftr,0,DIM_FILTRO);
   for(j=0;j<MAXPARFILTRO;j++)
   {
      ftr->var[j] = (char *)(malloc( DIM_PAR ));
      memset(ftr->var[j],0,DIM_PAR);
   }
   printf("\nterminata la inizializzaione \n");
   return(ftr);
}

/*===============================================================
|
|    swap_filtri:  compatta il vettore  dei filtri dopo 
|                  una cancellazione
|
================================================================*/

void swap_filtri(int ind)
{
   int j;

   for(j=ind;j<num_filtri;j++)
   {
      filtri[j] = filtri[j+1];    
   }
}


/*================================================================
|
|    filtro_free:  libera la memoria allocata per un filtro 
|                  e ne ripulisce il contenuto
|
=================================================================*/

void filtro_free(int ind)
{
   int j;

   for(j=0;j<filtri[ind]->num_var;j++)
   {
      memset(filtri[ind]->var[j],0,DIM_PAR);
      free(filtri[ind]->var[j]);
   }
   memset(filtri[ind],0,DIM_FILTRO);
   free(filtri[ind]);         
   num_filtri--;
   swap_filtri(ind);
   
}




/*===================================================================
|
|  openedf:   apertura file di editing.  Il file pointer e' globale
|             cosi' come le definizioni di errore.            
|             Attualmente il nome del file e'cablato
|            
|
===================================================================*/

int openedf()
{
   char *getenv();

   if(fpedf != NULL)
      closedf();

   strcpy(filedf,getenv("LEGOCAD_USER"));
   strcat(filedf,"/filtri.edf");

   printf("\nApro file -> %s \n",filedf);
   if( ( fpedf=fopen(filedf,"r+") ) == NULL)
   {
      if( ( fpedf=fopen(filedf,"w") ) == NULL)
         return(ERROPEN);
   }
   return(OK);
}
 
/*===================================================================
|
|  closedf:   chiusura file di editing. Il file pointer e' globale
|
====================================================================*/

int closedf()
{
   fclose(fpedf);
   printf("\n terminata la closedf\n");
}

/*===================================================================
|
|  readedf:   lettura file di editing.  Il file pointer e' globale
|             
|             cosi come le condizioni di errore
|
===================================================================*/

int readedf()
{
   char buff[MAXCHNAME];
   short i=0,j=0;

   printf("\ninizia la lettura filtri \n");

   num_filtri = 0; /* inizializzo */

   while(1)
   {
      if((fscanf(fpedf,"%s",buff)) == EOF)
         break;
      if(feof(fpedf))
         break;

      if(!strcmp(buff,TAPPO))   /* inizio record */
      {
         filtri[num_filtri] =  filtro_init();
         fscanf(fpedf,"%s",buff);
         strcpy(filtri[num_filtri]->nome_filtro,buff);
         printf("/nletto filtro -> %s\n",filtri[num_filtri]->nome_filtro);
         num_filtri++;
         j=0;
      }
      else
      {
         strcpy((filtri[num_filtri - 1])->var[j],buff);
         j++;
         filtri[num_filtri - 1]->num_var = j;
      }
   }

   printf("termina la lettura filtri \n");
}


/*===================================================================
|
|  writedf:   scrittura file di editing.  Il file pointer e' globale             
|             cosi come le condizioni di errore
|
===================================================================*/

int writedf()
{
   char buff[MAXCHNAME];
   short i=0,j=0;

   if(fpedf == NULL)
   {
      printf("\nNESSUN FILE DI EDITING APERTO\n");
      return(-1);
   }

   fclose(fpedf);
   fpedf = fopen(filedf,"w");
   for(i=0;i<num_filtri;i++)
   {
      fprintf(fpedf,"%s","FILTRO ");
      printf("/n scrivo il tappo %s\n",TAPPO);
      fprintf(fpedf,"%s\n",filtri[i]->nome_filtro);
      printf("/n scrivo il nome filtro %s\n",filtri[i]->nome_filtro);
      for(j=0;j<filtri[i]->num_var;j++)
      {
         fprintf(fpedf,"%s\n",filtri[i]->var[j]);
         printf("/n scrivo il filtro %s\n",filtri[i]->var[j]);
      }
   }
   fclose(fpedf);
   printf("\nsalvataggio terminato\n");
}

/*******************************************************************************
	The following are callback functions.
*******************************************************************************/

static void	activateCB_menu1_p1_b3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	   writedf();
	}
	UxFiltriShell1Context = UxSaveCtx;
}

static void	activateCB_menu1_p1_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	   UxPopupInterface(exitdialogShell1,no_grab);
	}
	UxFiltriShell1Context = UxSaveCtx;
}

static void	activateCB_menu2_p2_b1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	   if(selez == True)
	   {
	      UxPopupInterface(editShell1,no_grab);
	      operaz = OPEN_FILTRO;
	      aggiorna_edit_shell(filtri[num_filtro_selez]);
	   }
	}
	UxFiltriShell1Context = UxSaveCtx;
}

static void	activateCB_menu2_p2_b2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	   extern int clear_field();
	 
	   printf("\npopup interface editShell1\n");
	   operaz = NEW_FILTRO;
	   clear_field();
	   UxPopupInterface(editShell1,no_grab);
	}
	UxFiltriShell1Context = UxSaveCtx;
}

static void	activateCB_menu2_p2_b3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	   extern void filtro_free();
	   int i,j;
	
	   if(selez == True)
	   {
	      printf("\nparte la delete del filtro n.%d\n",num_filtro_selez);
	  
	      filtro_free(num_filtro_selez);
	
	      delete_all_items(scrolledList1);
	      delete_all_items(scrolledList2);
	      refresh_lista_nomi();
	      printf("\ndelete filtro terminata\n");
	
	      selez=False;
	
	   }
	}
	UxFiltriShell1Context = UxSaveCtx;
}

static void	singleSelectionCB_scrolledList1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;
{
	_UxCfiltriShell1        *UxSaveCtx, *UxContext;

	UxSaveCtx = UxFiltriShell1Context;
	UxFiltriShell1Context = UxContext =
			(_UxCfiltriShell1 *) UxGetContext( UxWidget );
	{
	short i;
	XmListCallbackStruct *callb;
	XmString item;
	char *par[MAXPARFILTRO];
	
	selez = True;
	
	num_filtro_selez = -1;
	callb = (XmListCallbackStruct *)UxCallbackArg;
	num_filtro_selez = callb->item_position -1;
	
	for(i=0;i<(filtri[num_filtro_selez])->num_var;i++)
	{
	   par[i] = (char *)malloc(sizeof(char) * MAXCHARPAR);
	   strcpy(par[i],filtri[num_filtro_selez]->var[i]);
	}
	
	printf("n var in risposta %d\n",(filtri[num_filtro_selez])->num_var);
	
	delete_all_items(scrolledList2);
	add_items_list(scrolledList2,par,filtri[num_filtro_selez]->num_var);
	
	
	for(i=0;i<(filtri[num_filtro_selez])->num_var;i++)
	   free(par[i]);
	
	/*  niente free(par): e' un array sullo stack, non memoria allocata, e
	    liberarlo faceva abortire il programma (free(): invalid pointer).
	    Si liberano solo le stringhe, qui sopra. Niente free(callb): e' la
	    struttura che Motif passa al callback, e appartiene a Motif.  */
	}
	UxFiltriShell1Context = UxSaveCtx;
}

/*******************************************************************************
	The 'build_' function creates all the widgets
	using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_filtriShell1()
{
	Widget	menu1_p1_shell;
	Widget	menu2_p2_shell;

	filtriShell1 = XtVaCreatePopupShell( "filtriShell1",
			applicationShellWidgetClass, UxTopLevel,
			XmNminWidth, 300,
			XmNminHeight, 200,
			XmNallowShellResize, FALSE,
			XmNheight, 490,
			XmNwidth, 610,
			XmNy, 430,
			XmNx, 630,
			NULL );

	UxPutContext( filtriShell1, (char *) UxFiltriShell1Context );

	mainWindow1 = XtVaCreateManagedWidget( "mainWindow1",
			xmMainWindowWidgetClass, filtriShell1,
			XmNheight, 460,
			XmNwidth, 590,
			XmNy, 20,
			XmNx, 10,
			XmNunitType, XmPIXELS,
			NULL );

	UxPutContext( mainWindow1, (char *) UxFiltriShell1Context );

	menu2 = XtVaCreateManagedWidget( "menu2",
			xmRowColumnWidgetClass, mainWindow1,
			XmNmenuAccelerator, "<KeyUp>F10",
			XmNrowColumnType, XmMENU_BAR,
			NULL );

	UxPutContext( menu2, (char *) UxFiltriShell1Context );

	menu1_p1_shell = XtVaCreatePopupShell ("menu1_p1_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu1_p1 = XtVaCreateWidget( "menu1_p1",
			xmRowColumnWidgetClass, menu1_p1_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu1_p1, (char *) UxFiltriShell1Context );

	menu1_p1_b3 = XtVaCreateManagedWidget( "menu1_p1_b3",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "S" ),
			RES_CONVERT( XmNlabelString, "Save" ),
			NULL );

	UxPutContext( menu1_p1_b3, (char *) UxFiltriShell1Context );

	menu1_p1_b2 = XtVaCreateManagedWidget( "menu1_p1_b2",
			xmPushButtonWidgetClass, menu1_p1,
			RES_CONVERT( XmNmnemonic, "Q" ),
			RES_CONVERT( XmNlabelString, "Quit" ),
			NULL );

	UxPutContext( menu1_p1_b2, (char *) UxFiltriShell1Context );

	menu1_top_b1 = XtVaCreateManagedWidget( "menu1_top_b1",
			xmCascadeButtonWidgetClass, menu2,
			XmNsubMenuId, menu1_p1,
			RES_CONVERT( XmNlabelString, "File" ),
			NULL );

	UxPutContext( menu1_top_b1, (char *) UxFiltriShell1Context );

	menu2_p2_shell = XtVaCreatePopupShell ("menu2_p2_shell",
			xmMenuShellWidgetClass, menu2,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	menu2_p2 = XtVaCreateWidget( "menu2_p2",
			xmRowColumnWidgetClass, menu2_p2_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );

	UxPutContext( menu2_p2, (char *) UxFiltriShell1Context );

	menu2_p2_b1 = XtVaCreateManagedWidget( "menu2_p2_b1",
			xmPushButtonWidgetClass, menu2_p2,
			RES_CONVERT( XmNlabelString, "Open" ),
			NULL );

	UxPutContext( menu2_p2_b1, (char *) UxFiltriShell1Context );

	menu2_p2_b2 = XtVaCreateManagedWidget( "menu2_p2_b2",
			xmPushButtonWidgetClass, menu2_p2,
			RES_CONVERT( XmNlabelString, "New" ),
			NULL );

	UxPutContext( menu2_p2_b2, (char *) UxFiltriShell1Context );

	menu2_p2_b3 = XtVaCreateManagedWidget( "menu2_p2_b3",
			xmPushButtonWidgetClass, menu2_p2,
			RES_CONVERT( XmNlabelString, "Delete" ),
			NULL );

	UxPutContext( menu2_p2_b3, (char *) UxFiltriShell1Context );

	menu2_top_b1 = XtVaCreateManagedWidget( "menu2_top_b1",
			xmCascadeButtonWidgetClass, menu2,
			XmNsubMenuId, menu2_p2,
			RES_CONVERT( XmNlabelString, "Edit" ),
			NULL );

	UxPutContext( menu2_top_b1, (char *) UxFiltriShell1Context );

	form1 = XtVaCreateManagedWidget( "form1",
			xmFormWidgetClass, mainWindow1,
			NULL );

	UxPutContext( form1, (char *) UxFiltriShell1Context );

	scrolledWindow1 = XtVaCreateManagedWidget( "scrolledWindow1",
			xmScrolledWindowWidgetClass, form1,
			XmNrightPosition, 70,
			XmNresizable, FALSE,
			XmNtopOffset, 50,
			XmNtopAttachment, XmATTACH_FORM,
			XmNleftOffset, 30,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightOffset, 0,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 60,
			XmNx, 30,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow1, (char *) UxFiltriShell1Context );

	scrolledList1 = XtVaCreateManagedWidget( "scrolledList1",
			xmListWidgetClass, scrolledWindow1,
			XmNtraversalOn, TRUE,
			XmNvisibleItemCount, 1,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNheight, 310,
			XmNwidth, 280,
			NULL );

	UxPutContext( scrolledList1, (char *) UxFiltriShell1Context );

	scrolledWindow2 = XtVaCreateManagedWidget( "scrolledWindow2",
			xmScrolledWindowWidgetClass, form1,
			XmNrightPosition, 95,
			XmNleftPosition, 75,
			XmNleftWidget, scrolledWindow1,
			XmNtopOffset, 50,
			XmNtopAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftOffset, 0,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNbottomOffset, 50,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNwidth, 100,
			XmNheight, 320,
			XmNshadowThickness, 0,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNvisualPolicy, XmVARIABLE,
			XmNy, 50,
			XmNx, 360,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			NULL );

	UxPutContext( scrolledWindow2, (char *) UxFiltriShell1Context );

	scrolledList2 = XtVaCreateManagedWidget( "scrolledList2",
			xmListWidgetClass, scrolledWindow2,
			XmNlistSizePolicy, XmCONSTANT,
			XmNselectionPolicy, XmSINGLE_SELECT,
			XmNheight, 320,
			XmNwidth, 230,
			NULL );

	UxPutContext( scrolledList2, (char *) UxFiltriShell1Context );

	labelGadget2 = XtVaCreateManagedWidget( "labelGadget2",
			xmLabelGadgetClass, form1,
			XmNbottomWidget, scrolledWindow2,
			XmNbottomOffset, 5,
			RES_CONVERT( XmNlabelString, "Filter" ),
			XmNrightPosition, 95,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNleftPosition, 75,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNtopPosition, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNbottomPosition, 0,
			XmNbottomAttachment, XmATTACH_WIDGET,
			XmNheight, 30,
			XmNwidth, 120,
			XmNy, 10,
			XmNx, 440,
			NULL );

	UxPutContext( labelGadget2, (char *) UxFiltriShell1Context );

	labelGadget1 = XtVaCreateManagedWidget( "labelGadget1",
			xmLabelGadgetClass, form1,
			XmNbottomWidget, scrolledWindow1,
			XmNbottomOffset, 5,
			XmNtopPosition, 0,
			XmNrightPosition, 70,
			XmNleftOffset, 30,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopOffset, 0,
			XmNtopAttachment, XmATTACH_POSITION,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNbottomAttachment, XmATTACH_WIDGET,
			RES_CONVERT( XmNlabelString, "Filter Name" ),
			XmNheight, 20,
			XmNwidth, 280,
			XmNy, 10,
			XmNx, 30,
			NULL );

	UxPutContext( labelGadget1, (char *) UxFiltriShell1Context );

	XtAddCallback( filtriShell1, XmNdestroyCallback,
			UxFreeClientDataCB,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( menu1_p1_b3, XmNactivateCallback,
			activateCB_menu1_p1_b3,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( menu1_p1_b2, XmNactivateCallback,
			activateCB_menu1_p1_b2,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( menu2_p2_b1, XmNactivateCallback,
			activateCB_menu2_p2_b1,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( menu2_p2_b2, XmNactivateCallback,
			activateCB_menu2_p2_b2,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( menu2_p2_b3, XmNactivateCallback,
			activateCB_menu2_p2_b3,
			(XtPointer) UxFiltriShell1Context );

	XtAddCallback( scrolledList1, XmNsingleSelectionCallback,
			singleSelectionCB_scrolledList1,
			(XtPointer) UxFiltriShell1Context );


	XmMainWindowSetAreas( mainWindow1, menu2, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, form1 );

	return ( filtriShell1 );
}

/*******************************************************************************
	The following function includes the code that was entered
	in the 'Initial Code' and 'Final Code' sections of the
	Declarations Editor. This function is called from the
	'Interface function' below.
*******************************************************************************/

static Widget	_Ux_create_filtriShell1()
{
	Widget                  rtrn;
	_UxCfiltriShell1        *UxContext;

	UxFiltriShell1Context = UxContext =
		(_UxCfiltriShell1 *) XtMalloc( sizeof(_UxCfiltriShell1) );

	{
		/* selez = variabile di stato filtro selezionato */
		
		selez=False;
		rtrn = _Uxbuild_filtriShell1();

		/*================================================================
		|
		|  mappa le interfacce apre e legge il file di editing dei filtri
		|  e visualizza i filtri 
		|
		================================================================*/
		            
		{
		   short i=0,j=0,IER=0;
		   char *nomi[MAXFILTRI];
		
		   create_exitdialogShell1();
		   create_editShell1();
		
		
		   operaz = FILE_OPEN;
		
		   strcpy(filedf,NOMEFILEDIEDITING);
		
		   IER = openedf();                           /* apertura file di editing */
		   if( IER != OK)
		      printf("Errore apertura file di editing\n");
		   else
		   {
		      readedf();                             /* lettura file di editing */
		
		      for(i=0;i<num_filtri;i++)
		      {   
		         nomi[i] = (char *)(malloc(sizeof(char) * MAXCHNAME));
		         strcpy(nomi[i],filtri[i]->nome_filtro);
		      }
		
		      delete_all_items(scrolledList1);  /* elimino tutti gli items precedenti */
		      delete_all_items(scrolledList2);  
		      add_items_list(scrolledList1,nomi,num_filtri);  /* visualizzo la nuova lista */
		
		      closedf();
		
		      for(i=0;i<num_filtri;i++)
		         free(nomi[i]);
		   }
		
		   return(rtrn);
		}
	}
}

/*******************************************************************************
	The following is the 'Interface function' which is the
	external entry point for creating this interface.
	This function should be called from your application or from
	a callback function.
*******************************************************************************/

Widget	create_filtriShell1()
{
	Widget			_Uxrtrn;

	_Uxrtrn = _Ux_create_filtriShell1();

	return ( _Uxrtrn );
}

/*******************************************************************************
	END OF FILE
*******************************************************************************/

