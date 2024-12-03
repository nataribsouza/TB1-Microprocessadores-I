/**
 * @file i2c.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef I2C_H
#define I2C_H

/* Includes */
#include "../utils.h"
#include "../kernel/timers.h"

/* Defines */
#define I2C_BAUDRATE 100000
#define I2C_REGISTER_TWBR (F_CPU / (16 + 2*I2C_BAUDRATE))

/* Prototypes */
void init_i2c(void);
void i2c_start(void);
void i2c_write(uint8_t data);
void i2c_stop(void);

#endif /* I2C_H*/