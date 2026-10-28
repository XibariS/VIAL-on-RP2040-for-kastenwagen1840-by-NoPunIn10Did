// Kastenwagen 1840 / Frood Revision 6 (RP2040)
#pragma once

// Frood Rev6: externer Flash-Speicher (boot stage 2)
#define RP2040_FLASH_GD25Q64CS

// Die originale LED-Routine greift auf B0 zu (kein normaler GPIO am Frood).
// Stattdessen steuert unsere Vial-Keymap D4/C6 und D0 selbst.
#define LAYER_LED_DISABLE

// Rot: der fruehere Encoder-Pin D0 / GPIO0.
// Ohne zusaetzliche Leiterbahn/Bodge Wire ist D1 physisch NICHT angeschlossen!
#define LED_RED_PIN D0

// Vial erkennt das Keyboard selbst, da vial.json eingebettet wird.
#define VIAL_KEYBOARD_UID {0x04, 0x9F, 0x9C, 0x50, 0x25, 0x8A, 0x2D, 0xED}

// Vial-Sicherheitsfreigabe: ESC (0,0) und K (2,4) gedrueckt halten.
#define VIAL_UNLOCK_COMBO_ROWS { 0, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 4 }

// Vier Vial-editierbare Layer, initial nur Layer 0 belegt.
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
