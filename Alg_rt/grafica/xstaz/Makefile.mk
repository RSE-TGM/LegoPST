# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
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
#	Makefile Header:               Makefile.mk
#       Subsystem:              99
#       Description:
#       %created_by:    zanna %
#       %date_created:  Fri Jun 21 19:06:21 1996 %
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE)\
             -I$(LEGOROOT_LIB) -I$(LEGOROOT_INCLUDE)/widget
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) $(VERSIONE) -D$(OS) $(C_FLAGS)
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB)
LIBUTIL = $(LEGOROOT_LIB)/libutilx.a $(LEGORT_LIB)/libsim.a \
	$(LEGORT_LIB)/libcom.a $(LEGORT_LIB)/libwidget.a \
	$(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a \
	$(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
	$(LEGOROOT_LIB)/libRt.a

SORGENTI1 = am1_r.c am2_r.c am3_r.c amd_r.c id1_r.c tr1_r.c sp1_r.c spd_r.c\
        br1_r.c sd1_r.c sa1_r.c lu1_r.c  mr1_r.c stazgenr.c

OGGETTI1 = am1_r.o am2_r.o am3_r.o amd_r.o id1_r.o tr1_r.o sp1_r.o spd_r.o\
        br1_r.o sd1_r.o sa1_r.o lu1_r.o  mr1_r.o stazgenr.o


SORGENTI2 = xstaz.c cnewstaz.c gled.c gstringa.c glampada.c gpulsluce.c\
        gpulsante.c gselet.c gdisplay.c gindic.c gluce.c gtasto.c\
        gsetval.c gdisplayscal.c gsincro.c

OGGETTI2 = xstaz.o cnewstaz.o gled.o gstringa.o glampada.o gpulsluce.o\
        gpulsante.o gselet.o gdisplay.o gindic.o gluce.o gtasto.o\
        gsetval.o gdisplayscal.o gsincro.o

SORGENTI = $(SORGENTI1) $(SORGENTI2)                                                                     
all:  libstaz_r.a $(LEGORT_BIN)/xstaz 


libstaz_r.a: $(OGGETTI1)
	$(CC) -c $(CFLAGS) $(?:.o=.c)
	ar rvs libstaz_r.a $?
	@echo libstaz_r.a is now up-to-date

$(LEGORT_BIN)/xstaz: $(OGGETTI2) $(LIBUTIL) libstaz_r.a
	cc -o $(LEGORT_BIN)/xstaz $(OGGETTI2) $(LINKER_OPTIONS) libstaz_r.a \
	$(LIBUTIL) $(C_LIB) $(LIBSVIL)  -lX11 $(STUB_LIBS)  $(OTHER_LIB) -lm

$(LEGORT_BIN)/trypag: trypag.c
	$(CC) -o $(LEGORT_BIN)/trypag trypag.c libstaz_r.a \
	$(LIBUTIL)
