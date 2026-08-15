#pragma once

#include <string>
class Economy;      // Forward declaration
class Asset {
public:
    Asset(std::string name, double value);
    virtual ~Asset() = default;
    virtual void simulatePrice(const Economy& economy) = 0;

    double getValue() const;
    const std::string& getName() const;

protected:
    void setValue(double value);

private:
    std::string name_;
    double value_;
};
