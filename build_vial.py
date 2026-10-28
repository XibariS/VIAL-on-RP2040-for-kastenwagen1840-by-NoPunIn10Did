#!/usr/bin/env python3
"""Installiere Vial-Keymap im getrennten vial-qmk-Repo und starte RP2040-Build."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time

KEYBOARD = 'nopunin10did/kastenwagen1840'
NAME = 'vial'


def main():
    package_dir = Path(__file__).resolve().parent
    vial_root = Path(sys.argv[1]).expanduser().resolve() if len(sys.argv) > 1 else Path.home() / 'vial-qmk'
    source = package_dir / 'keyboards' / 'nopunin10did' / 'kastenwagen1840' / 'keymaps' / NAME
    keyboard_dir = vial_root / 'keyboards' / 'nopunin10did' / 'kastenwagen1840'
    if not (source/'keymap.c').is_file():
        sys.exit('FEHLER: Dateien nicht gefunden. ZIP bitte direkt entpacken, sodass build_vial.py neben keyboards/ liegt.')
    metadata = keyboard_dir / 'keyboard.json'
    if not metadata.exists():
        sys.exit('FEHLER: keyboard.json im vial-qmk Ordner nicht gefunden: ' + str(metadata) + '\nBitte vial-qmk zuerst per Git klonen (siehe README_DE.md).')
    if not (vial_root / '.git').exists():
        sys.exit('FEHLER: Das ist kein Git-Checkout von vial-qmk: ' + str(vial_root))
    for check in ('lib/chibios/os', 'lib/pico-sdk/src'):
        if not (vial_root / check).exists():
            sys.exit('FEHLER: Vial-Submodule fehlen: '+check+'\nIm vial-qmk-Verzeichnis `make git-submodule` ausfuehren.')
    info = json.loads(metadata.read_text(encoding='utf8'))
    if info.get('processor') != 'atmega32u4':
        sys.exit('FEHLER: Unerwartete MCU im Vial-Keyboard-Metadatensatz.')
    positions = [tuple(k['matrix']) for k in info['layouts']['LAYOUT_all']['layout']]
    defined = json.loads((source/'vial.json').read_text(encoding='utf8'))['layouts']['keymap']
    expected = []
    for row in defined:
        for element in row:
            if isinstance(element, str) and '\n' not in element:
                expected.append(tuple(map(int, element.split(','))))
    if positions != expected or len(positions)!=65:
        sys.exit('FEHLER: Das Layout im aktuellen Vial-QMK stimmt nicht mit unserer 65-Tasten-Vial-Definition ueberein.\nNichts wurde geaendert. Bitte Fehler hier melden.')
    # 2025+ QMK verlangt explizite Elite-C-Kompatibilitaet.
    pin_compat = info.get('pin_compatible')
    if pin_compat not in (None,'elite_c'):
        sys.exit('FEHLER: Unerwartete pin_compatible Angabe: ' + str(pin_compat))
    if pin_compat is None:
        back = metadata.with_name('keyboard.json.bak_vor_frood_vial')
        if not back.exists():
            shutil.copy2(metadata,back)
        info['pin_compatible']='elite_c'
        metadata.write_text(json.dumps(info,indent=4,ensure_ascii=False)+'\n',encoding='utf8')
        print('Elite-C-Pinkompatibilitaet hinzugefuegt. Sicherheitskopie:',back, flush=True)
    # Die Encoder-Pins werden fuer D0 (rote LED) frei gemacht.
    # In der Keymap ist ENCODER_ENABLE=no; zusaetzlich die JSON-Metadaten
    # aktualisieren, damit QMK keinen Encoder-Treiber aktiviert.
    changed = False
    if info.get('features', {}).get('encoder') is not False:
        info.setdefault('features', {})['encoder'] = False
        changed = True
    if 'encoder' in info:
        del info['encoder']
        changed = True
    if changed:
        backup = metadata.with_name('keyboard.json.bak_vor_ampel')
        if not backup.exists(): shutil.copy2(metadata, backup)
        metadata.write_text(json.dumps(info, indent=4, ensure_ascii=False)+'\n', encoding='utf8')
        print('Encoder in keyboard.json deaktiviert (Backup:', backup, ')', flush=True)

    # Tastatur-Quellcode enthaelt einen unbedingten Encoder-Callback.
    # Damit die deaktivierte Encoder-Bibliothek nicht verlinkt werden muss,
    # schliessen wir genau diesen Callback in #ifdef ENCODER_ENABLE ein.
    kb_c = keyboard_dir / 'kastenwagen1840.c'
    if not kb_c.is_file():
        sys.exit('FEHLER: kastenwagen1840.c nicht gefunden: ' + str(kb_c))
    c = kb_c.read_text(encoding='utf8')
    callback = 'bool encoder_update_kb(uint8_t index, bool clockwise) {'
    guard = '#ifdef ENCODER_ENABLE\n' + callback
    if guard not in c:
        if c.count(callback) != 1:
            sys.exit('FEHLER: Encoder-Callback im Originalcode nicht eindeutig gefunden. Bitte melden, keine Aenderung vorgenommen.')
        backup = kb_c.with_name('kastenwagen1840.c.bak_vor_ampel')
        if not backup.exists(): shutil.copy2(kb_c, backup)
        c = c.replace(callback, guard, 1).rstrip() + '\n#endif // ENCODER_ENABLE\n'
        kb_c.write_text(c, encoding='utf8')
        print('Original-Encoder-Callback deaktiviert (Backup:', backup, ')', flush=True)

    target=keyboard_dir/'keymaps'/NAME
    target.mkdir(parents=True,exist_ok=True)
    for filename in ('config.h','keymap.c','rules.mk','vial.json'):
        current = target/filename
        backup = target/(filename + '.bak_vor_ampel')
        if current.exists() and not backup.exists():
            shutil.copy2(current, backup)
        shutil.copy2(source/filename,current)
    print('Vial-Dateien installiert:',target,flush=True)
    # Explicitly use the separate repository and do not rely on qmk config's default.
    env=os.environ.copy()
    env['QMK_HOME']=str(vial_root)
    env['QMK_FIRMWARE']=str(vial_root)
    env['SKIP_GIT']='yes'
    cmd=['make',f'{KEYBOARD}:{NAME}','CONVERT_TO=rp2040_ce']
    print('Starte Vial-Compile:', ' '.join(cmd), '\nArbeitsordner:',vial_root, flush=True)
    started=time.time()
    try:
        exit_code=subprocess.call(cmd,cwd=vial_root,env=env)
    except FileNotFoundError:
        sys.exit('FEHLER: make nicht gefunden. Bitte QMK MSYS nutzen.')
    if exit_code:
        sys.exit('Vial-Compile fehlgeschlagen (Exit-Code %s). Bitte Fehlermeldung kopieren.'%exit_code)
    files=sorted(vial_root.glob('*kastenwagen1840*vial*rp2040_ce*.uf2'))
    if not files:
        files=sorted(vial_root.glob('.build/*kastenwagen1840*vial*rp2040_ce*.uf2'))
    fresh=[f for f in files if f.stat().st_mtime >= started-5]
    if not fresh:
        sys.exit('Kompilierung erfolgreich, aber keine aktuelle UF2 gefunden. Bitte Build-Ausgabe pruefen.')
    print('\nBuild erfolgreich! Neue Vial UF2:',flush=True)
    for f in fresh: print(' ',f,flush=True)
    print('WICHTIG: Alte funktionierende QMK-UF2 als Rueckfall sichern; Vial-Erkennung erst nach dem Flashen pruefen.',flush=True)


if __name__=='__main__':
    main()
