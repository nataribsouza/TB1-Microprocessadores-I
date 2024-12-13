/* Includes */
#include "utils.h"

/* Functions */

/**
 * @brief Reboot the microcontroler
 * 
 */
void reboot(void) {
  // Disable interruptions
  cli();
  
  // Set watchdog for 15 ms
  wdt_enable(WDTO_15MS);
  
  // Wait untill watchdog reboot the system
  while (1) {}
}