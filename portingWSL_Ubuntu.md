# PORTING legoroot to Ubuntu

nella macchina con WSl, mount di un disco ad esempio Z: 
* sudo mount -t drvfs Z: /mnt/z 

### Installazione dei pacchetti di base

motif e tcl/tk:
~~~
sudo apt install libmrm4
sudo apt install tcl
sudo apt install tk
sudo apt install tix
~~~
patch di libgcc:
~~~
cd ~/Downloads
wget -c https://old-releases.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/gcc-3.4-base_3.4.6-6ubuntu3_amd64.deb
wget -c https://old-releases.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/libg2c0_3.4.6-6ubuntu3_amd64.deb
sudo apt-get install ./gcc-3.4-base_3.4.6-6ubuntu3_amd64.deb ./libg2c0_3.4.6-6ubuntu3_amd64.deb
~~~

Altre librerie X e motif:
~~~
sudo apt-get install libxmu-dev libxmu-headers freeglut3-dev libxext-dev libxi-dev
dpkg -L libmotif-dev
sudo apt-get install libmotif-dev
sudo apt-get install -y libbsd-dev
~~~

sqlite3:
~~~
sudo apt-get install -y libsqlite3-dev
~~~

Esempi di comandi di clean (molto pericolosi!):
* find . -type f -name "*.bak" -exec rm -f {} \;
* find . -iname ".svn" -print0 | xargs -0 rm -r
* comando che simula 'git clean -n' 

### Appunti vari:
/home/antonio/svn_wa/branches/LegoPST2010A_RHE6_64/AlgLib/dcethreads_lib/
http://osnet.cs.nchu.edu.tw/powpoint/Distributed_OS/RPC.pdf

cc -o ../bin/client_scada  client_scada.o  client_scada_func.o pthread_kill_other_threads_np.o         ../AlgLib/libnet.a ../AlgLib/libsim.a ../AlgLib/libutil.a ../lib/libCs.a ../AlgLib/libipc.a -lm -lsqlite3 -ldl -lc ../AlgLib/dcethreads_lib/libdcethreads.so

vi ./AlgLib/libsim/gf22.c
#define Boolean anotherBOOLtype
#include <Rt/RtMemory.h>

vi ../include/config.h
uguale

AlgLib/Rt/Rt.h
typedef char Boolean


https://gcc.gnu.org/onlinedocs/gfortran/Fortran-Dialect-Options.html

gfortran -c -fno-second-underscore -g -fno-automatic -finit-local-zero -std=legacy -fdec-char-conversions  ./blleg1.f ./cercno.f ./errf01.f filpos.f gotoca.f inizbl.f jacreg.f jacto.f leci.f legbl1.f legdyn.f legjac.f lego1.f lego3.f legpri.f legres.f legsis.f legtop.f leou.f leout.f lepert.f letf01.f lg1fil.f lg3fil.f lg5sim.f lgdyns.f lgtops.f lismod.f list1.f ma28.f nomvar.f ordin.f parsim.f passot.f pertur.f plprot.f prepo.f pswtch.f rec24.f recout.f resreg.f rg5sim.f rgdyns.f separ.f serva1.f serva2.f sswtch.f sta2.f stajac.f stato1.f toprid.f topsis.f i2goto.f lgsins.f jflolog.f

https://stackoverflow.com/questions/5703952/data-int-string-problem-when-compiling-with-gfortran

 
