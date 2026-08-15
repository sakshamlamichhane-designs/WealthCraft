#pragma once

#include "Asset.h"

class MutualFund : public Asset
{
private:
    double growthRate_;

public:
    MutualFund(const std::string& name,
               double value,
               double growthRate);

    void simulatePrice(const Economy& economy) override;
};