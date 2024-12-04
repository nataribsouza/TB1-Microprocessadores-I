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
#include "kernel/regLib.h"
#include "kernel/gpio.h"
#include "kernel/timers.h"
#include "kernel/serial.h"

/* Defines */
#define ROBOT_NUM_BLOCKED_AREAS 2
#define ROBOT_BATTERY_CAPACITY_MA_S 36000
#define ROBOT_BATTERY_CONSUMPTION_STD_MA_S 30
#define ROBOT_BATTERY_CONSUMPTION_VACUUM_MA_S 180

/* Enums */
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
    uint8_t x;
    uint8_t y;
    uint8_t center;
} st_blocked_area;

typedef struct {
    bool vacuum;
    en_user user_en;
    uint16_t battery;
    st_time wakeup_time;
    st_blocked_area blocked_area_st[ROBOT_NUM_BLOCKED_AREAS];
}st_robot;

/* Prototypes */
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

#endif /* ROBOT_H */