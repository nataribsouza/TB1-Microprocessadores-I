/**
 * @file serial.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERIAL_H
#define SERIAL_H

/* Includes */
#include "regLib.h"

/* Defines */
#define SERIAL_BAUDRATE 19200
#define SERIAL_UBRR0H (((F_CPU / 16) / SERIAL_BAUDRATE) - 1)

/* Prototypes */
void serial_begin(void);
void serial_write(uint8_t data);
void serial_print(const char *str);
bool serial_available(void);
uint8_t serial_read(void);

#endif /* SERIAL_H */