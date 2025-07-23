# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-6"
SCADA_INCLUDE=../../incl
SCADA_LIBOSF1=..
SCADA_LIB=../../lib
SCADA_TCP=../../tcp
SCADA_ROOT=../..
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
#       Subsystem:              158
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Oct 31 11:24:41 2002 %

CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIBOSF1) -I$(SCADA_ROOT)/scada/mandbrt -I. -D$(OS) \
	-D_BSD   -DXOPEN_CATALOG -DAIXV3 $(C_FLAGS) $(SCADA_C_FLAGS) \
	$(SCADA_OPT)

SORGENTI = Scada.c ScadaDebugMain.c ScadaLogMain.c ScadaMandbMain.c UxXt.c \
other_xscada.c util_xscada.c xscada.c AttivaXscada.c

OGGETTI  = Scada.o ScadaDebugMain.o ScadaLogMain.o ScadaMandbMain.o UxXt.o \
	other_xscada.o util_xscada.o xscada.o AttivaXscada.o

all:  $(SCADA_LIBOSF1)/libXscada.a 

.c.o:
	$(CC) -c $(CFLAGS) $<

$(SCADA_LIBOSF1)/libXscada.a: $(OGGETTI) $(LEGOROOT_LIB)/libCAP.a
	ar rvs $(SCADA_LIBOSF1)/libXscada.a $?
	@echo libXscada.a aggiornata
.c.a:;


