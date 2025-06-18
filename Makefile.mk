# ******* Telelogic expanded section *******

# make_macros from project "Alg_global-2007A1_RHE4_lomgr
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              146
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Feb 19 16:09:30 2007 %
# make_macros from project "Alg_global-2007A1_RHE4_lomgr
#
#       Makefile Header:               Makefile.mk
#       Subsystem:              146
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Feb 19 16:09:30 2007 %

.PHONY: all clean force_version_h # Aggiunto .PHONY

VERSION_H = version.h
GIT_VERSION := $(shell git describe --tags --always --long)
GIT_COMMIT_COUNT := $(shell git rev-list --count HEAD)
BUILD_DATE := $(shell date +%Y%m%d)

# Esempio di utizzo di version.h in un progetto C
# Per includere version.h nei tuoi file C, usa:
# #include "version.h"
# my_program: main.c $(VERSION_H)
# 	gcc main.c -o my_program -I.


# 'all' è ora il primo target esplicito
all: $(VERSION_H) # Dipende da version.h
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

# La regola per version.h.
# Viene eseguita se version.h non esiste, o se 'force_version_h' è invocato.

# Per forzare la rigenerazione di version.h, puoi usare il target 'force_version_h'.
# Trova il ref attuale (branch o commit diretto)
GIT_REF := $(shell if [ -f .git/HEAD ]; then ref=$$(cat .git/HEAD | sed 's/^ref: //'); \
  if [ -n "$$ref" ] && [ -f .git/$$ref ]; then echo .git/$$ref; else echo .git/HEAD; fi; else echo .git/HEAD; fi)

$(VERSION_H): .git/HEAD $(GIT_REF)
	@echo "--- Generating $(VERSION_H) ---"
	@echo "#define GIT_VERSION_STRING \"$(GIT_VERSION)\"" > $(VERSION_H)
	@echo "#define BUILD_NUMBER $(GIT_COMMIT_COUNT)" >> $(VERSION_H)
	@echo "#define BUILD_DATE_STRING \"$(BUILD_DATE)\"" >> $(VERSION_H)
	@echo "Generated $(VERSION_H) with version $(GIT_VERSION)"

# Un target phony per forzare la rigenerazione di version.h se necessario
force_version_h:
	@echo "--- Forcing regeneration of $(VERSION_H) ---"
	@rm -f $(VERSION_H) # Rimuove il file per forzare la sua ricreazione
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) $(VERSION_H)
#	$(MAKE) $(VERSION_H) # Chiama make per ricrearlo

clean:
	@echo "--- Cleaning project ---"
	rm -f $(VERSION_H)
	find . -type f -name "*.o" -exec rm -f {} \;
	find . -type f -name "*.a" -exec rm -f {} \;
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) force_version_h # Forza la rigenerazione di version.h
	@echo "--- Clean finished ---"

$(info Makefile.mk: Startup - F_FLAGS is currently set to = $(F_FLAGS))
$(info -------->  Makefile.mk processing complete!)