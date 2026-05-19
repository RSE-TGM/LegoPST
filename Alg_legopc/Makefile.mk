# ******* Telelogic expanded section *******

# make_macros from project "Alg_legopc-2007A1_RHE4_lomgr
#CFLAGS = -g -Wall
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero
CFLAGS = -Wall -DLinux
CC = gcc
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              
#       Description:
#       %created_by:    jurij %
#       %date_created:   Tue Mar 27 10:24:54 2007 %
all:
	cd ./src/c_files; $(MAKE) -f makefile
	cd ./src/f01totom; $(MAKE) -f makefile
	cd ./src/tables; $(MAKE) -f makefile
	cd ./src/tix; $(MAKE) -f makefile
	cd ./src/tix_old; $(MAKE) -f makefile
	cd ./src/inhoud; $(MAKE) -f makefile
	cd ./src/postproc; $(MAKE) -f makefile
	cd ./src/lego; $(MAKE) -f Makefile.mk
