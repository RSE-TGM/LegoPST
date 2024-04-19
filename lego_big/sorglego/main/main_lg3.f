C*********************************************************************
C       Fortran PreCompile:             main_lg3.pf
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Thu Mar 24 17:57:57 2005 %
C
C**********************************************************************


C
C Procedura contenete la variabile per identificare la versione
C
      BLOCK DATA BDD_main_lg3_pf
      CHARACTER*80  RepoID
      COMMON /CM_main_lg3_pf / RepoID
      DATA RepoID/'@(#)1,pfsrc,main_lg3.pf,2'/
      END
C**********************************************************************
C******************************************************************************
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C           LEGO unificato per singola / doppia precisione             C
C                 e per diverse piattaforme operative                  C
C                                                                      C
C   Attivata versione singola precisione per sistema operativo Unix    C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C***********************************************************************
       PROGRAM LG3
C
C          PROGRAMMA DI ACQUISIZIONE DATI GEOMETRICI E FISICI
C          E CALCOLO DEL REGIME STAZIONARIO INIZIALE
C
C***********************************************************************
C
C______ FILES DI LAVORO DI LG3
C
      CHARACTER*32 F14DAT,F03DAT,F04DAT,F24DAT,LG3ERR,N04DAT
C
      COMMON/LG3FLS/F14DAT,F03DAT,F04DAT,F24DAT,LG3ERR,N04DAT
      COMMON/LGTV01/LGTEMPO,ILEGO,ICOUNT,ICOUNTMX
      COMMON/LGTV02/LGMODULO,LGBLOCCO
C
C     Permette la visualizzazione di molti errori sulle tavole
C     perche' la prima fase e' quella di lettura parametri
C
      ILEGO=1   	
      ICOUNTMX=1000
      CALL LG3FIL
C
C     APERTURA FILE TAVOLE VAPORE
C
      CALL INITSM
C Inizio parte modificata da Furlani
      OPEN(UNIT=3,FILE=F03DAT,STATUS='OLD',FORM='UNFORMATTED')
      OPEN(UNIT=14,FILE=F14DAT,STATUS='OLD',FORM='FORMATTED')
      OPEN(UNIT=4,FILE=F04DAT,STATUS='UNKNOWN',FORM='UNFORMATTED')
      OPEN(UNIT=44,FILE=N04DAT,STATUS='UNKNOWN',FORM='FORMATTED')
C      OPEN(UNIT=24,FILE=F24DAT,STATUS='UNKNOWN')
      IER=0
C
C_________ LETTURA DEI DATI TOPOLOGIGICI,GEOMETRICI E FISICI DEL MODELLO
C
      CALL LEGO3(IER)
      IF (IER.EQ.1) CALL LGABRT
C
C_________ CALCOLO DEL REGIME STAZIONARIO
C
      CALL LEGO34
C
      CALL LGSTOP
      STOP
      END
C
C
C
      BLOCK DATA MA2801
      LOGICAL ABORT1,ABORT2,ABORT3
      COMMON /MA30E/ LP,ABORT1,ABORT2,ABORT3
      COMMON /MA30G/ EPS,RMIN
      DATA EPS/1.0E-4/
      DATA LP/6/
      DATA ABORT1/.FALSE./,ABORT2/.FALSE./,ABORT3/.FALSE./
      END
C
C
C
      BLOCK DATA MA2802
      COMMON/MC19B /LP,IFAIL
      DATA LP/6/
      END
C
C
C
      BLOCK DATA MA2803
      LOGICAL LBLOCK,GROW,ABORT1,ABORT2
      COMMON /MA28E/ LP,MP,LBLOCK,GROW
      COMMON /MA28F/ EPS,RMIN,RESID,IRNCP,ICNCP,MINIRN,MINICN,
     $               IRANK,ABORT1,ABORT2
      DATA EPS/1.0E-4/
      DATA LP/6/,MP/6/
      DATA LBLOCK/.TRUE./,GROW/.TRUE./
      DATA ABORT1/.FALSE./,ABORT2/.FALSE./
      END
C
C
C
      BLOCK DATA MA2804
      LOGICAL ABORT
      COMMON /MC23B/ LP,NUMNZ,NUM,LARGE,ABORT
      DATA LP/6/,ABORT/.FALSE./
      END
C            
