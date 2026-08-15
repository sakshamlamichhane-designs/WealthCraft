#include <iostream>
#include "Portfolio.h"
#include "Economy.h"
#include <stdexcept>

Portfolio::Portfolio() = default;

void Portfolio::addAsset(std::unique_ptr<Asset> asset) {
    assets_.push_back(std::move(asset));
}
void Portfolio::updateAssets(const Economy& economy)
{
    for (auto& asset : assets_)
    {
        asset->simulatePrice(economy);
    }
}
double Portfolio::calculateTotalValue() const {
    double total = 0.0;
    for (const auto& asset : assets_) {
        total += asset->getValue();
    }
    return total;
}

std::size_t Portfolio::assetCount() const {
    return assets_.size();
}

const Asset& Portfolio::getAsset(std::size_t index) const {
    if (index >= assets_.size()) {
        throw std::out_of_range("Asset index out of range");
    }
    return *assets_[index];
}
bool Portfolio::removeAsset(std::size_t index)
{
    if (index >= assets_.size())
    {
        return false;
    }

    assets_.erase(assets_.begin() + index);

    return true;
}

void Portfolio::printAssets() const
{
    std::cout << "\n===== Portfolio =====\n";

    if (assets_.empty())
    {
        std::cout << "No assets owned.\n";
        return;
    }

    for (std::size_t i = 0; i < assets_.size(); i++)
    {
        std::cout
            << i + 1
            << ". "
            << assets_[i]->getName()
            << "  $"
            << assets_[i]->getValue()
            << '\n';
    }

    std::cout
        << "\nTotal Value: $"
        << calculateTotalValue()
        << "\n";
}