

C*********************************************************************
C Fortran PreCompile: cloc.pf
C Subsystem: 1
C Description:
C %created_by: lopez %
C %date_created: Mon Oct 28 11:04:58 2002 %
C
C**********************************************************************


C
C Procedura contenete la variabile per l identificazione della versione
C
      BLOCK DATA BDD_cloc_pf
      CHARACTER*80 RepoID
      COMMON /CM_cloc_pf / RepoID
      DATA RepoID/'@(#)1,pfsrc,cloc.pf,2'/
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

       SUBROUTINE CLOC ( TEMPO )
C La subroutine rida TEMPO (reale) in secondi
C come riferimento cronologico assoluto.
C A seconda del sistema operativa utilizzato l utente
C LEGO deve scommentare lo spezzone di modulo relativo.
C Inoltre deve commentare le prime due linee che generano
C il messaggio di errore e interrompono il programma
C
C TEMPO = 0.
C *************** ERRORE *********************
C WRITE(*,*)'Errore: occorre editare CLOC.F'
C STOP
C ******************* VMS ********************
C La subroutine CPUTIM e'' una routine in assembler
C che rida in ITEMPO un intero come riferimento
C cronologico assoluto in centesimi di secondo.




C ********************************************
C **************** ULTRIX ********************
C La funzione ETIME (elapsed time) rida il tempo
C trascorso dall inizio del processo
C e in T (array di due elementi) il
C 'tempo utente' e il 'tempo di sistema'.
C



C ********************************************
C ***************** AIX **********************
C La funzione MCLOCK (senza parametri) rida
C un intero che e'' il tempo trascorso in
C centesimi di secondo.
C
C ********************************************
C
       RETURN
       END
C$$$$_____________________ ASSEMBLER VMS DI CPUTIME ____________________
C$$$$
C$$$$ QUESTA ROUTINE VA COPIATA IN UN FILE A PARTE DI TIPO "MAR"
C$$$$ E DOPO AVER TOLTO I CARATTERI C$$$$ VA COMPILATA COL
C$$$$ COMANDO MACRO E CARICATA IN LIBRERIA
C$$$$
C$$$$
C$$$$%%%%% M %%%%% CPUTIM.MAR
C$$$$;
C$$$$; calcola il tempo di CPU utilizzato dal processo chiamante
C$$$$; chiamata FORTRAN : CALL CPUTIM (ITIME)
C$$$$; ITIME e'' il tempo di CPU in centesimi di secondo.
C$$$$;
C$$$$ .TITLE CPUTIM
C$$$$ LUNGO: .LONG 0
C$$$$ TEMPO: .LONG 0
C$$$$ LISTA: .WORD 4
C$$$$ .WORD ^X407
C$$$$ .LONG TEMPO
C$$$$ .LONG LUNGO
C$$$$ .LONG 0
C$$$$;
C$$$$ .ENTRY CPUTIM, 0
C$$$$ $GETJPI_S ITMLST=LISTA
C$$$$ MOVL TEMPO,@4(AP)
C$$$$ RET
C$$$$ .END
C$$$$_____________________ ASSEMBLER VMS DI CPUTIME ____________________
C
