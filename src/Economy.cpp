#include "Economy.h"

Economy::Economy()
    : inflationRate_(0.03),
      interestRate_(0.06),
      stockMarketMultiplier_(1.0),
      cryptoMultiplier_(1.0),
      realEstateMultiplier_(1.0),
      bondMultiplier_(1.0)
{
}

void Economy::update(const Event& event)
{
    // Gradually move economic conditions back toward normal.
    inflationRate_ += (0.03 - inflationRate_) * 0.20;
    interestRate_ += (0.06 - interestRate_) * 0.20;

    stockMarketMultiplier_ +=
        (1.0 - stockMarketMultiplier_) * 0.20;

    cryptoMultiplier_ +=
        (1.0 - cryptoMultiplier_) * 0.20;

    realEstateMultiplier_ +=
        (1.0 - realEstateMultiplier_) * 0.20;

    bondMultiplier_ +=
        (1.0 - bondMultiplier_) * 0.20;

    // Apply the current month's economic event.
    switch (event.type)
    {
        case EventType::BullMarket:
            stockMarketMultiplier_ *= 1.15;
            break;

        case EventType::MarketCrash:
            stockMarketMultiplier_ *= 0.82;
            break;

        case EventType::CryptoBoom:
            cryptoMultiplier_ *= 1.35;
            break;

        case EventType::CryptoCrash:
            cryptoMultiplier_ *= 0.70;
            break;

        case EventType::HousingBoom:
            realEstateMultiplier_ *= 1.12;
            break;

        case EventType::HousingCrash:
            realEstateMultiplier_ *= 0.88;
            break;

        case EventType::InflationSpike:
            inflationRate_ = 0.08;
            bondMultiplier_ *= 0.97;
            break;

        case EventType::InterestRateHike:
            interestRate_ = 0.09;
            bondMultiplier_ *= 1.08;
            break;

        case EventType::Recession:
            stockMarketMultiplier_ *= 0.90;
            cryptoMultiplier_ *= 0.80;
            realEstateMultiplier_ *= 0.95;
            break;

        default:
            break;
    }
}

double Economy::getInflationRate() const
{
    return inflationRate_;
}

double Economy::getInterestRate() const
{
    return interestRate_;
}

double Economy::getStockMultiplier() const
{
    return stockMarketMultiplier_;
}

double Economy::getCryptoMultiplier() const
{
    return cryptoMultiplier_;
}

double Economy::getRealEstateMultiplier() const
{
    return realEstateMultiplier_;
}

double Economy::getBondMultiplier() const
{
    return bondMultiplier_;
}