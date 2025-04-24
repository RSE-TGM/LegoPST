# ******* Telelogic expanded section *******

# make_macros from project "util97-2007A1_RHE4_lomgr
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib
C_FLAGS=-g -DLINUX  -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_  \
-I/usr/include -I/usr/include/gdbm -I/usr/local/include \
 -I../AlgLib/libinclude -L../AlgLib -O2 -I$(LEGOROOT_LIB)/sqlite_include \
 $(SQLITE_LIB)

VAR=`date`
RRR=`echo $$$$`

default:
	@echo	"default: .... ($(VAR)) ..."

test:	m10 m20 m30 m40 m50 m60 m70 m80
	@echo	"test -- VAR: {$(VAR)}"

m10:
	@echo RRR is $(RRR)
m20:
m30:
	@echo	"Current target name: $@"
	@echo	$@ | sum | awk -f 0.awk
#%	@echo	$@ | sum | awk '{ printf( "\n%d. %% 100 ! %02d\n", $1, $1 % 100 ); }'
m40:
	@echo	"Current target name: $@"
	@echo	$@ | sum | awk '{ print $1 }'
m50:
m60:
	@echo	"m60 !!! $(RRR)"
	-@if [ $(RRR) -gt 2500 ]; then \
		echo "GT 2500"; \
	else \
		echo "LE 2500"; \
	fi
m70:
m80:
	@echo	"Current target name: $@"
	@echo	$@ | sum | awk '{ print $1 }'

#=======================================================================


L0.a:	M00.o M01.o M02.o M03.o M04.o M05.o M06.o M07.o M08.o M09.o 
	ar -ru L0.a M00.o M01.o M02.o M03.o M04.o M05.o M06.o M07.o M08.o M09.o
L1.a:	M10.o M11.o M12.o M13.o M14.o M15.o M16.o M17.o M18.o M19.o 
	ar -ru L1.a M10.o M11.o M12.o M13.o M14.o M15.o M16.o M17.o M18.o M19.o
L2.a:	M20.o M21.o M22.o M23.o M24.o M25.o M26.o M27.o M28.o M29.o 
	ar -ru L2.a M20.o M21.o M22.o M23.o M24.o M25.o M26.o M27.o M28.o M29.o
L3.a:	M30.o M31.o M32.o M33.o M34.o M35.o M36.o M37.o M38.o M39.o 
	ar -ru L3.a M30.o M31.o M32.o M33.o M34.o M35.o M36.o M37.o M38.o M39.o
L4.a:	M40.o M41.o M42.o M43.o M44.o M45.o M46.o M47.o M48.o M49.o 
	ar -ru L4.a M40.o M41.o M42.o M43.o M44.o M45.o M46.o M47.o M48.o M49.o
L5.a:	M50.o M51.o M52.o M53.o M54.o M55.o M56.o M57.o M58.o M59.o 
	ar -ru L5.a M50.o M51.o M52.o M53.o M54.o M55.o M56.o M57.o M58.o M59.o
L6.a:	M60.o M61.o M62.o M63.o M64.o M65.o M66.o M67.o M68.o M69.o 
	ar -ru L6.a M60.o M61.o M62.o M63.o M64.o M65.o M66.o M67.o M68.o M69.o
L7.a:	M70.o M71.o M72.o M73.o M74.o M75.o M76.o M77.o M78.o M79.o 
	ar -ru L7.a M70.o M71.o M72.o M73.o M74.o M75.o M76.o M77.o M78.o M79.o
L8.a:	M80.o M81.o M82.o M83.o M84.o M85.o M86.o M87.o M88.o M89.o 
	ar -ru L8.a M80.o M81.o M82.o M83.o M84.o M85.o M86.o M87.o M88.o M89.o
L9.a:	M90.o M91.o M92.o M93.o M94.o M95.o M96.o M97.o M98.o M99.o 
	ar -ru L9.a M90.o M91.o M92.o M93.o M94.o M95.o M96.o M97.o M98.o M99.o
