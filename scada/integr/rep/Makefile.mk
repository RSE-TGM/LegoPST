# ******* Telelogic expanded section *******

# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
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
F_FLAGS=-fno-second-underscore
#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

#*************************************************
#*     Makefile generato con le dipendenze       *
#*     a partire dal file Makefile.tpl           *
CLAGS = -I.
LIBUTIL = 

SORGENTI = report.c
OGGETTI  = report.o

all:  $(SCADA_BIN)/report

makefile:Makefile
	make inc

$(SCADA_BIN)/report: $(OGGETTI) $(LIBUTIL)
	cc -o $(SCADA_BIN)/report  $(OGGETTI) \
        $(LIBUTIL) -lm

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
	cc -M $(CFLAGS) $(SORGENTI) >> Makefile
	echo "#     END   DEPEND" >> Makefile
                                                           

#
# Parte dedicata alla gestione con SCCS
#
INCLUDE = 
FFILES =
CFILES = $(SORGENTI) $(INCLUDE)
UILFILES =
COMFILES =
ALLFILES = $(CFILES) $(UILFILES) $(FFILES) Makefile.tpl
GET=sccs get
REL=

what:
	sccs what $(ALLFILES)

get: $(ALLFILES)
$(ALLFILES):
	sccs get $(REL) $(ALLFILES)

edit:
	sccs get -e $(ALLFILES)

delta:
	sccs delta $(REL) $(ALLFILES)

force:
	sccs clean
	sccs get $(REL) Makefile
	make all REL=$(REL)

newversion:
	sccs check
	sccs clean
	$(GET) -e $(REL) $(ALLFILES)
	sccs delta $(ALLFILES)

#     START DEPEND
report.o:	report.c
report.o:	/usr/include/stdio.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/sys/seek.h
report.o:	/usr/include/va_list.h
report.o:	/usr/include/sys/types.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/mach/machine/vm_types.h
report.o:	/usr/include/sys/select.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/getopt.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/sys/limits.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/machine/machlimits.h
report.o:	/usr/include/sys/syslimits.h
report.o:	/usr/include/machine/machtime.h
report.o:	/usr/include/sys/rt_limits.h
report.o:	/usr/include/string.h
report.o:	/usr/include/standards.h
report.o:	/usr/include/sys/types.h
#     END   DEPEND
