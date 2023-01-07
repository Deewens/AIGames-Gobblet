#include "Entity.hpp"

Entity::Entity(sf::Color t_entityColor, bool t_isAI):
	m_entityColor(t_entityColor),
	m_isAi(t_isAI)
{
	m_canUseReserves = false;
}

sf::Color Entity::GetColor()
{
	return m_entityColor;
}

bool Entity::CanUseReserves()
{
	return m_canUseReserves;
}

void Entity::SetReserveUse(bool t_b)
{
	m_canUseReserves = t_b;
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
