# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGORT_BIN=../bin

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
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
#       Makefile Header:               Makefile.mk
#       Subsystem:              124
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Nov  7 12:15:28 2005 %
all: $(LEGORT_BIN)/cad_simula  $(LEGORT_BIN)/net_simula  $(LEGORT_BIN)/simula \
     $(LEGORT_BIN)/startup $(LEGORT_BIN)/net_startup $(LEGORT_BIN)/web_startup \
     $(LEGORT_BIN)/tsimula $(LEGORT_BIN)/simprova $(LEGORT_BIN)/snaf14 \
     $(LEGORT_BIN)/archive $(LEGORT_BIN)/alg_respawn $(LEGORT_BIN)/net_banco

$(LEGORT_BIN)/cad_simula: cad_simula.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/net_simula: net_simula.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/simula: simula.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/startup: startup.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/net_startup: net_startup.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/web_startup: web_startup.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/net_banco: net_banco.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/tsimula: tsimula.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/simprova: simprova.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/snaf14: snaf14.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/archive: archive.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/alg_respawn: alg_respawn.sh
	cp $? $@
	chmod 755 $@

