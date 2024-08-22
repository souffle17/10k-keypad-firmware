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

static uint16_t previous_voltage_1 = 0;
static uint16_t previous_voltage_2 = 0;
static bool key_pressed_1 = false; 
static bool key_pressed_2 = false; 
static uint16_t buffer = 70;

void keyboard_post_init_user(void) {
    analogReference(ADC_REF_POWER);
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

    if (user_config.key_layer != layer) {
        user_config.key_layer = layer;
        eeconfig_update_user(user_config.raw); 
    }
    
}

// Scary!
void matrix_scan_user(void) {
    if (user_config.key_layer == 4) {
        uint16_t current_voltage_1 = adc_read(pinToMux(F6));
        if (current_voltage_1 > previous_voltage_1) {
            if(!key_pressed_1 && current_voltage_1 > previous_voltage_1 + buffer) {
                register_code(KC_Z);
                // SEND_STRING("\n");
                // send_word(current_voltage_1);
                // SEND_STRING("\n");
                key_pressed_1 = true;
            }
            else if(key_pressed_1) {
                previous_voltage_1 = current_voltage_1;
            }
        }
        else if(current_voltage_1 < previous_voltage_1) {
            if(key_pressed_1 && current_voltage_1 < previous_voltage_1 - buffer) {
                unregister_code(KC_Z);
                key_pressed_1 = false;
            }
            else if(!key_pressed_1) {
                previous_voltage_1 = current_voltage_1;
            }
        }

        uint16_t current_voltage_2 = adc_read(pinToMux(F7));
        if (current_voltage_2 > previous_voltage_2) {
            if(!key_pressed_2 && current_voltage_2 > previous_voltage_2 + buffer) {
                register_code(KC_X);
                key_pressed_2 = true;
            }
            else if(key_pressed_2) {
                previous_voltage_2 = current_voltage_2;
            }
        }
        else if(current_voltage_2 < previous_voltage_2) {
            if(key_pressed_2 && current_voltage_2 < previous_voltage_2 - buffer) {
                unregister_code(KC_X);
                key_pressed_2 = false;
            }
            else if(!key_pressed_2) {
                previous_voltage_2 = current_voltage_2;
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
    [1] = LAYOUT_ortho_1x5( // this should be disabled when active
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
    )
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
