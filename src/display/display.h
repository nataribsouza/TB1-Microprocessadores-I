/**
 * @file display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* Include */
#include "i2c.h"

/* Defines */
#define LCD_DISPLAY_NUM_COL 16
#define LCD_DISPLAY_NUM_ROW 2
#define LCD_I2C_ADDRESS 0x27 
#define LCD_NIBBLE_SHIFT 4
#define LCD_BIT_EN (0x04)
#define LCD_BIT_BACKLIGHT (0x08)
#define LCD_BIT_RS_SEND_DATA (0b01)
#define LCD_BIT_RS_SEND_COMMAND (0b00)
#define LCD_ADDR_ROW0 0x00
#define LCD_ADDR_ROW1 0x40
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_SET_MODE 0x02
#define LCD_CMD_SET_SIZE 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_SET_CURSOR_MODE 0x0C
#define LCD_CMD_SET_CURSOR_INCREMENT 0x06
#define LCD_DELAY_SHOW_START_SCREE 5000
#define LCD_SCREEN_START "   AspiranTec   "
#define LCD_SCREEN_LOGIN "SENHA: %4d"

/* Structures */
typedef struct {
    bool update;
    char row1[LCD_DISPLAY_NUM_COL+1]; 
    char row2[LCD_DISPLAY_NUM_COL+1]; 
}st_display;


/* Prototypes */
void init_display(void);
void display_nibble(uint8_t, uint8_t control);
void display_nibble_data(uint8_t nibble);
void display_nibble_command(uint8_t nibble);
void display_data(uint8_t data);
void display_command(uint8_t data);
void display_setCursor(uint8_t row, uint8_t col);
void display_print(const char *str);
void display_clear(void);

#endif /* DISPLAY_H */