#pragma once

#include <string>

enum class EventType
{
    None,

    BullMarket,
    MarketCrash,

    CryptoBoom,
    CryptoCrash,

    HousingBoom,
    HousingCrash,

    InflationSpike,
    InterestRateHike,

    SalaryRaise,
    SalaryCut,

    StartupIPO,
    GovernmentStimulus,
    Recession
};

struct Event
{
    EventType type;

    std::string title;

    std::string description;
};