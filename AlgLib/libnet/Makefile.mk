# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
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
SORGENTI = io_sono.c rw_net.c getindbyname.c \
	   socketlettura.c socketscrittura.c do_demone.c \
	   socketclient.c socketserver.c \
	   conv.c setta_socket.c test_host.c conf_host.c


OGGETTI  = io_sono.o rw_net.o getindbyname.o \
	   socketlettura.o socketscrittura.o do_demone.o \
	   socketclient.o socketserver.o \
	   conv.o setta_socket.o test_host.o conf_host.o

CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)

all:  $(LEGORT_LIB)/libnet.a

$(LEGORT_LIB)/libnet.a: $(OGGETTI)
	ar rvs $(LEGORT_LIB)/libnet.a $?
	@echo $(LEGORT_LIB)/libnet.a is now up-to-date
.c.a:;

