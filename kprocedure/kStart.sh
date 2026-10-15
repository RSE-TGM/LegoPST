#!/bin/ksh
#
#  kStart - avvia la simulazione del simulatore corrente ($KSIM) con
#  net_startup, e aspetta che sia davvero partita.
#
#  Riscritto a ottobre 2026. Prima:
#    - pretendeva che kScd (lo SCADA) risultasse avviato: SCADA e' stato
#      rimosso (docs/COMPONENTI_RIMOSSI.md), e il controllo la fermava sempre;
#    - aspettava, senza limite di tempo, che il banco scrivesse in banco.log
#      "Reading last recorded B.T.": un messaggio che il codice non stampa
#      piu', quindi restava appesa per sempre;
#    - alla fine lanciava kLeeF22, che chiama un LeeF22 mai compilato.
#
#  Ora la simulazione e' "partita" quando dispatcher, net_sked e banco sono
#  vivi e restano vivi per PROVA secondi; e' "fallita" se nell'output di
#  net_startup compare "Errori in fase di Startup", se uno dei tre muore, o se
#  non partono entro KSTART_TIMEOUT secondi (default 120). In ogni caso kStart
#  finisce e lo dice: non resta mai appesa.
#
#  L'output di net_startup (e dei processi che lancia) va in $KLOG/kStart.log.
#
#  Uso:  kStart [DEBUG]
#
debug=$1
TIMEOUT=${KSTART_TIMEOUT:-120}
PROVA=5
###############################################################################
#	VERIFICATION START
###############################################################################
${KBIN}/kTest
KTEST=`cat $KSTATUS/kTest.status`
echo "kTest result : $KTEST"
if [ ! "$KTEST" = "OK" ]
then
print "Environement test not succesful\a"
banner "NOK"
exit 1
fi
kpresentation
if [ -f $KLOG/kStart.log ]
then
mv $KLOG/kStart.log $KLOG/kStart.log.kold
fi
#
kStartStatus=`cat $KSTATUS/kStart.status 2>/dev/null`
if [ "$kStartStatus" = "Start" ]
then
banner "NOK"
print "ERROR : kStart starting ...\n\a"
exit 1
fi
if [ "$kStartStatus" = "Started" ]
then
banner "NOK"
print "ERROR : kStart already started\n\a"
exit 1
fi
###############################################################################
#	VERIFICATION END
###############################################################################
echo ${star}
echo ${star} >> $KLOG/kStart.log
kAddScreen kStart Start
kAddLog kStart Start
kAddStatus kStart Start
kAddLog kLogger "Start kStart"
kAddStatistic kStart
#
rm -f ${KSIM}/.message_list
touch ${KSIM}/.message_list
rm -f ${KSIM}/banco.log
touch ${KSIM}/banco.log
kStat
kAddStatus kClean NOK
cd $KSIM ; net_startup >> $KLOG/kStart.log 2>&1 &
#
#  Attesa dell'esito, con un limite di tempo
#
vivi() {	# vero se il processo $1 dell'utente e' vivo
    pgrep -u $(id -u) -x "$1" >/dev/null 2>&1
}
ESITO=""
trascorso=0
stabile=0
while [ -z "$ESITO" ]
do
    sleep 1
    trascorso=$((trascorso + 1))
    if grep -q "Errori in fase di Startup" $KLOG/kStart.log 2>/dev/null
    then
        ESITO="ERRORI"
    elif vivi dispatcher && vivi net_sked && vivi banco
    then
        stabile=$((stabile + 1))
        [ $stabile -ge $PROVA ] && ESITO="OK"
    else
        # partiti e poi morti: dopo i primi secondi, dispatcher e net_sked
        # devono esserci
        if [ $stabile -gt 0 ] || { [ $trascorso -gt 15 ] && ! vivi dispatcher; }
        then
            ESITO="MORTI"
        fi
        stabile=0
    fi
    [ -z "$ESITO" ] && [ $trascorso -ge $TIMEOUT ] && ESITO="TIMEOUT"
    if [ "${debug}" = "DEBUG" ]
    then
        print "kStart: t=${trascorso}s stabile=${stabile} dispatcher=$(vivi dispatcher && echo si || echo no) net_sked=$(vivi net_sked && echo si || echo no) banco=$(vivi banco && echo si || echo no)"
    fi
done
#
echo "$star"
echo "$star" >> $KLOG/kStart.log
if [ "$ESITO" = "OK" ]
then
    kAddScreen kStart Started
    kAddLog kStart Started
    kAddStatus kStart Started
    kAddLog kLogger "Started kStart"
    print "kStart : simulazione avviata (${trascorso} s)"
    echo "$star"
    echo "$star" >> $KLOG/kStart.log
    exit 0
fi
case "$ESITO" in
    ERRORI)  MOTIVO="net_startup ha segnalato errori in fase di startup" ;;
    MORTI)   MOTIVO="un processo della simulazione e' terminato (dispatcher, net_sked o banco)" ;;
    TIMEOUT) MOTIVO="la simulazione non e' partita entro ${TIMEOUT} s" ;;
esac
banner "NOK"
print "ERROR\t: kStart, $MOTIVO"
print "SOLUTION: il dettaglio e' in $KLOG/kStart.log e in $KSIM/parametri.out"
echo "ERROR : $MOTIVO" >> $KLOG/kStart.log
kAddScreen kStart NOK
kAddLog kStart NOK
kAddStatus kStart NOK
kAddLog kLogger "kStart NOK: $MOTIVO"
echo "$star"
echo "$star" >> $KLOG/kStart.log
exit 1
