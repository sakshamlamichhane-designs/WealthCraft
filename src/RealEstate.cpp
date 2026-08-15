#include "RealEstate.h"
#include "Economy.h"
#include "Random.h"

#include <algorithm>

RealEstate::RealEstate(
    const std::string& name,
    double value)
    : Asset(name, value)
{
}

void RealEstate::simulatePrice(const Economy& economy)
{
    double currentValue = getValue();

    // Real estate normally appreciates slowly over time.
    const double baseAppreciation = 0.008; // 0.8% per month

    // Small random variation represents local market differences.
    const double randomVariation =
        Random::randomDouble(-0.005, 0.005);

    // Economic conditions can increase or decrease property values.
    const double economicEffect =
        economy.getRealEstateMultiplier() - 1.0;

    double totalChange =
        baseAppreciation
        + randomVariation
        + economicEffect;

    double newValue =
        currentValue * (1.0 + totalChange);

    // Property value cannot become negative.
    newValue = std::max(0.01, newValue);

    setValue(newValue);
}