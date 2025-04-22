# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1.1.2"
LEGO_BIN=../bin
LEGO_LIB=../lib

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX -D_NO_PROTO -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) -Wno-implicit-function-declaration
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
CPPFLAGS= -traditional-cpp -C -P -nostdinc -DLINUX
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#  modulo regonewmake
#  tipo 
#  release 1.5
#  data 11/13/95
#  reserved @(#)regonewmake	1.5
#

OBJS=$(LEGO_LIB)/pslego3.o $(LEGO_LIB)/rg5sim.o
OBJS_LIB=pslegoaux.o scanf04.o jflolog.o 
OBJSPF= lgstop.o

all: $(LEGO_BIN)/crealtm $(LEGO_LIB)/regonew.a $(OBJS) $(OBJS_LIB)
#
$(LEGO_BIN)/crealtm: crealtm.f
	$(FC) $(FFLAGS) -o $(LEGO_BIN)/crealtm crealtm.f
#
#
$(OBJSPF): lgstop.pf
	$(CPP) $(CPPFLAGS) lgstop.pf > lgstop.f
	$(FC) $(FFLAGS) -c lgstop.f
$(LEGO_LIB)/regonew.a:$(OBJSPF) $(OBJS_LIB)
	ar ru $(LEGO_LIB)/regonew.a $(OBJSPF) $(OBJS_LIB)

$(LEGO_LIB)/pslego3.o: pslego3.c 
	$(CC) $(CFLAGS) -c pslego3.c
	cp pslego3.o $(LEGO_LIB)/pslego3.o

$(LEGO_LIB)/rg5sim.o: rg5sim.c
	$(CC) $(CFLAGS) -c  rg5sim.c
	cp  rg5sim.o $(LEGO_LIB)/rg5sim.o
