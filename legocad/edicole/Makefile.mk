# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
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
#UIL_COMPILER=/usr/bin/X11/uil
UIL_COMPILER=uil
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
#       Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#
DEPEND=$(LEGOROOT)/install/makedepend
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOCAD_INCLUDE) 
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) -D$(OS) -DMAIN
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) $(STUB_LIBS)
LIBUTIL = $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libutilx.a \
          $(LEGOCAD_LIB)/libicofile.a 
                                                            
SORGENTI = edicole.c
OGGETTI = edicole.o
all:  $(LEGOCAD_UID)/edicole.uid $(LEGOCAD_BIN)/edicole


$(LEGOCAD_BIN)/edicole: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGOCAD_BIN)/edicole  $(OGGETTI) \
	-lm $(LIBSVIL) $(LIBUTIL) -lX11

$(LEGOCAD_UID)/edicole.uid: edicole.uil$(MOTIF_VER)
	$(UIL) -o $(LEGOCAD_UID)/edicole.uid edicole.uil$(MOTIF_VER) \
        $(UIL_INCLUDE)

	
