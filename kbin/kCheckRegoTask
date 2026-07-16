#!/bin/ksh
#
#!/bin/ksh
#
clear
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
if [ -f $KLOG/kCheckRegoTask.log ]
then
mv $KLOG/kCheckRegoTask.log $KLOG/kCheckRegoTask.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kCheckRegoTask.log
kAddScreen kCheckRegoTask Start
kAddLog kCheckRegoTask Start
#
donde=$1
if [ "${donde}" = "All" ]
then
cd $LEGOCAD_USER/legocad
REGO_TASK_LIST=`ls -d r_* | grep -v sps`
elif [ "${donde}" = "Sim" ]
then
REGO_TASK_LIST=`grep ^R $KSIM/al_sim.conf | tr -s '\011' ' ' |  cut -f3 -d" " | grep -v sps`
elif [ "${donde}" = "Local" ]
then
REGO_TASK_LIST=`pwd`
else
print "Correct Use :\n\tkCheckRegoTask  All/Sim/Local\a"
exit
fi
#
cd $LEGOCAD_USER/legocad
for task in ${REGO_TASK_LIST}
do
cd $task
echo ${star5}
echo "$task : Check rego task $1"
echo ${star5}
echo ${star5} >> $KLOG/kCheckRegoTask.log
echo "$task : Check rego task $1" >> $KLOG/kCheckRegoTask.log
echo ${star5} >> $KLOG/kCheckRegoTask.log
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s560 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XB86=`echo "${line}" | grep "_XB86" | wc -l | tr -d ' ' `
XC86=`echo "${line}" | grep "_XC86" | wc -l | tr -d ' ' `
XF51=`echo "${line}" | grep "_XF51" | wc -l | tr -d ' ' `
if [ "${XB86}" = "1" ] && [ "${XF51}" = "0" ]
then
print "${page}\tICI\t\t\t: OK ALARM\tSLAVE"
elif [ "${XC86}" = "1" ] && [ "${XF51}" = "1" ]
then
print "${page}\tICI\t\t\t: OK ALARM\tMASTER"
elif [ "${XB86}" = "1" ] && [ "${XF51}" = "1" ]
then
print "${page}\tICI\t\t\t:ERROR ALARM\tCONFIGURATION SLAVE"
print "${page}\tICI\t\t\t:ERROR ALARM\tCONFIGURATION SLAVE" >> $KLOG/kCheckRegoTask.log
else
print "${page}\tICI\t\t\t:ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tICI\t\t\t:ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s580 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XF51=`echo "${line}" | grep "_XF51" | wc -l | tr -d ' ' `
XC86=`echo "${line}" | grep "_XC86" | wc -l | tr -d ' ' `
if [ "${XF51}" = "1" ] && [ "${XC86}" = "1" ]
then
print "${page}\tCC-CONTROLE DRIVE\t: OK ALARM"
else
print "${page}\tCC-CONTROLE DRIVE\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tCC-CONTROLE DRIVE\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s587 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XF51=`echo "${line}" | grep "_XF51" | wc -l | tr -d ' ' `
XC86=`echo "${line}" | grep "_XC86" | wc -l | tr -d ' ' `
if [ "${XF51}" = "1" ] && [ "${XC86}" = "1" ]
then
print "${page}\tCC-CONTROLE DRIVE\t: OK ALARM"
else
print "${page}\tCC-CONTROLE DRIVE\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tCC-CONTROLE DRIVE\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s531 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XZ01=`echo "${line}" | grep "_XZ01" | wc -l | tr -d ' ' `
KKS=`grep w.labelText: ${page} | grep '[012BMX][A-Z]..[0-9]......' | cut -f2`
KKSLINE=`echo "${KKS}" | wc -l | tr -s '\011' ' '| tr -d ' ' `
if [ "${KKSLINE}" = "1" ]
then
KKSTYPE=`echo "${KKS}" | cut -c1`
else
KKSTYPE="X"
print "ERROR : The page ${page} contains more than one KKS label text ...\a"
echo "ERROR : The page ${page} contains more than one KKS label text ..." >> $KLOG/kCheckRegoTask.log
fi
if [ "${XZ01}" = "1" ]
then
print "${page}\tMEASURE\t\t\t: OK ALARM"
elif [ ! "${KKSTYPE}" = "X" ]
then
print "${page}\tMEASURE\t\t\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tMEASURE\t\t\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s141 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XA05=`echo "${line}" | grep "_XA05" | wc -l | tr -d ' ' `
if [ "${XA05}" = "1" ]
then
print "${page}\tSUB LOOP CTRL\t: OK ALARM"
else
print "${page}\tSUB LOOP CTRL\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tSUB LOOP CTRL\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s138 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XZ01=`echo "${line}" | grep "_XZ01" | wc -l | tr -d ' ' `
if [ "${XZ01}" = "1" ]
then
print "${page}\tSUB GROUP CTRL\t\t: OK ALARM"
else
print "${page}\tSUB GROUP CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tSUB GROUP CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s111 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
XZ01=`echo "${line}" | grep "_XZ01" | wc -l | tr -d ' ' `
if [ "${XZ01}" = "1" ]
then
print "${page}\tGROUP CTRL\t\t: OK ALARM"
else
print "${page}\tGROUP CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED"
print "${page}\tGROUP CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s176 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
CCONS=`grep "${tag}" ${page} | grep labelText | grep "CCON-S" | tr ':'  ' ' | cut -f2 | wc -l | tr -s '\011' ' '| tr -d ' '`
XF52=`echo "${line}" | grep "_XF52" | wc -l | tr -d ' ' `
if [ "${CCONS}" = "1" ]
then
	if [ "${XF52}" = "1" ]
	then
	print "${page}\tSLAVE CTRL\t\t: ERROR ALARM\tXF52 NOT NECESSARY FOR SLAVE"
	print "${page}\tSLAVE CTRL\t\t: ERROR ALARM\tXF52 NOT NECESSARY FOR SLAVE" >> $KLOG/kCheckRegoTask.log
	else
	print "${page}\tSLAVE CTRL\t\t: OK ALARM"
	fi
else
	if [ "${XF52}" = "1" ]
	then
	print "${page}\tMASTER CTRL\t\t: OK ALARM"
	else
	print "${page}\tMASTER CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED"
	print "${page}\tMASTER CTRL\t\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
	fi
fi
done
###############################################################################
echo "${star2}"
echo "${star2}"  >> $KLOG/kCheckRegoTask.log
grep s087 ????.pag | grep modulName | tr ':'  ' ' | sed  "s/.modulName/ /g" | tr -s '\011' ' ' | while read page tag poubelle
do
line=`grep "${tag}.inputValue:" ${page}`
MASTER=`grep "${tag}" ${page} | grep labelText | grep MASTER | tr ':'  ' ' | cut -f2 | wc -l | tr -s '\011' ' '| tr -d ' '`
#SETP=`grep "${tag}" ${page} | grep labelText | grep ADJUSTER | tr ':'  ' ' | cut -f2 | wc -l | tr -s '\011' ' '| tr -d ' '`
icon=`grep "${tag}.nomeFile:" ${page}`
XZ01=`echo "${line}" | grep "_XZ01" | wc -l | tr -d ' ' `
if [ "${MASTER}" = "1" ]
then
	if [ "${XZ01}" = "1" ]
	then
	print "${page}\tMASTER CONTROLLER\t: ERROR ALARM\tXZ01 NOT NECESSARY FOR MASTER CONTROLLER"
	print "${page}\tMASTER CONTROLLER\t: ERROR ALARM\tXZ01 NOT NECESSARY FOR MASTER CONTROLLER" >> $KLOG/kCheckRegoTask.log
	else
	print "${page}\tMASTER CONTROLLER\t: OK ALARM"
	fi
else
	if [ "${XZ01}" = "1" ]
	then
	print "${page}\tSETPOINT ADJUSTER\t: OK ALARM"
	else
	print "${page}\tSETPOINT ADJUSTER\t: ERROR ALARM\tNOT CONFIGURATED"
	print "${page}\tSETPOINT ADJUSTER\t: ERROR ALARM\tNOT CONFIGURATED" >> $KLOG/kCheckRegoTask.log
	fi
fi
done
###############################################################################
cd $LEGOCAD_USER/legocad
done
print "\a"
#
kAddScreen kCheckRegoTask End 
kAddLog kCheckRegoTask End
echo "$star"
echo "$star" >> $KLOG/kCheckRegoTask.log
print "Log file\t: $KLOG/kCheckRegoTask.log"
