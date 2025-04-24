# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile-1"
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
#       Subsystem:              
#       Description:
#       %created_by:    %
#       %date_created:   %

CFLAGS=  $(C_FLAGS) -D$(OS) -I$(LEGORT_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB)
all:  $(LEGORT_BIN)/sqlite_test 

$(LEGORT_BIN)/sqlite_test: sqlite_test.c
	$(CC) $(CFLAGS) sqlite_test.c  -I/usr/include -lsqlite3 -o $@ 

