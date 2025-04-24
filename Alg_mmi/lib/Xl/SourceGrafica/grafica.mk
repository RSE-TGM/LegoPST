# ******* Telelogic expanded section *******

# make_macros from makefile "grafica.mk-5"
LEGOROOT_LIB=../../../AlgLib
LEGORT_LIB=../../../AlgLib
LEGOROOT_INCLUDE=../../../AlgLib/libinclude
LEGORT_INCLUDE=../../../AlgLib/libinclude
LEGOMMI_LIB=../..
LEGOMMI_INCLUDE=../../../include
TOOLS=../../../Tools

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
#       Makefile Header:               grafica.mk
#       Subsystem:              1
#       Description:
#       %created_by:    mauric %
#       %date_created:  Tue Jan 14 16:49:11 1997 %

CFLAGSINCL=-D$(OS) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) $(X_INCLUDE) -I$(LEGOMMI_LIB) -I$(LEGOROOT_LIB)
CFLAGS = $(X_FLAGS) $(C_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) 

SORGENTI = S_MAIN_WINDOW.c	funzioni.c\
	   defumis_dialog.c	dir_dialog.c\
	   fsca_dialog.c	grsfio.c\
	   memgr_dialog.c	nyi.c\
	   selgr_dialog.c	selmis_dialog.c\
	   selzoom_dialog.c	umis_dialog.c\
	   run_time_graf.c	S_MAIN_WINDOW_RUNTIME.c\
	   S_MAIN_WINDOW_PLOT.c UxXt.c
           
OGGETTI  = S_MAIN_WINDOW.o      funzioni.o\
           defumis_dialog.o     dir_dialog.o\
           fsca_dialog.o        grsfio.o\
           memgr_dialog.o       nyi.o\
           selgr_dialog.o       selmis_dialog.o\
           selzoom_dialog.o     umis_dialog.o\
	   run_time_graf.o	S_MAIN_WINDOW_RUNTIME.o\
	   S_MAIN_WINDOW_PLOT.o UxXt.o

all:  $(LEGOMMI_LIB)/libXl.a


$(LEGOMMI_LIB)/libXl.a: $(OGGETTI)
	ar rvls $(LEGOMMI_LIB)/libXl.a $?
	@echo libXl.a aggiornata
.c.a:;
