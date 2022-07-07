/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>
          2020 Pierre Chevalier <pierrechevalier83@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * This code was heavily inspired by the ergodox_ez keymap, and modernized
 * to take advantage of the quantum.h microcontroller agnostics gpio control
 * abstractions and use the macros defined in config.h for the wiring as opposed
 * to repeating that information all over the place.
 */

#include QMK_KEYBOARD_H
#include "i2c_master.h"
#include <print.h>

extern i2c_status_t pcal9539a_status;
#define PCAL9539A_I2C_TIMEOUT 1000
#define I2C_WRITE 0x00
#define I2C_READ 0x01

// I2C address:
// | 1  | 1  | 1  | 0  | 1  | A1 | A0 |
// | 1  | 1  | 1  | 0  | 1  | 0  | 0  |
#define I2C_ADDR 0b1110100
#define I2C_ADDR_WRITE ((I2C_ADDR << 1) | I2C_WRITE)
#define I2C_ADDR_READ ((I2C_ADDR << 1) | I2C_READ)

// Register addresses
#define IN_0        0x00  // input
#define IN_1        0x01
#define OUT_0       0x02  // output
#define OUT_1       0x03
#define POLARITY_0  0x04  // polarity inversion
#define POLARITY_1  0x05
#define CONFIG_0    0x06  // i/o direction registers, 1 = input (default), 0 = output
#define CONFIG_1    0x07
#define PULL_ENABLE_0    0x46  // i/o direction registers, 1 = input (default), 0 = output
#define PULL_ENABLE_1    0x47
#define PULL_SELECT_0    0x48  // i/o direction registers, 1 = input (default), 0 = output
#define PULL_SELECT_1    0x49


bool         i2c_initialized = 0;
i2c_status_t pcal9539a_status = I2C_ADDR;

uint8_t init_pcal9539a(void) {
    print("init pcal9539a\n");
    pcal9539a_status = I2C_ADDR;

    // I2C subsystem
    if (i2c_initialized == 0) {
        i2c_init();
        i2c_initialized = true;
        wait_ms(PCAL9539A_I2C_TIMEOUT);
    }

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    // This means: we will read all the bits on Port 0 (specifically care about pins 0-4)
    // This means: we will write to the pins 0-5 on Port 1 (in select_rows)
    uint8_t buf[] = {CONFIG_0, 0b11111111, 0b11000000};
    print("before transmit\n");
    pcal9539a_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), PCAL9539A_I2C_TIMEOUT);
    uprintf("after transmit %i\n", pcal9539a_status);
    if (!pcal9539a_status) {
        // enable pull-up resistors
        // note that we don't need to configure the PULL_SELECT registers because pull-ups are defaul
        uint8_t pullup_buf[] = {PULL_ENABLE_0, 0b11111111, 0b11000000};
        pcal9539a_status      = i2c_transmit(I2C_ADDR_WRITE, pullup_buf, sizeof(pullup_buf), PCAL9539A_I2C_TIMEOUT);
        uprintf("after transmit2 %i\n", pcal9539a_status);
    }
    return pcal9539a_status;
}

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];  // debounced values

static matrix_row_t read_cols(uint8_t row);
static void         unselect_row(uint8_t row);
static void         unselect_rows(void);
static void         unselect_cols(void);
static void         select_row(uint8_t row);

static uint8_t pcal9539a_reset_loop;

static void init_mcu_pins(void) {
    unselect_rows();
    unselect_cols();
}

void matrix_init_custom(void) {
    debug_enable = true;
    debug_matrix = true;
    dprint("matrix_init_custom\n");
    // initialize row and col
    init_mcu_pins();
    pcal9539a_status = init_pcal9539a();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

// Reads and stores a row, returning
// whether a change occurred.
static inline bool store_matrix_row(matrix_row_t current_matrix[], uint8_t index) {
    matrix_row_t temp = read_cols(index);
    if (current_matrix[index] != temp) {
        current_matrix[index] = temp;
        return true;
    }
    return false;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    if (pcal9539a_status) {  // if there was an error
        if (++pcal9539a_reset_loop == 0) {
            // if (++pcal9539a_reset_loop >= 1300) {
            // since pcal9539a_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans
            // this will be approx bit more frequent than once per second
            print("trying to reset pcal9539a\n");
            pcal9539a_status = init_pcal9539a();
            if (pcal9539a_status) {
                print("right side not responding\n");
            } else {
                print("right side attached\n");
            }
        }
    }

    bool changed = false;
    for (uint8_t i = 0; i < MATRIX_ROWS_PER_SIDE; i++) {
        // select rows from left and right hands
        uint8_t left_index  = i;
        uint8_t right_index = i + MATRIX_ROWS_PER_SIDE;

        changed |= store_matrix_row(current_matrix, left_index);
        changed |= store_matrix_row(current_matrix, right_index);

        unselect_rows();
    }

    return changed;
}

static matrix_row_t read_cols(uint8_t row) {
    select_row(row);
    if (row < MATRIX_ROWS_PER_SIDE) {
        pin_t        matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
        matrix_row_t current_row_value                         = 0;
        matrix_io_delay();
        // For each col...
        for (uint8_t col_index = 0; col_index < MATRIX_COLS_PER_SIDE; col_index++) {
            // Select the col pin to read (active low)
            uint8_t pin_state = readPin(matrix_col_pins_mcu[col_index]);

            // Populate the matrix row with the state of the col pin
            current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
        }
        unselect_row(row);
        return current_row_value;
    } else {
        // we don't need a 30us delay anymore, because selecting a
        // right-hand row requires more than 30us for i2c.
        if (pcal9539a_status) {  // if there was an error
            return 0;
        } else {
            uint8_t buf[]   = {IN_0};
            pcal9539a_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), PCAL9539A_I2C_TIMEOUT);
            // We read all the pins on Port 0.
            // The initial state was all ones and any depressed key at a given column 
            // for the currently selected row will have its bit flipped to zero. 

            // The return value is a row as represented in the generic matrix code were the rightmost bits 
            // represent the lower columns and zeroes represent non-depressed keys while ones represent depressed keys.
            // Since the pins connected to eact columns are sequential, and counting from zero up (col 5 -> GPIOA0, col 6 -> GPIOA1 and so on), 
            // the only transformation needed is a bitwise not to swap all zeroes and ones.
            uint8_t data[] = {0};
            if (!pcal9539a_status) {
                pcal9539a_status = i2c_receive(I2C_ADDR_READ, data, sizeof(data), PCAL9539A_I2C_TIMEOUT);

                // TODO: consider doing inversion in hardware using register.
                data[0]         = ~(data[0]);

                // reverse bit order (temp solution, fix in next rev of expander breakout PCB)
                data[0] = (data[0] & 0xF0) >> 4 | (data[0] & 0x0F) << 4;
                data[0] = (data[0] & 0xCC) >> 2 | (data[0] & 0x33) << 2;
                data[0] = (data[0] & 0xAA) >> 1 | (data[0] & 0x55) << 1;
                data[0] >>= 2;
            }
            return data[0];
        }
    }
}


static void unselect_row(uint8_t row) {
    pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
    setPinInputHigh(matrix_row_pins_mcu[row]);
}


static void unselect_rows(void) {
    // no need to unselect on pcal9539a, because the select step sets all
    // the other row bits high, and it's not changing to a different
    // direction

    // unselect rows on microcontroller
    pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_ROWS_PER_SIDE; pin_index++) {
        pin_t pin = matrix_row_pins_mcu[pin_index];
        setPinInputHigh(pin);
    }
}


static void unselect_cols(void) {
    pin_t matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_COLS_PER_SIDE; pin_index++) {
        pin_t pin = matrix_col_pins_mcu[pin_index];
        setPinInputHigh(pin);
    }
}


static void select_row(uint8_t row) {
    if (row < MATRIX_ROWS_PER_SIDE) {
        // select on MCU
        pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
        pin_t pin                                       = matrix_row_pins_mcu[row];
        setPinOutput(pin);
        writePinLow(pin);
    } else {
        // select on pcal9539a
        if (pcal9539a_status) {  // if there was an error
                                // do nothing
        } else {
            // Select the desired row by writing a byte for the entire GPIOB bus where only the bit representing the row we want to select is a zero (write instruction) and every other bit is a one.
            // Note that the row - MATRIX_ROWS_PER_SIDE reflects the fact that being on the right hand, the columns are numbered from MATRIX_ROWS_PER_SIDE to MATRIX_ROWS, but the pins we want to write to are indexed from zero up on the GPIOB bus.
            uint8_t buf[]   = {OUT_1, 0xFF & ~(1 << (row - MATRIX_ROWS_PER_SIDE))};
            pcal9539a_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
        }
    }
}
