#ifndef LIB_LOADER_H
#define LIB_LOADER_H

#include "../include/crypto_api.h"

#include <windows.h>

#include <string>
#include <vector>


struct LoadedLibrary
{
    HMODULE handle;

    std::string path;

    std::string name;
    std::string description;
    std::string version;

    EncryptFn encrypt;
    DecryptFn decrypt;
    GenerateKeyFn generateKey;
    GetOutputSizeFn getOutputSize;
};



bool loadLibraryFile(
    const std::string& path,
    LoadedLibrary& library
);


void unloadLibraries(
    std::vector<LoadedLibrary>& libraries
);


void scanLibraries(
    const std::string& folder,
    std::vector<LoadedLibrary>& libraries
);


#endif