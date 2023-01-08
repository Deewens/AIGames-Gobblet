#include "GobbletStack.hpp"

#include <iostream>

GobbletStack::GobbletStack() : 
{
}

void GobbletStack::add(const Gobblet& t_gobblet)
{
    m_stack.push(t_gobblet);

    m_stack.top().setGridCoordinates(m_gridPosition);
}

Gobblet& GobbletStack::top()
{
    return m_stack.top();
}

Gobblet GobbletStack::pop()
{
    auto removedGobblet = m_stack.top();

    m_stack.pop();

    return removedGobblet;
}

bool GobbletStack::isEmpty() const
{
    return m_stack.empty();
}

void GobbletStack::setPosition(const sf::Vector2f& t_position)
{
    m_position = t_position;

    if (m_stack.empty()) return;
    
    m_stack.top().setPosition(m_position);
}

sf::Vector2f GobbletStack::getPosition() const
{
    return m_position;
}

std::optional<sf::Vector2i> GobbletStack::getGridPosition() const
{
    return m_gridPosition;
}

bool GobbletStack::isExternalStack() const
{
    // If grid position has not value, it is an external stack
    return !m_gridPosition.has_value();
}

void GobbletStack::setGridPosition(const std::optional<sf::Vector2i>& t_gridPosition, Grid& t_grid)
{
    m_gridPosition = t_gridPosition;

    if (m_stack.empty()) return;

    if (m_gridPosition)
    {
        Tile tile;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Set the gobblet's world position to the grid coordinates
                if (m_gridPosition->x == i && m_gridPosition->y == j)
                {
                    tile = t_grid.getGridArray()[i][j];
                    setPosition(tile.getCenter());
                }
            }
        }

        m_stack.top().setGridCoordinates(m_gridPosition);
    }
}

void GobbletStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_stack.empty()) return;

    target.draw(m_stack.top(), states);
}

bool operator==(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return t_lhs.m_position == t_rhs.m_position
        && t_lhs.m_gridPosition == t_rhs.m_gridPosition;
}

bool operator!=(const GobbletStack& t_lhs, const GobbletStack& t_rhs)
{
    return !(t_lhs == t_rhs);
}
