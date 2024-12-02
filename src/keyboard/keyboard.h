/**
 * @file keyboard.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/* Includes */
#include "../utils.h"
#include "../kernel/gpio.h"
#include "../kernel/timers.h"

/* Defines */
#define KEYBOARD_NUM_BUTTONS 6
#define KEYBOARD_PRESSING_TIME_MS 200
#define KEYBOARD_PIN_ROW_1 2
#define KEYBOARD_PIN_ROW_2 3
#define KEYBOARD_PIN_COLLUM_1 4
#define KEYBOARD_PIN_COLLUM_2 5
#define KEYBOARD_PIN_COLLUM_3 6

/* Enums */
typedef enum {
    ENUM_BUTTON_1,
    ENUM_BUTTON_2,
    ENUM_BUTTON_3,
    ENUM_BUTTON_4,
    ENUM_BUTTON_5,
    ENUM_BUTTON_6
} en_button;

/* Structures */
typedef struct {
    bool value;
    bool old_value;
    bool pressed;
    bool executed;
    uint8_t pin;
    uint32_t timer;
} st_button;

/* Prototypes */
void init_keyboard(void);
void handle_keyboard(void);
bool read_keyboard(uint8_t button);

#endif /* Keyboard */