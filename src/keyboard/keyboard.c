/* Includes */
#include "keyboard.h"

/* Variables */
st_button keyboard[KEYBOARD_NUM_BUTTONS];

/* Functions */
/**
 * @brief Initialize keyboard pins
 * 
 */
void init_keyboard(void) {
    // Set pins to keyboard buttons
    keyboard[ENUM_BUTTON_1].pin = KEYBOARD_PIN_COLLUM_1;
    keyboard[ENUM_BUTTON_2].pin = KEYBOARD_PIN_COLLUM_2;
    keyboard[ENUM_BUTTON_3].pin = KEYBOARD_PIN_COLLUM_3;
    keyboard[ENUM_BUTTON_4].pin = KEYBOARD_PIN_COLLUM_1;
    keyboard[ENUM_BUTTON_5].pin = KEYBOARD_PIN_COLLUM_2;
    keyboard[ENUM_BUTTON_6].pin = KEYBOARD_PIN_COLLUM_3;
    
    // Init input pins
    init_pin(KEYBOARD_PIN_COLLUM_1, ENUM_PORT_PORTD, ENUM_PINMODE_INPUTPULLUP);
    init_pin(KEYBOARD_PIN_COLLUM_2, ENUM_PORT_PORTD, ENUM_PINMODE_INPUTPULLUP);
    init_pin(KEYBOARD_PIN_COLLUM_3, ENUM_PORT_PORTD, ENUM_PINMODE_INPUTPULLUP);

    // Init output pins
    init_pin(KEYBOARD_PIN_ROW_1, ENUM_PORT_PORTD, ENUM_PINMODE_OUTPUT);
    init_pin(KEYBOARD_PIN_ROW_2, ENUM_PORT_PORTD, ENUM_PINMODE_OUTPUT);

    // Set output pins as low
    set_pin(KEYBOARD_PIN_ROW_1, ENUM_PORT_PORTD, true);
    set_pin(KEYBOARD_PIN_ROW_2, ENUM_PORT_PORTD, true);
}

/**
 * @brief Handle keyboard, reading 
 * all buttons with pooling
 * 
 */
void handle_keyboard(void) {
    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
        st_button *button = &keyboard[i];

        // Select rows
        if(i < (KEYBOARD_NUM_BUTTONS)/2) {
            set_pin(KEYBOARD_PIN_ROW_1, ENUM_PORT_PORTD, false);
            set_pin(KEYBOARD_PIN_ROW_2, ENUM_PORT_PORTD, true);
        } else {
            set_pin(KEYBOARD_PIN_ROW_1, ENUM_PORT_PORTD, true);
            set_pin(KEYBOARD_PIN_ROW_2, ENUM_PORT_PORTD, false);
        }

        button->old_value = button->value;
        button->value = read_pin(button->pin, ENUM_PORT_PORTD);

        // If button unpressed
        if(button->value == true) {
            button->pressed = false;
            button->executed = false;
        }

        // Set timestamp when pressed
        if(button->value == false && button->old_value == true) {
            button->timer = millis();
        }

        // If button was continues pressed for a determinated time
        if(millis()-button->timer >= KEYBOARD_PRESSING_TIME_MS && button->value == false) {
            button->pressed = true;
        }
    }
}

/**
 * @brief Return true one single
 * time if the button was pressed
 * 
 * @param button 
 * @return true 
 * @return false 
 */
bool read_keyboard(uint8_t button) {
    bool pressed = keyboard[button].pressed;
    bool executed = keyboard[button].executed;

    /* Check if button was pressed and 
    command was not executed yet */
    if(executed || !pressed) {
        return false;
    } else {
        keyboard[button].executed = true;
        return true;
    }
}