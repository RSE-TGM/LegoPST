#!/usr/bin/env python3
"""
lg_cosim2s01.py — traduce un lg_cosim.json in un file S01.

Uso:
    python3 lg_cosim2s01.py [lg_cosim.json] [-o <file>] [-q]

lg_cosim.json e' concettualmente un S01 in JSON: entrambi descrivono un
simulatore composto da N task con le interconnessioni. Questo tool traduce il
primo nel secondo, per poter lanciare `lghmi` sulla co-simulazione: `lghmi`
entra in modalita' S01 se trova un file `S01` nella cwd, e ne legge SOLO le
prime tre sezioni (nome simulatore, elenco task, path+tipo).

    cd <dir con lg_cosim.json>
    python3 lg_cosim2s01.py        # genera ./S01
    lghmi                          # elenca le task della co-simulazione

Limiti (voluti — l'S01 generato serve a lghmi, non allo scheduler):

  * S01 PARZIALE: solo le sezioni 1-3. Mancano la distribuzione (sez.4), il dt
    per task (sez.5), le interconnessioni (sez.6+) e i processi di sessione.
    lghmi non le legge; kDiffS01/net_compi/net_sked SI' -> questo file NON e'
    dato in pasto a loro.
  * NON eseguibile: un S01 nativo descrive UN scheduler con N task; una
    co-simulazione e' N scheduler indipendenti, uno per FMU. Qui l'S01 descrive
    e basta.
  * Solo FMU LegoPST: una FMU standard non ha una task dir da elencare e viene
    saltata (con avviso).
"""

import json, os, sys, argparse

TIPO_PROCESSO = "P"          # lghmi elenca solo le task di tipo P


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


def _task_dir(fmu_path, quiet=False):
    """Task dir di una FMU LegoPST, o None se non lo e'.

    Il discriminante e' resources/task_info.env (lo stesso usato da lg_cosim.py).
    In BUNDLE_MODE TASK_PATH e' relativo a <unzip>/resources/bundle."""
    unzip_dir = os.path.splitext(os.path.abspath(fmu_path))[0]
    info = os.path.join(unzip_dir, "resources", "task_info.env")
    if not os.path.isfile(info):
        # non ancora estratta: estraggo con la stessa convenzione di lg_cosim.py
        # (dir accanto al .fmu). La dir deve esistere davvero: lghmi ci fa cd.
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
    """lg_cosim.json -> S01 (sezioni 1-3). Ritorna il path scritto."""
    cfg_dir = os.path.dirname(os.path.abspath(cfg_path))
    with open(cfg_path) as f:
        cfg = json.load(f)

    nome = cfg.get("model_name", "COSIM")
    desc = cfg.get("description", "co-simulazione lg_cosim")

    righe_task, righe_path = [], []
    for name, spec in cfg.get("fmus", {}).items():
        path = spec["path"] if isinstance(spec, dict) else spec
        d    = spec.get("desc", "") if isinstance(spec, dict) else ""
        if not os.path.isabs(path):
            path = os.path.join(cfg_dir, path)
        tdir = _task_dir(path, quiet)
        if tdir is None:
            if not quiet:
                print(f"[lg_cosim2s01] {name}: saltata (non e' una FMU LegoPST, nessuna task dir)")
            continue
        if not d:
            d = f"task {os.path.basename(tdir)} ({os.path.basename(path)})"
        righe_task.append(f"{name} {d}")
        # path ASSOLUTO: lghmi fa `file normalize [file join <s01dir> <path>]`,
        # che con un path assoluto restituisce il path stesso. Le task stanno
        # dentro i bundle estratti, non sotto una root comune.
        righe_path.append(f"{tdir}\t{TIPO_PROCESSO}")

    if not righe_task:
        raise SystemExit("[lg_cosim2s01] nessuna FMU LegoPST: S01 non generato")

    out_path = out_path or os.path.join(cfg_dir, "S01")
    with open(out_path, "w") as f:
        f.write("****\n")
        f.write(f"{nome} {desc}\n")
        f.write("****\n")
        for r in righe_task:
            f.write(r + "\n")
        f.write("****\n")
        for r in righe_path:
            f.write(r + "\n")
        f.write("****\n")
    if not quiet:
        print(f"[lg_cosim2s01] {len(righe_task)} task -> {out_path}")
    return out_path


def main():
    ap = argparse.ArgumentParser(
        description="Traduce lg_cosim.json in un S01 (parziale) per lghmi")
    ap.add_argument("config", nargs="?", default="lg_cosim.json")
    ap.add_argument("-o", "--output", metavar="FILE",
                    help="file S01 da scrivere (default: <dir del config>/S01)")
    ap.add_argument("-q", "--quiet", action="store_true")
    a = ap.parse_args()
    genera_s01(a.config, a.output, a.quiet)


if __name__ == "__main__":
    main()
