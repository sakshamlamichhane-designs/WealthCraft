#pragma once

#include "Event.h"

#include <random>

class EventManager
{
private:

    Event currentEvent_;

    std::mt19937 rng_;

public:

    EventManager();

    Event generateEvent();

    const Event& getCurrentEvent() const;
};