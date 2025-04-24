# ******* Telelogic expanded section *******

# make_macros from makefile "mkMacroTopol.mk-3.1.1"
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
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
F_LIB=-lg2c -lm
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
F_FLAGS=-fno-second-underscore
#
#       Makefile Header:               mkMacroTopol.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lopez %
#       %date_created:  Fri Oct 25 14:42:24 2002 %

#Make per compilaz. macro Topologia

CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOCAD_INCLUDE)\
-I$(UX_DIR)/include -I$(LEGOCAD)/topologia
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) -D$(OS) -DTOPOLOGIA
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB)
LIBUTIL = $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libutilx.a \
$(LEGOCAD_LIB)/libicofile.a

ODIR=./macro_topol_obj

SORGENTI = block_wdg.c draw.c files.c macro.c macro_wdg.c symb_wdg.c popup.c
OGGETTI = $(ODIR)/block_wdg.o $(ODIR)/draw.o $(ODIR)/files.o $(ODIR)/macro.o \
          $(ODIR)/macro_wdg.o $(ODIR)/symb_wdg.o $(ODIR)/popup.o

all: $(LEGOCAD_LIB)/macro_topol.a
$(LEGOCAD_LIB)/macro_topol.a: $(OGGETTI)
	ar rvs $(LEGOCAD_LIB)/macro_topol.a $?
	@echo fine  creazione libreria macro_topol
.c.a:;

$(ODIR)/block_wdg.o: block_wdg.c
	$(CC) $(CFLAGS) -c block_wdg.c
	mv block_wdg.o $(ODIR)

$(ODIR)/draw.o: draw.c
	$(CC) $(CFLAGS) -c draw.c
	mv draw.o $(ODIR)

$(ODIR)/files.o: files.c
	$(CC) $(CFLAGS) -c files.c
	mv files.o $(ODIR)

$(ODIR)/macro.o: macro.c
	$(CC) $(CFLAGS) -c macro.c
	mv macro.o $(ODIR)

$(ODIR)/macro_wdg.o: macro_wdg.c
	$(CC) $(CFLAGS) -c macro_wdg.c
	mv macro_wdg.o $(ODIR)

$(ODIR)/symb_wdg.o: symb_wdg.c
	$(CC) $(CFLAGS) -c symb_wdg.c
	mv symb_wdg.o $(ODIR)

$(ODIR)/popup.o: popup.c
	$(CC) $(CFLAGS) -c popup.c
	mv popup.o $(ODIR)
