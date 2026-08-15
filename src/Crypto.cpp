#include "Crypto.h"
#include "Economy.h"
#include "Random.h"

#include <algorithm>

Crypto::Crypto(const std::string& name,
               double initialPrice,
               double volatility)
    : Asset(name, initialPrice),
      volatility_(volatility)
{
}

void Crypto::simulatePrice(const Economy& economy)
{
    double currentPrice = getValue();

    // Crypto has larger random percentage movements.
    double randomRate =
        Random::randomDouble(-volatility_, volatility_);

    // Economic event effect.
    double marketEffect =
        economy.getCryptoMultiplier() - 1.0;

    double totalRate =
        randomRate + marketEffect;

    double newPrice =
        currentPrice * (1.0 + totalRate);

    // Keep the crypto price above zero.
    setValue(std::max(0.01, newPrice));
}

double Crypto::getVolatility() const
{
    return volatility_;
}