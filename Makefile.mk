# ******* Telelogic expanded section *******
# ... (i tuoi commenti iniziali rimangono invariati) ...

# Aggiunto .PHONY per i target che non rappresentano file reali.
.PHONY: all clean force_version_h docker docker-push controlla_profilo help

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
	cd ./util97; $(MAKE) -f Makefile.mk
	cd ./Alg_legopc; $(MAKE) -f Makefile.mk
	cd ./util2007; $(MAKE) -f Makefile.mk
	cd ./docker; $(MAKE) -f Makefile.mk   # solo i lanciatori lgdock*, non l'immagine
#	cd ./docker_root; $(MAKE) -f Makefile.mk

# --- Gestione di version.h ---

VERSION_H = version.h
VERSION_H_TMP = $(VERSION_H).tmp

# Definizioni delle variabili Git. Vengono eseguite ogni volta che make viene avviato.
#  Solo i tag di rilascio (v2026-2.0, v2026.1...): i segnalibri di lavoro
#  (prima-fase4, demo-2.0...) non devono dare il nome alla versione. Vedi
#  docs/BUILD.md, "Versioni e rilasci".
GIT_VERSION := $(shell git describe --tags --match 'v[0-9]*' --always --long --dirty)
GIT_COMMIT_COUNT := $(shell git rev-list --count HEAD)
BUILD_DATE := $(shell date +%Y%m%d)
#  Il numero di versione del progetto, versionato nel file VERSION: e' il
#  ripiego quando git non c'e' (vedi la nota sulla regola version.h).
PROJECT_VERSION := $(shell cat VERSION 2>/dev/null | tr -d '[:space:]')
#  L'immagine Docker: le cifre dopo il trattino di VERSION (2026-2.0 -> :2.0)
DOCKER_IMAGE := aguagliardi/legopst:$(shell sed -n 's/^[0-9][0-9][0-9][0-9]-//p' VERSION 2>/dev/null | tr -d '[:space:]')

# Regola per generare version.h.
# Questa regola dipende da 'force_version_h', che è un target .PHONY.
# Questo forza l'esecuzione dei comandi *sempre*.
# Tuttavia, il file version.h verrà aggiornato solo se il suo contenuto cambia.
#  Dove git non c'e' - dentro un'immagine Docker, che non si porta il .git, in
#  un bundle FMU, in un tarball dei sorgenti - le due variabili qui sopra
#  escono VUOTE, e la regola scriveva "#define BUILD_NUMBER" senza valore, con
#  GIT_VERSION_STRING a stringa vuota. Non e' un dettaglio di forma: quel file
#  lo leggono lghmi e legopc per mostrare la versione di LegoPST, e legopc lo
#  RIGENERA da se' con make se manca. Il risultato era una versione in bianco.
#  Quindi: se git non ha detto niente e un version.h c'e' gia', si tiene quello
#  (l'ha scritto chi ha costruito l'immagine, ed e' l'unica fonte di verita'
#  rimasta la' dentro). Se non c'e' nemmeno quello si scrive la versione del
#  file VERSION, che e' versionato, con BUILD_NUMBER 0.
version.h: force_version_h
	@echo "--- Checking/Generating $(VERSION_H) ---"
	@if [ -z "$(GIT_COMMIT_COUNT)" ] && [ -f $(VERSION_H) ]; then \
		echo "$(VERSION_H): git non disponibile qui, tengo quello che c'e'."; \
	else \
		echo "#define GIT_VERSION_STRING \"$(if $(GIT_VERSION),$(GIT_VERSION),v$(PROJECT_VERSION)-nogit)\"" > $(VERSION_H_TMP); \
		echo "#define BUILD_NUMBER $(if $(GIT_COMMIT_COUNT),$(GIT_COMMIT_COUNT),0)" >> $(VERSION_H_TMP); \
		echo "#define BUILD_DATE_STRING \"$(BUILD_DATE)\"" >> $(VERSION_H_TMP); \
		if ! cmp -s $(VERSION_H_TMP) $(VERSION_H); then \
			echo "Generated $(VERSION_H) with version $(if $(GIT_VERSION),$(GIT_VERSION),v$(PROJECT_VERSION)-nogit)"; \
			mv $(VERSION_H_TMP) $(VERSION_H); \
		else \
			echo "$(VERSION_H) is already up to date."; \
			rm $(VERSION_H_TMP); \
		fi; \
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
#  Una sola immagine, aguagliardi/legopst:2.0. Fino al 2026-09-27 erano due
#  (legopst_multi e legopst_slim): vedi l'intestazione di
#  docker/Dockerfile_LegoPST per cosa le distingueva e perche' non valeva la
#  pena tenerle separate.
#
#  L'immagine si costruisce dalla copia SUL DISCO, compresi i file compilati
#  (BuildImage non compila niente). Per questo docker e docker-push fanno
#  prima un "all": dopo un clean all'immagine sarebbero mancati i .o che
#  LegoPC collega nelle task e tutte le librerie .a, e senza un build recente
#  si sarebbe portata un version.h vecchio. "all" si lancia come comando e non
#  come dipendenza, cosi' con make -j finisce PRIMA di BuildImage. Se il build
#  fallisce, l'immagine non si costruisce. Per l'immagine dello stato attuale
#  senza ricompilare: cd docker && ./BuildImage -y.
docker: controlla_profilo
	$(MAKE) -f Makefile.mk all
	cd ./docker && ./BuildImage -y

docker-push: controlla_profilo
	$(MAKE) -f Makefile.mk all
	cd ./docker && ./BuildImage -y --push

#  "all" ha bisogno del profilo: senza, gli include path restano vuoti
#  (-I -I) e il build fallisce con errori che non dicono il perche'. E il
#  profilo deve essere QUESTO albero: con LEGOROOT di un'altra copia si
#  compilerebbe contro le sue librerie.
controlla_profilo:
	@if [ -z "$$LEGOROOT" ]; then \
		echo "ERRORE: profilo LegoPST non caricato. Prima:  source .profile_legoroot"; \
		exit 1; \
	fi
	@if [ "$$(cd "$$LEGOROOT" 2>/dev/null && pwd -P)" != "$$(pwd -P)" ]; then \
		echo "ERRORE: LEGOROOT=$$LEGOROOT non e' questo albero ($$(pwd -P))."; \
		echo "        Prima:  source .profile_legoroot"; \
		exit 1; \
	fi

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
	@echo "  docker           Compila (all) e poi costruisce l'immagine Docker"
	@echo "                   $(DOCKER_IMAGE) dalla copia sul disco"
	@echo "                   (docker/BuildImage -y). Senza le dipendenze"
	@echo "                   deboli, senza gimp e senza il .git del repository:"
	@echo "                   vedi docker/Dockerfile_LegoPST"
	@echo ""
	@echo "  docker-push      Compila (all), costruisce e pubblica l'immagine"
	@echo "                   su Docker Hub (docker/BuildImage -y --push)."
	@echo "                   docker e docker-push vogliono il profilo caricato"
	@echo ""
	@echo "  help             Mostra questo messaggio"
	@echo ""
	@echo "Utilizzo: make -f Makefile.mk [target]"
	@echo ""

$(info Makefile.mk: Startup - F_FLAGS is currently set to = $(F_FLAGS))
$(info -------->  Makefile.mk processing complete!)