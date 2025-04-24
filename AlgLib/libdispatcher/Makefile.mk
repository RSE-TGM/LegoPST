# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-4"
LEGORT_LIB=..
LEGOROOT_LIB=..
LEGOROOT_INCLUDE=../libinclude
LEGORT_INCLUDE=../libinclude


# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fcommon -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 5.1
#   data 96/03/19
#   reserved @(#)Makefile	5.1
#

SORGENTI = to_dispatcher.c from_dispatcher.c SD_aggancio.c \
	SD_freeze.c SD_inizializza.c SD_run.c SD_goup.c SD_stepcr.c \
	SD_stop.c SD_timescaling.c SD_editic.c SD_loadic.c SD_saveic.c \
	SD_chiusura.c SD_sgancio.c SD_removeic.c SD_backtrack.c SD_lsnapreg.c \
	SD_lsnapload.c SD_lbtreg.c SD_lbtload.c SD_stato.c SD_listaci.c \
	SD_editbt.c SD_loadbt.c SD_savebt.c SD_clear.c SD_editpert.c \
	SD_removepert.c SD_replay.c SD_allinea.c SD_stepscaling.c \
	SD_editdata.c SD_modidata.c SD_modic.c SD_newvarcr.c \
	SD_validatebt.c SD_varupd.c SD_maxsimtime.c

OGGETTI  = to_dispatcher.o from_dispatcher.o SD_aggancio.o \
	SD_freeze.o SD_inizializza.o SD_run.o SD_goup.o SD_stepcr.o \
	SD_stop.o SD_timescaling.o SD_editic.o SD_loadic.o SD_saveic.o \
	SD_chiusura.o SD_sgancio.o SD_removeic.o SD_backtrack.o SD_lsnapreg.o \
	SD_lsnapload.o SD_lbtreg.o SD_lbtload.o SD_stato.o SD_listaci.o \
	SD_editbt.o SD_loadbt.o SD_savebt.o SD_clear.o SD_editpert.o \
	SD_removepert.o SD_replay.o SD_allinea.o SD_stepscaling.o \
	SD_editdata.o SD_modidata.o SD_modic.o SD_newvarcr.o \
	SD_validatebt.o SD_varupd.o SD_maxsimtime.o


CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)

all:  $(LEGORT_LIB)/libdispatcher.a

makefile:Makefile
	make inc

$(LEGORT_LIB)/libdispatcher.a: $(OGGETTI)
	ar rvs $(LEGORT_LIB)/libdispatcher.a $?
	@echo $(LEGORT_LIB)/libdispatcher.a is now up-to-date
.c.a:;

inc:
	$(DEPEND) -s "# DO NOT DELETE" -- $(CFLAGS) -- $(SORGENTI)
	make

