C*********************************************************************
C       Fortran Source:             diagn.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Wed Mar 23 10:49:45 2005 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_diagn_f
      CHARACTER*80  RepoID
      COMMON /CM_diagn_f / RepoID
      DATA RepoID/'@(#)1,fsrc,diagn.f,2'/
      END
C**********************************************************************
      SUBROUTINE DIAGN(N,SH,P)
C**********************************************************************
C     MODIFICHE ALLA VERSIONE ORIGINALE PER EVITARE LO STOP
C     QUANDO SI VA FUORI DAI LIMITI DELLE TAVOLE .
C**********************************************************************
C
C  STAMPA P,H O S E LA TRACCIA DELLE CHIAMATE
C
C       ILEGO = INDICATORE DEL PROGRAMMA CHIAMANTE LE TAVOLE
C             =2 SID E` IL PROGRAMMA CHIAMANTE
C             =1 LEGO E` IL PROGRAMMA CHIAMANTE
C             =0 NON E` NE` SID NE` LEGO IL PROGRAMMA CHIAMANTE
C
C       ICOUNTMX = CONTATORE STABILITO DAL PROGRAMMA CHIAMANTE LE TAVOLE
C                  SUPERATO IL QUALE VIENE DECRETATO LO STOP .
C


C     LA COMMON IF_CALLED_BY_SID SERVE AD IMPLEMENTARE LA DIAGNOSTICA
C     NEL CASO CHE IL PROGRAMMA CHIAMANTE SIA IL SID . IN QUEL CASO LA
C     VARIABILE ERROR_CODE VIENE SETTATA UGUALE AL PARAMETRO N DELLA
C     SUBROUTINE DIAGN() .

      COMMON/IF_CALLED_BY_SID/ERROR_CODE
      INTEGER ERROR_CODE       


      COMMON/LGTV01/LGTEMPO,ILEGO,ICOUNT,ICOUNTMX
      COMMON/LGTV02/LGMODULO,LGBLOCCO
      CHARACTER*8 LGMODULO,LGBLOCCO
      REAL LGTEMPO
C
C -- GUAG 21-10-91 
      CHARACTER*60 PATH
      CHARACTER*20 FILE06
      INTEGER ERRORE
C
      LOGICAL IPRIMA
      DATA IPRIMA/.TRUE./

C  GUAG 09-07-2026
      DATA PATH/' '/
      DATA FILE06/' '/
      DATA ERRORE/0/
      DATA ERROR_CODE/0/

C     GESTIONE DIAGNOSTICA SID

      IF(ILEGO.EQ.2) THEN
         ERROR_CODE = N
         RETURN
      ENDIF

C     FINE GESTIONE DIAGNOSTICA SID

      if(IPRIMA) THEN 
C      ERRORE = diagn_io()
      FILE06='tavole.out'
C      OPEN(UNIT=06,FILE=FILE06,STATUS='UNKNOWN')
       WRITE(6,*)'WORK_DIR= ',PATH
       WRITE(6,*)'FILE06= ', FILE06
       WRITE(6,*)'ICOUNTMX= ', ICOUNTMX
       WRITE(6,*)'ILEGO= ', ILEGO
      IPRIMA=.FALSE.
      ENDIF
c

      WRITE(6,*)' ****************************************************'
      IF(ILEGO.EQ.1) THEN
        WRITE(6,*)' TAVOLE ACQUA/VAPORE FUORI LIMITI. TEMPO ',LGTEMPO 
        WRITE(6,*)' NEL BLOCCO ',LGBLOCCO,' ( MODULO ',LGMODULO,' )'
      ELSE IF(ILEGO.EQ.0) THEN
          WRITE(6,*)' TAVOLE ACQUA/VAPORE FUORI LIMITI'
      ENDIF
C
      PD=P*1.E-5
      GO TO (1,1,1,1,2,3) , N
    1 WRITE(6,101) PD,SH
      GO TO 4
    2 WRITE(6,102) PD
      WRITE(6,*) '        P > P CRITICA'
      GO TO 4
C
C
C      STOP
C
    3 HD=SH*1.E-3
      WRITE(6,103) PD,HD
C      
 4    CONTINUE
C
C_______ FORNISCE  LA TRACCIA DELLE CHIAMATE IN ORDINE INVERSO
C
C      IF(ILEGO.EQ.1) CALL LIB$SIGNAL(%VAL(0))
      ICOUNT=ICOUNT+1
      IF(ICOUNT.GT.ICOUNTMX) STOP
      RETURN
  101 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,2HS=,E12.5,7H J/KG/K,5X,6(1H*))
  102 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,34HNON ESISTONO VALORI DI SATURAZIONE,5X,3(1H*))
  103 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,2HH=,E12.5,6H KJ/KG,5X,6(1H*))
      END
