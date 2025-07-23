# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
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
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#   modulo Makefile
#   tipo 
#   release 5.1
#   data 96/03/19
#   reserved @(#)Makefile	5.1
UILINCLUDE = $(UIL_INCLUDE) 
CFLAGSINCL=-D$(OS) -I$(LEGOROOT_INCLUDE) $(X_INCLUDE)
CFLAGS =$(X_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) $(C_FLAGS)

SORGENTI = utstatowidget.c get_something.c set_something.c get_something_val.c set_something_val.c \
           attesa.c def_cursore.c attenzione.c errore.c utlistascroll.c utstrstr.c\
	   commento.c undef_cursore.c conferma.c \
           extract_string.c editor.c attention.c information.c \
           find_kit.c find_in_text.c utilx.c
           
OGGETTI = utstatowidget.o get_something.o set_something.o get_something_val.o set_something_val.o \
           attesa.o def_cursore.o attenzione.o errore.o utlistascroll.o utstrstr.o\
	   commento.o undef_cursore.o conferma.o \
           extract_string.o editor.o attention.o information.o \
           find_kit.o find_in_text.o utilx.o

all:  $(LEGOROOT_LIB)/libutilx.a

$(LEGOROOT_LIB)/libutilx.a: $(OGGETTI)
#	$(CC) -c $(CFLAGS) $(?:.o=.c)  
	ar rvs $(LEGOROOT_LIB)/libutilx.a $?
	@echo libutilx.a aggiornata
.c.a:;

