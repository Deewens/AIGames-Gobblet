#pragma once

#include <memory>

#include "GobbletStack.hpp"
#include "Tile.h"

struct Move
{
    Gobblet gobblet;
    
    std::optional<Tile> fromTile;
    GobbletStack fromStack;
    
    Tile toTile;
    GobbletStack toStack;
};
