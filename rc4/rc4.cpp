#include "rc4_core.h"
#include "../include/crypto_api.h"

#include <vector>
#include <random>
#include <iostream>
#include <exception>

using namespace std;

static AlgorithmInfo algorithm_info =
{
    "RC4",
    "Потоковый шифр RC4",
    "1.0"
};

static vector<uint8_t> generated_key;

static bool isKeyWeak(ConstBuffer key)
{
    for (size_t i = 1; i < key.size; i++)
    {
        if (key.data[i] != key.data[0])
        {
            return false;
        }
    }

    return true;
}

static int validateBuffers(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
)
{
    if (key.data == nullptr)
    {
        cerr << "Ошибка: ключ является нулевым указателем\n";
        return CRYPTO_ERR_NULL_KEY;
    }

    if (input.data == nullptr)
    {
        cerr << "Ошибка: входные данные являются нулевым указателем\n";
        return CRYPTO_ERR_NULL_INPUT;
    }

    if (output == nullptr)
    {
        cerr << "Ошибка: выходной буфер является нулевым указателем\n";
        return CRYPTO_ERR_NULL_OUTPUT;
    }

    if (key.size == 0)
    {
        cerr << "Ошибка: ключ не может быть пустым\n";
        return CRYPTO_ERR_EMPTY_KEY;
    }

    if (input.size == 0)
    {
        cerr << "Ошибка: входные данные не могут быть пустыми\n";
        return CRYPTO_ERR_EMPTY_INPUT;
    }

    if (output->size < input.size)
    {
        cerr << "Ошибка: выходной буфер слишком маленький\n";
        return CRYPTO_ERR_SMALL_BUF;
    }

    if (isKeyWeak(key))
    {
        cerr << "Предупреждение: все байты ключа одинаковые\n";
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
    return input_size;
}

extern "C" __declspec(dllexport)
int encrypt(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
)
{
    int result = validateBuffers(
        key,
        input,
        output
    );

    if (result != CRYPTO_OK)
    {
        return result;
    }

    try
    {
        vector<uint8_t> key_data(
            key.data,
            key.data + key.size
        );

        vector<uint8_t> input_data(
            input.data,
            input.data + input.size
        );

        vector<uint8_t> encrypted =
            rc4Transform(
                key_data,
                input_data
            );

        for (size_t i = 0; i < encrypted.size(); i++)
        {
            output->data[i] = encrypted[i];
        }

        return CRYPTO_OK;
    }
    catch (const bad_alloc&)
    {
        cerr << "Ошибка: недостаточно памяти\n";
        return CRYPTO_ERR_GENERAL;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: "
             << e.what()
             << "\n";

        return CRYPTO_ERR_GENERAL;
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка\n";
        return CRYPTO_ERR_GENERAL;
    }
}

extern "C" __declspec(dllexport)
int decrypt(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
)
{
    return encrypt(
        key,
        input,
        output
    );
}

extern "C" __declspec(dllexport)
ConstBuffer generate_key(size_t length)
{
    try
    {
        if (length == 0)
        {
            length = 16;
        }

        generated_key.resize(length);

        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<int> dist(
            0,
            255
        );

        for (size_t i = 0; i < length; i++)
        {
            generated_key[i] =
                static_cast<uint8_t>(
                    dist(gen)
                );
        }

        ConstBuffer result;

        result.data =
            generated_key.data();

        result.size =
            generated_key.size();

        return result;
    }
    catch (...)
    {
        ConstBuffer result;

        result.data = nullptr;
        result.size = 0;

        return result;
    }
}