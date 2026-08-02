#!/bin/ksh
#
#  Script:                      testerr.sh
#  Subsystem:           1
#  %version:            2 %
#  Description:
#  %created_by:         lomgr %
#  %date_created:       Wed Feb 19 17:00:48 1997 %

#  $1 = file col codice d'errore che lg3b/lg4/lg5 scrivono a fine calcolo.
#  Il target di maketask fa "rm -f $1" prima di lanciare il calcolo: se il
#  calcolo (es. steady state con lg3b) fallisce senza riscriverlo, senza questo
#  controllo il "cat" fallirebbe e l'utente vedrebbe solo
#  "cat: <file>: No such file or directory", che nasconde la vera causa.
if [ ! -f "$1" ]; then
	print "testerr: $1 non prodotto: calcolo fallito (vedi lg3b.out/lg4.out/lg5.out)" >&2
	exit 1
fi
exit `cat $1 |tr -d " "`
