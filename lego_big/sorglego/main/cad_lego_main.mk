# ******* Telelogic expanded section *******

# make_macros from makefile "cad_lego_main.mk-12"
FFLAGS=$(F_FLAGS)
LEGO_BIN=../../bin
LEGO_LIB=../../lib
LEGOCAD_BIN=../../bin
CAD_LEGO_LIB=../../lib

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX -D_NO_PROTO -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c -D_NO_PROTO -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
CPPFLAGS=-P -C -DLINUX -traditional
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero -std=legacy

#
#       Makefile Header:               cad_lego_main.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Feb 19 17:41:32 2007 %
#N.B. in questa versione non viene compilato main_refresh.c
.SUFFIXES:  .pf .o .f .c .sh .h .a
FFLAGS=$(F_FLAGS)
CFLAGS = -D$(OS) $(VERSIONE) -DUNIX
#
all: $(CAD_LEGO_LIB)/cmain_lg1.o $(LEGOCAD_BIN)/clg1a_exe
#

$(LEGOCAD_BIN)/clg1a_exe: $(CAD_LEGO_LIB)/cmain_lg1.o $(CAD_LEGO_LIB)/clegolib.a
	$(FC) $(FFLAGS) $(CAD_LEGO_LIB)/cmain_lg1.o \
                        $(CAD_LEGO_LIB)/clegolib.a -lc -o $@
$(CAD_LEGO_LIB)/cmain_lg1.o: main_lg1.pf $(CAD_LEGO_LIB)/lg_parameter.fh $(CAD_LEGO_LIB)/main_lg1.pf
	/lib/cpp $(CPPFLAGS) -DLEGOCAD  $(CAD_LEGO_LIB)/main_lg1.pf \
		         > main_lg1.f
	$(FC) -c $(FFLAGS) main_lg1.f -o $(CAD_LEGO_LIB)/cmain_lg1.o

$(CAD_LEGO_LIB)/lg_parameter.fh:  lg_parameter.fh
	cp -f $? $@

$(CAD_LEGO_LIB)/main_lg1.pf: main_lg1.pf
	cp -f $? $@

	
#$(CAD_LEGO_LIB)/cmain_lg1.o: main_lg1.pf
#	cp -f main_lg1.pf $(CAD_LEGO_LIB)/main_lg1.pf
#	chmod +w $(CAD_LEGO_LIB)/main_lg1.pf
#	/lib/cpp $(CPPFLAGS) -DLEGOCAD  $(CAD_LEGO_LIB)/main_lg1.pf \
#		         > $(CAD_LEGO_LIB)/main_lg1.f
#	$(FC) -c $(FFLAGS) $(CAD_LEGO_LIB)/main_lg1.f -o $(CAD_LEGO_LIB)/cmain_lg1.o


              
