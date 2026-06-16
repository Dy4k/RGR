#include "permutation_core.h"

using namespace std;


bool buildPermutation(
    const uint8_t* key,
    size_t key_size,
    size_t* permutation
)
{
    if (key == nullptr || permutation == nullptr || key_size == 0)
    {
        return false;
    }


    for (size_t i = 0; i < key_size; i++)
    {
        permutation[i] = i;
    }


    for (size_t i = 0; i < key_size; i++)
    {
        for (size_t j = i + 1; j < key_size; j++)
        {
            if (key[permutation[i]] > key[permutation[j]])
            {
                size_t temp = permutation[i];

                permutation[i] = permutation[j];

                permutation[j] = temp;
            }
        }
    }


    return true;
}



void encryptPermutation(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const size_t* permutation,
    size_t key_size
)
{
    size_t full_blocks = size / key_size;

    size_t current_index = 0;


    for (size_t block = 0; block < full_blocks; block++)
    {
        for (size_t i = 0; i < key_size; i++)
        {
            output[current_index + i] =
                input[current_index + permutation[i]];
        }

        current_index += key_size;
    }


    while (current_index < size)
    {
        output[current_index] = input[current_index];

        current_index++;
    }
}



void decryptPermutation(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const size_t* permutation,
    size_t key_size
)
{
    size_t full_blocks = size / key_size;

    size_t current_index = 0;


    for (size_t block = 0; block < full_blocks; block++)
    {
        for (size_t i = 0; i < key_size; i++)
        {
            output[current_index + permutation[i]] =
                input[current_index + i];
        }

        current_index += key_size;
    }


    while (current_index < size)
    {
        output[current_index] = input[current_index];

        current_index++;
    }
}