#include "permutation_utils.h"

#include "permutation_core.h"

#include <iostream>
#include <random>
#include <vector>


using namespace std;



bool validateBuffers(ConstBuffer key, ConstBuffer input, MutBuffer* output)
{
    if (key.data == nullptr)
    {
        cerr << "Ошибка: ключ отсутствует\n";
        return false;
    }


    if (input.data == nullptr)
    {
        cerr << "Ошибка: входные данные отсутствуют\n";
        return false;
    }


    if (output == nullptr)
    {
        cerr << "Ошибка: выходной буфер отсутствует\n";
        return false;
    }


    if (key.size == 0)
    {
        cerr << "Ошибка: ключ пустой\n";
        return false;
    }


    if (input.size == 0)
    {
        cerr << "Ошибка: входные данные пустые\n";
        return false;
    }


    if (output->size < input.size)
    {
        cerr << "Ошибка: выходной буфер меньше входных данных\n";
        return false;
    }


    return true;
}



bool checkSameBytes(ConstBuffer key)
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



int safeEncryptPermutation(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
)
{
    try
    {
        if (!validateBuffers(key, input, output))
        {
            return CRYPTO_ERR_GENERAL;
        }


        if (checkSameBytes(key))
        {
            cerr << "Предупреждение: все байты ключа одинаковые\n";
        }


        vector<size_t> permutation(key.size);


        if (!buildPermutation(
            key.data,
            key.size,
            permutation.data()))
        {
            return CRYPTO_ERR_INVALID_KEY;
        }


        encryptPermutation(
            input.data,
            output->data,
            input.size,
            permutation.data(),
            key.size
        );


        return CRYPTO_OK;
    }


    catch (bad_alloc&)
    {
        cerr << "Ошибка: недостаточно памяти\n";
        return CRYPTO_ERR_GENERAL;
    }


    catch (exception& e)
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



int safeDecryptPermutation(
    ConstBuffer key,
    ConstBuffer input,
    MutBuffer* output
)
{
    try
    {
        if (!validateBuffers(key, input, output))
        {
            return CRYPTO_ERR_GENERAL;
        }


        vector<size_t> permutation(key.size);


        if (!buildPermutation(
            key.data,
            key.size,
            permutation.data()))
        {
            return CRYPTO_ERR_INVALID_KEY;
        }


        decryptPermutation(
            input.data,
            output->data,
            input.size,
            permutation.data(),
            key.size
        );


        return CRYPTO_OK;
    }


    catch (bad_alloc&)
    {
        cerr << "Ошибка: недостаточно памяти\n";
        return CRYPTO_ERR_GENERAL;
    }


    catch (exception& e)
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



ConstBuffer generateRandomKey(size_t length)
{
    uint8_t* key = new uint8_t[length];


    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> distribution(0, 255);


    for (size_t i = 0; i < length; i++)
    {
        key[i] = static_cast<uint8_t>(distribution(generator));
    }


    return { key, length };
}