# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGOROOT_LIB=../AlgLib

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
all: ../bin/dbmrea ../bin/dbm2sql ../bin/dbmftc ../bin/dbmftc2 ../bin/dbmins ../bin/dbmins_mul ../bin/mk_reg_int_conn_db ../bin/mk_name_punt_db

../bin/dbmrea: dbmrea.c
	cc -g $(C_FLAGS) -o $@ dbmrea.c -lgdbm

../bin/dbm2sql: dbm2sql.c
	cc -g $(C_FLAGS) -o $@ dbm2sql.c -lgdbm -lsqlite3

../bin/dbmftc: dbmftc.c
	cc -g $(C_FLAGS) -o $@ dbmftc.c -lgdbm

../bin/dbmftc2: dbmftc2.c
	cc -g $(C_FLAGS) -o $@ dbmftc2.c -lgdbm

../bin/dbmins: dbmins.c
	cc -g $(C_FLAGS) -o $@ dbmins.c -lgdbm

../bin/dbmins_mul: dbmins_mul.c
	cc -g $(C_FLAGS) -o $@ dbmins_mul.c -lgdbm

../bin/mk_reg_int_conn_db:mk_reg_int_conn_db.sh
	cp mk_reg_int_conn_db.sh $@
	chmod a+x $@

../bin/mk_name_punt_db:mk_name_punt_db.sh
	cp mk_name_punt_db.sh $@
	chmod a+x $@
