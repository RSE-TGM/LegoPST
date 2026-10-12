#define _GNU_SOURCE
/**********************************************************************
*
*       C Source:               _subproc.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed Jan 10 14:47:06 2007 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: _subproc.c-6 %  (%full_filespec: _subproc.c-6:csrc:1 %)";
#endif
static char sccsid[] = "@(#)14	1.2  com/contrib/Subproc/_subproc.c, aic, aic322, 9238322 4/27/92 18:42:35";
/*
 *  COMPONENT_NAME: AIC     AIXwindows Interface Composer
 *  
 *  ORIGINS: 58
 *  
 *  
 *                   Copyright IBM Corporation 1991, 1992
 *  
 *                         All Rights Reserved
 *  
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation, and that the name of IBM not be
 *   used in advertising or publicity pertaining to distribution of the
 *   software without specific, written prior permission.
 *  
 *   IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *   ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. IN NO EVENT SHALL IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 *   USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 *   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 *   OR PERFORMANCE OF THIS SOFTWARE.
*/
/*------------------------------------------------------------------------
 * $Date: 91/11/28 14:42:02 $ 		        $Revision: 1.42 $
 *
 * Copyright 1989, 1990, 1991,  Visual Edge Software Ltd.
 * ----------------------------------------------------------------------
 * ALL RIGHTS RESERVED.  This notice is intended as a precaution against
 * inadvertent publication, and shall not be deemed to constitute an 
 * acknowledgment that publication has occurred nor to imply any waiver  
 * of confidentiality.	The year included in the notice is the year 
 * of the creation of the work.			   
 *------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
 * Motorola additions to make the subprocess management more amenable to
 * an application doing it's own signal handling and/or child process
 * creation (esp. in the "inocuous" cases of calls to popen(3S), system(3S)
 * or getcwd(3C) are contained in "ifdef SPMD conditionals.
 *------------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#if defined (LINUX)
#define USE_SYSV_TERMIO
#include <termios.h>
#include <sys/ioctl.h>
struct msgbuf
{
  long int mtype;		/* type of received/sent message */
  char mtext[1];		/* text of the message */
};
#endif

#ifdef	DEBUG
	    FILE *DebugLog;
#endif	/* DEBUG */

#undef SPMD
#define _STREAM_DRIVER

#ifdef SPMD			/* If SPMD is required it needs ... */
#	ifndef _PTY_DRIVER
#		define _PTY_DRIVER	/* _PTY_DRIVER  defined and ... */
#	endif /*_PTY_DRIVER*/

#	ifdef _STREAM_DRIVER
#		undef _STREAM_DRIVER	/* _STREAM_DRIVER undefined */
#	endif /*_STREAM_DRIVER*/
#endif /*SPMD*/

#ifdef	SUBPROC_OVERLAY
#define	DEBUG
#define	UxSetSubprocExitCallback	UxSubprocSetSubprocExitCallback
#define	UxDeleteSubproc			UxSubprocDeleteSubproc
#define	UxCreateSubproc			UxSubprocCreateSubproc
#define	UxSetSubprocEcho		UxSubprocSetSubprocEcho
#define	UxRunSubproc			UxSubprocRunSubproc
#define	UxExecSubproc			UxSubprocExecSubproc
#define	UxExitSubproc			UxSubprocExitSubproc
#define	UxGetSubprocPid			UxSubprocGetSubprocPid
#define	UxSendSubproc			UxSubprocSendSubproc
#define	UxSendSubprocNoCR		UxSubprocSendSubprocNoCR
#define	UxTransferToBuffer		UxSubprocTransferToBuffer
#define	UxAppendTo			UxSubprocAppendTo
#define	UxSetSubprocClosure		UxSubprocSetSubprocClosure
#define	UxSetSubprocFunction		UxSubprocSetSubprocFunction
#endif	/* SUBPROC_OVERLAY */

#ifdef	SYSV
# 	define	USE_SYSV_TERMIO
#	define	USE_SYSV_PGRP
#endif	/* SYSV */

#include <signal.h>

#ifdef _PTY_DRIVER
#	include <setjmp.h>
#	ifdef	SYSV
#		include <sys/ioctl.h>
#		include <sys/termio.h>
#		if !defined(m88k) && !defined(sgi) && !defined(magnum) && !defined(sco) 
#			include <sys/ptyio.h>
#			if defined(SIGTSTP) 
#				include <sys/bsdtty.h>
#			endif	/* SIGTSTP */
#		endif	/* (m88k) && (sgi) && (magnum) && (sco) */
#	endif	/* SYSV */

#	if defined(USES_SGTTY_H)
#		include <sys/sgtty.h>
#		include <sys/termio.h>
#	endif	/* USES_SGTTY_H */

#	if defined(_TEK88) || defined(_MOTOR88)
#		include <sys/pty.h>
#	endif   /* _TEK88 */

#	include <sys/ioctl.h>
#	include <sys/types.h>
#	include <sys/wait.h>

#	if !defined(_TEK88) && !defined(_MOTOR88)
#		include <sys/file.h>
#	endif  

#	include <errno.h>
#	include <fcntl.h>

#	if defined(sgi)
#		include <sys/stat.h>
#		include <sys/sysmacros.h>
#	endif /* sgi */

#endif /* _PTY_DRIVER */

#ifdef _STREAM_DRIVER
#	include <stropts.h>
#	include <sys/types.h>
#	include <sys/ioctl.h>
#	include <errno.h>
#	include <signal.h>
#	include <fcntl.h>
#	include <termios.h>	/* termio.h non esiste piu' da glibc 2.42 */
#endif /* _STREAM_DRIVER */

#include <X11/Intrinsic.h>
#include <X11/X.h>
#include <Xm/Text.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "UxSubproc.h"
#include "uimx_cat.h"
#include "misc1_ds.h"
#include "global.h"

typedef struct {
	char		*process;
	char		*defarg;
	char		*output_str;
	void		(*output_fnt)();
	void		(*exit_cb)();
#ifdef _PTY_DRIVER
	int		pty;
#endif
#ifdef _STREAM_DRIVER
	int		stream;
#endif
	int		echo,
			pid;
	char		*user_data;
	XtInputId	input_id;
	XtInputId	except_id;
} subproc;

/*------------------------------------------------------------------------
 * Definitions used with the subprocess manager daemon variation
 *------------------------------------------------------------------------*/

#ifdef SPMD
#	include <sys/ipc.h>
#	include <sys/msg.h>
#	include <sys/stat.h> /* For S_IWUSR, S_IRUSR permission mode masks. */

#	ifdef DEBUG
#		if defined(__STDC__) && !defined(apollo)
#			define ASSERT(EX) if (!(EX)) \
			fprintf(stderr, \
			"Assert %s failed, File %s, Line %d\n", #EX, \
			       __FILE__, __LINE__)
#		else
#			define ASSERT(EX) if (!(EX)) \
			fprintf(stderr, \
			"Assert %s failed, File %s, Line %d\n", "EX", \
			       __FILE__, __LINE__)
#		endif /* (__STDC__) && !(apollo) */
#	else /*DEBUG*/
#		define ASSERT(EX)
#	endif /*DEBUG*/

typedef struct { 		/* Sub-Process Manager Daemon description */
	int spmd_pid;		/* process id */
	int spmd_cp_msqid;	/* daemon (child) -> app (parent) msg queue */
	int spmd_pc_msqid;	/* app (parent) -> daemon (child) msg queue */
	struct {		/* last message received from daemon */
		int  msgsiz;
		char *msgrp;	/* SPMD_OUTPUT ptr into spmd_output_t msg */
		struct msgbuf *msgp;
	} spmd_rcv_msg;
#define spmd_rmsgtyp spmd_rcv_msg.msgp->mtype
#define spmd_rmsgtxt spmd_rcv_msg.msgp->mtext
#define spmd_rmsgsiz spmd_rcv_msg.msgsiz
#define spmd_rmsgrp spmd_rcv_msg.msgrp
#define spmd_rmsgp spmd_rcv_msg.msgp
} spmd_info_t;

static spmd_info_t Spmd;

typedef struct { /* sub-process daemon pid-pty structure */
	int pid, pty, exit_status;
} spmd_pid_pty_t;

#define MAX_PROCS 20

static spmd_pid_pty_t 	SpmdStates[MAX_PROCS]; 
static int 		SpmdSubprocCount;
static int		SpmdStatesEmergencyPIDFlag=0;
static int		SpmdStatesEmergencyPIDStatus=0;

/*
 * message types:
 *	for protocol between application and Sub-Process Manager Daemon
 */
#define SPMD_CREATE	0x1
#define SPMD_INPUT	0x2	/* input (to sub-process, from application) */
#define SPMD_OUTPUT	0x4	/* output (from sub-process, to application) */
#define SPMD_DIED	0x8
#define SPMD_RSP	0x20	/* used to ACK or NACK _CREATE and/or _INPUT */

#ifndef MSGMAX
#	define MSGMAX 2048
#endif /*MSGMAX*/

#define SIZEOFMAXMSGBUF (MSGMAX + sizeof(long))

/*
 * message structures: these are the actual message formats used.
 */
typedef	struct { long mtype; int pid; char output[MSGMAX-10]; } spmd_output_t;
typedef	struct { long mtype; int pid; char input[MSGMAX-10]; }  spmd_input_t;
typedef	struct { long mtype; int echo; char cmd[MSGMAX-10]; } spmd_create_req;
typedef struct { long mtype; int pid; int response; } 	      spmd_create_rsp;
typedef	struct { long mtype; int pid; int status; } spmd_dead_msg;

#define ACK  3
#define NACK 12

static int spmd_read_msg();
static void spmd_ciao();

/* We can't select() on the msg queue, so we can't use XtAddInput ().
 * Instead we schedule a check every SpmdInterval millisecs. 
 * See spmd_read_msg()
 */
#define  SHORT_INTERVAL   100
#define  LONG_INTERVAL    500
static long SpmdInterval = LONG_INTERVAL;	

#else /*SPMD*/	

/*  When not using SPMD, we clean up dead child processes regularly,
 *  since SIGCHLD may be lost due to use of system().
 */

#define CHECK_INTERVAL 		((long) 10000)
#define CHILD_LOST 		99

static XtIntervalId 	timeoutid = 0;
static void 		CheckAllChildren();

#endif /*SPMD*/


#define error_msg(x)  UxStandardError("%s", x);

/* forward declarations - see UxSubproc.h for all others */

static void output_handler(), exception_handler();
static void sp_exit();

#ifdef _PTY_DRIVER
/* The following structures are initialized in main() in order
** to eliminate any assumptions about the internal order of their
** contents.
*/
#	ifdef USE_SYSV_TERMIO
		static struct termio d_tio;
		static struct termio tio;
#		ifdef TIOCSLTC
			static struct ltchars d_ltc;
			static struct ltchars ltc;
#		endif	/* TIOCSLTC */

#		ifdef TIOCLSET
			static unsigned int d_lmode;
			static unsigned int lmode;
#		endif	/* TIOCLSET */
#	else /* not USE_SYSV_TERMIO */
		static struct  sgttyb d_sg = {
        		0, 0, 0177, CKILL, EVENP|ODDP|ECHO|XTABS|CRMOD
			};
		static struct  sgttyb sg;
	
		static struct  tchars d_tc = {
        		CINTR, CQUIT, CSTART,
        		CSTOP, CEOF, CBRK,
			};
		static struct  tchars tc;

		static struct  ltchars d_ltc = {
        		CSUSP, CDSUSP, CRPRNT,
        		CFLUSH, CWERASE, CLNEXT
			};
		static struct  ltchars ltc;

		static int d_discipline = NTTYDISC;
		static int discipline;

		static long int d_lmode = LCRTBS|LCRTERA|LCRTKIL|LCTLECH;
		static long int lmode;
#	endif /* USE_SYSV_TERMIO */

#endif	/* _PTY_DRIVER */
/* Lorenzo 
extern  void    UxInternalError();
extern  void    UxStandardError();
extern  char    *UxMalloc();
extern  void    UxFree();
 Lorenzo */

#define CGETS(ms,ds_ms)                 UxCatGets(MC_MISC1,ms,ds_ms)

/****
 Funzioni sostituite da Lorenzo:
 sistemazione provvisoria
****/

int UxInternalError()
{
printf(" Errore Interno \n");
}

int UxStandardError(char * tipo, char * stringa)
{
printf("Errore standard: %s\n", stringa);
}

char *UxMalloc(int size)
{
return(XtMalloc(size));
}

char *UxRealloc(char *ptr, int size)
{
return(XtRealloc(ptr,size));
}

void UxFree(char *ptr)
{
XtFree(ptr);
}

void    UxTextAppend(w,str)
     Widget w;
     char *str;
{
        int     last_position;

        last_position = XmTextGetLastPosition(w);
        XmTextReplace(w, last_position, last_position, str);
       /*
        * get last position again so that last part of
        * message is visible in message window
        */
        last_position = XmTextGetLastPosition(w);
        XmTextShowPosition(w, last_position);
}


#include <nl_types.h>
static nl_catd  catd = (nl_catd)-1;

int UxCatOpen()
{
        catd = catopen(CAT_FILE, 0);
        return (int)(intptr_t)catd;
}


char    *UxCatGets(set_num, msg_num, default_str)
int     set_num;
int     msg_num;
char    *default_str;
{
    char *result;

    if (catd == (nl_catd)-1)
        return(default_str);
    result = catgets(catd, set_num, msg_num, default_str);
    if ( *result == '\0' )
        return(default_str);
    return( result);
}




/****
 Funzioni sostituite da Lorenzo:
 sistemazione provvisoria
	F I N E   
****/

	/* Provide 32 entries which is the maximum number of
         * of tty/pty pairs for one process on most extended 
	 * architectures. Pseudo terminals are only in use if
	 * a subprocess is between a Run - Exit cycle. Double
	 * the number of subprocess structures should suffice.
         * Refered to in the functions UxDeleteSubproc and UxGetSp.
	 */

#define MAX_SUBPROC	64
static subproc *sp_array[MAX_SUBPROC];


/* ---------------------------------------------------------------------------
NAME:		static void init_subproc(sp)
INPUT:		subproc *sp
RETURNS:	--
DESCRIPTION:	initialization of a newly created object
---------------------------------------------------------------------------*/

static int UxGetSp()
{
	/* declare register as int because of PS/2 compiler 		*/
	/* where the default register declarations are of type const 	*/
	register int i;

#ifdef SPMD
	/* Start the Sub-Process Manager Daemon 
	 * if it hasn't already been started.
	 */
	if ((Spmd.spmd_pid <= 0) && spmd_create_daemon()) {
		error_msg("Subprocess Manager Daemon cannot be started");
		return(ERROR);
	}
#endif /*SPMD*/

	/* look for a free slot in the array */
	for(i = 0; i < MAX_SUBPROC; i++)
	{
		if( sp_array[i] == (subproc *)0)
			break;
	}
	if(i == MAX_SUBPROC)
	{
		char *msg, *fmt;

		fmt = CGETS( MS_MISC_MAX_PROCS, DS_MS_MISC_MAX_PROCS);
		msg = UxMalloc(strlen(fmt) + 16 + 1);
		sprintf(msg,fmt,MAX_SUBPROC);
		error_msg(msg);
		UxFree(msg);
		return(ERROR);
	}
	else
	{
		if(! (sp_array[i] = (subproc *)calloc(1, sizeof(subproc))) )
		{
			UxInternalError(__FILE__, __LINE__,
			    CGETS( MS_MISC_CALLOC_ERR,DS_MS_MISC_CALLOC_ERR));
		        return ERROR;
                }
                else 
		{
#ifdef _PTY_DRIVER
			sp_array[i]->pty = -1;
#endif
#ifdef _STREAM_DRIVER
			sp_array[i]->stream = -1;
#endif
			return i;
		}
        }
} 

/******************************************************************************
NAME:		int		check_index(index)
INPUT:		handle		index		- index to subproc struct
RETURN:		ERROR/NO_ERROR
DESCRIPTION:	returns ERROR if invalid handle or entry not set
                and NO_ERROR if valid
-----------------------------------------------------------------------------*/

static int check_index(index)
handle index;
{
	if( (index < 0) || (index > 63) )
	{
#ifdef	RUNTIME
		error_msg( CGETS( MS_MISC_INV_HNDL, DS_MS_MISC_INV_HNDL));
#else
		UxError( CGETS( MS_MISC_INV_HNDL, DS_MS_MISC_INV_HNDL));
#endif
		return(ERROR);
	}
	if( !(sp_array[index]) )
	{
#ifdef	RUNTIME
		error_msg( CGETS( MS_MISC_NOT_HNDL, DS_MS_MISC_NOT_HNDL));
#else
		UxError( CGETS( MS_MISC_NOT_HNDL, DS_MS_MISC_NOT_HNDL));
#endif
		return(ERROR);
	}
	return(NO_ERROR);
}

/******************************************************************************
NAME:		int		UxSetSubprocExitCallback(index)
INPUT:		handle		h		- handle to subproc
		void		(*fnt)()	- exit callback function
RETURN:		ERROR or NO_ERROR
DESCRIPTION:	Delete the structure and set the entry to 0 
-----------------------------------------------------------------------------*/

int	UxSetSubprocExitCallback(h, fnt)
	handle	h;
	void	(*fnt)();
{
	subproc *sp;

	if(check_index(h) == ERROR)
		return(ERROR);

	sp = sp_array[h];
	sp->exit_cb = fnt;
	return(NO_ERROR);
}

/******************************************************************************
NAME:		int		UxDeleteSubproc(index)
INPUT:		handle		index		- index  into array of subprocs
RETURN:		ERROR or NO_ERROR
DESCRIPTION:	Delete the structure and set the entry to 0 
-----------------------------------------------------------------------------*/

int UxDeleteSubproc(index)
handle index;
{
	if(check_index(index) == ERROR)
		return ERROR;
	/* Exit the process if running and close tty/pty 
         * remove the XInput fd */

	UxExitSubproc(index);

	if(sp_array[index]->process)
		UxFree(sp_array[index]->process);

	if(sp_array[index]->defarg)
		UxFree(sp_array[index]->defarg);

	UxFree((char*)sp_array[index]);
	sp_array[index] = NULL;

	return NO_ERROR;
}

/******************************************************************************
NAME:		handle		UxCreateSubproc(process, defarg, fnt)
INPUT:		char		*process	- subprocess to be created
				*defarg		- default argument to the 
				                subprocess - overridden by 
						argument passed to UxRunSubproc
		void		(*fnt)()	- output function caused when 
		                                output present from subproc
RETURN:		subprocess handle = index into subproc arrray
DESCRIPTION:	creates a subprocess entry and returns a handle to it
-----------------------------------------------------------------------------*/

handle UxCreateSubproc(process, defarg, fnt)
     char *process, *defarg;
     void (*fnt)();
{
	subproc		*sp;
	int 		index;	
	static int 	initialized = 0;
	int 		putenv();
	
	if( process == NULL ) 
	{
		UxInternalError( __FILE__, __LINE__,
			CGETS( MS_MISC1_NULL_PROC_ERR, DS_MS_MISC1_NULL_PROC_ERR));
		return ERROR;
	}

	if(!initialized)
	{
		putenv("TERM=uimx");
		initialized = 1;
	}

	if( (index = UxGetSp()) == ERROR)
		return ERROR;

	sp = sp_array[index];
	
	if(! (sp->process = UxMalloc(strlen(process) + 1)) )
	{
		UxInternalError( __FILE__, __LINE__,
			CGETS( MS_MISC_MALLOC_ERR, DS_MS_MISC_MALLOC_ERR));
		UxFree( (char *) sp);
		sp_array[index] = NULL;
		return ERROR;
	}
	strcpy(sp->process, process);

	if(!defarg || (defarg[0] == '\0'))
		sp->defarg = (char *)0;
	else if(! (sp->defarg = UxMalloc(strlen(defarg) + 1)) )
	{
		UxInternalError( __FILE__, __LINE__,
			CGETS( MS_MISC_MALLOC_ERR, DS_MS_MISC_MALLOC_ERR));
		UxFree(sp->process);
		UxFree( (char *) sp);
		sp_array[index] = (subproc *)0;
		return ERROR;
	}
	
	if(defarg && (defarg[0] != '\0'))
	   strcpy(sp->defarg, defarg);

	sp->output_fnt = fnt;
	sp->exit_cb = 0;
	sp->echo= 0;

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) [UxCreateSubproc] handle=%d\n", getpid(),
		index);
#endif	/* DEBUG */
	
	return(index);
}

int UxSetSubprocEcho(sp_h, echo)
	handle		sp_h;
	int		echo;
{
	subproc		*sp;

	if(check_index(sp_h) == ERROR)
		return(ERROR);

	sp = sp_array[sp_h];

	sp->echo= echo;
	return NO_ERROR;
}

/* ---------------------------------------------------------------------------
NAME:		static void syserr(msg1, msg2)
INPUT:		char *msg1, *msg2; message to print.
RETURNS:	--
DESCRIPTION:	Generate an error message including the system errno.
---------------------------------------------------------------------------*/

static void syserr(msg1, msg2)
char *msg1;
char *msg2;
{
    int error;
    extern int errno, sys_nerr;
#if !defined (LINUX)
    extern char *sys_errlist[];
#endif
    char buffer[BUFSIZ];
    *buffer = '\0';
    error = errno;

    if (msg1 && *msg1) {
	(void) strcat(buffer, msg1);
	(void) strcat(buffer, ": ");
	if (msg2 && *msg2) {
	    (void) strcat(buffer, msg2);
	    (void) strcat(buffer, ": ");
	}
    }
    if(error > 0) {
	(void) strcat(buffer, strerror(errno));
	(void) strcat(buffer, "\n");
    } else {
	(void) strcat(buffer, CGETS( MS_MISC_SYS_ERR, DS_MS_MISC_SYS_ERR));
    }
    error_msg(buffer);
}

/* ---------------------------------------------------------------------------
NAME:		UxMarkFileCloseOnExec
INPUT:		NONE
OUTPUT:		NONE
RETURNS:	NONE
DESCRIPTION:	This function marks all the file descriptors above 2 
		to be closed for the child proc after 'exec' calls.

		This is used before running any standard scripts via system(),
		or subprocesses which involve pipes.

		Otherwise things start hanging if the child process
		cannot 'exec' properly, leading to wierd states on the X
		connection, and the pipes themselves.

CREATION:	14 Novembre 1991
--------------------------------------------------------------------------- */

void	UxMarkFileCloseOnExec()
{
	int	i;
#if defined(SYSV) 
	    for (i = 3; i < _NFILE; i++)
		(void) fcntl(i, F_SETFD, 1);
#else	/* SYSV */
	    for (i = getdtablesize(); i > 2; i--)
		(void) fcntl(i, F_SETFD, 1);
#endif	/* SYSV */
}

/* ---------------------------------------------------------------------------
NAME:		static void exit_handler()
DESCRIPTION:	Signal handler for death of child.
---------------------------------------------------------------------------*/

static	void	exit_handler(int sig)
{
	int	i, status, pid;


#ifdef SPMD
	spmd_dead_msg *msg = (spmd_dead_msg *)Spmd.spmd_rmsgp;

#ifdef DEBUG
	fprintf(stderr, "<<(%d) [exit_handler] ricevuto segnale %d pid %d died\n", getpid(), sig, msg->pid);
#endif /*DEBUG*/

#endif /*SPMD*/


#ifdef SPMD
	pid = msg->pid;
	status = msg->status;
#else /*SPMD*/
	pid = wait(&status);
#endif /*SPMD*/

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) calling wait\n", getpid());
	fprintf(stderr, ">>(%d) waited for pid %d\n", getpid(), pid);
#endif	/* DEBUG */

	if (pid != -1)
	{
	    for (i=0; i<MAX_SUBPROC; i++)
	    {
		if (!sp_array[i])
		    continue;
		if (sp_array[i]->pid == pid)
		{
		    if (sp_array[i]->exit_cb) {
#ifdef	DEBUG
			fprintf(stderr, ">>(%d) calling exit callback\n",
				getpid());
#endif	/* DEBUG */
                        (*sp_array[i]->exit_cb)(pid, status, i);
#ifdef	DEBUG
		    } else {
			fprintf(stderr, ">>(%d) subproc has no exit callback\n",
				getpid());
#endif	/* DEBUG */
		    }
           /*
	   *** sp_exit() leads to a call of XtRemoveInput() which 
	   *** is necessary when the subproc's connection is closed.
	   *** Otherwise, an infinite loop occurs as evidenced by
	   *** the calls to the function registered in XtAddInput().
	   */
#ifndef HP
		    sp_exit( sp_array[i] );
		    sp_array[i]->pid = 0;
#endif
		    break;
		}
	    }
	}
#ifdef HP
	(void) signal(SIGCHLD, exit_handler);
#endif
	return;
}

/*************************************************************************/
#ifdef _STREAM_DRIVER

static int UxGetStream()
{
	int		stream;

	stream= open("/dev/ptmx", O_RDWR | O_NDELAY);

	if(stream < 0)
		error_msg(CGETS( MS_MISC_STREAM_DRV_ERR, 
				DS_MS_MISC_STREAM_DRV_ERR));

	return(stream);
}

/* ---------------------------------------------------------------------------
NAME:		static int set_stream(m_stream, echo)
INPUT:		int m_srteam;		-- master stream
		int echo;
DESCRIPTION:	Set the stream.
---------------------------------------------------------------------------*/
static int set_stream(m_stream, echo)
	int		m_stream, echo;
{
	struct termios	term;
	char *slavename;
	extern char *ptsname();
	int	s_stream;
	
printf("set_stream:master stream = %d \n", m_stream); 
	setpgrp();

	grantpt(m_stream);
	unlockpt(m_stream);

	/* open the slave stream */
	slavename= ptsname(m_stream);
printf("set_stream:slavename = %d \n", slavename); 
	s_stream= open(slavename, O_RDWR);
printf("set_stream:slave stream = %d \n", s_stream); 

	if(s_stream < 0)
	{	syserr(NULL, NULL);
		return(ERROR);
	}
/* temporanea	
printf("set_stream:prima di ptem \n"); 

	if(ioctl(s_stream, I_PUSH, "ptem") < 0)
	{	syserr(NULL, NULL);
		return(ERROR);
	}


#if defined(OL_WIDGETS)
	if((getenv("CONSEM") == NULL) && (ioctl(s_stream, I_PUSH, "consem") < 0))
	{	syserr(NULL, NULL);
		return(ERROR);
	}
#endif /* (OL_WIDGETS) */
/* 
	if(ioctl(s_stream, I_PUSH, "ldterm") < 0)
	{	syserr(NULL, NULL);
		return(ERROR);
	}
printf("set_stream:prima di TCGETA \n"); 

	if(ioctl(s_stream, TCGETA, &term) < 0)
	{	syserr(NULL, NULL);
		return(ERROR);
	}

	if(!echo)
		term.c_lflag&= ~ECHO;

	term.c_oflag|= ONLRET;

	if(ioctl(s_stream, TCSETA, &term) < 0)
	{	syserr(NULL, NULL);
		return(ERROR);
	}
*/ 

	dup2(s_stream, 0);
	dup2(s_stream, 1);
	dup2(s_stream, 2);

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	return(NO_ERROR);
}

static int start_child(stream, echo, args)
	int		stream, echo;
	char		**args;
{
	int pid, i;

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) [start_child] inizio stream=%d \n", getpid(), stream);
#endif	/* DEBUG */
	switch(pid = fork()){
		case -1:
			syserr(NULL, NULL);
		case 0:

			if(set_stream(stream, echo) == ERROR)
				syserr(NULL, NULL);

			for(i = 19 ; i > 2; i--)
					close(i);

			execvp(args[0], args);

			syserr(NULL, NULL);
			_exit(1);
	}

	if (kill(pid, 0) && (errno != EPERM))
		return(ERROR);

	 return(pid);
}

/* ---------------------------------------------------------------------------
NAME:		static int exec_subprocess(sp, args)
INPUT:		subproc *sp -- the subprocess object 
                char **args -- the arguments
RETURNS:	ERROR / NO_ERROR
DESCRIPTION:	Get the stream, start the childprocess and set the
		characteristics of it. Set the stream as an input source to
		the Xtoolkit
CREATION:       Jun 12 1989  Re-coded for streams
---------------------------------------------------------------------------*/

static 	int 	exec_subprocess(sp, args)
     	subproc *sp;
     	char **args;
{
	int pid;
	int stream;
	void	exit_handler();

	stream= UxGetStream();

	if(stream < 0)
		return(ERROR);

	if( (pid = start_child(stream, sp->echo, args)) == ERROR)
	{
		close(stream);
		return(ERROR);
	}

	if (sp->exit_cb) {
		signal(SIGCHLD, exit_handler);
	}

	sp->stream = stream;
	sp->pid = pid;

	return(NO_ERROR);
}

#endif /* _STREAM_DRIVER */

#ifdef _PTY_DRIVER
static jmp_buf env;
static hungtty()
{
	longjmp(env, 1);
}

/*************************************************************************/


/* ---------------------------------------------------------------------------
NAME:		static int UxGetPtty(pty, reset)
INPUT:		int *pty, reset;
RETURNS:	ERROR/NO_ERROR
DESCRIPTION:	Find an available pty/tty line and open it,
                setting the passed variable.  This function is based on the
		X11 xterm and hpterm get_pty routine.  It does not open the
		pty slave because it must be opened in the child process
		in order to prevent possible process group problems.  For
		this reason, it is designed to be reentrant to allow it
		to be called repeatedly in the event that the child process
		is unable to open the child pty.

		It is also written to be easily ported to different
		architectures that support different pty naming schemes.
 
 		The master tty file device name is left in global ptydev.
 		The slave tty file device name is left in global ttydev.
 
 		A separate block (currently used only for sgi)
 		handles the case where one master device is always opened,
 		and the minor number of the result identifies the slave.
		
		IRIX support added 10 Oct 90 [code change 1085]
---------------------------------------------------------------------------*/
struct _pty_dirs {
    char *pty_dir;	/* pty slave directory...			*/
    char *ptym_dir;	/* pty master directory...			*/
    char *char_1;	/* string of first characters in pty name...	*/
    char *char_2;	/* string of second characters in pty name...	*/
    char *char_3;	/* string of third characters in pty name...	*/
    int fast;		/* true->skip all if first does not exist...	*/
};
#ifndef HP
static char PTY_dev[]		= "/dev";
static char PTY_0_9a_f[]	= "0123456789abcdef";
static char PTY_p_r[]		= "pqr";
#define	PTY_null		(char *) 0

static struct _pty_dirs pty_dirs[] = {
    {PTY_dev,     PTY_dev,      PTY_p_r,    PTY_0_9a_f, PTY_null, False},
    {PTY_null,    PTY_null,     PTY_null,   PTY_null,   PTY_null, False},
};
#else	/* HP */
static char PTY_dev[]		= "/dev";
static char PTY_dev_pty[]	= "/dev/pty";
static char PTY_dev_ptym[]	= "/dev/ptym";
static char PTY_a_ce_o[]	= "abcefghijklmno";
static char PTY_0_9[]		= "0123456789";
static char PTY_0_9a_f[]	= "0123456789abcdef";
static char PTY_p_z[]		= "pqrstuvwxyz";
static char PTY_p_r[]		= "pqr";
#define		PTY_null	(char *) 0

static struct _pty_dirs pty_dirs[] = {
    {PTY_dev_pty, PTY_dev_ptym, PTY_a_ce_o, PTY_0_9,    PTY_0_9,  True},
    {PTY_dev_pty, PTY_dev_ptym, PTY_p_z,    PTY_0_9,    PTY_0_9,  True},
    {PTY_dev_pty, PTY_dev_ptym, PTY_a_ce_o, PTY_0_9a_f, PTY_null, True},
    {PTY_dev_pty, PTY_dev_ptym, PTY_p_z,    PTY_0_9a_f, PTY_null, False},
    {PTY_dev,     PTY_dev,      PTY_p_r,    PTY_0_9a_f, PTY_null, False},
    {PTY_null,    PTY_null,     PTY_null,   PTY_null,   PTY_null, False},
};
#endif	/* HP */

static char *ttydev = (char *) 0;
static char *ptydev = (char *) 0;

static int UxGetPtty(pty, reset)
int *pty;
int reset;
{
    static struct _pty_dirs *pty_dirs_ptr;
    static char *char_1;
    static char *char_2;
    static char *char_3;
    static int first = 1;
    static int initialize = 1;
    int tty;
#if defined(sgi)
    struct stat sb;
    int ptynum;
#endif
 

    if (reset || initialize) {
	/* set up pointers into pty filename structures stuff... */
	pty_dirs_ptr = pty_dirs;
	char_1 = pty_dirs_ptr->char_1;
	char_2 = pty_dirs_ptr->char_2;
	char_3 = pty_dirs_ptr->char_3;
    }

    if (initialize) {
	/* initialize terminal modes... */
#ifdef USE_SYSV_TERMIO
	/* Initialization is done here rather than above in order
	** to prevent any assumptions about the order of the contents
	** of the various terminal structures (which may change from
	** implementation to implementation).
	*/
#ifdef macII
	d_tio.c_iflag = ICRNL|IXON;
	d_tio.c_oflag = OPOST|ONLCR|TAB3;
    	d_tio.c_cflag = B9600|CS8|CREAD|PARENB|HUPCL;
    	d_tio.c_lflag = ISIG|ICANON|ECHO|ECHOE|ECHOK;

	d_tio.c_line = 0;

	d_tio.c_cc[VINTR] = CINTR;
	d_tio.c_cc[VQUIT] = CQUIT;
	d_tio.c_cc[VERASE] = CERASE;
	d_tio.c_cc[VKILL] = CKILL;
    	d_tio.c_cc[VEOF] = CEOF;
	d_tio.c_cc[VEOL] = CNUL;
	d_tio.c_cc[VEOL2] = CNUL;
	d_tio.c_cc[VSWTCH] = CNUL;

        d_ltc.t_suspc = CSUSP;		/* t_suspc */
        d_ltc.t_dsuspc = CDSUSP;	/* t_dsuspc */
        d_ltc.t_rprntc = 0;		/* reserved...*/
        d_ltc.t_flushc = 0;
        d_ltc.t_werasc = 0;
        d_ltc.t_lnextc = 0;
#else  /* macII */
	d_tio.c_iflag = ICRNL|IXON|IXOFF;
	d_tio.c_oflag = OPOST|ONLCR|TAB3;
#ifdef BAUD_0
    	d_tio.c_cflag = CS8|CREAD|PARENB|HUPCL;
	/*  Needed to turn on pacing from the pty to _subproc.c.  
	 *  This will cause the program generating output to block 
	 *  if the pty input buffer fills up.
	 */
#else	/* !BAUD_0 */
    	d_tio.c_cflag = B9600|CS8|CREAD|PARENB|HUPCL;
#endif	/* !BAUD_0 */
    	d_tio.c_lflag = ISIG|ICANON|ECHO|ECHOE|ECHOK;
	d_tio.c_line = 0;
	d_tio.c_cc[VINTR] = 0x7f;		/* DEL  */
	d_tio.c_cc[VQUIT] = '\\' & 0x3f;	/* '^\'	*/
	d_tio.c_cc[VERASE] = '#';		/* '#'	*/
	d_tio.c_cc[VKILL] = '@';		/* '@'	*/
    	d_tio.c_cc[VEOF] = 'D' & 0x3f;		/* '^D'	*/
	d_tio.c_cc[VEOL] = '@' & 0x3f;		/* '^@'	*/
#ifdef VSWTCH
	d_tio.c_cc[VSWTCH] = '@' & 0x3f;	/* '^@'	*/
#endif	/* VSWTCH */
	/* now, try to inherit tty settings */
	{
	    int i;

	    for (i = 0; i <= 2; i++) {
		struct termio deftio;
		if (ioctl (i, TCGETA, &deftio) == 0) {
		    d_tio.c_cc[VINTR] = deftio.c_cc[VINTR];
		    d_tio.c_cc[VQUIT] = deftio.c_cc[VQUIT];
		    d_tio.c_cc[VERASE] = deftio.c_cc[VERASE];
		    d_tio.c_cc[VKILL] = deftio.c_cc[VKILL];
		    d_tio.c_cc[VEOF] = deftio.c_cc[VEOF];
		    d_tio.c_cc[VEOL] = deftio.c_cc[VEOL];
#ifdef VSWTCH
		    d_tio.c_cc[VSWTCH] = deftio.c_cc[VSWTCH];
#endif /* VSWTCH */
		    break;
		}
	    }
	}
#ifdef TIOCSLTC
        d_ltc.t_suspc = '\000';		/* t_suspc */
        d_ltc.t_dsuspc = '\000';	/* t_dsuspc */
        d_ltc.t_rprntc = '\377';	/* reserved...*/
        d_ltc.t_flushc = '\377';
        d_ltc.t_werasc = '\377';
        d_ltc.t_lnextc = '\377';
#endif	/* TIOCSLTC */
#ifdef TIOCLSET
	d_lmode = 0;
#endif	/* TIOCLSET */
#endif  /* macII */
#endif	/* USE_SYSV_TERMIO */

	initialize = 0;

	/* Try to inherit modes from /dev/tty...
	 */

	/*
	 * Sometimes /dev/tty hangs on open (as in the case of a pty
	 * that has gone away).  Simply make up some reasonable
	 * defaults.
	 */
#ifdef SPMD
     {  void (*save_func)() =
#endif /*SPMD*/
	signal(SIGALRM, hungtty);
	alarm(2);		/* alarm(1) might return too soon */

	if (! setjmp(env)) {
	    tty = open ("/dev/tty", O_RDWR, 0);
	    alarm(0);
	} else {
	    tty = -1;
	    errno = ENXIO;
	}
#ifdef SPMD
	signal(SIGALRM, save_func);
	/*
	 * We should probably save/restore the alarm() value also,
	 * in case the application is using alarms and using UxGetPtty().
	 */
     }
#else /*SPMD*/
	signal(SIGALRM, SIG_DFL);
#endif /*SPMD*/

	if (tty < 0) {
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) open of /dev/tty failed\n", getpid());
#endif	/* DEBUG */
#ifdef USE_SYSV_TERMIO
	    tio = d_tio;
#ifdef TIOCSLTC
	    ltc = d_ltc;
#endif	/* TIOCSLTC */
#ifdef TIOCLSET
	    lmode = d_lmode;
#endif	/* TIOCLSET */
#else	/* not USE_SYSV_TERMIO */
	    sg = d_sg;
	    tc = d_tc;
	    discipline = d_discipline;
	    ltc = d_ltc;
	    lmode = d_lmode;
#endif	/* USE_SYSV_TERMIO */
	} else {
	    /* Get a copy of the current terminal's state...
	     */
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) getting /dev/tty's modes 1\n", getpid());
#endif	/* DEBUG */

#ifdef USE_SYSV_TERMIO
	    if (ioctl(tty, TCGETA, &tio) == -1)
		tio = d_tio; /* should probably print an error message... */
#ifdef TIOCSLTC
	    if (ioctl(tty, TIOCGLTC, &ltc) == -1)
		ltc = d_ltc; /* should probably print an error message... */
#endif	/* TIOCSLTC */
#ifdef TIOCLSET
	    if (ioctl(tty, TIOCLGET, &lmode) == -1)
		lmode = d_lmode; /* should probably print an error message... */
#endif	/* TIOCLSET */
#else	/* not USE_SYSV_TERMIO */
	    if (ioctl(tty, TIOCGETP, (char *)&sg) == -1)
		sg = d_sg; /* should probably print an error message... */
	    if (ioctl(tty, TIOCGETC, (char *)&tc) == -1)
		tc = d_tc;; /* should probably print an error message... */
	    if (ioctl(tty, TIOCGETD, (char *)&discipline) == -1)
		/* should probably print an error message... */
		discipline = d_discipline;
	    if (ioctl(tty, TIOCGLTC, (char *)&ltc) == -1)
		ltc = d_ltc;; /* should probably print an error message... */
	    if (ioctl(tty, TIOCLGET, (char *)&lmode) == -1)
		lmode = d_lmode; /* should probably print an error message... */
#endif	/* USE_SYSV_TERMIO */
	    (void) close (tty);
	}
    }

    /* Find a master/slave device pair. 
     */
#ifdef UNIX98_PTY /* unix98 like implementation using pseudo-tty multiplexer /dev/ptmx */
			
    if (*pty = getpt() == -1) {
    	perror("_subproc:UxGetPtty:getpt()");
	exit(11);
	}
#ifdef	DEBUG
	(void) fprintf(stderr,
	">>(%d) successfully opened ptmx pty=%d\n",
	getpid(), *pty);
#endif	/* DEBUG */
    if (grantpt(*pty) == -1) {
    	perror("_subproc:UxGetPtty:grantpt()");
	exit(12);
	}	
    if (unlockpt(*pty) == -1) {
    	perror("_subproc:UxGetPtty:grantpt()");
	exit(13);
	}	return(NO_ERROR);			
#else

    /* On HP (and many other machines) pty master/slaves
     * are opened as two devices with parallel names,
     * such as /dev/ptym/pty38 and /dev/pty/tty38
     */

    /* as long as these are valid, we have a good pair... */
    while (pty_dirs_ptr->pty_dir && pty_dirs_ptr->ptym_dir) {
	/* allocate space for the pty slave... */
	if (ttydev)
	    (void) UxFree(ttydev);
	ttydev = UxMalloc((unsigned) strlen(pty_dirs_ptr->pty_dir) + 8);

	/* allocate space for the pty master... */
	if (ptydev)
	    (void) UxFree(ptydev);
	ptydev = UxMalloc((unsigned) strlen(pty_dirs_ptr->ptym_dir) + 8);

	/* check malloc success... */
	if (!ttydev || !ptydev) {
	    UxInternalError(__FILE__, __LINE__,
		CGETS( MS_MISC_MALLOC_ERR, DS_MS_MISC_MALLOC_ERR));
	    return(ERROR);
	}

	while ((first || !pty_dirs_ptr->fast) && *char_1) {
	    while ((first || !pty_dirs_ptr->fast) && *char_2) {
		if ((first || !pty_dirs_ptr->fast) && char_3) {
		    while ((first || !pty_dirs_ptr->fast) && *char_3) {

			(void) sprintf(ttydev, "%s/tty%c%c%c",
				pty_dirs_ptr->pty_dir,
				*char_1, *char_2, *char_3);
			(void) sprintf(ptydev, "%s/pty%c%c%c",
				pty_dirs_ptr->ptym_dir,
				*char_1, *char_2, *char_3);

			if ((*pty = open(ptydev, O_RDWR)) >= 0) {
			    /* we need to set things up for our next entry
			     * into this function...
			     */
			    (void) char_3++;
			    /* success... */

#ifdef	DEBUG
			    (void) fprintf(stderr,
				   ">>(%d) successfully opened pty \"%s\"\n",
				   getpid(), ptydev);
#endif	/* DEBUG */
			    return(NO_ERROR);
			} else {
			    /* if the device is busy, then we should keep
			     * going until we hit one that doesn't exist...
			     */
			    if (errno != EBUSY)
				first = False;
#ifdef	DEBUG
			    (void) fprintf(stderr,
				    ">>(%d) open of pty 1\"%s\" failed: %s\n",
				    getpid(), ptydev, sys_errlist[errno]);
#endif	/* DEBUG */
			}
			(void) char_3++;
		    }
		} else {
		    (void) sprintf(ttydev, "%s/tty%c%c",
			    pty_dirs_ptr->pty_dir,
			    *char_1, *char_2);
		    (void) sprintf(ptydev, "%s/pty%c%c",
			    pty_dirs_ptr->ptym_dir,
			    *char_1, *char_2);
		    if ((*pty = open(ptydev, O_RDWR)) >= 0) {
			/* we need to set things up for our next entry into
			 * this function!
			 */
			(void) char_2++;
			/* success... */
#ifdef	DEBUG
			(void) fprintf(stderr,
			       ">>(%d) successfully opened pty \"%s\"\n",
			       getpid(), ptydev);
#endif	/* DEBUG */
			return(NO_ERROR);
		    } else {
			/* if the device is busy, then we should keep going
			 * until we hit one that doesn't exist...
			 */
			if (errno != EBUSY)
			    first = False;
#ifdef	DEBUG
			(void) fprintf(stderr,
				">>(%d) open of pty 2\"%s\" failed: %s\n",
				getpid(), ptydev, sys_errlist[errno]);
#endif	/* DEBUG */
		    }
		}
		char_3 = pty_dirs_ptr->char_3;
		(void) char_2++;
	    }
	    char_2 = pty_dirs_ptr->char_2;
	    (void) char_1++;
	}
	first = True;
	(void) pty_dirs_ptr++;
	char_1 = pty_dirs_ptr->char_1;
	char_2 = pty_dirs_ptr->char_2;
	char_3 = pty_dirs_ptr->char_3;
    }
    
#endif	/* UNIX98_PTY */
	
    /* we were unable to allocate a pty master... */
    error_msg(CGETS( MS_MISC_PTY_DRV_ERR, DS_MS_MISC_PTY_DRV_ERR));
    return(ERROR);

}



/* The following enum is used for communicating between the child and parent
 * processes.  The child (c->p) process passes back to the parent success or
 * failure on pty slave opens.  The parent (p->c) passes back to the child
 * either a new pty slave name, or a "sorry, no more ptys -- terminate"
 * message.
 */

typedef enum {
	PTY_BAD,	/* c->p: can't open pty slave for some reason	*/
	PTY_GOOD,	/* c->p: we have a good pty, let's go on	*/
	PTY_NEW,	/* p->c: here is a new pty slave, try this	*/
	PTY_NOMORE,	/* p->c: no more pty's, terminate		*/
	SUBPROC_ERROR,	/* c->p: here is an error msg to display	*/
	SUBPROC_EXEC_ERROR,
			/* c->p: unable to exec subprocess		*/
} status_t;

typedef struct {
	status_t status;	/* packet type...			*/
	int error;		/* open error...			*/
	char buffer[1024];	/* name of pty slave...			*/
} handshake_t;

static int pc_pipe[2];		/* parent to child communication...	*/
static int cp_pipe[2];		/* child to parent communication...	*/

/* ---------------------------------------------------------------------------
NAME:		static void pass_syserr(msg1, msg2)
INPUT:		char *msg1, *msg2 -- message to print
RETURNS:	--
DESCRIPTION:	This function is called by the child process to pass back
		to the parent an error message.
CREATION:       Feb 22, 1990
---------------------------------------------------------------------------*/

static void pass_syserr(msg1, msg2)
char *msg1;
char *msg2;
{
    handshake_t handshake;

    /* let our master know that the open failed... */
    handshake.status = SUBPROC_ERROR;
    handshake.error = errno;
    if (msg1) {
	(void) strcpy(handshake.buffer, msg1);
	if (msg2) {
	    (void) strcat(handshake.buffer, ": ");
	    (void) strcat(handshake.buffer, msg2);
	}
    } else {
	*handshake.buffer = 0;
    }
#ifdef	DEBUG
    fprintf(stderr, "<<(%d)  sending SUBPROC_ERROR message\n", getpid());
    fflush(stderr);
#endif	/* DEBUG */
    (void) write(cp_pipe[1], (char *) &handshake, sizeof(handshake));
    return;
}

/* ---------------------------------------------------------------------------
NAME:		static int start_child(pty, echo, args)
INPUT:		int pty -- master pty (for child to close)
		int echo -- echo on or off
		char **args -- the argument list
RETURNS:	ERROR/NO_ERROR
DESCRIPTION:	fork the process and exec a new process with the args
                passed. 
---------------------------------------------------------------------------*/

static int start_child(pty, echo, args)
int pty;
int echo;
char **args;
{
	extern Display *UxDisplay;
	int pid, i;
	int tty;
	handshake_t handshake;
	static int first = 1;
	static int force_failures = 0;
	int failures = 5;

	if (first) {
	    if (getenv("SUBPROC_PTYOPEN_FAIL"))
		force_failures = 1;
	    first = 0;
	}

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) forking!\n", getpid());
#endif	/* DEBUG */
	for (i = 0; (i < 10) && ((pid = fork()) < 0); i++) {
	    /* out of resources?... */
	    if (errno != EAGAIN)
		/* no!... */
		break;

	    /* give things a chance to clear up... */
	    (void) sleep((unsigned long) 2);
	}
#ifdef	DEBUG
	fprintf(stderr, ">>(%d) mi sono forkato con i=%d!\n", getpid(), i);
#endif	/* DEBUG */
	if (pid < 0) {
	    syserr(args[0], NULL);
	    return(ERROR);
	}

	if (pid == 0) {
	    int pgrp = getpid();

	    /* child process...
	     */
#ifdef	DEBUG
	    /* First off, let's protect ourself.  We do not want to have any
	     * connection to the pty master, or we may create a problem if
	     * our parent process dies.  Also, we do not want to have the
	     * X socket open because we may end up hurting ourself in some
	     * manner.
	     */
	    (void) close(pty);
	    (void) close(ConnectionNumber(UxDisplay));

	    /* Set up file for use in logging...
	     */
	    /* Dup stderr outside range of 0, 1, 2... */
	    i = fcntl(2, F_DUPFD, 3);

	    /* Open up a file... */
	    DebugLog = fdopen(i, "w");

	    /* Turn on line buffering (just to make sure everything will
	     * get out...
	     */
	    (void) setvbuf(DebugLog, (char *) 0, _IOLBF, BUFSIZ);
#endif	/* DEBUG */

	    /* close the parent's sides of the pipes... */
	    (void) close(cp_pipe[0]);
	    (void) close(pc_pipe[1]);

	    /* make sure the pipes are outside the range of 0, 1, 2... */
	    if (cp_pipe[1] < 3) {
		i = fcntl(cp_pipe[1], F_DUPFD, 3);
		if (i >= 0) {
		    (void) close(cp_pipe[1]);
		    cp_pipe[1] = i;
		}
	    }

	    if (pc_pipe[0] < 3) {
		i = fcntl(pc_pipe[0], F_DUPFD, 3);
		if (i >= 0) {
		    (void) close(pc_pipe[0]);
		    pc_pipe[0] = i;
		}
	    }

#ifdef	USE_SYSV_PGRP
	    /* now, let's kick ourself into a new process group... */
	    (void) setpgrp();
#endif	/* USE_SYSV_PGRP */

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di while (1) \n", getpid());
#endif	/* DEBUG */
	    /* now it is safe to go and open up the pty slave... */
	    while (1) {
		/* The following code allows debugging of slave side
		 * problems openning up the pty slave.
		 */
		if (force_failures && (failures-- > 0)) {
#ifdef	DEBUG
		    (void) fprintf(DebugLog, "<<(%d)  forced open failure\n",
			    getpid());
		    fflush(DebugLog);
#endif	/* DEBUG */
		    tty = -1;
		    errno = EACCES;
		} else {
		    tty = open(ttydev, O_RDWR, 0);
		}

		if (tty >= 0) {
#ifdef	USE_SYSV_PGRP
		    /* We need to make sure that we are actually the
		     * process group leader for the pty.  If
		     * we are, then we should now be able to open
		     * /dev/tty...
		     */
		    if ((i = open("/dev/tty", O_RDWR, 0)) >= 0) {
			/* success!... */
			(void) close(i);
			break;
		    }
#else	/* USE_SYSV_PGRP */
		    break;
#endif	/* USE_SYSV_PGRP */
		}

		/* We were unable to open up a pty.  Inform our master, and
		 * wait for a new one...
		 */
		handshake.status = PTY_BAD;
		handshake.error = errno;
		(void) strcpy(handshake.buffer, ttydev);
#ifdef	DEBUG
		fprintf(DebugLog, 
			"<<(%d)  sending PTY_BAD message\n", getpid());
		fflush(DebugLog);
#endif	/* DEBUG */
		(void) write(cp_pipe[1], (char *) &handshake,
			sizeof(handshake));
		
		/* get reply from parent... */
#ifdef	DEBUG
		fprintf(DebugLog, "<<(%d)  reading reply\n", getpid());
		fflush(DebugLog);
#endif	/* DEBUG */
		if (read(pc_pipe[0], (char *) &handshake, sizeof(handshake)) <
			sizeof(handshake)) {
		    /* parent terminated... */
		    (void) _exit(1);
		}

		if (handshake.status == PTY_NOMORE) {
#ifdef	DEBUG
		    fprintf(DebugLog, "<<(%d)  received PTY_NOMORE message\n",
			    getpid());
		    fflush(DebugLog);
		    fclose(DebugLog);
#endif	/* DEBUG */
		    /* No more ptys, let's shutdown... */
		    (void) _exit(1);
		}

		/* We have a new pty to try... */
		(void) UxFree(ttydev);
		ttydev = UxMalloc((unsigned) (strlen(handshake.buffer) + 1));
		if (!ttydev) {
		    (void) _exit(1);
		}

		(void) strcpy(ttydev, handshake.buffer);
#ifdef	DEBUG
		fprintf(DebugLog, 
			"<<(%d)  got new pty \"%s\"\n", getpid(), ttydev);
		fflush(DebugLog);
#endif	/* DEBUG */
	    }

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) dopo while (1) \n", getpid());
#endif	/* DEBUG */

	    /* Good open...
	     * Set up the pty slave... 
	     */
#ifdef USE_SYSV_TERMIO
#ifdef sgi
	    /* If the control tty had its modes screwed around with,
	       eg. by lineedit in the shell, or emacs, etc. then tio
	       will have bad values.  Let's just get termio from the
	       new tty and tailor it.  */
	    if (ioctl (tty, TCGETA, &tio) == -1)
	        pass_syserr(ttydev, "TCGETA");
	    tio.c_lflag |= ECHOE;
#endif /* sgi */

	    /* Now is also the time to change the modes of the
	     * child pty.
	     */
	    /* input: nl->nl, don't ignore cr, cr->nl */
	    tio.c_iflag &= ~(INLCR|IGNCR);
	    tio.c_iflag |= ICRNL;
	    /* ouput: cr->cr, nl is not return, no delays, ln->cr/nl */
	    tio.c_oflag &=
		    ~(OCRNL|ONLRET|NLDLY|CRDLY|TABDLY|BSDLY|VTDLY|FFDLY);
	    tio.c_oflag |= ONLCR;
#ifdef BAUD_0
	    /* baud rate is 0 (don't care) */
	    tio.c_cflag &= ~(CBAUD);
#else	/* !BAUD_0 */
	    /* baud rate is 9600 (nice default) */
	    tio.c_cflag &= ~(CBAUD);
	    tio.c_cflag |= B9600;
#endif	/* !BAUD_0 */
	    /* enable signals, canonical processing (erase, kill, etc),
	    ** echo
	    */
	    tio.c_lflag |= ISIG|ICANON|ECHO;
	    if (!echo)
		tio.c_lflag &= ~ECHO;
	    /* reset EOL to defalult value */
	    tio.c_cc[VEOL] = '@' & 0x3f;		/* '^@'	*/
	    /* certain shells (ksh & csh) change EOF as well */
	    tio.c_cc[VEOF] = 'D' & 0x3f;		/* '^D'	*/
	    if (ioctl (tty, TCSETA, &tio) == -1)
		; /* should probably print an error message... */
#ifdef TIOCSLTC
	    if (ioctl (tty, TIOCSLTC, &ltc) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCSLTC");
	    }
#endif	/* TIOCSLTC */
#ifdef TIOCLSET
	    if (ioctl (tty, TIOCLSET, (char *)&lmode) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCLSET");
	    }
#endif	/* TIOCLSET */
#else	/* USE_SYSV_TERMIO */
	    sg.sg_flags &= ~(ALLDELAY | XTABS | CBREAK | RAW);
	    sg.sg_flags |= ECHO | CRMOD;
	    if (!echo)
		sg.sg_flags &= ~ECHO;
	    /* make sure speed is set on pty so that editors work right*/
	    sg.sg_ispeed = B9600;
	    sg.sg_ospeed = B9600;
	    /* reset t_brkc to default value */
	    tc.t_brkc = -1;

	    if (ioctl (tty, TIOCSETP, (char *)&sg) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCSETP");
	    }
	    if (ioctl (tty, TIOCSETC, (char *)&tc) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCSETC");
	    }
	    if (ioctl (tty, TIOCSETD, (char *)&discipline) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCSETD");
	    }
	    if (ioctl (tty, TIOCSLTC, (char *)&ltc) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCSLTC");
	    }
	    if (ioctl (tty, TIOCLSET, (char *)&lmode) == -1) {
		/* print an error message... */
	        pass_syserr(ttydev, "TIOCLSET");
	    }
#endif	/* !USE_SYSV_TERMIO */

	    /* set up stdin, out, err to pty slave... */
	    for (i = 0; i < 3; i++) {
		if (i != tty) {
		    (void) close(i);
		    (void) dup(tty);
		}
	    }

#ifndef	USE_SYSV_PGRP
	    (void) ioctl(0, TIOCSPGRP, (char *)&pgrp);
	    (void) setpgrp(0, 0);
	    (void) close(open(ttydev, O_WRONLY, 0));
	    (void) setpgrp(0, pgrp);
#endif	/* USE_SYSV_PGRP */

#ifdef	DEBUG
	    /* let's check our pgrp... */
	    if (getpgrp() != getpid()) {
		fprintf(DebugLog, 
			"<<(%d)  WRONG PGRP!!! %d\n", getpid(), getpgrp());
		fflush(DebugLog);
	    }
	    /* let's check the process group leader of stdin... */
	    {
		int pgrp_leader;

		if (ioctl(0, TIOCGPGRP, &pgrp_leader)) {
		    fprintf(DebugLog, "<<(%d)  TIOCPGRP failed: %s\n", getpid(),
			    sys_errlist[errno]);
		} else if (pgrp_leader != getpid()) {
		    fprintf(DebugLog, "<<(%d)  WRONG TTY PGRP LEADER!!! %d\n",
			    getpid(), pgrp_leader);
		}
		fflush(DebugLog);
	    }
#endif	/* DEBUG */
		
	    /* mark all other files as close on exec... */
	UxMarkFileCloseOnExec();
#ifdef DEBUG
	fprintf(DebugLog, "<<(%d) UxMarkFileCloseOnExec prima di execvp args[0]=%s \n", getpid(), args[0]);
	fflush(DebugLog);
	fclose(DebugLog);
#endif
	    (void) execvp(args[0], args);

	    pass_syserr(args[0], NULL);
	    /* Added a new handshake type to let the master know that we 
	     * were unable to exec the program and we should return an error.  
	     * Previously, this information was lost and we returned NO_ERROR.
	     */
	    handshake.status = SUBPROC_EXEC_ERROR;
	    (void) strcpy(handshake.buffer, args[0]);
#ifdef	DEBUG
	    fprintf(stderr, "<<(%d)  sending SUBPROC_EXEC_ERROR message\n",
		    getpid());
	    fflush(stderr);
#endif	/* DEBUG */
	    (void) write(cp_pipe[1], (char *) &handshake, sizeof(handshake));
	    _exit(1);
	}
    /* If the exec fails, tell the master process.  
     * This way it can return an error.
     */
    if (kill(pid, 0) && (errno != EPERM))
      return(ERROR);
    else
      return(pid);
}

/* ---------------------------------------------------------------------------
NAME:		static int set_pseudo(pty)
INPUT:		int pty -- the pseudo terminal line
RETURNS:	ERROR/NO_ERROR
DESCRIPTION:	Set the characteristic fo the pseudo terminal pair.
Note:           This routine is the most likely to have portability 
                problems.
---------------------------------------------------------------------------*/

static int set_pseudo(pty)
	int		pty;

{
	static int on_flag = 1;
	static int off_flag = 0;
	static int *on;
	static int *off;
	
	int n;
	int zero=0;
	
	on = &on_flag; 
	off = &off_flag;

#if !defined(sgi) && !defined (LINUX)
	/* set master side */
	if(ioctl(pty, TIOCREMOTE, off) == -1){
		syserr(ptydev, "TIOCREMOTE");
		return(ERROR);
	}
#endif

#ifdef UNIX98_PTY /* unix98 like implementation using pseudo-tty multiplexer /dev/ptmx */
	/* Don't use packet mode */
	if(ioctl(pty, TIOCGPTN, &n) == -1){
		syserr("ptmx", "TIOCGPTN");
		return(ERROR);
	}

	/* Use non-blocking IO */
	if(ioctl(pty, TIOCSPTLCK, &zero) == -1){
		syserr(ptydev, "TIOCSPTLCK");
		return(ERROR);
	}
#else
	/* Don't use packet mode */
	if(ioctl(pty, TIOCPKT, off) == -1){
		syserr(ptydev, "TIOCPKT");
		return(ERROR);
	}

	/* Use non-blocking IO */
	if(ioctl(pty, FIONBIO, on) == -1){
		syserr(ptydev, "FIONBIO");
		return(ERROR);
	}
#endif /* UNIX98_PTY */

	/* Don't use asynchronous IO */
#if !defined(sgi) && !defined(magnum)
	if(ioctl(pty, FIOASYNC, off) == -1){
		syserr(ptydev, "FIOASYNC");
		return(ERROR);
	}
#endif	

	return(NO_ERROR);
}


#ifdef SPMD
/* ---------------------------------------------------------------------------
NAME:		static int spmd_exec_subprocesses(char *cmd)
INPUT:		char *cmd -- the full command line
RETURNS:	ERROR || 0 <= index into SpmdStates[] < MAX_PROCS

DESCRIPTION:	Get the pseudo terminal pair, sets the characteristics
                of it, start the childprocess and sets the output
		side (pty) side as an Input Source to the Xtoolkit.

		This function is much the same under SPMD in the manager
		daemon as it is in the application itself without SPMD.
		It is renamed under SPMD because the application's function,
		exec_subprocess, has the same calling interface
		in both versions, but the SPMD version of exec_subprocess
		works by sending the daemon a message to run this function.

		The main difference between the SPMD and non-SPMD code
		is that under SPMD, this function receives the cmd string
		and expands it into args[], while in the standard version 
		that is done by the caller.  Under SPMD, it easier
		to expand the command line here than try to pass args[]
		through the message queue.
---------------------------------------------------------------------------*/

static int spmd_exec_subprocess(cmd, echo)
	char *cmd;
	int echo;
{
	char *proc, *proc_args, **args, **build_args();
	int i, finished;

#else /*SPMD*/

/* ---------------------------------------------------------------------------
NAME:		static int exec_subprocesses(sp, args)
INPUT:		subproc *sp -- the subprocess object 
                char **args -- the arguments
RETURNS:	ERROR / NO_ERROR
DESCRIPTION:	Get the pseudo terminal pair, set the characteristics
                of it, start the child process, and set the output
		side (pty) side as an Input Source to the Xt toolkit.
		This is the non-spmd call used from UxRunSubproc.
---------------------------------------------------------------------------*/

static 	int 	exec_subprocess(sp, args)
     	subproc *sp;
     	char 	**args;
{
#endif /*SPMD*/

	handshake_t handshake;
  	int 	flags;
  	int 	pid;
  	int 	pty;
	int	done;
	void	exit_handler();

#ifdef SPMD
#ifdef DEBUG
	fprintf(stderr,"<<(%d) [spmd_exec_subprocess merda] cmd <%s>\n",
			  getpid(),cmd);
#endif /*DEBUG*/
	proc = cmd;
	for (proc_args = proc, finished = 0; !finished; ) {
		switch (*proc_args) {
			case ' ': case '\t':
				*proc_args++ = '\0';
				/*FALLTHROUGH*/
			case '\0':	/* End of string. */
				finished = 1;
				break;
			default:
				proc_args++;
				break;
		}
	}
	if(! (args = build_args(proc, proc_args)) ) {
		return(ERROR);
	}
#endif /*SPMD*/

	if (pipe(pc_pipe) || pipe(cp_pipe)) {
	    UxInternalError(__FILE__, __LINE__,
	    	CGETS( MS_MISC_PIPE_ERR, DS_MS_MISC_PIPE_ERR));
	    return(ERROR);
	}

  	if(UxGetPtty(&pty, 1) == ERROR)
    		return(ERROR);
#ifdef	DEBUG
	fprintf(stderr, ">>(%d) pty=%d\n", getpid(), pty);
#endif	/* DEBUG */

#ifdef SPMD
  	if( (pid = start_child(pty, echo, args)) == ERROR) {
#else /*SPMD*/
  	if( (pid = start_child(pty, sp->echo, args)) == ERROR) {
#endif /*SPMD*/
		if (pty >= 0)
		    close(pty);
		pty = -1;
    		return(ERROR);
  	}

	/* This is the parent side of the fork...
	 */
	/* close the child's side of the pipes... */
	(void) close(cp_pipe[1]);
	(void) close(pc_pipe[0]);

	for (done = 0; !done; ) {
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) prima di read reply figlio ha pid=%d\n", getpid(), pid);
#endif	/* DEBUG */
            memset(&handshake,0,sizeof(handshake));
	    if (read(cp_pipe[0], &handshake, sizeof(handshake)) <= 0) {
		/* Our child is done talking to us.  If it terminated due
		 * to an error, we will catch the death of child and
		 * clean up.
		 */
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) in read reply handshake=%s \n", getpid(), handshake.buffer);
#endif	/* DEBUG */
		(void) close(cp_pipe[0]);
		(void) close(pc_pipe[1]);

		/* we can stop looping now... */
		break;
	    }

#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) dopo read reply\n", getpid());
#endif	/* DEBUG */

	    switch(handshake.status) {
	    case SUBPROC_ERROR:
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) received SUBPROC_ERROR message\n",
			getpid());
#endif	/* DEBUG */
		/* Child has an error message that they would like us to
		 * print...
		 */
		/* set errno... */
		errno = handshake.error;
		if (*handshake.buffer) {
		    /* they passed us a string to print... */
		    syserr(handshake.buffer, NULL);
		} else {
		    /* no message to print... */
		    syserr(NULL, NULL);
		}
		break;

	    case SUBPROC_EXEC_ERROR:
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) received SUBPROC_EXEC_ERROR message\n",
			getpid());
#endif	/* DEBUG */
		/* Child was unable to exec the subprocess...
		 */
		(void) close(cp_pipe[0]);
		(void) close(pc_pipe[1]);
	        /* Just to be safe, don't close the pty unless it is a 
	         * valid file descriptor, and after doing it, make it 
	         * invalid so we won't accidently close it again
	         * (who knows if it will point to anything important).
		 */
		if (pty >= 0)
		    (void) close(pty);
		pty = -1;
		return(ERROR);
		break;

	    case PTY_GOOD:
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) received PTY_GOOD message\n", getpid());
#endif	/* DEBUG */
		/* Success!  Let's free up resources and continue...
		 */
		done = True;
		(void) close(cp_pipe[0]);
		(void) close(pc_pipe[1]);
		break;

	    case PTY_BAD:
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) received PTY_BAD message\n", getpid());
#endif	/* DEBUG */
		/* Pty slave open failed, get them another one...
		 */
		/* get errno from our child... */
		errno = handshake.error;
#ifdef	DEBUG
		(void) fprintf(stderr, ">>(%d) child error: %s\n",
			getpid(), sys_errlist[errno]);
#endif	/* DEBUG */

		if (pty >= 0)
		    (void) close(pty);
		pty = -1;
		if (UxGetPtty(&pty, 0) == ERROR) {
		    /* no more ptys... */
		    handshake.status = PTY_NOMORE;
#ifdef	DEBUG
		    fprintf(stderr, ">>(%d) sending PTY_NOMORE message\n",
			    getpid());
#endif	/* DEBUG */
		    (void) write(pc_pipe[1], &handshake, sizeof(handshake));
		    (void) close(cp_pipe[0]);
		    (void) close(pc_pipe[1]);
		    return(ERROR);
		}
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) pty=%d\n", getpid(), pty);
#endif	/* DEBUG */
		handshake.status = PTY_NEW;
		(void) strcpy(handshake.buffer, ttydev);
#ifdef	DEBUG
		fprintf(stderr, ">>(%d) sending PTY_NEW message\n",
			getpid());
#endif	/* DEBUG */
		(void) write(pc_pipe[1], &handshake, sizeof(handshake));
		break;

#ifdef	DEBUG
	    default:
		fprintf(stderr, ">>(%d) received bogus message\n", getpid());
		break;
#endif	/* DEBUG */
	    }
	}

#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) prima di fcntl(pty ... \n", getpid());
#endif	/* DEBUG */

#ifdef SPMD 
  	if( (flags = fcntl(pty, F_GETFL, 0)) == -1) {
    		signal(SIGKILL, pid);
    		syserr(ptydev, "F_GETFL"); 
		if (pty >= 0)
		    close(pty);
		pty = -1;
    		return(ERROR);
  	}

 	/* O_NDELAY and FNDELAY have the same value. But only by chance ! */
  	if( fcntl(pty, F_SETFL, flags|O_NDELAY) == -1)  {
    		signal(SIGKILL, pid);
    		syserr(ptydev, "F_SETFL"); 
		if (pty >= 0)
		    close(pty);
		pty = -1;
    		return(ERROR);
  	}
#endif	/* SPMD */

#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) dopo fcntl(pty ... \n", getpid());
#endif	/* DEBUG */

#ifndef	HP
	/* set up pty master... */
  	if(set_pseudo(pty) == ERROR) {
		if (pty >= 0)
		    close(pty);
		pty = -1;
    		return(ERROR);
  	}
#endif	/* HP */

#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) dopo set up pty master pid=%d ... \n", getpid(), pid);
#endif	/* DEBUG */

#ifdef SPMD
	for (i=0; i < MAX_PROCS; i++) {
		if (SpmdStates[i].pid == -1) {
			break;	/* found an empty slot .. */
		}
	}
	ASSERT (i < MAX_PROCS);
	SpmdStates[i].pid = pid;
	SpmdStates[i].pty = pty;
        if (SpmdStatesEmergencyPIDFlag == 1) {
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) Setto SpmdStatesEmergencyPIDStatus\n", getpid());
	    fprintf(stderr, ">>(%d) RESetto SpmdStatesEmergencyPIDFlag\n", getpid());
#endif	/* DEBUG */
           SpmdStates[i].exit_status = SpmdStatesEmergencyPIDStatus;
           SpmdStatesEmergencyPIDFlag = 0;
        }
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) prima di return(i) i=%d SpmdStatesEmergencyPIDFlag=%d  \n",
                                    getpid(), i, SpmdStatesEmergencyPIDFlag);
#endif	/* DEBUG */
	return (i);
#else /*SPMD*/
  	sp->pty = pty;
  	sp->pid = pid;

	/* we need to catch death of child regardless... */
#ifdef HP
	signal(SIGCHLD, exit_handler);
#endif 
#ifdef	DEBUG
	    fprintf(stderr, ">>(%d) prima di return(NO_ERROR)  \n", getpid());
#endif	/* DEBUG */
  	return(NO_ERROR);
#endif /*SPMD*/
}

#endif /* _PTY_DRIVER */

#ifdef SPMD
/*------------------------------------------------------------------------
 *	exec_subprocess(subproc *sp, char *process, char *args)
 *
 *	An application process interface to generate a child process
 *
 *	exec_subprocess interface has remained unchanged. The non-SPMD
 *	functionality of exec_process has been replaced by 
 *	spmd_exec_subprocess() in the daemon process. Confused? Sorry!
 * 
 *------------------------------------------------------------------------*/
static 	int 	exec_subprocess(sp, process, args)
subproc *sp;
char 	*process, *args;
{
	int i, cmdlen;
	spmd_create_req req;
	spmd_create_rsp *rsp;

	req.mtype = SPMD_CREATE;
	req.echo = sp->echo;
	req.cmd[0] = '\0'; /*EOS*/

	if (args == NULL)  args = "";
	sprintf(req.cmd, "%s %s", process, args);

	cmdlen = strlen(req.cmd);	/* also used in msgsnd() */
	
	if (-1 == msgsnd(Spmd.spmd_pc_msqid, &req, 
		   	 cmdlen+sizeof(req.echo), IPC_NOWAIT)) 
	{
		perror("exec_subprocess: can not send SPMD_CREATE to daemon");
		return(ERROR);
	}
	
	/* now we must wait for the daemon to do it's magic ... */
	if (spmd_read_msg(SPMD_RSP) == ERROR) {
		return(ERROR);
	}
		
	rsp = (spmd_create_rsp *)Spmd.spmd_rmsgp;

	if (rsp->response == NACK) {	
		error_msg("Cannot start child process %s \n");
		return(ERROR);
	}

	sp->pid = rsp->pid;
  	return(NO_ERROR);
}
#endif /*SPMD*/

/* ---------------------------------------------------------------------------
NAME:		static char **build_args(process, command_line)
INPUT:		char *process, *command_line -- args
RETURNS:	char **args - returns pointer to the argument array
DESCRIPTION:	Small parser of the argument list.  It supports the
		following:

			- '\' quotes the next character.
			- all text inside of "" is quoted (with
			  the exception of '\' quoting).
			- all text inside of '' is quoted (with
			  the exception of '\' quoting).
			- arguments are separated by non quoted spaces
			  or tabs.
			- there is no attempt to remove special characters
			  since they may mean something to the application
			  which is running the command (such as:
			  sh -c "filter </etc/passwd").
			- dynamically creates the arg list.
			- dynamically creates storage for breaking up
			  command line.
---------------------------------------------------------------------------*/

static 	char 	**build_args(process, command_line)
     	char 	*process, *command_line;
{
    static char *Buffer = NULL;
    static char **Argv = (char **) 0;
    register char *From;
    register char *To;
    register char **ArgvPtr;
    register int Quote;
    int i;

    if (! command_line)
        command_line = "";

    /* allocate storage for string to be mucked with and copy it over... */
    if (Buffer)
	Buffer = (char*)UxRealloc(Buffer, strlen(command_line) + 1);
    else
	Buffer = UxMalloc(strlen(command_line) + 1);
    (void) strcpy(Buffer, command_line);

    /* Allocate storage for Argv.  We don't need to be exact.  We just don't
     * want to underestimate the number of arguments (or we trash memory),
     * and we don't want to grossly overestimate the amount of memory.  If
     * we count all groups of spaces as separators we will be close -- we
     * will overcount any that are quoted.
     */
    /* The first one is for the command being executed... */
    for (i = 1, From = Buffer; *From; ) {
	/* count this as a potential argument... */
	(void) i++;

	/* skip over characters... */
	while (*From && ((*From != ' ') && (*From != '\t')))
	    (void) From++;
	
	/* skip over white space... */
	while (*From && ((*From == ' ') || (*From == '\t')))
	    (void) From++;
    }

    /* leave an extra one for null terminating the array... */
    if (Argv)
	Argv = (char **) UxRealloc((char *) Argv, (i + 1) * sizeof(char *));
    else
	Argv = (char **) UxMalloc((i + 1) * sizeof(char *));
	
    /* initialize the world... */
    ArgvPtr = Argv;
    From = Buffer;
    To = Buffer;
    Quote = 0;

    *ArgvPtr++ = process;
    *ArgvPtr = To;

    /*
    ** skip over leading blanks
    */

    while (*From && ((*From == ' ') || (*From == '\t')))
	(void) From++;

    if (*From == '\0') {
	*ArgvPtr = (char *) 0;
	return (Argv);
    }
    
    while (*From) {
#ifdef DEBUGPARSE
	printf ("char='%c' ", *From);
#endif /* DEBUGPARSE */
        switch (*From) {
	    case '\'' :
	    case '"' :
		/*
		** if we are not quoting, start doing so
		*/

		if (Quote == 0) {
#ifdef DEBUGPARSE
		    printf ("quoting on\n");
#endif /* DEBUGPARSE */
		    Quote = *From++;
		    break;
		}

		/*
		** if we are done quoting, do so
		*/

		if (Quote == *From) {
#ifdef DEBUGPARSE
		    printf ("quoting off\n");
#endif /* DEBUGPARSE */
		    Quote = 0;
		    From++;
		    break;
		}

		/*
		** else, it is a valid character
		*/

#ifdef DEBUGPARSE
		printf ("copied\n");
#endif /* DEBUGPARSE */
		*To++ = *From++;
		break;

	    case ' ' :
	    case '\t' :
		if (Quote) {
#ifdef DEBUGPARSE
		    printf ("quoted tab/space\n");
#endif /* DEBUGPARSE */
		    *To++ = *From++;
		    break;
		}

		/*
		** put in a null and advance ArgvPtr
		*/

#ifdef DEBUGPARSE
		printf ("seperator\n");
#endif /* DEBUGPARSE */
		while ((*From == ' ') || (*From == '\t'))
		    (void) From++;

		*To++ = '\0';

		/* if we are done, return the string.  This takes care
		 * of the problem of command lines terminated by spaces.
		 */
		if (*From == '\0') {
		    *(++ArgvPtr) = (char *) 0;
		    return (Argv);
		}

		*(++ArgvPtr) = To;
		break;

	    case '\\' :
		/*
		** quote the next character directly
		*/

		(void) From++;
		if (*From) {
#ifdef DEBUGPARSE
		    printf ("quoting '%c'\n", *From);
#endif /* DEBUGPARSE */
		    *To++ = *From++;
		}
		break;

	    default :
#ifdef DEBUGPARSE
		printf ("copied\n");
#endif /* DEBUGPARSE */
		*To++ = *From++;
		break;
        }
    }

    *To = '\0';
    *(++ArgvPtr) = (char *) 0;
    return (Argv);
}

#ifdef DEBUG
/* ---------------------------------------------------------------------------
NAME:		static int print_args(tmp)
INPUT:		char **tmp - arg list
RETURNS:	--
DESCRIPTION:	debug fnt - print the argument list
---------------------------------------------------------------------------*/

static int print_args(tmp)
  char **tmp;
{
  int i;
  
  printf("Argument list\n");
  for(i = 0; tmp[i]; i++)
    printf("Arg %d: %s\n", i, tmp[i]);
  printf("\n");
}
#endif /* DEBUG */

/* ---------------------------------------------------------------------------
NAME:		int UxRunSubproc(sp_vh, cmd_line)
INPUT:		handle sp_vh -- handle indicating the process
                char *cmd_line -- arguments to the process
RETURNS:	ERROR / NO_ERROR
DESCRIPTION:	Looks up the object and start the subprocess.
---------------------------------------------------------------------------*/

int 	UxRunSubproc(sp_vh, cmd_line)
     	handle sp_vh;
     	char *cmd_line;
{
	char **arg_array;
	char *cmd;
	subproc *sp;

	if(check_index(sp_vh) == ERROR)
		return(ERROR);
	sp = sp_array[sp_vh];

	if(sp->pid) {
		error_msg(CGETS( MS_MISC_SUBPROC_RUNNIN, 
				DS_MS_MISC_SUBPROC_RUNNIN));
		return(ERROR);
	} 

	if(cmd_line && (cmd_line[0] != '\0'))
		cmd = cmd_line;
	else
		cmd = sp->defarg;

	/*-----------------------------------------------------
	 * Under SPMD, we don't translate cmd into argv[]
	 * here because we're going to pass it across the msgq.
	 *-----------------------------------------------------*/
#ifdef SPMD 
	if (exec_subprocess(sp, sp->process, cmd) == ERROR)  {
		return(ERROR);
	}
#else
	if (! (arg_array = build_args(sp->process, cmd)) )  {
		return(ERROR);
	}
	if (exec_subprocess(sp, arg_array) == ERROR)  {
		return(ERROR);
	}
#endif /*SPMD*/
 
#ifdef _PTY_DRIVER
#ifndef SPMD
	/*-----------------------------------------------------
	 * Make the toolkit read input from the subprocess.
	 * Also start the periodic checks for dead children.
	 *-----------------------------------------------------*/

	sp->input_id= XtAppAddInput(UxAppContext, sp->pty, XtInputReadMask,
				    output_handler, sp);
	if (!timeoutid) {
		timeoutid = XtAppAddTimeOut(UxAppContext, 
					    CHECK_INTERVAL, 
					    CheckAllChildren, 
					    NULL);
	}
#endif /*SPMD*/
#endif

#ifdef _STREAM_DRIVER
	sp->input_id= XtAppAddInput(UxAppContext, sp->stream, (XtPointer)XtInputReadMask,
				       output_handler, sp);
	/* It is necessary to handle exceptions due to a bug in select(2) */
	sp->except_id= XtAppAddInput(UxAppContext, sp->stream, 
				(XtPointer)XtInputExceptMask, exception_handler, sp);
#endif

	return(NO_ERROR);
}
/* ---------------------------------------------------------------------------
NAME:		int UxExecSubproc(sp_vh, cmd_line)
INPUT:		handle sp_vh -- handle indicating the process
                char *cmd_line -- arguments to the process
RETURNS:	ERROR / NO_ERROR
DESCRIPTION:	Looks up the object and start the subprocess
		Same as UxRunSubproc, but kill the process if it is
		still running. Main purpose is for processes 
		that die after execution e.g. "ls".
---------------------------------------------------------------------------*/
int 	UxExecSubproc(sp_vh, cmd_line)
     	handle sp_vh;
     	char *cmd_line;
{
	if(UxExitSubproc(sp_vh) == ERROR)
		return(ERROR);
	if(UxRunSubproc(sp_vh, cmd_line) == ERROR)
		return(ERROR);
	return(NO_ERROR);
}

/* ---------------------------------------------------------------------------
NAME:		sp_exit (sp)
INPUT:		subproc	*sp
RETURNS:	void
DESCRIPTION:	Closes the pty/tty pair or the stream (if open),
		removes the input source for X, and resets the 'input_id'.
CREATION:       June 13/89
---------------------------------------------------------------------------*/
static	void	sp_exit (sp)
	subproc	*sp;
{
#ifdef _PTY_DRIVER
	if(sp->pty != -1)
	{
		(void)close(sp->pty);
		sp->pty = -1;
	}
#endif /* _PTY_DRIVER */

#ifdef _STREAM_DRIVER
	if(sp->stream != -1)
	{
		(void)close(sp->stream);
		sp->stream = -1;
	}
#endif /* _STREAM_DRIVER */
	if(sp->input_id)
	{
		XtRemoveInput(sp->input_id); 
		sp->input_id = (XtInputId) 0;
	}

#if defined(_STREAM_DRIVER)
	if(sp->except_id)
	{
		XtRemoveInput(sp->except_id); 
		sp->except_id = (XtInputId) 0;
	}
#endif	/* _STREAM_DRIVER */
}

/* ---------------------------------------------------------------------------
NAME:		int UxExitSubproc(sp_vh)
INPUT:		handle sp_vh -- object handle
RETURNS:	ERROR / NO_ERROR
DESCRIPTION:	Remove the input source of the subprocess.
                Kill the process and free the memory associated with
		it.
---------------------------------------------------------------------------*/

int UxExitSubproc(sp_vh)
     handle sp_vh;
{
	subproc *sp;

	if(check_index(sp_vh) == ERROR)
		return(ERROR);

	sp = sp_array[sp_vh];

	if(sp->pid)
	{
		kill(sp->pid, SIGKILL);
		sp->pid = 0;
	        sp_exit (sp);
	}


	return(NO_ERROR);
}

/* ---------------------------------------------------------------------------
NAME:		int UxGetSubprocPid(sp_vh)
INPUT:		handle sp_vh -- object handle
RETURNS:	ERROR -1 if the subprocess object does not exist
                0        if the process is dead
		pid      if the process is running
                
DESCRIPTION:	Test if the process is still alive and return
                pid if it is not. If the process is dead, it
		resets the pid field and closes the pseudo terminal
		fd's if they are open.
---------------------------------------------------------------------------*/

int UxGetSubprocPid(sp_vh)
     handle sp_vh;
{
	subproc *sp;

	if(check_index(sp_vh) == ERROR)
		return(ERROR);
	sp = sp_array[sp_vh];

	if(! sp->pid){
		error_msg(CGETS( MS_MISC_SUBPROC_NOACT,
                                DS_MS_MISC_SUBPROC_NOACT));
		return(ERROR);
	}
	/** if there is a process id */
	if(sp->pid)
	{
		/* and the process is dead 
		   -- reset the pid entry
		   -- close the pty/tty and reset the entries
		   -- remove the input source for X
		   */
		if (kill(sp->pid, 0) && (errno != EPERM))
		{
			sp->pid = 0;
			sp_exit (sp);
		}
	}
	return(sp->pid);
}
/* ---------------------------------------------------------------------------
NAME:		static void sp_check(sp)
INPUT:		subproc *sp -- subprocess object
RETURNS:	--
DESCRIPTION:	check if the subprocess is still alive
---------------------------------------------------------------------------*/

static void sp_check(sp)
     subproc *sp;
{
	if(!sp)
		return;

	if (kill(sp->pid, 0) && (errno != EPERM))
	{
		sp->pid = 0;
		sp_exit (sp);
	}
}
/* ---------------------------------------------------------------------------
NAME:		int UxSendSubproc(sp_vh, tex)
INPUT:		char *sp_vh
                char *tex
RETURNS:	ERROR /NO_ERROR
DESCRIPTION:	send a the text string passed to the subprocess
                indicated by sp_vh
---------------------------------------------------------------------------*/

int UxSendSubproc(sp_vh, tex)
  	handle sp_vh;
  	char *tex;
{
	char	*tbuff;
	int	ret;

	if(tex == NULL) {
	    tex = "";
	}

	tbuff = (char *) UxMalloc((strlen(tex) + 2)*sizeof(char));

	strcpy(tbuff, tex);
	strcat(tbuff, "\n");

	ret = UxSendSubprocNoCR(sp_vh, tbuff);

	UxFree(tbuff);

	return ret;
}

/**********************************************/
int UxSendSubprocNoCR(sp_vh, tex)
  	handle sp_vh;
  	char *tex;
{
	register int len;
	subproc *sp;
#ifdef HP
	static int one=1;
#endif
#ifdef SPMD
	spmd_input_t msg;	/* viewed as input to subprocess */
#endif /*SPMD*/

	if(tex == NULL) {
	    tex = "";
	}

	if(check_index(sp_vh) == ERROR)
		return(ERROR);
	sp = sp_array[sp_vh];
	sp_check(sp);

#ifdef _PTY_DRIVER
#ifdef SPMD
	if(!sp->pid) /* sp_check() sets pid to zero if process isn't there */
#else /*SPMD*/
	if(sp->pty == -1)
#endif /*SPMD*/
	{
		error_msg(CGETS( MS_MISC_SUBPROC_NOACT,
                                DS_MS_MISC_SUBPROC_NOACT));
		return(ERROR);
	} 

	len = strlen(tex);

#ifdef SPMD
	msg.mtype = SPMD_INPUT;		/* input from subprocess perspective */
	msg.pid = sp->pid;
	ASSERT (len < sizeof(msg.input)); 	/* Could use more messages.. */
	strcpy (msg.input, tex);
	if (msgsnd(Spmd.spmd_pc_msqid, &msg, len+sizeof(msg.pid), IPC_NOWAIT) 
	    == -1)
#else /*SPMD*/
	if (write(sp->pty, tex, len) != len)
#endif /*SPMD*/
	{
		UxInternalError(__FILE__, __LINE__,
			CGETS( MS_MISC_W_SUBPROC_ERR,DS_MS_MISC_W_SUBPROC_ERR));
		sp_check(sp);
		return(ERROR);
	}
#endif /* _PTY_DRIVER */

#ifdef _STREAM_DRIVER
	if(sp->stream == -1)
	{
		error_msg(CGETS( MS_MISC_SUBPROC_NOACT,
                                DS_MS_MISC_SUBPROC_NOACT));
		return(ERROR);
	} 

	len = strlen(tex);

	if(write(sp->stream, tex, len) != len)
	{
		error_msg(CGETS( MS_MISC_W_SUBPROC_ERR,
                                DS_MS_MISC_W_SUBPROC_ERR));
		sp_check(sp);
		return(ERROR);
	}
#endif /* _STREAM_DRIVER */

	return(NO_ERROR);
}

/* ---------------------------------------------------------------------------
NAME:		static void output_handler(sp)
INPUT:		subproc *sp - subproc object
RETURNS:	--
DESCRIPTION:	Low level output function - reads the output 
                of the subprocess into a temp. buffer and executes
		the user function passing a pointer to the buffer.
		NOTE: This might be changed to just passing the fd
		and a pointer to user supplied data.
---------------------------------------------------------------------------*/

static void output_handler(sp)
     subproc *sp;
{
	if(!sp)
	{
		error_msg(CGETS( MS_MISC_INV_SPRC_STRUCT,
                                DS_MS_MISC_INV_SPRC_STRUCT));
		return;
	}

	if(! sp->output_fnt )
	{
		error_msg(CGETS( MS_MISC_INV_OUT_FNCTN,
                                DS_MS_MISC_INV_OUT_FNCTN));
		return;
	}

#ifdef _PTY_DRIVER
	(*sp->output_fnt) (sp->pty, sp->user_data);
#endif
#ifdef _STREAM_DRIVER
	(*sp->output_fnt) (sp->stream, sp->user_data);
#endif
}

/******************************************************/
#ifdef _STREAM_DRIVER

static void exception_handler()
{
	/* Because of bug in select system call on AT&T Unix i386-SYSV,
	 * an exception input handler is needed. (installed with XtAddInput)
	 */
}
#endif /* _STREAM_DRIVER */

/* ---------------------------------------------------------------------------
NAME:		generic user utilities of type void fnt_name(fd, user_data)
INPUT:		int fd - the file descriptor that has output on it
                char *user_data - if set, the user data otherwise NULL
RETURNS:	--
DESCRIPTION:	The output handler of the subprocess module calls the user 
                supplied function with the file descriptor that has output
		from the subprocess. The user_data is set with the routine
		UxSp_set_udata() and must be cast to type char *.

		With SPMD, the file descriptor is unused.
---------------------------------------------------------------------------*/

#define MY_BUFFER 512

char * UxTransferToBuffer(fd, status)
     int fd, *status;
{
	int len;
	int max_read = MY_BUFFER -1;
	static char text_buff[MY_BUFFER];

#ifdef SPMD
	if ((Spmd.spmd_rmsgrp == NULL) || (Spmd.spmd_rmsgsiz <= 0)) {
		return (NULL); /* sanity checking failed */
	}

	/*
	 * Copying could be avoided by returning a pointer into 
	 * Spmd.spmd_rmsgrp directly.
	 */
    	len = (max_read < Spmd.spmd_rmsgsiz)
			? max_read 
			: Spmd.spmd_rmsgsiz;
	strncpy (text_buff, Spmd.spmd_rmsgrp, len);
	Spmd.spmd_rmsgsiz -= len;	/* decrement remainder count */

	if (Spmd.spmd_rmsgsiz <= 0) {
		*status = 0;		/* no more to read */
		ASSERT (Spmd.spmd_rmsgsiz == 0);
		Spmd.spmd_rmsgrp = NULL;
	} else {
		*status = 1;
		Spmd.spmd_rmsgrp += len;	/* increment the read-ptr */
	}
#else /*SPMD*/

	len = read(fd, text_buff, max_read);
	if(len == -1) {
#ifndef HP
        /*
	*** If errno == EIO (subproc is assumed to be defunct),
	*** call exit_handler().  We no longer rely on exit_handler
	*** being the signal handler for SIG_CHLD since this
	*** unpredictably leads to an interrupt at a critical
	*** time when exit_handler's tasks would cause Xt code
	*** to get a segmentation violation.
	*** ( see exit_handler's comments )
	*/
           extern int errno;
	   if( errno != EIO ) {	
	      UxInternalError(__FILE__, __LINE__,
	               CGETS( MS_MISC_R_SUBPROC_ERR,DS_MS_MISC_R_SUBPROC_ERR));
           } else {
	      exit_handler(9999);
           }
#else
		UxInternalError(__FILE__, __LINE__,
			CGETS( MS_MISC_R_SUBPROC_ERR,DS_MS_MISC_R_SUBPROC_ERR));
#endif /* HP */
		return(NULL);
	}
	text_buff[len] = '\0';
	if(len < max_read)
		*status = 0;
	else 
		*status = 1;

#endif /* SPMD */
	return(text_buff);
}


/*******************************************/

void UxAppendTo(fd, tw)
     int fd;
     Widget  tw;
{
	char *text;
	int more;

	while( (text = UxTransferToBuffer(fd, &more)) )
	{
		if(tw)
			UxTextAppend(tw, text);
		else 
			fprintf(stdout, "%s",text);

		if(!more)
			break;
	}
}
    

/******************************************************************************
NAME:		int		UxSetSubprocClosure(sp_vh, data)
INPUT:		handle		sp_vh		- handle to subproc
		char		*data		- data passed to the 
		                                subprocess function
RETURN:		ERROR/NO_ERROR
DESCRIPTION:	Sets the data/closure passed to the output function
REVISIONS:	--
-----------------------------------------------------------------------------*/

int UxSetSubprocClosure(sp_vh, data)
     handle sp_vh;
     char *data;
{
	subproc *sp;

	if(check_index(sp_vh) == ERROR)
		return(ERROR);
	sp = sp_array[sp_vh];
  
	sp->user_data = data;
	return(NO_ERROR);
}


/******************************************************************************
NAME:		int		UxSetSubprocFunction(sp_vh, fnt)
INPUT:		handle		sp_vh		- handle to subproc
		void		(*fnt)()	- output function
RETURN:		ERROR/NO_ERROR
DESCRIPTION:	Sets the output function that is executed on ouput
                from the child process
REVISIONS:	--
-----------------------------------------------------------------------------*/

int UxSetSubprocFunction(sp_vh, fnt)
     handle sp_vh;
     void (*fnt)();
{
	subproc *sp;

	if (check_index(sp_vh) == ERROR)
		return(ERROR);

	sp = sp_array[sp_vh];
	sp->output_fnt = fnt;
	return(NO_ERROR);
}

/*------------------------------------------------------------------------
 * NAME: CheckAllChildren
 * DESCRIPTION:
 * 	This timer procedure is used to check for dead children
 *	and clean up after them, including removing them from
 *	our list of subprocesses.
 *
 *	We do this when NOT using the SPMD daemon. 
 *	We can't rely on SIGCHLD to be informed of dying children in most
 *	cases because it causes unpredictable behaviour
 *	in the toolkit.  So, instead, we regularly check all the
 *	processes listed and clear out those that we can't signal.
 *
 * LAST REV:
 *------------------------------------------------------------------------*/

#ifndef SPMD

static void CheckAllChildren()
{
	int i, alive_kids = 0;

#ifdef DEBUG
	fprintf(stderr,"CheckAllChildren: timer went off\n"); 
#endif /*DEBUG*/

	/* look for a free slot in the array */

	for(i = 0; i < MAX_SUBPROC; i++)
	{
		if( sp_array[i] == (subproc *)0) {
			continue;
		} else if (kill(sp_array[i]->pid, 0) == -1 && errno == ESRCH) {
#ifdef DEBUG
			fprintf(stderr,"CheckAllChildren: found dead child \n");
#endif /*DEBUG*/
			if (sp_array[i]->exit_cb) {
				(*sp_array[i]->exit_cb)
					(sp_array[i]->pid, CHILD_LOST, i);
			}
			/*
			 * sp_exit() leads to a call of XtRemoveInput() which
			 * is necessary when the subproc connection is closed.
			 * Otherwise an infinite loop occurs as evidenced by
			 * the calls to the function registered in XtAddInput.
			*/
			sp_exit( sp_array[i] );
			sp_array[i]->pid = 0;
		} else {
			alive_kids++;
		}
	}

	/*--------------------------------------------------------------
	 * Keep the periodic check going as long as there are subprocs.
	 *--------------------------------------------------------------*/
	if (alive_kids) {
		timeoutid = XtAppAddTimeOut(UxAppContext, 
				 	    CHECK_INTERVAL, 
					    CheckAllChildren, 
					    NULL);
	} else {
		timeoutid = 0;
	}
}
#endif /*SPMD*/


/*------------------------------------------------------------------------
 * The remainder of the file is the SPMD implementation.
 *------------------------------------------------------------------------*/

#ifdef SPMD

/*------------------------------------------------------------------------
 * spmd_read_msg() - check for and process any messages arriving from the
 *		     the Sub-Process Manager daemon
 *
 * We arrange for this routine to be called ``periodically'' to see if 
 * work has arrived from the daemon.  Each time it is called, we
 * schedule it to be called again after an SpmdInterval.
 * We use a longer SpmdInterval when no messages are found
 * than when there are messages, assuming activity may be bursty.
 *
 * Also called from exec_subprocess explicitly waiting for SPMD_RSP
 *
 * Note:  The timeout interval SpmdInterval could be adjusted in
 * many ways, to account for number of processes, or rate of activity;
 * instead of a timeout, XtWorkProcs could be used.
 *
 * The periodic use of this function is in the application (parent) process
 * so we can use XtAppAddTimeOut to schedule the calls.
 *------------------------------------------------------------------------*/

static int spmd_read_msg (msgtyp)
	long msgtyp;		/* zero     -> called from timeout
			 	 * non-zero -> explicit rsp 
				 */
{
	int i, rc, msgflg;
	extern int errno;
	int msgfound = 0;

	msgflg = msgtyp ? 0 : IPC_NOWAIT;

	Spmd.spmd_rmsgtyp = 0;	/* clear buffer */

	while ((rc=msgrcv(Spmd.spmd_cp_msqid, 
			  Spmd.spmd_rmsgp, MSGMAX, msgtyp, msgflg)) 
	       != -1) 
	{
		msgfound = 1;

		switch (Spmd.spmd_rmsgp->mtype)
		{
		    case SPMD_OUTPUT:	/* output from child process */
		    {
			spmd_output_t *msg = (spmd_output_t *)Spmd.spmd_rmsgp;
			Spmd.spmd_rmsgrp = msg->output;
			Spmd.spmd_rmsgsiz = rc - sizeof(msg->pid);
			for (i=0; i<MAX_SUBPROC; i++) {
				if ((sp_array[i]) && 
				    (sp_array[i]->pid == msg->pid)) 
				{
					output_handler(sp_array[i]);
			    	break;
				}
			}
			ASSERT (i<MAX_SUBPROC);	/* should not fall off loop */
			break;
		    }
		    case SPMD_DIED:	/* A managed process has died */
			exit_handler(9998);
			break;
		    case SPMD_RSP:
#ifdef DEBUG
			if (!msgtyp) {
				UxInternalError(
					"spmd_read_msg: timeout received RSP");
			}
#endif /*DEBUG*/
			return (0);
			/*NOTREACHED*/
			break;

		    default:	/* Either garbage or wrong directional msg */
#ifdef DEBUG
			UxInternalError("spmd_read_msg: protocol error");
#endif /*DEBUG*/
			break;
		}
	}

	if (rc == -1 && errno != ENOMSG) {
		perror("Subprocess manager: msgrcv failed");
#ifdef DEBUG
		exit (errno);
		/*NOTREACHED*/
#endif /*DEBUG*/
		return(ERROR);
	}

	/*-----------------------------------------------------
	 * If called from the timer callback, re-arm.
	 *-----------------------------------------------------*/
	if (!msgtyp) {
		/* Assume that activity means there may be more.
		 */
		SpmdInterval = msgfound ? SHORT_INTERVAL : LONG_INTERVAL;
		XtAppAddTimeOut(UxAppContext, SpmdInterval, spmd_read_msg, 0);
	}
	return (0);
}

/*------------------------------------------------------------------------
 * NAME: spmd_main()
 * DESCRIPTION:
 *	This is the main routine of the manager daemon itself,
 *	called after the manager is fork/exec'd.
 *
 * LAST REV:
 *------------------------------------------------------------------------*/

static void spmd_main()
{
	int rc, i;
	struct msgbuf *tmsgp;
	extern int errno;
	void spmd_sig_handler();
	int spmd_read_ptys();
        struct sigaction sa;

//        sigaction(SIGCHLD, NULL, &sa);
//        sa.sa_handler = spmd_sig_handler;
//        sa.sa_flags = SA_INTERRUPT;
//        sigaction(SIGCHLD, &sa, NULL);      /* arm the signal catcher */
//        sigaction(SIGALRM, &sa, NULL);      /* arm the signal catcher */
	signal(SIGCHLD, spmd_sig_handler);	/* arm the signal catcher */
	signal(SIGALRM, spmd_sig_handler);	/* arm the signal catcher */

	if ( !(tmsgp=(struct msgbuf *)UxMalloc(SIZEOFMAXMSGBUF))) {
		perror("Subprocess manager: memory allocation failed");
		exit (errno);
	}

	for (i=0;i<MAX_PROCS;i++) {
		SpmdStates[i].pid = SpmdStates[i].pty =
			SpmdStates[i].exit_status = -1;
	}

	SpmdSubprocCount = 0;

	/*
	 * Main processing loop is to wait on the message queue from the parent.
	 * Note: will be extended to select on this queue plus all managed
	 * sub-process pty's
	 */

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di Main processing loop \n", getpid());
#endif	/* DEBUG */

	for(;;) {

		/* Read a message from the application 
		 * (more frequently if we have subprocs)
		 */

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di alarm(%d) \n", getpid(), (SpmdSubprocCount ? 1 : 5));
#endif	/* DEBUG */

		alarm ((SpmdSubprocCount ? 1 : 5));

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) dopo alarm(%d) \n", getpid(), (SpmdSubprocCount ? 1 : 5));
#endif	/* DEBUG */

		while (spmd_read_ptys() && 
		       (rc=msgrcv(Spmd.spmd_pc_msqid,tmsgp, MSGMAX, 0, 0)) 
			 == -1 && 
		       errno == EINTR) 
		{
			/* nop body; skip interrupts */ ;
		}

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di alarm(0) \n", getpid());
#endif	/* DEBUG */

		alarm(0);
		if (rc == -1) {
			perror("UIM/X: spmd_main: msgrcv failed");
#ifdef DEBUG
			/* check out the error ... could be fatal */
			exit (errno);
#endif /*DEBUG*/
		}

		/*-----------------------------------------------------
		 *  Process the message.
		 *-----------------------------------------------------*/
		switch (tmsgp->mtype) { 
		    case SPMD_CREATE:	/* create a sub-process */
		    {
			spmd_create_req *req = (spmd_create_req *)tmsgp;
			spmd_create_rsp msg;
			req->cmd[rc-sizeof(req->echo)] = '\0'; /* */

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di spmd_exec_subprocess ...\n", getpid());
#endif	/* DEBUG */

			if((i=spmd_exec_subprocess(req->cmd, req->echo)) 
			   == ERROR) {
				msg.response = NACK;
			} else {
				ASSERT((i>=0) && (i<MAX_PROCS) && (SpmdStates[i].pid!=-1));
				msg.response = ACK;
				msg.pid = SpmdStates[i].pid;
			}
			msg.mtype = SPMD_RSP;

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di while ( (msgsnd(Spmd ...\n", getpid());
#endif	/* DEBUG */

			while ( (msgsnd(Spmd.spmd_cp_msqid, &msg, sizeof(spmd_create_rsp)-sizeof(msg.mtype), 0) == -1) && (errno == EINTR) ) {
				/* nop body */ ;
			}
			SpmdSubprocCount++;

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) prima di break  ...\n", getpid());
#endif	/* DEBUG */

			break;
		    }
		    case SPMD_INPUT:	/* input for a process */
		    {
			spmd_input_t *msg = (spmd_input_t *)tmsgp;
			msg->input[rc-sizeof(msg->pid)] = '\0'; /* */
			for (i=0; i < MAX_PROCS; i++) {
				if (SpmdStates[i].pid == msg->pid) {
					break;
				}
			}
			if (i == MAX_PROCS) { /* App. gave us a bad pid */
#ifdef DEBUG
				fprintf(stderr, ">>(%d)) [spmd_main] SPMD_INPUT received BUT %d is a BAD PID !\n", getpid(), msg->pid);
#endif /*DEBUG*/
					break;	/* silently ignore */
			}
			if (SpmdStates[i].exit_status == -1) { 
				/* not dead yet ? */
				ASSERT (SpmdStates[i].pty >= 0);
				write(SpmdStates[i].pty, 
				      msg->input, rc-sizeof(msg->pid));
			}
			break;
		    }
		    default:		/* garbage or wrong directional msg */
#ifdef DEBUG
			fprintf(stderr,"UIM/X: spmd_main: protocol error\n");
#endif /*DEBUG*/
			break;
		}
		tmsgp->mtype = 0;	/* A.Nil. Legal value */

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) dopo il case \n", getpid());
#endif	/* DEBUG */

	}

#ifdef	DEBUG
	fprintf(stderr, ">>(%d) dopo Main processing loop \n", getpid());
#endif	/* DEBUG */

}

/*------------------------------------------------------------------------
 * NAME: static int spmd_create_daemon
 * DESCRIPTION:
 *
 *	Create the Sub-Process Manager Daemon.
 *	set up the messages queues and message buffers.
 *
 *	side-effects: spmd_info_t struct is filled in with relevant info.
 *	returns: 0 on success, non-0 on error/failure
 *------------------------------------------------------------------------*/

static int spmd_create_daemon()
{
	int rc;
	void spmd_main();

	/* create directional message queues (one each direction) */
	Spmd.spmd_pc_msqid = msgget(IPC_PRIVATE,IPC_CREAT|S_IRUSR|S_IWUSR);
	Spmd.spmd_cp_msqid = msgget(IPC_PRIVATE,IPC_CREAT|S_IRUSR|S_IWUSR);

	if ( (Spmd.spmd_cp_msqid == -1) || (Spmd.spmd_pc_msqid == -1) ) 
	{
#ifdef DEBUG
		perror("spmd_create_daemon: msgget failed");
#endif /*DEBUG*/
		return(ERROR);
	}
#ifdef DEBUG

	fprintf(stderr,
		"spmd_create_daemon c->p msqid = 0x%x, p->c msqid = 0x%x\n", 
		Spmd.spmd_cp_msqid, Spmd.spmd_pc_msqid);
#endif /*DEBUG*/

	/* allocate the receive-message buffer */
	if ( !(Spmd.spmd_rmsgp=(struct msgbuf *)UxMalloc(SIZEOFMAXMSGBUF)) ) 
	{
#ifdef DEBUG
		perror("spmd: spmd_create_daemon: UxMalloc failed");
#endif /*DEBUG*/
		return(ERROR);
	}

	switch (Spmd.spmd_pid=fork()) {
	    case -1:	/* forking error */
#ifdef DEBUG
		perror("spmd: spmd_create_daemon: fork failed");
#endif /*DEBUG*/
		return(ERROR);

	    case 0:	/* child: the sub-process manager daemon */
		spmd_main();
		/*NOTREACHED*/
		break;

	    default:	/* parent: the application */
		/*-------------------------------------------
		 * Start reading the Spmd msg queue.
		 *-------------------------------------------*/
		XtAppAddTimeOut(UxAppContext, SpmdInterval, spmd_read_msg, 0); 
		break;
	}

	return (0);
}

/*------------------------------------------------------------------------
 * NAME: spmd_sig_handler
 * INPUT:	int sig		-- signal being handled
 * DESCRIPTION:
 *	Handler for signals of interest to subproc manager.
 *	SIGCHLD:	- mark dying process as dead in state table
 *	SIGALRM:	- used to induce EINTR break from blocked msgrcv(),
 *			  check that our parent process still owns us.
 *------------------------------------------------------------------------*/

static void spmd_sig_handler(sig)
int sig;
{
	int status, pid, i;

	switch (sig) {
	    case SIGCHLD:
#ifdef DEBUG
		fprintf(stderr,
		"<<(%d) [spmd_sig_handler] ricevuto SIGCHLD \n", getpid());
#endif /*DEBUG*/
		while ( (pid=wait(&status)) == -1 && (errno == EINTR) ) {
			/* nop body */ ;
		}
#ifdef DEBUG
		fprintf(stderr,
		"<<(%d) [spmd_sig_handler] dopo pid=wait (SIGCHLD) pid=%d \n", getpid(), pid);
#endif /*DEBUG*/
		if (pid == -1) {
			perror("subprocess manager: wait failed");
		}
		
		for (i=0; i < MAX_PROCS; i++) 
		{	
#ifdef DEBUG
		fprintf(stderr,
		"<<(%d) [spmd_sig_handler] SpmdStates[%d].pid =%d || pid=%d \n", getpid(), i, SpmdStates[i].pid, pid);
#endif /*DEBUG*/
                /*
                 * Se non corrisponde a nessuno setto un flag di emergenza
                 */

#ifdef DEBUG
		fprintf(stderr,
		"<<(%d) [spmd_sig_handler] setto SpmdStatesEmergency  \n", getpid());
#endif /*DEBUG*/
                SpmdStatesEmergencyPIDFlag=1;
                SpmdStatesEmergencyPIDStatus=status;

			/* Mark the process dead. */ 
			if (SpmdStates[i].pid == pid) 
			{ 
				ASSERT (SpmdStates[i].exit_status == -1);
#ifdef DEBUG
				fprintf(stderr,
			"<<(%d) [spmd_sig_handler] gestione SIGCHLD i=%d pid %d died, status %d\n",
					getpid(), i, pid, status);
#endif /*DEBUG*/
				SpmdStates[i].exit_status = status;
                                /*
                                 * Se ho trovato resetto i flag di emergenza
                                 */
#ifdef DEBUG
		fprintf(stderr,
		"<<(%d) [spmd_sig_handler] resetto SpmdStatesEmergency  \n", getpid());
#endif /*DEBUG*/
                                SpmdStatesEmergencyPIDFlag=0;
                                SpmdStatesEmergencyPIDStatus=0;
				break;
			}
		}

		/*
		 * Don't check for falling off the end of the loop because
		 * a forked child would only be placed into the table if the
		 * handshake, pty's, exec, etc. all worked.
		 */
		break;

	    case SIGALRM:	/* used merely to break a blocked msgrcv(). */
#ifdef DEBUG
/*				fprintf(stderr,
			"<<(%d) [spmd_sig_handler] ricevuto SIGALRM SpmdStatesEmergencyPIDFlag=%d \n", getpid(), SpmdStatesEmergencyPIDFlag); */
#endif /*DEBUG*/
		if (getppid() == 1) { /* have we been adopted by init ? */
			spmd_ciao();
		}
		alarm ((SpmdSubprocCount?1:5));
		break;
	    default:
		fprintf(stderr,"<<(%d) Unexpected signal caught. Impossible!\n", getpid());
		break;
	}

	signal(sig, spmd_sig_handler);	/* re-arm the signal catcher */
}


/*------------------------------------------------------------------------
 * NAME: 	spmd_read_ptys
 * DESCRIPTION:
 *	Read input from each of the ptys for active processes in turn,
 *	passing the input on to the application in a message.
 *
 *------------------------------------------------------------------------*/

static int spmd_read_ptys()
{
	int i;
	void spmd_dead_proc();

	/* while there are children ... */

	for (i=0; SpmdSubprocCount && (i < MAX_PROCS); i++) 
	{
		if (SpmdStates[i].pid != -1) 
		{
			/* slot is associated with a process */
			int rec = 0, wanted, num;
			spmd_output_t msg;
			char *buf = msg.output;

                        memset(&msg, 0, sizeof(msg));
			wanted = sizeof(msg.output);

			/* read must be non-blocking */

			while ( wanted && 
				(num=read(SpmdStates[i].pty, buf, wanted)) ) 
			{
				if (num==-1) {
					if (errno == EINTR) {
						continue;
					}
					break;
				}
				wanted -= num;
				buf[num] = '\0';	/* tail the string */
				buf += num; /* buffer pointer bump */
				rec += num; /* increment received counter */
			}
			if (rec) {
				msg.mtype = SPMD_OUTPUT;
				msg.pid = SpmdStates[i].pid;

				/* send SPMD_OUTPUT message to application */

				while ((msgsnd(Spmd.spmd_cp_msqid, 
					       &msg, 
					       sizeof(msg)-sizeof(msg.mtype)
							-wanted, 
					       0) == -1) 
				       && (errno == EINTR) ) 
				{
					/* nop body */ ;
				}
			}
			if (SpmdStates[i].exit_status != -1) { 
				/* Process has died. */
				spmd_dead_proc(i);
			}
		}
	}
	return(1);	/* spmd_main() relies on this always succeeding :-) */
}

/*------------------------------------------------------------------------
 * NAME:	spmd_dead_proc( i )
 * INPUT:		int i;		-- subprocess table index
 * DESCRIPTION:
 *	The speciefied process has died.
 *	Send a message to the application to this effect.
 *	Remove the process from the subprocess table
 *
 * LAST REV:
 *------------------------------------------------------------------------*/

static void spmd_dead_proc(i)
int i;
{
	spmd_dead_msg msg;

	msg.mtype = SPMD_DIED;
	ASSERT((SpmdStates[i].pid != -1)&&(SpmdStates[i].exit_status != -1));

	msg.pid = SpmdStates[i].pid;
	msg.status = SpmdStates[i].exit_status;
			
	/* generate the SPMD_DIED message and send it to the application */

	while ( (msgsnd(Spmd.spmd_cp_msqid, &msg, 
			sizeof(msg)-sizeof(msg.mtype), 0) == -1) && 
		(errno == EINTR) ) 
	{
		/* nop body */ ;
	}

	/* Assume the PTY is done with. 
	 * This guy is dead. Bury him and forget him ! 
	 */

	(void) close(SpmdStates[i].pty);
	SpmdStates[i].pid = SpmdStates[i].pty 
			   = SpmdStates[i].exit_status = -1;
	SpmdSubprocCount--; 
	ASSERT(SpmdSubprocCount >= 0);
}

/*------------------------------------------------------------------------
 *	spmd_ciao()
 *
 * The application disappeared. We follow suit.
 * Try and tidy up the message queues. We can't do anything if it fails so
 * we don't even check the return status
 *------------------------------------------------------------------------*/

static void spmd_ciao()
{
	(void)msgctl(Spmd.spmd_pc_msqid, IPC_RMID, NULL);
	(void)msgctl(Spmd.spmd_cp_msqid, IPC_RMID, NULL);
	exit(0);
}

#endif /*SPMD*/

