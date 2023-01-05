#include "Entity.h"

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
