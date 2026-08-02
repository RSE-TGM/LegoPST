#!/bin/ksh
#
#  Script:                      maketask.sh
#  Subsystem:           1
#  %version:            4 %
#  Description:
#  %created_by:         lomgr %
#  %date_created:       Wed Mar 23 15:40:55 2005 %

# Makefile per l'attivita LG1 del codice LEGO.
# DECFFLAGS= -fpe3
# IBMFFLAGS= -O -qextname

FFLAGS= $(F_FLAGS)

#######################################
# Sezione   L G 5 
#######################################
lg5: proc proc/lg5 

proc/lg5: proc/ltm.o proc/foraus.o proc/f04.dat
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_lg5.o \
        $(LEGO_LIB)/legolib.a proc/ltm.o \
        ../libut/modulilib.a  proc/foraus.o\
        $(LEGO_LIB)/vapo.a  $(ALG_FPE_OBJ) -o  proc/lg5

#######################################
# Sezione  S K E D   M R G 5 
#######################################
mrg5sk: proc proc/mrg5sk

proc/mrg5sk : proc/ltm.o proc/f04.dat
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_rg5sk.o \
        $(LEGO_LIB)/legolib.a proc/ltm.o proc/schemi_reg.a \
        $(AMBRA_LIB) \
        ../libut_reg/libreg/reglib.a \
        $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a\
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
        $(LEGO_LIB)/vapo.a   $(ALG_FPE_OBJ) -o  proc/lg5sk

#######################################
# Sezione  S K E D   R G 5 
#######################################
rg5sk: proc proc/rg5sk

proc/rg5sk : proc/ltm.o proc/foraus.o proc/f04.dat
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_rg5sk.o \
        $(LEGO_LIB)/legolib.a proc/ltm.o ../libut_reg/schemi_reg.a \
        $(AMBRA_LIB) \
        ../libut/modulilib.a  ../libut_reg/libreg/reglib.a proc/foraus.o \
        $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a\
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
        $(LEGO_LIB)/vapo.a   $(ALG_FPE_OBJ) -o  proc/lg5sk

#######################################
# Sezione  S K E D   L G 5 
#######################################
lg5sk: proc proc/lg5sk

proc/lg5sk : proc/ltm.o proc/foraus.o proc/f04.dat
	$(FC) $(FFLAGS) $(LEGO)/sorglego/main/main_lg5sk.o \
        $(LEGO_LIB)/legolib.a proc/ltm.o ../libut_reg/schemi_reg.a \
        ../libut/modulilib.a ../libut_reg/libreg/reglib.a proc/foraus.o \
        $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
        $(LEGO_LIB)/vapo.a   $(ALG_FPE_OBJ) -o  proc/lg5sk

#######################################
# Sezione  S K E D  NON LEGO 
#######################################
nlsk: proc proc/nlsk

CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) -D$(VERSIONE)
LIBUTIL = $(LEGORT_LIB)/libsim.a $(LEGORT_LIB)/libipc.a $(LEGORT_LIB)/libnet.a

SORGENTI = tasknl.c
OGGETTI  = tasknl.o

proc/nlsk : $(OGGETTI) $(LIBUTIL)
	cc -o ./proc/lg5sk $(OGGETTI) \
        $(LIBUTIL) -lm

#######################################
# Sezione regime
#######################################
regime: proc proc/f24.dat

proc/f24.dat: f14.dat proc/lg3b
	rm -f proc/lgerr.out
	proc/lg3b 
	testerr proc/lgerr.out

proc/f04.dat: f14.dat proc/lg3b
	rm -f proc/lgerr.out
	proc/lg3b < $(LEGO_BIN)/lg3.inp > lg3b.out
	testerr proc/lgerr.out

#######################################
# Sezione lg3b
#######################################

OGGETTI_LG3=$(LEGO)/sorglego/main/main_lg3.o \
        $(AMBRA_LIB) \
$(LEGO_LIB)/legolib.a proc/ltm.o  \
../libut/modulilib.a  proc/foraus.o \
$(LEGO_LIB)/vapo.a 

proc/lg3b: $(OGGETTI_LG3) proc/modif04
	$(FC) $(FFLAGS) $(OGGETTI_LG3)  $(ALG_FPE_OBJ) -o proc/lg3b

#######################################
# Creazione f04 da f14.s 
#######################################
OGGETTI_CF04=$(LEGO)/sorglego/main/main_cf04.o \
        $(AMBRA_LIB) \
$(LEGO_LIB)/legolib.a proc/ltm.o  \
../libut/modulilib.a  proc/foraus.o \
$(LEGO_LIB)/vapo.a

proc/modif04:  $(OGGETTI_CF04)
	$(FC) $(FFLAGS) $(OGGETTI_CF04)  $(ALG_FPE_OBJ) -o proc/modif04

#######################################
# Sezione F14
#######################################
lg1:proc f14.dat

f14.dat: proc/f02.dat proc/lg2
	 rm -f proc/lgerr.out
	 proc/lg2
#	 lg2 NON scrive proc/lgerr.out (a differenza di lg3b, usato dai target
#	 f04.dat/f24.dat): segnala gli errori con l'exit code (LGABRT -> exit 1),
#	 che make controlla gia' da se'. Senza il test di esistenza, testerr
#	 falliva SEMPRE qui e bloccava il build anche a calcolo riuscito -- cosa
#	 che si vedeva solo ricostruendo f14.dat, cioe' dopo una modifica alla
#	 topologia. Il controllo resta attivo se il file c'e' davvero.
	 if test -f proc/lgerr.out; then testerr proc/lgerr.out; fi
	 if test -f f14.dat;\
	 then\
	    edi14_exe > edi14.lis;\
	    mv f14.out f14.dat;\
	    rm -f proc/f14.dat;\
	 else\
	    mv proc/f14.dat f14.dat;\
	 fi

#######################################
# Sezione lg2
#######################################

OGGETTI_LG2=$(LEGO)/sorglego/main/main_lg2.o  $(LEGO_LIB)/legolib.a \
        $(AMBRA_LIB) \
proc/ltm.o  ../libut/modulilib.a \
 proc/foraus.o $(LEGO_LIB)/vapo.a 

proc/lg2: $(OGGETTI_LG2)
	$(FC) $(FFLAGS) $(OGGETTI_LG2)  $(ALG_FPE_OBJ) -o proc/lg2

#######################################
# Sezione librerie
#######################################
librerie: proc proc/ltm.o proc/foraus.o

proc/ltm.o: proc/ltm.f 
	$(FC) -c $(FFLAGS) proc/ltm.f
	mv ltm.o proc/ltm.o 

proc/ltm.f: f01.dat 
	rm -f proc/lgerr.out
	lg1a_exe > lg1a.out
	testerr proc/lgerr.out

proc/f02.dat: f01.dat
	rm -f proc/lgerr.out
	lg1a_exe > lg1a.out
	testerr proc/lgerr.out

proc/foraus.o: foraus.f
	$(FC) -c $(FFLAGS) foraus.f
	mv foraus.o proc/foraus.o

foraus.f: foraus.for forausbase.f
	aggforaus_exe

foraus.for:
	touch foraus.for

forausbase.f: ../libut/forausbase.f
	cp ../libut/forausbase.f forausbase.f

proc:
	mkdir proc

.f.a:
	$(FC) -c $(FFLAGS) $(?:.o=.f) 
	ar crv $@ $(*F).o
	ranlib $@ 
	rm -f $(*F).o
#######################################################
#
#       SEZIONE CREAZIONE MAIN_REFRESH_DATI
#
#######################################################

CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(VERSIONE) $(X_INCLUDE)

main_refresh_l: $(LEGO)/sorglego/main/main_refresh.o forausbase.o
	cc -o refresh_dati  $(FC_LIBS) -lsqlite3 $(LEGO)/sorglego/main/main_refresh.o forausbase.o \
    $(LEGORT_LIB)/libsim.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
    $(LEGORT_LIB)/libnet.a  modulilib.a $(LEGO_LIB)/vapo.a  -lm 
	rm forausbase.o

main_refresh_r: $(LEGO)/sorglego/main/main_refresh.o forausbase.o
	cc -o refresh_dati  $(FC_LIBS) -lsqlite3 $(LEGO)/sorglego/main/main_refresh.o forausbase.o \
    $(LEGORT_LIB)/libsim.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
    $(LEGORT_LIB)/libnet.a  schemi_reg.a libreg/reglib.a \
    $(LEGO_LIB)/vapo.a  -lm  
	rm forausbase.o

main_refresh_mr: $(LEGO)/sorglego/main/main_refresh.o 
	cc -o refresh_dati $(FC_LIBS) -lsqlite3  $(LEGO)/sorglego/main/main_refresh.o \
    $(LEGORT_LIB)/libsim.a \
	$(LEGOROOT_LIB)/libutil.a  $(LEGOROOT_LIB)/libRt.a $(LEGORT_LIB)/libsim.a\
    $(LEGORT_LIB)/libnet.a  schemi_reg.a \
    $(LEGOCAD_USER)/legocad/libut_reg/libreg/reglib.a \
    $(LEGO_LIB)/legolib.a $(LEGO_LIB)/vapo.a  -lm 
