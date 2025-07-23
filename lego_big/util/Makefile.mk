# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
LEGO_BIN=../bin
LEGO_LIB=../lib
LEGORT_INCLUDE=../../AlgLib/libinclude
LEGORT_LIB=../../AlgLib/lib

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c  -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
CPPFLAGS=-P -C -DLINUX -traditional
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %
FFLAGS= $(F_FLAGS) 
CLIBS= $(LEGORT_LIB)/libnet.a
CLIBINC= -I$(LEGORT_INCLUDE)
CFLAGS= $(C_FLAGS) $(CLIBINC)
all: $(LEGO_BIN)/printgraf_exe $(LEGO_BIN)/aggforaus_exe $(LEGO_BIN)/traduci_f22 $(LEGO_BIN)/fbin2asc $(LEGO_BIN)/fasc2bin
#all: $(LEGO_BIN)/printgraf_exe $(LEGO_BIN)/aggforaus_exe $(LEGO_BIN)/traduci_f22 $(LEGO_BIN)/convf22  $(LEGO_BIN)/fbin2asc $(LEGO_BIN)/fasc2bin

$(LEGO_BIN)/printgraf_exe: printgraf.f
	$(FC) $(FFLAGS)  $? -o $@
#
$(LEGO_BIN)/aggforaus_exe: aggforaus.f
	$(FC) $(FFLAGS)  $? -o $@
#
$(LEGO_BIN)/traduci_f22: traduci_f22.o iof22bin.o
	$(FC)  $(FFLAGS) traduci_f22.o iof22bin.o -lc -o $@
#
$(LEGO_BIN)/convf22: convf22.c 
	echo $(CFLAGS)
	$(CC) $? $(CFLAGS) $(CLIBS) -o $@
#
$(LEGO_BIN)/fbin2asc: fbin2asc.o iof22bin.o iof22asc.o
	$(CC) $(CFLAGS) fbin2asc.o iof22bin.o iof22asc.o -o $@
#
$(LEGO_BIN)/fasc2bin: fasc2bin.o iof22bin.o iof22asc.o
	$(CC) $(CFLAGS) fasc2bin.o iof22bin.o iof22asc.o -o $@

              
