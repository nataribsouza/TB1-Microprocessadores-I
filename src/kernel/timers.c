/* Includes */
#include "timers.h"

uint32_t tmillis = 0;

/**
 * @brief Set timer 0 to work as a counter of
 * milliseconds since the start of the microcontroller
 * 
 */
void timer_init(void) {  
    // Set CTC mode
    set_bit_reg(&TCCR0A, WGM01);

    // Set on compare reset value
    set_reg(&OCR0A, TIMER0_ONCOMPARE_RESET_VALUE);

    // Set 64-bit prescaler
    set_reg(&TCCR0B, ((1 << CS01) | (1 << CS00)));

    // Enable timer 0 interrupt by compare
    set_bit_reg(&TIMSK0, OCIE0A);

    // Enable global interrupts
    sei();
}

/**
 * @brief Delay ms milliseconds
 * 
 * @param ms 
 */
void delay(uint16_t ms) {
    uint32_t time = millis();

    while(millis() - time < ms);
}

/**
 * @brief Return the number of milliseconds passed 
 * since the start of the microcontroller
 * 
 * @return uint32_t 
 */
uint32_t millis(void) {
    uint32_t ltime;

    // Disable interrups while reading timer0_millis
    // to prevent inconsistent values 
    cli();
    ltime = tmillis;
    sei();

    return ltime;
}

/**
 * @brief Timer0 interrupt routine
 * 
 */
ISR(TIMER0_COMPA_vect) {
    tmillis++;
}