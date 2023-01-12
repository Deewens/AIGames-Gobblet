#pragma once

#include "Gobblet.hpp"
#include "GobbletStack.hpp"

struct MovePtr
{
    Gobblet* gobblet;

    GobbletStack* fromStack;
    GobbletStack* toStack;
};
