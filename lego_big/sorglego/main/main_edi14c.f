C*********************************************************************
C       Fortran Source:             main_edi14c.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Wed May 12 12:46:50 2004 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_main_edi14c_f
      CHARACTER*80  RepoID
      COMMON /CM_main_edi14c_f / RepoID
      DATA RepoID/'@(#)1,fsrc,main_edi14c.f,3'/
      END
C******************************************************************************
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
      INTEGER IO ! Usato nel blocco commentato
C
C     DIMENSION LINE(33)
C     DIMENSION IA(KN003,2),IX(KN003,3),ICD(KN001),
C    &          NMBL1(KN002),NMBL2(KN002),IPDT(KN002)
      CHARACTER*4 LINE(33)      ! Modificato
      CHARACTER*4 IA(KN003,2)   ! Modificato
      CHARACTER*4 IX(KN003,3)   ! Modificato (legge anche A2, ma base e` A4)
      CHARACTER*4 ICD(KN001)    ! Modificato
      CHARACTER*4 NMBL1(KN002)  ! Modificato
      CHARACTER*4 NMBL2(KN002)  ! Modificato
C
      CHARACTER*15 IA_V(KN003)
      CHARACTER*88 IB_V(KN003)
C
      CHARACTER*100 INPUT_FILE,OLD_FILE,OUT_FILE
      CHARACTER*4 IEOF          ! Modificato
C
      DATA IEOF/'EOF'/
C100      FORMAT(5X,$,A)
101      FORMAT(A)
102      FORMAT(//)
C
C    Il programma EDI14 puo` essere usato da LEGOCAD o
C    dall'utente direttamente.
C    EDI14 ha quindi due diverse modalita` di lavoro.
C    EDI14 riconosce di essere chiamato da LEGOCAD
C    se ,leggendo il file 4 ,trova un dato.
C
C
C      READ(4,*,ERR=99)IO
C
C_______ modalita` di LEGOCAD
C
C      INPUT_FILE='[.PROC]F14.DAT'
C      OLD_FILE='F14.DAT'
C      OUT_FILE='F14.DAT'
C
C      IF(IO.EQ.1)GO TO 2
C
C      INPUT_FILE='F14.DAT'
C      OLD_FILE='F14.DAT'
C      OUT_FILE='[.PROC]F14.DAT'
C________ LETTURA DEL NOME DEL FILE "VECCHIO" DA CUI COPIARE I DATI
C      READ(4,101) OLD_FILE
C      GO TO 2
C
C
C_______ modalita` diretta
C
C
  99  CONTINUE
      PRINT *,'           PROGRAMMA EDI14'
      PRINT *,'           ==============='
      PRINT *,' '
      PRINT *,'           TRASFERISCE I DATI FISICI E GEOMETRICI'
      PRINT *,' '
      PRINT *,'           DAL FILE F14 O F24 DI UN DATO MODELLO '
      PRINT *,' '
      PRINT *,'           AL MODELLO ATTUALE'
      PRINT *,' '
      PRINT *,'   F14.DAT + FILE_CHE_MI_DICI ====> EDI14.OUT'
      PRINT *,' '
      PRINT *,'  NOME DEL FILE DEL MODELLO DA CUI COPIARE I DATI ?   '
C________ LETTURA DEL NOME DEL FILE "VECCHIO" DA CUI COPIARE I DATI
      READ(5,101) OLD_FILE
      INPUT_FILE='f14.dat'
      OUT_FILE='edi14.out'
C
   2  OPEN(UNIT=1,FILE=INPUT_FILE,STATUS='OLD', ERR=200)
      OPEN(UNIT=3,FILE=OLD_FILE,STATUS='OLD', ERR=210)
      OPEN(UNIT=2,FILE=OUT_FILE,STATUS='UNKNOWN', ERR=220)
C
      MX1=KN003
      REWIND 1
      REWIND 2
      IPOSF1=0
      IRCF1I=0
C
 10   CALL PFILE5(IA,IX,ICD,MX1,NMBL1,NMBL2,IPDT,NBL,NVAR,LINE,
     &            IA_V,IB_V)
      CALL PFILE6(LINE,NVAR,IA,IX,MX1,ICD,NBL,NMBL1,NMBL2,IPDT,
     &            IA_V,IB_V)
      STOP 'Esecuzione EDI14 terminata regolarmente.'
C
 200  STOP 'ERRORE: Impossibile aprire il file di input F14 attuale.'
 210  STOP 'ERRORE: Impossibile aprire il file sorgente specificato.'
 220  STOP 'ERRORE: Impossibile creare il file di output.'
      END
C
C-----------------------------------------------------------------------
C
      SUBROUTINE PFILE3(NF,NREC,IGO,LINE,IPOSF1)
C --- Dichiarazioni Originali e Modificate ---
      INTEGER NF, NREC, IGO, IPOSF1, KEOF, I
C     DIMENSION LINE(*)
      CHARACTER*4 LINE(*)       ! Modificato (dummy)
C
      CHARACTER*4 IEOF, ILG       ! Modificato
      CHARACTER*1 IAS           ! Modificato
      CHARACTER*4 IFIS, IBLO    ! Modificato
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
      READ(NF,1000,END=900)(LINE(I),I=1,33) ! Aggiunto END= per fine file inatteso
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
C
 900  IGO=5 ! Tratta la fine del file come EOF
      KEOF=1
      PRINT *, '*** PFILE3: Raggiunta fine file inaspettata su unita ',echo $FFLAGS NF
      RETURN
      END
C
C-----------------------------------------------------------------------
C
      SUBROUTINE PFILE5(IA,IX,ICD,MX1,NMBL1,NMBL2,IPDT,NBL,NVAR,LINE,
     &                  IA_V,IB_V)
C
C      SUBROUTINE PER LA LETTURA DEI SIMBOLI DEI DATI E DEI VALORI
C      DAL FILE CONSIDERATO VECCHIO (3)
C
C --- Dichiarazioni Originali e Modificate ---
      INTEGER MX1, NBL, NVAR, N, KBL, M, I, NSY
      INTEGER IPDT(*)
C
C     DIMENSION LINE(*),IB(3,2),IP(3,3),IA(MX1,2),IX(MX1,3),ICD(*),
C    &   NMBL1(*),NMBL2(*),IPDT(*)
      CHARACTER*4 LINE(*)       ! Modificato (dummy)
      CHARACTER*4 IA(MX1,2)   ! Modificato (dummy)
      CHARACTER*4 IX(MX1,3)   ! Modificato (dummy)
      CHARACTER*4 ICD(*)      ! Modificato (dummy)
      CHARACTER*4 NMBL1(*)    ! Modificato (dummy)
      CHARACTER*4 NMBL2(*)    ! Modificato (dummy)
      CHARACTER*4 IB(3,2)     ! Modificato
      CHARACTER*4 IP(3,3)     ! Modificato
C
      CHARACTER*(*) IA_V(*)
      CHARACTER*(*) IB_V(*)
      CHARACTER*132 LIN
C
      CHARACTER*4 ILG, IDT, IEOFR, IEOF  ! Modificato
      CHARACTER*1 IAS                   ! Modificato
      CHARACTER*4 IKO                   ! Modificato (letta con A4)
C
      DATA ILG/'*LG*'/,IDT/'DATI'/,IEOFR/' '/,IEOF/'EOF'/,
     &     IAS/'*'/
C
      REWIND 3
      DO 1 I=1,4
      READ(3,1000,END=990) ! Lettura intestazione, aggiunto END=
    1 CONTINUE
      N=0
C
C      LETTURA VARIABILI
C
    2 N=N+1
      IF (N .GT. MX1) THEN
         PRINT *, '*** ERRORE PFILE5: Troppe linee nel file sorgente.'
         PRINT *, '*** Aumentare KN003 (attualmente ', MX1,')'
         STOP 'Errore dimensione array in PFILE5'
      ENDIF
C     Formato originale: 3A4,2X,2A4,A2,A,A4,1X,A
C     Modificato per leggere IKO separatamente e allineare con le var CHARACTER*4
      READ(3,1000,END=990) IKO,IA(N,1),IA(N,2),IX(N,1),IX(N,2),IX(N,3),
     &            IA_V(N)(1:15),ICD(N),IB_V(N)(1:88)
 1000 FORMAT(  A4,2A4,2X,2A4,A2,1X,A15,1X,A4,1X,A) ! Adattato per IA_V, ICD, IB_V
      IF(IKO.NE.ILG)GO TO 2
C     Riga *LG* trovata, l'ultima lettura era l'inizio dei dati o blocchi
      N=N-1
      IF(IA(N+1,1).NE.IDT) THEN
         PRINT *, '*** ERRORE PFILE5: Attesa riga *LG*DATI dopo variabili'
         PRINT *, '*** Trovato invece: ', IA(N+1,1)
         STOP 'Errore formato file sorgente in PFILE5 (1)'
      ENDIF
C
C      LETTURA DATI DEI BLOCCHI
C
      KBL=0
      IPDT(1)=N
      NVAR=N
   20 READ(3,1001,END=50)(LINE(I),I=1,33) ! Fine file qui va a 50
 1001 FORMAT(33A4)
      IF(LINE(2).EQ.IEOF)GO TO 50
      IF(LINE(1).EQ.IAS)GO TO 20
C
      IF(LINE(1).NE.ILG)GO TO 30
C------BLOCCO
      KBL=KBL+1
      IF (KBL .GE. KN002) THEN ! Controllo limiti array NMBL*, IPDT
         PRINT *, '*** ERRORE PFILE5: Troppi blocchi nel file sorgente.'
         PRINT *, '*** Aumentare KN002 (attualmente ', KN002,')'
         STOP 'Errore dimensione array blocchi in PFILE5'
      ENDIF
      IPDT(KBL+1)=IPDT(KBL) ! Prepara puntatore per prossimo blocco
      IPDT(KBL)=IPDT(KBL)+1 ! Incrementa puntatore corrente (per nome blocco)
      NMBL1(KBL)=LINE(6)
      NMBL2(KBL)=LINE(7)
      GO TO 20
C------DATI DI UN BLOCCO
   30 WRITE(LIN,1001)(LINE(I),I=1,33)
      READ(LIN,1002)(IB(I,1),IB(I,2),IP(I,1),IP(I,2),IP(I,3),I=1,3)
 1002 FORMAT(3(4X,2A4,2X,2A4,A2,1X)) ! Legge identificativi e valori come char
      M=1
      IF(IB(2,1).NE.IEOFR)M=2
      IF(IB(3,1).NE.IEOFR)M=3
      IF (KBL .EQ. 0) THEN
         PRINT *, '*** ERRORE PFILE5: Trovati dati prima di un blocco *LG*'
         STOP 'Errore formato file sorgente in PFILE5 (2)'
      ENDIF
      IPDT(KBL+1)=IPDT(KBL+1)+M ! Incrementa puntatore fine blocco
      DO 40 I=1,M
      N=N+1
      IF (N .GT. MX1) THEN
         PRINT *, '*** ERRORE PFILE5: Troppe linee nel file sorgente.'
         PRINT *, '*** Aumentare KN003 (attualmente ', MX1,')'
         STOP 'Errore dimensione array in PFILE5'
      ENDIF
      IA(N,1)=IB(I,1)
      IA(N,2)=IB(I,2)
      IX(N,1)=IP(I,1)
      IX(N,2)=IP(I,2)
      IX(N,3)=IP(I,3)
   40 CONTINUE
      GO TO 20
   50 NBL=KBL
      NSY=N
      IF (KBL+1 .GT. KN002) THEN ! Controllo limite per IPDT(KBL+1)
         PRINT *, '*** ERRORE PFILE5: Indice blocco fuori limite.'
         STOP 'Errore indice array blocchi in PFILE5'
      ENDIF
      IPDT(KBL+1)=NSY+1 ! Segna la fine dell'ultimo blocco + 1
      RETURN
C
 990  PRINT *, '*** ERRORE PFILE5: Fine file inattesa durante lettura.'
      STOP 'Errore lettura file sorgente in PFILE5'
      END
C
C-----------------------------------------------------------------------
C
      SUBROUTINE PFILE6(LINE,NVAR,IA,IX,MX1,ICD,NBL,NMBL1,NMBL2,IPDT,
     &                 IA_V,IB_V)
C --- Dichiarazioni Originali e Modificate ---
      INTEGER NVAR, MX1, NBL, IPOSF1, IPRIM, IPRIM1, IGO, K, KPTB
      INTEGER M, I1, I2, I, J, KK, NREC ! NREC non inizializzato localmente
C
      INTEGER IPDT(*)
C     DIMENSION LINE(*),IA(MX1,2),IX(MX1,3),ICD(*),NMBL1(*),NMBL2(*),
C    &           IPDT(*)
C     DIMENSION INDT(3,2),IVDT(3,3)
      CHARACTER*4 LINE(*)       ! Modificato (dummy)
      CHARACTER*4 IA(MX1,2)   ! Modificato (dummy)
      CHARACTER*4 IX(MX1,3)   ! Modificato (dummy)
      CHARACTER*4 ICD(*)      ! Modificato (dummy)
      CHARACTER*4 NMBL1(*)    ! Modificato (dummy)
      CHARACTER*4 NMBL2(*)    ! Modificato (dummy)
      CHARACTER*4 INDT(3,2)   ! Modificato
      CHARACTER*4 IVDT(3,3)   ! Modificato
C
      CHARACTER*(*) IA_V(1)  ! Dichiarati ma non usati in questa sub
      CHARACTER*(*) IB_V(1)  ! Dichiarati ma non usati in questa sub
      CHARACTER*132 LIN
      CHARACTER*132 PLIN
C
      CHARACTER*4 IEOFR, IBL, IEOFR1 ! Modificato
C
      DATA IEOFR/' '/,IBL/' '/,IEOFR1/' '/
C
      REWIND 3 ! Unità 3 non viene letta qui, ma nell'originale c'era
C
C     Lettura/Scrittura Intestazione (4 righe)
      READ (1,1000,END=900) (LINE(I),I=1,33)
      WRITE(2,1000) (LINE(I),I=1,33)
C
      READ (1,1000,END=900) (LINE(I),I=1,33)
      WRITE(2,1000) (LINE(I),I=1,33)
C
      READ (1,1000,END=900) (LINE(I),I=1,33)
      WRITE(2,1000) (LINE(I),I=1,33)
C
      READ (1,1000,END=900) (LINE(I),I=1,33)
      WRITE(2,1000) (LINE(I),I=1,33)
C
      IPOSF1=0
      IPRIM=0
      IPRIM1=0
      NREC=4 ! Contatore righe lette da Unità 1
C
    1 CALL PFILE3(1,NREC,IGO,LINE,IPOSF1) ! NREC ora incrementato in PFILE3
      WRITE(PLIN,'(33A4)')(LINE(KK),KK=1,33)
      GO TO (10,20,40,60,10),IGO
C
   10 WRITE(2,1000)(LINE(I),I=1,33)
 1000 FORMAT(33A4)
      IF(IGO.NE.5)GO TO 1
      GO TO 80
C-----VARIABILI
   20 DO 25 I=1,NVAR
      IF(LINE(2).NE.IA(I,1))GO TO 25
C     Aggiunto .OR. con chiamata a funzione esterna
      IF((LINE(3).EQ.IA(I,2)).OR.
     &   (IVERECC(LINE(3),IA(I,2)).EQ.1))GOTO 30
   25 CONTINUE
C     Variabile non trovata nel file sorgente letto da PFILE5
      IF(IPRIM.EQ.0) THEN
         WRITE(6,*)' '
         WRITE(6,*)'**** VARIABILI DEL MODELLO ATTUALE (F14.DAT)'
         WRITE(6,*)'     NON RITROVATE NEL MODELLO SORGENTE:'
         WRITE(6,*)' '
         IPRIM=1
      ENDIF
      WRITE(6,*) '      > ', LINE(2), LINE(3)
      WRITE(2,1000)(LINE(I),I=1,33) ! Copia la linea originale da F14.DAT
      GO TO 1
C     Variabile trovata
   30 K=I
      WRITE(2,1001)PLIN(1:12),(IX(K,J),J=1,3),
     &              PLIN(25:39),ICD(K),PLIN(45:132)
 1001 FORMAT(A12,' =',2A4,A2,A15,A4,'*',A88) ! Adattato per PLIN e arrays
      GO TO 1
C
C-----BLOCCO
C
   40 DO 45 I=1,NBL
      IF(LINE(6).NE.NMBL1(I))GO TO 45
C     Aggiunto .OR. con chiamata a funzione esterna
      IF((LINE(7).EQ.NMBL2(I)).OR.(IVERECC(LINE(7),NMBL2(I)).EQ.1))
     & GO TO 50
   45 CONTINUE
C     Blocco non trovato nel file sorgente letto da PFILE5
      IF(IPRIM1.EQ.0) THEN
         WRITE(6,*)' '
         WRITE(6,*)'**** BLOCCHI DEL MODELLO ATTUALE (F14.DAT)'
         WRITE(6,*)'     NON RITROVATI NEL MODELLO SORGENTE:'
         WRITE(6,*)' '
         IPRIM1=1
      ENDIF
      WRITE(6,*) '      > Block: ', LINE(6), LINE(7)
      KPTB=0 ! Flag: non copiare dati per questo blocco
      GO TO 55
C     Blocco trovato
   50 KPTB=I
   55 WRITE(2,1000)(LINE(I),I=1,33) ! Scrive comunque l'header del blocco
      GO TO 1
C
C----DATI DI UN BLOCCO
C
   60 IF(KPTB.EQ.0) THEN ! Se il blocco non è stato trovato nel sorgente...
        WRITE(2,1000)(LINE(I),I=1,33) ! ...copia la linea dati originale da F14.DAT
        GO TO 1
      ENDIF
C     Blocco trovato nel sorgente, procedi alla copia/sostituzione dei dati
      WRITE(LIN,1000)(LINE(I),I=1,33)
      READ(LIN,1002)((INDT(I,J),J=1,2),I=1,3)
 1002 FORMAT(3(4X,2A4,13X))
      M=1
      IF(INDT(2,1).NE.IEOFR1)M=2
      IF(INDT(3,1).NE.IEOFR)M=3
   63 CONTINUE
      I1=IPDT(KPTB)
      I2=IPDT(KPTB+1)-1
      DO 70 I=1,M
      DO 65 J=I1,I2
      IF(INDT(I,1).NE.IA(J,1))GO TO 65
      IF(INDT(I,2).EQ.IA(J,2))GO TO 67
   65 CONTINUE
C     Dato specifico (INDT(I,*)) non trovato nel blocco corrispondente (KPTB) del sorgente
      WRITE(6,3003)INDT(I,1),INDT(I,2),NMBL1(KPTB),NMBL2(KPTB)
 3003 FORMAT(5X,'***DATO ',2A4,' nel BLOCCO ',2A4,
     &       ' non trovato nel file sorgente. Verra'' usato BLANK.')
      IVDT(I,1)=IBL
      IVDT(I,2)=IBL
      IVDT(I,3)=IBL
      GO TO 70
C     Dato specifico trovato, copia i valori da IX(J,*) a IVDT(I,*)
   67 IVDT(I,1)=IX(J,1)
      IVDT(I,2)=IX(J,2)
      IVDT(I,3)=IX(J,3)
   70 CONTINUE
C     Scrive la linea dati nell'output usando i valori trovati/blank (IVDT)
      WRITE(2,1004)((INDT(I,J),J=1,2),(IVDT(I,J),J=1,3),I=1,M)
 1004 FORMAT(3(4X,2A4,' =',2A4,A2,'*')) ! Usa IVDT con 2A4,A2
      GO TO 1
C
   80 WRITE(6,3005)
 3005 FORMAT(//5X,'ESECUZIONE TERMINATA')
      RETURN
C
 900  PRINT *, '*** PFILE6: Fine file inattesa leggendo F14.DAT (unita 1)'
      STOP 'Errore lettura file F14.DAT'
      END
C
C-----------------------------------------------------------------------
C
      INTEGER FUNCTION IVERECC(IBLDEST,IBLSORG)
C --- Dichiarazioni Originali e Modificate ---
      PARAMETER(NSORG=100,NDEST=100)
C
      CHARACTER*4 IBLDEST     ! Modificato (dummy)
      CHARACTER*4 IBLSORG     ! Modificato (dummy)
C
C      integer*4 IECC(NSORG,NDEST),NECC(NSORG) <--- Originale
      CHARACTER*4 IECC(NSORG,NDEST) ! Modificato
      INTEGER NECC(NSORG)
      INTEGER IPRIMAVOLTA,ICEFILE,NUMECC
      INTEGER I, J, JJ ! Dichiarate esplicitamente
C
      COMMON/EDI14ECC/IECC,NECC,NUMECC,IPRIMAVOLTA,ICEFILE
C
      CHARACTER*132 LINEA, FINP
C
C GUAG 17 maggio 2012 (commenti omessi)
C      print*,'IPRIMAVOLTA ',IPRIMAVOLTA,' ICEFILE', ICEFILE

      IF(IPRIMAVOLTA.EQ.0) then
        ICEFILE=0
        call leggi_inp() ! Chiama la subroutine che usa il COMMON
        IPRIMAVOLTA=1
C       print*,'Debug: IPRIMAVOLTA ',IPRIMAVOLTA,' ICEFILE', ICEFILE
      ENDIF
      if (ICEFILE.EQ.0) then
        IVERECC=0
C       print*,'NO File' ! Messaggio stampato da leggi_inp se file manca
        RETURN
      endif

!     (Stampa debug commentata)
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
C
C-----------------------------------------------------------------------
C
      SUBROUTINE leggi_inp()
C --- Dichiarazioni Originali e Modificate ---
      PARAMETER(NSORG=100,NDEST=100)
C
      CHARACTER*4 IECC(NSORG,NDEST) ! Via COMMON
      INTEGER NECC(NSORG)           ! Via COMMON
      INTEGER IPRIMAVOLTA,ICEFILE,NUMECC ! Via COMMON
      INTEGER lunlin, iposblank, I, K, II, KK, JJ, J ! Dichiarate esplicitamente
C
      COMMON/EDI14ECC/IECC,NECC,NUMECC,IPRIMAVOLTA,ICEFILE
C
      CHARACTER*132 LINEA, FINP, FOUT
      CHARACTER*4 SECC      ! Modificato
      CHARACTER*4 IBL       ! Modificato
C     ISORG non usato, rimosso
C
        NUMECC=0
C       PRINT*, 'edi14: leggo edi14_eccezioni.inp'
        FINP='edi14_eccezioni.inp'
        FOUT='edi14_eccezioni_letto.out'
        OPEN(UNIT=27,FILE=FINP,STATUS='OLD', ERR=2000)
C       Aperura file output solo se input OK
        OPEN(UNIT=28,FILE=FOUT,STATUS='UNKNOWN', ERR=2010)
        rewind(28)
C       Lettura file eccezioni
   50   READ(27,'(A)', END=1000) LINEA

C       Usa LEN e gestisci blank esplicitamente per F77
        lunlin = 0
        DO 55 I=132,1,-1
          IF (LINEA(I:I) .NE. ' ') THEN
             lunlin = I
             GOTO 56
          ENDIF
   55   CONTINUE
   56   CONTINUE
C       Se lunlin e` 0, linea vuota, salta
        IF (lunlin .EQ. 0) GOTO 50

C       iposblank=INDEX(LINEA,' ') ! Meno utile senza LEN_TRIM
C       print*, lunlin,'Debug: iposblank=',iposblank,'=',LINEA(1:lunlin)

        I=1 ! Posizione corrente nella LINEA
        K=0 ! Contatore blocchi per la riga corrente (NUMECC)
        NUMECC=NUMECC+1
        if (NUMECC.GT.NSORG) THEN
          Print*,'edi14: ERRORE. Max righe eccezioni superato:',NSORG
          STOP 'Troppe righe in edi14_eccezioni.inp'
        ENDIF
C       Leggi primo blocco (o successivo) dalla posizione I
  100   IF (I+3 .GT. lunlin) THEN ! Controllo se ci sono abbastanza caratteri
           PRINT *, '*** edi14: Errore formato riga in ', FINP
           PRINT *, '*** Riga: ', LINEA(1:lunlin)
           GOTO 50 ! Salta alla prossima riga
        ENDIF
        read(LINEA(I:I+3),'(A4)') IBL

C   Logica per gestire catene: se IBL è già una *destinazione* in una
C   riga precedente (II), aggiungi le *nuove* destinazioni da questa
C   riga (LINEA) a quella riga precedente (II)
        do II=1,NUMECC-1 ! Controlla solo le righe precedenti
          do KK=1,NECC(II)
           if(IBL.EQ.IECC(II,KK)) then
C             IBL trovato come destinazione nella riga II.
C             Leggi la prossima destinazione dalla riga corrente (LINEA).
             IF (I+4 .GT. lunlin .OR. LINEA(I+4:I+4) .NE. ',') THEN
                 PRINT *, '*** edi14: Errore formato (virgola mancante?)'
                 PRINT *, '*** Riga: ', LINEA(1:lunlin)
                 GOTO 50 ! Errore formato, salta riga
             ENDIF
             IF (I+5+3 .GT. lunlin) THEN
                 PRINT *, '*** edi14: Errore formato (blocco corto?)'
                 PRINT *, '*** Riga: ', LINEA(1:lunlin)
                 GOTO 50 ! Errore formato, salta riga
             ENDIF
C             Aggiungi blocco successivo da LINEA a riga II
             NECC(II)=NECC(II)+1
             if (NECC(II).GT.NDEST) THEN
               Print*,'edi14: ERRORE. Max destinazioni per ',
     $                 IECC(II,1), ' superato:', NDEST
               STOP 'Troppe destinazioni in edi14_eccezioni.inp'
             ENDIF
             read(LINEA(I+5:I+5+3),'(A4)') IECC(II,NECC(II))
             NUMECC=NUMECC-1 ! Riga corrente era una continuazione, annulla ++
             GO TO 50 ! Riga corrente processata come continuazione
           endif
          enddo
         enddo

C     Nessuna catena trovata, IBL è (probabilmente) un nuovo sorgente
C     o una destinazione per la riga corrente NUMECC
        K=K+1
        IF(K.GT.NDEST) THEN
          Print*,'edi14: ERRORE. Max destinazioni per riga superato:',NDEST
          STOP 'Troppe destinazioni in edi14_eccezioni.inp'
        ENDIF
        IECC(NUMECC,K)=IBL
        NECC(NUMECC)=K

C     Controlla se c'è una virgola per continuare sulla stessa riga
        if (I+4 .LE. lunlin .AND. LINEA(I+4:I+4) .EQ. ',') then
           I=I+5
           IF (I .LE. lunlin) THEN ! Assicura che ci sia qualcosa dopo la virgola
              GO TO 100 ! Leggi prossimo blocco sulla stessa riga
           ELSE ! Virgola alla fine della linea, considerala terminata
              GO TO 50 ! Passa alla prossima riga del file
           ENDIF
        else ! Nessuna virgola o fine linea
          GO TO 50 ! Passa alla prossima riga del file
        endif

 1000   continue
        CLOSE(27)

C       Stampa debug su file output
C        print*,'Debug: NUMECC',NUMECC
C        DO JJ=1,NUMECC
C        DO J=1,NECC(JJ)
C        write(SECC,'(A4)') IECC(JJ,J)
C        print*,'Debug: char IECC(',JJ,',',J,')=',SECC
C        ENDDO
C        ENDDO

C       Scrivi il file di output con le eccezioni lette
        DO JJ=1,NUMECC
           WRITE(28,1002) (IECC(JJ,J), J=1,NECC(JJ))
 1002      FORMAT(A4,19(',' ,A4)) ! Formato per scrivere fino a 20 blocchi per riga
        ENDDO

        CLOSE(28)
        ICEFILE=1 ! Segnala che il file (o tentativo) è stato processato
        RETURN

 2000   ICEFILE=0
        PRINT*, ' '
        PRINT*, '*** edi14: ATTENZIONE! File edi14_eccezioni.inp',
     &          ' non trovato.'
        PRINT*, '***         Nessuna eccezione di copia verra'' applicata.'
        PRINT*, ' '
        RETURN
 2010   ICEFILE=0
        PRINT*, '*** edi14: ERRORE! Impossibile creare il file output ', FOUT
        CLOSE(UNIT=27, STATUS='KEEP') ! Prova a chiudere l'unità 27
C        IF (UNIT=27) CLOSE(27) ! Chiudi input se era aperto
        RETURN
        end