C******************************************************************************
C modulo letture_dati_f.f
C tipo 
C release 5.1
C data 11/7/95
C reserver @(#)letture_dati_f.f	5.1
C******************************************************************************
      SUBROUTINE r_lstmod(PATH,LPATH,MODULO,NST)
C>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
C     Fornisce il numero d'ordine del modulo presente
C     nella lista moduli.
C>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

C     FILE '05'  - ELENCO DEI MODULI PRESENTI NELLA LIBRERIA LEGO
C                   MASSIMO ATTUALE 300 MODULI
      CHARACTER*100 PATH
      INTEGER LPATH
      CHARACTER*5 MODULO
      INTEGER NST
      INTEGER NS1
      CHARACTER*4 MODU
      PARAMETER (MAXMOD=300)
      CHARACTER*4 MODUL(300)
      CHARACTER*80 LISTA_MODULI



      OPEN(UNIT=05,FILE=PATH(1:LPATH),STATUS='OLD')

      NST=0
1     READ(5,1002,END=99)MODU
1002  FORMAT(A4)
      NST=NST+1
      IF(NST.EQ.1)GO TO 6
      NS1=NST-1
      DO 5 J=1,NS1
      IF(MODU.EQ.MODUL(J))GO TO 10
5     CONTINUE
6     MODUL(NST)=MODU
      IF(MODU.EQ.MODULO(1:4))GO TO 20 
      IF(NST.EQ.MAXMOD) GO TO 99
      GO TO 1
10    NST=NST-1
      GO TO1
99    WRITE(6,1003)
      NST = 0
1003  FORMAT('FINE LISTA MODULI, NON TROVATO IL MODULO CERCATO') 
20    CONTINUE
      CLOSE(05)

      RETURN
      END

        SUBROUTINE leggi_for(NOME_FILE,LNOMEF,NOME_BLOK,LNOMEB,ERR)
        CHARACTER*100 NOME_FILE
        CHARACTER*9  NOME_BLOK
        INTEGER      LNOMEF,LNOMEB
        CHARACTER*40 RIGA 
        CHARACTER*8  BLOCCO 
        CHARACTER*40 INIZ_DATI
        INTEGER NRIGHE,ERR
        COMMON/NORM/P0,H0,T0,Q0,R0,AL0,V0,DP0
        DATA PZERO/20000000./,HZERO/1000000. /,QZERO/1000./
        DATA TZERO/1000./,ROZERO/100./,ALZERO/10./,VZERO/50./,
     $       DPZERO/1.E+5/
        DATA INIZ_DATI /'*LG*DATI FISICI E GEOMETRICI DEL SISTEMA'/


112     FORMAT('ATTENZIONE Fine file durante la ricerca del blocco ',
     &         A,' nel file ',/,A,
     &         ' in lettura fortran prima del lancio di I2.')
666     FORMAT(20X,A8)
777     FORMAT(A40)
888     FORMAT(3(14X,F10.2,1X))
1004    FORMAT(27X,5(4X,F6.0)/27X,2(4X,F6.0),5X,F6.0)


C Apertura file
        ERR = 0
        NRIGHE = 0 
        OPEN(UNIT=14,FILE=NOME_FILE,STATUS='OLD',FORM='FORMATTED')
         
C Lettura e controllo dei dati di normalizzazone
        READ(14,777) 
        READ(14,1004)P0,H0,Q0,T0,R0,AL0,V0,DP0

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
3005    FORMAT(//10X,'DATI DI NORMALIZZAZIONE '/10X,'P0= ',F10.0/
     $10X,'H0= ',F10.0/10X,'W0= ',F10.0/,10X,'T0= ',F10.0/10X,'R0= ',
     $F10.0/10X,'L0= ',F10.0/10X,'V0= ',F10.0/10X,'DP0=',F10.0)

         NRIGHE = 3

C Cerca l'inizio della zona dei dati 

1000    CONTINUE
          READ(14,777,END=111) RIGA
          NRIGHE = NRIGHE + 1 
        IF ( RIGA.NE.INIZ_DATI ) GOTO 1000

C Cerca l'inizio dei dati del blocco

2000    CONTINUE
          READ(14,666,END=111) BLOCCO
          NRIGHE = NRIGHE + 1
        IF ( BLOCCO(1:LNOMEB).NE.NOME_BLOK(1:LNOMEB) ) GOTO 2000

        REWIND(14)
        DO 2100 I=1,NRIGHE-1
          READ(14,777) RIGA
2100    CONTINUE

        RETURN

111     CONTINUE    

        WRITE(6,112) NOME_BLOK(1:LNOMEB),NOME_FILE(1:LNOMEF) 
        ERR = -1

        RETURN
        END


 
      SUBROUTINE lancia_i2(KI,NVAR,NDAT)
C Parametri passati
      INTEGER     KI,NVAR,NDAT

C**   Descrizione delle parameter (tratte da LG3)
C**
C**   N000= MOLTIPLICATORE
C**
C**   N001= N. MODULI
C**   N002= N. BLOCCHI
C**   N003= N.STATI+ALG. = ORDINE MASSIMO SISTEMA ALGEBRICO
C**   N004= N. INGRESSI
C**   N005= N. VARIABILI
C**   N006= N. VAR. DI 1 BLOCCO (moduli di processo)
C**   NT006= N. VAR. DI 1 BLOCCO (moduli di regolazione)
C**   N007= N. DI DATI
C**   N008= N. DI EQUAZIONI DI UN BLOCCO
C**   NR00= N. DI TERMINI #0 PER OGNI EQUAZIONE
C**   NP00= N. DI PERTURBAZIONI
       
          include '../../lego_big/lib/lg_parameter.fh'
C  GUAG2025 
C      PARAMETER (N000=160, N001=N000*10, N002=N000*25, N003=N000*50,
C     $           N004=N000*25, N005=N000*100, N006=100, NT006=N003+N004,
C     $           N007=N000*500, N008=40, NR00=12, NP00=10)


C Variabili per modidat
      CHARACTER*4 IBLOC1,IBLOC2
      INTEGER     IFUN,NX5,IV1,IV2,ID1,ID2,IER
      REAL        VAR(N005,2),DATI(N007),XYU(N005),CNXYU(N005),TOL(N003)
C Variabili locali
      CHARACTER*80 RIGA


C Inizializzazioni

      IFUN=2

      NX5 = N005

      IV1=1
      IV2=NVAR
      ID1=1
      ID2=NDAT

      IER=0

C Chiamata alla routine di svicolo per l'inizializzazione di tipo I2

      CALL MODIDAT(KI,IFUN,IBLOC1,IBLOC2,VAR,NX5,IV1,IV2,XYU,
     &             DATI,ID1,ID2,IER,CNXYU,TOL)

      CLOSE(14)

      RETURN
      END
C
C Procedura contenete la variabile per l'identificazione della versione
C
      SUBROUTINE SCCS_letture_dati_f
      CHARACTER*80 SccsID
      DATA SccsId/'@(#)letture_dati_f.f	5.1\t11/7/95'/
      END
