C*********************************************************************
C       Fortran Source:             leou.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:27:06 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_leou_f
      CHARACTER*80  RepoID
      COMMON /CM_leou_f / RepoID
      DATA RepoID/'@(#)1,fsrc,leou.f,2'/
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

      SUBROUTINE LEOU(NEQSIS,SIVAR,MX1,NU,VARI,MX3,NOUT,IPOUT)
      DIMENSION IPOUT(*)
      CHARACTER*8 SIVAR(MX1),VARI(MX1),VAVA
C Guag2024      DATA NO/'NO'/
      NOUT=0
    1 K=1
      VAVA = ' '
      READ(5,1001)VAVA
 1001 FORMAT(A8)
      IF(VAVA .EQ. '*')GO TO 60
      IF(VAVA .EQ. ' ')GO TO 40
      DO 10 I=1,NEQSIS
      IF(VAVA.EQ.SIVAR(I))GO TO 30
   10 CONTINUE
      K=2
      DO 20 I=1,NU
      IF(VAVA.EQ.VARI(I))GO TO 30
   20 CONTINUE
      WRITE(6,1002)VAVA
 1002 FORMAT(//10X,'ER - LA VARIABILE ',A8,'  NON ESISTE FRA QUELLE',
     $   ' ASSEGNATE ALL''IMPIANTO')
      GO TO 1
   30 NOUT=NOUT+1
      IPOUT(NOUT)=I
      IF(K.EQ.2)IPOUT(NOUT)=-I
      GO TO 1
   40 IF(NOUT.EQ.0)GO TO 70
      WRITE(6,1004)
 1004 FORMAT(///10X,'FINE ASSEGNAZIONE'//10X,'ELENCO DELLE VARIABILI',
     $   ' ASSEGNATE'/)
      DO 50 I=1,NOUT
      K=IPOUT(I)
      IF(K.LT.0)GO TO 45
      WRITE(6,1005)SIVAR(K)
      GO TO 50
   45 K=-K
      WRITE(6,1005)VARI(K)
 1005 FORMAT(10X,A8)
   50 CONTINUE
      GO TO 70
C
   60 NOUT=99999
   70 RETURN
      END
C            
