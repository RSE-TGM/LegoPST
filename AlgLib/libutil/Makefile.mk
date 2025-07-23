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
C_FLAGS=-g -fcommon -D$(PLATFORM) -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 5.1
#   data 96/04/01
#   reserved @(#)Makefile	5.1
#
UILINCLUDE = $(UIL_INCLUDE) 
CFLAGSINCL=-D$(OS) -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB) -I$(LEGORT_INCLUDE) $(X_INCLUDE)
CFLAGS =$(X_FLAGS) $(CFLAGSINCL) $(VERSIONE) $(C_FLAGS)

SORGENTI = tempo_file.c testata.c chdefaults.c utile.c  \
	spazio_disco.c stato_processo.c suoni.c about.c \
	data.c ora.c genera_lista_entry.c time.c converti_tempo.c \
	statistics.c read_ftn.c
           
OGGETTI =  tempo_file.o testata.o chdefaults.o utile.o  \
	spazio_disco.o stato_processo.o suoni.o about.o \
	data.o ora.o genera_lista_entry.o time.o converti_tempo.o\
	statistics.o read_ftn.o

all:  $(LEGOROOT_LIB)/libutil.a $(LEGOROOT_LIB)/trapfpe.o

$(LEGOROOT_LIB)/libutil.a: $(OGGETTI)
#	$(CC) -c $(CFLAGS) $(?:.o=.c)  
	ar rvs $(LEGOROOT_LIB)/libutil.a $?
	@echo libutil.a aggiornata

$(LEGOROOT_LIB)/trapfpe.o:trapfpe.c
	$(CC) -c $(CFLAGS) trapfpe.c -o $(LEGOROOT_LIB)/trapfpe.o

.c.a:;

