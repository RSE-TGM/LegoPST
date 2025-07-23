# ******* Telelogic expanded section *******
$(info -------->  DEBUG Makefile.mk: F_FLAGS impostata a = $(F_FLAGS))
# make_macros from makefile "Makefile.mk-1"
FFLAGS=$(F_FLAGS)
LEGO_BIN=../../bin
LEGO_LIB=../../lib
LEGOCAD_BIN=../../../bin
CAD_LEGO_LIB=../../../lib
LEGORT_INCLUDE=../../AlgLib/libinclude

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
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c  -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
#CPPFLAGS=-P -C -DLINUX -traditional
CPPFLAGS= -traditional-cpp -C -P -nostdinc -DLINUX 
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %
all: 
	$(MAKE) -f lego_main.mk
	$(MAKE) -f cad_lego_main.mk
