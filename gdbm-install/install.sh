#!/usr/bin/ksh
#   Procedura di installazione di libgdbm.so.2 da pacchetto rpm (vedere readme.txt)

# Calcola la cartella dove si trova questo script (cioè .../gdbm-install)
SCRIPT_DIR=$(cd $(dirname "$0") && pwd)

# 1) Download dl paccketto rpm: curl -O "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"

# 2) Estrazione della libreria:
# Usiamo SCRIPT_DIR invece di LEGOROOT/gdbm-install
echo "Estrazione in corso in: $SCRIPT_DIR"
(cd "$SCRIPT_DIR" && rpm2cpio gdbm-1.8.0-33.fc12.x86_64.rpm | cpio -idmv)

# 3) Installazione ...
echo "Copia dei file..."
# Copia usando il percorso calcolato
cp "$SCRIPT_DIR/usr/lib64/libgdbm.so.2.0.0"  /usr/lib64/

# I comandi successivi rimangono uguali
ln -sf /usr/lib64/libgdbm.so.2.0.0 /usr/lib64/libgdbm.so.2
ldconfig

echo "Fatto."
#   Procedura di installazione di libgdbm.so.2 da pacchetto rpm (vedere readme.txt)
# 1) Download dl paccketto rpm: curl -O "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"
# 2) Estrazione della libreria: 
#sudo cp $LEGOROOT/gdbm-install/usr/lib64/libgdbm.so.2.0.0  /usr/lib64/
#sudo ln -s /usr/lib64/libgdbm.so.2.0.0 /usr/lib64/libgdbm.so.2
#sudo ldconfig

