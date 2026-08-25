# Makefile per tix — versione allineata alla UI Windows
# Deploya in ../../bin  (LG_TIX=$LG_BIN/tix per usare questa versione)

PREFIXBIN = ../../bin

# 1. Lista dei file nella root
SHELLFILES = connectors.tix edit_tasks_v3.tix legodat_v2.tix libraria.tix \
preinst2.tix edit_simulx_v3.tix foraus.tix legopc.tix prefirst.tix simul_v2.tix \
draw2gr.tcl golg3_v2.tcl openhelp.tcl presave.tcl read_f14.tcl subwgs.tcl \
f01totom.tcl itemjoin.tcl preinit.tcl read_con.tcl rotate.tcl viewmgr.tcl \
fileio.tcl monitor_v2.tcl preinst.tcl read_f01.tcl select.tcl lg4_drift.inp lg5_drift.inp \
checkopen.tcl animate.tcl balloon.tcl sincviewGui.tcl settings.tcl muovi.tcl tixwish.ico \
vorwahl1.for vorwahl2.for migrate_i5.tcl gen_missing_i5.sh bgelement.tcl linkstyle.tcl \
lghmi.tcl

# 2. Lista dei file destinati alla cartella preinst/
PREINST_FILES = vorwahl1_x.for vorwahl2_x.for templ.lib makevorwahl

# 3. Lista delle immagini
IMGFILE = img/regime.gif img/transitorio.gif img/fase1.gif img/fase2.gif \
img/fase3.gif img/fase4.gif img/lego.gif img/legopc_3d.gif img/legopc1.gif \
img/legopc2.gif img/conn_e.ppm img/conn_e.xbm img/conn_n.ppm img/conn_n.xbm \
img/conn_s.ppm img/conn_s.xbm img/conn_w.ppm img/conn_w.xbm img/conne.gif \
img/connn.gif img/conns.gif img/connw.gif img/black.gif img/bulloni.bmp img/mat.gif

# 4. Libreria degli elementi di annotazione (testo e display dinamici).
#    Vive qui e non piu' nelle librerie grafiche dell'utente
#    (libgraph/libraries/remark): sono elementi generali di legopc, non devono
#    dipendere dall'assetto delle librerie di un particolare impianto.
#    Il wildcard evita di dover elencare i file uno per uno.
REMARK_FILES = $(wildcard remark/*)

# Creiamo le vere dipendenze finali
TARGETS = $(SHELLFILES:%=$(PREFIXBIN)/%) \
          $(IMGFILE:%=$(PREFIXBIN)/%) \
          $(REMARK_FILES:%=$(PREFIXBIN)/%) \
          $(PREINST_FILES:%=$(PREFIXBIN)/preinst/%)

# --- REGOLE ---

# Questo è il target di default
all: $(TARGETS)

# Regola per copiare i file nella root (e le immagini)
$(PREFIXBIN)/%: %
	@mkdir -p $(dir $@)
	cp $< $@

# Regola specifica per copiare i file nella cartella preinst/
$(PREFIXBIN)/preinst/%: %
	@mkdir -p $(dir $@)
	cp $< $@

clean:
	rm -rf $(PREFIXBIN)

# Dichiara i target che non sono file reali
.PHONY: all clean
