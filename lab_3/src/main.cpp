#include "exam_grade_set.h"

int main()
{
    ExamGradeSet a;
    a += ExamGrade(1, 3);
    a += ExamGrade(2, 4);
    a += ExamGrade(1, 5);

    ExamGradeSet b;
    b += ExamGrade(2, 5);
    b += ExamGrade(3, 4);

    std::cout << a << '\n';
    std::cout << b << '\n';
    std::cout << (a | b) << '\n';
    std::cout << (a & b) << '\n';
    std::cout << (a - b) << '\n';
    return 0;
}
