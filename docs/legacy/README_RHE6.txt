ISTRUZIONI PER L'INSTALLAZIONE SU LINUX RED HAT ENTERPRISE 4 O SIMILARI
(CENTOS o Scientific Linux)

Le attività si sviluppano in 3 punti di cui i primi due obbligatori
SEMPRE, mentre il terzo è necessario solo pe simulatori di dimensioni
particolarmente grandi

1) ATTIVAZIONE DELLA CHIAVE HARDWARE USB (OBBLIGATORIO):

Con una installazione standard di REH HAT ENTERRPRISE 3 o similari
(i test sono stati positivi sia su SCIENTIFIC LINUX che su CENTOS)
occorre aggiungere una linea al file "/etc/fstab" al fine di poter
interrogare la chiave USB contenente la licenza di attivazione
del software AlterLego.
La procedura è la seguente:

diventare root

aggiungere la seguente riga al file "/etc/fstab":

  none /proc/bus/usb usbfs defaults,devmode=0666 0 0

eseguire il comando:

  umount /proc/bus/usb

e, successivamente, il comando opposto:

  mount /proc/bus/usb

La chiave dovrebbe ora funzionare correttamente.
In caso di ulteriore malfunzionamento chiamare l'assistenza.

2) LIMITE DEL NUMERO DI CODE DI MESSAGGIO (OBBLIGATORIO):

Una installazione standard ha come numero massimo di code il valore 16
come verificabile con il comando:

$ ipcs -l

------ Limiti della memoria condivisa --------
max number of segments = 4096
max seg size (kbytes) = 32768
max total shared memory (kbytes) = 8388608
min seg size (bytes) = 1

------ Limiti di semaforo --------
numero max di matrici = 128
numero max di semafori per matrice = 250
numero max di semafori su tutto il sistema = 32000
numero max opz. per chiamata semop = 32
valore max del semaforo = 32767

------ Messaggi: limiti --------
numero max di code su tutto il sistema = 16
dimensione max del messaggio (byte) = 8192
dimensione massima predefinita della coda (byte) = 16384

Occorre aumentare sempre il numero di code da 16 a 128

Si può fare ciò diventando root e successivamente posizionandosi
in "/proc/sys/kernel"

si esegue

"echo 128 > msgmni"

Se ora rifacciamo
ipcs -l
vediamo che il numero max di code e correttamente cambiato.

Naturalmente l'effetto viene annullato da un reboot della macchina per cui
è opportuno modificare il parametro in modo permanente a livello di kernel,
editando il file /etc/sysctl.conf ed aggiungendo le seguenti linee

# Sets maximum number of message queues to 128
kernel.msgmni = 128

Ci sono anche altri metodi che lasciamo ai professori che li conoscono

3) LIMITE NELLA DIMENSIONE DI UN SEGMENTO DI SHARED MEMORY (OPZIONALE):

Una installazione standard ha come dimensione massima per un segmento
di shared memory il valore 32768
come verificabile con il comando:

$ ipcs -l

------ Limiti della memoria condivisa --------
max number of segments = 4096
max seg size (kbytes) = 32768
max total shared memory (kbytes) = 8388608
min seg size (bytes) = 1

------ Limiti di semaforo --------
numero max di matrici = 128
numero max di semafori per matrice = 250
numero max di semafori su tutto il sistema = 32000
numero max opz. per chiamata semop = 32
valore max del semaforo = 32767

------ Messaggi: limiti --------
numero max di code su tutto il sistema = 16
dimensione max del messaggio (byte) = 8192
dimensione massima predefinita della coda (byte) = 16384

Il problema si ha con simulatori di grosse dimensioni quando il file
"variabili.rtf" supera le dimensioni massime sopra indicate.

anche in questo caso le due soluzioni (temporanea e permanente) sono
simili al caso precedente:

Si diventa root e ci si posiziona in "/proc/sys/kernel"

se, per esempio, si vuole una shared memory di 64MByte si esegue

"echo 64000000 > shmmax"

La modifica permanente viene fatta editando il file /etc/sysctl.conf
ed aggiungendo le seguenti linee:

# Sets maximum shared memory segment dimension to 128
kernel.shmmax=64000000