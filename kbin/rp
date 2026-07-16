#!/bin/ksh
#
kAddStatistic rp
echo "$*" > $TMPDIR/rphoriz.tmp
tr '[:space:]' '[\n*]' < $TMPDIR/rphoriz.tmp > $TMPDIR/rplower.tmp
tr '[:lower:]' '[:upper:]' < $TMPDIR/rplower.tmp > $TMPDIR/rpupper.tmp
if [ -d ${KDISPLAY}/plant_display ]
then
echo ${star2}
cd ${KDISPLAY}/plant_display
grep -f $TMPDIR/rpupper.tmp ./*/pag/RP.list | sed "s/RP.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read fa pd rp
do
pddescr=` grep -w ${pd} ./*/pag/PDDESCR.list | cut -f2- -d' ' `
print "|" ${fa}"\t" "|" ${rp} "|" ${pd} "|" ${pddescr} "|"
done
echo ${star2}
else
cd ${KWIN}
echo ${star7}
grep -f $TMPDIR/rpupper.tmp ./RP.list | sed "s/RP.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read pd rp
do
pddescr=` grep -w ${pd} ./PDDESCR.list | cut -f2- -d' ' `
echo "|" ${rp} "|" ${pd} "|" ${pddescr} "|"
done
echo ${star7}
fi
