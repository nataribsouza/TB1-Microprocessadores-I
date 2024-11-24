/**
 * @file utils.h
 * @author your name (you@domain.com)
 * @brief Utils functions and others 
 * @version 0.1
 * @date 2024-11-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UTILS_H
#define UTILS_h

/* Includes */
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Prototypes */
void set_bit_reg(volatile uint8_t *reg, uint8_t index);
void clear_bit_reg(volatile uint8_t *reg, uint8_t index);
bool get_bit_reg(volatile uint8_t *reg, uint8_t index);
void set_reg(volatile uint8_t *reg, uint8_t value);
void clear_reg(volatile uint8_t *reg);

#endif /* UTILS_H */