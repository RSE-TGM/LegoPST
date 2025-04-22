

C*********************************************************************
C Fortran PreCompile: lgabrt.pf
C Subsystem: 1
C Description:
C %created_by: lopez %
C %date_created: Mon Oct 28 11:18:03 2002 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l identificazione della versione
C
      BLOCK DATA BDD_lgabrt_pf
      CHARACTER*80 RepoID
      COMMON /CM_lgabrt_pf / RepoID
      DATA RepoID/'@(#)1,pfsrc,lgabrt.pf,2'/
      END
C**********************************************************************
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C C
C LEGO unificato per singola / doppia precisione C
C e per diverse piattaforme operative C
C C
C Attivata versione singola precisione per sistema operativo Unix C
C C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE LGABRT





C
C______ SCRIVE UN FLAG DI ERRORE
C SUL FILE "PROC/LGERR.OUT"
C (IL FLAG PUO` ESSERE INTERCETTATO DALLA PROCEDURA
C CHE HA LANCIATO IL PROGRAMMA LEGO)
C
      CHARACTER*20 FILE13
      FILE13='proc/lgerr.out'
C
      OPEN(UNIT=13,FILE=FILE13,STATUS='UNKNOWN',FORM='FORMATTED')
C
      IERR=1
      WRITE(13,*)IERR
      CLOSE (13)
      STOP
      END

C
