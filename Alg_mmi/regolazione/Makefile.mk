# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-8"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=-lbsd


# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g -fcommon  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              36
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Apr  6 10:57:18 2004 %

#   modulo Makefile
#   tipo 
#   release 5.2
#   data 96/03/22
#   reserved @(#)Makefile	5.2
#
.SUFFIXES: .pf .o .f .c .sh .h .a
CFLAGS = -I$(LEGOMMI_INCLUDE) $(X_FLAGS) $(X_INCLUDE) -I$(LEGOMMI_LIB) \
	-D$(OS) $(VERSIONE) -DUNIX -I$(LEGOMMI_LIB)/Ol $(C_FLAGS) \
	-I$(LEGOROOT_INCLUDE) -I$(UX_DIR)/include 
#FFLAGS = -O -I$(LEGOROOT_INCLUDE)
FFLAGS = $(F_FLAGS)

X_LIBS      = $(STUB_LIBS)

LIBUTIL = \
	$(LEGOROOT_LIB)/libutil.a $(LEGORT_LIB)/libnet.a\
	$(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a


SORGENTI_TOP = gen_topologia.c read_topologia.c gen_util.c
OGGETTI_TOP  = gen_topologia.o read_topologia.o gen_util.o

SORGENTI_G14 = gen_f14.c read_topologia.c gen_util.c
OGGETTI_G14  = gen_f14.o read_topologia.o gen_util.o

SORGENTI_01TASK = gen_f01task.c  gen_util.c
OGGETTI_01TASK  = gen_f01task.o gen_util.o

SORGENTI_14TASK = gen_f14task.c  gen_util.c
OGGETTI_14TASK  = gen_f14task.o gen_util.o

SORGENTI_STZRG = gen_stazionario.c  gen_util.c
OGGETTI_STZRG  = gen_stazionario.o gen_util.o

SORGENTI_SCHE_F = gen_sche.f 
OGGETTI_SCHE_F  = gen_sche.o

all:  $(LEGOMMI_BIN)/gen_topologia  $(LEGOMMI_BIN)/gen_f14  \
	$(LEGOMMI_BIN)/gen_stazionario $(LEGOMMI_BIN)/gen_f01task \
	$(LEGOMMI_BIN)/gen_sche \
	$(LEGOMMI_BIN)/gen_f14task 

$(LEGOMMI_BIN)/gen_topologia: $(OGGETTI_TOP) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/gen_topologia $(OGGETTI_TOP) \
	$(LIBUTIL) $(X_LIBS) -lm $(C_LIB)

$(LEGOMMI_BIN)/gen_f14: $(OGGETTI_G14) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/gen_f14 $(OGGETTI_G14) \
        $(LIBUTIL) $(X_LIBS) -lm $(C_LIB)

$(LEGOMMI_BIN)/gen_f01task: $(OGGETTI_01TASK) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/gen_f01task $(OGGETTI_01TASK) \
        $(LIBUTIL) $(X_LIBS) -lm $(C_LIB)

$(LEGOMMI_BIN)/gen_f14task: $(OGGETTI_14TASK) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/gen_f14task $(OGGETTI_14TASK) \
        $(LIBUTIL) $(X_LIBS) -lm $(C_LIB)

$(LEGOMMI_BIN)/gen_stazionario: $(OGGETTI_STZRG) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/gen_stazionario $(OGGETTI_STZRG) \
        $(LIBUTIL) $(X_LIBS) -lm $(C_LIB)

# Prima dell'uso di .pf
#$(LEGOMMI_BIN)/gen_sche:  $(OGGETTI_SCHE_F) $(LIBUTIL)
#	$(FC) $(FFLAGS)  -o $(LEGOMMI_BIN)/gen_sche $(OGGETTI_SCHE_F) \
#	$(LIBUTIL) $(X_LIBS) -lbsd  -lc

$(LEGOMMI_BIN)/gen_sche:  gen_sche.o $(LIBUTIL)
	$(FC) $(FFLAGS)  -o $(LEGOMMI_BIN)/gen_sche gen_sche.o \
	$(LIBUTIL) $(X_LIBS)  -lc $(C_LIB)

.f.o:
	$(FC) -c $(FFLAGS) $<
.pf.o:
	/lib/cpp -C -P -traditional $(CPP_FLAGS) $(PREPROCESSOR_OPTIONS) $< > $*.f
	-$(FC) -c $(FFLAGS) $*.f
	rm -f $*.f
