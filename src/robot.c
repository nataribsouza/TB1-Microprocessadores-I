/* Includes */
#include "robot.h"

/* Functions */
void init_robot(st_robot *robot_st) {
    // Initialize led pin
    init_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, ENUM_PINMODE_OUTPUT);
    set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC,false);

    // Set display structure
    robot_st->display_st.update = true;
    sprintf(robot_st->display_st.row0, LCD_SCREEN_LOGIN_R0W0, "");
    sprintf(robot_st->display_st.row0, LCD_SCREEN_LOGIN_R0W1);

    // Set robot structure
    robot_st->vacuum = true;
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
            run_keyboard();
            handle_keyboard(&(robot_st->display_st));
            robot_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
            break;

        case ENUM_STATE_UPDATE_DISPLAY:
            // Check if there are changes in display message
            update_display(&(robot_st->display_st));
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
            robot_st->state_en = ENUM_STATE_MOVE;
            break;

        case ENUM_STATE_MOVE:
            robot_st->state_en = ENUM_STATE_INIT;
            break;
        
        default:
            robot_st->state_en = ENUM_STATE_INIT;
            break;
    }
}

void handle_keyboard(st_display *display_st) {
    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
        if(read_keyboard(i)) {
            char bt = identify_button(i);

            handle_display(display_st, bt);
        }
    }
}

void handle_display(st_display *display_st, char bt) {
    static char password[LCD_PARAMETER_4_CHAR_FIRST_POS+2] = "";
    static char slt_hour[LCD_PARAMETER_4_CHAR_FIRST_POS+2] = "0900";
    static char slt_len[LCD_PARAMETER_2_CHAR_FIRST_POS+2]= "";
    static char slt_hgt[LCD_PARAMETER_2_CHAR_FIRST_POS+2] = "";
    static char slt_cnt[LCD_PARAMETER_2_CHAR_FIRST_POS+2] = "";
    static uint8_t index = LCD_PARAMETER_4_CHAR_FIRST_POS;

    switch (display_st->screen_en) {
        case ENUM_SCREEN_LOGIN:
            // Check if is digit
            if(isdigit(bt)) {
                password[index] = bt;
                index = index > 0 ? index-1 : LCD_PARAMETER_4_CHAR_FIRST_POS; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            } else if(bt == '#') {
                // Reset password
                strcpy(password, "");
                index = LCD_PARAMETER_4_CHAR_FIRST_POS;

                // Check password
                if(!strcmp(password, ROBOT_USER_PASSWORD_ADM)) {
                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_ADM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW1);
                    display_st->update = true;                            
                } else if(!strcmp(password, ROBOT_USER_PASSWORD_COM)) {
                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_COM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;  
                } else {                     
                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true; 
                }
            }
            break;

        case ENUM_SCREEN_MAIN_MENU_USER_ADM:
            switch (bt) {
                case '1':
                    

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0, password);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;
                    break;
                
                case '2':
                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;
                    break;

                case '3':
                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;
                    break;

                case '4':
                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;
                    break;
                default:
                    break;
            }
            break;

        case ENUM_SCREEN_SLCT_LEN_USER_ADM:
            if(isdigit(bt)) {

            } else if(bt == '#') {

            }                
            break;

        case ENUM_SCREEN_SLCT_HGT_USER_ADM:
            if(isdigit(bt)) {

            } else if(bt == '#') {

            }
            break;

        case ENUM_SCREEN_SLCT_CNT_USER_ADM:
            if(isdigit(bt)) {

            } else if(bt == '#') {

            }
            break;

        case ENUM_SCREEN_CHNG_HOUR_USER_ADM:
            if(isdigit(bt)) {

            } else if(bt == '#') {

            }
            break;

        case ENUM_SCREEN_MAIN_MENU_USER_COM:

            break;

        default:
            break;
    }
}


void update_display(st_display *display_st) {
    unsigned long time = millis();
    static unsigned long timer = 0;

    // Update display at 1 FPS rate
    if(time - time >= ROBOT_TIME_UPDATE_DISPLAY_MS) {
        timer = time;

        if(display_st->update == true) {
            display_st->update = false;
            display_clear();
            display_setCursor(0, 0);
            display_print(display_st->row0);
            display_setCursor(1, 0);
            display_print(display_st->row0);
        }
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

char identify_button(uint8_t bt_index) {
    char bt = ' ';
    switch (bt_index) {
        case ENUM_BUTTON_0:
            bt = '0';
            break;

        case ENUM_BUTTON_1:
            bt = '1';
            break;

        case ENUM_BUTTON_2:
            bt = '2';
            break;

        case ENUM_BUTTON_3:
            bt = '3';
            break;

        case ENUM_BUTTON_4:
            bt = '4';
            break;

        case ENUM_BUTTON_5:
            bt = '5';
            break;

        case ENUM_BUTTON_6:
            bt = '6';
            break;

        case ENUM_BUTTON_7:
            bt = '7';
            break;

        case ENUM_BUTTON_8:
            bt = '8';
            break;

        case ENUM_BUTTON_9:
            bt = '9';
            break;
        
        case ENUM_BUTTON_STAR:
            bt = '*';
            break;

        case ENUM_BUTTON_HASHTAG:
            bt = '#';
            break;

        default:
            bt = ' ';
            break;
    }

    return bt;
}