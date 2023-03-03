#include "adc.h"

#include <avr/io.h>

void adc_disable() {
    // ADCSRA &= ~_BV(ADEN);
#ifdef __AVR_HAVE_PRR_PRADC
    PRR |= _BV(PRADC);
#endif
}
