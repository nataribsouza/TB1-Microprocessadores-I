/* Includes */
#include "i2c.h"

/* Functions */

/**
 * @brief Init TWI hardware
 * 
 */
void init_i2c(void) {
    // Set I2C interface baudrate
    // f_i2c = f_cpu / (16 + 2 * TWBR)    
    set_reg(&TWBR, I2C_REGISTER_TWBR);

    // Enable I2C
    set_bit_reg(&TWCR, TWEN);
}

/**
 * @brief Set start condition in I2C bus
 * 
 */
void i2c_start(void) {
    // Start, enable and complet operation bits
    set_bit_reg(&TWCR, TWSTA);
    set_bit_reg(&TWCR, TWINT);

    // Wait until operation is completed
    while (!read_bit_reg(&TWCR, TWINT));
}

/**
 * @brief Write data in I2C bus
 * 
 * @param data 
 */
void i2c_write(uint8_t data) {
    // Write on data register
    set_reg(&TWDR, data);

    // Wait until operation is completed
    set_reg(&TWCR, (0x80 | 0x4));
    while (!read_bit_reg(&TWCR, TWINT));
}

/**
 * @brief Set stop condition in I2C bus
 * 
 */
void i2c_stop(void) {
    // Stop and wait until is completed
    set_bit_reg(&TWCR, TWSTO);
    set_bit_reg(&TWCR, TWINT);

    // Wait until operation is completed
    while (!read_bit_reg(&TWCR, TWSTO));
}