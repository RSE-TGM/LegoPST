# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-11"
LEGORT_LIB=../../AlgLib
LEGOROOT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../include
LEGORT_BIN=../../bin
LEGORT_UID=../../uid
PATHCADOBJ=./cad_monit_o
PATHNETOBJ=./net_monit_o

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I$(LEGORT)/net_simula/monitor -L$(LEGOROOT_LIB)/sqlite_lib  
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
#UIL_COMPILER=/usr/bin/X11/uil
UIL_COMPILER=/usr/bin/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              80
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Mar  7 19:56:56 2006 %

CFLAGSINCL = $(X_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) \
          -I$(LEGOROOT_LIB)
CFLAGSINCLUDE = $(X_FLAGS) $(CFLAGSINCL) $(VERSIONE) -D$(OS) $(C_FLAGS)
UIL = $(UIL_COMPILER)
LIBSVIL = $(X_LIB) $(SQLITE_LIB)
LIBUTIL = $(LEGORT_LIB)/libdispatcher.a $(LEGOROOT_LIB)/libutilx.a \
          $(LEGOROOT_LIB)/libRt.a \
          $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a \
          $(LEGORT_LIB)/libnet.a $(LEGOROOT_LIB)/libutil.a \
	  $(LEGORT_LIB)/libnet.a -lsqlite3

SORGENTI = monit.c monit_aggancia.c monit_aggiorna.c monit_stat.c \
           monit_variabili.c monit_snapshot.c monit_perturba.c monit_staz.c \
           monit_data.c monit_frem.c monit_malf.c monit_mffr.c \
           monit_var_malf.c monit_var_frem.c

CAD_OGGETTI = $(PATHCADOBJ)/monit.o  $(PATHCADOBJ)/monit_aggancia.o \
         $(PATHCADOBJ)/monit_aggiorna.o $(PATHCADOBJ)/monit_stat.o \
         $(PATHCADOBJ)/monit_variabili.o $(PATHCADOBJ)/monit_snapshot.o \
         $(PATHCADOBJ)/monit_perturba.o $(PATHCADOBJ)/monit_staz.o \
         $(PATHCADOBJ)/monit_data.o $(PATHCADOBJ)/monit_frem.o \
         $(PATHCADOBJ)/monit_malf.o $(PATHCADOBJ)/monit_mffr.o \
         $(PATHCADOBJ)/monit_var_malf.o $(PATHCADOBJ)/monit_var_frem.o

NET_OGGETTI = $(PATHNETOBJ)/monit.o  $(PATHNETOBJ)/monit_aggancia.o \
         $(PATHNETOBJ)/monit_aggiorna.o $(PATHNETOBJ)/monit_stat.o \
         $(PATHNETOBJ)/monit_variabili.o $(PATHNETOBJ)/monit_snapshot.o \
         $(PATHNETOBJ)/monit_perturba.o $(PATHNETOBJ)/monit_staz.o \
         $(PATHNETOBJ)/monit_data.o $(PATHNETOBJ)/monit_frem.o \
         $(PATHNETOBJ)/monit_malf.o $(PATHNETOBJ)/monit_mffr.o \
         $(PATHNETOBJ)/monit_var_malf.o $(PATHNETOBJ)/monit_var_frem.o

CFLAGS=$(CFLAGSINCLUDE) -DLEGOCAD
all:  util_monit_make $(LEGORT_BIN)/cad_monit $(LEGORT_UID)/net_monit.uid $(LEGORT_BIN)/net_monit

util_monit_make:
	cd ./util_monit ; $(MAKE) -f Makefile.mk

$(LEGORT_UID)/net_monit.uid: monit.uil
	$(UIL) $(UIL_INCLUDE) -o $(LEGORT_UID)/net_monit.uid monit.uil

$(LEGORT_BIN)/cad_monit: $(CAD_OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/cad_monit  $(LINKER_OPTIONS) $(CAD_OGGETTI) \
        $(LIBSVIL) $(LIBUTIL) -lX11 $(STUB_LIBS) $(C_LIB) $(OTHER_LIB)
$(LEGORT_BIN)/net_monit: $(NET_OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/net_monit $(LINKER_OPTIONS) $(NET_OGGETTI) \
        $(LIBSVIL) $(LIBUTIL) -lX11 $(STUB_LIBS) $(C_LIB) $(OTHER_LIB)
#$(CAD_OGGETTI):$(SORGENTI)
#	@echo $<  
#	cc -c $(CFLAGS) $< -o $@
$(PATHCADOBJ)/monit.o :monit.c
	cc $(CFLAGS) monit.c -o $@
$(PATHCADOBJ)/monit_aggancia.o :monit_aggancia.c
	cc $(CFLAGS) monit_aggancia.c -o $@
$(PATHCADOBJ)/monit_aggiorna.o :monit_aggiorna.c
	cc $(CFLAGS) monit_aggiorna.c -o $@
$(PATHCADOBJ)/monit_stat.o :monit_stat.c
	cc $(CFLAGS) monit_stat.c -o $@
$(PATHCADOBJ)/monit_variabili.o :monit_variabili.c
	cc $(CFLAGS) monit_variabili.c -o $@
$(PATHCADOBJ)/monit_snapshot.o :monit_snapshot.c
	cc $(CFLAGS) monit_snapshot.c -o $@
$(PATHCADOBJ)/monit_perturba.o :monit_perturba.c
	cc $(CFLAGS) monit_perturba.c -o $@
$(PATHCADOBJ)/monit_staz.o :monit_staz.c
	cc $(CFLAGS) monit_staz.c -o $@
$(PATHCADOBJ)/monit_data.o :monit_data.c
	cc $(CFLAGS) monit_data.c -o $@
$(PATHCADOBJ)/monit_frem.o :monit_frem.c
	cc $(CFLAGS) monit_frem.c -o $@
$(PATHCADOBJ)/monit_malf.o :monit_malf.c
	cc $(CFLAGS) monit_malf.c -o $@
$(PATHCADOBJ)/monit_mffr.o :monit_mffr.c
	cc $(CFLAGS) monit_mffr.c -o $@
$(PATHCADOBJ)/monit_var_malf.o :monit_var_malf.c
	cc $(CFLAGS) monit_var_malf.c -o $@
$(PATHCADOBJ)/monit_var_frem.o :monit_var_frem.c
	cc $(CFLAGS) monit_var_frem.c -o $@

$(PATHNETOBJ)/monit.o :monit.c
	cc $(CFLAGSINCLUDE) monit.c -o $@
$(PATHNETOBJ)/monit_aggancia.o :monit_aggancia.c
	cc $(CFLAGSINCLUDE) monit_aggancia.c -o $@
$(PATHNETOBJ)/monit_aggiorna.o :monit_aggiorna.c
	cc $(CFLAGSINCLUDE) monit_aggiorna.c -o $@
$(PATHNETOBJ)/monit_stat.o :monit_stat.c
	cc $(CFLAGSINCLUDE) monit_stat.c -o $@
$(PATHNETOBJ)/monit_variabili.o :monit_variabili.c
	cc $(CFLAGSINCLUDE) monit_variabili.c -o $@
$(PATHNETOBJ)/monit_snapshot.o :monit_snapshot.c
	cc $(CFLAGSINCLUDE) monit_snapshot.c -o $@
$(PATHNETOBJ)/monit_perturba.o :monit_perturba.c
	cc $(CFLAGSINCLUDE) monit_perturba.c -o $@
$(PATHNETOBJ)/monit_staz.o :monit_staz.c
	cc $(CFLAGSINCLUDE) monit_staz.c -o $@
$(PATHNETOBJ)/monit_data.o :monit_data.c
	cc $(CFLAGSINCLUDE) monit_data.c -o $@
$(PATHNETOBJ)/monit_frem.o :monit_frem.c
	cc $(CFLAGSINCLUDE) monit_frem.c -o $@
$(PATHNETOBJ)/monit_malf.o :monit_malf.c
	cc $(CFLAGSINCLUDE) monit_malf.c -o $@
$(PATHNETOBJ)/monit_mffr.o :monit_mffr.c
	cc $(CFLAGSINCLUDE) monit_mffr.c -o $@
$(PATHNETOBJ)/monit_var_malf.o :monit_var_malf.c
	cc $(CFLAGSINCLUDE) monit_var_malf.c -o $@
$(PATHNETOBJ)/monit_var_frem.o :monit_var_frem.c
	cc $(CFLAGSINCLUDE) monit_var_frem.c -o $@

