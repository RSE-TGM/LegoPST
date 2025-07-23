# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-6"
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
C_FLAGS=-g -fcommon -DDEBUG -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#X_INCLUDE=. -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib -I$(HHHOME)/include -DOSF1
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
UIL_INCLUDE=-I/usr/include/uil
#UIL_COMPILER=/usr/bin/X11/uil
UIL_COMPILER=/usr/bin/uil
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
#       Subsystem:              53
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Jan 15 11:44:49 2003 %
#
.SUFFIXES: .i .o .c .h
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOCAD_INCLUDE) 
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) -D$(OS) -DMAIN
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) -lbsd $(STUB_LIBS)
LIBUTIL = $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/libutilx.a \
          $(LEGOCAD_LIB)/libicofile.a $(LEGOCAD_LIB)/liblegocad.a 
                                                            
INTERFACES = fileSelectionBox.c
SORGENTI = calcstaz.c jacobian.c  UxXt.c
OGGETTI = calcstaz.o jacobian.o $(INTERFACES:.c=.o) UxXt.o
IFILES=$(INTERFACES:.c=.i)

all:  $(LEGOCAD_UID)/calcstaz.uid $(LEGOCAD_BIN)/calcstaz $(INTERFACES) 

#
#.i.c:
#	$(UXCGEN)
#

$(LEGOCAD_BIN)/calcstaz: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGOCAD_BIN)/calcstaz  $(OGGETTI) \
	-lm $(LIBSVIL) $(LIBUTIL) -lX11

$(LEGOCAD_UID)/calcstaz.uid: calcstaz.uil$(MOTIF_VER)
	$(UIL) -o $(LEGOCAD_UID)/calcstaz.uid calcstaz.uil$(MOTIF_VER) \
        $(UIL_INCLUDE)
remove_attr:
	echo "Controllare che i cgenerati siano senza platform"
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
.c.o:
	cc $(CFLAGS) $< -o $@


