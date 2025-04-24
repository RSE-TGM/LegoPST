# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2"
UTIL97_BIN=../bin

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#   modulo Makefile
#   tipo 
#   release 1.4
#   data 2/28/96
#   reserved @(#)Makefile	1.4
#
CFLAGS =  -I.  -D$(OS) $(C_FLAGS) 
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
CFILES = $(SORGENTI1) $(SORGENTI2) $(SORGENTI3) $(SORGENTI4) $(SORGENTI6) $(SORGENTI7) $(SORGENTI8)
FFILES = $(SORGENTI5)

all:  $(UTIL97_BIN)/modpag  $(UTIL97_BIN)/modallpag $(UTIL97_BIN)/chgpag $(UTIL97_BIN)/chgtask $(UTIL97_BIN)/chgcompiled $(UTIL97_BIN)/chgvar $(UTIL97_BIN)/creasuperlist $(UTIL97_BIN)/UP_SCHEMA_F01

$(UTIL97_BIN)/modpag: $(OGGETTI1)
	$(CC) -o $(UTIL97_BIN)/modpag $(LINKER_OPTIONS) $(OGGETTI1) 
         
$(UTIL97_BIN)/modallpag: $(OGGETTI2)
	$(CC) -o $(UTIL97_BIN)/modallpag $(LINKER_OPTIONS) $(OGGETTI2) 

$(UTIL97_BIN)/chgpag: $(OGGETTI3)
	$(CC) -o $(UTIL97_BIN)/chgpag $(LINKER_OPTIONS) $(OGGETTI3) 

$(UTIL97_BIN)/chgtask: $(OGGETTI4)
	$(CC) -o $(UTIL97_BIN)/chgtask $(LINKER_OPTIONS) $(OGGETTI4) 

$(UTIL97_BIN)/chgcompiled: $(OGGETTI6)
	$(CC) -o $(UTIL97_BIN)/chgcompiled $(LINKER_OPTIONS) $(OGGETTI6)

$(UTIL97_BIN)/creasuperlist: $(OGGETTI7)
	$(CC) -o $(UTIL97_BIN)/creasuperlist $(LINKER_OPTIONS) $(OGGETTI7)

$(UTIL97_BIN)/chgvar: $(OGGETTI8)
	$(CC) -o $(UTIL97_BIN)/chgvar $(LINKER_OPTIONS) $(OGGETTI8)

$(UTIL97_BIN)/UP_SCHEMA_F01: $(OGGETTI5)
	$(FC) -O  -o $(UTIL97_BIN)/UP_SCHEMA_F01 $(OGGETTI5)

