# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-6"
SCADA_BIN=../bin
SCADA_INCLUDE=../incl
SCADA_LIB=../lib
SCADA_ATTI=./attiper
SCADA_CFG=../incl/ene/simul
SCADA_TCP=../tcp
SCADA_LIBUT=../libut
SCADA_LIBOSF1=../libOSF1
SCADA_ROOT=..
SCADA_SIMACQ=./simacq
SCADA_ACQUI=./acqui
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOROOT_LIB=../AlgLib


# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-L/usr/users/legor2/xprinter/lib.dec3000
UXCGEN=run_uxcgen12.sh $@ $<
SCADA_C_FLAGS=-DWINMMI -DVIRTLBG -DGERARC -DENEL_SIM -DALARM_SIGNAL -DXSCADA_INTERFACE
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIBS=$(X_LIB)
SQLITE_LIB=-I$(LEGOROOT_LIB)/sqlite_include -L$(LEGOROOT_LIB)/sqlite_lib
#------------------------ C preprocessor
#CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              181
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon May 17 17:48:23 2004 %

all:
	 cd ./libOSF1/Marte; $(MAKE) -f Makefile.mk
	 cd ./libOSF1/Net; $(MAKE) -f Makefile.mk
	 cd ./libOSF1/Xscada; $(MAKE) -f Makefile.mk
	 cd ./libOSF1/Util; $(MAKE) -f Makefile.mk
	 cd ./libOSF1/InterPol; $(MAKE) -f Makefile.mk
	 cd ./libut; $(MAKE) -f Makefile.mk
	 cd ./UtilPrint; $(MAKE) -f Makefile.mk
	 cd ./starter; $(MAKE) -f Makefile.mk
	 cd ./scada/alldbs; $(MAKE) -f Makefile.mk
	 cd ./scada/simacq; $(MAKE) -f Makefile.mk
	cd ./scada/attiper/calc; $(MAKE) -f Makefile.mk
	cd ./scada/gesrep; $(MAKE) -f Makefile.mk
	cd ./scada/mandbrt; $(MAKE) -f Makefile.mk
	cd ./scada/stampe/tabul; $(MAKE) -f Makefile.mk
	cd ./scada/stampe/virt; $(MAKE) -f Makefile.mk
	cd ./scada/arc; $(MAKE) -f Makefile.mk
	cd ./scada/gesrep; $(MAKE) -f Makefile.mk
	cd ./scada/isa; $(MAKE) -f Makefile.mk
	cd ./scada/acqui; $(MAKE) -f Makefile.mk
	cd ./scada; $(MAKE) -f Makefile.mk
	cd ./integr; $(MAKE) -f Makefile.mk
	cd ./comppol; $(MAKE) -f Makefile.mk


