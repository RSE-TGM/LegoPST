# ******* Telelogic expanded section *******

# make_macros from makefile "mkMacroDati.mk-2.2.1"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=..
LEGOCAD_INCLUDE=../../include
LEGOCAD_BIN=../../bin
LEGOCAD=../..



# make_macros from project "legocad-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
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
F_LIB=-lgfortran -lm
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
#
#       Makefile Header:               mkMacroDati.mk
#       Subsystem:              1
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Nov  7 11:49:45 2002 %

#Make per compilaz. macro Dati

CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOCAD_INCLUDE)\
-I$(UX_DIR)/include
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) -D$(OS) -DDATI
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB)
LIBUTIL = $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libutilx.a \
$(LEGOCAD_LIB)/libicofile.a

ODIR=./macro_dati_obj

SORGENTI = block_wdg.c draw.c files.c macro.c macro_wdg.c symb_wdg.c popup.c
OGGETTI = $(ODIR)/block_wdg.o $(ODIR)/draw.o $(ODIR)/files.o $(ODIR)/macro.o \
          $(ODIR)/macro_wdg.o $(ODIR)/symb_wdg.o $(ODIR)/popup.o

all: $(LEGOCAD_LIB)/macro_dati.a
$(LEGOCAD_LIB)/macro_dati.a: $(OGGETTI)
	ar rvs $(LEGOCAD_LIB)/macro_dati.a $?
	@echo fine  creazione libreria macro_dati
.c.a:;

$(ODIR)/block_wdg.o: block_wdg.c
	$(CC) $(CFLAGS) -c block_wdg.c -o $(ODIR)/block_wdg.o

$(ODIR)/draw.o: draw.c
	$(CC) $(CFLAGS) -c draw.c -o $(ODIR)/draw.o

$(ODIR)/files.o: files.c
	$(CC) $(CFLAGS) -c files.c -o $(ODIR)/files.o

$(ODIR)/macro.o: macro.c
	$(CC) $(CFLAGS) -c macro.c -o $(ODIR)/macro.o

$(ODIR)/macro_wdg.o: macro_wdg.c
	$(CC) $(CFLAGS) -c macro_wdg.c -o $(ODIR)/macro_wdg.o

$(ODIR)/symb_wdg.o: symb_wdg.c
	$(CC) $(CFLAGS) -c symb_wdg.c -o $(ODIR)/symb_wdg.o

$(ODIR)/popup.o: popup.c
	$(CC) $(CFLAGS) -c popup.c -o $(ODIR)/popup.o

