#!/bin/ksh
#
clear
kTest
echo $star7
echo
print "\t\t\tSimulator Installation\a"
echo
echo $star7
print "\n\t$KREV\n\t$KDESCR"
#kpresentation
CONFIRM=""
export CONFIRM
echo ""
print "\tPlease confirm $KSIMNAME simulator file updating"
print "\tin $KSIM directory.\n"
print "\tPress  y to confirm"
read CONFIRM
if [ ! "${CONFIRM}" = "y" ]
then
	print "\nSorry ...\a\n"
	exit
fi
echo
if [ -d $KSTATUS ]
then
rm -rf $KSTATUS
fi
if [ -d $KLOG ]
then
rm -rf $KLOG
fi
rm -f $KSTATISTIC/*.statistic
kAddStatistic kInstall
###############################################################################
#	UpDating User environement
###############################################################################
kMakeUser
###############################################################################
#	UpDating UserName
###############################################################################
kUpDateUserName
###############################################################################
#	UpDating HostName
###############################################################################
kUpDateHostName
###############################################################################
#	UpDating Globpages
###############################################################################
kMmiConfig
###############################################################################
#	UpDating Rego Task
###############################################################################
#kCompile Task Sim
###############################################################################
#	UpDating variabili.rtf
###############################################################################
echo UpDating variabili.rtf
cd $KSIM
export DEBUG=NO
kNetCompi
export DEBUG=YES
###############################################################################


