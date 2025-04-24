# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5.2.2"
LEGOMMI_BIN=../bin
#LEGORT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOROOT_LIB=../AlgLib


# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#   modulo Makefile
#   tipo 
#   release 1.4
#   data 2/28/96
#   reserved @(#)Makefile	1.4
#
CFLAGS =  -I. -I$(LEGOROOT_LIB) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB) \
          -D$(OS) $(C_FLAGS) 
LIBUTIL =

SORGENTI1 = modpag.c  modpagutil.c
OGGETTI1  = modpag.o  modpagutil.o
SORGENTI2 = modallpag.c modpagutil.c
OGGETTI2  = modallpag.o modpagutil.o
SORGENTI3 = chgpag.c  modifica.c
OGGETTI3  = chgpag.o  modifica.o
SORGENTI4 = chgtask.c chgtask1.c
OGGETTI4  = chgtask.o chgtask1.o 
SORGENTI5 = UP_SCHEMA_F01.f 
OGGETTI5 = UP_SCHEMA_F01.o
SORGENTI6 = chgcompiled.c chgcompiled1.c
OGGETTI6 = chgcompiled.o chgcompiled1.o
SORGENTI7 = creasuperlist.c creasuperlist1.c
OGGETTI7 = creasuperlist.o creasuperlist1.o
SORGENTI8 = chgvar.c chgvar1.c 
OGGETTI8 = chgvar.o chgvar1.o
SORGENTI9 = cleanlocmmi.c 
OGGETTI9 = cleanlocmmi.o
CFILES = $(SORGENTI1) $(SORGENTI2) $(SORGENTI3) $(SORGENTI4) $(SORGENTI6) $(SORGENTI7) $(SORGENTI8) $(SORGENTI9)
FFILES = $(SORGENTI5)
#all:  $(LEGOMMI_BIN)/modpag  $(LEGOMMI_BIN)/modallpag $(LEGOMMI_BIN)/chgpag $(LEGOMMI_BIN)/chgtask $(LEGOMMI_BIN)/chgcompiled $(LEGOMMI_BIN)/chgvar $(LEGOMMI_BIN)/creasuperlist $(LEGOMMI_BIN)/UP_SCHEMA_F01
all: $(LEGOMMI_BIN)/chgcompiled $(LEGOMMI_BIN)/cleanloc
$(LEGOMMI_BIN)/modpag: $(OGGETTI1)
	cc -o $(LEGOMMI_BIN)/modpag $(LINKER_OPTIONS) $(OGGETTI1) 
         
$(LEGOMMI_BIN)/modallpag: $(OGGETTI2)
	cc -o $(LEGOMMI_BIN)/modallpag $(LINKER_OPTIONS) $(OGGETTI2) 

$(LEGOMMI_BIN)/chgpag: $(OGGETTI3)
	cc -o $(LEGOMMI_BIN)/chgpag $(LINKER_OPTIONS) $(OGGETTI3) 

$(LEGOMMI_BIN)/chgtask: $(OGGETTI4)
	cc -o $(LEGOMMI_BIN)/chgtask $(LINKER_OPTIONS) $(OGGETTI4) 

$(LEGOMMI_BIN)/chgcompiled: $(OGGETTI6)
	cc -o $(LEGOMMI_BIN)/chgcompiled $(LINKER_OPTIONS) $(OGGETTI6)

$(LEGOMMI_BIN)/creasuperlist: $(OGGETTI7)
	cc -o $(LEGOMMI_BIN)/creasuperlist $(LINKER_OPTIONS) $(OGGETTI7)

$(LEGOMMI_BIN)/chgvar: $(OGGETTI8)
	cc -o $(LEGOMMI_BIN)/chgvar $(LINKER_OPTIONS) $(OGGETTI8)

$(LEGOMMI_BIN)/cleanloc: $(OGGETTI9)
	cc -o $(LEGOMMI_BIN)/cleanloc $(LINKER_OPTIONS) $(OGGETTI9)

$(LEGOMMI_BIN)/UP_SCHEMA_F01: $(OGGETTI5)
	f77 -O  -o $(LEGOMMI_BIN)/UP_SCHEMA_F01 $(OGGETTI5)

