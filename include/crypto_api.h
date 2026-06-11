#ifndef CRYPTO_API_H
#define CRYPTO_API_H

#include <cstddef>
#include <cstdint>

struct ConstBuffer {
    const uint8_t* data;
    size_t size;
};

struct MutBuffer {
    uint8_t* data;
    size_t size;
};

struct AlgorithmInfo {
    const char* name;
    const char* description;
    const char* version;
};

typedef const AlgorithmInfo* (*GetAlgorithmInfoFn)();
typedef size_t (*GetOutputSizeFn)(size_t input_size, int mode);
typedef int (*EncryptFn)(ConstBuffer key, ConstBuffer input, MutBuffer* output);
typedef int (*DecryptFn)(ConstBuffer key, ConstBuffer input, MutBuffer* output);
typedef ConstBuffer (*GenerateKeyFn)(size_t length);


#define CRYPTO_OK              0
#define CRYPTO_ERR_NULL_KEY    1
#define CRYPTO_ERR_NULL_INPUT  2
#define CRYPTO_ERR_NULL_OUTPUT 3
#define CRYPTO_ERR_EMPTY_KEY   4
#define CRYPTO_ERR_EMPTY_INPUT 5
#define CRYPTO_ERR_SMALL_BUF   6
#define CRYPTO_ERR_GENERAL     7

#endif