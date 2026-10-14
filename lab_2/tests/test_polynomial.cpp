#include <gtest/gtest.h>

#include <cmath>
#include <sstream>
#include <utility>

#include "polynomial.h"

namespace
{
    std::string print_str(const Polynomial &p)
    {
        std::ostringstream os;
        p.print(os);
        return os.str();
    }
}

TEST(Polynomial, ConstructAndEvaluate)
{
    double c[] = {1.0, 2.0}; // 1 + 2x
    Polynomial p(c, 1);

    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0);
    EXPECT_DOUBLE_EQ(p.evaluate(-1.0), -1.0);
    EXPECT_EQ(print_str(p), "2x + 1\n");
}

TEST(Polynomial, ConstructorTrimsLeadingZeros)
{
    double c[] = {3.0, 2.0, 0.0, 0.0}; // 3 + 2x
    Polynomial p(c, 3);

    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 5.0);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 7.0);
    EXPECT_EQ(print_str(p), "2x + 3\n");
}

TEST(Polynomial, AllZeroCoefficientsIsEmpty)
{
    double c[] = {0.0, 0.0, 0.0};
    Polynomial p(c, 2);

    EXPECT_DOUBLE_EQ(p.evaluate(10.0), 0.0);
    EXPECT_EQ(print_str(p), "[empty polynomial]\n");
}

TEST(Polynomial, NullDataIsEmpty)
{
    Polynomial p(nullptr, 5);

    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0);
    EXPECT_EQ(print_str(p), "[empty polynomial]\n");
}

TEST(Polynomial, EmptyPolynomialEvaluatesToZero)
{
    double c[] = {0.0};
    Polynomial p(c, 0);

    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 0.0);
    EXPECT_DOUBLE_EQ(p.evaluate(-7.5), 0.0);
    EXPECT_EQ(print_str(p), "[empty polynomial]\n");
}

TEST(Polynomial, CopyIsDeep)
{
    double c[] = {1.0, 2.0};
    Polynomial a(c, 1);
    Polynomial b(a);

    double ten[] = {10.0};
    a.add(Polynomial(ten, 0));

    EXPECT_DOUBLE_EQ(a.evaluate(0.0), 11.0);
    EXPECT_DOUBLE_EQ(b.evaluate(0.0), 1.0);
    EXPECT_EQ(print_str(b), "2x + 1\n");
}

TEST(Polynomial, MoveEmptiesSource)
{
    double c[] = {1.0, 2.0};
    Polynomial src(c, 1);
    Polynomial dst(std::move(src));

    EXPECT_DOUBLE_EQ(dst.evaluate(2.0), 5.0);
    EXPECT_EQ(print_str(dst), "2x + 1\n");

    EXPECT_DOUBLE_EQ(src.evaluate(100.0), 0.0);
    EXPECT_EQ(print_str(src), "[empty polynomial]\n");
}

TEST(Polynomial, CopyAssignment)
{
    double c1[] = {1.0, 2.0};
    double c2[] = {0.0, 0.0, 1.0}; // x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);

    a = b;
    EXPECT_EQ(print_str(a), "x^2 \n");
    EXPECT_DOUBLE_EQ(a.evaluate(3.0), 9.0);

    double five[] = {5.0};
    b.add(Polynomial(five, 0));
    EXPECT_DOUBLE_EQ(a.evaluate(2.0), 4.0);
}

TEST(Polynomial, SelfAssignmentIsSafe)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);

    p = p;
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0);
    EXPECT_EQ(print_str(p), "2x + 1\n");
}

TEST(Polynomial, SelfMoveAssignmentIsSafe)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);
    p = std::move(p);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0);
    EXPECT_EQ(print_str(p), "2x + 1\n");
}

TEST(Polynomial, AddBasic)
{
    double c1[] = {1.0, 2.0};      // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0}; // x + x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);

    Polynomial s(a);
    s.add(b); // 1 + 3x + x^2

    EXPECT_EQ(print_str(s), "x^2 + 3x + 1\n");
    EXPECT_DOUBLE_EQ(s.evaluate(2.0), 11.0);
}

TEST(Polynomial, AddWithEmptyKeepsOperands)
{
    double c[] = {1.0, 2.0};
    double z[] = {0.0};
    Polynomial p(c, 1);
    Polynomial empty(z, 0);

    Polynomial a(p);
    a.add(empty);
    EXPECT_EQ(print_str(a), "2x + 1\n");
    EXPECT_DOUBLE_EQ(a.evaluate(2.0), 5.0);

    Polynomial b(empty);
    b.add(p);
    EXPECT_EQ(print_str(b), "2x + 1\n");
    EXPECT_DOUBLE_EQ(b.evaluate(2.0), 5.0);

    Polynomial e1(empty);
    Polynomial e2(empty);
    e1.add(e2);
    EXPECT_EQ(print_str(e1), "[empty polynomial]\n");
}

TEST(Polynomial, AddCancellationLowersDegree)
{
    double c1[] = {1.0, 1.0};  // 1 + x
    double c2[] = {0.0, -1.0}; // -x
    Polynomial a(c1, 1);
    Polynomial b(c2, 1);

    a.add(b); // 1
    EXPECT_EQ(print_str(a), "1\n");
    EXPECT_DOUBLE_EQ(a.evaluate(7.0), 1.0);
}

TEST(Polynomial, AddCancelsToEmpty)
{
    double c1[] = {1.0, 1.0};   // 1 + x
    double c2[] = {-1.0, -1.0}; // -1 - x
    Polynomial a(c1, 1);
    Polynomial b(c2, 1);

    a.add(b);
    EXPECT_EQ(print_str(a), "[empty polynomial]\n");
    EXPECT_DOUBLE_EQ(a.evaluate(3.0), 0.0);
}

TEST(Polynomial, MulBasic)
{
    double c1[] = {1.0, 2.0};      // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0}; // x + x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);

    Polynomial m(a);
    m.mul(b); // x + 3x^2 + 2x^3

    EXPECT_EQ(print_str(m), "2x^3 + 3x^2 + x \n");
    EXPECT_DOUBLE_EQ(m.evaluate(2.0), 30.0); // 2 + 12 + 16
    EXPECT_DOUBLE_EQ(m.evaluate(0.0), 0.0);
}

TEST(Polynomial, MulByConstant)
{
    double c1[] = {1.0, 2.0};
    double c2[] = {3.0};
    Polynomial a(c1, 1);
    Polynomial b(c2, 0);

    a.mul(b); // 3 + 6x
    EXPECT_EQ(print_str(a), "6x + 3\n");
    EXPECT_DOUBLE_EQ(a.evaluate(1.0), 9.0);
}

TEST(Polynomial, MulByEmptyGivesEmpty)
{
    double c[] = {1.0, 2.0};
    double z[] = {0.0};
    Polynomial p(c, 1);
    Polynomial empty(z, 0);

    Polynomial a(p);
    a.mul(empty);
    EXPECT_EQ(print_str(a), "[empty polynomial]\n");

    Polynomial b(empty);
    b.mul(p);
    EXPECT_EQ(print_str(b), "[empty polynomial]\n");
}

TEST(Polynomial, MulTwoCopiesOfEmpty)
{
    double z[] = {0.0};
    Polynomial empty(z, 0);
    Polynomial b(empty);
    Polynomial c(empty);

    EXPECT_NO_THROW(b.mul(c));
    EXPECT_EQ(print_str(b), "[empty polynomial]\n");
    EXPECT_DOUBLE_EQ(b.evaluate(1.0), 0.0);
}

TEST(Polynomial, PrintConstantOne)
{
    double c[] = {1.0};
    Polynomial p(c, 0);
    EXPECT_EQ(print_str(p), "1\n");
}

TEST(Polynomial, PrintNegativeLeadingCoefficient)
{
    double c[] = {-2.0, 0.0, 1.0}; // -2 + x^2
    Polynomial p(c, 2);

    EXPECT_EQ(print_str(p), "x^2 - 2\n");
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), -2.0);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 2.0);
}

TEST(Polynomial, PrintUnitCoefficients)
{
    double c[] = {1.0, -1.0}; // 1 - x
    Polynomial p(c, 1);

    EXPECT_EQ(print_str(p), "- x + 1\n");
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0);
}

TEST(EvaluateMany, ExampleFromVariant)
{
    double c[] = {0.0, 0.0, 1.0}; // x^2
    Polynomial p(c, 2);

    double xs[] = {-1.0, 0.0, 1.0, 2.0};
    double ys[4] = {};
    p.evaluate_many(xs, ys, 4);

    EXPECT_DOUBLE_EQ(ys[0], 1.0);
    EXPECT_DOUBLE_EQ(ys[1], 0.0);
    EXPECT_DOUBLE_EQ(ys[2], 1.0);
    EXPECT_DOUBLE_EQ(ys[3], 4.0);
}

TEST(EvaluateMany, MatchesEvaluate)
{
    double c[] = {1.0, -2.0, 0.5, 3.0};
    Polynomial p(c, 3);

    double xs[] = {-2.0, -0.5, 0.0, 0.25, 1.0, 3.7};
    double ys[6] = {};
    p.evaluate_many(xs, ys, 6);

    for (size_t i = 0; i < 6; ++i)
    {
        EXPECT_NEAR(ys[i], p.evaluate(xs[i]), 1e-9) << "i = " << i;
    }
}

TEST(EvaluateMany, ZeroCountLeavesBufferUntouched)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);

    double xs[] = {1.0, 2.0, 3.0};
    double ys[] = {42.0, 42.0, 42.0};
    p.evaluate_many(xs, ys, 0);

    EXPECT_DOUBLE_EQ(ys[0], 42.0);
    EXPECT_DOUBLE_EQ(ys[1], 42.0);
    EXPECT_DOUBLE_EQ(ys[2], 42.0);
}

TEST(EvaluateMany, EmptyPolynomialGivesZeros)
{
    double z[] = {0.0};
    Polynomial p(z, 0);

    double xs[] = {-5.0, 0.0, 5.0};
    double ys[3] = {1.0, 1.0, 1.0};
    p.evaluate_many(xs, ys, 3);

    EXPECT_DOUBLE_EQ(ys[0], 0.0);
    EXPECT_DOUBLE_EQ(ys[1], 0.0);
    EXPECT_DOUBLE_EQ(ys[2], 0.0);
}

TEST(EvaluateMany, SinglePoint)
{
    double c[] = {1.0, 2.0};
    Polynomial p(c, 1);

    double xs[] = {2.0};
    double ys[] = {0.0};
    p.evaluate_many(xs, ys, 1);

    EXPECT_DOUBLE_EQ(ys[0], 5.0);
}

TEST(EvaluateMany, FractionalAndNegativePoints)
{
    double c[] = {1.0, 2.0}; // 1 + 2x
    Polynomial p(c, 1);

    double xs[] = {-0.5, 0.25, 1.5};
    double ys[3] = {};
    p.evaluate_many(xs, ys, 3);

    EXPECT_DOUBLE_EQ(ys[0], 0.0);
    EXPECT_DOUBLE_EQ(ys[1], 1.5);
    EXPECT_DOUBLE_EQ(ys[2], 4.0);
}
