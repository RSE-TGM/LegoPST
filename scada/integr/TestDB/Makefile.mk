# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
SCADA_INCLUDE=../../incl
SCADA_STAMPE=../../scada/stampe
SCADA_SIMACQ=../../scada/simacq
SCADA_INTEGR_INCLUDE=../include
SCADA_LIBOSF1=../../libOSF1
SCADA_INTEGR_LIB=../lib
SCADA_BIN=../../bin
SCADA_LIB=../../lib
SCADA_LIBUT=../../libut


# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
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
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

CFLAGS = -I$(SCADA_INCLUDE) -D$(OS) $(C_FLAGS) -g -DASCII_DB -I. \
	-I$(SCADA_SIMACQ)
LIBUTIL = $(SCADA_LIB)/libUtil.a $(SCADA_LIBUT)/libpscs.a \
	$(SCADA_LIBOSF1)/libMarte.a

SORGENTI = testdb.c 
OGGETTI  = testdb.o 

all:  $(SCADA_BIN)/testdb


$(SCADA_BIN)/testdb: $(OGGETTI) $(LIBUTIL)
	cc -o $(SCADA_BIN)/testdb  $(OGGETTI) \
        $(LIBUTIL) $(THREAD_LIB) -lm
