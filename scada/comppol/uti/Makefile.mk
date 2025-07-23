# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-4"
SCADA_INCLUDE=../../incl
LEGOROOT_LIB=../../AlgLib


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
#       Subsystem:              183
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue May 25 08:59:05 2004 %

CFLAGS = $(C_FLAGS) -I$(SCADA_INCLUDE) -D$(OS) $(SCADA_C_FLAGS) $(SCADA_OPT) \
	-I. -I..  

SORGENTI = intfile.c ins_symb.c sbianca.c scrivisu.c isreale.c isintero.c \
	iscostan.c ismenu.c isgruppo.c new_lico.c instable.c isstring.c \
	stampa_s.c strupr.c strcmpi.c

OGGETTI = intfile.o ins_symb.o sbianca.o scrivisu.o isreale.o isintero.o \
	iscostan.o ismenu.o isgruppo.o new_lico.o instable.o isstring.o \
	stampa_s.o strupr.o strcmpi.o

all:utility.a 

utility.a: $(OGGETTI)
	ar rvs utility.a $?
	@echo utility.a aggiornata
.c.a:;

