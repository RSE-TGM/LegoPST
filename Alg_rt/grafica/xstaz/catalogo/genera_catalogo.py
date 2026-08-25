#!/usr/bin/env python3
"""Genera r01.dat con TUTTI i tipi di stazione e il nome sotto ciascuno.

Legge la tabella new_staz[] di AlgLib/libinclude/newstaz.h, quindi resta
allineato da solo se si aggiungono tipi. Uso:

    python3 genera_catalogo.py > r01.dat
    compstaz            # nella stessa dir, con variabili.rtf
    xstaz 1 &  ;  stazpag LED
"""
import re, os
TIPO2KW = {"STRINGA_DESCR": "STRINGA"}
BASE = {
 "LED":["COLORE","ETICHETTA","INPUT","INPUT_BLINK"],
 "PULS_LUCE":["COLORE","OUTPUT","INPUT","INPUT_BLINK"],
 "PULSANTE":["COLORE","OUTPUT"],
 "LAMPADA":["COLORE","INPUT","INPUT_BLINK"],
 "SELETTORE":["ETICHETTA","ETICHETTA","OUTPUT","INPUT"],
 "STRINGA":["ETICHETTA"],
 "DISPLAY":["INPUT"],
 "LUCE":["COLORE","INPUT","INPUT_BLINK"],
 "TASTO":["COLORE","OUTPUT"],
 "SET_VALORE":["ETICHETTA","OUTPUT","OUTPUT","INPUT","SCALAMENTO","OFFSET","INIBIZIONE"],
 "DISPLAY_SCALATO":["INPUT","SCALAMENTO","OFFSET"],
 "INDICATORE_SINCRO":["INPUT"]*6 + ["OUTPUT"]*2}

def righe(ogg, sottotipo):
    #  INDICATORE e' l'unico con righe condizionate al sottotipo: SCALAMENTO_ERR,
    #  MINMAX_ERR e INPUT_ERR si leggono solo con INDIC_AGO_ERR (c_indic.c:92,139)
    if ogg == "INDICATORE":
        if sottotipo == "INDIC_AGO_ERR":
            return ["SCALAMENTO","MINMAX","OFFSET","SCALAMENTO_ERR","MINMAX_ERR","INPUT","INPUT_ERR"]
        return ["SCALAMENTO","MINMAX","OFFSET","INPUT"]
    return BASE[ogg]

def _trova_newstaz():
    """newstaz.h risolto dalla posizione dello script: .../Alg_rt/grafica/xstaz/
    catalogo -> LEGOROOT/AlgLib/libinclude/newstaz.h. Se lo script viene spostato,
    si puo' passare il percorso come primo argomento."""
    if len(sys.argv) > 1:
        return sys.argv[1]
    qui = os.path.dirname(os.path.abspath(__file__))
    root = os.path.normpath(os.path.join(qui, "..", "..", "..", ".."))
    return os.path.join(root, "AlgLib", "libinclude", "newstaz.h")


def leggi_tabella(path=None):
    if path is None:
        path = _trova_newstaz()
    src = open(path, encoding="latin-1").read()
    tab = src[src.index("TIPI_NEWSTAZ  new_staz[]"):]; tab = tab[:tab.index("\n};")]
    tab = re.sub(r'/\*.*?\*/', '', tab, flags=re.S)
    out = []
    for nome, resto in re.findall(r'\{\s*"([A-Z0-9_]+)"\s*,(.*?)\}', tab, re.S):
        tok = [t.strip() for t in resto.replace("\n", " ").split(",") if t.strip()]
        n, larg, alt = int(tok[0]), int(tok[1]), int(tok[2]); rest = tok[3:]
        ogg = [(TIPO2KW.get(rest[5*i], rest[5*i]), rest[5*i+1])
               for i in range(n) if len(rest) >= 5*(i+1)]
        out.append((nome, larg, alt, ogg))
    return out
import sys

VAL = {"COLORE":"GIALLO","ETICHETTA":"-","SCALAMENTO":"1.","SCALAMENTO_ERR":"1.",
       "OFFSET":"0.","MINMAX":"0. 100.","MINMAX_ERR":"0. 100."}
ETI_L  = 8      # la stazione TESTO usata come etichetta e' larga 8 celle
LIM_X  = 28     # 28 celle x 62 px = 1736 px: la finestra ci sta sullo schermo
LIM_Y  = 14

def genera():
    t = leggi_tabella()
    staz = {n: (l, a, o) for n, l, a, o in t}
    ordine = [n for n, _, _, _ in t]
    led  = [x for x in ordine if x.startswith("LED")]
    puls = [x for x in ordine if re.match(r'^(P\d|PL)', x)]
    FAM = [("LED",     "Segnalazioni a LED", led),
           ("PULS","Pulsanti con spie", puls),
           ("INDIC", "Indicatori e impostatori",
            ["IBARRA1","IBARRA2","IAGO","IAGOERR","AGOSETV","AGERSETV","SINCRONO"]),
           ("DISP", "Display e testi",
            ["DISPLAY","DISPSET","DISPSCAL","DISSETSC","TESTO","TESTOBIS"]),
           ("VARIE",   "Selettori, lampade, comandi",
            ["SELET_A","SELET_B","LAMP1","LAMP2","LAMP1L3","LUCE","TASTO","TASTOBIS","MIXER","MIXER1"])]
    noti = set(sum([f[2] for f in FAM], []))
    resto = [x for x in ordine if x not in noti]
    if resto: FAM.append(("ALTRE", "Altri tipi", resto))

    pagine, corpo, npag, num = [], [], 0, 0
    for sigla, descr, elenco in FAM:
        npag += 1; pag = npag; parte = 1
        pagine.append((pag, sigla, descr))
        x = y = 1; rowh = 0
        for nome in elenco:
            larg, alt, ogg = staz[nome]
            w, h = max(larg, ETI_L) + 1, alt + 2
            if x + w > LIM_X:
                x = 1; y += rowh; rowh = 0
            if y + h > LIM_Y:
                npag += 1; pag = npag; parte += 1
                pagine.append((pag, ("%s%d" % (sigla, parte))[:8], "%s (%d)" % (descr, parte)))
                x = y = 1; rowh = 0
            #  L'asse Y e' INVERTITO nel disegno (cnewstaz.c: ydraw = height - ydraw
            #  - htot): per far comparire il nome SOTTO al widget, l'etichetta va
            #  messa alla cella di y MINORE.
            num += 1
            corpo.append("****\nSTAZIONE\nNUMERO       %d\nTIPO         TESTO\nDESCRIZIONE  etichetta\nPAGINA       %d\nPOSIZIONE    %d   %d"
                         % (num, pag, x, y))
            corpo.append("STRINGA\nETICHETTA    %s" % nome)
            num += 1
            corpo.append("****\nSTAZIONE\nNUMERO       %d\nTIPO         %s\nDESCRIZIONE  %s\nPAGINA       %d\nPOSIZIONE    %d   %d"
                         % (num, nome, nome, pag, x, y + 1))
            for o, sot in ogg:
                corpo.append(o)
                for r in righe(o, sot):
                    corpo.append(("%s %s" % (r, VAL.get(r, ""))).rstrip())
            x += w; rowh = max(rowh, h)

    fuori = ["****\nPAGINA\nNUMERO       %d\nNOME         %s\nDESCRIZIONE  %s" % p for p in pagine]
    return "\n".join(fuori + corpo + ["****\nEND_OF_FILE"]), len(pagine), num

if __name__ == "__main__":
    testo, npag, nstaz = genera()
    print(testo)
    print("catalogo: %d pagine, %d stazioni" % (npag, nstaz), file=sys.stderr)
