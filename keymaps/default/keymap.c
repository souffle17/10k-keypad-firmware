// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H


void bootmagic_lite(void) {
    matrix_scan();
    wait_ms(DEBOUNCE * 2);
    matrix_scan();
    
    if (matrix_get_row(0) & (1 << 0) 
        && matrix_get_row(0) & (1 << 1) 
        && matrix_get_row(0) & (1 << 2) 
        && matrix_get_row(0) & (1 << 3)
        && matrix_get_row(0) & (1 << 4)
        ) {
        layer_on(0);
    }
    else {
        layer_on(1);
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┐
     * │ 1 │ 2 │ 3 │ 4 │ 5 │
     * ├───┼───┼───┼───┼───┤
     * │ 6 │ 7 │ 8 │ 9 │ 0 │
     * └───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT_ortho_2x6(
        QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,
        QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT
    ),
    [1] = LAYOUT_ortho_2x6(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
    )
};
