# ******* Telelogic expanded section *******

# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fpermissive -fcommon -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 1.8
#   data 96/03/19
#   reserved @(#)Makefile	1.8
#
all:
	cd ./libutilx; $(MAKE) -f Makefile.mk
	cd ./libutil;$(MAKE) -f Makefile.mk
	cd ./Rt; $(MAKE) -f Makefile.mk
	cd ./libcom; $(MAKE) -f Makefile.mk
	cd ./libsim; $(MAKE) -f Makefile.mk
#	cd ./libsims; $(MAKE) -f sims.mk
	cd ./libipc; $(MAKE) -f Makefile.mk
	cd ./libnet; $(MAKE) -f Makefile.mk
	cd ./libwidget; $(MAKE) -f Makefile.mk
	cd ./libdispatcher; $(MAKE) -f Makefile.mk
	cd ./libmanovra; $(MAKE) -f Makefile.mk
	cd ./libcap; $(MAKE) -f Makefile.mk
	cd ./libsmartkey; $(MAKE) -f Makefile.mk

