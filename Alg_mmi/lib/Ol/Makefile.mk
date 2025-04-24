# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
LEGOMMI_LIB=..
LEGOMMI_INCLUDE=../../include
TOOLS=../../Tools

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
#	Makefile Header:               Ol.mk
#       Subsystem:              2
#       Description:
#       %created_by:    famgr %
#       %date_created:  Tue Feb  3 15:38:22 1998 %

CFLAGSINCL=-D$(OS) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) $(X_INCLUDE) -I$(LEGOMMI_LIB) -I$(LEGOROOT_LIB)
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) $(C_FLAGS)

SORGENTI = OlDatabaseTopologia.c     OlCompiler.c  \
	OlPert.c         OlSetRes.c OlDatabasePunti.c OlDatabaseP_func.c \
	OlForm.c OlForm_regol.c OlConn.c OlTree.c

OGGETTI  = OlDatabaseTopologia.o  OlCompiler.o \
	OlPert.o         OlSetRes.o OlDatabasePunti.o OlDatabaseP_func.o \
	OlForm.o OlForm_regol.o OlConn.o OlTree.o

all:  $(LEGOMMI_LIB)/libOl.a

$(LEGOMMI_LIB)/libOl.a: $(OGGETTI)
	ar rvs $(LEGOMMI_LIB)/libOl.a $?
	@echo libOl.a aggiornata
.c.a:;

