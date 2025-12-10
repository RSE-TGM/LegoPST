# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7"
LEGO_BIN=../bin
LEGO_LIB=../lib

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -fpermissive -D_BSD -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
#VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C -DOSF1
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
#------------------------ Fortran compiler (gfortran)
FC=gfortran
F_FLAGS=-fno-second-underscore -g -fno-automatic

#
#	Makefile Header:               Makefile.mk
#       Subsystem:              120
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Wed May 12 14:33:10 2004 %
# Makefile per la costruzione dei due eseguibili di LEGOGRAF riguardanti
# la gestione delle tavole del vapore.
#
#
#CCM_MAKE_SERIAL=cc f77 ar ranlib /lib/cpp $(LEGO_BIN)/creatav
.SUFFIXES:  .pf .o .f .c .sh .h .a
#C_FLAGS=-fpermissive
FFLAGS=$(F_FLAGS)
CFLAGS=$(C_FLAGS) -D$(OS) 
TAVOLE=../bin
#
#
##all:  $(TAVOLE)/TAVOLE.DAT $(LEGO_BIN)/initav $(LEGO_LIB)/vapo.a $(LEGO_BIN)/provatav $(LEGO_BIN)/killtav $(LEGO_BIN)/killshrmem
all:  $(TAVOLE)/TAVOLE.DAT $(LEGO_BIN)/initav $(LEGO_LIB)/vapo.a $(LEGO_BIN)/killtav $(LEGO_BIN)/killshrmem $(LEGO_BIN)/creatav $(LEGO_BIN)/provatav
#
# Initav inizializza l'area shared contenente i dati.
#
INITAV_C  = initav.c ism01.c read_tav.c
INITAV_C_OBJ   = initav.o ism01.o read_tav.o
#
# Regola per generare TAVOLE.DAT
$(TAVOLE)/TAVOLE.DAT: $(LEGO_BIN)/creatav
	export TAVOLE; TAVOLE=../bin ; $(LEGO_BIN)/creatav
	chmod ugo+r $(TAVOLE)/TAVOLE.DAT

# Regola per compilare creatav (corretto: dipende dal sorgente, non da se stesso)
$(LEGO_BIN)/creatav: creatav.f
	$(FC) $(FFLAGS) creatav.f -o $(LEGO_BIN)/creatav

$(LEGO_BIN)/initav : $(INITAV_C_OBJ)  
	$(CC) $(INITAV_C_OBJ) -o $(LEGO_BIN)/initav
#
VAPO_C = calsr.c cerca.c conlis.c droph.c drsev.c ism01.c newt.c table.c trova.c diagn_io.c read_tav.c phsat_new.c
VAPO_C_OBJ = calsr.o cerca.o conlis.o droph.o drsev.o ism01.o newt.o table.o trova.o diagn_io.o read_tav.o phsat_new.o
VAPO_FORTRAN = initsm.f vapo1.f diagn.f phsat.f
VAPO_FORTRAN_OBJ = initsm.o vapo1.o diagn.o phsat.o
#
#Inserito il flag s (in ar) per hash-table su aix, in OSF1 hash viene fatta
#di default con il comando ar
$(LEGO_LIB)/vapo.a: $(VAPO_C_OBJ) $(VAPO_FORTRAN_OBJ) 
	ar -crsv $(LEGO_LIB)/vapo.a $(VAPO_C_OBJ) $(VAPO_FORTRAN_OBJ)
#	ranlib $(LEGO_LIB)/vapo.a 
#
$(LEGO_BIN)/provatav: provatav.f $(VAPO_C_OBJ) $(VAPO_FORTRAN_OBJ) 
	$(FC) $(FFLAGS) provatav.f $(VAPO_C_OBJ) $(VAPO_FORTRAN_OBJ) -lc -o $(LEGO_BIN)/provatav
#
# killtav libera l'area shared dopo l'uso o permette di ricaricare le tavole
# usando initav, killshrmem fa lo stesso con l'area condivisa tra tasks.
#
$(LEGO_BIN)/killtav: killtav.c
	$(CC)  killtav.c  -o  $@
#
$(LEGO_BIN)/killshrmem: killshrmem.c
	$(CC)  killshrmem.c -o $@
#
#  nuova regola per precompilare un .pf
#
.pf.o:
	/lib/cpp  $(CPPFLAGS)  -D$(OS)   $< >> $*.f
	-$(FC) -c $(FFLAGS) $*.f
#	rm -f $*.f

# Regola per pulire
clean:
	rm -f *.o $(LEGO_BIN)/creatav $(LEGO_BIN)/initav $(LEGO_BIN)/killtav $(LEGO_BIN)/killshrmem $(LEGO_BIN)/provatav $(LEGO_LIB)/vapo.a $(TAVOLE)/TAVOLE.DAT
