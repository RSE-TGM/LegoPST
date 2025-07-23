# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
LEGOMMI_LIB=..
LEGOMMI_INCLUDE=../../include
TOOLS=../../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              49
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Jul 11 15:25:42 2006 %

DEPEND=$(LEGOROOT)/install/makedepend
CFLAGSINCL= -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) $(X_INCLUDE) -I$(LEGOMMI_LIB) -I$(LEGOROOT_LIB)
CFLAGS = $(C_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE)

# GUAG2025 Tolgo messageBoxDialog2 per il problema del UxNewContext
# GUAG2025 provo reinserire messageBoxDialog2.c
SORGENTI = XdUtil.c XdGeom.c XdCore.c XdLine.c XdRect.c XdCircle.c XdLista.c XdPoli.c XdConn.c XdGroup.c XdUndo.c messageBoxDialog2.c
OGGETTI  = XdUtil.o XdGeom.o XdCore.o XdLine.o XdRect.o XdCircle.o XdLista.o XdPoli.o XdConn.o XdGroup.o XdUndo.o messageBoxDialog2.o
#SORGENTI = XdUtil.c XdGeom.c XdCore.c XdLine.c XdRect.c XdCircle.c XdLista.c XdPoli.c XdConn.c XdGroup.c XdUndo.c 
#OGGETTI  = XdUtil.o XdGeom.o XdCore.o XdLine.o XdRect.o XdCircle.o XdLista.o XdPoli.o XdConn.o XdGroup.o XdUndo.o 



all:  $(LEGOMMI_LIB)/libXd.a

$(LEGOMMI_LIB)/libXd.a: $(OGGETTI)
	ar rvs $(LEGOMMI_LIB)/libXd.a $?
	@echo libXd.a aggiornata
.c.a:;

