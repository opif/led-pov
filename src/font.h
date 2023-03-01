#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct {
    uint8_t gsl[4];
} GLYPH_SCANLINES;

typedef struct {
    uint8_t data[5];
} FONT_GLYPH_PAIR;

constexpr uint8_t MIN_CHARACTER = ' ';
constexpr uint8_t MAX_CHARACTER = '~';
constexpr uint8_t charCount = (MAX_CHARACTER - MIN_CHARACTER) / 2;
constexpr FONT_GLYPH_PAIR encodedList[charCount] PROGMEM = {
    {74, 74, 64, 0, 64}, {167, 250, 166, 245, 174}, {196, 26, 100, 138, 53},
    {66, 68, 4, 4, 2}, {73, 38, 47, 38, 73}, {0, 64, 224, 68, 8}, {0, 0, 224, 0, 4},
    {38, 43, 77, 137, 134}, {38, 105, 34, 36, 47}, {233, 25, 111, 17, 225},
    {246, 136, 238, 25, 230}, {246, 25, 38, 41, 38}, {96, 148, 112, 20, 96},
    {2, 68, 8, 68, 66}, {8, 228, 2, 228, 8}, {198, 41, 75, 8, 71},
    {110, 153, 254, 153, 158}, {110, 153, 137, 153, 110}, {255, 136, 238, 136, 248},
    {105, 137, 191, 153, 105}, {65, 65, 65, 73, 70}, {152, 168, 200, 168, 159},
    {153, 253, 155, 153, 153}, {110, 153, 158, 152, 104}, {110, 153, 158, 186, 121},
    {126, 132, 100, 20, 228}, {154, 154, 154, 154, 100}, {153, 153, 150, 249, 153},
    {175, 161, 70, 72, 79}, {104, 72, 68, 66, 98}, {100, 42, 32, 32, 96},
    {4, 2, 0, 0, 240}, {110, 153, 254, 153, 158}, {110, 153, 137, 153, 110},
    {255, 136, 238, 136, 248}, {105, 137, 191, 153, 105}, {65, 65, 65, 73, 70},
    {152, 168, 200, 168, 159}, {153, 253, 155, 153, 153}, {110, 153, 158, 152, 104},
    {110, 153, 158, 186, 121}, {126, 132, 100, 20, 228}, {154, 154, 154, 154, 100},
    {153, 153, 150, 249, 153}, {175, 161, 70, 72, 79}, {100, 68, 192, 68, 100},
    {192, 69, 106, 64, 192}
};


constexpr bool isInBounds(uint8_t ch) {
    return !(ch < MIN_CHARACTER || ch > MAX_CHARACTER);
}

constexpr void decodeScanlines(uint8_t ch, GLYPH_SCANLINES &out) {
    const uint8_t index = (ch - MIN_CHARACTER) / 2;
    const uint8_t offset = (ch - MIN_CHARACTER) % 2;
    FONT_GLYPH_PAIR pair = encodedList[index];

    if (offset == 1) {
        for (unsigned int i = 0; i < sizeof(pair.data); i++) {
            pair.data[i] &= 0xF0;
            pair.data[i] >>= 4;
        }
    } else {
        for (unsigned int i = 0; i < sizeof(pair.data); i++) {
            pair.data[i] &= 0x0F;
        }
    }

    for (unsigned int i = 0; i < 4; i++) {
       uint8_t skanline = 0;

        for (int j = 4; j >= 0; j--) {
            skanline <<= 1;
            skanline |= (pair.data[j] & 1);

            pair.data[j] >>= 1;
        }

        out.gsl[i] = skanline;
    }
}


constexpr bool getGlyphScanlines(char in, GLYPH_SCANLINES &out) {
    if (!isInBounds(in)) {
        return false;
    }

    decodeScanlines(in, out);

    return true;
}

constexpr int length(const char* string) {
    int i = 0;
    for(; string[i] != '\0'; i++);

    return i;
}

template<const char* message>
struct CharacterGlyph {
    constexpr CharacterGlyph() : data() {
        for (unsigned int i = 0; i < length(message); i++) {
            getGlyphScanlines(message[i], data[i]);
        }
    }

    GLYPH_SCANLINES data[length(message)];
};
