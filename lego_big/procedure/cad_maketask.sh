#!/bin/ksh
#
#  Script:                      cad_maketask.sh
#  Subsystem:           1
#  %version:            17 %
#  Description:
#  %created_by:         lomgr %
#  %date_created:       Thu Apr  9 16:17:58 2009 %

# Makefile per l'attivita LG1 del codice LEGO.
# DECFFLAGS= -fpe3
# IBMFFLAGS= -O -qextname

FFLAGS= $(F_FLAGS)

#######################################
# Sezione   L G 5 
#######################################
#
# Nella variabile di envir. CAD_LIB_MODULI sono inseriti i nomi delle 
# librerie dei moduli di processo e/o di regolazione a seconda che si
# stie generando un modello di processo, di regolazione o misto.
#
lg5: proc/first proc/lg5

proc/lg5: proc/ltm.o proc/foraus.o 
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_lg5.o proc/ltm.o \
        $(CAD_LIB_MODULI) proc/foraus.o $(CAD_LEGO_LIB)/clegolib.a \
        $(LEGO_LIB)/vapo.a  $(ALG_FPE_OBJ) -o  proc/lg5
	rm -f lg5.o

#######################################
# Sezione  S K E D   R G 5 
#######################################
rg5sk: proc/first proc/rg5sk 

proc/rg5sk : proc/ltm.o proc/foraus.o 
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_rg5sk.o proc/ltm.o \
        $(CAD_LIB_MODULI) proc/foraus.o $(CAD_LEGO_LIB)/clegolib.a \
        $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
        $(LEGO_LIB)/vapo.a  $(ALG_FPE_OBJ) -o  proc/lg5sk
	rm -f rg5sk.o
	cp $(LEGO)/procedure/S01.tpl S01
	chmod a+w S01
	net_compi


#######################################
# Sezione  S K E D   L G 5 
#######################################
lg5sk: proc/first proc/lg5sk variabili.rtf

proc/lg5sk : proc/ltm.o proc/foraus.o 
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_lg5sk.o proc/ltm.o \
        $(CAD_LIB_MODULI)  proc/foraus.o $(CAD_LEGO_LIB)/clegolib.a \
        $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a \
        $(LEGO_LIB)/vapo.a  $(ALG_FPE_OBJ) -o  proc/lg5sk
	rm -f lg5sk.o

#######################################
# Sezione  variabili.rtf
#######################################
variabili.rtf: S01
	net_compi


#######################################
# Sezione  S01
#######################################
S01: f01.dat
	cat $(LEGO)/procedure/S01.tpl | sed  "s/LEGOCAD/`cad_curdir`/g" \
	| sed  "s/ALG_TASK_TYPE_FLAG/P/g" > S01
	chmod a+w S01

#######################################
# Sezione regime
#######################################
proc/f04.dat: lg1 proc/first proc/lg3
	rm -f proc/f11.dat
	rm -f to_calcstaz
	rm -f proc/ok_staz
	if [ -s .user_setting ] ;\
	then proc/lg3 `grep tolerance .user_setting | cut -f2 -d' '` > lg3.out ;\
	else proc/lg3 0.00001 > lg3.out ;\
	fi
	touch proc/ok_staz

#######################################
# Sezione lg3
#######################################

OGGETTI_LG3=$(LEGO)/sorglego/main/main_lg3.o proc/ltm.o \
$(CAD_LIB_MODULI) proc/foraus.o  $(CAD_LEGO_LIB)/clegolib.a \
$(LEGO_LIB)/vapo.a $(LEGOCAD_LIB)/liblegocad.a

proc/lg3: $(OGGETTI_LG3)
	$(FC) $(FFLAGS) $(OGGETTI_LG3) $(ALG_FPE_OBJ) -o proc/lg3
	rm -f lg3.o

#######################################
# Sezione F14
#######################################
lg1:  proc/first proc/f03.dat f14.dat
#	touch lg1_done.out Commentata l'aggiunta di Guag. Attenzione al funzionamento di LegoPC
f14.dat: f01.dat
	 if test -f f14.dat;\
	 then\
	    edi14_exe > edi14.lis;\
	    mv f14.out f14.dat;\
	    rm -f proc/f14.dat;\
	 else\
	    mv proc/f14.dat f14.dat;\
	 fi

#######################################
# Sezione f03
#######################################
proc/f03.dat: proc/f02.dat proc/lg2
	proc/lg2

#######################################
# Sezione lg2
#######################################

OGGETTI_LG2=$(LEGO)/sorglego/main/main_lg2.o  \
proc/ltm.o \
$(CAD_LIB_MODULI) proc/foraus.o $(CAD_LEGO_LIB)/clegolib.a \
$(LEGO_LIB)/vapo.a 

proc/lg2: $(OGGETTI_LG2)
	$(FC) $(FFLAGS) $(OGGETTI_LG2) -o proc/lg2
	rm -f lg2.o

#######################################
#  Creazione librerie utente
#######################################
lib: $(CAD_LIB_MODULI)
../libut/modulilib.a: ../libut/lista_moduli.dat
	cd ../libut  ; cad_crealibut
../libut_reg/schemi_reg.a: ../libut_reg/lista_schemi.dat
	cd ../libut_reg ; crealibut_reg
../libut/lista_moduli.dat:
	@echo "Cannot find lista_moduli.dat"
../libut_reg/lista_schemi.dat:
	@echo "Cannot find lista_schemi.dat"

#######################################
# Sezione librerie
#######################################
librerie: proc/first proc/ltm.o proc/foraus.o

proc/ltm.o: proc/ltm.f
	$(FC) -c $(FFLAGS) proc/ltm.f
	mv ltm.o proc/ltm.o

proc/ltm.f: f01.dat
	$(LEGO_BIN)/clg1a_exe $(LG1_FLAGS) $(CAD_LISTA_SCHEMI) > lg1a.out

proc/f02.dat: f01.dat
	$(LEGO_BIN)/clg1a_exe $(LG1_FLAGS) $(CAD_LISTA_SCHEMI) > lg1a.out

proc/foraus.o: foraus.f
	$(FC) -c $(FFLAGS) foraus.f
	mv foraus.o proc/foraus.o

foraus.f: foraus.for forausbase.f
	aggforaus_exe

foraus.for:
	touch foraus.for

forausbase.f: ../../legocad/libut/forausbase.f
#  La riga "file ../../legocad/libut/forausbase.f" che stava qui era solo
#  diagnostica - stampava il tipo del sorgente - e nessuno ne leggeva l'uscita:
#  il lavoro lo fa il cp qui sotto. Ma make interrompe la ricetta al primo
#  comando fallito, e dove il comando "file" non e' installato (il container
#  LegoPST, per esempio) la costruzione moriva con
#      make: file: No such file or directory
#      make: *** [cad_maketask:175: forausbase.f] Error 127
#  cioe' "comando non trovato", che pero' si legge come se mancasse il sorgente.
#  Era l'unica invocazione di "file" in tutto il repository.
	cp ../../legocad/libut/forausbase.f forausbase.f

proc/first:
	-mkdir proc
	touch proc/first

.f.a:
	$(FC) -c $(FFLAGS) $(?:.o=.f)
	ar crv $@ $(*F).o
	ranlib $@
	rm -f $(*F).o
#######################################
# Sezione legopc
#######################################
#$(TOPF01)f01.dat: $(LGTOPNAME).top
#	$(LG_TOOLS)/pag2f01 $(LGTOPNAME).top > pag.out
