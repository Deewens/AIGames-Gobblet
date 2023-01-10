#include "Entity.hpp"

Entity::Entity(const sf::Color& t_gobbletColor, PlayerAIType t_AIType) :
    m_gobbletColor(t_gobbletColor),
    m_type(t_AIType)
{
    m_canUseReserves = false;
}

std::vector<GobbletStack> Entity::initialiseGame()
{
    
}

const sf::Color& Entity::getColor() const
{
    return m_gobbletColor;
}

const PlayerAIType& Entity::getType() const
{
    return m_type;
}

bool Entity::canUseReserves() const
{
    return m_canUseReserves;
}

void Entity::setReserveUse(bool t_b)
{
    m_canUseReserves = t_b;
}

bool operator==(const Entity& t_lhs, const Entity& t_rhs)
{
    return t_lhs.m_type == t_rhs.m_type
        && t_lhs.m_gobbletColor == t_rhs.m_gobbletColor;
}

bool operator!=(const Entity& t_lhs, const Entity& t_rhs)
{
    return !(t_lhs == t_rhs);
}
