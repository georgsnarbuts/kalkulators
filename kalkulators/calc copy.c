#include "wait.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdlib.h>
#include "halconf.h"
#include "tm1638.h"
#include "usb_util.h"
#include "print.h"
#include <math.h>

enum custom_keycodes {
  KC_clear = QK_USER,   
  KC_div, 
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

enum USBConnectionNotifState {
    NONE,
    NOTIFY,
    CLEAR
};

bool show_usb_connection = true;
bool show_calc_connection = true;

void matrix_scan_user(void) {
  if (usb_connected_state() == 1) {

    set_single_persistent_default_layer(1);
    
    if (show_usb_connection == true){
        show_usb_connection = false;
        reset();
        display7Seg(3, 0b01110011);
        display7Seg(4, 0b00111001);
        wait_ms(500);
        reset();
    }
    show_calc_connection = true;


  } else if (usb_connected_state() == 0){
    set_single_persistent_default_layer(1);


    if (show_calc_connection == true){
        show_calc_connection = false;
        reset();
        display7Seg(2, 0b00111001);
        display7Seg(3, 0b01110111);
        display7Seg(4, 0b00111000);
        display7Seg(5, 0b00111001);
        wait_ms(500);
        reset();
    }
    show_usb_connection = true;
  }
}
double accum;
double input;

typedef struct {
    double accumulator; 
    char current_input[8];
    bool expect_operator;
    bool decimal_point_pressed;
} CALCULATOR_STATE;

typedef struct {
    int operator;
} OPERATOR_TYPE;

CALCULATOR_STATE calc_state = {0, "", true};
OPERATOR_TYPE selected_operator = {0};

void process_input(const char* input) {
    if (strcmp(input, "dot") == 0) {
        if (calc_state.decimal_point_pressed) {
            reset();
            char buffer[2] = {"."};
            strcat(calc_state.current_input, buffer);
            displayTextRightFloat(calc_state.current_input);
            calc_state.decimal_point_pressed = !calc_state.decimal_point_pressed;
        }
    }

    else if (strcmp(input, "clear") == 0){
        reset();
        calc_state.accumulator = 0;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;
    }

    else if (strcmp(input, "plus") == 0) {
        // Handle the "plus" key separately
        reset();
        
        selected_operator.operator = 1;

        accum = atof(calc_state.current_input);
        calc_state.accumulator += accum;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;

    }

    else if (strcmp(input, "minus") == 0) {
        // Handle the "plus" key separately
        reset();
        
        selected_operator.operator = 2;

        accum = atof(calc_state.current_input);
        calc_state.accumulator += accum;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;

    }

    else if (strcmp(input, "times") == 0) {
        // Handle the "plus" key separately
        reset();
        
        selected_operator.operator = 3;

        accum = atof(calc_state.current_input);
        calc_state.accumulator += accum;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;

    }

    else if (strcmp(input, "div") == 0) {
        // Handle the "plus" key separately
        print("pressed");
        reset();
        
        selected_operator.operator = 4;

        accum = atof(calc_state.current_input);
        calc_state.accumulator += accum;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;

    }

    
    else if (strcmp(input, "equals") == 0){
        
        reset();

        if (selected_operator.operator == 1){
            calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
            printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 2){
            calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
            printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 3){
            calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
            printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 4){
            calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
            printf("%f", calc_state.accumulator);

        }

        char output[8];
        snprintf(output, 8, "%.2f", calc_state.accumulator);
        
        if (strchr(output, '.') == NULL) {
            displayTextRight(output);
        } else {
            displayTextRightFloat(output);
        }

        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;



    }

    else {
        reset();
        char buffer[2] = {*input};
        strcat(calc_state.current_input, buffer);

        if (strchr(calc_state.current_input, '.') == NULL) {

            print(calc_state.current_input);
            displayTextRight(calc_state.current_input);

        } else {
            print(calc_state.current_input);
            displayTextRightFloat(calc_state.current_input);
        }
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t   *record) {
    switch (keycode) {
    case KC_clear:
        if (record->event.pressed) {
            process_input("clear");
        }
        break;
    case KC_div:
        if (record->event.pressed) {
            print("enters the case.");
            print("\n");
            process_input("div");
        }
        break;
    case KC_times:
        if (record->event.pressed) {
            process_input("times");
        }
        break;
    case KC_minus:
        if (record->event.pressed) {
            process_input("minus");
        }
        break;
    case KC_seven:
        if (record->event.pressed) {
            process_input("7");
        }
        break;
    case KC_eight:
        if (record->event.pressed) {
            process_input("8");
        }
        break;
    case KC_nine:
        if (record->event.pressed) {
            process_input("9");
        }
        break;
    case KC_four:
        if (record->event.pressed) {
            process_input("4");
        }
        break;
    case KC_five:
        if (record->event.pressed) {
            process_input("5");
        }
        break;
    case KC_six:
        if (record->event.pressed) {
            process_input("6");
        }
        break;
    case KC_plus:
        if (record->event.pressed) {
            process_input("plus");    
        }
        break;
    case KC_one:
        if (record->event.pressed) {
            process_input("1");
        }
        break;
    case KC_two:
        if (record->event.pressed) {
            process_input("2");
        }
        break;
    case KC_three:
        if (record->event.pressed) {
            process_input("3");
        }
        break;
    case KC_zero:
        if (record->event.pressed) {
            process_input("0");
        }
        break;
    case KC_dot:
        if (record->event.pressed) {
            process_input("dot");
        }
        break;
    case KC_equals:
        if (record->event.pressed) {
            process_input("equals");
        }
        break;
    
    }
    return true;
};


