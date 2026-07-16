#!/bin/ksh
#
print "\n"
echo "blocks    bytes        subdirectory                                      "
echo "--------  -----------  --------------------------------------------------"
ls -la \
| grep '^d' \
| awk ' NR != 2    { stringa1=sprintf("du -s %s", $9) ; 
                     system(stringa1)}' \
| awk ' { printf("%-8d  %-11d  %s\n", \
                 $1, $1*512, $2 ); }' \
| sort -k 1n 
