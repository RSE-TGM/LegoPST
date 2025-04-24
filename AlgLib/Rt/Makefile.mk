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
#   release 5.3
#   data 96/03/20
#   reserved @(#)Makefile	5.3
#
SORGENTI = RtDbPunti.c RtErrore.c RtMemory.c
OGGETTI  = RtDbPunti.o RtErrore.o RtMemory.o
CFLAGS =  -D$(OS) $(VERSIONE) $(C_FLAGS) -I$(LEGOROOT_LIB) \
-I$(LEGOROOT_INCLUDE) 


all:  $(LEGOROOT_LIB)/libRt.a

$(LEGOROOT_LIB)/libRt.a: $(OGGETTI)
	ar rvs $(LEGOROOT_LIB)/libRt.a $?
	@echo $(LEGOROOT_LIB)/libRt.a is now up-to-date
.c.a:;

