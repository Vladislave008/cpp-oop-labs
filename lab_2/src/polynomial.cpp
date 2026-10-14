#include <algorithm>
#include <iostream>
#include <cmath>
#include "polynomial.h"

void Polynomial::trim()
{
    while (degree_ >= 0 && data_[degree_] == 0.0)
        --degree_;

    if (degree_ < 0)
    {
        delete[] data_;
        data_ = nullptr;
    }
}

void Polynomial::swap(Polynomial &other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(degree_, other.degree_);
}

Polynomial::Polynomial(const double *data, int degree)
{
    if (degree < 0 || data == nullptr)
    {
        return;
    }
    degree_ = degree;
    data_ = new double[degree_ + 1];
    for (int i = 0; i <= degree_; ++i)
    {
        data_[i] = data[i];
    }
    trim();
}

Polynomial::Polynomial(const Polynomial &other) : data_(new double[other.degree_ + 1]), degree_(other.degree_)
{
    for (int i = 0; i <= degree_; ++i)
    {
        data_[i] = other.data_[i];
    }
    trim();
}

Polynomial::Polynomial(Polynomial &&other) noexcept : data_(other.data_), degree_(other.degree_)
{
    other.data_ = nullptr;
    other.degree_ = -1;
    trim();
}

Polynomial &Polynomial::operator=(Polynomial other) noexcept
{
    swap(other);
    trim();
    return *this;
}

Polynomial::~Polynomial()
{
    delete[] data_;
}

void Polynomial::print(std::ostream &os) const
{
    if (data_ == nullptr || degree_ == -1)
    {
        os << "[empty polynomial]";
    }
    for (int i = degree_; i >= 0; --i)
    {
        if (data_[i] == 0)
        {
            continue;
        }
        if (i < degree_ && data_[i] > 0)
        {
            os << "+ ";
        }
        if (data_[i] < 0)
        {
            os << "- ";
        }
        if (data_[i] != 1 && data_[i] != -1)
        {
            if (i > 1)
            {
                os << ((data_[i] > 0) ? data_[i] : -data_[i]) << "x^" << i << " ";
            }
            else if (i == 1)
            {
                os << ((data_[i] > 0) ? data_[i] : -data_[i]) << "x" << " ";
            }
            else
            {
                os << ((data_[i] > 0) ? data_[i] : -data_[i]);
            }
        }
        else
        {
            if (i > 1)
            {
                os << "x^" << i << " ";
            }
            else if (i == 1)
            {
                os << "x" << " ";
            }
            else
            {
                os << "1";
            }
        }
    }
    os << "\n";
}

double Polynomial::evaluate(double in) const
{
    double out = 0.0;
    for (int i = degree_; i >= 0; --i)
    {
        out += data_[i] * std::pow(in, i);
    }
    return out;
}

void Polynomial::evaluate_many(const double *xs, double *ys, size_t n) const
{
    for (size_t i = 0; i < n; ++i)
    {
        ys[i] = evaluate(xs[i]);
    }
}

void Polynomial::add(const Polynomial &other)
{
    int max_degree = std::max(degree_, other.degree_);
    double *temp = new double[max_degree + 1]{};
    for (int i = max_degree; i >= 0; --i)
    {
        if (i <= degree_ && data_ != nullptr)
        {
            temp[i] += data_[i];
        }
        if (i <= other.degree_ && other.data_ != nullptr)
        {
            temp[i] += other.data_[i];
        }
    }
    delete[] data_;
    data_ = temp;
    degree_ = max_degree;
    trim();
}

void Polynomial::mul(const Polynomial &other)
{
    if (data_ == nullptr || other.data_ == nullptr)
    {
        delete[] data_;
        data_ = nullptr;
        degree_ = -1;
        return;
    }
    int new_degree = degree_ + other.degree_;
    double *temp = new double[new_degree + 1]{};
    for (int i = degree_; i >= 0; --i)
    {
        for (int j = other.degree_; j >= 0; --j)
        {
            temp[i + j] += data_[i] * other.data_[j];
        }
    }
    delete[] data_;
    data_ = temp;
    degree_ = new_degree;
    trim();
}