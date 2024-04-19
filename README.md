
# LegoPST
## LegoPowerSystemTechnology
## Ambiente modulare di simulazione e controllo di processi termici, idraulici ed elettrici per la produzione e trasporto di energia.
### Versione  Ubuntu 20.04 - WSL </div>
***
### Compilazione e installazione di LegoPST
* Installare i pacchetti indicati in portingWSL_Ubuntu.md
* Clonare il repository ad esempio nella home dell'utente /home/utente
~~~ 
git clone remotepath/to/LegoPST2010A_WSL_DARHE6_64.git
cd /home/utente/LegoPST2010A_WSL_DARHE6_64
~~~

* per compilare il pacchetto lanciare il comando: 
  > make -f Makefile.mk
* Editare il file .profile_legoroot e cambiare la variabile di ambiente LEGOROOT
  > LEGOROOT=/home/utente/LegoPST2010A_WSL_DARHE6_64
* Inserire l'esecuzione di   .profile_legoroot aad esempio in .bashrc dell'utente, sconnettersi (logout) e riconnetersi (login) in una nuova shell session
***


