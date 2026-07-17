#!/usr/bin/env python3
"""
lg_cosim2s01.py — traduce un lg_cosim.json in un file S01 completo.

lg_cosim.json e' concettualmente un S01 in JSON: entrambi descrivono un
simulatore composto da N task con le loro interconnessioni. Questo tool traduce
il primo nel secondo. Il JSON e' piu' facile da scrivere a mano; l'S01 e' il
formato che la toolchain LegoPST gia' consuma (lghmi, kDiffS01, net_compi,
TopSim), quindi il convertitore serve oltre alla co-simulazione.

    cd <dir con lg_cosim.json>
    python3 lg_cosim2s01.py        # genera ./S01
    lghmi                          # elenca le task
    kDiffS01                       # verifica coerenza variabili di interconnessione

Formato S01 generato (sezioni separate da righe `****` in colonna 1, come da
util97/S01_2_f01.c e net_compi/co_main.c):

    ****
    <nome_sim> <descrizione>                     # 1. simulatore
    ****
    <task> <descrizione>                         # 2. elenco task (una per riga)
    ...
    ****
    <path_task>\tP                               # 3. path + tipo (P=processo)
    ...
    ****
    OS <host> guest \t<path_task>                # 4. distribuzione (host per task)
    ...
    ****
    <dt>                                         # 5. passo di integrazione per task
    ...
    ****
    <task_dest>                                  # 6. un blocco per task destinazione:
    <var_in> <task_sorgente> <var_out>           #    ingressi connessi ad altre task
    ...
    ****
    ...                                          #    (un blocco **** per ogni task)
    ****
    BM                                           # 7. processi di sessione (fissi:
    SCADA                                        #    net_compi li pretende in
    BI                                           #    quest'ordine)
    ****

Direzione delle connessioni (co_main.c: strin[0]=input locale, strin[1]=modello
sorgente, strin[2]=output sorgente): la riga `<var_in> <src> <var_out>` nel
blocco della task T equivale a `{"from": "src.var_out", "to": "T.var_in"}`.

Note e limiti:
  * Solo le task LegoPST finiscono nell'S01 (discriminante: resources/task_info.env
    nel .fmu). Le FMU standard non hanno una task dir ne' variabili LegoPST: sono
    saltate con un avviso, e le connessioni da/verso di esse pure.
  * I nomi di variabile NON sono troncati a 8 caratteri: l'S01 storico li vincola,
    ma qui li lasciamo interi (nomi FMU qualificati, es. collet.B2.ALZAVAU1).
  * L'S01 generato descrive la co-simulazione e pilota gli strumenti che lo
    leggono; NON e' dato in pasto a net_sked per girare: un S01 nativo ha UN
    scheduler per N task, una co-simulazione ha N scheduler indipendenti.
  * dt (sez.5): default = settings.step_size, override per-FMU con "dt".
"""

import json, os, socket, argparse

TIPO_PROCESSO = "P"          # tipo task in sez.3 (P=processo, R=regolazione)
PROC_SESSIONE = ["BM", "SCADA", "BI"]   # sez.7, ordine imposto da co_main.c


def _read_env_file(path):
    """Legge un file KEY=VALUE (task_info.env) in un dict."""
    out = {}
    try:
        with open(path) as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#") or "=" not in line:
                    continue
                k, _, v = line.partition("=")
                out[k.strip()] = v.strip()
    except OSError:
        pass
    return out


def _split_ref(ref):
    """'FMU.VAR' -> ('FMU', 'VAR'); come lg_cosim._split, sul primo punto
    (il resto e' il nome variabile, che puo' essere qualificato)."""
    fmu, _, var = ref.partition(".")
    return fmu, var


def _task_dir(fmu_path, quiet=False):
    """Task dir di una FMU LegoPST, o None se non lo e'.

    Discriminante: resources/task_info.env (lo stesso di lg_cosim.py). In
    BUNDLE_MODE TASK_PATH e' relativo a <unzip>/resources/bundle."""
    unzip_dir = os.path.splitext(os.path.abspath(fmu_path))[0]
    info = os.path.join(unzip_dir, "resources", "task_info.env")
    if not os.path.isfile(info):
        # non ancora estratta: la estraggo (dir accanto al .fmu, come lg_cosim.py).
        # La dir deve esistere davvero: lghmi/kDiffS01 ci fanno cd.
        try:
            from fmpy import extract
            extract(fmu_path, unzipdir=unzip_dir)
        except Exception as e:
            if not quiet:
                print(f"[lg_cosim2s01] {os.path.basename(fmu_path)}: estrazione fallita ({e})")
            return None
    if not os.path.isfile(info):
        return None                      # FMU standard: nessuna task
    env = _read_env_file(info)
    tp = env.get("TASK_PATH", "")
    if not tp:
        return None
    if os.path.isabs(tp):
        return tp
    base = os.path.join(unzip_dir, "resources", "bundle")
    return os.path.normpath(os.path.join(base, tp))


def genera_s01(cfg_path, out_path=None, quiet=False):
    """lg_cosim.json -> S01 (completo). Ritorna il path scritto."""
    cfg_dir = os.path.dirname(os.path.abspath(cfg_path))
    with open(cfg_path) as f:
        cfg = json.load(f)

    settings     = cfg.get("settings", {})
    dt_default   = settings.get("step_size", 1.0)
    host_default = settings.get("host") or socket.gethostname()
    nome = cfg.get("model_name", "COSIM")
    desc = cfg.get("description", "co-simulazione lg_cosim")

    # --- task LegoPST (nell'ordine di dichiarazione) ---------------------
    tasks = []          # [{name, tdir, desc, host, dt}]
    for name, spec in cfg.get("fmus", {}).items():
        if isinstance(spec, dict):
            path = spec["path"]
            d    = spec.get("desc", "")
            host = spec.get("host") or host_default
            dt   = spec.get("dt", dt_default)
        else:
            path, d, host, dt = spec, "", host_default, dt_default
        if not os.path.isabs(path):
            path = os.path.join(cfg_dir, path)
        tdir = _task_dir(path, quiet)
        if tdir is None:
            if not quiet:
                print(f"[lg_cosim2s01] {name}: saltata (non e' una FMU LegoPST, nessuna task dir)")
            continue
        if not d:
            d = f"task {os.path.basename(tdir)} ({os.path.basename(path)})"
        tasks.append({"name": name, "tdir": tdir, "desc": d,
                      "host": host, "dt": float(dt)})

    if not tasks:
        raise SystemExit("[lg_cosim2s01] nessuna FMU LegoPST: S01 non generato")

    task_names = {t["name"] for t in tasks}

    # --- connessioni raggruppate per task destinazione -------------------
    conns = {t["name"]: [] for t in tasks}   # name -> [(var_in, src_task, var_out)]
    for c in cfg.get("connections", []):
        sf, sv = _split_ref(c["from"])
        df, dv = _split_ref(c["to"])
        if df not in conns:
            if not quiet:
                print(f"[lg_cosim2s01] connessione ignorata (destinazione '{df}' non e' una task LegoPST): {c}")
            continue
        if sf not in task_names:
            if not quiet:
                print(f"[lg_cosim2s01] connessione ignorata (sorgente '{sf}' non e' una task LegoPST): {c}")
            continue
        conns[df].append((dv, sf, sv))

    # --- montaggio sezioni ----------------------------------------------
    sezioni = []
    sezioni.append([f"{nome} {desc}"])                                   # 1
    sezioni.append([f"{t['name']} {t['desc']}" for t in tasks])          # 2
    sezioni.append([f"{t['tdir']}\t{TIPO_PROCESSO}" for t in tasks])     # 3
    sezioni.append([f"OS {t['host']} guest \t{t['tdir']}" for t in tasks])  # 4
    sezioni.append([f"{t['dt']:.5f}" for t in tasks])                    # 5
    for t in tasks:                                                      # 6..
        blocco = [t["name"]]
        for var_in, src, var_out in conns[t["name"]]:
            blocco.append(f"{var_in}\t{src}\t{var_out}")
        sezioni.append(blocco)
    sezioni.append(list(PROC_SESSIONE))                                  # 7

    testo = "****\n" + "\n****\n".join("\n".join(s) for s in sezioni) + "\n****\n"

    out_path = out_path or os.path.join(cfg_dir, "S01")
    with open(out_path, "w") as f:
        f.write(testo)
    if not quiet:
        n_conn = sum(len(v) for v in conns.values())
        print(f"[lg_cosim2s01] {len(tasks)} task, {n_conn} connessioni -> {out_path}")
    return out_path


def main():
    ap = argparse.ArgumentParser(
        description="Traduce lg_cosim.json in un file S01 completo")
    ap.add_argument("config", nargs="?", default="lg_cosim.json")
    ap.add_argument("-o", "--output", metavar="FILE",
                    help="file S01 da scrivere (default: <dir del config>/S01)")
    ap.add_argument("-q", "--quiet", action="store_true")
    a = ap.parse_args()
    genera_s01(a.config, a.output, a.quiet)


if __name__ == "__main__":
    main()
