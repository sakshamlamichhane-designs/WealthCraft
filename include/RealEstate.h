#pragma once

#include "Asset.h"

class RealEstate : public Asset
{
public:
    RealEstate(const std::string& name,
               double value);

    void simulatePrice(const Economy& economy) override;
};