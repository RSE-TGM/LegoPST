# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-1"
LEGOROOT_LIB=../../AlgLib
LEGORT_LIB=../../AlgLib
LEGORT_BIN=../../bin
LEGOROOT_INCLUDE=../../AlgLib/libinclude
LEGORT_INCLUDE=../../include


# make_macros from project "Alg_rt-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11 -g
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
X_INCLUDE=-I.  -I../ -I$(LEGOROOT_LIB)/dcethreads_include -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib
C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib  
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT -DMOTIF
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
F_LIB=
MOTIF_VER=11
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              189
#       Description:
#       %created_by:    zanna %
#       %date_created:  Wed May 21 17:31:52 1997 %

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
EXECUTABLE	= $(LEGORT_BIN)/monitor
MAIN		= monitor.c
INTERFACES	= 	monitorInterface.c
APPL_OBJS	=  UxXt.o comuni.o
##eliminato UX_DIR		= /usr/lpp/aic12
UX_LIBPATH	= $(UX_DIR)/lib
##eliminato UX_INCLUDES     = -I$(UX_DIR)/include
X_LIBS		= -lXm -lXt $(X_LIB) -lX11

X_LIBPATH	= -L/usr/lib
MOTIF_LIBPATH	=
APPL_INCL       = -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOROOT_LIB)
X_CFLAGS	= -I/usr/users/ $(X_INCLUDE) $(APPL_INCL)
MOTIF_CFLAGS	=

#CFLAGS		= -D_BSD   -DXOPEN_CATALOG -DAIXV3 $(VERSIONE) \
#                  $(C_FLAGS) -D$(OS) -DXT_CODE -g \
#		  -DOLD_STAT $(X_CFLAGS) $(MOTIF_CFLAGS) $(UX_INCLUDES)

CFLAGS          = -DXLIB_ILLEGAL_ACCESS $(VERSIONE) $(C_FLAGS)\
                   $(X_CFLAGS) $(APPL_INCL) -DXT_CODE -DOLD_STAT 

LIBPATH		= $(X_LIBPATH) $(MOTIF_LIBPATH) 
APPL_LIBS       = 	$(LEGOROOT_LIB)/libutilx.a  \
							$(LEGORT_LIB)/libnet.a \
                   	$(LEGORT_LIB)/libdispatcher.a \
                   	$(LEGORT_LIB)/libsim.a \
							$(LEGORT_LIB)/libipc.a \
	           			$(LEGOROOT_LIB)/libRt.a \
							$(LEGOROOT_LIB)/libutil.a
LIBS		= $(APPL_LIBS) $(X_LIBS) $(C_LIB) $(STUB_LIBS) -lm                  

OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS)
IFILES= $(INTERFACES:.c=.i)
all: $(EXECUTABLE) $(INTERFACES)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
.i.c:
	$(UXCGEN)

$(EXECUTABLE): $(OBJS) $(APPL_LIBS)
	$(CC) $(OBJS) $(LIBPATH) $(LIBS) $(LINKER_OPTIONS) -o $(EXECUTABLE)
	@echo "done"
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
