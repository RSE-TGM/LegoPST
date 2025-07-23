# ******* Telelogic expanded section *******

# make_macros from makefile "lego_main.mk-11"
FFLAGS=$(F_FLAGS)
LEGO_BIN=../../bin
LEGO_LIB=../../lib


# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS=
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c  -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
#CPPFLAGS=-P -C -DLINUX -traditional
CPPFLAGS= -traditional-cpp -C -P -nostdinc -DLINUX 
CPP_FLAGS= -traditional-cpp -C -P -nostdinc -DLINUX 
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#       Makefile Header:               lego_main.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Apr 29 12:27:38 2004 %
#N.B. versione senza main_refresh.c
.SUFFIXES:  .pf .o .f .c .sh .h .a
FFLAGS=$(F_FLAGS)
#echo $FFLAGS
CFLAGS = -D$(OS) $(VERSIONE) -DUNIX
#
all: $(LEGO_BIN)/lg1a_exe $(LEGO_BIN)/lg4_exe $(LEGO_BIN)/edi14_exe \
     $(LEGO_BIN)/edi14c main_lg2.o main_lg3.o main_lg5.o main_cf04.o\
     main_lg5sk.o main_rg5sk.o \
     $(LEGO_BIN)/svinli2 $(LEGO_BIN)/svinri2 $(LEGO_BIN)/main_modscreg\
     main_svinli2.o main_svinri2.o \
     main_modscreg.o \
     $(LEGO_BIN)/svinforms main_svinforms.o
#
$(LEGO_BIN)/lg1a_exe: main_lg1.o $(LEGO_LIB)/legolib.a
#	make main_lg1.o CPP_FLAGS=
	$(FC) $(FFLAGS) main_lg1.o $(LEGO_LIB)/legolib.a -lc -o $@
#
$(LEGO_BIN)/lg4_exe: main_lg4.o $(LEGO_LIB)/legolib.a
#	make main_lg4.o
	$(FC) $(FFLAGS) main_lg4.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/edi14_exe: main_edi14.o $(LEGO_LIB)/legolib.a
#	make main_edi14.o
	$(FC) $(FFLAGS) main_edi14.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/edi14c: main_edi14c.o $(LEGO_LIB)/legolib.a
#	make main_edi14.o
	$(FC) $(FFLAGS) main_edi14c.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/svinli2: main_svinli2.o $(LEGO_LIB)/legolib.a
#	make main_svinli2.o
	$(FC) $(FFLAGS) main_svinli2.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/svinri2: main_svinri2.o $(LEGO_LIB)/legolib.a
#	make main_svinri2.o
	$(FC) $(FFLAGS) main_svinri2.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/svinforms: main_svinforms.o $(LEGO_LIB)/legolib.a
#	make main_svinforms.o
	$(FC) $(FFLAGS) main_svinforms.o $(LEGO_LIB)/legolib.a -o $@
#
$(LEGO_BIN)/main_modscreg: main_modscreg.o
	$(CC) main_modscreg.o -o $@
#

$(info -------->  DEBUG FFLAGS impostata a = $(FFLAGS))

.f.o:
	$(FC) -c $(FFLAGS) $<
.pf.f:
	/lib/cpp -P $(CPP_FLAGS) $(PREPROCESSOR_OPTIONS) $< > $*.f

              
