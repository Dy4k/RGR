#ifndef RC4_CORE_H
#define RC4_CORE_H

#include <vector>
#include <cstdint>

std::vector<uint8_t> rc4Transform(
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& input
);

#endif