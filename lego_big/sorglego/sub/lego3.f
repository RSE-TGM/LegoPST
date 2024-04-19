C*********************************************************************
C       Fortran Source:             lego3.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Thu Mar 24 17:56:13 2005 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_lego3_f
      CHARACTER*80  RepoID
      COMMON /CM_lego3_f / RepoID
      DATA RepoID/'@(#)1,fsrc,lego3.f,2'/
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

      SUBROUTINE LEGO3(IERROR)
C      PROGRAMMA  L E G O 3
C
C
       include '../main/lg_parameter.fh'
C
C
      DIMENSION NOSL(N001),NOSUB(N002),NOBLC(N002,2),
     $          NUSTA(N002),NUSCI(N002),NINGR(N002),ISLB(N002),IP(M003),
     $          IPVRS(N005),IPS(M004),
     $          IPVRT(N005),IPI(M005),IPVRI(M001)
      DIMENSION XY(N003),UU(N004),XYU(N005),ICV(N003),ICI(N004),
     $          DATI(N007),IPDATI(M003),
     $          XN(N002),XIN(N003),XYUP(N005),CNXYU(N005),TOLL(N003),
     $          TOL(NT006),KCERCN(N005),LCERCN(N005)
      CHARACTER*8 VARI(N004),VAR(N005),SIVAR(N003),VARNO(N004),
     $            VARSI(N003)
      INTEGER RVAR (N005,2)
C
      CHARACTER*100  NMSIVA(N003)
      CHARACTER*100  NMVARI(N004)
      CHARACTER*80   NMBLOC(N002)
      CHARACTER*132  RIGA
      CHARACTER*8    SIGLA,MODEL,SI12,SIG12
      DIMENSION IOUSIV(N003),IOUVAR(N004)
C
      COMMON/NORM/P0,H0,T0,Q0,R0,AL0,V0,DP0
      COMMON/AUSIL1/NSTATI,NUSCIT,NINGRE
      COMMON/LGTV01/LGTEMP,ILEGO,ICOUNT,ICNTMX
      COMMON/LGTV02/LGMODU,LGBLOC
      CHARACTER*8 LGMODU,LGBLOC
      REAL LGTEMP
      INTEGER I,J,III,JJJ
C
C$$$  DATA IBLK/' '/,NOT/'NO'/,BLK/' '/,ISSO/'SI'/,ISTAR/'****'/,
C$$$ $     STAR/'****'/
      DATA PZERO/20000000./,HZERO/1000000. /,QZERO/1000./
      DATA TZERO/1000./,ROZERO/100./,ALZERO/10./,VZERO/50./,
     $     DPZERO/1.E+5/
C Guag2024      DATA LG/'LG'/,IS/'SI'/
      NX1=N001
      NX2=N002
      NX3=N003
      NX4=N004
      NX5=N005
      NX6=NT006
      NX7=N007
      MX1=M001
      MX2=M002
C
C     PROGRAMMA PER LA LETTURA DATI DEL REGIME INIZIALE,GEOMETRICI
C      E FISICI DI UN IMPIANTO
C     CONTROLLA VALIDITA C.I. REGIME
C     ARCHIVIA SU FILE 04 TUTTI I DATI PER ESEGUIRE LE SIMULAZIONI O
C     IL CALCOLO DEL REGIME
C
C     LETTURA DEL FILE 03 CONNESSIONE LEGO2 - LEGO3
C
      ILEGO=1
      REWIND 3
      READ(3)NBL,NEQAL,NBL1,NVART,NEQSIS,NEQS1,NPVRT,NU,NU1,NVRI
      READ(3)ISSIS,NBTRI,NST,SIGLA,(NOSL(I),I=1,NST)
      READ(3)(NOSUB(I),NOBLC(I,1),NOBLC(I,2),NUSTA(I),NUSCI(I),
     $       NINGR(I),ISLB(I),NMBLOC(I),I=1,NBL)
      READ(3)(IP(I),I=1,NBL1),(VAR(I),IPVRS(I),I=1,NVART)
      READ(3)(IPS(I),I=1,NEQS1),(SIVAR(I),NMSIVA(I),
     $        IOUSIV(I),I=1,NEQSIS),(IPVRT(I),I=1,NPVRT)
      READ(3)(IPI(I),I=1,NU1),(VARI(I),NMVARI(I),
     $        IOUVAR(I),I=1,NU),(IPVRI(I),I=1,NVRI)
C
      DO 543 I = 1, NVART
  543 READ (VAR(I), 544) RVAR (I,1), RVAR (I,2)
  544 FORMAT (2A4)
C
 1000 FORMAT(1H1///10X,'PROGRAMMA LEGO - ACQUISIZIONE DATI DI REGIME '/)
      WRITE(6,1001)SIGLA
 1001 FORMAT(//10X,'IMPIANTO = ',A8 //)
C
      REWIND 14
      READ(14,1010) RIGA
 1010 FORMAT(A)
      SIG12 = RIGA(20:27)
      IF(SIG12 .EQ. SIGLA)GO TO 15
   10 WRITE(6,1003)SIG12
 1003 FORMAT(1H1//10X,'I DATI DI REGIME REGISTRATI SUL FILE 14 '/
     $ 10X,'SI RIFERISCONO ALL'' IMPIANTO ',A8,/10X,'DIVERSO',
     $ ' DA QUELLO PER IL QUALE SONO REGISTRATI SU FILE 3 '/
     $ 10X,'I DATI TOPOLOGICI - ESECUZIONE INTERROTTA '//)
      IERROR=1
      RETURN
   15 READ(14,1004)P0,H0,Q0,T0,R0,AL0,V0,DP0
 1004 FORMAT(27X,5(4X,F6.0)/27X,2(4X,F6.0),5X,F6.0)
C
      IF(P0.LE.0.)P0=PZERO
      IF(H0.LE.0.)H0=HZERO
      IF(Q0.LE.0.)Q0=QZERO
      IF(T0.LE.0.)T0=TZERO
      IF(R0.LE.0.)R0=ROZERO
      IF(AL0.LE.0.)AL0=ALZERO
      IF(V0.LE.0.)V0=VZERO
      IF(DP0.LE.0.)DP0=DPZERO
C
      WRITE(6,3005)P0,H0,Q0,T0,R0,AL0,V0,DP0
 3005 FORMAT(//10X,'DATI DI NORMALIZZAZIONE '/10X,'P0= ',F10.0/
     $10X,'H0= ',F10.0/10X,'W0= ',F10.0/,10X,'T0= ',F10.0/10X,'R0= ',
     $F10.0/10X,'L0= ',F10.0/10X,'V0= ',F10.0/10X,'DP0=',F10.0)
C
C     LETTURA CONDIZIONI INIZIALI VARIABILI DEL SISTEMA (STATI,USCITE,IN
C                                                        GRESSI)
C
 3006 FORMAT(// 1X,'ELENCO DELLE VARIABILI DEL SISTEMA E DEI LORO ',
     $   'VALORI DI REGIME (MKS)'//)
      IFER=0
      IER=0
      READ(14,3007)
      DO 60 I=1,NEQSIS
 5080   FORMAT(1X,A2)
        READ(14,3007)J,SI12,XY(I),ICV(I)
 3007   FORMAT(1X,I2,1X,A8,2X,G10.3,15X,A4)
        IF(SI12.EQ.SIVAR(I))GO TO 60
        WRITE(6,3008)J,SI12,I,SIVAR(I)
 3008   FORMAT(//10X,'LA VARIABILE ',I2,1X,A8/10X,'NON CORRISPONDE ',
     $     'ALLA VARIABILE CONTENUTA NEI DATI TOPOLOGICI'/10X,I2,2X,A8)
        IER=1
   60 CONTINUE
      READ(14,5080)
 3010 FORMAT(1X,'VARIABILI DI STATO E DI USCITA '//)
 3009 FORMAT(///1X,'VARIABILI DI INGRESSO '//)
   80 IFER=IER
      IER=0
      DO 90 I=1,NU
        READ(14,3007)J,SI12,UU(I),ICI(I)
        IF(SI12.EQ.VARI(I))GO TO 90
        WRITE(6,3008)J,SI12,I,VARI(I)
        IER=1
   90 CONTINUE
      READ(14,5080)
C
C     TRASF. CONDIZ.INIZIALI NEL VETTORE XYU RELATIVO ALLE VARIABILI DI
C                                                          OGNI BLOCCO
  100 IFER=IFER+IER
      IF(IFER.EQ.0)GO TO 101
      WRITE(6,4998)
 4998 FORMAT(///10X,'ESECUZIONE INTERROTTA'//)
      IERROR=1
      RETURN
  101 CONTINUE
      CALL LEGBL1(NEQSIS,IPS,IPVRT,XY,XYU)
      CALL LEGBL1(NU,IPI,IPVRI,UU,XYU)
      DO 105 I=1,NVART
      XYUP(I)=XYU(I)
      CNXYU(I)=1.
  105 CONTINUE
C     LETTURA,CONTROLLI,NORMALIZZAZIONE DATI FISICI E
C     GEOMETRICI DI OGNI BLOCCO COMPONENTE L'IMPIANTO
C     E ORGANIZZAZIONE DEI DATI
C
      WRITE(6,5002)
 5002 FORMAT(//10X,'LETTURA,CONTROLLO,NORMALIZZAZIONE DEI PARAMETRI '/
     $   10X,'DI IMPIANTO E NORMALIZZAZIONE DELLE VARIABILI DEL ',
     $   'SISTEMA A REGIME '//)
      IER=0
      K1=1
      K2=1
      IFUN=2
      DO 500 I=1,NBL
      I1=IP(I)
      I2=IP(I+1)-1
      K=ISLB(I)
      NSTATI=NUSTA(I)
      NUSCIT=NUSCI(I)
      NINGRE=NINGR(I)
      NE=NUSTA(I)+NUSCI(I)
      DO 299 J=1,NE
      TOL(J)=0.
  299 CONTINUE
C
      DO 63 J=I1,I2
      CNXYU(J)=1.
   63 CONTINUE
C
      IBLOC1=NOBLC(I,1)
      IBLOC2=NOBLC(I,2)
      WRITE(6,5821)IBLOC1,IBLOC2
 5821 FORMAT(///10X,'PARAMETRI DEL BLOCCO   ',2A4/
     $10X,            '--------------------'//)
C
C********************** TAVOLE ******************************
      WRITE(LGMODU,'(A4)')NOSUB(I)
      WRITE(LGBLOC,'(2A4)')IBLOC1,IBLOC2
C********************** TAVOLE ******************************
C
      CALL MODI2(K,IFUN,IBLOC1,IBLOC2,RVAR,NX5,I1,I2,XYU,DATI,
     $           K1,K2,IER,CNXYU,TOL)
C
C      MEMORIZZA LA TOLLERANZA PER OGNI EQUAZIONE
C
      DO 300 J=1,NE
      J1=IPVRS(I1-1+J)
      TOLL(J1)=TOL(J)
      IF(TOLL(J1).LE.0.)TOLL(J1)=1.E-4
  300 CONTINUE
C>>>>>>>>>>>>>>>>>>>>>>3/3/87<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
C
C      Ogni modulo puo' inizializzare le sue variabili di uscita
C      indipendentemente dal valore che l'utente ha fornito su file 14.
C      Questa correzione rende possibile sempre il trasferimento
C      del valore inizializzato nel modulo a tutti gli altri moduli che
C      lo usano come variabile di ingresso.
C
      DO 320 J=1,NE
      KJ=I1-1+J
      KI=IPVRS(KJ)
      DO 310 JJ=1,NVART
      IF(JJ.EQ.KJ)GO TO 310
      IF(KI.NE.IPVRS(JJ))GO TO 310
      XYU(JJ)=XYU(KJ)
  310 CONTINUE
  320 CONTINUE
C>>>>>>>>>>>>>>>>>>>>>>3/3/87<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
C
C     MEMORIZZA PUNTATORE AL VETTORE DATI PER OGNI BLOCCO
C
  400 IPDATI(I)=K1
      K2=K2+1
      IF(K2.LE.NX7)GO TO 410
      WRITE(6,3821)NX7
 3821 FORMAT(//10X,'SUPERATA LA MASSIMA DIMENSIONE',I5,
     $             ' DEL VETTORE DATI')
      IERROR=1
      RETURN
  410 CONTINUE
      IPDATI(I+1)=K2
      K1=K2
  500 CONTINUE
      NDATI=IPDATI(NBL+1)-1
C
C      NORMALIZZAZIONE-VARIABILI
C
      DO 505 I=1,NVART
      XYU(I)=XYU(I)/CNXYU(I)
  505 CONTINUE
C
C     TRASFERIMENTO DEL VETTORE XYU (NORMALIZZATO) IN XY E UU
C     USANDO LA MATR.DI COLL. DIRETTA (BLOCCHI-LEGO)
C
      CALL BLLEG1(IPVRS,NVART,XYU,XY,UU)
      IF(IER.EQ.0)GO TO 600
      IERROR=1
      WRITE(6,5003)
 5003 FORMAT(//10X,'PER GLI ERRORI SEGNALATI DAI BLOCCHI IL PROGRAMMA ',
     $   'ARRESTA LA SUA ESECUZIONE ')
      RETURN
  600 CONTINUE
C
C  Controllo congruenza CNXYU di varibili "legate"
C
      CALL CERCNO(IP,NBL1,IPVRS,NOBLC,NX2,VAR,NX5,CNXYU,NVART,
     $            KCERCN,LCERCN,ICRAC)
C
  605 CONTINUE
C
C     CONTROLLI SULLA VALIDITA' DELLE C.I.(REGIME)
C
      IREGIM=0
C
C     CONVERSAZIONE CON L'UTENTE PER VERIFICA E DEFINIRE LE VARIABILI
C     DI REGIME NOTE DA QUELLE INCOGNITE
C
      CALL LECI(ICV,NEQSIS,ICI,NU,SIVAR,NX3,VARI,NX4,VARNO,VARSI,XY,UU,
     $          XN,XIN,KN,KIN,IERROR)
      IF(IERROR.EQ.1)RETURN
      IREGIM=1
C
C     SCRITTURA DEL FILE 04 CONNESSIONE LEGO3 - LEGO4
C
  607 rewind 44
      write(44,*)'Merda'
C
      write(44,*)NBL
C
      DO 670 III=1,NBL
         write(44,*)'IP(',III,')=',IP(III)
         write(44,*)'IPS(',III,')=',IPS(III)
         write(44,*)'IPVRS(',III,')=',IPVRS(III)
         write(44,*)'IPI(',III,')=',IPI(III)
         write(44,*)'IPVRT(',III,')=',IPVRT(III)
         write(44,*)'IPVRI(',III,')=',IPVRI(III)
         write(44,*)'IPDATI(',III,')=',IPDATI(III)
         write(44,*)NMBLOC(III)
         write(44,*)NUSCI(III)
C
         DO 650 JJJ=1,NUSCI(III)
            write(44,*)VAR(JJJ)
  650    CONTINUE
C
         write(44,*)'****'
  670 CONTINUE
C
  690 REWIND 4
      WRITE(4)NBL,NEQAL,NBL1,NVART,NEQSIS,NEQS1,NPVRT,NU,NU1,NVRI
      WRITE(4)ISSIS,NBTRI,NST,SIGLA,(NOSL(I),I=1,NST)
      WRITE(4)(NOSUB(I),NOBLC(I,1),NOBLC(I,2),NUSTA(I),NUSCI(I),
     $       NINGR(I),ISLB(I),NMBLOC(I),I=1,NBL)
      WRITE(4)(IP(I),I=1,NBL1),(VAR(I),IPVRS(I),I=1,NVART)
      WRITE(4)(IPS(I),I=1,NEQS1),(SIVAR(I),NMSIVA(I),
     $        IOUSIV(I),I=1,NEQSIS),
     $        (IPVRT(I),I=1,NPVRT)
      WRITE(4)(IPI(I),I=1,NU1),(VARI(I),NMVARI(I),
     $        IOUVAR(I),I=1,NU),
     $       (IPVRI(I),I=1,NVRI)
C
      WRITE(4)P0,H0,T0,Q0,R0,AL0,V0,DP0
      WRITE(4) (XY(I),I=1,NEQSIS),(UU(I),I=1,NU),(XYU(I),I=1,NVART)
      WRITE(4) NDATI,(IPDATI(I),I=1,NBL1),(DATI(I),I=1,NDATI)
     $            ,(CNXYU(I),I=1,NVART),(TOLL(I),I=1,NEQSIS)
      WRITE(4) IREGIM
      IF(IREGIM.EQ.0)GO TO 700
      WRITE(4) (VARNO(I),XN(I),I=1,NU),(VARSI(I),XIN(I),I=1,NEQSIS)
  700 RETURN
      END
C            
