      PROGRAM VORWAHL1
C   VORWAHL1 per Linux
      LOGICAL FOUND,ENDE,BLANK,ENDEINT
      CHARACTER*4 NOME
      CHARACTER*8 NOMEFOR
      CHARACTER*80 LINE
      CHARACTER*80 COMMENTO
      INTEGER N
      OPEN(UNIT=12,FILE='sca.tmp',STATUS='OLD',FORM='FORMATTED')
      READ(12,'(A4)') NOME
      PRINT*,'NOME=',NOME

      READ(12,'(A)') NOMEFOR
      PRINT*,'NOMEFOR=',NOMEFOR

      READ(12,*) N
      PRINT*,'N=',N

      READ(12,'(A80)') COMMENTO
      PRINT*,'COMMENTO=',COMMENTO
      CLOSE(UNIT=12)
      DO I=1,4
        IF(ICHAR(NOME(I:I)).GE.ICHAR('a').AND.
     $     ICHAR(NOME(I:I)).LE.ICHAR('z'))
     $     NOME(I:I)=CHAR(ICHAR(NOME(I:I))+ICHAR('A')-ICHAR('a'))
      ENDDO
C      NOMEFOR=NOME//'.FOR'
      OPEN(UNIT=1,FILE='F',
     $     STATUS='UNKNOWN',FORM='FORMATTED')
      OPEN(UNIT=2,FILE=NOMEFOR,
     $     STATUS='OLD',FORM='FORMATTED')
      OPEN(UNIT=3,FILE='i3i4.for',
     $     STATUS='UNKNOWN',FORM='FORMATTED')
      NCOUNT=0
      FOUND=.FALSE.
10    CONTINUE
         READ(2,'(A)') LINE
         DO I=7,80
            IF (LINE(I:I).NE.' ') GOTO 20
         ENDDO
20       IF (LINE(I:I+9).EQ.'SUBROUTINE') THEN
             DO J=I+10,78
                IF (LINE(J:J).NE.' ') GOTO 30
             ENDDO
30           IF( ( LINE(J:J+6).EQ.(NOME//'I3(')).OR.
     $           ( LINE(J:J+6).EQ.(NOME//'I3 ')).OR.
     $           ( LINE(J:J+6).EQ.(NOME//'I4(')).OR.
     $           ( LINE(J:J+6).EQ.(NOME//'I4 ')) ) THEN
                 FOUND=.TRUE.
                 GOTO 40
             ENDIF
         ENDIF
40       IF (FOUND) THEN
            NCOUNT=NCOUNT+1
            IF (LINE(J+4:J+5).EQ.'I3') THEN
               WRITE(3,'(A,1X,A)') LINE(1:I+9),LINE(J+4:80)
            ELSE
               WRITE(3,'(A)') LINE
            ENDIF
            ENDE=.FALSE.
            DO WHILE (.NOT.ENDE)
               READ(2,'(A)') LINE
               WRITE(3,'(A)') LINE
	       IF ((Index(LINE,'WRITE').ne.0).or.
     $             (Index(LINE,'write').ne.0)) THEN
                     ENDEINT=.false.
                     DO WHILE (.NOT.ENDEINT)
		      READ(2,'(A)') LINE
		      IF (LINE(6:6).eq.' ') THEN 
                        WRITE(3,'(A)') '        call FLUSH'
			ENDEINT=.true.
                      ENDIF
		      WRITE(3,'(A)') LINE
		     ENDDO 
	       ENDIF         
               DO K=7,78
                  IF (LINE(K:K+3).EQ.'END') THEN
                     BLANK=.TRUE.
                     DO L=K+4,80  !enddo, endif
                        IF (LINE(L:L).NE.' ') BLANK=.FALSE.
                     ENDDO
                     IF (BLANK) THEN
                        ENDE=.TRUE.
                        FOUND=.FALSE.
                     ENDIF
                  ENDIF
               ENDDO
            ENDDO
         ENDIF
      IF (NCOUNT.LT.2) GOTO 10
      CLOSE(UNIT=2)
      CLOSE(UNIT=3)
      WRITE(1,'(A4)') NOME
      WRITE(1,'(I2)') N
      WRITE(1,'(A)') COMMENTO            
      CLOSE(UNIT=1)
      END
