# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3.1.1"

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -fpermissive -D_BSD -DLINUX -D_NO_PROTO -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c -D_NO_PROTO -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
CPPFLAGS=-P -C -DLINUX -traditional
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#       Makefile Header:               Makefile.mk
#       Subsystem:              123
#       Description:
#       %created_by:    lopez %
#       %date_created:  Fri Oct 25 16:43:34 2002 %
all:
	cd ./procedure; $(MAKE) -f Makefile.mk
	cd ./tavole; $(MAKE) -f Makefile.mk
	cd ./sorglego/sub; $(MAKE) -f Makefile.mk
	cd ./sorglego/main; $(MAKE) -f Makefile.mk
	cd ./sorgrego; $(MAKE) -f Makefile.mk
	cd ./util; $(MAKE) -f Makefile.mk
