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

    for (uint8_t i = 0; i < sizeof(T) * 8; i++) {
        reverse |= (input & 1);
        reverse <<= 1;
        input >>= 1;
    }

    return reverse;
}

constexpr uint8_t string_length(const char *string) {
    uint8_t i = 0;
    for (; string[i] != '\0'; i++)
        ;

    return i;
}
}  // namespace util
