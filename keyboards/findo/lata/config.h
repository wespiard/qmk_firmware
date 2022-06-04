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

/* key matrix size */
#define MATRIX_ROWS 10
#define MATRIX_COLS 12

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)

#define UNUSED_MCU 18
#define UNUSED_MCP 5

// wiring
#define MATRIX_ROW_PINS_MCU { A6, A5, A4, A3, A10 }
#define MATRIX_COL_PINS_MCU { B11, B10, B2, B1, B0, A7 }
#define UNUSED_PINS_MCU \
    { A0, A1, A2, A15, B3, B4, B5, B8, B9, B12, B13, B14, B15, C13, C14, C15, F0, F1 }

#define MATRIX_ROW_PINS_MCP { B0, B1, B2, B3, B4 }
#define MATRIX_COL_PINS_MCP { A0, A1, A2, A3, A4, A5 }
#define UNUSED_PINS_MCP     { B5, B6, B7, A6, A7 }

/* COL2ROW, ROW2COL*/
//#define DIODE_DIRECTION COL2ROW

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

//#define MATRIX_ROW_PINS { A6, A5, A4, A3, A10, A0, A0, A0, A0, A0 }
//#define MATRIX_COL_PINS { B11, B10, B2, B1, B0, A1, A1, A1, A1, A1, A1, A1 }

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
//#define DEBOUNCE 5

/* i2c settings */

#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
#define I2C1_TIMINGR_PRESC 2U
#define I2C1_TIMINGR_SCLDEL 1U
#define I2C1_TIMINGR_SDADEL 0U
#define I2C1_TIMINGR_SCLH 9U
#define I2C1_TIMINGR_SCLL 26U
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1

