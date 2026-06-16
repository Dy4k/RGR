#include "../include/crypto_api.h"

#include "autokey_utils.h"



extern "C" __declspec(dllexport)
const AlgorithmInfo* get_algorithm_info()
{
    static AlgorithmInfo info =
    {
        "Autokey",
        "Автоключевой байтовый шифр сложением по модулю 256",
        "1.0",
        1,
        256
    };


    return &info;
}



extern "C" __declspec(dllexport)
size_t get_output_size(size_t input_size, int mode)
{
    return input_size;
}



extern "C" __declspec(dllexport)
int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    return safeEncryptAutokey(key, input, output);
}



extern "C" __declspec(dllexport)
int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    return safeDecryptAutokey(key, input, output);
}



extern "C" __declspec(dllexport)
ConstBuffer generate_key(size_t length)
{
    return generateRandomKey(length);
}