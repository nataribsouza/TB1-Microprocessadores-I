/**
 * @file display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* Include */
#include "../utils.h"
#include "../kernel/timers.h"

/* Defines */
#define I2C_BAUDRATE 100000
#define I2C_REGISTER_TWBR (F_CPU / (16 + 2*I2C_BAUDRATE))
#define SLA_LCD 0x27 

/* Prototypes */
void TWI_init(void);
void TWI_start(void);
void TWI_write(uint8_t data);
void TWI_stop(void);
void LCD_sendNibble(uint8_t nibble, uint8_t control);
void LCD_sendByte(uint8_t byte, uint8_t control);
void LCD_command(uint8_t command);
void LCD_data(uint8_t data);
void LCD_init(void);
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_print(const char *str);

#endif /* DISPLAY_H*/