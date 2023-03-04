#include "clock.h"

#include <avr/interrupt.h>

static volatile uint32_t ms = 0;
constexpr uint16_t ms_per_tick = 16ul;

ISR(WDT_vect) {
    WDTCSR |= _BV(WDIE);

    ms += ms_per_tick;
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
