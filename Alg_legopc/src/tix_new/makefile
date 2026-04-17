# Makefile per tix_new — versione allineata alla UI Windows
# Deploya in ../../bin/tix_new  (LG_TIX=$LG_BIN/tix_new per usare questa versione)

PREFIXBIN= ../../bin/tix_new

SHELLFILES= connectors.tix edit_tasks_v3.tix  legodat_v2.tix  libraria.tix  \
preinst2.tix edit_simulx_v3.tix  foraus.tix legopc.tix prefirst.tix simul_v2.tix \
draw2gr.tcl golg3_v2.tcl openhelp.tcl presave.tcl read_f14.tcl subwgs.tcl \
f01totom.tcl itemjoin.tcl preinit.tcl read_con.tcl rotate.tcl viewmgr.tcl \
fileio.tcl monitor_v2.tcl preinst.tcl read_f01.tcl select.tcl lg4_drift.inp lg5_drift.inp \
vorwahl1_x.for vorwahl2_x.for templ.lib checkopen.tcl animate.tcl makevorwahl \
balloon.tcl sincviewGui.tcl settings.tcl muovi.tcl

IMGFILE=img/regime.gif img/transitorio.gif img/fase1.gif img/fase2.gif \
img/fase3.gif img/fase4.gif img/lego.gif \
img/legopc_3d.gif img/legopc1.gif img/legopc2.gif

all: makedirs \
$(SHELLFILES:%=$(PREFIXBIN)/%) \
$(IMGFILE:%=$(PREFIXBIN)/%)

makedirs:
	mkdir -p $(PREFIXBIN)/img
	mkdir -p $(PREFIXBIN)/preinst

$(PREFIXBIN)/legopc.tix         : legopc.tix         ; cp $< $@
$(PREFIXBIN)/connectors.tix     : connectors.tix     ; cp $< $@
$(PREFIXBIN)/edit_tasks_v3.tix  : edit_tasks_v3.tix  ; cp $< $@
$(PREFIXBIN)/legodat_v2.tix     : legodat_v2.tix     ; cp $< $@
$(PREFIXBIN)/libraria.tix       : libraria.tix       ; cp $< $@
$(PREFIXBIN)/preinst2.tix       : preinst2.tix       ; cp $< $@
$(PREFIXBIN)/edit_simulx_v3.tix : edit_simulx_v3.tix ; cp $< $@
$(PREFIXBIN)/foraus.tix         : foraus.tix         ; cp $< $@
$(PREFIXBIN)/prefirst.tix       : prefirst.tix       ; cp $< $@
$(PREFIXBIN)/simul_v2.tix       : simul_v2.tix       ; cp $< $@
$(PREFIXBIN)/draw2gr.tcl        : draw2gr.tcl        ; cp $< $@
$(PREFIXBIN)/golg3_v2.tcl       : golg3_v2.tcl       ; cp $< $@
$(PREFIXBIN)/openhelp.tcl       : openhelp.tcl       ; cp $< $@
$(PREFIXBIN)/presave.tcl        : presave.tcl        ; cp $< $@
$(PREFIXBIN)/read_f14.tcl       : read_f14.tcl       ; cp $< $@
$(PREFIXBIN)/subwgs.tcl         : subwgs.tcl         ; cp $< $@
$(PREFIXBIN)/f01totom.tcl       : f01totom.tcl       ; cp $< $@
$(PREFIXBIN)/itemjoin.tcl       : itemjoin.tcl       ; cp $< $@
$(PREFIXBIN)/preinit.tcl        : preinit.tcl        ; cp $< $@
$(PREFIXBIN)/read_con.tcl       : read_con.tcl       ; cp $< $@
$(PREFIXBIN)/rotate.tcl         : rotate.tcl         ; cp $< $@
$(PREFIXBIN)/viewmgr.tcl        : viewmgr.tcl        ; cp $< $@
$(PREFIXBIN)/fileio.tcl         : fileio.tcl         ; cp $< $@
$(PREFIXBIN)/monitor_v2.tcl     : monitor_v2.tcl     ; cp $< $@
$(PREFIXBIN)/preinst.tcl        : preinst.tcl        ; cp $< $@
$(PREFIXBIN)/read_f01.tcl       : read_f01.tcl       ; cp $< $@
$(PREFIXBIN)/select.tcl         : select.tcl         ; cp $< $@
$(PREFIXBIN)/checkopen.tcl      : checkopen.tcl      ; cp $< $@
$(PREFIXBIN)/animate.tcl        : animate.tcl        ; cp $< $@
$(PREFIXBIN)/balloon.tcl        : balloon.tcl        ; cp $< $@
$(PREFIXBIN)/sincviewGui.tcl    : sincviewGui.tcl    ; cp $< $@
$(PREFIXBIN)/settings.tcl       : settings.tcl       ; cp $< $@
$(PREFIXBIN)/muovi.tcl          : muovi.tcl          ; cp $< $@
$(PREFIXBIN)/lg4_drift.inp      : lg4_drift.inp      ; cp $< $@
$(PREFIXBIN)/lg5_drift.inp      : lg5_drift.inp      ; cp $< $@
$(PREFIXBIN)/vorwahl1_x.for     : vorwahl1_x.for     ; cp $< $(PREFIXBIN)/preinst/
$(PREFIXBIN)/vorwahl2_x.for     : vorwahl2_x.for     ; cp $< $(PREFIXBIN)/preinst/
$(PREFIXBIN)/templ.lib          : templ.lib          ; cp $< $(PREFIXBIN)/preinst/
$(PREFIXBIN)/makevorwahl        : makevorwahl        ; cp $< $(PREFIXBIN)/preinst/

$(PREFIXBIN)/img/regime.gif      : img/regime.gif      ; cp $< $@
$(PREFIXBIN)/img/transitorio.gif : img/transitorio.gif ; cp $< $@
$(PREFIXBIN)/img/fase1.gif       : img/fase1.gif       ; cp $< $@
$(PREFIXBIN)/img/fase2.gif       : img/fase2.gif       ; cp $< $@
$(PREFIXBIN)/img/fase3.gif       : img/fase3.gif       ; cp $< $@
$(PREFIXBIN)/img/fase4.gif       : img/fase4.gif       ; cp $< $@
$(PREFIXBIN)/img/lego.gif        : img/lego.gif        ; cp $< $@
$(PREFIXBIN)/img/legopc_3d.gif   : img/legopc_3d.gif   ; cp $< $@
$(PREFIXBIN)/img/legopc1.gif     : img/legopc1.gif     ; cp $< $@
$(PREFIXBIN)/img/legopc2.gif     : img/legopc2.gif     ; cp $< $@

clean:
	rm -rf $(PREFIXBIN)
