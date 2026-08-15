#include "MutualFund.h"
#include "Economy.h"
#include <algorithm>

MutualFund::MutualFund(const std::string& name,
                       double value,
                       double growthRate)
    : Asset(name, value),
      growthRate_(growthRate)
{
}

void MutualFund::simulatePrice(const Economy& economy)
{
    double current = getValue();

    double monthlyGrowth = growthRate_ / 12.0;

    double marketEffect =
        (economy.getStockMultiplier() - 1.0) * 0.50;

    double totalChange =
        monthlyGrowth + marketEffect;

    current *= (1.0 + totalChange);

    setValue(std::max(0.0, current));
}