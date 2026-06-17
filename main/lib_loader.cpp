#include "lib_loader.h"

#include <iostream>


using namespace std;

bool loadLibraryFile(
    const string& path,
    LoadedLibrary& library
)
{
    try
    {
        HMODULE handle = LoadLibraryA(
            path.c_str()
        );


        if (handle == nullptr)
        {
            cerr << "Ошибка загрузки DLL: "
                 << path
                 << "\n";

            return false;
        }



        GetAlgorithmInfoFn getInfo =
            reinterpret_cast<GetAlgorithmInfoFn>(
                GetProcAddress(
                    handle,
                    "get_algorithm_info"
                )
            );


        library.encrypt =
            reinterpret_cast<EncryptFn>(
                GetProcAddress(
                    handle,
                    "encrypt"
                )
            );


        library.decrypt =
            reinterpret_cast<DecryptFn>(
                GetProcAddress(
                    handle,
                    "decrypt"
                )
            );


        library.generateKey =
            reinterpret_cast<GenerateKeyFn>(
                GetProcAddress(
                    handle,
                    "generate_key"
                )
            );


        library.getOutputSize =
            reinterpret_cast<GetOutputSizeFn>(
                GetProcAddress(
                    handle,
                    "get_output_size"
                )
            );



        if (getInfo == nullptr ||
            library.encrypt == nullptr ||
            library.decrypt == nullptr ||
            library.generateKey == nullptr ||
            library.getOutputSize == nullptr)
        {
            cerr << "Ошибка: DLL имеет неправильный интерфейс\n";


            FreeLibrary(handle);


            return false;
        }



        const AlgorithmInfo* info = getInfo();


        library.handle = handle;

        library.path = path;

        library.name = info->name;
        library.description = info->description;
        library.version = info->version;



        return true;
    }


    catch (exception& e)
    {
        cerr << "Ошибка загрузки библиотеки: "
             << e.what()
             << "\n";

        return false;
    }


    catch (...)
    {
        cerr << "Неизвестная ошибка загрузки DLL\n";

        return false;
    }
}



void unloadLibraries(
    vector<LoadedLibrary>& libraries
)
{
    for (size_t i = 0; i < libraries.size(); i++)
    {
        if (libraries[i].handle != nullptr)
        {
            FreeLibrary(
                libraries[i].handle
            );
        }
    }


    libraries.clear();
}



void scanLibraries(
    const string& folder,
    vector<LoadedLibrary>& libraries
)
{
    try
    {
        string searchPath = folder + "\\*.dll";


        WIN32_FIND_DATAA fileData;


        HANDLE searchHandle =
            FindFirstFileA(
                searchPath.c_str(),
                &fileData
            );


        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            cerr << "DLL не найдены в папке: "
                 << folder
                 << "\n";

            return;
        }



        do
        {
            string fullPath =
                folder + "\\" + fileData.cFileName;


            LoadedLibrary library;



            if (loadLibraryFile(
                    fullPath,
                    library))
            {
                libraries.push_back(
                    library
                );


                cout << "Загружен алгоритм: "
                     << library.name
                     << "\n";
            }


        }
        while (
            FindNextFileA(
                searchHandle,
                &fileData
            )
        );



        FindClose(
            searchHandle
        );
    }


    catch (exception& e)
    {
        cerr << "Ошибка поиска DLL: "
             << e.what()
             << "\n";
    }


    catch (...)
    {
        cerr << "Неизвестная ошибка поиска DLL\n";
    }
}