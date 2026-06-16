#ifndef PERMUTATION_UTILS_H
#define PERMUTATION_UTILS_H

#include "../include/crypto_api.h"


bool validateBuffers(ConstBuffer key, ConstBuffer input, MutBuffer* output);

bool checkSameBytes(ConstBuffer key);


int safeEncryptPermutation(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);


int safeDecryptPermutation(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);


ConstBuffer generateRandomKey(size_t length);


#endif