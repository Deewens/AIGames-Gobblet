#pragma once
#include <optional>

#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"
#include "Gobblet.hpp"

struct Move
{
    Move(const Entity& t_player, const Gobblet& t_gobblet, const std::optional<sf::Vector2i>& t_from,
         const sf::Vector2i& t_to);

    Entity player;
    Gobblet gobblet;
    std::optional<sf::Vector2i> from; // if null, the gobblet come from the external stacks
    sf::Vector2i to;
};
