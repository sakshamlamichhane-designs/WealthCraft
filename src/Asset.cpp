#include "Asset.h"
#include <utility>

Asset::Asset(std::string name, double value)
    : name_(std::move(name)),
      value_(value)
{
}

double Asset::getValue() const
{
    return value_;
}

const std::string& Asset::getName() const
{
    return name_;
}

void Asset::setValue(double value)
{
    value_ = value;
}