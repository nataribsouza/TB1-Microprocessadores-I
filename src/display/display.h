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
#define LCD_ROW0 0
#define LCD_ROW1 1
#define LCD_BUFFER_LEN 17
#define LCD_ADDR_ROW0 0x00
#define LCD_ADDR_ROW1 0x40
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_SET_MODE 0x02
#define LCD_CMD_SET_SIZE 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_SET_CURSOR_MODE 0x0C
#define LCD_CMD_SET_CURSOR_INCREMENT 0x06
#define LCD_DELAY_SHOW_START_SCREE 2000
#define LAST_POS_4CHAR_STR 3
#define LAST_POS_3CHAR_STR 2
#define LAST_POS_2CHAR_STR 1
#define ASCII_NUM_0
#define ASCII_NUM_9

/* Screens display */
// Start 
#define LCD_SCREEN_START "   AspiranTec   "
// Login 
#define LCD_SCREEN_LOGIN_R0W0 "SENHA: %s"
#define LCD_SCREEN_LOGIN_R0W1 "[#] Enter"
// Main menu user adm
#define LCD_SCREEN_MAIN_MENU_USER_ADM_ROW0 "[1]Nor   [2]Cant"
#define LCD_SCREEN_MAIN_MENU_USER_ADM_ROW1 "[3]Blk   [4]Hora"
// Select block area
#define LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW0 "Selecione a area"
#define LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW1 "[1] 1      [2] 2"
// Select block area length
#define LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0 "Larg da area"
#define LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1 "%s   [#] Enter"
// Select block area height
#define LCD_SCREEN_SLCT_HGT_USER_ADM_ROW0 "Altr da area"
#define LCD_SCREEN_SLCT_HGT_USER_ADM_ROW1 "%s   [#] Enter"
// Select block area center
#define LCD_SCREEN_SLCT_CNT_USER_ADM_ROW0 "Cent da area"
#define LCD_SCREEN_SLCT_CNT_USER_ADM_ROW1 "%s   [#] Enter"
// Change wakup time
#define LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW0 "Hora limp normal"
#define LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW1 "%s:%s  [#] Enter"
// Main menu use commum
#define LCD_SCREEN_MAIN_MENU_USER_COM_ROW0 "[1]Nor   [2]Cant"
#define LCD_SCREEN_MAIN_MENU_USER_COM_ROW1 ""

/* Enums */
typedef enum {
    ENUM_SCREEN_LOGIN,
    ENUM_SCREEN_MAIN_MENU_USER_ADM,
    ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM,
    ENUM_SCREEN_SLCT_LEN_USER_ADM,
    ENUM_SCREEN_SLCT_HGT_USER_ADM,
    ENUM_SCREEN_SLCT_CNT_USER_ADM,
    ENUM_SCREEN_CHNG_HOUR_USER_ADM,
    ENUM_SCREEN_MAIN_MENU_USER_COM
} en_screen;

/* Structures */
typedef struct {
    bool update;
    char row0[LCD_DISPLAY_NUM_COL+1]; 
    char row1[LCD_DISPLAY_NUM_COL+1]; 
    en_screen screen_en;
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
void display_set_line(st_display *display_st, uint8_t row,  const char *str);

#endif /* DISPLAY_H */