# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
SCADA_INCLUDE=../../incl
SCADA_LIBOSF1=..
SCADA_LIB=../../lib
SCADA_TCP=../../tcp
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
#       Subsystem:              155
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon May 24 17:29:18 2004 %

CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -D$(OS) $(C_FLAGS) \
	-I$(SCADA_LIBOSF1) -I$(SCADA_TCP) $(SCADA_C_FLAGS) $(SCADA_OPT)

SORGENTI = socketclient.c socketserver.c io_sono.c getindbyname.c \
		rw_net.c setta_socket.c DriverRete.c

OGGETTI  = socketclient.o socketserver.o io_sono.o getindbyname.o \
		rw_net.o setta_socket.o DriverRete.o

all:  $(SCADA_LIB)/libNet.a 

.c.o:
	$(CC) -c $(CFLAGS) $<

$(SCADA_LIB)/libNet.a: $(OGGETTI)
	ar rvs $(SCADA_LIB)/libNet.a $?
	@echo libNet.a aggiornata
.c.a:;


