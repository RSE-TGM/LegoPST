# FILE DI CONFIGURAZIONE PER PARAMETRI SIMULATORE
# SIntassi:
# *Simulator.VARIABILE_DA_CONFIGURARE: ESPRESSIONE
#
# Numero di snapshot salvabili
*Simulator.MAX_SNAP_SHOT: 60
# Numero di backtrack salvabili prima di perdere
# il backtrack piu' vecchio
# (file circolare)  (salvataggio backtrack: uno ogni 2 minuti)
*Simulator.MAX_BACK_TRACK: 30
# Massimo numero di istanti temporali nel file circolare
# di archivio
*Simulator.MAX_CAMPIONI: 14400
# Massimo numero di variabili salvabili per ogni istante di tempo
# nel file di archivio
*Simulator.NUM_VAR: 10000
# Massimo numero di perturbazioni contemporaneamente attive
# (e' anche usato come massimo numero delle perturbazioni
#  in attesa di attivazione per delay)
*Simulator.MAX_PERTUR: 50
# Numero di aree spare di 4 Kb salvate in ogni sanpshot
# o backtrack
*Simulator.SPARE_SNAP: 1
# Parametro che elimina tutte le perturbazioni lette
# dai backtrack e snapshot in caso di caricamento
# di condizione iniziale (escluso sempre caricamento
# per replay): 1 eliminazione, 0 mantenimento.
*Simulator.PERT_CLEAR: 0
