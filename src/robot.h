/**
 * @file robot.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ROBOT_H
#define ROBOT_H

/* Includes */
#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kernel/regLib.h"
#include "kernel/gpio.h"
#include "kernel/timers.h"
#include "kernel/serial.h"
#include "display/display.h"
#include "keyboard/keyboard.h"

/* Defines */
#define CONVERT_TO_PERCENTAGE 100
#define DEC_BASE 10
#define ROOM_LENGTH 10
#define ROOM_HEIGHT 8
#define MAX_HOUR 23
#define MAX_MINUTE 59
#define ROBOT_USER_PASSWORD_ADM "1325"
#define ROBOT_USER_PASSWORD_COM "4213"
#define ROBOT_PIN_LED 1
#define ROBOT_TIME_BLINK_LED_MS 500
#define ROBOT_NUM_BLOCKED_AREAS 2
#define ROBOT_BATTERY_CAPACITY_MA_S 36000
#define ROBOT_BATTERY_CRITICAL_PERCENTAGE 10
#define ROBOT_BATTERY_CONSUMPTION_STD_MA_S 30
#define ROBOT_BATTERY_CONSUMPTION_VACUUM_MA_S 180
#define ROBOT_DFL_WAKEUP_TIME_H 9
#define ROBOT_DFL_WAKEUP_TIME_MIN 0
#define ROBOT_BLOCKED_ARAE_0 0
#define ROBOT_BLOCKED_ARAE_1 1
#define ROBOT_TIME_UPDATE_DISPLAY_MS 200

/* Enums */
typedef enum {
    ENUM_STATE_INIT,
    ENUM_STATE_READ_KEYBOARD,
    ENUM_STATE_UPDATE_DISPLAY,
    ENUM_STATE_BLINK_LED,
    ENUM_STATE_CHECK_SERIAL,
    ENUM_STATE_MOVE
} en_state;

typedef enum{
    ENUM_NO_USER,
    ENUM_ADM_USER,
    ENUM_NORMAL_USER
} en_user;

/* Structs */
typedef struct {
    uint8_t hour;
    uint8_t minute;
} st_time;

typedef struct {
    bool valid;
    uint8_t length;
    uint8_t height;
    uint8_t center;
} st_blocked_area;

typedef struct {
    bool vacuum;
    en_state state_en;
    en_user user_en;
    uint16_t battery;
    st_time wakeup_time;
    st_blocked_area blocked_area_st[ROBOT_NUM_BLOCKED_AREAS];
    st_display display_st;
} st_robot;

/* Prototypes */
void reboot(void);
void blink_led(void);
void update_display(st_robot *robot_st);
void handle_keyboard(st_robot *robot_st);
void handle_display(st_robot *robot_st, char bt);
void init_robot(st_robot *robot_st);
void robot_state_machine(st_robot *robot_st);
void robot_clean_edges(st_robot *robot_st);
void robot_clean_room(st_robot *robot_st);
void robot_start_vacuum(st_robot *robot_st);
void robot_stop_vacuum(st_robot *robot_st);
void robot_move_forwar(st_robot *robot_st);
void robot_turn_right(st_robot *robot_st);
void robot_turn_left(st_robot *robot_st);
void robot_goback_base(st_robot *robot_st);
bool robot_check_obstacle(st_robot *robot_st);
char identify_button(uint8_t bt);
uint8_t calculate_battery_level(uint16_t battery);

#endif /* ROBOT_H */