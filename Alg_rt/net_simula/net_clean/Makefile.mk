# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5.1.1"
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
#       Subsystem:              81
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Tue Jun 11 11:35:47 2002 %
CFLAGS = -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB) -I$(LEGOMMI_LIB) -D$(OS) $(C_FLAGS) $(VERSIONE) -I/usr/include

LIBUTIL = $(LEGOROOT_LIB)/libutil.a
SORGENTI = killsim.c

OGGETTI = killsim.o

all:  $(LEGORT_BIN)/killsim


$(LEGORT_BIN)/killsim: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/killsim $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) $(OTHER_LIB) -lc
