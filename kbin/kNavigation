#!/bin/ksh
#
clear
type=$1
authorisation=$2
if [  "${type}" = "-Child" -o  "${type}" = "-Parent" ]
then
if [ ! "${authorisation}" = "7777" ]
then
echo "Sorry : Not authorised"
exit
fi
kAddScreen kNavigation "Start on `pwd`"
kAddLog kNavigation "Start on `pwd`"
if [  "$2" = "-Init" ]
then
ls NavigationSave/*AnteskNavigation | \
	sed "s/.pag.AnteskNavigation//g" | \
	sed "s/NavigationSave//g" | \
	tr -d '/' | while read pagesave
do
echo "Warning : Old Page ${pagesave} Recuperation"
cp NavigationSave/${pagesave}.pag.AnteskNavigation ${pagesave}.pag
done
exit
fi
################################################################################
#	Input kNavigation
################################################################################
print "$star\n\tInput kNavigation\n$star"
print "$star\n\tInput kNavigation\n$star" >> kNavigation.log
grep r_saio_03 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' > kNavigationInput.tmp
grep r_slio_03 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationInput.tmp
# SPS
grep r_saio_06 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationInput.tmp
grep r_saio_07 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationInput.tmp
grep r_slio_06 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationInput.tmp
sort -o kNavigationInput.tmp kNavigationInput.tmp
cat kNavigationInput.tmp | while read line
do
page=`echo "$line" | cut -f1 -d" " `
internaltag=`echo "$line" | cut -f2 -d" " `
ypositionmaster=`grep ^*${internaltag}.y0 ${page}.pag | tr -s '\011' ' ' | cut -f2 -d" " `
yheigh=`grep ^*${internaltag}.height0 ${page}.pag | tr -s '\011' ' ' | cut -f2 -d" "`
cp ${page}.pag ${page}.new
if [ ! -d ./NavigationSave ]
then
mkdir ./NavigationSave
fi
if [ ! -f ./NavigationSave/${page}.pag.AnteskNavigation ]
then
cp ${page}.pag ./NavigationSave/${page}.pag.AnteskNavigation
fi
export SuperTag=""
if [ "${type}" = "-Child" ]
then
yposition=`expr $ypositionmaster + 1`
. kAddChildrenWidget ChangePage ${internaltag} ${page}.new
echo "Elaborating Input Page ${page} Widget ${internaltag} Children ${SuperTag} ..."
echo "Elaborating Input Page ${page} Widget ${internaltag} Children ${SuperTag} ..." >> kNavigation.log
if [ "${SuperTag}" = "" ]
then
echo "ERROR in kAddChildrenWidget with page ${page}"
echo "ERROR in kAddChildrenWidget with page ${page}" >> kNavigation.log
exit
fi
echo "*${internaltag}${SuperTag}.background:	#00000000ffff" >> ${page}.new
echo "*${internaltag}${SuperTag}.bitmapName:	kChangePage" >> ${page}.new
echo "*${internaltag}${SuperTag}.borderColor:	#000000000000" >> ${page}.new
echo "*${internaltag}${SuperTag}.borderWidth:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.buttonBackground:	#ffffffff0000" >> ${page}.new
echo "*${internaltag}${SuperTag}.changeDraw:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.changeType:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.compiled:	# " >> ${page}.new
echo "*${internaltag}${SuperTag}.fillButton:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.height0:	8" >> ${page}.new
echo "*${internaltag}${SuperTag}.inheritBackground:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.normFg:	#000000000000" >> ${page}.new
echo "*${internaltag}${SuperTag}.normalFont:	fixed" >> ${page}.new
echo "*${internaltag}${SuperTag}.objectTag:	kNavigationInput" >> ${page}.new
echo "*${internaltag}${SuperTag}.pageName:	NotConfiguredNavigationPage" >> ${page}.new
echo "*${internaltag}${SuperTag}.trasparent:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.userLevel:	1" >> ${page}.new
echo "*${internaltag}${SuperTag}.width0:	8" >> ${page}.new
echo "*${internaltag}${SuperTag}.x0:	113" >> ${page}.new
echo "*${internaltag}${SuperTag}.x:	113" >> ${page}.new
echo "*${internaltag}${SuperTag}.y0:	1" >> ${page}.new
echo "*${internaltag}${SuperTag}.y:	1" >> ${page}.new
elif [ "${type}" = "-Parent" ]
then
yposition=`expr $ypositionmaster`
. kAddWidget ChangePage ${page}.new
echo "Elaborating Input Page ${page} Widget ${SuperTag} ..."
echo "Elaborating Input page ${page} Widget ${SuperTag} ..." >> kNavigation.log
if [ "${SuperTag}" = "" ]
then
echo "ERROR in kAddWidget with page ${page}"
echo "ERROR in kAddWidget with page ${page}" >> kNavigation.log
exit
fi
echo "*${SuperTag}.background:	#00000000ffff" >> ${page}.new
echo "*${SuperTag}.bitmapName:	kChangePage" >> ${page}.new
echo "*${SuperTag}.borderColor:	#000000000000" >> ${page}.new
echo "*${SuperTag}.borderWidth:	1" >> ${page}.new
#echo "*${SuperTag}.borderWidth:	0" >> ${page}.new
echo "*${SuperTag}.buttonBackground:	#ffffffff0000" >> ${page}.new
echo "*${SuperTag}.changeDraw:	0" >> ${page}.new
echo "*${SuperTag}.changeType:	0" >> ${page}.new
echo "*${SuperTag}.compiled:	#"  >> ${page}.new
echo "*${SuperTag}.fillButton:	0" >> ${page}.new
echo "*${SuperTag}.height0:	8" >> ${page}.new
echo "*${SuperTag}.inheritBackground:	0" >> ${page}.new
echo "*${SuperTag}.normFg:	#ffffffff0000" >> ${page}.new
echo "*${SuperTag}.normalFont:	fixed" >> ${page}.new
echo "*${SuperTag}.objectTag:	kNavigationInput" >> ${page}.new
echo "*${SuperTag}.pageName:	NotConfiguratedNavigationPage" >> ${page}.new
echo "*${SuperTag}.trasparent:	1" >> ${page}.new
echo "*${SuperTag}.userLevel:	1" >> ${page}.new
echo "*${SuperTag}.width0:	8" >> ${page}.new
if [ "${yheigh}" = "49" ]
then
echo "*${SuperTag}.x0:	240" >> ${page}.new
echo "*${SuperTag}.x:	240" >> ${page}.new
else
echo "*${SuperTag}.x0:	264" >> ${page}.new
echo "*${SuperTag}.x:	264" >> ${page}.new
fi
#echo "*${SuperTag}.x0:	120" >> ${page}.new
#echo "*${SuperTag}.x:	120" >> ${page}.new
echo "*${SuperTag}.y0:	${yposition}" >> ${page}.new
echo "*${SuperTag}.y:	${yposition}" >> ${page}.new
else
echo Error1
exit
fi
mv ${page}.new ${page}.pag
done
rm -f kNavigationInput.tmp
################################################################################
################################################################################
#	Output kNavigation
################################################################################
print "$star\n\tOutput kNavigation\n$star"
print "$star\n\tOutput kNavigation\n$star" >> kNavigation.log
grep r_saio_04 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' > kNavigationOutput.tmp
grep r_slio_02 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationOutput.tmp
# SPS
grep r_saio_08 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationOutput.tmp
grep r_slio_08 ????.pag | sed "s/.pag:/ /g" | sed "s/.nomeFile:/ /g" | \
                          tr '*' ' ' | tr -s '\011' ' ' >> kNavigationOutput.tmp
sort -o kNavigationOutput.tmp kNavigationOutput.tmp
cat kNavigationOutput.tmp | while read line
do
page=`echo "$line" | cut -f1 -d" " `
internaltag=`echo "$line" | cut -f2 -d" " `
ypositionmaster=`grep ^*${internaltag}.y0 ${page}.pag | tr -s '\011' ' ' | cut -f2 -d" " `
yheigh=`grep ^*${internaltag}.height0 ${page}.pag | tr -s '\011' ' ' | cut -f2 -d" "`
cp ${page}.pag ${page}.new
if [ ! -f ./NavigationSave/${page}.pag.AnteskNavigation ]
then
cp ${page}.pag ./NavigationSave/${page}.pag.AnteskNavigation
fi
export SuperTag=""
if [ "${type}" = "-Child" ]
then
yposition=`expr $ypositionmaster`
. kAddChildrenWidget ChangePage ${internaltag} ${page}.new
echo "Elaborating Output Page ${page} Widget ${internaltag} Children ${SuperTag} ..."
echo "Elaborating Output Page ${page} Widget ${internaltag} Children ${SuperTag} ..." >> kNavigation.log
if [ "${SuperTag}" = "" ]
then
echo "ERROR in kAddChildrenWidget with page ${page}"
echo "ERROR in kAddChildrenWidget with page ${page}" >> kNavigation.log
exit
fi
echo "*${internaltag}${SuperTag}.background:	#ffff0000ffff" >> ${page}.new
echo "*${internaltag}${SuperTag}.bitmapName:	kChangePage" >> ${page}.new
echo "*${internaltag}${SuperTag}.borderColor:	#000000000000" >> ${page}.new
echo "*${internaltag}${SuperTag}.borderWidth:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.buttonBackground:	#ffffffff0000" >> ${page}.new
echo "*${internaltag}${SuperTag}.changeDraw:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.changeType:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.compiled:	# " >> ${page}.new
echo "*${internaltag}${SuperTag}.fillButton:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.height0:	8" >> ${page}.new
echo "*${internaltag}${SuperTag}.inheritBackground:	1" >> ${page}.new
echo "*${internaltag}${SuperTag}.normFg:	#000000000000" >> ${page}.new
echo "*${internaltag}${SuperTag}.normalFont:	fixed" >> ${page}.new
echo "*${internaltag}${SuperTag}.objectTag:	kNavigationOutput" >> ${page}.new
echo "*${internaltag}${SuperTag}.pageName:	NotConfiguredNavigationPage" >> ${page}.new
echo "*${internaltag}${SuperTag}.trasparent:	0" >> ${page}.new
echo "*${internaltag}${SuperTag}.userLevel:	1" >> ${page}.new
echo "*${internaltag}${SuperTag}.width0:	8" >> ${page}.new
echo "*${internaltag}${SuperTag}.x0:	97" >> ${page}.new
echo "*${internaltag}${SuperTag}.x:	97" >> ${page}.new
echo "*${internaltag}${SuperTag}.y0:	1" >> ${page}.new
echo "*${internaltag}${SuperTag}.y:	1" >> ${page}.new
elif [ "${type}" = "-Parent" ]
then
yposition=`expr $ypositionmaster`
. kAddWidget ChangePage  ${page}.new
echo "Elaborating Output Page ${page} Widget ${SuperTag} ..."
echo "Elaborating Output page ${page} Widget ${SuperTag} ..." >> kNavigation.log
if [ "${SuperTag}" = "" ]
then
echo "ERROR in kAddWidget with page ${page}"
echo "ERROR in kAddWidget with page ${page}" >> kNavigation.log
exit
fi
echo "*${SuperTag}.background:	#ffff0000ffff" >> ${page}.new
echo "*${SuperTag}.bitmapName:	kChangePage" >> ${page}.new
echo "*${SuperTag}.borderColor:	#000000000000" >> ${page}.new
echo "*${SuperTag}.borderWidth:	1" >> ${page}.new
#echo "*${SuperTag}.borderWidth:	0" >> ${page}.new
echo "*${SuperTag}.buttonBackground:	#ffffffff0000" >> ${page}.new
echo "*${SuperTag}.changeDraw:	0" >> ${page}.new
echo "*${SuperTag}.changeType:	0" >> ${page}.new
echo "*${SuperTag}.compiled:	#"  >> ${page}.new
echo "*${SuperTag}.fillButton:	0" >> ${page}.new
echo "*${SuperTag}.height0:	8" >> ${page}.new
echo "*${SuperTag}.inheritBackground:	0" >> ${page}.new
#echo "*${SuperTag}.inheritBackground:	1" >> ${page}.new
echo "*${SuperTag}.normFg:	#ffffffff0000" >> ${page}.new
echo "*${SuperTag}.normalFont:	fixed" >> ${page}.new
echo "*${SuperTag}.objectTag:	kNavigationOutput" >> ${page}.new
echo "*${SuperTag}.pageName:	NotConfiguratedNavigationPage" >> ${page}.new
echo "*${SuperTag}.trasparent:	1" >> ${page}.new
echo "*${SuperTag}.userLevel:	1" >> ${page}.new
echo "*${SuperTag}.width0:	8" >> ${page}.new
if [ "${yheigh}" = "49" ]
then
echo "*${SuperTag}.x0:	1024" >> ${page}.new
echo "*${SuperTag}.x:	1024" >> ${page}.new
else
echo "*${SuperTag}.x0:	1016" >> ${page}.new
echo "*${SuperTag}.x:	1016" >> ${page}.new
fi
#echo "*${SuperTag}.x0:	1128" >> ${page}.new
#echo "*${SuperTag}.x:	1128" >> ${page}.new
echo "*${SuperTag}.y0:	${yposition}" >> ${page}.new
echo "*${SuperTag}.y:	${yposition}" >> ${page}.new
else
echo Error1
exit
fi
mv ${page}.new ${page}.pag
done
rm -f kNavigationOutput.tmp
################################################################################
kAddScreen kNavigation "End on `pwd`"
kAddLog kNavigation "End on `pwd`"
else
print "\n\tCorrect Use : kNavigation  -Parent/-Child  [ -Init ]\n\a"
exit
fi
print "Log File :\t kNavigation.log"
