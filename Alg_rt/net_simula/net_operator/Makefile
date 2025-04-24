# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile-1"
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
C_FLAGS=-g -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c  -DSNAPSHOT
F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              140
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Sep 26 17:47:02 2005 %
CFLAGS = -g -I$(LEGORT_INCLUDE) -I$(LEGOROOT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS) -I$(LEGOROOT_LIB)

LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libnet.a \
	$(LEGORT_LIB)/libdispatcher.a $(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a \
        $(LEGORT_LIB)/libsim.a 

SORGENTI = net_operator.c

OGGETTI  = net_operator.o

all:  $(LEGORT_BIN)/net_operator


$(LEGORT_BIN)/net_operator: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/net_operator -g $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) $(C_LIB) -lm $(SQLITE_LIB)  -lsqlite3

