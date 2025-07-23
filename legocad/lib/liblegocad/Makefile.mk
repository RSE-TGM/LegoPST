# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=..
LEGOCAD_INCLUDE=../../include
RT_INCLUDE=../../AlgLib

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
#       Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#

UILINCLUDE = $(UIL_INCLUDE) 
CFLAGSINCL= -I$(LEGOROOT_INCLUDE) -I$(LEGOCAD_INCLUDE) $(X_INCLUDE) -I$(UX_DIR)/include -I$(RT_INCLUDE)
CFLAGS = $(C_FLAGS) $(CFLAGSINCL) 

SORGENTI = ipcom.c f03.c f14.c liste.c pixmap_cursor.c\
           npipe.c f11.c read_ftn.c lc_varie.c lc_comandi.c \
           lc_util.c lc_errore.c lc_UxXt.c
           
OGGETTI = ipcom.o f03.o f14.o liste.o pixmap_cursor.o\
           npipe.o f11.o read_ftn.o lc_varie.o lc_comandi.o \
           lc_util.o lc_errore.o lc_UxXt.o

all: $(LEGOCAD_LIB)/liblegocad.a

makefile:Makefile
	make inc

$(LEGOCAD_LIB)/liblegocad.a: $(OGGETTI)
#	$(CC) -c $(CFLAGS) $(?:.o=.c)  
	ar rvs $(LEGOCAD_LIB)/liblegocad.a $?
	@echo liblegocad.a aggiornata
.c.a:;
