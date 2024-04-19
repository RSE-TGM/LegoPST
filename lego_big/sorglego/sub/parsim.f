C*********************************************************************
C       Fortran Source:             parsim.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:29:30 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_parsim_f
      CHARACTER*80  RepoID
      COMMON /CM_parsim_f / RepoID
      DATA RepoID/'@(#)1,fsrc,parsim.f,2'/
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

      SUBROUTINE PARSIM
      DIMENSION ITITOL(20)
      COMMON/PARINT/ITITOL,TFIN,TSTEP,FATOLL,INTERR,TINTER
C Guag2024      DATA NOT/'NO'/,ISSI/'SI'/
 1000 FORMAT(/10X,'TITOLO DELLA SIMULAZIONE (MAX 80 CAR.)?')
      WRITE(6,1000)
      READ(5,1001)(ITITOL(I),I=1,20)
 1001 FORMAT(20A4)
      WRITE(6,1002)
 1002 FORMAT(/10X,'TEMPO FINALE DI SIMULAZIONE (SEC)?'/)
      READ(5,1003)TFIN
 1003 FORMAT(F10.2)
      TSTEP=1.
      TINTER=1.
      INTERR=0
C
      FATOLL=1.
      WRITE(6,1005)
 1005 FORMAT(/10X,'PASSO DI INTEGRAZIONE (SEC)?')
      READ(5,1003)TSTEPO
      IF(TSTEPO.GT.1.E-5)TSTEP=TSTEPO
C
C      WRITE(6,1006)
C 1006 FORMAT(/10X,'VUOI IMPEDIRE L''AUMENTO AUTOMATICO DEL PASSO',
C     $       /10X,'DI INTEGRAZIONE?')
C      READ(5,1001)IFAT
C 1007 FORMAT(F10.0)
C      IF(IFAT.EQ.ISSI)FATOLL=-1.
      FATOLL=-1.
C
   10 CALL SSWTCH(1,LL)
      IF(LL.NE.1)GO TO 30
      WRITE(6,1010)TFIN,TSTEP
 1010 FORMAT(//10X,'STAMPA OPZIONALE SSWTCH -1 '/
     $ 10X,'TFIN =',F10.2,2X,'TSTEP =',F10.4)
   30 RETURN
      END
C            
