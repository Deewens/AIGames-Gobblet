#include "Move.hpp"

Move::Move(const Entity& t_player, const GobbletStack& t_gobbletStack, const std::optional<sf::Vector2i>& t_from,
           const sf::Vector2i& t_to)
    : player(t_player),
      gobbletStack(t_gobbletStack),
      from(t_from),
      to(t_to)
{
}
