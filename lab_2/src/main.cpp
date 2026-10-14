#include "polynomial.h"

int main() {
double c1[] = {1.0, 2.0}; // 1 + 2x
    double c2[] = {0.0, 1.0, 1.0}; // x + x^2
    Polynomial a(c1, 1);
    Polynomial b(c2, 2);
    Polynomial s(a);
    s.add(b); // 1 + 3x + x^2
    s.print(std::cout);
    std::cout << s.evaluate(2.0) << std::endl; // 1 + 6 + 4 = 11
    Polynomial m(a);
    m.mul(b); // (1 + 2x)(x + x^2) = x + 3x^2 + 2x^3
    m.print(std::cout);
    return 0;
}