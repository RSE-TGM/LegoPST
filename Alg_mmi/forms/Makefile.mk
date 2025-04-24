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
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              37
#       Description:
#       %created_by:    zanna %
#       %date_created:  Mon Jul  1 17:29:59 1996 %

CFLAGS = -I$(LEGOMMI_INCLUDE) $(C_FLAGS)
SORGENTI_FORMS = forme.c
OGGETTI_FORMS  = forme.o
SORGENTI_ALLFORMS = crea_all_regforms.c
OGGETTI_ALLFORMS  = crea_all_regforms.o
SORGENTI_RFORMS = read_forms.c
OGGETTI_RFORMS  = read_forms.o

all: $(LEGOMMI_BIN)/forme  $(LEGOMMI_BIN)/crea_regforms $(LEGOMMI_BIN)/read_forms
$(LEGOMMI_BIN)/forme: $(OGGETTI_FORMS) 
	cc $(CFLAGS) -o $(LEGOMMI_BIN)/forme $(OGGETTI_FORMS)

$(LEGOMMI_BIN)/crea_regforms: $(OGGETTI_ALLFORMS) 
	cc $(CFLAGS) -o $(LEGOMMI_BIN)/crea_regforms $(OGGETTI_ALLFORMS)

$(LEGOMMI_BIN)/read_forms: $(OGGETTI_RFORMS) 
	cc $(CFLAGS) -o $(LEGOMMI_BIN)/read_forms $(OGGETTI_RFORMS)
