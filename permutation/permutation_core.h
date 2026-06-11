#ifndef PERMUTATION_CORE_H
#define PERMUTATION_CORE_H

#include <cstdint>
#include <cstddef>
#include <vector>


std::vector<size_t> buildPermutation(const uint8_t* key, size_t key_size);


std::vector<size_t> buildInversePermutation(const std::vector<size_t>& perm);


void applyPermutation(const uint8_t* input, uint8_t* output, const std::vector<size_t>& perm, size_t block_size);

#endif 