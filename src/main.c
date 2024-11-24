/* Includes */
#include "main.h"

/* Variables */

/* Main function */
int main(void) {
  /* Init setup */

  // Timer setup
  timer_init();

  // IO's setup
  init_pin(5, ENUM_PORT_PORTB, ENUM_PINMODE_OUTPUT);

  /* Main loop */
  while(true) {
    set_pin(5, ENUM_PORT_PORTB, !read_pin(5, ENUM_PORT_PORTB));
    delay(10);
  }
}