#!/bin/ksh
#
print "\n$star8"
print "\tCurve page generation"
print "$star8\n"
cd $KGRAF
if [ ! -f curve.list ]
then
echo File curve.list not found
exit
fi
if [ ! -r ./tmp ]
then
mkdir ./tmp
else
echo "Error : Directory tmp already exixts"
exit
fi
grep -v '^#' ./curve.list | while read line
do
if [ "$line" = "****" ]
then
	if [ -f ./tmp/curentgraph.list ]
	then
	filename=`grep nomePag ./tmp/curentgraph.list | awk '{ print $2 }'`
	echo $filename >> ./tmp/curvename.list
	if [ "$filename" = "" ]
	then
		echo $star >> ./tmp/$0.err
		cat ./tmp/curentgraph.list >> ./tmp/$0.err
		rm ./tmp/curentgraph.list
	else
		cut -f1 -d' ' ./tmp/curentgraph.list > ./tmp/filename.field
		grep -v -f ./tmp/filename.field $KDISPLAY/curve/template/M_S_KKS_1GR.pag > $KWIN/$filename.pag
		rm ./tmp/filename.field
		cp $KDISPLAY/curve/template/M_S_KKS_1GR.bkg $KWIN/$filename.bkg
		cat ./tmp/curentgraph.list >> $KWIN/$filename.pag
		rm ./tmp/curentgraph.list
		print "$filename \c"
		echo "${filename} generated" >> $KLOG/kMakeCurve.log
	fi
	fi
else
echo $line >> ./tmp/curentgraph.list
fi
done
echo
sort -o ./tmp/curvename.list ./tmp/curvename.list
uniq -d ./tmp/curvename.list > ./tmp/multiple.list
if [ -s ./tmp/multiple.list ]
then
print "\n$star \n ERROR : Multiplied defined curves \n"
cat ./tmp/multiple.list
echo "$star"
print "\n$star \n ERROR : Multiplied defined curves \n" >> $KLOG/kMakeCurve.log
cat ./tmp/multiple.list >> $KLOG/kMakeCurve.log
echo "$star" >> $KLOG/kMakeCurve.log
fi
if [ -s ./tmp/$0.err ]
then
echo $star8 >> $KLOG/kMakeCurve.log
cat ./tmp/$0.err $KLOG/kMakeCurve.log
echo $star8 >> $KLOG/kMakeCurve.log
banner NOK
print "\a"
fi
rm -rf ./tmp
