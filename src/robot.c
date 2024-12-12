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
    sprintf(robot_st->display_st.row1, LCD_SCREEN_LOGIN_R0W1);

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
            run_keyboard();
            handle_keyboard(robot_st);
            robot_st->state_en = ENUM_STATE_UPDATE_DISPLAY;
            break;

        case ENUM_STATE_UPDATE_DISPLAY:
            // Check if there are changes in display message
            update_display(robot_st);
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
            robot_st->state_en = ENUM_STATE_READ_KEYBOARD;
            break;
        
        default:
            // Reboot the system if unknown state
            reboot();
            break;
    }
}

void handle_keyboard(st_robot *robot_st) {
    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
        if(read_keyboard(i)) {
            // Identify wich button was pressed 
            char bt = identify_button(i);

            serial_write(bt);

            // Handle display behavior
            handle_display(robot_st, bt);
        }
    }
}

void handle_display(st_robot *robot_st, char bt) {
    st_display *display_st = &(robot_st->display_st);
    
    static char password[LAST_POS_4CHAR_STR+2] = "    ";
    static char slt_hour[LAST_POS_2CHAR_STR+2] = "  ";
    static char slt_min[LAST_POS_2CHAR_STR+2] = "  ";
    static char slt_len[LAST_POS_2CHAR_STR+2] = "  ";
    static char slt_hgt[LAST_POS_2CHAR_STR+2] = "  ";
    static char slt_cnt[LAST_POS_3CHAR_STR+2] = "   ";
    static char time_unit = 'm';
    static uint8_t slt_area = 0;
    static uint8_t index = 0;

    switch (display_st->screen_en) {
// =============== SCREEN LOGIN ===============
        case ENUM_SCREEN_LOGIN:
            // Check if is digit
            if(isdigit(bt)) {
                password[index] = bt;
                index = index >= LAST_POS_4CHAR_STR ? 0 : index+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            } else if(bt == '#') {
                // Check password
                if(!strcmp(password, ROBOT_USER_PASSWORD_ADM)) {
                    // Reset password
                    strcpy(password, "    ");
                    index = 0;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_ADM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW1);
                    display_st->update = true;                            
                } else if(!strcmp(password, ROBOT_USER_PASSWORD_COM)) {
                    // Reset password
                    strcpy(password, "    ");
                    index = 0;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_COM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;  
                } else {            
                    // Reset password
                    strcpy(password, "    ");
                    index = 0;
                    
                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true; 
                }
            }
            break;

// =============== MAIN MENU USER ADM ===============
        case ENUM_SCREEN_MAIN_MENU_USER_ADM:
            switch (bt) {
                case '1':
                    // Start normal cleaning routine
                    

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;
                
                case '2':
                    // Start edge cleaning routine

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;

                case '3':
                    index = 0;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW1);
                    display_st->update = true;
                    break;

                case '4':
                    // Reset string index
                    index = 0;
                    time_unit = 'm';

                    // Set hour and minute
                    sprintf(slt_hour, "%02d", robot_st->wakeup_time.hour);
                    sprintf(slt_min, "%02d", robot_st->wakeup_time.minute);

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_CHNG_HOUR_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW1, slt_hour, slt_min);
                    display_st->update = true;
                    break;

                default:
                    break;
            }
            break;

// =============== SELECT BLOCKED AREA ===============
        case ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM: 
            switch (bt) {
                case '1':
                    slt_area = 0;

                    itoa(robot_st->blocked_area_st[slt_area].x, slt_len, DEC_BASE);
                    index = 0;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_SLCT_LEN_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, slt_len);
                    display_st->update = true;
                    break;
                
                case '2':
                    slt_area = 1;

                    itoa(robot_st->blocked_area_st[slt_area].x, slt_len, DEC_BASE);
                    index = 0;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_SLCT_LEN_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, slt_len);
                    display_st->update = true;
                    break;

                default:
                    break;
            }
            break;

// =============== SELECT LENGTH OF BLOCKED AREA ===============
        case ENUM_SCREEN_SLCT_LEN_USER_ADM:
            if(isdigit(bt)) {
                slt_len[index] = bt;
                index = index >= LAST_POS_2CHAR_STR ? 0 : index+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, slt_len);
                display_st->update = true;
            } else if(bt == '#') {
                // New blocked area length
                robot_st->blocked_area_st[slt_area].x = atoi(slt_len);

                itoa(robot_st->blocked_area_st[slt_area].y, slt_hgt, DEC_BASE);
                index = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_SLCT_HGT_USER_ADM;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW1, slt_hgt);
                display_st->update = true;
            }
            break;

// =============== SELECT HEIGHT OF BLOCKED AREA ===============
        case ENUM_SCREEN_SLCT_HGT_USER_ADM:
            if(isdigit(bt)) {
                slt_hgt[index] = bt;
                index = index >= LAST_POS_2CHAR_STR ? 0 : index+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW1, slt_hgt);
                display_st->update = true;
            } else if(bt == '#') {
                // New blocked area height
                robot_st->blocked_area_st[slt_area].y = atoi(slt_hgt);

                itoa(robot_st->blocked_area_st[slt_area].center, slt_cnt, DEC_BASE);
                index = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_SLCT_CNT_USER_ADM;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_CNT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_CNT_USER_ADM_ROW1, slt_cnt);
                display_st->update = true;
            }
            break;

// =============== SELECT CENTER OF BLOCKED AREA ===============
        case ENUM_SCREEN_SLCT_CNT_USER_ADM:
            if(isdigit(bt)) {
                slt_cnt[index] = bt;
                index = index >= LAST_POS_3CHAR_STR ? 0 : index+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_CNT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_CNT_USER_ADM_ROW1, slt_cnt);
                display_st->update = true;
            } else if(bt == '#') {
                // New blocked area center
                robot_st->blocked_area_st[slt_area].center = atoi(slt_cnt);

                index = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_LOGIN;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            }
            break;

// =============== SELECT AUTOMATIC WAKEUP TIME ===============
        case ENUM_SCREEN_CHNG_HOUR_USER_ADM:
            if(isdigit(bt)) {
                if(time_unit == 'm') {
                    slt_hour[index] = bt;
                } else if(time_unit == 'h') {
                    slt_min[index] = bt;
                }

                if(index < LAST_POS_2CHAR_STR) {
                    index++;
                } else if(time_unit == 'm') {
                    index = 0;
                    time_unit = 'h';
                } else if(time_unit == 'h') {
                    index = 0;
                    time_unit = 'm';
                }

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW1, slt_hour, slt_min);
                display_st->update = true;
            } else if(bt == '#') {
                // New wakeup time
                robot_st->wakeup_time.hour = atoi(slt_hour);
                robot_st->wakeup_time.minute = atoi(slt_min);

                index = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_LOGIN;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            }
            break;

// =============== MAIN MENU USER COMMUM ===============
        case ENUM_SCREEN_MAIN_MENU_USER_COM:
            switch (bt) {
                case '1':
                    // Start normal cleaning routine
                    

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;
                
                case '2':
                    // Start edge cleaning routine

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, password);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}


void update_display(st_robot *robot_st) {
    unsigned long time = millis();
    static unsigned long timer = 0;
    st_display *display_st = &(robot_st->display_st);

    // Update display at 1 FPS rate
    if(time - timer >= ROBOT_TIME_UPDATE_DISPLAY_MS) {
        timer = time;

        if(display_st->update) {
            robot_st->display_st.update = false;

            display_clear();
            display_setCursor(0, 0);
            display_print(display_st->row0);
            display_setCursor(1, 0);
            display_print(display_st->row1);
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

void reboot(void) {
  // Disable interruptions
  cli();
  
  // Set watchdog for 15 ms
  wdt_enable(WDTO_15MS);
  
  // Wait untill watchdog reboot the system
  while (1) {}
}