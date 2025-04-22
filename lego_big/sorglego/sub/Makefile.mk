# ******* Telelogic expanded section *******

# make_macros from makefile "Makefile.mk-2.3.1"
LEGO_LIB=../../lib
LEGO_BIN=../../bin
FFLAGS=$(F_FLAGS)
CFLAGS=$(C_FLAGS)
PATHCADOBJ=./cad_sub_o
PATHOBJ=./sub_o

# make_macros from project "lego_big-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
GCC_INCLUDE=
X_INCLUDE=-I. $(GCC_INCLUDE) 
C_FLAGS=-g -D_BSD -DLINUX -D_NO_PROTO -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ $(X_INCLUDE)
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
#C_LIB=/lib/libbsd.a
C_LIB=
OTHER_LIB=-lm
MOTIF_VER=11
#PREPROCESSOR_OPTIONS=-C -DOSF1
PREPROCESSOR_OPTIONS= -C
UIL_INCLUDE=-I/usr/include/uil
UIL_COMPILER=/usr/X11R6/bin/uil
X_FLAGS=-c -D_NO_PROTO -DSNAPSHOT
#------------------------ C preprocessor
CPP=cpp
CPPFLAGS=-P -C -DLINUX -traditional
#------------------------ C compiler
#CC=cc
CFLAGS=$(C_FLAGS) -g
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
#------------------------ Fortran compiler (g77)
F_FLAGS=-fno-second-underscore -g -fno-automatic -finit-local-zero

#
#	Makefile Header:               Makefile.mk
#       Subsystem:              119
#       Description:
#       %created_by:    lomgr %
#       %date_created:  Tue Jan 14 14:57:57 2003 %
              
.SUFFIXES:  .pf .o .f .c .sh .h .a
CFLAGS=$(C_FLAGS)
FFLAGS=$(F_FLAGS)
FFILES= ./blleg1.f ./cercno.f ./errf01.f filpos.f gotoca.f \
inizbl.f jacreg.f jacto.f leci.f legbl1.f legdyn.f legjac.f lego1.f \
lego3.f legpri.f legres.f legsis.f legtop.f leou.f \
leout.f lepert.f letf01.f lg1fil.f lg3fil.f lg5sim.f lgdyns.f \
lgtops.f lismod.f list1.f ma28.f nomvar.f ordin.f parsim.f  \
passot.f pertur.f plprot.f prepo.f pswtch.f rec24.f recout.f resreg.f  \
rg5sim.f rgdyns.f separ.f serva1.f serva2.f sswtch.f sta2.f stajac.f \
stato1.f toprid.f topsis.f i2goto.f lgsins.f jflolog.f

OBJFILES= ./blleg1.o ./cercno.o ./errf01.o filpos.o gotoca.o \
inizbl.o jacreg.o jacto.o leci.o legbl1.o legdyn.o legjac.o lego1.o \
lego3.o legpri.o legres.o legsis.o legtop.o leou.o \
leout.o lepert.o letf01.o lg1fil.o lg3fil.o lg5sim.o lgdyns.o \
lgtops.o lismod.o list1.o ma28.o nomvar.o ordin.o parsim.o  \
passot.o pertur.o plprot.o prepo.o pswtch.o rec24.o recout.o resreg.o  \
rg5sim.o rgdyns.o separ.o serva1.o serva2.o sswtch.o sta2.o stajac.o \
stato1.o toprid.o topsis.o i2goto.o lgsins.o jflolog.o

SOURCEPF=cloc.pf crltm.pf lego34.pf legreg.pf  lgabrt.pf  lgstop.pf

FORTRAN= $(PATHOBJ)/cloc.f $(PATHOBJ)/crltm.f $(PATHOBJ)/lego34.f \
$(PATHOBJ)/legreg.f $(PATHOBJ)/lgabrt.f $(PATHOBJ)/lgstop.f

FORTRANCAD=$(PATHCADOBJ)/cloc.f $(PATHCADOBJ)/crltm.f $(PATHCADOBJ)/lego34.f \
$(PATHCADOBJ)/legreg.f $(PATHCADOBJ)/lgabrt.f $(PATHCADOBJ)/lgstop.f

OBJFOR= $(PATHOBJ)/cloc.o $(PATHOBJ)/crltm.o $(PATHOBJ)/lego34.o \
$(PATHOBJ)/legreg.o $(PATHOBJ)/lgabrt.o $(PATHOBJ)/lgstop.o

OBJCADFOR=$(PATHCADOBJ)/cloc.o $(PATHCADOBJ)/crltm.o $(PATHCADOBJ)/lego34.o \
$(PATHCADOBJ)/legreg.o $(PATHCADOBJ)/lgabrt.o $(PATHCADOBJ)/lgstop.o

all: $(LEGO_LIB)/legolib.a $(LEGO_LIB)/clegolib.a

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBJFILES): $(FFILES)
	$(FC) -c $(FFLAGS) $(FFILES)

$(PATHOBJ)/cloc.o: $(PATHOBJ)/cloc.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/cloc.f -o $(PATHOBJ)/cloc.o
$(PATHOBJ)/crltm.o: $(PATHOBJ)/crltm.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/crltm.f -o $(PATHOBJ)/crltm.o
$(PATHOBJ)/lego34.o: $(PATHOBJ)/lego34.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/lego34.f -o $(PATHOBJ)/lego34.o
$(PATHOBJ)/legreg.o: $(PATHOBJ)/legreg.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/legreg.f -o $(PATHOBJ)/legreg.o
$(PATHOBJ)/lgabrt.o: $(PATHOBJ)/lgabrt.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/lgabrt.f -o $(PATHOBJ)/lgabrt.o
$(PATHOBJ)/lgstop.o: $(PATHOBJ)/lgstop.f
	$(FC) -c $(FFLAGS) $(PATHOBJ)/lgstop.f -o $(PATHOBJ)/lgstop.o

$(PATHCADOBJ)/cloc.o: $(PATHCADOBJ)/cloc.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/cloc.f -o $(PATHCADOBJ)/cloc.o
$(PATHCADOBJ)/crltm.o: $(PATHCADOBJ)/crltm.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/crltm.f -o $(PATHCADOBJ)/crltm.o
$(PATHCADOBJ)/lego34.o: $(PATHCADOBJ)/lego34.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/lego34.f -o $(PATHCADOBJ)/lego34.o
$(PATHCADOBJ)/legreg.o: $(PATHCADOBJ)/legreg.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/legreg.f -o $(PATHCADOBJ)/legreg.o
$(PATHCADOBJ)/lgabrt.o: $(PATHCADOBJ)/lgabrt.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/lgabrt.f -o $(PATHCADOBJ)/lgabrt.o
$(PATHCADOBJ)/lgstop.o: $(PATHCADOBJ)/lgstop.f
	$(FC) -c $(FFLAGS) $(PATHCADOBJ)/lgstop.f  -o $(PATHCADOBJ)/lgstop.o

$(PATHOBJ)/cloc.f: cloc.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) cloc.pf > $(PATHOBJ)/cloc.f
$(PATHOBJ)/crltm.f: crltm.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) crltm.pf > $(PATHOBJ)/crltm.f
$(PATHOBJ)/lego34.f: lego34.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) lego34.pf > $(PATHOBJ)/lego34.f
$(PATHOBJ)/legreg.f: legreg.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) legreg.pf > $(PATHOBJ)/legreg.f
$(PATHOBJ)/lgabrt.f: lgabrt.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) lgabrt.pf > $(PATHOBJ)/lgabrt.f
$(PATHOBJ)/lgstop.f: lgstop.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) lgstop.pf > $(PATHOBJ)/lgstop.f


$(PATHCADOBJ)/cloc.f: cloc.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD cloc.pf > $(PATHCADOBJ)/cloc.f
$(PATHCADOBJ)/crltm.f: crltm.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD crltm.pf > $(PATHCADOBJ)/crltm.f
$(PATHCADOBJ)/lego34.f: lego34.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD lego34.pf > $(PATHCADOBJ)/lego34.f
$(PATHCADOBJ)/legreg.f: legreg.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD legreg.pf > $(PATHCADOBJ)/legreg.f
$(PATHCADOBJ)/lgabrt.f: lgabrt.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD lgabrt.pf > $(PATHCADOBJ)/lgabrt.f
$(PATHCADOBJ)/lgstop.f: lgstop.pf
	/lib/cpp -P $(PREPROCESSOR_OPTIONS) -DLEGOCAD lgstop.pf > $(PATHCADOBJ)/lgstop.f

$(LEGO_LIB)/legolib.a: $(OBJFILES) $(OBJFOR)
	ar rsv $(LEGO_LIB)/legolib.a $(OBJFILES) $(OBJFOR)
$(LEGO_LIB)/clegolib.a: $(OBJFILES) $(OBJCADFOR)
	ar rsv $(LEGO_LIB)/clegolib.a $(OBJFILES) $(OBJCADFOR)
