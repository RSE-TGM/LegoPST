#!/usr/bin/env python3
"""
icone_c.py - genera dai PNG di icone/ i sorgenti C con i pixel delle icone
delle finestre dei programmi Motif (config, xstaz, mmi).

Per ogni nome scrive AlgLib/libutilx/icona_<nome>.c, con i lati 16, 32 e 48
nel formato di _NET_WM_ICON: per ogni lato larghezza, altezza e poi i pixel
ARGB (alfa nel byte alto), riga per riga. Li usa IconaFinestreApp
(AlgLib/libutilx/iconafinestra.c).

Uso, dalla radice del repository:
    python3 icone/icone_c.py              # config, xstaz e mmi
    python3 icone/icone_c.py config       # solo config

Python puro (zlib per i PNG): non servono ImageMagick o PIL. Legge i PNG
come li scrive ImageMagick: RGBA a 8 bit, non interlacciati.
"""

import os
import struct
import sys
import zlib

LATI = (16, 32, 48)
NOMI = ("config", "xstaz", "mmi")

QUI = os.path.dirname(os.path.abspath(__file__))
RADICE = os.path.dirname(QUI)
USCITA = os.path.join(RADICE, "AlgLib", "libutilx")


def leggi_png(percorso):
    """Ritorna (larghezza, altezza, byte RGBA)."""
    with open(percorso, "rb") as f:
        dati = f.read()
    if dati[:8] != b"\x89PNG\r\n\x1a\n":
        sys.exit(percorso + ": non e' un PNG")
    pos = 8
    idat = b""
    w = h = 0
    while pos < len(dati):
        lun, tipo = struct.unpack(">I4s", dati[pos:pos + 8])
        corpo = dati[pos + 8:pos + 8 + lun]
        pos += 12 + lun
        if tipo == b"IHDR":
            w, h, prof, colore, _, _, interl = struct.unpack(">IIBBBBB", corpo)
            if prof != 8 or colore != 6 or interl != 0:
                sys.exit(percorso + ": serve RGBA a 8 bit non interlacciato "
                         "(prof. %d, colore %d, interl. %d)" % (prof, colore, interl))
        elif tipo == b"IDAT":
            idat += corpo
        elif tipo == b"IEND":
            break
    grezzi = zlib.decompress(idat)
    bpp = 4
    riga = w * bpp
    out = bytearray()
    prec = bytearray(riga)
    p = 0
    for _ in range(h):
        filtro = grezzi[p]
        cur = bytearray(grezzi[p + 1:p + 1 + riga])
        p += 1 + riga
        for i in range(riga):
            a = cur[i - bpp] if i >= bpp else 0
            b = prec[i]
            c = prec[i - bpp] if i >= bpp else 0
            if filtro == 1:
                cur[i] = (cur[i] + a) & 255
            elif filtro == 2:
                cur[i] = (cur[i] + b) & 255
            elif filtro == 3:
                cur[i] = (cur[i] + ((a + b) >> 1)) & 255
            elif filtro == 4:
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                cur[i] = (cur[i] + pr) & 255
            elif filtro != 0:
                sys.exit(percorso + ": filtro PNG %d sconosciuto" % filtro)
        out += cur
        prec = cur
    return w, h, bytes(out)


def genera(nome):
    valori = []
    for lato in LATI:
        png = os.path.join(QUI, nome, "%s_%d.png" % (nome, lato))
        w, h, rgba = leggi_png(png)
        valori += [w, h]
        for i in range(0, len(rgba), 4):
            r, g, b, a = rgba[i:i + 4]
            valori.append((a << 24) | (r << 16) | (g << 8) | b)

    simbolo = "Icona" + nome.capitalize()
    righe = [
        "/*",
        "   icona_%s.c" % nome,
        "",
        "   GENERATO da icone/icone_c.py dai PNG di icone/%s/ (lati %s):" % (
            nome, ", ".join(str(l) for l in LATI)),
        "   non modificare a mano, si rigenera se l'icona cambia.",
        "   Formato di _NET_WM_ICON: larghezza, altezza, pixel ARGB.",
        "*/",
        "#include \"iconafinestra.h\"",
        "",
        "static const unsigned int dati[%d] = {" % len(valori),
    ]
    for i in range(0, len(valori), 8):
        righe.append("  " + ",".join("0x%08x" % v for v in valori[i:i + 8]) + ",")
    righe += [
        "};",
        "",
        "const IconaArgb %s = { %d, dati };" % (simbolo, len(valori)),
        "",
    ]
    dest = os.path.join(USCITA, "icona_%s.c" % nome)
    with open(dest, "w") as f:
        f.write("\n".join(righe))
    print("scritto", os.path.relpath(dest, RADICE), "(%d valori)" % len(valori))


if __name__ == "__main__":
    for n in (sys.argv[1:] or NOMI):
        genera(n)
