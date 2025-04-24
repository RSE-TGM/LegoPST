# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-8"
LEGOMMI_BIN=../bin

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              41
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Jul  3 15:42:09 2002 %

all: $(LEGOMMI_BIN)/clip $(LEGOMMI_BIN)/trova $(LEGOMMI_BIN)/clean \
     $(LEGOMMI_BIN)/servko 

$(LEGOMMI_BIN)/clip:./clip.sh
	cp clip.sh $(LEGOMMI_BIN)/clip ; chmod a+wx $(LEGOMMI_BIN)/clip
$(LEGOMMI_BIN)/clean:./clean.sh
	cp clean.sh $(LEGOMMI_BIN)/clean ; chmod a+wx $(LEGOMMI_BIN)/clean
$(LEGOMMI_BIN)/trova:./trova.sh
	cp trova.sh $(LEGOMMI_BIN)/trova ; chmod a+wx $(LEGOMMI_BIN)/trova
$(LEGOMMI_BIN)/servko:./servko.sh
	cp servko.sh $(LEGOMMI_BIN)/servko ; chmod a+wx $(LEGOMMI_BIN)/servko
