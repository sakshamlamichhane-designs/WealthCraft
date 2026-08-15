#include "Bond.h"
#include "Economy.h"
#include <algorithm>

Bond::Bond(const std::string& name,
           double faceValue,
           double annualYield)
    : Asset(name, faceValue),
      annualYield_(annualYield)
{
}

void Bond::simulatePrice(const Economy& economy)
{
    double current = getValue();

    double effectiveYield =
        annualYield_ - economy.getInflationRate();

    double interestEffect =
        (economy.getInterestRate() - annualYield_) * 0.05;

    double monthlyChange =
        effectiveYield / 12.0 + interestEffect;

    current *= (1.0 + monthlyChange);

    current *= economy.getBondMultiplier();

    setValue(std::max(0.0, current));
}

double Bond::getAnnualYield() const
{
    return annualYield_;
}