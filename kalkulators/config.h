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

#undef  PRINTF_SUPPORT_DECIMAL_SPECIFIERS
#define PRINTF_SUPPORT_DECIMAL_SPECIFIERS 1

#undef  DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
#define DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS 1

#undef  DPRINTF_SUPPORT_LONG_LONG
#define DPRINTF_SUPPORT_LONG_LONG 1

#undef  DPRINTF_SUPPORT_WRITEBACK_SPECIFIER
#define DPRINTF_SUPPORT_WRITEBACK_SPECIFIER 1

#undef  DSUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS
#define DSUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS 1
