C*********************************************************************
C       Fortran Source:             legres.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Oct 28 08:33:32 2003 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_legres_f
      CHARACTER*80  RepoID
      COMMON /CM_legres_f / RepoID
      DATA RepoID/'@(#)1,fsrc,legres.f,2'/
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

      SUBROUTINE LEGRES(NBL,ISLB,NUSTA,NUSCI,IPDATI,DATI,IP,TN,
     $                  CDT,XYU,RN,ICONV,SIVAR,MX1,NEQSIS,IPVRS,TOLL,
     $                  IOK,NOBLC,MX3,RNI,
C_________ NUOVI PARAMETRI
     $                  IPVVSS,IPSSVV,RNV,XY,IPS,IPVRT,
C********************** TAVOLE *****************************************
     $                  NOSUB)
C********************** TAVOLE *****************************************
C
C
       include '../main/lg_parameter.fh'
C
      CHARACTER*8 SIVAR(MX1)
      DIMENSION ISLB(*),NUSTA(*),NUSCI(*),IPDATI(*),DATI(*),IP(*),
     $          TN(*),XYU(*),RN(*),ICONV(*),
     $          IPVRS(*),TOLL(*),NOBLC(MX3,2),RNI(*),
C_________ NUOVI PARAMETRI
     $          IPVVSS(*),IPSSVV(*),XY(*),RNV(*),IPS(*),IPVRT(*)
C
C********************************* TAVOLE ******************************
      DIMENSION NOSUB(*)
C       ILEGO = INDICATORE DEL PROGRAMMA CHIAMANTE LE TAVOLE
C             =1 LEGO E` IL PROGRAMMA CHIAMANTE
C             =0 NON E` IL LEGO IL PROGRAMMA CHIAMANTE
C
C       ICNTMX = CONTATORE STABILITO DAL PROGRAMMA CHIAMANTE LE TAVOLE
C                  SUPERATO IL QUALE VIENE DECRETATO LO STOP .
C
C
C
      COMMON/LGTV01/LGTEMP,ILEGO,ICOUNT,ICNTMX
      COMMON/LGTV02/LGMODU,LGBLOC
      CHARACTER*8 LGMODU,LGBLOC
      REAL LGTEMP
C********************** TAVOLE ******************************
C
      COMMON/PARPAR/NPER,NVPLT,NVSTP,NSTPLT,NSTSTP,KPLT,KSTP,ITERT
      COMMON/NORM/P0,H0,T0,Q0,RO0,AL0,V0,DP0
C%%% TOLLERANZA ASSEGNATA RUN_TIME DAI MODULI
      COMMON/TOLEG00/TOLIN(N008)
C%%%
      DIMENSION NFORM(8),IFORM(8),IFOR1(5),IFOR2(5)
C
C     Modifica MAURO RECINE 05.12.1991 (variabile FORMA)
      CHARACTER * 32 FORMA, VFORMA(10)
C
C Guag2024      DATA IFORM/'(' , ' ' , ' ' , ',I5,' , ' A8,' , '1X,E' ,
C Guag2024      $       '12.5' , ')'/
C Guag2024      DATA IFOR1/'1X','1H+','1H+','1H+','1H+'/
C Guag2024      DATA IFOR2/' ','26X','52X','78X','104X'/
C
C     CALCOLO DEI RESIDUI DELLE EQUAZIONI ALGEBRICHE -DIFFERENZ.
C
C     LE EQUAZ. DIFF. SONO DEL TIPO    XN -CDT.F(XN)-T(N-1)= FF(XN)
C                                    RES= T(N-1)+CDT.F(XN)-XN
      M=0
      IFUN=2
      DO 500 I=1,NBL
      K=ISLB(I)
      IPD=IPDATI(I)
      IXYU=IP(I)
      NS=NUSTA(I)
      NY=NUSCI(I)
      NEBL=NS+NY
      DO 5 J=1,NEBL
C%%%
      TOLIN(J)=0.
C%%%
      RNI(J)=0.
    5 CONTINUE
      IBLOC1=NOBLC(I,1)
      IBLOC2=NOBLC(I,2)
C********************** TAVOLE ******************************
      WRITE(LGMODU,'(A4)')NOSUB(I)
      WRITE(LGBLOC,'(2A4)')IBLOC1,IBLOC2
C********************** TAVOLE ******************************
C
      CALL MODC1(K,IFUN,AJAC,1,IXYU,XYU,IPD,DATI,RNI,IBLOC1,IBLOC2)
C
C     TRASFERIMENTO DI RNI (RESIDUI EQ. DI UN BLOCCO)
C     NEL VETTORE RN (RESIDUI EQ. DEL SISTEMA)
C
      IXYU=IXYU-1
      DO 200 J=1,NEBL
      J1= IXYU+J
      JJ1=IPVRS(IXYU+J)
      IF(J.GT.NS)GO TO 199
      M=M+1
      RNI(J)=TN(M)+CDT*RNI(J)-XYU(J1)
  199 RN(JJ1)=RNI(J)
C%%%
C____ AGGIORNAMENTO TOLLERANZA EQUAZIONI
C
      IF(TOLIN(J).GT.0.) THEN
         TOLL(JJ1)=TOLIN(J)
      ENDIF
C%%%
C$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
C
C_________ COSTRUZIONE DEL VETTORE --RNV--
C         TERMINI NOTI DEL SISTEMA DA RISOLVERE
C
C         CARICAMENTO IN --XY-- DEI VALORI DELLE VARIABILI
C         CALCOLATE NEI MODULI
C
C         CARICAMENTO IN --XYU- DEI VALORI DELLE VARIABILI
C         CALCOLATE NEI MODULI
C
C      IF(ITERT.GT.0)GO TO 200
      IK=IPSSVV(JJ1)
      IF(IK.LT.0) THEN
C
      XY(JJ1)= RN(JJ1)
C
      KJ1=IPS(JJ1)
      KJ2=IPS(JJ1+1)-1
      IF(KJ2.LT.KJ1)GO TO 200
      DO 195 KJ=KJ1,KJ2
      KKJ=IPVRT(KJ)
      XYU(KKJ)=XY(JJ1)
  195 CONTINUE
C
C      CARICAMENTO DEL RESIDUO DELL'EQUAZIONE NEL VETTORE RNV
C      (RESIDUI DELLE VERE EQUAZIONI DA RISOLVERE)
C
      ELSE
         RNV(IK)=RN(JJ1)
      ENDIF
C
  200 CONTINUE
  500 CONTINUE
C
      IOK=1
C
C     TEST DI CONVERGENZA SUL SISTEMA
C
      DO 505 I=1,NEQSIS
      ICONV(I)=0
C
      IF(IPSSVV(I).LT.0)GO TO 505
C
      IF(ABS(RN(I)).LE.TOLL(I))GO TO 505
      ICONV(I)=1
      IOK=0
  505 CONTINUE
C
C     STAMPE OPZIONALI -SSWTCH 5
C
      ISWT=5
      CALL SSWTCH(ISWT,LL)
      IF(LL.NE.1)RETURN
C
      IF(IOK.EQ.1)RETURN
      WRITE(6,1000)
 1000 FORMAT(//10X,'RESIDUI DELLE EQUAZIONI NON SODDISFATTE'//)

C Guag2024
      VFORMA(1)='(1X,1X,I5,A8,1X,E12.5)'
      VFORMA(2)='(1H+,26X,I5,A8,1X,E12.5)'
      VFORMA(3)='(1H+,52X,I5,A8,1X,E12.5)'
      VFORMA(4)='(1H+,78X,I5,A8,1X,E12.5)'
      VFORMA(5)='(1H+,104X,I5,A8,1X,E12.5)'

      K=0
      DO 530 I=1,NEQSIS
      IF(ICONV(I).EQ.0)GO TO 530
      K=K+1
      IF(K.EQ.6)K=1
C Guag2024      DO 525 J=1,8
C Guag2024      NFORM(J)=IFORM(J)
C Guag2024  525 CONTINUE
C Guag2024      NFORM(2)=IFOR1(K)
C Guag2024      NFORM(3)=IFOR2(K)
C Guag2024      WRITE (FORMA,'(8A4)') NFORM
      WRITE (FORMA,'(A)') VFORNA(k)
      WRITE (6,FORMA) I,SIVAR(I),RN(I)
  530 CONTINUE
      RETURN
      END
C            
