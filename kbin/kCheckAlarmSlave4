#!/bin/ksh
#
#
kAddStatus kCheckAlarm4 UNKNOWN
echo ${star5}
echo ${star5} >> $KLOG/kCheckAlarm.log
kAddScreen kCheckAlarm "Verification n.4 : START"
kAddLog kCheckAlarm "Verification n.4 : START"
print "\n\tUpdating Nwin_XXX.list Hierarchy using sosti.dat file"
print "\n\tUpdating Nwin_XXX.list Hierarchy using sosti.dat file" >> $KLOG/kCheckAlarm.log
#
echo "Waiting ..."
rm -f $KLOG/sosti.log $KLOG/sosti2.log $KWIN/*.newhier
if [ -f $KWIN/sosti.dat ]
then
  ls $KWIN/N_win*.list | grep -v DFGK_S | while read file
  do
#      echo "Elaborating ${file} ..."
      echo "Elaborating ${file} ..." >> $KLOG/kCheckAlarm.log
      cp ${file} ${file}.kold 
      numcampohier=` grep "@#L@HIER" ${file} | tr -s '\011' '\12' | grep -n "@#L@HIER" | cut -f1 -d":" `
      
#	numcampo=` grep "@#L@HIER" ${file} | tr -s '\011' ' ' |  awk ' { print NF } '  `
      grep "@#L@HIER" ${file} > ${file}.newhier
      grep -v "@#" ${file} | while read riga 
      do
        stringa=` echo "${riga}" | tr -s '\011' ' ' | cut -f1,${numcampohier} -d' ' `
        owin=` echo "${stringa}" | cut -f1 -d' ' | cut -c3- `
        oldhier=`echo "${stringa}" | cut -f2 -d' '`
        newhier=`grep -w ${owin} $KWIN/sosti.dat | cut -f4 -d'"'`
        if [ "${oldhier}" = "" ]
        then
          echo ${owin} without hierarchy in ${file} >> $KLOG/sosti.log
          grep ${owin} ${file}.kold >> ${file}.newhier
        else
          if [ "${newhier}" = "" ]
          then
            echo ${owin} not found in database afsin >> $KLOG/sosti.log
            grep ${owin} ${file}.kold >> ${file}.newhier
          else
            grep ${owin} ${file}.kold | sed "s/${oldhier}/${newhier}/g" >> ${file}.newhier
            if [ "${newhier}" != ${oldhier} ]
            then 
            echo "@ Warning ${owin} Old : ${oldhier} New : ${newhier} @" >> $KLOG/sosti2.log
            fi
          fi
        fi
      done
      mv ${file}.newhier ${file}
  done
	if [ -f $KLOG/sosti2.log ]
	then
	echo Warnings in file $KLOG/sosti2.log
#	cat $KLOG/sosti2.log >> $KLOG/kCheckAlarm.log
	kAddStatus kCheckAlarm4 WARNING
	else
	kAddStatus kCheckAlarm4 OK
	fi
	if [ -f $KLOG/sosti.log ]
	then
	echo Errors in file $KLOG/sosti.log
	cat $KLOG/sosti.log >> $KLOG/kCheckAlarm.log
	kAddStatus kCheckAlarm4 ERROR
	banner NOK
#	exit
	fi
else
  echo File $KWIN/sosti.dat not found !!
fi
#
kAddScreen kCheckAlarm "Verification n.4 : END"
kAddLog kCheckAlarm "Verification n.4 : END"
print "${star5}\n"
print "${star5}\n" >> $KLOG/kCheckAlarm.log
