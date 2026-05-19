all: lg2_done.out

lg2_done.out: proc\f03.dat
	type nul: > lg2_done.out

proc\f03.dat: proc\f02.dat
	proc\lg2.exe > lg2.out

proc\f02.dat: f01.dat
	$(LG_LEGO)\lg1.exe > lg1.out
	if exist "proc\ltm.f" del "proc\ltm.f"
