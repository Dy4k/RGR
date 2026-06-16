#ifndef AUTOKEY_CORE_H
#define AUTOKEY_CORE_H

#include <cstddef>
#include <cstdint>


void encryptAutokey(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const uint8_t* key,
    size_t key_size
);


void decryptAutokey(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const uint8_t* key,
    size_t key_size
);


#endif