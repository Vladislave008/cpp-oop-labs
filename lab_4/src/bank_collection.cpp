#include "bank_collection.h"

#include <limits>
#include <stdexcept>
#include <utility>

BankCollection::BankCollection() : _data(nullptr), _size(0), _capacity(0) {}

BankCollection::BankCollection(const BankCollection& other) : BankCollection() {
    BankCollection copy;
    copy.reserve(other._size);
    for (std::size_t i = 0; i < other._size; ++i) {
        copy.add(*other._data[i]);
    }
    swap(copy);
}

BankCollection::BankCollection(BankCollection&& other) noexcept : BankCollection() {
    swap(other);
}

BankCollection& BankCollection::operator=(BankCollection other) noexcept {
    swap(other);
    return *this;
}

BankCollection::~BankCollection() {
    clear();
    delete[] _data;
}

void BankCollection::swap(BankCollection& other) noexcept {
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}

void swap(BankCollection& lhs, BankCollection& rhs) noexcept {
    lhs.swap(rhs);
}

void BankCollection::reserve(std::size_t capacity) {
    if (capacity <= _capacity) {
        return;
    }
    BankProduct** data = new BankProduct*[capacity];
    for (std::size_t i = 0; i < _size; ++i) {
        data[i] = _data[i];
    }
    delete[] _data;
    _data = data;
    _capacity = capacity;
}

void BankCollection::add(const BankProduct& product) {
    if (_size == _capacity) {
        const auto max_capacity = std::numeric_limits<std::size_t>::max() / sizeof(BankProduct*);
        if (_capacity > max_capacity / 2) {
            throw std::length_error("Collection is too large");
        }
        reserve(_capacity == 0 ? 4 : _capacity * 2);
    }
    BankProduct* copy = product.clone();
    if (copy == nullptr) {
        throw std::runtime_error("clone() returned nullptr");
    }
    _data[_size] = copy;
    ++_size;
}

void BankCollection::erase(std::size_t index) {
    if (index >= _size) {
        throw std::out_of_range("Product index is out of range");
    }
    delete _data[index];
    for (std::size_t i = index + 1; i < _size; ++i) {
        _data[i - 1] = _data[i];
    }
    --_size;
    _data[_size] = nullptr;
}

void BankCollection::clear() noexcept {
    for (std::size_t i = 0; i < _size; ++i) {
        delete _data[i];
        _data[i] = nullptr;
    }
    _size = 0;
}

std::size_t BankCollection::size() const noexcept {
    return _size;
}

bool BankCollection::empty() const noexcept {
    return _size == 0;
}

BankProduct& BankCollection::operator[](std::size_t index) {
    if (index >= _size) {
        throw std::out_of_range("Product index is out of range");
    }
    return *_data[index];
}

const BankProduct& BankCollection::operator[](std::size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("Product index is out of range");
    }
    return *_data[index];
}

double BankCollection::total_metric() const {
    double total = 0;
    for (std::size_t i = 0; i < _size; ++i) {
        total += _data[i]->metric();
    }
    return total;
}

BankProduct* BankCollection::max_product() {
    return const_cast<BankProduct*>(std::as_const(*this).max_product());
}

const BankProduct* BankCollection::max_product() const {
    if (empty()) {
        return nullptr;
    }
    const BankProduct* best = _data[0];
    double maximum = best->metric();
    for (std::size_t i = 1; i < _size; ++i) {
        const double value = _data[i]->metric();
        if (value > maximum) {
            best = _data[i];
            maximum = value;
        }
    }
    return best;
}
