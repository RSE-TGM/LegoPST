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
if [ -f $KLOG/kUpDatePD.log ]
then
mv $KLOG/kUpDatePD.log $KLOG/kUpDatePD.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kUpDatePD.log
kAddScreen kUpDatePD Start
kAddLog kUpDatePD Start
#
rm -rf $KBASIC
mkdir $KBASIC
cd $KBASIC
if [ -f $KSIM/pd.list ]
then
	cat $KSIM/pd.list | while read pdkks
	do
		for fa in $KFUNCTIONALAREA
		do
			for directory in $KDIRPD $KDIRGR
			do
			if [ -f $directory/${fa}/pag/M_S_${pdkks}.pag ]
			then
			echo "Updating ${pdkks} (${fa})"
			echo "Updating ${pdkks} (${fa})" >> $KLOG/kUpDatePD.log
			cp $directory/${fa}/pag/M_S_${pdkks}.pag .
			cp $directory/${fa}/pag/M_S_${pdkks}.bkg .
			fi
			done
		done	
	done
else
	echo "Error : File $KSIM/pd.list not found ! "
	echo "Error : File $KSIM/pd.list not found ! " >> $KLOG/kUpDatePD.log
fi
pdlistnumber=`cat $KSIM/pd.list | wc -l `
pdlocalnumber=`ls $KBASIC/*pag | wc -l`
if [ "${pdlocalnumber}" = "${pdlistnumber}" ]
then
echo "${pdlocalnumber} plant displaies have been copied"
else
print "${star3}\n\tERROR :\n\tThe file pd.list contains "${pdlistnumber}" plant displaies, but\
 only "${pdlocalnumber}" plant\n\tdisplaies have been found in the\
 directories : \n\t\t$KDIRPD\n\t\t$KDIRGR\a\n${star3}"
print "${star3}\n\tERROR :\n\tThe file pd.list contains "${pdlistnumber}" plant displaies, but\
 only "${pdlocalnumber}" plant\n\tdisplaies have been found in the\
 directories : \n\t\t$KDIRPD\n\t\t$KDIRGR\a\n${star3}" >> $KLOG/kUpDatePD.log
banner "NOK"
banner "NOK" >> $KLOG/kUpDatePD.log
exit
fi
###############################################################################
#	Context.ctx creation
###############################################################################
cd ${KBASIC}
if [ -f $KBASIC/Context.ctx ]
then
mv  $KBASIC/Context.ctx $KBASIC/Context.ctx.kold
fi
echo "Context.ctx generation ..."
export list_page=`ls | grep pag$ | grep -v place | grep -v REG_INT_CONN_DB | \
grep -v Compress | cut -f1 -d'.' | tr "\n" " "`
export num_page=`echo $list_page | wc -w`

ls | grep pag$ | grep -v place | grep -v REG_INT_CONN_DB | \
grep -v Compress | \
while read pagina
do
    echo "Loading ${pagina} ..."
    tag=`echo ${pagina} | cut -f1 -d'.'`
    echo '*'${tag}.`grep 'tagPag:' ${pagina} | cut -c2-` >> Context.ctx
    echo '*'${tag}.`grep 'top_descrizione:' ${pagina} | cut -c2-` >> Context.ctx
    echo '*'${tag}.`grep 'schemeInUse:' ${pagina} | cut -c2-` >> Context.ctx
    stringa=`grep 'gerarchia:' ${pagina}`
    if [ "${stringa}" = "" ]
    then
      print "*${tag}.gerarchia:\t-1,-1,-1,-1,-1,-1" >> Context.ctx
    else
      echo '*'${tag}.`grep 'gerarchia:' ${pagina} | cut -c2-` >> Context.ctx
    fi
    echo '*'${tag}.`grep 'refresh_freq:' ${pagina} | cut -c2-` >> Context.ctx
    echo '*'${tag}.`grep 'top_tipo:' ${pagina} | cut -c2-` >> Context.ctx
done
cat >> Context.ctx << !EOF
*hostName:	${HOST}
*hostNameS:	${HOST}
*iconlib_list:	\\ 
*displayList:	
*numDisplay:	0
*iconlib_num:	0
*description:	Plant Display
*iconlib_label:	\\ 
*nextTagPag:	00
*simulator:	${KSIM}
*pages:		${KBASIC}
*objectLibraries:	$HOME/legocad/libut_reg/libreg
*animatedIconLibraries:	$HOME/legocad/libut_reg/libreg
*pag_num:	${num_page}
*page_list:	${list_page}
!EOF
print "\n${num_page} mmi pages have been added in Context.ctx\n"
print "\n${num_page} mmi pages have been added in Context.ctx\n" >> $KLOG/kUpDatePD.log
###############################################################################
kAddScreen kUpDatePD End 
kAddLog kUpDatePD End
echo "$star"
echo "$star" >> $KLOG/kUpDatePD.log
print "Log file\t: $KLOG/kUpDatePD.log"
