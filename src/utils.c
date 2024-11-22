/* Includes */
#include "utils.h"

/* Functions */

void set_bit_reg(uint8_t *reg, uint8_t index){
    *reg |= (1 << index);
}

void clear_bit_reg(uint8_t *reg, uint8_t index) {
    *reg &= !(1 << index);
}

void set_reg(uint8_t *reg, uint8_t value) {
    *reg = value;
}

void clear_reg(uint8_t *reg) {
    *reg = 0x00;
}

void init_pin(uint8_t pin, ENUM_PORT port_en, ENUM_PIN_MODE mode_en) {

    uint8_t *ddr = {0};
    uint8_t *port = {0};
    uint8_t *pin = {0};

    switch (port_en) {
    case ENUM_PORTB:
        ddr = &DDRB;
        port = &PORTB;
        pin = &PINB;
        break;
    
    case ENUM_PORTC:
        ddr = &DDRC;
        port = &PORTC;
        pin = &PINB;
        break;

    case ENUM_PORTD:
        ddr = &DDRD;
        port = &PORTD;
        pin = &PIND;
        break;

    }

    switch(mode) {
    case ENUM_INPUT:
        clear_bit_reg(ddr, pin);
        break;
    
    case ENUM_OUPUT:
        set_bit_reg(ddr, pin);
        clear_bit_reg(port, pin);
        break;

    case ENUM_INPUT_PULLUP:
        clear_bit_reg(ddr, pin);
        set_bit_reg(pin, pin);
        break;
    }
}

void set_pin() {

}

uint8_t read_pin() {
    
}