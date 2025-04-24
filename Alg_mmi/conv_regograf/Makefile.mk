# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=-lbsd
TOOLS=../Tools


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
#       Makefile Header:               Makefile.mk
#       Subsystem:              39
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Mar  7 19:50:29 2006 %

DEPEND=$(LEGOROOT)/install/makedepend
CFLAGS = -I$(LEGOROOT_INCLUDE) -I$(LEGOROOT_LIB) -I$(X_INCLUDE)\
         -I$(LEGOMMI_LIB) -I$(LEGOMMI_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) 
LIBUTIL = $(LEGOMMI_LIB)/libXl.a $(LEGOMMI_LIB)/libOl.a\
                  $(LEGOMMI_LIB)/libCs.a $(LEGOROOT_LIB)/libutilx.a\
                  $(LEGOROOT_LIB)/libutil.a $(LEGORT_LIB)/libnet.a\
                  $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a\
                  $(LEGOROOT_LIB)/libutilx.a $(LEGOROOT_LIB)/libRt.a \
		  $(LEGORT_LIB)/libnet.a

LIBUTIL_LIB = $(LEGOROOT_LIB)/libutil.a $(LEGORT_LIB)/libnet.a\
                  $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a \
		$(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libnet.a

LIBSVIL = $(X_LIB) $(SQLITE_LIB)
X_LIBS          = -lXm -lXt -lX11

SORGENTI = conv_r.c util.c ../config/UxMethod.c ../config/UxXt.c
OGGETTI  = conv_r.o util.o ../config/UxMethod.o ../config/UxXt.o

SORGENTI_LIB = crea_rego_lib.c util.c ../config/UxMethod.c ../config/UxXt.c
OGGETTI_LIB  = crea_rego_lib.o util.o ../config/UxMethod.o ../config/UxXt.o

all:  $(LEGOMMI_BIN)/conv_r $(LEGOMMI_BIN)/crea_rego_lib

makefile:Makefile
	make inc

$(LEGOMMI_BIN)/conv_r: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGOMMI_BIN)/conv_r $(LINKER_OPTIONS) $(OGGETTI) \
	$(LIBSVIL) $(LIBUTIL) $(X_LIB) -lXm -lX11 $(STUB_LIBS) $(C_LIB) -lm -lsqlite3

$(LEGOMMI_BIN)/crea_rego_lib: $(OGGETTI_LIB) $(LIBUTIL) $(LIBUTIL_LIB)
	cc -o $(LEGOMMI_BIN)/crea_rego_lib $(LINKER_OPTIONS) $(OGGETTI_LIB) \
	$(LIBSVIL) $(LIBUTIL_LIB) $(LIBUTIL) $(X_LIB) -lXm -lX11 $(STUB_LIBS) $(C_LIB) -lm  -lsqlite3
