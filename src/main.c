/* Includes */
#include "main.h"

/* Main function */
int main(void) {
  /* Init setup */

  // Timer setup
  timer_init();

  // Serial 0 begin
  serial_begin();

  // Keyboard setup
  init_keyboard();


  serial_print("Hello world!\n", strlen("Hello world!\n"));

  /* Main loop */
  while(true) {
    // Continuous read keyboard
    handle_keyboard();

    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
      if(read_keyboard(i)) {
        char buffer[24];

        sprintf(buffer, "Botao pressionado: %d\n", i+1);

        serial_print(buffer, strlen(buffer)); 
      }
    }

    if(serial_available()) {
      uint8_t data = serial_read();

      if(data == 'c') {
        set_pin(5, ENUM_PORT_PORTB, !read_pin(5, ENUM_PORT_PORTB));
      }
    }
  }
}