#pragma once

#include "Asset.h"

class Bond : public Asset {
public:
    Bond(const std::string& name, double faceValue, double annualYield);
    void simulatePrice(const Economy& economy) override;

    double getAnnualYield() const;

private:
    double annualYield_;
};
