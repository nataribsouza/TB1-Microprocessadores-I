/* Includes */
#include "gpio.h"

/* Functions */

/**
 * @brief Configure register for pin initialization
 * 
 * @param pin Pin number
 * @param port_en  Pin port (PORTB, PORTC or PORTD)
 * @param mode_en  Pin mode (input, output or input-pullup)
 */
void init_pin(uint8_t pin, en_port port_en, en_pinMode mode_en) {
    volatile uint8_t *DDRX = {0};
    volatile uint8_t *PORTX = {0};
    volatile uint8_t *PINX = {0};

    port_mask(port_en, &DDRX, &PORTX, &PINX);
    
    switch(mode_en) {
        case ENUM_PINMODE_INPUT:
            clear_bit_reg(DDRX, pin);
            break;
        
        case ENUM_PINMODE_OUTPUT:
            set_bit_reg(DDRX, pin);
            clear_bit_reg(PORTX, pin);
            break;

        case ENUM_PINMODE_INPUTPULLUP:
            clear_bit_reg(DDRX, pin);
            set_bit_reg(PINX, pin);
            break;
    }
}

/**
 * @brief Set the pin output value
 * 
 * @param pin 
 * @param port_en 
 * @param state 
 */
void set_pin(uint8_t pin, en_port port_en, bool state) {
    volatile uint8_t *DDRX = {0};
    volatile uint8_t *PORTX = {0};
    volatile uint8_t *PINX = {0};

    port_mask(port_en, &DDRX, &PORTX, &PINX);

    state ? set_bit_reg(PORTX, pin) : clear_bit_reg(PORTX, pin);
}

/**
 * @brief Return the readed digital pin value
 * 
 * @param pin 
 * @param port_en 
 * @return true 
 * @return false 
 */
bool read_pin(uint8_t pin, en_port port_en) {
    volatile uint8_t *DDRX = {0};
    volatile uint8_t *PORTX = {0};
    volatile uint8_t *PINX = {0};

    port_mask(port_en, &DDRX, &PORTX, &PINX);

    return get_bit_reg(PINX, pin);
}

/**
 * @brief Configure the register pointer based
 * in what port do you want to set
 * 
 * @param port_en 
 * @param ddr 
 * @param port 
 * @param pin 
 */
void port_mask(en_port port_en, volatile uint8_t **ddr, volatile uint8_t **port, volatile uint8_t **pin) {
    switch (port_en) {
        case ENUM_PORT_PORTB:
            *ddr = &DDRB;
            *port = &PORTB;
            *pin = &PINB;
            break;
        
        case ENUM_PORT_PORTC:
            *ddr = &DDRC;
            *port = &PORTC;
            *pin = &PINC;
            break;

        case ENUM_PORT_PORTD:
            *ddr = &DDRD;
            *port = &PORTD;
            *pin = &PIND;
            break;
    }
}