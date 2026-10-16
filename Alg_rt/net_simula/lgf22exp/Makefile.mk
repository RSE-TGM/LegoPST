#
#   lgf22exp - esporta in testo (CSV) le variabili registrate in f22circ.dat.
#   Non usa librerie: legge il file con le strutture di sim_types.h e
#   f22_circ.h, le stesse con cui lo scrive net_prepf22.
#
OS=LINUX
C_FLAGS=-g -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -I.
CFLAGS = -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE) -Wall

all:  $(LEGORT_BIN)/lgf22exp

$(LEGORT_BIN)/lgf22exp: lgf22exp.o
	$(CC) -o $(LEGORT_BIN)/lgf22exp lgf22exp.o

lgf22exp.o: lgf22exp.c $(LEGORT_INCLUDE)/f22_circ.h $(LEGORT_INCLUDE)/sim_types.h

clean:
	rm -f lgf22exp.o
