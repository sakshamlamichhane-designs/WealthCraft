#include "FixedDeposit.h"
#include "Economy.h"

FixedDeposit::FixedDeposit(const std::string& name,
                           double amount,
                           double interestRate)
    : Asset(name, amount),
      interestRate_(interestRate)
{
}

void FixedDeposit::simulatePrice(const Economy& economy)
{
    double current = getValue();

    double effectiveRate =
        (interestRate_ + economy.getInterestRate()) / 2.0;

    current += current * effectiveRate / 12.0;

    setValue(current);
}