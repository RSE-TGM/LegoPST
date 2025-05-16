      SUBROUTINE INITSM
      REAL VALDA
      CHARACTER*100 TAB_PATH, TAB_FILE
      COMMON /LECT/VALDA(129690)
      CALL GETENV("LG_LEGO",TAB_PATH)
      DO I=1,100
         IF (TAB_PATH(I:I).EQ.' ') GOTO 1000
      ENDDO
1000  CONTINUE
      TAB_FILE=TAB_PATH(1:I-1) // '/TAVOLE.DAT'
      WRITE(6,*) TAB_FILE
      OPEN(UNIT=2,FILE=TAB_FILE,STATUS='OLD',
     $     FORM='UNFORMATTED')
      READ(2) (VALDA(I),I=1,129690)
      CLOSE(2)
      RETURN
      END
      SUBROUTINE NEWT (NEW,FONC)
      COMMON/LECT/VALDA(129690)
      COMMON/INTE/ID,IP,X(2),Y(2),INCR,PAE,PAI
      DIMENSION FX(2),FON(3)
      JJ=ID
      IF(NEW.GT.0) JJ=JJ+12
      I=0
      DO 3 J=ID,JJ,6
      I=I+1
      FX(1)=(VALDA(J+IP)-VALDA(J))*X(1)*PAI
      FX(2)=(VALDA(J+2*IP)-VALDA(J+IP))*X(1)*PAI
      FON(I)=VALDA(J)+FX(1)+(FX(2)-FX(1))*X(2)*.5*PAI
    3 CONTINUE
      IF(NEW.GT.0) GO TO 4
      FONC=FON(1)
      RETURN
    4 FX(1)=(FON(2)-FON(1))*Y(1)*PAE
      FX(2)=(FON(3)-FON(2))*Y(1)*PAE
      FONC=FON( 1)+FX( 1)+(FX(2)-FX(1))*Y(2)*.5*PAE
      RETURN
      END
      SUBROUTINE PRST (TETA,BETS)
      DOUBLE PRECISION CK
      COMMON/TSAT/CK(9)
      DIFT=1.-TETA
      TNUM=0.
      DO 1 I=1,5
    1 TNUM=TNUM+CK(I)*DIFT**I
      DNUM=1.
      DO 2 I=1,2
    2 DNUM=DNUM+CK(I+5)*DIFT**I
      BETS=EXP(1./TETA*TNUM/DNUM-DIFT/(CK(8)*DIFT**2+CK(9)))
      RETURN
      END
      SUBROUTINE CALSR
      COMMON/INIT/II,KK,KLIM,I0,S(5)
      COMMON/THER/IPHA,IN,TITRE
      COMMON/CONT/PD,SD
      COMMON/LECT/VALDA(129690)
      COMMON/INTE/ID,IP,X(2),Y0,Y1,INCR,PAE,PAI
      DIMENSION SLIM(5)
      TITRE=0.
      IPHA=0
      DO 4 I=1,4,3
      IF(I0-KK) 2,1,1
    1 SLIM(I)=4442.86472
      GO TO 3
    2 ID=INCR-6+I
      CALL NEWT (0,SLIM(I))
    3 IF(SD-SLIM(I)) 5,5,4
    4 CONTINUE
C*********************************************************
      IF(SD.GT.VALDA(INCR+6+2*IP)) THEN
          CALL DIAGN(2,SD,PD)
          SD=VALDA(INCR+6+2*IP)
          ENDIF
C*********************************************************
      I=4
      TITRE=1.
      GO TO 6
    5 IF(I.EQ.4) GO TO 10
    6 SLIM(I+1)=SLIM(I)+S(I)*(VALDA(INCR+I)-1)
      IF(SD-SLIM(I+1)) 7,8,8
    7 J=2+0.67*(I-1)
      GO TO 9
    8 J=1+1.34*(I-1)
    9 PAE=1./S(J)
      JZ=ABS((SD-SLIM(J))*PAE)
      IF(JZ+3.GT.INT(VALDA(INCR+J))) JZ=JZ-1
      Y0=SD-SLIM(J)-JZ*S(J)
      Y1=Y0-S(J)
      JZ=JZ+2
      J=J-1
      IF(J.EQ.0) GO TO 13
      GO TO 11
   10 IPHA=1
      TITRE=(SD-SLIM(1))/(SLIM(4)-SLIM(1))
      JZ=2
      J=2
   11 DO 12 I=1,J
   12 JZ=JZ+VALDA(INCR+I)
   13 IN=INCR+(JZ-1)*6
      RETURN
      END
      FUNCTION ROEV (P,S,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      DIMENSION V(2)
      CALL CONTRO (P,S)
      ID=IN+1
      IF(IPHA.EQ.1) GO TO 3
      CALL NEWT (1,ROEV)
      RETURN
    3 DO 5 I=1,2
      CALL NEWT (0,V(I))
    5 ID=IN+7
      ROEV=1./(V(1)*(1.-TITRE)+V(2)*TITRE)
      RETURN
      END
      FUNCTION TEV (P,S,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      CALL CONTRO (P,S)
      ID=IN+2
      NWT=1
      IF(IPHA.EQ.1) NWT=0
      CALL NEWT (NWT,TEV)
      RETURN
      END
      FUNCTION HEV (P,S,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      DIMENSION V(2)
      CALL CONTRO (P,S)
      ID=IN+3
      IF(IPHA.EQ.1) GO TO 3
      CALL NEWT (1,HEV)
      RETURN
    3 DO 5 I=1,2
      CALL NEWT (0,V(I))
    5 ID=IN+9
      HEV=V(1)*(1.-TITRE)+V(2)*TITRE
      RETURN
      END
      FUNCTION A2EV (P,S,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      DIMENSION V(2)
      CALL CONTRO (P,S)
      IF(IPHA.EQ.1) GO TO 3
      ID=IN+4
      CALL NEWT (1,A2EV)
      RETURN
    3 L=1
    4 ID=IN+L
      DO 6 I=1,2
      CALL NEWT (0,V(I))
    6 ID=IN+L+6
      IF(L.GT.1) GO TO 7
      VOL=V(1)*(1.-TITRE)+V(2)*TITRE
      L=4
      GO TO 4
    7 A2EV=VOL*VOL/(V(1)*(1.-TITRE)+V(2)*TITRE)
      RETURN
      END
      FUNCTION BEV (P,S,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      DIMENSION V(2)
      CALL CONTRO (P,S)
      IF(IPHA.EQ.1) GO TO 3
      ID=IN+5
      CALL NEWT (1,BEV)
      RETURN
    3 L=1
      M=2
    4 ID=IN+L
      DO 6 I=1,M
      CALL NEWT (0,V(I))
    6 ID=IN+L+6
      IF(L.GT.1) GO TO 7
      RHO=1./(V(1)*(1.-TITRE)+V(2)*TITRE)
      L=5
      M=1
      GO TO 4
    7 BEV=V(1)*RHO*RHO
      RETURN
      END
      FUNCTION CPEV (P,S,Y,YC,NFL)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      COMMON/THER/IPHA,IN,TITRE
      CALL CONTRO (P,S)
      IF(IPHA.EQ.1) GO TO 3
      I=1
    1 ID=IN+6
    2 CALL NEWT (I,CPEV)
      RETURN
    3 I=0
      IF(Y-YC) 1,1,4
    4 ID=IN+12
      GO TO 2
      END
      FUNCTION YEV (P,S,NFL)
      COMMON/THER/IPHA,IN,TITRE
      CALL CONTRO (P,S)
      YEV=TITRE
      RETURN
      END
      SUBROUTINE SATUR (P,NCAL,RES1,RES2,NFL)
      COMMON/CONT/PD,SD
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      DIMENSION R(2)
      IF(P.GT.221.2E5) GO TO 7
      IF(P-PD) 1,2,1
C*********************************************************
    1 IF(P.LT.610.) THEN
          CALL DIAGN (3,SD,P)
          P=610.
          ENDIF
C*********************************************************
      PD=P
      SD=-.1
      CALL TABLE
    2 IF(NCAL-7) 4,3,3
    3 IN=NCAL+INCR+1
      N=1
      GO TO 5
    4 IN=NCAL+INCR-6
      N=2
    5 DO 6 I=1,N
      ID=IN
      CALL NEWT (0,FONC)
      R(I)=FONC
    6 IN=IN+3
      RES1=R(1)
      RES2=R(2)
      R(2)=0.
      RETURN
C*********************************************************
    7 CALL DIAGN (5,SD,P)
           P=221.2E+05
C*********************************************************
      RETURN
      END
      SUBROUTINE CONTRO (P,S)
      COMMON/CONT/PD,SD
      IF(P-PD) 3,1,3
    1 IF(S-SD) 4,2,4
    2 RETURN
C*********************************************************
    3 IF(P.LT.610.) THEN
          CALL DIAGN (3,S,P)
          P=610.
          ENDIF
C*********************************************************
      PD=P
      CALL TABLE
    4 IF(S.LT.(-.2)) THEN
          CALL DIAGN (1,S,P)
          S=-.2
          ENDIF
C*********************************************************
      SD=S
      CALL CALSR
      RETURN
      END
      SUBROUTINE TABLE
      COMMON/INIT/II,KK,KLIM,I0,S(5)
      COMMON/CONT/PD,SD
      COMMON/TABL/PRT(33),PAP(32),IC(32),IOFFSET(32)
      COMMON/INTE/ID,IP,X0,X1,Y(2),INCR,PAE,PAI
      COMMON/LECT/VALDA(129690)
      IF(PD-PRT(21)) 1,1,2
    1 IA=1
      IF=KK-1
      GO TO 3
    2 IA=KK
      IF=KLIM
    3 DO 7 IREG=IA,IF
      IF(PD-PRT(IREG+1)) 4,4,7
    4 PAI=1./PAP(IREG)
      I1=(PD-PRT(IREG))*PAI+1
      IF(I1-II+2) 6,5,5
    5 I1=II-2
    6 X0=PD-PRT(IREG)-(I1-1)*PAP(IREG)
      X1=X0-PAP(IREG)
      GO TO 8
    7 CONTINUE
C*********************************************************
      CALL DIAGN (4,SD,PD)
      PD=PRT(IREG+1)
C*********************************************************
    8 I0=IREG
	    IP=IC(IREG)*6
	    I=0
	    IF(IREG.LT.KK) I=1
	    INCR=(I1-1)*IP+6*I+IOFFSET(IREG)
      RETURN
      END
      BLOCK DATA CONLIS
      DOUBLE PRECISION A,B,C,D,PA,PB,PC,CK,
     &                 COE1,COE2,COE3,ETA1,ETA2,ALAM,ALBM,ALCM
      COMMON/LECT/VALDA(129690)
      COMMON/TABL/PRT(33),PAP(32),IC(32),IOFFSET(32)
      COMMON/CONT/PD,SD
      COMMON/INIT/II,KK,KLIM,I0,S(5)
      COMMON/VISC/A(5),B(3),C(3),D(3),COE1,COE2,COE3,ETA1,ETA2,ET1(14),E
     &T2(14),ET3(14),ET4(10)
      COMMON/COMU/PTS(23),TES(14)
      COMMON/TSAT/CK(9)
      COMMON/COND/PA(5),PB(4),PC(4),ALE(9),ALV(10),AL2(21),ALAM,ALBM,ALC
     &M
      DATA PAP/.99E2,.19E3,.40E3,.75E3,.15E4,.30E4,.70E4,.17E5,.35E5,.65
     &E5,.14E6,.29E6,.50E6,.50E6,.40E6,.20E6,4*.13E5,3*.8E4,2*.9E4,.10E5
     &,.16E5,.15E6,.24E6,.33E6,.40E6,.60E6/
      DATA PRT/.61E3,.16E4,.35E4,.75E4,.15E5,.30E5,.60E5,.13E6,.30E6,.65
     &E6,.13E7,.27E7,.56E7,.106E8,.156E8,.196E8,.216E8,.2173E8,.2186E8,.
     &2199E8,.2212E8,.222E8,.2228E8,.2236E8,.2245E8,.2254E8,.2264E8,.228
     &E8,.243E8,.267E8,.300E8,.340E8,.400E8/
      DATA S(1),S(2),S(4),S(5),S(3)/-56.,-150.,56.,120.,0./
      DATA IC/41,44,48,47,48,2*49,51,52,53,54,57,59,62,66,3*69,71,73,8*7
     &0,2*69,2*68/
      DATA A,B,C,D/2.414D+2,3.828209486D-1,2.162830218D-1,1.498693949D-1
     &,4.711880117D-1,2.634511D+2,4.219836243D-1,8.04D+1,5.861198738D+2,
     &1.204753943D+3,4.219836243D-1,1.113564669D+2,6.732080129D+1,3.2051
     &47019D+0/
      DATA ET1/901.,865.,830.,794.,754.,709.,683.,653.,614.,589.,560.,52
     &7.,491.,450./
      DATA ET2/198.4,207.,217.,231.,247.,266.,277.5,292.,311.,323.,340.,
     &363.,400.,450./
      DATA ET3/245.7,248.3,251.4,255.4,260.6,268.,273.1,279.8,289.1,295.
     &8,305.1,316.6,331.6,343.5/
      DATA ET4/343.5,487.8,535.7,568.0,596.3,633.1,658.5,678.7,695.2,722
     &.1/
      DATA PA,PB,PC/-9.2247D-1,6.728934102D+0,-1.011230521D+1,6.99695383
     &2D+0,-2.31606251D+0,-2.0954276D-1,1.320227345D+0,-2.485904388D+0,1
     &.517081933D+0,8.104183147D-2,-4.513858027D-1,8.057261332D-1,-4.668
     &315566D-1/
      DATA ALE/.437,.4187,.399,.377,.3635,.348,.326,.290,.238/
      DATA ALV/.1070,.119,.1272,.137,.148,.156,.166,.1815,.2075,.238/
      DATA AL2/.0882,.0929,.0988,.1061,.1105,.1155,.1201,.1239,.1256,.12
     &98,.1087,.1115,.1163,.1246,.1468,.1225,.1376,.1569,.1817,.1322,.15
     &33/
      DATA PTS/85.927E5,98.7E5,112.89E5,128.63E5,146.05E5,165.35E5,175.7
     &7E5,186.75E5,198.33E5,204.35E5,210.54E5,215.62E5,219.5E5,221.2E5,2
     &25.E5,230.E5,238.E5,250.E5,275.E5,300.E5,325.E5,350.E5,400.E5/
      DATA TES/573.15,583.15,593.15,603.15,613.15,623.15,628.15,633.15,6
     &38.15,640.65,643.15,645.15,646.65,647.3/
      DATA CK/-7.691234564D+0,-2.608023696D+1,-1.681706546D+2,6.42328550
     &4D+1,-1.189646225D+2,4.16711732D+0,2.09750676D+1,1.D+9,6.0D+0/
	    DATA COE1,COE2,COE3,ETA1,ETA2/.9012087711D+03,.6208479236D-01,
     &       .2776015497D+02,.2330250165D+03,.2025000213D+03/
	    DATA ALAM,ALBM,ALCM/.4369649291D+00,.1110928059D+00,
     &                      -.2328296876D-01/
	    DATA IOFFSET/0,2706,5610,8778,11880,15048,18282,21516,24882,
     &28314,31812,35376,39138,43032,47124,51480,56034,60588,65142,69828,
     &74646,79266,83886,88506,93126,97746,102366,106986,111606,116160,
     &120714,125202/
	    DATA II,KK,KLIM/11,21,32/
	    DATA PD,SD/0.,-.2/
C
      END
      FUNCTION ETEV(P,T,R,RA,Y,YC,NFL)
      DOUBLE PRECISION  A,B,C,D,COE1,COE2,COE3,ETA1,ETA2
      COMMON/VISC/A(5),B(3),C(3),D(3),COE1,COE2,COE3,ETA1,ETA2,ET1(14),E
     &T2(14),ET3(14),ET4(10)
      COMMON/COMU/PTS(23),TES(14)
      IT=1
      TETA=T/647.3
      IF(T-573.15) 1,1,4
    1 IF(Y-YC) 2,2,5
    2 AAAA=(A(2)/(TETA-A(3)))
      ETEV=A(1)*10.**AAAA
      IF(Y) 3,3,11
    3 CALL PRST (TETA,BETS)
      ETEV=ETEV*(1.+(P/221.2E5-BETS)*A(4)*(TETA-A(5)))
      GO TO 11
    4 IT=0
    5 RHOR=R*.317E-2
      ETEV=B(1)*(TETA-B(2))+B(3)
      IF(IT.EQ.0) GO TO 8
      IF(Y-1.) 6,7,7
    6 RHOR=RHOR*((1.-Y)/RA+Y)
    7 ETEV=ETEV-RHOR*(C(1)-C(2)*(TETA-C(3)))
      GO TO 11
    8 IF(T-648.15) 12,9,9
    9 DO 10 I=1,3
   10 ETEV=ETEV+D(I)*RHOR**I
   11 ETEV=ETEV*1.E-7
      RETURN
   12 IF(P-PTS(1)) 22,22,13
   13 DO 19 I=2,14
      IF(P-PTS(I)) 14,14,19
   14 N1=I-1
      N2=I
      PRO=(P-PTS(N1))/(PTS(N2)-PTS(N1))
      IF(Y-YC) 15,15,17
   15 ETEV=ET1(N1)*(1-PRO)+ET1(N2)*PRO
      IF(Y) 16,16,11
   16 ETS=ETEV
      TS=TES(N1)*(1-PRO)+TES(N2)*PRO
      GO TO 23
   17 ETEV=ET2(N1)*(1-PRO)+ET2(N2)*PRO
      IF(Y-1.) 11,18,18
   18 ETE=ETEV
      ETS=ET3(N1)*(1-PRO)+ET3(N2)*PRO
      TE=TES(N1)*(1-PRO)+TES(N2)*PRO
      TS=648.15
      GO TO 25
   19 CONTINUE
      DO 21 I=15,23
      IF(P-PTS(I)) 20,20,21
   20 ETS=ET4(I-14)+(ET4(I-13)-ET4(I-14))*(P-PTS(I-1))/(PTS(I)-PTS(I-1))
      TS=648.15
      GO TO 23
   21 CONTINUE
   22 ETE=ETA2-RHOR*COE3
      ETS=ETA1+D(1)*RHOR+D(2)*RHOR**2+D(3)*RHOR**3
      TS=648.15
      GO TO 24
   23 ETE=COE1*(1.+(P-PTS(1))/221.2E5*COE2)
   24 TE=573.15
   25 ETEV=(((ETS-ETE)/(TS-TE)*(T-TE))+ETE)
      GO TO 11
      END
      FUNCTION ALEV (P,T,R,RA,Y,YC,NFL)
      DOUBLE PRECISION PA,PB,PC,ALAM,ALBM,ALCM
      COMMON/COND/PA(5),PB(4),PC(4),ALE(9),ALV(10),AL2(21),ALAM,ALBM,ALC
     &M
      COMMON/COMU/PTS(23),TES(14)
      IT=1
      TETA=T/647.3
      IF(T-623.15) 1,1,8
    1 IF(Y-YC) 2,2,6
    2 ALEV=PA(1)
      DO 3 I=2,5
    3 ALEV=ALEV+PA(I)*TETA**(I-1)
      IF(Y) 4,4,29
    4 AMO1=PB(1)
      AMO2=PC(1)
      DO 5 I=2,4
      TT=TETA**(I-1)
      AMO1=AMO1+PB(I)*TT
    5 AMO2=AMO2+PC(I)*TT
      CALL PRST (TETA,BETS)
      DIFB=P/221.2E5-BETS
      ALEV=ALEV+DIFB*AMO1+DIFB**2*AMO2
      GO TO 29
    6 IF(Y-1.) 7,9,9
    7 DELTA=(R*((1-Y)/RA+Y))*1.E-3
      GO TO 35
    8 IT=0
      TE=623.15
    9 DELTA=R*1.E-3
      IF(IT.EQ.1) GO TO 35
      IF(P.GT.350.E5) GO TO 11
      IF(P.GT.275.E5) GO TO 12
      IF(P.GT.225.E5) GO TO 13
      IF(P.GT.221.2E5)GO TO 14
      IF(P.GT.PTS(6)) GO TO 26
      GO TO 35
   11 IF(T-773.15) 15,35,35
   12 IF(T-723.15) 16,35,35
   13 IF(T-698.15) 17,35,35
   14 IF(T-673.15) 18,35,35
   15 N1=23
      N2=23
      N3=3
      TS=773.15
      GO TO 30
   16 N1=20
      N2=22
      N3=2
      TS=723.15
      GO TO 30
   17 N1=16
      N2=19
      N3=1
      TS=698.15
      GO TO 30
   18 N1=15
      N2=15
      N3=0
      TS=673.15
      GO TO 30
   19 DO 25 I=7,14
      IF(P-PTS(I)) 20,20,25
   20 PRO=(P-PTS(I-1))/(PTS(I)-PTS(I-1))
      IF(Y-YC) 21,21,23
   21 ALEV=ALE(I-6)*(1-PRO)+ALE(I-5)*PRO
      IF(Y) 22,22,29
   22 ALS=ALEV
      TS=TES(I-1)*(1-PRO)+TES(I)*PRO
      GO TO 33
   23 ALEV=ALV(I-6)*(1-PRO)+ALV(I-5)*PRO
      IF(Y-1.) 29,24,24
   24 ALI=ALEV
      TE=TES(I-1)*(1-PRO)+TES(I)*PRO
      ALS=AL2(I-6)*(1-PRO)+AL2(I-5)*PRO
      TS=673.15
      GO TO 34
   25 CONTINUE
   26 IF(T-673.15) 19,35,35
   29 RETURN
   30 DO 32 I=N1,N2
      IF(P-PTS(I)) 31,31,32
   31 N4=I-6+N3
      ALS=AL2(N4)+(AL2(N4+1)-AL2(N4))*(P-PTS(I-1))/(PTS(I)-PTS(I-1))
      GO TO 33
   32 CONTINUE
   33 DIFB=(P-PTS(6))/221.2E5
      ALI=ALAM+DIFB*ALBM+DIFB**2*ALCM
   34 ALEV=(((ALS-ALI)/(TS-TE)*(T-TE))+ALI)
      GO TO 29
   35 TT=T-273.15
      IF(TT.LE.0.) TT=.1
      AL1=17.6+5.87E-2*TT+1.04E-4*TT**2-4.51E-8*TT**3
      ALEV=(AL1+(103.51+.4198*TT-2.771E-5*TT**2)*DELTA+2.1482E14/TT**4.2
     &*DELTA**2)*1.E-3
      GO TO 29
      END
      FUNCTION SHEV(P,H,NFL)
C
C  CALCOLA S(P,H) CON LE TAVOLE COMPLETE
C
      COMMON/THER/IPHA,IN,TITRE
      COMMON/CONT/PD,SD
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      DIMENSION SWV(2)
C
C  CONTROLLA P MIN. E CARICA LA REGIONE
C
      CALL CHI(P)
C
C  INDIVIDUA L'ISOCLINA E CALCOLA IL TITOLO
C
      CALL CERCA(H,S0,NON)
C--- NON =1 SE FUORI DALLE TAVOLE
      IF(NON.EQ.1) THEN
        SD=S0
        GO TO 3
      ENDIF
C
      IF(IPHA.EQ.1) GO TO 1
C
C  NON SATURO : INVERTE INTERPOLAZIONE IN S
C
      ID=IN+3
      CALL TROVA(H,S0)
      GO TO 3
C
C  SATURO : CALCOLA S CON IL TITOLO
C
    1 DO 2 I=1,2
      ID=INCR-6+I*I
      CALL NEWT(0,SWV(I))
    2 CONTINUE
      SD=SWV(1)*(1.-TITRE)+SWV(2)*TITRE
C
    3 SHEV=SD
      RETURN
      END
      SUBROUTINE CHI(P)
      COMMON/CONT/PD,SD
C
      IF(P-PD) 1,2,1
C
C  CONTROLLA IL VALORE MINIMO E IL VALORE MASSIMO P
C
C*********************************************************
    1 IF(P.LT.610.) THEN
           CALL DIAGN(6,H,P)
           P=610.
           ENDIF
      IF(P.GT.400.E5) THEN
           CALL DIAGN(6,H,P)
           P=400.E5
           ENDIF
C*********************************************************
      PD=P
C
C  CARICA IN MEMORIA LA REGIONE CHE CONTIENE PD
C
      CALL TABLE
C
    2 RETURN
      END
      SUBROUTINE CERCA(HD,S0,NON)
      COMMON/INIT/II,KK,KLIM,I0,S(5)
      COMMON/THER/IPHA,IN,TITRE
      COMMON/CONT/PD,SD
      COMMON/LECT/VALDA(129690)
      COMMON/INTE/ID,IP,XY(4),INCR,PAE,PAI
      DIMENSION SLIM(5),HLIM(6),NNP(5)
C
      NON=0
      TITRE=0.
      IPHA=0
C
C  CALCOLA H LIMITE SOTTOREG. 1,4
C  E DISTINGUE SOTTOREG. 1,2 O 3 O 4,5
C
      IF(I0-KK) 2,1,1
C
C  (REGIONI SOPRACRITICHE)
C
    1 ID=INCR+9
      CALL NEWT(0,HLIM(1))
      HLIM(4)=HLIM(1)
      IF(HD-HLIM(1))100,200,200
C
C  (REGIONI SOTTOCRITICHE)
C
    2 DO 3 I=1,4,3
      ID=INCR-6+I+1
      CALL NEWT(0,HLIM(I))
      IF(HD-HLIM(I)) 4,4,3
    3 CONTINUE
      GO TO 200
    4 IF(I.EQ.1)GO TO 100
C
C  SATURO : CALCOLA IN E TITOLO
C
      IPHA=1
      TITRE=(HD-HLIM(1))/(HLIM(4)-HLIM(1))
      IS=VALDA(INCR+1)+VALDA(INCR+2)
      IN=INCR+(IS+1)*6
      RETURN
C
C  SOTTOREG. 1,2
C
C  (CALCOLA H MIN SOTTOREG. 1,2 E INDIVIDUA SOTTOREG. :I)
C
  100 ID=INCR+3
      DO 101 I=1,2
      JLIM=I
      ID=ID+6*VALDA(INCR+I)
      CALL NEWT(0,HLIM(I+1))
      IF(HD-HLIM(I+1)) 101,102,102
  101 CONTINUE
C
C  (ENTALPIA TROPPO PICCOLA)
C
C*********************************************************
      CALL DIAGN(6,HD,PD)
           HD=HLIM(JLIM+1)
           S0=SLIM(JLIM)           
           NON=1
           RETURN
C*********************************************************
C
  102 IN=INCR+6*VALDA(INCR+I-1)*(I-1)+3
      NNP(I)=VALDA(INCR+I)-2
C
C  (SE LA SOTTOREG. HA SOLO 3 P.TI IS=1)
C
      K=2
      IF(NNP(I).EQ.1)GO TO 106
C
C  CERCA H(P0) < HD
C
      DO 103 J=2,NNP(I)
      IF(HD-VALDA(IN+6*J)) 103,104,104
  103 CONTINUE
C
C  (L'ISOCLINA E' LA TERZULTIMA IS=NNP(I) )
C
      K=NNP(I)+1
      GO TO 106
C
C  CERCA H(P) < HD
C
  104 DO 105 K=J,NNP(I)
      ID=IN+6*K
      CALL NEWT(0,HINT)
      IF(HD-HINT)105,106,106
  105 CONTINUE
C
C  L'ISOCLINA E' LA TERZULTIMA IS=NNP(I)
C
      K=NNP(I)+1
  106 IS=K-1
C
C  CALCOLA S MAX DELLE SOTTOREG. 1,2 PER IL CALCOLO DI S0
C
      IF(I0-KK)108,107,107
  107 SLIM(1)=4442.86472
      GO TO 109
  108 ID=INCR-6+1
      CALL NEWT(0,SLIM(1))
  109 IF(I.EQ.2) SLIM(2)=SLIM(1)+S(1)*(VALDA(INCR+1)-1)
      GO TO 300
C
C  SOTTOREG. 4,5
C
  200 TITRE=1.
      INCR4=INCR
      DO 201 I=1,3
  201 INCR4=INCR4+6*VALDA(INCR+I)
      ID=INCR4+3
C
C  (CALCOLA H MAX  SOTTOREG. 4,5 E INDIVIDUA SOTTOREG. :I)
C
      DO 202 I=4,5
      JLIM=I
      ID=ID+6*VALDA(INCR+I)
      CALL NEWT(0,HLIM(I+1))
      IF(HLIM(I+1)-HD) 202,203,203
  202 CONTINUE
C
C  (ENTALPIA TROPPO ELEVATA)
C
C*********************************************************
      CALL DIAGN(6,HD,PD)
           HD=HLIM(JLIM+1)
           S0=SLIM(JLIM)
           NON=1
           RETURN
C*********************************************************
C
  203 IN=INCR4+6*VALDA(INCR+I-1)*(I-4)+3
      NNP(I)=VALDA(INCR+I)-2
C
C  (SE LA SOTTOREG. HA SOLO 3 P.TI IS=1)
C
      K=1
      IF(NNP(I).EQ.1)GO TO 207
C
C  CERCA H(P0) > HD
C
      DO 204 J=2,NNP(I)
      IF(VALDA(IN+6*J)-HD) 204,205,205
  204 CONTINUE
      J=NNP(I)
C
  205 JJ=J-1
C
C  CERCA H(P) < HD
C
      DO 206 KJ=1,JJ
      K=J-KJ+1
C  (K=J,2)
      ID=IN+6*K
      CALL NEWT(0,HINT)
      IF(HD-HINT) 206,207,207
  206 CONTINUE
C
C  (L'ISOCLINA E' LA PRIMA IS=1)
C
      K=1
C
  207 IS=K
C
C  CALCOLA S MIN DELLE SOTTOREG. 4,5 PER IL CALCOLO DI S0
C
      IF(I0-KK)209,208,208
  208 SLIM(4)=4442.86472
      GO TO 210
  209 ID=INCR-6+4
      CALL NEWT(0,SLIM(4))
  210 IF(I.EQ.5) SLIM(5)=SLIM(4)+S(4)*(VALDA(INCR+4)-1)
      GO TO 300
C
C  SOTTOREG. 1,2,4,5
C  CALCOLA PASS0 IN S,IN,S0
C
  300  PAE=1./S(I)
      IN=IN+6*IS-3
      S0=SLIM(I)+S(I)*(IS-1)
      RETURN
      END
      SUBROUTINE TROVA(HD,S0)
      COMMON/LECT/VALDA(129690)
      COMMON/INTE/ID,IP,X(2),Y(2),INCR,PAE,PAI
      COMMON/CONT/PD,SD
      DIMENSION FX(2),FON(3)
C
C  CALCOLA H(P0),H(P1),H(P2)
C
      JJ=ID+12
      I=0
      DO 3 J=ID,JJ,6
      I=I+1
      FX(1)=(VALDA(J+IP)-VALDA(J))*X(1)*PAI
      FX(2)=(VALDA(J+2*IP)-VALDA(J+IP))*X(1)*PAI
      FON(I)=VALDA(J)+FX(1)+(FX(2)-FX(1))*X(2)*.5*PAI
    3 CONTINUE
C
C  INVERSIONE INTERPOLAZIONE IN S
C  CALCOLA SD,Y0,Y1
C
      AA=FON(3)-2.*FON(2)+FON(1)
      BB=-AA+2.*FON(2)-2.*FON(1)
      CC=-2.*(HD-FON(1))
      CSI=-2.*CC/(BB+SIGN(1.,BB)*SQRT(BB*BB-4.*AA*CC))
      SD=S0+CSI/PAE
      Y(1)=SD-S0
      Y(2)=Y(1)-1./PAE
      RETURN
      END
      SUBROUTINE DRSEV(P,NCAL,RES1,RES2)
      COMMON/CONT/PD,SD
      COMMON/INTE/ID,IP,X(2),Y(2),INCR,PAE,PAI
      COMMON/LECT/VALDA(129690)
      DIMENSION DRP(3),R(3)
      IF(P.GT.221.2E5) GO TO 8
      IF(P-PD) 1,2,1
C*********************************************************
    1 IF(P.LT.610.) THEN
           CALL DIAGN (3,SD,P)
           P=610.
           ENDIF
C*********************************************************
      PD=P
      SD=-.1
      CALL TABLE
   2  IF(NCAL-2) 7,4,3
   3  IN=INCR+(VALDA(INCR+1)+VALDA(INCR+2)+3)*6
      GO TO 5
   4  IN=INCR+6
   5  DO 6 I=1,3
      R(I)=VALDA(IN+IP*(I-1)+1)
      T=VALDA(IN+IP*(I-1)+2)
      A2=VALDA(IN+IP*(I-1)+4)
      B=VALDA(IN+IP*(I-1)+5)
      DRP(I)=1./A2-B/(R(I)*T)
   6  CONTINUE
      FX1=(DRP(2)-DRP(1))*X(1)*PAI
      FX2=(DRP(3)-DRP(2))*X(1)*PAI
      RES1=DRP(1)+FX1+(FX2-FX1)*X(2)*.5*PAI
      FX1=(R(2)-R(1))*X(1)*PAI
      FX2=(R(3)-R(2))*X(1)*PAI
      RES2=R(1)+FX1+(FX2-FX1)*X(2)*.5*PAI
      RETURN
    7 IN=INCR+(VALDA(INCR+1)+VALDA(INCR+2)+1)*6
      ID=IN+4
      CALL NEWT(0,RES1)
      ID=ID+6
      CALL NEWT(0,RES2)
      RETURN
C*********************************************************
    8 CALL DIAGN (5,SD,P)
           P=221.2E+05
C*********************************************************
      RETURN
      END
	    FUNCTION PHSAT(HNOW)
	    COMMON/INIT/II,KK,KLIM,I0,S(5)
	    COMMON/TABL/PRT(33),PAP(32),IC(32),IOFFSET(32)
	    COMMON/LECT/VALDA(129690)
C
C  ricerca regione (test su HWS(ultima isobara))
C				=1a isobara regione superiore
	    IA=1
	    IF=KK-1		! ultima regione sottocritica
	    DO 10 IREG=IA,IF
	    IP=IC(IREG)*6	! occupazione di una isobara
	    IF(HNOW.LE.VALDA(IOFFSET(IREG+1)-IP+2)) GOTO 20
  10	CONTINUE
	    GOTO 50		! fine regioni sottocritiche
C
C  ricerca isobara 	! dalla seconda all'ultima
C
  20	IF((IREG.EQ.1).AND.(HNOW.LT.VALDA(2))) GOTO 50	! errore
	    IS1=1
  30	IS1=IS1+1
      IF(IS1.GT.II) STOP '.....STOP in PHSAT ?????'   ! assurdo
	    IF(HNOW.GT.VALDA(IOFFSET(IREG)+IP*(IS1-1)+2)) GOTO 30
	    IS0=IS1-1			! n. isobara di P0
	    IF(IS1.EQ.II) IS0=IS0-1		! limite superiore di regione
C
	    PASP=PAP(IREG)			! passo in pressione
	    P0=PRT(IREG)+(IS0-1)*PASP
	    IP0=IOFFSET(IREG)+IP*(IS0-1)+2	! puntatore ad HWS(P0)
	    HW0=VALDA(IP0)
	    HW1=VALDA(IP0+IP)
	    HW2=VALDA(IP0+IP+IP)
C
C  inversione interpolazione in P
C
	    AA=HW2-2.*HW1+HW0
	    BB=-AA+2.*HW1-2.*HW0
	    CC=-2.*(HNOW-HW0)
	    CSI=-2.*CC/(BB+SIGN(1.,BB)*SQRT(BB*BB-4.*AA*CC))
C
	    PHSAT=P0+CSI*PASP
C
	    RETURN
C
  50	WRITE(6,*) '.....H = ',HNOW,' non esiste PSAT(H)'
	    STOP       '.....STOP in PHSAT'
	    END
      SUBROUTINE DROPH(P,NCAL,RES1,RES2,RES3)
      COMMON/CONT/PD,SD
      COMMON/INTE/ID,IP,X(2),Y(2),INCR,PAE,PAI
      COMMON/LECT/VALDA(129690)
      DIMENSION DRP(3),DRH(3),RO(3)
      IF(P.GT.221.2E5) GO TO 8
      IF(P-PD) 1,2,1
C*********************************************************
   1  IF(P.LT.610.) THEN
           CALL DIAGN (3,SD,P)
           P=610.
           ENDIF
C*********************************************************
      PD=P
      SD=-.1
      CALL TABLE
   2  IF(NCAL-1) 3,3,6
C
   3  IN=INCR+6
      DO 4 I=1,3
      RO(I)=VALDA(IN+IP*(I-1)+1)
      T=VALDA(IN+IP*(I-1)+2)
      A2=VALDA(IN+IP*(I-1)+4)
      B=VALDA(IN+IP*(I-1)+5)
      DRH(I)=B/T
      DRP(I)=1./A2-DRH(I)/RO(I)
   4  CONTINUE
C
   5  FX1=(DRP(2)-DRP(1))*X(1)*PAI
      FX2=(DRP(3)-DRP(2))*X(1)*PAI
      RES1=DRP(1)+FX1+(FX2-FX1)*X(2)*.5*PAI
      FX1=(DRH(2)-DRH(1))*X(1)*PAI
      FX2=(DRH(3)-DRH(2))*X(1)*PAI
      RES2=DRH(1)+FX1+(FX2-FX1)*X(2)*.5*PAI
      FX1=(RO(2)-RO(1))*X(1)*PAI
      FX2=(RO(3)-RO(2))*X(1)*PAI
      RES3=RO(1)+FX1+(FX2-FX1)*X(2)*.5*PAI
      RETURN
C
   6  IN=INCR+(VALDA(INCR+1)+VALDA(INCR+2)+1)*6
      DO 7 I=1,3
      VW=VALDA(IN+IP*(I-1)+1)
      T =VALDA(IN+IP*(I-1)+2)
      AW=VALDA(IN+IP*(I-1)+4)
      BW=VALDA(IN+IP*(I-1)+5)
      RO(I)=1./VW
      DRH(I)=BW/(VW*VW*T)
      DRP(I)=AW/(VW*VW)-DRH(I)*VW
   7  CONTINUE
      GOTO 5
C
C*********************************************************
   8  CALL DIAGN (5,SD,P)
      P=221.2E+05
C*********************************************************
      RETURN
      END
C**********************************************************************
      FUNCTION PSATEV(T,ID)
C
C
C     CALCOLA LA PRESSIONE DI SATURAZIONE   
C
C     DATA LA TEMPERATURA T
C
C  ID = 1 H2O
C  ID = 2 AMMONIACA
C  ID = 6 FREON
C
C      COMMON /PT1/ P(97,6)
C
      PSATEV = ((T-273.15)/100.)**4. *100000.
C   NEWTON
   8  CALL SATUR(PSATEV,7,TSAT,TVAP,ID)
      DP = 4. *(T-273.15)**3. *1.E-3 *(T-TSAT)
      PSATEV = PSATEV + DP
C      IF (PSATEV .LT. P(1,ID)) PSATEV = P(1,ID)
      IF(ABS(DP/PSATEV).GT.1.E-4)GOTO 8
      RETURN
      END
C**********************************************************************
      FUNCTION STEV(P,T,Y,N)
C
C     CALCOLA L' ENTROPIA   DATI:
C
C     PRESSIONE   P
C     TEMPERATURA T
C     TITOLO      Y
C

      if(P.GT.221.2E5)then
	      TSAT=647.
	      STEV = ENTROP(P,T,TSAT)
      else
C
	     CALL SATUR(P,7,TSAT,Z,N)
		   IF(Y.EQ.0.) THEN
C--- ACQUA SOTTORAFFREDDATA O SATURA
		     IF(T.GT.TSAT) T=TSAT
	       STEV = ENTROP(P,T+1.E-4,TSAT)
C      
       ELSE IF(Y.EQ.1.) THEN
C--- VAPORE SURRISCALDATO O SATURO
	       IF(T.LT.TSAT) T=TSAT
	       STEV = ENTROP(P,T+1.E-4,TSAT)
C
        ELSE
C--- MISCELA
         CALL SATUR(P,1,SA,SV,N)
	       STEV =  SA*(1.-Y)+SV*Y
	      ENDIF
C
      endif
      
      RETURN
      END      
C**********************************************************************
      FUNCTION ENTROP(P,T0,TSAT)
C
C     CALCOLA :
C              ENTROPIA =ENTROP
C
C     DATI : PRESSIONE =P
C            TEMPERATURA=T0
C            TEMP. DI SATURAZIONE(P) =TSAT
C
      NTENT=1
      FASE=0.
      IF(P.GT.221.2E5) GOTO 3
      CALL SATUR(P,1,SW,SV,1)
      T=TSAT
      IF(T0-T) 1,2,2
   1  S=SW
      GOTO 4
   2  S=SV
      FASE=1.
      GOTO 4
   3  S=4442.9
C
  10  T=TEV(P,S,1)
   4  CP=CPEV(P,S,FASE,.5,1)
      DS=CP*(T0-T)/T0
      DELS=AMIN1(ABS(DS),1000.)
      S=S+DELS*SIGN(1.,DS)
      IF(DELS.LE.0.01) GOTO 20
      NTENT=NTENT+1
      IF(NTENT.LT.20) GOTO 10
C
      WRITE(6,99) DELS
  99  FORMAT(1X,'....ERRORE : ENTROP NON CONVERGE  DELTAS = ',E15.8)
  20  ENTROP=S
      RETURN
      END
      SUBROUTINE DIAGN(N,SH,P)
C**********************************************************************
C     MODIFICHE ALLA VERSIONE ORIGINALE PER EVITARE LO STOP
C     QUANDO SI VA FUORI DAI LIMITI DELLE TAVOLE .
C**********************************************************************
C
C  STAMPA P,H O S E LA TRACCIA DELLE CHIAMATE
C
C       ILEGO = INDICATORE DEL PROGRAMMA CHIAMANTE LE TAVOLE
C             =1 LEGO E` IL PROGRAMMA CHIAMANTE
C             =0 NON E` IL LEGO IL PROGRAMMA CHIAMANTE
C
C       ICNTMX = CONTATORE STABILITO DAL PROGRAMMA CHIAMANTE LE TAVOLE
C                SUPERATO IL QUALE VIENE DECRETATO LO STOP .
C
C
C       
      COMMON/LGTV01/LGTEMP,ILEGO,ICOUNT,ICNTMX
      COMMON/LGTV02/LGMODU,LGBLOC
      CHARACTER*8 LGMODU,LGBLOC
      REAL LGTEMP
C
C
      WRITE(6,*)' ****************************************************'
      IF(ILEGO.EQ.1) THEN
        WRITE(6,*)' TAVOLE ACQUA/VAPORE FUORI LIMITI. TEMPO ',LGTEMP 
        WRITE(6,*)' NEL BLOCCO ',LGBLOC,' ( MODULO ',LGMODU,' )'
      ELSE
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
C     IF(ILEGO.EQ.1) CALL LIB$SIGNAL(%VAL(0))
      ICOUNT=ICOUNT+1
      IF(ICOUNT.GT.ICNTMX) STOP
      RETURN
  101 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,2HS=,E12.5,7H J/KG/K,5X,6(1H*))
  102 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,34HNON ESISTONO VALORI DI SATURAZIONE,5X,3(1H*))
  103 FORMAT(1H0,6(1H*),6HERRORE,5X,2HP=,E12.5,4H BAR,
     &        2X,2HH=,E12.5,6H KJ/KG,5X,6(1H*))
      END
