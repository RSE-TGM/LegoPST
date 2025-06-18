# Makefile per installare la libreria libgdbm.so.2 da un vecchio pacchetto RPM.
#
# OBIETTIVI PRINCIPALI:
#   make           - Scarica, estrae e installa la libreria. (Equivalente a 'make install')
#   make install   - Esegue l'installazione completa.
#   make uninstall - Rimuove la libreria e il link simbolico dal sistema.
#   make clean     - Pulisce i file scaricati ed estratti localmente.
#

# --- Variabili ---
# Definiamo nomi e percorsi per renderli facili da modificare.
RPM_URL         := "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"
RPM_FILE        := gdbm-1.8.0-33.fc12.x86_64.rpm
EXTRACTED_LIB   := usr/lib64/libgdbm.so.2.0.0
SYSTEM_LIB_PATH := /usr/lib64
SYSTEM_LIB_FILE := $(SYSTEM_LIB_PATH)/libgdbm.so.2.0.0
SYSTEM_LINK     := $(SYSTEM_LIB_PATH)/libgdbm.so.2

# --- Target Principali ---

# Il target di default: 'make' eseguira'  'make install'.
all: install

# Definiamo i target che non creano un file con lo stesso nome.
.PHONY: all install uninstall clean

# Target per l'installazione. Dipende dal fatto che la libreria sia stata estratta.
install: $(EXTRACTED_LIB)
	@echo "--> 3. Installazione della libreria e del link simbolico..."
	sudo cp $(EXTRACTED_LIB) $(SYSTEM_LIB_PATH)/
	sudo ln -sf $(SYSTEM_LIB_FILE) $(SYSTEM_LINK)
	@echo "--> Aggiornamento della cache del linker..."
	sudo ldconfig
	@echo "--> Installazione completata con successo."

# Target per estrarre la libreria. Dipende dal file RPM scaricato.
$(EXTRACTED_LIB): $(RPM_FILE)
	@echo "--> 2. Estrazione dei contenuti dal pacchetto RPM..."
	rpm2cpio $(RPM_FILE) | cpio -idmv

# Target per scaricare il file RPM. VerrÃ  eseguito solo se il file non esiste.
$(RPM_FILE):
	@echo "--> 1. Download del pacchetto RPM..."
	curl -fLO $(RPM_URL)

# Target per disinstallare la libreria dal sistema.
uninstall:
	@echo "--> Disinstallazione della libreria e del link simbolico..."
	sudo rm -f $(SYSTEM_LINK)
	sudo rm -f $(SYSTEM_LIB_FILE)
	@echo "--> Aggiornamento della cache del linker..."
	sudo ldconfig
	@echo "--> Disinstallazione completata."

# Target per pulire la directory locale dai file temporanei.
clean:
	@echo "--> Pulizia dei file scaricati ed estratti..."
	rm -f $(RPM_FILE)
	rm -rf usr
