#ifndef CRYPTO_API_H
#define CRYPTO_API_H

#include <cstddef>
#include <cstdint>

struct ConstBuffer
{
    const uint8_t* data;
    size_t size;
};

struct MutBuffer
{
    uint8_t* data;
    size_t size;
};

struct AlgorithmInfo
{
    const char* name;
    const char* description;
    const char* version;

    size_t min_key_size;
    size_t max_key_size;
};

enum CryptoError
{
    CRYPTO_OK = 0,

    CRYPTO_ERR_NULL_KEY,
    CRYPTO_ERR_NULL_INPUT,
    CRYPTO_ERR_NULL_OUTPUT,

    CRYPTO_ERR_EMPTY_KEY,
    CRYPTO_ERR_EMPTY_INPUT,

    CRYPTO_ERR_SMALL_BUF,

    CRYPTO_ERR_INVALID_KEY,

    CRYPTO_ERR_GENERAL
};

typedef const AlgorithmInfo* (*GetAlgorithmInfoFn)();

typedef size_t (*GetOutputSizeFn)(
    size_t input_size,
    int mode
);

typedef int (*EncryptFn)(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);

typedef int (*DecryptFn)(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
);

typedef ConstBuffer (*GenerateKeyFn)(
    size_t length
);

#endif