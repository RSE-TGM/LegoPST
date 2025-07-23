# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-10"
LEGOROOT_LIB=../AlgLib
LEGORT_LIB=../AlgLib
LEGOROOT_INCLUDE=../AlgLib/libinclude
LEGORT_INCLUDE=../AlgLib/libinclude
LEGOMMI_LIB=../lib
LEGOMMI_INCLUDE=../include
LEGOMMI_BIN=../bin
LEGOMMI=..
C_LIB=/lib/libbsd.a
TOOLS=../Tools

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g -fcommon  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              35
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Fri Feb 21 15:51:32 1997 %
#
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
#flags per lancio AIC
APPL_INCL       = -I$(LEGOCAD_INCLUDE) -I$(LEGOROOT_INCLUDE) -I$(X_INCLUDE)\
                  -I$(LEGOMMI_INCLUDE) -I$(LEGORT_INCLUDE) -I$(LEGOMMI_LIB)\
                  -I$(UX_DIR)/include -I$(LEGOROOT_LIB) -I.

X_CFLAGS        = -I/usr/include/ $(X_INCLUDE) $(APPL_INCL)

CFLAGS          = -DON_MMI $(C_FLAGS) $(X_CFLAGS)\
                  $(MOTIF_CFLAGS) $(VERSIONE)

all:
	$(MAKE) -f run_time.mk
get_gui_builder:
	@echo GUI:$(GUI_BUILD):$(CFLAGS)
