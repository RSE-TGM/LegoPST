# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
SCADA_BIN=../bin

# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-L/usr/users/legor2/xprinter/lib.dec3000
UXCGEN=run_uxcgen12.sh $@ $<
SCADA_C_FLAGS=-DWINMMI -DVIRTLBG -DGERARC -DENEL_SIM -DALARM_SIGNAL -DXSCADA_INTERFACE
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIBS=$(X_LIB)
SQLITE_LIB=-I$(LEGOROOT_LIB)/sqlite_include -L$(LEGOROOT_LIB)/sqlite_lib
#------------------------ C preprocessor
#CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              180
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Sep  8 13:35:19 1997 %

#
#	Poiche' starter e' un eseguibile che deve essere modificato
#	da root della macchina da cui viene lanciato lo scada, viene
#	messo sotto configurazione un template e in locale (non configurato)
#	viene inserito l' eseguibile starter.
CFLAGS = -DSCADA_MMI -D$(OS) $(C_FLAGS) $(VERSIONE) $(SCADA_C_FLAGS) $(SCADA_OPT)
LIBUTIL = 

SORGENTI = starter.c
OGGETTI  = starter.o


all:  $(SCADA_BIN)/starter_tpl


$(SCADA_BIN)/starter_tpl: $(OGGETTI) $(LIBUTIL)
	cc -o $(SCADA_BIN)/starter_tpl $(OGGETTI) \
        $(LIBUTIL) -lm

