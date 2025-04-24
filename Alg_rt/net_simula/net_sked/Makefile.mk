# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-11"
LEGORT_LIB=../../AlgLib
LEGOROOT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../include
LEGORT_BIN=../../bin
LEGORT_UID=../../uid
PATHCADOBJ=./cad_sked_o
PATHNETOBJ=./net_sked_o

# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
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
#	Makefile Header:               Makefile.mk
#       Subsystem:              77
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Jul 11 17:47:44 2006 %
CFLAGSINCLUDE = -c -D$(OS) $(VERSIONE) $(C_FLAGS)  -I$(LEGOROOT_LIB) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE)
LIBSVIL = -lsqlite3  $(SQLITE_LIB)  
LIBUTIL =  $(LEGORT_LIB)/libnet.a \
           $(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libRt.a $(LEGOROOT_LIB)/libutil.a \
           $(LEGORT_LIB)/libipc.a $(LEGOROOT_LIB)/libCAP.a $(LEGOROOT_LIB)/libsmartkey.a


SORGENTI = sked.c sked_prolog.c sked_timer.c sked_fine.c sked_start.c\
           sked_task.c sked_ingressi.c sked_perturba.c\
           sked_pacchetti.c sked_startup.c sked_pf22.c \
           sked_statistiche.c sked_errore.c sked_snapshot.c\
           sked_legograf.c sked_s02.c sked_aggiornamento.c \
           sked_stato.c sked_banco.c sked_slave.c sked_next.c \
           sked_backtrack.c sked_tempo_iniziale.c sked_replay.c \
           sked_snapshot_piac.c sked_backtrack_piac.c

CAD_OGGETTI =$(PATHCADOBJ)/sked.o $(PATHCADOBJ)/sked_prolog.o \
              $(PATHCADOBJ)/sked_timer.o $(PATHCADOBJ)/sked_fine.o \
              $(PATHCADOBJ)/sked_start.o \
              $(PATHCADOBJ)/sked_task.o $(PATHCADOBJ)/sked_ingressi.o \
              $(PATHCADOBJ)/sked_perturba.o\
              $(PATHCADOBJ)/sked_pacchetti.o $(PATHCADOBJ)/sked_startup.o \
              $(PATHCADOBJ)/sked_pf22.o \
              $(PATHCADOBJ)/sked_statistiche.o $(PATHCADOBJ)/sked_errore.o \
              $(PATHCADOBJ)/sked_snapshot.o\
              $(PATHCADOBJ)/sked_legograf.o $(PATHCADOBJ)/sked_s02.o \
              $(PATHCADOBJ)/sked_aggiornamento.o \
              $(PATHCADOBJ)/sked_stato.o $(PATHCADOBJ)/sked_banco.o \
              $(PATHCADOBJ)/sked_slave.o $(PATHCADOBJ)/sked_next.o \
              $(PATHCADOBJ)/sked_backtrack.o \
              $(PATHCADOBJ)/sked_tempo_iniziale.o \
              $(PATHCADOBJ)/sked_replay.o \
              $(PATHCADOBJ)/sked_snapshot_piac.o \
              $(PATHCADOBJ)/sked_backtrack_piac.o

NET_OGGETTI  = $(PATHNETOBJ)/sked.o $(PATHNETOBJ)/sked_prolog.o \
              $(PATHNETOBJ)/sked_timer.o $(PATHNETOBJ)/sked_fine.o \
              $(PATHNETOBJ)/sked_start.o \
              $(PATHNETOBJ)/sked_task.o $(PATHNETOBJ)/sked_ingressi.o \
              $(PATHNETOBJ)/sked_perturba.o\
              $(PATHNETOBJ)/sked_pacchetti.o $(PATHNETOBJ)/sked_startup.o \
              $(PATHNETOBJ)/sked_pf22.o \
              $(PATHNETOBJ)/sked_statistiche.o $(PATHNETOBJ)/sked_errore.o \
              $(PATHNETOBJ)/sked_snapshot.o\
              $(PATHNETOBJ)/sked_legograf.o $(PATHNETOBJ)/sked_s02.o \
              $(PATHNETOBJ)/sked_aggiornamento.o \
              $(PATHNETOBJ)/sked_stato.o $(PATHNETOBJ)/sked_banco.o \
              $(PATHNETOBJ)/sked_slave.o $(PATHNETOBJ)/sked_next.o \
              $(PATHNETOBJ)/sked_backtrack.o \
              $(PATHNETOBJ)/sked_tempo_iniziale.o \
              $(PATHNETOBJ)/sked_replay.o \
              $(PATHNETOBJ)/sked_snapshot_piac.o \
              $(PATHNETOBJ)/sked_backtrack_piac.o

CFLAGS= $(CFLAGSINCLUDE) -DLEGOCAD -DSAVEPERT

all:  $(LEGORT_BIN)/cad_sked $(LEGORT_BIN)/net_sked

$(LEGORT_BIN)/cad_sked: $(CAD_OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/cad_sked $(LINKER_OPTIONS) $(CAD_OGGETTI) \
        $(LIBUTIL) $(LIBSVIL) $(C_LIB) $(OTHER_LIB) $(F_LIB)
$(LEGORT_BIN)/net_sked: $(NET_OGGETTI) $(LIBUTIL)
	cc -o $(LEGORT_BIN)/net_sked $(LINKER_OPTIONS) $(NET_OGGETTI) \
        $(LIBUTIL) $(LIBSVIL) $(C_LIB) $(OTHER_LIB) $(F_LIB)

$(PATHCADOBJ)/sked.o: sked.c
	cc $(CFLAGS) sked.c -o $@
$(PATHCADOBJ)/sked_prolog.o: sked_prolog.c
	cc $(CFLAGS) sked_prolog.c  -o $@
$(PATHCADOBJ)/sked_timer.o: sked_timer.c
	cc $(CFLAGS) sked_timer.c -o $@
$(PATHCADOBJ)/sked_fine.o: sked_fine.c
	cc $(CFLAGS) sked_fine.c -o $@
$(PATHCADOBJ)/sked_start.o: sked_start.c
	cc $(CFLAGS) sked_start.c -o $@
$(PATHCADOBJ)/sked_task.o: sked_task.c
	cc $(CFLAGS) sked_task.c -o $@
$(PATHCADOBJ)/sked_ingressi.o: sked_ingressi.c
	cc $(CFLAGS) sked_ingressi.c -o $@
$(PATHCADOBJ)/sked_perturba.o: sked_perturba.c
	cc $(CFLAGS) sked_perturba.c -o $@
$(PATHCADOBJ)/sked_pacchetti.o: sked_pacchetti.c
	cc $(CFLAGS) sked_pacchetti.c -o $@
$(PATHCADOBJ)/sked_startup.o: sked_startup.c
	cc $(CFLAGS) sked_startup.c -o $@
$(PATHCADOBJ)/sked_pf22.o: sked_pf22.c
	cc $(CFLAGS) sked_pf22.c -o $@
$(PATHCADOBJ)/sked_statistiche.o: sked_statistiche.c
	cc $(CFLAGS) sked_statistiche.c -o $@
$(PATHCADOBJ)/sked_errore.o: sked_errore.c
	cc $(CFLAGS) sked_errore.c -o $@
$(PATHCADOBJ)/sked_snapshot.o: sked_snapshot.c
	cc $(CFLAGS) sked_snapshot.c -o $@
$(PATHCADOBJ)/sked_legograf.o: sked_legograf.c
	cc $(CFLAGS) sked_legograf.c -o $@
$(PATHCADOBJ)/sked_s02.o: sked_s02.c
	cc $(CFLAGS) sked_s02.c -o $@
$(PATHCADOBJ)/sked_aggiornamento.o: sked_aggiornamento.c
	cc $(CFLAGS) sked_aggiornamento.c -o $@
$(PATHCADOBJ)/sked_stato.o: sked_stato.c
	cc $(CFLAGS) sked_stato.c -o $@
$(PATHCADOBJ)/sked_banco.o: sked_banco.c
	cc $(CFLAGS) sked_banco.c -o $@
$(PATHCADOBJ)/sked_slave.o: sked_slave.c
	cc $(CFLAGS) sked_slave.c -o $@
$(PATHCADOBJ)/sked_next.o: sked_next.c
	cc $(CFLAGS) sked_next.c -o $@
$(PATHCADOBJ)/sked_backtrack.o: sked_backtrack.c
	cc $(CFLAGS) sked_backtrack.c -o $@
$(PATHCADOBJ)/sked_tempo_iniziale.o: sked_tempo_iniziale.c
	cc $(CFLAGS) sked_tempo_iniziale.c -o $@
$(PATHCADOBJ)/sked_replay.o: sked_replay.c
	cc $(CFLAGS) sked_replay.c -o $@
$(PATHCADOBJ)/sked_snapshot_piac.o: sked_snapshot_piac.c
	cc $(CFLAGS) sked_snapshot_piac.c -o $@
$(PATHCADOBJ)/sked_backtrack_piac.o: sked_backtrack_piac.c
	cc $(CFLAGS) sked_backtrack_piac.c -o $@

$(PATHNETOBJ)/sked.o: sked.c
	cc $(CFLAGSINCLUDE) sked.c -o $@
$(PATHNETOBJ)/sked_prolog.o: sked_prolog.c
	cc $(CFLAGSINCLUDE) sked_prolog.c  -o $@
$(PATHNETOBJ)/sked_timer.o: sked_timer.c
	cc $(CFLAGSINCLUDE) sked_timer.c -o $@
$(PATHNETOBJ)/sked_fine.o: sked_fine.c
	cc $(CFLAGSINCLUDE) sked_fine.c -o $@
$(PATHNETOBJ)/sked_start.o: sked_start.c
	cc $(CFLAGSINCLUDE) sked_start.c -o $@
$(PATHNETOBJ)/sked_task.o: sked_task.c
	cc $(CFLAGSINCLUDE) sked_task.c -o $@
$(PATHNETOBJ)/sked_ingressi.o: sked_ingressi.c
	cc $(CFLAGSINCLUDE) sked_ingressi.c -o $@
$(PATHNETOBJ)/sked_perturba.o: sked_perturba.c
	cc $(CFLAGSINCLUDE) sked_perturba.c -o $@
$(PATHNETOBJ)/sked_pacchetti.o: sked_pacchetti.c
	cc $(CFLAGSINCLUDE) sked_pacchetti.c -o $@
$(PATHNETOBJ)/sked_startup.o: sked_startup.c
	cc $(CFLAGSINCLUDE) sked_startup.c -o $@
$(PATHNETOBJ)/sked_pf22.o: sked_pf22.c
	cc $(CFLAGSINCLUDE) sked_pf22.c -o $@
$(PATHNETOBJ)/sked_statistiche.o: sked_statistiche.c
	cc $(CFLAGSINCLUDE) sked_statistiche.c -o $@
$(PATHNETOBJ)/sked_errore.o: sked_errore.c
	cc $(CFLAGSINCLUDE) sked_errore.c -o $@
$(PATHNETOBJ)/sked_snapshot.o: sked_snapshot.c
	cc $(CFLAGSINCLUDE) sked_snapshot.c -o $@
$(PATHNETOBJ)/sked_legograf.o: sked_legograf.c
	cc $(CFLAGSINCLUDE) sked_legograf.c -o $@
$(PATHNETOBJ)/sked_s02.o: sked_s02.c
	cc $(CFLAGSINCLUDE) sked_s02.c -o $@
$(PATHNETOBJ)/sked_aggiornamento.o: sked_aggiornamento.c
	cc $(CFLAGSINCLUDE) sked_aggiornamento.c -o $@
$(PATHNETOBJ)/sked_stato.o: sked_stato.c
	cc $(CFLAGSINCLUDE) sked_stato.c -o $@
$(PATHNETOBJ)/sked_banco.o: sked_banco.c
	cc $(CFLAGSINCLUDE) sked_banco.c -o $@
$(PATHNETOBJ)/sked_slave.o: sked_slave.c
	cc $(CFLAGSINCLUDE) sked_slave.c -o $@
$(PATHNETOBJ)/sked_next.o: sked_next.c
	cc $(CFLAGSINCLUDE) sked_next.c -o $@
$(PATHNETOBJ)/sked_backtrack.o: sked_backtrack.c
	cc $(CFLAGSINCLUDE) sked_backtrack.c -o $@
$(PATHNETOBJ)/sked_tempo_iniziale.o: sked_tempo_iniziale.c
	cc $(CFLAGSINCLUDE) sked_tempo_iniziale.c -o $@
$(PATHNETOBJ)/sked_replay.o: sked_replay.c
	cc $(CFLAGSINCLUDE) sked_replay.c -o $@
$(PATHNETOBJ)/sked_snapshot_piac.o: sked_snapshot_piac.c
	cc $(CFLAGSINCLUDE) sked_snapshot_piac.c -o $@
$(PATHNETOBJ)/sked_backtrack_piac.o: sked_backtrack_piac.c
	cc $(CFLAGSINCLUDE) sked_backtrack_piac.c -o $@


