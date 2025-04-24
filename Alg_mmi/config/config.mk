# ******* Telelogic expanded section *******

# make_macros from makefile "config.mk-22"
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
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=-fcommon
UXCGEN=run_uxcgen12.sh $@ $<
#
#       Makefile Header:               config.mk
#       Subsystem:              10
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed Feb 28 10:42:52 2007 %
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
EXECREGCOMP	= $(LEGOMMI_BIN)/compreg
MAINREGCOMP	= comp_reg.c
EXECUTABLE	= $(LEGOMMI_BIN)/config
MAIN		= config.c
INTERFACES	= 	topLevelShell.c \
	PagShell.c \
	IconShell.c \
	PagFunction.c \
	errorDialog.c \
	PagDialog.c \
	LibFunction.c \
	ClipBoard.c \
	messageBoxDialog1.c \
	Set_Snap.c \
	LibToPag.c \
	ContextSelection.c \
	DefineContext2.c \
	PageSelection.c \
	ContextSelection1.c  \
	topRegoDisplayMode.c

RESINTERFACES= $(PATHRES)/AnimateIconSelection.c $(PATHRES)/BitmapProperties.c \
        $(PATHRES)/ColorEditor.c \
        $(PATHRES)/ColorMapViewer.c \
        $(PATHRES)/ColorViewer.c \
        $(PATHRES)/DatabaseSelection.c \
        $(PATHRES)/FontsEditor.c \
        $(PATHRES)/FontsViewer.c \
        $(PATHRES)/FormSelection.c \
        $(PATHRES)/Message.c \
        $(PATHRES)/Resource.c \
        $(PATHRES)/TagsInput.c \
        $(PATHRES)/ValueSelection.c \
        $(PATHRES)/VariablesSelection.c

VARINTERFACES= $(PATHVAR)/OneList.c  $(PATHVAR)/PerturbEditor.c \
	     $(PATHVAR)/TwoList.c \
             $(PATHVAR)/VariableEditor.c
 
DRAWINTERFACES= $(PATHDRAW)/drawShell.c \
                $(PATHDRAW)/line_width_dialog.c \
                $(PATHDRAW)/snap_width_dialog.c

APPL_OBJS	= UxXt.o pagedit.o pagina.o iconlib.o othercnf.o \
                  gestmem.o regolazione.o iconinterf.o comp_all.o UxMethod.o \
                  gest_msg_out.o comp_pag_util.o regol_util.o

APPLREG_OBJS	= UxXt.o UxMethod.o comp_pag_util.o ClipBoardComp.o \
                  regol_util.o

UX_LIBPATH	= $(UX_DIR)/lib
X_LIBS		= -lX11 -lXmu $(X_LIB) -lX11

X_LIBPATH	= -L/usr/lib
MOTIF_LIBPATH	= -L/usr/lib/ $(X_LIB)

APPL_LIBPATH    = -L$(LEGOROOT_LIB)
APPL_INCL       = -I../include -I$(LEGOROOT_INCLUDE) -I$(X_INCLUDE) \
                  -I$(LEGOMMI_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB)\
		  -I$(LEGOROOT_LIB) -I$(UX_DIR)/include

X_CFLAGS	= -I/usr/include/ $(X_INCLUDE)
MOTIF_CFLAGS    =

CFLAGS		= $(C_FLAGS) -DXLIB_ILLEGAL_ACCESS -DON_PAGEDIT \
		  $(VERSIONE) $(APPL_INCL) $(X_INCLUDE) \
		  $(X_CFLAGS) $(MOTIF_CFLAGS) \
                  -DXT_CODE

LIBPATH         = $(X_LIBPATH) $(MOTIF_LIBPATH) $(APPL_LIBPATH) 
LIBS            = $(LEGOMMI_LIB)/libXl.a\
                  $(LEGOMMI_LIB)/libXd.a\
                  $(LEGOMMI_LIB)/libOl.a\
		  $(LEGOMMI_LIB)/libCs.a\
                  $(LEGOROOT_LIB)/libutil.a\
                  $(LEGOROOT_LIB)/libRt.a\
		  $(LEGORT_LIB)/libnet.a\
                  $(LEGORT_LIB)/libsim.a\
                  $(LEGORT_LIB)/libipc.a\
		  $(LEGORT_LIB)/libnet.a\
                  $(LEGOROOT_LIB)/libRt.a\
                  $(LEGOROOT_LIB)/libutil.a\
                  $(LEGOROOT_LIB)/libCAP.a\
		  $(LEGOROOT_LIB)/libsmartkey.a\
                  -lutilx $(X_LIBS) $(STUB_LIBS) -lm $(C_LIB) -lutil $(SQLITE_LIB) -lsqlite3
#
#  definizione path per i sorgenti non locali
#
PATHRES  = $(LEGOMMI)/resource
PATHDRAW = $(LEGOMMI)/draw
PATHVAR =  $(LEGOMMI)/resource/VARIABILI

LNKOBJ          = Resource.o\
                  BitmapProperties.o \
                  ColorViewer.o\
                  ColorEditor.o\
	          other.o\
		  gest_composite.o\
	          Message.o\
                  FontsViewer.o\
                  FontsEditor.o\
                  DatabaseSelection.o\
                  ColorMapViewer.o\
                  AnimateIconSelection.o\
                  VariableEditor.o\
                  OneList.o\
                  TwoList.o\
		  UtilRes.o\
   	          PerturbEditor.o\
		  aux.o\
		  Apply.o\
		  FormSelection.o\
		  ValueSelection.o\
		  TagsInput.o\
		  VariablesSelection.o\
                  snap_width_dialog.o\
                  line_width_dialog.o\
                  drawShell.o\
		  graphics.o\
		  ClipBoardUndo.o draw_wid.o


LIBRERIE = $(LEGOMMI_LIB)/libXl.a $(LEGOMMI_LIB)/libOl.a $(LEGOMMI_LIB)/libXd.a\
		$(LEGOMMI_LIB)/libCs.a $(LEGOROOT_LIB)/libsmartkey.a


OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS) $(LNK_OBJ)
OBJS_REG = $(MAINREGCOMP:.c=.o) $(APPLREG_OBJS) 
IFILES= $(INTERFACES:.c=.i) $(RESINTERFACES:.c=.i) $(VARINTERFACES:.c=.i) $(DRAWINTERFACES:.c=.i)
all: $(EXECUTABLE) $(INTERFACES) $(RESINTERFACES) $(VARINTERFACES) \
	$(DRAWINTERFACES) $(EXECREGCOMP) 
.c.o:
	$(CC) -c  $(CFLAGS) $< -o $@ 
#
#.i.c:
#	$(UXCGEN)
#

$(EXECUTABLE): $(OBJS) $(LIBRERIE) $(LNKOBJ)
	$(CC) $(LINKER_OPTIONS) $(OBJS)  $(LNKOBJ)  $(LIBPATH) $(LIBS) -o $(EXECUTABLE)
$(EXECREGCOMP): $(OBJS_REG)  $(LIBRERIE) 
	$(CC) $(LINKER_OPTIONS) $(OBJS_REG) $(LIBPATH) $(LIBS) -o $(EXECREGCOMP)
	@echo "done"

#
#  Compilazione dei sorgenti non residenti
#

Resource.o : $(PATHRES)/Resource.c
	$(CC) -c $(CFLAGS) $(PATHRES)/Resource.c -o $@
ColorViewer.o : $(PATHRES)/ColorViewer.c
	$(CC) -c $(CFLAGS) $(PATHRES)/ColorViewer.c -o $@
ColorEditor.o : $(PATHRES)/ColorEditor.c
	$(CC) -c $(CFLAGS) $(PATHRES)/ColorEditor.c -o $@
other.o : $(PATHRES)/other.c
	$(CC) -c $(CFLAGS) $(PATHRES)/other.c -o $@
gest_composite.o : $(PATHRES)/gest_composite.c
	$(CC) -c $(CFLAGS) $(PATHRES)/gest_composite.c -o $@
Message.o : $(PATHRES)/Message.c
	$(CC) -c $(CFLAGS) $(PATHRES)/Message.c -o $@
FontsViewer.o : $(PATHRES)/FontsViewer.c
	$(CC) -c $(CFLAGS) $(PATHRES)/FontsViewer.c -o $@
FontsEditor.o : $(PATHRES)/FontsEditor.c
	$(CC) -c $(CFLAGS) $(PATHRES)/FontsEditor.c -o $@
DatabaseSelection.o : $(PATHRES)/DatabaseSelection.c
	$(CC) -c $(CFLAGS) $(PATHRES)/DatabaseSelection.c -o $@
ColorMapViewer.o : $(PATHRES)/ColorMapViewer.c
	$(CC) -c $(CFLAGS) $(PATHRES)/ColorMapViewer.c -o $@
AnimateIconSelection.o : $(PATHRES)/AnimateIconSelection.c
	$(CC) -c $(CFLAGS) $(PATHRES)/AnimateIconSelection.c -o $@
BitmapProperties.o : $(PATHRES)/BitmapProperties.c
	$(CC) -c $(CFLAGS) $(PATHRES)/BitmapProperties.c -o $@
VariableEditor.o : $(PATHVAR)/VariableEditor.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/VariableEditor.c -o $@
OneList.o  : $(PATHVAR)/OneList.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/OneList.c -o $@
TwoList.o  : $(PATHVAR)/TwoList.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/TwoList.c -o $@
UtilRes.o : $(PATHRES)/UtilRes.c
	$(CC) -c $(CFLAGS) $(PATHRES)/UtilRes.c -o $@
PerturbEditor.o : $(PATHVAR)/PerturbEditor.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/PerturbEditor.c -o $@
aux.o : $(PATHVAR)/aux.c
	$(CC) -c $(CFLAGS) $(PATHVAR)/aux.c -o $@
Apply.o : $(PATHRES)/Apply.c
	$(CC) -c $(CFLAGS) $(PATHRES)/Apply.c -o $@
FormSelection.o : $(PATHRES)/FormSelection.c
	$(CC) -c $(CFLAGS) $(PATHRES)/FormSelection.c -o $@
ValueSelection.o : $(PATHRES)/ValueSelection.c
	$(CC) -c $(CFLAGS) $(PATHRES)/ValueSelection.c -o $@
TagsInput.o : $(PATHRES)/TagsInput.c
	$(CC) -c $(CFLAGS) $(PATHRES)/TagsInput.c -o $@
VariablesSelection.o : $(PATHRES)/VariablesSelection.c
	$(CC) -c $(CFLAGS) $(PATHRES)/VariablesSelection.c -o $@
snap_width_dialog.o : $(PATHDRAW)/snap_width_dialog.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/snap_width_dialog.c -o $@
line_width_dialog.o : $(PATHDRAW)/line_width_dialog.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/line_width_dialog.c -o $@
drawShell.o : $(PATHDRAW)/drawShell.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/drawShell.c -o $@
graphics.o : $(PATHDRAW)/graphics.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/graphics.c -o $@
ClipBoardUndo.o : $(PATHDRAW)/ClipBoardUndo.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/ClipBoardUndo.c -o $@
draw_wid.o : $(PATHDRAW)/draw_wid.c
	$(CC) -c $(CFLAGS) $(PATHDRAW)/draw_wid.c -o $@ 
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
