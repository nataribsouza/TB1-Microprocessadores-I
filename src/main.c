/* Includes */
#include "main.h"

/* Variables */
st_robot robot_st;
st_environment environment_st;

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

  // Show start message
  display_setCursor(0, 0);
  display_clear();
  display_print(LCD_SCREEN_START);
  // Delay show start message
  delay(LCD_DELAY_SHOW_START_SCREE);

  /* Main loop */
  while(true) {
    state_machine(&robot_st, &environment_st);
  }
}