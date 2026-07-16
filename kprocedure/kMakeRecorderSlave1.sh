#!/bin/ksh
#
# Variables extraction from recorder.edf_*
if [ ! -f $KSIM/recorder.edf_* ]
then
print "WARNING : $KSIM/recorder.edf_* not found\a"
sleep 2
exit
fi
ls $KSIM/recorder.edf_* | while read GroupRecorder
do
count=0
next=model
print "\n${star6}\nVariables extraction from ${GroupRecorder}\n${star6}\n" 
print "\n${star6}\nVariables extraction from ${GroupRecorder}\n${star6}\n" >> $KLOG/kMakeRecorder.log
cat ${GroupRecorder} |
while read var
do
	if [ $count -lt 3 ]
	then
		if [ "${var}" = '****' ]
		then 
		let count=count+1 
		fi
	continue
	elif [ "$next" = "model" ]
	then
	ModelName=`echo ${var} | cut -f2 -d" "`
	echo "Variables extracted for task ${ModelName} :"
	elif  [ "${var}" != '****' ]
	then
	echo "${var}" >> $KSIM/kRecorderSupportDirectory/${ModelName}
	echo "${var}"
	fi
		if [ "${var}" = '****' ]
		then 
		next=model
		touch $KSIM/kRecorderSupportDirectory/${ModelName}
		else
		next=variable
		fi
done
done
