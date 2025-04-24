# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
SCADA_INCLUDE=../incl
SCADA_LIB=../lib
SCADA_LIBUT=.

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
#       Subsystem:              159
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Fri Mar 14 10:45:48 1997 %

CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -D$(OS) $(C_FLAGS) $(SCADA_C_FLAGS) \
	$(SCADA_OPT)

SORGENTI = aggfconf.c allocdb.c bitset.c bitvalue.c ctrlstat.c decnum.c \
    exstati.c get_key.c rdpd.c readdb.c readef.c readiz.c ricerca.c rwdbal.c \
	rwgerarc.c fsec.c ReadAlarmSignal.c

OGGETTI  = aggfconf.o allocdb.o bitset.o bitvalue.o ctrlstat.o decnum.o \
    exstati.o get_key.o rdpd.o readdb.o readef.o readiz.o ricerca.o rwdbal.o \
	rwgerarc.o fsec.o ReadAlarmSignal.o

all:  $(SCADA_LIBUT)/libpscs.a 

$(SCADA_LIBUT)/libpscs.a: $(OGGETTI)
	ar rvs $(SCADA_LIBUT)/libpscs.a $?
	@echo libpscs.a aggiornata
.c.a:;


