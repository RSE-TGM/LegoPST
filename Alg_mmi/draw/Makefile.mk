# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-20"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB= -lm -lbsd
TOOLS=../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g -fpermissive -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#	Makefile Header:               Makefile.mk
#       Subsystem:              34
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Thu Mar 31 16:51:59 2005 %

#   modulo Makefile
#   tipo 
#   release 5.2
#   data 96/03/21
#   reserved @(#)Makefile	5.2
#
# @(#)48	1.3  com/config/xtmake.tem.m4, aic, aic322, 9239322 9/30/92 16:31:08
 
#  COMPONENT_NAME: AIC     AIXwindows Interface Composer
#  
#  ORIGINS: 58
#  
#  
#                   Copyright IBM Corporation 1991, 1992
#  
#                         All Rights Reserved
#  
#   Permission to use, copy, modify, and distribute this software and its
#   documentation for any purpose and without fee is hereby granted,
#   provided that the above copyright notice appear in all copies and that
#   both that copyright notice and this permission notice appear in
#   supporting documentation, and that the name of IBM not be
#   used in advertising or publicity pertaining to distribution of the
#   software without specific, written prior permission.
#  

#########################################################################
#
#       MAKEFILE FOR STAND-ALONE XT CODE APPLICATION.  
#
#       EXECUTABLE      is the name of the executable to be created 
#       MAIN            is the .c file containing your main() function 
#       INTERFACES      is a list of the generated C code files
#       APP_OBJS        is a (possibly empty) list of the object code
#                       files that form the non-interface portion of
#                       your application
#
#       In the first three statements, the variables on the right 
#	of the equal sign will be replaced with their corresponding
#       values when the makefile is automatically generated.
#
#       This template is used for makefiles which do not reference 
#       the Ux runtime library.
#
#  	$Date: 92/03/02 14:32:00 $ 		$Revision: 1.28.37.3 $
#########################################################################
.SUFFIXES: .i .o .c .h
EXECUTABLE	= $(LEGOMMI_BIN)/draw
MAIN		= draw.c
INTERFACES	= 	drawShell.c \
	applicationShell1.c \
	line_width_dialog.c \
	snap_width_dialog.c \
	MessageBox.c \
	SaveSelection.c 

APPL_OBJS	= UxXt.o UxMethod.o graphics.o ColorViewer.o\
                  ColorEditor.o\
		  ColorMapViewer.o\
		  UtilRes.o draw_wid.o draw_other.o\
		  ClipBoardUndo.o

#UX_DIR		= /usr/lpp/aic
UX_LIBPATH	= $(UX_DIR)/lib
X_LIBS		= -lX11 $(X_LIB) -lX11

X_LIBPATH	= -L/usr/lib 
X_CFLAGS	= -I. -I/usr/include\
                        -I$(X_INCLUDE)\
                        -I$(LEGORT_INCLUDE)\
                        -I$(LEGOR00T_INCLUDE)\
                        -I$(LEGORT_INCLUDE)/widget\
                        -I$(LEGOMMI_INCLUDE)\
                        -I$(LEGOMMI_LIB)\
                        -I$(LEGOROOT_LIB)\
                        -I$(UX_DIR)/include

APPL_INCLUDE    =  -I$(LEGOCAD_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(X_INCLUDE) \
                  -I$(LEGOMMI_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB)

CFLAGS		= -DXLIB_ILLEGAL_ACCESS $(VERSIONE) $(C_FLAGS)\
		  $(X_CFLAGS) $(APPL_INCLUDE)

LIBPATH		= -L$(LEGOROOT_LIB) $(X_LIBPATH) -L$(LEGOMMI_LIB) $(SQLITE_LIB)

LIBS		= 	$(LEGOMMI_LIB)/libXl.a\
			$(LEGOMMI_LIB)/libOl.a\
			$(LEGOMMI_LIB)/libCs.a\
			$(LEGOMMI_LIB)/libXd.a\
                        $(LEGOROOT_LIB)/libRt.a\
                        $(LEGORT_LIB)/libnet.a\
                        $(LEGORT_LIB)/libsim.a\
                        $(LEGORT_LIB)/libcom.a\
                        $(LEGORT_LIB)/libipc.a\
                        $(LEGORT_LIB)/libnet.a\
                        $(LEGOROOT_LIB)/libRt.a\
                        -lutil -lutilx $(X_LIBS)  $(C_LIB) $(STUB_LIBS) $(OTHER_LIB) -lsqlite3

LIBRERIE	= $(LEGOMMI_LIB)/libXl.a $(LEGOMMI_LIB)/libOl.a\
			$(LEGOMMI_LIB)/libCs.a $(LEGOMMI_LIB)/libXd.a

OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS)
IFILES= $(INTERFACES:.c=.i)
all: $(EXECUTABLE) $(INTERFACES) 

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#
#.i.c:
#	$(UXCGEN)
#

$(EXECUTABLE): $(OBJS) $(LIBRERIE)
	$(CC) $(LINKER_OPTIONS) $(OBJS) $(LIBPATH) $(LIBS) -o $(EXECUTABLE)
	@echo "done"
#
#   oggetti provenienti da altri direttori
#
draw.o : draw.c
	$(CC) -c $(CFLAGS) draw.c -o draw.o
draw_wid.o : draw_wid.c
	$(CC) -c $(CFLAGS) draw_wid.c -o draw_wid.o
draw_other.o : draw_other.c
	$(CC) -c $(CFLAGS) draw_other.c -o draw_other.o
drawShell.o : drawShell.c
	$(CC) -c $(CFLAGS) drawShell.c -o drawShell.o
applicationShell1.o : applicationShell1.c
	$(CC) -c $(CFLAGS) applicationShell1.c -o applicationShell1.o
line_width_dialog.o : line_width_dialog.c
	$(CC) -c $(CFLAGS) line_width_dialog.c -o line_width_dialog.o
snap_width_dialog.o : snap_width_dialog.c
	$(CC) -c $(CFLAGS) snap_width_dialog.c -o snap_width_dialog.o
MessageBox.o : MessageBox.c
	$(CC) -c $(CFLAGS) MessageBox.c -o MessageBox.o
SaveSelection.o : SaveSelection.c
	$(CC) -c $(CFLAGS) SaveSelection.c -o SaveSelection.o
graphics.o : graphics.c
	$(CC) -c $(CFLAGS) graphics.c -o graphics.o
UxXt.o : UxXt.c
	$(CC) -c $(CFLAGS) UxXt.c -o UxXt.o
UxMethod.o : UxMethod.c
	$(CC) -c $(CFLAGS) UxMethod.c -o UxMethod.o

ColorEditor.o : ../resource/ColorEditor.c
	$(CC) -c $(CFLAGS) ../resource/ColorEditor.c -o ColorEditor.o

ColorViewer.o : ../resource/ColorViewer.c
	$(CC) -c $(CFLAGS) ../resource/ColorViewer.c -o ColorViewer.o

ColorMapViewer.o : ../resource/ColorMapViewer.c
	$(CC) -c $(CFLAGS) ../resource/ColorMapViewer.c -o ColorMapViewer.o

UtilRes.o : ../resource/UtilRes.c
	$(CC) -c $(CFLAGS) ../resource/UtilRes.c -o UtilRes.o
gui:
	$(GUI_BUILD) -xrm "Aic.cflags: $(CFLAGS)" $(PRJ_FILE)
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)

