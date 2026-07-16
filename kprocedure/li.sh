#!/bin/ksh
#
kAddStatistic li
echo "$*" > $TMPDIR/lihoriz.tmp
tr '[:space:]' '[\n*]' < $TMPDIR/lihoriz.tmp > $TMPDIR/lillier.tmp
tr '[:lower:]' '[:upper:]' < $TMPDIR/lillier.tmp > $TMPDIR/liupper.tmp
if [ -d ${KDISPLAY}/plant_display ]
then
echo ${star2}
cd ${KDISPLAY}/plant_display
grep -f $TMPDIR/liupper.tmp ./*/pag/LI.list | sed "s/LI.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read fa pd li
do
print "|" ${fa}"\t" "|" ${pd} "|" ${li} "|"
done
echo ${star2}
else
cd ${KWIN}
echo ${star7}
grep -f $TMPDIR/liupper.tmp ./LI.list | sed "s/LI.list:/ /g" | \
tr -d '.' | tr -d '/' | sed "s/pag//g" | while read fa pd li
do
print "|" ${fa}"\t" "|" ${pd} "|" ${li} "|"
done
echo ${star7}
fi
