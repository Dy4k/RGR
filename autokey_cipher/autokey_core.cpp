#include "autokey_core.h"


void encryptAutokey(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const uint8_t* key,
    size_t key_size
)
{
    for (size_t i = 0; i < size; i++)
    {
        uint8_t key_byte;


        if (i < key_size)
        {
            key_byte = key[i];
        }
        else
        {
            key_byte = input[i - key_size];
        }


        output[i] = static_cast<uint8_t>(input[i] + key_byte);
    }
}



void decryptAutokey(
    const uint8_t* input,
    uint8_t* output,
    size_t size,
    const uint8_t* key,
    size_t key_size
)
{
    for (size_t i = 0; i < size; i++)
    {
        uint8_t key_byte;


        if (i < key_size)
        {
            key_byte = key[i];
        }
        else
        {
            key_byte = output[i - key_size];
        }


        output[i] = static_cast<uint8_t>(input[i] - key_byte);
    }
}