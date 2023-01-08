#include "Move.hpp"

Move::Move(const Entity& t_player, const Gobblet& t_gobblet, const std::optional<sf::Vector2i>& t_from,
           const sf::Vector2i& t_to)
    : player(t_player),
      gobblet(t_gobblet),
      from(t_from),
      to(t_to)
{
}
