# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
UTIL97_BIN=../bin

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
all: $(UTIL97_BIN)/MF $(UTIL97_BIN)/MF.1.4
#
$(UTIL97_BIN)/MF: MF.perl
	cp MF.perl $(UTIL97_BIN)/MF; chmod 777  $(UTIL97_BIN)/MF
#
$(UTIL97_BIN)/MF.1.4: MF.1.4.perl
	cp MF.1.4.perl $(UTIL97_BIN)/MF.1.4; chmod 777  $(UTIL97_BIN)/MF.1.4
