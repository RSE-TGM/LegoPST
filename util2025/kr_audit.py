#!/usr/bin/env python3
"""
kr_audit.py — trova le definizioni K&R con parametri float/short che sono in
conflitto con un prototipo, cioe' il difetto di porting che fa arrivare alla
funzione un valore spazzatura (tipicamente 0).

Perche' e' un bug
-----------------
In una definizione in stile K&R

    void f(x)
    float x;
    { ... }

il parametro `float` subisce la *promozione automatica* a `double`: il tipo
effettivo della funzione e' `void f(double)`. Se pero' esiste anche un prototipo

    void f(float);

il chiamante passa un `float` (32 bit) mentre il chiamato legge un `double`
(64 bit) -> i due lati non concordano, comportamento indefinito, in pratica il
valore arriva corrotto e spesso vale 0.

Con i compilatori dell'epoca (senza prototipi) entrambi i lati promuovevano a
double e il codice funzionava: il difetto emerge solo ricompilando con un
toolchain moderno. Vedi docs/KR_PROTOTYPE_AUDIT.md.

Uso:
    python3 util2025/kr_audit.py [radice]      # default: directory corrente
    python3 util2025/kr_audit.py --all         # elenca anche i candidati senza
                                               # prototipo (non sono bug)
"""

import re, os, sys

KR = re.compile(
    r'^[A-Za-z_][A-Za-z0-9_ \*]*\s+\*?([A-Za-z_]\w*)\s*'
    r'\(\s*([a-z_]\w*(\s*,\s*[a-z_]\w*)*)\s*\)\s*$')
DECL = re.compile(r'^\s*(float|short)\b')


def scan(root):
    """Ritorna (candidati, conflitti). conflitti = [(file, riga, funzione)]."""
    cand, conflitti = 0, []
    for dirpath, _dirs, files in os.walk(root):
        if '/.git' in dirpath:
            continue
        for fn in files:
            if not fn.endswith('.c'):
                continue
            path = os.path.join(dirpath, fn)
            try:
                src = open(path, errors='ignore').read()
            except OSError:
                continue
            lines = src.split('\n')
            for i, line in enumerate(lines):
                m = KR.match(line)
                if not m:
                    continue
                name = m.group(1)
                # righe seguenti = dichiarazioni dei parametri, fino a '{'
                j, has_float, n = i + 1, False, 0
                while j < len(lines) and n < 8 and not lines[j].strip().startswith('{'):
                    if DECL.match(lines[j]):
                        has_float = True
                    if lines[j].strip() == '':
                        break
                    j += 1
                    n += 1
                if not has_float:
                    continue
                cand += 1
                # esiste un prototipo che dichiara float/short per questa funzione?
                proto = re.search(
                    r'^[^\n]*\b' + re.escape(name) +
                    r'\s*\([^)]*\b(float|short)\b[^)]*\)\s*;', src, re.M)
                if proto:
                    conflitti.append((path, i + 1, name))
    return cand, conflitti


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('-')]
    root = args[0] if args else '.'
    cand, conflitti = scan(root)
    print(f"candidati K&R con parametri float/short : {cand}")
    print(f"CON prototipo in conflitto (bug reali)  : {len(conflitti)}")
    for path, line, name in conflitti:
        print(f"  {path}:{line}  {name}()")
    return 1 if conflitti else 0


if __name__ == '__main__':
    sys.exit(main())
