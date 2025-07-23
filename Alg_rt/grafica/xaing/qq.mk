# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-11"
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
#SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
#UIL_COMPILER=/usr/bin/X11/uil
UIL_COMPILER=/usr/bin/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              75
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Mar  7 19:55:25 2006 %
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) \
		-I$(LEGOROOT_LIB) 
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) $(VERSIONE) -D$(OS) $(C_FLAGS)
UIL = $(UIL_COMPILER)
#LIBSVIL = $(X_LIB) -lsqlite3  $(SQLITE_LIB)
LIBSVIL = $(X_LIB) -lsqlite3 
LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libutilx.a \
          $(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a \
	  $(LEGOROOT_LIB)/libRt.a $(LEGOROOT_LIB)/libnet.a

SORGENTI1 = mandb.c mandb_io.c
OGGETTI1 = mandb.o mandb_io.o
SORGENTI2 = acqmandb.c
OGGETTI2  = acqmandb.o
SORGENTI = $(SORGENTI1) $(SORGENTI2)

all:  $(LEGORT_UID)/mandb.uid $(LEGORT_BIN)/mandb $(LEGORT_BIN)/acqmandb


$(LEGORT_UID)/mandb.uid: mandb.uil$(MOTIF_VER)
	$(UIL) -o $(LEGORT_UID)/mandb.uid mandb.uil$(MOTIF_VER) $(UIL_INCLUDE) 

$(LEGORT_BIN)/mandb: $(OGGETTI1) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/mandb $(LINKER_OPTIONS) $(OGGETTI1)\
        $(LIBUTIL) -lX11 $(STUB_LIBS) $(C_LIB) $(OTHER_LIB) $(LIBSVIL)

$(LEGORT_BIN)/acqmandb: $(OGGETTI2) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/acqmandb $(LINKER_OPTIONS) $(OGGETTI2) \
        $(LIBUTIL) $(OTHER_LIB) $(LIBSVIL)
