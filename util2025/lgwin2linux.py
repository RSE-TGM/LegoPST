#!/usr/bin/env python3
"""
lgwin2linux.py - Converte directory libut, libgraph e models da formato Windows a formato Linux

Regole di conversione per libut:
1. Crea directory <dest>/libut
2. *.for -> *.f (rinomina estensione Fortran)
3. l_moduli.dat -> lista_moduli.dat
4. l_aux.dat -> lista_complementari.dat
5. *.c, *.h -> copiati con stesso nome
6. Converte line-ending da CRLF (Windows) a LF (Linux)
7. Ignora file .bat, .lib, .obj, .exe, .dll

Regole di conversione per libgraph:
1. Crea directory <dest>/libgraph (con tutte le sottodirectory)
2. File di testo -> conversione line-ending CRLF -> LF
3. File binari (gif, jpeg, ecc.) -> copiati intatti
4. l_moduli.dat -> lista_moduli.dat, l_aux.dat -> lista_complementari.dat

Regole di conversione per models:
1. Le sottodirectory dei modelli vanno direttamente nella root destinazione
   (es. user/models/collet -> user_linux/collet)
2. Trasferisce solo file di testo
3. Trasferisce *.tom, f01.dat, f14.dat
4. *.for -> *.f (rinomina estensione Fortran)
5. NON trasferisce directory proc/

Uso:
    python lgwin2linux.py [sorgente] [destinazione] [-y]

Parametri:
    sorgente     Directory sorgente Windows (default: user)
    destinazione Directory destinazione Linux (default: <sorgente>_legocad_linux)
    -y           Esegui senza chiedere conferma

Output:
    Crea la directory destinazione e produce un archivio tar.gz

Esempio:
    python lgwin2linux.py
    -> Converte user/ -> user_legocad_linux/
    -> Produce user_legocad_linux.tar.gz

    python lgwin2linux.py pippo
    -> Converte pippo/ -> pippo_legocad_linux/
    -> Produce pippo_legocad_linux.tar.gz
"""

import os
import sys
import argparse
import shutil
import tarfile
from pathlib import Path


# Mappatura nomi file speciali Windows -> Linux
FILE_RENAME_MAP = {
    'l_moduli.dat': 'lista_moduli.dat',
    'l_aux.dat': 'lista_complementari.dat',
}

# Estensioni da processare in libut
FORTRAN_EXT = '.for'
FORTRAN_EXT_LINUX = '.f'

# Estensioni da copiare direttamente in libut (con conversione line-ending)
COPY_EXTENSIONS_LIBUT = {'.c', '.h'}

# Estensioni da ignorare in libut
IGNORE_EXTENSIONS_LIBUT = {'.bat', '.lib', '.obj', '.exe', '.dll'}


def convert_line_endings_to_unix(content_bytes):
    """Converte CRLF -> LF"""
    return content_bytes.replace(b'\r\n', b'\n')


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


def get_linux_filename(win_filename):
    """
    Determina il nome file Linux a partire dal nome Windows.
    Applica le regole di rinomina.
    """
    name_lower = win_filename.lower()

    # Controlla se e' un file da rinominare
    if name_lower in FILE_RENAME_MAP:
        return FILE_RENAME_MAP[name_lower]

    # Controlla estensione Fortran
    base, ext = os.path.splitext(win_filename)
    if ext.lower() == FORTRAN_EXT:
        return base + FORTRAN_EXT_LINUX

    return win_filename


def should_process_file_libut(filename):
    """Determina se un file deve essere processato per libut"""
    base, ext = os.path.splitext(filename)
    ext_lower = ext.lower()

    # Ignora file con estensioni nella lista di esclusione
    if ext_lower in IGNORE_EXTENSIONS_LIBUT:
        return False

    # Processa file Fortran
    if ext_lower == FORTRAN_EXT:
        return True

    # Processa file C/H
    if ext_lower in COPY_EXTENSIONS_LIBUT:
        return True

    # Processa file .dat rinominabili
    if filename.lower() in FILE_RENAME_MAP:
        return True

    return False


def convert_libut_win2linux(src_base, dst_base):
    """
    Converte la directory libut da Windows a Linux.
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
            dst_filename = get_linux_filename(filename)
            dst_file = dst_libut / dst_filename

            # Leggi contenuto
            with open(src_file, 'rb') as f:
                content = f.read()

            # Converti line-ending se e' un file di testo
            if is_text_file(src_file):
                content = convert_line_endings_to_unix(content)

            # Scrivi file destinazione
            with open(dst_file, 'wb') as f:
                f.write(content)

            # Aggiorna statistiche
            ext = src_file.suffix.lower()
            if ext == FORTRAN_EXT:
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
    print(f"File Fortran (.for -> .f):  {stats['fortran']}")
    print(f"File C (.c):                {stats['c_files']}")
    print(f"File Header (.h):           {stats['h_files']}")
    print(f"File .dat rinominati:       {stats['dat_renamed']}")
    print(f"File ignorati:              {stats['skipped']}")
    print(f"Errori:                     {stats['errors']}")

    return stats['errors'] == 0


def convert_libgraph_win2linux(src_base, dst_base):
    """
    Converte la directory libgraph da Windows a Linux.
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
            dst_filename = get_linux_filename(filename)

            # Path destinazione
            dst_file = dst_libgraph / rel_path.parent / dst_filename

            # Assicura che la directory esista
            dst_file.parent.mkdir(parents=True, exist_ok=True)

            # Leggi contenuto
            with open(src_path, 'rb') as f:
                content = f.read()

            # Converti line-ending solo se e' un file di testo
            if is_text_file(src_path):
                content = convert_line_endings_to_unix(content)
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
    - Trasferisce *.for (-> *.f)
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

    # Trasferisci file Fortran
    if ext_lower == '.for':
        return True

    return False


def convert_models_win2linux(src_base, dst_base):
    """
    Converte la directory models da Windows a Linux.
    Le sottodirectory dei modelli vengono copiate direttamente nella root
    della destinazione (es. user/models/collet -> user_linux/collet).
    Processa solo le sottodirectory dei modelli, escludendo proc/.
    """
    src_models = src_base / 'models'
    if not src_models.exists():
        print(f"AVVISO: Directory '{src_models}' non esiste, salto models")
        return True

    # NOTA: i modelli vanno direttamente nella root della destinazione
    # (non in una sottodirectory 'models')

    print(f"\n{'='*50}")
    print(f"CONVERSIONE MODELS -> ROOT")
    print(f"{'='*50}")
    print(f"Sorgente:     {src_models}")
    print(f"Destinazione: {dst_base}/<modello>")
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

    # Processa ricorsivamente
    for src_path in src_models.rglob('*'):
        # Calcola path relativo
        rel_path = src_path.relative_to(src_models)

        # Salta directory proc
        if 'proc' in rel_path.parts:
            continue

        if src_path.is_dir():
            # Crea directory (escluso proc) direttamente nella root destinazione
            if src_path.name.lower() != 'proc':
                dst_dir = dst_base / rel_path
                dst_dir.mkdir(parents=True, exist_ok=True)
                stats['directories'] += 1
            continue

        if not src_path.is_file():
            continue

        filename = src_path.name

        # Verifica se il file deve essere processato
        if not should_process_file_models(filename, rel_path):
            stats['skipped'] += 1
            continue

        # Verifica che sia un file di testo
        if not is_text_file(src_path):
            print(f"  SKIP (binario): {rel_path}")
            stats['skipped'] += 1
            continue

        try:
            # Determina nome destinazione
            dst_filename = get_linux_filename(filename)

            # Path destinazione (direttamente nella root, non in models/)
            dst_file = dst_base / rel_path.parent / dst_filename

            # Assicura che la directory esista
            dst_file.parent.mkdir(parents=True, exist_ok=True)

            # Leggi contenuto
            with open(src_path, 'rb') as f:
                content = f.read()

            # Converti line-ending
            content = convert_line_endings_to_unix(content)

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
            elif ext == '.for':
                stats['fortran_files'] += 1
                action = f"-> {dst_filename}"
            else:
                action = ""

            print(f"  OK: {rel_path} {action}")

        except Exception as e:
            print(f"  ERRORE: {rel_path} - {e}")
            stats['errors'] += 1

    # Report
    print(f"{'-'*50}")
    print(f"Directory create:           {stats['directories']}")
    print(f"File .tom:                  {stats['tom_files']}")
    print(f"File .dat (f01, f14):       {stats['dat_files']}")
    print(f"File Fortran (.for -> .f):  {stats['fortran_files']}")
    print(f"File ignorati:              {stats['skipped']}")
    print(f"Errori:                     {stats['errors']}")

    return stats['errors'] == 0


def convert_win2linux(src_base_dir, dst_base_dir=None):
    """
    Converte le directory libut, libgraph e models da Windows a Linux.

    Args:
        src_base_dir: Directory base dell'applicazione (es. 'user')
        dst_base_dir: Directory destinazione (opzionale, default: <src>_legocad_linux)
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
        if src_name.endswith('_legocad_linux'):
            print(f"ERRORE: La directory sorgente sembra gia' essere in formato Linux")
            return False
        dst_base = src_base.parent / f"{src_name}_legocad_linux"

    print(f"Conversione Windows -> Linux")
    print(f"Sorgente:     {src_base}")
    print(f"Destinazione: {dst_base}")

    # Converti libut
    success_libut = convert_libut_win2linux(src_base, dst_base)

    # Converti libgraph
    success_libgraph = convert_libgraph_win2linux(src_base, dst_base)

    # Converti models
    success_models = convert_models_win2linux(src_base, dst_base)

    # Report finale
    print(f"\n{'='*50}")
    print("CONVERSIONE COMPLETATA")
    print(f"{'='*50}")

    success = success_libut and success_libgraph and success_models

    # Crea archivio tar.gz
    if success:
        tar_name = f"{dst_base}.tar.gz"
        print(f"\nCreazione archivio: {tar_name}")
        try:
            with tarfile.open(tar_name, "w:gz") as tar:
                tar.add(dst_base, arcname=dst_base.name)
            print(f"Archivio creato: {tar_name}")
        except Exception as e:
            print(f"ERRORE creazione archivio: {e}")
            success = False

    return success


def main():
    parser = argparse.ArgumentParser(
        prog='lgwin2linux.py',
        description='Converte applicazioni Legopc da Windows a Linux.',
        epilog='''
Regole di conversione:
  libut:    *.for -> *.f, l_moduli.dat -> lista_moduli.dat,
            l_aux.dat -> lista_complementari.dat, CRLF -> LF
  libgraph: copia ricorsiva, conversione line-ending per file testo
  models:   *.tom, f01.dat, f14.dat, *.for -> *.f (escluso proc/)
            NOTA: models/* -> <dest>/* (modelli nella root destinazione)

Output: produce directory + archivio tar.gz

Esempi:
  python lgwin2linux.py                     # -> user_legocad_linux.tar.gz
  python lgwin2linux.py pippo               # -> pippo_legocad_linux.tar.gz
  python lgwin2linux.py pippo pippo_linux   # -> pippo_linux.tar.gz
        ''',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        'sorgente',
        nargs='?',
        default='user',
        help='Directory sorgente Windows (default: user)'
    )
    parser.add_argument(
        'destinazione',
        nargs='?',
        default=None,
        help='Directory destinazione Linux (default: <sorgente>_legocad_linux)'
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
        dst_name = f"{src_name}_legocad_linux"

    # Mostra comando e chiedi conferma
    print(f"\nComando: python lgwin2linux.py {src_name} {dst_name}")
    print(f"  Input:  {src_name}/")
    print(f"  Output: {dst_name}/")

    if not args.yes:
        risposta = input("\nProcedere? [s/N]: ").strip().lower()
        if risposta not in ('s', 'si', 'y', 'yes'):
            print("Operazione annullata.")
            sys.exit(0)

    print()
    success = convert_win2linux(src_name, dst_name)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
