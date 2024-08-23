// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "analog.h"

typedef union {
  uint32_t raw;
  struct {
    uint16_t     key_layer :16;
  };
} user_config_t;

user_config_t user_config;

bool disable_scan = false;

static uint16_t buffer = 10;

static uint8_t hall_pins[2] = {F6, F7};
static uint16_t previous_voltage[2] = {0, 0};
static bool key_pressed[2] = {false, false};
static uint16_t hall_keys[2] = {KC_Z, KC_X};

void keyboard_post_init_user(void) {
    layer_on(user_config.key_layer);
}

void bootmagic_scan(void) {
    matrix_scan();
    wait_ms(DEBOUNCE * 2);
    matrix_scan();

    user_config.raw = eeconfig_read_user();

    if (user_config.key_layer == 0 || user_config.key_layer == 1) {
        user_config.key_layer = 2;
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
    else if (matrix_get_row(0) & (1 << 0)) {
        layer = 1;
        disable_scan = true;
    }
    else if (matrix_get_row(0) & (1 << 1)) {
        layer = 2;
    }
    else if (matrix_get_row(0) & (1 << 2)) {
        layer = 3;
    }
    else if (matrix_get_row(0) & (1 << 3)) {
        layer = 4;
    }
    else if (matrix_get_row(0) & (1 << 4)) {
        layer = 5;
    }

    if (user_config.key_layer != layer) {
        user_config.key_layer = layer;
        eeconfig_update_user(user_config.raw); 
    }

    for (int sensor = 0; sensor < sizeof(hall_pins); sensor++) {
        previous_voltage[sensor] = analogReadPin(hall_pins[sensor]);
    }
    
}

// Scary!
void matrix_scan_user(void) {

    //debug
    // if (user_config.key_layer == 2) {
    //     if(matrix_get_row(0) & (1 << 1)) {
    //         SEND_STRING("\n");
    //         send_word(analogReadPin(hall_pins[0]));
    //         SEND_STRING("\n");

    //     }
    // }

    // only active on layer 1
    if (user_config.key_layer == 1) {

        for(int sensor = 0; sensor < sizeof(hall_pins); sensor++) {
            uint16_t current_voltage = analogReadPin(hall_pins[sensor]);
            // press the key

            if (!key_pressed[sensor]) {
                if (current_voltage > previous_voltage[sensor] + buffer) {
                    register_code(hall_keys[sensor]);
                    key_pressed[sensor] = true;
                    previous_voltage[sensor] = current_voltage;
                }
            }

            //unpress the key
            else {
                if (current_voltage < previous_voltage[sensor] - buffer) {
                    unregister_code(hall_keys[sensor]);
                    key_pressed[sensor] = false;
                    previous_voltage[sensor] = current_voltage;
                }
            }
        }

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
    [1] = LAYOUT_ortho_2x5( // this should be disabled when active
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
    ),
    [2] = LAYOUT_ortho_2x5(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
    ),
    [3] = LAYOUT_ortho_2x5(
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B
    ),
    [4] = LAYOUT_ortho_2x5(
        KC_H,    KC_J,    KC_K,    KC_L,    KC_SEMICOLON,
        KC_N,    KC_M,    KC_COMMA,    KC_DOT,    KC_SLASH
    )
};
