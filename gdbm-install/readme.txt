==============================================================================
 OBSOLETO — questa installazione manuale NON è più necessaria (verificato 2026-06-03)
==============================================================================

I tool che usano gdbm (dbmftc2, dbmins, dbmins_mul, dbmrea, dbmftc, dbm2sql,
pagcompiler, kReadDB) vengono compilati contro la gdbm moderna di sistema
(-lgdbm_compat -lgdbm  →  libgdbm.so.6). Il database CAI_VAR_DB viene creato
e riletto a runtime dalla STESSA libreria moderna (vedi leggi_cai_var.sh +
elab_cai_var.sh), quindi non c'è alcun vincolo di formato on-disk.

Test di verifica eseguito con successo usando solo libgdbm.so.6:
  touch testdb.dir testdb.pag
  printf "ALFA:A val\n" | dbmins_mul testdb     # dbm_store OK
  dbmftc2 testdb ALFA:A                          # -> val
  ldd $(which dbmftc2) | grep gdbm               # libgdbm.so.6 (NON .so.2)

Quindi:
 - NON eseguire install.sh nelle nuove installazioni.
 - Basta il pacchetto di sviluppo: gdbm-devel (Fedora) / libgdbm-compat-dev (Debian/Ubuntu).
 - La .so.2 servirebbe SOLO se si importa un binario precompilato vecchio
   linkato a libgdbm.so.2 (invece di ricompilarlo dai sorgenti).

------------------------------------------------------------------------------
 ISTRUZIONI STORICHE (conservate solo come riferimento — vedi sopra)
------------------------------------------------------------------------------
Installare manualmente la libgdbm.so.2, obsoleta, per lanciare dbmftc2 da config:
# prerequisito: dnf install cpio -y
mkdir ~/temp-gdbm
cd  ~/temp-gdbm
curl -O "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"

rpm2cpio gdbm-1.8.0-33.fc12.x86_64.rpm | cpio -idmv
sudo cp ./usr/lib64/libgdbm.so.2.0.0 /usr/lib64/
sudo ln -s /usr/lib64/libgdbm.so.2.0.0 /usr/lib64/libgdbm.so.2
sudo ldconfig

controllo:
ldconfig -p | grep libgdbm.so.2


==============================================================================
 Componenti che usano gdbm (interfaccia ndbm, link -lgdbm_compat -lgdbm)
==============================================================================
1. util97/pagmod/pagcompiler.c
   Compilatore di risorse (.pag -> .rtf). Apre il database "edfdb" e fa
   dbm_fetch degli attributi dei widget. Parte della pipeline MMI/widget.

2. kutil/kReadDB.c
   Legge UN valore dato dbm-basename + key (con timing in millisecondi).

3. util97/dbutil/ (sei tool -> ../bin/)
   dbmrea.c       Dump completo: itera tutte le chiavi e stampa coppie K/V
   dbmins.c       Insert di un singolo record
   dbmins_mul.c   Insert multiplo (legge "key value" da stdin)
   dbmftc.c       Fetch (ftc=fetch) di un valore dato basename+key
   dbmftc2.c      Fetch con timing (variante benchmark di dbmftc)
   dbm2sql.c      Migrazione: converte un db gdbm in SQLite3 (-lsqlite3)

Funzioni dbm usate: dbm_open, dbm_close, dbm_fetch, dbm_store,
                    dbm_firstkey, dbm_nextkey, dbm_error, dbm_clearerr.

Nota: gdbm NON è usato dal simulatore principale né da legopc.tix —
è limitato a util97/, kutil/ e al compilatore risorse pagmod/.
