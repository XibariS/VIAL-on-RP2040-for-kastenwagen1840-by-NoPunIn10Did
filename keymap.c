// Kastenwagen 1840, Frood RP2040 Rev6, Vial-Version
// Originaler QMK-Layer 0 als editierbare Startbelegung.
// Die weiteren drei Vial-Layer sind anfangs transparent.
#include QMK_KEYBOARD_H

// Aktive HIGH-Ampel fuer den Kastenwagen-Prototypen:
//   D3 / gruen (C6 -> GP5) : Layer 0
//   D2 / gelb  (D4 -> GP4) : Layer 1
//   D1 / rot   (D0 -> GP0) : Layer 2 (derzeit nur logischer Testausgang)
//
// WICHTIG: D1 auf dem PCB ist derzeit an B0 angeschlossen, NICHT an D0.
// Solange kein sicher gepruefter Signalweg zwischen D0 und D1 existiert,
// kann Rot durch keine Firmware-Aenderung leuchten.
// Die urspruengliche 3-LED-Routine ist per LAYER_LED_DISABLE ausgeschaltet.
static void update_traffic_light(layer_state_t active_layers, layer_state_t base_layer) {
    const uint8_t top_layer = get_highest_layer(active_layers | base_layer);
    // An = HIGH, Aus = LOW. Es leuchtet immer maximal eine Farbe.
    gpio_write_pin(LED_INDICATOR_BOT, top_layer == 0); // Gruen
    gpio_write_pin(LED_INDICATOR_MID, top_layer == 1); // Gelb
    gpio_write_pin(LED_RED_PIN,       top_layer == 2); // Rot / D0
}

void keyboard_post_init_user(void) {
    // Erst Ausgangszustand LOW, dann Pins zu Ausgaengen machen.
    gpio_write_pin_low(LED_INDICATOR_BOT);
    gpio_write_pin_low(LED_INDICATOR_MID);
    gpio_write_pin_low(LED_RED_PIN);
    gpio_set_pin_output(LED_INDICATOR_BOT);
    gpio_set_pin_output(LED_INDICATOR_MID);
    gpio_set_pin_output(LED_RED_PIN);
    update_traffic_light(layer_state, default_layer_state);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    update_traffic_light(state, default_layer_state);
    return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    update_traffic_light(layer_state, state);
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_BSPC, KC_P7, KC_P8, KC_P9, KC_PMNS,
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_P4, KC_P5, KC_P6, KC_PPLS,
        SC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, SC_RSPC, KC_UP, KC_P1, KC_P2, KC_P3, KC_PAST,
        KC_LCTL, KC_LALT, MO(2), LT(2,KC_SPC), LT(3,KC_SPC), MO(3), RALT_T(KC_RGUI), MO(1), KC_LEFT, KC_DOWN, KC_RGHT, KC_P0, KC_PDOT, LT(3,KC_PENT)
    ),
    [1] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [2] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [3] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
