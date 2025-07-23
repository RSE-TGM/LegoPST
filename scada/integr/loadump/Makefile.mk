# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-3"
SCADA_INCLUDE=../../incl
SCADA_STAMPE=../../scada/stampe
SCADA_SIMACQ=../../scada/simacq
SCADA_INTEGR_INCLUDE=../include
SCADA_LIBOSF1=../../libOSF1
SCADA_INTEGR_LIB=../lib
SCADA_BIN=../../bin
SCADA_LIB=../../lib
SCADA_LIBUT=../../libut


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
#       Subsystem:              174
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Dec 19 10:25:59 2002 %


SHELL=/bin/ksh

# include
INCLUDE= -I. -I$(SCADA_INTEGR_INCLUDE)

# define new rules
#
.SUFFIXES:
.SUFFIXES: .ec .c .o .ic .std .mak

# Rule for making a machine specific makefile from this makefile

.ec.o:
	@echo "ESQL Compiling:  $<"
	@echo "ESQL IFXINCL:  " $(IFXINCL)
	@$(ECCOMP) $(ECFLAGS) $(CCOPTS) $(INCLUDE) $*.ec

.c.o:
	@echo "CC Compiling:  $<"
	@$(CC) $(ECFLAGS)  -I$(IFXINCL) $(INCLUDE) 
	$*.c


# Informix bin
INFORMIXBIN = $(INFORMIXDIR)/bin

# Compilers
ECCOMP = $(INFORMIXBIN)/esql

# Commands

ECHO = echo
GET = get
LINK = ln
UNAME = `uname`
ERROR = ++++
SED = sed

# Flags
ECFLAGS = -c -g -DASCII_DB

CCFLAGS = -c -g -DASCII_DB -I$(SCADA_INTEGR_INCLUDE)

# Other platforms use make's default 

# Include files
IFXINCL = $(INFORMIXDIR)/incl 

# Define shorthand for forms used and for compiled objects and
# p-code objects.

# Source files
SRCS = loadump.ec exp_chk.ec 

# Object Files
OBJS = loadump.o exp_chk.o 

# libreries
OLIB = $(SCADA_INTEGR_LIB)/libInformix.a $(SCADA_LIB)/libUtil.a

# Executable files
EPROG = $(SCADA_BIN)/loadump 


# Dependencies for the compiled version of the program.
all: $(EPROG) $(SCADA_BIN)/blobvi

#
# This label is intended for test.
# You must define a main in source first.
#
$(EPROG): $(OBJS) $(OLIB)
	@echo "Linking $(EPROG)..."
	@$(ECCOMP)  $(LDFLAGS) -I$(IFXINCL) $(INCLUDE) \
		$(OBJS) -o $(EPROG) $(OLIB) $(INCLUDE)
	@$(ECCOMP)  -I$(IFXINCL) $(OBJS)  -o $(EPROG) $(OLIB)

$(SCADA_BIN)/blobvi:blobvi.sh
	cp blobvi.sh $(SCADA_BIN)/blobvi
	chmod 777 $(SCADA_BIN)/blobvi

