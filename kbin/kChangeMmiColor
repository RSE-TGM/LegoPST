#!/bin/ksh
#
###############################################################################
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kChangeMmiColor.log ]
then
mv $KLOG/kChangeMmiColor.log $KLOG/kChangeMmiColor.log.kold
fi
#
echo ${star}
echo ${star} >> $KLOG/kChangeMmiColor.log
kAddScreen kChangeMmiColor Start
kAddLog kChangeMmiColor Start
kAddStatus kChangeMmiColor Start
kAddLog kLogger "Start kChangeMmiColor"
kAddStatistic kChangeMmiColor
#
cd $KWIN
rm -f *.AntesChangeMMIColor
# Color Definition
#kBluSim=#2828aaaaffff
kBluSim=#6e6e6e6effff
kGreyBackground=#b6b6b6b6b6b6
kGreyBackgroundDirect=#b4b4b4b4b4b4
kGreyBackgroundLed="#cbcbcbcbcbcb 0.000000 0"
kSoftGreenLed="#6f6fb6b63333 0.000000 0"
kSuperGreenLed=#000089890000
kOldCurveBackground=#ffffd2d20000
kNewCurveBackground=#ffff63634747
rm -f config.log
rm -f pagmod.log
rm -f ChangeMMIColor.scr
star="*******************************************************"
# Make ChangeMMIColor Script
        echo "#" > ChangeMMIColor.scr
        echo "#" ChangeMMIColor Script >> ChangeMMIColor.scr
        echo "#" >> ChangeMMIColor.scr
        echo "#" Misurator with OW >> ChangeMMIColor.scr
        echo widgetclass=IndicTelep >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=4 >> ChangeMMIColor.scr
        echo hasattrib=background >> ChangeMMIColor.scr
        echo value=$kGreyBackground >> ChangeMMIColor.scr
        echo attrib=background >> ChangeMMIColor.scr
        echo value=$kBluSim >> ChangeMMIColor.scr
#
        echo "#" Misurator with Link Direct >> ChangeMMIColor.scr
        echo widgetclass=Indic >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=2 >> ChangeMMIColor.scr
        echo hasattrib=background >> ChangeMMIColor.scr
        echo value=$kGreyBackgroundDirect >> ChangeMMIColor.scr
        echo attrib=background >> ChangeMMIColor.scr
        echo value=$kBluSim >> ChangeMMIColor.scr
#        
        echo "#" Problem with SGC number >> ChangeMMIColor.scr
        echo widgetclass=Indic >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=2 >> ChangeMMIColor.scr
        echo hasattrib=width0 >> ChangeMMIColor.scr
        echo value=13 >> ChangeMMIColor.scr
        echo attrib=background >> ChangeMMIColor.scr
        echo value=#000000000000 >> ChangeMMIColor.scr
#
        echo "#" Problem with SELECT number >> ChangeMMIColor.scr
        echo widgetclass=Indic >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=2 >> ChangeMMIColor.scr
        echo hasattrib=width0 >> ChangeMMIColor.scr
        echo value=11 >> ChangeMMIColor.scr
        echo attrib=background >> ChangeMMIColor.scr
        echo value=#0000a0a00000 >> ChangeMMIColor.scr
#
        echo "#" Led Rectangolar >> ChangeMMIColor.scr
        echo widgetclass=IndicTelep >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=0 >> ChangeMMIColor.scr
        echo hasattrib=coloreVarOff >> ChangeMMIColor.scr
        echo value=$kGreyBackgroundLed >> ChangeMMIColor.scr
        echo attrib=coloreVarOff >> ChangeMMIColor.scr
        echo value=#ffffffffffff >> ChangeMMIColor.scr
#
        echo "#" Led Rectangolar Green >> ChangeMMIColor.scr
        echo widgetclass=IndicTelep >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=0 >> ChangeMMIColor.scr
        echo hasattrib=coloreVarOn >> ChangeMMIColor.scr
        echo value=$kSoftGreenLed >> ChangeMMIColor.scr
        echo attrib=coloreVarOn >> ChangeMMIColor.scr
        echo value=$kSuperGreenLed >> ChangeMMIColor.scr
#
        echo "#" Led Circular >> ChangeMMIColor.scr
        echo widgetclass=IndicTelep >> ChangeMMIColor.scr
        echo hasattrib=tipoInd >> ChangeMMIColor.scr
        echo value=1 >> ChangeMMIColor.scr
        echo hasattrib=coloreVarOff >> ChangeMMIColor.scr
        echo value=$kGreyBackgroundLed >> ChangeMMIColor.scr
        echo attrib=coloreVarOff >> ChangeMMIColor.scr
        echo value=#ffffffffffff >> ChangeMMIColor.scr
#
        echo "#" Curve Change Page >> ChangeMMIColor.scr
        echo widgetclass=ChangePage >> ChangeMMIColor.scr
        echo hasattrib=bitmapName >> ChangeMMIColor.scr
        echo value=curve >> ChangeMMIColor.scr
        echo hasattrib=background >> ChangeMMIColor.scr
        echo value=$kOldCurveBackground >> ChangeMMIColor.scr
        echo attrib=background >> ChangeMMIColor.scr
        echo value=$kNewCurveBackground >> ChangeMMIColor.scr
        echo attrib=buttonBackground >> ChangeMMIColor.scr
        echo value=$kNewCurveBackground >> ChangeMMIColor.scr
#
# Elaborating New Page
  if [ -f "ChangeMMIColor.scr" ]
  then
    ls M_S_*.pag | grep -v GR | while read plantd
    do
    if [ ! -f ${plantd}.AntesChangeMMIColor ]
    then
    	print "$star\nElaborating ${plantd}\n$star" >> $KLOG/kChangeMmiColor.log
    	cp ${plantd} ${plantd}.AntesChangeMMIColor
    	pagmod.1.4 -p ${plantd} -s ChangeMMIColor.scr
    	mv ${plantd}.mod ${plantd}
    else
    	echo "WARNING : ${plantd} not elaborated because ${plantd}.AntesChangeMMIColor already exists !!!!" >> $KLOG/kChangeMmiColor.log
    fi
    done
  else
    echo ChangeMMIColor.scr not found
    exit
  fi
# Cleaning
rm -f config.log
rm -f pagmod.log
rm -f ChangeMMIColor.scr
rm -f core
rm -f *.AntesChangeMMIColor
#
echo "$star"
echo "$star" >> $KLOG/kChangeMmiColor.log
kAddScreen kChangeMmiColor End
kAddLog kChangeMmiColor End
kAddStatus kChangeMmiColor End
kAddLog kLogger "End kChangeMmiColor"
echo "$star"
echo "$star" >> $KLOG/kChangeMmiColor.log
#
