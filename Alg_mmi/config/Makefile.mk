# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10.1.1"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=-lbsd
TOOLS=../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-lsqlite3
#SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
#C_FLAGS=-g  -DLINUX -DLINUX9   -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
C_FLAGS=-g -fpermissive -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=--allow-multiple-definition
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              43
#       Description:
#       %created_by:    lopez %
#       %date_created:  Thu Oct 24 14:20:49 2002 %
#
APPL_INCL       = -I$(LEGOROOT_INCLUDE) -I$(LEGOMMI_INCLUDE) -I$(X_INCLUDE)\
                  -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB) -I$(LEGOROOT_LIB)\
                  -I$(UX_DIR)/include

X_CFLAGS        = -I/usr/include/ $(X_INCLUDE)

CFLAGS          = $(C_FLAGS) -DXLIB_ILLEGAL_ACCESS -DON_PAGEDIT $(VERSIONE) \
                  $(APPL_INCL) $(X_CFLAGS) $(MOTIF_CFLAGS)
                  
all:
	$(MAKE) -f config.mk
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
