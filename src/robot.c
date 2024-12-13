/* Includes */
#include "robot.h"

/* Functions */

/**
 * @brief Init robot variables
 * 
 * @param robot_st 
 */
void init_robot(st_robot *robot_st) {
    // Initialize led pin
    init_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, ENUM_PINMODE_OUTPUT);
    set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC,false);

    // Set display structure
    robot_st->display_st.update = true;
    sprintf(robot_st->display_st.row0, LCD_SCREEN_LOGIN_R0W0, "");
    sprintf(robot_st->display_st.row1, LCD_SCREEN_LOGIN_R0W1);

    // Set robot structure
    robot_st->mop = false;
    robot_st->vacuum = false;
    robot_st->docked = true;
    robot_st->dust = 0;
    robot_st->battery = ROBOT_BATTERY_CAPACITY_MA_S;
    robot_st->orientation_en = ENUM_ORIENTATION_NORTH;
    robot_st->position_st.x = ROBOT_INITIAL_POSITION_X;
    robot_st->position_st.y = ROBOT_INITIAL_POSITION_Y;
    robot_st->time.hour = ROBOT_DFL_WAKEUP_TIME_H;
    robot_st->time.minute = ROBOT_DFL_WAKEUP_TIME_MIN;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_0].length = 0;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_0].height = 0;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_1].length = 0;
    robot_st->blocked_area_st[ROBOT_BLOCKED_ARAE_1].height = 0;
    robot_st->state_machine_en = ENUM_STATE_INIT;
}

/**
 * @brief Main robot state machine
 * 
 * @param robot_st 
 */
void state_machine(st_robot *robot_st, st_environment *environment_st) {
    switch (robot_st->state_machine_en) {
        case ENUM_STATE_INIT:
            init_robot(robot_st);
            init_environment(environment_st);
            robot_st->state_machine_en = ENUM_STATE_READ_KEYBOARD;
            break;

        case ENUM_STATE_READ_KEYBOARD:
            run_keyboard();
            handle_keyboard(robot_st);
            robot_st->state_machine_en = ENUM_STATE_UPDATE_DISPLAY;
            break;

        case ENUM_STATE_UPDATE_DISPLAY:
            // Check if there are changes in display message
            update_display(robot_st);
            robot_st->state_machine_en = ENUM_STATE_BLINK_LED;
            break;

        case ENUM_STATE_BLINK_LED:
            // Blink if battery is bellow 10%
            if(calculate_dust_level(robot_st->dust) >= ROBOT_DUST_MAX_PERCENTAGE) {
                blink_led();                
            } else {
                set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC,false);
            }

            robot_st->state_machine_en = ENUM_STATE_CHECK_SERIAL;
            break;

        case ENUM_STATE_CHECK_SERIAL:
            simulator_handle_receive(environment_st);
            robot_st->state_machine_en = ENUM_STATE_MOVE;
            break;

        case ENUM_STATE_MOVE:
            handle_movement(robot_st, environment_st);
            robot_st->state_machine_en = ENUM_STATE_READ_KEYBOARD;
            break;
        
        default:
            // Reboot the system if unknown state
            reboot();
            break;
    }
}

/**
 * @brief Read all buttons and 
 * manage actions according with it
 * 
 * @param robot_st 
 */
void handle_keyboard(st_robot *robot_st) {
    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
        if(read_keyboard(i)) {
            // Identify wich button was pressed 
            char bt = identify_button(i);

            // Handle display behavior
            handle_display(robot_st, bt);
        }
    }
}

/**
 * @brief Display response to buttons press
 * 
 * @param robot_st 
 * @param bt 
 */
void handle_display(st_robot *robot_st, char bt) {
    st_display *display_st = &(robot_st->display_st);

    // Local variables for display management
    static char lpassword[LAST_POS_4CHAR_STR+2] = "    ";
    static char lhour[LAST_POS_2CHAR_STR+2] = "  ";
    static char lminute[LAST_POS_2CHAR_STR+2] = "  ";
    static char llength[LAST_POS_2CHAR_STR+2] = "  ";
    static char lheight[LAST_POS_2CHAR_STR+2] = "  ";
    static char lx[LAST_POS_2CHAR_STR+2] = "  ";
    static char ly[LAST_POS_2CHAR_STR+2] = "  ";
    static char lunit = 'h'; // [h] Hour  | [m] minute
    static uint8_t larea_index = 0;
    static uint8_t lindex = 0;

    // Manage actions based on screen state
    switch (display_st->screen_en) {
        // Screen login
        case ENUM_SCREEN_LOGIN:
            // Check if pressed button is digit
            if(isdigit(bt)) {
                lpassword[lindex] = bt;

                // Update string index
                lindex = lindex >= LAST_POS_4CHAR_STR ? 0 : lindex+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;

            // Check is # was pressed
            } else if(bt == '#') {
                // Verify if actual password is ADM USER password
                if(!strcmp(lpassword, ROBOT_USER_PASSWORD_ADM)) {
                    // Reset password
                    strcpy(lpassword, "    ");
                    // Reset string index
                    lindex = 0;

                    // Change to ADM USER main screen
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_ADM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_ADM_ROW1);
                    display_st->update = true;       

                } else if(!strcmp(lpassword, ROBOT_USER_PASSWORD_COM)) {
                    // Verify if actual password is COMMUM USER password
                    strcpy(lpassword, "    ");
                    lindex = 0;

                    // Change to COMMUM USER main screen
                    display_st->screen_en = ENUM_SCREEN_MAIN_MENU_USER_COM;

                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_MAIN_MENU_USER_COM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_MAIN_MENU_USER_COM_ROW1);
                    display_st->update = true;  

                // Invalid password
                } else {           
                    // Reset password
                    strcpy(lpassword, "    ");
                    lindex = 0;
                    
                    // Update display
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true; 
                }
            }
            break;

        // Screen main menu ADM USER
        case ENUM_SCREEN_MAIN_MENU_USER_ADM:
            switch (bt) {
                // Option [1]: Start normal cleaning routine
                case '1':
                    // Call normal cleaning function
                    robot_st->cleaning_mode_en = ENUM_STATE_CLEAN_NORMAL;

                    // Change back to login screen
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;

                // Option [2]: Start edge cleaning routine
                case '2':
                    // Call edge cleaning function
                    robot_st->cleaning_mode_en = ENUM_STATE_CLEAN_EDGES;

                    // Change back to login screen
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;

                // Option [3]: Set new blocked area
                case '3':
                    // Change to Select Blocked Area screen
                    display_st->screen_en = ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_BLOCK_AREA_USER_ADM_ROW1);
                    display_st->update = true;
                    break;

                // Option [4]: Set new automatic cleaning hour
                case '4':
                    // Start setting the hour
                    lunit = 'h';

                    // Get actual automatic cleaning hour
                    sprintf(lhour, "%02d", robot_st->time.hour);
                    sprintf(lminute, "%02d", robot_st->time.minute);

                    // Change to Setting hour screen
                    display_st->screen_en = ENUM_SCREEN_CHNG_HOUR_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW1, lhour, lminute);
                    display_st->update = true;
                    break;

                default:
                    break;
            }
            break;

        // Screen Select Blocked Area
        case ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM: 
            switch (bt) {
                case '1':
                    // Select blocked area 0
                    larea_index = 0;

                    // Get actual blocked area length
                    itoa(robot_st->blocked_area_st[larea_index].length, llength, DEC_BASE);

                    // Change to Select Blocked Area Length screen
                    display_st->screen_en = ENUM_SCREEN_SLCT_LEN_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, llength);
                    display_st->update = true;
                    break;
                
                case '2':
                    // Select blocked area 1
                    larea_index = 1;
                    
                    // Get actual blocked area length
                    itoa(robot_st->blocked_area_st[larea_index].length, llength, DEC_BASE);

                    // Change to Select Blocked Area Length screen
                    display_st->screen_en = ENUM_SCREEN_SLCT_LEN_USER_ADM;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                    sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, llength);
                    display_st->update = true;
                    break;

                default:
                    break;
            }
            break;

        // Screen Select Blocked Area Length
        case ENUM_SCREEN_SLCT_LEN_USER_ADM:
            if(isdigit(bt)) {
                // Update blocked area length variable
                llength[lindex] = bt;
                // Update string index
                lindex = lindex >= LAST_POS_2CHAR_STR ? 0 : lindex+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_LEN_USER_ADM_ROW1, llength);
                display_st->update = true;
            } else if(bt == '#') {          
                // Get actual blocked area height
                itoa(robot_st->blocked_area_st[larea_index].height, lheight, DEC_BASE);

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_SLCT_HGT_USER_ADM;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW1, lheight);
                display_st->update = true;
            }
            break;

        // Screen Select Blocked Area Height
        case ENUM_SCREEN_SLCT_HGT_USER_ADM:
            if(isdigit(bt)) {
                // Update blocked area height variable
                lheight[lindex] = bt;
                // Update string index
                lindex = lindex >= LAST_POS_2CHAR_STR ? 0 : lindex+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_HGT_USER_ADM_ROW1, lheight);
                display_st->update = true;
            } else if(bt == '#') {
                // Get actual blocked area X position
                itoa(robot_st->blocked_area_st[larea_index].position_st.x, lx, DEC_BASE);

                // Change to Select Blocked Area Center screen
                display_st->screen_en = ENUM_SCREEN_SLCT_POS_X_USER_ADM;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_POS_X_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_POS_X_USER_ADM_ROW1, lx);
                display_st->update = true;
            }
            break;

        // Screen Select Blocked Area Position X
        case ENUM_SCREEN_SLCT_POS_X_USER_ADM:
            if(isdigit(bt)) {
                // Update blocked area height variable
                lx[lindex] = bt;
                // Update string index
                lindex = lindex >= LAST_POS_2CHAR_STR ? 0 : lindex+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_POS_X_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_POS_X_USER_ADM_ROW1, lx);
                display_st->update = true;
            } else if(bt == '#') {
                // Reset local index
                lindex = 0;

                // Get actual blocked area Y position
                itoa(robot_st->blocked_area_st[larea_index].position_st.y, ly, DEC_BASE);

                // Change to Login screen
                display_st->screen_en = ENUM_SCREEN_SLCT_POS_Y_USER_ADM;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_POS_Y_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_POS_Y_USER_ADM_ROW1, ly);
                display_st->update = true;
            }
            break;

        // Screen Select Blocked Area Position X
        case ENUM_SCREEN_SLCT_POS_Y_USER_ADM:
            if(isdigit(bt)) {
                // Update blocked area height variable
                ly[lindex] = bt;
                // Update string index
                lindex = lindex >= LAST_POS_2CHAR_STR ? 0 : lindex+1; 

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_SLCT_POS_Y_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_SLCT_POS_Y_USER_ADM_ROW1, ly);
                display_st->update = true;
            } else if(bt == '#') {
                // New blocked area center
                uint8_t length = atoi(llength);
                uint8_t height = atoi(lheight);
                uint8_t x = atoi(lx);
                uint8_t y = atoi(ly);
                if(length <= ROOM_LENGTH && height <= ROOM_HEIGHT
                    && x < (ROOM_LENGTH - robot_st->blocked_area_st[larea_index].length)
                    && y < (ROOM_HEIGHT - robot_st->blocked_area_st[larea_index].height)) {

                    robot_st->blocked_area_st[larea_index].length = length;
                    robot_st->blocked_area_st[larea_index].height = height;
                    robot_st->blocked_area_st[larea_index].position_st.x = x;
                    robot_st->blocked_area_st[larea_index].position_st.y = y;
                }    

                // Reset local index
                lindex = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_LOGIN;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            }
            break;

        // Screen set Automatic Clening Hour
        case ENUM_SCREEN_CHNG_HOUR_USER_ADM:
            if(isdigit(bt)) {
                // Update hour and minute local variables
                if(lunit == 'h') {
                    lhour[lindex] = bt;
                } else if(lunit == 'm') {
                    lminute[lindex] = bt;
                }

                // Update setting algarims
                if(lindex < LAST_POS_2CHAR_STR) {
                    lindex++;
                } else if(lunit == 'm') {
                    lindex = 0;
                    lunit = 'h';
                } else if(lunit == 'h') {
                    lindex = 0;
                    lunit = 'm';
                }

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW0);
                sprintf(display_st->row1, LCD_SCREEN_CHNG_HOUR_USER_ADM_ROW1, lhour, lminute);
                display_st->update = true;
            } else if(bt == '#') {
                // Set new actomatic clening time
                uint8_t hour = atoi(lhour);
                uint8_t minute = atoi(lminute);
                if(hour <= MAX_HOUR && minute <= MAX_MINUTE) {
                    robot_st->time.hour = atoi(lhour);
                    robot_st->time.minute = atoi(lminute);
                }              

                // Reset local index
                lindex = 0;

                // Change screen state
                display_st->screen_en = ENUM_SCREEN_LOGIN;

                // Update display
                sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                display_st->update = true;
            }
            break;

        // Screen COMMUM USER Main Menu
        case ENUM_SCREEN_MAIN_MENU_USER_COM:
            switch (bt) {
                // Option [1]: Start normal cleaning routine
                case '1':
                    // Call normal cleaning function
                    robot_st->cleaning_mode_en = ENUM_STATE_CLEAN_NORMAL;

                    // Change back to login screen
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
                    sprintf(display_st->row1, LCD_SCREEN_LOGIN_R0W1);
                    display_st->update = true;
                    break;
                
                // Option [2] : Start edge cleaning routine
                case '2':
                    // Call edge cleaning function
                    robot_st->cleaning_mode_en = ENUM_STATE_CLEAN_EDGES;

                    // Change screen state
                    display_st->screen_en = ENUM_SCREEN_LOGIN;

                    // Update screen
                    sprintf(display_st->row0, LCD_SCREEN_LOGIN_R0W0, lpassword);
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

/**
 * @brief Update display at constant rate
 * if there are some modification
 * 
 * @param robot_st 
 */
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

void handle_movement(st_robot *robot_st, st_environment *environment_st) {
    uint32_t time = millis();
    static uint32_t timer = 0;

    update_environment_parameters(robot_st, environment_st);

    if(time - timer >= ROBOT_TIME_MOVEMENT_MS) {
        timer = time;
        uint8_t posx = robot_st->position_st.x;
        uint8_t posy = robot_st->position_st.y;
        en_orient orientation = robot_st->orientation_en;
        st_blocked_area area0 = robot_st->blocked_area_st[0];
        st_blocked_area area1 = robot_st->blocked_area_st[1];

        recalculate_battery(robot_st);
        uint16_t battery_percentage = (robot_st->battery / ROBOT_BATTERY_CAPACITY_MA_S) * CONVERT_TO_PERCENTAGE;

        // Go base if battery less than 10%
        if(battery_percentage < 10) {
            //robot_st->cleaning_mode_en = ENUM_STATE_NO_CLEANING;
        }

        switch (robot_st->cleaning_mode_en) {
            case ENUM_STATE_NO_CLEANING:
                if (robot_st->docked) {
                    // Charge battery and clean storage
                    robot_st->dust = 0;
                    robot_st->battery = ROBOT_BATTERY_CAPACITY_MA_S;
                    robot_st->orientation_en = ENUM_ORIENTATION_NORTH;
                    break;
                }
                
                // Got to collum 9
                if (robot_st->position_st.x < ROOM_MAX_X_POSITION) {                
                    if (robot_st->orientation_en != ENUM_ORIENTATION_WEST) {
                        simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                    } else if (robot_st->obstacle_ahead || !check_move_forward(robot_st)) {
                        simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                    } else {
                        simulator_move(environment_st,ENUM_MOVE_FORWARD);
                    }
                // Go to line 0
                } else if (robot_st->position_st.y > ROM_MIN_Y_POS) {
                    if (robot_st->orientation_en != ENUM_ORIENTATION_SOUTH) {
                        simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                    } else if (robot_st->obstacle_ahead || !check_move_forward(robot_st)) {
                        simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                    } else {
                        simulator_move(environment_st, ENUM_MOVE_FORWARD);
                    }
                } else {
                    simulator_move(environment_st, ENUM_MOVE_FORWARD);
                }
                break;

            case ENUM_STATE_CLEAN_NORMAL:

               // Estratégia simples: movimento em zig-zag
                if (robot_st->obstacle_ahead || check_block_area(posx, posy, area0) || check_block_area(posx, posy, area1)) {
                    simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                } else {
                    simulator_move(environment_st, ENUM_MOVE_FORWARD);
                }
                break;

            case ENUM_STATE_CLEAN_EDGES:
                // Limpeza apenas no perímetro da sala
                if (robot_st->obstacle_ahead || check_block_area(posx, posy, area0) || check_block_area(posx, posy, area1)) {
                    simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                } else if (posx == 0 && posy > 0 && orientation != ENUM_ORIENTATION_NORTH) {
                    simulator_move(environment_st, ENUM_MOVE_TURN_LEFT);
                } else if (posx == ROOM_MAX_X_POSITION && posy > ROM_MIN_Y_POS && orientation != ENUM_ORIENTATION_SOUTH) {
                    simulator_move(environment_st, ENUM_MOVE_TURN_RIGHT);
                } else {
                    simulator_move(environment_st, ENUM_MOVE_FORWARD);
                }
                break;

            default:
                robot_st->cleaning_mode_en = ENUM_STATE_NO_CLEANING;
                break;
        }
    }
}

void update_environment_parameters(st_robot *robot_st, st_environment *environment_st) {
    if(environment_st->move_st.valid) {
        environment_st->move_st.valid = false;
        switch (environment_st->move_st.move_en) {
            case ENUM_MOVE_FORWARD:
                switch (robot_st->orientation_en) {
                    case ENUM_ORIENTATION_NORTH: (robot_st->position_st.y)++; break;
                    case ENUM_ORIENTATION_EST: (robot_st->position_st.x)++; break;
                    case ENUM_ORIENTATION_SOUTH: (robot_st->position_st.y)--; break;
                    case ENUM_ORIENTATION_WEST: (robot_st->position_st.x)--; break;
                }
                break;

            case ENUM_MOVE_TURN_LEFT:
                robot_st->orientation_en = (robot_st->orientation_en == ENUM_ORIENTATION_EST) ? ENUM_ORIENTATION_NORTH : (robot_st->orientation_en + 1);
                break;

            case ENUM_MOVE_TURN_RIGHT:
                robot_st->orientation_en = (robot_st->orientation_en == ENUM_ORIENTATION_NORTH) ? ENUM_ORIENTATION_EST : (robot_st->orientation_en - 1);
                break;

            default:
                break;
        }
    }

    if(environment_st->vaccuum_st.valid) {
        environment_st->vaccuum_st.valid = false;
        robot_st->vacuum = environment_st->vaccuum_st.state;
    }

    if(environment_st->dust_st.valid) {
        environment_st->dust_st.valid = false;
        robot_st->dust += environment_st->dust_st.collected_dust;
    }

    if(environment_st->obstacle_st.valid) {
        environment_st->obstacle_st.valid = false;
        robot_st->obstacle_ahead = environment_st->obstacle_st.obstacle;
    }

    if(environment_st->clock_st.valid) {
        environment_st->clock_st.valid = false;
        uint8_t hour = environment_st->clock_st.time_st.hour;
        uint8_t minute = environment_st->clock_st.time_st.minute;

        if(robot_st->time.hour == hour && robot_st->time.minute == minute) {
            robot_st->cleaning_mode_en = ENUM_STATE_CLEAN_NORMAL;
        }
    }
}

bool check_move_forward(st_robot *robot_st) {
    uint8_t posx = robot_st->position_st.x;
    uint8_t posy = robot_st->position_st.y;
    st_blocked_area area0_st = robot_st->blocked_area_st[0];
    st_blocked_area area1_st = robot_st->blocked_area_st[1];

    switch (robot_st->orientation_en) {
        case ENUM_ORIENTATION_NORTH: 
            return !check_block_area(posx, posy - 1, area0_st) && !check_block_area(posx, posy - 1, area1_st);

        case ENUM_ORIENTATION_EST: 
            return !check_block_area(posx + 1, posy, area0_st) && !check_block_area(posx + 1, posy, area1_st);

        case ENUM_ORIENTATION_SOUTH:
            return !check_block_area(posx, posy + 1, area0_st) && !check_block_area(posx, posy + 1, area1_st);

        case ENUM_ORIENTATION_WEST:
            return !check_block_area(posx - 1, posy, area0_st) && !check_block_area(posx - 1, posy, area1_st);
    }
    return false;
}

bool check_block_area(uint8_t x, uint8_t y, st_blocked_area area) {
    if (area.length == 0 || area.height == 0) {
        return false;
    }

    return (x >= area.position_st.x && x < area.position_st.x + area.length &&
            y >= area.position_st.y && y < area.position_st.y + area.height);
}

/**
 * @brief Blink LED
 * 
 */
void blink_led(void) {
  static uint32_t timer = 0;

  if(millis() - timer >= ROBOT_TIME_BLINK_LED_MS) {
    timer = millis();
    bool state = read_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC);
    set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, !state);
  }
}

/**
 * @brief Identify wich button was pressed
 * based on the en_button enumeration
 * 
 * @param bt_index 
 * @return char 
 */
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

void recalculate_battery(st_robot *robot_st) {
    uint16_t battery_consumption = ROBOT_BATTERY_CONSUMPTION_STD_MA_S;
    uint32_t time = millis();
    static uint32_t timer = 0;

    uint8_t passed_seconds = (time - timer) / CONVERT_MS_TO_S;

    if(robot_st->vacuum) {
        battery_consumption += ROBOT_BATTERY_CONSUMPTION_VACUUM_MA_S;
    }

    if(robot_st->mop) {
        battery_consumption += ROBOT_BATTERY_CONSUMPTION_MOP_MA_S;
    }

    robot_st->battery -= passed_seconds * battery_consumption;
}

/**
 * @brief Calculate the battery percentage level
 * 
 * @param battery 
 * @return uint8_t 
 */
uint8_t calculate_dust_level(uint8_t dust) {
    uint8_t percent = (dust / ROBOT_DUST_CAPACITY_G) * CONVERT_TO_PERCENTAGE;
    return percent;
}