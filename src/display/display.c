/* Includes */
#include "display.h"

/* Functions */

// Funções de controle do TWI
void i2c(void) {


    
    // Set I2C interface baudrate
    // f_i2c = f_cpu / (16 + 2 * TWBR)    
    TWBR = I2C_REGISTER_TWBR;              // SCL = 100 kHz (F_CPU = 16 MHz)
    TWCR = (1 << TWEN);     // Habilita TWI
}

void TWI_start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    while (TWCR & (1 << TWSTO));
}

void LCD_sendNibble(uint8_t nibble, uint8_t control) {
    uint8_t data = (nibble << 4) | control | 0x08;  // Mantém o backlight ativo
    TWI_start();
    TWI_write(SLA_LCD << 1);  // Endereço do escravo em modo escrita
    TWI_write(data | 0x04);   // EN = 1
    delay(1);
    TWI_write(data & ~0x04);  // EN = 0
    delay(50);
    TWI_stop();
}

void LCD_sendByte(uint8_t byte, uint8_t control) {
    LCD_sendNibble(byte >> 4, control);  // Envia o nibble superior
    LCD_sendNibble(byte & 0x0F, control); // Envia o nibble inferior
}

void LCD_command(uint8_t command) {
    LCD_sendByte(command, 0x00);  // RS = 0 para comando
    delay(2);
}

void LCD_data(uint8_t data) {
    LCD_sendByte(data, 0x01);  // RS = 1 para dado
    delay(2);
}

void LCD_init(void) {
    delay(50);  // Aguarda estabilização do LCD
    LCD_sendNibble(0x03, 0x00);  // Modo 8 bits
    delay(5);
    LCD_sendNibble(0x03, 0x00);
    delay(1);
    LCD_sendNibble(0x03, 0x00);
    LCD_sendNibble(0x02, 0x00);  // Modo 4 bits

    // Configurações básicas do LCD
    LCD_command(0x28);  // Modo 4 bits, 2 linhas, 5x8
    LCD_command(0x0C);  // Display ligado, cursor desligado
    LCD_command(0x01);  // Limpa display
    delay(2);
    LCD_command(0x06);  // Incrementa cursor
}

void LCD_setCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0 ? 0x00 : 0x40) + col;
    LCD_command(0x80 | address);
}

void LCD_print(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}