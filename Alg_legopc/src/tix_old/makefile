# ******* Telelogic expanded section *******

# make_macros from makefile "makefile-5"

# make_macros from project "Alg_legopc-2007A1_RHE4_lomgr
#CFLAGS = -g -Wall
F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero
CFLAGS = -Wall -DLinux
CC = gcc
#       Makefile Header:
#       Subsystem:
#       Description:
#       %created_by: guag %
#       %date_created: Fri Mar 24 10:24:00 2006 %

#CCM_MAKE_LOCAL=cp

PREFIXBIN= ../../bin_old/


# funziona anche cos�...
#SHELLFILES=*.tix *.tcl
#IMGFILE=img/*.gif

# ... ma cos��meglio ...
SHELLFILES= connectors.tix edit_tasks_v3.tix  legodat_v2.tix  libraria.tix  \
preinst2.tix edit_simulx_v3.tix  foraus.tix legopc.tix prefirst.tix simul_v2.tix \
draw2gr.tcl golg3_v2.tcl openhelp.tcl presave.tcl read_f14.tcl subwgs.tcl \
f01totom.tcl itemjoin.tcl preinit.tcl read_con.tcl rotate.tcl viewmgr.tcl \
fileio.tcl monitor_v2.tcl preinst.tcl read_f01.tcl  select.tcl lg4_drift.inp lg5_drift.inp \
vorwahl1_x.for vorwahl2_x.for templ.lib checkopen.tcl animate.tcl makevorwahl

IMGFILE=img/regime.gif img/transitorio.gif img/fase1.gif img/fase2.gif img/fase3.gif img/fase4.gif \
img/black.gif  img/conne.gif  img/connn.gif  img/conns.gif  img/connw.gif \
img/conn_e.xbm  img/conn_n.xbm  img/conn_s.xbm  img/conn_w.xbm \
img/conn_e.ppm  img/conn_n.ppm  img/tconn_s.ppm  img/conn_w.ppm img/lego.gif

ALLFILES =  $(SHELLFILES) $(IMGFILE)

#ALLFILESBIN=$(addprefix $(PREFIXBIN),  $(ALLFILES))

ALLFILESBIN=$(PREFIXBIN)connectors.tix $(PREFIXBIN)edit_tasks_v3.tix  $(PREFIXBIN)legodat_v2.tix \
$(PREFIXBIN)libraria.tix  $(PREFIXBIN)preinst2.tix $(PREFIXBIN)edit_simulx_v3.tix  $(PREFIXBIN)foraus.tix \
$(PREFIXBIN)legopc.tix $(PREFIXBIN)prefirst.tix $(PREFIXBIN)simul_v2.tix $(PREFIXBIN)draw2gr.tcl \
$(PREFIXBIN)golg3_v2.tcl $(PREFIXBIN)openhelp.tcl $(PREFIXBIN)presave.tcl $(PREFIXBIN)read_f14.tcl \
$(PREFIXBIN)subwgs.tcl $(PREFIXBIN)f01totom.tcl $(PREFIXBIN)itemjoin.tcl \
$(PREFIXBIN)preinit.tcl $(PREFIXBIN)read_con.tcl $(PREFIXBIN)rotate.tcl $(PREFIXBIN)viewmgr.tcl \
$(PREFIXBIN)fileio.tcl $(PREFIXBIN)monitor_v2.tcl $(PREFIXBIN)preinst.tcl $(PREFIXBIN)read_f01.tcl \
$(PREFIXBIN)select.tcl $(PREFIXBIN)checkopen.tcl $(PREFIXBIN)animate.tcl\
$(PREFIXBIN)preinst/vorwahl1_x.for $(PREFIXBIN)preinst/vorwahl2_x.for $(PREFIXBIN)preinst/templ.lib \
$(PREFIXBIN)preinst/makevorwahl $(PREFIXBIN)img/regime.gif $(PREFIXBIN)img/transitorio.gif \
$(PREFIXBIN)img/fase1.gif $(PREFIXBIN)img/fase2.gif $(PREFIXBIN)img/fase3.gif $(PREFIXBIN)img/fase3.gif \
$(PREFIXBIN)img/fase4.gif $(PREFIXBIN)lg4_drift.inp $(PREFIXBIN)lg5_drift.inp \
$(PREFIXBIN)preinst/black.gif  $(PREFIXBIN)preinst/conne.gif  $(PREFIXBIN)preinst/connn.gif \
$(PREFIXBIN)preinst/conns.gif  $(PREFIXBIN)preinst/connw.gif $(PREFIXBIN)preinst/conn_e.xbm  \
$(PREFIXBIN)preinst/conn_n.xbm  $(PREFIXBIN)preinst/conn_s.xbm  $(PREFIXBIN)preinst/conn_w.xbm \
$(PREFIXBIN)preinst/conn_e.ppm  $(PREFIXBIN)preinst/conn_n.ppm  $(PREFIXBIN)preinst/conn_s.ppm  \
$(PREFIXBIN)preinst/conn_w.ppm $(PREFIXBIN)img/lego.gif


all: $(ALLFILESBIN) makefile

debug:
	echo $(ALLFILES)
	echo $(ALLFILESBIN)


#$(PREFIXBIN)%.tix: %.tix
#	cp %.tix $(PREFIXBIN)


$(PREFIXBIN)legopc.tix         : legopc.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)connectors.tix     : connectors.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)edit_tasks_v3.tix  : edit_tasks_v3.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)legodat_v2.tix     : legodat_v2.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)libraria.tix       : libraria.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)preinst2.tix       : preinst2.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)edit_simulx_v3.tix : edit_simulx_v3.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)foraus.tix         : foraus.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)prefirst.tix       : prefirst.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)simul_v2.tix       : simul_v2.tix
	cp $? $(PREFIXBIN)
$(PREFIXBIN)draw2gr.tcl        : draw2gr.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)golg3_v2.tcl       : golg3_v2.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)openhelp.tcl       : openhelp.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)presave.tcl        : presave.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)read_f14.tcl       : read_f14.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)subwgs.tcl         : subwgs.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)f01totom.tcl       : f01totom.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)itemjoin.tcl       : itemjoin.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)preinit.tcl        : preinit.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)read_con.tcl       : read_con.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)rotate.tcl         : rotate.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)viewmgr.tcl        : viewmgr.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)fileio.tcl         : fileio.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)monitor_v2.tcl     : monitor_v2.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)preinst.tcl        : preinst.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)read_f01.tcl       : read_f01.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)select.tcl         : select.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)lg4_drift.inp      : lg4_drift.inp
	cp $? $(PREFIXBIN)
$(PREFIXBIN)lg5_drift.inp      : lg5_drift.inp
	cp $? $(PREFIXBIN)
$(PREFIXBIN)preinst/vorwahl1_x.for   : vorwahl1_x.for
	cp $? $(PREFIXBIN)preinst
$(PREFIXBIN)preinst/vorwahl2_x.for   : vorwahl2_x.for
	cp $? $(PREFIXBIN)preinst
$(PREFIXBIN)preinst/templ.lib      : templ.lib
	cp $? $(PREFIXBIN)preinst
$(PREFIXBIN)preinst/makevorwahl    : makevorwahl
	cp $? $(PREFIXBIN)preinst
$(PREFIXBIN)checkopen.tcl         : checkopen.tcl
	cp $? $(PREFIXBIN)
$(PREFIXBIN)animate.tcl         : animate.tcl
	cp $? $(PREFIXBIN)


$(PREFIXBIN)img/regime.gif      : img/regime.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/transitorio.gif : img/transitorio.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/fase1.gif       : img/fase1.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/fase2.gif       : img/fase2.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/fase3.gif       : img/fase3.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/fase4.gif       : img/fase4.gif
	cp $? $(PREFIXBIN)/img
$(PREFIXBIN)img/lego.gif       : img/lego.gif
	cp $? $(PREFIXBIN)/img

$(PREFIXBIN)preinst/black.gif       : img/black.gif
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conne.gif       : img/conne.gif
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/connn.gif       : img/connn.gif
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conns.gif       : img/conns.gif
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/connw.gif       : img/connw.gif
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_e.xbm       : img/conn_e.xbm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_n.xbm       : img/conn_n.xbm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_s.xbm       : img/conn_s.xbm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_w.xbm       : img/conn_w.xbm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_e.ppm       : img/conn_e.ppm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_n.ppm       : img/conn_n.ppm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_s.ppm       : img/conn_s.ppm
	cp $? $(PREFIXBIN)/preinst
$(PREFIXBIN)preinst/conn_w.ppm       : img/conn_w.ppm
	cp $? $(PREFIXBIN)/preinst



# PROVA
#$(LEGO_BIN)/cad_curdir: cad_curdir.sh
#	cp cad_curdir.sh cad_curdir ;chmod a+wx cad_curdir ;mv cad_curdir $@
#

