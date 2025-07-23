# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7"

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
F_LIB= -lm
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
#       Makefile Header:               Makefile.mk
#       Subsystem:              73
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Dec 19 12:02:33 2002 %

all:
#
#  con il comando:   $(MAKE)
#
#  vengono compilate tutte le librerie e eseguibili legocad
#
#       for example
#
#	cd ./lib; $(MAKE) 
#
#
	@echo	"Compilation in progress. Wait please...";
	cd ./lib/libicofile; $(MAKE) -f Makefile.mk
	cd ./lib/liblegocad; $(MAKE) -f Makefile.mk
#	cd ./lib/macro; $(MAKE) -f Makefile.mk
	cd ./legoserver/creamodi; $(MAKE) -f Makefile.mk
#	cd ./autodoc; $(MAKE) -f Makefile.mk
#	cd ./bitmap; $(MAKE) -f Makefile.mk
#	cd ./calcstaz; $(MAKE) -f Makefile.mk
#	cd ./dati_legocad; $(MAKE) -f Makefile.mk
#	cd ./edicole; $(MAKE) -f Makefile.mk
#	cd ./editf01; $(MAKE) -f Makefile.mk
# 	cd ./filtri; $(MAKE) -f Makefile.mk
# 	cd ./iconvert; $(MAKE) -f Makefile.mk
# 	cd ./info; $(MAKE) -f Makefile.mk
# 	cd ./librarian; $(MAKE) -f Makefile.mk
# 	cd ./main; $(MAKE) -f Makefile.mk
# 	cd ./tavole; $(MAKE) -f Makefile.mk
# 	cd ./topologia; $(MAKE) -f Makefile.mk
# #	cd ./topsim; $(MAKE) -f Makefile.mk
# 	cd ./know; $(MAKE) -f Makefile.mk
# 	cd ./utility; $(MAKE) -f Makefile.mk
	@echo	"Compilation Successfull";
#---end of file ---
