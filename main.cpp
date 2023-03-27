#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "src/adc.h"
#include "src/clock.h"
#include "src/font.h"
#include "src/util.h"

constexpr uint8_t MAX_LINE_LENGTH = 7;  // random guess
constexpr uint16_t SEGMENT_DURATION_MS = 10_sec;
constexpr uint32_t RUNTIME_DURATION_MS = 1_min;
constexpr double SWIPE_TIME_MS = static_cast<double>(1_sec) * 8 / 240;  // captured with camera
constexpr double WAIT_TIME_MS = static_cast<double>(1_sec) * 15 / 240;  // captured with camera
// constexpr double SWIPE_TIME_MS = 28;  // measured with oscilloscope
// constexpr double WAIT_TIME_MS = 65.5;   // measured with oscilloscope
constexpr double GLYPH_SPACING = 2;  // measured with oscilloscope
constexpr double SCANLINE_TIME_US = 1000.0 * SWIPE_TIME_MS / MAX_LINE_LENGTH / (GLYPH_WIDTH + GLYPH_SPACING);
constexpr double GLYPH_SPACING_TIME_US = GLYPH_SPACING * SCANLINE_TIME_US;

// constexpr char message[] = "Lorem ipsum, dolor sit amet...";
constexpr char message[] = "Kamila\njest\nkochana\n{} {} {}";
constexpr auto displayText PROGMEM = CharacterGlyph<message>();

void setup() {
    // cli(); // not required because default SREG value is 0;

    DDRA = 0xFF;
    PORTA = 0;
    DDRB = 0xFF;
    PORTB = 0;

    adc_disable();
    clock_setup();

    sei();
}

void loop() {
    auto currentTime = clock();
    auto startTime = currentTime;
    auto segmentStartTime = currentTime;
    uint8_t currentSegment = 0;

    while (currentTime - startTime < RUNTIME_DURATION_MS) {
        GLYPH_SCANLINES glyph;
        currentTime = clock();

        for (auto i = currentSegment; i < util::array_size(displayText.data); i++) {
            memcpy_P(&glyph, &(displayText.data[i]), sizeof(glyph));

            for (auto sl : glyph.gsl) {
                PORTA = sl;
                _delay_us(SCANLINE_TIME_US);
            }

            PORTA = 0;
            if (glyph.type != GLYPH_TYPE::COMPOUND) {
                _delay_us(GLYPH_SPACING_TIME_US);
            }

            if (glyph.type == GLYPH_TYPE::NEWLINE) {
                if (currentTime - segmentStartTime >= SEGMENT_DURATION_MS) {
                    currentSegment = i + 1;
                    segmentStartTime = currentTime;

                    if (currentSegment >= util::array_size(displayText.data)) {
                        currentSegment = 0;
                    }
                }

                break;
            }
        }
        _delay_ms(WAIT_TIME_MS);
    }
}

void disable_avr_clocks() { PRR |= _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRUSI) | _BV(PRADC); }

void halt() {
    cli();

    adc_disable();
    clock_disable();
    disable_avr_clocks();
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
