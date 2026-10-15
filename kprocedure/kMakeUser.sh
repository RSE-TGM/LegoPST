#!/bin/ksh
#
if [ ! -d "${KSOURCE}" ]
then
print "Sorry : ambient variable KSOURCE not defined\a."
banner NOK
exit
fi
if [ "${KSOURCE}" = "${HOME}" ]
then
print "Please not use this program with $USER user\a."
exit
fi
cd $HOME
print "${star5}\n"
if [ ! -d $HOME/tmp ]
then
echo "Creating tmp directory"
mkdir $HOME/tmp
fi
if [ ! -d $HOME/bin ]
then
echo "Creating bin directory"
mkdir $HOME/bin
fi
if [ ! -d $HOME/legocad ]
then
echo "Creating legocad directory"
mkdir $HOME/legocad
fi
if [ ! -d $HOME/sked ]
then 
echo "Creating sked directory"
mkdir $HOME/sked
fi
if [ -d $HOME/.dt ]
then
echo "Updating dt directory"
#cp -r $HOME/.dt $HOME/.dtold
rm -r $HOME/.dt
cp -R $KSOURCE/dt $HOME/.dt
fi
echo "Creating dt directory"
cp -R $KSOURCE/dt $HOME/.dt
# ~/defaults si crea se manca, e se c'e' si lascia com'e'. Prima la si
# riempiva copiando $KSOURCE/kdefaults - che conteneva solo i dati di avvio di
# SCADA, rimosso nel 2026 - e se esisteva gia' la si CANCELLAVA per ricopiarla,
# perdendo quello che l'utente ci aveva messo (Simulator, recenti di lghmi...).
if [ ! -d  $HOME/defaults ]
then 
echo "Creating defaults directory"
mkdir -p $HOME/defaults
fi
if [ ! -f   $HOME/.Xdefaults-* ]
then
echo "Creating .Xdefaults-`hostname` file"
cp $KSOURCE/Xdefaults $HOME/.Xdefaults-`hostname`
fi
if [ ! -f   $HOME/.Xpdefaults ]
then 
echo "Updating .Xpdefaults file"
cp $KSOURCE/Xpdefaults $HOME/.Xpdefaults
fi
${KBIN}/kUpDateLibrary
${KBIN}/kMakeLicense Silent
print "\n${star5}\n"
