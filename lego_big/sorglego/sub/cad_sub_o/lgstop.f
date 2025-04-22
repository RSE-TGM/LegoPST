

C*********************************************************************
C Fortran PreCompile: lgstop.pf
C Subsystem: 1
C Description:
C %created_by: lopez %
C %date_created: Mon Oct 28 11:19:07 2002 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l identificazione della versione
C
      BLOCK DATA BDD_lgstop_pf
      CHARACTER*80 RepoID
      COMMON /CM_lgstop_pf / RepoID
      DATA RepoID/'@(#)1,pfsrc,lgstop.pf,2'/
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

      SUBROUTINE LGSTOP

      IERR=0
      CALL EXIT(IERR)
      END
C
