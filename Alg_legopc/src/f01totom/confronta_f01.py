#!/usr/bin/env python3
"""confronta_f01.py - misura la fedelta' di una conversione f01totom.

Il criterio di riuscita della migrazione e' questo: ricostruire il modello a
partire dal .tom deve riprodurre lo stesso f01.dat di partenza. Se ci si
riesce, la task convertita e' semplicemente la task originale piu' il .tom, e
non c'e' niente da travasare: f14.dat, foraus.f e il resto restano quelli.

Uso:
    python3 confronta_f01.py <f01.dat originale> <f01.dat rigenerato>

Confronta blocchi, variabili per blocco e connessioni in ingresso, e dice
quanta parte delle differenze dipenda dai blocchi assenti (moduli non in
libreria) e quanta dalla ricostruzione delle connessioni.
"""
import collections
import re
import sys

RE_BLOCCO = re.compile(r'^(.{8})\s+BL\.-(.{4})-')


def leggi(percorso):
    """{nome blocco: [(variabile, tipo, blocco sorgente della connessione)]}"""
    blocchi = collections.OrderedDict()
    corrente = None
    for riga in open(percorso, encoding='latin-1'):
        riga = riga.rstrip('\n')
        intestazione = RE_BLOCCO.match(riga)
        if intestazione:
            corrente = intestazione.group(2)
            blocchi[corrente] = []
            continue
        if corrente is None:
            continue
        if riga.startswith('****') or riga.startswith('>>>>') or not riga.strip():
            continue
        # una variabile connessa porta il nome della variabile di provenienza,
        # e la sorgente compare dopo "<===" come <MODULO><BLOCCO>
        sorgente = re.search(r'<===(.{8})', riga)
        blocchi[corrente].append((riga[0:8], riga[10:16].strip(),
                                  sorgente.group(1)[4:8] if sorgente else ''))
    return blocchi


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)

    orig, conv = leggi(sys.argv[1]), leggi(sys.argv[2])
    assenti = [b for b in orig if b not in conv]
    aggiunti = [b for b in conv if b not in orig]
    comuni = [b for b in orig if b in conv]

    print(f"BLOCCHI          originale={len(orig)}  rigenerato={len(conv)}  comuni={len(comuni)}")
    if assenti:  print(f"  assenti dal rigenerato : {' '.join(assenti)}")
    if aggiunti: print(f"  in piu' nel rigenerato : {' '.join(aggiunti)}")

    uguali = [b for b in comuni
              if {v for v, _, _ in orig[b]} == {v for v, _, _ in conv[b]}]
    print(f"\nVARIABILI        blocchi con lo stesso insieme: {len(uguali)}/{len(comuni)}")

    tot = da_assenti = fra_presenti = 0
    for b in comuni:
        for _, _, sorgente in orig[b]:
            if not sorgente:
                continue
            tot += 1
            if sorgente in assenti:
                da_assenti += 1
            else:
                fra_presenti += 1
    ricostruite = sum(1 for b in comuni for _, _, s in conv[b] if s)

    print(f"\nCONNESSIONI in ingresso, sui soli blocchi comuni")
    print(f"  nell'originale                         : {tot}")
    print(f"    provenienti da un blocco assente     : {da_assenti}")
    print(f"    fra blocchi entrambi presenti        : {fra_presenti}")
    print(f"  ricostruite nel rigenerato             : {ricostruite}")
    if fra_presenti:
        perse = fra_presenti - ricostruite
        print(f"  PERSE pur essendo ricostruibili        : {perse}"
              f"  ({100.0*ricostruite/fra_presenti:.0f}% ricostruito)")

    fedele = (not assenti and not aggiunti and len(uguali) == len(comuni)
              and ricostruite == tot)
    print(f"\nESITO: {'conversione fedele' if fedele else 'conversione NON fedele'}")
    return 0 if fedele else 1


if __name__ == '__main__':
    sys.exit(main())
