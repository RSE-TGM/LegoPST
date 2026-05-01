# ==============================================================================
# Alg_rt/lg_fmu/tools/Makefile.mk — probe_attach builder
#
# Strumento diagnostico per il porting Linux della FMU LegoCliSINC.
# Si aggancia in sola lettura a una simulazione LegoPST attiva.
#
# NON e' agganciato al Makefile.mk di root: build manuale, isolato.
#
# Pre-requisiti:
#   source $LEGOROOT/.profile_legoroot     (definisce LEGORT_LIB, LEGORT_INCLUDE, ...)
#
# Build:
#   make -f Makefile.mk
#
# Run (in una shell con SHR_USR_KEY definita, dopo net_startup.sh sulla task):
#   ./probe_attach
#   ./probe_attach <NOME_VAR>
#
# Pulizia:
#   make -f Makefile.mk clean
# ==============================================================================

OS = LINUX

C_FLAGS = -g -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -I.

CFLAGS = -I../include -I$(LEGORT_INCLUDE) -D$(OS) $(C_FLAGS) \
	-I$(LEGOROOT_LIB)

# Stesso link set di dump_shr (Alg_rt/net_simula/dump_shr/Makefile.mk):
# read-only attach, niente X11/motif/dcethreads.
LIBS = $(LEGORT_LIB)/libsim.a \
       $(LEGORT_LIB)/libdispatcher.a \
       $(LEGORT_LIB)/libnet.a \
       $(LEGORT_LIB)/libipc.a \
       $(LEGOROOT_LIB)/libutil.a \
       $(LEGOROOT_LIB)/libRt.a

# gen_modeldescription riusa anche liblg_fmu.a (lg_var_mapping wrapper)
LIB_FMU = ../src/liblg_fmu.a

OTHER_LIB = -lsqlite3 -lm

BINS = probe_attach probe_step gen_modeldescription

all: $(BINS)

probe_attach: probe_attach.o $(LIBS)
	cc -o $@ probe_attach.o -Wl,--start-group $(LIBS) -Wl,--end-group $(OTHER_LIB)

probe_step: probe_step.o $(LIBS)
	cc -o $@ probe_step.o -Wl,--start-group $(LIBS) -Wl,--end-group $(OTHER_LIB)

gen_modeldescription: gen_modeldescription.o $(LIB_FMU) $(LIBS)
	cc -o $@ gen_modeldescription.o \
	    -Wl,--start-group $(LIB_FMU) $(LIBS) -Wl,--end-group $(OTHER_LIB)

%.o: %.c
	cc $(CFLAGS) -c $<

clean:
	rm -f *.o $(BINS)

.PHONY: all clean
