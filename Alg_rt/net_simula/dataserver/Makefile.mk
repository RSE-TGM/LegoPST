# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-4"
LEGORT_LIB=../../AlgLib
LEGOROOT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../include
LEGORT_BIN=../../bin
LEGORT_UID=../../uid

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#   modulo Makefile
#   tipo 
#   release 5.1
#   data 96/03/26
#   reserved @(#)Makefile	5.1
#
CFLAGS =  -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)  -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB)


LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libutilx.a \
          $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libcom.a $(LEGOROOT_LIB)/libutil.a \
	  $(LEGOROOT_LIB)/libRt.a  $(LEGOROOT_LIB)/libnet.a $(SQLITE_LIB) -lsqlite3


SORGENTI_DS = main_DataServer.c viewshr.c 
OGGETTI_DS = main_DataServer.o viewshr.o
 
all: $(LEGORT_BIN)/DataServer $(LEGORT_BIN)/view.template 


$(LEGORT_BIN)/DataServer: $(OGGETTI_DS)
	cc $(CFLAGS) $(OGGETTI_DS) $(LIBUTIL)  -o $(LEGORT_BIN)/DataServer

$(LEGORT_BIN)/view.template:view.template
	cp view.template $(LEGORT_BIN)/view.template
