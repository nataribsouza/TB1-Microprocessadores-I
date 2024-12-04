/**
 * @file regLib.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef REGLIB_H
#define REGLIB_H

/* Includes */
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Prototypes */
void set_bit_reg(volatile uint8_t *reg, uint8_t index);
void clear_bit_reg(volatile uint8_t *reg, uint8_t index);
bool read_bit_reg(volatile uint8_t *reg, uint8_t index);
void set_reg(volatile uint8_t *reg, uint8_t value);
void clear_reg(volatile uint8_t *reg);

#endif /* REGLIB_H */