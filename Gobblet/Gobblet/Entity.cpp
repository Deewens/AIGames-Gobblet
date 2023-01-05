#include "Entity.h"

Entity::Entity(sf::Color t_entityColor, bool t_isAI):
	m_entityColor(t_entityColor),
	m_isAi(t_isAI)
{

}

sf::Color Entity::GetColor()
{
	return m_entityColor;
}