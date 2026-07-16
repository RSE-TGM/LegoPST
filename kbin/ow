#!/bin/ksh
#
kAddStatistic ow
echo "$*" > $TMPDIR/owhoriz.tmp
tr '[:space:]' '[\n*]' < $TMPDIR/owhoriz.tmp > $TMPDIR/owlower.tmp
tr '[:lower:]' '[:upper:]' < $TMPDIR/owlower.tmp > $TMPDIR/owupper.tmp
if [ -d ${KDISPLAY}/plant_display ]
then
echo ${star2}
cd ${KDISPLAY}/plant_display
grep -f $TMPDIR/owupper.tmp ./*/pag/OW.list | sed "s/OW.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read fa pd ow
do
pddescr=` grep -w ${pd} ./*/pag/PDDESCR.list | cut -f2- -d' ' `
if [ -f ${KEXPORT}/kpd.txt ]
then
pdwithoutext=` echo ${pd} | sed "s/M_S_//g" `
pdhier=` grep -w ${pdwithoutext} ${KEXPORT}/kpd.txt | cut -f3- -d' ' `
else
pdhier=""
fi
print "|" ${fa}"\t" "|" ${ow} "|" ${pd} "|" ${pddescr} "|" ${pdhier} "|" 
done
echo ${star2}
else
cd ${KWIN}
echo ${star7}
grep -f $TMPDIR/owupper.tmp ./OW.list | sed "s/OW.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read pd ow
do
pddescr=` grep -w ${pd} ./PDDESCR.list | cut -f2- -d' ' `
if [ -f ${KEXPORT}/kpd.txt ]
then
pdwithoutext=` echo ${pd} | sed "s/M_S_//g" `
pdhier=` grep -w ${pdwithoutext} ${KEXPORT}/kpd.txt | cut -f3- -d' ' `
else
pdhier=""
fi
echo "|" ${ow} "|" ${pd} "|" ${pddescr} "|" ${pdhier} "|" 
done
echo ${star7}
fi
