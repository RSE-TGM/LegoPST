# ==============================================================================
# Alg_rt/lg_fmu/tests/Makefile.mk — test binaries for liblg_fmu
#
# Pre-requisiti:
#   - source $LEGOROOT/.profile_legoroot
#   - liblg_fmu.a gia' costruita: cd ../src && make -f Makefile.mk
#
# Build:
#   make -f Makefile.mk
#
# Run da una task con simulazione attiva:
#   ./test_var_mapping
#   ./test_var_mapping <NOME_VAR>
# ==============================================================================

OS = LINUX

C_FLAGS = -g -D_BSD -DLINUX -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_

CFLAGS = -I../include \
         -I$(LEGORT_INCLUDE) \
         -I$(LEGOROOT_LIB) \
         -D$(OS) $(C_FLAGS)

LIBS = ../src/liblg_fmu.a \
       $(LEGORT_LIB)/libsim.a \
       $(LEGORT_LIB)/libdispatcher.a \
       $(LEGORT_LIB)/libnet.a \
       $(LEGORT_LIB)/libipc.a \
       $(LEGOROOT_LIB)/libutil.a \
       $(LEGOROOT_LIB)/libRt.a

OTHER_LIB = -lsqlite3 -lm

BINS = test_var_mapping

all: $(BINS)

test_var_mapping: test_var_mapping.o $(LIBS)
	cc -o $@ test_var_mapping.o -Wl,--start-group $(LIBS) -Wl,--end-group $(OTHER_LIB)

%.o: %.c
	cc $(CFLAGS) -c $<

clean:
	rm -f *.o $(BINS)

.PHONY: all clean
