# ******* Telelogic expanded section *******
# ... (i tuoi commenti iniziali rimangono invariati) ...

# Aggiunto .PHONY per i target che non rappresentano file reali.
.PHONY: all clean force_version_h docker docker_slim docker-push docker_slim-push help

# Il target 'all' è il primo, quindi è il default.
all: version.h # Assicuriamoci che version.h sia controllato/generato prima di compilare
	@echo "--- Building all subprojects ---"
	cd ./kprocedure; $(MAKE) -f Makefile.mk
	cd ./kutil; $(MAKE) -f Makefile.mk
	cd ./Alg_mmi/AlgLib; $(MAKE) -f Makefile.mk
	cd ./Alg_mmi; $(MAKE) -f Makefile.mk
	cd ./Alg_rt; $(MAKE) -f Makefile.mk
	cd ./legocad/lego_big; $(MAKE) -f Makefile.mk
	cd ./legocad; $(MAKE) -f Makefile.mk
#	cd ./scada; $(MAKE) -f Makefile.mk
	cd ./util97; $(MAKE) -f Makefile.mk
	cd ./Alg_legopc; $(MAKE) -f Makefile.mk
	cd ./util2007; $(MAKE) -f Makefile.mk
	cd ./docker; $(MAKE) -f Makefile.mk   # solo i lanciatori lgdock*, non l'immagine
#	cd ./docker_root; $(MAKE) -f Makefile.mk

# --- Gestione di version.h ---

VERSION_H = version.h
VERSION_H_TMP = $(VERSION_H).tmp

# Definizioni delle variabili Git. Vengono eseguite ogni volta che make viene avviato.
GIT_VERSION := $(shell git describe --tags --always --long --dirty)
GIT_COMMIT_COUNT := $(shell git rev-list --count HEAD)
BUILD_DATE := $(shell date +%Y%m%d)

# Regola per generare version.h.
# Questa regola dipende da 'force_version_h', che è un target .PHONY.
# Questo forza l'esecuzione dei comandi *sempre*.
# Tuttavia, il file version.h verrà aggiornato solo se il suo contenuto cambia.
version.h: force_version_h
	@echo "--- Checking/Generating $(VERSION_H) ---"
	@echo "#define GIT_VERSION_STRING \"$(GIT_VERSION)\"" > $(VERSION_H_TMP)
	@echo "#define BUILD_NUMBER $(GIT_COMMIT_COUNT)" >> $(VERSION_H_TMP)
	@echo "#define BUILD_DATE_STRING \"$(BUILD_DATE)\"" >> $(VERSION_H_TMP)
	# Confronta il nuovo file con quello vecchio. Aggiorna solo se sono diversi.
	@if ! cmp -s $(VERSION_H_TMP) $(VERSION_H); then \
		echo "Generated $(VERSION_H) with version $(GIT_VERSION)"; \
		mv $(VERSION_H_TMP) $(VERSION_H); \
	else \
		echo "$(VERSION_H) is already up to date."; \
		rm $(VERSION_H_TMP); \
	fi

# Target PHONY per forzare l'esecuzione della regola di version.h
# Può essere usato anche manualmente: 'make force_version_h'
force_version_h:
	@# Questo target non fa nulla, serve solo come dipendenza phony.

# Target di pulizia migliorato
clean:
	@echo "--- Cleaning project ---"
	rm -f $(VERSION_H) $(VERSION_H_TMP)
	find . -type f -name "*.o" -exec rm -f {} \;
	find . -type f -name "*.a" -exec rm -f {} \;
	@echo "--- Clean finished ---"

# --- Target Docker ---
#  Due immagini con lo stesso ambiente dentro: la completa, come e' sempre
#  stata, e la snella. Si scelgono con due target distinti perche' non sono
#  l'una il rimpiazzo dell'altra - vedi docker/Dockerfile_LegoPST_slim per
#  cosa cambia e perche'.
docker:
	cd ./docker && ./BuildImage -y

docker_slim:
	cd ./docker && ./BuildImage -y --slim

docker-push:
	cd ./docker && ./BuildImage -y --push

docker_slim-push:
	cd ./docker && ./BuildImage -y --slim --push

# --- Help ---
help:
	@echo ""
	@echo "LegoPST - Targets disponibili:"
	@echo ""
	@echo "  all              Compila tutti i sottomoduli in sequenza (default)"
	@echo "                     kprocedure -> kutil -> Alg_mmi/AlgLib -> Alg_mmi"
	@echo "                     -> Alg_rt -> legocad/lego_big -> legocad"
	@echo "                     -> util97 -> Alg_legopc -> util2007 -> docker"
	@echo "                   Di docker/ installa solo i lanciatori (lgdock,"
	@echo "                   lgdock_socat, lgdock_multi): l'immagine Docker NON"
	@echo "                   viene costruita, si chiede con il target 'docker'"
	@echo ""
	@echo "  clean            Rimuove tutti i file oggetto (*.o), le librerie (*.a)"
	@echo "                   e il file version.h generato"
	@echo ""
	@echo "  version.h        Genera (o aggiorna) version.h con:"
	@echo "                     GIT_VERSION_STRING  - stringa di versione da 'git describe'"
	@echo "                     BUILD_NUMBER        - numero di commit da HEAD"
	@echo "                     BUILD_DATE_STRING   - data di build (YYYYMMDD)"
	@echo ""
	@echo "  force_version_h  Forza il ricalcolo di version.h alla prossima build"
	@echo ""
	@echo "  docker           Costruisce l'immagine Docker COMPLETA"
	@echo "                   aguagliardi/legopst_multi:2.0 (BuildImage -y)"
	@echo ""
	@echo "  docker_slim      Costruisce la variante SNELLA, stesso ambiente"
	@echo "                   aguagliardi/legopst_slim:2.0 (BuildImage -y --slim)"
	@echo "                   Senza le dipendenze deboli, senza gimp e senza il"
	@echo "                   .git del repository. Vedi docker/Dockerfile_LegoPST_slim"
	@echo ""
	@echo "  docker-push      Costruisce e pubblica la COMPLETA su Docker Hub"
	@echo "                   (esegue docker/BuildImage -y --push)"
	@echo ""
	@echo "  docker_slim-push Come sopra, ma la variante snella"
	@echo ""
	@echo "  help             Mostra questo messaggio"
	@echo ""
	@echo "Utilizzo: make -f Makefile.mk [target]"
	@echo ""

$(info Makefile.mk: Startup - F_FLAGS is currently set to = $(F_FLAGS))
$(info -------->  Makefile.mk processing complete!)