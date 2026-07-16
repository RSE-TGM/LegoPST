#!/usr/bin/env python3
"""
lg_cosim.py — Co-simulation master FMI 2.0 per FMU LegoPST
Gestisce N FMU in co-simulazione: scambio variabili, sync real-time/accelerato.

Uso:
    python3 lg_cosim.py [lg_cosim.json]
    python3 lg_cosim.py lg_cosim.json --stop-time 60 --speedup 1.0

Speedup: null/0 = massima velocità, 1.0 = real-time, 2.0 = 2× real-time
"""

import json, os, sys, csv, time, argparse, ctypes, glob, subprocess
from fmpy import read_model_description, extract
from fmpy.fmi2 import FMU2Slave

# os.environ opera su un dict Python che non riceve le modifiche fatte da
# codice C caricato come .so (setenv/unsetenv C non aggiornano il dict).
# Usiamo ctypes per manipolare direttamente l'env C del processo.
_libc = ctypes.CDLL(None, use_errno=True)
_libc.getenv.restype    = ctypes.c_char_p
_libc.setenv.argtypes   = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
_libc.unsetenv.argtypes = [ctypes.c_char_p]
_libc.shmget.restype    = ctypes.c_int
_libc.shmget.argtypes   = [ctypes.c_int, ctypes.c_size_t, ctypes.c_int]

# ID_SHM_VAR = 5 (sim_ipc.h): offset della SHM topology/variabili rispetto a
# SHR_USR_KEY. È l'unica SHM creata in modalità headless (RtCreateDbPunti).
# ID_SHM_SIM = 0 (la SHM net_sked) NON viene creata in bundle headless, quindi
# il probe in lg_fmi2.c (che usa ID_SHM_SIM) trova sempre slot libero.
_ID_SHM_VAR = 5

def _cenv_get(key):
    v = _libc.getenv(key.encode())
    return v.decode() if v else None

def _cenv_unset(key):
    _libc.unsetenv(key.encode())
    os.environ.pop(key, None)   # mantiene il dict Python allineato

def _cenv_set(key, value):
    _libc.setenv(key.encode(), value.encode(), 1)
    os.environ[key] = value     # mantiene il dict Python allineato

def _find_free_slot():
    """Trova il primo slot SHM LegoPST libero (1..8) per l'utente corrente.
    Controlla ID_SHM_VAR=5 che è la SHM effettivamente creata in headless mode.
    Il probe C usa ID_SHM_SIM=0 che non viene creata -> always free -> bug."""
    uid = os.getuid()
    for slot in range(1, 9):
        candidate = uid * 10000 + slot * 1100
        sid = _libc.shmget(candidate + _ID_SHM_VAR, 0, 0)
        if sid == -1:   # ENOENT: slot libero
            return candidate
    return None


class _FMUInstance:
    def __init__(self, name, fmu_path):
        self.name = name
        # Dir persistente accanto al .fmu (come run_fmu.sh): evita problemi di
        # path relativi in sked_start e collisioni su run successivi.
        self.unzip_dir = os.path.splitext(os.path.abspath(fmu_path))[0]
        extract(fmu_path, unzipdir=self.unzip_dir)
        self._restore_perms()
        self._patch_killsim_guard()
        # FMU LegoPST? Discriminante = resources/task_info.env, il file che
        # attach_or_launch (lg_fmi2.c) richiede per ricavare TASK_PATH/LEGOROOT:
        # c'e' in tutte le FMU LegoPST (bundle e non), non in una FMU standard.
        # Serve a saltare lo slot SHM e la HMI su FMU di terze parti, che non
        # sanno che farsene (vedi instantiate()).
        self.is_lego = os.path.isfile(
            os.path.join(self.unzip_dir, "resources", "task_info.env"))
        md = read_model_description(fmu_path)
        self.guid     = md.guid
        self.model_id = md.coSimulation.modelIdentifier
        self.vr    = {v.name: v.valueReference for v in md.modelVariables}
        self.vtype = {v.name: v.type           for v in md.modelVariables}
        # alias: nome breve (ultima componente) → nome completo, se univoco
        self._alias = {}
        for fullname in self.vr:
            short = fullname.rsplit('.', 1)[-1]
            self._alias[short] = self._alias.get(short, []) + [fullname]
        self.slave = None
        self._shr_usr_key  = None   # slot SHM scelto da fmi2Instantiate
        self._shr_usr_keys = None

    def _restore_perms(self):
        """fmpy estrae via zipfile Python, che NON preserva il bit +x: senza
        questo launch_sim.sh/net_sked/wish del bundle restano non-eseguibili e
        fmi2Instantiate muore con 'Permission denied'. Stessa cosa che fa
        run_fmu.sh dopo extract(). Invocato via `bash` (non serve l'exec bit sul
        file stesso). No-op per FMU non-bundle, idempotente."""
        rp = os.path.join(self.unzip_dir, "resources", "bundle", "restore_perms.sh")
        if os.path.isfile(rp):
            subprocess.run(["bash", rp], check=False)

    def _patch_killsim_guard(self):
        """Wrappa killsim nei net_startup_headless.sh estratti con la guardia
        LG_COSIM_NO_KILLSIM: killsim su Linux cancella TUTTE le SHM utente,
        uccidendo le FMU già avviate in co-simulazione. Idempotente."""
        _OLD = 'killsim 2>/dev/null || true'
        _NEW = ('if [ -z "${LG_COSIM_NO_KILLSIM:-}" ]; then\n'
                '    killsim 2>/dev/null || true\nfi')
        pattern = os.path.join(self.unzip_dir, '**', 'net_startup_headless.sh')
        for path in glob.glob(pattern, recursive=True):
            try:
                txt = open(path).read()
                if _OLD in txt and 'LG_COSIM_NO_KILLSIM' not in txt:
                    open(path, 'w').write(txt.replace(_OLD, _NEW))
            except OSError:
                pass

    def _resolve(self, var):
        if var in self.vr:
            return var
        candidates = self._alias.get(var, [])
        if len(candidates) == 1:
            return candidates[0]
        if len(candidates) > 1:
            raise KeyError(f"variabile '{var}' ambigua in '{self.name}': {candidates}")
        raise KeyError(f"variabile '{var}' non trovata in FMU '{self.name}'")

    def instantiate(self):
        # Slot SHM e HMI riguardano solo le FMU LegoPST: una FMU standard ignora
        # SHR_USR_KEY/LG_FMU_OPEN_HMI e non crea alcuna SHM, quindi il probe la
        # vedrebbe sempre "libera" assegnando lo stesso slot a tutte (innocuo ma
        # fuorviante) e potrebbe far fallire per "slot esauriti" una co-sim di
        # sole FMU di terze parti. Saltiamo il blocco per le non-LegoPST.
        if self.is_lego:
            # Il probe slot in lg_fmi2.c usa ID_SHM_SIM=0 che in modalità headless
            # non viene mai creato → tutti gli slot sembrano liberi → collisione.
            # Facciamo il probe qui da Python usando ID_SHM_VAR=5 (la SHM che esiste
            # davvero) e pre-assegniamo SHR_USR_KEY prima di chiamare fmi2Instantiate.
            # Usiamo ctypes perché os.environ non vede i setenv() delle .so C.
            slot_key = _find_free_slot()
            if slot_key is None:
                raise RuntimeError(f"[lg_cosim] tutti gli 8 slot SHM occupati per uid={os.getuid()}")
            _cenv_set('SHR_USR_KEY',  str(slot_key))
            _cenv_set('SHR_USR_KEYS', str(slot_key + 1000))
            # La HMI NON si apre qui: in co-simulazione la apre il selettore
            # lghmi, una volta sola e dopo l'instantiate (vedi LgCosim._apri_hmi).
            # Spegniamo il flag che la .so leggerebbe, per non ereditarlo
            # dall'ambiente del chiamante (es. una shell dove si e' usato
            # run_fmu --hmi, che invece si serve di questo meccanismo).
            _cenv_set('LG_FMU_OPEN_HMI', '0')
        self.slave = FMU2Slave(
            guid=self.guid, unzipDirectory=self.unzip_dir,
            modelIdentifier=self.model_id, instanceName=self.name)
        self.slave.instantiate()
        if not self.is_lego:
            return
        self._shr_usr_key  = _cenv_get('SHR_USR_KEY')
        self._shr_usr_keys = _cenv_get('SHR_USR_KEYS')
        print(f"[lg_cosim] {self.name}: slot SHM SHR_USR_KEY={self._shr_usr_key}")
        # Stampa percorsi log per il debug (net_sked.fmu.log, dispatcher.fmu.log)
        task_dir = os.path.join(self.unzip_dir, "resources", "bundle", "task")
        if os.path.isdir(task_dir):
            tasks = os.listdir(task_dir)
            for t in tasks:
                td = os.path.join(task_dir, t)
                print(f"[lg_cosim] {self.name}: log dir → {td}")

    def setup_experiment(self, t0, t_end):
        self.slave.setupExperiment(startTime=t0, stopTime=t_end)
        self.slave.enterInitializationMode()

    def exit_init(self):
        self.slave.exitInitializationMode()

    def step(self, t, h):
        self.slave.doStep(currentCommunicationPoint=t, communicationStepSize=h)

    def get(self, var):
        fullname = self._resolve(var)
        vr, vt = [self.vr[fullname]], self.vtype[fullname]
        if vt == 'Real':    return self.slave.getReal(vr)[0]
        if vt == 'Integer': return self.slave.getInteger(vr)[0]
        if vt == 'Boolean': return self.slave.getBoolean(vr)[0]
        return self.slave.getReal(vr)[0]

    def set(self, var, value):
        fullname = self._resolve(var)
        vr, vt = [self.vr[fullname]], self.vtype[fullname]
        if vt == 'Real':    self.slave.setReal(vr, [float(value)])
        elif vt == 'Integer': self.slave.setInteger(vr, [int(value)])
        elif vt == 'Boolean': self.slave.setBoolean(vr, [bool(value)])

    def terminate(self):
        if self.slave:
            try:
                # killsim (chiamato dentro fmi2FreeInstance via system())
                # legge SHR_USR_KEY dall'env C. Ripristiniamo la chiave
                # di questa istanza prima di freeInstance.
                if self._shr_usr_key:
                    _cenv_set('SHR_USR_KEY',  self._shr_usr_key)
                if self._shr_usr_keys:
                    _cenv_set('SHR_USR_KEYS', self._shr_usr_keys)
                self.slave.terminate()
                self.slave.freeInstance()
            except: pass
            self.slave = None

    def cleanup(self):
        pass  # dir persistente accanto al .fmu: non cancellare


class LgCosim:
    def __init__(self, config_path):
        self._cfg_dir = os.path.dirname(os.path.abspath(config_path))
        with open(config_path) as f:
            cfg = json.load(f)

        s = cfg["settings"]
        self.t0       = float(s.get("start_time", 0.0))
        self.t_end    = float(s["stop_time"])
        self.h        = float(s["step_size"])
        self.speedup  = s.get("speedup")       # None/0 = max, 1.0 = RT, 2.0 = 2×RT
        # lg_fmi2.c esegue chdir(task_path) durante fmi2Instantiate: i path
        # relativi vengono risolti nella task dir dell'ultima FMU. Li rendiamo
        # assoluti subito, ancorati alla dir del config.
        log_file = s.get("log_file", "log_cosim.csv")
        if not os.path.isabs(log_file):
            log_file = os.path.join(self._cfg_dir, log_file)
        self.log_file = log_file
        self.log_vars = s.get("log_vars", [])  # ["FMU.VAR", ...]

        self.connections  = cfg.get("connections", [])   # [{"from":"A.X","to":"B.Y"}, ...]
        self.start_values = cfg.get("start_values", {})  # {"A.VAR": val, ...}

        # HMI: un solo interruttore per l'intera sessione (settings.hmi), non piu'
        # per-FMU. Apre il selettore lghmi, da cui l'utente sceglie a run-time
        # quali pagine draw2gr aprire, quante ne vuole e quando -- invece di
        # doverlo decidere nel config prima di partire.
        self.hmi = bool(s.get("hmi", False))
        self._cfg_path = os.path.abspath(config_path)

        self.fmus = {}
        for name, spec in cfg["fmus"].items():
            # spec = "path.fmu"  oppure  {"path": "...", "desc": "..."}
            path = spec["path"] if isinstance(spec, dict) else spec
            if not os.path.isabs(path):
                path = os.path.join(self._cfg_dir, path)
            print(f"[lg_cosim] caricamento {name}: {os.path.basename(path)}")
            self.fmus[name] = _FMUInstance(name, path)

    @staticmethod
    def _split(ref):
        """'FMU.VAR' -> ('FMU', 'VAR')"""
        return ref.split('.', 1)

    def step_hook(self, t):
        """Override in sottoclasse per logica Python nel loop (es. calcoli ibridi)."""
        pass

    def _apri_hmi(self):
        """Genera l'S01 della co-simulazione e apre il selettore lghmi.

        Chiamata DOPO l'instantiate: le sim sono vive e gli slot SHM assegnati,
        quindi ogni pagina aperta dal selettore trova la propria sim. Il
        selettore resta aperto per tutta la sessione e sopravvive alla fine del
        run (setsid), cosi' puoi guardare lo stato finale.

        lg_cosim.json e' concettualmente un S01 (simulatore composto = N task +
        interconnessioni): lo traduciamo e usiamo la modalita' S01 che lghmi ha
        gia', invece di inventare un formato di lista solo nostro."""
        if not os.environ.get("DISPLAY"):
            print("[lg_cosim] hmi richiesta ma DISPLAY non impostato: salto")
            return
        try:
            from lg_cosim2s01 import genera_s01
            s01 = genera_s01(self._cfg_path, quiet=True)
        except Exception as e:
            print(f"[lg_cosim] hmi: generazione S01 fallita ({e})")
            return
        # Selettore: quello del bundle (unico disponibile sulla macchina target,
        # dove LegoPST non c'e'), altrimenti l'lghmi dell'installazione.
        cmd = None
        for inst in self.fmus.values():
            rl = os.path.join(inst.unzip_dir, "resources", "bundle", "run_lghmi.sh")
            if inst.is_lego and os.path.isfile(rl):
                cmd = ["bash", rl]; break
        if cmd is None:
            from shutil import which
            if which("lghmi"):
                cmd = ["lghmi"]
            else:
                print(f"[lg_cosim] hmi: nessun selettore trovato (S01 pronto: {s01})")
                return
        # cwd = dir dell'S01 -> lghmi entra in modalita' S01. setsid: il selettore
        # e' indipendente e non muore col master.
        try:
            subprocess.Popen(cmd, cwd=os.path.dirname(s01), start_new_session=True,
                             stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
            print(f"[lg_cosim] HMI: selettore task aperto (S01: {s01})")
        except OSError as e:
            print(f"[lg_cosim] hmi: lancio selettore fallito ({e})")

    def run(self):
        # Impedisce a killsim (Linux) di cancellare TUTTE le SHM utente:
        # in co-simulazione ammazzerebbe le FMU già avviate.
        _cenv_set('LG_COSIM_NO_KILLSIM', '1')

        # Instantiate + init
        for inst in self.fmus.values():
            inst.instantiate()
            inst.setup_experiment(self.t0, self.t_end)

        for ref, val in self.start_values.items():
            fmu, var = self._split(ref)
            self.fmus[fmu].set(var, val)

        for inst in self.fmus.values():
            inst.exit_init()

        if self.hmi:
            self._apri_hmi()

        log_f  = open(self.log_file, "w", newline="")
        writer = csv.writer(log_f)
        writer.writerow(["time"] + self.log_vars)

        t = self.t0
        wall_start = time.monotonic()
        overruns   = 0
        n_steps    = 0

        speedup_label = (f"{self.speedup}×RT" if self.speedup else "max")
        print(f"[lg_cosim] avvio: t_end={self.t_end} h={self.h} speedup={speedup_label}")

        interrupted = False
        try:
            while t < self.t_end - self.h * 0.5:
                # Step Gauss-Seidel (ordine di dichiarazione in config)
                for inst in self.fmus.values():
                    inst.step(t, self.h)
                t += self.h
                n_steps += 1

                # Scambio variabili lungo le connessioni
                for conn in self.connections:
                    sf, sv = self._split(conn["from"])
                    df, dv = self._split(conn["to"])
                    self.fmus[df].set(dv, self.fmus[sf].get(sv))

                # Hook Python in-the-loop
                self.step_hook(t)

                # Log CSV
                row = [t]
                for ref in self.log_vars:
                    f, v = self._split(ref)
                    row.append(self.fmus[f].get(v))
                writer.writerow(row)
                log_f.flush()

                # Sync real-time / accelerato
                if self.speedup:
                    t_expected = wall_start + (t - self.t0) / self.speedup
                    slack = t_expected - time.monotonic()
                    if slack > 1e-4:
                        time.sleep(slack)
                    elif slack < -0.005:
                        overruns += 1

                print(f"\r[lg_cosim] t={t:.2f}/{self.t_end:.1f}s", end="", flush=True)

        except KeyboardInterrupt:
            interrupted = True
            print(f"\n[lg_cosim] interrotto dall'utente a t={t:.2f}s")
        except Exception as e:
            print(f"\n[lg_cosim] ERRORE: {e}")
            raise
        finally:
            log_f.close()
            _cenv_unset('LG_COSIM_NO_KILLSIM')
            for inst in self.fmus.values():
                inst.terminate()
                print(f"[lg_cosim] {inst.name}: dir FMU → {inst.unzip_dir}")

        if not interrupted:
            print(f"\n[lg_cosim] fine — {n_steps} passi, log → {self.log_file}")
        if overruns:
            pct = overruns / n_steps * 100
            print(f"[lg_cosim] attenzione: {overruns} overrun real-time ({pct:.1f}% dei passi)")
        return overruns


def main():
    ap = argparse.ArgumentParser(description="LegoPST FMU co-simulation master")
    ap.add_argument("config", nargs="?", default="lg_cosim.json")
    ap.add_argument("--stop-time",  type=float, metavar="T", help="Override stop_time")
    ap.add_argument("--step-size",  type=float, metavar="H", help="Override step_size")
    ap.add_argument("--speedup",    type=float, metavar="S",
                    help="Override speedup: 1.0=RT, 2.0=2×RT, 0=max")
    ap.add_argument("--debug",      action="store_true",
                    help="Abilita LG_FMU_DEBUG=1 (output verbose dal C della FMU)")
    args = ap.parse_args()

    if args.debug:
        os.environ['LG_FMU_DEBUG'] = '1'

    sim = LgCosim(args.config)
    if args.stop_time is not None: sim.t_end   = args.stop_time
    if args.step_size is not None: sim.h       = args.step_size
    if args.speedup   is not None: sim.speedup = args.speedup or None

    sys.exit(0 if sim.run() == 0 else 1)


if __name__ == "__main__":
    main()
