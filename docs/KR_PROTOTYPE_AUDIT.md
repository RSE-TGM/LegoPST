# Audit K&R — parametri `float` in conflitto con il prototipo

Piano per la bonifica sistematica di un difetto di porting presente in tutto il
codebase. **Nome della task: `audit K&R`.**

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

## Stato attuale (misurato)

```
$ python3 util2025/kr_audit.py
candidati K&R con parametri float/short : 125
CON prototipo in conflitto (bug reali)  :  15
```

Solo i **15 con prototipo in conflitto** sono bug. Gli altri 110 sono K&R
coerenti (nessun prototipo): funzionano, **non vanno toccati**.

### I 15 da correggere

| File | Riga | Funzione | Priorità |
|---|---|---|---|
| `Alg_mmi/lib/Xl/SourceGrafica/funzioni.c` | 971, 1026, 2415 | `prep_str_timGR`, `prep_draw`, `formatta` | **alta** |
| `Alg_mmi/lib/Xl/SourceGrafica/grsfio.c` | 301 | `read_multi` | **alta** |
| `legocad/lib/liblegocad/f14.c` | 259, 629 | `pr_float`, `spr_float` | **alta** |
| `Alg_rt/net_simula/dataserver/viewshr.c` | 104 | `viewshr` | media |
| `Alg_rt/net_simula/viewval/viewshr.c` | 87 | `viewshr` | media |
| `Alg_rt/net_simula/new_monit/archiveSess.c` | 359 | `recoveryRangeF22` | media |
| `Alg_rt/net_simula/net_monit/monit_frem.c` | 947 | `perturba_riga_sommario_fr` | media |
| `Alg_rt/net_simula/net_monit/monit_malf.c` | 1568 | `perturba_riga_sommario_mf` | media |
| `Alg_rt/net_simula/mandb/acqmandb.c` | 259 | `write_sh` | media |
| `AlgLib/libsim/nega.c` | 30 | `nega` | media |
| `scada/libut/rwdbal.c` | 201 | `iodb` | bassa |
| `scada/scada/aggcfg/taggcfg.c` | 576 | `InvSlave` | bassa |

**Perché quelle priorità.** `funzioni.c`/`grsfio.c` in `Alg_mmi` sono una **terza
copia** del visualizzatore grafico (`formatta`, `prep_draw`, `read_multi`): hanno
gli stessi identici bug appena corretti in `grafics`, quindi la MMI mostra
verosimilmente gli stessi zeri. `f14.c::pr_float`/`spr_float` **scrivono** valori
nei file `f14`: qui il difetto non falsa solo la visualizzazione ma può corrompere
i dati salvati.

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

- **Non toccare** i 110 candidati senza prototipo: sono K&R coerenti e funzionanti.
  Convertirli sarebbe un rischio inutile su codice numerico che gira.
- Correggere **un componente per volta**, ricompilando e verificando: sono binari
  di produzione.
- Lo stesso conflitto vale per `short` (anch'esso promosso, a `int`): lo scanner
  lo copre già.
- `grafics::prep_str_tim` è K&R **senza** prototipo: lasciata com'è di proposito.
