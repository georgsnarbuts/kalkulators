#include "wait.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "halconf.h"
#include "tm1638.h"


void matrix_scan_user(void) {
  if (is_keyboard_master()) {
    set_single_persistent_default_layer(0);
    displayIntNum(30, 0);

  } else {
    displayIntNum(100, 0);
    set_single_persistent_default_layer(1);
  }
}
