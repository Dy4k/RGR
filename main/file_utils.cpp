#include "file_utils.h"

#include <fstream>
#include <iostream>
#include <exception>

using namespace std;

bool fileExists(const string& path)
{
    ifstream file(path, ios::binary);

    return file.is_open();
}

bool createEmptyFile(const string& path)
{
    try
    {
        ofstream file(path, ios::binary);

        if (!file.is_open())
        {
            cerr << "Не удалось создать файл: "
                 << path << "\n";

            return false;
        }

        file.close();

        return true;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: "
             << e.what()
             << "\n";

        return false;
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка при создании файла.\n";

        return false;
    }
}

vector<uint8_t> readBinaryFile(const string& path)
{
    vector<uint8_t> data;

    try
    {
        ifstream file(path, ios::binary);

        if (!file.is_open())
        {
            cerr << "Файл не найден: "
                 << path
                 << "\n";

            return data;
        }

        file.seekg(0, ios::end);

        size_t file_size =
            static_cast<size_t>(file.tellg());

        file.seekg(0, ios::beg);

        data.resize(file_size);

        if (file_size > 0)
        {
            file.read(
                reinterpret_cast<char*>(data.data()),
                file_size
            );
        }

        file.close();
    }
    catch (const bad_alloc&)
    {
        cerr << "Недостаточно памяти для чтения файла.\n";
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: "
             << e.what()
             << "\n";
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка при чтении файла.\n";
    }

    return data;
}

bool writeBinaryFile(
    const string& path,
    const vector<uint8_t>& data
)
{
    try
    {
        ofstream file(path, ios::binary);

        if (!file.is_open())
        {
            cerr << "Не удалось открыть файл: "
                 << path
                 << "\n";

            return false;
        }

        if (!data.empty())
        {
            file.write(
                reinterpret_cast<const char*>(data.data()),
                data.size()
            );
        }

        file.close();

        return true;
    }
    catch (const exception& e)
    {
        cerr << "Ошибка: "
             << e.what()
             << "\n";

        return false;
    }
    catch (...)
    {
        cerr << "Неизвестная ошибка при записи файла.\n";

        return false;
    }
}

bool askToCreateFile(const string& path)
{
    char answer;

    while (true)
    {
        cout << "\nФайл не существует: "
             << path
             << "\n";

        cout << "Создать его? (y/n): ";

        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            return createEmptyFile(path);
        }

        if (answer == 'n' || answer == 'N')
        {
            return false;
        }

        cerr << "Введите y или n.\n";
    }
}