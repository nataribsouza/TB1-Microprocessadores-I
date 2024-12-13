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
    if(serial_available()) {
        // Local variables
        char buffer_answer[SIMULATOR_BUFFER_LENGTH];
        uint8_t i  = 0;

        // Check if the sender is the simulator 'S'
        if(serial_read() == PROTOCOL_SENDER_SIMULATOR) {
            // Handle each command
            switch (serial_read()) {
                case PROTOCOL_COMMAND_VACCUUM_TURN_ON:
                    // Update vaccuum status
                    environment_st->vaccuum_st.state = true;
                    environment_st->vaccuum_st.valid = true;                    
                    break;

                case PROTOCOL_COMMAND_VACCUUM_TURN_OFF:
                    // Update vaccuum status
                    environment_st->vaccuum_st.state = false;
                    environment_st->vaccuum_st.valid = true;
                    break;

                case PROTOCOL_COMMAND_MOVEMENT:
                    // Confirm movement was executed
                    environment_st->move_st.valid = true;
                    break;

                case PROTOCOL_COMMAND_MOVEMENT_FAIL:
                    // Don't update the location of the robot
                    break;

                case PROTOCOL_COOMAND_MOP_TURN_ON:
                    // Update mop status
                    environment_st->mop_st.state = true;
                    environment_st->mop_st.valid = true;
                    break;

                case PROTOCOL_COOMAND_MOP_TURN_OFF:
                    // Update mop status
                    environment_st->mop_st.state = false;
                    environment_st->mop_st.valid = true;
                    break;

                case PROTOCOL_COMMAND_TEMPERATURE:
                    // Update enviroment temperature
                    environment_st->temperature_st.old_temperature = environment_st->temperature_st.temperature;
                    environment_st->temperature_st.temperature = serial_read();
                    environment_st->temperature_st.valid = true;
                    break;

                case PROTOCOL_COMMAND_INFO:
                    // Update environment info
                    environment_st->obstacle_st.obstacle = serial_read();
                    environment_st->dust_st.collected_dust = serial_read();;
                    environment_st->obstacle_st.valid = true;
                    environment_st->dust_st.valid = true;
                    
                    // Send answer
                    sprintf(buffer_answer, PROTOCOL_ROBOT_ANSWER_2BYTES, PROTOCOL_SENDER_ROBOT, PROTOCOL_COMMAND_INFO);
                    serial_print(buffer_answer);
                    break;

                case PROTOCOL_COMMAND_CLOCK:
                    // Update enviroment clock
                    environment_st->clock_st.time_st.hour = serial_read();
                    environment_st->clock_st.time_st.minute = serial_read();
                    environment_st->clock_st.valid = true;

                    // Send answer
                    sprintf(buffer_answer, PROTOCOL_ROBOT_ANSWER_2BYTES, PROTOCOL_SENDER_ROBOT, PROTOCOL_COMMAND_CLOCK);
                    serial_print(buffer_answer);
                    break;
                
                default:
                    break;
            }
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