#include <avr/io.h>
#include "src/adc.h"

void adc_disable()
{
    // ADCSRA &= ~_BV(ADEN);
#ifdef __AVR_HAVE_PRR_PRADC
    PRR |= _BV(PRADC);
#endif
}
