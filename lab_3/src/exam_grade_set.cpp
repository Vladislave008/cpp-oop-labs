#include "exam_grade_set.h"

#include <utility>

std::size_t ExamGradeSet::find(int student_id) const
{
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i].student_id == student_id) {
            return i;
        }
    }
    return size_;
}

void ExamGradeSet::grow()
{
    const std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
    ExamGrade* new_data = new ExamGrade[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

void ExamGradeSet::swap(ExamGradeSet& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

ExamGradeSet::ExamGradeSet(const ExamGradeSet& other): data_(other.capacity_ == 0 ? nullptr : new ExamGrade[other.capacity_]),
    size_(other.size_), capacity_(other.capacity_)
{
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

ExamGradeSet::ExamGradeSet(ExamGradeSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

ExamGradeSet& ExamGradeSet::operator=(const ExamGradeSet& other)
{
    if (this != &other) {
        ExamGradeSet copy(other);
        swap(copy);
    }
    return *this;
}

ExamGradeSet& ExamGradeSet::operator=(ExamGradeSet&& other) noexcept
{
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

ExamGradeSet::~ExamGradeSet()
{
    delete[] data_;
}

bool ExamGradeSet::add(const ExamGrade& value)
{
    const std::size_t index = find(value.student_id);
    if (index != size_) {
        if (data_[index] == value) {
            return false;
        }
        data_[index] = value;
        return true;
    }
    if (size_ == capacity_) {
        grow();
    }
    data_[size_++] = value;
    return true;
}

bool ExamGradeSet::remove(const ExamGrade& value)
{
    const std::size_t index = find(value.student_id);
    if (index == size_) {
        return false;
    }
    for (std::size_t i = index + 1; i < size_; ++i) {
        data_[i - 1] = data_[i];
    }
    --size_;
    return true;
}

bool ExamGradeSet::contains(const ExamGrade& value) const
{
    return find(value.student_id) != size_;
}

bool ExamGradeSet::operator()(const ExamGrade& value) const
{
    return contains(value);
}

std::size_t ExamGradeSet::size() const
{
    return size_;
}

bool ExamGradeSet::empty() const
{
    return size_ == 0;
}

ExamGradeSet& ExamGradeSet::operator+=(const ExamGrade& value)
{
    add(value);
    return *this;
}

ExamGradeSet& ExamGradeSet::operator-=(const ExamGrade& value)
{
    remove(value);
    return *this;
}

ExamGradeSet ExamGradeSet::operator|(const ExamGradeSet& other) const
{
    ExamGradeSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.add(other.data_[i]);
    }
    return result;
}

ExamGradeSet ExamGradeSet::operator&(const ExamGradeSet& other) const
{
    ExamGradeSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

ExamGradeSet ExamGradeSet::operator-(const ExamGradeSet& other) const
{
    ExamGradeSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.contains(data_[i])) {
            result.add(data_[i]);
        }
    }
    return result;
}

bool ExamGradeSet::operator==(const ExamGradeSet& other) const
{
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        const std::size_t index = other.find(data_[i].student_id);
        if (index == other.size_ || !(data_[i] == other.data_[index])) {
            return false;
        }
    }
    return true;
}

bool ExamGradeSet::operator!=(const ExamGradeSet& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const ExamGradeSet& values)
{
    os << "{ ";
    for (std::size_t i = 0; i < values.size_; ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << values.data_[i];
    }
    if (!values.empty()) {
        os << ' ';
    }
    return os << '}';
}
