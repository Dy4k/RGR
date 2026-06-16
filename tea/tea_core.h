#ifndef TEA_CORE_H
#define TEA_CORE_H

#include <vector>
#include <cstdint>

std::vector<uint8_t> teaEncrypt(
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& data
);

std::vector<uint8_t> teaDecrypt(
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& data
);

#endif