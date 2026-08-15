#pragma once

#include "Event.h"

class Economy
{
private:

    double inflationRate_;

    double interestRate_;

    double stockMarketMultiplier_;

    double cryptoMultiplier_;

    double realEstateMultiplier_;

    double bondMultiplier_;

public:

    Economy();

    void update(const Event& event);

    double getInflationRate() const;

    double getInterestRate() const;

    double getStockMultiplier() const;

    double getCryptoMultiplier() const;

    double getRealEstateMultiplier() const;

    double getBondMultiplier() const;
};