// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

typedef union {
  uint32_t raw;
  struct {
    uint16_t     key_layer :16;
  };
} user_config_t;

user_config_t user_config;

void keyboard_post_init_user(void) {
    layer_on(user_config.key_layer);
}

void bootmagic_lite(void) {
    matrix_scan();
    wait_ms(DEBOUNCE * 2);
    matrix_scan();

    user_config.raw = eeconfig_read_user();

    if (user_config.key_layer == 0) {
        user_config.key_layer = 1;
    }

    //type safety
    uint16_t layer = user_config.key_layer;

    // hold down top row
    if (matrix_get_row(0) & (1 << 0) 
        && matrix_get_row(0) & (1 << 1) 
        && matrix_get_row(0) & (1 << 2) 
        && matrix_get_row(0) & (1 << 3)
        && matrix_get_row(0) & (1 << 4)
        ) {
        layer = 0;
    }
    else if (matrix_get_row(0) & (1 << 0) ) {
        layer = 1;
    }
    else if (matrix_get_row(0) & (1 << 1) ) {
        layer = 2;
    }
    else {
        // shouldn't happen
        layer = 0;
    }

    if (user_config.key_layer != layer) {
        user_config.key_layer = layer;
        eeconfig_update_user(user_config.raw); 
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
    [0] = LAYOUT_ortho_2x5(
        QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,
        QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT,    QK_BOOT
    ),
    [1] = LAYOUT_ortho_2x5(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
    ),
    [2] = LAYOUT_ortho_2x5(
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B
    )
};
