C*********************************************************************
C       Fortran Source:             jacto.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:23:39 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_jacto_f
      CHARACTER*80  RepoID
      COMMON /CM_jacto_f / RepoID
      DATA RepoID/'@(#)1,fsrc,jacto.f,2'/
      END
C**********************************************************************
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C           LEGO unificato per singola / doppia precisione             C
C                 e per diverse piattaforme operative                  C
C                                                                      C
C   Attivata versione singola precisione per sistema operativo VMS     C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE JACTO(IRJ,ICJ,NZTER,NEQSIS,FJ)
      DIMENSION IRJ(*),ICJ(*),FJ(*)
C
C     CONTROLLO SE CI SONO RIGHE O COLONNE NULLE
C
      DO 5 I=1,NEQSIS
      IRIGA=I
      DO 3 J=1,NZTER
      IR=IRJ(J)
      IF(IR.EQ.IRIGA) GO TO 1
    3 CONTINUE
      WRITE(6,1000)IRIGA
C
    1 ICOL=I
      DO 4 J=1,NZTER
      IC=ICJ(J)
      IF(IC.EQ.ICOL) GO TO 5
    4 CONTINUE
      WRITE(6,1001)ICOL
    5 CONTINUE
C
      RETURN
 1000 FORMAT(///10X,'****LA RIGA DELLO JACOBIANO***',I6,
     &'  NON HA TERMINI')
 1001 FORMAT(///10X,'****LA COLONNA DELLO JACOBIANO***',I6,
     &'  NON HA TERMINI')
      END
C            
