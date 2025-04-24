# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
LEGORT_LIB=..
LEGOROOT_LIB=..
LEGOROOT_INCLUDE=../libinclude
LEGORT_INCLUDE=../libinclude


# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fcommon -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 5.3
#   data 96/03/19
#   reserved @(#)Makefile	5.3
#
SORGENTI = msg_rcv.c  msgesist.c msg_snd.c msg_create.c msg_close.c \
	   msg_create_fam_sim.c msg_close_fam_sim.c sem_set.c\
	   msg_create_fam.c msg_close_fam.c\
	   sem_create.c sem_open.c sem_rm.c sem_close.c sem_signal.c \
           sem_op.c sem_wait.c num_sem_agg.c \
	   fifo_close.c fifo_create.c write_fifo.c read_fifo.c

OGGETTI  = msg_rcv.o  msgesist.o msg_snd.o msg_create.o msg_close.o \
	   msg_create_fam_sim.o msg_close_fam_sim.o sem_set.o\
	   msg_create_fam.o msg_close_fam.o\
	   sem_create.o sem_open.o sem_rm.o sem_close.o sem_signal.o \
           sem_op.o sem_wait.o num_sem_agg.o \
	   fifo_close.o fifo_create.o write_fifo.o read_fifo.o

CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(C_FLAGS)

all:  $(LEGORT_LIB)/libipc.a

makefile:Makefile
	make inc

$(LEGORT_LIB)/libipc.a: $(OGGETTI)
#	$(CC) -c $(CFLAGS) $(?:.o=.c)
	ar rvs $(LEGORT_LIB)/libipc.a $?
	@echo $(LEGORT_LIB)/libipc.a is now up-to-date
.c.a:;

