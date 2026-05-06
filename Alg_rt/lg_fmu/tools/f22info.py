#!/usr/bin/env python3
"""
f22info.py — legge l'header di f22circ.dat e stampa statistiche.

Uso:
  f22info.py /path/to/f22circ          # senza .dat (come graphics)
  f22info.py /path/to/f22circ.dat      # con .dat
  f22info.py /path/to/task_dir/        # cerca f22circ.dat nella dir

Struttura file (Linux):
  [0]      HEADER_REGISTRAZIONI (2156 B)
  [2156]   F22CIRC_HD           (40 B): p_iniz, p_fine, num_campioni, num_var_graf, ...
  [2196]   num_var_graf × F22CIRC_VAR  (80 B ciascuno: 9 nome + 71 descr)
  [data]   buffer circolare: num_campioni record da (num_var_graf+1)*4 B

Nota: num_var_graf = _NUM_VAR (dimensionamento SHM della task), NON il numero di
variabili grafiche reali. Con F22_ALLOC/PIACENZA compilato nel binario net_prepf22,
variabili_registrate = num_var (= _NUM_VAR), riempiendo con "libera" gli slot non
selezionati. Le variabili effettive sono quelle con nome non vuoto/libera.
"""
import struct, os, sys

SIZEOF_HEADER_REG = 2156
SIZEOF_F22CIRC_HD = 40
SIZEOF_F22CIRC_VAR = 80

def find_dat(arg):
    if os.path.isdir(arg):
        p = os.path.join(arg, "f22circ.dat")
        return p if os.path.isfile(p) else None
    base = arg[:-4] if arg.endswith(".dat") else arg
    p = base + ".dat"
    return p if os.path.isfile(p) else None

def f22info(path):
    size = os.path.getsize(path)
    with open(path, 'rb') as f:
        f.seek(SIZEOF_HEADER_REG)
        p_iniz, p_fine, num_campioni, num_var_graf, \
            ore, minuti, secondi, giorno, mese, anno = struct.unpack('<10i', f.read(40))

        nomi = []
        for _ in range(num_var_graf):
            raw = f.read(SIZEOF_F22CIRC_VAR)
            nome = raw[:9].rstrip(b'\x00 ').decode('latin-1', errors='replace')
            desc = raw[9:80].rstrip(b'\x00 ').decode('latin-1', errors='replace')
            nomi.append((nome, desc))
        offdata = f.tell()

        rec_size = (num_var_graf + 1) * 4
        file_full = (p_iniz != 1)
        n = num_campioni if file_full else p_fine

        # t del primo e ultimo campione valido
        f.seek(offdata + (p_iniz - 1) * rec_size)
        t0 = struct.unpack('<f', f.read(4))[0]
        if n > 1:
            f.seek(offdata + (p_iniz % num_campioni) * rec_size)
            t1 = struct.unpack('<f', f.read(4))[0]
            f.seek(offdata + ((p_fine - 1) % num_campioni) * rec_size)
            tlast = struct.unpack('<f', f.read(4))[0]
        else:
            t1 = tlast = t0

    dt = t1 - t0

    # num_var_graf = _NUM_VAR (slot totali per record = dimensionamento SHM).
    # Con F22_ALLOC/PIACENZA nel binario, gli slot non selezionati sono riempiti
    # con nome "libera": le variabili reali sono solo quelle con nome non vuoto.
    PADDING = {'', 'libera', '-----'}
    nomi_reali = [(n, d) for n, d in nomi if n not in PADDING]

    print(f"File:           {path}")
    print(f"Creato:         {giorno:02d}/{mese:02d}/{anno:04d}  {ore:02d}:{minuti:02d}:{secondi:02d}")
    print(f"Slot/record:    {num_var_graf}  (= _NUM_VAR, dimensionamento SHM; rec_size={rec_size} B)")
    print(f"Var. effettive: {len(nomi_reali)}  (slot con nome reale, selezionate in recorder.edf)")
    print(f"Capacità:       {num_campioni} campioni")
    print(f"Registrati:     {n}  (p_iniz={p_iniz}, p_fine={p_fine}, full={file_full})")
    print(f"t_start:        {t0:.4f} s")
    print(f"t_end:          {tlast:.4f} s")
    print(f"dt:             {dt:.4f} s")
    print(f"T_tot:          {tlast - t0:.2f} s")
    print(f"File:           {size:,} B  (header={offdata} B, dati={size-offdata:,} B)")
    if nomi_reali:
        print(f"\nVariabili effettive ({len(nomi_reali)}):")
        for nome, desc in nomi_reali[:20]:
            print(f"  {nome:12s}  {desc[:60]}")
        if len(nomi_reali) > 20:
            print(f"  ... ({len(nomi_reali) - 20} ulteriori)")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: f22info.py <f22circ[.dat] | task_dir>", file=sys.stderr)
        sys.exit(1)
    path = find_dat(sys.argv[1])
    if path is None:
        print(f"ERR: f22circ.dat non trovato: {sys.argv[1]}", file=sys.stderr)
        sys.exit(2)
    f22info(path)
