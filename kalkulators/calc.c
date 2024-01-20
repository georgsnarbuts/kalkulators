#include "wait.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdlib.h>
#include "halconf.h"
#include "tm1638.h"
#include "usb_util.h"
#include "print.h"
#include <math.h>
#include <ctype.h>
#include <string.h>

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

void countDigitsAndDecimalPlaces(double input, int *wholeDigits, int *decimalPlaces) {
    // Convert double to string
    char inputString[32];  // Adjust the size based on your needs
    snprintf(inputString, sizeof(inputString), "%f", input);

    // Remove trailing zeros
    char* end = inputString + strlen(inputString) - 1;
    while (*end == '0') {
        *end-- = '\0';
    }

    // Initialize counts
    *wholeDigits = 0;
    *decimalPlaces = 0;
    int decimalFound = 0;

    // Skip the negative sign if present
    int startIdx = (inputString[0] == '-') ? 1 : 0;

    // Iterate through each character in the modified input string
    for (int i = startIdx; i < strlen(inputString); i++) {
        if (isdigit((unsigned char)inputString[i])) {
            if (!decimalFound) {
                (*wholeDigits)++; // Increment count of whole digits
            } else {
                (*decimalPlaces)++; // Increment count of decimal places
            }
        } else if (inputString[i] == '.') {
            decimalFound = 1; // Decimal point is found
        } else {
            // Break the loop if a non-digit or non-decimal point character is encountered
            break;
        }
    }
}

enum USBConnectionNotifState {
    NONE,
    NOTIFY,
    CLEAR
};

bool show_usb_connection = true;
bool show_calc_connection = true;

void housekeeping_task_user(void) {
  if (usb_connected_state() == 1) {

    set_single_persistent_default_layer(0);
    
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
        display7Seg(7, 0b00111111);

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
    bool operator_pressed;
} CALCULATOR_STATE;

typedef struct {
    int operator;
} OPERATOR_TYPE;

CALCULATOR_STATE calc_state = {0, "\0", true, false, false};
OPERATOR_TYPE selected_operator = {0};

void show_result(void){
    
    char output[10];
        int wholeDigits, decimalPlaces;

        countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

        snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
        
        if (strchr(output, '.') == NULL) {
            displayTextRight(output);
        } else {
            displayTextRightFloat(output);
        }
}

void process_input(double input) {

    char input_char[5];
    snprintf(input_char, 5, "%.f", input);
    print(input_char);


    if (strchr(calc_state.current_input, '.') == NULL) {

        if (strlen(input_char) == 1 && strlen(calc_state.current_input) >= 8) {
            return;
        }

    } else {
            if (strlen(input_char) == 1 && strlen(calc_state.current_input) >= 9) {
            return;
        }
    }

    // handle decimal point input
    if (strcmp(input_char, "111") == 0) {
        if (strchr(calc_state.current_input, '.') == NULL) {
            reset();
            char buffer[2] = { "." };
            strcat(calc_state.current_input, buffer);
            displayTextRightFloat(calc_state.current_input);
        }
    }


    // handle clear
    else if (strcmp(input_char, "777") == 0){
        reset();
        display7Seg(7, 0b00111111);
        calc_state.accumulator = 0;
        calc_state.current_input[0] = '\0';
        calc_state.decimal_point_pressed = true;
    }

    // handle sum operation
    else if (strcmp(input_char, "333") == 0) {

        if (calc_state.operator_pressed == true){
        reset();

            if (selected_operator.operator == 1){
                calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 2){
                calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 3){
                calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 4){
                calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            selected_operator.operator = 1;

            char output[10];
            int wholeDigits, decimalPlaces;

            countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

            

            snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
            
            if (strchr(output, '.') == NULL) {
                displayTextRight(output);
            } else {
                displayTextRightFloat(output);
            }
            calc_state.current_input[0] = '\0';
        
        }
        else{
            reset();
            display7Seg(7, 0b00111111);
            selected_operator.operator = 1;

            accum = atof(calc_state.current_input);
            calc_state.accumulator += accum;
            calc_state.current_input[0] = '\0';
            calc_state.decimal_point_pressed = true;
            calc_state.operator_pressed = true;
        }
        
    }

    // handle minus operation
    else if (strcmp(input_char, "444") == 0) {

        if (calc_state.operator_pressed == true){
        reset();

            if (selected_operator.operator == 1){
                calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 2){
                calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 3){
                calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 4){
                calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            selected_operator.operator = 2;


            char output[10];
            int wholeDigits, decimalPlaces;

            countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

            

            snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
            
            if (strchr(output, '.') == NULL) {
                displayTextRight(output);
            } else {
                displayTextRightFloat(output);
            }
            calc_state.current_input[0] = '\0';
        
        }
        else{
            reset();
            display7Seg(7, 0b00111111);
            selected_operator.operator = 2;

            accum = atof(calc_state.current_input);
            calc_state.accumulator += accum;
            calc_state.current_input[0] = '\0';
            calc_state.decimal_point_pressed = true;
            calc_state.operator_pressed = true;
        }

    }

    // handle multiplication operation
    else if (strcmp(input_char, "555") == 0) {
        
        if (calc_state.operator_pressed == true){
            reset();

            if (selected_operator.operator == 1){
                calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 2){
                calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 3){
                calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 4){
                calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            selected_operator.operator = 3;

            char output[10];
            int wholeDigits, decimalPlaces;

            countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

            

            snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
            
            if (strchr(output, '.') == NULL) {
                displayTextRight(output);
            } else {
                displayTextRightFloat(output);
            }
            calc_state.current_input[0] = '\0';
        
        }
        else{
            reset();
            display7Seg(7, 0b00111111);
            selected_operator.operator = 3;

            accum = atof(calc_state.current_input);
            calc_state.accumulator += accum;
            calc_state.current_input[0] = '\0';
            calc_state.decimal_point_pressed = true;
            calc_state.operator_pressed = true;
        }
    }


    // handle division operation
    else if (strcmp(input_char, "666") == 0) {
        
        if (calc_state.operator_pressed == true){
        reset();

            if (selected_operator.operator == 1){
                calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 2){
                calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 3){
                calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            else if (selected_operator.operator == 4){
                calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
                //printf("%f", calc_state.accumulator);

            }

            selected_operator.operator = 4;

            char output[10];
            int wholeDigits, decimalPlaces;

            countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

            

            snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
            
            if (strchr(output, '.') == NULL) {
                displayTextRight(output);
            } else {
                displayTextRightFloat(output);
            }
            calc_state.current_input[0] = '\0';
        
        }

        else{

            reset();
            display7Seg(7, 0b00111111);
            selected_operator.operator = 4;

            accum = atof(calc_state.current_input);
            calc_state.accumulator += accum;
            calc_state.current_input[0] = '\0';
            calc_state.decimal_point_pressed = true;
            calc_state.operator_pressed = true;
            
        }

    }


    
    else if (strcmp(input_char, "999") == 0){
        
        reset();
        calc_state.operator_pressed = false;

        if (selected_operator.operator == 1){
            calc_state.accumulator = calc_state.accumulator + atof(calc_state.current_input);
            //printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 2){
            calc_state.accumulator = calc_state.accumulator - atof(calc_state.current_input);
            //printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 3){
            calc_state.accumulator = calc_state.accumulator * atof(calc_state.current_input);
            //printf("%f", calc_state.accumulator);

        }

        else if (selected_operator.operator == 4){
            calc_state.accumulator = calc_state.accumulator / atof(calc_state.current_input);
            //printf("%f", calc_state.accumulator);

        }


        char output[10];
        int wholeDigits, decimalPlaces;

        countDigitsAndDecimalPlaces(calc_state.accumulator, &wholeDigits, &decimalPlaces);

        

        snprintf(output, 10, "%*.*f", wholeDigits, decimalPlaces, calc_state.accumulator);
        
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
        char buffer[2] = {*input_char};
        strcat(calc_state.current_input, buffer);

        if (strchr(calc_state.current_input, '.') == NULL) {

            //print(calc_state.current_input);
            displayTextRight(calc_state.current_input);

        } else {
            //print(calc_state.current_input);
            displayTextRightFloat(calc_state.current_input);
        }
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t   *record) {

    if (record->event.pressed){

        switch (keycode) {

        case KC_clear:
                process_input(777);
            break;
        case KC_div:
                process_input(666);
            break;
        case KC_times:
                process_input(555);
            break;
        case KC_minus:
                process_input(444);
            break;
        case KC_seven:
                process_input(7);
            break;
        case KC_eight:
                process_input(8);
            break;
        case KC_nine:
                process_input(9);
            break;
        case KC_four:
                process_input(4);
            break;
        case KC_five:
                process_input(5);
            break;
        case KC_six:
                process_input(6);
            break;
        case KC_plus:
                process_input(333);    
            break;
        case KC_one:
                process_input(1);
            break;
        case KC_two:
                process_input(2);
            break;
        case KC_three:
                process_input(3);
            break;
        case KC_zero:
                process_input(0);
            break;
        case KC_dot:
                process_input(111);
            break;
        case KC_equals:
                process_input(999);
            break;
        }
        return true;
    }

    return true;
}