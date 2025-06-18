-------------- Installare manualmente la libgdbm.so.2, obsoleta ma indispensabile per poter lanciare dbmftc2 da config
# prerequisito: dnf install cpio -y
mkdir ~/temp-gdbm
cd  ~/temp-gdbm
non cè questo:  wget https://dl.fedoraproject.org/pub/fedora/linux/releases/39/Everything/x86_64/os/Packages/c/compat-gdbm-libs-1.14.1-19.fc39.x86_64.rpm
curl -O "ftp://ftp.icm.edu.pl/vol/rzm6/pbone/archive.fedoraproject.org/fedora/linux/releases/13/Everything/x86_64/os/Packages/gdbm-1.8.0-33.fc12.x86_64.rpm"

rpm2cpio gdbm-1.8.0-33.fc12.x86_64.rpm | cpio -idmv
sudo cp ./usr/lib64/libgdbm.so.2.0.0 /usr/lib64/
sudo ln -s /usr/lib64/libgdbm.so.2.0.0 /usr/lib64/libgdbm.so.2
sudo ldconfig

controllo che è tutto OK:
ldconfig -p | grep libgdbm.so.2
dbmftc2

