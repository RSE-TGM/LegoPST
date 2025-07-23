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
SQLITE_LIB= 
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
#	Makefile Header:               Makefile.mk
#       Subsystem:              101
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Mar  7 20:04:11 2006 %
CFLAGSINCL = $(X_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOROOT_LIB) \
	 -I$(LEGOROOT_INCLUDE)
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) $(C_FLAGS)
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) $(SQLITE_LIB) -lsqlite3
LIBUTIL = $(LEGOROOT_LIB)/libutilx.a $(LEGORT_LIB)/libsim.a \
	  $(LEGOROOT_LIB)/libRt.a $(LEGOROOT_LIB)/libutil.a \
	  $(LEGOROOT_LIB)/libnet.a

SORGENTI1 =  co_float.c am1_c.c am2_c.c am3_c.c amd_c.c id1_c.c tr1_c.c\
        sp1_c.c spd_c.c br1_c.c sd1_c.c sa1_c.c mr1_c.c lu1_c.c stazgenc.c

OGGETTI1 =  co_float.o am1_c.o am2_c.o am3_c.o amd_c.o id1_c.o tr1_c.o\
        sp1_c.o spd_c.o br1_c.o sd1_c.o sa1_c.o mr1_c.o lu1_c.o stazgenc.o


SORGENTI2 = compstaz.c co_legge.c co_lungh.c co_sep_s.c checkvar.c\
        cnewstaz.c c_led.c c_puls.c c_pulsluce.c c_selet.c c_lamp.c\
        c_indic.c c_stringa.c c_display.c c_tasto.c c_luce.c\
        c_setval.c err_staz.c c_displayscal.c c_sincro.c

OGGETTI2 = compstaz.o co_legge.o co_lungh.o co_sep_s.o checkvar.o\
        cnewstaz.o c_led.o c_puls.o c_pulsluce.o c_selet.o c_lamp.o\
        c_indic.o c_stringa.o c_display.o c_tasto.o c_luce.o\
        c_setval.o err_staz.o c_displayscal.o c_sincro.o

SORGENTI = $(SORGENTI1) $(SORGENTI2)

all: libstaz_c.a $(LEGORT_BIN)/compstaz


libstaz_c.a: $(OGGETTI1)
	$(CC) -c $(CFLAGS) $(?:.o=.c)
	ar rvs libstaz_c.a $?
	@echo libstaz_c.a is now up-to-date

$(LEGORT_BIN)/compstaz: $(OGGETTI2) $(LIBUTIL) libstaz_c.a
	cc -o $(LEGORT_BIN)/compstaz $(LINKER_OPTIONS) $(OGGETTI2) \
	-lm  $(LIBUTIL) -lX11 $(OTHER_LIB) libstaz_c.a $(LIBSVIL)
