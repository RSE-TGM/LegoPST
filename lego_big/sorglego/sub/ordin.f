C*********************************************************************
C       Fortran Source:             ordin.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Tue Aug 21 10:29:28 2001 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_ordin_f
      CHARACTER*80  RepoID
      COMMON /CM_ordin_f / RepoID
      DATA RepoID/'@(#)1,fsrc,ordin.f,2'/
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

       SUBROUTINE ORDIN(IV1, IV2, IVV1, IVV2, K)
C
C     ORDINA L'ARRAY IV1 (E DI CONSEGUENZA IV2) IN MODO CRESCENTE.
C     METTE I RISULTATI IN IVV1 E IVV2.
C     UTILIZZA UN ALGORITMO SELECTION SORT MODIFICATO PER GESTIRE
C     GLI ELEMENTI GIA' USATI (MARCATI COME NEGATIVI).
C
C     PARAMETRI:
C       IV1  (INPUT)  : ARRAY CON I VALORI DA ORDINARE. VIENE MODIFICATO.
C       IV2  (INPUT)  : ARRAY DA RIORDINARE INSIEME A IV1.
C       IVV1 (OUTPUT) : IV1 ORDINATO.
C       IVV2 (OUTPUT) : IV2 RIORDINATO.
C       K    (INPUT)  : NUMERO DI ELEMENTI DA ORDINARE.
C
      IMPLICIT NONE
      INTEGER K, I, J, IMIN, MIN_VAL
      INTEGER IV1(*), IV2(*), IVV1(*), IVV2(*)
      LOGICAL FOUND

C     LOOP PRINCIPALE: TROVA IL MINIMO PER OGNI POSIZIONE J DELL'ARRAY
C     DI OUTPUT.
      DO 20 J = 1, K
        
C       INIZIALIZZA LA RICERCA DEL MINIMO PER QUESTO PASSO.
        FOUND = .FALSE.
        IMIN = 0
        MIN_VAL = 0 ! Il valore iniziale non e' importante

C       CERCA IL PROSSIMO ELEMENTO MINIMO NELL'ARRAY IV1
C       TRA QUELLI NON ANCORA SELEZIONATI (VALORE >= 0).
        DO 10 I = 1, K
          IF (IV1(I) .GE. 0) THEN
C           Questo e' un elemento valido da considerare.
            IF (.NOT. FOUND) THEN
C             E' il primo elemento valido che troviamo, quindi e'
C             il nostro minimo temporaneo.
              MIN_VAL = IV1(I)
              IMIN = I
              FOUND = .TRUE.
            ELSE IF (IV1(I) .LT. MIN_VAL) THEN
C             Abbiamo trovato un nuovo minimo.
              MIN_VAL = IV1(I)
              IMIN = I
            END IF
          END IF
   10   CONTINUE

C       CONTROLLO DI SICUREZZA: SE ABBIAMO TROVATO UN MINIMO,
C       LO AGGIUNGIAMO AGLI ARRAY DI OUTPUT.
        IF (FOUND) THEN
          IVV1(J) = MIN_VAL
          IVV2(J) = IV2(IMIN)
          
C         MARCA L'ELEMENTO COME "USATO" PER EVITARE DI SELEZIONARLO
C         DI NUOVO NELLE PROSSIME ITERAZIONI.
          IV1(IMIN) = -1
        ELSE
C         CASO DI ERRORE: non ci sono piu' elementi validi da ordinare,
C         ma il loop principale si aspetta di trovarne ancora. Questo
C         indica un problema logico a monte, ma per sicurezza
C         mettiamo un valore di errore e continuiamo.
          IVV1(J) = -777777
          IVV2(J) = -777777
        END IF

   20 CONTINUE
      
      RETURN
      END




      SUBROUTINE ORDIN_OLD(IV1,IV2,IVV1,IVV2,K)
      DIMENSION IV1(*),IV2(*),IVV1(*),IVV2(*)
      DO 20 J=1,K
      MIN=9999
      DO 10 I=1,K
      IF(IV1(I).LT.0)GO TO 10
      IF(IV1(I).GE.MIN)GO TO 10
      IMIN=I
      MIN=IV1(I)
   10 CONTINUE
      IVV1(J)=MIN
      IV1(IMIN)=-1
      IVV2(J)=IV2(IMIN)
   20 CONTINUE
      RETURN
      END
C            
