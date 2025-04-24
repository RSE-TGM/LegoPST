# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-11"
LEGORT_INCLUDE=../include
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
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
#       Subsystem:              190
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Feb 28 10:57:55 2007 %

CFLAGS=  $(C_FLAGS) -D$(OS) -I$(LEGORT_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB)
all:  $(LEGORT_BIN)/al_punt_mod $(LEGORT_BIN)/al_punt_mod_core $(LEGORT_BIN)/check_license


$(LEGORT_BIN)/al_punt_mod: al_punt_mod.sh
	cp al_punt_mod.sh $(LEGORT_BIN)/al_punt_mod
	chmod a+xw $(LEGORT_BIN)/al_punt_mod

$(LEGORT_BIN)/al_punt_mod_core: al_punt_mod.c  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a
	$(CC) $(CFLAGS) al_punt_mod.c -o $@ $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a 

$(LEGORT_BIN)/check_license: check_license.c  $(LEGOROOT_LIB)/libCAP.a $(LEGOROOT_LIB)/libsmartkey.a
	$(CC) $(CFLAGS) check_license.c -o $@ $(LEGOROOT_LIB)/libCAP.a \
	$(LEGOROOT_LIB)/libsmartkey.a
