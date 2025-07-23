# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=/lib/libbsd.a


# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              38
#       Description:
#       %created_by:    famgr %
#       %date_created:  Fri Aug  2 17:28:52 1996 %

#   modulo Makefile
#   tipo 
#   release 1.3
#   data 96/03/19
#   reserved @(#)Makefile	1.3
#
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) 
LIBUTIL = 

SORGENTI = conv_l.c util.c
OGGETTI  = conv_l.o util.o

all:  $(LEGOMMI_BIN)/conv_l


$(LEGOMMI_BIN)/conv_l: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGOMMI_BIN)/conv_l $(OGGETTI) \
        $(LIBUTIL) -lm
