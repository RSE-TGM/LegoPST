#!/bin/ksh
#
print "\n$star5"
echo "Unaccesible curves from mmi"
print "$star5\n"
print "\n$star5" >> $KLOG/kCheckSimulator.log
echo "Unaccesible curves from mmi" >> $KLOG/kCheckSimulator.log
print "$star5\n" >> $KLOG/kCheckSimulator.log
cd $KWIN
rm -f $KGRAF/Group.lst $KGRAF/Group.nok
grep ".pageName" M_S_*pag | cut -f2 | grep "M_S" | sed "s/M_S_//g" | sort | uniq > $KGRAF/PdCurveLink.lst
cut -f1 -d";" $KGRAF/grugraf.txt | sort | uniq > $KGRAF/Group.tmp
cat $KGRAF/Group.tmp | while read page
do
echo "${page}_GR" >> $KGRAF/Group.lst
done
grep -wf $KGRAF/Group.lst $KGRAF/PdCurveLink.lst > $KGRAF/Group.ok
grep -vwf $KGRAF/Group.ok $KGRAF/Group.lst | sed "s/_GR//g" > $KGRAF/Group.nok.tmp
grep -f $KGRAF/Group.nok.tmp $KGRAF/grugraf.txt | cut -f1-2 -d";" | sed "s/;/  /g" > $KGRAF/Group.nok.tmp2
cat $KGRAF/Group.nok.tmp2 | while read line
do
print "ERROR\t: ${line}" >> $KGRAF/Group.nok
done
rm -f $KGRAF/Group.lst $KGRAF/PdCurveLink.lst $KGRAF/Group.tmp $KGRAF/Group.nok.tmp $KGRAF/Group.nok.tmp2
if [ -f $KGRAF/Group.nok ]
then
cat $KGRAF/Group.nok >> $KLOG/kCheckSimulator.log
cat $KGRAF/Group.nok
fi
echo
