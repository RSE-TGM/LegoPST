# ******* Telelogic expanded section *******
# ... (i tuoi commenti iniziali rimangono invariati) ...

# Aggiunto .PHONY per i target che non rappresentano file reali.
.PHONY: all clean force_version_h

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
	cd ./docker; $(MAKE) -f Makefile.mk
	cd ./docker_root; $(MAKE) -f Makefile.mk

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

$(info Makefile.mk: Startup - F_FLAGS is currently set to = $(F_FLAGS))
$(info -------->  Makefile.mk processing complete!)