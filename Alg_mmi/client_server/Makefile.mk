# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
OTHER_LIB=-lm -lbsd


# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              45
#       Description:
#       %created_by:    famgr %
#       %date_created:  Thu Oct 24 15:40:50 1996 %

#2/2/96: inserito linkaggio con OTHER_LIB=-lm -lsocket in SCO_UNIX
DEPEND=$(LEGOROOT)/install/makedepend
#CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) -g
CFLAGS = -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB) -D$(OS) $(VERSIONE) -DUNIX  $(C_FLAGS) -I$(LEGOROOT_LIB) -I$(LEGOROOT_INCLUDE)

LIBUTIL = $(LEGORT_LIB)/libnet.a \
	$(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libutil.a \
	$(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
	$(LEGOMMI_LIB)/libCs.a $(LEGORT_LIB)/libipc.a
SORGENTI_DEMONE = demone_mmi.c
OGGETTI_DEMONE  = demone_mmi.o
SORGENTI_CLIENT = client_mmi.c
OGGETTI_CLIENT  = client_mmi.o
SORGENTI_SERVER = server_mmi.c
OGGETTI_SERVER  = server_mmi.o

all:  $(LEGOMMI_BIN)/demone_mmi $(LEGOMMI_BIN)/client_mmi \
	$(LEGOMMI_BIN)/server_mmi

$(LEGOMMI_BIN)/demone_mmi: $(OGGETTI_DEMONE) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/demone_mmi $(LINKER_OPTIONS) $(OGGETTI_DEMONE) \
	$(LIBUTIL) $(OTHER_LIB)

$(LEGOMMI_BIN)/client_mmi: $(OGGETTI_CLIENT) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/client_mmi $(LINKER_OPTIONS) $(OGGETTI_CLIENT) \
	$(LIBUTIL) $(OTHER_LIB)

$(LEGOMMI_BIN)/server_mmi: $(OGGETTI_SERVER) $(LIBUTIL)
	cc  -o $(LEGOMMI_BIN)/server_mmi $(LINKER_OPTIONS) $(OGGETTI_SERVER) \
	$(LIBUTIL) $(OTHER_LIB)

