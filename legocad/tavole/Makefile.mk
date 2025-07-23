# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=../lib
LEGOCAD_INCLUDE=../include
LEGOCAD_BIN=../bin
LEGOCAD=..
LEGOCAD_UID=../uid
LEGO_LIB=../lego_big/lib

# make_macros from project "legocad-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
#X_LIB=-lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
#C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
C_FLAGS=-g -DDEBUG -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#X_INCLUDE=. -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib -I$(HHHOME)/include -DOSF1
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
#F_LIB=-L/usr/lib/ -lg2c -lm
#F_LIB=-lg2c -lm
F_LIB=-lm
MOTIF_VER=11
HHHOME=$(TOOLS)/hyperhelp
HYPERHELP_OBJ = $(HHHOME)/winhelp/decaosf/winhelp.o
UXCGEN=run_uxcgen.sh $@ $<
#------------------------ C preprocessor
CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero -std=legacy
#F_FLAGS=-fno-underscoring -g -fno-automatic -finit-local-zero -std=legacy
#
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              56
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Fri May 14 11:32:21 2004 %

#
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE)
CFLAGS = $(X_FLAGS) $(CFLAGSINCL)
FFLAGS = $(F_FLAGS)
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) $(STUB_LIBS)
LIBUTIL = $(LEGOROOT_LIB)/libutilx.a $(LEGO_LIB)/vapo.a

SORGENTI   = tables.c crea_widget.c calcola.f
OGGETTI      = tables.o crea_widget.o calcola.o
SORGENTI_UIL =
USRINCLUDE   = tables.h def_tav.h

all: $(LEGOCAD_BIN)/tables


#versione per i sistemi operativi AIX ULTRIX OSF1
#$(LEGOCAD_BIN)/tables: $(OGGETTI) $(LIBUTIL)
#	gfortran -o $(LEGOCAD_BIN)/tables \
#       $(OGGETTI) \
#	$(LIBUTIL) -lm $(LIBOTS) $(F_LIB)  -lX11 $(LIBSVIL) /usr/lib/libg2c.so.0
#	$(LIBSVIL) $(LIBUTIL) -lm $(LIBOTS) $(F_LIB)  -lX11

$(LEGOCAD_BIN)/tables: $(OGGETTI) $(LIBUTIL)
	$(FC) -o ./tables \
       $(OGGETTI) \
	$(LIBUTIL) -lm $(LIBOTS) $(F_LIB)  -lX11 $(LIBSVIL)
#	$(LIBUTIL) -lm $(LIBOTS) $(F_LIB)  -lX11 $(LIBSVIL) /usr/lib/libg2c.so.0
#	$(LIBSVIL) $(LIBUTIL) -lm $(LIBOTS) $(F_LIB)  -lX11

