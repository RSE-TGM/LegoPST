# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              211
#       Description:
#       %created_by:    lopez %
#       %date_created:  Mon Oct 28 12:23:57 2002 %
#
#########################################################################
CCOPT = $(C_FLAGS) -DRANDSEED1=0x1111 -DRANDSEED2=0x2222 -DRANDSEED3=0x3333

all:	../bin/CAPinfo ../bin_private/CAPmngr ../bin_private/CAPtest ../bin_private/CAP_AZ ../bin_private/CAP_ZA ../bin_private/CAP_en ../bin_private/CAP_de

../bin/CAPinfo: CAPinfo.c
	$(CC) $(CCOPT) -o $@ CAPinfo.c -lCAP

../bin_private/CAPmngr: CAPmngr.c
	$(CC) $(CCOPT) -o $@ CAPmngr.c -lCAP

../bin_private/CAPtest: CAPtest.c
	$(CC) $(CCOPT) -o $@ CAPtest.c -lCAP

../bin_private/CAP_AZ:  AZ.c
	$(CC) $(CCOPT) -o $@ AZ.c

../bin_private/CAP_ZA:  ZA.c
	$(CC) $(CCOPT) -o $@ ZA.c

../bin_private/CAP_en:  en.c
	$(CC) $(CCOPT) -o $@ en.c

../bin_private/CAP_de:  de.c
	$(CC) $(CCOPT) -o $@ de.c

