# Audit K&R — parametri `float` in conflitto con il prototipo

Bonifica sistematica di un difetto di porting presente in tutto il codebase.
**Nome della task: `audit K&R`.**

> **ESEGUITA il 2026-08-02 — conflitti azzerati.** 13 funzioni corrette in 10 file,
> 2 segnalazioni erano falsi positivi (vedi *Esito*). Lo scanner ora riporta
> `CON prototipo in conflitto : 0`. Questa pagina resta come riferimento: rilanciare
> `python3 util2025/kr_audit.py` dopo ogni import di codice legacy.

## Il difetto

In una definizione in stile K&R il parametro `float` subisce la **promozione
automatica a `double`**: il tipo effettivo della funzione è `void f(double)`.

```c
void formatta(str, fval)      /* definizione K&R */
char *str;
float fval;                   /* -> in realtà riceve un double */
{ ... }
```

Se però esiste **anche un prototipo** che dichiara `float`:

```c
static void formatta(char*, float);   /* il chiamante passa un float (32 bit) */
```

i due lati non concordano: il chiamante scrive 32 bit, il chiamato ne legge 64.
È comportamento indefinito e in pratica **il valore arriva corrotto, quasi sempre
0**.

Con i compilatori dell'epoca (senza prototipi) entrambi i lati promuovevano a
`double` e il codice funzionava: **il difetto emerge solo ricompilando con un
toolchain moderno**. Non produce warning né errori — il programma gira e mostra
numeri sbagliati.

### Sintomo tipico

Valori **a zero senza motivo apparente**, con tutti i dati corretti in memoria.
È il primo sospetto da verificare quando i dati ci sono ma a video sono 0.

## Caso già risolto (2026-07-31)

`grafics` mostrava tutte le variabili a 0 dopo un drift. Dati, min/max e
conversione di unità erano **tutti corretti in memoria**: sbagliava solo
`formatta()`, che aveva esattamente questo conflitto. Corrette allora:

| File | Funzione |
|---|---|
| `Alg_rt/grafica/grafics/grafics.c` | `formatta()`, `prep_draw()` |
| `Alg_rt/grafica/graphics/graphics.c` | `prep_str_tim()` |

## Esito (2026-08-02)

```
prima:  candidati 125  |  conflitti 15
dopo :  candidati  91  |  conflitti  0
```

### Corrette (13 funzioni, 10 file)

| File | Funzioni |
|---|---|
| `Alg_mmi/lib/Xl/SourceGrafica/funzioni.c` | `prep_str_timGR`, `prep_draw`, `formatta` |
| `Alg_mmi/lib/Xl/SourceGrafica/grsfio.c` | `read_multi` |
| `legocad/lib/liblegocad/f14.c` | `pr_float`, `spr_float` |
| `Alg_rt/net_simula/viewval/viewshr.c` | `viewshr` |
| `Alg_rt/net_simula/net_monit/monit_frem.c` | `perturba_riga_sommario_fr` |
| `Alg_rt/net_simula/net_monit/monit_malf.c` | `perturba_riga_sommario_mf` |
| `Alg_rt/net_simula/mandb/acqmandb.c` | `write_sh` |
| `AlgLib/libsim/nega.c` | `nega` |
| `scada/libut/rwdbal.c` | `iodb` (parametri `short`) |
| `scada/scada/aggcfg/taggcfg.c` | `InvSlave` (parametri `short`) |

Piu' le tre gia' corrette il 2026-07-31 in `grafics.c`/`graphics.c`.

### NON toccate: 2 falsi positivi

- **`Alg_rt/net_simula/new_monit/archiveSess.c::recoveryRangeF22`** — i parametri
  sono `float *`: **i puntatori non subiscono promozione**, nessun conflitto.
- **`Alg_rt/net_simula/dataserver/viewshr.c::viewshr`** — il "prototipo" trovato
  era dentro un **blocco di commento**; il chiamante reale (`main_DataServer.c:73`)
  dichiara `int viewshr();` senza tipi, quindi promuove a `double` coerentemente
  col K&R. Convertire la definizione qui **introdurrebbe** il bug.

Lo scanner e' stato affinato per non segnalare piu' queste due classi (ignora i
commenti; richiede almeno un parametro `float`/`short` **non puntatore**).

### Verifica build

Ricompilati senza errori: `AlgLib/libsim`, `legocad/lib/liblegocad`,
`Alg_rt/net_simula/{viewval,mandb,net_monit}`, `Alg_mmi/lib/Xl/SourceGrafica`,
`scada/libut`, e `taggcfg.o` dentro `scada/scada`. In `scada/scada` il link finale
fallisce per `../lib/libUtil.a` mancante: **difetto preesistente**, non correlato
(nessuna occorrenza di `taggcfg`/`InvSlave` fra gli errori).

## Seconda famiglia: chiamate con il numero di argomenti sbagliato (2026-08-19)

L'audit di agosto ha coperto i **tipi** dei parametri. C'è però un secondo
difetto che nasce dalla stessa radice — una dichiarazione K&R non descrive i
parametri — e che l'audit sui `float` non intercetta: **chiamare una funzione con
meno argomenti di quanti ne ha**.

```c
extern void malf_proc();          /* dichiarazione K&R: nessuna informazione   */

void malf_proc(w, tag, reason)    /* definizione: tre parametri                */
Widget w;
int *tag;
XmListCallbackStruct *reason;
{
int widget_num = *tag;            /* dereferenza il secondo parametro          */
```

```c
malf_proc();                      /* chiamata senza argomenti: compila senza   */
                                  /* un solo warning                           */
```

`tag` contiene quello che capita di trovare nel registro, e la dereferenza è un
SIGSEGV. Con un prototipo vero il compilatore avrebbe rifiutato la chiamata.

**Caso reale**: `net_monit` moriva all'avvio, prima ancora di mostrare la
finestra. `monit.c` chiamava `malf_proc()` e `frem_proc()` senza argomenti per
pre-creare le finestre malfunzioni e funzioni remote; entrambe sono callback
Motif a tre parametri. Le due chiamate stanno sotto `#if defined MFFR`, mentre
gli stub vuoti `void malf_proc() {}` sono sotto `#ifndef MFFR`: la forma senza
argomenti era quella degli stub, ma finiva per essere applicata alle funzioni
vere. Risolto passando gli argomenti espliciti e togliendo dalle due funzioni
la riga `int widget_num = *tag;`, che era l'unico uso dei parametri e per giunta
assegnava una variabile mai letta.

**Come cercarlo**: non basta un grep, serve il compilatore. Trasformando le
dichiarazioni `extern void f();` in prototipi veri, ogni chiamata con arità
sbagliata diventa un errore di compilazione. Da fare per sottosistemi, partendo
da quelli con molte callback Motif dichiarate `()`.

## Procedura

1. **Rilevare**: `python3 util2025/kr_audit.py` (exit 1 se trova conflitti).
2. **Correggere** — convertire la *definizione* allo stile prototipo, mai togliere
   il prototipo:

   ```c
   /* da */                          /* a */
   void f(str, fval)                 void f(char *str, float fval)
   char *str;                        {
   float fval;                           ...
   {                                 }
   ```

   Aggiungere un commento breve che spieghi perché (vedi i tre casi già corretti).
3. **Ricompilare** il componente (`source .profile_legoroot`, poi
   `make -f Makefile.mk` nella sua directory).
4. **Verificare** che i valori a video/su file non siano più 0. Per i
   visualizzatori il controllo rapido è un breakpoint gdb sulla funzione corretta,
   confrontando il parametro ricevuto con quello passato.
5. Ripetere `kr_audit.py` finché **CON prototipo in conflitto : 0**.

## Note

- **Non toccare** i 91 candidati senza prototipo: sono K&R coerenti e funzionanti.
  Convertirli sarebbe un rischio inutile su codice numerico che gira.
- Correggere **un componente per volta**, ricompilando e verificando: sono binari
  di produzione.
- Lo stesso conflitto vale per `short` (anch'esso promosso, a `int`): lo scanner
  lo copre già.
- `grafics::prep_str_tim` è K&R **senza** prototipo: lasciata com'è di proposito.
