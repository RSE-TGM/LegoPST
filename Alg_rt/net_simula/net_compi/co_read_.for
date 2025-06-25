C******************************************************************************
C modulo co_read_.for
C tipo 
C release 5.1
C data 11/7/95
C reserver @(#)co_read_.for	5.1
C******************************************************************************
C-----------------------------------------------------------------
C
      SUBROUTINE READ_F04_()

            include '../../../lego_big/lib/lg_parameter.fh'
C  GUAG2025 
C        INCLUDE '[--.include]PARAMETER.F'
C
        DIMENSION
     &  NOSUB(N002),      NOBLC(N002,2),      NUSTA(N002),
     &  NUSCI(N002),      NINGR(N002),        ISLB(N002),
     &  VAR(N005,2),      IPS(M004),          SIVAR(2,N003),
     &  IPI(M005),        VARI(2,N004)
C
        DIMENSION
     &  IP(M003),         IPVRS(N005),        IOUT_SIVAR(N003),
     &  IOUT_VARI(N004),  NOSLX(N001)
C
        INTEGER LUN_PATH

        CHARACTER*90   PATH
        CHARACTER*80   NOM_BLOC(N002)
        CHARACTER*100  NOM_SIVAR(N003)
        CHARACTER*100  NOM_VARI(N004)
C
      COMMON/F04_/NBL,NEQAL,NVART,NEQSIS,NU,NBL1,
     &           NOM_BLOC,IP,IPVRS,NOM_SIVAR,IOUT_SIVAR,
     &           NOM_VARI,IOUT_VARI,NDATI,LUN_PATH,PATH
C
C lettura dal file F04.DAT di ogni modello
      OPEN(UNIT=4, FILE=PATH(1:LUN_PATH), STATUS='OLD',
     &     FORM='UNFORMATTED',ERR=999)

      WRITE(*,*)PATH(1:LUN_PATH)


      READ (04) NBL,
     &          NEQAL,
     &          NBL1,
     &          NVART,
     &          NEQSIS,
     &          NEQS1,
     &          NPVRT,
     &          NU,
     &          NU1,
     &          NVRI
C
      NDERIV=NEQSIS-NEQAL
C
      READ (04) ISSISX,
     &          NBTRIX,
     &          NSTX,
     &          SIGLA1X,
     &          SIGLA2X,
     &          (NOSLX(I),I=1,NSTX)
C
      READ (04) (NOSUB(I),
     &           NOBLC(I,1),
     &           NOBLC(I,2),
     &           NUSTA(I),
     &           NUSCI(I),
     &           NINGR(I),
     &           ISLB(I),
     &           NOM_BLOC(I),I=1,NBL)
C
      READ (04) (IP(I),I=1,NBL1),
     &          ((VAR(I,J),J=1,2),
     &          IPVRS(I),I=1,NVART)
C
      READ (04) (IPS(I),I=1,NEQS1),
     &          ((SIVAR(I,J),J=1,2),
     &          NOM_SIVAR(I),
     &          IOUT_SIVAR(I),I=1,NEQSIS)
C
      READ (04) (IPI(I),I=1,NU1),
     &          ((VARI(I,J),J=1,2),
     &          NOM_VARI(I),
     &          IOUT_VARI(I),I=1,NU)
C
      READ (04)
C
      READ (04)
C
      READ (04) NDATI
C
      CLOSE(4)
      RETURN
C
999   WRITE(*,*)' ERRORE NELL''APERTURA DEL FILE ',PATH
      STOP
      END
C
C    fine della parte necessariamente codificata in fortran.
C
