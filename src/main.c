/* Includes */
#include "main.h"

/* Prototypes */
void blink_led(void);

/* Variables */
bool blink;

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

  // Init pin led
  init_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, ENUM_PINMODE_OUTPUT);

  display_setCursor(0, 0);    // Cursor na linha 0, coluna 0
  display_print("Hello, Natan!");  // Exibe mensagem
  display_setCursor(1, 0);    // Cursor na linha 1, coluna 0
  display_print("I2C LCD Demo!");

  /* Main loop */
  while(true) {
    // Continuous read keyboard
    handle_keyboard();

    if(read_keyboard(0)) {
      blink = !blink;
    }

    // Toggle blink
    if(blink) {
      blink_led();
    } else {
      set_pin(ROBOT_PIN_LED, ENUM_PORT_PORTC, false);
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