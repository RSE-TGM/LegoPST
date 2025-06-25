C******************************************************************************
C modulo gen_sche.pf
C tipo
C release 5.2
C data 12/4/95
C reserver @(#)gen_sche.pf	5.2
C******************************************************************************
      PROGRAM GEN_SCHEMA
C
C
C
C               PROGRAMMA   GEN_SCHEMA  (VERSIONE UNIX)
C
C
C     INGRESSI    :   1)  Nome dello schema di regolazione
C                     2)  Path del diretttorio di lavoro
C
C     USCITE      :   1)  File pag.for
C                     2)  File pag_01.dat
C                     3)  File pag.dat
C
C     FILE DI USO :   1)  pag.top
C
C
C
C**   Descrizione delle parameter di GEN_SCHEMA
C**
C**   N000= MOLTIPLICATORE
C**
C**   N001= N. MODULI
C**   N002= N. BLOCCHI
C**   N003= N.STATI+ALG. = ORDINE MASSIMO SISTEMA ALGEBRICO
C**   N004= N. INGRESSI
C**   N005= N. VARIABILI
C**   N006= N. VAR. DI 1 BLOCCO
C**   N007= N. DI DATI
C**   N008= N. DI EQUAZIONI DI UN BLOCCO
C**   NR00= N. DI TERMINI #0 PER OGNI EQUAZIONE
C**   NP00= N. DI PERTURBAZIONI
C
C

C QUESTI PARAMETRI VENGONO MODIFICATI PER ESIGENZE
C RELATIVE A SCHEMI DA COMPILARE (PARTICOLARMENTE
C N008)

C      PARAMETER  (N000=10,
C     &           N001=N000*10,
C     &           N002=N000*25,
C     &           N003=N000*50,
C     &           N004=N000*25,
C     &           N005=N000*100,
C     &           N006=200,
C     &           N007=N000*500,
C     &           N008=200)

         PARAMETER  (N000=160,
     &              N001=N000*10,
     &              N002=N000*25,
     &              N003=N000*50,
     &              N004=N000*25,
     &              N005=N000*100,
     &              N006=1000,
     &              N007=N000*500,
     &              N008=1000)
C
C
C
C
C
         parameter (MN005=2*N005)
         parameter (MAXCAR=70)
C
C
         DIMENSION IORD(N002),ILIV(N002),IPORD(N002)
         DIMENSION ITUSC(N003),ITING(N004)
         CHARACTER*4 VARING(N006),VARUSC(N008)
         CHARACTER*80 FILREG(N005)
         CHARACTER*80 FORREG(MN005)
C
         INTEGER      IPUNB(N002)
         CHARACTER*80 FIL01(MN005)
C
C     SIMBOLI : SIMB_REG  = Simbolo dello schema di regolazione,
C                           di 4 caratteri  e  maiuscolo  viene
C                           inserito nei file prodotti.
C               SIMB_PAG  = Simbolo della pagina di regolazione,
C                           di al piu 4 caratteri serve per la
C                           costruzione del nome del file pag.top.
C               SIMB_FILE = Simbolo dello schema di regolazione,
C                           di 4 caratteri e maiuscolo serve per
C                           la costruzione dei nomi dei files di
C                           output del programma.
C		TAG_REG   = Letto dal file schema.top rappresenta
C			    il progressivo dello schema (2
C			    caratteri es. 01) e l identificatore
C			    della task (2 caratteri es. AA)
         COMMON/NOM_REG/SIMB_REG,DESC_REG,TAG_REG
         CHARACTER*4 SIMB_REG
         CHARACTER*50 DESC_REG
         CHARACTER*4 TAG_REG
         CHARACTER*4 SIMB_PAG
         CHARACTER*4 SIMB_FILE
C
         CHARACTER*180 PATH_U_CC
         CHARACTER*180 PATH
C
C______ FILES DI LAVORO DI GEN_SCHEMA
         CHARACTER*180 FILETOP
C
C______ FILES DI USCITA DI GEN_SCHEMA
         CHARACTER*180 F01_DAT
         CHARACTER*180 NOM_FOR,NOM_DAT
C
         CHARACTER*180 LINE

         INTEGER*4 NUM_ARG
         INTEGER*4 L_P,L_SF,L_SP

C
C

         NUM_ARG = IARGC()
         IF (NUM_ARG.NE.2) THEN
            write(*,*) 'ATTENZIONE, passare i seguenti argomenti :'
            write(*,*) '-1- Simb_reg'
            write(*,*) '-2- Path (direttorio di lavoro)'
            STOP 2
         ENDIF

         CALL GETARG(1,SIMB_PAG)
         CALL GETARG(2,LINE)

C        write(*,*) Parametri:
C        write(*,*) SIMB_PAG
C        write(*,*) LINE

C       Non viene messo minuscolo in quanto non indispensabile,
C       PATH_U_CC viene direttamente posto uguale a LINE
         PATH_U_CC = LINE
CC        CALL CONV_LOWER(LINE,180,PATH_U_CC)
         CALL CONV_LOWER(SIMB_PAG,4,SIMB_FILE)


C  PATH viene posto uguale a PATH_U_CC in quanto non solo il file
C  SIMB_REG.f viene collocato nella directory ./proc  ma anche gli altri
C  due file prodotti : C  SIMB_REG_01.dat SIMB_REG.dat
         PATH = PATH_U_CC


C       Si riporta maiuscolo il nome da inserire nei file prodotti
         DO 555 I = 1, 4
            IF ((ICHAR (SIMB_PAG(I:I)) .GE. 97) .AND.
     $        (ICHAR (SIMB_PAG(I:I)) .LE. 122)) THEN
               SIMB_REG(I:I) = CHAR (ICHAR (SIMB_PAG(I:I)) - 32)
            ELSE
               SIMB_REG(I:I) = SIMB_PAG(I:I)
            ENDIF
  555    CONTINUE
C
C       Con simbolo piu corto di 4 caratteri si aggiungono degli"_"
         DO 556 I=4,1,-1
            IF (ICHAR(SIMB_REG(I:I)) .EQ. 32) THEN
               SIMB_REG(I:I) = CHAR (95)
            ENDIF
  556    CONTINUE



C
C Costruzione dei nomi dei File
C
         L_UCC = LUNGHEZZA_P(PATH_U_CC)
         L_P = LUNGHEZZA_P(PATH)
         L_SP = LUNGHEZZA_S(SIMB_PAG)
         L_SF = LUNGHEZZA_S(SIMB_FILE)

         FILETOP = PATH_U_CC(1:L_UCC)//'/'//SIMB_PAG(1:L_SP)//'.top'
         F01_DAT=PATH_U_CC(1:L_UCC)//'/'//SIMB_FILE(1:L_SF)//'_01.dat'
         NOM_DAT  = PATH_U_CC(1:L_UCC)//'/'//SIMB_FILE(1:L_SF)//'.dat'
         NOM_FOR  = PATH(1:L_P)//'/'//SIMB_FILE(1:L_SF)//'.f'


C       write(*,*)  SIMB_PAG,L_SP,SIMB_FILE,L_SF,SIMB_REG,L_UCC,L_P
C       write(*,*) 'File .top aperto (FILETOP)=',FILETOP
C       write(*,*) 'File _01.dat prodotto (F01_DAT)=',F01_DAT
C       write(*,*) 'File .f prodotto (NOM_FOR)=',NOM_FOR
C       write(*,*) 'File .dat prodotto (NOM_DAT)=',NOM_DAT


         OPEN(UNIT=1,FILE=FILETOP,STATUS='OLD')
C
         OPEN(UNIT=2,FILE=F01_DAT,STATUS='UNKNOWN')
         REWIND(2)
C
C
C
C_____ LETTURA DEI DATI REGISTRATI SUL FILE PRODOTTO DA EASE+
C      CARICAMENTO IN MEMORIA FIL01()
C      E SCRITTURA DEL FILE TOPOLOGICO "_01.DAT"
C
         CALL TRAFO_LET(1,2,FIL01,N_L_FIL01,IPUNB,NBL,IPUN_ING)
C
         CLOSE (1)
         ClOSE (2)
C
C
C             L ORDINE DEI BOCCHI VIENE MEMORIZZATO IN IORD( )
C************ MEMORIZZAZIONE IN FILREG( ) DELLA TOPOLOGIA COMPATTA
C             DEL REGOLATORE
C
         CALL  TRA_ORD(NBL,IPUNB,FIL01,IPUN_ING,VARING,
     &     VARUSC,IORD,FILREG,ITUSC,ITING,NUSC,NING,KLIN)
C
C_______ CREAZIONE DEL SORGENTE DELLA SUBROUTINE I2
C
         CALL RG1_TRAD_I2(NBL,IPUNB,FIL01,IPUN_ING,
     &     IORD,ILIV,FILREG,FORREG,LI,IPORD,VARUSC,VARING,ITUSC,
     &     ITING,NUSC,NING)
C
C_______ CREAZIONE DEL SORGENTE DELLA SUBROUTINE C1
C
         CALL RG1_TRAD_C1(NBL,IPUNB,FIL01,IPUN_ING,
     &     IORD,ILIV,FILREG,FORREG,LI,IPORD,VARUSC,VARING,ITUSC,
     &     ITING,NUSC,NING)
C
  221    CONTINUE
C
C
C
C_______ SCRITTURA DEL FORTRAN DEL REGOLATORE
C	 NEL DIRETTORIO PATH_UTENTE
C
         OPEN(UNIT=12,FILE=NOM_FOR,STATUS='UNKNOWN')
         REWIND(12)
C
         DO I=1,LI
            WRITE(12,'(A)')FORREG(I)
         ENDDO
         CLOSE(12)
C
C
C_______ SCRITTURA DELLA TOPOLOGIA DEL REGOLATORE
C
         OPEN(UNIT=12,FILE=NOM_DAT,STATUS='UNKNOWN')
         REWIND(12)
C
         DO I=1,KLIN
            WRITE(12,'(A)')FILREG(I)
         ENDDO
         CLOSE(12)
C
         STOP 0
      END
C
C
C-----------------------------------------------------------------------
C
      SUBROUTINE TRAFO_LET(INPUT,IOUTPUT,FIL01,N_L_FIL01,IPUNB,NBL,
     &                       IPUN_ING)
C
C
C     LETTURA DEI DATI REGISTRATI SUL FILE TOPOLOGIA SCHEMA FATTO DA EASE+
C     SCRITTURA DEL FILE "_01.DAT" DI LEGOCAD
C     E CARICAMENTO IN MEMORIA (FIL01())
C
C
C
         COMMON/NOM_REG/SIMB_REG,DESC_REG,TAG_REG
         CHARACTER*4 SIMB_REG
         CHARACTER*50 DESC_REG
         CHARACTER*4 TAG_REG
         CHARACTER*(*) FIL01(*)
         INTEGER      IPUNB(*)
         CHARACTER*4 ING
         CHARACTER*12 IMAG1,IMAG2
         CHARACTER*4 ILAB
         CHARACTER*80 LINE
         INTEGER*4 ICOPY/0/
         CHARACTER*40 DESCR
C
         DATA ING/'--IN'/
         DATA IMAG1/'>>>>>>INGRES'/
         DATA IMAG2/'>>>>>>ELENCO'/
         DATA ILAB/'BL.-'/
C
         ICLIN=0
         NBL=0
         REWIND INPUT
         REWIND IOUTPUT
  519    READ(INPUT,'(A)',END=521)LINE
         IF(LINE(11:22).EQ.IMAG2)GO TO 521
         if(ICOPY.NE.0) THEN
            if(LINE(18:18).NE.'#') LINE(18:) = DESCR
            ICOPY = ICOPY - 1
         endif
         WRITE(IOUTPUT,'(A)')LINE
C
         if(LINE(26:36).EQ.'MODREG DIRA'.or.
     &      LINE(26:36).EQ.'MODREG DIRL') then
            ICOPY = 2
            DESCR = LINE(40:)
         endif
C
C_____ LETTURA DELLA TAG_REG E DELLA DESCRIZIONE DELLO SCHEMA
C
         IF(LINE(1:4).EQ.SIMB_REG) THEN
            TAG_REG=LINE(5:8)
            DESC_REG=LINE(11:60)
         ENDIF
C
         ICLIN=ICLIN+1
C
C----PUNTATORE AI DATI DEI BLOCCHI CONTENUTI IN FIL01()
C
         IF(LINE(11:14).EQ.ILAB)THEN
            NBL=NBL+1
            IPUNB(NBL)=ICLIN
         ENDIF
C
C----PUNTATORE AI DATI DEGLI INGRESSI DEI BLOCCHI CONTENUTI IN FIL01()
C
         IF(LINE(11:22).EQ.IMAG1)IPUN_ING=ICLIN
C
         FIL01(ICLIN)=LINE
         GO TO 519
C
  521    N_L_FIL01=ICLIN
         IPUNB(NBL+1)=IPUN_ING+1
         RETURN
      END
      SUBROUTINE TRA_ORD(NBL,IPUNB,FIL01,IPUN_ING,VARING,
     &  VARUSC,IORD,FILREG,ITUSC,ITING,NUSC,NING,KLIN)
C
         DIMENSION IPUNB(*),IORD(*),ITUSC(*),ITING(*)
         CHARACTER*(*) FIL01(*)
         CHARACTER*(*) FILREG(*)
         CHARACTER*4 VARING(*),VARUSC(*)
         COMMON/NOM_REG/SIMB_REG,DESC_REG,TAG_REG
         CHARACTER*4 SIMB_REG
         CHARACTER*50 DESC_REG
         CHARACTER*4 TAG_REG
         CHARACTER*80 SIGNIF
C
C
         DO JJ=1,NBL
            IORD(JJ)=JJ
         ENDDO
C
C
C___ COSTRUZIONE DEL FILE FILREG( )
C    CHE DESCRIVE LE VARIABILI DI INGRESSO E DI USCITA DEL REGOLATORE
C
C
C____ SELEZIONE DELLE USCITE E DEGLI INGRESSI DEL REGOLATORE
C     DEFINIZIONE SE SONO REAL O LOGICAL
C
         KLIN=1
         FILREG(KLIN)='****'
         FILREG(KLIN+1)=SIMB_REG//'      BLOCCO ('//SIMB_REG//
     &     ') - '//DESC_REG
         FILREG(KLIN+2)='****'
         FILREG(KLIN+3)=SIMB_REG//TAG_REG//'  BL.-'//SIMB_REG//
     &     '- **** REGOL. '//SIMB_REG//' - '// DESC_REG
C
         KLIN=KLIN+3
C
         KU=0
         DO 350 I=1,NBL
C
            II=IPUNB(I)+1
            IK=IPUNB(I+1)-1
            DO JJ=II,IK
               IF(FIL01(JJ)(13:13).EQ.'U') THEN
C___ USCITE
                  KLIN=KLIN+1
                  FILREG(KLIN)=FIL01(JJ)
C_____ INCLUDO NELLA DESCRIZIONE DELLA VARIABILE DI USCITA IL NOME DEL
C      BLOCCO DA CUI PROVIENE
C
                  SIGNIF(1:56)=FILREG(KLIN)(18:73)
                  FILREG(KLIN)(18:24)=FIL01(II-1)(1:6)//' '
                  FILREG(KLIN)(25:80)=SIGNIF
C
                  KU=KU+1
                  VARUSC(KU)=FIL01(JJ)(1:4)
                  ITUSC(KU)=0
                  IF(VARUSC(KU)(1:1).EQ.'J' .OR.
     $            VARUSC(KU)(1:1).EQ.'V' .OR.
     $            VARUSC(KU)(1:1).EQ.'L' .OR.
     $            VARUSC(KU)(1:1).EQ.'M' .OR.
     $            VARUSC(KU)(1:1).EQ.'N')ITUSC(KU)=1.
               ENDIF
C
            ENDDO
C
  350    CONTINUE
C
C_____ TRATTAMENTO DEGLI INGRESSI
C
         KI=0
         DO 360 I=1,NBL
C
            II=IPUNB(I)+1
            IK=IPUNB(I+1)-1
            DO JJ=II,IK
C
               IF(FIL01(JJ)(13:13).EQ.'I'.
     $            AND.FIL01(JJ)(18:18).NE.'#') THEN
C___ INGRESSI NON CONNESSI
                  KLIN=KLIN+1
                  FILREG(KLIN)=FIL01(JJ)
C_____ INCLUDO NELLA DESCRIZIONE DELLA VARIABILE DI USCITA IL NOME DEL
C      BLOCCO DA CUI PROVIENE
C
                  SIGNIF(1:56)=FILREG(KLIN)(18:73)
                  FILREG(KLIN)(18:24)=FIL01(II-1)(1:6)//' '
                  FILREG(KLIN)(25:80)=SIGNIF
C
                  KI=KI+1
                  VARING(KI)=FIL01(JJ)(1:4)
                  ITING(KI)=0
                  IF(VARING(KI)(1:1).EQ.'J' .OR.
     $            VARING(KI)(1:1).EQ.'L' .OR.
     $            VARING(KI)(1:1).EQ.'M' .OR.
     $            VARING(KI)(1:1).EQ.'N')ITING(KI)=1.
               ENDIF
            ENDDO
C
  360    CONTINUE
C
         KLIN=KLIN+1
         FILREG(KLIN)='****'
C
         NUSC=KU
         NING=KI
         RETURN
      END
      SUBROUTINE RG1_TRAD_C1(NBL,IPUNB,FIL01,IPUN_ING,
     &  IORD,ILIV,FILREG,FORREG,LI,IPORD,VARUSC,VARING,ITUSC,
     &  ITING,NUSC,NING)
C
         COMMON/NOM_REG/SIMB_REG,DESC_REG,TAG_REG
         CHARACTER*4 SIMB_REG
         CHARACTER*50 DESC_REG
         CHARACTER*4 TAG_REG
         CHARACTER*50 BUFF
         DIMENSION IPUNB(*),IORD(*),ILIV(*),IPORD(*)
         DIMENSION ITUSC(*),ITING(*)
         CHARACTER*(*) FIL01(*)
         CHARACTER*(*) FILREG(*)
         CHARACTER*(*) FORREG(*)
         CHARACTER*(*) VARUSC(*)
         CHARACTER*(*) VARING(*)
         CHARACTER*3 ICAR
C
         DO I=1,NBL
            DO 1 J=1,NBL
               IF(IORD(J).EQ.I)GO TO 2
    1       CONTINUE
    2       IPORD(I)=J
         ENDDO
C
         LI=LI+1
         FORREG(LI)='      SUBROUTINE '//SIMB_REG//'C1'//
     &   '(IFUN,AJAC,MX5,IXYU,XYU,IPD,DATI,RNI,IBL1,IBL2)'
C
         LI=LI+1
         FORREG(LI)='      '//
     &     'DIMENSION AJAC(MX5,*),XYU(*),DATI(*),RNI(*)'
         LI=LI+1
         FORREG(LI)='      '//
     &   'LOGICAL KREGIM'
         LI=LI+1
         FORREG(LI)='      '//
     &   'COMMON/REGIME/KREGIM'
         LI=LI+1
         FORREG(LI)='      '//
     &   'COMMON/INTEGR/TSTOP,TEMPO,DTINT,NPAS,CDT'
         LI=LI+1
         FORREG(LI)='      '//
     &    'COMMON/PARPAR/NUL(7),ITERT'
         LI=LI+1
         FORREG(LI)='      '//
     &   'COMMON/NEQUAZ/NEQMOD'
         LI=LI+1
         FORREG(LI)=
     &     'C*******************************************'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ REGOLATORE '//SIMB_REG//' - '//DESC_REG
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C     ELENCO DEI BLOCCHI :'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
C
         DO I=1,NBL
            J=IPORD(I)
            K=J+1
            LI=LI+1
            FORREG(LI)='C     '//
     &        FIL01(K)(1:4)//FIL01(K)(19:20)//FIL01(K)(24:80)
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C     FINE ELENCO DEI BLOCCHI '
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C*********** INIZIO ISTRUZIONI DICHIARATIVE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C______ USCITE REGOLATORE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NUSC
            LI=LI+1
            IF(ITUSC(I).EQ.0) THEN
               FORREG(LI)='      REAL '//VARUSC(I)
            ELSE
               FORREG(LI)='      LOGICAL '//VARUSC(I)
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C______ INGRESSI REGOLATORE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NING
            LI=LI+1
            IF(ITING(I).EQ.0) THEN
               FORREG(LI)='      REAL '//VARING(I)
            ELSE
               FORREG(LI)='      LOGICAL '//VARING(I)
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)='      REAL JLOGFLO'
         LI=LI+1
         FORREG(LI)='      LOGICAL JFLOLOG'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C*********** INIZIO ISTRUZIONI ESECUTIVE'
         LI=LI+1
         FORREG(LI)=
     &     'C'

         LI=LI+1
         FORREG(LI)=
     &     '      NEQMOD = 0'
         LI=LI+1
         FORREG(LI)=
     &     '      GO TO (1,100,1),IFUN'
         LI=LI+1
         FORREG(LI)=
     &     ' 1    RETURN'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ CALCOLO DELLA RISPOSTA DEI REGOLATORI'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     ' 100  CONTINUE'
         LI=LI+1
         FORREG(LI)=
     &     '      IF(KREGIM.AND.ITERT.GE.0) RETURN'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ DECODIFICA DELLE VARIABILI '
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NUSC
            WRITE(ICAR,'(I3)')I-1
            LI=LI+1
            IF(ITUSC(I).EQ.0) THEN
        FORREG(LI)='      '//VARUSC(I)//' = XYU(IXYU +'//ICAR//')'
            ELSE
               FORREG(LI)='      '//VARUSC(I)//
     &           ' = JFLOLOG(XYU(IXYU +'//ICAR//'))'
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NING
            WRITE(ICAR,'(I3)')NUSC-1+I
            LI=LI+1
            IF(ITING(I).EQ.0) THEN
        FORREG(LI)='      '//VARING(I)//' = XYU(IXYU +'//ICAR//')'
            ELSE
               FORREG(LI)='      '//VARING(I)//
     &           ' = JFLOLOG(XYU(IXYU +'//ICAR//'))'
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     '      IF(ITERT.GT.0)GO TO 333'
C
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ CHIAMATE AI MODULETTI DI REGOLAZIONE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)='      IDD=IPD'
C
         DO I=1,NBL
            J=IPORD(I)
            K=IPUNB(J)
C
            LI=LI+1
            FORREG(LI)='C             '//
     &        FIL01(K)(1:8)//FIL01(K)(38:80)
C
            LI=LI+1
            FORREG(LI)='      CALL '//FIL01(K)(1:4)//'$R('
C
            KI=K+1
            KK=IPUNB(J+1)-3
            NTOT=KK-KI+1
            NSS=MOD(NTOT,10)
            NS=NTOT/10
            IF(NSS.GT.0)NS=NS+1
C
            DO IL=1,NS
               I1=(IL-1)*10 +1
               I2=IL*10
               IF(I2.GT.NTOT)I2=NTOT
               IM=0
               DO JJ=I1,I2
                  KJ=K+JJ
C
                  IM=IM+1
                  II1=(IM-1)*5+1
                  II2=IM*5
                  BUFF(II1:II2)=FIL01(KJ)(1:4)//','
C
               ENDDO
               LI=LI+1
               FORREG(LI)='     &  '//BUFF(1:II2)
C
            ENDDO
            LI=LI+1
            FORREG(LI)='     &  IDD,DATI)'
            LI=LI+1
            FORREG(LI)='C'
C
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     '      IF(ITERT.LT.0)GO TO 111'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &   'C____ CARICAMENTO DELLE VARIABILI DI USCITA DEL REGOLATORE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)=
     &     '  333 CONTINUE'
C
         DO I=1,NUSC
            WRITE(ICAR,'(I3)')I
            LI=LI+1
            IF(ITUSC(I).EQ.0) THEN
               FORREG(LI)='      RNI('//ICAR//') = '//VARUSC(I)
            ELSE
        FORREG(LI)='      RNI('//ICAR//') = JLOGFLO('//VARUSC(I)//')'
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)=
     &     '      RETURN'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ INIZIALIZZAZIONE A REGIME '
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)=
     &     '  111 CONTINUE'
C
         DO I=1,NUSC
            WRITE(ICAR,'(I3)')I-1
            LI=LI+1
            IF(ITUSC(I).EQ.0) THEN
               FORREG(LI)='      XYU(IXYU +'//ICAR//') = '//VARUSC(I)
            ELSE
               FORREG(LI)='      XYU(IXYU +'//ICAR//') = JLOGFLO('
     &                      //VARUSC(I)//')'
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NING
            WRITE(ICAR,'(I3)')NUSC-1+I
            LI=LI+1
            IF(ITING(I).EQ.0) THEN
               FORREG(LI)='      XYU(IXYU +'//ICAR//') = '//VARING(I)
            ELSE
               FORREG(LI)='      XYU(IXYU +'//ICAR//') = JLOGFLO('
     &                      //VARING(I)//')'
            ENDIF
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)=
     &     '      RETURN'
C
         LI=LI+1
         FORREG(LI)=
     &     '      END'
C
         RETURN
      END
      SUBROUTINE RG1_TRAD_I2(NBL,IPUNB,FIL01,IPUN_ING,
     &  IORD,ILIV,FILREG,FORREG,LI,IPORD,VARUSC,VARING,ITUSC,
     &  ITING,NUSC,NING)
C
         COMMON/NOM_REG/SIMB_REG,DESC_REG,TAG_REG
         CHARACTER*4 SIMB_REG
         CHARACTER*50 DESC_REG
         CHARACTER*4 TAG_REG
         CHARACTER*50 BUFF
         DIMENSION IPUNB(*),IORD(*),ILIV(*),IPORD(*)
         DIMENSION ITUSC(*),ITING(*)
         CHARACTER*(*) FIL01(*)
         CHARACTER*(*) FILREG(*)
         CHARACTER*(*) FORREG(*)
         CHARACTER*(*) VARUSC(*)
         CHARACTER*(*) VARING(*)
         CHARACTER*3 ICAR
         CHARACTER*1 AP
         CHARACTER*4 AS
         CHARACTER*80 DESCRI
         DATA AS/'*   '/
C
         WRITE(AP,333)
  333    FORMAT('''')
         DO I=1,NBL
            DO 1 J=1,NBL
               IF(IORD(J).EQ.I)GO TO 2
    1       CONTINUE
    2       IPORD(I)=J
         ENDDO
C
         LI=1
         FORREG(LI)='      SUBROUTINE '//SIMB_REG//'I2'//
     &     '(IFUN,VAR,MX1,IV1,IV2,XYU,DATI,ID1,ID2,'
         LI=LI+1
         FORREG(LI)=
     &     '     &  IBL1,IBL2,IER,CNXYU,TOL)'
C
         LI=LI+1
         FORREG(LI)='      '//
     &     'DIMENSION VAR(MX1,*),XYU(*),DATI(*),CNXYU(*),TOL(*)'
C
         LI=LI+1
         FORREG(LI)='      '//
     &   'CHARACTER*6 NOM_BLOC'
C
         LI=LI+1
         FORREG(LI)='      '//
     &   'CHARACTER*80 DESC_BLOC'
C
         LI=LI+1
         FORREG(LI)=
     &'C BUFDATI:VETTORE CONTENENTE I DATI RICALCOLATI E, A PARTIRE DA'
         LI=LI+1
         FORREG(LI)=
     &   'C INIZOFF, I RISPETTIVI OFFSET NEL VETTORE DATI. IL VETTORE '
         LI=LI+1
         FORREG(LI)=
     &   'C E'' CHIUSO CON -1.'
         LI=LI+1
         FORREG(LI)='      '//
     &   'REAL BUFDATI(1)'
         LI=LI+1
         FORREG(LI)='      '//
     &   'COMMON /BUFFER/ IDBUFREG,NSKIP,INIZOFF,BUFDATI'
C
         LI=LI+1
         FORREG(LI)=
     &     'C*******************************************'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ REGOLATORE '//SIMB_REG//' - '//DESC_REG
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C     ELENCO DEI BLOCCHI :'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         DO I=1,NBL
            J=IPORD(I)
            K=J+1
            LI=LI+1
            FORREG(LI)='C     '//
     &        FIL01(K)(1:4)//FIL01(K)(19:20)//FIL01(K)(24:80)
         ENDDO
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C     FINE ELENCO DEI BLOCCHI '
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C*********** INIZIO ISTRUZIONI ESECUTIVE'
         LI=LI+1
         FORREG(LI)=
     &     'C'

         LI=LI+1
         FORREG(LI)=
     &     '      NEQMOD = 0'

         LI=LI+1
         FORREG(LI)=
     &     '      NSKIP = 0'

         LI=LI+1
         FORREG(LI)=
     &     '      IDBUFREG = 1'

         LI=LI+1
         FORREG(LI)=
     &     '      GO TO(100,200), IFUN'
         LI=LI+1
         FORREG(LI)=
     &     ' 100  CONTINUE'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ CHIAMATE AI MODULETTI DI REGOLAZIONE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)='      IDP=ID1'
C
C
         DO I=1,NBL
C
            WRITE(ICAR,'(I3)')I
C
            J=IPORD(I)
            K=IPUNB(J)
C
            LI=LI+1
            FORREG(LI)='C             '//
     &        FIL01(K)(1:8)//FIL01(K)(38:80)
C
            LI=LI+1
            FORREG(LI)='      NBLOC = '//ICAR
C
C
            LI=LI+1
            FORREG(LI)='      NOM_BLOC = '//AP//FIL01(K)(1:6)//AP
C
            DO JK=38,80
               DESCRI(JK:JK)=FIL01(K)(JK:JK)
               IF(DESCRI(JK:JK).EQ.AP)DESCRI(JK:JK)=' '
            ENDDO
            LI=LI+1
            FORREG(LI)='      DESC_BLOC = '//AP//AS//DESCRI(38:80)//AP
C
            LI=LI+1
            FORREG(LI)='      IF(IFUN.EQ.1) WRITE(14,1000)DESC_BLOC'
C
            LI=LI+1
            FORREG(LI)='      IF(IFUN.EQ.2) READ (14,1000)'
C
            LI=LI+1
            FORREG(LI)='      CALL '//FIL01(K)(1:4)//'$I('//
     &        'IFUN,NOM_BLOC,NBLOC,DATI,IDP,IER)'
C
            LI=LI+1
            FORREG(LI)='C'
C
         ENDDO
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)='      ID2=IDP'
C
         LI=LI+1
         FORREG(LI)='      BUFDATI(INIZOFF+IDBUFREG) = -1'
C
         LI=LI+1
         FORREG(LI)='      RETURN'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
         LI=LI+1
         FORREG(LI)=
     &     'C____ LETTURA DA FILE14 DEL NOME DEL REGOLATORE'
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         LI=LI+1
         FORREG(LI)=
     &     '  200 READ(14,1000)'
         LI=LI+1
         FORREG(LI)=
     &     ' 1000 FORMAT(A)'
C
         LI=LI+1
         FORREG(LI)=
     &     '      GO TO 100'
C
         LI=LI+1
         FORREG(LI)=
     &     '      END'
C
         LI=LI+1
         FORREG(LI)=
     &     'C'
C
         RETURN
      END


      INTEGER*4 FUNCTION LUNGHEZZA_S(STRINGA)
         CHARACTER*4 STRINGA
         INTEGER*4 I

         DO 666,I=1,4
C           WRITE(6,888) STRINGA(I:I)
C888        FORMAT(A1)
            NUM = ICHAR(STRINGA(I:I))
            IF ( (NUM.GE.123).OR.(NUM.LE.47) ) GOTO 998
            IF ( (NUM.GE.58).AND.(NUM.LE.64) ) GOTO 998
            IF ( (NUM.GE.91).AND.(NUM.LE.96) ) GOTO 998
  666    CONTINUE
  998    LUNGHEZZA_S = I-1
         RETURN
      END



      INTEGER*4 FUNCTION LUNGHEZZA_P(STRINGA)
         CHARACTER*180 STRINGA
         INTEGER*4 I

         DO 667,I=1,180
C          WRITE(6,889) STRINGA(I:I)
C889       FORMAT(A1)
            IF( STRINGA(I:I).EQ.' ' )  GOTO 999
  667    CONTINUE
  999    LUNGHEZZA_P = I-1
         RETURN
      END





      SUBROUTINE CONV_LOWER(
C
C INPUT
     &                       INSTR, LSTR,
C
C OUTPUT
     &                       OUTSTR )
C
C La subroutine effettua la conversione dei caratteri maiuscoli
C presenti nella variabile a stringa INSTR in caratteri minuscoli,
C lasciando inalterati gli altri caratteri eventualmente presenti e
C trasferendo il risultato della conversione nella variabile stringa
C OUTSTR.
C La lunghezza di tale stringa e'' passata come argomento nella
C variabile intera LSTR. Tale valore deve essere congruente con la
C lunghezza fornita nella dichiarazione della stinga (LSTR non deve
C essere maggiore della lunghezza fornita in fase di dichiarazione).
C In ogni caso vengono convertiti un numero di NSTR caratteri.
C La lunghezza della stringa di output OUTSTR deve essere uguale
C a quella di INSTR.
C Nell''algoritmo viene sfruttata la progressivita'' decimale dei
C simboli nella codifica ASCII. In tale codifica le lettere minuscole
C hanno codifica decimale da 97 ("a") a 122 ("z") mentre le lettere
C maiuscole hanno la codifica decimale da 65 ("A") a 90 ("Z").
C E'' quindi possibile ottenere la codifica decimale delle lettere da
C quella delle minuscole semplicemente sottraendo il valore 32 a
C tale codifica.
C E'' poi possibile attraverso le function ICHAR e CHAR ottenere
C rispettivamente il Codice Decimale ASCII partendo da un carattere
C ed il carattere ascii corrispondente ad una certa codifica decimale.
C
C Input
C
C   INSTR    char*1  Variabile contenente la STRINGA in INPUT.
C                    La lunghezza di tale stringa e quella fornita in
C                    fase di dichiarazione della variabile all''esterno
C                    di questa routine.
C
C   LSTR     int*2   LUNGHEZZA in byte della stringa in input INSTR.
C                    Tale valore deve essere minore o uguale alla
C                    lunghezza della variabile dichiarata nella routine
C                    chiamante; nel caso in cui tale valore sia minore
C                    di quello presente nella dichiarazione la
C                    conversione avra'' luogo solo per i primi LSTR
C                    caratteri di INSTR.
C
C Output
C
C   OUTSTR   char*1  Variabile contenente la STRINGA in OUTPUT.
C                    La lunghezza di tale stringa e'' quella fornita in
C                    fase di dichiarazione della variabile all''esterno
C                    di questa routine.


C Definizione Variabili

         integer LSTR

         character*(*) INSTR, OUTSTR

C Do-loop per lettura caratteri da stringa e conversione

         do  I=1, LSTR

C Se i caratteri in input sono maiuscoli oppure sono caratteri
C speciali vengono trasferiti inalterati nella stringa di output
C OUTSTR.

            if ( ( ichar(INSTR(I:I) ) .ge. 65 ) .and.
     &          ( ichar(INSTR(I:I) ) .le. 90 ) ) then

               OUTSTR(I:I) = char( ichar(INSTR(I:I) ) + 32 )
            else

C Se i caratteri in input sono minuscoli vengono trasferiti
C convertiti in maiuscolo stringa di output OUTSTR.

               OUTSTR(I:I) = INSTR(I:I)
            end if

         enddo
         return

      end
