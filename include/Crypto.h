#pragma once

#include "Asset.h"

class Crypto : public Asset
{
private:
    double volatility_;

public:
    Crypto(const std::string& name,
           double initialPrice,
           double volatility);

    void simulatePrice(const Economy& economy) override;

    double getVolatility() const;
};