# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
# Makefile							1997-06-27

CCOPT = $(C_FLAGS) -D_POSIX_SOURCE -D_POSIX_C_SOURCE=2 -DTASK_COUNT=100 -DVAR_COUNT=10000 

all: ../bin/connex2 ../bin/al_sim.conf.example
../bin/connex2: connex2.c
	cc $(CCOPT) -o $@ connex2.c

../bin/al_sim.conf.example: al_sim.conf
	cp -f al_sim.conf $@

connex2tar : 
	tar cvf connex2.tar Makefile connex2.c al_sim.conf

