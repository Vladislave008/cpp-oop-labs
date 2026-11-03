#include "exam_grade.h"

ExamGrade::ExamGrade(int student_id, int grade)
    : student_id(student_id), grade(grade)
{
}

bool ExamGrade::operator==(const ExamGrade& other) const
{
    return student_id == other.student_id && grade == other.grade;
}

std::ostream& operator<<(std::ostream& os, const ExamGrade& value)
{
    return os << '(' << value.student_id << ", " << value.grade << ')';
}
