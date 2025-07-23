# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
LEGORT_INCLUDE=../libinclude

# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fcommon -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT

#OGGETTI =  skey_manager3.o  clink.o
OGGETTI =  skey_manager3.o
CFLAGS=-I. -I$(LEGORT_INCLUDE) -g

all:  ../libsmartkey.a

../libsmartkey.a: $(OGGETTI)
	ar rvs ../libsmartkey.a $?
	@echo libsmartkey.a aggiornata


