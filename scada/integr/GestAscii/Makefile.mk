# ******* Telelogic expanded section *******

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
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#*************************************************
#*     Makefile generato con le dipendenze       *
#*     a partire dal file Makefile.tpl           *

SHELL=/bin/sh

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
INFORMIXBIN = $$INFORMIXDIR/bin

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
SRCS = outasc.ec 

# Object Files
OBJS = outasc.o 

# libreries
OLIB = $(SCADA_INTEGR_LIB)/libInformix.a $(SCADA_INTEGR_LIB)/libdb.a \
	$(SCADA_LIB)/libUtil.a

# Executable files
EPROG = $(SCADA_BIN)/GestAscii 


# Dependencies for the compiled version of the program.
all: $(EPROG) 

#
# This label is intended for test.
# You must define a main in source first.
#
$(EPROG): $(OBJS) $(OLIB)
	@echo "Linking $(EPROG)..."
	@$(ECCOMP)  $(LDFLAGS) -I$(IFXINCL) $(INCLUDE) \
		$(OBJS) -o $(EPROG) $(OLIB) $(INCLUDE)
	@$(ECCOMP)  -I$(IFXINCL) $(OBJS)  -o $(EPROG) $(OLIB)


dep: 
	make -f Makefile.tpl inc
	make

inc:
	echo "#*************************************************" > Makefile
	echo "#*     Makefile generato con le dipendenze       *" >> Makefile
	echo "#*     a partire dal file Makefile.tpl           *" >> Makefile
	echo "#*************************************************" >> Makefile
	cat Makefile.tpl >> Makefile
	echo "#     START DEPEND" >> Makefile
	cc -M $(CCFLAGS) $(SRCS) >> Makefile
	echo "#     END   DEPEND" >> Makefile


#     START DEPEND
outasc.o:	outasc.ec
outasc.o:	/usr/include/stdio.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/sys/seek.h
outasc.o:	/usr/include/va_list.h
outasc.o:	/usr/include/sys/types.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/mach/machine/vm_types.h
outasc.o:	/usr/include/sys/select.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/getopt.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/sys/limits.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/machine/machlimits.h
outasc.o:	/usr/include/sys/syslimits.h
outasc.o:	/usr/include/machine/machtime.h
outasc.o:	/usr/include/sys/rt_limits.h
outasc.o:	/usr/include/string.h
outasc.o:	/usr/include/standards.h
outasc.o:	/usr/include/sys/types.h
#     END   DEPEND
