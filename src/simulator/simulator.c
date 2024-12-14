/* Includes */
#include "simulator.h"

/* Functions */
void init_environment(st_environment *environment_st) {
    // Init collected dust values    
    environment_st->dust_st.collected_dust = 0;
    environment_st->dust_st.valid = true;

    // Init temperature values
    environment_st->temperature_st.old_temperature = ENVIROMENT_INIT_TEMPERATURE;
    environment_st->temperature_st.temperature = ENVIROMENT_INIT_TEMPERATURE;
    environment_st->temperature_st.valid = true;

    // Init obstacle values
    environment_st->obstacle_st.obstacle = false;
    environment_st->obstacle_st.valid = true;

    // Init clock time
    environment_st->clock_st.time_st.hour = 0;
    environment_st->clock_st.time_st.minute = 0;
    environment_st->clock_st.valid = false;
}

void simulator_handle_receive(st_environment *environment_st) {
    // Check if there are new bytes available
    static char buffer[8] = {0};
    static int i = 0;
    char buffer_answer[8];

    if(serial_available()) {
        buffer[i] = serial_read();
        i++;
    }

    if(buffer[0] == 'S') {
        if (buffer[1] == PROTOCOL_COMMAND_VACCUUM_TURN_ON && i >= 2) {
            // Update vacuum status
            environment_st->vaccuum_st.state = true;
            environment_st->vaccuum_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_VACCUUM_TURN_OFF && i >= 2) {
            // Update vacuum status
            environment_st->vaccuum_st.state = false;
            environment_st->vaccuum_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_MOVEMENT && i >= 2) {
            // Confirm movement was executed
            environment_st->move_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_MOVEMENT_FAIL && i >= 2) {
            // Don't update the location of the robot

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COOMAND_MOP_TURN_ON && i >= 2) {
            // Update mop status
            environment_st->mop_st.state = true;
            environment_st->mop_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COOMAND_MOP_TURN_OFF && i >= 2) {
            // Update mop status
            environment_st->mop_st.state = false;
            environment_st->mop_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_TEMPERATURE && i >= 2) {
            // Update environment temperature
            environment_st->temperature_st.old_temperature = environment_st->temperature_st.temperature;
            environment_st->temperature_st.temperature = buffer[2];
            environment_st->temperature_st.valid = true;

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_INFO && i >= 4 ) {
            // Update environment info
            environment_st->obstacle_st.obstacle = buffer[2];
            environment_st->dust_st.collected_dust = buffer[3];
            environment_st->obstacle_st.valid = true;
            environment_st->dust_st.valid = true;
            // Send answer
            sprintf(buffer_answer, PROTOCOL_ROBOT_ANSWER_2BYTES, PROTOCOL_SENDER_ROBOT, PROTOCOL_COMMAND_INFO);
            serial_print(buffer_answer);
            
            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        } else if (buffer[1] == PROTOCOL_COMMAND_CLOCK && i >= 4) {
            // Update environment clock
            environment_st->clock_st.time_st.hour = buffer[2];
            environment_st->clock_st.time_st.minute = buffer[3];
            environment_st->clock_st.valid = true;

            // Send answer
            sprintf(buffer_answer, PROTOCOL_ROBOT_ANSWER_2BYTES, PROTOCOL_SENDER_ROBOT, PROTOCOL_COMMAND_CLOCK);
            serial_print(buffer_answer);

            for(int j = 0; j < 8; j++) {
                buffer[j] = 0;
            }
            i = 0;
        }
    }    
}

void simulator_move(st_environment *environment_st, en_move move_en) {
    environment_st->move_st.move_en = move_en;
    char buffer[SIMULATOR_BUFFER_LENGTH];
    sprintf(buffer, PROTOCOL_ROBOT_ANSWER_3BYTES, PROTOCOL_SENDER_ROBOT, PROTOCOL_COMMAND_MOVEMENT, (char)move_en);
    serial_print(buffer);
}

void simulator_vaccuum(st_environment *environment_st, bool state) {
    environment_st->vaccuum_st.state = state;

    char buffer[SIMULATOR_BUFFER_LENGTH];
    sprintf(buffer, PROTOCOL_ROBOT_ANSWER_2BYTES, PROTOCOL_SENDER_ROBOT, state ? PROTOCOL_COMMAND_VACCUUM_TURN_ON : PROTOCOL_COMMAND_VACCUUM_TURN_OFF);
    serial_print(buffer);
}