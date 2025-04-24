# ******* Telelogic expanded section *******

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
#   modulo Makefile
#   tipo 
#   release 1.1
#   data 93/08/09
#   reserved @(#)Makefile	1.1
#
DEPEND=$(LEGOROOT)/install/makedepend
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS) \
	-I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB)

LIBUTIL = $(LEGORT_LIB)/libmanovra.a \
	$(LEGORT_LIB)/libdispatcher.a \
	$(LEGORT_LIB)/libsim.a \
  	$(LEGORT_LIB)/libipc.a \
 	$(LEGORT_LIB)/libnet.a \
	$(LEGOROOT_LIB)/libutil.a \
	$(LEGOROOT_LIB)/libRt.a \
	$(LEGORT_LIB)/libsim.a 


SORGENTI = agg_manovra.c agg_manovra_prolog.c agg_manovra_task.c \
	scoda_dispatcher.c  send_manovra.c 

OGGETTI = agg_manovra.o agg_manovra_prolog.o agg_manovra_task.o \
        scoda_dispatcher.o  send_manovra.o

SORGENTI_STATO = agg_manovra_stato.c

OGGETTI_STATO = agg_manovra_stato.o

SORGENTI_PERT = agg_manovra_pert.c

OGGETTI_PERT = agg_manovra_pert.o



all:  $(LEGORT_BIN)/agg_manovra $(LEGORT_BIN)/agg_manovra_stato $(LEGORT_BIN)/agg_manovra_pert # $(LEGORT_BIN)/bm

makefile:Makefile
	make inc

$(LEGORT_BIN)/agg_manovra: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/agg_manovra $(OGGETTI) \
        $(LIBUTIL) $(C_LIB) -lm

$(LEGORT_BIN)/agg_manovra_stato: $(OGGETTI_STATO) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/agg_manovra_stato $(OGGETTI_STATO) \
        $(LIBUTIL) $(C_LIB) $(OTHER_LIB)

$(LEGORT_BIN)/agg_manovra_pert: $(OGGETTI_PERT) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/agg_manovra_pert $(OGGETTI_PERT) \
        $(LIBUTIL) $(C_LIB) -lm

$(LEGORT_BIN)/bm: bm.o  $(LIBUTIL)
	cc -o $(LEGORT_BIN)/bm bm.o \
	$(LIBUTIL) $(C_LIB) -lm

