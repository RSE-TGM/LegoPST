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
. kSimMove
kpresentation
if [ -f $KLOG/kModRtf.log ]
then
mv $KLOG/kModRtf.log $KLOG/kModRtf.log.kold
fi
echo ${star}
echo ${star} >> $KLOG/kModRtf.log
kAddScreen kModRtf Start
kAddLog kModRtf Start
echo "Wait ..."
#
if [ "$1" = "" ]
then
  print "\n use:\t\tkModRtf parameter \n\a"
  print " parameter:\tsim = Add _sim to the nomeProgram attribute Pictograph"
  print "\t\tsca = Delete _sim to the nomeProgram attribute Pictograph\n"
  exit
elif [ "$1" = "sim" ]
then
  label1=""
  label2="_sim"
elif [ "$1" = "sca" ]
then
  label1="_sim"
  label2=""
else
  print "\n ERROR : Parameter unknown \n\a"
  exit
fi
#
#############################################################################
# Creazione del file temporaneo di lavoro
#############################################################################
#
cd $KWIN
rm -f kModRtf.scr
cat > kModRtf.tmp << EOT
aso
br
gc
master_controller
pos_valvdamp_Blue
pos_valvdamp_DBlue
pos_valvdamp_DGrey
pos_valvdamp_DRed
pos_valvdamp_DYellow
pos_valvdamp_Green
pos_valvdamp_LBlue
pos_valvdamp_LGreen
pos_valvdamp_LGrey
pos_valvdamp_LRed
pos_valvdamp_LYellow
pos_valvdamp_Lilac
pos_valvdamp_Ochre
pos_valvdamp_Orange
pos_valvdamp_Violet
pos_valvdamp_Yellow
pumpcompr_Blue
pumpcompr_DBlue
pumpcompr_DGrey
pumpcompr_DRed
pumpcompr_DYellow
pumpcompr_Green
pumpcompr_LBlue
pumpcompr_LGreen
pumpcompr_LGrey
pumpcompr_LRed
pumpcompr_LYellow
pumpcompr_Lilac
pumpcompr_Ochre
pumpcompr_Orange
pumpcompr_Violet
pumpcompr_Yellow
sgc
sol_valvdamp_Blue
sol_valvdamp_DBlue
sol_valvdamp_DGrey
sol_valvdamp_DRed
sol_valvdamp_DYellow
sol_valvdamp_Green
sol_valvdamp_LBlue
sol_valvdamp_LGreen
sol_valvdamp_LGrey
sol_valvdamp_LRed
sol_valvdamp_LYellow
sol_valvdamp_Lilac
sol_valvdamp_Ochre
sol_valvdamp_Orange
sol_valvdamp_Violet
sol_valvdamp_Yellow
step_controller
switch_Blue
switch_DBlue
switch_DGrey
switch_DRed
switch_DYellow
switch_Green
switch_LBlue
switch_LGreen
switch_LGrey
switch_LRed
switch_LYellow
switch_Lilac
switch_Ochre
switch_Orange
switch_Violet
switch_Yellow
EOT
#
#----------------------------------------------------------------------------
# Prepara lo script di ricerca per Pictograph
#----------------------------------------------------------------------------
#
cat kModRtf.tmp | while read riga
do
  echo "widgetclass=Pictograph" >> kModRtf.scr
  echo "hasattrib=nomeProgram" >> kModRtf.scr
  echo "value=${riga}${label1}" >> kModRtf.scr
  echo "attrib=nomeProgram" >> kModRtf.scr
  echo "value=${riga}${label2}" >> kModRtf.scr
done
rm -f kModRtf.tmp
#
#############################################################################
# Loop sulle pagine
#############################################################################
#
ls | grep ^M_S_ | grep .rtf | grep -v err | grep -v mod | while read pagina
do
#  echo ${pagina}
#  echo ${pagina} >> $KLOG/kModRtf.log
  pagmod.1.4 -g -G -p ${pagina} -s kModRtf.scr 2>> $KLOG/kModRtf.log
  if [ -f ${pagina}.mod ]
  then
  mv ${pagina}.mod ${pagina}
  else
  rm -f ${pagina}
  print "Error with plant display ${pagina}\a"
  print "\tSolution : Compile page ${pagina}"
  echo "Error with plant display ${pagina}" >> $KLOG/kModRtf.log
  print "\tSolution : Compile page ${pagina}" >> $KLOG/kModRtf.log
  fi
done
ls | grep ^O_ | grep .rtf | grep -v err | grep -v mod | while read pagina
do
#  echo ${pagina}
#  echo ${pagina} >> $KLOG/kModRtf.log
  pagmod.1.4 -g -G -p ${pagina} -s kModRtf.scr 2>> $KLOG/kModRtf.log
  if [ -f ${pagina}.mod ]
  then
  mv ${pagina}.mod ${pagina}
  else
  rm -f ${pagina}
  print "Error with operating window ${pagina}\a"
  print "\tSolution : Compile page ${pagina}"  
  echo "Error with operating window ${pagina}" >> $KLOG/kModRtf.log
  print "\tSolution : Compile page ${pagina}" >> $KLOG/kModRtf.log
  fi
done
#
kAddScreen kModRtf End 
kAddLog kModRtf End
echo "$star"
echo "$star" >> $KLOG/kModRtf.log
echo "Log file : $KLOG/kModRtf.log"

