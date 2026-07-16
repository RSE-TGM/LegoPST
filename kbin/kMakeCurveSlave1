#!/bin/ksh
#
Check=$1
cd $KGRAF
rm -f curve.tmp
echo $star > curve.tmp
echo "#" >> curve.tmp
print "#\tCurve File" >> curve.tmp
echo "#" >> curve.tmp
echo $star >> curve.tmp
print "\n$star8"
print "\tCurve list generation"
print "$star8\n"
cat grugraf.rtf | while read Group GroupDescr var1 var2 var3 var4 var5 var6 var7 var8 var9 var10 Indice
do
echo "****" >> curve.tmp
print "${Group} \c"
echo "${Group} add in curve list" >> $KLOG/kMakeCurve.log
print "*nomePag:\tM_S_${Group}_GR" >> curve.tmp
print "*top_descrizione:\t${GroupDescr}" >> curve.tmp
###############################################################################
if [ "$var1" != "&" ]
then
	IndiceCont="0"
	NumSig=`grep -w "$var1" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "\nERROR\t: Variable "$var1" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var1" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var1" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "\n\nERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var1 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var1" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var1" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var1" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var1" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var1" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var1Info:\t${LegoVar}|$var1|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var1Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var2" != "&" ]
then
	IndiceCont="1"
	NumSig=`grep -w "$var2" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var2" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var2" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var2" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var2 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var2" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var2" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var2" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var2" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var2" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var2Info:\t${LegoVar}|$var2|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var2Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var3" != "&" ]
then
	IndiceCont="2"
	NumSig=`grep -w "$var3" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var3" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var3" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var3" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var3 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var3" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var3" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var3" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var3" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var3" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var3Info:\t${LegoVar}|$var3|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var3Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var4" != "&" ]
then
	IndiceCont="3"
	NumSig=`grep -w "$var4" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var4" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var4" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var4" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var4 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var4" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var4" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var4" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var4" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var4" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var4Info:\t${LegoVar}|$var4|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var4Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var5" != "&" ]
then
	IndiceCont="4"
	NumSig=`grep -w "$var5" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var5" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var5" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var5" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var5 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var5" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var5" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var5" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var5" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var5" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var5Info:\t${LegoVar}|$var5|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var5Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var6" != "&" ]
then
	IndiceCont="5"
	NumSig=`grep -w "$var6" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var6" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var6" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var6" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var6 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var6" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var6" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var6" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var6" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var6" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var6Info:\t${LegoVar}|$var6|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var6Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var7" != "&" ]
then
	IndiceCont="6"
	NumSig=`grep -w "$var7" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var7" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var7" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var7" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var7 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var7" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var7" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var7" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var7" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var7" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var7Info:\t${LegoVar}|$var7|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var7Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var8" != "&" ]
then
	IndiceCont="7"
	NumSig=`grep -w "$var8" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var8" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var8" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var8" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var8 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var8" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var8" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var8" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var8" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var8" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var8Info:\t${LegoVar}|$var8|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var8Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var9" != "&" ]
then
	IndiceCont="8"
	NumSig=`grep -w "$var9" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var9" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var9" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var9" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var9 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var9" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var9" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var9" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var9" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var9" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var9Info:\t${LegoVar}|$var9|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var9Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
if [ "$var10" != "&" ]
then
	IndiceCont="9"
	NumSig=`grep -w "$var10" vargraf.rtf | wc -l | tr -d '[:blank:]' `
	if [ "$NumSig" != "1" ]
	then
	print "ERROR\t: Variable "$var10" not configurated in ACCESS database"
	print "ERROR\t: Variable "$var10" not configurated in ACCESS database" >> $KLOG/kMakeCurve.log
	banner "NOK"
	exit
	fi
	LegoVar=`grep -w "$var10" vargraf.rtf | cut -f6 -d " "`
		if [ "$Check" = "Check" ]
		then
		VariabliCheck=`grep -w "$LegoVar" $KSIM/variabili.edf | wc -l | tr -d '[:blank:]' `
		if [ "$VariabliCheck" = "0" ]
		then
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf"
		print "ERROR\t: Lego Variable "$LegoVar" not found in variabili.edf" >> $KLOG/kMakeCurve.log
		echo "$var10 $LegoVar" >> $KLOG/kMakeCurve.err
		LegoVar="NotFound"
		fi
		fi
	VarDescr=`grep -w "$var10" vargraf.rtf | cut -f2 -d " "`
	LowLimit=`grep -w "$var10" vargraf.rtf | cut -f3 -d " "`
	HighLimit=`grep -w "$var10" vargraf.rtf | cut -f4 -d " "`
	Unit=`grep -w "$var10" vargraf.rtf | cut -f5 -d " "`
	Type=`grep -w "$var10" vargraf.rtf | cut -f7 -d " "`
	print "*1w.var10Info:\t${LegoVar}|$var10|${VarDescr}|${LowLimit}|${HighLimit}|${Unit}|${Type}|" >> curve.tmp
else
	print "*1w.var10Info:\tLEGOname|KKS|KKSdescr|0.000|1.000|UnitMis|ENG|" >> curve.tmp
fi
###############################################################################
print "*1w.numCurvesInd:\t${IndiceCont}" >> curve.tmp
done
echo "****" >> curve.tmp
echo
tr '&' ' ' < curve.tmp > curve.list
rm curve.tmp

