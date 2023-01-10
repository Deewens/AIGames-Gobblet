#pragma once
#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"
#include "Gobblet.hpp"
#include "GobbletStack.hpp"

struct Move
{
    Move(const Entity& t_player, const Gobblet& t_gobblet, const GobbletStack& t_fromStack,
        const sf::Vector2i& t_toPosition);

    Entity player;
    Gobblet gobblet;
    
    GobbletStack fromStack;
    sf::Vector2i toPosition;
};
