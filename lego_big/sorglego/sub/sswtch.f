C*********************************************************************
C       Fortran Source:             sswtch.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:29:58 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_sswtch_f
      CHARACTER*80  RepoID
      COMMON /CM_sswtch_f / RepoID
      DATA RepoID/'@(#)1,fsrc,sswtch.f,2'/
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

      SUBROUTINE SSWTCH(K,I)
      CHARACTER*4 IS, ISIP, ISI
      DIMENSION KSW(6)
      COMMON /SSTCH/ KSW
      I=0
      IF(K.EQ.10)GO TO 7
      IF(K.NE.9)GO TO 10
      DO 5 J=1,6
      KSW(J)=0
    5 CONTINUE
      WRITE(6,1003)
 1003 FORMAT(/10X,'VUOI STAMPE INTERMEDIE DI CONTROLLO?')
      READ(5,1004)ISI      
      DATA IS/'SI'/,ISIP/'si'/
 1004 FORMAT(A4)
      IF((ISI.NE.IS).AND.(ISI.NE.ISIP))RETURN
      K=10
      RETURN
    7 CONTINUE
      WRITE(6,1000)
 1000 FORMAT(10X,'ASSEGNA I NUMERI DEI SSWTCH NON SEPARATI DA VIRGOLE')
      READ(5,1001)(KSW(J),J=1,6)
 1001 FORMAT(6I1)
      RETURN
C
   10 IF(K.GT.6.OR.K.LT.1)GO TO 15
      DO 12 J=1,6
      IF(K.EQ.KSW(J))GO TO 13
   12 CONTINUE
      GO TO 20
   13 I=1
      GO TO 20
   15 WRITE(6,1002)
 1002 FORMAT(///10X,'NUMERO S.SWTCH NON COMPRESO FRA 1 E 6 '/10X,
     $   'LO CONSIDERO ON  (ERRORE LEGO)'//)
      I=1
   20 RETURN
      END
C            
