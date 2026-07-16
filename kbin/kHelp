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
if [ -f $KLOG/kHelp.log ]
then
mv $KLOG/kHelp.log $KLOG/kHelp.log.kold
fi
kAddScreen kHelp Start
kAddLog kHelp Start
#
cat <<EOF

		$0
		-------------------------

		1.- Simulator Building
		2.- Simulator Running
		3.- Configuration Utilities
		4.- User Utilities
		5.- Cleaning Utilities
		
		0.- Exit


EOF
print "\tSelection ==> \c"
read option
case $option in
     0) exit
      ;;
     1) volume="1"
      ;;
     2) volume="2"
      ;;
     3) volume="3"
      ;;
     4) volume="4"
      ;;
     5) volume="5"
      ;;
esac
#
###################################################################################################
#	Interactive Mode
###################################################################################################
if [ -f ${KHELP}/kHelp${volume}.list ]
then
cat ${KHELP}/kHelp${volume}.list | while read menu
do
if [ "${K_HELP_ITEM1}" = "" ]
then
K_HELP_ITEM1=${menu}
elif [ "${K_HELP_ITEM2}" = "" ]
then
K_HELP_ITEM2=${menu}
elif [ "${K_HELP_ITEM3}" = "" ]
then
K_HELP_ITEM3=${menu}
elif [ "${K_HELP_ITEM4}" = "" ]
then
K_HELP_ITEM4=${menu}
elif [ "${K_HELP_ITEM5}" = "" ]
then
K_HELP_ITEM5=${menu}
elif [ "${K_HELP_ITEM6}" = "" ]
then
K_HELP_ITEM6=${menu}
elif [ "${K_HELP_ITEM7}" = "" ]
then
K_HELP_ITEM7=${menu}
elif [ "${K_HELP_ITEM8}" = "" ]
then
K_HELP_ITEM8=${menu}
elif [ "${K_HELP_ITEM9}" = "" ]
then
K_HELP_ITEM9=${menu}
elif [ "${K_HELP_ITEM10}" = "" ]
then
K_HELP_ITEM10=${menu}
elif [ "${K_HELP_ITEM11}" = "" ]
then
K_HELP_ITEM11=${menu}
elif [ "${K_HELP_ITEM12}" = "" ]
then
K_HELP_ITEM12=${menu}
elif [ "${K_HELP_ITEM13}" = "" ]
then
K_HELP_ITEM13=${menu}
elif [ "${K_HELP_ITEM14}" = "" ]
then
K_HELP_ITEM14=${menu}
elif [ "${K_HELP_ITEM15}" = "" ]
then
K_HELP_ITEM15=${menu}
elif [ "${K_HELP_ITEM16}" = "" ]
then
K_HELP_ITEM16=${menu}
elif [ "${K_HELP_ITEM17}" = "" ]
then
K_HELP_ITEM17=${menu}
elif [ "${K_HELP_ITEM18}" = "" ]
then
K_HELP_ITEM18=${menu}
elif [ "${K_HELP_ITEM19}" = "" ]
then
K_HELP_ITEM19=${menu}
elif [ "${K_HELP_ITEM20}" = "" ]
then
K_HELP_ITEM20=${menu}
elif [ "${K_HELP_ITEM21}" = "" ]
then
K_HELP_ITEM21=${menu}
elif [ "${K_HELP_ITEM22}" = "" ]
then
K_HELP_ITEM22=${menu}
elif [ "${K_HELP_ITEM23}" = "" ]
then
K_HELP_ITEM23=${menu}
elif [ "${K_HELP_ITEM24}" = "" ]
then
K_HELP_ITEM24=${menu}
elif [ "${K_HELP_ITEM25}" = "" ]
then
K_HELP_ITEM25=${menu}
else
print "\aWarning Task Number > 15"
fi
done
cat <<EOF

		$0
		-------------------------

		1.- 
		2.- 
		3.- ${K_HELP_ITEM1}
		4.- ${K_HELP_ITEM2}
		5.- ${K_HELP_ITEM3}
		6.- ${K_HELP_ITEM4}
		7.- ${K_HELP_ITEM5}
		8.- ${K_HELP_ITEM6}
		9.- ${K_HELP_ITEM7}
		10.- ${K_HELP_ITEM8}
		11.- ${K_HELP_ITEM9}
		12.- ${K_HELP_ITEM10}
		13.- ${K_HELP_ITEM11}
		14.- ${K_HELP_ITEM12}
		15.- ${K_HELP_ITEM13}
		16.- ${K_HELP_ITEM14}
		17.- ${K_HELP_ITEM15}
		18.- ${K_HELP_ITEM16}
		19.- ${K_HELP_ITEM17}
		20.- ${K_HELP_ITEM18}
		21.- ${K_HELP_ITEM19}
		22.- ${K_HELP_ITEM20}
		23.- ${K_HELP_ITEM21}
		24.- ${K_HELP_ITEM22}
		25.- ${K_HELP_ITEM23}
		26.- ${K_HELP_ITEM24}
		27.- ${K_HELP_ITEM25}
		
		0.- Exit


EOF
print "\tSelection ==> \c"
read option
case $option in
     0) exit;
      ;;
     1) exit
      ;;
     2) exit
      ;;
     3) K_HELP_ITEM_LIST=${K_HELP_ITEM1}
      ;;
     4) K_HELP_ITEM_LIST=${K_HELP_ITEM2}
      ;;
     5) K_HELP_ITEM_LIST=${K_HELP_ITEM3}
      ;;
     6) K_HELP_ITEM_LIST=${K_HELP_ITEM4}
      ;;
     7) K_HELP_ITEM_LIST=${K_HELP_ITEM5}
      ;;
     8) K_HELP_ITEM_LIST=${K_HELP_ITEM6}
      ;;
     9) K_HELP_ITEM_LIST=${K_HELP_ITEM7}
      ;;
     10) K_HELP_ITEM_LIST=${K_HELP_ITEM8}
      ;;
     11) K_HELP_ITEM_LIST=${K_HELP_ITEM9}
      ;;
     12) K_HELP_ITEM_LIST=${K_HELP_ITEM10}
      ;;
     13) K_HELP_ITEM_LIST=${K_HELP_ITEM11}
      ;;
     14) K_HELP_ITEM_LIST=${K_HELP_ITEM12}
      ;;
     15) K_HELP_ITEM_LIST=${K_HELP_ITEM13}
      ;;
     16) K_HELP_ITEM_LIST=${K_HELP_ITEM14}
      ;;
     17) K_HELP_ITEM_LIST=${K_HELP_ITEM15}
      ;;
     18) K_HELP_ITEM_LIST=${K_HELP_ITEM16}
      ;;
     19) K_HELP_ITEM_LIST=${K_HELP_ITEM17}
      ;;
     20) K_HELP_ITEM_LIST=${K_HELP_ITEM18}
      ;;
     21) K_HELP_ITEM_LIST=${K_HELP_ITEM19}
      ;;
     22) K_HELP_ITEM_LIST=${K_HELP_ITEM20}
      ;;
     23) K_HELP_ITEM_LIST=${K_HELP_ITEM21}
      ;;
     24) K_HELP_ITEM_LIST=${K_HELP_ITEM22}
      ;;
     25) K_HELP_ITEM_LIST=${K_HELP_ITEM23}
      ;;
     26) K_HELP_ITEM_LIST=${K_HELP_ITEM24}
      ;;
     27) K_HELP_ITEM_LIST=${K_HELP_ITEM25}
      ;;
esac
###################################################################################################
#	Background Mode
###################################################################################################
else
  echo "File ${KHELP}/kHelp${volume}.list not found"
fi
###################################################################################################
kAddScreen kHelp End
kAddLog kHelp End
echo "Log File : $KLOG/kHelp.log"
