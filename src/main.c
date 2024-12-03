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

  // I2C setup
  init_i2c();

  // Init LCD display
  init_display();

  display_setCursor(0, 0);    // Cursor na linha 0, coluna 0
  display_print("Hello, Natan!");  // Exibe mensagem
  display_setCursor(1, 0);    // Cursor na linha 1, coluna 0
  display_print("I2C LCD Demo!");

  /* Main loop */
  while(true) {
    // Continuous read keyboard
    handle_keyboard();

    for(int i = 0; i < KEYBOARD_NUM_BUTTONS; i++) {
      if(read_keyboard(i)) {
        char buffer[24];

        sprintf(buffer, "Botao pressionado: %d\n", i+1);

        serial_print(buffer); 
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