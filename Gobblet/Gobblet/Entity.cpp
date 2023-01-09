#include "Entity.hpp"

#include "GobbletStack.hpp"
#include "Grid.h"


Entity::Entity(Grid& t_grid, const PlayerType& t_playerType, const sf::Color& t_gobbletColor) :
    m_type(t_playerType),
    m_entityColor(t_gobbletColor)
{
    m_canUseReserves = false;
    
    /*for (int i = 0; i < 3; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            Gobblet newGobblet = Gobblet(t_gobbletColor, j, t_grid);
            m_gobblets.push_back(newGobblet);
        }
    }*/
}

sf::Color Entity::getColor()
{
    return m_entityColor;
}

bool Entity::canUseReserves()
{
    return m_canUseReserves;
}

void Entity::setReserveUse(bool t_b)
{
    m_canUseReserves = t_b;
}

void Entity::setGobblets(const std::vector<Gobblet>& t_gobblets)
{
    m_gobblets = t_gobblets;
}

Gobblet& Entity::addGobblet(const Gobblet& t_gobblet)
{
    return m_gobblets.emplace_back(t_gobblet);
}

std::vector<Gobblet>& Entity::getGobblets()
{
    return m_gobblets;
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
