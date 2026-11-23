#pragma once

#include "bank.h"

class Credit : public BankProduct {
private:
    double _sum;
    double _rate;
    int _years;
    bool _annuity;

public:
    Credit();
    Credit(double sum, double rate, int years, bool annuity);

    double metric() const override;
    BankProduct* clone() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    bool isAnnuity() const;
};
