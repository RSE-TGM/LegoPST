C*********************************************************************
C       Fortran Source:             provatav.f
C       Subsystem:              1
C       Description:
C       %created_by:    lomgr %
C       %date_created:  Wed Jun  7 12:33:15 2006 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l'identificazione della versione
C
      BLOCK DATA BDD_provatav_f
      CHARACTER*80  RepoID
      COMMON /CM_provatav_f / RepoID
      DATA RepoID/'@(#)1,fsrc,provatav.f,2'/
      END
C**********************************************************************
      PROGRAM PROVATAV
      CALL INITSM
  10  WRITE(6,*) ' P (bar),S (J/kg/K) ? '
C Valori tipici P=1 bar (100000 Pa) S=1000 J/kg/K --> 73 C
      READ(5,*,END=20) P,S
      P=P*1.E05
      T=TEV(P,S,1)
      T=T-273.15
      WRITE(6,*)'P=',P,'Pa  S=',S,'J/kg/K',' T(P,S)= ',T
      GOTO 10
  20  STOP
      END

