# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-7"
LEGORT_LIB=..
LEGOROOT_LIB=..
LEGOROOT_INCLUDE=../libinclude
LEGORT_INCLUDE=../libinclude


# make_macros from project "AlgLib-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-lMrm -lXm -lXt -lX11
X_INCLUDE=-I. 
C_FLAGS=-g -fcommon -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
#C_FLAGS=-g -fcommon -D_BSD -DLINUX   -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I. -I/usr/local/include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include -L$(LEGOROOT_LIB)/sqlite_lib
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#
#       Makefile Header:               libsim.mk
#       Subsystem:              1
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Mon Apr  5 10:15:37 2004 %
#
SORGENTI_F = letture_dati_f.f
SORGENTI = nega.c pert_rcv.c pert_snd.c reg_000.c rw_s02.c\
           seleziona_var.c shr_lock.c \
           var_sh.c reg_ing.c reg_prolog.c reg_rdshm.c\
	   reg_wrshm.c pert_scalino.c pert_rampa.c pert_rampa_stop.c \
	   pert_nega.c pert_impulso.c ts_chdir.c pert_up.c pert_down.c\
	   shrmem.c var_bin.c pert_rcv_lego.c\
	   gf22.c list_vargraf.c shresist.c \
	   stato_cr.c pert_impulso_stop.c pert_to.c pert_to_stop.c \
	   pert_period.c pert_period_stop.c pert_whitenoise.c \
	   pert_whitenoise_stop.c letture_dati.c refresh_proc.c wr_f22.c \
	   f22_func.c sim_shvar.c par_lego.c uni_mis.c pert_malfunction.c \
           pert_malfunction_stop.c pert_show.c simulator.c

OGGETTI_F = letture_dati_f.o
OGGETTI  = nega.o pert_rcv.o pert_snd.o reg_000.o rw_s02.o\
           seleziona_var.o shr_lock.o \
           var_sh.o reg_ing.o reg_prolog.o reg_rdshm.o\
           reg_wrshm.o pert_scalino.o pert_rampa.o pert_rampa_stop.o \
	   pert_nega.o pert_impulso.o  ts_chdir.o pert_up.o pert_down.o\
	   shrmem.o var_bin.o pert_rcv_lego.o\
	   gf22.o list_vargraf.o shresist.o \
	   stato_cr.o pert_impulso_stop.o pert_to.o pert_to_stop.o \
	   pert_period.o pert_period_stop.o pert_whitenoise.o \
	   pert_whitenoise_stop.o letture_dati.o refresh_proc.o wr_f22.o \
	   f22_func.o sim_shvar.o par_lego.o uni_mis.o pert_malfunction.o \
           pert_malfunction_stop.o pert_show.o simulator.o

CFLAGS = -I$(LEGOROOT_INCLUDE) -D$(OS) $(VERSIONE) $(X_INCLUDE) $(SCADA_MMI) $(C_FLAGS) -I$(LEGOROOT_LIB)
LIBRERIA=$(LEGORT_LIB)/libsim.a
all: $(LEGORT_LIB)/libsim.a

$(LEGORT_LIB)/libsim.a: $(OGGETTI) $(OGGETTI_F)
	ar rvs $(LEGORT_LIB)/libsim.a $?
	@echo $(LEGORT_LIB)/libsim.a is now up-to-date
.c.a:;

.f.o:
	$(FC) -c $(F_FLAGS) $<
#SCADA=`if [ "$(SCADA_MMI)" = "-DSCADA_MMI" ]; then echo $(LIBRERIAS) ;else echo $(LIBRERIA) ;fi;`
