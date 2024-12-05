/* Includes */
#include "robot.h"

/* Functions */
void init_robot(st_robot *robot_st) {
    // Initialize led pin
    init_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, ENUM_PINMODE_OUTPUT);
    set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC,false);

    // Set robot structure
    robot_st->vacuum = false;
    robot_st->state_en = ENUM_STATE_INIT;
    robot_st->battery = ROBOT_BATTERY_CAPACITY_MA_S;
    robot_st->wakeup_time.hour = ROBOT_DFL_WAKEUP_TIME_H;
    robot_st->wakeup_time.minute = ROBOT_DFL_WAKEUP_TIME_MIN;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_0].valid = false;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_1].valid = false;
}

void robot_state_machine(st_robot *robot_st) {
    switch (robot_st->state_en) {
        case ENUM_STATE_INIT:
            init_robot(robot_st);
            robot_st->state_en = ENUM_STATE_READ_KEYBOARD;
            break;

        case ENUM_STATE_READ_KEYBOARD:
            handle_keyboard();
            robot_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
            break;

        case ENUM_STATE_UPDATE_DISPLAY:
            // Check if there are changes in display message
            if(robot_st->display_st.update = true) {
                display_clear();
                display_setCursor(0, 0);
                display_print(robot_st->display_st.row1);
                display_setCursor(1, 0);
                display_print(robot_st->display_st.row2);
            }
            robot_st->state_en = ENUM_STATE_BLINK_LED;
            break;

        case ENUM_STATE_BLINK_LED:
            // Check if battery is bellow 10%
            if(robot_st->battery * 100 / ROBOT_BATTERY_CAPACITY_MA_S < 10) {
                blink_led();
            } else {
                set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC,false);
            }
            robot_st->state_en = ENUM_STATE_CHECK_SERIAL;
            break;

        case ENUM_STATE_CHECK_SERIAL:
            break;

        case ENUM_STATE_MOVE:
            break;
        
        default:
            robot_st->state_en = ENUM_STATE_INIT;
            break;
    }
}

void blink_led(void) {
  static uint32_t timer = 0;

  if(millis() - timer >= ROBOT_TIME_BLINK_LED_MS) {
    timer = millis();
    bool state = read_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC);
    set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, !state);
  }
}