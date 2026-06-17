#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <cstdint>

bool fileExists(const std::string& path);

bool createEmptyFile(const std::string& path);

std::vector<uint8_t> readBinaryFile(
    const std::string& path
);

bool writeBinaryFile(
    const std::string& path,
    const std::vector<uint8_t>& data
);

bool askToCreateFile(const std::string& path);

#endif