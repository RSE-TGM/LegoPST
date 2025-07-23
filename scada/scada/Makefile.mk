# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7"
SCADA_BIN=../bin
SCADA_INCLUDE=../incl
SCADA_LIB=../lib
SCADA_ATTI=./attiper
SCADA_CFG=../incl/ene/simul
SCADA_TCP=../tcp
SCADA_LIBUT=../libut
SCADA_LIBOSF1=../libOSF1
SCADA_ROOT=..
SCADA_SIMACQ=./simacq
SCADA_ACQUI=./acqui
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOROOT_LIB=../AlgLib

# make_macros from project "scada-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic12
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include  -I/usr/local/include -I/usr/include  -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -fcommon $(X_INCLUDE) -DLINUX -DUNIX -D_BSD   -DXOPEN_CATALOG -Dmmap=_mmap_32_ -DDEBUGYES -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-L/usr/users/legor2/xprinter/lib.dec3000
UXCGEN=run_uxcgen12.sh $@ $<
SCADA_C_FLAGS=-DWINMMI -DVIRTLBG -DGERARC -DENEL_SIM -DALARM_SIGNAL -DXSCADA_INTERFACE
#THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
#THREAD_LIB=-ldl ../AlgLib/dcethreads_lib/libdcethreads.so /usr/lib/x86_64-linux-gnu/libpthread.so
##THREAD_LIB=-ldl ../AlgLib/dcethreads_lib/libdcethreads.so  -lpthread
# Guag2024
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -lpthread -ldl
#THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -lpthread -ldl

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
#       Subsystem:              188
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Jul 11 19:04:41 2006 %

CFLAGS = -I$(SCADA_INCLUDE) -I$(SCADA_LIB) -I$(SCADA_ATTI) -I$(LEGORT_INCLUDE)\
	-D$(OS) $(C_FLAGS) -I$(SCADA_CFG) -I$(SCADA_TCP) $(SCADA_C_FLAGS) \
	$(SCADA_OPT)

LIB =  $(SCADA_LIB)/libUtil.a $(SCADA_LIBUT)/libpscs.a $(SCADA_LIB)/libmandb.a \
	$(SCADA_LIB)/libarchi.a $(SCADA_LIB)/libtabul.a\
	./libscada.a $(SCADA_LIBOSF1)/libMarte.a $(SCADA_LIBUT)/libpscs.a \
	$(SCADA_LIB)/libtabul.a $(SCADA_LIB)/libcalc.a $(SCADA_LIBUT)/libvirt.a\
	$(SCADA_ROOT)/scada/gesrep/librep.a $(SCADA_ROOT)/scada/isa/libisa.a \
	./libscada.a $(SCADA_LIB)/libUtil.a $(SCADA_LIBUT)/libpscs.a \
	$(SCADA_LIBOSF1)/libMarte.a $(SCADA_LIB)/libUtil.a $(SCADA_LIBOSF1)/libXscada.a \
	$(SCADA_LIB)/libcalc.a  $(SCADA_LIB)/liballar.a $(SCADA_LIB)/libarchi.a\
	$(SCADA_LIB)/libtabul.a ./libscada.a $(SCADA_ROOT)/scada/gesrep/librep.a  \
	$(SCADA_SIMACQ)/libsimacq.a $(SCADA_LIBUT)/libinter.a \
	$(SCADA_LIBOSF1)/libMarte.a $(SCADA_LIB)/libUtil.a \
	$(SCADA_LIB)/libNet.a $(SCADA_LIB)/libtabul.a \
	$(SCADA_ACQUI)/libacqui.a \
        $(LEGOROOT_LIB)/libCAP.a $(LEGOROOT_LIB)/libsmartkey.a

SORGENTI = mscada.c nonet.c initdb.c pscserr.c wrdb.c pscsini.c pscswd.c pthread_kill_other_threads_np.c

OGGETTI  = mscada.o nonet.o initdb.o pscserr.o wrdb.o  pscsini.o pscswd.o scgev.o fio.o gdac.o pthread_kill_other_threads_np.o

SORGENTI_LIB = init.c scan_arg.c diagnet.c 
SORGENTI_LIB_ATTIPER = ./attiper/initor.c ./attiper/attiper.c ./attiper/timeor.c
SORGENTI_LIB_PERIF = ./perif/initline.c
SORGENTI_LIB_PAG = ./pag/paglib.c
SORGENTI_LIB_FIO = ./fio/fio.c
SORGENTI_LIB_AGGCFG = ./aggcfg/taggcfg.c
SORGENTI_LIB_PROCV = ./procv/scgev.c ./procv/exall.c ./procv/gesele.c \
	./procv/mmidbl.c ./procv/msoglia.c ./procv/thcwin.c ./procv/exfa.c \
	./procv/hcwin.c ./procv/rwritewi.c ./procv/trfile.c ./procv/descriz.c \
	./procv/fsoglia.c ./procv/hostfio.c ./procv/hostfiow.c \
	./procv/modpun.c ./procv/descrizw.c ./procv/tabfound.c \
	./procv/vsoglia.c ./procv/gdac.c
SORGENTI_LIB_STAMPE = ./stampe/intesta.c ./stampe/testlbg.c ./stampe/stampe.c \
	./stampe/printal.c ./stampe/printar.c ./stampe/printele.c \
	./stampe/virt/printalt.c ./stampe/resetlbg.c
SORGENTI_LIB_DIGOR = ./digor/calcor.c ./digor/calcdi.c ./digor/inibiz.c \
	./digor/asinct.c ./digor/calcau.c ./digor/calcdit.c ./digor/calcort.c 

OGGETTI_LIB = init.o scan_arg.o initor.o initline.o paglib.o intesta.o attiper.o \
	timeor.o calcor.o calcdi.o inibiz.o exall.o gesele.o mmidbl.o \
	msoglia.o thcwin.o exfa.o hcwin.o rwritewi.o trfile.o descriz.o \
	fsoglia.o hostfio.o hostfiow.o modpun.o descrizw.o tabfound.o \
	vsoglia.o asinct.o calcau.o calcdit.o calcort.o diagnet.o \
	stamal.o stamalt.o testlbg.o taggcfg.o

OGGETTI_LIB_STAMPE = stampe.o printal.o printar.o printele.o \
	printalt.o resetlbg.o

SORGENTI_LIB_ALL = wrdb.c rdpb.c get_key.c scan_arg.c cmostime.c biosdate.c\
      riderr.c init.c\
      stamal.c printal.c printar.c printele.c stampe.c intesta.c\
      testlbg.c\
      timeor.c  initor.c calcor.c asinct.c inibiz.c calcdi.c\
      calcort.c calcdit.c calcau.c printalt.c verifalt.c stamalt.c\
      verifall.c exall.c exfa.c rtdebug.c\
      vsoglia.c gesele.c descriz.c descrizw.c modpun.c hostfio.c msoglia.c\
      fsoglia.c taggcfg.c diagnet.c tcpmaste.c dsioc.c hostfiow.c \
      rwritewi.c hcwin.c\
      thcwin.c mmidbl.c tabfound.c trfile.c haspinit.c haspqry.c \
      haspwc.c hasp32b.c 

OGGETTI_LIB_ALL = wrdb.o rdpb.o get_key.o scan_arg.o cmostime.o biosdate.o\
      riderr.o init.o\
      stamal.o printal.o printar.o printele.o stampe.o intesta.o\
      testlbg.o\
      timeor.o  initor.o calcor.o asinct.o inibiz.o calcdi.o\
      calcort.o calcdit.o calcau.o printalt.o verifalt.o stamalt.o\
      verifall.o exall.o exfa.o rtdebug.o\
      vsoglia.o gesele.o descriz.o descrizw.o modpun.o hostfio.o msoglia.o\
      fsoglia.o taggcfg.o diagnet.o tcpmaste.o dsioc.o hostfiow.o \
      rwritewi.o hcwin.o\
      thcwin.o mmidbl.o tabfound.o trfile.o haspinit.o haspqry.o \
      haspwc.o hasp32b.o 

#.c.o:
#	$(CC) -c -Wall $(CFLAGS) $<


all: ./libscada.a $(SCADA_BIN)/scada

./libscada.a: $(OGGETTI_LIB) $(OGGETTI_LIB_MANDBRT) \
	$(OGGETTI_LIB_STAMPE)	$(OGGETTILIB_PERIF) $(OGGETTI_LIB_PAG)
	ar rvs ./libscada.a $?
	@echo libscada.a aggiornata

.c.a:;

$(SCADA_BIN)/scada: $(OGGETTI) $(LIB)
	cc -o $(SCADA_BIN)/scada  $(OGGETTI) $(LIB) $(X_LIBS) -lm $(THREAD_LIB)

taggcfg.o: aggcfg/taggcfg.c
	$(CC) -c $(CFLAGS) aggcfg/taggcfg.c

initor.o: attiper/initor.c
	$(CC) -c $(CFLAGS) attiper/initor.c

timeor.o: attiper/timeor.c
	$(CC) -c $(CFLAGS) attiper/timeor.c

calcor.o: digor/calcor.c
	$(CC) -c $(CFLAGS) digor/calcor.c 

calcdi.o: digor/calcdi.c
	$(CC) -c $(CFLAGS)  digor/calcdi.c

inibiz.o: digor/inibiz.c
	$(CC) -c $(CFLAGS)  digor/inibiz.c

asinct.o: digor/asinct.c
	$(CC) -c $(CFLAGS)  digor/asinct.c

calcau.o: digor/calcau.c
	$(CC) -c $(CFLAGS)  digor/calcau.c

calcdit.o: digor/calcdit.c
	$(CC) -c $(CFLAGS) digor/calcdit.c

calcort.o: digor/calcort.c
	$(CC) -c $(CFLAGS) digor/calcort.c

attiper.o: attiper/attiper.c
	$(CC) -c -Wall $(CFLAGS) attiper/attiper.c

initline.o: perif/initline.c
	$(CC) -c $(CFLAGS) perif/initline.c

paglib.o: pag/paglib.c
	$(CC) -c $(CFLAGS) pag/paglib.c

testlbg.o: stampe/testlbg.c
	$(CC) -c $(CFLAGS) stampe/testlbg.c

stampe.o: stampe/stampe.c
	$(CC) -c $(CFLAGS) stampe/stampe.c

printal.o: stampe/printal.c
	$(CC) -c $(CFLAGS) stampe/printal.c

printalt.o: stampe/virt/printalt.c
	$(CC) -c $(CFLAGS) stampe/virt/printalt.c

resetlbg.o: stampe/resetlbg.c
	$(CC) -c $(CFLAGS) stampe/resetlbg.c

printar.o: stampe/printar.c
	$(CC) -c $(CFLAGS) stampe/printar.c

printele.o: stampe/printele.c
	$(CC) -c $(CFLAGS) stampe/printele.c

intesta.o: stampe/intesta.c
	$(CC) -c $(CFLAGS) stampe/intesta.c

stamal.o: stampe/stamal.c
	$(CC) -c $(CFLAGS) stampe/stamal.c

stamalt.o: stampe/stamalt.c
	$(CC) -c $(CFLAGS) stampe/stamalt.c

scgev.o: procv/scgev.c
	$(CC) -c $(CFLAGS) procv/scgev.c

gdac.o: procv/gdac.c
	$(CC) -c $(CFLAGS) procv/gdac.c

exall.o: procv/exall.c
	$(CC) -c $(CFLAGS) procv/exall.c

gesele.o: procv/gesele.c
	$(CC) -c $(CFLAGS) procv/gesele.c

mmidbl.o: procv/mmidbl.c
	$(CC) -c $(CFLAGS) procv/mmidbl.c

msoglia.o: procv/msoglia.c
	$(CC) -c $(CFLAGS) procv/msoglia.c

thcwin.o: procv/thcwin.c
	$(CC) -c $(CFLAGS) procv/thcwin.c

exfa.o: procv/exfa.c
	$(CC) -c $(CFLAGS) procv/exfa.c

hcwin.o: procv/hcwin.c
	$(CC) -c $(CFLAGS) procv/hcwin.c

rwritewi.o: procv/rwritewi.c
	$(CC) -c $(CFLAGS) procv/rwritewi.c

trfile.o: procv/trfile.c
	$(CC) -c $(CFLAGS) procv/trfile.c

descriz.o: procv/descriz.c
	$(CC) -c $(CFLAGS) procv/descriz.c

descrizw.o: procv/descrizw.c
	$(CC) -c $(CFLAGS) procv/descrizw.c

fsoglia.o: procv/fsoglia.c
	$(CC) -c $(CFLAGS) procv/fsoglia.c

hostfio.o: procv/hostfio.c
	$(CC) -c $(CFLAGS) procv/hostfio.c

hostfiow.o: procv/hostfiow.c
	$(CC) -c $(CFLAGS) procv/hostfiow.c

modpun.o: procv/modpun.c
	$(CC) -c $(CFLAGS) procv/modpun.c

tabfound.o: procv/tabfound.c
	$(CC) -c $(CFLAGS) procv/tabfound.c

vsoglia.o: procv/vsoglia.c
	$(CC) -c $(CFLAGS) procv/vsoglia.c

fio.o: fio/fio.c
	$(CC) -c $(CFLAGS) fio/fio.c

