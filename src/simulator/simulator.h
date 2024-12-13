/**
 * @file simulator.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

/* Includes */
#include <stdio.h>
#include <stdbool.h>
#include "../utils.h"
#include "../kernel/serial.h"

/* Defines */
#define ROOM_LENGTH 10
#define ROOM_HEIGHT 8
#define ENVIROMENT_INIT_TEMPERATURE 25
#define SIMULATOR_BUFFER_LENGTH 8
#define PROTOCOL_SENDER_INDEX 0
#define PROTOCOL_COMMAND_INDEX 1
#define PROTOCOL_COMMAND_INFO_OBSTACLE_INDEX 2
#define PROTOCOL_COMMAND_INFO_DUST_INDEX 3
#define PROTOCOL_COMMAND_CLOCK_HOUR_INDEX 2
#define PROTOCOL_COMMAND_CLOCK_MINUTE_INDEX 3
#define PROTOCOL_COMMAND_TEMPERATURE_TEMP_INDEX 2
#define PROTOCOL_SENDER_ROBOT 'A'
#define PROTOCOL_SENDER_SIMULATOR 'S'
#define PROTOCOL_COMMAND_VACCUUM_TURN_ON 'A'
#define PROTOCOL_COMMAND_VACCUUM_TURN_OFF 'D'
#define PROTOCOL_COMMAND_MOVEMENT 'M'
#define PROTOCOL_COMMAND_MOVEMENT_FAIL 'X'
#define PROTOCOL_COOMAND_MOP_TURN_ON 'R'
#define PROTOCOL_COOMAND_MOP_TURN_OFF 'P'
#define PROTOCOL_COMMAND_TEMPERATURE 'T'
#define PROTOCOL_COMMAND_INFO 'I'
#define PROTOCOL_COMMAND_CLOCK 'H'
#define PROTOCOL_ROBOT_ANSWER "%c%c"

/* Enums */
typedef enum{
    ENUM_MOVE_UNDEFINED,
    ENUM_MOVE_FORWARD,
    ENUM_MOVE_TURN_LEFT,
    ENUM_MOVE_TURN_RIGHT
} en_move;

/* Structures */
typedef struct {
    bool valid;
    uint8_t collected_dust;
} st_dust;

typedef struct {
    bool valid;
    int8_t old_temperature;
    int8_t temperature;
} st_temperature;

typedef struct {
    bool valid;
    bool obstacle;
} st_obstacle;

typedef struct {
    bool valid;
    st_time time_st;
} st_clock;

typedef struct {
    bool valid;
    bool state;
} st_vaccuum;

typedef struct {
    bool valid;
    bool state;
} st_mop;

typedef struct {
    bool valid;
    en_move move_en;
} st_move;

typedef struct {
    st_mop mop_st;
    st_vaccuum vaccuum_st;
    st_dust dust_st;
    st_temperature temperature_st;
    st_obstacle obstacle_st;
    st_clock clock_st;
    st_move move_en;
} st_environment;

/* Prototypes */
void init_environment(st_environment *environment_st);
void simulator_handle_receive(st_environment *environment_st);
void simulator_move(en_move move_en);

#endif /* SIMULATOR_H */