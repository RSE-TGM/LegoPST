@echo off
	if exist lg2_done.out del lg2_done.out
	amake -f %LG_LEGO%\rerunlg2.mak
:exit
	
