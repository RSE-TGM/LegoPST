#!/bin/ksh
#
cd $KSIM
if [ -f $KSIM/S01 ]
then
print "\nSaving S01 in S01.kold\n"
echo "Saving S01 in S01.kold" >> $KLOG/kConnex.log
mv  $KSIM/S01 $KSIM/S01.kold
fi
connex2
#
SEPARATORS=0
cat S01.new  | awk '{
                    gsub("\t"," ");
                    if(index($1,"****")>0) {
                       ENVIRON[SEPARATORS]=ENVIRON[SEPARATORS] + 1 ;
                       }
                    if(ENVIRON[SEPARATORS]>=6) {
                       string=sprintf("%-8.8s %-10.10s %-8.8s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
                                      $1,      $2,     $3,     $4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20 );
                       }
                    else {
                       string=sprintf($0 );
                       }
                    printf("%-84.84s\n", string);
                    } ' \
> S01
rm -f S01.new
#
print "\n${star5}\nGenerated Simulator Toplogy File :\n\t$KSIM/S01"
print "\n${star5}\nGenerated Simulator Toplogy File :\n\t$KSIM/S01" >> $KLOG/kConnex.log
print "\nLog File :\n\t$KLOG/kConnex.log\n${star5}"
print "\nLog File :\n\t$KLOG/kConnex.log\n${star5}" >> $KLOG/kConnex.log
print "\n${star5}" >> $KLOG/kConnex.log
cat $KSIM/connex2.out >> $KLOG/kConnex.log
grep -v EJ $KSIM/connex2.out | grep -v C/ | grep -v _ZV | grep -v "\>  CONNECTED" | \
grep -v LOW_RANGE | grep -v HIGH_RANGE > $KLOG/kConnex.err
print "${star5}\n" >> $KLOG/kConnex.log
#
# Perso
if [ -x $KPERSO/kConnexPerso ]
then
print "$star\nIt has been detected a personalisation macro :\n\t$KPERSO/kConnexPerso"
print "$star\nIt has been detected a personalisation macro :\n\t$KPERSO/kConnexPerso" >> $KLOG/kConnex.log
. $KPERSO/kConnexPerso
echo "$star"
echo "$star" >> $KLOG/kConnex.log
fi
# Clean
rm -f $KSIM/connex2.out
print "\a"
