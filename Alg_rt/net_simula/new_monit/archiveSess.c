
/*******************************************************************************
       archiveSess.c
       (Generated from interface file archiveSess.i)
*******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#endif /* MOTIF */

#include "UxXt.h"
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#include <sys/stat.h>
#include <sys/types.h>
#include "sim_param.h"
#include "sim_types.h"
#include "comandi.h"
#include "sked.h"
#include "refresh.h"
#include "f22_circ.h"


#define MAX_NAME_DIR	256
#define SESS_STAT_WARN	"WARN_F22"
#define SESS_STAT_OK	"-OK_F22-"

typedef struct {
                int  position;
                char status[20];
               } POS_SESS;


extern int 	_MAX_BACK_TRACK;
extern VAL 	val;

int recoveryInfoSession();
int createDirSession();
int recoveryRangeF22(float *,float *);
int liberaAllocDinam();
int load_stato_cr(STATO_CR *);
int	create_attenzioneDialog( Widget, unsigned char	*, int );
//int UxGetIfClassCode( Widget  );


static	int _UxIfClassId;
#ifndef XKLOADDS
#define XKLOADDS
#endif /* XKLOADDS */

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
	Widget	UxarchiveSess;
	Widget	UxlabelTitle;
	Widget	UxscrolledWindowSess;
	Widget	UxscrolledListSess;
	Widget	UxseparSess;
	Widget	UxlabelDescr;
	Widget	UxtextField1;
	Widget	UxseparOkCanc;
	Widget	UxpushButOk;
	Widget	UxpushButCanc;
	XmString	*Uxlist_sess;
	BKTAB	*Uxbk_sess_head;
	POS_SESS	*Uxpos_sess_bktab;
	char	Uxnome_dir_sess[256];
	swidget	UxUxParent;
} _UxCarchiveSess;

static _UxCarchiveSess         *UxArchiveSessContext;
#define archiveSess             UxArchiveSessContext->UxarchiveSess
#define labelTitle              UxArchiveSessContext->UxlabelTitle
#define scrolledWindowSess      UxArchiveSessContext->UxscrolledWindowSess
#define scrolledListSess        UxArchiveSessContext->UxscrolledListSess
#define separSess               UxArchiveSessContext->UxseparSess
#define labelDescr              UxArchiveSessContext->UxlabelDescr
#define textField1              UxArchiveSessContext->UxtextField1
#define separOkCanc             UxArchiveSessContext->UxseparOkCanc
#define pushButOk               UxArchiveSessContext->UxpushButOk
#define pushButCanc             UxArchiveSessContext->UxpushButCanc
#define list_sess               UxArchiveSessContext->Uxlist_sess
#define bk_sess_head            UxArchiveSessContext->Uxbk_sess_head
#define pos_sess_bktab          UxArchiveSessContext->Uxpos_sess_bktab
#define nome_dir_sess           UxArchiveSessContext->Uxnome_dir_sess
#define UxParent                UxArchiveSessContext->UxUxParent



/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_archiveSess();

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*******************************************************************/
/*                                                                 */
/*   Funzione che recupera dal file backtrack.dat il tempo di      */
/*   inizio e la descrizione delle sessioni presenti. Carica       */
/*   inoltre queste informazioni nell'interfaccia 'archiveSess'.   */
/*                                                                 */
/*******************************************************************/
int recoveryInfoSession()
{
FILE		*fp_bktk = NULL;
int		tot_session = 0;
int		i,j,k,last_backtrack,ind_list;
STATO_CR	stato_cr;
char		str_sess[300];
int		ore,minuti,secondi,giorno,mese,anno;
float		time_st_f22,time_end_f22,time_st_sess;

   pos_sess_bktab=NULL;
   bk_sess_head=NULL;
   list_sess=NULL;

   if((fp_bktk=fopen("backtrack.dat", "r"))==NULL) {
      fprintf(stderr,"Backtrack file not existent\n");
      return(-1);
   }
   else {
      pos_sess_bktab=(POS_SESS *)XtCalloc(_MAX_BACK_TRACK,sizeof(POS_SESS));
      if(pos_sess_bktab==NULL) {
         fprintf(stderr,"Error in XtCalloc: pos_sess_bktab\n");
         return(-1);
      }
      bk_sess_head=(BKTAB*)XtCalloc(_MAX_BACK_TRACK,sizeof(BKTAB));
      if(bk_sess_head==NULL) {
         fprintf(stderr,"Error in XtCalloc:bk_sess_head\n");
         XtFree((char*)pos_sess_bktab);
         return(-1);
      }
      list_sess=(XmString *)XtCalloc(_MAX_BACK_TRACK,sizeof(XmString));
      if(list_sess==NULL) {
        fprintf(stderr,"Error in XtCalloc: list_sess\n");
        XtFree((char*)pos_sess_bktab);
        XtFree((char*)bk_sess_head);
        return(-1);
      }  
         
      fseek(fp_bktk,sizeof(HEADER_REGISTRAZIONI),SEEK_SET);
/*
printf("*********************************************\n");
printf("Stato dei backtrack presenti in backtrack.dat\n");
printf("*********************************************\n");
printf("NUM\tSTAT\tFORZ\tDESCR\t\tTEMPO\n");
*/
      for(i=1;i<=_MAX_BACK_TRACK;i++) {
         fread(&bk_sess_head[i-1],sizeof(BKTAB),1,fp_bktk);
/*
printf(" %d\t%d\t%d\t%s\t%f\n",
       i,bk_sess_head[i-1].stat,bk_sess_head[i-1].forzato,
       bk_sess_head[i-1].descr,bk_sess_head[i-1].tempo);
*/
      }
/*
printf("*********************************************\n");
*/
      load_stato_cr(&stato_cr);
/*
   Recupero dei tempi del primo e dell'ultimo campione presenti nel
   file f22circ.dat corrente
*/
      recoveryRangeF22(&time_st_f22,&time_end_f22);
/*
printf("time_st_f22 = %f\ttime_end_f22 = %f\n",time_st_f22,time_end_f22);
*/
/*      
   Ciclo su tutti i backtrack presenti. Si estraggono quelli che
   corrispondono all'inizio di una sessione e si caricano tempo e
   descrizione nell'interfaccia.
*/
      last_backtrack=stato_cr.last_bktk_save;
      k=last_backtrack+1;
      if(k>_MAX_BACK_TRACK)
         k=1;
      ind_list=0;
/*
printf("**********************************************************\n");
printf("Elenco dei backtrack che sono inizio di una nuova sessione\n");
printf("**********************************************************\n");
printf("NUM\tTIME\tDESCRIPTION\n");
*/
      for(j=0,k=last_backtrack+1;j<_MAX_BACK_TRACK;k++,j++) {
         if(k>_MAX_BACK_TRACK)
            k=1;
         if((bk_sess_head[k-1].stat==1)&&(bk_sess_head[k-1].forzato!=0)) {
            /* Il backtrack corrente e'inizio di una sessione */
            pos_sess_bktab[ind_list].position=k;
/*
printf("pos_sess_bktab[%d] = %d\n",ind_list,pos_sess_bktab[ind_list].position); 
*/
            ore=0;
            minuti=0;
            secondi=0;
            giorno=1;
            mese=1;
            anno=2000;
            converti_tempo((bk_sess_head[k-1].tempo/1000.0),(long int*)&ore,(long int*)&minuti,(long int*)&secondi,(long int*)&giorno,
                           (long int*)&mese,(long int*)&anno);
            time_st_sess=(bk_sess_head[k-1].tempo)/1000.0;
/*
printf("time_st_sess= %f\n", time_st_sess); 
*/
            if(time_st_sess<time_st_f22) {
               sprintf(str_sess,"%2d:%2d:%2d   -%s-   %s",ore,minuti,secondi,
                       SESS_STAT_WARN,bk_sess_head[k-1].descr);
               strcpy(pos_sess_bktab[ind_list].status,SESS_STAT_WARN);
            }
            else {
               sprintf(str_sess,"%2d:%2d:%2d   -%s-   %s",ore,minuti,secondi,
                       SESS_STAT_OK,bk_sess_head[k-1].descr);
               strcpy(pos_sess_bktab[ind_list].status,SESS_STAT_OK);
            } 
/* 
printf(" %d\t%2d:%2d:%2d\t%s\n", pos_sess_bktab[ind_list].position,ore,minuti,
       secondi,bk_sess_head[k-1].descr);
*/
            list_sess[ind_list]=XmStringCreateSimple(str_sess);
            ind_list++;
         }         
      }
/*
printf("**********************************************************\n");
*/      
      XtVaSetValues(scrolledListSess,XmNitems,list_sess,XmNitemCount,
                    ind_list,XmNvisibleItemCount,5,NULL);
      fclose(fp_bktk);
   }
return(0);           
}




/*******************************************************************/
/*                                                                 */
/*   Funzione che crea la directory di archiviazione dei files     */
/*   relativi alla sessione selezionata e il file README relativo. */
/*                                                                 */
/*******************************************************************/ 
int createDirSession(descr_session,sess_state)
char	*descr_session;
char	*sess_state;
{

FILE	*fp_app,*fp_readme;
int	out_sys;
char	*path_session;
char	path_tmp[FILENAME_MAX],pid_curr[10];
char	comando[FILENAME_MAX+20];
char	warn_readme[200];


printf("**************************\n");
printf("Lancio della shell archive\n");
printf("**************************\n");

   strcpy(path_tmp,getenv("HOME"));
   strcat(path_tmp,"/tmp");
   strcat(path_tmp,"/app_arch_");
   sprintf(pid_curr,"%d",getpid());
   strcat(path_tmp,pid_curr);

   strcpy(comando,"archive > ");
   strcat(comando,path_tmp);
      
   out_sys=system(comando);
   if(out_sys==-1) {
      perror("esecuzione della shell: archive");
      return(-1);
   }
   /* La shell e' stata eseguita correttamente. Recupero del nome della
      directory.                                                        */
   if((fp_app=fopen(path_tmp,"r")) == NULL) {
      perror("apertura del file: app_archive");
      return(-1);
   }
   fgets(nome_dir_sess,MAX_NAME_DIR,fp_app);
   nome_dir_sess[strlen(nome_dir_sess)-1]='\0';  /* si toglie il new line */
   fclose(fp_app);
   strcpy(comando,"rm ");
   strcat(comando,path_tmp);
   out_sys=system(comando);
   if(out_sys==-1) {
      perror("rimozione del file app_archive");
      return(-1);
   }
   /* Creazione della directory di archiviazione */
   path_session=(char *)getcwd((char *) NULL, FILENAME_MAX+1);
   strcat(path_session,"/");
   strcat(path_session,nome_dir_sess);
   if(mkdir(path_session,S_IRWXU|S_IRGRP|S_IROTH)) {
      perror("creazione directory di archiviazione");
      return(-1);
   }
   /* Creazione del file 'README' nella directory appena creata. */
   strcat(path_session,"/README");
   if((strcmp(descr_session,""))==0) {
      printf("WARNING!!! Descrizione sessione assente.\n");
      strcpy(descr_session,"No description");
   }
   if((fp_readme=fopen(path_session,"w")) == NULL) {
      perror("apertura del file: README");
      return(-1);
   }   
   strcat(descr_session,"\n");
   fwrite(descr_session,sizeof(char),(strlen(descr_session)),fp_readme);
   if(strcmp(sess_state,SESS_STAT_WARN)==0) {
      strcpy(warn_readme,"WARNING!!! Non allineati i tempi nei files backtrack.dat e f22circ.dat\n");
      fwrite(warn_readme,sizeof(char),(strlen(warn_readme)),fp_readme);
   }
   fclose(fp_readme);

return(0);

}

/*******************************************************************/
/*                                                                 */
/*   Funzione che recupera i tempi del primo e dell'ultimo         */
/*   presenti nel file f22circ.dat corrente                        */
/*                                                                 */
/*******************************************************************/
int recoveryRangeF22(first_time,last_time)
float *first_time;
float *last_time;
{
int campione,offset;
PUNT_FILE_F22 file_f22;
F22CIRC_HD header;
SIMULATOR simpar;
F22CIRC_T t_22;
int size_campione;
int offheader;


   if(f22_open_file("f22circ", &file_f22)==0) {
      fprintf(stderr,"Errore apertura file f22circ.dat\n");
      return(-1);
   }
   if(f22_leggo_header(&header,file_f22,0,&simpar)==0) {
     fprintf(stderr,"Errore lettura header file f22circ.dat\n"); 
     return(-1);
   }
   size_campione = (header.num_var_graf ) *sizeof(float) + sizeof(F22CIRC_T);
   offheader = sizeof(HEADER_REGISTRAZIONI) + sizeof(F22CIRC_HD) +
               sizeof(F22CIRC_VAR) * header.num_var_graf;
/*
   Recupero del tempo dell'ultimo campione
*/
   campione=header.p_fine;
   offset=offheader + (campione-1)*size_campione;
   fseek(file_f22.fp,offset,SEEK_SET);
   fread(&t_22,sizeof(F22CIRC_T),1,file_f22.fp);
   *last_time=t_22.tempo;
/*
   Recupero del tempo del primo campione
*/
   campione=header.p_iniz;
   offset=offheader + (campione-1)*size_campione;
   fseek(file_f22.fp,offset,SEEK_SET);
   fread(&t_22,sizeof(F22CIRC_T),1,file_f22.fp);
   *first_time=t_22.tempo;
 
   f22_close_file(file_f22);

}



/*******************************************************************/
/*                                                                 */
/*   Funzione che libera le strutture allocate dinamicamente       */
/*                                                                 */
/*******************************************************************/
int liberaAllocDinam()
{
int	i;

   for(i=0;i<_MAX_BACK_TRACK;i++) {
      if(list_sess[i]!=NULL) {
         XmStringFree(list_sess[i]);
      }
   }
   if(list_sess!=NULL) {
      XtFree((char*)list_sess);
   }
   if(bk_sess_head!=NULL) {
      XtFree((char*)bk_sess_head);
   }
   if(pos_sess_bktab!=NULL) {
      XtFree((char*)pos_sess_bktab);
   }

}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void  activateCB_pushButOk( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSess         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSessContext;
	UxArchiveSessContext = UxContext =
			(_UxCarchiveSess *) UxGetContext( UxWidget );
	{
	int	i,j,out_sys;
	int	*pos_list,pos_cnt;
	int	pos_first_bk,pos_last_bk,pos_sel_last;
	char    *descr_str;
	char	comando[256],stat_first_bk[20];
	float	time_start_sess,time_end_sess;
	
	   if(XmListGetSelectedPos(scrolledListSess,&pos_list,&pos_cnt)==True) {
	      /* Almeno una sessione e' stata selezionata */
	      UxDestroyInterface(UxThisWidget);
	      pos_first_bk=(pos_sess_bktab[(pos_list[0]-1)].position);
	      strcpy(stat_first_bk,(pos_sess_bktab[(pos_list[0]-1)].status));
	      time_start_sess=(bk_sess_head[pos_first_bk-1].tempo)/1000.0;
	      pos_sel_last=pos_list[(pos_cnt-1)];
	      pos_last_bk=(pos_sess_bktab[pos_sel_last].position);
	      if(pos_sel_last<_MAX_BACK_TRACK) {
	         if((pos_sess_bktab[pos_sel_last].position)!=0) {
	            time_end_sess=(bk_sess_head[pos_last_bk-1].tempo)/1000.0;
	         }
	         else {
	            time_end_sess=val.actual.tempo_sim;
	         }      
	      }
	      else {
	         time_end_sess=val.actual.tempo_sim;
	      }
	
	printf("*******************************\n");
	printf("Intervallo di tempo selezionato\n");
	printf("*******************************\n");
	printf("TEMPO INIZIALE (sec) -----> %f\n",time_start_sess);
	printf("TEMPO FINALE (sec)   -----> %f\n",time_end_sess);
	  
	      XtFree((char*)pos_list);
	
	      descr_str=XmTextFieldGetString(textField1);
	      if(createDirSession(descr_str,stat_first_bk)==0) {
	         /* E' stata predisposta la directory che deve contenere i files relativi
	            alla sessione selezionata. Lancio del programma nsession.              */ 
	         sprintf(comando,"nsession %f %f 5 %s",time_start_sess,time_end_sess,nome_dir_sess);
	
	printf("*********************************\n");
	printf("Comando per il lancio di nsession\n");
	printf("*********************************\n");
	printf("\t%s\n",comando);
	
	         out_sys=system(comando);
	         if(out_sys==-1) {
	            perror("esecuzione di: nsession");
	         }
	      }
	      liberaAllocDinam();
	   }
	   else {
	      /* Nessuna sessione e' stata selezionata */
	      create_attenzioneDialog(archiveSess,"No selection!!!",XmDIALOG_MESSAGE);
	   }
	
	}
	UxArchiveSessContext = UxSaveCtx;
}

static void  activateCB_pushButCanc( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCarchiveSess         *UxSaveCtx, *UxContext;

	UxSaveCtx = UxArchiveSessContext;
	UxArchiveSessContext = UxContext =
			(_UxCarchiveSess *) UxGetContext( UxWidget );
	{
	   liberaAllocDinam();
	   UxDestroyInterface(UxThisWidget);
	}
	UxArchiveSessContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_archiveSess()
{
	Widget		_UxParent;


	/* Creation of archiveSess */
	_UxParent = UxParent;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	_UxParent = XtVaCreatePopupShell( "archiveSess_shell",
			xmDialogShellWidgetClass, _UxParent,
			XmNx, 500,
			XmNy, 640,
			XmNwidth, 420,
			XmNheight, 460,
			XmNshellUnitType, XmPIXELS,
			XmNtitle, "archiveSess",
			NULL );

	archiveSess = XtVaCreateWidget( "archiveSess",
			xmFormWidgetClass,
			_UxParent,
			XmNwidth, 420,
			XmNheight, 460,
			XmNunitType, XmPIXELS,
			XmNautoUnmanage, FALSE,
			NULL );
	UxPutContext( archiveSess, (char *) UxArchiveSessContext );


	/* Creation of labelTitle */
	labelTitle = XtVaCreateManagedWidget( "labelTitle",
			xmLabelWidgetClass,
			archiveSess,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 460,
			XmNheight, 25,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			RES_CONVERT( XmNlabelString, "Selected session" ),
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftOffset, 20,
			XmNrightOffset, 20,
			XmNtopOffset, 10,
			XmNtopAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( labelTitle, (char *) UxArchiveSessContext );


	/* Creation of scrolledWindowSess */
	scrolledWindowSess = XtVaCreateManagedWidget( "scrolledWindowSess",
			xmScrolledWindowWidgetClass,
			archiveSess,
			XmNscrollingPolicy, XmAPPLICATION_DEFINED,
			XmNvisualPolicy, XmVARIABLE,
			XmNscrollBarDisplayPolicy, XmSTATIC,
			XmNshadowThickness, 0,
			XmNx, 20,
			XmNy, 40,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopOffset, 40,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 180,
			XmNtopAttachment, XmATTACH_FORM,
			XmNheight, 250,
			NULL );
	UxPutContext( scrolledWindowSess, (char *) UxArchiveSessContext );


	/* Creation of scrolledListSess */
	scrolledListSess = XtVaCreateManagedWidget( "scrolledListSess",
			xmListWidgetClass,
			scrolledWindowSess,
			XmNwidth, 410,
			XmNheight, 250,
			XmNselectionPolicy, XmEXTENDED_SELECT,
			XmNautomaticSelection, TRUE,
			XmNscrollBarDisplayPolicy, XmAS_NEEDED,
			XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE,
			NULL );
	UxPutContext( scrolledListSess, (char *) UxArchiveSessContext );


	/* Creation of separSess */
	separSess = XtVaCreateManagedWidget( "separSess",
			xmSeparatorWidgetClass,
			archiveSess,
			XmNwidth, 460,
			XmNheight, 20,
			XmNx, 0,
			XmNy, 420,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNtopWidget, scrolledWindowSess,
			NULL );
	UxPutContext( separSess, (char *) UxArchiveSessContext );


	/* Creation of labelDescr */
	labelDescr = XtVaCreateManagedWidget( "labelDescr",
			xmLabelWidgetClass,
			archiveSess,
			XmNx, 0,
			XmNy, 430,
			XmNwidth, 460,
			XmNheight, 25,
			RES_CONVERT( XmNlabelString, "Selected session description" ),
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 0,
			XmNtopWidget, separSess,
			XmNalignment, XmALIGNMENT_BEGINNING,
			XmNleftOffset, 20,
			XmNrightOffset, 20,
			NULL );
	UxPutContext( labelDescr, (char *) UxArchiveSessContext );


	/* Creation of textField1 */
	textField1 = XtVaCreateManagedWidget( "textField1",
			xmTextFieldWidgetClass,
			archiveSess,
			XmNwidth, 390,
			XmNx, 40,
			XmNy, 470,
			XmNheight, 30,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 5,
			XmNtopWidget, labelDescr,
			XmNleftOffset, 20,
			XmNmarginHeight, 2,
			NULL );
	UxPutContext( textField1, (char *) UxArchiveSessContext );


	/* Creation of separOkCanc */
	separOkCanc = XtVaCreateManagedWidget( "separOkCanc",
			xmSeparatorWidgetClass,
			archiveSess,
			XmNwidth, 460,
			XmNheight, 20,
			XmNx, 10,
			XmNy, 480,
			XmNleftOffset, 0,
			XmNrightAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNtopWidget, textField1,
			XmNleftAttachment, XmATTACH_FORM,
			NULL );
	UxPutContext( separOkCanc, (char *) UxArchiveSessContext );


	/* Creation of pushButOk */
	pushButOk = XtVaCreateManagedWidget( "pushButOk",
			xmPushButtonWidgetClass,
			archiveSess,
			XmNx, 40,
			XmNy, 510,
			XmNwidth, 120,
			XmNheight, 35,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNtopWidget, separOkCanc,
			RES_CONVERT( XmNlabelString, "Ok" ),
			XmNdefaultButtonShadowThickness, 0,
			NULL );
	XtAddCallback( pushButOk, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButOk,
		(XtPointer) UxArchiveSessContext );

	UxPutContext( pushButOk, (char *) UxArchiveSessContext );


	/* Creation of pushButCanc */
	pushButCanc = XtVaCreateManagedWidget( "pushButCanc",
			xmPushButtonWidgetClass,
			archiveSess,
			XmNx, 210,
			XmNy, 500,
			XmNwidth, 120,
			XmNheight, 35,
			RES_CONVERT( XmNlabelString, "Cancel" ),
			XmNleftAttachment, XmATTACH_WIDGET,
			XmNleftOffset, 60,
			XmNleftWidget, pushButOk,
			XmNtopAttachment, XmATTACH_WIDGET,
			XmNtopOffset, 10,
			XmNtopWidget, separOkCanc,
			NULL );
	XtAddCallback( pushButCanc, XmNactivateCallback,
		(XtCallbackProc) activateCB_pushButCanc,
		(XtPointer) UxArchiveSessContext );

	UxPutContext( pushButCanc, (char *) UxArchiveSessContext );


	XtAddCallback( archiveSess, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxArchiveSessContext);


	return ( archiveSess );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_archiveSess( _UxUxParent )
	swidget	_UxUxParent;
{
	Widget                  rtrn;
	_UxCarchiveSess         *UxContext;
	static int		_Uxinit = 0;

	UxArchiveSessContext = UxContext =
		(_UxCarchiveSess *) UxNewContext( sizeof(_UxCarchiveSess), False );

	UxParent = _UxUxParent;

	if ( ! _Uxinit )
	{
		_UxIfClassId = UxNewInterfaceClassId();
		_Uxinit = 1;
	}

	rtrn = _Uxbuild_archiveSess();
	UxPutClassCode( archiveSess, _UxIfClassId );

	if(recoveryInfoSession()==0) {
	   UxPopupInterface(rtrn, no_grab);
	}
	else {
	   fprintf(stderr,"WARNING!!! Error in dynamic allocation.\n");
	}
	return(rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

