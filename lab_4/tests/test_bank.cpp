#include "bank_collection.h"
#include "card.h"
#include "credit.h"
#include "deposit.h"

#include <gtest/gtest.h>

#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

TEST(Products, DepositMetric) {
    EXPECT_NEAR(Deposit(1000, 0.1, 2, false).metric(), 100, 1e-6);
    EXPECT_NEAR(Deposit(1000, 0.1, 2, true).metric(), 105, 1e-6);
    EXPECT_DOUBLE_EQ(Deposit(1000, 0, 2, true).metric(), 0);
}

TEST(Products, CreditMetric) {
    EXPECT_NEAR(Credit(120000, 0.12, 1, false).metric(), 14400, 1e-6);
    EXPECT_NEAR(Credit(120000, 12, 1, true).metric(), 127942.255697, 1e-6);
    EXPECT_DOUBLE_EQ(Credit(1000, 0, 1, false).metric(), 0);
}

TEST(Products, CardMetric) {
    EXPECT_DOUBLE_EQ(Card(10000, 0.02).metric(), 200);
    EXPECT_DOUBLE_EQ(Card(0, 0.02).metric(), 0);
    EXPECT_DOUBLE_EQ(Card(10000, 0).metric(), 0);
}

TEST(Products, SpecificMethodsAndDynamicCast) {
    BankCollection products;
    products.add(Deposit(1000, 0.1, 2, true));
    products.add(Credit(120000, 12, 1, true));
    products.add(Card(10000, 0.02));
    const auto* deposit = dynamic_cast<const Deposit*>(&products[0]);
    const auto* credit = dynamic_cast<const Credit*>(&products[1]);
    const auto* card = dynamic_cast<const Card*>(&products[2]);
    ASSERT_NE(deposit, nullptr);
    ASSERT_NE(credit, nullptr);
    ASSERT_NE(card, nullptr);
    EXPECT_TRUE(deposit->isCumulative());
    EXPECT_TRUE(credit->isAnnuity());
    EXPECT_TRUE(card->hasCashback());
    EXPECT_EQ(dynamic_cast<const Credit*>(&products[0]), nullptr);
    EXPECT_FALSE(Deposit().isCumulative());
    EXPECT_FALSE(Credit().isAnnuity());
    EXPECT_FALSE(Card().hasCashback());
}

TEST(Products, ReadFieldsWithoutClassName) {
    Deposit deposit;
    Credit credit;
    Card card;
    std::istringstream input("1000 0.1 2 1\n120000 12 1 1\n10000 0.02");
    input >> deposit >> credit >> card;
    ASSERT_FALSE(input.fail());
    EXPECT_NEAR(deposit.metric(), 105, 1e-6);
    EXPECT_NEAR(credit.metric(), 127942.255697, 1e-6);
    EXPECT_DOUBLE_EQ(card.metric(), 200);
}

TEST(Products, PrintNamesAndFields) {
    std::ostringstream output;
    output << Deposit(1000, 0.1, 2, true)
           << Credit(120000, 12, 1, true) << Card(10000, 0.02);
    EXPECT_EQ(output.str(),
              "Deposit: sum=1000 rate=0.1 years=2 cumulative=1\n"
              "Credit: sum=120000 rate=12 years=1 annuity=1\n"
              "Card: spending=10000 rate=0.02\n");
}

TEST(Products, InvalidInputPreservesObject) {
    Deposit deposit(1000, 0.1, 2, true);
    for (const char* text : {"-1 0.1 2 1", "1000 0.1 0 1", "1000 0.1 2 2", "1000 text", "1000 0.1"}) {
        std::istringstream input(text);
        input >> deposit;
        EXPECT_TRUE(input.fail());
        EXPECT_NEAR(deposit.metric(), 105, 1e-6);
    }
    Credit credit(1000, 0.1, 1, false);
    std::istringstream bad_credit("1000 0 1 1");
    bad_credit >> credit;
    EXPECT_TRUE(bad_credit.fail());
    EXPECT_FALSE(credit.isAnnuity());
    Card card(10000, 0.02);
    std::istringstream bad_card("10000 -0.02");
    bad_card >> card;
    EXPECT_TRUE(bad_card.fail());
    EXPECT_DOUBLE_EQ(card.metric(), 200);
}

TEST(Products, RejectInvalidParameters) {
    EXPECT_THROW(Deposit(-1, 0.1, 2, true), std::invalid_argument);
    EXPECT_THROW(Deposit(1000, 0.1, 0, true), std::invalid_argument);
    EXPECT_THROW(Credit(1000, 12, -1, true), std::invalid_argument);
    EXPECT_THROW(Credit(1000, 0, 1, true), std::invalid_argument);
    EXPECT_THROW(Card(1000, -0.02), std::invalid_argument);
    EXPECT_THROW(Card(-1, 0.02), std::invalid_argument);
    EXPECT_THROW(Card(1, std::numeric_limits<double>::infinity()), std::invalid_argument);
    EXPECT_THROW(Deposit(1, std::numeric_limits<double>::quiet_NaN(), 1, true), std::invalid_argument);
}

TEST(Products, EqualityAndDoubleConversion) {
    Deposit deposit(1000, 0.1, 1, false);
    const BankProduct& base = deposit;
    EXPECT_DOUBLE_EQ(static_cast<double>(base), 100);
    EXPECT_TRUE(base == Card(100, 1));
    EXPECT_TRUE(base == Card(100 + 1e-7, 1));
    EXPECT_FALSE(base == Card(101, 1));
}

TEST(Collection, EmptyStateAndBounds) {
    const BankCollection products;
    EXPECT_TRUE(products.empty());
    EXPECT_EQ(products.size(), 0u);
    EXPECT_DOUBLE_EQ(products.total_metric(), 0);
    EXPECT_EQ(products.max_product(), nullptr);
    EXPECT_THROW(products[0], std::out_of_range);
}

TEST(Collection, AddGrowAndSumMetrics) {
    BankCollection products;
    products.add(Deposit(1000, 0.1, 2, true));
    products.add(Credit(1000, 0.1, 1, false));
    for (int i = 0; i < 10; ++i) {
        products.add(Card(10000, 0.02));
    }
    EXPECT_EQ(products.size(), 12u);
    EXPECT_NEAR(products.total_metric(), 2205, 1e-6);
    EXPECT_THROW(products[12], std::out_of_range);
}

TEST(Collection, MaximumAndTies) {
    BankCollection products;
    EXPECT_EQ(products.max_product(), nullptr);
    products.add(Deposit(1000, 0.1, 1, false));
    products.add(Card(10000, 0.02));
    products.add(Credit(2000, 0.1, 1, false));
    EXPECT_EQ(products.max_product(), &products[1]);
    const BankCollection& view = products;
    EXPECT_EQ(view.max_product(), &view[1]);
}

TEST(Collection, DeepCopyAndCopyAssignment) {
    BankCollection original;
    original.add(Deposit(1000, 0.1, 2, true));
    original.add(Card(10000, 0.02));
    BankCollection copy(original);
    BankCollection assigned;
    assigned.add(Credit());
    assigned = original;
    EXPECT_NE(&copy[0], &original[0]);
    EXPECT_NE(&assigned[0], &original[0]);
    EXPECT_NE(dynamic_cast<Deposit*>(&copy[0]), nullptr);
    std::istringstream input("1000 0.2 1 0");
    input >> copy[0];
    EXPECT_NEAR(original[0].metric(), 105, 1e-6);
    EXPECT_DOUBLE_EQ(copy[0].metric(), 200);
    assigned.erase(0);
    EXPECT_EQ(original.size(), 2u);
    EXPECT_EQ(assigned.size(), 1u);
    original = original;
    EXPECT_EQ(original.size(), 2u);
}

TEST(Collection, MoveConstructionAndAssignment) {
    BankCollection source;
    source.add(Card(10000, 0.02));
    const BankProduct* address = &source[0];
    BankCollection moved(std::move(source));
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(&moved[0], address);
    BankCollection target;
    target.add(Deposit());
    target = std::move(moved);
    EXPECT_TRUE(moved.empty());
    ASSERT_EQ(target.size(), 1u);
    EXPECT_EQ(&target[0], address);
    source.add(Card(10, 0.1));
    EXPECT_DOUBLE_EQ(source.total_metric(), 1);
}

namespace {
class CountingCard : public Card {
    int* _destroyed;
public:
    explicit CountingCard(int& destroyed) : Card(100, 0.01), _destroyed(&destroyed) {}
    ~CountingCard() override { ++*_destroyed; }
    BankProduct* clone() const override { return new CountingCard(*this); }
};
}

TEST(Collection, EraseClearAndVirtualDestruction) {
    int destroyed = 0;
    CountingCard card(destroyed);
    {
        std::unique_ptr<BankProduct> product(card.clone());
    }
    EXPECT_EQ(destroyed, 1);
    {
        BankCollection products;
        products.add(card);
        products.add(Card(10000, 0.02));
        products.add(card);
        products.erase(0);
        EXPECT_EQ(destroyed, 2);
        ASSERT_EQ(products.size(), 2u);
        EXPECT_DOUBLE_EQ(products[0].metric(), 200);
        EXPECT_THROW(products.erase(2), std::out_of_range);
        products.clear();
        EXPECT_TRUE(products.empty());
        EXPECT_EQ(destroyed, 3);
        products.add(card);
    }
    EXPECT_EQ(destroyed, 4);
}
