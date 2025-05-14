# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c  -DSNAPSHOT
F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              125
#       Description:
#       %created_by:    carlo %
#       %date_created:  Wed Sep 14 15:42:01 2005 %

#
#
#  con il comando:
#                   make
#  si aggiorna l'eseguibile rispetto a modifiche nei sorgenti
#

all:
#
#	librerie
#
	#cd ./lib; $(MAKE) -f Makefile.mk 
#
#	simulatore
#
	cd ./net_simula; $(MAKE) -f Makefile.mk
#
#       grafica
#
	cd ./grafica; $(MAKE) -f Makefile.mk
#
#	snaf14
#
########	 cd ./snaf14; $(MAKE) -f Makefile.mk
#
#	procedure
#
	cd ./procedure; $(MAKE) -f Makefile.mk
#
#       sqlite_test
#
#### 	cd ./sqlite_test; $(MAKE) -f Makefile
#
#	util
#
	cd ./util; $(MAKE) -f Makefile.mk
#
#	algrt_db
#
	cd ./algrt_db; $(MAKE) -f Makefile.mk
