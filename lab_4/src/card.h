#pragma once

#include "bank.h"

class Card : public BankProduct {
private:
    double _spending;
    double _rate;

public:
    Card();
    Card(double spending, double rate);

    double metric() const override;
    BankProduct* clone() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    bool hasCashback() const;
};
