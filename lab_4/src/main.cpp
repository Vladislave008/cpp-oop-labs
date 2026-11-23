#include "deposit.h"
#include "credit.h"
#include "card.h"
#include "bank_collection.h"

#include <iostream>
#include <sstream>

int main() {
    Deposit deposit(1000, 0.1, 2, true);
    Credit credit(120000, 12, 1, true);
    Card card(10000, 0.02);

    const BankProduct& product = deposit;
    std::cout << product << "metric = " << static_cast<double>(product) << "\n\n";
    std::cout << credit << "metric = " << credit.metric() << "\n\n";
    std::cout << card << "metric = " << card.metric() << "\n\n";

    if (const auto* value = dynamic_cast<const Deposit*>(&product)) {
        std::cout << "Capitalization: " << value->isCumulative() << '\n';
    }
    std::cout << "Annuity: " << credit.isAnnuity() << '\n';
    std::cout << "Cashback: " << card.hasCashback() << '\n';

    BankCollection products;
    products.add(deposit);
    products.add(credit);
    products.add(card);
    std::cout << "\nTotal metric: " << products.total_metric() << '\n';
    if (const BankProduct* best = products.max_product()) {
        std::cout << "Maximum: " << *best;
    }

    BankCollection copy;
    copy = products;
    copy.erase(0);
    std::cout << "Original size: " << products.size()
              << ", copy size: " << copy.size() << '\n';

    products.erase(0);
    std::cout << "\nAfter removing deposit from collection, size: " << products.size() << '\n';
    std::cout << "Original deposit is still available: " << deposit;
    std::cout << "metric = " << deposit.metric() << '\n';

    Deposit input_deposit;
    std::istringstream input("1000 0.1 2 1");
    if (!(input >> input_deposit)) {
        std::cerr << "Invalid deposit data\n";
        return 1;
    }
    std::cout << "\nDeposit read from string: " << input_deposit
              << "metric = " << input_deposit.metric() << '\n';
}
