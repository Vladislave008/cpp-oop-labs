#include "array_ops.h"
#include <iostream>
#include <limits>
#include <new>

namespace
{

    void clearInput()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    bool readLongLong(long long &n)
    {
        if (!(std::cin >> n))
        {
            clearInput();
            return false;
        }
        return true;
    }

    bool readInt(int &x)
    {
        if (!(std::cin >> x))
        {
            clearInput();
            return false;
        }
        return true;
    }

} // namespace

int main()
{
    int *arr = nullptr;
    std::size_t size = 0;
    bool running = true;

    while (running)
    {
        std::cout << "\n======== Меню ========\n"
                  << "1. Создать массив\n"
                  << "2. Напечатать\n"
                  << "3. Вставить элемент\n"
                  << "4. Удалить элемент\n"
                  << "5. Изменить размер\n"
                  << "6. Печать\n"
                  << "7. Алгоритм варианта (сортировка выбором + разворот)\n"
                  << "8. Бинарный поиск (с предварительной сортировкой выбором по копии)\n"
                  << "0. Выход\n"
                  << "Выбор: ";

        int choice;
        if (!(std::cin >> choice))
        {
            if (std::cin.eof())
            {
                std::cout << "\nКонец ввода\n";
                break;
            }
            clearInput();
            std::cout << "Ошибка ввода\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            long long rawN;
            std::cout << "Размер: ";
            if (!readLongLong(rawN))
            {
                std::cout << "Ошибка\n";
                break;
            }
            if (rawN < 0)
            {
                std::cout << "Размер не может быть отрицательным\n";
                break;
            }
            std::size_t n = static_cast<std::size_t>(rawN);

            int *fresh = nullptr;
            try
            {
                fresh = array_create(n);
            }
            catch (const std::bad_alloc &)
            {
                std::cout << "Недостаточно памяти: не удалось выделить массив из " << n << " элементов\n";
                break;
            }
            array_delete(arr);
            arr = fresh;
            size = n;
            std::cout << "Введите " << n << " элементов:\n";
            for (std::size_t i = 0; i < n; ++i)
            {
                if (!readInt(arr[i]))
                {
                    std::cout << "Ошибка ввода\n";
                    break;
                }
            }
            std::cout << "Создан массив (размер " << size << "): ";
            array_print(arr, size);
            break;
        }
        case 2:
        case 6:
            array_print(arr, size);
            break;
        case 3:
        {
            long long rawPos;
            int value;
            std::cout << "Позиция: ";
            if (!readLongLong(rawPos))
            {
                std::cout << "Ошибка ввода\n";
                break;
            }
            if (rawPos < 0)
            {
                std::cout << "Позиция не может быть отрицательной\n";
                break;
            }
            std::size_t pos = static_cast<std::size_t>(rawPos);

            std::cout << "Значение: ";
            if (!readInt(value))
            {
                std::cout << "Ошибка ввода\n";
                break;
            }

            std::size_t before = size;
            int *fresh = nullptr;
            try
            {
                fresh = array_insert(arr, size, pos, value);
            }
            catch (const std::bad_alloc &)
            {
                std::cout << "Недостаточно памяти: вставка не выполнена\n";
                break;
            }
            arr = fresh;
            std::size_t actual = (pos < before) ? pos : before;
            if (pos > before)
            {
                std::cout << "Позиция " << pos << " больше длины " << before << " - вставлено в конец\n";
            }
            std::cout << "Вставлено " << value << " на позицию " << actual << ", новый размер " << size << ": ";
            array_print(arr, size);
            break;
        }
        case 4:
        {
            long long rawPos;
            std::cout << "Позиция: ";
            if (!readLongLong(rawPos))
            {
                std::cout << "Ошибка\n";
                break;
            }
            if (rawPos < 0)
            {
                std::cout << "Позиция не может быть отрицательной\n";
                break;
            }
            std::size_t pos = static_cast<std::size_t>(rawPos);

            if (size == 0)
            {
                std::cout << "Массив пуст, удалять нечего\n";
                break;
            }
            if (pos >= size)
            {
                std::cout << "Позиция " << pos << " вне диапазона [0, " << size - 1 << "], массив не изменён\n";
                break;
            }

            int removed = arr[pos];
            int *fresh = nullptr;
            try
            {
                fresh = array_remove(arr, size, pos);
            }
            catch (const std::bad_alloc &)
            {
                std::cout << "Недостаточно памяти: удаление не выполнено\n";
                break;
            }
            arr = fresh;
            std::cout << "Удалён элемент " << removed << " с позиции " << pos << ", новый размер " << size << ": ";
            array_print(arr, size);
            break;
        }
        case 5:
        {
            long long rawNew;
            std::cout << "Новый размер: ";
            if (!readLongLong(rawNew))
            {
                std::cout << "Ошибка\n";
                break;
            }
            if (rawNew < 0)
            {
                std::cout << "Размер не может быть отрицательным\n";
                break;
            }
            std::size_t newSize = static_cast<std::size_t>(rawNew);

            int *fresh = nullptr;
            try
            {
                fresh = array_resize(arr, size, newSize);
            }
            catch (const std::bad_alloc &)
            {
                std::cout << "Недостаточно памяти: массив не изменён\n";
                break;
            }
            arr = fresh;
            size = newSize;
            std::cout << "После изменения размера (размер " << size << "): ";
            array_print(arr, size);
            break;
        }
        case 7:
        {
            array_selection_sort(arr, size);
            std::cout << "После сортировки выбором: ";
            array_print(arr, size);

            array_reverse(arr, size);
            std::cout << "После разворота: ";
            array_print(arr, size);
            break;
        }
        case 8:
        {
            int target;
            std::cout << "Число для поиска: ";
            if (!readInt(target))
            {
                break;
            }

            int *copy = nullptr;
            try
            {
                copy = array_create(size);
            }
            catch (const std::bad_alloc &)
            {
                std::cout << "Недостаточно памяти: поиск не выполнен\n";
                break;
            }
            for (std::size_t i = 0; i < size; ++i)
            {
                copy[i] = arr[i];
            }
            array_selection_sort(copy, size);
            std::cout << "Отсортированный массив: ";
            array_print(copy, size);

            std::size_t index = 0;
            if (array_binary_search(copy, size, target, index))
            {
                std::cout << "Найдено на позиции " << index << '\n';
            }
            else
            {
                std::cout << "Не найдено\n";
            }
            array_delete(copy);
            break;
        }
        case 0:
            std::cout << "Выход\n";
            running = false;
            break;
        default:
            std::cout << "Нет такого пункта\n";
        }
    }

    array_delete(arr);
    return 0;
}