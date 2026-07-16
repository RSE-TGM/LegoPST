#!/bin/ksh
#
ls ????.pag | grep -v rego | cut -c1-4 > pipo.tmp
paste -s -d' ' pipo.tmp > pipo
rm pipo.tmp
wc -w pipo
mye pipo
mye Context.ctx
print "\a"
