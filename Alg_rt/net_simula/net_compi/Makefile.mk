# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"
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
C_FLAGS=-g -D$(PLATFORM) -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
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
#       Subsystem:              76
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Mar 17 16:42:47 2005 %
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)  -I$(LEGOROOT_INCLUDE)

LIBUTIL = $(LEGORT_LIB)/libsim.a  \
	$(LEGORT_LIB)/libnet.a $(LEGOROOT_LIB)/libutil.a \
	 $(LEGOROOT_LIB)/libRt.a -lsqlite3

SORGENTI = co_main.c co_legge.c co_sep_s.c check_dt.c tabella.c \
             f04_s04.c check_i.c check_o.c check_m.c co_n04_s.c \
             tabelle_net.c co_len04.c co_snap.c 

OGGETTI = co_main.o co_legge.o co_sep_s.o check_dt.o tabella.o f04_s04.o\
          check_i.o check_o.o check_m.o co_n04_s.o tabelle_net.o \
          co_len04.o co_snap.o 

all:  $(LEGORT_BIN)/net_compi


$(LEGORT_BIN)/net_compi: $(OGGETTI) $(LIBUTIL)
	 $(CC) $(CFLAGS)  -o $(LEGORT_BIN)/net_compi $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) $(OTHER_LIB) -lc 
                                   
