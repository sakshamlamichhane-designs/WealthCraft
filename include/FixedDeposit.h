#pragma once

#include "Asset.h"

class FixedDeposit : public Asset
{
private:
    double interestRate_;

public:
    FixedDeposit(const std::string& name,
                 double amount,
                 double interestRate);

    void simulatePrice(const Economy& economy) override;
};