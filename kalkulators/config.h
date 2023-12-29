// Copyright 2023 georgsnarbuts (@georgsnarbuts)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#define TM1638_CLK_PIN A8
#define TM1638_DIO_PIN A9
#define TM1638_STB_PIN B15
#define VBUS_CHECK_PIN B5


#define SPLIT_USB_DETECT 
#define SPLIT_USB_TIMEOUT 10000
#define SPLIT_USB_TIMEOUT_POLL 10

