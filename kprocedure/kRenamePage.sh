#!/bin/ksh
#
echo "Please not use this program ..."
#exit
print "${star5}\nUpDating Pages Connessioni.reg Context.ctx\n${star5}"
ls B3??.pag | while read page
do
dos=`echo ${page} | cut -c 3-4`
mv B3${dos}.bkg BA${dos}.bkg
echo " Change B3${dos}.pag in BA${dos}.pag "
sed  "s/*nomePag:	B3${dos}/*nomePag:	BA${dos}/g" < B3${dos}.pag > BA${dos}.pag
rm B3${dos}.pag
sed  "s/B3${dos}/BA${dos}/g" < Connessioni.reg > Connessioni.reg.new
mv Connessioni.reg.new Connessioni.reg
sed  "s/B3${dos}/BA${dos}/g" < Context.ctx > Context.ctx.new
mv Context.ctx.new Context.ctx
done
