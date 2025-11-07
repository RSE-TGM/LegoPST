

# lgdock.sh
### Lancio di LegoPST in un container docker, senza installazione dello script 

Funziona in due modalità una convenzionale con export del DISPLAY verso il container ed una estesa con supporto socat per il proxy e tunneling. 
La differenza principale tra le due è che nella versione estesa viene gestita la connessione X11 usando socat (che deve essere installato nella macchina host) per creare un bridge socket, il che può essere utile in certi ambienti dove la connessione X11 diretta non funziona correttamente. 
Inoltre lo script consente il download di una applicazione demo di LegoPST costituita da una struttura legocad ed una directry sked.

```bash
#ESEMPI

# Esecuzione dello script:  
 bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)"

# Esecuzione dello script con socat:  
 bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)" _ "-s"

# Esecuzione dello script con installazione demo:  
 bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)" _ "-d"


# Se si vuole essere sicuri di usare l'ultima versione dell'immagine docker LegoPST:
docker pull aguagliardi/legopst:2.0 && bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)"

# Lancio con help:
docker pull aguagliardi/legopst:2.0 && bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)"  _ "-h"

# Download dello script:
 curl -sSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh -o ./lgdock
```
