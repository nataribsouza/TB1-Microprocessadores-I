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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "kernel/regLib.h"
#include "kernel/gpio.h"
#include "kernel/timers.h"
#include "kernel/serial.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "simulator/simulator.h"

/* Defines */
#define ROBOT_USER_PASSWORD_ADM "1325"
#define ROBOT_USER_PASSWORD_COM "4213"
#define ROBOT_PIN_LED 1
#define ROBOT_NUM_BLOCKED_AREAS 2
#define ROBOT_TIME_BLINK_LED_MS 250
#define ROBOT_TIME_MOVEMENT_MS 1000
#define ROBOT_DUST_CAPACITY_G 250
#define ROBOT_DUST_MAX_PERCENTAGE (ROBOT_DUST_CAPACITY_G*0.9)
#define ROBOT_BATTERY_CAPACITY_MA_S 36000
#define ROBOT_BATTERY_CONSUMPTION_STD_MA_S 30
#define ROBOT_BATTERY_CONSUMPTION_VACUUM_MA_S 180
#define ROBOT_BATTERY_CONSUMPTION_MOP_MA_S 120
#define ROBOT_DFL_WAKEUP_TIME_H 9
#define ROBOT_DFL_WAKEUP_TIME_MIN 0
#define ROBOT_BLOCKED_ARAE_0 0
#define ROBOT_BLOCKED_ARAE_1 1
#define ROBOT_TIME_UPDATE_DISPLAY_MS 200
#define ROBOT_POSITION_X_INDEX 0
#define ROBOT_POSITION_Y_INDEX 1
#define ROBOT_INITIAL_POSITION_X 9
#define ROBOT_INITIAL_POSITION_Y 0

/* Enums */
typedef enum {
    ENUM_STATE_INIT,
    ENUM_STATE_READ_KEYBOARD,
    ENUM_STATE_UPDATE_DISPLAY,
    ENUM_STATE_BLINK_LED,
    ENUM_STATE_CHECK_SERIAL,
    ENUM_STATE_MOVE
} en_state_machine;

typedef enum {
    ENUM_STATE_NO_CLEANING,
    ENUM_STATE_CLEAN_NORMAL,
    ENUM_STATE_CLEAN_EDGES
} en_cleaning_mode;

typedef enum{
    ENUM_ORIENTATION_NORTH,
    ENUM_ORIENTATION_WEST,
    ENUM_ORIENTATION_SOUTH,
    ENUM_ORIENTATION_EST
} en_orient;

/* Structs */
typedef struct {
    uint8_t x;
    uint8_t y;
} st_position;

typedef struct {
    uint8_t length;
    uint8_t height;
    st_position position_st;
} st_blocked_area;

typedef struct {
    bool mop;
    bool vacuum;
    bool docked;
    uint8_t obstacle;

    uint8_t dust;    
    uint16_t battery;
    st_time time;
    st_blocked_area blocked_area_st[ROBOT_NUM_BLOCKED_AREAS];
    en_cleaning_mode cleaning_mode_en;

    en_orient orientation_en;
    st_position position_st;

    st_display display_st;

    en_state_machine state_machine_en;
} st_robot;

/* Prototypes */
void state_machine(st_robot *robot_st, st_environment *environment_st);
void blink_led(void);
void recalculate_battery(st_robot *robot_st);
void update_display(st_robot *robot_st);
void handle_movement(st_robot *robot_st, st_environment *environment_st);
bool check_move_forward(st_robot *robot_st);
bool check_block_area(uint8_t x, uint8_t y, st_blocked_area area);
void update_environment_parameters(st_robot *robot_st, st_environment *environment_st);
void handle_keyboard(st_robot *robot_st, st_environment *environment_st);
void handle_display(st_robot *robot_st,  st_environment *environment_st, char bt);
void init_robot(st_robot *robot_st);
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
uint8_t calculate_dust_level(uint8_t dust);

#endif /* ROBOT_H */