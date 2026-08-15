#include "EventManager.h"

EventManager::EventManager()
    : rng_(std::random_device{}())
{
    currentEvent_ =
    {
        EventType::None,
        "No Event",
        "Nothing happened."
    };
}

Event EventManager::generateEvent()
{
    std::uniform_int_distribution<int> chance(1,100);

    int roll = chance(rng_);

    if (roll <= 35)
    {
        currentEvent_ =
        {
            EventType::None,
            "Quiet Month",
            "Markets stayed relatively stable."
        };
    }
    else if (roll <= 50)
    {
        currentEvent_ =
        {
            EventType::BullMarket,
            "Bull Market",
            "Stocks are rising rapidly."
        };
    }
    else if (roll <= 60)
    {
        currentEvent_ =
        {
            EventType::MarketCrash,
            "Market Crash",
            "Stocks plunged sharply."
        };
    }
    else if (roll <= 68)
    {
        currentEvent_ =
        {
            EventType::CryptoBoom,
            "Crypto Boom",
            "Cryptocurrency prices exploded."
        };
    }
    else if (roll <= 76)
    {
        currentEvent_ =
        {
            EventType::CryptoCrash,
            "Crypto Crash",
            "Cryptocurrency prices collapsed."
        };
    }
    else if (roll <= 84)
    {
        currentEvent_ =
        {
            EventType::HousingBoom,
            "Housing Boom",
            "Real estate prices increased."
        };
    }
    else if (roll <= 90)
    {
        currentEvent_ =
        {
            EventType::InflationSpike,
            "Inflation Spike",
            "Living costs increased."
        };
    }
    else if (roll <= 95)
    {
        currentEvent_ =
        {
            EventType::SalaryRaise,
            "Promotion",
            "You received a salary raise."
        };
    }
    else
    {
        currentEvent_ =
        {
            EventType::Recession,
            "Recession",
            "The economy entered recession."
        };
    }

    return currentEvent_;
}

const Event& EventManager::getCurrentEvent() const
{
    return currentEvent_;
}