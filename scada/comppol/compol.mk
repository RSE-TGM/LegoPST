# ******* Telelogic expanded section *******

# make_macros from makefile "compol.mk-1.1.2"
SCADA_INCLUDE=../incl
SCADA_LIB=../lib
SCADA_BIN=../bin


# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-L/usr/users/legor2/xprinter/lib.dec3000
UXCGEN=run_uxcgen12.sh $@ $<
SCADA_C_FLAGS=-DWINMMI -DVIRTLBG -DGERARC -DENEL_SIM -DALARM_SIGNAL -DXSCADA_INTERFACE
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIBS=$(X_LIB)
SQLITE_LIB=-I$(LEGOROOT_LIB)/sqlite_include -L$(LEGOROOT_LIB)/sqlite_lib
#------------------------ C preprocessor
#CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              152
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Oct 31 16:17:09 2002 %

CFLAGS = -I$(C_FLAGS) -pthread -I$(SCADA_INCLUDE) -D$(OS) $(SCADA_C_FLAGS) $(SCADA_OPT)\
	-I. -I/usr/sys/include/sys -I./include/dedf

LIBUTIL = ./dlib/dassist/libassl6.a $(SCADA_LIB)/libUtil.a ./uti/utility.a

SORGENTI = oplogich.c op_io.c oparitm.c oplstarg.c errori.c \
	subr.c send.c se.c scegli.c step2.c cerca.c \
	cambianv.c cnome.c insvar_s.c locmem.c \
	altre.c inser_in.c pushinc.c popinc.c \
	detnriga.c delfcomp.c gesdizio.c execblc.c \
	scrvar_c.c mapfile.c scriviva.c main.c \
	help_men.c insmap.c ctrprint.c prvar.c \
	prgraf.c prstring.c ctrwait.c ctrloop.c \
	creploop.c ctrstop.c procloop.c \
	prbrloop.c prfloop.c ctrtipo.c estrnome.c \
	format.c issep.c tstnomva.c ctrfrewr.c \
	istrnoar.c ctrtpgen.c getnovar.c ctreninc.c \
	cercacol.c declink.c decgrp.c cercastr.c opmov.c

OGGETTI = oplogich.o op_io.o oparitm.o oplstarg.o errori.o \
	subr.o send.o se.o scegli.o step2.o cerca.o \
	cambianv.o cnome.o insvar_s.o locmem.o \
	altre.o inser_in.o pushinc.o popinc.o \
	detnriga.o delfcomp.o gesdizio.o execblc.o \
	scrvar_c.o mapfile.o scriviva.o main.o \
	help_men.o insmap.o ctrprint.o prvar.o \
	prgraf.o prstring.o ctrwait.o ctrloop.o \
	creploop.o ctrstop.o procloop.o \
	prbrloop.o prfloop.o ctrtipo.o estrnome.o\
	format.o issep.o tstnomva.o ctrfrewr.o \
	istrnoar.o ctrtpgen.o getnovar.o ctreninc.o \
	cercacol.o declink.o decgrp.o cercastr.o opmov.o

all: $(SCADA_BIN)/compila

$(SCADA_BIN)/compila: $(OGGETTI) $(LIBUTIL)
	cc -o $(SCADA_BIN)/compila  $(OGGETTI) \
	$(LIBUTIL) -lm
