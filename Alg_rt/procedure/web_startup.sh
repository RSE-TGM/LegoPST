#!/bin/ksh
#
#  Script:			startup.sh
#  Instance:		1
#  %version:		2 %
#  Description:		
#  %created_by:		lomgr %
#  %date_created:	Tue Jun 24 15:53:03 2003 %
if [ $# -eq 1 ]
then
cd $1
fi
#
# Controllo esistenza file variabili.rtf
#
echo "######################################"
echo "       Check file variabili.rtf"
echo "######################################"
ls variabili.rtf
if [ "$?" -eq 0 ]
then
#
# Controllo esistenza licenze
#
echo "######################################"
echo "       Check license"
echo "######################################"
check_license algrt
if [ "$?" -eq 0 ]
then
killsim
clean
MALLOCTYPE=3.1; export MALLOCTYPE
#
#Ricerca del file Simulator
#
if  test ! -r Simulator
then
  if  test  -r ~/defaults/Simulator
  then
      echo "Copio il file Simulator da ~/defaults"
      cp ~/defaults/Simulator .
  else
      if  test  -r $LEGO/procedure/Simulator.tpl
      then
        echo "Copio $LEGO/procedure/Simulator.tpl"
        cp $LEGO/procedure/Simulator.tpl Simulator
      else
        DEFAULT=1
      fi
  fi
fi
#
#Lettura dei parametri da passare ai processi
#
if  [ $DEFAULT ]
then
 SNAP_S=60
  BACK_T=30
  CAMPIO=14400
  NUM_VA=10000
  PERTUR=50
  SPARE_=1
  PERTCL=0
  SCAD_H=""
  NO_PRI=""
TIME_BACK_T=120.0
  echo "Mancando il file Simulator assegno i seguenti valori di defaults"
  echo MAX_SNAP_SHOT=$SNAP_S
  echo MAX_BACK_TRACK=$BACK_T
  echo MAX_CAMPIONI=$CAMPIO
  echo NUM_VAR=$NUM_VA
  echo MAX_PERTUR=$PERTUR
  echo SNAP_SPARE=$SPARE_
  echo PERT_CLEAR=$PERTCL
  echo SCAD_HOST=$SCAD_H
  echo PRIOLO_DIS=$NO_PRI
echo TIME_BACK_TRACK=$TIME_BACK_T
else
if [ $OS == 'Linux' ]
then
echo LLLINUX!!!
  SNAP_S=`grep 'MAX_SNAP_SHOT'  Simulator | cut -f2 -d':'`
  BACK_T=`grep 'MAX_BACK_TRACK' Simulator | cut -f2 -d':'`
  CAMPIO=`grep 'MAX_CAMPIONI'   Simulator | cut -f2 -d':'`
  NUM_VA=`grep 'NUM_VAR'        Simulator | cut -f2 -d':'`
  PERTUR=`grep 'MAX_PERTUR'     Simulator | cut -f2 -d':'`
  SPARE_=`grep 'SPARE_SNAP'     Simulator | cut -f2 -d':'`
  PERTCL=`grep 'PERT_CLEAR'     Simulator | cut -f2 -d':'`
  SCAD_H=`grep 'SCADA'          Simulator | cut -f2 -d':'`
  NO_PRI=`grep 'NO_PRIOLO'      Simulator | cut -f2 -d':'`
  TIME_BACK_T=`grep 'TIME_BACK_TRACK' Simulator | cut -f2 -d':'`
else
  SNAP_S=`grep 'MAX_SNAP_SHOT'  Simulator | cut -d:':' -f2`
  BACK_T=`grep 'MAX_BACK_TRACK' Simulator | cut -d:':' -f2`
  CAMPIO=`grep 'MAX_CAMPIONI'   Simulator | cut -d:':' -f2`
  NUM_VA=`grep 'NUM_VAR'        Simulator | cut -d:':' -f2`
  PERTUR=`grep 'MAX_PERTUR'     Simulator | cut -d:':' -f2`
  SPARE_=`grep 'SPARE_SNAP'     Simulator | cut -d:':' -f2`
  PERTCL=`grep 'PERT_CLEAR'     Simulator | cut -d:':' -f2`
  SCAD_H=`grep 'SCADA'          Simulator | cut -d:':' -f2`
  NO_PRI=`grep 'NO_PRIOLO'      Simulator | cut -d:':' -f2`
 TIME_BACK_T=`grep 'TIME_BACK_TRACK' Simulator | cut -d:':' -f2`
fi
fi
#
#Attivazione dei processi
#
echo prima di dispatcher
dispatcher -num_snap $SNAP_S -num_bktk $BACK_T -num_camp_cr $CAMPIO -num_var_cr $NUM_VA -num_pert_active $PERTUR -num_spare_forsnap $SPARE_ -clear_pert_bktk $PERTCL -time_bktk $TIME_BACK_T&

sleep 2

echo prima di net_sked
net_sked 2 -num_snap $SNAP_S -num_bktk $BACK_T -num_camp_cr $CAMPIO -num_var_cr $NUM_VA -num_pert_active $PERTUR -num_spare_forsnap $SPARE_ -clear_pert_bktk $PERTCL -time_bktk $TIME_BACK_T -host_scada_name $SCAD_H -no_priolo $NO_PRI&
echo dopo net_sked
fi
fi
