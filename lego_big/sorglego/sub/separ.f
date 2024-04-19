C*********************************************************************
C       Fortran Source:             separ.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:29:52 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_separ_f
      CHARACTER*80  RepoID
      COMMON /CM_separ_f / RepoID
      DATA RepoID/'@(#)1,fsrc,separ.f,2'/
      END
C**********************************************************************
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C           LEGO unificato per singola / doppia precisione             C
C                 e per diverse piattaforme operative                  C
C                                                                      C
C   Attivata versione singola precisione per sistema operativo Unix    C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE SEPAR(N,IC,SIV,MX1,VARN,VARIN,MX3,KN,KIN,X,XN,XIN,MX2)
      DIMENSION IC(*),X(*),XN(*),XIN(*)
      CHARACTER*8 SIV(MX1),VARN(MX3),VARIN(MX2)
C Guag2024
      CHARACTER*4 NOTOSTR
      DATA NOTOSTR/'NOTO'/

      READ(NOTOSTR,'(A4)') NO
C      DATA NO/'NOTO'/

      DO 10 I=1,N
      IF(IC(I).NE.NO)GO TO 7
      KN=KN+1
      VARN(KN)=SIV(I)
      XN(KN)=X(I)
      GO TO 10
C
    7 KIN=KIN+1
      VARIN(KIN)=SIV(I)
      XIN(KIN)=X(I)
   10 CONTINUE
      RETURN
      END
C            
