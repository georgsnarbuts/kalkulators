// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "halconf.h"
#include "tm1638.h"
#include "wait.h"

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
  setPinOutput(TM1638_CLK_PIN);
  setPinOutput(TM1638_STB_PIN);
  setPinOutput(TM1638_DIO_PIN);

  displayBegin();
  brightness(2);
};
enum custom_keycodes {
  KC_clear,   
  KC_divide, 
  KC_times, 
  KC_minus,
  KC_seven,   
  KC_eight,   
  KC_nine,
  KC_four,   
  KC_five,   
  KC_six,   
  KC_plus,
  KC_one,   
  KC_two,   
  KC_three,
  KC_zero,   
  KC_dot,          
  KC_equals
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │TG1│ / │ * │ - │
     * ├───┼───┼───┼───┤
     * │ 7 │ 8 │ 9 │   │
     * ├───┼───┼───┤ + │
     * │ 4 │ 5 │ 6 │   │
     * ├───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │   │
     * ├───┴───┼───┤Ent│
     * │   0   │ . │   │
     * └───────┴───┴───┘
     */
    [0] = LAYOUT_numpad(
        KC_NUM,   KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,
        KC_P0,   KC_PDOT,          KC_PENT
    ),

    [1] = LAYOUT_calc(
        KC_clear,   KC_divide, KC_times, KC_minus,
        KC_seven,   KC_eight,   KC_nine,
        KC_four,   KC_five,   KC_six,   KC_plus,
        KC_one,   KC_two,   KC_three,
        KC_zero,   KC_dot,          KC_equals
    )
};













