#
#	Makefile Header:               %name%
#       Subsystem:              %subsystem%
#       Description:
#       %created_by:    %
#       %date_created:  %

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

EXECUTABLE	= xscada
MAIN		= xscada.c
INTERFACES	= 	ScadaLogMain.c \
	ScadaDebugMain.c \
	Scada.c \
	ScadaMandbMain.c 

APPL_OBJS	= UxXt.o other_xscada.o util_xscada.o
UX_DIR		= /usr/lpp/aic
UX_LIBPATH	= $(UX_DIR)/lib
X_LIBS		= -lXm -lXt -lX11

X_LIBPATH	= 
MOTIF_LIBPATH	=
X_CFLAGS	= -DXSCADA_INTERFACE
MOTIF_CFLAGS	=

CFLAGS		= -D_BSD   -DXOPEN_CATALOG -DAIXV3 \
		  $(X_CFLAGS) $(MOTIF_CFLAGS) 
LIBPATH		= $(X_LIBPATH) $(MOTIF_LIBPATH) 
LIBS		= $(X_LIBS) -lm


OBJS = $(MAIN:.c=.o) $(INTERFACES:.c=.o) $(APPL_OBJS)


.c.o:
	$(CC) -c $(CFLAGS) $< -o $@


$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LIBPATH) $(LIBS) -o $(EXECUTABLE)
	@echo "done"
