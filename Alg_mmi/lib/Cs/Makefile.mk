# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../AlgLib/libinclude
LEGOMMI_LIB=..
LEGOMMI_INCLUDE=../../include

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
#	Makefile Header:               Cs.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Dec 23 10:39:47 1998 %

DEPEND=$(LEGOROOT)/install/makedepend
CFLAGSINCL=-D$(OS) -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) $(X_INCLUDE) -I$(LEGOMMI_LIB) -I$(LEGOROOT_LIB)
CFLAGS = $(X_FLAGS) $(CFLAGSINCL) -D$(OS) $(VERSIONE) $(C_FLAGS)

SORGENTI = CsPrepMsg.c CsPrepMsgS.c CsRcvToken.c CsRcvTokenS.c CsPrepMsgDel.c CsRcvMsgDel.c \
	CsPrepMsgF22.c CsRcvMsgF22.c CsChangePoint.c  \
	CsRcvMsgChangePoint.c CsRcvMsgAllReq.c CsRcvMsgAllCommand.c \
	CsPrepMsgAllCommand.c CsPrepMsgAllReq.c CsRcvMsgMiniASDReq.c \
        CsPrepMsgMiniASDReq.c CsRcvMsgMiniASDRic.c CsPrepMsgMiniASDRic.c \
        CsPrepMsgMiniASDDes.c CsRcvMsgMiniASDDes.c CsPrepMsgSimID.c \
        CsRcvMsgSimID.c CsPrepMsgSimIDAck.c CsRcvMsgSimIDAck.c CsFunc.c

OGGETTI  = CsPrepMsg.o CsPrepMsgS.o CsRcvToken.o CsRcvTokenS.o CsPrepMsgDel.o CsRcvMsgDel.o \
	CsPrepMsgF22.o CsRcvMsgF22.o CsChangePoint.o  \
	CsRcvMsgChangePoint.o CsRcvMsgAllReq.o CsRcvMsgAllCommand.o \
	CsPrepMsgAllCommand.o CsPrepMsgAllReq.o CsRcvMsgMiniASDReq.o \
        CsPrepMsgMiniASDReq.o CsRcvMsgMiniASDRic.o CsPrepMsgMiniASDRic.o \
        CsPrepMsgMiniASDDes.o CsRcvMsgMiniASDDes.o CsPrepMsgSimID.o \
        CsRcvMsgSimID.o CsPrepMsgSimIDAck.o CsRcvMsgSimIDAck.o CsFunc.o

all:  $(LEGOMMI_LIB)/libCs.a

$(LEGOMMI_LIB)/libCs.a: $(OGGETTI)
	ar rvs $(LEGOMMI_LIB)/libCs.a $?
	@echo libCs.a aggiornata
.c.a:;
