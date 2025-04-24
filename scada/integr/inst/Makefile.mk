# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"
SCADA_INCLUDE=../../incl
SCADA_STAMPE=../../scada/stampe
SCADA_SIMACQ=../../scada/simacq
SCADA_INTEGR_INCLUDE=../include
SCADA_LIBOSF1=../../libOSF1
SCADA_INTEGR_LIB=../lib
SCADA_BIN=../../bin
SCADA_LIB=../../lib
SCADA_LIBUT=../../libut
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
SCADA_ARC=../../scada/arc
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib

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
#THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -lpthread -ldl
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
#       Subsystem:              170
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Mar  7 19:41:07 2006 %

# Informix bin
INFORMIXBIN = $(INFORMIXDIR)/bin

# Compilers
#ECCOMP = $(INFORMIXBIN)/esql

# nel caso di utilizzo di informix definire
# COMPILA = $(ECCOMP)
# altrimenti
# COMPILA = cc
COMPILA = cc

CFLAGS = -I$(SCADA_INTEGR_INCLUDE) -I$(SCADA_INCLUDE) -I$(SCADA_STAMPE)/virt -D$(OS) $(C_FLAGS) \
	-DASCII_DB -I. -I$(SCADA_ARC) $(SCADA_C_FLAGS) \
	-I$(SCADA_SIMACQ)  \
	-I$(LEGORT_INCLUDE) -I$(LEGOROOT_INCLUDE) \
	$(SCADA_OPT) 
LIBUTIL = $(SCADA_LIB)/libUtil.a $(SCADA_LIBUT)/libpscs.a \
	$(SCADA_LIBOSF1)/libMarte.a $(SCADA_INTEGR_LIB)/libdb.a \
	$(LIBINFORMIXPUERTO) $(SCADA_LIB)/libUtil.a \
	$(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
	$(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libRt.a \
	$(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libnet.a

SORGENTI = installa.c installa_aa.c installa_variabili.c \
	installa_as.c installa_ad.c installa_dd.c installa_ao.c \
	installa_da.c installa_do.c installa_ac.c installa_nomi.c\
	installa_ds.c installa_dc.c installa_arc.c installa_or.c \
	installa_st.c installa_to.c installa_TabPeri.c installa_CPerif.c \
	installa_DescriSc.c installa_MisTabK.c installa_TDIZST.c \
	installa_AllarDB.c installa_AllarLin.c installa_AllarLiv.c \
	installa_AllarStr.c installa_AllarSer.c installa_ConfDB.c \
	installa_log.c installa_err.c installa_informix.c pthread_kill_other_threads_np.c

OGGETTI  = installa.o installa_aa.o installa_variabili.o \
	installa_as.o installa_ad.o installa_dd.o installa_ao.o \
	installa_da.o installa_do.o installa_ac.o installa_nomi.o\
	installa_ds.o installa_dc.o installa_arc.o installa_or.o \
	installa_st.o installa_to.o installa_TabPeri.o installa_CPerif.o \
	installa_DescriSc.o installa_MisTabK.o installa_TDIZST.o \
	installa_AllarDB.o installa_AllarLin.o installa_AllarLiv.o \
	installa_AllarStr.o installa_AllarSer.o installa_ConfDB.o \
	installa_log.o installa_err.o installa_informix.o pthread_kill_other_threads_np.o


all:  $(SCADA_BIN)/PointInst $(SCADA_BIN)/SortTable


$(SCADA_BIN)/PointInst: $(OGGETTI) $(LIBUTIL)
	$(COMPILA) -o $(SCADA_BIN)/PointInst  $(OGGETTI) \
        $(LIBUTIL) $(SQLITE_LIB) $(THREAD_LIB) -lm -lsqlite3


$(SCADA_BIN)/SortTable: SortTable.sh
	cp SortTable.sh $(SCADA_BIN)/SortTable
	chmod +x $(SCADA_BIN)/SortTable
	chmod +w $(SCADA_BIN)/SortTable


.c.o:
	cc -c $(CFLAGS) $< -o $@
