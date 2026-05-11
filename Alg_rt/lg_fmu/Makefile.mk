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
#                               Compila anche LegoCliSINC.so (src/Makefile.mk).
#                               Le lib AlgLib necessarie hanno già -fPIC nei
#                               loro Makefile.mk, quindi il link della .so
#                               funziona senza modifiche alla build globale.
#
LEGORT_BIN=../bin

all: $(LEGORT_BIN)/dolgfmu \
     $(LEGORT_BIN)/run_fmu \
     $(LEGORT_BIN)/net_startup_headless \
     $(LEGORT_BIN)/test_fmu_docker \
     src/LegoCliSINC.so

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

src/LegoCliSINC.so:
	$(MAKE) -C src -f Makefile.mk so

clean:
	rm -f $(LEGORT_BIN)/dolgfmu \
	      $(LEGORT_BIN)/run_fmu \
	      $(LEGORT_BIN)/net_startup_headless \
	      $(LEGORT_BIN)/test_fmu_docker
	$(MAKE) -C src -f Makefile.mk clean

.PHONY: all clean src/LegoCliSINC.so
