#!/bin/ksh
#
if [ "${USER}" = "root" -o "${USER}" =  "puerlib1" ]
then
echo "Please not use this program with superuser privileges"
exit
fi
#
if [ -L $HOME/legocad/libut ]
then
rm $HOME/legocad/libut
echo Cleaning libut link
fi
if [ -L $HOME/legocad/libut_reg ]
then
rm $HOME/legocad/libut_reg
echo Cleaning libut_reg link
fi
if [ -L $HOME/legocad/libut_mmi ]
then
rm $HOME/legocad/libut_mmi
echo Cleaning libut_mmi link
fi
if [ -L $HOME/legocad/libut_bin ]
then
rm $HOME/legocad/libut_bin
echo Cleaning libut_bin link
fi
#
if [ ! -d $HOME/legocad/libut ]
then
ln -fs $KSOURCE/legocad/libut $HOME/legocad/libut
echo Linking libut
else
print "${star5}\nWARNING : Local Libut already exists\n${star5}\a\a\a"
#rm -i $HOME/legocad/libut
fi
if [ ! -d $HOME/legocad/libut_reg ]
then
ln -fs $KSOURCE/legocad/libut_reg $HOME/legocad/libut_reg
echo Linking libut_reg
else
print "${star5}\nWARNING : Local Libut_reg already exists\n${star5}\a\a\a"
#rm -i $HOME/legocad/libut_reg
fi
if [ ! -d $HOME/legocad/libut_mmi ]
then
ln -fs $KSOURCE/legocad/libut_mmi $HOME/legocad/libut_mmi
echo Linking libut_mmi
else
print "${star5}\nWARNING : Local Libut_mmi already exists\n${star5}\a\a\a"
#rm -i $HOME/legocad/libut_mmi
fi
if [ ! -d $HOME/legocad/libut_bin ]
then
ln -fs $KSOURCE/legocad/libut_bin $HOME/legocad/libut_bin
echo Linking libut_bin
else
print "${star5}\nWARNING : Local Libut_bin already exists\n${star5}\a\a\a"
#rm -i $HOME/legocad/libut_bin
fi
#
cd $LEGOCAD_USER/legocad
ls ./*/macroblocks.dat | cut -f2 -d"/" | while read legotask
do
echo UpDating foraus.for for ${legotask}
cd ${legotask}
ln -fs $KSOURCE/legocad/FORAUS/foraus.for foraus.for
cd $LEGOCAD_USER/legocad
done
###############################################################################
# UpDate Context Library
###############################################################################
if [ "$1" = "Context" ]
then
echo $star
echo " UpDate Context Library"
echo $star
cd $LEGOCAD_USER/legocad
REGO_TASK_LIST=`ls -d r_* | grep -v sps`
SPS_REGO_TASK_LIST=`ls -d r_* | grep sps`
echo Task :
for task in ${REGO_TASK_LIST}
do
cd $task
echo $task
if [ -f Context.ctx ]
then
mv Context.ctx Context.ctx.AntesUpDateLib
grep -v iconlib Context.ctx.AntesUpDateLib > Context.ctx
echo "*iconlib_list:   \ LIB_MAT LIB_IO LIB_LOG std LIB_MEASURE LIB_MEQ LIB_SLC LIB_GC LIB_SGC LIB_FX" >> Context.ctx
echo "*iconlib_label:  \ Analogic-(red) Input-Output Logic-(blu) Standard Measure Actuador-Regolador-Setpoint Select-SLC-ASO Group-Control SGC FX" >> Context.ctx
echo "*iconlib_num:    10" >> Context.ctx
else
echo "ERROR : Context.ctx not found"
fi
cd $LEGOCAD_USER/legocad
done
echo SPS Task :
for task in ${SPS_REGO_TASK_LIST}
do
cd $task
echo $task
if [ -f Context.ctx ]
then
mv Context.ctx Context.ctx.AntesUpDateLib
grep -v iconlib Context.ctx.AntesUpDateLib > Context.ctx
echo "*iconlib_list:   \ LIB_MAT Lib_01 LIB_LOG std LIB_MEASURE LIB_MEQ LIB_SLC LIB_GC LIB_SGC LIB_FX" >> Context.ctx
echo "*iconlib_label:  \ Analogic-(red) Input-Output Logic-(blu) Standard Measure Actuador-Regolador-Setpoint Select-SLC-ASO Group-Control SGC FX" >> Context.ctx
echo "*iconlib_num:    10" >> Context.ctx
else
echo "ERROR : Context.ctx not found"
fi
cd $LEGOCAD_USER/legocad
done
fi
