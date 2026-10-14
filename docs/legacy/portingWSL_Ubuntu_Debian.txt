# PORTING legoroot to Ubuntu


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
~~~S
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

***
### Comandi utili
nella macchina con WSl, mount di un disco, ad esempio Z: 
> sudo mount -t drvfs Z: /mnt/z 
***
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
------
convertire DATA IS/'SI'/,ISIP/'si'/

      CHARACTER*2 SIMAIUS, SIMINUS
      DATA SIMAIUS/'SI'/
      DATA SIMINUS/'si'/

      READ(SIMAIUS,1004) IS
      READ(SIMINUS,1004) ISIP
1004 FORMAT(A4)
-------------------

       fprintf(stderr, "\n#################################################\n");
       fprintf(stderr, "       LICENSE NOT FOUND!!!\n");
       fprintf(stderr, "       MA VA BENE LO STESSO\n");
       fprintf(stderr, "#################################################\n");
//       GUAG 2024
//       return ( -1 ) ;
       return ( 0 ) ;

-P -C -DLINUX -traditional

cancellare commenti da /* fino a */ :
perl -0777 -pe 's,/\*.*?\*/,,gs' qq.f > qqqq.f

non va : THREAD_LIB=-ldl ../AlgLib/dcethreads_lib/libdcethreads.so  -lpthread

THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -lpthread -ldl

cc /home/antonio/svn_wa/branches/LegoPST2010A_WSL_DARHE6_64/legocad/legoserver/legoserver.c  modi.o forausbase.o \
              /home/milost64/legocad/libut/modulilib.a \
               /home/milost64/legocad/libut/auxlegolib.a /home/antonio/svn_wa/branches/LegoPST2010A_WSL_DARHE6_64/legocad/lego_big/lib/clegolib.a \
      /home/antonio/svn_wa/branches/LegoPST2010A_WSL_DARHE6_64/legocad/lego_big/lib/vapo.a -lm  -lm -o /home/milost64/legocad/libut/legoserver

Project->Build options...->Linker settings (tab)
-Wl,--allow-multiple-definition  NON FUNZIONA

DEBIAN:
x11-xserver-utils
geany - editor con semplice IDE
cat /etc/os-release
https://wiki.debian.org/ProgrammingApplication
sudo apt-get install libncurses5-dev libncursesw5-dev

multiple definition Debian, aggiungere a C_FLAGS nei *.mk:
-fcommon

Cambiato ar come sotto:
https://stackoverflow.com/questions/1583219/how-can-i-do-a-recursive-find-replace-of-a-string-with-awk-or-sed
find . -name *.mk | xargs sed -i 's/rvls/rvs/g'

-----------------------
In ubuntu and Deebian  there is apt-file package that allows you to find package which contains specified file. You need to install it with
sudo apt-get install apt-file
update the cache with
apt-file update
and than you can to search the package you need with
apt-file search ndbm.h
------------------------

sudo apt install libgdbm-compat-dev

http://www.carsten-koenig.de/2019/06/24/setup-and-configure-xterm-as-your-default-terminal-emulator/




 
