# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-6"
SCADA_BIN=../../bin
SCADA_INCLUDE=../../incl
SCADA_LIB=../../lib
SCADA_ROOT=../..

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
#	Makefile Header:               Makefile.mk
#       Subsystem:              160
#       Description:
#       %created_by:    famgr %
#       %date_created:  Fri May 14 11:23:50 1999 %


CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -I$(SCADA_ROOT)/scada/sepacq -D$(OS) $(C_FLAGS) $(SCADA_C_FLAGS) $(SCADA_OPT) 

SORGENTI = alldef.c allcall.c allabzon.c allcval.c alldec.c alldecminiASD.c  \
	allricon.c allsel.c  allstam.c cunita.c gestri.c allsev.c allriv.c \
	allcomp.c allricsi.c allrall.c allmute.c allpvis.c allpag.c allsric.c \
	allcod.c alllast.c allinger.c allgerzo.c allric.c allricsminiASD.c \
        allmatch.c allselASD.c ricPreviousAll.c

OGGETTI = alldef.o allcall.o allabzon.o allcval.o alldec.o allric.o \
	allricon.o allsel.o allstam.o cunita.o gestri.o allsev.o allriv.o \
	allcomp.o allricsi.o allrall.o allmute.o allpvis.o allpag.o allsric.o \
	allcod.o alllast.o allinger.o allgerzo.o allricsminiASD.o allmatch.o \
        allselASD.o alldecminiASD.o ricPreviousAll.o

all:	$(SCADA_LIB)/liballar.a

$(SCADA_LIB)/liballar.a: $(OGGETTI)
	ar rvs $(SCADA_LIB)/liballar.a $?
	@echo liballar.a aggiornata
.c.a:;

