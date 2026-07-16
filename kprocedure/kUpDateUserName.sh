#!/bin/ksh
#
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
#
###############################################################################
#	Old User
###############################################################################
olduser=${USER}
cd $LEGOCAD_USER/legocad
REGO_TASK_LIST=`ls -d r_*`
for task in ${REGO_TASK_LIST}
do
cd $task
if [ -f Context.ctx ]
then
olduserForse=`grep simulator: Context.ctx | awk -v FS="/" ' {print $4} '`
if [ ! "${olduserForse}" = "${USER}" ]
then
olduser=${olduserForse}
fi
#echo $olduser
fi
cd $LEGOCAD_USER/legocad
done
if [ "${olduser}" = "" ]
then
exit
fi
newusersdir=`echo $HOME | cut -f3 -d"/" | grep users`
if [ "${newusersdir}" = "" ]
then
print "New users directory not automaticaly identified\a"
exit
fi
echo ${star5}
if [ "${olduser}" = "${USER}" ]
then
	if [ ! "${1}" = "" ]
	then
	olduser=$1
	else
	echo "WARNING : Context.ctx already updated"
	echo "Alternative use : kUpDateUserName old_user"
	exit
	fi
fi	
print "Old user\t: ${olduser}"
print "New user\t: ${USER}"
###############################################################################
#	Simulator
###############################################################################
echo "${star5}"
echo "Globpages Context.ctx updating"
echo "${star5}"
if [ -f $KPAGES/Context.ctx ]
then
cd $KPAGES
cp Context.ctx Context.ctx.kold
sed  "s/\/users\//\/${newusersdir}\//g" Context.ctx > Context.ctx.tmp1 
sed  "s/\/users1\//\/${newusersdir}\//g" Context.ctx.tmp1 > Context.ctx.tmp2
sed  "s/\/users2\//\/${newusersdir}\//g" Context.ctx.tmp2 > Context.ctx.tmp3
sed  "s/\/users3\//\/${newusersdir}\//g" Context.ctx.tmp3 > Context.ctx.tmp4
sed  "s/\/users4\//\/${newusersdir}\//g" Context.ctx.tmp4 > Context.ctx.tmp5
sed  "s/${olduser}/${USER}/g" Context.ctx.tmp5 > Context.ctx
rm -f Context.ctx.new Context.ctx.tmp1 Context.ctx.tmp2 Context.ctx.tmp3 Context.ctx.tmp4 Context.ctx.tmp5
else
print "WARNING : Globpages Context.ctx not found\a"
fi
#
echo UpDating kMmi.cfg ...
if [ -f $KSIM/kMmi.cfg ]
then
cp $KSIM/kMmi.cfg $KSIM/kMmi.cfg.kold
sed "s/${olduser}/${USER}/g" $KSIM/kMmi.cfg.kold > $KSIM/kMmi.cfg
else
echo "INFO : File kMmi.cfg not found"
fi
###############################################################################
#	Regolation Task
###############################################################################
echo "${star5}"
echo "Regolation Task Context.ctx updating"
echo "${star5}"
cd $LEGOCAD_USER/legocad
REGO_TASK_LIST=`ls -d r_*`
for task in ${REGO_TASK_LIST}
do
print "$task \c"
cd $task
if [ -f Context.ctx ]
then
cp Context.ctx Context.ctx.kold
sed  "s/\/users\//\/${newusersdir}\//g" Context.ctx > Context.ctx.tmp1 
sed  "s/\/users1\//\/${newusersdir}\//g" Context.ctx.tmp1 > Context.ctx.tmp2
sed  "s/\/users2\//\/${newusersdir}\//g" Context.ctx.tmp2 > Context.ctx.tmp3
sed  "s/\/users3\//\/${newusersdir}\//g" Context.ctx.tmp3 > Context.ctx.tmp4
sed  "s/\/users4\//\/${newusersdir}\//g" Context.ctx.tmp4 > Context.ctx.tmp5
sed  "s/${olduser}/${USER}/g" Context.ctx.tmp5 > Context.ctx
rm -f Context.ctx.new Context.ctx.tmp1 Context.ctx.tmp2 Context.ctx.tmp3 Context.ctx.tmp4 Context.ctx.tmp5
else
print "WARNING : Context.ctx not found\a"
fi
cd $LEGOCAD_USER/legocad
done
###############################################################################
#	Appsid Task
###############################################################################
print "\n${star5}"
echo "task.info updating"
echo "${star5}"
cd ${TASKROOT}
REGO_TASK_LIST=`ls -d gi*`
for task in ${REGO_TASK_LIST}
do
echo "$task"
cd ${task}/work
	if [ -f task.info ]
	then
#	echo "task.info updated"
	cp task.info task.info.kold
	sed  "s/\/users\//\/${newusersdir}\//g" task.info > task.info.tmp1 
	sed  "s/\/users1\//\/${newusersdir}\//g" task.info.tmp1 > task.info.tmp2
	sed  "s/\/users2\//\/${newusersdir}\//g" task.info.tmp2 > task.info.tmp3
	sed  "s/\/users3\//\/${newusersdir}\//g" task.info.tmp3 > task.info.tmp4
	sed  "s/\/users4\//\/${newusersdir}\//g" task.info.tmp4 > task.info.tmp5
	sed  "s/${olduser}/${USER}/g" task.info.tmp5 > task.info
	rm -f task.info.new task.info.tmp1 task.info.tmp2 task.info.tmp3 task.info.tmp4 task.info.tmp5
	else
	print "WARNING : task.info not found\a"
	fi
	if [ -f task.info.100 ]
	then
#	echo "task.info.100 updated"
	cp task.info.100 task.info.100.kold
	sed  "s/\/users\//\/${newusersdir}\//g" task.info.100 > task.info.100.tmp1 
	sed  "s/\/users1\//\/${newusersdir}\//g" task.info.100.tmp1 > task.info.100.tmp2
	sed  "s/\/users2\//\/${newusersdir}\//g" task.info.100.tmp2 > task.info.100.tmp3
	sed  "s/\/users3\//\/${newusersdir}\//g" task.info.100.tmp3 > task.info.100.tmp4
	sed  "s/\/users4\//\/${newusersdir}\//g" task.info.100.tmp4 > task.info.100.tmp5
	sed  "s/${olduser}/${USER}/g" task.info.100.tmp5 > task.info.100
	rm -f task.info.100.new task.info.100.tmp1 task.info.100.tmp2 task.info.100.tmp3 task.info.100.tmp4 task.info.100.tmp5
	else
	print "WARNING : task.info.100 not found\a"
	fi
cd ${TASKROOT}
done
#
