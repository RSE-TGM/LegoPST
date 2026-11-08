# ******* Telelogic expanded section *******
all:
	cd ./banner; $(MAKE) -f Makefile.mk
	cd ./check_lego; $(MAKE) -f Makefile.mk