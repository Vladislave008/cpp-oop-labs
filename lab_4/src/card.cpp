#include "card.h"

#include <iostream>
#include <stdexcept>

Card::Card() : BankProduct(), _spending(0), _rate(0) {}

Card::Card(double spending, double rate)
    : BankProduct(), _spending(spending), _rate(rate) {
    check_constraints(Params(std::nullopt, rate, std::nullopt,
                             std::nullopt, std::nullopt, spending));
}

double Card::metric() const {
    return _spending * _rate;
}

bool Card::hasCashback() const {
    return _rate > 0;
}

BankProduct* Card::clone() const {
    return new Card(*this);
}

void Card::print(std::ostream& out) const {
    out << "Card: spending=" << _spending << " rate=" << _rate << '\n';
}

void Card::read(std::istream& in) {
    double spending, rate;

    if (!(in >> spending >> rate)) {
        return;
    }

    try {
        check_constraints(Params(std::nullopt, rate, std::nullopt,
                                 std::nullopt, std::nullopt, spending));
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
        return;
    }

    _spending = spending;
    _rate = rate;
}
