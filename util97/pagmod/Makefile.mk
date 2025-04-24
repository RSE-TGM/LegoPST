# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
FILES=pagmod.c \
      page.c \
      script.c \
      widgets.c \
      iconregmod.c \
      pagcompiler.c \
      edfdb.c \
      util.c \
      flist.c \
      datetime.c \
      log.c

../bin/pagmod : $(FILES)
	cc $(C_FLAGS) -DDEBUG -D_POSIX_SOURCE -D_POSIX_C_SOURCE=2 -o $@ $(FILES) -lgdbm -lbsd

clean :
	rm *.o pagmod
