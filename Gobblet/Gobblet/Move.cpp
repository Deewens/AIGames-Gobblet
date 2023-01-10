#include "Move.hpp"

Move::Move(const Entity& t_player, const Gobblet& t_gobblet, const GobbletStack& t_fromStack,
    const sf::Vector2i& t_toPosition): player(t_player),
                                       gobblet(t_gobblet),
                                       fromStack(t_fromStack),
                                       toPosition(t_toPosition)
{
}
