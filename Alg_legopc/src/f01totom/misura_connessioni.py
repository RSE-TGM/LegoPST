#!/usr/bin/env python3
"""misura_connessioni.py - quante connessioni del f01 d'epoca sono finite nel .tom.

Il .tom collega porta con porta, il f01 di legocad collega variabile con
variabile: non tutte le connessioni d'epoca sono esprimibili. Questo script
separa le tre categorie e dice quanta parte dell'esprimibile e' stata
ricostruita, che e' il vero voto della conversione.

Uso:
    python3 misura_connessioni.py <f01.dat d'epoca> <file.tom> <f01totom.inp>

Richiede LG_FILESI5 per leggere le porte dei .i5 (source .profile_legoroot).
"""
import collections
import os
import re
import sys

RE_BLOCCO = re.compile(r'^(.{8})\s+BL\.-(.{4})-')


def leggi_f01(percorso):
    """{blocco: {variabile formale: (tipo, blocco sorgente)}}"""
    blocchi = collections.OrderedDict()
    corrente = None
    for riga in open(percorso, encoding='latin-1'):
        riga = riga.rstrip('\n')
        intestazione = RE_BLOCCO.match(riga)
        if intestazione:
            corrente = intestazione.group(2)
            blocchi[corrente] = {}
            continue
        if corrente is None or riga.startswith('****') or not riga.strip():
            continue
        if len(riga) > 17 and riga[17] == '#':          # ingresso connesso
            blocchi[corrente][riga[18:22]] = ('CO', riga[4:8])
        else:
            blocchi[corrente][riga[0:4]] = (riga[10:16].strip(), '')
    return blocchi


def porte_i5(nomefile, i5dir):
    """[(idporta, [variabili])] del file .i5"""
    righe = open(os.path.join(i5dir, nomefile), encoding='latin-1').read().split('\n')
    return [(righe[i].strip(),
             [v for v in righe[i + 1].split() if v not in ('____', 'XXXX')])
            for i, riga in enumerate(righe)
            if re.match(r'^t\d', riga) and i + 1 < len(righe)]


def porte_busy(percorso):
    """{blocco: {porta occupata}} leggendo la seconda sezione del .tom"""
    occupate = collections.defaultdict(set)
    sezione = 0
    blocco = classe = ultima = None
    for riga in open(percorso, encoding='latin-1'):
        riga = riga.rstrip('\n')
        if riga == '****':
            sezione += 1
            continue
        if sezione != 1:
            continue
        if riga == '++++':
            blocco = classe = None
            continue
        if riga.startswith('port'):
            ultima = riga
        elif riga.startswith('busy'):
            occupate[blocco].add(ultima)
        elif riga != 'free' and blocco is None:
            if classe is None:
                classe = riga
            else:
                blocco = riga
    return occupate


def main():
    if len(sys.argv) != 4:
        sys.exit(__doc__)
    i5dir = os.environ.get('LG_FILESI5', '')
    if not os.path.isdir(i5dir):
        sys.exit("ERRORE: LG_FILESI5 non definita o inesistente. source .profile_legoroot")

    orig = leggi_f01(sys.argv[1])
    occupate = porte_busy(sys.argv[2])
    scelta = {}
    for riga in open(sys.argv[3], encoding='latin-1'):
        pezzi = riga.split()
        if len(pezzi) >= 3:
            scelta[pezzi[1]] = pezzi[2]

    # i blocchi che nel .tom non ci sono: moduli assenti dalle librerie
    convertiti = set(occupate) | {b for b in scelta if b in orig}
    assenti = {b for b in orig if b not in convertiti}

    esprimibili = ricostruite = set(), set()
    esprimibili, ricostruite, multi, orfane = set(), set(), set(), set()
    for blocco, variabili in orig.items():
        if blocco in assenti or blocco not in scelta:
            continue
        connesse = {v for v, (t, s) in variabili.items()
                    if t == 'CO' and s not in assenti}
        su_porta = set()
        for idporta, elenco in porte_i5(scelta[blocco], i5dir):
            qui = [v for v in elenco if v in connesse]
            if not qui:
                continue
            su_porta |= set(qui)
            if len({variabili[v][1] for v in qui}) == 1:
                esprimibili |= {(blocco, v) for v in qui}
                if 'por' + idporta in occupate.get(blocco, ()):
                    ricostruite |= {(blocco, v) for v in qui}
            else:
                multi |= {(blocco, v) for v in qui}
        orfane |= {(blocco, v) for v in connesse - su_porta}

    multi -= esprimibili
    tot = len(esprimibili | multi | orfane)
    print(f"  connessioni in ingresso dei blocchi convertiti : {tot}")
    print(f"    esprimibili in un .tom                       : {len(esprimibili)}")
    print(f"      di cui ricostruite                         : {len(ricostruite)}"
          f"   ({100.0*len(ricostruite)/len(esprimibili):.0f}%)" if esprimibili else "")
    print(f"    su porte con ingressi da blocchi diversi     : {len(multi)}")
    print(f"    su variabili che il modulo di oggi non ha    : {len(orfane)}")
    if assenti:
        print(f"    (blocchi non convertiti, moduli assenti      : {len(assenti)})")
    return 0 if esprimibili == ricostruite else 1


if __name__ == '__main__':
    sys.exit(main())
