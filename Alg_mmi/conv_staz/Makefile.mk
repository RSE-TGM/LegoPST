# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=/lib/libbsd.a
TOOLS=../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX -DLINUX -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#   modulo Makefile
#   tipo 
#   release 1.7
#   data 93/10/29
#   reserved @(#)Makefile	1.7
#
DEPEND=$(LEGOROOT)/install/makedepend
CFLAGSINCL = -I. $(X_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGORT_LIB) 
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) $(LINKER_OPTIONS)
UIL = $(UIL_COMPILER)
LIBSVIL =-L /usr/X11R6/lib -lMrm -lXm -lXt -lX11
#LIBUTIL = $(LEGOROOT_LIB)/libutilx.a #$(LEGORT_LIB)/libsim.a

SORGENTI1 =  co_float.c am1_c.c am2_c.c am3_c.c amd_c.c id1_c.c tr1_c.c\
        sp1_c.c spd_c.c br1_c.c sd1_c.c sa1_c.c mr1_c.c lu1_c.c stazgenc.c

OGGETTI1 =  co_float.o am1_c.o am2_c.o am3_c.o amd_c.o id1_c.o tr1_c.o\
        sp1_c.o spd_c.o br1_c.o sd1_c.o sa1_c.o mr1_c.o lu1_c.o stazgenc.o


SORGENTI2 = convstaz.c co_legge.c co_lungh.c co_sep_s.c checkvar.c\
        cnewstaz.c c_led.c c_puls.c c_pulsluce.c c_selet.c c_lamp.c\
        c_indic.c c_stringa.c c_display.c c_tasto.c c_luce.c\
        c_setval.c err_staz.c c_displayscal.c c_sincro.c  \
	conv_mmi.c

OGGETTI2 = convstaz.o co_legge.o co_lungh.o co_sep_s.o checkvar.o\
        cnewstaz.o c_led.o c_puls.o c_pulsluce.o c_selet.o c_lamp.o\
        c_indic.o c_stringa.o c_display.o c_tasto.o c_luce.o\
        c_setval.o err_staz.o c_displayscal.o c_sincro.o  \
	conv_mmi.o

SORGENTI = $(SORGENTI1) $(SORGENTI2)
OGGETTI = $(OGGETTI1) $(OGGETTI2)

all:  $(LEGOMMI_BIN)/convstaz

makefile:Makefile
	make inc


$(LEGOMMI_BIN)/convstaz: $(OGGETTI) $(LIBUTIL) 
	cc -o $(LEGOMMI_BIN)/convstaz $(OGGETTI) \
	-lm $(LIBSVIL) $(LIBUTIL) -lX11 

