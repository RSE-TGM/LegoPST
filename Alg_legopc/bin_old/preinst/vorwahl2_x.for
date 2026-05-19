      PROGRAM VORWAHL2
C   VORWAHL2 per Linux 
      PARAMETER (MAXCONF=10)
      LOGICAL FINE,DIFFE
      CHARACTER*1 C1
      CHARACTER*2 C2
      CHARACTER*7 NOMEFILE
      CHARACTER*11 NOMEEXT
      CHARACTER*4 IOB
      CHARACTER*80 DEBL,COMMENTO,LINE, LINE_TEST
      DIMENSION NSTATI(MAXCONF),NUSCIT(MAXCONF),
     $          NINGRE(MAXCONF)
      DATA NSTATI,NUSCIT,NINGRE/MAXCONF*0,MAXCONF*0,MAXCONF*0/
      IEREGOL=0
      IOB='    '
      DO I=1,80
         DEBL(I:I)=' '
         COMMENTO(I:I)=' '
      ENDDO
      OPEN(UNIT=1,FILE='F',
     $     STATUS='UNKNOWN',FORM='FORMATTED')
      READ(1,'(A4)') NOMEFILE(1:4)
      READ(1,'(I2)') N
      READ(1,'(A)') COMMENTO
      REWIND(1)
      NCOUNT=0
C GUAG:
C Modifica per aumentare il numero di istanze 
C per modulo: massimo 99
      IF(N.LE.9) THEN
      	WRITE(C1,'(I1)') N
        NOMEFILE(5:6)='_'//C1
        NOMEEXT = NOMEFILE(1:6) // ".pi3"
        WRITE(1,'(A6)') NOMEFILE
      ELSE
      	WRITE(C2,'(I2)') N
        NOMEFILE(5:7)='_'//C2
        NOMEEXT = NOMEFILE(1:7) // ".pi3"
        WRITE(1,'(A7)') NOMEFILE
      ENDIF      	
      WRITE(1,'(A4)') NOMEFILE(1:4)
      WRITE(1,'(A80/)') COMMENTO
      FINE=.FALSE.
      DO WHILE (.NOT.FINE)
         CALL I3(1,IOB,DEBL)
         WRITE(1,'(A)') '=============================='
         WRITE(6,100)
     $        ' ALTRE CONFIGURAZIONI DA PREISTANZIARE [S/N]?'
         call FLUSH
100      FORMAT(A)
         READ(5,'(A1)') C1
         IF (C1.EQ.'N'.OR.C1.EQ.'n') FINE=.TRUE.
      ENDDO
      WRITE(1,'(A)') 'EOF'
      REWIND(1)
C      DO I=1,5
C         READ(1,'(A)') LINE
C      ENDDO
C guag: test modulo di tipo REGOL.
      DO I=1,4
         READ(1,'(A)') LINE
C         WRITE(68,'(A)') "VORWAHL2 I: "//LINE      
      ENDDO
         READ(1,'(A)') LINE_TEST
      IF( LINE_TEST(26:31).EQ.'REGOL.') IEREGOL=1
C         WRITE(68,'(I4,1X,A)') IEREGOL,"VORWAHL2 eccomi: "//LINE_TEST
      
      READ(1,'(A)') LINE
      NCONF=0
      DO WHILE (LINE(1:3).NE.'EOF')
         NCONF=NCONF+1
         DO WHILE (LINE(1:1).NE.'=')
          IF (LINE(13:14).EQ.'US') NSTATI(NCONF)=NSTATI(NCONF)+1
          IF (LINE(13:14).EQ.'UA') NUSCIT(NCONF)=NUSCIT(NCONF)+1
          IF (LINE(13:14).EQ.'IN') NINGRE(NCONF)=NINGRE(NCONF)+1
          READ(1,'(A)') LINE
         ENDDO
         READ(1,'(A)') LINE
      ENDDO
      NSC=NSTATI(1)
      NUC=NUSCIT(1)
      NIC=NINGRE(1)
      DIFFE=.FALSE.
      DO I=2,NCONF
         IF (NSTATI(I).NE.NSC) THEN
            WRITE(6,200) I  
            DIFFE=.TRUE.
         ENDIF
         IF (NUSCIT(I).NE.NUC) THEN
            WRITE(6,300) I
            DIFFE=.TRUE.
         ENDIF
         IF (NINGRE(I).NE.NIC) THEN
            WRITE(6,400) I
            DIFFE=.TRUE.
         ENDIF
      ENDDO
      IF (DIFFE) STOP
C GUAG:
C Modifica per aumentare il numero di istanze 
C per modulo: massimo 99
C      NOMEEXT = NOMEFILE // ".PI3"
      DO I = 1, 11
         J = ICHAR (NOMEEXT(I:I))
         IF (J .GT. 64 .AND. J .LE. 90) NOMEEXT(I:I) = CHAR (J + 32)
      ENDDO
      OPEN (3, FILE=NOMEEXT, STATUS='UNKNOWN', FORM='FORMATTED')
      REWIND(1)
      
C      DO I=1,4
C         READ(1,'(A)') LINE
C         WRITE(3,'(A)') LINE
C      ENDDO
C Guag - correzione per i moduli di tipo REGOL.: hanno la descrizione che inizia per cancelletto      
      DO I=1,2
         READ(1,'(A)') LINE
         WRITE(3,'(A)') LINE
      ENDDO
         READ(1,'(A)') LINE
      
      IF( IEREGOL.EQ. 1) THEN
         WRITE(3,'(A)') '#'//LINE
      ELSE
         WRITE(3,'(A)') LINE
      ENDIF
            
      READ(1,'(A)') LINE
      WRITE(3,'(A)') LINE
	       
      WRITE(3,'(A7,I3)') 'NSTAT =',NSC
      WRITE(3,'(A7,I3)') 'NUSCI =',NUC
      WRITE(3,'(A7,I3)') 'NINGR =',NIC
      WRITE(3,'(A1)') ' '
      WRITE(3,'(A7,I2)') 'NCONF =',NCONF
      WRITE(3,'(A1)') ' '
      READ(1,'(A)') LINE
      DO WHILE (LINE(1:3).NE.'EOF')
         IF (LINE(11:12).NE.'BL') THEN
            DO I=1,4
               IF (LINE(I:I).EQ.' ') LINE(I:I)='_'
            ENDDO
            WRITE(3,'(A)') LINE
         ENDIF
         READ(1,'(A)') LINE
      ENDDO
      WRITE(3,'(A)') LINE
      CLOSE(UNIT=1)
      CLOSE(UNIT=3)
      OPEN(UNIT=16,FILE='sca1.tmp',STATUS='REPLACE',FORM='FORMATTED')
      WRITE(16,*) NOMEEXT
      call FLUSH
      CLOSE(UNIT=16)
200   FORMAT('NUMERO STATI CONFIG. ',I3,'DIVERSO DA CONFIG. 1')
300   FORMAT('NUMERO USCIT CONFIG. ',I3,'DIVERSO DA CONFIG. 1')
400   FORMAT('NUMERO INGRE CONFIG. ',I3,'DIVERSO DA CONFIG. 1')
      END
