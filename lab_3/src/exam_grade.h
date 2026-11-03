#pragma once

#include <iostream>

class ExamGrade {
public:
    int student_id = 0;
    int grade = 0;

    ExamGrade() = default;
    ExamGrade(int student_id, int grade);

    bool operator==(const ExamGrade& other) const;

    friend std::ostream& operator<<(std::ostream& os, const ExamGrade& value);
};
