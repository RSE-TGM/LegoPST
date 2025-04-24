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

SHELL=/bin/sh
LIBHOME=$(ORACLE_HOME)/lib
I_SYM= -I
CC=cc
CFLAGS=-I. -I$(SCADA_INTEGR_INCLUDE) -DASCII_DB -O -DENEL -g3 -I$(SCADA_INTEGR)/inst
LDFLAGS=-L$(LIBHOME)
LDFLAGS=-non_shared -L$(LIBHOME)  
ARLOCAL=l
AR=ar $(ARLOCAL)
ARCREATE=ar cr$(ARLOCAL)
ARDELETE=ar d$(ARLOCAL)
ARREPLACE=ar r$(ARLOCAL)
ECHO=$(ORACLE_HOME)/bin/echodo
OTHERLIBS= -lmld -lm `cat $(ORACLE_HOME)/rdbms/lib/sysliblist` 
LDLIBS= -lmld -lm
CLIBS=$(OTHERLIBS)
SO=a
NLSRTLHOME= $(SRCHOME)/nlsrtl23
NLSRTLLIB= -lnlsrtl
LIBNLSRTL= $(LIBHOME)/libnlsrtl.a
LLIBNLSRTL=-lnlsrtl
NLSRTLINC= $(I_SYM)$(NLSRTLHOME)/oracore/public
NLSRTLPUBLIC= $(I_SYM)$(NLSRTLHOME)/public \
 $(STDINCLUDE)
COREHOME= $(SRCHOME)/oracore
LIBOCIC= $(LIBHOME)/libocic.$(SO)
LIBCORE= $(LIBHOME)/libcore.$(SO)
LIBCV6= $(LIBHOME)/libcv6.$(SO)
LLIBOCIC= -locic
LLIBCORE= -lcore
LLIBCV6= -lcv6
CORELIBD= $(LIBNLSRTL) $(LIBCV6) $(LIBCORE) $(LIBOCIC)
CORELIBS= $(LLIBNLSRTL) $(LLIBCV6) $(LLIBOCIC) $(LLIBCORE) $(LLIBNLSRTL) $(LLIBCV6) \
          $(LLIBCORE)
LIBNETWORK= $(LIBHOME)/libnetwork.a
NTCONTAB= $(LIBHOME)/ntcontab.o
TNSLIBS= -lnetwork
TNSLIBD= $(LIBNETWORK)
LIBNETV2= $(LIBHOME)/libnetv2.a
NETV2LIBS= -lnetv2 $(TNSLIBS)
NETV2LIBD= $(LIBNETV2) $(TNSLIBD)
LIBNSG= $(LIBHOME)/libnsg.a
LLIBNSG= -lnsg
NSGLIBS= $(LLIBNSG)
NSGLIBD= $(LIBNSG)
NNETV2LIBS= -lnetv2 $(TTNSLIBS)
NNETV2LIBD= $(LIBNETV2) $(TTNSLIBD)
OSNTAB= $(LIBHOME)/osntab.o
OSNTABST= $(LIBHOME)/osntabst.o 
LIBSQLNET= $(LIBHOME)/libsqlnet.a
SQLNETLIBS= -lsqlnet
NETLIBS= $(OSNTAB) $(SQLNETLIBS) $(NETV2LIBS)
NETLIBD= $(OSNTAB) $(LIBSQLNET) $(NETV2LIBD)
LIBPLS= $(LIBHOME)/libpls.a
LLIBPLS= -lpls
LIBTK2C= $(LIBHOME)/libtk2c.a
LLIBTK2C= -ltk2c
LIBTK2M= $(LIBHOME)/libtk2m.a
LLIBTK2M= -ltk2m -ltk2p -ltk2m
LIBTK2P= $(LIBHOME)/libtk2p.a
LLIBTK2P= -ltk2p 
LIBTK2UC= $(LIBHOME)/libuc.a
LLIBTK2UC= -luc
LIBTK2REM= $(LIBHOME)/librem.a
LLIBTK2REM= -lrem
LIBTK2ROS= $(LIBHOME)/libros.a
LLIBTK2ROS= -lros
LIBTK2OT= $(LIBHOME)/libot.a
LLIBTK2OT= -lot
LIBTK2OTX= $(LIBHOME)/libotx.a
LLIBTK2OTX= -lotx
LIBTK2UT= $(LIBHOME)/libut.a
LLIBTK2UT= -lut
LIBTK2SL= $(LIBHOME)/libsl.a
LLIBTK2SL= -lsl
LIBTK2RE= $(LIBHOME)/libre.a
LLIBTK2RE= -lre
TK2UICLIBD= $(LIBTK2RE) $(LIBTK2UC) $(LIBTK2C) $(LIBTK2OT) $(LIBTK2REM) \
	$(LIBTK2ROS) $(LIBTK2P) $(LIBTK2UT) $(LIBTK2SL)
TK2UICLIBS= $(DYNAMIC_ON_TK2) \
	$(LLIBTK2SL) $(LLIBTK2RE) $(LLIBTK2UC) $(LLIBTK2C) $(LLIBTK2OT) \
	$(LLIBTK2RE) $(LLIBTK2REM) $(LLIBTK2ROS) $(LLIBTK2C) $(LLIBTK2P) \
	$(LLIBTK2C) $(LLIBTK2P) -lm $(LLIBTK2REM) $(LLIBTK2C) $(LLIBTK2OT) \
	$(LLIBTK2UT) $(LLIBTK2UC) $(LLIBTK2SL) $(LLIBTK2C) \
	$(DYNAMIC_OFF_TK2)
TK2UIMLIBD= $(LIBTK2OT) $(LIBTK2RE) $(LIBTK2UC) $(LIBTK2REM) $(LIBTK2ROS) \
	$(LIBTK2M) $(LIBTK2P) $(LIBTK2UT) $(LIBTK2SL)
TK2UIMLIBS= $(DYNAMIC_ON_TK2) \
	$(LLIBTK2OT) $(LLIBTK2RE) $(LLIBTK2UC) $(LLIBTK2REM) $(LLIBTK2ROS) \
	$(LLIBTK2M) $(LLIBTK2P) $(LLIBTK2M) $(LLIBTK2OT) $(LLIBTK2UT) \
	$(LLIBTK2SL) $(DYNAMIC_OFF_TK2)
AIXSTLIBOPT=
LIBOCIC= $(LIBHOME)/libocic.a
LLIBOCIC= -locic
TTLIBD= $(NETLIBD) $(LIBORA) $(CORELIBD)
TTLIBS= $(OSNTAB) $(SQLNETLIBS) $(LLIBORA) $(LIBPLSHACK) $(SQLNETLIBS) \
 $(LLIBORA) $(CORELIBS) $(LDLIBS) $(CLIBS)
STLIBDNOPLS= $(OSNTABST) $(CONFIG) $(CORELIBD) $(NETLIBD) $(RDBMSLIBDNOPLS)
STLIBD= $(OSNTABST) $(CONFIG) $(CORELIBD) $(NETLIBD) $(RDBMSLIBD)
STLIBSNOPLS= $(OSNTABST) $(CONFIG) $(RDBMSLIBSNOPLS) $(RDBMSLIBSNOPLS) \
 $(AIXSTLIBOPT) $(SQLNETLIBS) $(LLIBORA) $(CORELIBS) $(LLIBPSO) $(LDLIBS) \
 $(CLIBS)
STLIBS= $(OSNTABST) $(CONFIG) $(RDBMSLIBS) $(RDBMSLIBS) $(AIXSTLIBOPT) \
 $(SQLNETLIBS) $(LLIBORA) $(CORELIBS) $(LLIBPSO) $(LDLIBS) $(CLIBS)
LLIBNETV2= $(LIBNETV2)
LLIBNETWORK= $(LIBNETWORK)
CONFIG= $(LIBHOME)/config.o
OPIMAI= $(LIBHOME)/opimai.o
LIBORA= $(LIBHOME)/libora.a
LIBKNL= $(LIBHOME)/libknl.a
LIBKNLOPT= $(LIBHOME)/libknlopt.a
LLIBORA= -lora -losn -lora
LLIBKNL= -lknl
LLIBKNLOPT= -lknlopt
RDBMSLIBDNOPLS= $(LIBORA) $(LIBKNLOPT) $(LIBKNL)
RDBMSLIBD= $(LIBORA) $(LIBKNLOPT) $(LIBPLS) $(LIBKNL)
RDBMSLIBSNOPLS= $(LLIBORA) $(LLIBKNLOPT) $(LLIBKNL)
RDBMSLIBS= $(LLIBORA) $(LLIBKNLOPT) $(LLIBPLS) $(LLIBKNL)
LIBSOSD=
LLIBSOSD=
LIBPCORE=$(LIBHOME)/libpcore.a
LLIBPCORE= -lpcore
LIBCGEN= $(LIBHOME)/pdc.o
LIBFORGEN= $(LIBHOME)/pdf.o
LIBCOBGEN= $(LIBHOME)/pdb.o
LIBPASGEN= $(LIBHOME)/pdp.o
LIBADAGEN= $(LIBHOME)/pda.o
LIBOSDGEN= $(LIBHOME)/pds.o
CODEGENHOME= $(SRCHOME)/codegen
LIBSLAX=$(LIBHOME)/pxslax.o
SQLHOME= $(ORACLE_HOME)/sqllib
SQLPUBLIC=$(I_SYM)$(SQLHOME)/public
LIBSQL= $(LIBHOME)/libsql.a
SQLLIBS= -lsql
LIBPROC20=$(LIBHOME)/libproc.a
LLIBPROC20= -lproc
LIBPRO= $(LIBPROC20)
LLIBPRO= $(LLIBPROC20)
PROLDLIBS= $(SQLLIBS) $(TTLIBS)
PROC=$(ORACLE_HOME)/bin/proc
PROCOBJS=$(ORACLE_HOME)/proc/lib/main.o \
	$(ORACLE_HOME)/proc/lib/pcdsfv.o \
	$(ORACLE_HOME)/proc/lib/pcdlut.o
PROCPLSFLAGS= sqlcheck=full userid=$(USERID) dbms=v6_char
PROF_INCL1=$(SCADA_INTEGR)/inst
PROFLAGS= sqlcheck=full userid=$(USERID) dbms=v6_char include=$(SCADA_INTEGR_INCLUDE) include=$(PROF_INCL1) define=ASCII_DB
USERID= legosvil/legosvil
SORGENTI= outascii.c outascii_puntaa.c outascii_puntac.c \
	outascii_puntas.c outascii_puntao.c outascii_puntad.c \
	outascii_puntda.c outascii_puntdc.c outascii_puntds.c \
	outascii_puntdd.c outascii_puntdo.c outascii_puntarc.c \
	outascii_allardb.c outascii_allarline.c outascii_nomi.c \
	outascii_allarliv.c outascii_allarser.c outascii_allarstrat.c \
	outascii_configurazionedb.c outascii_puntst.c \
	outascii_puntto.c outascii_puntor.c outascii_mistabk.c \
	outascii_tdizst.c outascii_tabulati.c

OGGETTI= outascii.o outascii_puntaa.o outascii_puntac.o \
	outascii_puntas.o outascii_puntao.o outascii_puntad.o \
	outascii_puntda.o outascii_puntdc.o outascii_puntds.o \
	outascii_puntdd.o outascii_puntdo.o outascii_puntarc.o \
	outascii_allardb.o outascii_allarline.o outascii_nomi.o \
	outascii_allarliv.o outascii_allarser.o outascii_allarstrat.o \
	outascii_configurazionedb.o outascii_puntst.o \
        outascii_puntto.o outascii_puntor.o outascii_mistabk.o \
	outascii_tdizst.o outascii_tabulati.o

all: outascii 

# Rule to compile any program (specify EXE= and OBJS= on command line)
outascii: $(OGGETTI)
	$(CC) $(LDFLAGS) -o outascii $(OGGETTI) $(PROLDLIBS)


# Suffix rules
.SUFFIXES: .exe .o .c .pc

.pc.c:
	$(PROC) $(PROFLAGS) iname=$*.pc 

.pc.o:
	$(PROC) $(PROFLAGS) iname=$*.pc
	$(CC) $(CFLAGS) $(SQLPUBLIC) -c $*.c

.c.o:
	$(CC) $(CFLAGS) $(SQLPUBLIC) -c $*.c

LIBDIR= $(ORACLE_HOME)/proc/lib
DEMODIR= $(ORACLE_HOME)/proc/demo

install_files:
	-rm -f $(DEMODIR)/proc.mk
	-ln $(LIBDIR)/proc.mk $(DEMODIR)/proc.mk

# Rules to relink the proc executable
install: clean proc
	-chmod 755 $(ORACLE_HOME)/bin/proc
	-mv proc $(ORACLE_HOME)/bin/proc
	-chmod 755 $(ORACLE_HOME)/bin/proc

clean:
	-rm -f proc

proc:
	@$(ECHO) $(CC) $(LDFLAGS) -o proc $(PROCOBJS) \
	$(LLIBPROC20) $(LIBCGEN) $(LLIBPLS) $(LIBOSDGEN) $(LIBSLAX) \
	$(LLIBPCORE) $(LLIBPLS) $(LLIBORA) $(LLIBPLS) $(TTLIBS) -lm

