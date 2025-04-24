# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-12"

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              90
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Jun  8 18:05:53 2006 %
all:
#
#       simulatore
#
	cd ./net_monit; $(MAKE) -f Makefile.mk
	cd ./net_sked; $(MAKE) -f Makefile.mk
	cd ./net_compi; $(MAKE) -f Makefile.mk
	cd ./net_demone; $(MAKE) -f Makefile.mk
	cd ./net_updbase; $(MAKE) -f Makefile.mk
	cd ./net_net; $(MAKE) -f Makefile.mk
	cd ./net_prepf22_circ; $(MAKE) -f Makefile.mk
	cd ./malfunzioni; $(MAKE) -f Makefile.mk
	cd ./mandb; $(MAKE) -f Makefile.mk
	cd ./net_clean; $(MAKE) -f Makefile.mk
	cd ./net_disp; $(MAKE) -f Makefile.mk
	cd ./net_prova; $(MAKE) -f Makefile.mk
####	cd ./agg_scada; $(MAKE) -f Makefile.mk
	cd ./new_monit;$(MAKE) -f Makefile.mk
	cd ./newsess;$(MAKE) -f Makefile.mk
	cd ./rest_sess;$(MAKE) -f Makefile.mk
	cd ./net_tast; $(MAKE) -f Makefile.mk
	cd ./net_operator; $(MAKE) -f Makefile
	cd ./dataserver;  $(MAKE) -f Makefile.mk
	cd ./viewval;  $(MAKE) -f Makefile
