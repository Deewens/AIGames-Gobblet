#include "Entity.hpp"

Entity::Entity(sf::Color t_gobbletColor, PlayerAIType t_AIType):
    m_type(t_AIType),
    m_entityColor(t_gobbletColor)
{
    m_canUseReserves = false;
}

sf::Color Entity::getColor() const
{
    return m_entityColor;
}

bool Entity::canUseReserves() const
{
    return m_canUseReserves;
}

void Entity::setReserveUse(bool t_b)
{
    m_canUseReserves = t_b;
}

const PlayerAIType& Entity::getType() const
{
    return m_type;
}

bool operator==(const Entity& t_lhs, const Entity& t_rhs)
{
    return t_lhs.m_type == t_rhs.m_type
        && t_lhs.m_entityColor == t_rhs.m_entityColor;
}

bool operator!=(const Entity& t_lhs, const Entity& t_rhs)
{
    return !(t_lhs == t_rhs);
}
