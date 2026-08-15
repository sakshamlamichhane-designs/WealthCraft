#include "Stock.h"
#include "Economy.h"
#include "Random.h"

#include <algorithm>

Stock::Stock(const std::string& name, double initialPrice, double volatility)
    : Asset(name, initialPrice),
      volatility_(volatility)
{
}

void Stock::simulatePrice(const Economy& economy)
{
    double currentPrice = getValue();

    // Random percentage movement.
    double randomRate =
        Random::randomDouble(-volatility_, volatility_);

    // Economic event effect.
    double marketEffect =
        economy.getStockMultiplier() - 1.0;

    double totalRate =
        randomRate + marketEffect;

    double newPrice =
        currentPrice * (1.0 + totalRate);

    // Keep the stock price above zero.
    setValue(std::max(0.01, newPrice));
}

double Stock::getVolatility() const
{
    return volatility_;
}