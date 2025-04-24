# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
LEGOROOT_LIB=../../../AlgLib
LEGORT_LIB=../../../AlgLib
LEGOROOT_INCLUDE=../../../AlgLib/libinclude
LEGORT_INCLUDE=../../../AlgLib/libinclude
LEGOMMI_LIB=../..
LEGOMMI_INCLUDE=../../../include

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-fcommon
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#   modulo Makefile
#   tipo
#   release 1.5
#   data 96/03/18
#   reserved @(#)Makefile       1.5
#
all:
#
#       librerie standard
#
	-cd ./Xl/SourceGrafica; $(MAKE) -f Makefile.mk
	-cd ./Xl; $(MAKE) -f Makefile.mk
	-cd ./Ol; $(MAKE) -f Makefile.mk
	-cd ./Xd; $(MAKE) -f Makefile.mk
	-cd ./Cs; $(MAKE) -f Makefile.mk

