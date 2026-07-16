#!/bin/ksh
#
echo Start kcreastaz
echo Start kcreastaz > kcreastaz.log
date >> kcreastaz.log
ls mal*.list N_win*list |
cut -f1 -d. |
while read file
do
   echo Generazione operating windows $file ...
   echo Generazione operating windows $file >> kcreastaz.log
   mkstaz $file
done
echo 
echo Fine generazione operating windows !!
echo Fine generazione operating windows !! >> kcreastaz.log
date >> kcreastaz.log
echo   # Beep
print "\n"

