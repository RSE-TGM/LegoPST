# ******* Telelogic expanded section *******

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              44
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Jun  8 17:06:11 2006 %

#   modulo Makefile
#   tipo 
#   release 1.1
#   data 94/11/24
#   reserved @(#)Makefile	1.1
#
all:
#
#       librerie standard
#
#N.B. compilazione di .resource viene fatta dal dir. config
	cd ./lib; $(MAKE) -f Makefile.mk
#
#       eseguibili
#
	cd ./client_server; $(MAKE) -f Makefile.mk 
#	cd ./client_scada; $(MAKE) -f Makefile.mk
	cd ./resource; $(MAKE) -f Makefile.mk
	cd ./config; $(MAKE) -f Makefile.mk
	cd ./draw; $(MAKE) -f Makefile.mk
	cd ./run_time; $(MAKE) -f Makefile.mk
	cd ./conv_legograf; $(MAKE) -f Makefile.mk
	cd ./conv_regograf; $(MAKE) -f Makefile.mk
	cd ./conv_staz; $(MAKE) -f Makefile.mk
	cd ./forms; $(MAKE) -f Makefile.mk
	cd ./regolazione; $(MAKE) -f Makefile.mk
	cd ./procedure; $(MAKE) -f Makefile.mk
	cd ./utilities; $(MAKE) -f Makefile.mk
