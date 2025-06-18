#!/usr/bin/ksh
#   Procedura di installazione di libgdbm.so.2 da pacchetto rpm (vedere readme.txt)
# 1) Download dl paccketto rpm: curl -O "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"
# 2) Estrazione della libreria: 
rpm2cpio gdbm-1.8.0-33.fc12.x86_64.rpm | cpio -idmv
# 3) Installazione ...
sudo cp ./usr/lib64/libgdbm.so.2.0.0 /usr/lib64/
sudo ln -s /usr/lib64/libgdbm.so.2.0.0 /usr/lib64/libgdbm.so.2
sudo ldconfig

