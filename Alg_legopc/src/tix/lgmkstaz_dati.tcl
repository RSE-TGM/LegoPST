# lgmkstaz_dati.tcl - dati statici del formato r01.dat: le parole chiave, la
# grammatica delle righe di ogni oggetto elementare, e il catalogo dei 54 tipi
# di stazione "nuovi" con la loro composizione.
#
# QUESTO FILE NON HA CODICE DI LETTURA/SCRITTURA: e' solo la tabella, cosi' la
# si puo' leggere e verificare da sola, e lgmkstaz_leggi.tcl / lgmkstaz_scrivi.tcl
# ci lavorano sopra senza duplicarla. E' Tcl puro, non serve Tk: si carica e si
# testa anche con tclsh, senza display.
#
# FONTE DI VERITA'. Non e' stato scritto guardando la documentazione (che ha
# almeno una lacuna reale, vedi sotto): e' la trascrizione delle regole scritte
# nei sorgenti C di compstaz, che sono il compilatore vero:
#   AlgLib/libinclude/xstaz.h      - le parole chiave (s_colore, s_inp, ...)
#   AlgLib/libinclude/newstaz.h    - stipi_oggetti[], scolori_oggetti[],
#                                     stipo_perturb[], new_staz[] (i 54 tipi)
#   Alg_rt/grafica/compstaz/c_*.c  - un lettore per ogni oggetto elementare:
#                                     l'ordine delle sue letture (legge_riga) E'
#                                     la grammatica.
#
# UNA LACUNA TROVATA NELLA DOCUMENTAZIONE (Alg_rt/grafica/xstaz/HOWTO_faceplate.md):
# la riga OUTPUT ha un QUARTO campo numerico facoltativo, il valore della
# perturbazione (step/impulso), che HOWTO_faceplate.md non menziona:
# "OUTPUT <variabile> <modello> <modo> [<valore>]" - letto da cnewstaz.c::get_valore,
# default 1.0 se assente o non numerico. Da correggere anche li'.
#
# TRE TRAPPOLE DEL FORMATO, anch'esse non documentate, che riguardano chi
# scrive il file (lgmkstaz_scrivi.tcl le rispetta, qui sono solo annotate):
#   - una riga che comincia con uno spazio e' un errore FATALE
#     (co_legge.c: "il file S01 contiene una riga che inizia con blank" - il
#     messaggio nomina S01 per un refuso storico, ma il file letto e' r01.dat);
#   - le righe sono lette con fgets(riga,80,...): oltre 79 caratteri utili la
#     riga si tronca IN SILENZIO, senza errore;
#   - fine riga deve essere LF: lungh() toglie un solo carattere assumendo sia
#     '\n'; un file CRLF lascerebbe un '\r' appiccicato all'ultimo campo di
#     ogni riga (invisibile finche' non spacca un confronto o un numero).

namespace eval ::lgmkstaz {}

# ---------------------------------------------------------------------------
# Parole chiave letterali (da xstaz.h, macro s_*), colori e modi di
# perturbazione (da newstaz.h, scolori_oggetti[]/stipo_perturb[]).
# ---------------------------------------------------------------------------

set ::lgmkstaz::kw(colore)        COLORE
set ::lgmkstaz::kw(etichetta)     ETICHETTA
set ::lgmkstaz::kw(input)         INPUT
set ::lgmkstaz::kw(input_err)     INPUT_ERR
set ::lgmkstaz::kw(input_blink)   INPUT_BLINK
set ::lgmkstaz::kw(output)        OUTPUT
set ::lgmkstaz::kw(scalamento)    SCALAMENTO
set ::lgmkstaz::kw(scalamento_err) SCALAMENTO_ERR
set ::lgmkstaz::kw(minmax)        MINMAX
set ::lgmkstaz::kw(minmax_err)    MINMAX_ERR
set ::lgmkstaz::kw(offset)        OFFSET
set ::lgmkstaz::kw(inibizione)    INIBIZIONE
set ::lgmkstaz::kw(not)           NOT

# I colori ammessi. L'ordine conta poco qui (in compstaz e' l'indice salvato
# nel binario r02.dat), ma lo teniamo identico a scolori_oggetti[] per chiarezza.
set ::lgmkstaz::colori {NERO BIANCO GIALLO VERDE ROSSO GRIGIO BLU}

# I modi di perturbazione di una riga OUTPUT.
set ::lgmkstaz::perturbazioni {STEP IMPULSO NEGAZIONE UP_DOWN}

# ---------------------------------------------------------------------------
# Grammatica dei 12 oggetti elementari + le 2 varianti che dipendono dal
# sottotipo (INDICATORE normale/con errore) o che non sono un tipo di oggetto
# a se' ma una coppia di ETICHETTA (SELETTORE).
#
# Ogni "campo" della lista e' un nome di CAMPO GENERICO (non una parola
# chiave): lgmkstaz_leggi.tcl/lgmkstaz_scrivi.tcl sanno, per ogni nome di
# campo, quale riga leggere/scrivere e con quanti argomenti (vedi
# ::lgmkstaz::campo_leggi/campo_scrivi in lgmkstaz_leggi.tcl). Tenerli
# distinti da subito evita l'errore piu' insidioso del formato: una riga
# INPUT dentro un DISPLAY/INDICATORE/SELETTORE/SET_VALORE/INDICATORE_SINCRO
# ha 3 token (nessun NOT); la STESSA riga INPUT dentro un LED/LAMPADA/LUCE/
# PULS_LUCE ne ha 4 (il NOT finale e' ammesso). Sono davvero due grammatiche
# diverse con lo stesso nome di riga - "input" e "input_neg" qui sotto.
#
# La parola chiave scritta nel file per il BLOCCO di un oggetto (la riga che
# lo apre, es. "LED") e' quella di stipi_oggetti[]: coincide col nome della
# chiave qui sotto per tutti tranne STRINGA (blocco "STRINGA", non
# "STRINGA_DESCR") e le due varianti di INDICATORE, che scrivono entrambe
# "INDICATORE" (il sottotipo non e' nel file: lo decide il TIPO di stazione,
# vedi il catalogo piu' sotto).

array set ::lgmkstaz::grammatica {
    LED               {colore etichetta input_neg input_blink_neg}
    PULS_LUCE         {colore output input_neg input_blink_neg}
    PULSANTE          {colore output}
    LAMPADA           {colore input_neg input_blink_neg}
    SELETTORE         {etichetta etichetta output input}
    INDICATORE        {scalamento minmax offset input}
    INDICATORE_ERR    {scalamento minmax offset scalamento_err minmax_err input input_err}
    STRINGA           {etichetta}
    DISPLAY           {input}
    LUCE              {colore input_neg input_blink_neg}
    TASTO             {colore output}
    SET_VALORE        {etichetta output output input scalamento offset inibizione}
    DISPLAY_SCALATO   {input scalamento offset}
    INDICATORE_SINCRO {input input input input input input output output}
}

# La parola chiave di blocco scritta nel file, per ogni chiave della
# grammatica sopra. INDICATORE_ERR scrive "INDICATORE" come INDICATORE: la
# differenza sta SOLO nelle righe che porta, decisa dal tipo di stazione.
array set ::lgmkstaz::blocco_kw {
    LED               LED
    PULS_LUCE         PULS_LUCE
    PULSANTE          PULSANTE
    LAMPADA           LAMPADA
    SELETTORE         SELETTORE
    INDICATORE        INDICATORE
    INDICATORE_ERR    INDICATORE
    STRINGA           STRINGA
    DISPLAY           DISPLAY
    LUCE              LUCE
    TASTO             TASTO
    SET_VALORE        SET_VALORE
    DISPLAY_SCALATO   DISPLAY_SCALATO
    INDICATORE_SINCRO INDICATORE_SINCRO
}

# ---------------------------------------------------------------------------
# Catalogo dei 54 tipi di stazione "nuovi" (new_staz[] in newstaz.h): nome,
# celle L x A, e la sequenza ESATTA degli oggetti che compongono il tipo -
# nell'ordine in cui STANNO nel r01.dat, obbligatorio.
#
# Fonte: new_staz[] in AlgLib/libinclude/newstaz.h, letto riga per riga (il
# nome del tipo, poi num_oggetti/larg/altezza, poi un OGGETTO per riga). La
# tabella dell'help (Alg_rt/grafica/xstaz/HOWTO_faceplate.md, cap. 7) e' la
# stessa cosa scritta per un lettore umano: qui e' la stessa cosa in una
# forma che lgmkstaz puo' eseguire.
#
# NON include le 13 stazioni "storiche" (tipi_old_staz[]: SA1 SP1 SPD ID1 BR1
# TR1 MR1 LU1 AM1 AM2 AM3 AMD SD1) - decisione dell'utente: lgmkstaz le legge
# e le sposta come blocchi opachi (vedi lgmkstaz_leggi.tcl), non le edita.
#
# SINCRONO ha 4 INDICATORE (non 5): num_oggetti=7 in new_staz[] (newstaz.h) -
# 2 STRINGA + 4 INDICATORE + 1 INDICATORE_SINCRO. Il test di round-trip
# (2026-09-22) lo ha scovato: la trascrizione a mano di questa tabella ne
# aveva messo uno di troppo, e la sua unica stazione SINCRONO nel catalogo di
# consultazione non si rileggeva piu'. HOWTO_faceplate.md (cap. 7), dove la
# tabella era stata letta, aveva gia' il numero giusto: l'errore era solo qui.
# ---------------------------------------------------------------------------

array set ::lgmkstaz::catalogo {
    LEDS2     {2 1 {LED LED}}
    LEDS2DES  {2 1 {STRINGA LED LED}}
    LEDS2L    {2 1 {STRINGA LED LED}}
    LEDR2     {2 1 {STRINGA LED LED}}
    LEDR3     {2 1 {LED LED LED}}
    LEDS3     {2 1 {STRINGA LED LED LED}}
    LEDR4     {2 1 {LED LED LED LED}}
    LEDS4     {2 1 {LED LED LED LED}}
    LEDR4BIS  {2 1 {LED LED LED LED}}
    LEDR4DES  {2 1 {STRINGA STRINGA LED LED LED LED}}
    LEDS6     {2 1 {LED LED LED LED LED LED}}
    LEDR6     {2 1 {LED LED LED LED LED LED}}
    LEDR6BIS  {2 1 {LED LED LED LED LED LED}}
    SELET_A   {2 1 {STRINGA SELETTORE}}
    SELET_B   {2 1 {STRINGA SELETTORE}}
    IBARRA1   {2 1 {STRINGA INDICATORE}}
    IBARRA2   {2 1 {STRINGA INDICATORE}}
    IAGO      {2 2 {STRINGA INDICATORE}}
    AGOSETV   {2 2 {SET_VALORE INDICATORE}}
    IAGOERR   {2 2 {STRINGA INDICATORE_ERR}}
    AGERSETV  {2 2 {SET_VALORE INDICATORE_ERR}}
    DISPLAY   {2 1 {STRINGA DISPLAY}}
    DISPSET   {2 1 {SET_VALORE DISPLAY}}
    DISSETSC  {2 1 {SET_VALORE DISPLAY_SCALATO}}
    LUCE      {2 1 {STRINGA LUCE}}
    TASTO     {2 1 {STRINGA TASTO}}
    TASTOBIS  {2 2 {STRINGA TASTO}}
    P3L3      {2 1 {STRINGA LED LED LED PULSANTE PULSANTE PULSANTE}}
    P2L3      {2 1 {STRINGA LED LED LED PULSANTE PULSANTE}}
    P2L2      {2 1 {STRINGA LED LED PULSANTE PULSANTE}}
    P1L3      {2 1 {STRINGA LED LED LED PULSANTE}}
    P1L2      {2 1 {STRINGA LED LED PULSANTE}}
    P1L1      {2 1 {STRINGA LED PULSANTE}}
    P1L0      {2 1 {STRINGA PULSANTE}}
    P2L0      {2 1 {STRINGA PULSANTE PULSANTE}}
    P2L0BIS   {2 1 {STRINGA STRINGA STRINGA PULSANTE PULSANTE}}
    PL3L1     {2 1 {STRINGA LED PULS_LUCE PULS_LUCE PULS_LUCE}}
    PL3L3     {2 1 {STRINGA LED LED LED PULS_LUCE PULS_LUCE PULS_LUCE}}
    PL3L4     {2 1 {STRINGA LED LED LED LED PULS_LUCE PULS_LUCE PULS_LUCE}}
    PL2L3     {2 1 {STRINGA LED LED LED PULS_LUCE PULS_LUCE}}
    PL2L2     {2 1 {STRINGA LED LED PULS_LUCE PULS_LUCE}}
    PL2L0     {2 1 {STRINGA PULS_LUCE PULS_LUCE}}
    PL1       {2 1 {STRINGA PULS_LUCE}}
    PL1BIS    {2 1 {STRINGA STRINGA PULS_LUCE}}
    PL1P1     {2 1 {STRINGA STRINGA STRINGA PULS_LUCE PULSANTE}}
    LAMP1     {2 1 {STRINGA LAMPADA}}
    LAMP2     {2 1 {STRINGA STRINGA LAMPADA LAMPADA}}
    LAMP1L3   {2 1 {STRINGA LED LED LED LAMPADA}}
    MIXER     {2 1 {STRINGA LED LED LED PULS_LUCE LAMPADA PULS_LUCE}}
    MIXER1    {2 1 {STRINGA PULS_LUCE LAMPADA PULS_LUCE}}
    TESTO     {8 1 {STRINGA}}
    TESTOBIS  {2 1 {STRINGA}}
    DISPSCAL  {2 1 {STRINGA DISPLAY_SCALATO}}
    SINCRONO  {12 4 {STRINGA STRINGA INDICATORE INDICATORE INDICATORE INDICATORE INDICATORE_SINCRO}}
}

# Le 13 stazioni storiche (tipi_old_staz[] in newstaz.h): lgmkstaz le
# riconosce per nome ma non ne conosce la grammatica (ognuna ha un lettore C
# a se', pre-1995, mai portato a una tabella dati come new_staz[]). Servono
# solo per distinguere "tipo storico, blocco opaco" da "tipo sconosciuto",
# che nel messaggio di lgmkstaz sono due cose diverse.
set ::lgmkstaz::tipi_storici {SA1 SP1 SPD ID1 BR1 TR1 MR1 LU1 AM1 AM2 AM3 AMD SD1}

# Dimensione (celle L x A) delle 13 stazioni storiche - serve solo a
# disegnarle nel posto giusto sulla griglia, non a leggerne gli oggetti (che
# restano un blocco opaco). Tutte larghe 2 celle (ogni lettore C storico fa
# `posix1=ipx+2`); l'altezza varia da lettore a lettore (`posiy1=ipy+N`,
# controllato uno per uno in Alg_rt/grafica/compstaz/{sa1,sp1,spd,id1,br1,tr1,
# mr1,lu1,am1,am2,am3,amd,sd1}_c.c - non e' in nessuna tabella dati, a
# differenza delle stazioni nuove).
array set ::lgmkstaz::dimensioni_storiche {
    SA1  {2 1}
    SP1  {2 3}
    SPD  {2 2}
    ID1  {2 1}
    BR1  {2 1}
    TR1  {2 1}
    MR1  {2 1}
    LU1  {2 1}
    AM1  {2 3}
    AM2  {2 3}
    AM3  {2 3}
    AMD  {2 2}
    SD1  {2 1}
}

# Dimensione di una cella in pixel (DIM_UNITSTAZ in xstaz.h): usata dalla
# vista a canvas (fase 1), non dal modello/parser/writer - resta qui perche'
# e' un dato del formato, non della GUI.
set ::lgmkstaz::dim_cella_px 62

# ---------------------------------------------------------------------------
# Limiti del formato (xstaz.h). MAX_PAG e MAX_STAZ li applica lgmkstaz da se':
# compstaz non controlla che il NUMERO scritto in una pagina stia dentro
# MAX_PAG, solo che non sia gia' usato - un NUMERO fuori range indicizza fuori
# dalla tabella interna (comportamento indefinito, non un errore pulito).
# Meglio rifiutarlo qui che scoprirlo li'.
# ---------------------------------------------------------------------------
set ::lgmkstaz::max_pagine        500;   # MAX_PAG
set ::lgmkstaz::max_stazioni      2000;  # MAX_STAZ
set ::lgmkstaz::max_oggetti_pagina 200;  # MAX_OGG (stazioni per pagina)
set ::lgmkstaz::lun_nome_pagina   8;     # LUN_NOM_PAG
set ::lgmkstaz::lun_descrizione_pagina 49; # LUN_DES_PAG - 1 (spazio terminale)
set ::lgmkstaz::lun_etichetta     31;    # LUNG_ETICHETTA
