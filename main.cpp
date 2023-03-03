#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "src/font.h"
#include "src/adc.h"

const int swipeTimeMs = 100;

// constexpr char message[] = "Lorem ipsum, dolor sit amet...";
constexpr char message[] = "Kamila \njest super!";
const auto displayText PROGMEM = CharacterGlyph<message>();

void setup()
{
    // cli(); // not required because default SREG value is 0;

    // WDTCR = _BV(WDCE);
    // WDTCR = _BV(WDP2) | _BV(WDP1) | _BV(WDTIE);

    // TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0B1) | _BV(COM0B0);
    // TCCR0B = _BV(CS00);
    // DDRB = _BV(DDB1)  | _BV(DDB2);
    // DIDR0 = _BV(ADC2D);

    DDRA = 0xFF;
    PORTA = 0;
    DDRB = 0xFF;
    PORTB = 0;

    adc_disable();

    sei();
}

int i = 0; // todo: get rid of me

void loop()
{
    for (auto a : displayText.data)
    {
        i += *a.gsl;
    }
}

void disable_clocks()
{
    PRR |= _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRUSI) | _BV(PRADC);
}

void halt()
{
    adc_disable();
    disable_clocks();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

int main(void)
{
    setup();

    for (;;)
    {
        loop();
        halt();
    }
}
