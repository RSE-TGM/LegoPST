# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7.1.1"
LEGO_BIN=../bin

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c  -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
CPPFLAGS=-P -C -DLINUX -traditional
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
#F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#       Makefile Header:               Makefile.mk
#       Subsystem:              122
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Fri Jun 28 11:50:43 2002 %
CCM_MAKE_LOCAL=cp
SHELLFILES= S01.tpl info_mac.tpl lg3_reg.tpl \
cad_curdir.sh cad_crealg1.sh cad_crealg3.sh cad_crealg5sk.sh  \
cad_maketask.sh  cad_crealibut.sh cad_crealibrut.sh cad_crealibreg.sh\
crealg1.sh crealg5.sh crealg5sk.sh crealibreg.sh crealibut.sh crealibut_reg.sh \
crearegime.sh crearg5sk.sh fortof.sh legohelp.sh lg3.inp.sh librmake.tpl.sh \
makelibr.sh maketask.sh testerr.sh creanlsk.sh cad_environment.sh \
creamrg5sk.sh crealibut_mreg.sh cad_f012lis.sh cad_lism2lis.sh cad_lisc2lis.sh \
lg3debug.sh lg5debug.sh makelibr_pgnouse.sh testforaus.sh \
cad_marcotask.sh cad_testenv.sh
ALLFILES = $(CFILES) $(UILFILES) $(FFILES) $(SHELLFILES) Makefile 
REL=
all: $(LEGO_BIN)/cad_curdir $(LEGO_BIN)/crealg1 $(LEGO_BIN)/crealg5 $(LEGO_BIN)/crealg5sk \
$(LEGO_BIN)/creanlsk $(LEGO_BIN)/cad_crealg1 $(LEGO_BIN)/cad_crealg3 \
$(LEGO_BIN)/cad_crealg5sk $(LEGO_BIN)/crearegime $(LEGO_BIN)/crearg5sk \
$(LEGO_BIN)/crealibreg $(LEGO_BIN)/crealibut $(LEGO_BIN)/crealibut_reg \
$(LEGO_BIN)/legohelp $(LEGO_BIN)/lg3.inp $(LEGO_BIN)/librmake.tpl \
$(LEGO_BIN)/makelibr $(LEGO_BIN)/maketask $(LEGO_BIN)/cad_maketask \
$(LEGO_BIN)/cad_crealibrut \
$(LEGO_BIN)/cad_crealibut $(LEGO_BIN)/cad_crealibreg \
$(LEGO_BIN)/cad_environment $(LEGO_BIN)/creamrg5sk $(LEGO_BIN)/crealibut_mreg \
$(LEGO_BIN)/fortof $(LEGO_BIN)/testerr $(LEGO_BIN)/cad_f012lis \
$(LEGO_BIN)/lg3debug $(LEGO_BIN)/lg5debug $(LEGO_BIN)/cad_lisc2lis \
$(LEGO_BIN)/cad_lism2lis $(LEGO_BIN)/makelibr_pgnouse $(LEGO_BIN)/testforaus \
$(LEGO_BIN)/cad_marcotask $(LEGO_BIN)/cad_testenv

$(LEGO_BIN)/cad_curdir: cad_curdir.sh
	cp cad_curdir.sh cad_curdir ;chmod a+wx cad_curdir ;mv cad_curdir $@
$(LEGO_BIN)/crealg1: crealg1.sh
	cp crealg1.sh crealg1 ;chmod a+wx crealg1 ;mv crealg1 $@
$(LEGO_BIN)/cad_crealg1: cad_crealg1.sh
	cp  cad_crealg1.sh cad_crealg1 ;chmod a+wx cad_crealg1 ; mv cad_crealg1 $@
$(LEGO_BIN)/crealg5:crealg5.sh
	cp crealg5.sh crealg5 ; chmod a+wx crealg5 ; mv crealg5 $@
$(LEGO_BIN)/crealg5sk:crealg5sk.sh
	cp crealg5sk.sh crealg5sk ;chmod a+wx crealg5sk ;mv crealg5sk $@
$(LEGO_BIN)/creanlsk:creanlsk.sh
	cp creanlsk.sh creanlsk ;chmod a+wx creanlsk; mv creanlsk $@
$(LEGO_BIN)/crearegime:crearegime.sh
	cp crearegime.sh crearegime ; chmod a+wx crearegime ; mv crearegime $@
$(LEGO_BIN)/cad_crealg3   :cad_crealg3.sh
	cp cad_crealg3.sh cad_crealg3 ; chmod a+wx cad_crealg3; mv cad_crealg3 $@
$(LEGO_BIN)/cad_crealg5sk   :cad_crealg5sk.sh
	cp cad_crealg5sk.sh cad_crealg5sk ; chmod a+wx cad_crealg5sk; mv cad_crealg5sk $@ 
$(LEGO_BIN)/crearg5sk    :crearg5sk.sh
	cp crearg5sk.sh cad_crearg5sk ; chmod a+wx cad_crearg5sk ; mv cad_crearg5sk $@
$(LEGO_BIN)/crealibreg   :crealibreg.sh
	cp crealibreg.sh crealibreg ; chmod a+wx crealibreg ; mv crealibreg $@
$(LEGO_BIN)/crealibut   :crealibut.sh
	cp crealibut.sh crealibut ;chmod a+wx crealibut ; mv crealibut $@
$(LEGO_BIN)/crealibut_reg  :crealibut_reg.sh
	cp crealibut_reg.sh crealibut_reg ; chmod a+wx crealibut_reg ; mv crealibut_reg $@
$(LEGO_BIN)/legohelp   :legohelp.sh
	cp legohelp.sh legohelp ;chmod a+wx legohelp ; mv legohelp $@
$(LEGO_BIN)/lg3.inp  :lg3.inp.sh
	cp lg3.inp.sh lg3.inp ; chmod a+wx lg3.inp ; mv lg3.inp $@
$(LEGO_BIN)/librmake.tpl :librmake.tpl.sh
	cp librmake.tpl.sh librmake.tpl ; chmod a+wx librmake.tpl; mv librmake.tpl $@
$(LEGO_BIN)/makelibr     :makelibr.sh
	cp makelibr.sh makelibr ; chmod a+wx makelibr ; mv makelibr $@
$(LEGO_BIN)/maketask     :maketask.sh
	cp maketask.sh maketask ; chmod a+wx maketask ;mv maketask $@
$(LEGO_BIN)/cad_maketask     :cad_maketask.sh
	cp cad_maketask.sh cad_maketask ; chmod a+wx cad_maketask ; mv cad_maketask $@
$(LEGO_BIN)/cad_crealibrut    :cad_crealibrut.sh
	cp cad_crealibrut.sh cad_crealibrut ;chmod a+wx cad_crealibrut ; mv cad_crealibrut $@
$(LEGO_BIN)/cad_crealibut    :cad_crealibut.sh
	cp cad_crealibut.sh cad_crealibut ; chmod a+wx cad_crealibut; mv cad_crealibut $@
$(LEGO_BIN)/cad_crealibreg    :cad_crealibreg.sh
	cp cad_crealibreg.sh cad_crealibreg  ; chmod a+wx cad_crealibreg ; mv cad_crealibreg $@
$(LEGO_BIN)/cad_environment    :cad_environment.sh
	cp cad_environment.sh cad_environment ;chmod a+wx cad_environment; mv cad_environment $@
$(LEGO_BIN)/fortof       :fortof.sh
	cp fortof.sh fortof ;chmod a+wx fortof ; mv fortof $@
$(LEGO_BIN)/testerr       :testerr.sh
	cp testerr.sh testerr ;chmod a+wx testerr ; mv testerr $@
$(LEGO_BIN)/creamrg5sk    :creamrg5sk.sh
	cp creamrg5sk.sh creamrg5sk ;chmod a+wx creamrg5sk ; mv creamrg5sk $@
$(LEGO_BIN)/crealibut_mreg    :crealibut_mreg.sh
	cp crealibut_mreg.sh crealibut_mreg ;chmod a+wx crealibut_mreg ; mv crealibut_mreg $@
$(LEGO_BIN)/cad_f012lis    :cad_f012lis.sh
	cp cad_f012lis.sh cad_f012lis ;chmod a+wx cad_f012lis ; mv cad_f012lis $@
$(LEGO_BIN)/cad_lism2lis    :cad_lism2lis.sh
	cp cad_lism2lis.sh cad_lism2lis ;chmod a+wx cad_lism2lis ; mv cad_lism2lis $@
$(LEGO_BIN)/cad_lisc2lis    :cad_lisc2lis.sh
	cp cad_lisc2lis.sh cad_lisc2lis ;chmod a+wx cad_lisc2lis ; mv cad_lisc2lis $@
$(LEGO_BIN)/makelibr_pgnouse     :makelibr_pgnouse.sh
	cp makelibr_pgnouse.sh makelibr_pgnouse ; chmod a+wx makelibr_pgnouse ; mv makelibr_pgnouse $@
$(LEGO_BIN)/lg3debug     :lg3debug.sh
	cp lg3debug.sh lg3debug ; chmod a+wx lg3debug ; mv lg3debug $@
$(LEGO_BIN)/lg5debug     :lg5debug.sh
	cp lg5debug.sh lg5debug ; chmod a+wx lg5debug ; mv lg5debug $@
$(LEGO_BIN)/testforaus     :testforaus.sh
	cp testforaus.sh testforaus ;chmod a+wx testforaus ; mv testforaus $@
$(LEGO_BIN)/cad_marcotask     :cad_marcotask.sh
	cp cad_marcotask.sh cad_marcotask ;chmod a+wx cad_marcotask ; mv cad_marcotask $@
$(LEGO_BIN)/cad_testenv     :cad_testenv.sh
	cp cad_testenv.sh cad_testenv ;chmod a+wx cad_testenv ; mv cad_testenv $@
              
