#include "tea_core.h"
#include "../include/crypto_api.h"

#include <vector>
#include <random>
#include <iostream>
#include <exception>

using namespace std;

static AlgorithmInfo algorithm_info =
{
    "TEA",
    "Tiny Encryption Algorithm (64-bit block)",
    "1.0",
    16,
    16
};

static vector<uint8_t> generated_key;

static int validate(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    if (key.data == nullptr)
    {
        cerr << "Ошибка: ключ nullptr\n";
        return CRYPTO_ERR_NULL_KEY;
    }

    if (input.data == nullptr)
    {
        cerr << "Ошибка: input nullptr\n";
        return CRYPTO_ERR_NULL_INPUT;
    }

    if (output == nullptr)
    {
        cerr << "Ошибка: output nullptr\n";
        return CRYPTO_ERR_NULL_OUTPUT;
    }

    if (key.size == 0)
    {
        cerr << "Ошибка: пустой ключ\n";
        return CRYPTO_ERR_EMPTY_KEY;
    }

    if (input.size == 0)
    {
        cerr << "Ошибка: пустой input\n";
        return CRYPTO_ERR_EMPTY_INPUT;
    }

    if (output->size < input.size)
    {
        cerr << "Ошибка: маленький буфер\n";
        return CRYPTO_ERR_SMALL_BUF;
    }

    return CRYPTO_OK;
}

extern "C" __declspec(dllexport)
const AlgorithmInfo* get_algorithm_info()
{
    return &algorithm_info;
}

extern "C" __declspec(dllexport)
size_t get_output_size(size_t input_size, int mode)
{
    (void)mode;

    size_t rem = input_size % 8;
    if (rem == 0)
        return input_size;

    return input_size + (8 - rem);
}

extern "C" __declspec(dllexport)
int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    try
    {
        int err = validate(key, input, output);
        if (err != CRYPTO_OK)
            return err;

        vector<uint8_t> k(key.data, key.data + key.size);
        vector<uint8_t> data(input.data, input.data + input.size);

        vector<uint8_t> result = teaEncrypt(k, data);

        for (size_t i = 0; i < result.size(); i++)
            output->data[i] = result[i];

        return CRYPTO_OK;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return CRYPTO_ERR_GENERAL;
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка\n";
        return CRYPTO_ERR_GENERAL;
    }
}

extern "C" __declspec(dllexport)
int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    try
    {
        int err = validate(key, input, output);
        if (err != CRYPTO_OK)
            return err;

        vector<uint8_t> k(key.data, key.data + key.size);
        vector<uint8_t> data(input.data, input.data + input.size);

        vector<uint8_t> result = teaDecrypt(k, data);

        for (size_t i = 0; i < result.size(); i++)
            output->data[i] = result[i];

        return CRYPTO_OK;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return CRYPTO_ERR_GENERAL;
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка\n";
        return CRYPTO_ERR_GENERAL;
    }
}

extern "C" __declspec(dllexport)
ConstBuffer generate_key(size_t length)
{
    try
    {
        if (length != 16)
            length = 16;

        generated_key.resize(16);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 255);

        for (int i = 0; i < 16; i++)
            generated_key[i] = (uint8_t)dist(gen);

        return {
            generated_key.data(),
            generated_key.size()
        };
    }
    catch (...)
    {
        return {nullptr, 0};
    }
}