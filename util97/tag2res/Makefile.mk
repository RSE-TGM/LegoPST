# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#       Makefile Header:               Makefile
#       Subsystem:              76
#       Description:
#       %created_by:    joby %
#       %date_created:  Thu Sep  4 15:57:56 1997 %
#
#########################################################################
#
../bin/tag2res : tag2res.sh
	cp -f tag2res.sh $@
	chmod a+x $@
