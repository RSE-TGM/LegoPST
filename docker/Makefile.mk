# Nome dell'immagine Docker che BuildImage dovrebbe costruire (opzionale, ma utile per pulizia)
# Modifica questo se il tuo script BuildImage costruisce un'immagine con un nome specifico.
# Se BuildImage non produce un target tangibile che Make può tracciare,
# useremo un file "timestamp" per forzare la riesecuzione.
IMAGE_NAME := aguagliardi/legopst:latest
DOCKERFILE := Dockerfile_LegoPST
BUILD_SCRIPT := ./BuildImage
BUILD_SCRIPT_MULTI := ./BuildImage_multiplat

# Dipendenze per la regola di build dell'immagine
#BUILD_DEPENDENCIES := $(DOCKERFILE) lgdock lgdock startDock
BUILD_DEPENDENCIES := $(DOCKERFILE) lgdock lgdock_socat lgdock_multi

# Target principale, spesso chiamato 'all' o il nome dell'eseguibile/immagine principale
# In questo caso, potremmo considerare il risultato di BuildImage come un file "segnaposto"
# per indicare che la build è stata eseguita, oppure l'immagine Docker stessa
# (anche se tracciare le immagini Docker direttamente con Make è meno comune).

# Opzione 1: Target fittizio che esegue sempre BuildImage se le dipendenze cambiano
# Questo è il modo più semplice se BuildImage non crea un file specifico tracciabile da Make.
.PHONY: all build clean check_docker repo_info

all: build repo_info $(LEGORT_BIN)/lgdock $(LEGORT_BIN)/lgdock_socat $(LEGORT_BIN)/lgdock_multi

# Genera repo_info.conf con le coordinate del repository git
repo_info: repo_info.conf

repo_info.conf:
	@echo "Generazione repo_info.conf..."
	@REMOTE_URL=$$(git remote get-url origin 2>/dev/null || echo ""); \
	if [ -n "$$REMOTE_URL" ]; then \
		REPO_HOST=$$(echo "$$REMOTE_URL" | sed -E 's#https?://([^/]+)/.*#\1#; s#git@([^:]+):.*#\1#'); \
		REPO_SLUG=$$(echo "$$REMOTE_URL" | sed -E 's#(https?://[^/]+/|git@[^:]+:)##; s/\.git$$//'); \
		REPO_BRANCH=$$(git symbolic-ref --short HEAD 2>/dev/null || echo "master"); \
	else \
		REPO_HOST="github.com"; \
		REPO_SLUG="RSE-TGM/LegoPST"; \
		REPO_BRANCH="master"; \
	fi; \
	echo "# Generato automaticamente da Makefile.mk - non modificare" > $@; \
	echo "REPO_HOST=$$REPO_HOST" >> $@; \
	echo "REPO_SLUG=$$REPO_SLUG" >> $@; \
	echo "REPO_BRANCH=$$REPO_BRANCH" >> $@; \
	echo "repo_info.conf generato: $$REPO_HOST / $$REPO_SLUG (branch: $$REPO_BRANCH)"

check_docker:
	@if ! command -v docker >/dev/null 2>&1; then \
		echo "---------------------------------------------------------------------"; \
		echo "ERRORE: Docker non sembra essere installato o non è nel PATH."; \
		echo "        Per favore, installa Docker prima di procedere."; \
		echo "---------------------------------------------------------------------"; \
		exit 1; \
	fi
	@echo "Docker trovato."

build: check_docker $(BUILD_DEPENDENCIES)
	@echo "Controllo dipendenze per l'immagine Docker..."
	# La logica per decidere se buildare è ora dentro BuildImage
	$(BUILD_SCRIPT) -y # Passa l'opzione -y se necessario, es: $(BUILD_SCRIPT) -y
	@echo "Script BuildImage completato."
	$(BUILD_SCRIPT_MULTI) -y # Passa l'opzione -y se necessario, es: $(BUILD_SCRIPT) -y
	@echo "Script BuildImage_multi completato."

# Opzione 2: Usare un file "timestamp" per evitare ricompilazioni non necessarie
# se BuildImage è costoso e vuoi essere più preciso.
# Creiamo un file vuoto per marcare l'ultima build andata a buon fine.
# LAST_BUILD_STAMP := .last_docker_build_succeeded

# all: $(LAST_BUILD_STAMP)

# $(LAST_BUILD_STAMP): $(BUILD_DEPENDENCIES) $(BUILD_SCRIPT)
# 	@echo "Uno o più file sorgente per l'immagine Docker o lo script di build sono cambiati."
# 	@echo "Esecuzione di $(BUILD_SCRIPT)..."
# 	$(BUILD_SCRIPT)
# 	@echo "Esecuzione di $(BUILD_SCRIPT) completata."
# 	@touch $(LAST_BUILD_STAMP) # Aggiorna il timestamp se la build ha successo

$(LEGORT_BIN)/lgdock: lgdock.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/lgdock_multi: lgdock_multi.sh
	cp $? $@
	chmod 755 $@

$(LEGORT_BIN)/lgdock_socat: lgdock_socat.sh
	cp $? $@
	chmod 755 $@

# Regola di pulizia (opzionale, ma buona pratica)
clean:
	@echo "Pulizia..."
	@echo "rimuovo l'immagine docker $(IMAGE_NAME)"
	docker rmi $(IMAGE_NAME) || true
	# Rimuovi il file timestamp se usi l'Opzione 2
	# -rm -f $(LAST_BUILD_STAMP)
	@echo "Rimuovo lgdock"
	rm -f $(LEGORT_BIN)/lgdock
	@echo "Rimuovo lgdock_socat"
	rm -f $(LEGORT_BIN)/lgdock_socat
	@echo "Rimuovo lgdock_multi"
	rm -f $(LEGORT_BIN)/lgdock_multi
	@echo "Rimuovo repo_info.conf"
	rm -f repo_info.conf
	@echo "Pulizia completata."

# Nota: L'uso di '@' all'inizio di un comando sopprime la stampa del comando stesso.
# L'uso di '-' all'inizio di un comando (es. -rm) fa sì che Make ignori gli errori
# di quel comando (utile per i comandi di pulizia che potrebbero fallire se
# i file non esistono). '|| true' è un altro modo per ignorare gli errori.
