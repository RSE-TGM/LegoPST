C*********************************************************************
C       Fortran Source:             stato1.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:30:04 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_stato1_f
      CHARACTER*80  RepoID
      COMMON /CM_stato1_f / RepoID
      DATA RepoID/'@(#)1,fsrc,stato1.f,2'/
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

      SUBROUTINE STATO1(NBL,NOMBLC,NOMSUB,NSTA,NUSC,NIGR,IPV,VARS,
     $            MODEL,NX2,NX5,NX6,VARBL)
      DIMENSION NOMBLC(NX2,2),NOMSUB(*),NSTA(*),NUSC(*),NIGR(*),IPV(*)
      CHARACTER*8 MODEL,VARS(NX5),VARBL(NX6,3)
C
C     SUBROUTINE STAMPA CARATTERISTICHE DEL SISTEMA E TOPOLOGIA
C
C      DATA IBLK/' '/,IX/'X'/,ICAR/'I'/
C
      WRITE(6,1000) MODEL
 1000 FORMAT(1H1///10X,'STAMPA DELLA TOPOLOGIA DEL MODELLO ',A8//)
      NA=0
      NB=0
      DO 100 I=1,NBL
      I1=IPV(I)
      I2=IPV(I+1)-1
      NS=NSTA(I)
      NU=NUSC(I)
      NI=NIGR(I)
      DO 10 J=1,NX6
      DO 10 K=1,3
      VARBL(J,K)=' '
   10 CONTINUE
      K=0
      IF(NS.EQ.0)GO TO 21
      DO 20 J=1,NS
      K=K+1
      M=I1+K-1
      VARBL(J,2)=VARS(M)
   20 CONTINUE
   21 IF(NU.EQ.0)GO TO 31
      DO 30 J=1,NU
      K=K+1
      M=I1+K-1
      VARBL(J,3)=VARS(M)
   30 CONTINUE
   31 IF(NI.EQ.0)GOTO 41
      DO 40 J=1,NI
      K=K+1
      M=I1+K-1
      VARBL(J,1)=VARS(M)
   40 CONTINUE
   41 CONTINUE
C
C
C     STAMPA CARATT. DI UN BLOCCO
C
      WRITE(6,1001)NOMBLC(I,1),NOMBLC(I,2),NOMSUB(I)
 1001 FORMAT(///10X,'BLOCCO ',2A4,2X,'(SUB. ',A4,')'//)
      MM=NU+NS
      WRITE(6,1002)NI,MM,NS,NU
 1002 FORMAT(10X,I3,' INGRESSI',14X,I3,' USCITE'/27X,I3,
     $       ' VAR.STATO',3X,I3,' VAR.ALG.'//)
      MX=NI
      IF(MX.LT.NS)MX=NS
      IF(MX.LT.NU)MX=NU
   65 DO 70 K=1,MX
      WRITE(6,1003)(VARBL(K,L),L=1,3)
 1003 FORMAT(14X,3(A8,9X))
   70 CONTINUE
  100 CONTINUE
      RETURN
      END
C            
