#!/bin/ksh
#
FILES_TO_CLEAN="gianni.togni \
		KKS.list \
		TAG.list \
		REV.list \
		DESCR.list \
		LI.list \
		OW.list \
		RP.list \
		TASK.list \
                exception_tag.lis \
                Sim.rg\
                Task.rg\
                f22circ.dat \
                f22circ.hd \
                recorder.rtf \
                core \
                PClipDb.pag \
                FileAppo \
                Compress.pag \
                ClipDb.pag \
                f22_files.dat \
                config.log \
                Context.ctx_rtf \
                ApplDb.res \
                LEVALVOLE \
                out \
                .bi_options \
                .message_list \
                .FileResourceBanco \
                .preview \
                backtrack.dat \
                perturbazioni.dat \
                snapshot.dat \
                cat \
                lista.proc \
                crealeMalf \
                stato_cr.rtf \
                uni_misc.dat \
                ELENCO.MF \
                kUpDateNavigation.in \
                seq.seq \
                caca \
                pipo \
                superlista.list \
                kDiffS01.DB \
                Context.ctx.AntesUpDateLib \
		Context.ctx.AntesUpDateLibrary \
		Context.ctx.kold \
		net_compi.out \     
                "
for file in $FILES_TO_CLEAN
do
   if test -f $file
   then
      rm  $file
      print "$file\t\teliminated"
   fi
done
rm -f *.Outtaglist
rm -f *.Intaglist
rm -f *.bak
rm -f *.out
rm -f *.log
rm -f *.old
rm -f *.kold
rm -f *.lis
rm -f *.ori
rm -rf out
rm -f Simulator
rm -rf NavigationSave
echo
echo "Removing sed file ..."
ls | grep sed$ | while read sedfile
do
print "${sedfile} \c"
rm ${sedfile}
done
echo
echo "Removing error file ..."
ls | grep err$ | while read errfile
do
print "${errfile} \c"
rm ${errfile}
done
echo
echo "Removing pagmod file ..."
ls | grep mod$ | while read modfile
do
print "${modfile} \c"
rm ${modfile}
done
#
echo "Removing script file ..."
ls | grep scr$ | while read scrfile
do
print "${scrfile} \c"
rm ${scrfile}
done
#
echo "Removing Operating windows file ..."
ls | grep ^O_ | while read owfile
do
print "\a${owfile} \c"
#rm ${owfile}
done
#
echo "Removing Malfunction windows file ..."
ls | grep ^F_ | while read MFfile
do
print "\a${MFfile} \c"
#rm ${MFfile}
done
#
if [ ! -f DirLinksAn.list ]
then
banner "WARNING"
echo "Warning : DirLinksAn.list not found in $PWD"
fi
#
echo
