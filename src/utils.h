/**
 * @file utils.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UTILS_H
#define UTILS_H

/* Includes */
#include <avr/wdt.h>
#include <avr/interrupt.h>

/* Defines */
#define CONVERT_TO_PERCENTAGE 100
#define DEC_BASE 10
#define MAX_HOUR 23
#define MAX_MINUTE 59

/* Structs */
typedef struct {
    uint8_t hour;
    uint8_t minute;
} st_time;

/* Prototypes */
void reboot(void);

#endif /* UTILS_H */