#pragma once

#include <iostream>

class Polynomial {
    private:
        double* data_ = nullptr;
        int degree_ = -1;
        void trim();
        void swap(Polynomial& other) noexcept;
    public:
        Polynomial(const double* data, int degree);
        Polynomial(const Polynomial& other);
        Polynomial(Polynomial&& other) noexcept;
        Polynomial& operator= (Polynomial other) noexcept;
        ~Polynomial();
        void print(std::ostream& os) const;
        double evaluate(double in) const;
        void evaluate_many(const double* xs, double* ys, size_t n) const;
        void add(const Polynomial& other);
        void mul(const Polynomial& other);
};