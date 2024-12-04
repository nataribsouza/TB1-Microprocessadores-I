/* Includes */
#include "regLib.h"

/* Functions */

/**
 * @brief Set a specific bit of a register
 * 
 * @param reg 
 * @param index 
 */
void set_bit_reg(volatile uint8_t *reg, uint8_t index){
    *reg |= (1 << index);
}

/**
 * @brief Clear a specific bit of a register
 * 
 * @param reg 
 * @param index 
 */
void clear_bit_reg(volatile uint8_t *reg, uint8_t index) {
    *reg &= ~(1 << index);
}

/**
 * @brief Get a specific bit of a register
 * 
 * @param reg 
 * @param index 
 * @return true 
 * @return false 
 */
bool read_bit_reg(volatile uint8_t *reg, uint8_t index) {
    return (*reg & (1 << index));
}


/**
 * @brief Set the whole register value
 * 
 * @param reg 
 * @param value 
 */
void set_reg(volatile uint8_t *reg, uint8_t value) {
    *reg = value;
}

/**
 * @brief Clear the whole register
 * 
 * @param reg 
 */
void clear_reg(volatile uint8_t *reg) {
    *reg = 0x00;
}