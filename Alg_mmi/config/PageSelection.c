
/*******************************************************************************
       PageSelection.c
       (Generated from interface file PageSelection.i)
*******************************************************************************/

#include <stdio.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/SelectioB.h>
#include <Xm/FileSB.h>

#include <Xm/MenuShell.h>

#include "UxXt.h"
#include "libutilx.h"


/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo PageSelection.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)PageSelection.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/

#include <stdlib.h>
#include <Xm/List.h>
#include "config.h"
#include "message.h"
#include "libutilx.h"

extern swidget topLevelShell;

static void SelezioneMultipla( );
extern Boolean file_exist(char *);
extern void SetItemString();


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
	Widget	UxPageSelection;
	int	Uxtip_operaz;
	int	Uxope;
} _UxCPageSelection;

static _UxCPageSelection       *UxPageSelectionContext;
#define PageSelection           UxPageSelectionContext->UxPageSelection
#define tip_operaz              UxPageSelectionContext->Uxtip_operaz
#define ope                     UxPageSelectionContext->Uxope



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_PageSelection();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*
   estrae da fileSpec il nome del file senza path e senza estensione
   se non ce l'ha ritorna lo stesso puntatore
   il chiamante deve verificare che non sia NULL 
ATTENZIONE AL TOKEN 
*/

char *extractFileNameNExt(char *fileSpec) 
{
    int i,len;
#ifdef VMS
    char token = ']';
#else
    char token = '/';
#endif

    char extension = '.'; 
    char *InitPoint;
    char Name[MAXCHAR_PAGNAME];

    len = strlen(fileSpec);

    InitPoint = fileSpec;

/* elimino la path cercando il token */     
    for(i=len-1;i>=0;i--)
       if(fileSpec[i] == token)
       {
          InitPoint = &(fileSpec[i+1]);
          break;
       }

/* tolgo l'estensione cercando il punto */
    len = strlen(InitPoint);
    for(i=0;i<len;i++)
       if(InitPoint[i] == extension)
       {  
           InitPoint[i] = '\0';   
           break;
       }

    printf("Nome Pagina = %s\n",InitPoint);
    return(InitPoint);
}

static void SelezioneMultipla(Widget wid,
	XtPointer ClientData,XtPointer CallbackArg)
{
XmListCallbackStruct *CallArg = (XmListCallbackStruct *)CallbackArg;
Widget lista = (Widget)ClientData;
int i;
extern char *extract_string();
char *elenco = NULL;
int size = 0;


for(i=0;i<CallArg->selected_item_count;i++)
	size += (strlen(extract_string(CallArg->selected_items[i]))+4);

elenco = XtCalloc(sizeof(char),size+4);
i=0;
for(i=0;i<CallArg->selected_item_count;i++)
	{
	if(i == 0) 
	   strcpy(elenco,extract_string(CallArg->selected_items[i]));
	else
	   {
	    strcat(elenco," ");
	    strcat(elenco,extract_string(CallArg->selected_items[i]));
	   }
	}
if(i == 0) strcpy(elenco,"");
set_something(lista,XmNvalue,(void*) elenco);
XmUpdateDisplay(lista);
XtFree(elenco);
}

char *estrai_campo(char *t,int pos)
{
char *app_t;
char *app_char, *app_char2;
int i;
char *ret;

app_char = t;
for(i=0;i<pos;i++)
        {
        app_char = strstr(app_char," ");
        if(app_char == NULL) return(NULL);
        while(*app_char == ' ') ++app_char;
        }
app_t = calloc(sizeof(char),strlen(t)+1);
strcpy(app_t,t);
app_char = app_t;
while(*app_char == ' ') ++app_char;
for(i=0;i<pos;i++)
        {
        app_char = strstr(app_char," ");
        if(app_char == NULL) return(NULL);
        while(*app_char == ' ') ++app_char;
        }
app_char2=strstr(app_char," ");
if(app_char2!=NULL) *app_char2=0;
ret = calloc(sizeof(char),strlen(app_char)+1);
strcpy(ret,app_char);
free(app_t);
return(ret);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	okCallback_PageSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPageSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPageSelectionContext;
	UxPageSelectionContext = UxContext =
			(_UxCPageSelection *) UxGetContext( UxWidget );
	{
	extern Arg args[];
	extern Cardinal lnargs;
	extern Boolean ContextModified;
	extern char *extract_string();
	extern PAGEDIT_CONTEXT *pagedit;
	extern void add_item();
	extern int new_save_context();
	
	XmString pagname;
	char filename[MAXLENFILENAME],file1[MAXLENFILENAME],file2[MAXLENFILENAME];
	char *strpagname;
	char pag_inlist[MAXCHAR_PAGNAME],comando[200];
	char appo[100];
	int i;
	Boolean pathSpec=False;
	Boolean tipo_regolazione = FALSE;
	char *nomePag;
	Widget text;
	char * pagina;
	int num=0;
	PAGINA *pag;			/* CAPPE */
	
	/* IMPORT PAG/LIB devo gestire i casi
	   il file indicato deve essere esistente.
	   puo' essere nella directory LEGOMMI_PAG/LIB corrente oppure in una altra
	   se e' non e' nella LEGOMMI_PAG/LIB ce lo copio dentro se non c'e' n'e'
	      gia' uno con lo stesso nome
	   non puo' essere gia incluso nel Context corrente 
	*/   
	
	/* recupero il children dove c'e' il nome della pagina
	   e carico il suo contenuoto in strpagname */ 
	text = XmSelectionBoxGetChild(UxWidget,XmDIALOG_TEXT);
	get_something(text,XmNvalue, (void*) &pagina);
	
	while((strpagname = estrai_campo(pagina,num))!=NULL)
	{
	++num;
	switch(tip_operaz)
	{
	   case IMPORT_PAGE:
	
	      if( XlGetenv(ENVPAG) == NULL)
	         return;
	
	      /* verifica se il nome del file contiene la path */ 
	      nomePag = extractFileNameNExt(strpagname);
	
	      if( strcmp(strpagname,nomePag) )
	          pathSpec = True;
	  
	      /* se e' gia presente nel context non termino la operazione */
	      for(i=0;i<pagedit->pag_num;i++)
	      {
	         if(!strcmp(pagedit->page_list[i],nomePag))
	         {
		    char messaggio[50];
		    sprintf(messaggio,"Page %s Already Exist",nomePag);
	/*
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",messaggio,NULL,False,NULL,False,NULL); 
	*/
	            break;
	         }
	      }
	      if(i!=pagedit->pag_num)
		break;
	
	/*   verifico se la path e' LEGOMMI_PAG */
	
	      if(pathSpec)
	      {
	         strcpy(appo,strpagname);
	         for(i=(strlen(appo)-1);i>=0;i--)
	              if( appo[i] == '/')
	                 appo[i] = 0;
	                 
	          if( !strcmp( XlGetenv(ENVPAG),appo ) )
	             pathSpec = False;
	
	          printf("appo = %s\n",appo);
	          printf("strpagname = %s\n",strpagname);
	          printf("envpag = %s\n",XlGetenv(ENVPAG));
	      }
	
	      /* viene richiesto l'import da LEGOMMI_PAG */
	      if(!pathSpec)
	      {
	        /* viene richiesto l'import da LEGOMMI_PAG */
	         strcpy(filename,XlGetenv(ENVPAG) );
	         strcat(filename,"/"); 
	         strcat(filename,nomePag);
	         strcat(filename,PAG_EXT);
	         strcpy(filename,XlConvPathVms(filename) );
	
	         if( !file_exist(filename) )
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); 
	            break;
	         }
	         
	      }
	      else
	      {
	         /* viene richiesto l'import da una altra directory */
	         /* se per caso e' la stessa il file viene copiato ugualmente */
	
	         strcpy(filename,strpagname); 
	         strcat(filename,PAG_EXT);
	         strcpy(filename,XlConvPathVms(filename) );
	         
	         if( !file_exist(filename) ) 
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Page Not Exist",NULL,False,NULL,False,NULL); 
	            break;
	         }
	
	
	            strcpy( file1, filename);
	            strcpy( file2, XlGetenv(ENVPAG) );
	            strcat( file2,"/");
	            strcat( file2,nomePag);
	            strcat( file2,PAG_EXT);
	            strcpy( file2, XlConvPathVms(file2) );
	
	            XlCopy(file1,file2);
	
	            strcpy(file1,strpagname);
	            strcat(file1,BKG_EXT);
	            strcpy(file2,XlGetenv(ENVPAG));
	            strcat(file2,"/");
	            strcat(file2,nomePag);
	            strcat(file2,BKG_EXT);
	
	            strcpy(file2,XlConvPathVms(file2));
	            XlCopy(file1,file2);
	
	
	      }
	
		if(strcmp(TIPO_REGOLAZIONE,GetTypeFromName(nomePag))==0)
			tipo_regolazione=True;
		else
			tipo_regolazione=False;
		if(tipo_regolazione == True)
			{
			printf("Importata pagina di regolazione\n");
			
			SettaTagInPag(nomePag,pagedit->nextTagPag);
			TagPagSetNew(pagedit,TYPE_REGOLAZIONE);
			}
				
	  
	      /* se e' gia presente nel context non termino la operazione */
	
	
	      /* CAPPE. Aggiornamento del context. Apertura del file .pag della pagina da importare
		 per recuperare le risorse da inserire nel context. */
	
	      if( pagina_init(nomePag,&pag,TIPO_PAGINA) == True )
	      {
		 printf("Nome del file=%s\n",filename);
	         
	         if( (pag->db = XrmGetFileDatabase(filename)) == NULL )
	            {
	               char messaggio[100];
		       sprintf(messaggio,"Warning XrmGetFileDatabase Failed on page\n%s",filename);
	               SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo",messaggio,NULL,False,NULL,False,NULL);
	               return;
	            }
	           
	         GetResPaginaDb(pag,pag->db,True);
	      /* allocazione delle strutture per l'aggiornamento del context */      
	         if( ((pagedit->page_list = realloc(pagedit->page_list,sizeof(char *)*(pagedit->pag_num+1))) == NULL) || 
	             ((pagedit->res_page = realloc(pagedit->res_page, sizeof(INFO_PAGE *)*(pagedit->pag_num+1))) == NULL))
	         {
	            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	            return;
	         }
	         if( ((pagedit->page_list[pagedit->pag_num] = alloca_memoria(strlen(nomePag)+1,sizeof(char) )) == NULL) ||
	             ((pagedit->res_page[pagedit->pag_num] = (INFO_PAGE *)alloca_memoria(1,sizeof(INFO_PAGE))) == NULL))
	         {
	            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	            return;
	         }
	
	         /* Aggiornamento del context */
	         strcpy(pagedit->page_list[pagedit->pag_num],nomePag);
	         strcpy(pagedit->res_page[pagedit->pag_num]->descr_page, pag->geom.descriz);
	         strcpy(pagedit->res_page[pagedit->pag_num]->type_page, pag->geom.tipo);
	         pagedit->res_page[pagedit->pag_num]->refresh_freq = pag->refreshFreq;
	         strcpy(pagedit->res_page[pagedit->pag_num]->hier, pag->gerarchia);
	         pagedit->res_page[pagedit->pag_num]->in_use = pag->in_use;
	         strcpy(pagedit->res_page[pagedit->pag_num]->tagPag,pag->tagPag);
	
	         pagedit->pag_num++;
	
		 SetItemString(&pagname,pag->nomepag,pag->geom.tipo,pag->geom.descriz,
			       pag->in_use,pag->tagPag);   
	         add_item(pagname,0);         
	         pagina_free(pag);
	      }
	      else
	         printf("\n\n\tERRORE INIZIALIZZAZIONE PAGINA %s\n",nomePag);
	   break;
	   case IMPORT_LIB:
	
	      if( XlGetenv(ENVLIB) == NULL)
	         return;
	
	      /* verifica se il nome del file contiene la path */ 
	      nomePag = extractFileNameNExt(strpagname);
	
	      if( strcmp(strpagname,nomePag) )
	          pathSpec = True;
	
	      for(i=0;i<pagedit->iconlib_num;i++)
	      {
	         if(!strcmp(pagedit->iconlib_list[i],nomePag))
	         {
		    char messaggio[50];
		    sprintf(messaggio,"Library %s Already Exist",nomePag);
	             
	            break;
	         }
	      }
	      if(i != pagedit->iconlib_num)
		 break;
	
	/*   verifico se la path e' LEGOMMI_PAG */
	
	      if(pathSpec)
	      {
	         strcpy(appo,strpagname);
	         for(i=(strlen(appo)-1);i>=0;i--)
	              if( appo[i] == '/')
	                 appo[i] = 0;
	                 
	          if(!strcmp(XlGetenv(ENVLIB),appo))
	             pathSpec = False;
	
	          printf("appo = %s\n",appo);
	          printf("strpagname = %s\n",strpagname);
	          printf("envpag = %s\n",XlGetenv(ENVLIB));
	      }
	
	      /* viene richiesto l'import da LEGOMMI_LIB */
	      if(!pathSpec)
	      {
	        /* viene richiesto l'import da LEGOMMI_LIB */
	         strcpy(filename,XlGetenv(ENVLIB));
	         strcat(filename,"/"); 
	         strcat(filename,nomePag);
	         strcat(filename,LIB_EXT);
	         strcpy(filename,XlConvPathVms(filename));
	
	         if( !file_exist(filename) )
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Library Not Exist",NULL,False,NULL,False,NULL); 
	            break;
	         }
	         
	      }
	      else
	      {
	         /* viene richiesto l'import da una altra directory */
	         /* se per caso e' la stessa il file viene copiato ugualmente */
	
	         strcpy(filename,strpagname); 
	         strcat(filename,LIB_EXT);
	         strcpy(filename,XlConvPathVms(filename));
	
	         if( !file_exist(filename))
	         {
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo","Library Not Exist",NULL,False,NULL,False,NULL); 
	            break;
	         }
	     
	         if( system(NULL) == 0)
	         {
	            SetMsg(topLevelShell,NULL,ERRMSG,"ConfigError","Unix Command Shell Not Available",NULL,False,NULL,False,NULL); 
	            return;
	         }
	         else
	         {
	
	            strcpy(file1,filename);
	            strcpy(file2,XlGetenv(ENVLIB));
	            strcat(file2,"/");
	            strcat(file2,nomePag);
	            strcat(file2,LIB_EXT);
	            strcpy(file2,XlConvPathVms(file2));
	
	            XlCopy(file1,file2);
	          }
	      }
	  
	      /* se e' gia presente nel context non termino la operazione */
	/**
	      for(i=0;i<pagedit->iconlib_num;i++)
	      {
	         if(!strcmp(pagedit->iconlib_list[i],nomePag))
	         {
		    char messaggio[50];
		    sprintf(messaggio,"Library %s Already Exist",nomePag);
	            SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",messaggio,NULL,False,NULL,False,NULL); 
	            break;
	         }
	      }
	      if(i != pagedit->iconlib_num)
		 break;
	*/
	
	      /* aggiorno la lista delle librerie */      
	      if( (pagedit->iconlib_list = realloc(pagedit->iconlib_list,sizeof(char *)*(pagedit->iconlib_num+1))) == NULL)
	      {
	         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	         return;
	      }
	      if( (pagedit->iconlib_list[pagedit->iconlib_num] = alloca_memoria(strlen(nomePag)+1,sizeof(char) )) == NULL)
	      {
	         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	         return;
	      }
	      /* aggiorno la lista delle label librerie */      
	      if( (pagedit->iconlib_label = realloc(pagedit->iconlib_label,sizeof(char *)*(pagedit->iconlib_num+1))) == NULL)
	      {
	         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	         return;
	      }
	      if( (pagedit->iconlib_label[pagedit->iconlib_num] = alloca_memoria(strlen(nomePag)+1,sizeof(char) )) == NULL)
	      {
	         SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo","Error Allocating Memory",NULL,False,NULL,False,NULL); 
	         return;
	      }
	
	      /* CAPPE Apertura della pagina da importare per il recupero delle informazioni da
	         visualizzare nella scrolled list e per evitare la chiamata alla display_resources */
	      if( pagina_init(nomePag,&pag,TIPO_LIBRARY) == True )
	      {
	         if( (pag->db = XrmGetFileDatabase(filename)) == NULL )
	            {
	               char messaggio[100];
		       sprintf(messaggio,"Warning XrmGetFileDatabase Failed on page\n%s",filename);
	               SetMsg(topLevelShell,NULL,ERRMSG,"ConfigInfo",messaggio,NULL,False,NULL,False,NULL);
	               return;
	            }           
	         GetResPaginaDb(pag,pag->db,True);
	      }
	      else
		 printf("\n\n\tERRORE INIZIALIZZAZIONE LIBRERIA %s\n",nomePag);
	
	      strcpy(pagedit->iconlib_list[pagedit->iconlib_num],nomePag);
	      strcpy(pagedit->iconlib_label[pagedit->iconlib_num],nomePag);
	      pagedit->iconlib_num++;
	
	      SetItemString(&pagname,pag->nomepag,pag->geom.tipo,pag->geom.descriz,pag->in_use,pag->tagPag);
	      add_library_item(pagname,0);
	      pagina_free(pag);         
	
	   break;
	}
	
	} 
	
	if( new_save_context(pagedit) != True)
	   show_message("Error on Save Context.\n");
	/*
	ContextModified = True;
	*/
	/* display_resources(pagedit); CAPPE */
	UxDestroyInterface(UxWidget);
	
	}
	UxPageSelectionContext = UxSaveCtx;
}

static	void	cancelCB_PageSelection( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCPageSelection       *UxSaveCtx, *UxContext;

	UxSaveCtx = UxPageSelectionContext;
	UxPageSelectionContext = UxContext =
			(_UxCPageSelection *) UxGetContext( UxWidget );
	{
	/*
	UxDestroyInterface(PageSelection);
	*/
	UxDestroyInterface(UxWidget);
	}
	UxPageSelectionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_PageSelection()
{
	Widget		_UxParent;


	/* Creation of PageSelection */
	_UxParent = XtVaCreatePopupShell( "PageSelection_shell",
			topLevelShellWidgetClass, UxTopLevel,
			XmNx, 700,
			XmNy, 320,
			XmNwidth, 330,
			XmNheight, 350,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "PageSelection",
			XmNiconName, "PageSelection",
			NULL );

	PageSelection = XtVaCreateManagedWidget( "PageSelection",
			xmFileSelectionBoxWidgetClass,
			_UxParent,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNunitType, XmPIXELS,
			XmNwidth, 330,
			XmNheight, 350,
			RES_CONVERT( XmNpattern, "*.lib" ),
			NULL );
	XtAddCallback( PageSelection, XmNokCallback,
		(XtCallbackProc) okCallback_PageSelection,
		(XtPointer) UxPageSelectionContext );
	XtAddCallback( PageSelection, XmNcancelCallback,
		(XtCallbackProc) cancelCB_PageSelection,
		(XtPointer) UxPageSelectionContext );

	UxPutContext( PageSelection, (char *) UxPageSelectionContext );


	XtAddCallback( PageSelection, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxPageSelectionContext);


	return ( PageSelection );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_PageSelection( _Uxope )
	int	_Uxope;
{
	Widget                  rtrn;
	_UxCPageSelection       *UxContext;

	UxPageSelectionContext = UxContext =
		(_UxCPageSelection *) UxNewContext( sizeof(_UxCPageSelection), False );

	ope = _Uxope;

	rtrn = _Uxbuild_PageSelection();

	{
	Widget childF;
	XmString appo;
	Widget ListaFile;
	Widget ListaSelezionati;
        XmString EnvString;
	
	
	ListaFile=XmFileSelectionBoxGetChild(PageSelection,XmDIALOG_LIST);
	ListaSelezionati=XmFileSelectionBoxGetChild(PageSelection,XmDIALOG_TEXT);
	set_something(ListaFile,XmNselectionPolicy,(void*) XmEXTENDED_SELECT);
	XtAddCallback(ListaFile,XmNextendedSelectionCallback,SelezioneMultipla,
			    ListaSelezionati);
	
	if(ope == IMPORT_PAGE)
	{ 
	   if(getenv(ENVPAG) != NULL)
           {
              EnvString = XmStringCreateSimple(getenv(ENVPAG));
   	      set_something(PageSelection,XmNdirSpec,(void*) EnvString);
           }	
	    
	
	   appo = XmStringCreateSimple("*.pag");
	   set_something(PageSelection,XmNpattern,(void*) appo);
	   set_something(PageSelection,XmNdirSpec,(void*) appo);
	   
	
	 
	}
	else if(ope == IMPORT_LIB)
	{
	   if(getenv(ENVLIB) != NULL)
           {
              EnvString = XmStringCreateSimple(getenv(ENVLIB));
   	      set_something(PageSelection,XmNdirSpec,(void*) EnvString);
           }	
	   appo = XmStringCreateSimple("*.lib");
	   set_something(PageSelection,XmNpattern,(void*) appo);
	   set_something(PageSelection,XmNdirSpec,(void*) appo);
	}
	
	tip_operaz = ope;
	return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

