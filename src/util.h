#pragma once

#include <stdint.h>

namespace util {
template <uint8_t N, typename T>
constexpr uint8_t array_size(T (&)[N]) {
    return N;
}

template <typename T>
constexpr T reverse_bits(T input) {
    T reverse = 0;
    uint8_t i = 0;

    do {
        reverse <<= 1;
        reverse |= input & 1;
        input >>= 1;
    } while (++i < 7);

    return reverse;
}

constexpr uint8_t string_length(const char *string) {
    uint8_t i = 0;
    for (; string[i] != '\0'; i++)
        ;

    return i;
}
}  // namespace util
