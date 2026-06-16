#include "tea_core.h"

using namespace std;

static void teaEncryptBlock(uint32_t v[2], const uint32_t k[4])
{
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t sum = 0;
    uint32_t delta = 0x9e3779b9;

    for (int i = 0; i < 32; i++)
    {
        sum += delta;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }

    v[0] = v0;
    v[1] = v1;
}

static void teaDecryptBlock(uint32_t v[2], const uint32_t k[4])
{
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t delta = 0x9e3779b9;
    uint32_t sum = delta * 32;

    for (int i = 0; i < 32; i++)
    {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum -= delta;
    }

    v[0] = v0;
    v[1] = v1;
}

vector<uint8_t> teaEncrypt(
    const vector<uint8_t>& key,
    const vector<uint8_t>& data
)
{
    vector<uint8_t> key32(16, 0);

    for (size_t i = 0; i < 16 && i < key.size(); i++)
        key32[i] = key[i];

    const uint32_t* k = reinterpret_cast<const uint32_t*>(key32.data());

    vector<uint8_t> input = data;

    while (input.size() % 8 != 0)
        input.push_back(0);

    for (size_t i = 0; i < input.size(); i += 8)
    {
        uint32_t* block = reinterpret_cast<uint32_t*>(&input[i]);
        teaEncryptBlock(block, k);
    }

    return input;
}

vector<uint8_t> teaDecrypt(
    const vector<uint8_t>& key,
    const vector<uint8_t>& data
)
{
    vector<uint8_t> key32(16, 0);

    for (size_t i = 0; i < 16 && i < key.size(); i++)
        key32[i] = key[i];

    const uint32_t* k = reinterpret_cast<const uint32_t*>(key32.data());

    vector<uint8_t> input = data;

    for (size_t i = 0; i < input.size(); i += 8)
    {
        uint32_t* block = reinterpret_cast<uint32_t*>(&input[i]);
        teaDecryptBlock(block, k);
    }

    return input;
}