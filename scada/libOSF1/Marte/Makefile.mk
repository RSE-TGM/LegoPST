# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
SCADA_INCLUDE=../../incl
SCADA_LIBOSF1=..
LEGOROOT_LIB=../../AlgLib


# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
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
#F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              154
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon May 24 17:21:45 2004 %

#*************************************************
#*     Makefile generato con le dipendenze       *
#*     a partire dal file Makefile.tpl           *

CFLAGS =$(C_FLAGS)  -I$(SCADA_INCLUDE) -I$(SCADA_LIBOSF1) -D$(OS) $(C_FLAGS) $(SCADA_C_FLAGS) $(SCADA_OPT)

SORGENTI = OnceLibMarte.c Rew.c Tra.c Wai.c Private.c Ter.c Pri.c \
	   Res.c Kil.c Tas.c Esp.c Suw.c Sce.c Scd.c Syn.c RWbyte.c \
	   SynSim.c WaiSim.c

OGGETTI  = OnceLibMarte.o Rew.o Tra.o Wai.o Private.o Ter.o Pri.o \
	   Res.o Kil.o Tas.o Esp.o Suw.o Sce.o Scd.o Syn.o RWbyte.o \
	   SynSim.o WaiSim.o

all:  $(SCADA_LIBOSF1)/libMarte.a 

.c.o:
	$(CC) -c $(CFLAGS) $<

$(SCADA_LIBOSF1)/libMarte.a: $(OGGETTI)
	ar rvs $(SCADA_LIBOSF1)/libMarte.a $?
	@echo libMarte.a aggiornata
