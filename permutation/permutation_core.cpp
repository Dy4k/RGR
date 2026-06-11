#include "permutation_core.h"
#include <numeric>
#include <algorithm>

using namespace std;

vector<size_t> buildPermutation(const uint8_t* key, size_t key_size) {
    vector<size_t> order(key_size);
    iota(order.begin(), order.end(), 0);

    sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        return key[a] < key[b];
    });

    return order;
}

vector<size_t> buildInversePermutation(const vector<size_t>& perm) {
    vector<size_t> inv(perm.size());
    for (size_t i = 0; i < perm.size(); i++) {
        inv[perm[i]] = i;
    }
    return inv;
}

void applyPermutation(const uint8_t* input, uint8_t* output,
                      const vector<size_t>& perm, size_t block_size) {
    for (size_t i = 0; i < block_size; i++) {
        output[i] = input[perm[i]];
    }
}