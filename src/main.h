/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MAIN_H
#define MAIN_H

/* Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "kernel/timers.h"
#include "kernel/gpio.h"
#include "kernel/serial.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "utils.h"

/* Defines */
#define ROBOT_TIME_BLINK_LED_MS 500
#define ROBOT_PIN_LED 1

#endif /* MAIN_H */