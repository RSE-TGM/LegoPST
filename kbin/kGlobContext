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
if [ -f $KLOG/kGlobContext.log ]
then
mv $KLOG/kGlobContext.log $KLOG/kGlobContext.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kGlobContext.log
kAddScreen kGlobContext Start
kAddLog kGlobContext Start
#
cd ${KPAGES}
if [ -f $KPAGES/Context.ctx ]
then
mv  $KPAGES/Context.ctx $KPAGES/Context.ctx.kold
fi
num_page=0
ls | grep pag$ | grep -v place | grep -v REG_INT_CONN_DB | \
grep -v Compress | \
while read pagina
do
    echo "Loading ${pagina} ..."
    tag=`echo ${pagina} | cut -f1 -d'.'`
    list_page="${list_page} ${tag}"
    let num_page=num_page+1
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
*description:	Globpages regeneration
*iconlib_label:	\\ 
*nextTagPag:	00
*simulator:	${KSIM}
*pages:		${KPAGES}
*objectLibraries:	$HOME/legocad/libut_reg/libreg
*animatedIconLibraries:	$HOME/legocad/libut_reg/libreg
*pag_num:	${num_page}
*page_list:	${list_page}
!EOF
print "\n${num_page} mmi pages have been added in Context.ctx\n"
print "\n${num_page} mmi pages have been added in Context.ctx\n" >> $KLOG/kGlobContext.log
#
kAddScreen kGlobContext End 
kAddLog kGlobContext End
echo "$star"
echo "$star" >> $KLOG/kGlobContext.log
echo "Log file : $KLOG/kGlobContext.log"
