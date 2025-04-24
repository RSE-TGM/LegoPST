# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3.1.1"
SCADA_INCLUDE=../../../incl
SCADA_LIB=../../../lib
SCADA_INTEGR_INCLUDE=../../include
LEGORT_INCLUDE=../../../AlgLib/libinclude
SCADA_INTEGR_LIB=../../lib

# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
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
#	Makefile Header:               Makefile.mk
#       Subsystem:              168
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Oct 31 15:32:09 2002 %

CFLAGS = -I$(SCADA_INTEGR_INCLUDE)  -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -D$(OS) $(C_FLAGS) \
	-DASCII_DB $(SCADA_C_FLAGS) \
	-I$(LEGORT_INCLUDE) $(SCADA_OPT)

SORGENTI = db.c installa_func.c

OGGETTI  = db.o installa_func.o

all:  $(SCADA_INTEGR_LIB)/libdb.a 

$(SCADA_INTEGR_LIB)/libdb.a: $(OGGETTI)
	ar rvs $(SCADA_INTEGR_LIB)/libdb.a $?
	@echo libdb.a aggiornata
.c.a:;

