#include "deposit.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

Deposit::Deposit() : BankProduct(), _sum(0), _rate(0), _years(1), _cumulative(0) {}

Deposit::Deposit(double sum, double rate, int years, bool cumulative) : BankProduct() {
    BankProduct::check_constraints(Params(sum, rate, years, cumulative));
    _sum = sum;
    _rate = rate;
    _years = years;
    _cumulative = cumulative;
}

double Deposit::metric() const {
    if (!_cumulative) {
        return _sum * _rate;
    } else {
        return _sum*(std::pow((1+_rate), _years) - 1)/_years;
    }
}

bool Deposit::isCumulative() const {
    return _cumulative;
}

BankProduct* Deposit::clone() const {
    return new Deposit(*this);
}

void Deposit::print(std::ostream& out) const {
    out << "Deposit: sum=" << _sum
        << " rate=" << _rate << " years=" << _years
        << " cumulative=" << static_cast<int>(_cumulative) << '\n';
}

void Deposit::read(std::istream& in) {
    double sum, rate;
    int years, cumulative;

    if (!(in >> sum >> rate >> years >> cumulative)) {
        return;
    }

    if (cumulative != 0 && cumulative != 1) {
        in.setstate(std::ios::failbit);
        return;
    }

    try {
        check_constraints(Params(sum, rate, years, cumulative != 0));
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
        return;
    }

    _sum = sum;
    _rate = rate;
    _years = years;
    _cumulative = cumulative != 0;
}

