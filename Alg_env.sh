#!/bin/ksh
#
#  Script:			Alg_env.sh
#  Instance:		1
#  %version:		45 %
#  Description:		
#  %created_by:		lomgr %
#  %date_created:	Wed Mar 10 15:40:35 2010 %
#  %date_created:	Wed Mar 10 15:40:35 2010 %
#
#Setta le var di ambiente che puntano ai progetti selezionati 
#Variabili di ambiente da modificare a seconda delle necessita'
SEPARATOR="Alg_global"
export SEPARATOR
#################################################################
#
#	Variabile d'ambiente per lanciare AIC12 e UIM/X sotto OSF4.0
#
#################################################################
if [ "$XFILESEARCHPATH" ]
then
	export XFILESEARCHPATH=$XFILESEARCHPATH:/usr/lib/X11/app-defaults/%N
fi
#################################################################
#
#	Verifica esistenza variabili fondamentali esterne
#
#################################################################
if [ "$LEGOROOT" ]
then
	echo "LEGOROOT="$LEGOROOT
else
	echo ""
	echo "WARNING !!! environment variable LEGOROOT not set"
	echo ""
fi
###################################################################
#Parte riguardante  la piattaforma linux 64 o 32 bit
###################################################################
PLATFORM=`uname -i`
PLATFORM=X86_64
export PLATFORM
if [ "$PLATFORM" == "i386" ]
then
export PLAT_LIB=/usr/lib
echo "###################################################################"
echo "#     Piattaforma 32 bit PLATFORM=$PLATFORM"
echo "###################################################################"
else
export PLAT_LIB=/usr/lib64
echo "###################################################################"
echo "#     Piattaforma 64 bit PLATFORM=$PLATFORM"
echo "###################################################################"
fi
###################################################################
#Parte riguardante acquisizione estensione
###################################################################
if [ "$LEGOROOT" ]
then
EXTENSION=`echo $LEGOROOT | awk -F $SEPARATOR ' {print $2} ' | cut -f1 -d "-"`
export EXTENSION
echo "Extension found " $EXTENSION
fi
###################################################################
if [ "$LEGOCAD_USER" ]
then
	echo "LEGOCAD_USER="$LEGOCAD_USER
else
	echo ""
	echo "WARNING !!! environment variable LEGOCAD_USER not set"
	echo ""
fi
if [ "$OS" ]
then
        echo "PLATFORM="$OS
else
        echo ""
        echo "WARNING !!! environment variable OS not set"
        echo ""
fi
#####################################
#####################################
#
#   AMBIENTE ALTERLEGO
#
#   Le variabili riportate da questo
#   punto in poi non devono essere
#   modificate
#
#####################################
#Se su alpha setto OSF1
#
#####################################
if [ "$OS" = OSF1 ]
then
export PATH_MAKE=/usr/bin
fi
#####################################
#
#	Elimino la var di env LANG
#	solo se non settata
#
#####################################
if [ "$LANG" = "" ]
then
	unset LANG
fi
#####################################

#Variabili di ambiente da non modificare
export SCADA=$LEGOROOT/scada${EXTENSION}
export SCADA_BIN=$LEGOROOT/scada${EXTENSION}/bin
export SCDCOMPACTALL=YES
if [ "$LEGO_BIG" = "NO" ]
then
export LEGO=$LEGOROOT/legocad${EXTENSION}/lego${EXTENSION}
else
export LEGO=$LEGOROOT/legocad${EXTENSION}/lego_big${EXTENSION}
fi
export LEGOCAD=$LEGOROOT/legocad${EXTENSION}
export LEGO_BIN=$LEGO/bin
export LEGO_LIB=$LEGO/lib
export REGO_LIB=$LEGO/lib
export PROCLIB=$LEGOROOT/procLib
export LEGOROOT_LIB=$LEGOROOT/Alg_rt${EXTENSION}/AlgLib${EXTENSION}
export LEGORT_LIB=$LEGOROOT/Alg_rt${EXTENSION}/AlgLib${EXTENSION}
export LEGOCAD_UID=$LEGOCAD/uid
export LEGOCAD_LIB=$LEGOCAD/lib
export LEGOCAD_ICO=$LEGOCAD/icons
export LEGOCAD_BIN=$LEGOCAD/bin
export XAPPLRESDIR=$LEGOCAD/risorse
export PATH=$PATH_MAKE:$LEGOCAD_BIN:$PATH:$LEGO_BIN:$SCADA_BIN
#
# **************************************
# *     define per LEGO                *
# **************************************
#
export N000=70
export N001=`expr $N000 \* 10`
export N002=`expr $N000 \* 25`
export N003=`expr $N000 \* 215`
export N004=`expr $N000 \* 215`
export N005=`expr $N000 \* 472`
export N007=`expr $N000 \* 300`
export M001=`expr $N005 \- $N003`
export M002=`expr $M001 \- $N004`
export M003=`expr $N002 \+ 1`
export M004=`expr $N003 \+ 1`
export M005=`expr $N004 \+ 1`

#
# **************************************
# *            SICRE
# **************************************
#
export SICRE=$HOME/sicre_lego
export SICREBIN=$SICRE/bin
export OFFSETTIMESIM=10
export LOADCIRICALL=YES
#
# **************************************
# *            LEGORT-profile             *
# **************************************
#
export FORMATO_DATI=1
export LEGORT=$LEGOROOT/Alg_rt${EXTENSION}
export LEGORT_UID=$LEGORT/uid
export LEGORT_INCLUDE=$LEGORT/AlgLib${EXTENSION}/libinclude
export LEGORT_BIN=$LEGORT/bin
export PATH=$PATH:$LEGORT_BIN

#
# **************************************
# *            LEGOMMI-profile             *
# **************************************
#
export LEGOMMI=$LEGOROOT/Alg_mmi${EXTENSION}
export LEGOMMI_UID=$LEGOMMI/uid
export LEGOMMI_LIB=$LEGOMMI/lib
export LEGOMMI_INCLUDE=$LEGOMMI/include
export LEGOMMI_ICON=$LEGOMMI/icon
export LEGOMMI_BITMAP=$LEGOMMI/bitmap
export LEGOMMI_BIN=$LEGOMMI/bin
export PATH=$PATH:$LEGOMMI_BIN
export XPHOME=$LEGOROOT/Tools/xprinter
export HHHOME=$LEGOROOT/Tools/hyperhelp
export HLPPATH=$LEGOROOT/Alg_help/bin

# evita la stampa di errori di tipo RtFree
export LG_DISABLE_RTFREE_ERRORS=1
#
# **************************************
# *            LEGO-profile             *
# **************************************
#
export TAVOLE=$LEGO/bin
export LEGO_BIN=$LEGO/bin
export LEGO_LIB=$LEGO/lib
##
#   Attenzione: REGOLIB e LEGOLIB individuano
#   le directories di default per gli utenti
#   Sono le directory che contengono i moduli
#   LEGO di default.
##
export PROCLIB=$LEGOROOT/procLib
export REGOLIB=$LEGOROOT/autoLib
#
# Legocad sviluppo
#
export LEGOCAD_INCLUDE=$LEGOCAD/include
export CAD_LEGO_LIB=$LEGO/lib
#
# **************************************
# *          util97-profile            *
# **************************************
#
export UTIL97=$LEGOROOT/util97${EXTENSION}
export PATH=$PATH:$UTIL97/bin:$UTIL97/prebuilt_bin
#
# **************************************
# *          util2007-profile            *
# **************************************
#
export UTIL2007=$LEGOROOT/util2007${EXTENSION}
export PATH=$PATH:$UTIL2007/bin
#
# ******************************************
# * VARIABILI PER LA GESTIONE DEI SEMAFORI *
# * E DELLE SHARED MEMORY & ecc...         *
# ******************************************
#
export SHR_TAV_KEY=999
export UTENTE=`whoami`
export USR_KEY=`id | cut -f1 -d"(" | cut -f2 -d"="`
export SHR_USR_KEY=`expr $USR_KEY \* 10000`
export SHR_USR_KEYS=`expr $SHR_USR_KEY \+ 1000`
export REF_TIME=400.00

#
# ******************************************
# * aggiunta nel path di ricerca di librerie
# * dinamiche                              *
# ******************************************
#
export LD_LIBRARY_PATH=$LEGOROOT_LIB/dcethreads_lib:$LEGOROOT_LIB/sqlite_lib:$LD_LIBRARY_PATH
#
# ******************************************
# * aggiunta nel path di ricerca degli 
# * eseguibili di $LEGOROOT_LIB/sqlite_bin
# ******************************************
#
export PATH=$PATH:$LEGOROOT_LIB/sqlite_bin
#
#############################################################
#
# Controllo sulla definizione di ALGRTDB ed eventuale settaggio
# automatico a YES con segnalazione in console 
#
#############################################################
#
if [ ! $ALGRTDB ]
then
echo
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo
echo "ATTENZIONE! variabile d'ambiente ALGRTDB non DEFINITA"
echo "Viene definita AUTOMATICAMENTE A YES"
echo
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

export ALGRTDB="YES"
fi
#
#############################################################
#    
# Sezione relativa alle procedure Ksim di Kristof per la
# generazione di un Simulatore di addestramento
#
#############################################################
#
# Sezione DEFINIZIONI
#
export KSOURCE=$LEGOROOT
export star="###############################################################################"
export star2="_______________________________________________________________________________"
export star3="@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
export star4="-------------------------------------------------------------------------------"
export star5="%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
export star6="|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
export star7="///////////////////////////////////////////////////////////////////////////////"
export star8="==============================================================================="
export star9="+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
export star10="$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
#
export KBIN=$KSOURCE/kbin
export PATH=$PATH:$KBIN
export KBINSLAVE=$KBIN
export CBIN=$KSOURCE/kbin
export FBIN=$KSOURCE/kbin
export TBIN=$KSOURCE/kbin
export SBIN=$KSOURCE/kbin
export OBIN=$KSOURCE/kbin
export TMPDIR=$HOME/tmp
export KARCHIVE=$KSOURCE/archive
#
# ====================================================================
# Simulatore CORRENTE (KSIM) e variabili derivate.
# NON hardcodato qui: si sceglie a runtime con  ksetsim <nome-sim> .
# ksetsim ri-deriva TUTTE le K* dipendenti; gli override specifici del
# singolo simulatore stanno in  $KSIM/ksim.conf  (vivono col simulatore).
# ====================================================================
export KSKED=${KSKED:-$HOME/sked}          # root dei simulatori (generale)

ksetsim() {                                 # uso: ksetsim <nome | /path-assoluto>
    if [ -z "$1" ]; then echo "uso: ksetsim <nome-simulatore | /path>"; return 1; fi
    case "$1" in /*) _ksdir="$1";; *) _ksdir="$KSKED/$1";; esac
    if [ ! -d "$_ksdir" ]; then echo "ksetsim: directory non trovata: $_ksdir"; return 1; fi
    export KSIM="$_ksdir"
    export KSIMNAME="$(basename "$_ksdir")"
    export KCASSAFORTE="$KSIM/${KSIMNAME}safe"   # default; override in ksim.conf
    # --- derivate da KSIM ---
    export KSTATUS=$KSIM/status
    export KLOG=$KSIM/log
    export KBASIC=$KSIM/plant_display
    export KEXPORT=$KSIM/export
    export KSCADA=$KSIM/scada
    export KFILEOP=$KSCADA/fileop
    export KSTART_TABLES=$KSCADA/start_txt
    export KEDIT_TABLES=$KSCADA/txt
    export KOPER_TABLES=$KSCADA/rtf
    export KWIN=$KSIM/o_win
    export KINFO=$KSIM/.info
    export KINFOTAG=$KINFO/tag
    export KDATABASES=$KSIM/databases
    export KSTATISTIC=$KSIM/statistic
    export KPAGES=$KSIM/globpages
    # --- override PER-SIMULATORE (es. KCASSAFORTE con nome irregolare) ---
    if [ -f "$KSIM/ksim.conf" ]; then . "$KSIM/ksim.conf"; fi
    # --- derivate da KCASSAFORTE (dopo l'eventuale override) ---
    export KGRAF=$KCASSAFORTE/curve
    export KDIRGR=$KCASSAFORTE/curve
    export KDIRPD=$KCASSAFORTE/plant_display
    export KDISPLAY=$KCASSAFORTE
    mkdir -p "$KSTATUS" "$KLOG" 2>/dev/null
    echo "$KSIMNAME" > "$HOME/.legosim" 2>/dev/null   # scelta "sticky" tra shell
    echo "Simulatore corrente: $KSIMNAME  ($KSIM)"
}

# elenca i simulatori disponibili (solo directory sotto $KSKED)
ksims() { for _d in "$KSKED"/*/; do [ -d "$_d" ] && basename "$_d"; done; }

# sceglie il simulatore all'avvio, in ordine di preferenza:
#   1) ultimo scelto (~/.legosim)  2) cassano0  3) primo disponibile (ksims).
# I tentativi falliti sono silenziosi (stderr scartato); quello riuscito stampa
# "Simulatore corrente: ...". Solo se $KSKED non ha simulatori stampa l'avviso.
ksetsim_default() {
    _want="$(cat "$HOME/.legosim" 2>/dev/null)"
    [ -n "$_want" ] && ksetsim "$_want" 2>/dev/null && return 0
    ksetsim cassano0 2>/dev/null && return 0
    _first="$(ksims | head -1)"
    [ -n "$_first" ] && ksetsim "$_first" 2>/dev/null && return 0
    echo "Nessun simulatore in $KSKED: creane/scegline uno con 'ksetsim <nome>' (ksims per l'elenco)."
    return 1
}

# completion di ksetsim sui nomi dei simulatori in $KSKED (solo bash, solo dir)
if [ -n "$BASH_VERSION" ]; then
    _ksetsim_complete() {
        local cur="${COMP_WORDS[COMP_CWORD]}"
        COMPREPLY=( $(compgen -W "$(ksims)" -- "$cur") )
    }
    complete -F _ksetsim_complete ksetsim
fi
#
# Sezione ALIAS
#
alias lego='cd $LEGOCAD_USER/legocad; pwd'
alias libut='cd $LEGOCAD_USER/legocad/libut; pwd'
alias libreg='cd $LEGOCAD_USER/legocad/libut_reg/libreg; pwd'
alias libmmi='cd $LEGOCAD_USER/legocad/libut_mmi; pwd'
alias lbin='cd $LEGOCAD_USER/legocad/libut_bin; pwd'
alias bin='cd $HOME/bin; pwd'
alias binall='cd $ KBIN; pwd'
alias la='ls -a'
alias ll='ls -l'
alias ksim='cd $KSIM; pwd'
alias kpages='cd $KPAGES; pwd'
alias kwin='cd $KWIN; pwd'
alias kscada='cd $KSCADA; pwd'
alias kbasic='cd $KBASIC; pwd'
#  Aggiornamento completo della configurazione di un simulatore dopo una
#  modifica a modelli, schemi o faceplate. La catena (kConnex, kNetCompi,
#  kCompStaz, kStazPages, kWinContext, kCompileSim, kCollect) sta dentro
#  kUpSim, che si ferma al primo passo fallito e scrive in $KLOG/kUpSim.log.
#  Vedi docs/BUILD.md, sezione "Aggiornare la configurazione di un simulatore".
alias lgupsim='kUpSim'
#  Variante senza le pagine MMI dei faceplate (solo xstaz):
alias lgupsimx='kUpSim -nommi'
# erano
#alias lgupsim='cd $KSIM && kConnex && net_compi && kCompStaz && kStazPages && kWinContext && kCompileSim && kCollect'
#alias lgupsimx='cd $KSIM && kConnex && net_compi && kCompStaz && kCollect'
#
# Sezione per generazione curve: KGRAF/KDIRGR/KDIRPD/KDISPLAY e KPAGES sono
# ora derivate da KSIM/KCASSAFORTE dentro ksetsim() (vedi sopra).
#
#############################################################
#                 LegoPC
#
#############################################################
#
export LG_BASE=$LEGOROOT/Alg_legopc${EXTENSION}
export LG_ENTRY=$HOME/legocad
export LG_BIN=$LG_BASE/bin
export LG_PREINST=$LG_BIN/preinst
export LG_INSTALL=$LG_BASE/install
export LG_LIBGRAPH=$LG_ENTRY/libgraph
export LG_LIBUT=$LG_ENTRY/libut
export LG_MODELS=$LG_ENTRY
export LG_GRAF=$LG_BIN/grafics
export LG_LEGO=$LEGO
export LG_PIXMAPS=$LG_LIBGRAPH/pixmaps
export LG_TIX=$LG_BIN
export LG_TOOLS=$LG_BIN
export LG_TABLES=$LG_BIN
export LG_NEWMOD=$LG_BIN/newmod
export LG_POSTPROC=$LG_BIN
export LG_LIBRARIAN=$LG_BIN/libraria
export LG_FILESI5=$LG_LIBGRAPH/files_i5
export LG_HELP=$LG_LIBGRAPH/help
export LG_LIBRARIES=$LG_LIBGRAPH/libraries
export LG_XBM=$LG_LIBGRAPH/xbm
export LG_CONCIL=$LG_BIN%/lgconcil
export LG_SIMULATORS=$LG_ENTRY
export LG_HTML=$LG_BASE/Alg_legopc_help${EXTENSION}

#  ---- Programmi esterni usati da legopc (File->Settings) -------------------
#  Questi sono solo il FALLBACK: la scelta dell'utente sta in
#  $LG_ENTRY/legopc_prefs.tcl (::pref_browser, ::pref_texteditor, ...) e
#  legopc.tix la applica all'avvio SOVRASCRIVENDO queste variabili.
#  Erano hardcodate su programmi non sempre installati (/usr/bin/mozilla,
#  kwrite), mentre LG_ICOEDITOR/LG_PDFVIEWER/LG_XTERM non erano definite
#  affatto: `exec $env(LG_ICOEDITOR)` in libraria.tix finiva su una variabile
#  inesistente. Ora si prende il primo programma davvero presente; per
#  forzarne uno basta esportare la variabile prima di sorgiare il profilo.
lg_pick() {
    _lg_var=$1; shift
    eval "_lg_val=\${$_lg_var:-}"
    if [ -z "$_lg_val" ]; then
        _lg_val=$1                     # segnaposto se non ce n'e' nessuno
        for _lg_c in "$@"; do
            if command -v "$_lg_c" >/dev/null 2>&1; then _lg_val=$_lg_c; break; fi
        done
    fi
    eval "export $_lg_var=\"\$_lg_val\""
    unset _lg_val _lg_c _lg_var
}

lg_pick LG_BROWSER    firefox falkon chromium chromium-browser google-chrome epiphany xdg-open
lg_pick LG_TEXTEDITOR mousepad gedit kate kwrite xed pluma leafpad gvim xdg-open
lg_pick LG_ICOEDITOR  gimp krita drawing kolourpaint pinta mtpaint xdg-open
lg_pick LG_PDFVIEWER  evince atril okular qpdfview zathura xpdf xdg-open
lg_pick LG_XTERM      xterm konsole gnome-terminal xfce4-terminal lxterminal

unset -f lg_pick
#  ---------------------------------------------------------------------------
export PATH=$LG_BASE/tcltktix/bin:$PATH
export LD_LIBRARY_PATH=$LG_BASE/tcltktix/lib:$LD_LIBRARY_PATH
# lgpc0 rimosso (2026-08-02): lanciava la legopc originale da $LG_BASE/bin_old,
# che veniva generata da src/tix_old. Quel sorgente e' obsoleto ed e' stato
# escluso dal build (Alg_legopc/Makefile.mk), quindi bin_old non e' piu'
# aggiornato: usare 'lgpc' (src/tix -> $LG_BIN).
# alias lgpc0='export LG_TIX=$LG_BASE/bin_old; wish $LG_TIX/legopc.tix'
alias lgpc='export LG_TIX=$LG_BIN; wish $LG_TIX/legopc.tix'
export LG_WISH=$LEGOROOT/tcltktix-8.3.5b/tcltktix/bin/wish
alias lgpcu='$LG_WISH $LG_TIX/legopc.tix'
lgpc2() {
  wish $LG_TIX/legopc.tix
#  /percorso/completo/al/programma --con --opzioni "$@"
}
#
#############################################################
#                 LINUX
# Sezione relativa alla gestione delle eccezione floating point
#############################################################
#
if [ "${OS}" = "Linux" ]
then
  export ALG_FPE_OBJ=" ${LEGOROOT_LIB}/trapfpe.o "
  if [ "${ALG_FPE}" = "NO" ]
  then
    export ALG_FPE_OBJ=""
  fi
fi

#############################################################
#                 OSF1
# Sezione relativa alle compilazioni
#############################################################
if [ "$OS" = OSF1 ]
then
	export XPLIB=$XPHOME/lib.$ARCH/libXp.a
	export X_LIB=" $XPLIB -lMrm -lXm -lXt -lX11"
	export X_INCLUDE=' -I. -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib -DOSF1'
	export X_FLAGS='-c  -DSNAPSHOT'
	export UIL_INCLUDE=' -I/usr/include/uil'
	export UIL_COMPILER='/usr/bin/X11/uil'
	export C_LIB='/lib/libbsd.a'
	export C_FLAGS=" -O1 -D_BSD -D$OS  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I$XPHOME/include -I$HHHOME/WinHelp"
	export OTHER_LIB="-lm"
	export F_FLAGS=' -lfor'
	export LIBS='-ls -ldnet'
#	export HYPERHELP_OBJ="$HHHOME/WinHelp/WinHelp.o"
	export LINKER_OPTIONS=" -taso -L$XPHOME/lib.$ARCH"
	export PREPROCESSOR_OPTIONS=' -C -DOSF1'
	export STUB_LIBS=' -ldnet_stub'
	export FC_LIBS=' -lfor -lFutil -lots -lm '
	export F_LIB="$FC_LIBS"
	export LIBOTS="-lots"
	export PATH="$PATH:/usr/bin/X11:/usr/sbin:."
#############################################################
#                 OSF1
# FINE Sezione relativa alle compilazioni
#
#############################################################
elif [ "$OS" = "AIX" ]
then
#############################################################
#                 AIX
# Sezione relativa alle compilazioni
#############################################################
export X_FLAGS='-c  -DSNAPSHOT'
export C_LIB='/lib/libbsd.a'
export C_FLAGS=" -D_BSD -D$OS -DXOPEN_CATALOG -DAIXV3 -DUNIX"
export OTHER_LIB="-lm -lbsd"
export F_LIB=' -lxlf90'
export F_FLAGS=' -qextname '
export LIBS=' -ls '
export HYPERHELP_OBJ=""
export LINKER_OPTIONS=
export PREPROCESSOR_OPTIONS=' -DAIX'
export STUB_LIBS=
VERSIONAIX=`uname -a | cut -f4 -d " "`
if [ "$VERSIONAIX" = "3" ]
then
export FC_LIBS=' -lxlf'
elif [ "$VERSIONAIX" = "4" ]
then
export FC_LIBS=' -lxlf90'
fi
export LIBOTS=" "

#############################################################
#                 AIX
# FINE Sezione relativa alle compilazioni
#
#############################################################
fi

export  FC_LIBS=' -lm -L$(PLAT_LIB) -lgfortran '

#############################################################
# Inizializzazione tavole del vapore
initav

alias test=' cd '$LEGOCAD_USER' ; pwd'
