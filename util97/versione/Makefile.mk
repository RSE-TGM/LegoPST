# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              201
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Oct 24 18:26:39 2005 %
#
#########################################################################

all:../bin/LGversion
#
#
../bin/LGversion: LGversion.sh
	cp LGversion.sh ../bin/LGversion ; chmod 777 ../bin/LGversion
#