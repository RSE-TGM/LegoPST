#!/bin/ksh
#
clear
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status `
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit
fi
kpresentation
if [ -f $KLOG/kWinContext.log ]
then
mv $KLOG/kWinContext.log $KLOG/kWinContext.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kWinContext.log
kAddScreen kWinContext Start
kAddLog kWinContext Start
#
if [ -f $KSIM/al_sim.conf ]
then
   export HOST_MMI=`grep "MMI_HOSTNAME=" $KSIM/al_sim.conf | cut -f2 -d=`
   export HOST_SCADA=`grep "MMI_HOSTNAME_SCADA" $KSIM/al_sim.conf | cut -f2 -d=`
fi
cd ${KWIN}
if [ -f $KWIN/Context.ctx ]
then
mv  $KWIN/Context.ctx $KWIN/Context.ctx.kold
fi
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
*hostName:	${HOST_MMI}
*hostNameS:	${HOST_SCADA}
*iconlib_list:	\\ 
*displayList:	
*numDisplay:	0
*iconlib_num:	0
*description:	Operating Window and Plant Display
*iconlib_label:	\\ 
*nextTagPag:	00
*simulator:	${KSIM}
*pages:		${KWIN}
*objectLibraries:	$HOME/legocad/libut_reg/libreg
*animatedIconLibraries:	$HOME/legocad/libut_reg/libreg
*pag_num:	${num_page}
*page_list:	${list_page}
!EOF
print "\n${num_page} mmi pages have been added in Context.ctx\n"
print "\n${num_page} mmi pages have been added in Context.ctx\n" >> $KLOG/kWinContext.log
#
kAddScreen kWinContext End 
kAddLog kWinContext End
echo "$star"
echo "$star" >> $KLOG/kWinContext.log
echo "Log file : $KLOG/kWinContext.log"
