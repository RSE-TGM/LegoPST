# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2.2.1"
LEGORT_BIN=../../bin
LEGORT_LIB=../../AlgLib
LEGOROOT_LIB=../../AlgLib


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
#	Makefile Header:               Makefile.mk
#       Subsystem:              192
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Tue Jun 11 11:21:02 2002 %

CFLAGS = -I/usr/include -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)  -I$(LEGOROOT_LIB) -g

LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libnet.a \
  $(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a $(LEGORT_LIB)/libsim.a

SORGENTI = mkListaMalf.c

OGGETTI  = mkListaMalf.o

all:  $(LEGORT_BIN)/mkListaMalf $(LEGORT_BIN)/creaMalf


$(LEGORT_BIN)/mkListaMalf: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/mkListaMalf $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) $(C_LIB) $(OTHER_LIB)
$(LEGORT_BIN)/creaMalf:creaMalf.sh
	cp creaMalf.sh $(LEGORT_BIN)/creaMalf ; chmod 755 $(LEGORT_BIN)/creaMalf
