#!/bin/ksh
#
FILES_TO_CLEAN="CAI_VAR_DB.dir \
                CAI_VAR_DB.pag \
		agg_scada.err \
		agg_scada.log \
		banco.log
		cai_var.dat
		hierOW.pb
		kMakeGlobpages.list
		kMakeGlobpages.rtf
		kUpDateNavigation.DB
		kUpDateNavigation.list
		lista.proc
		malf_set.mf
		net_compi.out
		pagmod.log
		parametri.out
		stato_cr.rtf
		tipo_comp_malf.mf
		caca
		core
		pipo
		pippo
                "
cd $KSIM
for file in $FILES_TO_CLEAN
do
   if test -f $file
   then
      rm  $file
      print "$file\t\teliminated"
   fi
done
rm -f *.bak
rm -f *.out
rm -f *.log
rm -f *.old
rm -f *.kold
rm -f *.ori
rm -rf kRecorderSupportDirectory
#rm -rf log
rm -rf status
#
kPulirRegoAll
#
RemoveCore
#
cd ; pu
#
rm -r tmp
mkdir tmp
#

