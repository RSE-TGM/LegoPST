# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              201
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Oct 24 18:26:39 2005 %
#
#########################################################################

all:../bin/creasim ../bin/creastazsim ../bin/direct.sh ../bin/mkstaz \
    ../bin/togli_ctrlm ../bin/superscape ../bin/alg_help ../bin/creascada ../bin/lanciascada\
    ../bin/cercadoppi.sh ../bin/chgsino ../bin/chpg2newpg ../bin/creasvincolo \
    ../bin/diffs01 ../bin/divstaz ../bin/elab_cai_var.sh ../bin/leggi_cai_var.sh \
    ../bin/lego_back ../bin/listacai ../bin/mkdtw ../bin/mkstaz2 \
    ../bin/mod2pag ../bin/modoff ../bin/modrtf ../bin/newval.sh \
    ../bin/numcifre ../bin/pulipag ../bin/punt2canc.sh ../bin/sosti.sh \
    ../bin/tab2sem.sh ../bin/tab2sem_all.sh \
	../bin/lgswitch ../bin/lgswitch_legacy ../bin/lgversion
#
../bin/lgversion: lgversion.sh
	cp lgversion.sh ../bin/lgversion;chmod 777 ../bin/lgversion ;
#
../bin/lgswitch: lgswitch.sh
	cp lgswitch.sh ../bin/lgswitch;chmod 777 ../bin/lgswitch ;
#
../bin/lgswitch_legacy: lgswitch_legacy.sh
	cp lgswitch_legacy.sh ../bin/lgswitch_legacy;chmod 777 ../bin/lgswitch_legacy ;
#
../bin/creasim: creasim.sh
	cp creasim.sh ../bin/creasim;chmod 777 ../bin/creasim ;
#
../bin/creastazsim: creastazsim.sh
	 cp creastazsim.sh ../bin/creastazsim;chmod 777 ../bin/creastazsim;
#
../bin/direct.sh: direct.sh 
	cp direct.sh ../bin/direct.sh; chmod 777 ../bin/direct.sh;
#
../bin/mkstaz: mkstaz.sh
	cp mkstaz.sh ../bin/mkstaz;chmod 777 ../bin/mkstaz;
#
../bin/togli_ctrlm: togli_ctrlm.sh
	cp togli_ctrlm.sh ../bin/togli_ctrlm;chmod 777 ../bin/togli_ctrlm;
#
../bin/superscape: superscape.sh
	cp superscape.sh ../bin/superscape;chmod 777 ../bin/superscape;
#
../bin/alg_help: alg_help.sh
	cp alg_help.sh ../bin/alg_help;chmod 777 ../bin/alg_help;
#
../bin/creascada: creascada.sh
	cp creascada.sh ../bin/creascada;chmod 777 ../bin/creascada;
#
../bin/lanciascada: lanciascada.sh
	cp lanciascada.sh ../bin/lanciascada;chmod 777 ../bin/lanciascada;
#
../bin/cercadoppi.sh: cercadoppi.sh.sh
	cp cercadoppi.sh.sh ../bin/cercadoppi.sh ; chmod 777 ../bin/cercadoppi.sh
#
../bin/chgsino: chgsino.sh
	cp chgsino.sh ../bin/chgsino ; chmod 777 ../bin/chgsino
#
../bin/chpg2newpg: chpg2newpg.sh
	cp chpg2newpg.sh ../bin/chpg2newpg ; chmod 777 ../bin/chpg2newpg
#
../bin/creasvincolo: creasvincolo.sh
	cp creasvincolo.sh ../bin/creasvincolo ; chmod 777 ../bin/creasvincolo
#
../bin/diffs01: diffs01.sh
	cp diffs01.sh ../bin/diffs01 ; chmod 777 ../bin/diffs01
#
../bin/divstaz: divstaz.sh
	cp divstaz.sh ../bin/divstaz ; chmod 777 ../bin/divstaz
#
../bin/elab_cai_var.sh: elab_cai_var.sh.sh
	cp elab_cai_var.sh.sh ../bin/elab_cai_var.sh ; chmod 777 ../bin/elab_cai_var.sh
#
../bin/leggi_cai_var.sh: leggi_cai_var.sh.sh
	cp leggi_cai_var.sh.sh ../bin/leggi_cai_var.sh ; chmod 777 ../bin/leggi_cai_var.sh
#
../bin/lego_back: lego_back.sh
	cp lego_back.sh ../bin/lego_back ; chmod 777 ../bin/lego_back
#
../bin/listacai: listacai.sh
	cp listacai.sh ../bin/listacai ; chmod 777 ../bin/listacai
#
../bin/mkdtw: mkdtw.sh
	cp mkdtw.sh ../bin/mkdtw ; chmod 777 ../bin/mkdtw
#
../bin/mkstaz2: mkstaz2.sh
	cp mkstaz2.sh ../bin/mkstaz2 ; chmod 777 ../bin/mkstaz2
#
../bin/mod2pag: mod2pag.sh
	cp mod2pag.sh ../bin/mod2pag ; chmod 777 ../bin/mod2pag
#
../bin/modoff: modoff.sh
	cp modoff.sh ../bin/modoff ; chmod 777 ../bin/modoff
#
../bin/modrtf: modrtf.sh
	cp modrtf.sh ../bin/modrtf ; chmod 777 ../bin/modrtf
#
../bin/newval.sh: newval.sh.sh
	cp newval.sh.sh ../bin/newval.sh ; chmod 777 ../bin/newval.sh
#
../bin/numcifre: numcifre.sh
	cp numcifre.sh ../bin/numcifre ; chmod 777 ../bin/numcifre
#
../bin/pulipag: pulipag.sh
	cp pulipag.sh ../bin/pulipag ; chmod 777 ../bin/pulipag
#
../bin/punt2canc.sh: punt2canc.sh.sh
	cp punt2canc.sh.sh ../bin/punt2canc.sh ; chmod 777 ../bin/punt2canc.sh
#
../bin/sosti.sh: sosti.sh.sh
	cp sosti.sh.sh ../bin/sosti.sh ; chmod 777 ../bin/sosti.sh
#
../bin/tab2sem.sh: tab2sem.sh.sh
	cp tab2sem.sh.sh ../bin/tab2sem.sh ; chmod 777 ../bin/tab2sem.sh
#
../bin/tab2sem_all.sh: tab2sem_all.sh.sh
	cp tab2sem_all.sh.sh ../bin/tab2sem_all.sh ; chmod 777 ../bin/tab2sem_all.sh
#
