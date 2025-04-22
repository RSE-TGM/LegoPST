

C*********************************************************************
C Fortran PreCompile: crltm.pf
C Subsystem: 1
C Description:
C %created_by: lomgr %
C %date_created: Wed May 12 16:13:03 2004 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l identificazione della versione
C
      BLOCK DATA BDD_crltm_pf
      CHARACTER*80 RepoID
      COMMON /CM_crltm_pf / RepoID
      DATA RepoID/'@(#)1,pfsrc,crltm.pf,2'/
      END
C**********************************************************************
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C C
C LEGO unificato per singola / doppia precisione C
C e per diverse piattaforme operative C
C C
C Attivata versione singola precisione per sistema operativo Unix C
C C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      SUBROUTINE CRLTM(NST,NOSL,LIMODU,ISL,NBL,NSTN,NOSLN,LISLTM,ISLN
     $ ,ITIMOD)
      DIMENSION NOSL(*),ISL(*),NOSLN(*),ISLN(*)
      CHARACTER*(*) LIMODU(*)
      CHARACTER*(*) LISLTM(*)
      CHARACTER*70 LINE
C
C CREAZIONE DELLE SUBROUTINES MODI2,MODC1 E MODC2 IN LTM.FOR
C PER IL COLLEGAMENTO LG3/LG5-MODULI
C
C CREAZIONE ELENCO MODULI UTILIZZATI NEL MODELLO
C (CREALTM.DAT)
C
      IOUT1=35
C file LTM.FOR
      IOUT2=34
C file CREALTM.DAT
C
C________________SELEZIONE DELL ELENCO
C DEI MODULI USATI NEL MODELLO
C
      K=1
      J=ISL(K)
      NOSLN(K)=NOSL(J)
      LISLTM(K)=LIMODU(J)
      ISLN(K)=K
C
      DO 10 I=2,NBL
      J=ISL(I)
      MODU=NOSL(J)
      LINE=LIMODU(J)
C
      DO 15 L=1,K
      IF(NOSLN(L).EQ.MODU)GO TO 11
   15 CONTINUE
      K=K+1
      NOSLN(K)=MODU
      LISLTM(K)=LINE
      ISLN(I)=K
      GO TO 10
C
   11 ISLN(I)=L
   10 CONTINUE
      NSTN=K
C
C__________SCRITTURA DEL FILE CREALTM.DAT
C
      REWIND IOUT2

      IF(ITIMOD.EQ.0) THEN
      WRITE(IOUT2,2)'****       MODELLO CON SISTEMA DI EQUAZIONI '
      ELSE
      WRITE(IOUT2,2)'****       MODELLO SENZA SISTEMA DI EQUAZIONI '
      ENDIF

      DO 700 I =1,NSTN
      WRITE(IOUT2,2)LISLTM(I)
  700 CONTINUE
C
C___________SCRITTURA DEL FILE LTM.FOR
C
C CREAZIONE SUBROUTINE MODI2
C
C
      REWIND IOUT1
C
      WRITE(IOUT1,2)'      SUBROUTINE MODI2('
      WRITE(IOUT1,2)
     $'     $        KI,IFUN,IBLOC1,IBLOC2,VAR,NX5,I1,I2,XYU,DATI,'
      WRITE(IOUT1,2)
     $'     $        K1,K2,IER,CNXYU,TOL)'
      WRITE(IOUT1,2)'      DIMENSION XYU(*),DATI(*),CNXYU(*),TOL(*)'

      WRITE(IOUT1,2)'      INTEGER VAR (NX5,2)'
      WRITE(IOUT1,2)'      MX1=NX5'
      WRITE(IOUT1,2)'C****'
      KST=0
      IVIA= 2
      KEN=500
      CALL GOTOCA(NSTN,NOSLN,IVIA,KST,KEN,IOUT1)
      WRITE(IOUT1,2)'C****'
      WRITE(IOUT1,2)'  500 RETURN'
      WRITE(IOUT1,2)'      END'
C
C
C CREAZIONE SUBROUTINE MODC1
C
C
      WRITE(IOUT1,2)'      SUBROUTINE MODC1('
      WRITE(IOUT1,2)
     $'     $        KI,IFUN,AJAC,MX5,IXYU,XYU,IPD,DATI,RNI,'
      WRITE(IOUT1,2)
     $'     $        NOBL1,NOBL2)'
      WRITE(IOUT1,2)
     $'      DIMENSION  AJAC(MX5,MX5),XYU(*),DATI(*),RNI(*)'
      WRITE(IOUT1,2)'      MXX=MX5'
      WRITE(IOUT1,2)'C****'
      KST=0
      IVIA= 3
      KEN=500
      CALL GOTOCA(NSTN,NOSLN,IVIA,KST,KEN,IOUT1)
      WRITE(IOUT1,2)'C****'
      WRITE(IOUT1,2)'  500 RETURN'
      WRITE(IOUT1,2)'      END'

C
C CREAZIONE SUBROUTINE MODD1
C
      WRITE(IOUT1,2)'      SUBROUTINE MODD1('
      WRITE(IOUT1,2)
     $'     $        KI,BLOCCO,NEQUAZ,NSTATI,NUSCIT,'
      WRITE(IOUT1,2)
     $'     $        NINGRE,SYMVAR,XYU,'
      WRITE(IOUT1,2)
     $'     $        IXYU,DATI,IPD,SIGNEQ,UNITEQ,'
      WRITE(IOUT1,2)
     $'     $        COSNOR,ITOPVA,MXT)'
      WRITE(IOUT1,2)'C'
      WRITE(IOUT1,2)'      CHARACTER*(*) BLOCCO'
      WRITE(IOUT1,2)'      CHARACTER*(*) SYMVAR(*)'
      WRITE(IOUT1,2)'      CHARACTER*(*) SIGNEQ(*)'
      WRITE(IOUT1,2)'      CHARACTER*(*) UNITEQ(*)'
      WRITE(IOUT1,2)'      DIMENSION ITOPVA(MXT,*)'
      WRITE(IOUT1,2)'      DIMENSION COSNOR(*)'
      WRITE(IOUT1,2)'      DIMENSION DATI(*)'
      WRITE(IOUT1,2)'      DIMENSION XYU(*)'
      WRITE(IOUT1,2)'C'
      KST=0
      IVIA= 5
      KEN=500
      IF(ITIMOD.EQ.0) THEN
        CALL GOTOCA(NSTN,NOSLN,IVIA,KST,KEN,IOUT1)
      ENDIF
      WRITE(IOUT1,2)'C****'
      WRITE(IOUT1,2)'  500 RETURN'
      WRITE(IOUT1,2)'      END'
C

      RETURN
    2 FORMAT (A)
      END
C
