#!/bin/ksh
#
echo Start ktaskinfo
cd
find . -name task.info -print > ktaskinfo.out
grep -v out ktaskinfo.out > ktaskinfo.list
cat ktaskinfo.list | while read file
do
cp $file $file.sav
sed  "s/pentolin/puegasr0/g"  $file.sav  > $file
done
rm ktaskinfo.out ktaskinfo.list
print "End ktaskinfo \a"
