#pragma once
#include <optional>

#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"
#include "GobbletStack.hpp"

struct Move
{
    Move();
    
    Move(const Entity& t_player, const GobbletStack& t_gobbletStack, const std::optional<sf::Vector2i>& t_from,
         const sf::Vector2i& t_to);

    Entity player;
    GobbletStack gobbletStack; // The gobblet moved from the top of this stack
    std::optional<sf::Vector2i> from; // if null, the gobblet come from the external stacks
    sf::Vector2i to;
};
