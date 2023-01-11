#pragma once

#include "Gobblet.hpp"
#include "GobbletStack.hpp"

struct Move
{
    Gobblet* gobblet;

    GobbletStack* fromStack;
    GobbletStack* toStack;
};
