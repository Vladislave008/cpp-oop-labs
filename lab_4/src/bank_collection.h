#pragma once

#include "bank.h"

#include <cstddef>

class BankCollection {
private:
    BankProduct** _data;
    std::size_t _size;
    std::size_t _capacity;

    void reserve(std::size_t capacity);

public:
    BankCollection();
    BankCollection(const BankCollection& other);
    BankCollection(BankCollection&& other) noexcept;
    BankCollection& operator=(BankCollection other) noexcept;
    ~BankCollection();

    void swap(BankCollection& other) noexcept;

    void add(const BankProduct& product);
    void erase(std::size_t index);
    void clear() noexcept;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
    BankProduct& operator[](std::size_t index);
    const BankProduct& operator[](std::size_t index) const;

    double total_metric() const;
    BankProduct* max_product();
    const BankProduct* max_product() const;
};

void swap(BankCollection& lhs, BankCollection& rhs) noexcept;
