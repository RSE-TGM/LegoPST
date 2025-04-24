# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=..
LEGOCAD_INCLUDE=../include
LEGOCAD_BIN=../bin
LEGOCAD=..


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
#       Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#
DEPEND=$(LEGOROOT)/install/makedepend
CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) 
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) $(VERSIONE) -D$(OS)
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) $(STUB_LIBS)
LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libutilx.a\
	  $(LEGORT_LIB)/libnet.a\
          $(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libutil.a
                                                            
SORGENTI= bitmap.c dialog.c 
OGGETTI =bitmap.o dialog.o  

all: $(LEGOCAD_BIN)/bitmap 

makefile:Makefile
	make inc


$(LEGOCAD_BIN)/bitmap: $(OGGETTI) $(LIBUTIL)
	cc -o $@  $(OGGETTI)\
	-lm $(LIBSVIL) $(LIBUTIL) -lX11 

inc: 
	$(DEPEND) -s "# DO NOT DELETE" -- $(CFLAGSINCL) -- $(SORGENTI)
	make
