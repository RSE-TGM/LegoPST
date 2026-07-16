#!/bin/ksh
#
rm -f pagmod.log
star="*******************************************************"
REMOTE_PARAMETERS_LIST="malf_meas malf_dump malf_pump malf_elect malf_motor malf_valv pipe_burst rel86 hand_op_dev_no hand_op_dev_nz hand_op_dev_nc boiler flow_contr_branch rem_subgrcontr analog_remote_par motor trasformer generator"
# Make User Level Script
        echo "#" > UserLevel.scr
        echo "#" User Level Script >> UserLevel.scr
        echo "#" >> UserLevel.scr
for bitmapName in $REMOTE_PARAMETERS_LIST
do
        echo "#" Bitmap Name : ${bitmapName} >> UserLevel.scr
        echo widgetclass=ChangePage >> UserLevel.scr
        echo hasattrib=bitmapName >> UserLevel.scr
        echo value=${bitmapName} >> UserLevel.scr
#       echo newattrib=userLevel >> UserLevel.scr 
#       echo value=1 >> UserLevel.scr 
        echo attrib=userLevel >> UserLevel.scr
        echo value=1 >> UserLevel.scr
        echo "#" >> UserLevel.scr
done
# Elaborating New Page
  if [ -f "UserLevel.scr" ]
  then
    ls M_S_*.pag | while read plantd
    do
    if [ ! -f ${plantd}.AntesUserLevel ]
    then
    	print "$star\nElaborating ${plantd}\n$star"
    	cp ${plantd} ${plantd}.AntesUserLevel
    	pagmod.1.4 -p ${plantd} -s UserLevel.scr
    	mv ${plantd}.mod ${plantd}
    else
    	echo Warning : ${plantd} not elaborated because ${plantd}.AntesUserLevel already exists !!!!
    fi
    done
  else
    echo UserLevel.scr not found
    exit
  fi
# Cleaning
rm -f config.log
rm -f core
print "\a"
rm -i UserLevel.scr
print "\a"
rm -i pagmod.log
