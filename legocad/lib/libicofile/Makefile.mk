# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=..
LEGOCAD_INCLUDE=../../include

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
#       Makefile Header:               Makefile.mk
#       Subsystem:              63
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Sep 24 14:30:46 1996 %

#

DEPEND=$(LEGOROOT)/install/makedepend
UILINCLUDE = $(UIL_INCLUDE) 
CFLAGSINCL=-D$(OS) -I$(LEGOCAD_INCLUDE) -I$(LEGORT_INCLUDE) $(X_INCLUDE)
CFLAGS = $(C_FLAGS) $(X_FLAGS) $(CFLAGSINCL) $(VERSIONE)

SORGENTI = file_icone.c
           
OGGETTI =  file_icone.o

all:  $(LEGOCAD_LIB)/libicofile.a

makefile:Makefile
	make inc

$(LEGOCAD_LIB)/libicofile.a: $(OGGETTI)
#	$(CC) -c $(CFLAGS) $(?:.o=.c)  
	ar rvs $(LEGOCAD_LIB)/libicofile.a $?
	@echo libicofile.a aggiornata
.c.a:;


