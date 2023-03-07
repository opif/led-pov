#include "clock.h"

#include <avr/interrupt.h>

static volatile uint32_t ms = 0;
constexpr uint16_t ms_per_tick = 256ul + 40;  // slight clock adjustment based on measurements at 3.3V

ISR(WDT_vect) {
    WDTCSR |= _BV(WDIE);

    ms += ms_per_tick;
}

void clock_setup() {
    uint8_t tSREG = SREG;

    cli();
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDCE) | _BV(WDE) | _BV(WDIE) | _BV(WDP2);
    ms = 0;
    SREG = tSREG;
}

void clock_disable() {
    uint8_t tSREG = SREG;

    cli();
    MCUSR = 0;
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = 0;
    ms = 0;
    SREG = tSREG;
}

uint32_t clock() {
    uint32_t tmp;
    uint8_t tSREG = SREG;

    cli();
    tmp = ms;
    SREG = tSREG;

    return tmp;
}

void clock_reset() {
    uint8_t tSREG = SREG;

    cli();
    ms = 0;
    SREG = tSREG;
}
