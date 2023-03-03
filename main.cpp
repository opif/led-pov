#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "src/adc.h"
#include "src/clock.h"
#include "src/font.h"
#include "src/util.h"

// constexpr int SWIPE_TIME_MS = 100; // calculated
constexpr uint8_t MAX_LINE_LENGTH = 7;  // random guess
constexpr uint16_t SEGMENT_DURATION_MS = 5_sec;
constexpr uint32_t RUNTIME_DURATION_MS = 1_min;
constexpr double SWIPE_TIME_MS = 1000.0 * 8 / 240;  // captured with camera
constexpr double WAIT_TIME_MS = 1000.0 * 15 / 240;  // captured with camera
constexpr double SCANLINE_TIME = 1000.0 * SWIPE_TIME_MS / MAX_LINE_LENGTH / (GLYPH_WIDTH + 1);

// constexpr char message[] = "Lorem ipsum, dolor sit amet...";
constexpr char message[] = "Kamila\njest\nsuper!\n{} {} {}";
const auto displayText PROGMEM = CharacterGlyph<message>();

void setup() {
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
    clock_reset();

    sei();
}

void loop() {
    uint8_t currentSegment = 0;
    auto currentTime = clock();
    auto startTime = currentTime;
    auto segmentStartTime = currentTime;

    while (currentTime - startTime < RUNTIME_DURATION_MS) {
        currentTime = clock();

        for (auto i = currentSegment; i < util::array_size(displayText.data); i++) {
            auto glyph = displayText.data[i];

            for (auto sl : glyph.gsl) {
                PORTA = sl;
                _delay_us(SCANLINE_TIME);
            }

            PORTA = 0;
            if (glyph.type != GLYPH_TYPE::COMPOUND) {
                _delay_us(SCANLINE_TIME);
            }

            if (glyph.type == GLYPH_TYPE::NEWLINE) {
                if (currentTime - segmentStartTime >= SEGMENT_DURATION_MS) {
                    currentSegment = i + 1;
                    segmentStartTime = currentTime;

                    if (currentSegment >= sizeof(displayText.data)) {
                        currentSegment = 0;
                    }
                }

                break;
            }
        }
        _delay_ms(WAIT_TIME_MS);
    }
}

void disable_clocks() { PRR |= _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRUSI) | _BV(PRADC); }

void halt() {
    adc_disable();
    disable_clocks();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

int main(void) {
    setup();

    for (;;) {
        loop();
        halt();
    }
}
