# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-5"
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
F_LIB=-lgfortran -lm
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

#       Makefile Header:               Makefile.mk
#       Subsystem:              68
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed May 10 12:26:14 2000 %
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
#  	$Date: 92/04/23 14:57:07 $ 		$Revision: 1.28.37.5.45.1 $
#########################################################################
.SUFFIXES: .i .o .c .h
EXECUTABLE	= menu_graf
MAIN		= menu_graf.c
INTERFACES	= 	Menu_graf.c \
	fileSelectionBox1.c \
	Edit_Time_Win.c \
	Sampling.c \
	Resampling.c \
	Description.c \
	CalcWindow.c \
	File_Save.c \
	Overwrite.c \
	Menu_graf_exit.c \
	Message.c \
	SimulationTitle.c \
	FilterWindow.c 

APPL_OBJS	= UxXt.o f22io.o clip.o filtro.o
UX_LIBPATH	= $(UX_DIR)/lib
X_LIBS		= -lXm -lXt -lX11

X_LIBPATH       = -L/usr/lib
X_CFLAGS	= -I/usr/include/ -I/usr/lib/Xt -I$(UX_DIR)/include
MOTIF_CFLAGS	= -I/usr/include/ 
MY_INCL         = -I$(LEGOROOT_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOCAD_INCLUDE)

CFLAGS		= $(C_FLAGS) $(X_CFLAGS) $(MOTIF_CFLAGS) $(MY_INCL)
LIBPATH         = -L$(LEGOROOT_LIB) -L$(LEGOCAD_LIB) $(X_LIBPATH)
LIBUTIL         = $(LEGOROOT_LIB)/libutil.a $(LEGOCAD_LIB)/liblegocad.a \
                  $(LEGOROOT_LIB)/libutilx.a
LIBS		= $(LIBUTIL)  $(X_LIBS) -lm

OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS) 
IFILES= $(INTERFACES:.c=.i)
all: $(LEGOCAD_BIN)/$(EXECUTABLE) $(INTERFACES) 

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

#
#.i.c:
#	$(UXCGEN)
#

$(LEGOCAD_BIN)/$(EXECUTABLE): $(OBJS) $(LIBUTIL)
	$(CC) $(OBJS) $(LIBPATH) $(LIBS) $(C_LIB) -o $(LEGOCAD_BIN)/$(EXECUTABLE)
	@echo "done"
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
