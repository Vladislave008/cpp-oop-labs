#pragma once

#include "exam_grade.h"

class ExamGradeSet {
private:
    ExamGrade* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

    std::size_t find(int student_id) const;
    void grow();
    void swap(ExamGradeSet& other) noexcept;

public:
    ExamGradeSet() = default;
    ExamGradeSet(const ExamGradeSet& other);
    ExamGradeSet(ExamGradeSet&& other) noexcept;
    ExamGradeSet& operator=(const ExamGradeSet& other);
    ExamGradeSet& operator=(ExamGradeSet&& other) noexcept;
    ~ExamGradeSet();

    bool add(const ExamGrade& value);
    bool remove(const ExamGrade& value);
    bool contains(const ExamGrade& value) const;
    bool operator()(const ExamGrade& value) const;

    std::size_t size() const;
    bool empty() const;

    ExamGradeSet& operator+=(const ExamGrade& value);
    ExamGradeSet& operator-=(const ExamGrade& value);

    ExamGradeSet operator|(const ExamGradeSet& other) const;
    ExamGradeSet operator&(const ExamGradeSet& other) const;
    ExamGradeSet operator-(const ExamGradeSet& other) const;

    bool operator==(const ExamGradeSet& other) const;
    bool operator!=(const ExamGradeSet& other) const;

    friend std::ostream& operator<<(std::ostream& os, const ExamGradeSet& values);
};
