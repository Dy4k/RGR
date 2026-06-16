#ifndef PERMUTATION_CORE_H
#define PERMUTATION_CORE_H

#include <cstddef>
#include <cstdint>


bool buildPermutation(
    const uint8_t* key,
    size_t key_size,
    size_t* permutation
);


void encryptPermutation(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const size_t* permutation,
    size_t key_size
);


void decryptPermutation(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const size_t* permutation,
    size_t key_size
);


#endif