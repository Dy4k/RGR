#ifndef AUTOKEY_UTILS_H
#define AUTOKEY_UTILS_H

#include "../include/crypto_api.h"


bool validateBuffers(ConstBuffer key, ConstBuffer input, MutBuffer* output);

bool checkSameBytes(ConstBuffer key);


int safeEncryptAutokey(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);


int safeDecryptAutokey(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);


ConstBuffer generateRandomKey(size_t length);


#endif