# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
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
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              141
#       Description:
#       %created_by:    zanna %
#       %date_created:  Wed Jan 22 12:26:51 1997 %
CFLAGSINCLUDE = -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -D$(OS) \
		$(VERSIONE) $(C_FLAGS)  -I$(LEGOROOT_LIB)

LIBUTIL =  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
	   $(LEGOROOT_LIB)/libutil.a

LIBSVIL = $(X_LIB)

SORGENTI = rest_main.c rest_func.c 

OGGETTI  = rest_main.o rest_func.o 

CFLAGS=$(CFLAGSINCLUDE)

all:  $(LEGORT_BIN)/restsess

$(LEGORT_BIN)/restsess: $(OGGETTI) $(LIBUTIL)
	cc -g -o $(LEGORT_BIN)/restsess $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBSVIL) $(LIBUTIL) $(C_LIB) $(OTHER_LIB) $(F_LIB)
