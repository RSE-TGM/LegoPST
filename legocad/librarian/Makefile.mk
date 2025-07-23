# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-8.1.1"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
C_LIB=/lib/libbsd.a
LEGOCAD_LIB=../lib
LEGOCAD_INCLUDE=../include
LEGOCAD_BIN=../bin
LEGOCAD=..
LEGOCAD_UID=../uid
TOOLS=../Tools



# make_macros from project "legocad-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
#C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
C_FLAGS=-g -DDEBUG -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#C_FLAGS=-g -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE) 
#X_INCLUDE=. -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib -I$(HHHOME)/include -DOSF1
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/bin/X11/uil
X_FLAGS=-c   -DSNAPSHOT
#F_FLAGS=-lfor
#F_LIB=-lg2c -lm
F_LIB= -lm
MOTIF_VER=11
HHHOME=$(TOOLS)/hyperhelp
HYPERHELP_OBJ = $(HHHOME)/winhelp/decaosf/winhelp.o
UXCGEN=run_uxcgen.sh $@ $<
#------------------------ C preprocessor
CPP=cpp
#CPPFLAGS=-P -C -DLINUX
#------------------------ C compiler
#CC=cc
#CFLAGS=$(C_FLAGS)
#.c.o:
#	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              106
#       Description:
#       %created_by:    ciccotel %
#       %date_created:  Thu Nov  7 13:58:17 2002 %

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
EXECUTABLE	= $(LEGOCAD_BIN)/librarian
MAIN		= librarian.c
INTERFACES	= 	mainWindow1.c \
	dbox_lista_mod.c \
	abox_file_selection.c \
	abox_doc_module.c \
	nmod_app_shell.c \
	nmod_dialog_config.c \
	nmod_dialog_equations.c \
	dbox_user_data.c \
	dbox_user_jac.c \
	dbox_user_resid.c \
	dbox_user_eq.c 


APPL_OBJS	= UxXt.o newmod_util.o genera_ftn.o
X_LIBS		= $(X_LIB)

LEGO_LIBPATH    = -L$(LEGOCAD_LIB) -L$(LEGOROOT_LIB)
LEGO_CFLAGS     = -D$(OS) -I$(LEGOCAD_INCLUDE) -I$(LEGOROOT_INCLUDE) $(HYPERHELP_INC) $(X_INCLUDE)

X_LIBPATH	= 
MOTIF_LIBPATH	=
X_CFLAGS	=
MOTIF_CFLAGS	=

CFLAGS		= $(C_FLAGS) \
		  $(X_CFLAGS) $(MOTIF_CFLAGS) $(LEGO_CFLAGS) 
LIBPATH		= $(X_LIBPATH) $(MOTIF_LIBPATH) $(LEGO_LIBPATH) 
LIBUTIL         = $(LEGOCAD_LIB)/liblegocad.a $(LEGOROOT_LIB)/libutilx.a \
                  $(LEGOROOT_LIB)/libutil.a $(LEGOCAD_LIB)/libicofile.a
LIBS		= $(LIBUTIL)\
		  $(X_LIBS) -lm $(LIBOTS) $(F_LIB) -lbsd

OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS) 

IFILES= $(INTERFACES:.c=.i)
all: $(EXECUTABLE) $(INTERFACES) 
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

#
#.i.c:
#	$(UXCGEN)
#

$(EXECUTABLE): $(OBJS) $(LIBUTIL)
#$(CC) $(LINKER_OPTIONS) $(OBJS) $(HYPERHELP_OBJ) $(LIBPATH) $(LIBS) -o $(EXECUTABLE)
	$(CC) $(LINKER_OPTIONS) $(OBJS) $(LIBPATH) $(LIBS) -o $(EXECUTABLE)
	@echo "done"
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
