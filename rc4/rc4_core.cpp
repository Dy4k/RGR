#include "rc4_core.h"

using namespace std;

vector<uint8_t> rc4Transform(
    const vector<uint8_t>& key,
    const vector<uint8_t>& input
)
{
    vector<uint8_t> s(256);

    for (int i = 0; i < 256; i++)
    {
        s[i] = static_cast<uint8_t>(i);
    }

    int j = 0;

    for (int i = 0; i < 256; i++)
    {
        j = (
            j +
            s[i] +
            key[i % key.size()]
        ) % 256;

        uint8_t temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }

    vector<uint8_t> output(input.size());

    int i = 0;
    j = 0;

    for (size_t k = 0; k < input.size(); k++)
    {
        i = (i + 1) % 256;

        j = (j + s[i]) % 256;

        uint8_t temp = s[i];
        s[i] = s[j];
        s[j] = temp;

        int t = (s[i] + s[j]) % 256;

        uint8_t key_byte = s[t];

        output[k] =
            input[k] ^ key_byte;
    }

    return output;
}