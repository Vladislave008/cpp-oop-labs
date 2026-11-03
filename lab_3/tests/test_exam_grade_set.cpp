#include <gtest/gtest.h>

#include <sstream>
#include <utility>

#include "exam_grade_set.h"

TEST(ExamGrade, DefaultConstruction)
{
    ExamGrade value;
    EXPECT_EQ(value.student_id, 0);
    EXPECT_EQ(value.grade, 0);
}

TEST(ExamGrade, EqualityIncludesGrade)
{
    EXPECT_EQ(ExamGrade(1, 5), ExamGrade(1, 5));
    EXPECT_FALSE(ExamGrade(1, 5) == ExamGrade(1, 4));
    EXPECT_FALSE(ExamGrade(1, 5) == ExamGrade(2, 5));
}

TEST(ExamGrade, Output)
{
    std::ostringstream os;
    os << ExamGrade(12, 5);
    EXPECT_EQ(os.str(), "(12, 5)");
}

TEST(ExamGradeSet, Empty)
{
    const ExamGradeSet values;
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.size(), 0u);
    EXPECT_FALSE(values.contains(ExamGrade(1, 5)));
    EXPECT_FALSE(values(ExamGrade(1, 5)));
}

TEST(ExamGradeSet, AddAndUpsert)
{
    ExamGradeSet values;
    EXPECT_TRUE(values.add(ExamGrade(1, 3)));
    EXPECT_FALSE(values.add(ExamGrade(1, 3)));
    EXPECT_TRUE(values.add(ExamGrade(1, 5)));
    EXPECT_FALSE(values.add(ExamGrade(1, 5)));
    EXPECT_EQ(values.size(), 1u);
    EXPECT_FALSE(values.empty());

    std::ostringstream os;
    os << values;
    EXPECT_EQ(os.str(), "{ (1, 5) }");
}

TEST(ExamGradeSet, MembershipUsesId)
{
    ExamGradeSet values;
    values.add(ExamGrade(1, 5));
    EXPECT_TRUE(values.contains(ExamGrade(1, 2)));
    EXPECT_TRUE(values(ExamGrade(1, 2)));
    EXPECT_FALSE(values.contains(ExamGrade(2, 5)));
    EXPECT_FALSE(values(ExamGrade(2, 5)));
}

TEST(ExamGradeSet, RemoveUsesIdAndPreservesOrder)
{
    ExamGradeSet values;
    EXPECT_FALSE(values.remove(ExamGrade(1, 5)));
    values += ExamGrade(1, 3);
    values += ExamGrade(2, 4);
    values += ExamGrade(3, 5);
    EXPECT_TRUE(values.remove(ExamGrade(2, 2)));
    EXPECT_FALSE(values.remove(ExamGrade(2, 4)));
    EXPECT_EQ(values.size(), 2u);

    std::ostringstream os;
    os << values;
    EXPECT_EQ(os.str(), "{ (1, 3), (3, 5) }");
    EXPECT_TRUE(values.remove(ExamGrade(1, 3)));
    EXPECT_TRUE(values.remove(ExamGrade(3, 5)));
    EXPECT_TRUE(values.empty());
    EXPECT_TRUE(values.add(ExamGrade(4, 5)));
}

TEST(ExamGradeSet, CompoundOperatorsReturnSelf)
{
    ExamGradeSet values;
    EXPECT_EQ(&(values += ExamGrade(1, 3)), &values);
    (values += ExamGrade(2, 4)) += ExamGrade(1, 5);
    EXPECT_EQ(values.size(), 2u);
    EXPECT_EQ(&(values -= ExamGrade(1, 2)), &values);
    (values -= ExamGrade(2, 5)) -= ExamGrade(3, 4);
    EXPECT_TRUE(values.empty());
}

TEST(ExamGradeSet, CopyConstructorIsIndependent)
{
    ExamGradeSet original;
    original += ExamGrade(1, 3);
    original += ExamGrade(2, 4);
    ExamGradeSet copy(original);
    EXPECT_EQ(copy, original);
    copy += ExamGrade(1, 5);
    copy -= ExamGrade(2, 4);
    copy += ExamGrade(3, 5);

    std::ostringstream os;
    os << original;
    EXPECT_EQ(os.str(), "{ (1, 3), (2, 4) }");
    EXPECT_NE(copy, original);
}

TEST(ExamGradeSet, CopyAssignmentIsIndependent)
{
    ExamGradeSet original;
    original += ExamGrade(1, 3);
    ExamGradeSet copy;
    copy += ExamGrade(2, 5);
    EXPECT_EQ(&(copy = original), &copy);
    EXPECT_EQ(copy, original);
    original += ExamGrade(1, 5);
    original += ExamGrade(3, 4);

    std::ostringstream os;
    os << copy;
    EXPECT_EQ(os.str(), "{ (1, 3) }");
}

TEST(ExamGradeSet, SelfAssignment)
{
    ExamGradeSet values;
    values += ExamGrade(1, 5);
    const ExamGradeSet expected(values);
    ExamGradeSet& alias = values;
    values = alias;
    EXPECT_EQ(values, expected);
    values = std::move(alias);
    EXPECT_EQ(values, expected);
}

TEST(ExamGradeSet, MoveConstructorEmptiesSource)
{
    ExamGradeSet source;
    source += ExamGrade(1, 5);
    const ExamGradeSet expected(source);
    ExamGradeSet result(std::move(source));
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_FALSE(source.contains(ExamGrade(1, 5)));
    EXPECT_TRUE(source.add(ExamGrade(2, 4)));
    EXPECT_FALSE(result.contains(ExamGrade(2, 4)));
}

TEST(ExamGradeSet, MoveAssignmentReplacesDestination)
{
    ExamGradeSet source;
    source += ExamGrade(1, 5);
    const ExamGradeSet expected(source);
    ExamGradeSet result;
    result += ExamGrade(2, 4);
    EXPECT_EQ(&(result = std::move(source)), &result);
    EXPECT_EQ(result, expected);
    EXPECT_TRUE(source.empty());
    EXPECT_FALSE(source.remove(ExamGrade(1, 5)));
    EXPECT_TRUE(source.add(ExamGrade(3, 3)));
}

TEST(ExamGradeSet, CopyAndMoveEmpty)
{
    ExamGradeSet empty;
    ExamGradeSet copy(empty);
    ExamGradeSet moved(std::move(empty));
    EXPECT_TRUE(copy.empty());
    EXPECT_TRUE(moved.empty());
    copy += ExamGrade(1, 5);
    copy = empty;
    EXPECT_TRUE(copy.empty());
    moved += ExamGrade(2, 4);
    moved = std::move(empty);
    EXPECT_TRUE(moved.empty());
    EXPECT_TRUE(empty.empty());
    EXPECT_TRUE(empty.add(ExamGrade(3, 5)));
}

TEST(ExamGradeSet, EqualityIgnoresOrderButIncludesGrades)
{
    ExamGradeSet a;
    a += ExamGrade(1, 5);
    a += ExamGrade(2, 4);
    ExamGradeSet b;
    b += ExamGrade(2, 4);
    b += ExamGrade(1, 5);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    b += ExamGrade(1, 3);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    b -= ExamGrade(1, 3);
    EXPECT_NE(a, b);
    b += ExamGrade(3, 5);
    EXPECT_NE(a, b);
    EXPECT_EQ(ExamGradeSet(), ExamGradeSet());
}

TEST(ExamGradeSet, SetOperationsUseId)
{
    ExamGradeSet a;
    a += ExamGrade(1, 3);
    a += ExamGrade(2, 4);
    ExamGradeSet b;
    b += ExamGrade(2, 5);
    b += ExamGrade(3, 4);
    const ExamGradeSet original_a(a);
    const ExamGradeSet original_b(b);

    ExamGradeSet united;
    united += ExamGrade(1, 3);
    united += ExamGrade(2, 5);
    united += ExamGrade(3, 4);
    EXPECT_EQ(a | b, united);

    ExamGradeSet intersected;
    intersected += ExamGrade(2, 4);
    EXPECT_EQ(a & b, intersected);

    ExamGradeSet difference;
    difference += ExamGrade(1, 3);
    EXPECT_EQ(a - b, difference);
    EXPECT_EQ(a, original_a);
    EXPECT_EQ(b, original_b);
}

TEST(ExamGradeSet, SetOperationsWithEmptyAndSelf)
{
    const ExamGradeSet empty;
    ExamGradeSet values;
    values += ExamGrade(1, 5);
    EXPECT_EQ(values | empty, values);
    EXPECT_EQ(empty | values, values);
    EXPECT_EQ(values & empty, empty);
    EXPECT_EQ(empty & values, empty);
    EXPECT_EQ(values - empty, values);
    EXPECT_EQ(empty - values, empty);
    EXPECT_EQ(values | values, values);
    EXPECT_EQ(values & values, values);
    EXPECT_EQ(values - values, empty);
    EXPECT_EQ(empty | empty, empty);
    EXPECT_EQ(empty & empty, empty);
    EXPECT_EQ(empty - empty, empty);
}

TEST(ExamGradeSet, OutputPreservesInsertionOrderAfterUpsert)
{
    ExamGradeSet values;
    std::ostringstream empty_os;
    empty_os << values;
    EXPECT_EQ(empty_os.str(), "{ }");
    values += ExamGrade(3, 3);
    values += ExamGrade(1, 4);
    values += ExamGrade(3, 5);
    std::ostringstream os;
    EXPECT_EQ(&(os << values), &os);
    EXPECT_EQ(os.str(), "{ (3, 5), (1, 4) }");
}

TEST(ExamGradeSet, Stress)
{
    ExamGradeSet values;
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(values.add(ExamGrade(i, 3)));
    }
    EXPECT_EQ(values.size(), 1000u);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(values.contains(ExamGrade(i, 5)));
        EXPECT_TRUE(values(ExamGrade(i, 5)));
        EXPECT_FALSE(values.add(ExamGrade(i, 3)));
        EXPECT_TRUE(values.add(ExamGrade(i, 5)));
        EXPECT_FALSE(values.add(ExamGrade(i, 5)));
    }
    EXPECT_EQ(values.size(), 1000u);
    const ExamGradeSet copy(values);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(values.remove(ExamGrade(i, 2)));
        EXPECT_FALSE(values.contains(ExamGrade(i, 5)));
    }
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(copy.size(), 1000u);
    EXPECT_TRUE(values.add(ExamGrade(1000, 4)));
}
