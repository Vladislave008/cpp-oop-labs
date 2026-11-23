#pragma once

#include "bank.h"

class Deposit : public BankProduct {
private:
    double _sum;
    double _rate;
    int _years;
    bool _cumulative;

public:
    Deposit();
    Deposit(double sum, double rate, int years, bool cumulative);

    double metric() const override;
    BankProduct* clone() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    bool isCumulative() const;
};
