#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

constexpr uint8_t GLYPH_WIDTH = 5;

typedef struct
{
    uint8_t gsl[GLYPH_WIDTH];
    uint8_t state;
} GLYPH_SCANLINES;

typedef struct
{
    uint8_t data[GLYPH_WIDTH];
} FONT_GLYPH_PAIR;

constexpr uint8_t MIN_CHARACTER = ' ';
constexpr uint8_t MAX_CHARACTER = '~';
constexpr uint8_t charCount = (MAX_CHARACTER - MIN_CHARACTER);
constexpr FONT_GLYPH_PAIR packedFont[charCount] PROGMEM = {
    {0, 0, 125, 0, 0}, {80, 96, 0, 80, 96}, {20, 127, 20, 127, 20}, {18, 42, 127, 42, 36}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 80, 96, 0, 0}, {0, 28, 34, 65, 0}, {0, 65, 34, 28, 0}, {20, 8, 62, 8, 20}, {8, 8, 62, 8, 8}, {0, 5, 6, 0, 0}, {8, 8, 8, 8, 8}, {0, 3, 3, 0, 0}, {2, 4, 8, 16, 32}, {62, 69, 73, 81, 62}, {0, 33, 127, 1, 0}, {33, 67, 69, 73, 49}, {66, 65, 81, 105, 70}, {12, 20, 36, 127, 4}, {114, 81, 81, 81, 78}, {30, 41, 73, 73, 6}, {64, 64, 79, 80, 96}, {54, 73, 73, 73, 54}, {48, 73, 73, 74, 60}, {0, 54, 54, 0, 0}, {0, 53, 54, 0, 0}, {8, 20, 34, 65, 0}, {20, 20, 20, 20, 20}, {0, 65, 34, 20, 8}, {32, 64, 69, 72, 48}, {48, 72, 72, 48, 0}, {63, 72, 72, 72, 63}, {127, 73, 73, 73, 54}, {62, 65, 65, 65, 34}, {127, 65, 65, 34, 28}, {127, 73, 73, 73, 65}, {127, 72, 72, 72, 64}, {62, 65, 73, 73, 47}, {127, 8, 8, 8, 127}, {0, 65, 127, 65, 0}, {2, 1, 65, 126, 64}, {127, 8, 20, 34, 65}, {127, 1, 1, 1, 1}, {127, 32, 24, 32, 127}, {127, 16, 8, 4, 127}, {62, 65, 65, 65, 62}, {127, 72, 72, 72, 48}, {62, 65, 69, 66, 61}, {127, 72, 76, 74, 49}, {49, 73, 73, 73, 70}, {64, 64, 127, 64, 64}, {126, 1, 1, 1, 126}, {124, 2, 1, 2, 124}, {127, 2, 12, 2, 127}, {99, 20, 8, 20, 99}, {96, 16, 15, 16, 96}, {67, 69, 73, 81, 97}, {5, 13, 21, 37, 69}, {20, 52, 28, 22, 20}, {69, 37, 21, 13, 5}, {16, 32, 127, 32, 16}, {1, 1, 1, 1, 1}, {17, 30, 16, 31, 17}, {63, 72, 72, 72, 63}, {127, 73, 73, 73, 54}, {62, 65, 65, 65, 34}, {127, 65, 65, 34, 28}, {127, 73, 73, 73, 65}, {127, 72, 72, 72, 64}, {62, 65, 73, 73, 47}, {127, 8, 8, 8, 127}, {0, 65, 127, 65, 0}, {2, 1, 65, 126, 64}, {127, 8, 20, 34, 65}, {127, 1, 1, 1, 1}, {127, 32, 24, 32, 127}, {127, 16, 8, 4, 127}, {62, 65, 65, 65, 62}, {127, 72, 72, 72, 48}, {62, 65, 69, 66, 61}, {127, 72, 76, 74, 49}, {49, 73, 73, 73, 70}, {64, 64, 127, 64, 64}, {126, 1, 1, 1, 126}, {124, 2, 1, 2, 124}, {127, 2, 12, 2, 127}, {99, 20, 8, 20, 99}, {96, 16, 15, 16, 96}, {67, 69, 73, 81, 97}, {0, 0, 0, 0, 0}, {31, 21, 21, 17, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

constexpr bool isInBounds(uint8_t ch)
{
    return !(ch < MIN_CHARACTER || ch > MAX_CHARACTER);
}

constexpr void decodeScanlines(uint8_t ch, GLYPH_SCANLINES &out)
{
    const uint8_t index = (ch - MIN_CHARACTER) / 2;
    const uint8_t offset = (ch - MIN_CHARACTER) % 2;
    FONT_GLYPH_PAIR pair = packedFont[index];

    constexpr uint8_t PAIR_DATA_SIZE = sizeof(pair.data);
    constexpr uint8_t GSL_DATA_SIZE = sizeof(out.gsl);

    for (uint8_t i = 0; i < PAIR_DATA_SIZE; i++)
    {
        if (offset == 1)
        {
            pair.data[i] >>= 4;
        }
        pair.data[i] &= 0x0F;
    }

    for (uint8_t i = 0; i < GSL_DATA_SIZE; i++)
    {
        uint8_t skanline = 0;

        for (int8_t j = PAIR_DATA_SIZE - 1; j >= 0; j--)
        {
            skanline <<= 1;
            skanline |= (pair.data[j] & 1);

            pair.data[j] >>= 1;
        }

        out.gsl[i] = skanline;
    }

    out.state = 0;
}

constexpr bool getGlyphScanlines(char in, GLYPH_SCANLINES &out)
{
    if (!isInBounds(in))
    {
        if (in == '\n')
        {
            out.gsl[0] = 0;
            out.gsl[1] = 0;
            out.gsl[2] = 0;
            out.gsl[3] = 0;
            out.gsl[4] = 0;
            out.state = 1;
        }

        return false;
    }

    decodeScanlines(in, out);

    return true;
}

constexpr int length(const char *string)
{
    int i = 0;
    for (; string[i] != '\0'; i++)
        ;

    return i;
}

template <const char message[]>
struct CharacterGlyph
{
    constexpr CharacterGlyph() : data()
    {
        for (unsigned int i = 0; i < length(message); i++)
        {
            getGlyphScanlines(message[i], data[i]);
        }
    }

    GLYPH_SCANLINES data[length(message)];
};
