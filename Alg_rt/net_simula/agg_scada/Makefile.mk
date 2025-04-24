# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-22"
LEGORT_LIB=../../AlgLib
LEGOROOT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../include
LEGORT_BIN=../../bin
LEGORT_UID=../../uid
#PRIVATE_LIB= -lpthreads -lmach -lc_r
PRIVATE_LIB=


# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
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
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              85
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Aug 10 18:39:50 2005 %
#
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) \
	-I$(LEGOROOT_LIB) -I$(LEGOROOT_INCLUDE)
LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libnet.a \
	  $(LEGORT_LIB)/libdispatcher.a $(LEGORT_LIB)/libipc.a \
	  $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libRt.a $(LEGOROOT_LIB)/dcethreads_lib/libdcethreads.a 
LIBSVIL = -lsqlite3  $(SQLITE_LIB) $(THREAD_LIB)

SORGENTI = agg_scada.c agg_scada_ack.c agg_scada_db.c agg_scada_pert.c
OGGETTI  = agg_scada.o agg_scada_ack.o agg_scada_db.o agg_scada_pert.o

all:  $(LEGORT_BIN)/agg_scada


$(LEGORT_BIN)/agg_scada: $(OGGETTI) $(LIBUTIL)
	cc $(CFLAGS) -o $(LEGORT_BIN)/agg_scada $(OGGETTI) \
        $(LIBUTIL) $(OTHER_LIB) $(PRIVATE_LIB) $(LIBSVIL)
