#pragma once

#include "Gobblet.hpp"
#include "GobbletStack.hpp"

struct Move
{
    Move(const Gobblet& t_gobblet, const GobbletStack& t_fromStack, const GobbletStack& t_toStack) :
        gobblet(t_gobblet),
        fromStack(t_fromStack),
        toStack(t_toStack)
    {
    }

    Gobblet gobblet;

    GobbletStack fromStack;
    GobbletStack toStack;
};
