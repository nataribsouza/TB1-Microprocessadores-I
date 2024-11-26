/**
 * @file gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GPIO_H
#define GPIO_H

/* Includes */
#include <stdbool.h>
#include "../utils.h"

/* Defines */

/* Enums */
typedef enum {
    ENUM_PORT_PORTB,
    ENUM_PORT_PORTC,
    ENUM_PORT_PORTD
} en_port;

typedef enum {
    ENUM_PINMODE_INPUT,
    ENUM_PINMODE_OUTPUT,
    ENUM_PINMODE_INPUTPULLUP
} en_pinMode;

/* Prototypes */
void init_pin(uint8_t pin, en_port port_en, en_pinMode mode_en);
void set_pin(uint8_t pin, en_port port_en, bool state);
bool read_pin(uint8_t pin, en_port port_en);
void port_mask(en_port port_en, volatile uint8_t **ddr, volatile uint8_t **port, volatile uint8_t **pin);

#endif /* GPIO_H */