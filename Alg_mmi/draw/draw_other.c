/**********************************************************************
*
*       C Source:               draw_other.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Tue Jul 19 11:19:50 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: draw_other.c-5 %  (%full_filespec: draw_other.c-5:csrc:1 %)";
#endif
/*
        Inizio sezione per SCCS
*/
/*
   modulo draw_other.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)draw_other.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)draw_other.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <stdlib.h>   /* exit */
#include <unistd.h>   /* execvp, vfork */
/*
 routines di print per la versione stand
 alone di draw
*/

Widget PrintSetup(Widget cw)
{
#if defined XPRINTER_USED && !defined LINUX
return(XlPrintSetup(cw));
#else
return(NULL);
#endif
}

Boolean  PrintPag(Widget cw)
{
#if defined XPRINTER_USED && !defined LINUX
XlPrintOpen(cw);
print_callback(cw);
XlPrintClose();
return(True);
#else
return(False);
#endif
}

/*
 N.B. Questo file e' PROVVISORIO ed e' stato inserito solo per
 permettere il link locale del draw stand alone.
*/
/***************************************************
    Routine che permette dilanciare un eseguibile che richieda di
        specificare un numero di parametri arbitrario;la lista deve
        iniziare con il nome dell'eseguibile e quindi deve proseguire
        con l'elenco dei parametri;la lista deve essere terminata con
        un NULL;
    Esempio:
        ActiveProgWithParameter (nome_eseguibile,par1,par2, ...,(char *)0);

        Il Numero massimo di parametri e' fissato da MAX_ARGOMENTI;

int ActiveProgWithParameter
        (
        va_list                 -->  Lista degli argomenti;
        )

        Valore di ritorno       -->  Viene ritornato il pid del processo
                                        associato al programma lanciato;
****************************************************/
int ActiveProgWithParameter (char * StringaComando, ...)
{
int pid;
int i;
char *argomento; /* puntatore di appoggio per contenere di volta in volta
                    un nuovo argomento */
#define MAX_ARGOMENTI 10  /* numero massimo di argomenti di chiamata per
                             la function attiva_prog_par */
char *argv[MAX_ARGOMENTI];
va_list args;

argv[0]=StringaComando;
va_start(args, StringaComando);
i=1;
while ((argomento=va_arg(args,char *))!=0)
        {
        argv[i]=argomento;
        i++;
        if(i==MAX_ARGOMENTI)
                {
                printf("ActiveProgWhitParameter: Max number of argument!\n");
                exit(1);
                }
        }
argv[i]=argomento;
va_end(args);
#ifndef DESIGN_TIME
/*
signal(SIGCHLD,CloseProgram);
*/
#if defined SCO_UNIX
if((pid=fork())==0)
#else
if((pid=vfork())==0)
#endif
        {
        execvp(argv[0],argv);
        }
else
        return(pid);
#else
printf("Simula attivazione \n");
#endif
return(1);
}
