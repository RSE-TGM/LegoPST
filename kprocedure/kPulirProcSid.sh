#!/bin/ksh
#
FILES_TO_CLEAN="S01 \
		S02_OSF1 \
		S02_ULTRIX \
		S04 \
		S05 \
		f14.s \
		foraus.f \
		forausbase.f \
		lg1a.out \
		lg3.out \
		variabili.edf \
		variabili.rtf \
		edi14.lis \
		backtrack.dat \
		f22circ.dat \
		lista.proc \
		page_mandb.dat \
		perturbazioni.dat \
		recorder.edf \
		recorder.rtf \
		snapshot.dat \
		stato_cr.rtf \
		Context.ctx \
		f01.lastsave \
		f22_fgraf.edf \
		librarian_debug.cfg \
		macroblocks.lastsave \
		aiax \
		edi14.lis \
		f14.livellibase \
		kDiffS01.DB \
                "
for file in $FILES_TO_CLEAN
do
   if test -f $file
   then
      rm $file
      print "$file\t\teliminated"
   fi
done
rm -f *.bak
rm -f *.out
rm -f *.log
rm -f *.old
rm -f *.kold
rm -f *.tmp
rm -rf out
#rm -rf proc
rm -rf proc/*bak
rm -f Simulator
if [ -f *.o ]
then
print "Warning : Local modules in $PWD\a"
fi
