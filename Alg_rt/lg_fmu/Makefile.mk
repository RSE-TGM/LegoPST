#
#       Makefile Header:        Makefile.mk
#       Subsystem:              Alg_rt/lg_fmu (porting FMU LegoCliSINC su Linux)
#       Description:            installa gli script di lg_fmu/scripts/ in
#                               $(LEGORT_BIN) (= Alg_rt/bin), cosi' sono nel
#                               PATH dell'utente come gli altri eseguibili
#                               LegoPST.
#
#                               Lo script viene rinominato senza l'estensione
#                               .sh seguendo la convenzione di
#                               Alg_rt/procedure/Makefile.mk (es. net_startup
#                               viene da net_startup.sh).
#
#                               I sorgenti C/headers (src/, tools/, tests/,
#                               include/) e il builder bundle/build.sh hanno
#                               i loro Makefile.mk locali (build manuale,
#                               non agganciati qui per evitare di forzare
#                               -fPIC su tutta AlgLib a ogni build globale).
#
LEGORT_BIN=../bin

all: $(LEGORT_BIN)/dolgfmu \
     $(LEGORT_BIN)/run_fmu \
     $(LEGORT_BIN)/net_startup_headless \
	 $(LEGORT_BIN)/test_fmu_docker

$(LEGORT_BIN)/dolgfmu: scripts/dolgfmu.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/run_fmu: scripts/run_fmu.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/net_startup_headless: scripts/net_startup_headless.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/test_fmu_docker: scripts/test_fmu_docker.sh
	cp $? $@
	chmod 755 $@

clean:
	rm -f $(LEGORT_BIN)/dolgfmu \
	      $(LEGORT_BIN)/run_fmu \
	      $(LEGORT_BIN)/net_startup_headless \
	      $(LEGORT_BIN)/test_fmu_docker

.PHONY: all clean
