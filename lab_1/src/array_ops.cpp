#include "array_ops.h"
#include <iostream>

int* array_create(std::size_t size) {
    if (size == 0) return nullptr;
    return new int[size]{};
}

void array_delete(int*& arr) {
    delete[] arr;
    arr = nullptr;
}

int* array_resize(int* arr, std::size_t size, std::size_t new_size) {
    int* fresh = (new_size == 0) ? nullptr : new int[new_size]{};
    std::size_t to_copy = (size < new_size) ? size : new_size;
    for (std::size_t i = 0; i < to_copy; ++i)
        fresh[i] = arr[i];
    delete[] arr;
    return fresh;
}

int* array_insert(int* arr, std::size_t& size, std::size_t pos, int value) {
    if (pos > size) pos = size;
    int* fresh = new int[size + 1];
    for (std::size_t i = 0; i < pos; ++i) fresh[i] = arr[i];
    fresh[pos] = value;
    for (std::size_t i = pos; i < size; ++i) fresh[i + 1] = arr[i];
    delete[] arr;
    ++size;
    return fresh;
}

int* array_remove(int* arr, std::size_t& size, std::size_t pos) {
    if (pos >= size || size == 0) return arr;
    int* fresh = (size - 1 == 0) ? nullptr : new int[size - 1];
    for (std::size_t i = 0, j = 0; i < size; ++i) {
        if (i == pos) continue;
        fresh[j++] = arr[i];
    }
    delete[] arr;
    --size;
    return fresh;
}

void array_print(const int* arr, std::size_t size) {
    if (size == 0 || arr == nullptr) {
        std::cout << "[]\n";
        return;
    }
    std::cout << "[ ";
    for (std::size_t i = 0; i < size; ++i)
        std::cout << arr[i] << (i + 1 < size ? ", " : " ");
    std::cout << "]\n";
}

void array_selection_sort(int* arr, std::size_t size) {
    if (size < 2 || arr == nullptr) {
        return;
    }
    for (std::size_t i = 0; i + 1 < size; ++i) {
        std::size_t minInd = i;
        for (std::size_t j = i + 1; j < size; ++j) {
            if (arr[j] < arr[minInd]) minInd = j;
        }
        if (minInd != i) {
            int tmp = arr[i];
            arr[i] = arr[minInd];
            arr[minInd] = tmp;
        }
    }
}

void array_reverse(int* arr, std::size_t size) {
    if (arr == nullptr || size < 2) return;
    std::size_t left = 0;
    std::size_t right = size - 1;
    while (left < right) {
        int tmp = arr[left];
        arr[left] = arr[right];
        arr[right] = tmp;
        ++left;
        --right;
    }
}

bool array_binary_search(const int* arr, std::size_t size, int target, std::size_t& out_index) {
    if (arr == nullptr || size == 0) {
        return false;
    }

    std::size_t left = 0;
    std::size_t right = size;

    while (left < right) {
        std::size_t mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            out_index = mid;
            return true;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return false;
}