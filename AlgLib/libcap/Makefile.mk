# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-15"

# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fcommon -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
# Makefile							1997-03-25

#CFLAGS = $(C_FLAGS) -D_OSF_SOURCE -DRANDSEED1=0x1122 -DRANDSEED2=0x2233 -DRANDSEED3=0x3344 -D_POSIX_SOURCE -D_POSIX_C_SOURCE=2 -g2 -I../libinclude 
CFLAGS = $(C_FLAGS) -DRANDSEED1=0x1133 -DRANDSEED2=0x2244 -DRANDSEED3=0x3355 -I../libinclude 

SORGENTI = CAProut.c license.c

OGGETTI = CAProut.o license.o

all:	../libCAP.a 

../libCAP.a: $(OGGETTI) 
	ar rvs ../libCAP.a $?
	@echo libCAP.a aggiornata

# Sezione per lo sviluppo						1997-03-25

captar : 
	tar cvf cap.tar CAP*.[ch] Makefile

Ltest:
	lint -wk -DRANDSEED1=0x1111 -DRANDSEED2=0x2222 -DRANDSEED3=0x3333 CAPtest.c

Linfo:
	lint -wk -DRANDSEED1=0x1111 -DRANDSEED2=0x2222 -DRANDSEED3=0x3333 CAPinfo.c

Lrout:
	lint -wk -DRANDSEED1=0x1111 -DRANDSEED2=0x2222 -DRANDSEED3=0x3333 CAProut.c

Lmngr:
	lint -wk -DRANDSEED1=0x1111 -DRANDSEED2=0x2222 -DRANDSEED3=0x3333 CAPmngr.c

