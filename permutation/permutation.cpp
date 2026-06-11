#include <windows.h>
#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include "../include/crypto_api.h"
#include "permutation_core.h"

using namespace std;

static const AlgorithmInfo info = {
    "Permutation",
    "Шифр фиксированной перестановки по ключевому слову",
    "1.0"
};

extern "C" __declspec(dllexport)
const AlgorithmInfo* get_algorithm_info() {
    return &info;
}

extern "C" __declspec(dllexport)
size_t get_output_size(size_t input_size, int mode) {
    (void)mode;
    return input_size;
}

extern "C" __declspec(dllexport)
int encrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    try {
        if (key.data == nullptr) {
            cerr << "[Permutation] Ошибка: ключ является нулевым указателем\n";
            return CRYPTO_ERR_NULL_KEY;
        }
        if (input.data == nullptr) {
            cerr << "[Permutation] Ошибка: входные данные являются нулевым указателем\n";
            return CRYPTO_ERR_NULL_INPUT;
        }
        if (output == nullptr) {
            cerr << "[Permutation] Ошибка: выходной буфер является нулевым указателем\n";
            return CRYPTO_ERR_NULL_OUTPUT;
        }
        if (key.size == 0) {
            cerr << "[Permutation] Ошибка: ключ не может быть пустым\n";
            return CRYPTO_ERR_EMPTY_KEY;
        }
        if (input.size == 0) {
            cerr << "[Permutation] Ошибка: входные данные не могут быть пустыми\n";
            return CRYPTO_ERR_EMPTY_INPUT;
        }
        if (output->size < input.size) {
            cerr << "[Permutation] Ошибка: выходной буфер меньше входных данных\n";
            return CRYPTO_ERR_SMALL_BUF;
        }

        bool all_same = true;
        for (size_t i = 1; i < key.size; i++) {
            if (key.data[i] != key.data[0]) {
                all_same = false;
                break;
            }
        }
        if (all_same) {
            cerr << "[Permutation] Предупреждение: все байты ключа одинаковые\n";
        }

        vector<size_t> perm = buildPermutation(key.data, key.size);
        size_t block_size = key.size;

        size_t full_blocks = input.size / block_size;
        size_t remainder = input.size % block_size;

        for (size_t block = 0; block < full_blocks; block++) {
            size_t offset = block * block_size;
            applyPermutation(input.data + offset, output->data + offset,
                             perm, block_size);
        }

        if (remainder > 0) {
            size_t offset = full_blocks * block_size;
            for (size_t i = 0; i < remainder; i++) {
                output->data[offset + i] = input.data[offset + i];
            }
        }

        return CRYPTO_OK;

    } catch (const bad_alloc&) {
        cerr << "[Permutation] Ошибка: недостаточно памяти\n";
        return CRYPTO_ERR_GENERAL;
    } catch (const exception& e) {
        cerr << "[Permutation] Ошибка: " << e.what() << "\n";
        return CRYPTO_ERR_GENERAL;
    } catch (...) {
        cerr << "[Permutation] Ошибка: неожиданное исключение\n";
        return CRYPTO_ERR_GENERAL;
    }
}

extern "C" __declspec(dllexport)
int decrypt(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    try {
        if (key.data == nullptr) {
            cerr << "[Permutation] Ошибка: ключ является нулевым указателем\n";
            return CRYPTO_ERR_NULL_KEY;
        }
        if (input.data == nullptr) {
            cerr << "[Permutation] Ошибка: входные данные являются нулевым указателем\n";
            return CRYPTO_ERR_NULL_INPUT;
        }
        if (output == nullptr) {
            cerr << "[Permutation] Ошибка: выходной буфер является нулевым указателем\n";
            return CRYPTO_ERR_NULL_OUTPUT;
        }
        if (key.size == 0) {
            cerr << "[Permutation] Ошибка: ключ не может быть пустым\n";
            return CRYPTO_ERR_EMPTY_KEY;
        }
        if (input.size == 0) {
            cerr << "[Permutation] Ошибка: входные данные не могут быть пустыми\n";
            return CRYPTO_ERR_EMPTY_INPUT;
        }
        if (output->size < input.size) {
            cerr << "[Permutation] Ошибка: выходной буфер меньше входных данных\n";
            return CRYPTO_ERR_SMALL_BUF;
        }

        vector<size_t> perm = buildPermutation(key.data, key.size);
        vector<size_t> inv_perm = buildInversePermutation(perm);
        size_t block_size = key.size;

        size_t full_blocks = input.size / block_size;
        size_t remainder = input.size % block_size;

        for (size_t block = 0; block < full_blocks; block++) {
            size_t offset = block * block_size;
            applyPermutation(input.data + offset, output->data + offset,
                             inv_perm, block_size);
        }

        if (remainder > 0) {
            size_t offset = full_blocks * block_size;
            for (size_t i = 0; i < remainder; i++) {
                output->data[offset + i] = input.data[offset + i];
            }
        }

        return CRYPTO_OK;

    } catch (const bad_alloc&) {
        cerr << "[Permutation] Ошибка: недостаточно памяти\n";
        return CRYPTO_ERR_GENERAL;
    } catch (const exception& e) {
        cerr << "[Permutation] Ошибка: " << e.what() << "\n";
        return CRYPTO_ERR_GENERAL;
    } catch (...) {
        cerr << "[Permutation] Ошибка: неожиданное исключение\n";
        return CRYPTO_ERR_GENERAL;
    }
}

extern "C" __declspec(dllexport)
ConstBuffer generate_key(size_t length) {
    if (length == 0) { length = 8; }

    uint8_t* buf = new (nothrow) uint8_t[length];
    if (!buf) { return {nullptr, 0}; }

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(33, 126);

    for (size_t i = 0; i < length; i++) {
        buf[i] = static_cast<uint8_t>(dist(rng));
    }

    return {buf, length};
}