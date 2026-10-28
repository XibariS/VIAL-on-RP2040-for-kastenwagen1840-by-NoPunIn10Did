# Kastenwagen 1840 / Frood Rev6 – Vial-Ampel (Active High)

Dies ist ein Buildpaket auf Basis der **bereits erfolgreich geflashten Vial-Version**.
Keine fertige UF2: Die erprobte Windows-QMK-MSYS-Umgebung kompiliert das Paket.

## Gewuenschte Layer-Ampel

| Hoechster aktiver Layer | Gruen D3 | Gelb D2 | Rot D1 |
| --- | --- | --- | --- |
| 0 | AN | aus | aus |
| 1 | aus | AN | aus |
| 2 | aus | aus | AN* |
| 3 oder hoeher | aus | aus | aus |

*Rot ist derzeit nur als **Firmware-Ausgang D0/GP0** eingerichtet. Laut Durchgangsmessung fuehrt das linke Pad der roten LED D1 aber zu **B0**; es besteht **keine bestaetigte physische Verbindung zu D0**. Ohne geeignete Verbindung bleibt Rot sehr wahrscheinlich dunkel.

### Technische Aenderungen

- **Active High:** HIGH schaltet die jeweiligen LEDs ein, LOW aus.
- Gruen: LED_INDICATOR_BOT / C6 / GP5.
- Gelb: LED_INDICATOR_MID / D4 / GP4.
- Rot: LED_RED_PIN / D0 / GP0 (zuvor Encoder 1, Anschluss frei).
- Beide Encoder deaktiviert, inklusive Vial-Encoder-Darstellung. Die **MX-Switches an diesen Positionen bleiben normale Tasten in der Matrix**.
- Originale Layer-LED-Routine mit B0 bleibt deaktiviert (`LAYER_LED_DISABLE`).
- Frood Rev6-Flash-Definition (`RP2040_FLASH_GD25Q64CS`) und Vial-UID bleiben unveraendert.
- Die 4 Vial-Layer bleiben bearbeitbar, Belegung aus dem vorherigen Build bleibt im Quelltext erhalten; bereits in Vial gespeicherte EEPROM-Belegungen koennen nach dem Flashen weiterbestehen. Vorher exportieren!
- Build-Skript bearbeitet `keyboard.json` und `kastenwagen1840.c` im **separaten vial-qmk Repository**, jeweils mit `.bak_vor_ampel`-Backup. Alle Keymap-Dateien werden dort ebenfalls vor Ueberschreiben gesichert.

### Hardware, vor dem Flashen

1. USB abziehen, D1 auf eigene Verantwortung wie D2/D3 um 180 Grad drehen, Loetstellen kontrollieren.
2. D1 **nicht** mit einem unbestaetigten Anschluss verbinden; insbesondere B0/RAW nicht einfach an D0 kurzschliessen. Null Volt am B0-Pad beweist nicht, dass es sicher unverbunden ist.
3. Die 470-Ohm-Vorwiderstaende bleiben bestueckt.
4. Die bisher funktionierende Vial-UF2 **unbedingt aufheben** und in Vial die Tastenzuordnung sichern.

### Windows / QMK MSYS

Entpacke ZIP so nach `%USERPROFILE%\frood_vial_ampel`, dass `build_vial.py` direkt in diesem Ordner liegt.
In QMK MSYS:

```bash
cd ~/frood_vial_ampel
python3 build_vial.py ~/vial-qmk
```

Nach erfolgreichem Build liegt die UF2 im Ordner `%USERPROFILE%\vial-qmk` (Name enthaelt `kastenwagen1840_vial_rp2040_ce.uf2`).

Die Toolchain war in dieser Ausfuehrungsumgebung nicht installiert, deshalb wurde der **echte ARM-Build nicht ausgefuehrt**. Der Build kann daher noch Fehler melden; bitte die erste Fehlermeldung weitergeben.

### Test nach erfolgreichem Flashen

- In Vial auf Layer 0: **Gruen leuchtet**.
- Auf Layer 1: **Gelb leuchtet**.
- Auf Layer 2: Gruen und Gelb **aus**, D0 wird auf HIGH geschaltet. Rot wird ohne Draht **wahrscheinlich nicht leuchten**.
- Auf Layer 3: alle aus.

Die bisherigen GPIO-Spannungsmessungen waren am Controller: D4=GP4 und C6=GP5 wie erwartet. Fuer Rot kann spaeter sicherheitshalber D0-GND auf Layer 2 gemessen werden (ca. 3,3 V wenn aktiv), ohne irgendeinen Bodge-Draht zu setzen. Beim Messen keine Pins kurzschliessen.

### Wichtige Anmerkung

Die roten D1-Pads sind nach Messung links mit B0 verbunden, rechts ueber ca. 466 Ohm mit GND. Der Frood hat an B0 keine normale GPIO-Steuerfunktion. Das bedeutet, dass **Firmware allein keine elektrische Verbindung zu D0 herstellen kann.**

Falls der Build fehlschlaegt oder Vial die Tastatur nicht automatisch erkennt, bitte das vollstaendige Fehlerprotokoll bzw. die Beobachtung schicken. Die bisherige funktionierende UF2 bleibt als Rueckfall erhalten.
