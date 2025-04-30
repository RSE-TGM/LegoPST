/**********************************************************************
*
*       C Source:               comp_all.c
*       Subsystem:              8
*       Description:
*       %created_by:    furlani %
*       %date_created:  Wed Jun 30 11:29:37 1999 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: comp_all.c-8.1.1.1.4 %  (%full_filespec: comp_all.c-8.1.1.1.4:csrc:8 %)";
#endif
/*
   modulo comp_all.c
   tipo 
   release 5.13
   data 2/16/96
   reserved @(#)comp_all.c	5.13
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)comp_all.c	5.13\t2/16/96";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
# include <sys/time.h>
# include <utime.h>
#if defined VMS
#include <stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xresource.h>
#include <Xm/Xm.h>
#include <X11/Xlib.h>
#include <UxXt.h>
#include <X11/Core.h>
#include <Xl/XlCore.h>
#include <Xl/XlManager.h>
#include <Xl/Xl.h>
#include <Xl/XlIconReg.h>
#include <Xl/XlPort.h>
#include <Ol/OlConn.h>
#include <Ol/OlDatabaseTopologia.h>
#include <Xd/Xd.h>
#include <Xd/XdLista.h>
#include <Xm/DrawingA.h>
#include <unistd.h>

#include <sys/wait.h>

#include "config.h"
#include "message.h"
#include "forme.h"     /* per topologia schemi regolazione */
#include "top_icon.h"  /* per topologia schemi regolazione */


#define NOT_IN_LIB 1

/*
   Variabili esterne
*/
extern PAGEDIT_CONTEXT *pagedit;
extern Widget UxTopLevel;
extern XtAppContext UxAppContext;
extern Display *UxDisplay;
extern Boolean StateInterfaceMode;
extern OlConnObject conn_obj; /* oggetto OlConn (gestione connessioni) */
extern *lista_macro;
extern int comm_found;

/* 
  Variabili globali
*/
char workfile[200];
char envfile[200];
char rtffile[200];
typedef struct non_comp_st
        {
        char name[80];       /* Nome della pagina */
        int  tipocomp;       /* Tipo della compilazione */
        int  datapag;        /* Data del file .pag */
        int  datartf;        /* Data del file rtf */
        int  data04;         /* Data del file _04.dat */
        int  data14;         /* Data del file _14.dat */
        int already;         /* Pagina gia' compilata */
        }COMP_INFO;

/* Lista  per lettura pagine nella schemi_reg.a */
static int sche_in_lib=0;

/* 
  Prototyping funzioni contenute
*/
int compile_all_reg();
Boolean NewDataBase(char *);
Boolean IsPaginaCompiled(char *, int, int *);
int DataFile(char *);
int ControlComp(COMP_INFO *,COMP_INFO *,int ,int ,int);

Boolean InLibrary(char *);
extern Boolean PagGetInUses(PAGINA *);

/* 
  Prototyping funzioni esterne
*/
int FileNameLoc(char *, char *, char *);




/*
    Compilazione di tutte le pagine di regolazione
*/
int compile_all_reg()
{

int numero=0;
Boolean read_db;

//extern XtAppContext CompileAppContext;
//extern Display *CompileDisplay;
// extern Widget CompileTopLevel;



XEvent event;
extern int compila_reg(PAGINA *, int, int);
int macrobl=0;
Widget CompileBoard;
extern Cardinal lnargs;
extern Arg args[];
extern OlCompilerObject compilatore_severe;
extern XrmDatabase defdb;
extern swidget topLevelShell;
int err_level = 0;
char outname[105],mesg[200];
int i,j;
PAGINA *pag=NULL;
AN_OBJ *lista_oggetti=NULL;
WidgetList lista;
Widget *array_wid;
int flag=1;
int ret=1,ret1;
int no_error=1;
int inserimenti_in_lista_schemi=0;
COMP_INFO *elencoN;
COMP_INFO *elencoC;
int contN=0;
int contC=0;
int no_regolazione=0;

char nome_04[200];
char nome_14[200];
char nome_pag[200];
char nome_rtf[200];

int causa_comp_reg;

char comando_comp[100];
int  ret_sys,ret_sys_int;
/*
 Salva il puntatore al database delle risorse corrente
*/
/*
defdb= UxDisplay->db;
*/
   if( getenv(ENVPAG) == NULL)
      return;

   if(pagedit->pag_num > 0)
      {
      ret1 = EstrWorkFile(workfile,envfile,rtffile);
      if ( ret1!=0 )
         {
         fprintf(stderr,"Non sono definite le variabili d'ambiente\n");
         return;
         }
      elencoN = (COMP_INFO*)XtCalloc(pagedit->pag_num+10,sizeof(COMP_INFO));
      elencoC = (COMP_INFO*)XtCalloc(pagedit->pag_num+10,sizeof(COMP_INFO));
      }
/*
 Cicla su tutte le pagine per procedere alla compilazione
 delle pagine che lo necessitano.
 Per decidere se la pagina necessita la compilazione vengono
 confrontati i file .pag (oggetto iniziale della compilazione) e
 i file .proc/<schema>_14.dat e .proc/<schema>_04.dat che costituiscono
 il risultato finale della procedura di compilazione.
*/
      for(i=0;i<pagedit->pag_num;i++)
         {
/*
  Se la pagina non e' di regolazione  o non e' in uso non viene compilata
*/
         if( strcmp(pagedit->res_page[i]->type_page,TIPO_REGOLAZIONE)   ||
		pagedit->res_page[i]->in_use == False) 
            {
            fprintf(stderr,
            "Non si compila <%s>  perche' <not in use>\n",
                    pagedit->page_list[i]);
            continue;
            }
/*
 Se la pagina ha gia' subito il processo di compilazione
 passa alla pagina successiva. Il secondo parametro vale
 1 per la compilazione di pagine di regolazione.
*/
          if( IsPaginaCompiled(pagedit->page_list[i],1,&causa_comp_reg) == True )
              {
              /*printf("Gia' compilata\n");*/
              strcpy(elencoC[contC].name,pagedit->page_list[i]);
              elencoC[contC].tipocomp=1;
/*
   Salvo (provvisoriamente) le date nella struttura
*/
              FileNameLoc(pagedit->page_list[i],".pag",nome_pag);
              FileNameInproc(pagedit->page_list[i],"_04.dat",nome_04);
              FileNameInproc(pagedit->page_list[i],"_14.dat",nome_14);
              elencoC[contC].datapag = DataFile(nome_pag);
              elencoC[contC].data04  = DataFile(nome_04);
              elencoC[contC].data14  = DataFile(nome_14);
              elencoC[contC].already = 1;
              contC++;
              continue;
              }
           
           strcpy(comando_comp,"compreg ");
           strcat(comando_comp,pagedit->page_list[i]);
           strcat(comando_comp," ");
           strcat(comando_comp,XlGetenv(ENVPAG));
           strcat(comando_comp," ");
           strcat(comando_comp,XlGetenv(ENVRTF));
/*
   Attivazione dell'eseguibile 'compreg' per la compilazione
   della singola pagina di regolazione.
*/ 

printf("comando_comp = %s\n",comando_comp);

           ret_sys_int=esegui_comando(comando_comp);
           if(ret_sys_int>0)
              {
/*
   La compilazione della pagina corrente non e' andata a buon fine.
   Gestione dell'errore.
*/
              if(ret_sys_int==1)
/*
  La pagina in esame non era di tipo regolazione. Non e' stata
  compilata.
*/ 
                  {
                  sprintf(mesg,"Page %s (synoptic) did not compiled\n",
                          pagedit->page_list[i]);
                  if(comm_found==0)
                     show_message(mesg);
                  else
                     printf("%s",mesg);
                  }
               else
                  {
/*
   Errore durante la fase di compilazione della pagina.
*/
                  strcpy(elencoN[contN].name,pagedit->page_list[i]);
                  elencoN[contN].tipocomp=-1;
                  contN++;

                  no_error = 0;
                  sprintf(mesg,"Error at Regulation Compile Time:Page %s\n",
                          pagedit->page_list[i]);
                  fprintf(stderr,"\nerror at Compile time mesg = %s\n",mesg);
                  if(comm_found==0)
                     show_message(mesg);
                  else
                     printf("%s",mesg);
                  }
               }
            else
               {
/*
   La compilazione della pagina e' andata a buon fine.
*/
               strcpy(elencoC[contC].name,pagedit->page_list[i]);
               elencoC[contC].tipocomp=1;
               contC++;

               inserimenti_in_lista_schemi += ret_sys_int;
               sprintf(mesg,"Regulation Page Compiled  %s,  Ok\n",
               pagedit->page_list[i]);
               if(comm_found==0)
                  show_message(mesg);
               else
                  printf("%s",mesg);
               }
       } 
/*
UxDisplay->db = defdb;
*/
   if( no_error==1 )
      {
      sprintf(mesg,"Compilation correct of all pages, %d insertion in lista_moduli.dat",
              inserimenti_in_lista_schemi);
      if(comm_found==0)
         SetMsg(topLevelShell,NULL,INFMSG,"ConfigInfo",mesg,
                  NULL,False,NULL,False,NULL);
      else
         printf("%s",mesg);
      }
   else
      {
      sprintf(mesg,
              "Any errors during compilation of all pages, %d insertion in lista_moduli.dat",
              inserimenti_in_lista_schemi);
      }

   ControlComp(elencoN,elencoC,contN,contC,1);
   if(comm_found==0)
      show_message("-Compilation End-\n");
   else
      printf("-Compilation End-\n");

/* Chiusura files e liberazioni liste */
   if(pagedit->pag_num > 0)
       {
       XtFree(elencoC);
       XtFree(elencoN);
       }
}




/*
         compilazione di tutte le pagine nel context 
*/
int compile_all_pag()
{
extern swidget topLevelShell;
Widget *array_wid;
Boolean read_db;
Widget CompileBoard;

char appo[200];
char file1[200],file2[200],file3[200];
int touchdat=-1;
char firtf[200];
char filpg[200];
int ret1=-1,ret2=-2,ret3=-3;
COMP_INFO *elencoN;
COMP_INFO *elencoC;
int contN=0;
int contC=0;

char nome_04[200];
char nome_14[200];
char nome_pag[200];
char nome_rtf[200];
int causa_comp_pag;

           /*compilazione di tutte le pagine nel context */
           extern Cardinal lnargs;
           extern Arg args[];
           extern OlCompilerObject compilatore_severe;
           extern XrmDatabase defdb; /* database risorse di default */
           int err_level = 0;
           WidgetList children;
           Cardinal num_children;
           char outname[105],mesg[200];
           int i,j;
           PAGINA *pag=NULL;
           AN_OBJ *lista_oggetti=NULL;
           WidgetList lista;
           int g,m,a,o,min,s;

    int numero=0;

    if( getenv(ENVPAG) == NULL)
        return;
/*
    Salva il database delle risorse di default
*/
    defdb=UxDisplay->db;

/*   File con informazioni sulle compilazioni */
     if(pagedit->pag_num > 0)
       {
       ret1 = EstrWorkFile(workfile,envfile,rtffile);
       if ( ret1!=0 )
          {
          fprintf(stderr,"Non sono definite le variabili d'ambiente\n");
          return;
          }
       elencoN = (COMP_INFO*)XtCalloc(pagedit->pag_num,sizeof(COMP_INFO));
       elencoC = (COMP_INFO*)XtCalloc(pagedit->pag_num,sizeof(COMP_INFO));
       }

     for(i=0;i<pagedit->pag_num;i++)
       {
       XdLista lista_save;

/*
 Se la pagina ha gia' subito il processo di compilazione
 passa alla pagina successiva. Il secondo parametro vale
 0 per la compilazione di sinottico.
 Se la topologia e' cambiata, file variabili.rtf piu' recente
 di .pag la pagina deve essere ricompilata indipendentemente 
 dal file rtf.
*/

      if ( NewDataBase(pagedit->page_list[i]) == False )
          {
          if( IsPaginaCompiled(pagedit->page_list[i],0,&causa_comp_pag) == True )
             {
             strcpy(elencoC[contC].name,pagedit->page_list[i]);
             elencoC[contC].tipocomp=1;
/*
   Salvo (provvisoriamente) le date nella struttura
*/
             FileNameLoc(pagedit->page_list[i],".pag",nome_pag);
             FileNameLoc(pagedit->page_list[i],".rtf",nome_rtf);
             elencoC[contC].datapag = DataFile(nome_pag);
             elencoC[contC].datartf = DataFile(nome_rtf);
             elencoC[contC].already = 1;
/**/
             contC++;
             continue;
             }
          }

       lista_save = clip_get_lista();

       CompileBoard = create_ClipBoard();

       if( pagina_init( pagedit->page_list[i], &pag,TIPO_PAGINA) == False)
          return;
  

       fprintf(stderr,"    Compilazione pagina [%s]\n",pag->nomepag);

       if( pagina_load_file(pag,TIPO_PAGINA)  && 
           pagina_getres(pag,&lista_oggetti) )
         {

/*       Non compilo come MMI una pagina di regolazione non in uso */
         if( (PagGetType(pag)==TYPE_REGOLAZIONE) &&
             (PagGetInUse(pag)==False) )
            {
            fprintf(stderr,
              "    Non compilo come MMI <%s> non in uso \n",pag->nomepag);
               strcpy(elencoN[contN].name,pagedit->page_list[i]);
               elencoN[contN].tipocomp=-2;
               contN++;
            continue;
            }

         if(pag->num_widget > 0)
            {
            if( (lista=alloca_memoria(pag->num_widget,sizeof(Widget)))==NULL )
                return;

            UxDisplay->db = pag->db;

            for(j=0;j<pag->num_widget;j++)
                lista[j] = XtCreateWidget(lista_oggetti[j].nome,
                                          lista_oggetti[j].classe,
                                          CompileBoard,NULL,0);
            pag->drawing=CompileBoard;

            OlSetCompiler(lista,pag->num_widget,compilatore_severe);

            strcpy(outname,envfile);
            strcat(outname,pag->nomepag);
            strcat(outname,".rtf");
/*
            richiamo la routine PostOpenPage2 per settare le
            risorse, prelevate da Connessioni.reg, nelle 
            eventuali icone di interffaccia
*/
            PostOpenPage2(CompileBoard,pag->nomepag,pag->db);

            /* printf("Chiamo il compilatore, metto risultato in %s\n",
                       outname);*/


            if( (err_level = XlCompilaWidget(pag->db,outname,lista,
                                             pag->num_widget) ) == False)
               {
               strcpy(elencoN[contN].name,pagedit->page_list[i]);
               elencoN[contN].tipocomp=-1;
               contN++;

               sprintf(mesg,"Error at Compile Time on Page %s\n",pag->nomepag);
               fprintf(stderr,"error at Compile time mesg = %s\n",mesg);
               show_message(mesg);
/*             In presenza di errori cancello il file rtf */
               FileNameLoc(pag->nomepag,".rtf",firtf);
               ret1 = unlink(firtf);
               if ( ret1!=0 )
                  fprintf(stderr,"Errore eliminazione %s\n",firtf);
               }
            else
               {
               /*       
                setto la data della compilazione e
                forzo il salvataggio della pagina
               */
               strcpy(elencoC[contC].name,pagedit->page_list[i]);
               elencoC[contC].tipocomp=1;
               contC++;

               data(&g,&m,&a);
               ora(&o,&min,&s);
               sprintf(pag->compilationDate,
                       "%d/%d/%d %d:%d:%d",g,m,a,o,min,s); 
#ifndef DESIGN_TIME
/*
               Se come pagina di regolazione e' gia' stata salvata
               bisogna fare un touch dei file ./proc/pagina_14.dat
               e ./proc/pagina_14.dat dopo pagina_save.
*/
               if( (PagGetType(pag)==TYPE_REGOLAZIONE) )
                  {
                  if( IsPaginaCompiled(pag->nomepag,2,&causa_comp_pag) == True )
                     {
/*                   Pathname file e comando per touch dopo pagina_save */
                     FileNameInproc(pag->nomepag,"_04.dat",file1);
                     FileNameInproc(pag->nomepag,"_14.dat",file2);

                     printf("ESCO CON TRUE e touchdat=%d\n",touchdat);

                     touchdat=1;
                     }
                  }
#ifdef SALVAPAG
               pagina_save(pag,TIPO_PAGINA,0);
#endif
/*             Serie di touch */
               if( touchdat==1)
                 {
                 touchdat=0;
                 ret1=tempo_file_touch(file1);
                 ret2=tempo_file_touch(file2);
                 if( (ret1!=0)||(ret2!=0) )
                     fprintf(stderr,
                             "In compile_all_page fallito touch:<%s><%s>\n",
                             file1,file2);
                 }
               FileNameLoc(pag->nomepag,".rtf",firtf);
               ret1=tempo_file_touch(firtf);
               if( ret1!=0 )
                  fprintf(stderr,
                          "In compile_all_page fallito touch di <%s>\n",firtf);
#endif
               sprintf(mesg,"Page Compiled  %s,  Ok\n",pag->nomepag);
               if(comm_found!=0)
                  show_message(mesg);
               else
                  printf("%s\n",mesg);
               }

            for(j=0;j<pag->num_widget;j++)
               XtDestroyWidget(lista[j]);

            libera_memoria((char*)lista);
            }
         pagina_free(pag);
         if(pag->num_widget > 0)
            libera_memoria((char*)lista_oggetti);
         }
       else
         libera_memoria((char*)pag);

       UxDestroyInterface(CompileBoard);
/*
 Libera la lista di draget che e' stata creata interanmente alla
 funzione create_ClipBoard
*/
      XdDestroyLista(clip_get_lista());
/*
 ripristina la lista presente prima della
 compilazione
*/
       clip_set_lista(lista_save);
       UxDisplay->db = NULL;
       }

    UxDisplay->db = defdb;
 
    ControlComp(elencoN,elencoC,contN,contC,0);
    if(comm_found!=0)
       printf("-Compilation End-\n");
    else
       show_message("-Compilation End-\n");

/*  Chiusura files e liberazioni liste */
     if(pagedit->pag_num > 0)
       {
       XtFree(elencoC);
       XtFree(elencoN);
       }
}



/*
    Compila la singola pagina
*/
int compile_page(PAGINA *actual_page)
{

           extern Cardinal lnargs;
           extern Arg args[];
           extern OlCompilerObject compilatore_severe;
           int err_level = 0;
           WidgetList children;
           Cardinal num_children;
           char outname[105],mesg[200];
           int g,m,a,o,min,s;
           int ret1;
           char firtf[200];


           if(compilatore_severe == NULL)
                {
                SetMsg(NULL,NULL,ERRMSG,"ConfigInfo",
                       "Topology database not defined",
                        NULL,False,NULL,False,NULL);
                return;
                }

           if( getenv(ENVPAG) == NULL)
              return;

           ret1 = EstrWorkFile(workfile,envfile,rtffile);
           if ( ret1!=0 )
              {
              fprintf(stderr,"Non sono definite le variabili d'ambiente\n");
              return;
              }

           lnargs = 0;
           XtSetArg( args[lnargs],XmNchildren,&children);lnargs++;
           XtSetArg(args[lnargs],XmNnumChildren,&num_children);lnargs++;
           XtGetValues(actual_page->drawing,args,lnargs);

           OlSetCompiler(children,num_children,compilatore_severe);

           strcpy(outname, getenv(ENVPAG));
           strcat(outname,"/");
           strcat(outname,actual_page->nomepag);
           strcat(outname,".rtf");

           if( (err_level = XlCompilaWidget(actual_page->db,outname,
                                            children,num_children)) == False)
               {

               sprintf(mesg, "Error at Compile Time on Page  %s\n",
                       actual_page->nomepag);
               if(comm_found!=0)
                  printf("%s\n",mesg);
               else
                  show_message(mesg);
/*             In presenza di errori cancello il file rtf */
               FileNameLoc(actual_page->nomepag,".rtf",firtf);
               fprintf(stderr,"ATTENZIONE %s\n",firtf);
               ret1 = unlink(firtf);
               if ( ret1!=0 )
                  fprintf(stderr,"Errore eliminazione %s\n",firtf);
               }
               else
               {
               /*
               setto la data della compilazione
               */
               data(&g,&m,&a);
               ora(&o,&min,&s);
               sprintf(actual_page->compilationDate,
                       "%d/%d/%d %d:%d:%d",g,m,a,o,min,s);
               sprintf(mesg, "Page Compile Ok %s\n ",actual_page->nomepag);
               show_message(mesg);
               }
}




/*
   Ritorna True se la pagina identificata dal nome
   e' gia' stata compilata.
   Il controllo e' fatto confrontando le date degli
   ultimi file prodotti dalla compilazione   ed  e'
   positivo se i file prodotti sono piu' recenti del
   file con cui si confrontano.
   tipopg vale 1 o 2 se si tratta della compilazione di 
   una regolazione 0 se di un sinottico.
   Files per il confronto:
   - regolazione ---> pagina_04.dat, pagina_14.dat 
                      con pagina.pag 
                      In questo caso deve essere anche testata l'esistenza
                      del file pagina.reg dato che viene aperto durante la
                      generazione della task
   - sinottico   ---> pagina.pag
                      con pagina.pag 
*/
Boolean IsPaginaCompiled(char *nome,int tipopg, int *causa_comp)
{
char nomreg[200];
char nome14[200];
char nome04[200];
char nomrtf[200];
char nomepg[200];
char workpg[200];
int ret1=-1,ret2=-1,ret3=-1;
struct stat buf;



        *causa_comp = 0;
/*      Ricavo i pathname */
        strcpy(nomepg,envfile);
        strcat(nomepg,nome);
        strcat(nomepg,".pag");

        if (tipopg != 0)
           {
           ret1 = FileNameInproc(nome,"_04.dat",nome04);
           ret2 = FileNameInproc(nome,"_14.dat",nome14);
           ret3 = FileNameInproc(nome,".reg",nomreg);
           if ( (ret1==-1) || (ret2==-1) || (ret3==-1) )
               {
               if ( tipopg==1 )
                  {
                  fprintf(stderr,"%s-->Manca file out (04=%d 14=%d reg=%d)\n",
                          nome,ret1,ret2,ret3);
                  }
               return(False);
               }
           }
        else
           {
           strcpy(nomrtf,envfile);
           strcat(nomrtf,nome);
           strcat(nomrtf,".rtf");
           if(stat(nomrtf,&buf)==-1)
               {
               fprintf(stderr,"%s---> Manca file rtf (%s)\n",nome,nomrtf);
               return(False);
               }
           }

/*
        Se i file prodotti sono piu' recenti
        o contemporanei della pagina la
        routine esce con True.
*/
        if (tipopg!=0)
           {
           ret1=tempo_file_eq(nome14,nomepg);
           ret2=tempo_file_eq(nome04,nomepg);
/************
           ret3=tempo_file_eq(nomreg,nomepg);
************/
           if( tipopg==1 )
               fprintf(stderr," File pag=%d  04=%d  14=%d [%s]\n",
                       DataFile(nomepg),DataFile(nome04),DataFile(nome14),nome);
           }
        else
           {
           /*
           fprintf(stderr,"[%s] File rtf-pag=%d\n",nome,
                   (DataFile(nomrtf)-DataFile(nomepg)));
           */
           ret1=tempo_file_eq(nomrtf,nomepg);
           ret2=1;
           fprintf(stderr," File pag=%d  rtf=%d [%s]\n",
                   DataFile(nomepg),DataFile(nomrtf),nome);
           }
        /*
        if( tipopg==0 )
           fprintf(stderr,"ret1=%d \n",ret1);
        else
           fprintf(stderr,"ret1=%d  ret2=%d\n",ret1,ret2);
        */
        if( (ret1==1) && (ret2==1) )
           {
/*         Testo la libreria solo se compilo come regolazione (1) e non per il touch (2) */
           if( tipopg==1 )
              {
              if(InLibrary(nome)==False)
                 {
                 fprintf(stderr,"%s non e' in libreria, e' da compilare\n",
                         nome);
                 *causa_comp = 1;
                 return(False);
                 }
              else
                 {
                 return(True);
                 }
              }
            else
              return(True);
           }
        else
           {
           /*printf("Ritorno False\n");*/
           return(False);
           }
}



/*
    Controlla la presenza dell'oggetto nella libreria
*/
Boolean InLibrary(char *nome)
{
char comando[200];
char nome_letto[200];
char nome_cercato[200];
FILE *fp;

	sprintf(nome_cercato,"%s.o",nome);
	tominus(nome_cercato);
#ifdef NUTC

	sprintf(comando,"ls ./proc/%s > FileAppo", nome_cercato);
#else
	sprintf(comando,"ar -t ./proc/schemi_reg.a %s > FileAppo", nome_cercato);
#endif
	system(comando);
	fp=fopen("FileAppo","r");
	assert(fp);
	if(fscanf(fp,"%s",nome_letto)!=1)
		nome_letto[0]=0;
	fclose(fp);
#ifdef NUTC
	if(strstr(nome_letto,nome_cercato)!=NULL)
		{
		return(True);
		}
#else
	if(strcmp(nome_cercato,nome_letto)==NULL)
		{
		return(True);
		}
#endif
	return(False);
}






/*
   Ritorna True se il data base variabili.rtf e' stato
   aggiornato.
   Il confronto e' fatto tra .pag e variabili.rtf e vale
   per entrambe le compilazioni.
*/
Boolean NewDataBase(char *nome)
{
char varrtf[200];
char workpg[200];
char nomepg[200];
int ret1=-1,ret2=-1;


        strcpy(nomepg,envfile);
        strcpy(varrtf,rtffile);

        strcat(varrtf,"variabili.rtf");
        strcat(nomepg,nome);

/***
        strcat(nomepg,".pag");
***/
/*
Effettuo confronto sui .rtf e non sui .pag
*/

        strcat(nomepg,".rtf");

        ret1=tempo_file_eq(varrtf,nomepg);
        if( ret1==1 )
           {
           fprintf(stderr,"ATTENZIONE \n%s piu' recente di \n%s\n",
                   varrtf,nomepg);
           return(True);
           }
        else
           return(False);
}


/*
    Legge la data (in secondi) di un file
*/
int DataFile(char *filename)
{
struct stat buf_1;
int ret=-9;


     ret=stat(filename,&buf_1);
     /*printf("<%s> Data=%d  \n",filename,buf_1.st_mtime);*/

     if (ret==0)
         ret=buf_1.st_mtime;
     return(ret);
}


/*
    Dall'elenco delle pagine compilate, fornito nei 2 diversi
    tipi di compilazione scrive due file.
*/
int ControlComp(COMP_INFO *elN,COMP_INFO *elC, int cN, int cC, int tipo)
{
int i,ok=1;
char msg[200];
char fileC[200];
char fileN[200];
FILE *fpN=NULL;
FILE *fpC=NULL;
int ret;




   strcpy(fileN,workfile);
   strcpy(fileC,workfile);

   if ( tipo==1 )
      {
      strcat(fileN,"RegNotComp.edf");
      strcat(fileC,"RegComp.edf");
      fpN = fopen(fileN,"w");
      fpC = fopen(fileC,"w");
      if ( (fpN==NULL) || (fpC==NULL) )
          {
          fprintf(stderr,"Non posso aprire i seguenti file\n   %s\n   %s\n",
                  fileN,fileC);
          return(-1);
          }
      fprintf(fpC,"%s  ","REGULATION PAGES COMPILED SUCCESSFULLY");
      fprintf(fpN,"%s  ","REGULATION PAGES NOT COMPILED");
      }
   else
      {
      strcat(fileN,"PagNotComp.edf");
      strcat(fileC,"PagComp.edf");
      fpN = fopen(fileN,"w");
      fpC = fopen(fileC,"w");
      if ( (fpN==NULL) || (fpC==NULL) )
          {
          fprintf(stderr,"Non posso aprire i seguenti file\n   %s\n   %s\n",
                  fileN,fileC);
          return(-1);
          }
      fprintf(fpC,"%s  ","PAGES COMPILED SUCCESSFULLY");
      fprintf(fpN,"%s  ","PAGES NOT COMPILED");
      }

/* Scrittura e chiusura file compilazioni fallite */
   /*printf("   COMPILAZIONI NON CORRETTE\n");*/
   fprintf(fpN,"(total=%d)\n",cN);
   for(i=0;i<cN;i++)
      {
      if ( elN[i].tipocomp==-2 )
         fprintf(fpN,"%s   NOT IN USE\n",elN[i].name);
      else
         fprintf(fpN,"%s\n",elN[i].name);
      /*printf("%s\n",elN[i].name);*/
      }
   fprintf(fpN,"%s\n","EOF");
   fclose(fpN);

/* Scrittura e chiusura file compilazioni riuscite */
   /*printf("   COMPILAZIONI CORRETTE\n");*/
   fprintf(fpC,"(total=%d)\n",cC);
   for(i=0;i<cC;i++)
      {
/*    Per debug sulle date */
      if ( elC[i].already==1 )
         {
         if ( tipo==1 )
            fprintf(fpC,"%s ---> data file pag=%d   data _04=%d data _14=%d\n",
                    elC[i].name,elC[i].datapag,elC[i].data04,elC[i].data14);
         else
            fprintf(fpC,"%s ---> data file pag=%d   data file rtf=%d\n",
                    elC[i].name,elC[i].datapag,elC[i].datartf);
         }
      else
         fprintf(fpC,"%s\n",elC[i].name);
/*      printf("%s\n",elC[i].name);*/
      }
   fprintf(fpC,"%s\n","EOF");
   fclose(fpC);

   if( cN==0 )
     {
     if ( tipo==1 )
        sprintf(msg,"All regulation pages have been compiled \n");
     else
        sprintf(msg,"All pages have been compiled \n");
     show_message(msg);
     }

   return(0);
}



/*
    Data una pagina e una tag ricava il
    pathname del file relativo alla pagina
    ivi contenuto e lo restituisce nel
    parametro fileout.
*/
int FileNameLoc(char *pgname, char *tag, char *fileout)
{
char work_file[300];
char nome[50];
struct stat buf;
int ret;
int stat_ret;




   strcpy(nome,pgname);

   strcpy(work_file,envfile);

/* Compongo con il nome pagina e la tag desiderata */
   strcat(work_file,nome);
   strcat(work_file,tag);

   strcpy(fileout,work_file);
   /*printf("FileNameLoc restituisce=%s\n",fileout);*/

   stat_ret=stat(fileout,&buf);

   return(stat_ret);
}

/* Fine File*/
