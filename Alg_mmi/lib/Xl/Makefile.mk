# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7.2.1"
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
C_FLAGS=-g -fpermissive -fcommon -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-fcommon
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              50
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed May 10 10:57:05 2000 %
.SUFFIXES: .i .o .c .h
INTERFACES = XlCurve_dialog.c XlCurve_date.c XlCurve_conf.c \
             XlCurve_confVar.c XlCurve_selVar.c \
             XlCurve_warningBox.c

APPL_INCLUDE      = -I$(LEGOCAD_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(X_INCLUDE)\
                    -I$(LEGOMMI_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB)\
-I$(LEGOROOT_LIB) -I.

X_CFLAGS        = -I/usr/include/ $(X_INCLUDE) $(APPL_INCL)


CFLAGS = $(C_FLAGS) $(X_CFLAGS) -D$(OS) $(VERSIONE) $(APPL_INCLUDE)

SORGENTI = UxMethod.c XlAnObjList.c  XlCore.c XlLed.c XlCai.c XlCaiAll.c \
	XlCaiVar.c XlBottone.c XlSetResource.c \
	XlClassUtil.c  XlDrawingUtil.c  XlTransferResource.c \
	XlIndic.c        XlComposite.c  \
	XlLabel.c      XlCompilaWidget.c XlRefreshWidget.c \
	XlBitmap.c     XlSelettore.c    XlManager.c \
	XlSetValore.c  XlSetRunTime.c   XlUtilBkg.c \
	XlIcon.c       XlWidgetUtil.c   XlChangePage.c \
	XlIndicErr.c XlGrafico.c   XlEnv.c XlPort.c \
	XlIconReg.c  XlDispReg.c XlAllarmi.c XlAllarmi_func.c \
	XlWidgetRes.c XlPrint.c XlSincro.c XlSincroOld.c \
	XlPictograph.c XlIndicTelep.c  XlOperableKeys.c XlCurve.c \
	XlCurve_func.c XlGif.c XlPixvar.c

OGGETTI  = XlAnObjList.o  XlCore.o XlLed.o XlCai.o XlCaiAll.o XlCaiVar.o\
	XlBottone.o    XlSetResource.o \
	XlClassUtil.o  XlDrawingUtil.o  XlTransferResource.o \
	XlIndic.o        XlComposite.o  \
	XlLabel.o      XlCompilaWidget.o XlRefreshWidget.o \
	XlBitmap.o     XlSelettore.o    XlManager.o \
	XlSetValore.o  XlSetRunTime.o   XlUtilBkg.o \
	XlIcon.o       XlWidgetUtil.o   XlChangePage.o \
	XlIndicErr.o  XlGrafico.o   XlEnv.o XlPort.o \
	XlIconReg.o   XlDispReg.o XlAllarmi.o XlAllarmi_func.o \
	XlWidgetRes.o XlPrint.o XlSincro.o XlSincroOld.o \
	XlPictograph.o XlIndicTelep.o XlOperableKeys.o XlCurve.o \
	XlCurve_func.o XlGif.o XlPixvar.o

OBJS = $(INTERFACES:.c=.o) $(OGGETTI)
IFILES= $(INTERFACES:.c=.i) 
all:  $(LEGOMMI_LIB)/libXl.a $(INTERFACES)

$(LEGOMMI_LIB)/libXl.a : $(OBJS)
	ar rvs $(LEGOMMI_LIB)/libXl.a $?
	@echo libXl.a aggiornata
.c.a:;

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#
#.i.c:
# $(UXCGEN)
#

get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)

