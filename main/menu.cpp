#include "menu.h"

#include <iostream>
#include <limits>

using namespace std;

int getMainMenuChoice()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "=========================\n";
        cout << "1. Шифрование текста\n";
        cout << "2. Дешифрование текста\n";
        cout << "3. Шифрование файла\n";
        cout << "4. Дешифрование файла\n";
        cout << "5. Генерация ключа\n";
        cout << "0. Выход\n";
        cout << "=========================\n";

        cout << "Введите пункт меню: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cerr << "Ошибка ввода. Повторите попытку.\n";

            continue;
        }

        if (choice >= 0 && choice <= 5)
        {
            return choice;
        }

        cerr << "Неверный пункт меню.\n";
    }
}

int getAlgorithmChoice(int count)
{
    int choice;

    while (true)
    {
        cout << "\nВведите номер алгоритма: ";

        cin >> choice;

        if (cin.fail())
        {
            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cerr << "Ошибка ввода. Повторите попытку.\n";

            continue;
        }

        if (choice >= 1 && choice <= count)
        {
            return choice;
        }

        cerr << "Неверный номер алгоритма.\n";
    }
}