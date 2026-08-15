#pragma once

#include "Asset.h"

class Stock : public Asset {
public:
    Stock(const std::string& name, double initialPrice, double volatility);
    void simulatePrice(const Economy& economy) override;

    double getVolatility() const;

private:
    double volatility_;
};
