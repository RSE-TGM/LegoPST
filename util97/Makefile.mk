# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#       Makefile Header:               Makefile
#       Subsystem:              78
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Mar 20 15:45:06 2006 %
#
#########################################################################
# Makefile di svincolo per tutte le directory
# del progetto
all:
	cd ./S01_2_f01 ; $(MAKE) -f Makefile.mk
	cd ./cap ; $(MAKE) -f Makefile.mk
	cd ./connex2 ; $(MAKE) -f Makefile.mk
#	cd ./dbutil ; $(MAKE) -f Makefile.mk
	cd ./mmi_collect ; $(MAKE) -f Makefile.mk
	cd ./pagmod ; $(MAKE) -f Makefile.mk
	cd ./tag2res ; $(MAKE) -f Makefile.mk
	cd ./procedure ; $(MAKE) -f Makefile.mk
	cd ./utilities ; $(MAKE) -f Makefile.mk
	cd ./mf ; $(MAKE) -f Makefile.mk
	cd ./place ; $(MAKE) -f Makefile.mk

