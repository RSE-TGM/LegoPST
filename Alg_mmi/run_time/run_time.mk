# ******* Telelogic expanded section *******

# make_macros from makefile "run_time.mk-12"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=-lbsd
TOOLS=../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g -fcommon -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#       Makefile Header:               run_time.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Jul 11 18:56:57 2006 %

#########################################################################

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
#       Makefile da usarsi per ccm con ccm_make
#
#  	$Date: 92/04/23 14:57:07 $ 		$Revision: 1.28.37.5.45.1 $
#########################################################################
.SUFFIXES: .i .o .c .h
EXECUTABLE	= $(LEGOMMI_BIN)/mmi
MAIN		= run_time.c
INTERFACES	= 	topLevelShell1.c \
	CloseQuestion.c \
	formInfo.c \
	errorDialog.c \
	popup_warning_sim.c \
	topLevelShellMain.c \
	topLevelShellStaz.c \
	questionDialogQuit.c \
	Aing.c \
	Plot.c \
	Attesa.c \
	password.c \
	teleperm.c \
	OperatingWindow.c

APPL_OBJS	= UxXt.o UxMethod.o other_mmi.o Message.o TwoList.o\
                  VariableEditor.o UtilRes.o OneList.o\
                  aux.o PerturbEditor.o  
UX_LIBPATH	= $(UX_DIR)/lib
X_LIBS		= $(X_LIB) -lXm -lXt -lX11
APPL_LIBS	= $(LEGOMMI_LIB)/libXl.a $(LEGOMMI_LIB)/libOl.a\
                  $(LEGOMMI_LIB)/libCs.a \
		  $(LEGORT_LIB)/libnet.a\
 		  $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a \
                  $(LEGOROOT_LIB)/libutil.a \
                  $(LEGOROOT_LIB)/libRt.a \
                  $(LEGOROOT_LIB)/libutilx.a \
		  $(LEGORT_LIB)/libnet.a\
                  $(LEGOROOT_LIB)/libCAP.a\
		  $(LEGOROOT_LIB)/libsmartkey.a
#
#  definizione path per i sorgenti non locali
#
PATHRES  = $(LEGOMMI)/resource
PATHVAR =  $(LEGOMMI)/resource/VARIABILI

X_LIBPATH	= -L/usr/lib
MOTIF_LIBPATH	= -L/usr/lib/  $(X_LIB)

APPL_INCL       = -I$(LEGOCAD_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(X_INCLUDE)\
                  -I$(LEGOMMI_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB)\
                  -I$(UX_DIR)/include -I$(LEGOROOT_LIB) -I.


X_CFLAGS	= -I/usr/include/ $(X_INCLUDE) $(APPL_INCL)  
MOTIF_CFLAGS	= 

CFLAGS		= -DON_MMI  -DXT_CODE $(C_FLAGS) $(X_CFLAGS)\
                  $(MOTIF_CFLAGS) $(VERSIONE)
LIBPATH         = $(X_LIBPATH) $(MOTIF_LIBPATH)
LIBS		= $(APPL_LIBS)  $(SQLITE_LIB) $(X_LIBS) $(C_LIB) $(STUB_LIBS) -lm 


OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS)
IFILES= $(INTERFACES:.c=.i)
all: $(EXECUTABLE) $(OBJS) $(INTERFACES) 


.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

#
#.i.c:
#	$(UXCGEN)
#

$(EXECUTABLE): $(OBJS) $(APPL_LIBS)
	$(CC) $(OBJS) $(LIBPATH) $(LIBS) $(LINKER_OPTIONS) -lsqlite3 -o $(EXECUTABLE)
	@echo "done"
#
# Compilazione dei sorgenti non locali
#
Message.o : $(PATHRES)/Message.c
	$(CC) -c $(CFLAGS) $(PATHRES)/Message.c
OneList.o  : $(PATHVAR)/OneList.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/OneList.c
TwoList.o  : $(PATHVAR)/TwoList.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/TwoList.c 
VariableEditor.o : $(PATHVAR)/VariableEditor.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/VariableEditor.c
UtilRes.o : $(PATHRES)/UtilRes.c
	$(CC) -c $(CFLAGS) $(PATHRES)/UtilRes.c
aux.o : $(PATHVAR)/aux.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/aux.c 
PerturbEditor.o : $(PATHVAR)/PerturbEditor.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/PerturbEditor.c 
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
