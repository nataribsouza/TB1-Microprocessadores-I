/* Includes */
#include "main.h"

/* Variables */

/* Main function */
int main(void) {
  /* Init setup */

  // Timer setup
  timer_init();

  // Serial 0 begin
  serial_begin();

  // IO's setup
  init_pin(5, ENUM_PORT_PORTB, ENUM_PINMODE_OUTPUT);

  /* Main loop */
  while(true) {
    if(serial_available()) {
      uint8_t data = serial_read();

      if(data == 'c') {
        set_pin(5, ENUM_PORT_PORTB, !read_pin(5, ENUM_PORT_PORTB));
      }
    }
  }
}