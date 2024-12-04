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
#include "../kernel/regLib.h"
#include "../kernel/gpio.h"
#include "../kernel/timers.h"

/* Defines */
#define KEYBOARD_NUM_BUTTONS 12
#define KEYBOARD_PRESSING_TIME_MS 200
#define KEYBOARD_PIN_ROW_0 2
#define KEYBOARD_PIN_ROW_1 3
#define KEYBOARD_PIN_ROW_2 4
#define KEYBOARD_PIN_ROW_3 5
#define KEYBOARD_PIN_COLLUM_0 6
#define KEYBOARD_PIN_COLLUM_1 7
#define KEYBOARD_PIN_COLLUM_2 0

/* Enums */
typedef enum {
    ENUM_BUTTON_1,
    ENUM_BUTTON_2,
    ENUM_BUTTON_3,
    ENUM_BUTTON_4,
    ENUM_BUTTON_5,
    ENUM_BUTTON_6,
    ENUM_BUTTON_7,
    ENUM_BUTTON_8,
    ENUM_BUTTON_9,
    ENUM_BUTTON_STAR,
    ENUM_BUTTON_0,
    ENUM_BUTTON_HASHTAG
} en_button;

/* Structures */

/**
 * @brief Hold all needed variables
 * for button control
 * 
 */
typedef struct {
    bool value;
    bool old_value;
    bool pressed;
    bool executed;
    uint32_t timer;
    uint8_t pin;
    en_port port;
} st_button;

/* Prototypes */
void init_keyboard(void);
void handle_keyboard(void);
bool read_keyboard(uint8_t button);

#endif /* Keyboard */