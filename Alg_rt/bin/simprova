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
  echo "Mancando il file Simulator assegno i seguenti valori di defaults"
  echo MAX_SNAP_SHOT=$SNAP_S
  echo MAX_BACK_TRACK=$BACK_T
  echo MAX_CAMPIONI=$CAMPIO
  echo NUM_VAR=$NUM_VA
  echo MAX_PERTUR=$PERTUR
  echo SNAP_SPARE=$SPARE_
  echo PERT_CLEAR=$PERTCL
else
  SNAP_S=`grep 'MAX_SNAP_SHOT'  Simulator | cut -d:':' -f2`
  BACK_T=`grep 'MAX_BACK_TRACK' Simulator | cut -d:':' -f2`
  CAMPIO=`grep 'MAX_CAMPIONI'   Simulator | cut -d:':' -f2`
  NUM_VA=`grep 'NUM_VAR'        Simulator | cut -d:':' -f2`
  PERTUR=`grep 'MAX_PERTUR'     Simulator | cut -d:':' -f2`
  SPARE_=`grep 'SPARE_SNAP'     Simulator | cut -d:':' -f2`
  PERTCL=`grep 'PERT_CLEAR'     Simulator | cut -d:':' -f2`
fi
#
#Attivazione dei processi
#
net_prova $1 -num_snap $SNAP_S -num_bktk $BACK_T -num_camp_cr $CAMPIO -num_var_cr $NUM_VA -num_pert_active $PERTUR -num_spare_forsnap $SPARE_ -clear_pert_bktk $PERTCL &
