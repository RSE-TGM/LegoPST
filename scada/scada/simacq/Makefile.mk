# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
SCADA_BIN=../../bin
SCADA_INCLUDE=../../incl
SCADA_LIB=../../lib
SCADA_ROOT=../..
SCADA_ARC=../arc
SCADA_SIMACQ=.

# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-L/usr/users/legor2/xprinter/lib.dec3000
UXCGEN=run_uxcgen12.sh $@ $<
SCADA_C_FLAGS=-DWINMMI -DVIRTLBG -DGERARC -DENEL_SIM -DALARM_SIGNAL -DXSCADA_INTERFACE
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIBS=$(X_LIB)
SQLITE_LIB=-I$(LEGOROOT_LIB)/sqlite_include -L$(LEGOROOT_LIB)/sqlite_lib
#------------------------ C preprocessor
#CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %


CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -I$(SCADA_ROOT)/scada/sepacq -D$(OS) $(C_FLAGS) -I$(SCADA_ARC) $(SCADA_C_FLAGS) $(SCADA_OPT)

SORGENTI = simacqui.c invao.c coman.c simsvdbs.c simsvall.c\
     simcldbs.c simclall.c simclpag.c simlddbs.c simvardd.c\
     siminit.c simlgini.c simsdcfg.c simclarc.c simtest.c \
     simsetcom.c


OGGETTI = simacqui.o invao.o coman.o simsvdbs.o simsvall.o\
     simcldbs.o simclall.o simclpag.o simlddbs.o simvardd.o\
     siminit.o simlgini.o simsdcfg.o simclarc.o simtest.o \
     simsetcom.o

all:	$(SCADA_SIMACQ)/libsimacq.a

$(SCADA_SIMACQ)/libsimacq.a: $(OGGETTI)
	ar rvs $(SCADA_SIMACQ)/libsimacq.a $?
	@echo libsimacq.a aggiornata
.c.a:;

