C******************************************************************************
C modulo creatav.pf
C tipo 
C release 1.7
C data 3/26/96
C reserver @(#)creatav.pf	1.7
C******************************************************************************
        PROGRAM CREATAV
        INTEGER LEN
	CHARACTER*2 CNUM(32)
	CHARACTER*80 TAVOLED,FILOUT
	CHARACTER*8 FILINP
	PARAMETER (LUNG=4818,LUNG4=LUNG*4,NREG=32)
        DIMENSION VET(LUNG), IC(NREG)
	DATA FILINP /'ta00.dat'/ 
	DATA CNUM /'01','02','03','04','05','06','07','08','09','10',
     1             '11','12','13','14','15','16','17','18','19','20',
     1   '21','22','23','24','25','26','27','28','29','30','31','32'/
	DATA IC /41,44,48,47,48,2*49,51,52,53,54,57,59,62,66,3*69,71,
     1           73,8*70,2*69,2*68/
C #if defined AIX || ULTRIX || VMS || OSF1 || LINUX
        CALL GETENV('TAVOLE',TAVOLED)
C #endif
C #if defined SCO_UNIX
C         IRES = IGETEN('TAVOLE',TAVOLED)
C #endif
C
        LEN = 1
 1010   CONTINUE
        IF(TAVOLED(LEN:LEN).EQ.' ') THEN
            LEN = LEN -1
            GO TO 1020
         ELSE
            LEN = LEN +1
            GO TO 1010
        ENDIF
 1020   FILOUT = TAVOLED(1:LEN)//'/TAVOLE.DAT'
        write(*,*)filout
C #if defined AIX
C 	OPEN(2,FILE=FILOUT,STATUS='UNKNOWN',ACCESS='DIRECT',
C     1       FORM='UNFORMATTED',RECL=LUNG4)
C #endif
C #if defined ULTRIX || VMS || HELIOS || SCO_UNIX || OSF1 || LINUX
	OPEN(2,FILE=FILOUT,STATUS='UNKNOWN',ACCESS='DIRECT',
     1       FORM='UNFORMATTED',RECL=LUNG4)
C #endif
	DO 20 J=1,NREG
	LREG=IC(J)*6*11 
        WRITE(6,*) 'REGIONE ',J,'  LUNGHEZZA ',LREG
        DO 10 I=1,LUNG
        VET(I)=0.0
 10     CONTINUE
	FILINP(3:4)=CNUM(J)
        write(*,*)filinp
C #if defined ULTRIX || OSF1
C 	OPEN(3,FILE=FILINP,READONLY,STATUS='OLD',FORM='FORMATTED')
C #endif
C #if defined AIX || VMS || HELIOS || SCO_UNIX || LINUX
        OPEN(3,FILE=FILINP,STATUS='OLD',FORM='FORMATTED')
C #endif
	IF(J.EQ.1) THEN
	READ(3,101) (VET(K),K=1,LREG)
 101	FORMAT(6E15.8)
	ELSE
	READ(3,100) (VET(K),K=1,LREG)
 100	FORMAT(E14.8,5E15.8)
	END IF
        WRITE(2,REC=J) VET
        CLOSE(3)
 20     CONTINUE
        STOP
        END
