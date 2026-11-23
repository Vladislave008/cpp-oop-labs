#include "credit.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

Credit::Credit() : BankProduct(), _sum(0), _rate(0), _years(1), _annuity(false) {}

Credit::Credit(double sum, double rate, int years, bool annuity)
    : BankProduct(), _sum(sum), _rate(rate), _years(years), _annuity(annuity) {
    check_constraints(Params(sum, rate, years, std::nullopt, annuity));
}

double Credit::metric() const {
    if (!_annuity) {
        return _sum * _rate;
    }

    const double monthly_rate = _rate / 1200.0;
    const double months = static_cast<double>(_years) * 12.0;
    const double denominator = 1 - std::pow(1 + monthly_rate, -months);
    return _sum * (12.0 * monthly_rate / denominator);
}

bool Credit::isAnnuity() const {
    return _annuity;
}

BankProduct* Credit::clone() const {
    return new Credit(*this);
}

void Credit::print(std::ostream& out) const {
    out << "Credit: sum=" << _sum
        << " rate=" << _rate << " years=" << _years
        << " annuity=" << static_cast<int>(_annuity) << '\n';
}

void Credit::read(std::istream& in) {
    double sum, rate;
    int years, annuity;

    if (!(in >> sum >> rate >> years >> annuity)) {
        return;
    }
    if (annuity != 0 && annuity != 1) {
        in.setstate(std::ios::failbit);
        return;
    }

    try {
        check_constraints(Params(sum, rate, years, std::nullopt, annuity != 0));
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
        return;
    }

    _sum = sum;
    _rate = rate;
    _years = years;
    _annuity = annuity != 0;
}
