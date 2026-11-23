#pragma once

#include <optional>

struct Params {
    std::optional<double> _sum;
    std::optional<double> _rate;
    std::optional<int> _years;
    std::optional<bool> _cumulative;
    std::optional<bool> _annuity;
    std::optional<double> _spending;

    Params(
        std::optional<double> sum = std::nullopt,
        std::optional<double> rate = std::nullopt,
        std::optional<int> years = std::nullopt,
        std::optional<bool> cumulative = std::nullopt,
        std::optional<bool> annuity = std::nullopt,
        std::optional<double> spending = std::nullopt
    ) : _sum(sum), _rate(rate), _years(years),
        _cumulative(cumulative), _annuity(annuity), _spending(spending) {}
};
