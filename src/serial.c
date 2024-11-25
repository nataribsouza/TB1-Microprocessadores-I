/* Includes */
#include "serial.h"

/* Functions */

/**
 * @brief Initialize Serial 0
 * 
 */
void serial_begin(void) {
    // Serial mode 19200 8-N-1

    // Set baudrate to 19200 bps
    clear_reg(&UBRR0H);
    set_reg(&UBRR0L, SERIAL_UBRR0H);

    // Enable transmition and reception
    set_bit_reg(&UCSR0B, TXEN0);
    set_bit_reg(&UCSR0B, RXEN0);

    // Set 8-bit data length
    set_bit_reg(&UCSR0C, UCSZ00);
    set_bit_reg(&UCSR0C, UCSZ01);

    set_bit_reg(&UCSR0C, UCPOL0);
}

/**
 * @brief Verify if there is received data
 * that wasn´t be readed
 * 
 * @return true 
 * @return false 
 */
bool serial_available(void) {
    return read_bit_reg(&UCSR0A, RXC0);
}

/**
 * @brief Read serial received data
 * 
 * @return uint8_t 
 */
uint8_t serial_read(void) {
    return UDR0;
}

/**
 * @brief Write data on serial
 * 
 * @param data 
 */
void serial_write(uint8_t data) {
    // Wait until transmit register is empty
    while(read_bit_reg(&UCSR0A, UDRE0) == 0);

    UDR0 = data;
}

/**
 * @brief Write an array of bytes in the serial
 * 
 * @param data 
 * @param length 
 */
void serial_print(uint8_t *data, uint16_t length) {
    for(int i = 0; i < length; i++) {
        serial_write(data[i]);
    }
}