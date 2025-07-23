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
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 5.1
#   data 96/03/19
#   reserved @(#)Makefile	5.1
#
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)

UILINCLUDE = -I/usr/lib/DXM/clients/uil
SORGENTI = clr_ef.c ef_clust.c ef_is_se.c set_ef.c set_nom_.c\
	   tra_nom_.c wait_ef.c end_nom_.c

OGGETTI  = clr_ef.o ef_clust.o\
           ef_is_se.o set_ef.o set_nom_.o\
	   tra_nom_.o wait_ef.o end_nom_.o

all:  $(LEGORT_LIB)/libcom.a

$(LEGORT_LIB)/libcom.a: $(OGGETTI)
	ar rvs $(LEGORT_LIB)/libcom.a $?
	@echo $(LEGORT_LIB)/libcom.a is now up-to-date
.c.a:;


