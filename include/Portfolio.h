#pragma once

#include "Asset.h"
#include <memory>
#include <string>
#include <vector>

class Portfolio {
public:
    Portfolio();

    void addAsset(std::unique_ptr<Asset> asset);
    double calculateTotalValue() const;
    std::size_t assetCount() const;
    const Asset& getAsset(std::size_t index) const;
    void updateAssets(const Economy& economy);
    bool removeAsset(std::size_t index);
    void printAssets() const;

private:
    std::vector<std::unique_ptr<Asset>> assets_;
};
