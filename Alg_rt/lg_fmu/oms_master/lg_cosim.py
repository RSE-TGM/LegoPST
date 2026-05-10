#!/usr/bin/env python3
"""
lg_cosim.py — Co-simulation master FMI 2.0 per FMU LegoPST
Gestisce N FMU in co-simulazione: scambio variabili, sync real-time/accelerato.

Uso:
    python3 lg_cosim.py [config.json]
    python3 lg_cosim.py config.json --stop-time 60 --speedup 1.0

Speedup: null/0 = massima velocità, 1.0 = real-time, 2.0 = 2× real-time
"""

import json, os, sys, csv, time, shutil, tempfile, argparse
from fmpy import read_model_description, extract
from fmpy.fmi2 import FMU2Slave


class _FMUInstance:
    def __init__(self, name, fmu_path):
        self.name = name
        self.unzip_dir = tempfile.mkdtemp(prefix=f"lg_cosim_{name}_")
        extract(fmu_path, unzipdir=self.unzip_dir)
        md = read_model_description(fmu_path)
        self.guid     = md.guid
        self.model_id = md.coSimulation.modelIdentifier
        self.vr    = {v.name: v.valueReference for v in md.modelVariables}
        self.vtype = {v.name: v.type           for v in md.modelVariables}
        self.slave = None

    def instantiate(self):
        self.slave = FMU2Slave(
            guid=self.guid, unzipDirectory=self.unzip_dir,
            modelIdentifier=self.model_id, instanceName=self.name)
        self.slave.instantiate()

    def setup_experiment(self, t0, t_end):
        self.slave.setupExperiment(startTime=t0, stopTime=t_end)
        self.slave.enterInitializationMode()

    def exit_init(self):
        self.slave.exitInitializationMode()

    def step(self, t, h):
        self.slave.doStep(currentCommunicationPoint=t, communicationStepSize=h)

    def get(self, var):
        vr, vt = [self.vr[var]], self.vtype[var]
        if vt == 'Real':    return self.slave.getReal(vr)[0]
        if vt == 'Integer': return self.slave.getInteger(vr)[0]
        if vt == 'Boolean': return self.slave.getBoolean(vr)[0]
        return self.slave.getReal(vr)[0]

    def set(self, var, value):
        vr, vt = [self.vr[var]], self.vtype[var]
        if vt == 'Real':    self.slave.setReal(vr, [float(value)])
        elif vt == 'Integer': self.slave.setInteger(vr, [int(value)])
        elif vt == 'Boolean': self.slave.setBoolean(vr, [bool(value)])

    def terminate(self):
        if self.slave:
            try: self.slave.terminate(); self.slave.freeInstance()
            except: pass
            self.slave = None

    def cleanup(self):
        shutil.rmtree(self.unzip_dir, ignore_errors=True)


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
        self.log_file = s.get("log_file", "log_cosim.csv")
        self.log_vars = s.get("log_vars", [])  # ["FMU.VAR", ...]

        self.connections  = cfg.get("connections", [])   # [{"from":"A.X","to":"B.Y"}, ...]
        self.start_values = cfg.get("start_values", {})  # {"A.VAR": val, ...}

        self.fmus = {}
        for name, path in cfg["fmus"].items():
            if not os.path.isabs(path):
                path = os.path.join(self._cfg_dir, path)
            print(f"[lg_cosim] caricamento {name}: {os.path.basename(path)}")
            self.fmus[name] = _FMUInstance(name, path)

    @staticmethod
    def _split(ref):
        """'FMU.VAR' -> ('FMU', 'VAR')"""
        dot = ref.index('.')
        return ref[:dot], ref[dot+1:]

    def step_hook(self, t):
        """Override in sottoclasse per logica Python nel loop (es. calcoli ibridi)."""
        pass

    def run(self):
        # Instantiate + init
        for inst in self.fmus.values():
            inst.instantiate()
            inst.setup_experiment(self.t0, self.t_end)

        for ref, val in self.start_values.items():
            fmu, var = self._split(ref)
            self.fmus[fmu].set(var, val)

        for inst in self.fmus.values():
            inst.exit_init()

        log_f  = open(self.log_file, "w", newline="")
        writer = csv.writer(log_f)
        writer.writerow(["time"] + self.log_vars)

        t = self.t0
        wall_start = time.monotonic()
        overruns   = 0
        n_steps    = 0

        speedup_label = (f"{self.speedup}×RT" if self.speedup else "max")
        print(f"[lg_cosim] avvio: t_end={self.t_end} h={self.h} speedup={speedup_label}")

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

                # Sync real-time / accelerato
                if self.speedup:
                    t_expected = wall_start + (t - self.t0) / self.speedup
                    slack = t_expected - time.monotonic()
                    if slack > 1e-4:
                        time.sleep(slack)
                    elif slack < -0.005:
                        overruns += 1

                print(f"\r[lg_cosim] t={t:.2f}/{self.t_end:.1f}s", end="", flush=True)

        finally:
            log_f.close()
            for inst in self.fmus.values():
                inst.terminate()
                inst.cleanup()

        print(f"\n[lg_cosim] fine — {n_steps} passi, log → {self.log_file}")
        if overruns:
            pct = overruns / n_steps * 100
            print(f"[lg_cosim] attenzione: {overruns} overrun real-time ({pct:.1f}% dei passi)")
        return overruns


def main():
    ap = argparse.ArgumentParser(description="LegoPST FMU co-simulation master")
    ap.add_argument("config", nargs="?", default="config.json")
    ap.add_argument("--stop-time", type=float, metavar="T",   help="Override stop_time")
    ap.add_argument("--step-size", type=float, metavar="H",   help="Override step_size")
    ap.add_argument("--speedup",   type=float, metavar="S",
                    help="Override speedup: 1.0=RT, 2.0=2×RT, 0=max")
    args = ap.parse_args()

    sim = LgCosim(args.config)
    if args.stop_time is not None: sim.t_end   = args.stop_time
    if args.step_size is not None: sim.h       = args.step_size
    if args.speedup   is not None: sim.speedup = args.speedup or None

    sys.exit(0 if sim.run() == 0 else 1)


if __name__ == "__main__":
    main()
