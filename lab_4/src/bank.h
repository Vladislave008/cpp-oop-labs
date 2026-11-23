#pragma once

#include <iostream>
#include "params.h"


class BankProduct {
public:
    virtual ~BankProduct() = default;
    virtual double metric() const = 0;
    virtual BankProduct* clone() const = 0;
    virtual operator double() const {
        return metric();
    }
    static void check_constraints(const Params& params);
    virtual void print(std::ostream& out) const = 0;
    virtual void read(std::istream& in) = 0;
};

std::ostream& operator<<(std::ostream& out, const BankProduct& product);
std::istream& operator>>(std::istream& in, BankProduct& product);
bool operator==(const BankProduct& lhs, const BankProduct& rhs);
