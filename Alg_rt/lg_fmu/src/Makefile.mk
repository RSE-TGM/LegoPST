# ==============================================================================
# Alg_rt/lg_fmu/src/Makefile.mk — liblg_fmu.a + LegoCliSINC.so builder
#
# Linux porting della FMU LegoCliSINC.
#
# Targets:
#   liblg_fmu.a    archivio statico con lg_var_mapping + lg_fmi2 (per test/tool)
#   LegoCliSINC.so libreria condivisa caricabile come binario FMU
#                  --> richiede dipendenze (libsim/libRt/libdispatcher/...)
#                      compilate con -fPIC. Le .a di LegoPST attualmente
#                      NON sono PIC (R_X86_64_32 link error). Follow-up: o
#                      ricompilare quelle .a con -fPIC, oppure produrre
#                      versioni .so condivise. Per ora il target `so` e'
#                      lasciato a documentazione ma fallisce il link.
#
# NON e' agganciato al Makefile.mk di root: build manuale.
#
# Pre-requisiti:
#   source $LEGOROOT/.profile_legoroot
#
# Build:
#   make -f Makefile.mk             (default: solo .a per ora)
#   make -f Makefile.mk lib         (solo .a)
#   make -f Makefile.mk so          (.so, fallisce finche' deps non sono PIC)
#
# Pulizia:
#   make -f Makefile.mk clean
# ==============================================================================

OS = LINUX

C_FLAGS = -g -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ \
          -fPIC -fvisibility=hidden

CFLAGS = -I../include \
         -I$(LEGORT_INCLUDE) \
         -I$(LEGOROOT_LIB) \
         -D$(OS) $(C_FLAGS)

LIB    = liblg_fmu.a
SO     = LegoCliSINC.so

OBJS   = lg_var_mapping.o lg_fmi2.o

# Stesso link set di probe_step (read-only + dispatcher):
LIBS_RT = $(LEGORT_LIB)/libsim.a \
          $(LEGORT_LIB)/libdispatcher.a \
          $(LEGORT_LIB)/libnet.a \
          $(LEGORT_LIB)/libipc.a \
          $(LEGOROOT_LIB)/libutil.a \
          $(LEGOROOT_LIB)/libRt.a

OTHER_LIB = -lsqlite3 -lm -lpthread

all: $(LIB) $(SO)

lib: $(LIB)
so:  $(SO)

$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

# .so con visibility=hidden + esplicito -fvisibility=default sulle entry FMI2
# (FMI2_Export e' gia' __attribute__((visibility("default"))), ok cosi'.)
$(SO): $(OBJS)
	cc -shared -o $@ $(OBJS) \
	    -Wl,--start-group $(LIBS_RT) -Wl,--end-group $(OTHER_LIB)

%.o: %.c
	cc $(CFLAGS) -c $<

clean:
	rm -f *.o $(LIB) $(SO)

.PHONY: all lib so clean
