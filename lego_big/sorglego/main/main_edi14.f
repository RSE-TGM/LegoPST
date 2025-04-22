C*********************************************************************
C       Fortran Source:             main_edi14.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:14:40 2001 %
C
C**********************************************************************

 
C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_main_edi14_f
      CHARACTER*80  RepoID
      COMMON /CM_main_edi14_f / RepoID
      DATA RepoID/'@(#)1,fsrc,main_edi14.f,2'/
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

      PROGRAM EDI14
C
       include 'lg_parameter.fh'
C
C parameter di EDI14
C
C      KN001 = N. VARIABILI SU FILE 14
C      KN002 = N. BLOCCHI
C      KN003 = N. LINEE FILE 14
C
      PARAMETER (KN001=N005, KN002=N002, KN003=KN001+N007)
C
C --- Dichiarazioni Originali e Modificate ---
      INTEGER IPDT(KN002)
      INTEGER MX1, IPOSF1, IRCF1I, NBL, NVAR
C     DIMENSION LINE(33),IA(KN003,2),IX(KN003,3),ICD(KN001),
C    $          NMBL1(KN002),NMBL2(KN002)
      CHARACTER*4 LINE(33)      ! Modificato da INTEGER implicito
      CHARACTER*4 IA(KN003,2)   ! Modificato da INTEGER implicito
      CHARACTER*4 IX(KN003,3)   ! Modificato da INTEGER implicito (NOTA: legge 2A4,A2)
      CHARACTER*4 ICD(KN001)    ! Modificato da INTEGER implicito
      CHARACTER*4 NMBL1(KN002)  ! Modificato da INTEGER implicito
      CHARACTER*4 NMBL2(KN002)  ! Modificato da INTEGER implicito
C
      CHARACTER*15 IAV(KN003)
      CHARACTER*88 IBV(KN003)
C
      CHARACTER*100 INPFIL,OLDFIL,OUTFIL
      CHARACTER*4 IEOF          ! Modificato da INTEGER implicito
C
      DATA IEOF/'EOF'/
101      FORMAT(A)
102      FORMAT(//)
      INPFIL='proc/f14.dat'
      OLDFIL='f14.dat'
      OUTFIL='f14.out'
C
    2 OPEN(UNIT=1,FILE=INPFIL,STATUS='OLD')
      OPEN(UNIT=3,FILE=OLDFIL,STATUS='OLD')
      OPEN(UNIT=2,FILE=OUTFIL,STATUS='UNKNOWN')
C
      MX1=KN003
      REWIND 1
      REWIND 2
      IPOSF1=0
      IRCF1I=0
C
 10   CALL PFILE5(IA,IX,ICD,MX1,NMBL1,NMBL2,IPDT,NBL,NVAR,LINE,
     $            IAV,IBV)
      CALL PFILE6(LINE,NVAR,IA,IX,MX1,ICD,NBL,NMBL1,NMBL2,IPDT,
     $            IAV,IBV)
      CALL LGSTOP
      STOP
      END
C
C
C
      SUBROUTINE PFILE3(NF,NREC,IGO,LINE,IPOSF1)
C --- Dichiarazioni Originali e Modificate ---
      INTEGER NF, NREC, IGO, IPOSF1, KEOF, I
C     DIMENSION LINE(*)
      CHARACTER*4 LINE(*)       ! Modificato da INTEGER implicito (dummy)
C
      CHARACTER*4 IEOF, ILG       ! Modificato da INTEGER implicito
      CHARACTER*1 IAS           ! Modificato da INTEGER implicito
      CHARACTER*4 IFIS, IBLO    ! Modificato da INTEGER implicito
C
      DATA IEOF/'EOF'/,ILG/'*LG*'/,IAS/'*'/,IFIS/' FIS'/,IBLO/' BLO'/
C
C      IGO =1  COMMENTO
C          =2  VARIABILI
C          =3  BLOCCO
C          =4  DATI
C          =5  EOF
C
      IGO=0
      READ(NF,1000)(LINE(I),I=1,33)
 1000 FORMAT(33A4)
      NREC=NREC+1
      IF(LINE(2).NE.IEOF)GO TO 10
      KEOF=1
      IGO=5
      GO TO 100
C
   10 IF(LINE(1).EQ.IAS)GO TO 30
      IF(LINE(1).EQ.ILG)GO TO 40
      GO TO (15,20),IPOSF1
   15 IGO=2
      GO TO 100
   20 IGO=4
      GO TO 100
   30 IGO=1
      GO TO 100
   40 IGO=1
      IF(IPOSF1.NE.0)GO TO 45
      IPOSF1=1
      GO TO 100
   45 IF(LINE(3).EQ.IFIS)GO TO 50
      IF(LINE(4).EQ.IBLO)IGO=3
      GO TO 100
   50 IPOSF1=2
  100 RETURN
      END
C
C
C
      SUBROUTINE PFILE5(IA,IX,ICD,MX1,NMBL1,NMBL2,IPDT,NBL,NVAR,LINE,
     $                  IAV,IBV)
C
C      SUBROUTINE PER LA LETTURA DEI SIMBOLI DEI DATI E DEI VALORI
C      DAL FILE CONSIDERATO VECCHIO (3)
C
C --- Dichiarazioni Originali e Modificate ---
      INTEGER MX1, NBL, NVAR, N, KBL, M, I
      INTEGER IPDT(*)
C
C     DIMENSION LINE(*),IB(3,2),IP(3,3),IA(MX1,2),IX(MX1,3),ICD(*),
C    $   NMBL1(*),NMBL2(*),IPDT(*)
      CHARACTER*4 LINE(*)       ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IA(MX1,2)   ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IX(MX1,3)   ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 ICD(*)      ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 NMBL1(*)    ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 NMBL2(*)    ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IB(3,2)     ! Modificato da INTEGER implicito
      CHARACTER*4 IP(3,3)     ! Modificato da INTEGER implicito (NOTA: legge 2A4,A2)
C
      CHARACTER*(*) IAV(*)
      CHARACTER*(*) IBV(*)
      CHARACTER*132 LIN
C
      CHARACTER*4 ILG, IDT, IEOFR, IEOF  ! Modificato da INTEGER implicito
      CHARACTER*1 IAS                   ! Modificato da INTEGER implicito
      CHARACTER*4 IKO                   ! Modificato da INTEGER implicito (letta con A4)
C
      DATA ILG/'*LG*'/,IDT/'DATI'/,IEOFR/' '/,IEOF/'EOF'/,
     $     IAS/'*'/
C
      REWIND 3
      DO 1 I=1,4
      READ(3,1000)
    1 CONTINUE
      N=0
C
C      LETTURA VARIABILI
C
    2 N=N+1
C     Formato originale era: 3A4,2X,2A4,A2,A,A4,1X,A
C     IKO letto con il primo A4
      READ(3,1000)IKO,IA(N,1),IA(N,2),IX(N,1),IX(N,2),IX(N,3),
     $            IAV(N)(1:15),ICD(N),IBV(N)(1:88)
 1000 FORMAT(  A4,2A4,2X,2A4,A2,A,A4,1X,A) ! Modificato primo 3A4 -> A4,2A4
      IF(IKO.NE.ILG)GO TO 2
      N=N-1
      IF(IA(N+1,1).NE.IDT)GO TO 2
C
C      LETTURA DATI DEI BLOCCHI
C
      KBL=0
      IPDT(1)=N
      NVAR=N
   20 READ(3,1001)(LINE(I),I=1,33)
 1001 FORMAT(33A4)
      IF(LINE(2).EQ.IEOF)GO TO 50
      IF(LINE(1).EQ.IAS)GO TO 20
C
      IF(LINE(1).NE.ILG)GO TO 30
C------BLOCCO
      KBL=KBL+1
      IPDT(KBL+1)=IPDT(KBL)
      IPDT(KBL)=IPDT(KBL)+1
      NMBL1(KBL)=LINE(6)
      NMBL2(KBL)=LINE(7)
      GO TO 20
C------DATI DI UN BLOCCO
   30 WRITE(LIN,1001)(LINE(I),I=1,33)
      READ(LIN,1002)(IB(I,1),IB(I,2),IP(I,1),IP(I,2),IP(I,3),I=1,3)
 1002 FORMAT(3(4X,2A4,2X,2A4,A2,1X))
      M=1
      IF(IB(2,1).NE.IEOFR)M=2
      IF(IB(3,1).NE.IEOFR)M=3
      IPDT(KBL+1)=IPDT(KBL+1)+M
      DO 40 I=1,M
      N=N+1
      IA(N,1)=IB(I,1)
      IA(N,2)=IB(I,2)
      IX(N,1)=IP(I,1)
      IX(N,2)=IP(I,2)
      IX(N,3)=IP(I,3)
   40 CONTINUE
      GO TO 20
   50 NBL=KBL
      NSY=N ! NSY non viene usato, ma lo lascio per non modificare troppo
      IPDT(KBL+1)=NSY+1
      RETURN
      END
C
C
C
      SUBROUTINE PFILE6(LINE,NVAR,IA,IX,MX1,ICD,NBL,NMBL1,NMBL2,IPDT,
     $                 IAV,IBV)
C --- Dichiarazioni Originali e Modificate ---
      INTEGER NVAR, MX1, NBL, IPOSF1, IPRIM, IPRIM1, IGO, K, KPTB
      INTEGER M, I1, I2, I, J, KK
      INTEGER IPDT(*)
C     DIMENSION LINE(*),IA(MX1,2),IX(MX1,3),ICD(*),NMBL1(*),NMBL2(*),
C    $          IPDT(*),INDT(3,2),IVDT(3,3)
      CHARACTER*4 LINE(*)       ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IA(MX1,2)   ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IX(MX1,3)   ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 ICD(*)      ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 NMBL1(*)    ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 NMBL2(*)    ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 INDT(3,2)   ! Modificato da INTEGER implicito
      CHARACTER*4 IVDT(3,3)   ! Modificato da INTEGER implicito
C
      CHARACTER*(*) IAV(1)
      CHARACTER*(*) IBV(1)
      CHARACTER*132 PLIN,LIN
C
      CHARACTER*4 IEOFR, IBL, IEOFR1 ! Modificato da INTEGER implicito
C
      DATA IEOFR/' '/,IBL/' '/,IEOFR1/' '/
C
      REWIND 3
C
      READ (1,1000)(LINE(I),I=1,33)
      READ(3,1000)
      WRITE(2,1000)(LINE(I),I=1,33)
C
      READ (1,1000)
      READ (3,1000)(LINE(I),I=1,33)
      WRITE(2,1000)(LINE(I),I=1,33)
C
      READ (1,1000)
      READ (3,1000)(LINE(I),I=1,33)
      WRITE(2,1000)(LINE(I),I=1,33)
C
      READ (1,1000)
      READ (3,1000)(LINE(I),I=1,33)
      WRITE(2,1000)(LINE(I),I=1,33)
C
      IPOSF1=0
      IPRIM=0
      IPRIM1=0
    1 CALL PFILE3(1,NREC,IGO,LINE,IPOSF1) ! NREC non inizializzato qui, usa valore dal chiamante? Potenziale problema.
      WRITE(PLIN,1000)(LINE(KK),KK=1,33)
      GO TO (10,20,40,60,10),IGO
C
   10 WRITE(2,1000)(LINE(I),I=1,33)
 1000 FORMAT(33A4)
      IF(IGO.NE.5)GO TO 1
      GO TO 80
C-----VARIABILI
   20 DO 25 I=1,NVAR
      IF(LINE(2).NE.IA(I,1))GO TO 25
C     Aggiunto .OR. con chiamata a funzione esterna (che DEVE ritornare INTEGER)
      IF((LINE(3).EQ.IA(I,2)).OR.
     &   (IVERECC(LINE(3),IA(I,2)).EQ.1))GOTO 30
   25 CONTINUE
C
      IF(IPRIM.EQ.1)GO TO 26
      WRITE(6,*)' '
      WRITE(6,*)'**** VARIABILI DEL MODELLO ATTUALE'
      WRITE(6,*)'     NON RITROVATE NEL MODELLO DA COPIARE'
      WRITE(6,*)' '
      WRITE(6,*)' '
      IPRIM=1
  26  CONTINUE
      WRITE(6,1000)(LINE(I),I=1,33)
      WRITE(2,1000)(LINE(I),I=1,33)
      GO TO 1
   30 K=I
      WRITE(2,1001)PLIN(1:12),(IX(K,J),J=1,3),
     $              PLIN(25:39),ICD(K),PLIN(45:132)
 1001 FORMAT(A,' =',2A4,A2,A,A4,'*',A) ! Usa IX con 2A4,A2 - OK con IX CHARACTER*4(3)
      GO TO 1
C
C-----BLOCCO
C
   40 DO 45 I=1,NBL
      IF(LINE(6).NE.NMBL1(I))GO TO 45
C     Aggiunto .OR. con chiamata a funzione esterna (che DEVE ritornare INTEGER)
      IF((LINE(7).EQ.NMBL2(I)).OR.(IVERECC(LINE(7),NMBL2(I)).EQ.1))
     & GO TO 50
   45 CONTINUE
C
      IF(IPRIM1.EQ.1)GO TO 46
      WRITE(6,*)' '
      WRITE(6,*)'**** BLOCCHI DEL MODELLO ATTUALE'
      WRITE(6,*)'     NON RITROVATI NEL MODELLO DA COPIARE'
      WRITE(6,*)' '
      WRITE(6,*)' '
      IPRIM1=1
  46  CONTINUE
      WRITE(6,1000)(LINE(I),I=1,33)
      KPTB=0
      GO TO 55
   50 KPTB=I
   55 WRITE(2,1000)(LINE(I),I=1,33)
      GO TO 1
C
C----DATI DI UN BLOCCO
C
   60 IF(KPTB.EQ.0)GO TO 55 ! Se il blocco non è stato trovato, salta la scrittura dei dati
      WRITE(LIN,1000)(LINE(I),I=1,33)
      READ(LIN,1002)((INDT(I,J),J=1,2),I=1,3)
 1002 FORMAT(3(4X,2A4,13X))
      M=1
      IF(INDT(2,1).EQ.IEOFR1)GO TO 63
      M=2
      IF(INDT(3,1).EQ.IEOFR)GO TO 63
      M=3
   63 CONTINUE
      I1=IPDT(KPTB)
      I2=IPDT(KPTB+1)-1
      DO 70 I=1,M
      DO 65 J=I1,I2
      IF(INDT(I,1).NE.IA(J,1))GO TO 65
      IF(INDT(I,2).EQ.IA(J,2))GO TO 67
   65 CONTINUE
C     Blocco non trovato o dato non trovato nel blocco sorgente (KPTB)
      WRITE(6,3003)INDT(I,1),INDT(I,2),NMBL1(KPTB),NMBL2(KPTB)
 3003 FORMAT(/5X,'***DATO ',2A4,' BL. ',2A4,' NON TROVATO',
     $       ' NEL MODELLO SORGENTE PER QUESTO BLOCCO'/
     $       5X,'***VERRA'' SCRITTO COME BLANK NEL FILE DI OUTPUT'/)
      IVDT(I,1)=IBL
      IVDT(I,2)=IBL
      IVDT(I,3)=IBL
      GO TO 70
C     Dato trovato, copia i valori da IX(J,*) a IVDT(I,*)
   67 IVDT(I,1)=IX(J,1)
      IVDT(I,2)=IX(J,2)
      IVDT(I,3)=IX(J,3)
   70 CONTINUE
      WRITE(2,1004)((INDT(I,J),J=1,2),(IVDT(I,J),J=1,3),I=1,M)
 1004 FORMAT(3(4X,2A4,' =',2A4,A2,'*')) ! Usa IVDT con 2A4,A2 - OK con IVDT CHARACTER*4(3)
      GO TO 1
C
   80 WRITE(6,3005)
 3005 FORMAT(//5X,'ESECUZIONE TERMINATA')
      RETURN
      END
C
C
C NOTA: La funzione IVERECC ritorna INTEGER ma prende argomenti CHARACTER
C
      INTEGER FUNCTION IVERECC(IBLDEST,IBLSORG)
C --- Dichiarazioni Originali e Modificate ---
      PARAMETER(NSORG=100,NDEST=100)
C
      CHARACTER*4 IBLDEST     ! Modificato da INTEGER implicito (dummy)
      CHARACTER*4 IBLSORG     ! Modificato da INTEGER implicito (dummy)
C
C      integer*4 IECC(NSORG,NDEST),NECC(NSORG) <--- Originale
      CHARACTER*4 IECC(NSORG,NDEST) ! Modificato da INTEGER
      INTEGER NECC(NSORG)
      INTEGER IPRIMAVOLTA,ICEFILE,NUMECC
      INTEGER I, J, JJ ! Dichiarate esplicitamente
C
      COMMON/EDI14ECC/IECC,NECC,NUMECC,IPRIMAVOLTA,ICEFILE
C
      CHARACTER*132 LINEA, FINP
C
C GUAG 17 maggio 2012
C (commenti originali omessi per brevità)
C      print*,'IPRIMAVOLTA ',IPRIMAVOLTA,' ICEFILE', ICEFILE

      IF(IPRIMAVOLTA.EQ.0) then
        ICEFILE=0
C        call leggi_inp(IECC,NECC,NUMECC,ICEFILE) <--- chiamata originale commentata
        call leggi_inp() ! Chiama la subroutine che ora usa il COMMON
        IPRIMAVOLTA=1
C       print*,'Debug: IPRIMAVOLTA ',IPRIMAVOLTA,' ICEFILE', ICEFILE
      ENDIF
      if (ICEFILE.EQ.0) then
        IVERECC=0
C       print*,'NO File'
        RETURN
      endif

!     (Blocco di stampa commentato nell'originale)
!        DO JJ=1,NUMECC
!        DO J=1,NECC(JJ)
!        print*,'IVERECC - int  IECC(',JJ,',',J,')=',IECC(JJ,J)
!        ENDDO
!        ENDDO

      DO 100 I=1,NUMECC
      IF(IBLSORG.EQ.IECC(I,1)) GO TO 10
  100 CONTINUE
      GO TO 300

   10 DO 200 J=1,NECC(I)
      IF(IBLDEST.EQ.IECC(I,J)) THEN
       IVERECC=1
C      PRINT*,'Debug: IVERECC=',IVERECC, 'IBLDEST=',IBLDEST,
C    &        ') NECC(',I,')=',NECC(I),' IBLSORG=',IBLSORG
       RETURN
      ENDIF
  200 CONTINUE

  300 IVERECC=0
      RETURN
      END

      SUBROUTINE leggi_inp()
C --- Dichiarazioni Originali e Modificate ---
      PARAMETER(NSORG=100,NDEST=100)
C
C      integer*4 IECC(NSORG,NDEST),NECC(NSORG) <--- Originale in COMMON
      CHARACTER*4 IECC(NSORG,NDEST) ! Modificato da INTEGER (via COMMON)
      INTEGER NECC(NSORG)           ! Via COMMON
      INTEGER IPRIMAVOLTA,ICEFILE,NUMECC ! Via COMMON
      INTEGER lunlin, iposblank, I, K, II, KK, JJ, J ! Dichiarate esplicitamente
C
      COMMON/EDI14ECC/IECC,NECC,NUMECC,IPRIMAVOLTA,ICEFILE
C
      CHARACTER*132 LINEA, FINP, FOUT
      CHARACTER*4 SECC      ! Modificato da INTEGER implicito
      CHARACTER*4 IBL       ! Modificato da INTEGER implicito
C     integer*4 IBL,ISORG   <- ISORG non usato, rimosso dalla dichiarazione
C
C        integer*4 IECC(1,1),NECC(1) <--- Commentato nell'originale

        NUMECC=0
C       PRINT*, 'edi14: leggo edi14_eccezioni.inp'
        FINP='edi14_eccezioni.inp'
        FOUT='edi14_eccezioni_letto.out'
        OPEN(UNIT=27,FILE=FINP,STATUS='OLD', ERR=2000)
        OPEN(UNIT=28,FILE=FOUT,STATUS='UNKNOWN', ERR=50)
        rewind(28)
   50   READ(27,'(A)', END=1000) LINEA

        lunlin=LEN_TRIM(LINEA) ! Usiamo LEN_TRIM se disponibile, o LEN
        iposblank=INDEX(LINEA,' ')
C       print*, lunlin,'Debug: iposblank=',iposblank,'=',LINEA(1:lunlin)

        I=1
        K=0
        NUMECC=NUMECC+1
        if (NUMECC.GT.NSORG) THEN
          Print*,'edi14: ERRORE. Max righe eccezioni superato:',NSORG
          STOP
        ENDIF
  100   read(LINEA(I:I+3),'(A4)') IBL

C       (Blocco di stampa commentato nell'originale)
C        print*,'I=',I,' ',
C     &   LINEA(I:I+3),' int  IECC(',NUMECC,',',K,')=',IECC(NUMECC,K)

C   Cerco eventuali catene di sorgenti e destinazioni
C   (Logica originale mantenuta)


        do II=1,NUMECC-1 ! Evita confronto con se stesso
          do KK=1,NECC(II)
           if(IBL.EQ.IECC(II,KK)) then
C             Verifica se c'è un prossimo elemento nella linea corrente
              if (INDEX(LINEA(I:lunlin),',') .GT. 0) then
                 IF (I+5 .LE. lunlin) THEN ! Assicura che ci siano abbastanza caratteri
                    NECC(II)=NECC(II)+1
                    if (NECC(II).GT.NDEST) THEN
      Print*,'edi14: ERRORE. Max destinazioni per ', IECC(II,1)
      Print*,' superato:', NDEST
                       STOP
                    ENDIF
                    if (LINEA(I+4:I+4).NE.',') THEN
      Print*,'edi14: ERRORE formato eccezioni vicino a:', LINEA(I:I+4)
                       STOP
                    ENDIF
                    read(LINEA(I+5:I+5+3),'(A4)') IECC(II,NECC(II))
                    NUMECC=NUMECC-1
                    GO TO 50
                 ELSE
      Print*,'edi14: ERRORE formato eccezioni, linea corta:'
      Print*, LINEA(1:lunlin)
                    STOP
                 ENDIF
              else
C               Non c'è virgola dopo IBL, questa riga è finita
C               Non faccio nulla qui, vado avanti a leggere prossima riga
                 NUMECC=NUMECC-1 ! Annulla incremento NUMECC precedente
                 GO TO 50
              endif
           endif
          enddo
         enddo

C     Nessuna catena trovata, aggiungi IBL alla riga corrente NUMECC
        K=K+1
        IF(K.GT.NDEST) THEN
      Print*,'edi14: ERRORE. Max destinazioni per ', IECC(NUMECC,1)
      Print*,'superato:', NDEST
          STOP
        ENDIF
        IECC(NUMECC,K)=IBL
        NECC(NUMECC)=K

C     Verifica se c'è una virgola per continuare sulla stessa riga
        if (I+4 .LE. lunlin .AND. LINEA(I+4:I+4) .EQ. ',') then
           I=I+5
           IF (I .LE. lunlin) THEN ! Assicura che ci sia qualcosa dopo la virgola
              GO TO 100
           ELSE ! Virgola alla fine della linea, considerala terminata
              GO TO 50
           ENDIF
        else ! Nessuna virgola o fine linea
          GO TO 50
        endif

 1000   continue
        CLOSE(27)

C       (Blocco di stampa commentato nell'originale)
C        print*,'Debug: NUMECC',NUMECC
C        DO JJ=1,NUMECC
C        DO J=1,NECC(JJ)
C        write(SECC,'(A4)') IECC(JJ,J)
C        print*,'Debug: int  IECC(',JJ,',',J,')=',IECC(JJ,J),' ',SECC
C        ENDDO
C        ENDDO

C     Scrivi il file di output con le eccezioni lette
        DO JJ=1,NUMECC
           WRITE(28,1002) (IECC(JJ,J), J=1,NECC(JJ))
 1002      FORMAT(A4,19(',' ,A4)) ! Formato per scrivere fino a 20 blocchi per riga
        ENDDO

        CLOSE(28)

        ICEFILE=1
        RETURN
 2000   ICEFILE=0
        PRINT*, 'edi14: ATTENZIONE! File edi14_eccezioni.inp '
        PRINT*, 'non trovato.'
        PRINT*, '         Nessuna eccezione di copia verra'' applicata.'
        RETURN
        end
