#!/usr/bin/env python3
"""
lglinux2win.py - Converte directory da formato Linux a formato Windows

Struttura sorgente (directory legocad):
    legocad/
    ├── libut/      -> <dest>/libut
    ├── libgraph/   -> <dest>/libgraph
    ├── collet/     -> <dest>/models/collet (modello)
    └── pum/        -> <dest>/models/pum (modello)

Regole di conversione per libut:
1. Crea directory <dest>/libut
2. *.f -> *.for (rinomina estensione Fortran)
3. lista_moduli.dat -> l_moduli.dat
4. lista_complementari.dat -> l_aux.dat
5. *.c, *.h -> copiati con stesso nome
6. Converte line-ending da LF (Linux) a CRLF (Windows)
7. Ignora file .o, .so, .a, .sh

Regole di conversione per libgraph:
1. Crea directory <dest>/libgraph (con tutte le sottodirectory)
2. File di testo -> conversione line-ending LF -> CRLF
3. File binari (gif, jpeg, ecc.) -> copiati intatti
4. lista_moduli.dat -> l_moduli.dat, lista_complementari.dat -> l_aux.dat

Regole di conversione per models:
1. Per ogni directory in sorgente (esclusi libut, libgraph) crea <dest>/models/<modello>
2. Trasferisce solo: *.tom, f01.dat, f14.dat, *.f (-> *.for)
3. NON trasferisce directory proc/

Uso:
    python lglinux2win.py [sorgente] [destinazione] [-y]

Parametri:
    sorgente     Directory sorgente (default: legocad)
    destinazione Directory destinazione Windows (default: <sorgente>_user_win)
    -y           Esegui senza chiedere conferma

Esempio:
    python lglinux2win.py
    -> Converte legocad/ -> legocad_user_win/

Output:
    Crea la directory destinazione e produce un archivio zip

Esempio:
    python lglinux2win.py
    -> Converte legocad/ -> legocad_user_win/
    -> Produce legocad_user_win.zip

    python lglinux2win.py pippo
    -> Converte pippo/ -> pippo_user_win/
    -> Produce pippo_user_win.zip
"""

import os
import sys
import argparse
import shutil
import zipfile
from pathlib import Path


# Mappatura nomi file speciali Linux -> Windows
FILE_RENAME_MAP = {
    'lista_moduli.dat': 'l_moduli.dat',
    'lista_complementari.dat': 'l_aux.dat',
}

# Estensioni Fortran
FORTRAN_EXT_LINUX = '.f'
FORTRAN_EXT_WIN = '.for'

# Estensioni da copiare direttamente in libut (con conversione line-ending)
COPY_EXTENSIONS_LIBUT = {'.c', '.h'}

# Estensioni da ignorare in libut (tipiche Linux)
IGNORE_EXTENSIONS_LIBUT = {'.o', '.so', '.a', '.sh'}


def convert_line_endings_to_windows(content_bytes):
    """Converte LF -> CRLF (evitando doppia conversione)"""
    # Prima normalizza tutto a LF
    content = content_bytes.replace(b'\r\n', b'\n')
    # Poi converte a CRLF
    return content.replace(b'\n', b'\r\n')


def is_text_file(filepath):
    """Verifica se un file e' di testo (non binario)"""
    try:
        with open(filepath, 'rb') as f:
            chunk = f.read(8192)
            # Se contiene byte nulli, probabilmente e' binario
            if b'\x00' in chunk:
                return False
        return True
    except:
        return False


def get_windows_filename(linux_filename):
    """
    Determina il nome file Windows a partire dal nome Linux.
    Applica le regole di rinomina.
    """
    name_lower = linux_filename.lower()

    # Controlla se e' un file da rinominare
    if name_lower in FILE_RENAME_MAP:
        return FILE_RENAME_MAP[name_lower]

    # Controlla estensione Fortran
    base, ext = os.path.splitext(linux_filename)
    if ext.lower() == FORTRAN_EXT_LINUX:
        return base + FORTRAN_EXT_WIN

    return linux_filename


def should_process_file_libut(filename):
    """Determina se un file deve essere processato per libut"""
    base, ext = os.path.splitext(filename)
    ext_lower = ext.lower()

    # Ignora file con estensioni nella lista di esclusione
    if ext_lower in IGNORE_EXTENSIONS_LIBUT:
        return False

    # Processa file Fortran
    if ext_lower == FORTRAN_EXT_LINUX:
        return True

    # Processa file C/H
    if ext_lower in COPY_EXTENSIONS_LIBUT:
        return True

    # Processa file .dat rinominabili
    if filename.lower() in FILE_RENAME_MAP:
        return True

    return False


def convert_libut_linux2win(src_base, dst_base):
    """
    Converte la directory libut da Linux a Windows.
    """
    src_libut = src_base / 'libut'
    if not src_libut.exists():
        print(f"AVVISO: Directory '{src_libut}' non esiste, salto libut")
        return True

    dst_libut = dst_base / 'libut'

    # Crea directory destinazione
    dst_libut.mkdir(parents=True, exist_ok=True)
    print(f"\n{'='*50}")
    print(f"CONVERSIONE LIBUT")
    print(f"{'='*50}")
    print(f"Sorgente:     {src_libut}")
    print(f"Destinazione: {dst_libut}")
    print(f"{'-'*50}")

    # Statistiche
    stats = {
        'fortran': 0,
        'c_files': 0,
        'h_files': 0,
        'dat_renamed': 0,
        'skipped': 0,
        'errors': 0
    }

    # Processa tutti i file
    for src_file in src_libut.iterdir():
        if not src_file.is_file():
            continue

        filename = src_file.name

        if not should_process_file_libut(filename):
            print(f"  SKIP: {filename}")
            stats['skipped'] += 1
            continue

        try:
            # Determina nome destinazione
            dst_filename = get_windows_filename(filename)
            dst_file = dst_libut / dst_filename

            # Leggi contenuto
            with open(src_file, 'rb') as f:
                content = f.read()

            # Converti line-ending se e' un file di testo
            if is_text_file(src_file):
                content = convert_line_endings_to_windows(content)

            # Scrivi file destinazione
            with open(dst_file, 'wb') as f:
                f.write(content)

            # Aggiorna statistiche
            ext = src_file.suffix.lower()
            if ext == FORTRAN_EXT_LINUX:
                stats['fortran'] += 1
                action = f"-> {dst_filename}"
            elif ext == '.c':
                stats['c_files'] += 1
                action = "(copiato)"
            elif ext == '.h':
                stats['h_files'] += 1
                action = "(copiato)"
            elif filename.lower() in FILE_RENAME_MAP:
                stats['dat_renamed'] += 1
                action = f"-> {dst_filename}"
            else:
                action = ""

            print(f"  OK: {filename} {action}")

        except Exception as e:
            print(f"  ERRORE: {filename} - {e}")
            stats['errors'] += 1

    # Report
    print(f"{'-'*50}")
    print(f"File Fortran (.f -> .for):  {stats['fortran']}")
    print(f"File C (.c):                {stats['c_files']}")
    print(f"File Header (.h):           {stats['h_files']}")
    print(f"File .dat rinominati:       {stats['dat_renamed']}")
    print(f"File ignorati:              {stats['skipped']}")
    print(f"Errori:                     {stats['errors']}")

    return stats['errors'] == 0


def convert_libgraph_linux2win(src_base, dst_base):
    """
    Converte la directory libgraph da Linux a Windows.
    Copia ricorsivamente tutte le sottodirectory.
    """
    src_libgraph = src_base / 'libgraph'
    if not src_libgraph.exists():
        print(f"AVVISO: Directory '{src_libgraph}' non esiste, salto libgraph")
        return True

    dst_libgraph = dst_base / 'libgraph'

    print(f"\n{'='*50}")
    print(f"CONVERSIONE LIBGRAPH")
    print(f"{'='*50}")
    print(f"Sorgente:     {src_libgraph}")
    print(f"Destinazione: {dst_libgraph}")
    print(f"{'-'*50}")

    # Statistiche
    stats = {
        'text_files': 0,
        'binary_files': 0,
        'dat_renamed': 0,
        'directories': 0,
        'errors': 0
    }

    # Processa ricorsivamente
    for src_path in src_libgraph.rglob('*'):
        # Calcola path relativo
        rel_path = src_path.relative_to(src_libgraph)

        if src_path.is_dir():
            # Crea directory
            dst_dir = dst_libgraph / rel_path
            dst_dir.mkdir(parents=True, exist_ok=True)
            stats['directories'] += 1
            continue

        if not src_path.is_file():
            continue

        try:
            # Determina nome destinazione (applica rinominazioni)
            filename = src_path.name
            dst_filename = get_windows_filename(filename)

            # Path destinazione
            dst_file = dst_libgraph / rel_path.parent / dst_filename

            # Assicura che la directory esista
            dst_file.parent.mkdir(parents=True, exist_ok=True)

            # Leggi contenuto
            with open(src_path, 'rb') as f:
                content = f.read()

            # Converti line-ending solo se e' un file di testo
            if is_text_file(src_path):
                content = convert_line_endings_to_windows(content)
                stats['text_files'] += 1
                file_type = "testo"
            else:
                stats['binary_files'] += 1
                file_type = "binario"

            # Scrivi file destinazione
            with open(dst_file, 'wb') as f:
                f.write(content)

            # Rinominato?
            if filename.lower() in FILE_RENAME_MAP:
                stats['dat_renamed'] += 1
                print(f"  OK: {rel_path} -> {dst_filename} ({file_type})")
            else:
                print(f"  OK: {rel_path} ({file_type})")

        except Exception as e:
            print(f"  ERRORE: {rel_path} - {e}")
            stats['errors'] += 1

    # Report
    print(f"{'-'*50}")
    print(f"Directory create:           {stats['directories']}")
    print(f"File di testo convertiti:   {stats['text_files']}")
    print(f"File binari copiati:        {stats['binary_files']}")
    print(f"File .dat rinominati:       {stats['dat_renamed']}")
    print(f"Errori:                     {stats['errors']}")

    return stats['errors'] == 0


def should_process_file_models(filename, rel_path):
    """
    Determina se un file in models deve essere processato.
    Regole:
    - Trasferisce *.tom
    - Trasferisce f01.dat, f14.dat
    - Trasferisce *.f (-> *.for)
    - NON trasferisce nulla dentro proc/
    """
    # Escludi tutto cio' che e' dentro una directory 'proc'
    path_parts = rel_path.parts
    if 'proc' in path_parts:
        return False

    name_lower = filename.lower()
    base, ext = os.path.splitext(filename)
    ext_lower = ext.lower()

    # Trasferisci file .tom
    if ext_lower == '.tom':
        return True

    # Trasferisci f01.dat e f14.dat
    if name_lower in ('f01.dat', 'f14.dat'):
        return True

    # Trasferisci file Fortran (.f su Linux)
    if ext_lower == '.f':
        return True

    return False


def convert_models_linux2win(src_base, dst_base):
    """
    Converte le directory dei modelli da Linux a Windows.
    Cerca i modelli nella directory sorgente (esclusi libut, libgraph).
    Crea <dest>/models/<modello>/ per ogni modello trovato.
    Processa solo *.tom, f01.dat, f14.dat, *.f (-> *.for), escludendo proc/.
    """
    # Directory da escludere (non sono modelli)
    EXCLUDE_DIRS = {'libut', 'libgraph'}

    # Trova le directory dei modelli nella sorgente
    model_dirs = []
    for item in src_base.iterdir():
        if item.is_dir() and item.name.lower() not in EXCLUDE_DIRS:
            model_dirs.append(item)

    if not model_dirs:
        print(f"AVVISO: Nessun modello trovato in '{src_base}'")
        return True

    dst_models = dst_base / 'models'

    print(f"\n{'='*50}")
    print(f"CONVERSIONE MODELLI -> MODELS")
    print(f"{'='*50}")
    print(f"Sorgente:     {src_base}")
    print(f"Destinazione: {dst_models}")
    print(f"Modelli trovati: {[d.name for d in model_dirs]}")
    print(f"{'-'*50}")

    # Statistiche
    stats = {
        'tom_files': 0,
        'dat_files': 0,
        'fortran_files': 0,
        'directories': 0,
        'skipped': 0,
        'errors': 0
    }

    # Processa ogni directory modello
    for model_dir in model_dirs:
        model_name = model_dir.name

        # Processa ricorsivamente i file del modello
        for src_path in model_dir.rglob('*'):
            # Calcola path relativo rispetto alla directory del modello
            rel_path = src_path.relative_to(model_dir)

            # Salta directory proc
            if 'proc' in rel_path.parts:
                continue

            if src_path.is_dir():
                # Crea directory (escluso proc)
                if src_path.name.lower() != 'proc':
                    dst_dir = dst_models / model_name / rel_path
                    dst_dir.mkdir(parents=True, exist_ok=True)
                    stats['directories'] += 1
                continue

            if not src_path.is_file():
                continue

            filename = src_path.name

            # Verifica se il file deve essere processato
            # Costruisci rel_path completo per should_process_file_models
            full_rel_path = Path(model_name) / rel_path
            if not should_process_file_models(filename, full_rel_path):
                stats['skipped'] += 1
                continue

            # Verifica che sia un file di testo
            if not is_text_file(src_path):
                print(f"  SKIP (binario): {model_name}/{rel_path}")
                stats['skipped'] += 1
                continue

            try:
                # Determina nome destinazione
                dst_filename = get_windows_filename(filename)

                # Path destinazione
                dst_file = dst_models / model_name / rel_path.parent / dst_filename

                # Assicura che la directory esista
                dst_file.parent.mkdir(parents=True, exist_ok=True)

                # Leggi contenuto
                with open(src_path, 'rb') as f:
                    content = f.read()

                # Converti line-ending
                content = convert_line_endings_to_windows(content)

                # Scrivi file destinazione
                with open(dst_file, 'wb') as f:
                    f.write(content)

                # Aggiorna statistiche
                ext = src_path.suffix.lower()
                if ext == '.tom':
                    stats['tom_files'] += 1
                    action = "(testo)"
                elif filename.lower() in ('f01.dat', 'f14.dat'):
                    stats['dat_files'] += 1
                    action = "(testo)"
                elif ext == '.f':
                    stats['fortran_files'] += 1
                    action = f"-> {dst_filename}"
                else:
                    action = ""

                print(f"  OK: {model_name}/{rel_path} {action}")

            except Exception as e:
                print(f"  ERRORE: {model_name}/{rel_path} - {e}")
                stats['errors'] += 1

    # Report
    print(f"{'-'*50}")
    print(f"Directory create:           {stats['directories']}")
    print(f"File .tom:                  {stats['tom_files']}")
    print(f"File .dat (f01, f14):       {stats['dat_files']}")
    print(f"File Fortran (.f -> .for):  {stats['fortran_files']}")
    print(f"File ignorati:              {stats['skipped']}")
    print(f"Errori:                     {stats['errors']}")

    return stats['errors'] == 0


def convert_linux2win(src_base_dir, dst_base_dir=None):
    """
    Converte le directory libut, libgraph e models da Linux a Windows.

    Args:
        src_base_dir: Directory sorgente (es. 'legocad')
        dst_base_dir: Directory destinazione (opzionale, default: <sorgente>_user_win)
    """
    src_base = Path(src_base_dir)

    if not src_base.exists():
        print(f"ERRORE: Directory sorgente '{src_base}' non esiste")
        return False

    # Crea nome directory destinazione
    if dst_base_dir:
        dst_base = Path(dst_base_dir)
    else:
        src_name = src_base.name
        dst_name = src_name + '_win'
        dst_base = src_base.parent / dst_name

    print(f"Conversione Linux -> Windows")
    print(f"Sorgente:     {src_base}")
    print(f"Destinazione: {dst_base}")

    # Converti libut
    success_libut = convert_libut_linux2win(src_base, dst_base)

    # Converti libgraph
    success_libgraph = convert_libgraph_linux2win(src_base, dst_base)

    # Converti models -> simulators
    success_models = convert_models_linux2win(src_base, dst_base)

    # Report finale
    print(f"\n{'='*50}")
    print("CONVERSIONE COMPLETATA")
    print(f"{'='*50}")

    success = success_libut and success_libgraph and success_models

    # Crea archivio zip
    if success:
        zip_name = f"{dst_base}.zip"
        print(f"\nCreazione archivio: {zip_name}")
        try:
            with zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED) as zipf:
                for root, dirs, files in os.walk(dst_base):
                    for file in files:
                        file_path = Path(root) / file
                        arcname = file_path.relative_to(dst_base.parent)
                        zipf.write(file_path, arcname)
            print(f"Archivio creato: {zip_name}")
        except Exception as e:
            print(f"ERRORE creazione archivio: {e}")
            success = False

    return success


def main():
    parser = argparse.ArgumentParser(
        prog='lglinux2win.py',
        description='Converte applicazioni Legopc da Linux a Windows.',
        epilog='''
Struttura sorgente:
  <src>/libut/      -> <dest>/libut
  <src>/libgraph/   -> <dest>/libgraph
  <src>/<modello>/  -> <dest>/models/<modello>

Regole di conversione:
  libut:      *.f -> *.for, lista_moduli.dat -> l_moduli.dat,
              lista_complementari.dat -> l_aux.dat, LF -> CRLF
  libgraph:   copia ricorsiva, conversione line-ending per file testo
  models:     *.tom, f01.dat, f14.dat, *.f -> *.for (escluso proc/)

Output: produce directory + archivio zip

Esempi:
  python lglinux2win.py                     # -> legocad_user_win.zip
  python lglinux2win.py pippo               # -> pippo_user_win.zip
  python lglinux2win.py pippo pippo_win     # -> pippo_win.zip
        ''',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        'sorgente',
        nargs='?',
        default='legocad',
        help='Directory sorgente (default: legocad)'
    )
    parser.add_argument(
        'destinazione',
        nargs='?',
        default=None,
        help='Directory destinazione Windows (default: <sorgente>_user_win)'
    )
    parser.add_argument(
        '-y', '--yes',
        action='store_true',
        help='Esegui senza chiedere conferma'
    )

    args = parser.parse_args()

    # Determina nome destinazione
    src_name = args.sorgente
    if args.destinazione:
        dst_name = args.destinazione
    else:
        dst_name = f"{src_name}_user_win"

    # Mostra comando e chiedi conferma
    print(f"\nComando: python lglinux2win.py {src_name} {dst_name}")
    print(f"  Input:  {src_name}/")
    print(f"  Output: {dst_name}/")

    if not args.yes:
        risposta = input("\nProcedere? [s/N]: ").strip().lower()
        if risposta not in ('s', 'si', 'y', 'yes'):
            print("Operazione annullata.")
            sys.exit(0)

    print()
    success = convert_linux2win(src_name, dst_name)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
