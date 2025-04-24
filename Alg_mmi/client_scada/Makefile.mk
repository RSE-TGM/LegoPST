# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-8"
ROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
OTHER_LIB=-lm -lbsd



# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
#SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
SQLITE_LIB=-lsqlite3
#THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
THREAD_LIB=-ldl $(LEGOROOT_LIB)/dcethreads_lib/libdcethreads.so
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
#C_FLAGS=-g  -DLINUX -DLINUX9   -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include   -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              130
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Oct 24 15:28:05 2005 %
#
#########################################################################
#
CFLAGS = -I. -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) -I$(LEGOMMI_LIB) -D$(OS) -DSCADA_MMI $(C_FLAGS) $(VERSIONE) -I$(LEGOROOT_LIB)
LIBUTIL = $(LEGORT_LIB)/libnet.a \
$(LEGORT_LIB)/libsim.a $(LEGOROOT_LIB)/libutil.a \
$(LEGOMMI_LIB)/libCs.a $(LEGORT_LIB)/libipc.a 

SORGENTI = client_scada.c  client_scada_func.c pthread_kill_other_threads_np.c
OGGETTI  = client_scada.o  client_scada_func.o pthread_kill_other_threads_np.o

all:  $(LEGOMMI_BIN)/client_scada 

$(LEGOMMI_BIN)/client_scada: $(OGGETTI) $(LIBUTIL)
	$(CC) -o $(LEGOMMI_BIN)/client_scada $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) -lm $(SQLITE_LIB) -lc  $(THREAD_LIB) 

