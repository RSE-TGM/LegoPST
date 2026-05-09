from OMSimulator import capi
import os
import glob
import shutil
import json
import csv

class OMSMasterPro:
    def __init__(self, config_path):
        self.oms = capi.Capi
        self.work_dir = os.path.abspath(".")
        
        # 1. Caricamento Configurazione
        if not os.path.exists(config_path):
            raise FileNotFoundError(f"Configurazione {config_path} non trovata!")
        with open(config_path, 'r') as f:
            self.config = json.load(f)
        
        self.model_name = self.config["model_name"]
        self.fmu_instances = self.config["fmus"] # Dizionario con tutte le FMU
        
        # 2. Inizializzazione Motore OMS
        self._check_status(self.oms.setTempDirectory(self.work_dir), "Set Temp Dir")
        self._check_status(self.oms.newModel(self.model_name), "New Model")
        self._check_status(self.oms.addSystem(f"{self.model_name}.root", 1), "Add System")
        
        self._setup_model()

    def _check_status(self, status, action):
        if status > 1:
            raise RuntimeError(f"[FATAL ERROR] {action} fallita (Status {status}).")

    def _setup_model(self):
        """Costruisce l'architettura Multi-FMU"""
        # Aggiunta di tutte le FMU definite
        for name, path in self.fmu_instances.items():
            print(f"Caricamento FMU: {name} da {path}")
            self._check_status(self.oms.addSubModel(f"{self.model_name}.root.{name}", path), f"Add {name}")
        
        # Connessioni tra le FMU
        for src, targets in self.config.get("connections", {}).items():
            if isinstance(targets, str): targets = [targets]
            for t in targets:
                self._check_status(
                    self.oms.addConnection(f"{self.model_name}.root.{src}", f"{self.model_name}.root.{t}"),
                    f"Connect {src}->{t}"
                )
        
        # Parametri e Condizioni Iniziali per tutti i componenti
        all_params = {**self.config.get("parameters", {}), **self.config.get("boundary_conditions", {})}
        for p_name, p_val in all_params.items():
            self.set_value(p_name, p_val)

    def set_value(self, var_path, value):
        full_path = f"{self.model_name}.root.{var_path}"
        if isinstance(value, float): self.oms.setReal(full_path, value)
        elif isinstance(value, bool): self.oms.setBoolean(full_path, value)
        elif isinstance(value, int): self.oms.setInteger(full_path, value)

    def get_value(self, var_path, var_type='Real'):
        full_path = f"{self.model_name}.root.{var_path}"
        try:
            if var_type == 'Real': return self.oms.getReal(full_path)[0]
            if var_type == 'Boolean': return self.oms.getBoolean(full_path)[0]
            if var_type == 'Integer': return self.oms.getInteger(full_path)[0]
        except: return 0.0

    def harvest_resources(self, output_dir):
        """Recupera i file prodotti da OGNI FMU separatamente"""
        folders = glob.glob(os.path.join(self.work_dir, f"{self.model_name}-*"))
        if not folders: return
        base_temp = max(folders, key=os.path.getmtime)
        
        # OMSimulator numera le FMU in ordine di inserimento (0001, 0002, ...)
        for idx, name in enumerate(self.fmu_instances.keys(), 1):
            src = os.path.join(base_temp, "temp", f"{idx:04d}_{name}", "resources")
            if os.path.exists(src):
                dst = os.path.join(output_dir, name) # Sottocartella specifica per FMU
                if not os.path.exists(dst): os.makedirs(dst)
                for item in os.listdir(src):
                    shutil.copy2(os.path.join(src, item), os.path.join(dst, item))

    def run(self):
        s = self.config["settings"]
        
        # Setup Log Ibrido (Logghiamo dati da entrambe le FMU)
        log_f = open("log_ibrido.csv", mode='w', newline='')
        writer = csv.writer(log_f)
        writer.writerow(["time", "controller_out", "mdc_in", "mdc_res_python"]) 

        print(f"--- Avvio Simulazione: {self.model_name} ---")
        self._check_status(self.oms.instantiate(self.model_name), "Instantiate")
        self._check_status(self.oms.initialize(self.model_name), "Initialize")

        curr_t = 0.0
        last_h = 0.0
        
        try:
            while curr_t <= s["stop_time"]:
                self.oms.stepUntil(self.model_name, curr_t)
                
                # --- SOMETHING-IN-THE-LOOP ---
                # Leggiamo da una FMU e calcoliamo qualcosa per l'altra
                val_ctrl = self.get_value("CONTROLLER.output_signal")
                val_mdc = self.get_value("MDC.input_signal")
                
                # Calcolo Python (es. efficienza calcolata esternamente)
                py_res = (val_ctrl * 0.95) + 0.1 
                
                # [OPTIMIZATION TIP] Se py_res deve tornare in una FMU:
                # self.set_value("MDC.external_adjustment", py_res)

                # Scrittura Log
                writer.writerow([curr_t, val_ctrl, val_mdc, py_res])

                # Harvesting Risorse (copia i file prodotti mentre girano)
                if (curr_t - last_h) >= s["harvest_interval"]:
                    self.harvest_resources("RISULTATI_SISTEMA")
                    last_h = curr_t
                
                print(f"Progresso: {curr_t:.1f}/{s['stop_time']} s", end='\r')
                curr_t += s["step_size"]

        finally:
            log_f.close()
            self.oms.terminate(self.model_name)
            print(f"\nSimulazione finita. Risultati in 'RISULTATI_SISTEMA/'")

if __name__ == "__main__":
    master = OMSMasterPro("config.json")
    master.run()
