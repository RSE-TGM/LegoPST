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
#       %created_by:    lomgr %
#       %date_created:  Tue Nov  4 08:59:53 2003 %
#
#########################################################################
#
all: ../bin/mmi_collect2 ../bin/mmi_collect3 ../bin/mmi_collect4
#
../bin/mmi_collect2 : mmi_collect2.sh
	cp -f mmi_collect2.sh $@
	chmod a+x $@
#
../bin/mmi_collect3 : mmi_collect3.sh
	cp -f mmi_collect3.sh $@
	chmod a+x $@
#
../bin/mmi_collect4 : mmi_collect4.sh
	cp -f mmi_collect4.sh $@
	chmod a+x $@
