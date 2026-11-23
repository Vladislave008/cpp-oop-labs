#include "bank.h"
#include <stdexcept>
#include <cmath>
#include <iostream>


void BankProduct::check_constraints(const Params& params) {
    const auto check_nonnegative = [](const std::optional<double>& value,
                                      const char* message) {
        if (value.has_value() && (!std::isfinite(*value) || *value < 0.0)) {
            throw std::invalid_argument(message);
        }
    };

    check_nonnegative(params._sum, "Sum must be finite and greater than or equal to 0");
    check_nonnegative(params._rate, "Rate must be finite and greater than or equal to 0");
    check_nonnegative(params._spending, "Spending must be finite and greater than or equal to 0");

    if (params._annuity.value_or(false) && params._rate.has_value() && *params._rate == 0.0) {
        throw std::invalid_argument("Rate must be greater than 0 for an annuity credit");
    }

    if (params._years.has_value() && *params._years <= 0) {
        throw std::invalid_argument("Years must be greater than 0");
    }
}

std::ostream& operator<<(std::ostream& out, const BankProduct& product) {
    product.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, BankProduct& product) {
    product.read(in);
    return in;
}

bool operator==(const BankProduct& lhs, const BankProduct& rhs) {
    constexpr double eps = 1e-6;
    return std::abs(lhs.metric() - rhs.metric()) < eps;
}
