// Copyright 2022 Wes Piard (@wespiard)
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

/* USB Device descriptor parameter */
// #define VENDOR_ID    0x4653
// #define PRODUCT_ID   0x0001
// #define DEVICE_VER   0x0001

#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// default but used in macros
#undef TAPPING_TERM
#define TAPPING_TERM 200

// Prevent normal rollover on alphas from accidentally triggering mods.
// #define IGNORE_MOD_TAP_INTERRUPT

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
// #define TAPPING_FORCE_HOLD

// Auto Shift
// #define NO_AUTO_SHIFT_ALPHA
// #define AUTO_SHIFT_TIMEOUT TAPPING_TERM
// #define AUTO_SHIFT_NO_SETUP

/* key matrix size */
#define MATRIX_ROWS 10
#define MATRIX_COLS 12

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)

#define UNUSED_MCU 22
#define UNUSED_EXPANDER 5

// wiring
#define MATRIX_ROW_PINS_MCU { B1, A7, A5, A3, A1 }
#define MATRIX_COL_PINS_MCU { A0, A2, A4, A6, B0, B2 }
#define UNUSED_PINS_MCU \
    { A8, A9, A10, A13, A14, A15, B3, B4, B5, B6, B7, B8, B9, B12, B13, B14, B15, C13, C14, C15, F0, F1 }

#define MATRIX_ROW_PINS_EXPANDER { P1_0, P1_1, P1_2, P1_3, P1_4 }
#define MATRIX_COL_PINS_EXPANDER { P0_0, P0_1, P0_2, P0_3, P0_4, P0_5}
#define UNUSED_PINS_EXPANDER     { P1_5, P1_6, P1_7, P0_6, P0_7 }

/* i2c settings */

#define I2C_DRIVER I2CD2
#define I2C1_SCL_PIN B10
#define I2C1_SDA_PIN B11
#define I2C1_TIMINGR_PRESC 2U
#define I2C1_TIMINGR_SCLDEL 1U
#define I2C1_TIMINGR_SDADEL 0U
#define I2C1_TIMINGR_SCLH 9U
#define I2C1_TIMINGR_SCLL 26U
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1