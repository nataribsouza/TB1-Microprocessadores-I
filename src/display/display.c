/* Includes */
#include "display.h"

/* Functions */

/**
 * @brief 
 * 
 */
void init_display(void) {
    // Wait stabilize
    delay(50);

    // Set 4-bit mode
    display_nibble_command(LCD_CMD_SET_MODE);

    // Set 2 rows and 5x8 matrix
    display_command(LCD_CMD_SET_SIZE);
    // Turn on display, no blink cursor
    display_command(LCD_CMD_SET_CURSOR_MODE);

    display_clear();
    delay(2);

    // Display positive increment
    display_command(LCD_CMD_SET_CURSOR_INCREMENT);
}

/**
 * @brief Send a half-byte data
 * 
 * @param nibble 
 */
void display_nibble_data(uint8_t nibble) {
    // Shift nibble 
    uint8_t data = (nibble << LCD_NIBBLE_SHIFT);

    // Set backlight on
    data |= LCD_BIT_BACKLIGHT;

    // Select display to receive data
    data |= LCD_BIT_RS_SEND_DATA;

    // Send data steps
    i2c_start();
    i2c_write(LCD_I2C_ADDRESS << 1);
    i2c_write(data | LCD_BIT_EN);
    delay(1);
    i2c_write(data & ~LCD_BIT_EN);
    delay(1);
    i2c_stop();
}

/**
 * @brief Send a half-byte command
 * 
 * @param nibble 
 */
void display_nibble_command(uint8_t nibble) {
    // Shift nibble 
    uint8_t data = (nibble << LCD_NIBBLE_SHIFT);

    // Set backlight on
    data |= LCD_BIT_BACKLIGHT;

    // Select display to receive data
    data |= LCD_BIT_RS_SEND_COMMAND;

    // Send data steps
    i2c_start();
    i2c_write(LCD_I2C_ADDRESS << 1);
    i2c_write(data | LCD_BIT_EN);
    delay(1);
    i2c_write(data & ~LCD_BIT_EN);
    delay(1);
    i2c_stop();
}

/**
 * @brief Send data
 * 
 * @param data 
 */
void display_data(uint8_t data) {
    // Send superior nibble
    display_nibble_data(data >> LCD_NIBBLE_SHIFT);
    // Send inferior nibble
    display_nibble_data(data & 0x0F);
    delay(2);
}


/**
 * @brief Send Command
 * 
 * @param data 
 */
void display_command(uint8_t data) {
    // Send superior nibble
    display_nibble_command(data >> LCD_NIBBLE_SHIFT);
    // Send inferior nibble
    display_nibble_command(data & 0x0F);
    delay(2);
}

/**
 * @brief Set cursor row and collum
 * 
 * @param row 
 * @param col 
 */
void display_setCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0 ? LCD_ADDR_ROW0 : LCD_ADDR_ROW1) + col;
    display_command(LCD_CMD_SET_CURSOR | address);
}

/**
 * @brief Print a string on display
 * 
 * @param str 
 */
void display_print(const char *str) {
    while (*str) {
        display_data(*str++);
    }
}

char string[8] = "hello mundo";
serial_print(string);

/**
 * @brief Clear display
 * 
 */
void display_clear(void) {
    display_command(LCD_CMD_CLEAR);
    delay(2);
}

void display_set_line(st_display *display_st,  uint8_t row, const char *str) {
    switch (row){
    case LCD_ADDR_ROW0:
        strcpy(display_st->row0, str);
        break;

    case LCD_ADDR_ROW1:
        strcpy(display_st->row1, str);
        break;

    default:
        break;
    }

    display_st->update = true;    
}