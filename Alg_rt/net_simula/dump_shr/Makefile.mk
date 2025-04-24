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
#   data 93/11/24
#   reserved @(#)Makefile	1.1
#
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) \
	-I$(LEGOROOT_LIB) -I$(LEGOROOT_INCLUDE)

LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libnet.a \
	$(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a \
	$(LEGOROOT_LIB)/libRt.a


SORGENTI = dump_shr.c
OGGETTI  = dump_shr.o

all:  $(LEGORT_BIN)/dump_shr

makefile:Makefile
	make inc

$(LEGORT_BIN)/dump_shr: $(OGGETTI) $(LIBUTIL)
	cc  -o $(LEGORT_BIN)/dump_shr $(OGGETTI) \
        $(LIBUTIL) $(OTHER_LIB)
