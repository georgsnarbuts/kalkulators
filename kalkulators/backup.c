#include "tm1638.h"
#include "gpio.h"
#include "progmem.h"
#include "wait.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "halconf.h"
#include "print.h"



#ifndef TM1638_CLK_PIN
#error tm1638: no CLK pin defined!
#endif

#ifndef TM1638_STB_PIN
#error tm1638: no STB pin defined!
#endif

#ifndef TM1638_DIO_PIN
#error tm1638: no DIO pin defined!
#endif

uint8_t first = 0x44;
uint8_t second = 0xC0;
uint8_t third = 0x06;
uint8_t fourth = 0xCE;
uint8_t fifth = 0x7F;

//writePinHigh(TM1638_STB_PIN);

enum custom_keycodes {
    QMKBEST = KC_P0,
};



__attribute__((__unused__)) static void stb_start_comm(void){
    writePinLow(TM1638_STB_PIN);
};

__attribute__((__unused__)) static void stb_end_comm(void){
    writePinHigh(TM1638_STB_PIN);
};

__attribute__((__unused__)) static void stb_gate_pulse(void){
    writePinHigh(TM1638_STB_PIN);
    wait_us(1);
    writePinLow(TM1638_STB_PIN);
};




void tm1368_write_bytes(const uint8_t *Data, uint8_t num_of_bytes){

    uint8_t i, j, value;

    for (j = 0; j < num_of_bytes; j++){
        for (i=0, value=Data[j]; i < 8; value >>= 1){
            //uprintf("bytes: %u", value);
            writePinLow(TM1638_CLK_PIN);
            wait_us(1);
            writePin(TM1638_DIO_PIN, value & 0x01);
            writePinHigh(TM1638_CLK_PIN);
            wait_us(1);
        }
    }

};

__attribute__((__unused__)) static void testing(void){
    stb_start_comm();
    tm1368_write_bytes(&first, 1);
    stb_end_comm();
    stb_start_comm();
    tm1368_write_bytes(&second, 1);
    tm1368_write_bytes(&third, 1);
    stb_end_comm();
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case QMKBEST:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            //uprintf("lk pin: %lu, stb pin: %lu, dio pin: %lu", TM1638_CLK_PIN, TM1638_STB_PIN, TM1638_DIO_PIN);
            //uprintf("bytes: %u", first);
            testing();
        } else {
            // when keycode QMKBEST is released
        }
        break;
    }
    return true;
};