/**
 * @file timers.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TIMERS_H
#define TIMERS_H

/* Includes */
#include <avr/interrupt.h>
#include "../utils.h"

/* Defines */
#define TIMER0_PRESCALER 64 
#define TIMER0_ONCOMPARE_RESET_VALUE 249

/* Prototypes */
void timer_init(void);
void delay(uint16_t ms);
uint32_t millis(void);

#endif /* TIMERS_H */