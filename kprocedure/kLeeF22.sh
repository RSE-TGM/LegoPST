#!/bin/ksh
#
#  Lettura del file f22 in una finestra di terminale. lgterm apre il terminale
#  scelto dall'utente (LG_XTERM, File -> Settings di legopc): prima qui c'era
#  /usr/bin/X11/xterm, che su Fedora non esiste.
lgterm -t "Lectura fichero F22" -g 80x20+287+180 -bg coral -- ksh $KBIN/kLeeF22Slave1 &
